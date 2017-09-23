using AOT;

using System;
using System.IO;
using System.Runtime.InteropServices;

using UnityEngine;
using UnityEngine.Assertions;

namespace NativeScript
{
	/// <summary>
	/// Internals of the bindings between native and .NET code.
	/// Game code shouldn't go here.
	/// </summary>
	/// <author>
	/// Jackson Dunstan, 2017, http://JacksonDunstan.com
	/// </author>
	/// <license>
	/// MIT
	/// </license>
	public static class Bindings
	{
		// Holds objects and provides handles to them in the form of ints
		public static class ObjectStore
		{
			// Stored objects. The first is never used so 0 can be "null".
			static object[] objects;
			
			// Stack of available handles
			static int[] handles;
			
			// Hash table of stored objects to their handles.
			static object[] keys;
			static int[] values;
			
			// Index of the next available handle
			static int nextHandleIndex;
			
			// The maximum number of objects to store. Must be positive.
			static int maxObjects;
			
			public static void Init(int maxObjects)
			{
				ObjectStore.maxObjects = maxObjects;
				
				// Initialize the objects as all null plus room for the
				// first to always be null.
				objects = new object[maxObjects + 1];

				// Initialize the handles stack as 1, 2, 3, ...
				handles = new int[maxObjects];
				for (
					int i = 0, handle = maxObjects;
					i < maxObjects;
					++i, --handle)
				{
					handles[i] = handle;
				}
				nextHandleIndex = maxObjects - 1;
				
				// Initialize the hash table
				keys = new object[maxObjects];
				values = new int[maxObjects];
			}
			
			public static int Store(object obj)
			{
				// Null is always zero
				if (object.ReferenceEquals(obj, null))
				{
					return 0;
				}
				
				lock (objects)
				{
					// Pop a handle off the stack
					int handle = handles[nextHandleIndex];
					nextHandleIndex--;
					
					// Store the object
					objects[handle] = obj;
					
					// Insert into the hash table
					int initialIndex = (int)(
						((uint)obj.GetHashCode()) % maxObjects);
					int index = initialIndex;
					do
					{
						if (object.ReferenceEquals(keys[index], null))
						{
							keys[index] = obj;
							values[index] = handle;
							break;
						}
						index = (index + 1) % maxObjects;
					}
					while (index != initialIndex);
					
					return handle;
				}
			}
			
			public static object Get(int handle)
			{
				return objects[handle];
			}
			
			public static int GetHandle(object obj)
			{
				// Null is always zero
				if (object.ReferenceEquals(obj, null))
				{
					return 0;
				}
				
				lock (objects)
				{
					// Look up the object in the hash table
					int initialIndex = (int)(
						((uint)obj.GetHashCode()) % maxObjects);
					int index = initialIndex;
					do
					{
						if (object.ReferenceEquals(keys[index], obj))
						{
							return values[index];
						}
						index = (index + 1) % maxObjects;
					}
					while (index != initialIndex);
				}
				
				// Object not found
				return Store(obj);
			}
			
			public static void Remove(int handle)
			{
				// Null is never stored, so there's nothing to remove
				if (handle == 0)
				{
					return;
				}
				
				lock (objects)
				{
					// Forget the object
					object obj = objects[handle];
					objects[handle] = null;
					
					// Push the handle onto the stack
					nextHandleIndex++;
					handles[nextHandleIndex] = handle;
					
					// Remove the object from the hash table
					int initialIndex = (int)(
						((uint)obj.GetHashCode()) % maxObjects);
					int index = initialIndex;
					do
					{
						if (object.ReferenceEquals(keys[index], obj))
						{
							// Only the key needs to be removed (set to null)
							// because values corresponding to null will never
							// be read and the values are just integers, so
							// we're not holding on to a managed reference that
							// will prevent GC.
							keys[index] = null;
							break;
						}
						index = (index + 1) % maxObjects;
					}
					while (index != initialIndex);
				}
			}
		}
		
		// Holds structs and provides handles to them in the form of ints
		public static class StructStore<T>
			where T : struct
		{
			// Stored structs. The first is never used so 0 can be "null".
			static T[] structs;
			
			// Stack of available handles
			static int[] handles;
			
			// Index of the next available handle
			static int nextHandleIndex;
			
			public static void Init(int maxStructs)
			{
				// Initialize the objects as all default plus room for the
				// first to always be unused.
				structs = new T[maxStructs + 1];

				// Initialize the handles stack as 1, 2, 3, ...
				handles = new int[maxStructs];
				for (
					int i = 0, handle = maxStructs;
					i < maxStructs;
					++i, --handle)
				{
					handles[i] = handle;
				}
				nextHandleIndex = maxStructs - 1;
			}
			
			public static int Store(T structToStore)
			{
				lock (structs)
				{
					// Pop a handle off the stack
					int handle = handles[nextHandleIndex];
					nextHandleIndex--;
					
					// Store the struct
					structs[handle] = structToStore;
					
					return handle;
				}
			}
			
			public static void Replace(int handle, ref T structToStore)
			{
				structs[handle] = structToStore;
			}
			
			public static T Get(int handle)
			{
				return structs[handle];
			}
			
			public static void Remove(int handle)
			{
				if (handle != 0)
				{
					lock (structs)
					{
						// Forget the struct
						structs[handle] = default(T);

						// Push the handle onto the stack
						nextHandleIndex++;
						handles[nextHandleIndex] = handle;
					}
				}
			}
		}
		
		// Name of the plugin when using [DllImport]
		const string PluginName = "NativeScript";
		
		// Path to load the plugin from when running inside the editor
#if UNITY_EDITOR_OSX
		const string PluginPath = "/Plugins/Editor/NativeScript.bundle/Contents/MacOS/NativeScript";
#elif UNITY_EDITOR_LINUX
		const string PluginPath = "/Plugins/Editor/libNativeScript.so";
#elif UNITY_EDITOR_WIN
		const string PluginPath = "/Plugins/Editor/NativeScript.dll";
#endif

#if UNITY_EDITOR
		// Handle to the C++ DLL
		static IntPtr libraryHandle;

		delegate void InitDelegate(
			int maxManagedObjects,
			IntPtr releaseObject,
			IntPtr stringNew,
			IntPtr setException,
			/*BEGIN INIT PARAMS*/
			IntPtr systemDiagnosticsStopwatchConstructor,
			IntPtr systemDiagnosticsStopwatchPropertyGetElapsedMilliseconds,
			IntPtr systemDiagnosticsStopwatchMethodStart,
			IntPtr systemDiagnosticsStopwatchMethodReset,
			IntPtr unityEngineObjectPropertyGetName,
			IntPtr unityEngineObjectPropertySetName,
			IntPtr unityEngineGameObjectConstructor,
			IntPtr unityEngineGameObjectConstructorSystemString,
			IntPtr unityEngineGameObjectPropertyGetTransform,
			IntPtr unityEngineGameObjectMethodFindSystemString,
			IntPtr unityEngineGameObjectMethodAddComponentMyGameMonoBehavioursTestScript,
			IntPtr unityEngineComponentPropertyGetTransform,
			IntPtr unityEngineTransformPropertyGetPosition,
			IntPtr unityEngineTransformPropertySetPosition,
			IntPtr unityEngineDebugMethodLogSystemObject,
			IntPtr unityEngineAssertionsAssertFieldGetRaiseExceptions,
			IntPtr unityEngineAssertionsAssertFieldSetRaiseExceptions,
			IntPtr unityEngineAssertionsAssertMethodAreEqualSystemStringSystemString_SystemString,
			IntPtr unityEngineAssertionsAssertMethodAreEqualUnityEngineGameObjectUnityEngineGameObject_UnityEngineGameObject,
			IntPtr unityEngineAudioSettingsMethodGetDSPBufferSizeSystemInt32_SystemInt32,
			IntPtr unityEngineNetworkingNetworkTransportMethodGetBroadcastConnectionInfoSystemInt32_SystemString_SystemInt32_SystemByte,
			IntPtr unityEngineNetworkingNetworkTransportMethodInit,
			IntPtr unityEngineVector3ConstructorSystemSingle_SystemSingle_SystemSingle,
			IntPtr unityEngineVector3PropertyGetMagnitude,
			IntPtr unityEngineVector3MethodSetSystemSingle_SystemSingle_SystemSingle,
			IntPtr releaseUnityEngineRaycastHit,
			int ReleaseUnityEngineRaycastHit,
			IntPtr unityEngineRaycastHitPropertyGetPoint,
			IntPtr unityEngineRaycastHitPropertySetPoint,
			IntPtr unityEngineRaycastHitPropertyGetTransform,
			IntPtr releaseSystemCollectionsGenericKeyValuePairSystemString_SystemDouble,
			int ReleaseSystemCollectionsGenericKeyValuePairSystemString_SystemDouble,
			IntPtr systemCollectionsGenericKeyValuePairSystemString_SystemDoubleConstructorSystemString_SystemDouble,
			IntPtr systemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetKey,
			IntPtr systemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetValue,
			IntPtr systemCollectionsGenericListSystemStringConstructor,
			IntPtr systemCollectionsGenericListSystemStringMethodAddSystemString,
			IntPtr systemCollectionsGenericLinkedListNodeSystemStringConstructorSystemString,
			IntPtr systemCollectionsGenericLinkedListNodeSystemStringPropertyGetValue,
			IntPtr systemCollectionsGenericLinkedListNodeSystemStringPropertySetValue,
			IntPtr systemRuntimeCompilerServicesStrongBoxSystemStringConstructorSystemString,
			IntPtr systemRuntimeCompilerServicesStrongBoxSystemStringFieldGetValue,
			IntPtr systemRuntimeCompilerServicesStrongBoxSystemStringFieldSetValue,
			IntPtr systemExceptionConstructorSystemString
			/*END INIT PARAMS*/);
		
		public delegate void SetCsharpExceptionDelegate(int handle);
		
		/*BEGIN MONOBEHAVIOUR DELEGATES*/
		public delegate void TestScriptAwakeDelegate(int thisHandle);
		public static TestScriptAwakeDelegate TestScriptAwake;
		
		public delegate void TestScriptOnAnimatorIKDelegate(int thisHandle, int param0);
		public static TestScriptOnAnimatorIKDelegate TestScriptOnAnimatorIK;
		
		public delegate void TestScriptOnCollisionEnterDelegate(int thisHandle, int param0);
		public static TestScriptOnCollisionEnterDelegate TestScriptOnCollisionEnter;
		
		public delegate void TestScriptUpdateDelegate(int thisHandle);
		public static TestScriptUpdateDelegate TestScriptUpdate;
		/*END MONOBEHAVIOUR DELEGATES*/
#endif

#if UNITY_EDITOR_OSX || UNITY_EDITOR_LINUX
		[DllImport("__Internal")]
		static extern IntPtr dlopen(
			string path,
			int flag);

		[DllImport("__Internal")]
		static extern IntPtr dlsym(
			IntPtr handle,
			string symbolName);

		[DllImport("__Internal")]
		static extern int dlclose(
			IntPtr handle);

		static IntPtr OpenLibrary(
			string path)
		{
			IntPtr handle = dlopen(path, 0);
			if (handle == IntPtr.Zero)
			{
				throw new Exception("Couldn't open native library: " + path);
			}
			return handle;
		}
		
		static void CloseLibrary(
			IntPtr libraryHandle)
		{
			dlclose(libraryHandle);
		}
		
		static T GetDelegate<T>(
			IntPtr libraryHandle,
			string functionName) where T : class
		{
			IntPtr symbol = dlsym(libraryHandle, functionName);
			if (symbol == IntPtr.Zero)
			{
				throw new Exception("Couldn't get function: " + functionName);
			}
			return Marshal.GetDelegateForFunctionPointer(
				symbol,
				typeof(T)) as T;
		}
#elif UNITY_EDITOR_WIN
		[DllImport("kernel32")]
		static extern IntPtr LoadLibrary(
			string path);
		
		[DllImport("kernel32")]
		static extern IntPtr GetProcAddress(
			IntPtr libraryHandle,
			string symbolName);
		
		[DllImport("kernel32")]
		static extern bool FreeLibrary(
			IntPtr libraryHandle);
		
		static IntPtr OpenLibrary(string path)
		{
			IntPtr handle = LoadLibrary(path);
			if (handle == IntPtr.Zero)
			{
				throw new Exception("Couldn't open native library: " + path);
			}
			return handle;
		}
		
		static void CloseLibrary(IntPtr libraryHandle)
		{
			FreeLibrary(libraryHandle);
		}
		
		static T GetDelegate<T>(
			IntPtr libraryHandle,
			string functionName) where T : class
		{
			IntPtr symbol = GetProcAddress(libraryHandle, functionName);
			if (symbol == IntPtr.Zero)
			{
				throw new Exception("Couldn't get function: " + functionName);
			}
			return Marshal.GetDelegateForFunctionPointer(
				symbol,
				typeof(T)) as T;
		}
#else
		[DllImport(PluginName)]
		static extern void Init(
			int maxManagedObjects,
			IntPtr releaseObject,
			IntPtr stringNew,
			IntPtr setException,
			/*BEGIN INIT PARAMS*/
			IntPtr systemDiagnosticsStopwatchConstructor,
			IntPtr systemDiagnosticsStopwatchPropertyGetElapsedMilliseconds,
			IntPtr systemDiagnosticsStopwatchMethodStart,
			IntPtr systemDiagnosticsStopwatchMethodReset,
			IntPtr unityEngineObjectPropertyGetName,
			IntPtr unityEngineObjectPropertySetName,
			IntPtr unityEngineGameObjectConstructor,
			IntPtr unityEngineGameObjectConstructorSystemString,
			IntPtr unityEngineGameObjectPropertyGetTransform,
			IntPtr unityEngineGameObjectMethodFindSystemString,
			IntPtr unityEngineGameObjectMethodAddComponentMyGameMonoBehavioursTestScript,
			IntPtr unityEngineComponentPropertyGetTransform,
			IntPtr unityEngineTransformPropertyGetPosition,
			IntPtr unityEngineTransformPropertySetPosition,
			IntPtr unityEngineDebugMethodLogSystemObject,
			IntPtr unityEngineAssertionsAssertFieldGetRaiseExceptions,
			IntPtr unityEngineAssertionsAssertFieldSetRaiseExceptions,
			IntPtr unityEngineAssertionsAssertMethodAreEqualSystemStringSystemString_SystemString,
			IntPtr unityEngineAssertionsAssertMethodAreEqualUnityEngineGameObjectUnityEngineGameObject_UnityEngineGameObject,
			IntPtr unityEngineAudioSettingsMethodGetDSPBufferSizeSystemInt32_SystemInt32,
			IntPtr unityEngineNetworkingNetworkTransportMethodGetBroadcastConnectionInfoSystemInt32_SystemString_SystemInt32_SystemByte,
			IntPtr unityEngineNetworkingNetworkTransportMethodInit,
			IntPtr unityEngineVector3ConstructorSystemSingle_SystemSingle_SystemSingle,
			IntPtr unityEngineVector3PropertyGetMagnitude,
			IntPtr unityEngineVector3MethodSetSystemSingle_SystemSingle_SystemSingle,
			IntPtr releaseUnityEngineRaycastHit,
			int ReleaseUnityEngineRaycastHit,
			IntPtr unityEngineRaycastHitPropertyGetPoint,
			IntPtr unityEngineRaycastHitPropertySetPoint,
			IntPtr unityEngineRaycastHitPropertyGetTransform,
			IntPtr releaseSystemCollectionsGenericKeyValuePairSystemString_SystemDouble,
			int ReleaseSystemCollectionsGenericKeyValuePairSystemString_SystemDouble,
			IntPtr systemCollectionsGenericKeyValuePairSystemString_SystemDoubleConstructorSystemString_SystemDouble,
			IntPtr systemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetKey,
			IntPtr systemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetValue,
			IntPtr systemCollectionsGenericListSystemStringConstructor,
			IntPtr systemCollectionsGenericListSystemStringMethodAddSystemString,
			IntPtr systemCollectionsGenericLinkedListNodeSystemStringConstructorSystemString,
			IntPtr systemCollectionsGenericLinkedListNodeSystemStringPropertyGetValue,
			IntPtr systemCollectionsGenericLinkedListNodeSystemStringPropertySetValue,
			IntPtr systemRuntimeCompilerServicesStrongBoxSystemStringConstructorSystemString,
			IntPtr systemRuntimeCompilerServicesStrongBoxSystemStringFieldGetValue,
			IntPtr systemRuntimeCompilerServicesStrongBoxSystemStringFieldSetValue,
			IntPtr systemExceptionConstructorSystemString
			/*END INIT PARAMS*/);
		
		[DllImport(PluginName)]
		static extern void SetCsharpException(int handle);
		
		/*BEGIN MONOBEHAVIOUR IMPORTS*/
		[DllImport(Constants.PluginName)]
		public static extern void TestScriptAwake(int thisHandle);
		
		[DllImport(Constants.PluginName)]
		public static extern void TestScriptOnAnimatorIK(int thisHandle, int param0);
		
		[DllImport(Constants.PluginName)]
		public static extern void TestScriptOnCollisionEnter(int thisHandle, int param0);
		
		[DllImport(Constants.PluginName)]
		public static extern void TestScriptUpdate(int thisHandle);
		/*END MONOBEHAVIOUR IMPORTS*/
#endif
		
		delegate void ReleaseObjectDelegate(int handle);
		delegate int StringNewDelegate(string chars);
		delegate void SetExceptionDelegate(int handle);
		
		/*BEGIN DELEGATE TYPES*/
		delegate int SystemDiagnosticsStopwatchConstructorDelegate();
		delegate long SystemDiagnosticsStopwatchPropertyGetElapsedMillisecondsDelegate(int thisHandle);
		delegate void SystemDiagnosticsStopwatchMethodStartDelegate(int thisHandle);
		delegate void SystemDiagnosticsStopwatchMethodResetDelegate(int thisHandle);
		delegate int UnityEngineObjectPropertyGetNameDelegate(int thisHandle);
		delegate void UnityEngineObjectPropertySetNameDelegate(int thisHandle, int valueHandle);
		delegate int UnityEngineGameObjectConstructorDelegate();
		delegate int UnityEngineGameObjectConstructorSystemStringDelegate(int nameHandle);
		delegate int UnityEngineGameObjectPropertyGetTransformDelegate(int thisHandle);
		delegate int UnityEngineGameObjectMethodFindSystemStringDelegate(int nameHandle);
		delegate int UnityEngineGameObjectMethodAddComponentMyGameMonoBehavioursTestScriptDelegate(int thisHandle);
		delegate int UnityEngineComponentPropertyGetTransformDelegate(int thisHandle);
		delegate UnityEngine.Vector3 UnityEngineTransformPropertyGetPositionDelegate(int thisHandle);
		delegate void UnityEngineTransformPropertySetPositionDelegate(int thisHandle, ref UnityEngine.Vector3 value);
		delegate void UnityEngineDebugMethodLogSystemObjectDelegate(int messageHandle);
		delegate bool UnityEngineAssertionsAssertFieldGetRaiseExceptionsDelegate();
		delegate void UnityEngineAssertionsAssertFieldSetRaiseExceptionsDelegate(bool value);
		delegate void UnityEngineAssertionsAssertMethodAreEqualSystemStringSystemString_SystemStringDelegate(int expectedHandle, int actualHandle);
		delegate void UnityEngineAssertionsAssertMethodAreEqualUnityEngineGameObjectUnityEngineGameObject_UnityEngineGameObjectDelegate(int expectedHandle, int actualHandle);
		delegate void UnityEngineAudioSettingsMethodGetDSPBufferSizeSystemInt32_SystemInt32Delegate(ref int bufferLength, ref int numBuffers);
		delegate void UnityEngineNetworkingNetworkTransportMethodGetBroadcastConnectionInfoSystemInt32_SystemString_SystemInt32_SystemByteDelegate(int hostId, ref int addressHandle, ref int port, ref byte error);
		delegate void UnityEngineNetworkingNetworkTransportMethodInitDelegate();
		delegate UnityEngine.Vector3 UnityEngineVector3ConstructorSystemSingle_SystemSingle_SystemSingleDelegate(float x, float y, float z);
		delegate float UnityEngineVector3PropertyGetMagnitudeDelegate(ref UnityEngine.Vector3 thiz);
		delegate void UnityEngineVector3MethodSetSystemSingle_SystemSingle_SystemSingleDelegate(ref UnityEngine.Vector3 thiz, float newX, float newY, float newZ);
		delegate void ReleaseUnityEngineRaycastHitDelegate(int handle);
		delegate UnityEngine.Vector3 UnityEngineRaycastHitPropertyGetPointDelegate(int thisHandle);
		delegate void UnityEngineRaycastHitPropertySetPointDelegate(int thisHandle, ref UnityEngine.Vector3 value);
		delegate int UnityEngineRaycastHitPropertyGetTransformDelegate(int thisHandle);
		delegate void ReleaseSystemCollectionsGenericKeyValuePairSystemString_SystemDoubleDelegate(int handle);
		delegate int SystemCollectionsGenericKeyValuePairSystemString_SystemDoubleConstructorSystemString_SystemDoubleDelegate(int keyHandle, double value);
		delegate int SystemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetKeyDelegate(int thisHandle);
		delegate double SystemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetValueDelegate(int thisHandle);
		delegate int SystemCollectionsGenericListSystemStringConstructorDelegate();
		delegate void SystemCollectionsGenericListSystemStringMethodAddSystemStringDelegate(int thisHandle, int itemHandle);
		delegate int SystemCollectionsGenericLinkedListNodeSystemStringConstructorSystemStringDelegate(int valueHandle);
		delegate int SystemCollectionsGenericLinkedListNodeSystemStringPropertyGetValueDelegate(int thisHandle);
		delegate void SystemCollectionsGenericLinkedListNodeSystemStringPropertySetValueDelegate(int thisHandle, int valueHandle);
		delegate int SystemRuntimeCompilerServicesStrongBoxSystemStringConstructorSystemStringDelegate(int valueHandle);
		delegate int SystemRuntimeCompilerServicesStrongBoxSystemStringFieldGetValueDelegate(int thisHandle);
		delegate void SystemRuntimeCompilerServicesStrongBoxSystemStringFieldSetValueDelegate(int thisHandle, int valueHandle);
		delegate int SystemExceptionConstructorSystemStringDelegate(int messageHandle);
		/*END DELEGATE TYPES*/
		
		public static Exception UnhandledCppException;
		public static SetCsharpExceptionDelegate SetCsharpException;
		
		/// <summary>
		/// Open the C++ plugin and call its PluginMain()
		/// </summary>
		/// 
		/// <param name="maxManagedObjects">
		/// Maximum number of simultaneous managed objects that the C++ plugin
		/// uses.
		/// </param>
		public static void Open(
			int maxManagedObjects)
		{
			ObjectStore.Init(maxManagedObjects);
			/*BEGIN STRUCTSTORE INIT CALLS*/
			NativeScript.Bindings.StructStore<UnityEngine.RaycastHit>.Init(1000);
			NativeScript.Bindings.StructStore<System.Collections.Generic.KeyValuePair<string, double>>.Init(maxManagedObjects);
			/*END STRUCTSTORE INIT CALLS*/
			
#if UNITY_EDITOR

			// Open native library
			libraryHandle = OpenLibrary(
				Application.dataPath + PluginPath);
			InitDelegate Init = GetDelegate<InitDelegate>(
				libraryHandle,
				"Init");
			SetCsharpException = GetDelegate<SetCsharpExceptionDelegate>(
				libraryHandle,
				"SetCsharpException");
			/*BEGIN MONOBEHAVIOUR GETDELEGATE CALLS*/
			TestScriptAwake = GetDelegate<TestScriptAwakeDelegate>(libraryHandle, "TestScriptAwake");
			TestScriptOnAnimatorIK = GetDelegate<TestScriptOnAnimatorIKDelegate>(libraryHandle, "TestScriptOnAnimatorIK");
			TestScriptOnCollisionEnter = GetDelegate<TestScriptOnCollisionEnterDelegate>(libraryHandle, "TestScriptOnCollisionEnter");
			TestScriptUpdate = GetDelegate<TestScriptUpdateDelegate>(libraryHandle, "TestScriptUpdate");
			/*END MONOBEHAVIOUR GETDELEGATE CALLS*/

#endif
			
			// Init C++ library
			Init(
				maxManagedObjects,
				Marshal.GetFunctionPointerForDelegate(new ReleaseObjectDelegate(ReleaseObject)),
				Marshal.GetFunctionPointerForDelegate(new StringNewDelegate(StringNew)),
				Marshal.GetFunctionPointerForDelegate(new SetExceptionDelegate(SetException)),
				/*BEGIN INIT CALL*/
				Marshal.GetFunctionPointerForDelegate(new SystemDiagnosticsStopwatchConstructorDelegate(SystemDiagnosticsStopwatchConstructor)),
				Marshal.GetFunctionPointerForDelegate(new SystemDiagnosticsStopwatchPropertyGetElapsedMillisecondsDelegate(SystemDiagnosticsStopwatchPropertyGetElapsedMilliseconds)),
				Marshal.GetFunctionPointerForDelegate(new SystemDiagnosticsStopwatchMethodStartDelegate(SystemDiagnosticsStopwatchMethodStart)),
				Marshal.GetFunctionPointerForDelegate(new SystemDiagnosticsStopwatchMethodResetDelegate(SystemDiagnosticsStopwatchMethodReset)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineObjectPropertyGetNameDelegate(UnityEngineObjectPropertyGetName)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineObjectPropertySetNameDelegate(UnityEngineObjectPropertySetName)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineGameObjectConstructorDelegate(UnityEngineGameObjectConstructor)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineGameObjectConstructorSystemStringDelegate(UnityEngineGameObjectConstructorSystemString)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineGameObjectPropertyGetTransformDelegate(UnityEngineGameObjectPropertyGetTransform)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineGameObjectMethodFindSystemStringDelegate(UnityEngineGameObjectMethodFindSystemString)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineGameObjectMethodAddComponentMyGameMonoBehavioursTestScriptDelegate(UnityEngineGameObjectMethodAddComponentMyGameMonoBehavioursTestScript)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineComponentPropertyGetTransformDelegate(UnityEngineComponentPropertyGetTransform)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineTransformPropertyGetPositionDelegate(UnityEngineTransformPropertyGetPosition)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineTransformPropertySetPositionDelegate(UnityEngineTransformPropertySetPosition)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineDebugMethodLogSystemObjectDelegate(UnityEngineDebugMethodLogSystemObject)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineAssertionsAssertFieldGetRaiseExceptionsDelegate(UnityEngineAssertionsAssertFieldGetRaiseExceptions)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineAssertionsAssertFieldSetRaiseExceptionsDelegate(UnityEngineAssertionsAssertFieldSetRaiseExceptions)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineAssertionsAssertMethodAreEqualSystemStringSystemString_SystemStringDelegate(UnityEngineAssertionsAssertMethodAreEqualSystemStringSystemString_SystemString)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineAssertionsAssertMethodAreEqualUnityEngineGameObjectUnityEngineGameObject_UnityEngineGameObjectDelegate(UnityEngineAssertionsAssertMethodAreEqualUnityEngineGameObjectUnityEngineGameObject_UnityEngineGameObject)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineAudioSettingsMethodGetDSPBufferSizeSystemInt32_SystemInt32Delegate(UnityEngineAudioSettingsMethodGetDSPBufferSizeSystemInt32_SystemInt32)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineNetworkingNetworkTransportMethodGetBroadcastConnectionInfoSystemInt32_SystemString_SystemInt32_SystemByteDelegate(UnityEngineNetworkingNetworkTransportMethodGetBroadcastConnectionInfoSystemInt32_SystemString_SystemInt32_SystemByte)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineNetworkingNetworkTransportMethodInitDelegate(UnityEngineNetworkingNetworkTransportMethodInit)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineVector3ConstructorSystemSingle_SystemSingle_SystemSingleDelegate(UnityEngineVector3ConstructorSystemSingle_SystemSingle_SystemSingle)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineVector3PropertyGetMagnitudeDelegate(UnityEngineVector3PropertyGetMagnitude)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineVector3MethodSetSystemSingle_SystemSingle_SystemSingleDelegate(UnityEngineVector3MethodSetSystemSingle_SystemSingle_SystemSingle)),
				Marshal.GetFunctionPointerForDelegate(new ReleaseUnityEngineRaycastHitDelegate(ReleaseUnityEngineRaycastHit)),
				1000,
				Marshal.GetFunctionPointerForDelegate(new UnityEngineRaycastHitPropertyGetPointDelegate(UnityEngineRaycastHitPropertyGetPoint)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineRaycastHitPropertySetPointDelegate(UnityEngineRaycastHitPropertySetPoint)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineRaycastHitPropertyGetTransformDelegate(UnityEngineRaycastHitPropertyGetTransform)),
				Marshal.GetFunctionPointerForDelegate(new ReleaseSystemCollectionsGenericKeyValuePairSystemString_SystemDoubleDelegate(ReleaseSystemCollectionsGenericKeyValuePairSystemString_SystemDouble)),
				maxManagedObjects,
				Marshal.GetFunctionPointerForDelegate(new SystemCollectionsGenericKeyValuePairSystemString_SystemDoubleConstructorSystemString_SystemDoubleDelegate(SystemCollectionsGenericKeyValuePairSystemString_SystemDoubleConstructorSystemString_SystemDouble)),
				Marshal.GetFunctionPointerForDelegate(new SystemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetKeyDelegate(SystemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetKey)),
				Marshal.GetFunctionPointerForDelegate(new SystemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetValueDelegate(SystemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetValue)),
				Marshal.GetFunctionPointerForDelegate(new SystemCollectionsGenericListSystemStringConstructorDelegate(SystemCollectionsGenericListSystemStringConstructor)),
				Marshal.GetFunctionPointerForDelegate(new SystemCollectionsGenericListSystemStringMethodAddSystemStringDelegate(SystemCollectionsGenericListSystemStringMethodAddSystemString)),
				Marshal.GetFunctionPointerForDelegate(new SystemCollectionsGenericLinkedListNodeSystemStringConstructorSystemStringDelegate(SystemCollectionsGenericLinkedListNodeSystemStringConstructorSystemString)),
				Marshal.GetFunctionPointerForDelegate(new SystemCollectionsGenericLinkedListNodeSystemStringPropertyGetValueDelegate(SystemCollectionsGenericLinkedListNodeSystemStringPropertyGetValue)),
				Marshal.GetFunctionPointerForDelegate(new SystemCollectionsGenericLinkedListNodeSystemStringPropertySetValueDelegate(SystemCollectionsGenericLinkedListNodeSystemStringPropertySetValue)),
				Marshal.GetFunctionPointerForDelegate(new SystemRuntimeCompilerServicesStrongBoxSystemStringConstructorSystemStringDelegate(SystemRuntimeCompilerServicesStrongBoxSystemStringConstructorSystemString)),
				Marshal.GetFunctionPointerForDelegate(new SystemRuntimeCompilerServicesStrongBoxSystemStringFieldGetValueDelegate(SystemRuntimeCompilerServicesStrongBoxSystemStringFieldGetValue)),
				Marshal.GetFunctionPointerForDelegate(new SystemRuntimeCompilerServicesStrongBoxSystemStringFieldSetValueDelegate(SystemRuntimeCompilerServicesStrongBoxSystemStringFieldSetValue)),
				Marshal.GetFunctionPointerForDelegate(new SystemExceptionConstructorSystemStringDelegate(SystemExceptionConstructorSystemString))
				/*END INIT CALL*/
				);
			if (UnhandledCppException != null)
			{
				Exception ex = UnhandledCppException;
				UnhandledCppException = null;
				throw new Exception("Unhandled C++ exception in Init", ex);
			}
		}
		
		/// <summary>
		/// Close the C++ plugin
		/// </summary>
		public static void Close()
		{
#if UNITY_EDITOR
			CloseLibrary(libraryHandle);
			libraryHandle = IntPtr.Zero;
#endif
		}
		
		////////////////////////////////////////////////////////////////
		// C# functions for C++ to call
		////////////////////////////////////////////////////////////////
		
		[MonoPInvokeCallback(typeof(ReleaseObjectDelegate))]
		static void ReleaseObject(
			int handle)
		{
			if (handle != 0)
			{
				ObjectStore.Remove(handle);
			}
		}
		
		[MonoPInvokeCallback(typeof(StringNewDelegate))]
		static int StringNew(
			string chars)
		{
			int handle = ObjectStore.Store(chars);
			return handle;
		}
		
		[MonoPInvokeCallback(typeof(SetExceptionDelegate))]
		static void SetException(int handle)
		{
			UnhandledCppException = ObjectStore.Get(handle) as Exception;
		}
		
		/*BEGIN FUNCTIONS*/
		[MonoPInvokeCallback(typeof(SystemDiagnosticsStopwatchConstructorDelegate))]
		static int SystemDiagnosticsStopwatchConstructor()
		{
			try
			{
				var returnValue = NativeScript.Bindings.ObjectStore.Store(new System.Diagnostics.Stopwatch());
				return returnValue;
			}
			catch (Exception ex)
			{
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
		}
		
		[MonoPInvokeCallback(typeof(SystemDiagnosticsStopwatchPropertyGetElapsedMillisecondsDelegate))]
		static long SystemDiagnosticsStopwatchPropertyGetElapsedMilliseconds(int thisHandle)
		{
			try
			{
				var thiz = (System.Diagnostics.Stopwatch)NativeScript.Bindings.ObjectStore.Get(thisHandle);
				var returnValue = thiz.ElapsedMilliseconds;
				return returnValue;
			}
			catch (Exception ex)
			{
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(long);
			}
		}
		
		[MonoPInvokeCallback(typeof(SystemDiagnosticsStopwatchMethodStartDelegate))]
		static void SystemDiagnosticsStopwatchMethodStart(int thisHandle)
		{
			try
			{
				var thiz = (System.Diagnostics.Stopwatch)NativeScript.Bindings.ObjectStore.Get(thisHandle);
				thiz.Start();
			}
			catch (Exception ex)
			{
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
		}
		
		[MonoPInvokeCallback(typeof(SystemDiagnosticsStopwatchMethodResetDelegate))]
		static void SystemDiagnosticsStopwatchMethodReset(int thisHandle)
		{
			try
			{
				var thiz = (System.Diagnostics.Stopwatch)NativeScript.Bindings.ObjectStore.Get(thisHandle);
				thiz.Reset();
			}
			catch (Exception ex)
			{
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
		}
		
		[MonoPInvokeCallback(typeof(UnityEngineObjectPropertyGetNameDelegate))]
		static int UnityEngineObjectPropertyGetName(int thisHandle)
		{
			try
			{
				var thiz = (UnityEngine.Object)NativeScript.Bindings.ObjectStore.Get(thisHandle);
				var returnValue = thiz.name;
				return NativeScript.Bindings.ObjectStore.GetHandle(returnValue);
			}
			catch (Exception ex)
			{
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
		}
		
		[MonoPInvokeCallback(typeof(UnityEngineObjectPropertySetNameDelegate))]
		static void UnityEngineObjectPropertySetName(int thisHandle, int valueHandle)
		{
			try
			{
				var thiz = (UnityEngine.Object)NativeScript.Bindings.ObjectStore.Get(thisHandle);
				var value = (string)NativeScript.Bindings.ObjectStore.Get(valueHandle);
				thiz.name = value;
			}
			catch (Exception ex)
			{
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
		}
		
		[MonoPInvokeCallback(typeof(UnityEngineGameObjectConstructorDelegate))]
		static int UnityEngineGameObjectConstructor()
		{
			try
			{
				var returnValue = NativeScript.Bindings.ObjectStore.Store(new UnityEngine.GameObject());
				return returnValue;
			}
			catch (Exception ex)
			{
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
		}
		
		[MonoPInvokeCallback(typeof(UnityEngineGameObjectConstructorSystemStringDelegate))]
		static int UnityEngineGameObjectConstructorSystemString(int nameHandle)
		{
			try
			{
				var name = (string)NativeScript.Bindings.ObjectStore.Get(nameHandle);
				var returnValue = NativeScript.Bindings.ObjectStore.Store(new UnityEngine.GameObject(name));
				return returnValue;
			}
			catch (Exception ex)
			{
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
		}
		
		[MonoPInvokeCallback(typeof(UnityEngineGameObjectPropertyGetTransformDelegate))]
		static int UnityEngineGameObjectPropertyGetTransform(int thisHandle)
		{
			try
			{
				var thiz = (UnityEngine.GameObject)NativeScript.Bindings.ObjectStore.Get(thisHandle);
				var returnValue = thiz.transform;
				return NativeScript.Bindings.ObjectStore.GetHandle(returnValue);
			}
			catch (Exception ex)
			{
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
		}
		
		[MonoPInvokeCallback(typeof(UnityEngineGameObjectMethodFindSystemStringDelegate))]
		static int UnityEngineGameObjectMethodFindSystemString(int nameHandle)
		{
			try
			{
				var name = (string)NativeScript.Bindings.ObjectStore.Get(nameHandle);
				var returnValue = UnityEngine.GameObject.Find(name);
				return NativeScript.Bindings.ObjectStore.GetHandle(returnValue);
			}
			catch (Exception ex)
			{
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
		}
		
		[MonoPInvokeCallback(typeof(UnityEngineGameObjectMethodAddComponentMyGameMonoBehavioursTestScriptDelegate))]
		static int UnityEngineGameObjectMethodAddComponentMyGameMonoBehavioursTestScript(int thisHandle)
		{
			try
			{
				var thiz = (UnityEngine.GameObject)NativeScript.Bindings.ObjectStore.Get(thisHandle);
				var returnValue = thiz.AddComponent<MyGame.MonoBehaviours.TestScript>();
				return NativeScript.Bindings.ObjectStore.GetHandle(returnValue);
			}
			catch (Exception ex)
			{
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
		}
		
		[MonoPInvokeCallback(typeof(UnityEngineComponentPropertyGetTransformDelegate))]
		static int UnityEngineComponentPropertyGetTransform(int thisHandle)
		{
			try
			{
				var thiz = (UnityEngine.Component)NativeScript.Bindings.ObjectStore.Get(thisHandle);
				var returnValue = thiz.transform;
				return NativeScript.Bindings.ObjectStore.GetHandle(returnValue);
			}
			catch (Exception ex)
			{
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
		}
		
		[MonoPInvokeCallback(typeof(UnityEngineTransformPropertyGetPositionDelegate))]
		static UnityEngine.Vector3 UnityEngineTransformPropertyGetPosition(int thisHandle)
		{
			try
			{
				var thiz = (UnityEngine.Transform)NativeScript.Bindings.ObjectStore.Get(thisHandle);
				var returnValue = thiz.position;
				return returnValue;
			}
			catch (Exception ex)
			{
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(UnityEngine.Vector3);
			}
		}
		
		[MonoPInvokeCallback(typeof(UnityEngineTransformPropertySetPositionDelegate))]
		static void UnityEngineTransformPropertySetPosition(int thisHandle, ref UnityEngine.Vector3 value)
		{
			try
			{
				var thiz = (UnityEngine.Transform)NativeScript.Bindings.ObjectStore.Get(thisHandle);
				thiz.position = value;
			}
			catch (Exception ex)
			{
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
		}
		
		[MonoPInvokeCallback(typeof(UnityEngineDebugMethodLogSystemObjectDelegate))]
		static void UnityEngineDebugMethodLogSystemObject(int messageHandle)
		{
			try
			{
				var message = NativeScript.Bindings.ObjectStore.Get(messageHandle);
				UnityEngine.Debug.Log(message);
			}
			catch (Exception ex)
			{
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
		}
		
		[MonoPInvokeCallback(typeof(UnityEngineAssertionsAssertFieldGetRaiseExceptionsDelegate))]
		static bool UnityEngineAssertionsAssertFieldGetRaiseExceptions()
		{
			try
			{
				var returnValue = UnityEngine.Assertions.Assert.raiseExceptions;
				return returnValue;
			}
			catch (Exception ex)
			{
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(bool);
			}
		}
		
		[MonoPInvokeCallback(typeof(UnityEngineAssertionsAssertFieldSetRaiseExceptionsDelegate))]
		static void UnityEngineAssertionsAssertFieldSetRaiseExceptions(bool value)
		{
			try
			{
				UnityEngine.Assertions.Assert.raiseExceptions = value;
			}
			catch (Exception ex)
			{
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
		}
		
		[MonoPInvokeCallback(typeof(UnityEngineAssertionsAssertMethodAreEqualSystemStringSystemString_SystemStringDelegate))]
		static void UnityEngineAssertionsAssertMethodAreEqualSystemStringSystemString_SystemString(int expectedHandle, int actualHandle)
		{
			try
			{
				var expected = (string)NativeScript.Bindings.ObjectStore.Get(expectedHandle);
				var actual = (string)NativeScript.Bindings.ObjectStore.Get(actualHandle);
				UnityEngine.Assertions.Assert.AreEqual<string>(expected, actual);
			}
			catch (Exception ex)
			{
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
		}
		
		[MonoPInvokeCallback(typeof(UnityEngineAssertionsAssertMethodAreEqualUnityEngineGameObjectUnityEngineGameObject_UnityEngineGameObjectDelegate))]
		static void UnityEngineAssertionsAssertMethodAreEqualUnityEngineGameObjectUnityEngineGameObject_UnityEngineGameObject(int expectedHandle, int actualHandle)
		{
			try
			{
				var expected = (UnityEngine.GameObject)NativeScript.Bindings.ObjectStore.Get(expectedHandle);
				var actual = (UnityEngine.GameObject)NativeScript.Bindings.ObjectStore.Get(actualHandle);
				UnityEngine.Assertions.Assert.AreEqual<UnityEngine.GameObject>(expected, actual);
			}
			catch (Exception ex)
			{
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
		}
		
		[MonoPInvokeCallback(typeof(UnityEngineAudioSettingsMethodGetDSPBufferSizeSystemInt32_SystemInt32Delegate))]
		static void UnityEngineAudioSettingsMethodGetDSPBufferSizeSystemInt32_SystemInt32(ref int bufferLength, ref int numBuffers)
		{
			try
			{
				UnityEngine.AudioSettings.GetDSPBufferSize(out bufferLength, out numBuffers);
			}
			catch (Exception ex)
			{
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
		}
		
		[MonoPInvokeCallback(typeof(UnityEngineNetworkingNetworkTransportMethodGetBroadcastConnectionInfoSystemInt32_SystemString_SystemInt32_SystemByteDelegate))]
		static void UnityEngineNetworkingNetworkTransportMethodGetBroadcastConnectionInfoSystemInt32_SystemString_SystemInt32_SystemByte(int hostId, ref int addressHandle, ref int port, ref byte error)
		{
			try
			{
				var address = (string)NativeScript.Bindings.ObjectStore.Get(addressHandle);
				UnityEngine.Networking.NetworkTransport.GetBroadcastConnectionInfo(hostId, out address, out port, out error);
				int addressHandleNew = NativeScript.Bindings.ObjectStore.GetHandle(address);
				addressHandle = addressHandleNew;
			}
			catch (Exception ex)
			{
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
		}
		
		[MonoPInvokeCallback(typeof(UnityEngineNetworkingNetworkTransportMethodInitDelegate))]
		static void UnityEngineNetworkingNetworkTransportMethodInit()
		{
			try
			{
				UnityEngine.Networking.NetworkTransport.Init();
			}
			catch (Exception ex)
			{
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
		}
		
		[MonoPInvokeCallback(typeof(UnityEngineVector3ConstructorSystemSingle_SystemSingle_SystemSingleDelegate))]
		static UnityEngine.Vector3 UnityEngineVector3ConstructorSystemSingle_SystemSingle_SystemSingle(float x, float y, float z)
		{
			try
			{
				var returnValue = new UnityEngine.Vector3(x, y, z);
				return returnValue;
			}
			catch (Exception ex)
			{
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(UnityEngine.Vector3);
			}
		}
		
		[MonoPInvokeCallback(typeof(UnityEngineVector3PropertyGetMagnitudeDelegate))]
		static float UnityEngineVector3PropertyGetMagnitude(ref UnityEngine.Vector3 thiz)
		{
			try
			{
				var returnValue = thiz.magnitude;
				return returnValue;
			}
			catch (Exception ex)
			{
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(float);
			}
		}
		
		[MonoPInvokeCallback(typeof(UnityEngineVector3MethodSetSystemSingle_SystemSingle_SystemSingleDelegate))]
		static void UnityEngineVector3MethodSetSystemSingle_SystemSingle_SystemSingle(ref UnityEngine.Vector3 thiz, float newX, float newY, float newZ)
		{
			try
			{
				thiz.Set(newX, newY, newZ);
			}
			catch (Exception ex)
			{
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
		}
		
		[MonoPInvokeCallback(typeof(ReleaseUnityEngineRaycastHitDelegate))]
		static void ReleaseUnityEngineRaycastHit(int handle)
		{
			try
			{
				if (handle != 0)
			{
				NativeScript.Bindings.StructStore<UnityEngine.RaycastHit>.Remove(handle);
			}
			}
			catch (Exception ex)
			{
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
		}
		
		[MonoPInvokeCallback(typeof(UnityEngineRaycastHitPropertyGetPointDelegate))]
		static UnityEngine.Vector3 UnityEngineRaycastHitPropertyGetPoint(int thisHandle)
		{
			try
			{
				var thiz = (UnityEngine.RaycastHit)NativeScript.Bindings.StructStore<UnityEngine.RaycastHit>.Get(thisHandle);
				var returnValue = thiz.point;
				return returnValue;
			}
			catch (Exception ex)
			{
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(UnityEngine.Vector3);
			}
		}
		
		[MonoPInvokeCallback(typeof(UnityEngineRaycastHitPropertySetPointDelegate))]
		static void UnityEngineRaycastHitPropertySetPoint(int thisHandle, ref UnityEngine.Vector3 value)
		{
			try
			{
				var thiz = (UnityEngine.RaycastHit)NativeScript.Bindings.StructStore<UnityEngine.RaycastHit>.Get(thisHandle);
				thiz.point = value;
				NativeScript.Bindings.StructStore<UnityEngine.RaycastHit>.Replace(thisHandle, ref thiz);
			}
			catch (Exception ex)
			{
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
		}
		
		[MonoPInvokeCallback(typeof(UnityEngineRaycastHitPropertyGetTransformDelegate))]
		static int UnityEngineRaycastHitPropertyGetTransform(int thisHandle)
		{
			try
			{
				var thiz = (UnityEngine.RaycastHit)NativeScript.Bindings.StructStore<UnityEngine.RaycastHit>.Get(thisHandle);
				var returnValue = thiz.transform;
				return NativeScript.Bindings.ObjectStore.GetHandle(returnValue);
			}
			catch (Exception ex)
			{
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
		}
		
		[MonoPInvokeCallback(typeof(ReleaseSystemCollectionsGenericKeyValuePairSystemString_SystemDoubleDelegate))]
		static void ReleaseSystemCollectionsGenericKeyValuePairSystemString_SystemDouble(int handle)
		{
			try
			{
				if (handle != 0)
			{
				NativeScript.Bindings.StructStore<System.Collections.Generic.KeyValuePair<string, double>>.Remove(handle);
			}
			}
			catch (Exception ex)
			{
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
		}
		
		[MonoPInvokeCallback(typeof(SystemCollectionsGenericKeyValuePairSystemString_SystemDoubleConstructorSystemString_SystemDoubleDelegate))]
		static int SystemCollectionsGenericKeyValuePairSystemString_SystemDoubleConstructorSystemString_SystemDouble(int keyHandle, double value)
		{
			try
			{
				var key = (string)NativeScript.Bindings.ObjectStore.Get(keyHandle);
				var returnValue = NativeScript.Bindings.StructStore<System.Collections.Generic.KeyValuePair<string, double>>.Store(new System.Collections.Generic.KeyValuePair<string, double>(key, value));
				return returnValue;
			}
			catch (Exception ex)
			{
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
		}
		
		[MonoPInvokeCallback(typeof(SystemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetKeyDelegate))]
		static int SystemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetKey(int thisHandle)
		{
			try
			{
				var thiz = (System.Collections.Generic.KeyValuePair<string, double>)NativeScript.Bindings.StructStore<System.Collections.Generic.KeyValuePair<string, double>>.Get(thisHandle);
				var returnValue = thiz.Key;
				return NativeScript.Bindings.ObjectStore.GetHandle(returnValue);
			}
			catch (Exception ex)
			{
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
		}
		
		[MonoPInvokeCallback(typeof(SystemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetValueDelegate))]
		static double SystemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetValue(int thisHandle)
		{
			try
			{
				var thiz = (System.Collections.Generic.KeyValuePair<string, double>)NativeScript.Bindings.StructStore<System.Collections.Generic.KeyValuePair<string, double>>.Get(thisHandle);
				var returnValue = thiz.Value;
				return returnValue;
			}
			catch (Exception ex)
			{
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(double);
			}
		}
		
		[MonoPInvokeCallback(typeof(SystemCollectionsGenericListSystemStringConstructorDelegate))]
		static int SystemCollectionsGenericListSystemStringConstructor()
		{
			try
			{
				var returnValue = NativeScript.Bindings.ObjectStore.Store(new System.Collections.Generic.List<string>());
				return returnValue;
			}
			catch (Exception ex)
			{
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
		}
		
		[MonoPInvokeCallback(typeof(SystemCollectionsGenericListSystemStringMethodAddSystemStringDelegate))]
		static void SystemCollectionsGenericListSystemStringMethodAddSystemString(int thisHandle, int itemHandle)
		{
			try
			{
				var thiz = (System.Collections.Generic.List<string>)NativeScript.Bindings.ObjectStore.Get(thisHandle);
				var item = (string)NativeScript.Bindings.ObjectStore.Get(itemHandle);
				thiz.Add(item);
			}
			catch (Exception ex)
			{
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
		}
		
		[MonoPInvokeCallback(typeof(SystemCollectionsGenericLinkedListNodeSystemStringConstructorSystemStringDelegate))]
		static int SystemCollectionsGenericLinkedListNodeSystemStringConstructorSystemString(int valueHandle)
		{
			try
			{
				var value = (string)NativeScript.Bindings.ObjectStore.Get(valueHandle);
				var returnValue = NativeScript.Bindings.ObjectStore.Store(new System.Collections.Generic.LinkedListNode<string>(value));
				return returnValue;
			}
			catch (Exception ex)
			{
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
		}
		
		[MonoPInvokeCallback(typeof(SystemCollectionsGenericLinkedListNodeSystemStringPropertyGetValueDelegate))]
		static int SystemCollectionsGenericLinkedListNodeSystemStringPropertyGetValue(int thisHandle)
		{
			try
			{
				var thiz = (System.Collections.Generic.LinkedListNode<string>)NativeScript.Bindings.ObjectStore.Get(thisHandle);
				var returnValue = thiz.Value;
				return NativeScript.Bindings.ObjectStore.GetHandle(returnValue);
			}
			catch (Exception ex)
			{
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
		}
		
		[MonoPInvokeCallback(typeof(SystemCollectionsGenericLinkedListNodeSystemStringPropertySetValueDelegate))]
		static void SystemCollectionsGenericLinkedListNodeSystemStringPropertySetValue(int thisHandle, int valueHandle)
		{
			try
			{
				var thiz = (System.Collections.Generic.LinkedListNode<string>)NativeScript.Bindings.ObjectStore.Get(thisHandle);
				var value = (string)NativeScript.Bindings.ObjectStore.Get(valueHandle);
				thiz.Value = value;
			}
			catch (Exception ex)
			{
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
		}
		
		[MonoPInvokeCallback(typeof(SystemRuntimeCompilerServicesStrongBoxSystemStringConstructorSystemStringDelegate))]
		static int SystemRuntimeCompilerServicesStrongBoxSystemStringConstructorSystemString(int valueHandle)
		{
			try
			{
				var value = (string)NativeScript.Bindings.ObjectStore.Get(valueHandle);
				var returnValue = NativeScript.Bindings.ObjectStore.Store(new System.Runtime.CompilerServices.StrongBox<string>(value));
				return returnValue;
			}
			catch (Exception ex)
			{
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
		}
		
		[MonoPInvokeCallback(typeof(SystemRuntimeCompilerServicesStrongBoxSystemStringFieldGetValueDelegate))]
		static int SystemRuntimeCompilerServicesStrongBoxSystemStringFieldGetValue(int thisHandle)
		{
			try
			{
				var thiz = (System.Runtime.CompilerServices.StrongBox<string>)NativeScript.Bindings.ObjectStore.Get(thisHandle);
				var returnValue = thiz.Value;
				return NativeScript.Bindings.ObjectStore.GetHandle(returnValue);
			}
			catch (Exception ex)
			{
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
		}
		
		[MonoPInvokeCallback(typeof(SystemRuntimeCompilerServicesStrongBoxSystemStringFieldSetValueDelegate))]
		static void SystemRuntimeCompilerServicesStrongBoxSystemStringFieldSetValue(int thisHandle, int valueHandle)
		{
			try
			{
				var thiz = (System.Runtime.CompilerServices.StrongBox<string>)NativeScript.Bindings.ObjectStore.Get(thisHandle);
				var value = (string)NativeScript.Bindings.ObjectStore.Get(valueHandle);
				thiz.Value = value;
			}
			catch (Exception ex)
			{
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
		}
		
		[MonoPInvokeCallback(typeof(SystemExceptionConstructorSystemStringDelegate))]
		static int SystemExceptionConstructorSystemString(int messageHandle)
		{
			try
			{
				var message = (string)NativeScript.Bindings.ObjectStore.Get(messageHandle);
				var returnValue = NativeScript.Bindings.ObjectStore.Store(new System.Exception(message));
				return returnValue;
			}
			catch (Exception ex)
			{
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
		}
		/*END FUNCTIONS*/
	}
}

/*BEGIN MONOBEHAVIOURS*/
namespace MyGame
{
	namespace MonoBehaviours
	{
		public class TestScript : UnityEngine.MonoBehaviour
		{
			public void Awake()
			{
				int thisHandle = NativeScript.Bindings.ObjectStore.GetHandle(this);
				NativeScript.Bindings.TestScriptAwake(thisHandle);
				if (NativeScript.Bindings.UnhandledCppException != null)
				{
					Exception ex = NativeScript.Bindings.UnhandledCppException;
					NativeScript.Bindings.UnhandledCppException = null;
					throw ex;
				}
			}
			
			public void OnAnimatorIK(int param0)
			{
				int thisHandle = NativeScript.Bindings.ObjectStore.GetHandle(this);
				NativeScript.Bindings.TestScriptOnAnimatorIK(thisHandle, param0);
				if (NativeScript.Bindings.UnhandledCppException != null)
				{
					Exception ex = NativeScript.Bindings.UnhandledCppException;
					NativeScript.Bindings.UnhandledCppException = null;
					throw ex;
				}
			}
			
			public void OnCollisionEnter(UnityEngine.Collision param0)
			{
				int param0Handle = NativeScript.Bindings.ObjectStore.Store(param0);
				int thisHandle = NativeScript.Bindings.ObjectStore.GetHandle(this);
				NativeScript.Bindings.TestScriptOnCollisionEnter(thisHandle, param0Handle);
				if (NativeScript.Bindings.UnhandledCppException != null)
				{
					Exception ex = NativeScript.Bindings.UnhandledCppException;
					NativeScript.Bindings.UnhandledCppException = null;
					throw ex;
				}
			}
			
			public void Update()
			{
				int thisHandle = NativeScript.Bindings.ObjectStore.GetHandle(this);
				NativeScript.Bindings.TestScriptUpdate(thisHandle);
				if (NativeScript.Bindings.UnhandledCppException != null)
				{
					Exception ex = NativeScript.Bindings.UnhandledCppException;
					NativeScript.Bindings.UnhandledCppException = null;
					throw ex;
				}
			}
		}
	}
}
/*END MONOBEHAVIOURS*/
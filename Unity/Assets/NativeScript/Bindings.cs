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
			
			public static object Remove(int handle)
			{
				// Null is never stored, so there's nothing to remove
				if (handle == 0)
				{
					return null;
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
					
					return obj;
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
			IntPtr arrayGetLength,
			IntPtr arrayGetRank,
			/*BEGIN INIT PARAMS*/
			IntPtr systemDiagnosticsStopwatchConstructor,
			IntPtr systemDiagnosticsStopwatchPropertyGetElapsedMilliseconds,
			IntPtr systemDiagnosticsStopwatchMethodStart,
			IntPtr systemDiagnosticsStopwatchMethodReset,
			IntPtr unityEngineObjectPropertyGetName,
			IntPtr unityEngineObjectPropertySetName,
			IntPtr unityEngineObjectMethodop_EqualityUnityEngineObject_UnityEngineObject,
			IntPtr unityEngineObjectMethodop_ImplicitUnityEngineObject,
			IntPtr unityEngineGameObjectConstructor,
			IntPtr unityEngineGameObjectConstructorSystemString,
			IntPtr unityEngineGameObjectPropertyGetTransform,
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
			IntPtr unityEngineVector3Methodop_AdditionUnityEngineVector3_UnityEngineVector3,
			IntPtr unityEngineVector3Methodop_UnaryNegationUnityEngineVector3,
			IntPtr unityEngineMatrix4x4PropertyGetItem,
			IntPtr unityEngineMatrix4x4PropertySetItem,
			IntPtr releaseUnityEngineRaycastHit,
			IntPtr unityEngineRaycastHitPropertyGetPoint,
			IntPtr unityEngineRaycastHitPropertySetPoint,
			IntPtr unityEngineRaycastHitPropertyGetTransform,
			IntPtr releaseSystemCollectionsGenericKeyValuePairSystemString_SystemDouble,
			IntPtr systemCollectionsGenericKeyValuePairSystemString_SystemDoubleConstructorSystemString_SystemDouble,
			IntPtr systemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetKey,
			IntPtr systemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetValue,
			IntPtr systemCollectionsGenericListSystemStringConstructor,
			IntPtr systemCollectionsGenericListSystemStringPropertyGetItem,
			IntPtr systemCollectionsGenericListSystemStringPropertySetItem,
			IntPtr systemCollectionsGenericListSystemStringMethodAddSystemString,
			IntPtr systemCollectionsGenericLinkedListNodeSystemStringConstructorSystemString,
			IntPtr systemCollectionsGenericLinkedListNodeSystemStringPropertyGetValue,
			IntPtr systemCollectionsGenericLinkedListNodeSystemStringPropertySetValue,
			IntPtr systemRuntimeCompilerServicesStrongBoxSystemStringConstructorSystemString,
			IntPtr systemRuntimeCompilerServicesStrongBoxSystemStringFieldGetValue,
			IntPtr systemRuntimeCompilerServicesStrongBoxSystemStringFieldSetValue,
			IntPtr systemExceptionConstructorSystemString,
			IntPtr unityEngineResolutionPropertyGetWidth,
			IntPtr unityEngineResolutionPropertySetWidth,
			IntPtr unityEngineResolutionPropertyGetHeight,
			IntPtr unityEngineResolutionPropertySetHeight,
			IntPtr unityEngineResolutionPropertyGetRefreshRate,
			IntPtr unityEngineResolutionPropertySetRefreshRate,
			IntPtr unityEngineScreenPropertyGetResolutions,
			IntPtr unityEngineRayConstructorUnityEngineVector3_UnityEngineVector3,
			IntPtr unityEnginePhysicsMethodRaycastNonAllocUnityEngineRay_UnityEngineRaycastHit,
			IntPtr unityEnginePhysicsMethodRaycastAllUnityEngineRay,
			IntPtr unityEngineGradientConstructor,
			IntPtr unityEngineGradientPropertyGetColorKeys,
			IntPtr unityEngineGradientPropertySetColorKeys,
			IntPtr systemInt32Array1Constructor1,
			IntPtr systemInt32Array1GetItem1,
			IntPtr systemInt32Array1SetItem1,
			IntPtr systemSingleArray1Constructor1,
			IntPtr systemSingleArray1GetItem1,
			IntPtr systemSingleArray1SetItem1,
			IntPtr systemSingleArray2Constructor2,
			IntPtr systemSingleArray2GetLength2,
			IntPtr systemSingleArray2GetItem2,
			IntPtr systemSingleArray2SetItem2,
			IntPtr systemSingleArray3Constructor3,
			IntPtr systemSingleArray3GetLength3,
			IntPtr systemSingleArray3GetItem3,
			IntPtr systemSingleArray3SetItem3,
			IntPtr systemStringArray1Constructor1,
			IntPtr systemStringArray1GetItem1,
			IntPtr systemStringArray1SetItem1,
			IntPtr unityEngineResolutionArray1Constructor1,
			IntPtr unityEngineResolutionArray1GetItem1,
			IntPtr unityEngineResolutionArray1SetItem1,
			IntPtr unityEngineRaycastHitArray1Constructor1,
			IntPtr unityEngineRaycastHitArray1GetItem1,
			IntPtr unityEngineRaycastHitArray1SetItem1,
			IntPtr unityEngineGradientColorKeyArray1Constructor1,
			IntPtr unityEngineGradientColorKeyArray1GetItem1,
			IntPtr unityEngineGradientColorKeyArray1SetItem1,
			IntPtr ReleaseSystemAction,
			IntPtr SystemActionConstructor,
			IntPtr SystemActionInvoke,
			IntPtr SystemActionAdd,
			IntPtr SystemActionRemove,
			IntPtr ReleaseSystemActionSystemSingle,
			IntPtr SystemActionSystemSingleConstructor,
			IntPtr SystemActionSystemSingleInvoke,
			IntPtr SystemActionSystemSingleAdd,
			IntPtr SystemActionSystemSingleRemove,
			IntPtr ReleaseSystemActionSystemSingle_SystemSingle,
			IntPtr SystemActionSystemSingle_SystemSingleConstructor,
			IntPtr SystemActionSystemSingle_SystemSingleInvoke,
			IntPtr SystemActionSystemSingle_SystemSingleAdd,
			IntPtr SystemActionSystemSingle_SystemSingleRemove,
			IntPtr ReleaseSystemFuncSystemInt32_SystemSingle_SystemDouble,
			IntPtr SystemFuncSystemInt32_SystemSingle_SystemDoubleConstructor,
			IntPtr SystemFuncSystemInt32_SystemSingle_SystemDoubleInvoke,
			IntPtr SystemFuncSystemInt32_SystemSingle_SystemDoubleAdd,
			IntPtr SystemFuncSystemInt32_SystemSingle_SystemDoubleRemove,
			IntPtr ReleaseSystemFuncSystemInt16_SystemInt32_SystemString,
			IntPtr SystemFuncSystemInt16_SystemInt32_SystemStringConstructor,
			IntPtr SystemFuncSystemInt16_SystemInt32_SystemStringInvoke,
			IntPtr SystemFuncSystemInt16_SystemInt32_SystemStringAdd,
			IntPtr SystemFuncSystemInt16_SystemInt32_SystemStringRemove
			/*END INIT PARAMS*/);
		
		public delegate void SetCsharpExceptionDelegate(int handle);
		
		/*BEGIN MONOBEHAVIOUR DELEGATES*/
		public delegate void MyGameMonoBehavioursTestScriptAwakeDelegate(int thisHandle);
		public static MyGameMonoBehavioursTestScriptAwakeDelegate MyGameMonoBehavioursTestScriptAwake;
		
		public delegate void MyGameMonoBehavioursTestScriptOnAnimatorIKDelegate(int thisHandle, int param0);
		public static MyGameMonoBehavioursTestScriptOnAnimatorIKDelegate MyGameMonoBehavioursTestScriptOnAnimatorIK;
		
		public delegate void MyGameMonoBehavioursTestScriptOnCollisionEnterDelegate(int thisHandle, int param0);
		public static MyGameMonoBehavioursTestScriptOnCollisionEnterDelegate MyGameMonoBehavioursTestScriptOnCollisionEnter;
		
		public delegate void MyGameMonoBehavioursTestScriptUpdateDelegate(int thisHandle);
		public static MyGameMonoBehavioursTestScriptUpdateDelegate MyGameMonoBehavioursTestScriptUpdate;
		
		public delegate void SystemActionCppInvokeDelegate(int thisHandle);
		public static SystemActionCppInvokeDelegate SystemActionCppInvoke;
		
		public delegate void SystemActionSystemSingleCppInvokeDelegate(int thisHandle, float param0);
		public static SystemActionSystemSingleCppInvokeDelegate SystemActionSystemSingleCppInvoke;
		
		public delegate void SystemActionSystemSingle_SystemSingleCppInvokeDelegate(int thisHandle, float param0, float param1);
		public static SystemActionSystemSingle_SystemSingleCppInvokeDelegate SystemActionSystemSingle_SystemSingleCppInvoke;
		
		public delegate double SystemFuncSystemInt32_SystemSingle_SystemDoubleCppInvokeDelegate(int thisHandle, int param0, float param1);
		public static SystemFuncSystemInt32_SystemSingle_SystemDoubleCppInvokeDelegate SystemFuncSystemInt32_SystemSingle_SystemDoubleCppInvoke;
		
		public delegate int SystemFuncSystemInt16_SystemInt32_SystemStringCppInvokeDelegate(int thisHandle, short param0, int param1);
		public static SystemFuncSystemInt16_SystemInt32_SystemStringCppInvokeDelegate SystemFuncSystemInt16_SystemInt32_SystemStringCppInvoke;
		
		public delegate void SetCsharpExceptionSystemNullReferenceExceptionDelegate(int param0);
		public static SetCsharpExceptionSystemNullReferenceExceptionDelegate SetCsharpExceptionSystemNullReferenceException;
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
			IntPtr arrayGetLength,
			IntPtr arrayGetRank,
			/*BEGIN INIT PARAMS*/
			IntPtr systemDiagnosticsStopwatchConstructor,
			IntPtr systemDiagnosticsStopwatchPropertyGetElapsedMilliseconds,
			IntPtr systemDiagnosticsStopwatchMethodStart,
			IntPtr systemDiagnosticsStopwatchMethodReset,
			IntPtr unityEngineObjectPropertyGetName,
			IntPtr unityEngineObjectPropertySetName,
			IntPtr unityEngineObjectMethodop_EqualityUnityEngineObject_UnityEngineObject,
			IntPtr unityEngineObjectMethodop_ImplicitUnityEngineObject,
			IntPtr unityEngineGameObjectConstructor,
			IntPtr unityEngineGameObjectConstructorSystemString,
			IntPtr unityEngineGameObjectPropertyGetTransform,
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
			IntPtr unityEngineVector3Methodop_AdditionUnityEngineVector3_UnityEngineVector3,
			IntPtr unityEngineVector3Methodop_UnaryNegationUnityEngineVector3,
			IntPtr unityEngineMatrix4x4PropertyGetItem,
			IntPtr unityEngineMatrix4x4PropertySetItem,
			IntPtr releaseUnityEngineRaycastHit,
			IntPtr unityEngineRaycastHitPropertyGetPoint,
			IntPtr unityEngineRaycastHitPropertySetPoint,
			IntPtr unityEngineRaycastHitPropertyGetTransform,
			IntPtr releaseSystemCollectionsGenericKeyValuePairSystemString_SystemDouble,
			IntPtr systemCollectionsGenericKeyValuePairSystemString_SystemDoubleConstructorSystemString_SystemDouble,
			IntPtr systemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetKey,
			IntPtr systemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetValue,
			IntPtr systemCollectionsGenericListSystemStringConstructor,
			IntPtr systemCollectionsGenericListSystemStringPropertyGetItem,
			IntPtr systemCollectionsGenericListSystemStringPropertySetItem,
			IntPtr systemCollectionsGenericListSystemStringMethodAddSystemString,
			IntPtr systemCollectionsGenericLinkedListNodeSystemStringConstructorSystemString,
			IntPtr systemCollectionsGenericLinkedListNodeSystemStringPropertyGetValue,
			IntPtr systemCollectionsGenericLinkedListNodeSystemStringPropertySetValue,
			IntPtr systemRuntimeCompilerServicesStrongBoxSystemStringConstructorSystemString,
			IntPtr systemRuntimeCompilerServicesStrongBoxSystemStringFieldGetValue,
			IntPtr systemRuntimeCompilerServicesStrongBoxSystemStringFieldSetValue,
			IntPtr systemExceptionConstructorSystemString,
			IntPtr unityEngineResolutionPropertyGetWidth,
			IntPtr unityEngineResolutionPropertySetWidth,
			IntPtr unityEngineResolutionPropertyGetHeight,
			IntPtr unityEngineResolutionPropertySetHeight,
			IntPtr unityEngineResolutionPropertyGetRefreshRate,
			IntPtr unityEngineResolutionPropertySetRefreshRate,
			IntPtr unityEngineScreenPropertyGetResolutions,
			IntPtr unityEngineRayConstructorUnityEngineVector3_UnityEngineVector3,
			IntPtr unityEnginePhysicsMethodRaycastNonAllocUnityEngineRay_UnityEngineRaycastHit,
			IntPtr unityEnginePhysicsMethodRaycastAllUnityEngineRay,
			IntPtr unityEngineGradientConstructor,
			IntPtr unityEngineGradientPropertyGetColorKeys,
			IntPtr unityEngineGradientPropertySetColorKeys,
			IntPtr systemInt32Array1Constructor1,
			IntPtr systemInt32Array1GetItem1,
			IntPtr systemInt32Array1SetItem1,
			IntPtr systemSingleArray1Constructor1,
			IntPtr systemSingleArray1GetItem1,
			IntPtr systemSingleArray1SetItem1,
			IntPtr systemSingleArray2Constructor2,
			IntPtr systemSingleArray2GetLength2,
			IntPtr systemSingleArray2GetItem2,
			IntPtr systemSingleArray2SetItem2,
			IntPtr systemSingleArray3Constructor3,
			IntPtr systemSingleArray3GetLength3,
			IntPtr systemSingleArray3GetItem3,
			IntPtr systemSingleArray3SetItem3,
			IntPtr systemStringArray1Constructor1,
			IntPtr systemStringArray1GetItem1,
			IntPtr systemStringArray1SetItem1,
			IntPtr unityEngineResolutionArray1Constructor1,
			IntPtr unityEngineResolutionArray1GetItem1,
			IntPtr unityEngineResolutionArray1SetItem1,
			IntPtr unityEngineRaycastHitArray1Constructor1,
			IntPtr unityEngineRaycastHitArray1GetItem1,
			IntPtr unityEngineRaycastHitArray1SetItem1,
			IntPtr unityEngineGradientColorKeyArray1Constructor1,
			IntPtr unityEngineGradientColorKeyArray1GetItem1,
			IntPtr unityEngineGradientColorKeyArray1SetItem1,
			IntPtr ReleaseSystemAction,
			IntPtr SystemActionConstructor,
			IntPtr SystemActionInvoke,
			IntPtr SystemActionAdd,
			IntPtr SystemActionRemove,
			IntPtr ReleaseSystemActionSystemSingle,
			IntPtr SystemActionSystemSingleConstructor,
			IntPtr SystemActionSystemSingleInvoke,
			IntPtr SystemActionSystemSingleAdd,
			IntPtr SystemActionSystemSingleRemove,
			IntPtr ReleaseSystemActionSystemSingle_SystemSingle,
			IntPtr SystemActionSystemSingle_SystemSingleConstructor,
			IntPtr SystemActionSystemSingle_SystemSingleInvoke,
			IntPtr SystemActionSystemSingle_SystemSingleAdd,
			IntPtr SystemActionSystemSingle_SystemSingleRemove,
			IntPtr ReleaseSystemFuncSystemInt32_SystemSingle_SystemDouble,
			IntPtr SystemFuncSystemInt32_SystemSingle_SystemDoubleConstructor,
			IntPtr SystemFuncSystemInt32_SystemSingle_SystemDoubleInvoke,
			IntPtr SystemFuncSystemInt32_SystemSingle_SystemDoubleAdd,
			IntPtr SystemFuncSystemInt32_SystemSingle_SystemDoubleRemove,
			IntPtr ReleaseSystemFuncSystemInt16_SystemInt32_SystemString,
			IntPtr SystemFuncSystemInt16_SystemInt32_SystemStringConstructor,
			IntPtr SystemFuncSystemInt16_SystemInt32_SystemStringInvoke,
			IntPtr SystemFuncSystemInt16_SystemInt32_SystemStringAdd,
			IntPtr SystemFuncSystemInt16_SystemInt32_SystemStringRemove
			/*END INIT PARAMS*/);
		
		[DllImport(PluginName)]
		static extern void SetCsharpException(int handle);
		
		/*BEGIN MONOBEHAVIOUR IMPORTS*/
		[DllImport(Constants.PluginName)]
		public static extern void MyGameMonoBehavioursTestScriptAwake(int thisHandle);
		
		[DllImport(Constants.PluginName)]
		public static extern void MyGameMonoBehavioursTestScriptOnAnimatorIK(int thisHandle, int param0);
		
		[DllImport(Constants.PluginName)]
		public static extern void MyGameMonoBehavioursTestScriptOnCollisionEnter(int thisHandle, int param0);
		
		[DllImport(Constants.PluginName)]
		public static extern void MyGameMonoBehavioursTestScriptUpdate(int thisHandle);
		
		[DllImport(Constants.PluginName)]
		public static extern void SystemActionCppInvoke(int thisHandle);
		
		[DllImport(Constants.PluginName)]
		public static extern void SystemActionSystemSingleCppInvoke(int thisHandle, int param0);
		
		[DllImport(Constants.PluginName)]
		public static extern void SystemActionSystemSingle_SystemSingleCppInvoke(int thisHandle, int param0, int param1);
		
		[DllImport(Constants.PluginName)]
		public static extern void SystemFuncSystemInt32_SystemSingle_SystemDoubleCppInvoke(int thisHandle, int param0, int param1);
		
		[DllImport(Constants.PluginName)]
		public static extern void SystemFuncSystemInt16_SystemInt32_SystemStringCppInvoke(int thisHandle, int param0, int param1);
		
		[DllImport(Constants.PluginName)]
		public static extern void SetCsharpExceptionSystemNullReferenceException(int thisHandle, int param0);
		/*END MONOBEHAVIOUR IMPORTS*/
#endif
		
		delegate void ReleaseObjectDelegate(int handle);
		delegate int StringNewDelegate(string chars);
		delegate void SetExceptionDelegate(int handle);
		delegate int ArrayGetLengthDelegate(int handle);
		delegate int ArrayGetRankDelegate(int handle);
		
		/*BEGIN DELEGATE TYPES*/
		delegate int SystemDiagnosticsStopwatchConstructorDelegate();
		delegate long SystemDiagnosticsStopwatchPropertyGetElapsedMillisecondsDelegate(int thisHandle);
		delegate void SystemDiagnosticsStopwatchMethodStartDelegate(int thisHandle);
		delegate void SystemDiagnosticsStopwatchMethodResetDelegate(int thisHandle);
		delegate int UnityEngineObjectPropertyGetNameDelegate(int thisHandle);
		delegate void UnityEngineObjectPropertySetNameDelegate(int thisHandle, int valueHandle);
		delegate bool UnityEngineObjectMethodop_EqualityUnityEngineObject_UnityEngineObjectDelegate(int xHandle, int yHandle);
		delegate bool UnityEngineObjectMethodop_ImplicitUnityEngineObjectDelegate(int existsHandle);
		delegate int UnityEngineGameObjectConstructorDelegate();
		delegate int UnityEngineGameObjectConstructorSystemStringDelegate(int nameHandle);
		delegate int UnityEngineGameObjectPropertyGetTransformDelegate(int thisHandle);
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
		delegate UnityEngine.Vector3 UnityEngineVector3Methodop_AdditionUnityEngineVector3_UnityEngineVector3Delegate(ref UnityEngine.Vector3 a, ref UnityEngine.Vector3 b);
		delegate UnityEngine.Vector3 UnityEngineVector3Methodop_UnaryNegationUnityEngineVector3Delegate(ref UnityEngine.Vector3 a);
		delegate float UnityEngineMatrix4x4PropertyGetItemDelegate(ref UnityEngine.Matrix4x4 thiz, int row, int column);
		delegate void UnityEngineMatrix4x4PropertySetItemDelegate(ref UnityEngine.Matrix4x4 thiz, int row, int column, float value);
		delegate void ReleaseUnityEngineRaycastHitDelegate(int handle);
		delegate UnityEngine.Vector3 UnityEngineRaycastHitPropertyGetPointDelegate(int thisHandle);
		delegate void UnityEngineRaycastHitPropertySetPointDelegate(int thisHandle, ref UnityEngine.Vector3 value);
		delegate int UnityEngineRaycastHitPropertyGetTransformDelegate(int thisHandle);
		delegate void ReleaseSystemCollectionsGenericKeyValuePairSystemString_SystemDoubleDelegate(int handle);
		delegate int SystemCollectionsGenericKeyValuePairSystemString_SystemDoubleConstructorSystemString_SystemDoubleDelegate(int keyHandle, double value);
		delegate int SystemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetKeyDelegate(int thisHandle);
		delegate double SystemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetValueDelegate(int thisHandle);
		delegate int SystemCollectionsGenericListSystemStringConstructorDelegate();
		delegate int SystemCollectionsGenericListSystemStringPropertyGetItemDelegate(int thisHandle, int index);
		delegate void SystemCollectionsGenericListSystemStringPropertySetItemDelegate(int thisHandle, int index, int valueHandle);
		delegate void SystemCollectionsGenericListSystemStringMethodAddSystemStringDelegate(int thisHandle, int itemHandle);
		delegate int SystemCollectionsGenericLinkedListNodeSystemStringConstructorSystemStringDelegate(int valueHandle);
		delegate int SystemCollectionsGenericLinkedListNodeSystemStringPropertyGetValueDelegate(int thisHandle);
		delegate void SystemCollectionsGenericLinkedListNodeSystemStringPropertySetValueDelegate(int thisHandle, int valueHandle);
		delegate int SystemRuntimeCompilerServicesStrongBoxSystemStringConstructorSystemStringDelegate(int valueHandle);
		delegate int SystemRuntimeCompilerServicesStrongBoxSystemStringFieldGetValueDelegate(int thisHandle);
		delegate void SystemRuntimeCompilerServicesStrongBoxSystemStringFieldSetValueDelegate(int thisHandle, int valueHandle);
		delegate int SystemExceptionConstructorSystemStringDelegate(int messageHandle);
		delegate int UnityEngineResolutionPropertyGetWidthDelegate(ref UnityEngine.Resolution thiz);
		delegate void UnityEngineResolutionPropertySetWidthDelegate(ref UnityEngine.Resolution thiz, int value);
		delegate int UnityEngineResolutionPropertyGetHeightDelegate(ref UnityEngine.Resolution thiz);
		delegate void UnityEngineResolutionPropertySetHeightDelegate(ref UnityEngine.Resolution thiz, int value);
		delegate int UnityEngineResolutionPropertyGetRefreshRateDelegate(ref UnityEngine.Resolution thiz);
		delegate void UnityEngineResolutionPropertySetRefreshRateDelegate(ref UnityEngine.Resolution thiz, int value);
		delegate int UnityEngineScreenPropertyGetResolutionsDelegate();
		delegate UnityEngine.Ray UnityEngineRayConstructorUnityEngineVector3_UnityEngineVector3Delegate(ref UnityEngine.Vector3 origin, ref UnityEngine.Vector3 direction);
		delegate int UnityEnginePhysicsMethodRaycastNonAllocUnityEngineRay_UnityEngineRaycastHitDelegate(ref UnityEngine.Ray ray, int resultsHandle);
		delegate int UnityEnginePhysicsMethodRaycastAllUnityEngineRayDelegate(ref UnityEngine.Ray ray);
		delegate int UnityEngineGradientConstructorDelegate();
		delegate int UnityEngineGradientPropertyGetColorKeysDelegate(int thisHandle);
		delegate void UnityEngineGradientPropertySetColorKeysDelegate(int thisHandle, int valueHandle);
		delegate int SystemInt32Array1Constructor1Delegate(int length0);
		delegate int SystemInt32Array1GetItem1Delegate(int thisHandle, int index0);
		delegate void SystemInt32Array1SetItem1Delegate(int thisHandle, int index0, int item);
		delegate int SystemSingleArray1Constructor1Delegate(int length0);
		delegate float SystemSingleArray1GetItem1Delegate(int thisHandle, int index0);
		delegate void SystemSingleArray1SetItem1Delegate(int thisHandle, int index0, float item);
		delegate int SystemSingleArray2Constructor2Delegate(int length0, int length1);
		delegate int SystemSingleArray2GetLength2Delegate(int thisHandle, int dimension);
		delegate float SystemSingleArray2GetItem2Delegate(int thisHandle, int index0, int index1);
		delegate void SystemSingleArray2SetItem2Delegate(int thisHandle, int index0, int index1, float item);
		delegate int SystemSingleArray3Constructor3Delegate(int length0, int length1, int length2);
		delegate int SystemSingleArray3GetLength3Delegate(int thisHandle, int dimension);
		delegate float SystemSingleArray3GetItem3Delegate(int thisHandle, int index0, int index1, int index2);
		delegate void SystemSingleArray3SetItem3Delegate(int thisHandle, int index0, int index1, int index2, float item);
		delegate int SystemStringArray1Constructor1Delegate(int length0);
		delegate int SystemStringArray1GetItem1Delegate(int thisHandle, int index0);
		delegate void SystemStringArray1SetItem1Delegate(int thisHandle, int index0, int itemHandle);
		delegate int UnityEngineResolutionArray1Constructor1Delegate(int length0);
		delegate UnityEngine.Resolution UnityEngineResolutionArray1GetItem1Delegate(int thisHandle, int index0);
		delegate void UnityEngineResolutionArray1SetItem1Delegate(int thisHandle, int index0, ref UnityEngine.Resolution item);
		delegate int UnityEngineRaycastHitArray1Constructor1Delegate(int length0);
		delegate int UnityEngineRaycastHitArray1GetItem1Delegate(int thisHandle, int index0);
		delegate void UnityEngineRaycastHitArray1SetItem1Delegate(int thisHandle, int index0, int itemHandle);
		delegate int UnityEngineGradientColorKeyArray1Constructor1Delegate(int length0);
		delegate UnityEngine.GradientColorKey UnityEngineGradientColorKeyArray1GetItem1Delegate(int thisHandle, int index0);
		delegate void UnityEngineGradientColorKeyArray1SetItem1Delegate(int thisHandle, int index0, ref UnityEngine.GradientColorKey item);
		delegate void SystemActionConstructorDelegate(int cppHandle, ref int handle, ref int delegateHandle);
		delegate void ReleaseSystemActionDelegate(int handle, int delegateHandle);
		delegate void SystemActionInvokeDelegate(int thisHandle);
		delegate void SystemActionAddDelegate(int thisHandle, int delHandle);
		delegate void SystemActionRemoveDelegate(int thisHandle, int delHandle);
		delegate void SystemActionSystemSingleConstructorDelegate(int cppHandle, ref int handle, ref int delegateHandle);
		delegate void ReleaseSystemActionSystemSingleDelegate(int handle, int delegateHandle);
		delegate void SystemActionSystemSingleInvokeDelegate(int thisHandle, float obj);
		delegate void SystemActionSystemSingleAddDelegate(int thisHandle, int delHandle);
		delegate void SystemActionSystemSingleRemoveDelegate(int thisHandle, int delHandle);
		delegate void SystemActionSystemSingle_SystemSingleConstructorDelegate(int cppHandle, ref int handle, ref int delegateHandle);
		delegate void ReleaseSystemActionSystemSingle_SystemSingleDelegate(int handle, int delegateHandle);
		delegate void SystemActionSystemSingle_SystemSingleInvokeDelegate(int thisHandle, float arg1, float arg2);
		delegate void SystemActionSystemSingle_SystemSingleAddDelegate(int thisHandle, int delHandle);
		delegate void SystemActionSystemSingle_SystemSingleRemoveDelegate(int thisHandle, int delHandle);
		delegate void SystemFuncSystemInt32_SystemSingle_SystemDoubleConstructorDelegate(int cppHandle, ref int handle, ref int delegateHandle);
		delegate void ReleaseSystemFuncSystemInt32_SystemSingle_SystemDoubleDelegate(int handle, int delegateHandle);
		delegate double SystemFuncSystemInt32_SystemSingle_SystemDoubleInvokeDelegate(int thisHandle, int arg1, float arg2);
		delegate void SystemFuncSystemInt32_SystemSingle_SystemDoubleAddDelegate(int thisHandle, int delHandle);
		delegate void SystemFuncSystemInt32_SystemSingle_SystemDoubleRemoveDelegate(int thisHandle, int delHandle);
		delegate void SystemFuncSystemInt16_SystemInt32_SystemStringConstructorDelegate(int cppHandle, ref int handle, ref int delegateHandle);
		delegate void ReleaseSystemFuncSystemInt16_SystemInt32_SystemStringDelegate(int handle, int delegateHandle);
		delegate int SystemFuncSystemInt16_SystemInt32_SystemStringInvokeDelegate(int thisHandle, short arg1, int arg2);
		delegate void SystemFuncSystemInt16_SystemInt32_SystemStringAddDelegate(int thisHandle, int delHandle);
		delegate void SystemFuncSystemInt16_SystemInt32_SystemStringRemoveDelegate(int thisHandle, int delHandle);
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
			MyGameMonoBehavioursTestScriptAwake = GetDelegate<MyGameMonoBehavioursTestScriptAwakeDelegate>(libraryHandle, "MyGameMonoBehavioursTestScriptAwake");
			MyGameMonoBehavioursTestScriptOnAnimatorIK = GetDelegate<MyGameMonoBehavioursTestScriptOnAnimatorIKDelegate>(libraryHandle, "MyGameMonoBehavioursTestScriptOnAnimatorIK");
			MyGameMonoBehavioursTestScriptOnCollisionEnter = GetDelegate<MyGameMonoBehavioursTestScriptOnCollisionEnterDelegate>(libraryHandle, "MyGameMonoBehavioursTestScriptOnCollisionEnter");
			MyGameMonoBehavioursTestScriptUpdate = GetDelegate<MyGameMonoBehavioursTestScriptUpdateDelegate>(libraryHandle, "MyGameMonoBehavioursTestScriptUpdate");
			SystemActionCppInvoke = GetDelegate<SystemActionCppInvokeDelegate>(libraryHandle, "SystemActionCppInvoke");
			SystemActionSystemSingleCppInvoke = GetDelegate<SystemActionSystemSingleCppInvokeDelegate>(libraryHandle, "SystemActionSystemSingleCppInvoke");
			SystemActionSystemSingle_SystemSingleCppInvoke = GetDelegate<SystemActionSystemSingle_SystemSingleCppInvokeDelegate>(libraryHandle, "SystemActionSystemSingle_SystemSingleCppInvoke");
			SystemFuncSystemInt32_SystemSingle_SystemDoubleCppInvoke = GetDelegate<SystemFuncSystemInt32_SystemSingle_SystemDoubleCppInvokeDelegate>(libraryHandle, "SystemFuncSystemInt32_SystemSingle_SystemDoubleCppInvoke");
			SystemFuncSystemInt16_SystemInt32_SystemStringCppInvoke = GetDelegate<SystemFuncSystemInt16_SystemInt32_SystemStringCppInvokeDelegate>(libraryHandle, "SystemFuncSystemInt16_SystemInt32_SystemStringCppInvoke");
			SetCsharpExceptionSystemNullReferenceException = GetDelegate<SetCsharpExceptionSystemNullReferenceExceptionDelegate>(libraryHandle, "SetCsharpExceptionSystemNullReferenceException");
			/*END MONOBEHAVIOUR GETDELEGATE CALLS*/

#endif
			
			// Init C++ library
			Init(
				maxManagedObjects,
				Marshal.GetFunctionPointerForDelegate(new ReleaseObjectDelegate(ReleaseObject)),
				Marshal.GetFunctionPointerForDelegate(new StringNewDelegate(StringNew)),
				Marshal.GetFunctionPointerForDelegate(new SetExceptionDelegate(SetException)),
				Marshal.GetFunctionPointerForDelegate(new ArrayGetLengthDelegate(ArrayGetLength)),
				Marshal.GetFunctionPointerForDelegate(new ArrayGetRankDelegate(ArrayGetRank)),
				/*BEGIN INIT CALL*/
				Marshal.GetFunctionPointerForDelegate(new SystemDiagnosticsStopwatchConstructorDelegate(SystemDiagnosticsStopwatchConstructor)),
				Marshal.GetFunctionPointerForDelegate(new SystemDiagnosticsStopwatchPropertyGetElapsedMillisecondsDelegate(SystemDiagnosticsStopwatchPropertyGetElapsedMilliseconds)),
				Marshal.GetFunctionPointerForDelegate(new SystemDiagnosticsStopwatchMethodStartDelegate(SystemDiagnosticsStopwatchMethodStart)),
				Marshal.GetFunctionPointerForDelegate(new SystemDiagnosticsStopwatchMethodResetDelegate(SystemDiagnosticsStopwatchMethodReset)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineObjectPropertyGetNameDelegate(UnityEngineObjectPropertyGetName)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineObjectPropertySetNameDelegate(UnityEngineObjectPropertySetName)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineObjectMethodop_EqualityUnityEngineObject_UnityEngineObjectDelegate(UnityEngineObjectMethodop_EqualityUnityEngineObject_UnityEngineObject)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineObjectMethodop_ImplicitUnityEngineObjectDelegate(UnityEngineObjectMethodop_ImplicitUnityEngineObject)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineGameObjectConstructorDelegate(UnityEngineGameObjectConstructor)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineGameObjectConstructorSystemStringDelegate(UnityEngineGameObjectConstructorSystemString)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineGameObjectPropertyGetTransformDelegate(UnityEngineGameObjectPropertyGetTransform)),
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
				Marshal.GetFunctionPointerForDelegate(new UnityEngineVector3Methodop_AdditionUnityEngineVector3_UnityEngineVector3Delegate(UnityEngineVector3Methodop_AdditionUnityEngineVector3_UnityEngineVector3)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineVector3Methodop_UnaryNegationUnityEngineVector3Delegate(UnityEngineVector3Methodop_UnaryNegationUnityEngineVector3)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineMatrix4x4PropertyGetItemDelegate(UnityEngineMatrix4x4PropertyGetItem)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineMatrix4x4PropertySetItemDelegate(UnityEngineMatrix4x4PropertySetItem)),
				Marshal.GetFunctionPointerForDelegate(new ReleaseUnityEngineRaycastHitDelegate(ReleaseUnityEngineRaycastHit)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineRaycastHitPropertyGetPointDelegate(UnityEngineRaycastHitPropertyGetPoint)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineRaycastHitPropertySetPointDelegate(UnityEngineRaycastHitPropertySetPoint)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineRaycastHitPropertyGetTransformDelegate(UnityEngineRaycastHitPropertyGetTransform)),
				Marshal.GetFunctionPointerForDelegate(new ReleaseSystemCollectionsGenericKeyValuePairSystemString_SystemDoubleDelegate(ReleaseSystemCollectionsGenericKeyValuePairSystemString_SystemDouble)),
				Marshal.GetFunctionPointerForDelegate(new SystemCollectionsGenericKeyValuePairSystemString_SystemDoubleConstructorSystemString_SystemDoubleDelegate(SystemCollectionsGenericKeyValuePairSystemString_SystemDoubleConstructorSystemString_SystemDouble)),
				Marshal.GetFunctionPointerForDelegate(new SystemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetKeyDelegate(SystemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetKey)),
				Marshal.GetFunctionPointerForDelegate(new SystemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetValueDelegate(SystemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetValue)),
				Marshal.GetFunctionPointerForDelegate(new SystemCollectionsGenericListSystemStringConstructorDelegate(SystemCollectionsGenericListSystemStringConstructor)),
				Marshal.GetFunctionPointerForDelegate(new SystemCollectionsGenericListSystemStringPropertyGetItemDelegate(SystemCollectionsGenericListSystemStringPropertyGetItem)),
				Marshal.GetFunctionPointerForDelegate(new SystemCollectionsGenericListSystemStringPropertySetItemDelegate(SystemCollectionsGenericListSystemStringPropertySetItem)),
				Marshal.GetFunctionPointerForDelegate(new SystemCollectionsGenericListSystemStringMethodAddSystemStringDelegate(SystemCollectionsGenericListSystemStringMethodAddSystemString)),
				Marshal.GetFunctionPointerForDelegate(new SystemCollectionsGenericLinkedListNodeSystemStringConstructorSystemStringDelegate(SystemCollectionsGenericLinkedListNodeSystemStringConstructorSystemString)),
				Marshal.GetFunctionPointerForDelegate(new SystemCollectionsGenericLinkedListNodeSystemStringPropertyGetValueDelegate(SystemCollectionsGenericLinkedListNodeSystemStringPropertyGetValue)),
				Marshal.GetFunctionPointerForDelegate(new SystemCollectionsGenericLinkedListNodeSystemStringPropertySetValueDelegate(SystemCollectionsGenericLinkedListNodeSystemStringPropertySetValue)),
				Marshal.GetFunctionPointerForDelegate(new SystemRuntimeCompilerServicesStrongBoxSystemStringConstructorSystemStringDelegate(SystemRuntimeCompilerServicesStrongBoxSystemStringConstructorSystemString)),
				Marshal.GetFunctionPointerForDelegate(new SystemRuntimeCompilerServicesStrongBoxSystemStringFieldGetValueDelegate(SystemRuntimeCompilerServicesStrongBoxSystemStringFieldGetValue)),
				Marshal.GetFunctionPointerForDelegate(new SystemRuntimeCompilerServicesStrongBoxSystemStringFieldSetValueDelegate(SystemRuntimeCompilerServicesStrongBoxSystemStringFieldSetValue)),
				Marshal.GetFunctionPointerForDelegate(new SystemExceptionConstructorSystemStringDelegate(SystemExceptionConstructorSystemString)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineResolutionPropertyGetWidthDelegate(UnityEngineResolutionPropertyGetWidth)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineResolutionPropertySetWidthDelegate(UnityEngineResolutionPropertySetWidth)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineResolutionPropertyGetHeightDelegate(UnityEngineResolutionPropertyGetHeight)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineResolutionPropertySetHeightDelegate(UnityEngineResolutionPropertySetHeight)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineResolutionPropertyGetRefreshRateDelegate(UnityEngineResolutionPropertyGetRefreshRate)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineResolutionPropertySetRefreshRateDelegate(UnityEngineResolutionPropertySetRefreshRate)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineScreenPropertyGetResolutionsDelegate(UnityEngineScreenPropertyGetResolutions)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineRayConstructorUnityEngineVector3_UnityEngineVector3Delegate(UnityEngineRayConstructorUnityEngineVector3_UnityEngineVector3)),
				Marshal.GetFunctionPointerForDelegate(new UnityEnginePhysicsMethodRaycastNonAllocUnityEngineRay_UnityEngineRaycastHitDelegate(UnityEnginePhysicsMethodRaycastNonAllocUnityEngineRay_UnityEngineRaycastHit)),
				Marshal.GetFunctionPointerForDelegate(new UnityEnginePhysicsMethodRaycastAllUnityEngineRayDelegate(UnityEnginePhysicsMethodRaycastAllUnityEngineRay)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineGradientConstructorDelegate(UnityEngineGradientConstructor)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineGradientPropertyGetColorKeysDelegate(UnityEngineGradientPropertyGetColorKeys)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineGradientPropertySetColorKeysDelegate(UnityEngineGradientPropertySetColorKeys)),
				Marshal.GetFunctionPointerForDelegate(new SystemInt32Array1Constructor1Delegate(SystemInt32Array1Constructor1)),
				Marshal.GetFunctionPointerForDelegate(new SystemInt32Array1GetItem1Delegate(SystemInt32Array1GetItem1)),
				Marshal.GetFunctionPointerForDelegate(new SystemInt32Array1SetItem1Delegate(SystemInt32Array1SetItem1)),
				Marshal.GetFunctionPointerForDelegate(new SystemSingleArray1Constructor1Delegate(SystemSingleArray1Constructor1)),
				Marshal.GetFunctionPointerForDelegate(new SystemSingleArray1GetItem1Delegate(SystemSingleArray1GetItem1)),
				Marshal.GetFunctionPointerForDelegate(new SystemSingleArray1SetItem1Delegate(SystemSingleArray1SetItem1)),
				Marshal.GetFunctionPointerForDelegate(new SystemSingleArray2Constructor2Delegate(SystemSingleArray2Constructor2)),
				Marshal.GetFunctionPointerForDelegate(new SystemSingleArray2GetLength2Delegate(SystemSingleArray2GetLength2)),
				Marshal.GetFunctionPointerForDelegate(new SystemSingleArray2GetItem2Delegate(SystemSingleArray2GetItem2)),
				Marshal.GetFunctionPointerForDelegate(new SystemSingleArray2SetItem2Delegate(SystemSingleArray2SetItem2)),
				Marshal.GetFunctionPointerForDelegate(new SystemSingleArray3Constructor3Delegate(SystemSingleArray3Constructor3)),
				Marshal.GetFunctionPointerForDelegate(new SystemSingleArray3GetLength3Delegate(SystemSingleArray3GetLength3)),
				Marshal.GetFunctionPointerForDelegate(new SystemSingleArray3GetItem3Delegate(SystemSingleArray3GetItem3)),
				Marshal.GetFunctionPointerForDelegate(new SystemSingleArray3SetItem3Delegate(SystemSingleArray3SetItem3)),
				Marshal.GetFunctionPointerForDelegate(new SystemStringArray1Constructor1Delegate(SystemStringArray1Constructor1)),
				Marshal.GetFunctionPointerForDelegate(new SystemStringArray1GetItem1Delegate(SystemStringArray1GetItem1)),
				Marshal.GetFunctionPointerForDelegate(new SystemStringArray1SetItem1Delegate(SystemStringArray1SetItem1)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineResolutionArray1Constructor1Delegate(UnityEngineResolutionArray1Constructor1)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineResolutionArray1GetItem1Delegate(UnityEngineResolutionArray1GetItem1)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineResolutionArray1SetItem1Delegate(UnityEngineResolutionArray1SetItem1)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineRaycastHitArray1Constructor1Delegate(UnityEngineRaycastHitArray1Constructor1)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineRaycastHitArray1GetItem1Delegate(UnityEngineRaycastHitArray1GetItem1)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineRaycastHitArray1SetItem1Delegate(UnityEngineRaycastHitArray1SetItem1)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineGradientColorKeyArray1Constructor1Delegate(UnityEngineGradientColorKeyArray1Constructor1)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineGradientColorKeyArray1GetItem1Delegate(UnityEngineGradientColorKeyArray1GetItem1)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineGradientColorKeyArray1SetItem1Delegate(UnityEngineGradientColorKeyArray1SetItem1)),
				Marshal.GetFunctionPointerForDelegate(new ReleaseSystemActionDelegate(ReleaseSystemAction)),
				Marshal.GetFunctionPointerForDelegate(new SystemActionConstructorDelegate(SystemActionConstructor)),
				Marshal.GetFunctionPointerForDelegate(new SystemActionInvokeDelegate(SystemActionInvoke)),
				Marshal.GetFunctionPointerForDelegate(new SystemActionAddDelegate(SystemActionAdd)),
				Marshal.GetFunctionPointerForDelegate(new SystemActionRemoveDelegate(SystemActionRemove)),
				Marshal.GetFunctionPointerForDelegate(new ReleaseSystemActionSystemSingleDelegate(ReleaseSystemActionSystemSingle)),
				Marshal.GetFunctionPointerForDelegate(new SystemActionSystemSingleConstructorDelegate(SystemActionSystemSingleConstructor)),
				Marshal.GetFunctionPointerForDelegate(new SystemActionSystemSingleInvokeDelegate(SystemActionSystemSingleInvoke)),
				Marshal.GetFunctionPointerForDelegate(new SystemActionSystemSingleAddDelegate(SystemActionSystemSingleAdd)),
				Marshal.GetFunctionPointerForDelegate(new SystemActionSystemSingleRemoveDelegate(SystemActionSystemSingleRemove)),
				Marshal.GetFunctionPointerForDelegate(new ReleaseSystemActionSystemSingle_SystemSingleDelegate(ReleaseSystemActionSystemSingle_SystemSingle)),
				Marshal.GetFunctionPointerForDelegate(new SystemActionSystemSingle_SystemSingleConstructorDelegate(SystemActionSystemSingle_SystemSingleConstructor)),
				Marshal.GetFunctionPointerForDelegate(new SystemActionSystemSingle_SystemSingleInvokeDelegate(SystemActionSystemSingle_SystemSingleInvoke)),
				Marshal.GetFunctionPointerForDelegate(new SystemActionSystemSingle_SystemSingleAddDelegate(SystemActionSystemSingle_SystemSingleAdd)),
				Marshal.GetFunctionPointerForDelegate(new SystemActionSystemSingle_SystemSingleRemoveDelegate(SystemActionSystemSingle_SystemSingleRemove)),
				Marshal.GetFunctionPointerForDelegate(new ReleaseSystemFuncSystemInt32_SystemSingle_SystemDoubleDelegate(ReleaseSystemFuncSystemInt32_SystemSingle_SystemDouble)),
				Marshal.GetFunctionPointerForDelegate(new SystemFuncSystemInt32_SystemSingle_SystemDoubleConstructorDelegate(SystemFuncSystemInt32_SystemSingle_SystemDoubleConstructor)),
				Marshal.GetFunctionPointerForDelegate(new SystemFuncSystemInt32_SystemSingle_SystemDoubleInvokeDelegate(SystemFuncSystemInt32_SystemSingle_SystemDoubleInvoke)),
				Marshal.GetFunctionPointerForDelegate(new SystemFuncSystemInt32_SystemSingle_SystemDoubleAddDelegate(SystemFuncSystemInt32_SystemSingle_SystemDoubleAdd)),
				Marshal.GetFunctionPointerForDelegate(new SystemFuncSystemInt32_SystemSingle_SystemDoubleRemoveDelegate(SystemFuncSystemInt32_SystemSingle_SystemDoubleRemove)),
				Marshal.GetFunctionPointerForDelegate(new ReleaseSystemFuncSystemInt16_SystemInt32_SystemStringDelegate(ReleaseSystemFuncSystemInt16_SystemInt32_SystemString)),
				Marshal.GetFunctionPointerForDelegate(new SystemFuncSystemInt16_SystemInt32_SystemStringConstructorDelegate(SystemFuncSystemInt16_SystemInt32_SystemStringConstructor)),
				Marshal.GetFunctionPointerForDelegate(new SystemFuncSystemInt16_SystemInt32_SystemStringInvokeDelegate(SystemFuncSystemInt16_SystemInt32_SystemStringInvoke)),
				Marshal.GetFunctionPointerForDelegate(new SystemFuncSystemInt16_SystemInt32_SystemStringAddDelegate(SystemFuncSystemInt16_SystemInt32_SystemStringAdd)),
				Marshal.GetFunctionPointerForDelegate(new SystemFuncSystemInt16_SystemInt32_SystemStringRemoveDelegate(SystemFuncSystemInt16_SystemInt32_SystemStringRemove))
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
		
		[MonoPInvokeCallback(typeof(ArrayGetLengthDelegate))]
		static int ArrayGetLength(int handle)
		{
			return ((Array)ObjectStore.Get(handle)).Length;
		}
		
		[MonoPInvokeCallback(typeof(ArrayGetRankDelegate))]
		static int ArrayGetRank(int handle)
		{
			return ((Array)ObjectStore.Get(handle)).Rank;
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
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
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
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(long);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
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
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
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
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
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
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
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
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
		}
		
		[MonoPInvokeCallback(typeof(UnityEngineObjectMethodop_EqualityUnityEngineObject_UnityEngineObjectDelegate))]
		static bool UnityEngineObjectMethodop_EqualityUnityEngineObject_UnityEngineObject(int xHandle, int yHandle)
		{
			try
			{
				var x = (UnityEngine.Object)NativeScript.Bindings.ObjectStore.Get(xHandle);
				var y = (UnityEngine.Object)NativeScript.Bindings.ObjectStore.Get(yHandle);
				var returnValue = x == y;
				return returnValue;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(bool);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(bool);
			}
		}
		
		[MonoPInvokeCallback(typeof(UnityEngineObjectMethodop_ImplicitUnityEngineObjectDelegate))]
		static bool UnityEngineObjectMethodop_ImplicitUnityEngineObject(int existsHandle)
		{
			try
			{
				var exists = (UnityEngine.Object)NativeScript.Bindings.ObjectStore.Get(existsHandle);
				var returnValue = exists;
				return returnValue;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(bool);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(bool);
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
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
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
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
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
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
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
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
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
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
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
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(UnityEngine.Vector3);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
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
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
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
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
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
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(bool);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
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
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
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
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
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
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
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
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
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
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
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
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
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
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(UnityEngine.Vector3);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
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
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(float);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
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
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
		}
		
		[MonoPInvokeCallback(typeof(UnityEngineVector3Methodop_AdditionUnityEngineVector3_UnityEngineVector3Delegate))]
		static UnityEngine.Vector3 UnityEngineVector3Methodop_AdditionUnityEngineVector3_UnityEngineVector3(ref UnityEngine.Vector3 a, ref UnityEngine.Vector3 b)
		{
			try
			{
				var returnValue = a + b;
				return returnValue;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(UnityEngine.Vector3);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(UnityEngine.Vector3);
			}
		}
		
		[MonoPInvokeCallback(typeof(UnityEngineVector3Methodop_UnaryNegationUnityEngineVector3Delegate))]
		static UnityEngine.Vector3 UnityEngineVector3Methodop_UnaryNegationUnityEngineVector3(ref UnityEngine.Vector3 a)
		{
			try
			{
				var returnValue = -a;
				return returnValue;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(UnityEngine.Vector3);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(UnityEngine.Vector3);
			}
		}
		
		[MonoPInvokeCallback(typeof(UnityEngineMatrix4x4PropertyGetItemDelegate))]
		static float UnityEngineMatrix4x4PropertyGetItem(ref UnityEngine.Matrix4x4 thiz, int row, int column)
		{
			try
			{
				var returnValue = thiz[row, row];
				return returnValue;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(float);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(float);
			}
		}
		
		[MonoPInvokeCallback(typeof(UnityEngineMatrix4x4PropertySetItemDelegate))]
		static void UnityEngineMatrix4x4PropertySetItem(ref UnityEngine.Matrix4x4 thiz, int row, int column, float value)
		{
			try
			{
				thiz[row, column] = column;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
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
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
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
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(UnityEngine.Vector3);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
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
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
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
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
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
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
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
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
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
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
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
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(double);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
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
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
		}
		
		[MonoPInvokeCallback(typeof(SystemCollectionsGenericListSystemStringPropertyGetItemDelegate))]
		static int SystemCollectionsGenericListSystemStringPropertyGetItem(int thisHandle, int index)
		{
			try
			{
				var thiz = (System.Collections.Generic.List<string>)NativeScript.Bindings.ObjectStore.Get(thisHandle);
				var returnValue = thiz[index];
				return NativeScript.Bindings.ObjectStore.GetHandle(returnValue);
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
		}
		
		[MonoPInvokeCallback(typeof(SystemCollectionsGenericListSystemStringPropertySetItemDelegate))]
		static void SystemCollectionsGenericListSystemStringPropertySetItem(int thisHandle, int index, int valueHandle)
		{
			try
			{
				var thiz = (System.Collections.Generic.List<string>)NativeScript.Bindings.ObjectStore.Get(thisHandle);
				var value = (string)NativeScript.Bindings.ObjectStore.Get(valueHandle);
				thiz[index] = value;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
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
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
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
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
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
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
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
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
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
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
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
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
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
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
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
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
		}
		
		[MonoPInvokeCallback(typeof(UnityEngineResolutionPropertyGetWidthDelegate))]
		static int UnityEngineResolutionPropertyGetWidth(ref UnityEngine.Resolution thiz)
		{
			try
			{
				var returnValue = thiz.width;
				return returnValue;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
		}
		
		[MonoPInvokeCallback(typeof(UnityEngineResolutionPropertySetWidthDelegate))]
		static void UnityEngineResolutionPropertySetWidth(ref UnityEngine.Resolution thiz, int value)
		{
			try
			{
				thiz.width = value;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
		}
		
		[MonoPInvokeCallback(typeof(UnityEngineResolutionPropertyGetHeightDelegate))]
		static int UnityEngineResolutionPropertyGetHeight(ref UnityEngine.Resolution thiz)
		{
			try
			{
				var returnValue = thiz.height;
				return returnValue;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
		}
		
		[MonoPInvokeCallback(typeof(UnityEngineResolutionPropertySetHeightDelegate))]
		static void UnityEngineResolutionPropertySetHeight(ref UnityEngine.Resolution thiz, int value)
		{
			try
			{
				thiz.height = value;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
		}
		
		[MonoPInvokeCallback(typeof(UnityEngineResolutionPropertyGetRefreshRateDelegate))]
		static int UnityEngineResolutionPropertyGetRefreshRate(ref UnityEngine.Resolution thiz)
		{
			try
			{
				var returnValue = thiz.refreshRate;
				return returnValue;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
		}
		
		[MonoPInvokeCallback(typeof(UnityEngineResolutionPropertySetRefreshRateDelegate))]
		static void UnityEngineResolutionPropertySetRefreshRate(ref UnityEngine.Resolution thiz, int value)
		{
			try
			{
				thiz.refreshRate = value;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
		}
		
		[MonoPInvokeCallback(typeof(UnityEngineScreenPropertyGetResolutionsDelegate))]
		static int UnityEngineScreenPropertyGetResolutions()
		{
			try
			{
				var returnValue = UnityEngine.Screen.resolutions;
				return NativeScript.Bindings.ObjectStore.GetHandle(returnValue);
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
		}
		
		[MonoPInvokeCallback(typeof(UnityEngineRayConstructorUnityEngineVector3_UnityEngineVector3Delegate))]
		static UnityEngine.Ray UnityEngineRayConstructorUnityEngineVector3_UnityEngineVector3(ref UnityEngine.Vector3 origin, ref UnityEngine.Vector3 direction)
		{
			try
			{
				var returnValue = new UnityEngine.Ray(origin, direction);
				return returnValue;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(UnityEngine.Ray);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(UnityEngine.Ray);
			}
		}
		
		[MonoPInvokeCallback(typeof(UnityEnginePhysicsMethodRaycastNonAllocUnityEngineRay_UnityEngineRaycastHitDelegate))]
		static int UnityEnginePhysicsMethodRaycastNonAllocUnityEngineRay_UnityEngineRaycastHit(ref UnityEngine.Ray ray, int resultsHandle)
		{
			try
			{
				var results = (UnityEngine.RaycastHit[])NativeScript.Bindings.ObjectStore.Get(resultsHandle);
				var returnValue = UnityEngine.Physics.RaycastNonAlloc(ray, results);
				return returnValue;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
		}
		
		[MonoPInvokeCallback(typeof(UnityEnginePhysicsMethodRaycastAllUnityEngineRayDelegate))]
		static int UnityEnginePhysicsMethodRaycastAllUnityEngineRay(ref UnityEngine.Ray ray)
		{
			try
			{
				var returnValue = UnityEngine.Physics.RaycastAll(ray);
				return NativeScript.Bindings.ObjectStore.GetHandle(returnValue);
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
		}
		
		[MonoPInvokeCallback(typeof(UnityEngineGradientConstructorDelegate))]
		static int UnityEngineGradientConstructor()
		{
			try
			{
				var returnValue = NativeScript.Bindings.ObjectStore.Store(new UnityEngine.Gradient());
				return returnValue;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
		}
		
		[MonoPInvokeCallback(typeof(UnityEngineGradientPropertyGetColorKeysDelegate))]
		static int UnityEngineGradientPropertyGetColorKeys(int thisHandle)
		{
			try
			{
				var thiz = (UnityEngine.Gradient)NativeScript.Bindings.ObjectStore.Get(thisHandle);
				var returnValue = thiz.colorKeys;
				return NativeScript.Bindings.ObjectStore.GetHandle(returnValue);
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
		}
		
		[MonoPInvokeCallback(typeof(UnityEngineGradientPropertySetColorKeysDelegate))]
		static void UnityEngineGradientPropertySetColorKeys(int thisHandle, int valueHandle)
		{
			try
			{
				var thiz = (UnityEngine.Gradient)NativeScript.Bindings.ObjectStore.Get(thisHandle);
				var value = (UnityEngine.GradientColorKey[])NativeScript.Bindings.ObjectStore.Get(valueHandle);
				thiz.colorKeys = value;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
		}
		
		[MonoPInvokeCallback(typeof(SystemInt32Array1Constructor1Delegate))]
		static int SystemInt32Array1Constructor1(int length0)
		{
			try
			{
				var returnValue = NativeScript.Bindings.ObjectStore.Store(new int[length0]);
				return returnValue;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
		}
		
		[MonoPInvokeCallback(typeof(SystemInt32Array1GetItem1Delegate))]
		static int SystemInt32Array1GetItem1(int thisHandle, int index0)
		{
			try
			{
				var thiz = (int[])NativeScript.Bindings.ObjectStore.Get(thisHandle);
				var returnValue = thiz[index0];
				return returnValue;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
		}
		
		[MonoPInvokeCallback(typeof(SystemInt32Array1SetItem1Delegate))]
		static void SystemInt32Array1SetItem1(int thisHandle, int index0, int item)
		{
			try
			{
				var thiz = (int[])NativeScript.Bindings.ObjectStore.Get(thisHandle);
				thiz[index0] = item;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
		}
		
		[MonoPInvokeCallback(typeof(SystemSingleArray1Constructor1Delegate))]
		static int SystemSingleArray1Constructor1(int length0)
		{
			try
			{
				var returnValue = NativeScript.Bindings.ObjectStore.Store(new float[length0]);
				return returnValue;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
		}
		
		[MonoPInvokeCallback(typeof(SystemSingleArray1GetItem1Delegate))]
		static float SystemSingleArray1GetItem1(int thisHandle, int index0)
		{
			try
			{
				var thiz = (float[])NativeScript.Bindings.ObjectStore.Get(thisHandle);
				var returnValue = thiz[index0];
				return returnValue;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(float);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(float);
			}
		}
		
		[MonoPInvokeCallback(typeof(SystemSingleArray1SetItem1Delegate))]
		static void SystemSingleArray1SetItem1(int thisHandle, int index0, float item)
		{
			try
			{
				var thiz = (float[])NativeScript.Bindings.ObjectStore.Get(thisHandle);
				thiz[index0] = item;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
		}
		
		[MonoPInvokeCallback(typeof(SystemSingleArray2Constructor2Delegate))]
		static int SystemSingleArray2Constructor2(int length0, int length1)
		{
			try
			{
				var returnValue = NativeScript.Bindings.ObjectStore.Store(new float[length0, length1]);
				return returnValue;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
		}
		
		[MonoPInvokeCallback(typeof(SystemSingleArray2GetLength2Delegate))]
		static int SystemSingleArray2GetLength2(int thisHandle, int dimension)
		{
			try
			{
				var thiz = (float[,])NativeScript.Bindings.ObjectStore.Get(thisHandle);
				var returnValue = thiz.GetLength(dimension);
				return returnValue;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
		}
		
		[MonoPInvokeCallback(typeof(SystemSingleArray2GetItem2Delegate))]
		static float SystemSingleArray2GetItem2(int thisHandle, int index0, int index1)
		{
			try
			{
				var thiz = (float[,])NativeScript.Bindings.ObjectStore.Get(thisHandle);
				var returnValue = thiz[index0, index1];
				return returnValue;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(float);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(float);
			}
		}
		
		[MonoPInvokeCallback(typeof(SystemSingleArray2SetItem2Delegate))]
		static void SystemSingleArray2SetItem2(int thisHandle, int index0, int index1, float item)
		{
			try
			{
				var thiz = (float[,])NativeScript.Bindings.ObjectStore.Get(thisHandle);
				thiz[index0, index1] = item;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
		}
		
		[MonoPInvokeCallback(typeof(SystemSingleArray3Constructor3Delegate))]
		static int SystemSingleArray3Constructor3(int length0, int length1, int length2)
		{
			try
			{
				var returnValue = NativeScript.Bindings.ObjectStore.Store(new float[length0, length1, length2]);
				return returnValue;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
		}
		
		[MonoPInvokeCallback(typeof(SystemSingleArray3GetLength3Delegate))]
		static int SystemSingleArray3GetLength3(int thisHandle, int dimension)
		{
			try
			{
				var thiz = (float[,,])NativeScript.Bindings.ObjectStore.Get(thisHandle);
				var returnValue = thiz.GetLength(dimension);
				return returnValue;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
		}
		
		[MonoPInvokeCallback(typeof(SystemSingleArray3GetItem3Delegate))]
		static float SystemSingleArray3GetItem3(int thisHandle, int index0, int index1, int index2)
		{
			try
			{
				var thiz = (float[,,])NativeScript.Bindings.ObjectStore.Get(thisHandle);
				var returnValue = thiz[index0, index1, index2];
				return returnValue;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(float);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(float);
			}
		}
		
		[MonoPInvokeCallback(typeof(SystemSingleArray3SetItem3Delegate))]
		static void SystemSingleArray3SetItem3(int thisHandle, int index0, int index1, int index2, float item)
		{
			try
			{
				var thiz = (float[,,])NativeScript.Bindings.ObjectStore.Get(thisHandle);
				thiz[index0, index1, index2] = item;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
		}
		
		[MonoPInvokeCallback(typeof(SystemStringArray1Constructor1Delegate))]
		static int SystemStringArray1Constructor1(int length0)
		{
			try
			{
				var returnValue = NativeScript.Bindings.ObjectStore.Store(new string[length0]);
				return returnValue;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
		}
		
		[MonoPInvokeCallback(typeof(SystemStringArray1GetItem1Delegate))]
		static int SystemStringArray1GetItem1(int thisHandle, int index0)
		{
			try
			{
				var thiz = (string[])NativeScript.Bindings.ObjectStore.Get(thisHandle);
				var returnValue = thiz[index0];
				return NativeScript.Bindings.ObjectStore.GetHandle(returnValue);
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
		}
		
		[MonoPInvokeCallback(typeof(SystemStringArray1SetItem1Delegate))]
		static void SystemStringArray1SetItem1(int thisHandle, int index0, int itemHandle)
		{
			try
			{
				var thiz = (string[])NativeScript.Bindings.ObjectStore.Get(thisHandle);
				var item = (string)NativeScript.Bindings.ObjectStore.Get(itemHandle);
				thiz[index0] = item;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
		}
		
		[MonoPInvokeCallback(typeof(UnityEngineResolutionArray1Constructor1Delegate))]
		static int UnityEngineResolutionArray1Constructor1(int length0)
		{
			try
			{
				var returnValue = NativeScript.Bindings.ObjectStore.Store(new UnityEngine.Resolution[length0]);
				return returnValue;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
		}
		
		[MonoPInvokeCallback(typeof(UnityEngineResolutionArray1GetItem1Delegate))]
		static UnityEngine.Resolution UnityEngineResolutionArray1GetItem1(int thisHandle, int index0)
		{
			try
			{
				var thiz = (UnityEngine.Resolution[])NativeScript.Bindings.ObjectStore.Get(thisHandle);
				var returnValue = thiz[index0];
				return returnValue;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(UnityEngine.Resolution);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(UnityEngine.Resolution);
			}
		}
		
		[MonoPInvokeCallback(typeof(UnityEngineResolutionArray1SetItem1Delegate))]
		static void UnityEngineResolutionArray1SetItem1(int thisHandle, int index0, ref UnityEngine.Resolution item)
		{
			try
			{
				var thiz = (UnityEngine.Resolution[])NativeScript.Bindings.ObjectStore.Get(thisHandle);
				thiz[index0] = item;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
		}
		
		[MonoPInvokeCallback(typeof(UnityEngineRaycastHitArray1Constructor1Delegate))]
		static int UnityEngineRaycastHitArray1Constructor1(int length0)
		{
			try
			{
				var returnValue = NativeScript.Bindings.ObjectStore.Store(new UnityEngine.RaycastHit[length0]);
				return returnValue;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
		}
		
		[MonoPInvokeCallback(typeof(UnityEngineRaycastHitArray1GetItem1Delegate))]
		static int UnityEngineRaycastHitArray1GetItem1(int thisHandle, int index0)
		{
			try
			{
				var thiz = (UnityEngine.RaycastHit[])NativeScript.Bindings.ObjectStore.Get(thisHandle);
				var returnValue = thiz[index0];
				return NativeScript.Bindings.StructStore<UnityEngine.RaycastHit>.Store(returnValue);
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
		}
		
		[MonoPInvokeCallback(typeof(UnityEngineRaycastHitArray1SetItem1Delegate))]
		static void UnityEngineRaycastHitArray1SetItem1(int thisHandle, int index0, int itemHandle)
		{
			try
			{
				var thiz = (UnityEngine.RaycastHit[])NativeScript.Bindings.ObjectStore.Get(thisHandle);
				var item = (UnityEngine.RaycastHit)NativeScript.Bindings.StructStore<UnityEngine.RaycastHit>.Get(itemHandle);
				thiz[index0] = item;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
		}
		
		[MonoPInvokeCallback(typeof(UnityEngineGradientColorKeyArray1Constructor1Delegate))]
		static int UnityEngineGradientColorKeyArray1Constructor1(int length0)
		{
			try
			{
				var returnValue = NativeScript.Bindings.ObjectStore.Store(new UnityEngine.GradientColorKey[length0]);
				return returnValue;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
		}
		
		[MonoPInvokeCallback(typeof(UnityEngineGradientColorKeyArray1GetItem1Delegate))]
		static UnityEngine.GradientColorKey UnityEngineGradientColorKeyArray1GetItem1(int thisHandle, int index0)
		{
			try
			{
				var thiz = (UnityEngine.GradientColorKey[])NativeScript.Bindings.ObjectStore.Get(thisHandle);
				var returnValue = thiz[index0];
				return returnValue;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(UnityEngine.GradientColorKey);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(UnityEngine.GradientColorKey);
			}
		}
		
		[MonoPInvokeCallback(typeof(UnityEngineGradientColorKeyArray1SetItem1Delegate))]
		static void UnityEngineGradientColorKeyArray1SetItem1(int thisHandle, int index0, ref UnityEngine.GradientColorKey item)
		{
			try
			{
				var thiz = (UnityEngine.GradientColorKey[])NativeScript.Bindings.ObjectStore.Get(thisHandle);
				thiz[index0] = item;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
		}
		
		class SystemAction
		{
			public int CppHandle;
			public System.Action Delegate;
			
			public SystemAction(int cppHandle)
			{
				CppHandle = cppHandle;
				Delegate = Invoke;
			}			
			public void Invoke()
			{
				if (CppHandle != 0)
				{
					SystemActionCppInvoke(CppHandle);
				}
			}
		}
		
		[MonoPInvokeCallback(typeof(SystemActionConstructorDelegate))]
		static void SystemActionConstructor(int cppHandle, ref int handle, ref int delegateHandle)
		{
			try
			{
				var thiz = new SystemAction(cppHandle);
				handle = NativeScript.Bindings.ObjectStore.Store(thiz);
				delegateHandle = NativeScript.Bindings.ObjectStore.Store(thiz.Delegate);
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
		}
		
		[MonoPInvokeCallback(typeof(ReleaseSystemActionDelegate))]
		static void ReleaseSystemAction(int handle, int delegateHandle)
		{
			try
			{
				var thiz = (SystemAction)NativeScript.Bindings.ObjectStore.Remove(handle);
				thiz.CppHandle = 0;
				NativeScript.Bindings.ObjectStore.Remove(delegateHandle);
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
		}
		
		[MonoPInvokeCallback(typeof(SystemActionInvokeDelegate))]
		static void SystemActionInvoke(int thisHandle)
		{
			try
			{
				((SystemAction)NativeScript.Bindings.ObjectStore.Get(thisHandle)).Delegate();

			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
		}
		
		[MonoPInvokeCallback(typeof(SystemActionAddDelegate))]
		static void SystemActionAdd(int thisHandle, int delHandle)
		{
			try
			{
				var del = (System.Action)NativeScript.Bindings.ObjectStore.Get(delHandle);
				var thiz = (SystemAction)NativeScript.Bindings.ObjectStore.Get(thisHandle);
				thiz.Delegate += del;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
		}
		
		[MonoPInvokeCallback(typeof(SystemActionRemoveDelegate))]
		static void SystemActionRemove(int thisHandle, int delHandle)
		{
			try
			{
				var del = (System.Action)NativeScript.Bindings.ObjectStore.Get(delHandle);
				var thiz = (SystemAction)NativeScript.Bindings.ObjectStore.Get(thisHandle);
				thiz.Delegate -= del;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
		}
		
		class SystemActionSystemSingle
		{
			public int CppHandle;
			public System.Action<float> Delegate;
			
			public SystemActionSystemSingle(int cppHandle)
			{
				CppHandle = cppHandle;
				Delegate = Invoke;
			}			
			public void Invoke(float obj)
			{
				if (CppHandle != 0)
				{
					SystemActionSystemSingleCppInvoke(CppHandle, obj);
				}
			}
		}
		
		[MonoPInvokeCallback(typeof(SystemActionSystemSingleConstructorDelegate))]
		static void SystemActionSystemSingleConstructor(int cppHandle, ref int handle, ref int delegateHandle)
		{
			try
			{
				var thiz = new SystemActionSystemSingle(cppHandle);
				handle = NativeScript.Bindings.ObjectStore.Store(thiz);
				delegateHandle = NativeScript.Bindings.ObjectStore.Store(thiz.Delegate);
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
		}
		
		[MonoPInvokeCallback(typeof(ReleaseSystemActionSystemSingleDelegate))]
		static void ReleaseSystemActionSystemSingle(int handle, int delegateHandle)
		{
			try
			{
				var thiz = (SystemActionSystemSingle)NativeScript.Bindings.ObjectStore.Remove(handle);
				thiz.CppHandle = 0;
				NativeScript.Bindings.ObjectStore.Remove(delegateHandle);
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
		}
		
		[MonoPInvokeCallback(typeof(SystemActionSystemSingleInvokeDelegate))]
		static void SystemActionSystemSingleInvoke(int thisHandle, float obj)
		{
			try
			{
				((SystemActionSystemSingle)NativeScript.Bindings.ObjectStore.Get(thisHandle)).Delegate(obj);

			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
		}
		
		[MonoPInvokeCallback(typeof(SystemActionSystemSingleAddDelegate))]
		static void SystemActionSystemSingleAdd(int thisHandle, int delHandle)
		{
			try
			{
				var del = (System.Action<float>)NativeScript.Bindings.ObjectStore.Get(delHandle);
				var thiz = (SystemActionSystemSingle)NativeScript.Bindings.ObjectStore.Get(thisHandle);
				thiz.Delegate += del;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
		}
		
		[MonoPInvokeCallback(typeof(SystemActionSystemSingleRemoveDelegate))]
		static void SystemActionSystemSingleRemove(int thisHandle, int delHandle)
		{
			try
			{
				var del = (System.Action<float>)NativeScript.Bindings.ObjectStore.Get(delHandle);
				var thiz = (SystemActionSystemSingle)NativeScript.Bindings.ObjectStore.Get(thisHandle);
				thiz.Delegate -= del;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
		}
		
		class SystemActionSystemSingle_SystemSingle
		{
			public int CppHandle;
			public System.Action<float, float> Delegate;
			
			public SystemActionSystemSingle_SystemSingle(int cppHandle)
			{
				CppHandle = cppHandle;
				Delegate = Invoke;
			}			
			public void Invoke(float arg1, float arg2)
			{
				if (CppHandle != 0)
				{
					SystemActionSystemSingle_SystemSingleCppInvoke(CppHandle, arg1, arg2);
				}
			}
		}
		
		[MonoPInvokeCallback(typeof(SystemActionSystemSingle_SystemSingleConstructorDelegate))]
		static void SystemActionSystemSingle_SystemSingleConstructor(int cppHandle, ref int handle, ref int delegateHandle)
		{
			try
			{
				var thiz = new SystemActionSystemSingle_SystemSingle(cppHandle);
				handle = NativeScript.Bindings.ObjectStore.Store(thiz);
				delegateHandle = NativeScript.Bindings.ObjectStore.Store(thiz.Delegate);
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
		}
		
		[MonoPInvokeCallback(typeof(ReleaseSystemActionSystemSingle_SystemSingleDelegate))]
		static void ReleaseSystemActionSystemSingle_SystemSingle(int handle, int delegateHandle)
		{
			try
			{
				var thiz = (SystemActionSystemSingle_SystemSingle)NativeScript.Bindings.ObjectStore.Remove(handle);
				thiz.CppHandle = 0;
				NativeScript.Bindings.ObjectStore.Remove(delegateHandle);
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
		}
		
		[MonoPInvokeCallback(typeof(SystemActionSystemSingle_SystemSingleInvokeDelegate))]
		static void SystemActionSystemSingle_SystemSingleInvoke(int thisHandle, float arg1, float arg2)
		{
			try
			{
				((SystemActionSystemSingle_SystemSingle)NativeScript.Bindings.ObjectStore.Get(thisHandle)).Delegate(arg1, arg2);

			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
		}
		
		[MonoPInvokeCallback(typeof(SystemActionSystemSingle_SystemSingleAddDelegate))]
		static void SystemActionSystemSingle_SystemSingleAdd(int thisHandle, int delHandle)
		{
			try
			{
				var del = (System.Action<float, float>)NativeScript.Bindings.ObjectStore.Get(delHandle);
				var thiz = (SystemActionSystemSingle_SystemSingle)NativeScript.Bindings.ObjectStore.Get(thisHandle);
				thiz.Delegate += del;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
		}
		
		[MonoPInvokeCallback(typeof(SystemActionSystemSingle_SystemSingleRemoveDelegate))]
		static void SystemActionSystemSingle_SystemSingleRemove(int thisHandle, int delHandle)
		{
			try
			{
				var del = (System.Action<float, float>)NativeScript.Bindings.ObjectStore.Get(delHandle);
				var thiz = (SystemActionSystemSingle_SystemSingle)NativeScript.Bindings.ObjectStore.Get(thisHandle);
				thiz.Delegate -= del;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
		}
		
		class SystemFuncSystemInt32_SystemSingle_SystemDouble
		{
			public int CppHandle;
			public System.Func<int, float, double> Delegate;
			
			public SystemFuncSystemInt32_SystemSingle_SystemDouble(int cppHandle)
			{
				CppHandle = cppHandle;
				Delegate = Invoke;
			}			
			public double Invoke(int arg1, float arg2)
			{
				if (CppHandle != 0)
				{
					return SystemFuncSystemInt32_SystemSingle_SystemDoubleCppInvoke(CppHandle, arg1, arg2);
				}
				else
				{
					return default(double);
				}
			}
		}
		
		[MonoPInvokeCallback(typeof(SystemFuncSystemInt32_SystemSingle_SystemDoubleConstructorDelegate))]
		static void SystemFuncSystemInt32_SystemSingle_SystemDoubleConstructor(int cppHandle, ref int handle, ref int delegateHandle)
		{
			try
			{
				var thiz = new SystemFuncSystemInt32_SystemSingle_SystemDouble(cppHandle);
				handle = NativeScript.Bindings.ObjectStore.Store(thiz);
				delegateHandle = NativeScript.Bindings.ObjectStore.Store(thiz.Delegate);
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
		}
		
		[MonoPInvokeCallback(typeof(ReleaseSystemFuncSystemInt32_SystemSingle_SystemDoubleDelegate))]
		static void ReleaseSystemFuncSystemInt32_SystemSingle_SystemDouble(int handle, int delegateHandle)
		{
			try
			{
				var thiz = (SystemFuncSystemInt32_SystemSingle_SystemDouble)NativeScript.Bindings.ObjectStore.Remove(handle);
				thiz.CppHandle = 0;
				NativeScript.Bindings.ObjectStore.Remove(delegateHandle);
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
		}
		
		[MonoPInvokeCallback(typeof(SystemFuncSystemInt32_SystemSingle_SystemDoubleInvokeDelegate))]
		static double SystemFuncSystemInt32_SystemSingle_SystemDoubleInvoke(int thisHandle, int arg1, float arg2)
		{
			try
			{
				var returnValue = ((SystemFuncSystemInt32_SystemSingle_SystemDouble)NativeScript.Bindings.ObjectStore.Get(thisHandle)).Delegate(arg1, arg2);

				return returnValue;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(double);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(double);
			}
		}
		
		[MonoPInvokeCallback(typeof(SystemFuncSystemInt32_SystemSingle_SystemDoubleAddDelegate))]
		static void SystemFuncSystemInt32_SystemSingle_SystemDoubleAdd(int thisHandle, int delHandle)
		{
			try
			{
				var del = (System.Func<int, float, double>)NativeScript.Bindings.ObjectStore.Get(delHandle);
				var thiz = (SystemFuncSystemInt32_SystemSingle_SystemDouble)NativeScript.Bindings.ObjectStore.Get(thisHandle);
				thiz.Delegate += del;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
		}
		
		[MonoPInvokeCallback(typeof(SystemFuncSystemInt32_SystemSingle_SystemDoubleRemoveDelegate))]
		static void SystemFuncSystemInt32_SystemSingle_SystemDoubleRemove(int thisHandle, int delHandle)
		{
			try
			{
				var del = (System.Func<int, float, double>)NativeScript.Bindings.ObjectStore.Get(delHandle);
				var thiz = (SystemFuncSystemInt32_SystemSingle_SystemDouble)NativeScript.Bindings.ObjectStore.Get(thisHandle);
				thiz.Delegate -= del;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
		}
		
		class SystemFuncSystemInt16_SystemInt32_SystemString
		{
			public int CppHandle;
			public System.Func<short, int, string> Delegate;
			
			public SystemFuncSystemInt16_SystemInt32_SystemString(int cppHandle)
			{
				CppHandle = cppHandle;
				Delegate = Invoke;
			}			
			public string Invoke(short arg1, int arg2)
			{
				if (CppHandle != 0)
				{
					return (string)NativeScript.Bindings.ObjectStore.Get(SystemFuncSystemInt16_SystemInt32_SystemStringCppInvoke(CppHandle, arg1, arg2));
				}
				else
				{
					return default(string);
				}
			}
		}
		
		[MonoPInvokeCallback(typeof(SystemFuncSystemInt16_SystemInt32_SystemStringConstructorDelegate))]
		static void SystemFuncSystemInt16_SystemInt32_SystemStringConstructor(int cppHandle, ref int handle, ref int delegateHandle)
		{
			try
			{
				var thiz = new SystemFuncSystemInt16_SystemInt32_SystemString(cppHandle);
				handle = NativeScript.Bindings.ObjectStore.Store(thiz);
				delegateHandle = NativeScript.Bindings.ObjectStore.Store(thiz.Delegate);
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
		}
		
		[MonoPInvokeCallback(typeof(ReleaseSystemFuncSystemInt16_SystemInt32_SystemStringDelegate))]
		static void ReleaseSystemFuncSystemInt16_SystemInt32_SystemString(int handle, int delegateHandle)
		{
			try
			{
				var thiz = (SystemFuncSystemInt16_SystemInt32_SystemString)NativeScript.Bindings.ObjectStore.Remove(handle);
				thiz.CppHandle = 0;
				NativeScript.Bindings.ObjectStore.Remove(delegateHandle);
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
		}
		
		[MonoPInvokeCallback(typeof(SystemFuncSystemInt16_SystemInt32_SystemStringInvokeDelegate))]
		static int SystemFuncSystemInt16_SystemInt32_SystemStringInvoke(int thisHandle, short arg1, int arg2)
		{
			try
			{
				var returnValue = ((SystemFuncSystemInt16_SystemInt32_SystemString)NativeScript.Bindings.ObjectStore.Get(thisHandle)).Delegate(arg1, arg2);

				return NativeScript.Bindings.ObjectStore.GetHandle(returnValue);
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(int);
			}
		}
		
		[MonoPInvokeCallback(typeof(SystemFuncSystemInt16_SystemInt32_SystemStringAddDelegate))]
		static void SystemFuncSystemInt16_SystemInt32_SystemStringAdd(int thisHandle, int delHandle)
		{
			try
			{
				var del = (System.Func<short, int, string>)NativeScript.Bindings.ObjectStore.Get(delHandle);
				var thiz = (SystemFuncSystemInt16_SystemInt32_SystemString)NativeScript.Bindings.ObjectStore.Get(thisHandle);
				thiz.Delegate += del;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
		}
		
		[MonoPInvokeCallback(typeof(SystemFuncSystemInt16_SystemInt32_SystemStringRemoveDelegate))]
		static void SystemFuncSystemInt16_SystemInt32_SystemStringRemove(int thisHandle, int delHandle)
		{
			try
			{
				var del = (System.Func<short, int, string>)NativeScript.Bindings.ObjectStore.Get(delHandle);
				var thiz = (SystemFuncSystemInt16_SystemInt32_SystemString)NativeScript.Bindings.ObjectStore.Get(thisHandle);
				thiz.Delegate -= del;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
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
				NativeScript.Bindings.MyGameMonoBehavioursTestScriptAwake(thisHandle);
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
				NativeScript.Bindings.MyGameMonoBehavioursTestScriptOnAnimatorIK(thisHandle, param0);
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
				NativeScript.Bindings.MyGameMonoBehavioursTestScriptOnCollisionEnter(thisHandle, param0Handle);
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
				NativeScript.Bindings.MyGameMonoBehavioursTestScriptUpdate(thisHandle);
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
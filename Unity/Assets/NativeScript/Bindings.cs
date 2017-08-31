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
			/*BEGIN INIT PARAMS*/
			IntPtr stopwatchConstructor,
			IntPtr stopwatchPropertyGetElapsedMilliseconds,
			IntPtr stopwatchMethodStart,
			IntPtr stopwatchMethodReset,
			IntPtr objectPropertyGetName,
			IntPtr objectPropertySetName,
			IntPtr gameObjectConstructor,
			IntPtr gameObjectConstructorSystemString,
			IntPtr gameObjectPropertyGetTransform,
			IntPtr gameObjectMethodFindSystemString,
			IntPtr gameObjectMethodAddComponentMyGameMonoBehavioursTestScript,
			IntPtr componentPropertyGetTransform,
			IntPtr transformPropertyGetPosition,
			IntPtr transformPropertySetPosition,
			IntPtr debugMethodLogSystemObject,
			IntPtr assertFieldGetRaiseExceptions,
			IntPtr assertFieldSetRaiseExceptions,
			IntPtr audioSettingsMethodGetDSPBufferSizeSystemInt32_SystemInt32,
			IntPtr networkTransportMethodGetBroadcastConnectionInfoSystemInt32_SystemString_SystemInt32_SystemByte,
			IntPtr networkTransportMethodInit
			/*END INIT PARAMS*/);

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
			/*BEGIN INIT PARAMS*/
			IntPtr stopwatchConstructor,
			IntPtr stopwatchPropertyGetElapsedMilliseconds,
			IntPtr stopwatchMethodStart,
			IntPtr stopwatchMethodReset,
			IntPtr objectPropertyGetName,
			IntPtr objectPropertySetName,
			IntPtr gameObjectConstructor,
			IntPtr gameObjectConstructorSystemString,
			IntPtr gameObjectPropertyGetTransform,
			IntPtr gameObjectMethodFindSystemString,
			IntPtr gameObjectMethodAddComponentMyGameMonoBehavioursTestScript,
			IntPtr componentPropertyGetTransform,
			IntPtr transformPropertyGetPosition,
			IntPtr transformPropertySetPosition,
			IntPtr debugMethodLogSystemObject,
			IntPtr assertFieldGetRaiseExceptions,
			IntPtr assertFieldSetRaiseExceptions,
			IntPtr audioSettingsMethodGetDSPBufferSizeSystemInt32_SystemInt32,
			IntPtr networkTransportMethodGetBroadcastConnectionInfoSystemInt32_SystemString_SystemInt32_SystemByte,
			IntPtr networkTransportMethodInit
			/*END INIT PARAMS*/);
		
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
		
		/*BEGIN DELEGATE TYPES*/
		delegate int StopwatchConstructorDelegate();
		delegate long StopwatchPropertyGetElapsedMillisecondsDelegate(int thisHandle);
		delegate void StopwatchMethodStartDelegate(int thisHandle);
		delegate void StopwatchMethodResetDelegate(int thisHandle);
		delegate int ObjectPropertyGetNameDelegate(int thisHandle);
		delegate void ObjectPropertySetNameDelegate(int thisHandle, int valueHandle);
		delegate int GameObjectConstructorDelegate();
		delegate int GameObjectConstructorSystemStringDelegate(int nameHandle);
		delegate int GameObjectPropertyGetTransformDelegate(int thisHandle);
		delegate int GameObjectMethodFindSystemStringDelegate(int nameHandle);
		delegate int GameObjectMethodAddComponentMyGameMonoBehavioursTestScriptDelegate(int thisHandle);
		delegate int ComponentPropertyGetTransformDelegate(int thisHandle);
		delegate UnityEngine.Vector3 TransformPropertyGetPositionDelegate(int thisHandle);
		delegate void TransformPropertySetPositionDelegate(int thisHandle, UnityEngine.Vector3 value);
		delegate void DebugMethodLogSystemObjectDelegate(int messageHandle);
		delegate bool AssertFieldGetRaiseExceptionsDelegate();
		delegate void AssertFieldSetRaiseExceptionsDelegate(bool value);
		delegate void AudioSettingsMethodGetDSPBufferSizeSystemInt32_SystemInt32Delegate(out int bufferLength, out int numBuffers);
		delegate void NetworkTransportMethodGetBroadcastConnectionInfoSystemInt32_SystemString_SystemInt32_SystemByteDelegate(int hostId, ref int addressHandle, out int port, out byte error);
		delegate void NetworkTransportMethodInitDelegate();
		/*END DELEGATE TYPES*/
		
		// Stored objects. The first is always null.
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
		
		public static int StoreObject(object obj)
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
		
		public static object GetObject(int handle)
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
            return -1;
		}
		
		public static void RemoveObject(int handle)
		{
			if (handle != 0)
			{
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
			Bindings.maxObjects = maxManagedObjects;
			
			// Initialize the objects as all null plus room for the
			// first to always be null.
			objects = new object[maxManagedObjects + 1];

			// Initialize the handles stack as 1, 2, 3, ...
			handles = new int[maxManagedObjects];
			for (
				int i = 0, handle = maxManagedObjects;
				i < maxManagedObjects;
				++i, --handle)
			{
				handles[i] = handle;
			}
			nextHandleIndex = maxManagedObjects - 1;
			
			// Initialize the hash table
			keys = new object[maxManagedObjects];
			values = new int[maxManagedObjects];
			
#if UNITY_EDITOR

			// Open native library
			libraryHandle = OpenLibrary(
				Application.dataPath + PluginPath);
			InitDelegate Init = GetDelegate<InitDelegate>(
				libraryHandle,
				"Init");
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
				/*BEGIN INIT CALL*/
				Marshal.GetFunctionPointerForDelegate(new StopwatchConstructorDelegate(StopwatchConstructor)),
				Marshal.GetFunctionPointerForDelegate(new StopwatchPropertyGetElapsedMillisecondsDelegate(StopwatchPropertyGetElapsedMilliseconds)),
				Marshal.GetFunctionPointerForDelegate(new StopwatchMethodStartDelegate(StopwatchMethodStart)),
				Marshal.GetFunctionPointerForDelegate(new StopwatchMethodResetDelegate(StopwatchMethodReset)),
				Marshal.GetFunctionPointerForDelegate(new ObjectPropertyGetNameDelegate(ObjectPropertyGetName)),
				Marshal.GetFunctionPointerForDelegate(new ObjectPropertySetNameDelegate(ObjectPropertySetName)),
				Marshal.GetFunctionPointerForDelegate(new GameObjectConstructorDelegate(GameObjectConstructor)),
				Marshal.GetFunctionPointerForDelegate(new GameObjectConstructorSystemStringDelegate(GameObjectConstructorSystemString)),
				Marshal.GetFunctionPointerForDelegate(new GameObjectPropertyGetTransformDelegate(GameObjectPropertyGetTransform)),
				Marshal.GetFunctionPointerForDelegate(new GameObjectMethodFindSystemStringDelegate(GameObjectMethodFindSystemString)),
				Marshal.GetFunctionPointerForDelegate(new GameObjectMethodAddComponentMyGameMonoBehavioursTestScriptDelegate(GameObjectMethodAddComponentMyGameMonoBehavioursTestScript)),
				Marshal.GetFunctionPointerForDelegate(new ComponentPropertyGetTransformDelegate(ComponentPropertyGetTransform)),
				Marshal.GetFunctionPointerForDelegate(new TransformPropertyGetPositionDelegate(TransformPropertyGetPosition)),
				Marshal.GetFunctionPointerForDelegate(new TransformPropertySetPositionDelegate(TransformPropertySetPosition)),
				Marshal.GetFunctionPointerForDelegate(new DebugMethodLogSystemObjectDelegate(DebugMethodLogSystemObject)),
				Marshal.GetFunctionPointerForDelegate(new AssertFieldGetRaiseExceptionsDelegate(AssertFieldGetRaiseExceptions)),
				Marshal.GetFunctionPointerForDelegate(new AssertFieldSetRaiseExceptionsDelegate(AssertFieldSetRaiseExceptions)),
				Marshal.GetFunctionPointerForDelegate(new AudioSettingsMethodGetDSPBufferSizeSystemInt32_SystemInt32Delegate(AudioSettingsMethodGetDSPBufferSizeSystemInt32_SystemInt32)),
				Marshal.GetFunctionPointerForDelegate(new NetworkTransportMethodGetBroadcastConnectionInfoSystemInt32_SystemString_SystemInt32_SystemByteDelegate(NetworkTransportMethodGetBroadcastConnectionInfoSystemInt32_SystemString_SystemInt32_SystemByte)),
				Marshal.GetFunctionPointerForDelegate(new NetworkTransportMethodInitDelegate(NetworkTransportMethodInit))
				/*END INIT CALL*/
				);
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
				NativeScript.Bindings.RemoveObject(handle);
			}
		}
		
		[MonoPInvokeCallback(typeof(StringNewDelegate))]
		static int StringNew(
			string chars)
		{
			int handle = NativeScript.Bindings.StoreObject(chars);
			return handle;
		}
		
		/*BEGIN FUNCTIONS*/
		[MonoPInvokeCallback(typeof(StopwatchConstructorDelegate))]
		static int StopwatchConstructor()
		{
			var returnValue = NativeScript.Bindings.StoreObject(new System.Diagnostics.Stopwatch());
			return returnValue;
		}
		
		[MonoPInvokeCallback(typeof(StopwatchPropertyGetElapsedMillisecondsDelegate))]
		static long StopwatchPropertyGetElapsedMilliseconds(int thisHandle)
		{
			var thiz = (System.Diagnostics.Stopwatch)NativeScript.Bindings.GetObject(thisHandle);
			var returnValue = thiz.ElapsedMilliseconds;
			return returnValue;
		}
		
		[MonoPInvokeCallback(typeof(StopwatchMethodStartDelegate))]
		static void StopwatchMethodStart(int thisHandle)
		{
			var thiz = (System.Diagnostics.Stopwatch)NativeScript.Bindings.GetObject(thisHandle);
			thiz.Start();
		}
		
		[MonoPInvokeCallback(typeof(StopwatchMethodResetDelegate))]
		static void StopwatchMethodReset(int thisHandle)
		{
			var thiz = (System.Diagnostics.Stopwatch)NativeScript.Bindings.GetObject(thisHandle);
			thiz.Reset();
		}
		
		[MonoPInvokeCallback(typeof(ObjectPropertyGetNameDelegate))]
		static int ObjectPropertyGetName(int thisHandle)
		{
			var thiz = (UnityEngine.Object)NativeScript.Bindings.GetObject(thisHandle);
			var returnValue = thiz.name;
			int returnValueHandle = NativeScript.Bindings.GetHandle(returnValue);
			if (returnValueHandle < 0)
			{
				return NativeScript.Bindings.StoreObject(returnValue);
			}
			else
			{
				return returnValueHandle;
			}
		}
		
		[MonoPInvokeCallback(typeof(ObjectPropertySetNameDelegate))]
		static void ObjectPropertySetName(int thisHandle, int valueHandle)
		{
			var thiz = (UnityEngine.Object)NativeScript.Bindings.GetObject(thisHandle);
			var value = (System.String)NativeScript.Bindings.GetObject(valueHandle);
			thiz.name = value;
		}
		
		[MonoPInvokeCallback(typeof(GameObjectConstructorDelegate))]
		static int GameObjectConstructor()
		{
			var returnValue = NativeScript.Bindings.StoreObject(new UnityEngine.GameObject());
			return returnValue;
		}
		
		[MonoPInvokeCallback(typeof(GameObjectConstructorSystemStringDelegate))]
		static int GameObjectConstructorSystemString(int nameHandle)
		{
			var name = (System.String)NativeScript.Bindings.GetObject(nameHandle);
			var returnValue = NativeScript.Bindings.StoreObject(new UnityEngine.GameObject(name));
			return returnValue;
		}
		
		[MonoPInvokeCallback(typeof(GameObjectPropertyGetTransformDelegate))]
		static int GameObjectPropertyGetTransform(int thisHandle)
		{
			var thiz = (UnityEngine.GameObject)NativeScript.Bindings.GetObject(thisHandle);
			var returnValue = thiz.transform;
			int returnValueHandle = NativeScript.Bindings.GetHandle(returnValue);
			if (returnValueHandle < 0)
			{
				return NativeScript.Bindings.StoreObject(returnValue);
			}
			else
			{
				return returnValueHandle;
			}
		}
		
		[MonoPInvokeCallback(typeof(GameObjectMethodFindSystemStringDelegate))]
		static int GameObjectMethodFindSystemString(int nameHandle)
		{
			var name = (System.String)NativeScript.Bindings.GetObject(nameHandle);
			var returnValue = UnityEngine.GameObject.Find(name);
			int returnValueHandle = NativeScript.Bindings.GetHandle(returnValue);
			if (returnValueHandle < 0)
			{
				return NativeScript.Bindings.StoreObject(returnValue);
			}
			else
			{
				return returnValueHandle;
			}
		}
		
		[MonoPInvokeCallback(typeof(GameObjectMethodAddComponentMyGameMonoBehavioursTestScriptDelegate))]
		static int GameObjectMethodAddComponentMyGameMonoBehavioursTestScript(int thisHandle)
		{
			var thiz = (UnityEngine.GameObject)NativeScript.Bindings.GetObject(thisHandle);
			var returnValue = thiz.AddComponent<MyGame.MonoBehaviours.TestScript>();
			int returnValueHandle = NativeScript.Bindings.GetHandle(returnValue);
			if (returnValueHandle < 0)
			{
				return NativeScript.Bindings.StoreObject(returnValue);
			}
			else
			{
				return returnValueHandle;
			}
		}
		
		[MonoPInvokeCallback(typeof(ComponentPropertyGetTransformDelegate))]
		static int ComponentPropertyGetTransform(int thisHandle)
		{
			var thiz = (UnityEngine.Component)NativeScript.Bindings.GetObject(thisHandle);
			var returnValue = thiz.transform;
			int returnValueHandle = NativeScript.Bindings.GetHandle(returnValue);
			if (returnValueHandle < 0)
			{
				return NativeScript.Bindings.StoreObject(returnValue);
			}
			else
			{
				return returnValueHandle;
			}
		}
		
		[MonoPInvokeCallback(typeof(TransformPropertyGetPositionDelegate))]
		static UnityEngine.Vector3 TransformPropertyGetPosition(int thisHandle)
		{
			var thiz = (UnityEngine.Transform)NativeScript.Bindings.GetObject(thisHandle);
			var returnValue = thiz.position;
			return returnValue;
		}
		
		[MonoPInvokeCallback(typeof(TransformPropertySetPositionDelegate))]
		static void TransformPropertySetPosition(int thisHandle, UnityEngine.Vector3 value)
		{
			var thiz = (UnityEngine.Transform)NativeScript.Bindings.GetObject(thisHandle);
			thiz.position = value;
		}
		
		[MonoPInvokeCallback(typeof(DebugMethodLogSystemObjectDelegate))]
		static void DebugMethodLogSystemObject(int messageHandle)
		{
			var message = NativeScript.Bindings.GetObject(messageHandle);
			UnityEngine.Debug.Log(message);
		}
		
		[MonoPInvokeCallback(typeof(AssertFieldGetRaiseExceptionsDelegate))]
		static bool AssertFieldGetRaiseExceptions()
		{
			var returnValue = UnityEngine.Assertions.Assert.raiseExceptions;
			return returnValue;
		}
		
		[MonoPInvokeCallback(typeof(AssertFieldSetRaiseExceptionsDelegate))]
		static void AssertFieldSetRaiseExceptions(bool value)
		{
			UnityEngine.Assertions.Assert.raiseExceptions = value;
		}
		
		[MonoPInvokeCallback(typeof(AudioSettingsMethodGetDSPBufferSizeSystemInt32_SystemInt32Delegate))]
		static void AudioSettingsMethodGetDSPBufferSizeSystemInt32_SystemInt32(out int bufferLength, out int numBuffers)
		{
			UnityEngine.AudioSettings.GetDSPBufferSize(out bufferLength, out numBuffers);
		}
		
		[MonoPInvokeCallback(typeof(NetworkTransportMethodGetBroadcastConnectionInfoSystemInt32_SystemString_SystemInt32_SystemByteDelegate))]
		static void NetworkTransportMethodGetBroadcastConnectionInfoSystemInt32_SystemString_SystemInt32_SystemByte(int hostId, ref int addressHandle, out int port, out byte error)
		{
			var address = (System.String)NativeScript.Bindings.GetObject(addressHandle);
			UnityEngine.Networking.NetworkTransport.GetBroadcastConnectionInfo(hostId, out address, out port, out error);
			int addressHandleNew = NativeScript.Bindings.GetHandle(address);
			if (addressHandleNew < 0)
			{
				addressHandle = NativeScript.Bindings.StoreObject(address);
			}
			else
			{
				addressHandle = addressHandleNew;
			}
		}
		
		[MonoPInvokeCallback(typeof(NetworkTransportMethodInitDelegate))]
		static void NetworkTransportMethodInit()
		{
			UnityEngine.Networking.NetworkTransport.Init();
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
			int thisHandle;
			
			public TestScript()
			{
				thisHandle = NativeScript.Bindings.StoreObject(this);
			}
			
			public void Awake()
			{
				NativeScript.Bindings.TestScriptAwake(thisHandle);
			}
			
			public void OnAnimatorIK(int param0)
			{
				NativeScript.Bindings.TestScriptOnAnimatorIK(thisHandle, param0);
			}
			
			public void OnCollisionEnter(UnityEngine.Collision param0)
			{
				int param0Handle = NativeScript.Bindings.GetHandle(param0);
				if (param0Handle < 0)
				{
					param0Handle = NativeScript.Bindings.StoreObject(param0);
				}
				NativeScript.Bindings.TestScriptOnCollisionEnter(thisHandle, param0Handle);
			}
			
			public void Update()
			{
				NativeScript.Bindings.TestScriptUpdate(thisHandle);
			}
		}
	}
}
/*END MONOBEHAVIOURS*/
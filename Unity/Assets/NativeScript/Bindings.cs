using AOT;

using System;
using System.Collections;
using System.IO;
using System.Runtime.InteropServices;

using UnityEngine;

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
		
		/// <summary>
		/// A reusable version of UnityEngine.WaitForSecondsRealtime to avoid
		/// GC allocs
		/// </summary>
		class ReusableWaitForSecondsRealtime : CustomYieldInstruction
		{
			private float waitTime;
			
			public float WaitTime
			{
				set
				{
					waitTime = Time.realtimeSinceStartup + value;
				}
			}

			public override bool keepWaiting
			{
				get
				{
					return Time.realtimeSinceStartup < waitTime;
				}
			}

			public ReusableWaitForSecondsRealtime(float time)
			{
				WaitTime = time;
			}
		}

		public enum DestroyFunction
		{
			/*BEGIN DESTROY FUNCTION ENUMERATORS*/
			BaseBallScript
			/*END DESTROY FUNCTION ENUMERATORS*/
		}

		struct DestroyEntry
		{
			public DestroyFunction Function;
			public int CppHandle;

			public DestroyEntry(DestroyFunction function, int cppHandle)
			{
				Function = function;
				CppHandle = cppHandle;
			}
		}
		
		// Name of the plugin when using [DllImport]
		const string PLUGIN_NAME = "NativeScript";
		
		// Path to load the plugin from when running inside the editor
#if UNITY_EDITOR_OSX
		const string PLUGIN_PATH = "/Plugins/Editor/NativeScript.bundle/Contents/MacOS/NativeScript";
#elif UNITY_EDITOR_LINUX
		const string PLUGIN_PATH = "/Plugins/Editor/libNativeScript.so";
#elif UNITY_EDITOR_WIN
		const string PLUGIN_PATH = "/Plugins/Editor/NativeScript.dll";
		const string PLUGIN_TEMP_PATH = "/Plugins/Editor/NativeScript_temp.dll";
#endif

		enum InitMode : byte
		{
			FirstBoot,
			Reload
		}
		
#if UNITY_EDITOR
		// Handle to the C++ DLL
		static IntPtr libraryHandle;
		
		delegate void InitDelegate(
			IntPtr memory,
			int memorySize,
			InitMode initMode,
			IntPtr releaseObject,
			IntPtr stringNew,
			IntPtr setException,
			IntPtr arrayGetLength,
			IntPtr enumerableGetEnumerator,
			/*BEGIN INIT PARAMS*/
			int maxManagedObjects,
			IntPtr unityEngineVector3ConstructorSystemSingle_SystemSingle_SystemSingle,
			IntPtr unityEngineVector3Methodop_AdditionUnityEngineVector3_UnityEngineVector3,
			IntPtr boxVector3,
			IntPtr unboxVector3,
			IntPtr unityEngineObjectPropertyGetName,
			IntPtr unityEngineObjectPropertySetName,
			IntPtr unityEngineComponentPropertyGetTransform,
			IntPtr unityEngineTransformPropertyGetPosition,
			IntPtr unityEngineTransformPropertySetPosition,
			IntPtr systemCollectionsIEnumeratorPropertyGetCurrent,
			IntPtr systemCollectionsIEnumeratorMethodMoveNext,
			IntPtr unityEngineGameObjectMethodAddComponentMyGameBaseBallScript,
			IntPtr unityEngineGameObjectMethodCreatePrimitiveUnityEnginePrimitiveType,
			IntPtr unityEngineDebugMethodLogSystemObject,
			IntPtr unityEngineMonoBehaviourPropertyGetTransform,
			IntPtr systemExceptionConstructorSystemString,
			IntPtr boxPrimitiveType,
			IntPtr unboxPrimitiveType,
			IntPtr unityEngineTimePropertyGetDeltaTime,
			IntPtr releaseBaseBallScript,
			IntPtr baseBallScriptConstructor,
			IntPtr boxBoolean,
			IntPtr unboxBoolean,
			IntPtr boxSByte,
			IntPtr unboxSByte,
			IntPtr boxByte,
			IntPtr unboxByte,
			IntPtr boxInt16,
			IntPtr unboxInt16,
			IntPtr boxUInt16,
			IntPtr unboxUInt16,
			IntPtr boxInt32,
			IntPtr unboxInt32,
			IntPtr boxUInt32,
			IntPtr unboxUInt32,
			IntPtr boxInt64,
			IntPtr unboxInt64,
			IntPtr boxUInt64,
			IntPtr unboxUInt64,
			IntPtr boxChar,
			IntPtr unboxChar,
			IntPtr boxSingle,
			IntPtr unboxSingle,
			IntPtr boxDouble,
			IntPtr unboxDouble
			/*END INIT PARAMS*/);
		
		public delegate void SetCsharpExceptionDelegate(int handle);
		
		/*BEGIN DELEGATES*/
		public delegate int NewBaseBallScriptDelegate(int param0);
		public static NewBaseBallScriptDelegate NewBaseBallScript;
		
		public delegate void DestroyBaseBallScriptDelegate(int param0);
		public static DestroyBaseBallScriptDelegate DestroyBaseBallScript;
		
		public delegate void MyGameAbstractBaseBallScriptUpdateDelegate(int thisHandle);
		public static MyGameAbstractBaseBallScriptUpdateDelegate MyGameAbstractBaseBallScriptUpdate;
		
		public delegate void SetCsharpExceptionSystemNullReferenceExceptionDelegate(int param0);
		public static SetCsharpExceptionSystemNullReferenceExceptionDelegate SetCsharpExceptionSystemNullReferenceException;
		/*END DELEGATES*/
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
			IntPtr memory,
			int memorySize,
			initMode initMode,
			IntPtr releaseObject,
			IntPtr stringNew,
			IntPtr setException,
			IntPtr arrayGetLength,
			IntPtr enumerableGetEnumerator,
			/*BEGIN INIT PARAMS*/
			int maxManagedObjects,
			IntPtr unityEngineVector3ConstructorSystemSingle_SystemSingle_SystemSingle,
			IntPtr unityEngineVector3Methodop_AdditionUnityEngineVector3_UnityEngineVector3,
			IntPtr boxVector3,
			IntPtr unboxVector3,
			IntPtr unityEngineObjectPropertyGetName,
			IntPtr unityEngineObjectPropertySetName,
			IntPtr unityEngineComponentPropertyGetTransform,
			IntPtr unityEngineTransformPropertyGetPosition,
			IntPtr unityEngineTransformPropertySetPosition,
			IntPtr systemCollectionsIEnumeratorPropertyGetCurrent,
			IntPtr systemCollectionsIEnumeratorMethodMoveNext,
			IntPtr unityEngineGameObjectMethodAddComponentMyGameBaseBallScript,
			IntPtr unityEngineGameObjectMethodCreatePrimitiveUnityEnginePrimitiveType,
			IntPtr unityEngineDebugMethodLogSystemObject,
			IntPtr unityEngineMonoBehaviourPropertyGetTransform,
			IntPtr systemExceptionConstructorSystemString,
			IntPtr boxPrimitiveType,
			IntPtr unboxPrimitiveType,
			IntPtr unityEngineTimePropertyGetDeltaTime,
			IntPtr releaseBaseBallScript,
			IntPtr baseBallScriptConstructor,
			IntPtr boxBoolean,
			IntPtr unboxBoolean,
			IntPtr boxSByte,
			IntPtr unboxSByte,
			IntPtr boxByte,
			IntPtr unboxByte,
			IntPtr boxInt16,
			IntPtr unboxInt16,
			IntPtr boxUInt16,
			IntPtr unboxUInt16,
			IntPtr boxInt32,
			IntPtr unboxInt32,
			IntPtr boxUInt32,
			IntPtr unboxUInt32,
			IntPtr boxInt64,
			IntPtr unboxInt64,
			IntPtr boxUInt64,
			IntPtr unboxUInt64,
			IntPtr boxChar,
			IntPtr unboxChar,
			IntPtr boxSingle,
			IntPtr unboxSingle,
			IntPtr boxDouble,
			IntPtr unboxDouble
			/*END INIT PARAMS*/);
		
		[DllImport(PluginName)]
		static extern void SetCsharpException(int handle);
		
		/*BEGIN IMPORTS*/
		[DllImport(Constants.PluginName)]
		public static extern void NewBaseBallScript(int thisHandle, int param0);
		
		[DllImport(Constants.PluginName)]
		public static extern void DestroyBaseBallScript(int thisHandle, int param0);
		
		[DllImport(Constants.PluginName)]
		public static extern void MyGameAbstractBaseBallScriptUpdate(int thisHandle);
		
		[DllImport(Constants.PluginName)]
		public static extern void SetCsharpExceptionSystemNullReferenceException(int thisHandle, int param0);
		/*END IMPORTS*/
#endif
		
		delegate void ReleaseObjectDelegate(int handle);
		delegate int StringNewDelegate(string chars);
		delegate void SetExceptionDelegate(int handle);
		delegate int ArrayGetLengthDelegate(int handle);
		delegate int EnumerableGetEnumeratorDelegate(int handle);
		
		/*BEGIN DELEGATE TYPES*/
		delegate UnityEngine.Vector3 UnityEngineVector3ConstructorSystemSingle_SystemSingle_SystemSingleDelegate(float x, float y, float z);
		delegate UnityEngine.Vector3 UnityEngineVector3Methodop_AdditionUnityEngineVector3_UnityEngineVector3Delegate(ref UnityEngine.Vector3 a, ref UnityEngine.Vector3 b);
		delegate int BoxVector3Delegate(ref UnityEngine.Vector3 val);
		delegate UnityEngine.Vector3 UnboxVector3Delegate(int valHandle);
		delegate int UnityEngineObjectPropertyGetNameDelegate(int thisHandle);
		delegate void UnityEngineObjectPropertySetNameDelegate(int thisHandle, int valueHandle);
		delegate int UnityEngineComponentPropertyGetTransformDelegate(int thisHandle);
		delegate UnityEngine.Vector3 UnityEngineTransformPropertyGetPositionDelegate(int thisHandle);
		delegate void UnityEngineTransformPropertySetPositionDelegate(int thisHandle, ref UnityEngine.Vector3 value);
		delegate int SystemCollectionsIEnumeratorPropertyGetCurrentDelegate(int thisHandle);
		delegate bool SystemCollectionsIEnumeratorMethodMoveNextDelegate(int thisHandle);
		delegate int UnityEngineGameObjectMethodAddComponentMyGameBaseBallScriptDelegate(int thisHandle);
		delegate int UnityEngineGameObjectMethodCreatePrimitiveUnityEnginePrimitiveTypeDelegate(UnityEngine.PrimitiveType type);
		delegate void UnityEngineDebugMethodLogSystemObjectDelegate(int messageHandle);
		delegate int UnityEngineMonoBehaviourPropertyGetTransformDelegate(int thisHandle);
		delegate int SystemExceptionConstructorSystemStringDelegate(int messageHandle);
		delegate int BoxPrimitiveTypeDelegate(UnityEngine.PrimitiveType val);
		delegate UnityEngine.PrimitiveType UnboxPrimitiveTypeDelegate(int valHandle);
		delegate float UnityEngineTimePropertyGetDeltaTimeDelegate();
		delegate void BaseBallScriptConstructorDelegate(int cppHandle, ref int handle);
		delegate void ReleaseBaseBallScriptDelegate(int handle);
		delegate int BoxBooleanDelegate(bool val);
		delegate bool UnboxBooleanDelegate(int valHandle);
		delegate int BoxSByteDelegate(sbyte val);
		delegate sbyte UnboxSByteDelegate(int valHandle);
		delegate int BoxByteDelegate(byte val);
		delegate byte UnboxByteDelegate(int valHandle);
		delegate int BoxInt16Delegate(short val);
		delegate short UnboxInt16Delegate(int valHandle);
		delegate int BoxUInt16Delegate(ushort val);
		delegate ushort UnboxUInt16Delegate(int valHandle);
		delegate int BoxInt32Delegate(int val);
		delegate int UnboxInt32Delegate(int valHandle);
		delegate int BoxUInt32Delegate(uint val);
		delegate uint UnboxUInt32Delegate(int valHandle);
		delegate int BoxInt64Delegate(long val);
		delegate long UnboxInt64Delegate(int valHandle);
		delegate int BoxUInt64Delegate(ulong val);
		delegate ulong UnboxUInt64Delegate(int valHandle);
		delegate int BoxCharDelegate(char val);
		delegate char UnboxCharDelegate(int valHandle);
		delegate int BoxSingleDelegate(float val);
		delegate float UnboxSingleDelegate(int valHandle);
		delegate int BoxDoubleDelegate(double val);
		delegate double UnboxDoubleDelegate(int valHandle);
		/*END DELEGATE TYPES*/
		
		private static readonly string pluginPath = Application.dataPath + PLUGIN_PATH;
#if UNITY_EDITOR_WIN
		private static readonly string pluginTempPath = Application.dataPath + PLUGIN_TEMP_PATH;
#endif
		public static Exception UnhandledCppException;
		public static SetCsharpExceptionDelegate SetCsharpException;
		static IntPtr memory;
		static int memorySize;
		static DestroyEntry[] destroyQueue;
		static int destroyQueueCount;
		static int destroyQueueCapacity;
		static object destroyQueueLockObj;
		
		/// <summary>
		/// Open the C++ plugin and call its PluginMain()
		/// </summary>
		/// 
		/// <param name="memorySize">
		/// Number of bytes of memory to make available to the C++ plugin
		/// </param>
		public static void Open(int memorySize)
		{
			/*BEGIN STORE INIT CALLS*/
			NativeScript.Bindings.ObjectStore.Init(1000);
			/*END STORE INIT CALLS*/

			// Allocate unmanaged memory
			Bindings.memorySize = memorySize;
			memory = Marshal.AllocHGlobal(memorySize);

			// Allocate destroy queue
			destroyQueueCapacity = 128;
			destroyQueue = new DestroyEntry[destroyQueueCapacity];
			destroyQueueLockObj = new object();

			OpenPlugin(InitMode.FirstBoot);
		}
		
		// Reloading requires dynamic loading of the C++ plugin, which is only
		// available in the editor
#if UNITY_EDITOR
		/// <summary>
		/// Reload the C++ plugin. Its memory is intact and false is passed for
		/// the isFirstBoot parameter of PluginMain().
		/// </summary>
		public static void Reload()
		{
			DestroyAll();
			ClosePlugin();
			OpenPlugin(InitMode.Reload);
		}
		
		/// <summary>
		/// Poll the plugin for changes and reload if any are found.
		/// </summary>
		/// 
		/// <param name="pollTime">
		/// Number of seconds between polls.
		/// </param>
		/// 
		/// <returns>
		/// Enumerator for this iterator function. Can be passed to
		/// MonoBehaviour.StartCoroutine for easy usage.
		/// </returns>
		public static IEnumerator AutoReload(float pollTime)
		{
			// Get the original time
			long lastWriteTime = File.GetLastWriteTime(pluginPath).Ticks;
			
			ReusableWaitForSecondsRealtime poll
				= new ReusableWaitForSecondsRealtime(pollTime);
			do
			{
				// Poll. Reload if the last write time changed.
				long cur = File.GetLastWriteTime(pluginPath).Ticks;
				if (cur != lastWriteTime)
				{
					lastWriteTime = cur;
					Reload();
				}
				
				// Wait to poll again
				poll.WaitTime = pollTime;
				yield return poll;
			}
			while (true);
		}
#endif
		
		private static void OpenPlugin(InitMode initMode)
		{
#if UNITY_EDITOR
			string loadPath;
#if UNITY_EDITOR_WIN
			// Copy native library to temporary file
			File.Copy(pluginPath, pluginTempPath);
			loadPath = pluginTempPath;
#else
			loadPath = pluginPath;
#endif
			// Open native library
			libraryHandle = OpenLibrary(loadPath);
			InitDelegate Init = GetDelegate<InitDelegate>(
				libraryHandle,
				"Init");
			SetCsharpException = GetDelegate<SetCsharpExceptionDelegate>(
				libraryHandle,
				"SetCsharpException");
			/*BEGIN GETDELEGATE CALLS*/
			NewBaseBallScript = GetDelegate<NewBaseBallScriptDelegate>(libraryHandle, "NewBaseBallScript");
			DestroyBaseBallScript = GetDelegate<DestroyBaseBallScriptDelegate>(libraryHandle, "DestroyBaseBallScript");
			MyGameAbstractBaseBallScriptUpdate = GetDelegate<MyGameAbstractBaseBallScriptUpdateDelegate>(libraryHandle, "MyGameAbstractBaseBallScriptUpdate");
			SetCsharpExceptionSystemNullReferenceException = GetDelegate<SetCsharpExceptionSystemNullReferenceExceptionDelegate>(libraryHandle, "SetCsharpExceptionSystemNullReferenceException");
			/*END GETDELEGATE CALLS*/
#endif
			// Init C++ library
			Init(
				memory,
				memorySize,
				initMode,
				Marshal.GetFunctionPointerForDelegate(new ReleaseObjectDelegate(ReleaseObject)),
				Marshal.GetFunctionPointerForDelegate(new StringNewDelegate(StringNew)),
				Marshal.GetFunctionPointerForDelegate(new SetExceptionDelegate(SetException)),
				Marshal.GetFunctionPointerForDelegate(new ArrayGetLengthDelegate(ArrayGetLength)),
				Marshal.GetFunctionPointerForDelegate(new EnumerableGetEnumeratorDelegate(EnumerableGetEnumerator)),
				/*BEGIN INIT CALL*/
				1000,
				Marshal.GetFunctionPointerForDelegate(new UnityEngineVector3ConstructorSystemSingle_SystemSingle_SystemSingleDelegate(UnityEngineVector3ConstructorSystemSingle_SystemSingle_SystemSingle)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineVector3Methodop_AdditionUnityEngineVector3_UnityEngineVector3Delegate(UnityEngineVector3Methodop_AdditionUnityEngineVector3_UnityEngineVector3)),
				Marshal.GetFunctionPointerForDelegate(new BoxVector3Delegate(BoxVector3)),
				Marshal.GetFunctionPointerForDelegate(new UnboxVector3Delegate(UnboxVector3)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineObjectPropertyGetNameDelegate(UnityEngineObjectPropertyGetName)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineObjectPropertySetNameDelegate(UnityEngineObjectPropertySetName)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineComponentPropertyGetTransformDelegate(UnityEngineComponentPropertyGetTransform)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineTransformPropertyGetPositionDelegate(UnityEngineTransformPropertyGetPosition)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineTransformPropertySetPositionDelegate(UnityEngineTransformPropertySetPosition)),
				Marshal.GetFunctionPointerForDelegate(new SystemCollectionsIEnumeratorPropertyGetCurrentDelegate(SystemCollectionsIEnumeratorPropertyGetCurrent)),
				Marshal.GetFunctionPointerForDelegate(new SystemCollectionsIEnumeratorMethodMoveNextDelegate(SystemCollectionsIEnumeratorMethodMoveNext)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineGameObjectMethodAddComponentMyGameBaseBallScriptDelegate(UnityEngineGameObjectMethodAddComponentMyGameBaseBallScript)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineGameObjectMethodCreatePrimitiveUnityEnginePrimitiveTypeDelegate(UnityEngineGameObjectMethodCreatePrimitiveUnityEnginePrimitiveType)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineDebugMethodLogSystemObjectDelegate(UnityEngineDebugMethodLogSystemObject)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineMonoBehaviourPropertyGetTransformDelegate(UnityEngineMonoBehaviourPropertyGetTransform)),
				Marshal.GetFunctionPointerForDelegate(new SystemExceptionConstructorSystemStringDelegate(SystemExceptionConstructorSystemString)),
				Marshal.GetFunctionPointerForDelegate(new BoxPrimitiveTypeDelegate(BoxPrimitiveType)),
				Marshal.GetFunctionPointerForDelegate(new UnboxPrimitiveTypeDelegate(UnboxPrimitiveType)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineTimePropertyGetDeltaTimeDelegate(UnityEngineTimePropertyGetDeltaTime)),
				Marshal.GetFunctionPointerForDelegate(new ReleaseBaseBallScriptDelegate(ReleaseBaseBallScript)),
				Marshal.GetFunctionPointerForDelegate(new BaseBallScriptConstructorDelegate(BaseBallScriptConstructor)),
				Marshal.GetFunctionPointerForDelegate(new BoxBooleanDelegate(BoxBoolean)),
				Marshal.GetFunctionPointerForDelegate(new UnboxBooleanDelegate(UnboxBoolean)),
				Marshal.GetFunctionPointerForDelegate(new BoxSByteDelegate(BoxSByte)),
				Marshal.GetFunctionPointerForDelegate(new UnboxSByteDelegate(UnboxSByte)),
				Marshal.GetFunctionPointerForDelegate(new BoxByteDelegate(BoxByte)),
				Marshal.GetFunctionPointerForDelegate(new UnboxByteDelegate(UnboxByte)),
				Marshal.GetFunctionPointerForDelegate(new BoxInt16Delegate(BoxInt16)),
				Marshal.GetFunctionPointerForDelegate(new UnboxInt16Delegate(UnboxInt16)),
				Marshal.GetFunctionPointerForDelegate(new BoxUInt16Delegate(BoxUInt16)),
				Marshal.GetFunctionPointerForDelegate(new UnboxUInt16Delegate(UnboxUInt16)),
				Marshal.GetFunctionPointerForDelegate(new BoxInt32Delegate(BoxInt32)),
				Marshal.GetFunctionPointerForDelegate(new UnboxInt32Delegate(UnboxInt32)),
				Marshal.GetFunctionPointerForDelegate(new BoxUInt32Delegate(BoxUInt32)),
				Marshal.GetFunctionPointerForDelegate(new UnboxUInt32Delegate(UnboxUInt32)),
				Marshal.GetFunctionPointerForDelegate(new BoxInt64Delegate(BoxInt64)),
				Marshal.GetFunctionPointerForDelegate(new UnboxInt64Delegate(UnboxInt64)),
				Marshal.GetFunctionPointerForDelegate(new BoxUInt64Delegate(BoxUInt64)),
				Marshal.GetFunctionPointerForDelegate(new UnboxUInt64Delegate(UnboxUInt64)),
				Marshal.GetFunctionPointerForDelegate(new BoxCharDelegate(BoxChar)),
				Marshal.GetFunctionPointerForDelegate(new UnboxCharDelegate(UnboxChar)),
				Marshal.GetFunctionPointerForDelegate(new BoxSingleDelegate(BoxSingle)),
				Marshal.GetFunctionPointerForDelegate(new UnboxSingleDelegate(UnboxSingle)),
				Marshal.GetFunctionPointerForDelegate(new BoxDoubleDelegate(BoxDouble)),
				Marshal.GetFunctionPointerForDelegate(new UnboxDoubleDelegate(UnboxDouble))
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
			ClosePlugin();
			Marshal.FreeHGlobal(memory);
			memory = IntPtr.Zero;
		}

		/// <summary>
		/// Perform updates over time
		/// </summary>
		public static void Update()
		{
			DestroyAll();
		}
		
		private static void ClosePlugin()
		{
#if UNITY_EDITOR
			CloseLibrary(libraryHandle);
			libraryHandle = IntPtr.Zero;
#endif
#if UNITY_EDITOR_WIN
			File.Delete(pluginTempPath);
#endif
		}

		public static void QueueDestroy(DestroyFunction function, int cppHandle)
		{
			lock (destroyQueueLockObj)
			{
				// Grow capacity if necessary
				int count = destroyQueueCount;
				int capacity = destroyQueueCapacity;
				DestroyEntry[] queue = destroyQueue;
				if (count == capacity)
				{
					int newCapacity = capacity * 2;
					DestroyEntry[] newQueue = new DestroyEntry[newCapacity];
					for (int i = 0; i < capacity; ++i)
					{
						newQueue[i] = queue[i];
					}
					destroyQueueCapacity = newCapacity;
					destroyQueue = newQueue;
					queue = newQueue;
				}

				// Add to the end
				queue[count] = new DestroyEntry(function, cppHandle);
				destroyQueueCount = count + 1;
			}
		}

		static void DestroyAll()
		{
			lock (destroyQueueLockObj)
			{
				int count = destroyQueueCount;
				DestroyEntry[] queue = destroyQueue;
				for (int i = 0; i < count; ++i)
				{
					DestroyEntry entry = queue[i];
					switch (entry.Function)
					{
						/*BEGIN DESTROY QUEUE CASES*/
						case DestroyFunction.BaseBallScript:
							DestroyBaseBallScript(entry.CppHandle);
							break;
						/*END DESTROY QUEUE CASES*/
					}
				}
				destroyQueueCount = 0;
			}
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
		
		[MonoPInvokeCallback(typeof(EnumerableGetEnumeratorDelegate))]
		static int EnumerableGetEnumerator(int handle)
		{
			return ObjectStore.Store(((IEnumerable)ObjectStore.Get(handle)).GetEnumerator());
		}

		/*BEGIN FUNCTIONS*/
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
		
		[MonoPInvokeCallback(typeof(BoxVector3Delegate))]
		static int BoxVector3(ref UnityEngine.Vector3 val)
		{
			try
			{
				var returnValue = NativeScript.Bindings.ObjectStore.Store((object)val);
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
		
		[MonoPInvokeCallback(typeof(UnboxVector3Delegate))]
		static UnityEngine.Vector3 UnboxVector3(int valHandle)
		{
			try
			{
				var val = NativeScript.Bindings.ObjectStore.Get(valHandle);
				var returnValue = (UnityEngine.Vector3)val;
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
		
		[MonoPInvokeCallback(typeof(SystemCollectionsIEnumeratorPropertyGetCurrentDelegate))]
		static int SystemCollectionsIEnumeratorPropertyGetCurrent(int thisHandle)
		{
			try
			{
				var thiz = (System.Collections.IEnumerator)NativeScript.Bindings.ObjectStore.Get(thisHandle);
				var returnValue = thiz.Current;
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
		
		[MonoPInvokeCallback(typeof(SystemCollectionsIEnumeratorMethodMoveNextDelegate))]
		static bool SystemCollectionsIEnumeratorMethodMoveNext(int thisHandle)
		{
			try
			{
				var thiz = (System.Collections.IEnumerator)NativeScript.Bindings.ObjectStore.Get(thisHandle);
				var returnValue = thiz.MoveNext();
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
		
		[MonoPInvokeCallback(typeof(UnityEngineGameObjectMethodAddComponentMyGameBaseBallScriptDelegate))]
		static int UnityEngineGameObjectMethodAddComponentMyGameBaseBallScript(int thisHandle)
		{
			try
			{
				var thiz = (UnityEngine.GameObject)NativeScript.Bindings.ObjectStore.Get(thisHandle);
				var returnValue = thiz.AddComponent<MyGame.BaseBallScript>();
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
		
		[MonoPInvokeCallback(typeof(UnityEngineGameObjectMethodCreatePrimitiveUnityEnginePrimitiveTypeDelegate))]
		static int UnityEngineGameObjectMethodCreatePrimitiveUnityEnginePrimitiveType(UnityEngine.PrimitiveType type)
		{
			try
			{
				var returnValue = UnityEngine.GameObject.CreatePrimitive(type);
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
		
		[MonoPInvokeCallback(typeof(UnityEngineMonoBehaviourPropertyGetTransformDelegate))]
		static int UnityEngineMonoBehaviourPropertyGetTransform(int thisHandle)
		{
			try
			{
				var thiz = (UnityEngine.MonoBehaviour)NativeScript.Bindings.ObjectStore.Get(thisHandle);
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
		
		[MonoPInvokeCallback(typeof(BoxPrimitiveTypeDelegate))]
		static int BoxPrimitiveType(UnityEngine.PrimitiveType val)
		{
			try
			{
				var returnValue = NativeScript.Bindings.ObjectStore.Store((object)val);
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
		
		[MonoPInvokeCallback(typeof(UnboxPrimitiveTypeDelegate))]
		static UnityEngine.PrimitiveType UnboxPrimitiveType(int valHandle)
		{
			try
			{
				var val = NativeScript.Bindings.ObjectStore.Get(valHandle);
				var returnValue = (UnityEngine.PrimitiveType)val;
				return returnValue;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(UnityEngine.PrimitiveType);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(UnityEngine.PrimitiveType);
			}
		}
		
		[MonoPInvokeCallback(typeof(UnityEngineTimePropertyGetDeltaTimeDelegate))]
		static float UnityEngineTimePropertyGetDeltaTime()
		{
			try
			{
				var returnValue = UnityEngine.Time.deltaTime;
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
		
		[MonoPInvokeCallback(typeof(BaseBallScriptConstructorDelegate))]
		static void BaseBallScriptConstructor(int cppHandle, ref int handle)
		{
			try
			{
				var thiz = new MyGame.BaseBallScript(cppHandle);
				handle = NativeScript.Bindings.ObjectStore.Store(thiz);
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				handle = default(int);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				handle = default(int);
			}
		}
		
		[MonoPInvokeCallback(typeof(ReleaseBaseBallScriptDelegate))]
		static void ReleaseBaseBallScript(int handle)
		{
			try
			{
				MyGame.BaseBallScript thiz;
				thiz = (MyGame.BaseBallScript)ObjectStore.Get(handle);
				int cppHandle = thiz.CppHandle;
				thiz.CppHandle = 0;
				QueueDestroy(DestroyFunction.BaseBallScript, cppHandle);
				ObjectStore.Remove(handle);
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
		
		[MonoPInvokeCallback(typeof(BoxBooleanDelegate))]
		static int BoxBoolean(bool val)
		{
			try
			{
				var returnValue = NativeScript.Bindings.ObjectStore.Store((object)val);
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
		
		[MonoPInvokeCallback(typeof(UnboxBooleanDelegate))]
		static bool UnboxBoolean(int valHandle)
		{
			try
			{
				var val = NativeScript.Bindings.ObjectStore.Get(valHandle);
				var returnValue = (bool)val;
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
		
		[MonoPInvokeCallback(typeof(BoxSByteDelegate))]
		static int BoxSByte(sbyte val)
		{
			try
			{
				var returnValue = NativeScript.Bindings.ObjectStore.Store((object)val);
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
		
		[MonoPInvokeCallback(typeof(UnboxSByteDelegate))]
		static sbyte UnboxSByte(int valHandle)
		{
			try
			{
				var val = NativeScript.Bindings.ObjectStore.Get(valHandle);
				var returnValue = (sbyte)val;
				return returnValue;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(sbyte);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(sbyte);
			}
		}
		
		[MonoPInvokeCallback(typeof(BoxByteDelegate))]
		static int BoxByte(byte val)
		{
			try
			{
				var returnValue = NativeScript.Bindings.ObjectStore.Store((object)val);
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
		
		[MonoPInvokeCallback(typeof(UnboxByteDelegate))]
		static byte UnboxByte(int valHandle)
		{
			try
			{
				var val = NativeScript.Bindings.ObjectStore.Get(valHandle);
				var returnValue = (byte)val;
				return returnValue;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(byte);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(byte);
			}
		}
		
		[MonoPInvokeCallback(typeof(BoxInt16Delegate))]
		static int BoxInt16(short val)
		{
			try
			{
				var returnValue = NativeScript.Bindings.ObjectStore.Store((object)val);
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
		
		[MonoPInvokeCallback(typeof(UnboxInt16Delegate))]
		static short UnboxInt16(int valHandle)
		{
			try
			{
				var val = NativeScript.Bindings.ObjectStore.Get(valHandle);
				var returnValue = (short)val;
				return returnValue;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(short);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(short);
			}
		}
		
		[MonoPInvokeCallback(typeof(BoxUInt16Delegate))]
		static int BoxUInt16(ushort val)
		{
			try
			{
				var returnValue = NativeScript.Bindings.ObjectStore.Store((object)val);
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
		
		[MonoPInvokeCallback(typeof(UnboxUInt16Delegate))]
		static ushort UnboxUInt16(int valHandle)
		{
			try
			{
				var val = NativeScript.Bindings.ObjectStore.Get(valHandle);
				var returnValue = (ushort)val;
				return returnValue;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(ushort);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(ushort);
			}
		}
		
		[MonoPInvokeCallback(typeof(BoxInt32Delegate))]
		static int BoxInt32(int val)
		{
			try
			{
				var returnValue = NativeScript.Bindings.ObjectStore.Store((object)val);
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
		
		[MonoPInvokeCallback(typeof(UnboxInt32Delegate))]
		static int UnboxInt32(int valHandle)
		{
			try
			{
				var val = NativeScript.Bindings.ObjectStore.Get(valHandle);
				var returnValue = (int)val;
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
		
		[MonoPInvokeCallback(typeof(BoxUInt32Delegate))]
		static int BoxUInt32(uint val)
		{
			try
			{
				var returnValue = NativeScript.Bindings.ObjectStore.Store((object)val);
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
		
		[MonoPInvokeCallback(typeof(UnboxUInt32Delegate))]
		static uint UnboxUInt32(int valHandle)
		{
			try
			{
				var val = NativeScript.Bindings.ObjectStore.Get(valHandle);
				var returnValue = (uint)val;
				return returnValue;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(uint);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(uint);
			}
		}
		
		[MonoPInvokeCallback(typeof(BoxInt64Delegate))]
		static int BoxInt64(long val)
		{
			try
			{
				var returnValue = NativeScript.Bindings.ObjectStore.Store((object)val);
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
		
		[MonoPInvokeCallback(typeof(UnboxInt64Delegate))]
		static long UnboxInt64(int valHandle)
		{
			try
			{
				var val = NativeScript.Bindings.ObjectStore.Get(valHandle);
				var returnValue = (long)val;
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
		
		[MonoPInvokeCallback(typeof(BoxUInt64Delegate))]
		static int BoxUInt64(ulong val)
		{
			try
			{
				var returnValue = NativeScript.Bindings.ObjectStore.Store((object)val);
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
		
		[MonoPInvokeCallback(typeof(UnboxUInt64Delegate))]
		static ulong UnboxUInt64(int valHandle)
		{
			try
			{
				var val = NativeScript.Bindings.ObjectStore.Get(valHandle);
				var returnValue = (ulong)val;
				return returnValue;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(ulong);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(ulong);
			}
		}
		
		[MonoPInvokeCallback(typeof(BoxCharDelegate))]
		static int BoxChar(char val)
		{
			try
			{
				var returnValue = NativeScript.Bindings.ObjectStore.Store((object)val);
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
		
		[MonoPInvokeCallback(typeof(UnboxCharDelegate))]
		static char UnboxChar(int valHandle)
		{
			try
			{
				var val = NativeScript.Bindings.ObjectStore.Get(valHandle);
				var returnValue = (char)val;
				return returnValue;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(char);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(char);
			}
		}
		
		[MonoPInvokeCallback(typeof(BoxSingleDelegate))]
		static int BoxSingle(float val)
		{
			try
			{
				var returnValue = NativeScript.Bindings.ObjectStore.Store((object)val);
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
		
		[MonoPInvokeCallback(typeof(UnboxSingleDelegate))]
		static float UnboxSingle(int valHandle)
		{
			try
			{
				var val = NativeScript.Bindings.ObjectStore.Get(valHandle);
				var returnValue = (float)val;
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
		
		[MonoPInvokeCallback(typeof(BoxDoubleDelegate))]
		static int BoxDouble(double val)
		{
			try
			{
				var returnValue = NativeScript.Bindings.ObjectStore.Store((object)val);
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
		
		[MonoPInvokeCallback(typeof(UnboxDoubleDelegate))]
		static double UnboxDouble(int valHandle)
		{
			try
			{
				var val = NativeScript.Bindings.ObjectStore.Get(valHandle);
				var returnValue = (double)val;
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
		/*END FUNCTIONS*/
	}
}

/*BEGIN BASE TYPES*/
namespace MyGame
{
	class BaseBallScript : MyGame.AbstractBaseBallScript
	{
		public int CppHandle;
		
		public BaseBallScript()
		{
			int handle = NativeScript.Bindings.ObjectStore.Store(this);
			CppHandle = NativeScript.Bindings.NewBaseBallScript(handle);
		}
		
		~BaseBallScript()
		{
			if (CppHandle != 0)
			{
				NativeScript.Bindings.QueueDestroy(NativeScript.Bindings.DestroyFunction.BaseBallScript, CppHandle);
				CppHandle = 0;
			}
		}
		
		public BaseBallScript(int cppHandle)
			: base()
		{
			CppHandle = cppHandle;
		}
		
		public override void Update()
		{
			if (CppHandle != 0)
			{
				int thisHandle = CppHandle;
				NativeScript.Bindings.MyGameAbstractBaseBallScriptUpdate(thisHandle);
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
/*END BASE TYPES*/
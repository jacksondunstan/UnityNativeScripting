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
	class Bindings
	{
	#if UNITY_EDITOR

		// Handle to the C++ DLL
		public IntPtr libraryHandle;

		public delegate void InitDelegate(
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
			IntPtr gameObjectPropertyGetTransform,
			IntPtr gameObjectMethodFindSystemString,
			IntPtr componentPropertyGetTransform,
			IntPtr transformPropertyGetPosition,
			IntPtr transformPropertySetPosition,
			IntPtr debugMethodLogSystemObject,
			IntPtr assertFieldGetRaiseExceptions,
			IntPtr assertFieldSetRaiseExceptions
			/*END INIT PARAMS*/);

		public delegate void MonoBehaviourUpdateDelegate();
		public MonoBehaviourUpdateDelegate MonoBehaviourUpdate;

	#endif

	#if UNITY_EDITOR_OSX || UNITY_EDITOR_LINUX

		[DllImport("__Internal")]
		public static extern IntPtr dlopen(
			string path,
			int flag);

		[DllImport("__Internal")]
		public static extern IntPtr dlsym(
			IntPtr handle,
			string symbolName);

		[DllImport("__Internal")]
		public static extern int dlclose(
			IntPtr handle);

		public static IntPtr OpenLibrary(string path)
		{
			IntPtr handle = dlopen(path, 0);
			if (handle == IntPtr.Zero)
			{
				throw new Exception("Couldn't open native library: " + path);
			}
			return handle;
		}

		public static void CloseLibrary(IntPtr libraryHandle)
		{
			dlclose(libraryHandle);
		}

		public static T GetDelegate<T>(
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
		public static extern IntPtr LoadLibrary(
			string path);

		[DllImport("kernel32")]
		public static extern IntPtr GetProcAddress(
			IntPtr libraryHandle,
			string symbolName);

		[DllImport("kernel32")]
		public static extern bool FreeLibrary(
			IntPtr libraryHandle);

		public static IntPtr OpenLibrary(string path)
		{
			IntPtr handle = LoadLibrary(path);
			if (handle == IntPtr.Zero)
			{
				throw new Exception("Couldn't open native library: " + path);
			}
			return handle;
		}

		public static void CloseLibrary(IntPtr libraryHandle)
		{
			FreeLibrary(libraryHandle);
		}

		public static T GetDelegate<T>(
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

		[DllImport(Constants.PluginName)]
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
			IntPtr gameObjectPropertyGetTransform,
			IntPtr gameObjectMethodFindSystemString,
			IntPtr componentPropertyGetTransform,
			IntPtr transformPropertyGetPosition,
			IntPtr transformPropertySetPosition,
			IntPtr debugMethodLogSystemObject,
			IntPtr assertFieldGetRaiseExceptions,
			IntPtr assertFieldSetRaiseExceptions
			/*END INIT PARAMS*/);

		[DllImport(Constants.PluginName)]
		static extern void MonoBehaviourUpdate();

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
		
		delegate int GameObjectPropertyGetTransformDelegate(int thisHandle);
		
		delegate int GameObjectMethodFindSystemStringDelegate(int nameHandle);
		
		delegate int ComponentPropertyGetTransformDelegate(int thisHandle);
		
		delegate UnityEngine.Vector3 TransformPropertyGetPositionDelegate(int thisHandle);
		
		delegate void TransformPropertySetPositionDelegate(int thisHandle, UnityEngine.Vector3 value);
		
		delegate void DebugMethodLogSystemObjectDelegate(int messageHandle);
		
		delegate bool AssertFieldGetRaiseExceptionsDelegate();
		
		delegate void AssertFieldSetRaiseExceptionsDelegate(bool value);
		/*END DELEGATE TYPES*/

		public void Open()
		{
	#if UNITY_EDITOR

			// Open native library
			libraryHandle = OpenLibrary(
				Application.dataPath + BindingConstants.PluginPath);
			InitDelegate Init = GetDelegate<InitDelegate>(
				libraryHandle,
				"Init");
			MonoBehaviourUpdate = GetDelegate<MonoBehaviourUpdateDelegate>(
				libraryHandle,
				"MonoBehaviourUpdate");

	#endif

			// Init C++ library
			ObjectStore.Init(BindingConstants.MaxManagedObjects);
			Init(
				BindingConstants.MaxManagedObjects,
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
				Marshal.GetFunctionPointerForDelegate(new GameObjectPropertyGetTransformDelegate(GameObjectPropertyGetTransform)),
				Marshal.GetFunctionPointerForDelegate(new GameObjectMethodFindSystemStringDelegate(GameObjectMethodFindSystemString)),
				Marshal.GetFunctionPointerForDelegate(new ComponentPropertyGetTransformDelegate(ComponentPropertyGetTransform)),
				Marshal.GetFunctionPointerForDelegate(new TransformPropertyGetPositionDelegate(TransformPropertyGetPosition)),
				Marshal.GetFunctionPointerForDelegate(new TransformPropertySetPositionDelegate(TransformPropertySetPosition)),
				Marshal.GetFunctionPointerForDelegate(new DebugMethodLogSystemObjectDelegate(DebugMethodLogSystemObject)),
				Marshal.GetFunctionPointerForDelegate(new AssertFieldGetRaiseExceptionsDelegate(AssertFieldGetRaiseExceptions)),
				Marshal.GetFunctionPointerForDelegate(new AssertFieldSetRaiseExceptionsDelegate(AssertFieldSetRaiseExceptions))
				/*END INIT CALL*/
				);
		}
		
		public void Close()
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
		
		/*BEGIN FUNCTIONS*/
		[MonoPInvokeCallback(typeof(StopwatchConstructorDelegate))]
		static int StopwatchConstructor()
		{
			int obj = ObjectStore.Store(new System.Diagnostics.Stopwatch());
			return obj;
		}
		
		[MonoPInvokeCallback(typeof(StopwatchPropertyGetElapsedMillisecondsDelegate))]
		static long StopwatchPropertyGetElapsedMilliseconds(int thisHandle)
		{
			System.Diagnostics.Stopwatch thiz = (System.Diagnostics.Stopwatch)ObjectStore.Get(thisHandle);
			long obj = thiz.ElapsedMilliseconds;
			return obj;
		}
		
		[MonoPInvokeCallback(typeof(StopwatchMethodStartDelegate))]
		static void StopwatchMethodStart(int thisHandle)
		{
			System.Diagnostics.Stopwatch thiz = (System.Diagnostics.Stopwatch)ObjectStore.Get(thisHandle);
			thiz.Start();
		}
		
		[MonoPInvokeCallback(typeof(StopwatchMethodResetDelegate))]
		static void StopwatchMethodReset(int thisHandle)
		{
			System.Diagnostics.Stopwatch thiz = (System.Diagnostics.Stopwatch)ObjectStore.Get(thisHandle);
			thiz.Reset();
		}
		
		[MonoPInvokeCallback(typeof(ObjectPropertyGetNameDelegate))]
		static int ObjectPropertyGetName(int thisHandle)
		{
			UnityEngine.Object thiz = (UnityEngine.Object)ObjectStore.Get(thisHandle);
			string obj = thiz.name;
			int handle = ObjectStore.Store(obj);
			return handle;
		}
		
		[MonoPInvokeCallback(typeof(ObjectPropertySetNameDelegate))]
		static void ObjectPropertySetName(int thisHandle, int valueHandle)
		{
			UnityEngine.Object thiz = (UnityEngine.Object)ObjectStore.Get(thisHandle);
			thiz.name = (string)ObjectStore.Get(valueHandle);
		}
		
		[MonoPInvokeCallback(typeof(GameObjectConstructorDelegate))]
		static int GameObjectConstructor()
		{
			int obj = ObjectStore.Store(new UnityEngine.GameObject());
			return obj;
		}
		
		[MonoPInvokeCallback(typeof(GameObjectPropertyGetTransformDelegate))]
		static int GameObjectPropertyGetTransform(int thisHandle)
		{
			UnityEngine.GameObject thiz = (UnityEngine.GameObject)ObjectStore.Get(thisHandle);
			UnityEngine.Transform obj = thiz.transform;
			int handle = ObjectStore.Store(obj);
			return handle;
		}
		
		[MonoPInvokeCallback(typeof(GameObjectMethodFindSystemStringDelegate))]
		static int GameObjectMethodFindSystemString(int nameHandle)
		{
			UnityEngine.GameObject obj = UnityEngine.GameObject.Find((System.String)ObjectStore.Get(nameHandle));
			int handle = ObjectStore.Store(obj);
			return handle;
		}
		
		[MonoPInvokeCallback(typeof(ComponentPropertyGetTransformDelegate))]
		static int ComponentPropertyGetTransform(int thisHandle)
		{
			UnityEngine.Component thiz = (UnityEngine.Component)ObjectStore.Get(thisHandle);
			UnityEngine.Transform obj = thiz.transform;
			int handle = ObjectStore.Store(obj);
			return handle;
		}
		
		[MonoPInvokeCallback(typeof(TransformPropertyGetPositionDelegate))]
		static UnityEngine.Vector3 TransformPropertyGetPosition(int thisHandle)
		{
			UnityEngine.Transform thiz = (UnityEngine.Transform)ObjectStore.Get(thisHandle);
			UnityEngine.Vector3 obj = thiz.position;
			return obj;
		}
		
		[MonoPInvokeCallback(typeof(TransformPropertySetPositionDelegate))]
		static void TransformPropertySetPosition(int thisHandle, UnityEngine.Vector3 value)
		{
			UnityEngine.Transform thiz = (UnityEngine.Transform)ObjectStore.Get(thisHandle);
			thiz.position = value;
		}
		
		[MonoPInvokeCallback(typeof(DebugMethodLogSystemObjectDelegate))]
		static void DebugMethodLogSystemObject(int messageHandle)
		{
			UnityEngine.Debug.Log(ObjectStore.Get(messageHandle));
		}
		
		[MonoPInvokeCallback(typeof(AssertFieldGetRaiseExceptionsDelegate))]
		static bool AssertFieldGetRaiseExceptions()
		{
			bool obj = UnityEngine.Assertions.Assert.raiseExceptions;
			return obj;
		}
		
		[MonoPInvokeCallback(typeof(AssertFieldSetRaiseExceptionsDelegate))]
		static void AssertFieldSetRaiseExceptions(bool value)
		{
			UnityEngine.Assertions.Assert.raiseExceptions = value;
		}
		/*END FUNCTIONS*/
	}
}
using AOT;

using System;
using System.Collections;
using System.IO;
using System.Runtime.InteropServices;
using System.Collections.Generic;

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
			static Dictionary<uint, int> handleLookupByHash;
			static Dictionary<int, uint> hashLookupByHandle;
			static Stack<int> freeHandleStack;
			
			// Stored objects. The first is never used so 0 can be "null".
			static object[] objects;
			
			// The maximum number of objects to store. Must be positive.
			static int maxObjects;
			
			public static void Init(int maxObjects)
			{
				ObjectStore.maxObjects = maxObjects;
				
				// Initialize the objects as all null plus room for the
				// first to always be null.
				objects = new object[maxObjects + 1];

				// Initialize the handles stack as 1, 2, 3, ...
				for (
					int i = 0, handle = maxObjects;
					i < maxObjects;
					++i, --handle)
				{
					freeHandleStack.Push(handle);
				}
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
					int handle = freeHandleStack.Pop();
					
					// Store the object
					objects[handle] = obj;
					
					// Insert into the hash table
					uint hash = (uint)obj.GetHashCode();
					handleLookupByHash.Add(hash, handle);
					hashLookupByHandle.Add(handle, hash);
					
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
					// Look up the handle in the hash table
					uint hash = (uint)obj.GetHashCode();
					if (handleLookupByHash.ContainsKey(hash))
					{
						return handleLookupByHash[hash];
					}
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
					freeHandleStack.Push(handle);
					
					// Remove the object from the hash dictionary's
					var hash = hashLookupByHandle[handle];
					handleLookupByHash.Remove(hash);
					hashLookupByHandle.Remove(handle);
					
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
#if !UNITY_EDITOR && UNITY_IOS
		const string PLUGIN_NAME = "__Internal";
#else
		const string PLUGIN_NAME = "NativeScript";
#endif
		
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
		
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		delegate void InitDelegate(
			IntPtr memory,
			int memorySize,
			InitMode initMode);
		
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		public delegate void SetCsharpExceptionDelegate(int handle);
		
		/*BEGIN CPP DELEGATES*/
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		public delegate int NewBaseBallScriptDelegateType(int param0);
		public static NewBaseBallScriptDelegateType NewBaseBallScript;
		
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		public delegate void DestroyBaseBallScriptDelegateType(int param0);
		public static DestroyBaseBallScriptDelegateType DestroyBaseBallScript;
		
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		public delegate void MyGameAbstractBaseBallScriptUpdateDelegateType(int thisHandle);
		public static MyGameAbstractBaseBallScriptUpdateDelegateType MyGameAbstractBaseBallScriptUpdate;
		
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		public delegate void SetCsharpExceptionSystemNullReferenceExceptionDelegateType(int param0);
		public static SetCsharpExceptionSystemNullReferenceExceptionDelegateType SetCsharpExceptionSystemNullReferenceException;
		/*END CPP DELEGATES*/
#endif

#if UNITY_EDITOR_OSX || UNITY_EDITOR_LINUX
		[DllImport("__Internal", CallingConvention = CallingConvention.Cdecl)]
		static extern IntPtr dlopen(
			string path,
			int flag);

		[DllImport("__Internal", CallingConvention = CallingConvention.Cdecl)]
		static extern IntPtr dlsym(
			IntPtr handle,
			string symbolName);

		[DllImport("__Internal", CallingConvention = CallingConvention.Cdecl)]
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
		[DllImport("kernel32", SetLastError=true, CharSet = CharSet.Ansi)]
		static extern IntPtr LoadLibrary(
			string path);
		
		[DllImport("kernel32", CharSet=CharSet.Ansi, ExactSpelling=true, SetLastError=true)]
		static extern IntPtr GetProcAddress(
			IntPtr libraryHandle,
			string symbolName);
		
		[DllImport("kernel32.dll", SetLastError=true)]
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
		[DllImport(PLUGIN_NAME, CallingConvention = CallingConvention.Cdecl)]
		static extern void Init(
			IntPtr memory,
			int memorySize,
			InitMode initMode);
		
		[DllImport(PLUGIN_NAME, CallingConvention = CallingConvention.Cdecl)]
		static extern void SetCsharpException(int handle);
		
		/*BEGIN IMPORTS*/
		[DllImport(PLUGIN_NAME, CallingConvention = CallingConvention.Cdecl)]
		public static extern int NewBaseBallScript(int thisHandle);
		
		[DllImport(PLUGIN_NAME, CallingConvention = CallingConvention.Cdecl)]
		public static extern void DestroyBaseBallScript(int thisHandle);
		
		[DllImport(PLUGIN_NAME, CallingConvention = CallingConvention.Cdecl)]
		public static extern void MyGameAbstractBaseBallScriptUpdate(int thisHandle);
		
		[DllImport(PLUGIN_NAME, CallingConvention = CallingConvention.Cdecl)]
		public static extern void SetCsharpExceptionSystemNullReferenceException(int thisHandle);
		/*END IMPORTS*/
#endif
		
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		delegate void ReleaseObjectDelegateType(int handle);
		
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		delegate int StringNewDelegateType(string chars);
		
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		delegate void SetExceptionDelegateType(int handle);
		
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		delegate int ArrayGetLengthDelegateType(int handle);
		
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		delegate int EnumerableGetEnumeratorDelegateType(int handle);
		
		/*BEGIN DELEGATE TYPES*/
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		delegate void ReleaseSystemDecimalDelegateType(int handle);
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		delegate int SystemDecimalConstructorSystemDoubleDelegateType(double value);
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		delegate int SystemDecimalConstructorSystemUInt64DelegateType(ulong value);
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		delegate int BoxDecimalDelegateType(int valHandle);
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		delegate int UnboxDecimalDelegateType(int valHandle);
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		delegate UnityEngine.Vector3 UnityEngineVector3ConstructorSystemSingle_SystemSingle_SystemSingleDelegateType(float x, float y, float z);
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		delegate UnityEngine.Vector3 UnityEngineVector3Methodop_AdditionUnityEngineVector3_UnityEngineVector3DelegateType(ref UnityEngine.Vector3 a, ref UnityEngine.Vector3 b);
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		delegate int BoxVector3DelegateType(ref UnityEngine.Vector3 val);
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		delegate UnityEngine.Vector3 UnboxVector3DelegateType(int valHandle);
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		delegate int UnityEngineObjectPropertyGetNameDelegateType(int thisHandle);
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		delegate void UnityEngineObjectPropertySetNameDelegateType(int thisHandle, int valueHandle);
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		delegate int UnityEngineComponentPropertyGetTransformDelegateType(int thisHandle);
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		delegate UnityEngine.Vector3 UnityEngineTransformPropertyGetPositionDelegateType(int thisHandle);
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		delegate void UnityEngineTransformPropertySetPositionDelegateType(int thisHandle, ref UnityEngine.Vector3 value);
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		delegate int SystemCollectionsIEnumeratorPropertyGetCurrentDelegateType(int thisHandle);
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		delegate bool SystemCollectionsIEnumeratorMethodMoveNextDelegateType(int thisHandle);
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		delegate int UnityEngineGameObjectMethodAddComponentMyGameBaseBallScriptDelegateType(int thisHandle);
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		delegate int UnityEngineGameObjectMethodCreatePrimitiveUnityEnginePrimitiveTypeDelegateType(UnityEngine.PrimitiveType type);
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		delegate void UnityEngineDebugMethodLogSystemObjectDelegateType(int messageHandle);
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		delegate int UnityEngineMonoBehaviourPropertyGetTransformDelegateType(int thisHandle);
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		delegate int SystemExceptionConstructorSystemStringDelegateType(int messageHandle);
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		delegate int BoxPrimitiveTypeDelegateType(UnityEngine.PrimitiveType val);
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		delegate UnityEngine.PrimitiveType UnboxPrimitiveTypeDelegateType(int valHandle);
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		delegate float UnityEngineTimePropertyGetDeltaTimeDelegateType();
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		delegate void BaseBallScriptConstructorDelegateType(int cppHandle, ref int handle);
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		delegate void ReleaseBaseBallScriptDelegateType(int handle);
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		delegate int BoxBooleanDelegateType(bool val);
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		delegate bool UnboxBooleanDelegateType(int valHandle);
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		delegate int BoxSByteDelegateType(sbyte val);
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		delegate sbyte UnboxSByteDelegateType(int valHandle);
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		delegate int BoxByteDelegateType(byte val);
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		delegate byte UnboxByteDelegateType(int valHandle);
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		delegate int BoxInt16DelegateType(short val);
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		delegate short UnboxInt16DelegateType(int valHandle);
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		delegate int BoxUInt16DelegateType(ushort val);
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		delegate ushort UnboxUInt16DelegateType(int valHandle);
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		delegate int BoxInt32DelegateType(int val);
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		delegate int UnboxInt32DelegateType(int valHandle);
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		delegate int BoxUInt32DelegateType(uint val);
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		delegate uint UnboxUInt32DelegateType(int valHandle);
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		delegate int BoxInt64DelegateType(long val);
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		delegate long UnboxInt64DelegateType(int valHandle);
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		delegate int BoxUInt64DelegateType(ulong val);
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		delegate ulong UnboxUInt64DelegateType(int valHandle);
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		delegate int BoxCharDelegateType(char val);
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		delegate char UnboxCharDelegateType(int valHandle);
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		delegate int BoxSingleDelegateType(float val);
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		delegate float UnboxSingleDelegateType(int valHandle);
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		delegate int BoxDoubleDelegateType(double val);
		[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
		delegate double UnboxDoubleDelegateType(int valHandle);
		/*END DELEGATE TYPES*/

#if UNITY_EDITOR_WIN
		private static readonly string pluginTempPath = Application.dataPath + PLUGIN_TEMP_PATH;
#endif
		public static Exception UnhandledCppException;
#if UNITY_EDITOR
		private static readonly string pluginPath = Application.dataPath + PLUGIN_PATH;
		public static SetCsharpExceptionDelegate SetCsharpException;
#endif
		static IntPtr memory;
		static int memorySize;
		static DestroyEntry[] destroyQueue;
		static int destroyQueueCount;
		static int destroyQueueCapacity;
		static object destroyQueueLockObj;
		
		// Fixed delegates
		static readonly ReleaseObjectDelegateType ReleaseObjectDelegate = new ReleaseObjectDelegateType(ReleaseObject);
		static readonly StringNewDelegateType StringNewDelegate = new StringNewDelegateType(StringNew);
		static readonly SetExceptionDelegateType SetExceptionDelegate = new SetExceptionDelegateType(SetException);
		static readonly ArrayGetLengthDelegateType ArrayGetLengthDelegate = new ArrayGetLengthDelegateType(ArrayGetLength);
		static readonly EnumerableGetEnumeratorDelegateType EnumerableGetEnumeratorDelegate = new EnumerableGetEnumeratorDelegateType(EnumerableGetEnumerator);
		
		// Generated delegates
		/*BEGIN CSHARP DELEGATES*/
		static readonly ReleaseSystemDecimalDelegateType ReleaseSystemDecimalDelegate = new ReleaseSystemDecimalDelegateType(ReleaseSystemDecimal);
		static readonly SystemDecimalConstructorSystemDoubleDelegateType SystemDecimalConstructorSystemDoubleDelegate = new SystemDecimalConstructorSystemDoubleDelegateType(SystemDecimalConstructorSystemDouble);
		static readonly SystemDecimalConstructorSystemUInt64DelegateType SystemDecimalConstructorSystemUInt64Delegate = new SystemDecimalConstructorSystemUInt64DelegateType(SystemDecimalConstructorSystemUInt64);
		static readonly BoxDecimalDelegateType BoxDecimalDelegate = new BoxDecimalDelegateType(BoxDecimal);
		static readonly UnboxDecimalDelegateType UnboxDecimalDelegate = new UnboxDecimalDelegateType(UnboxDecimal);
		static readonly UnityEngineVector3ConstructorSystemSingle_SystemSingle_SystemSingleDelegateType UnityEngineVector3ConstructorSystemSingle_SystemSingle_SystemSingleDelegate = new UnityEngineVector3ConstructorSystemSingle_SystemSingle_SystemSingleDelegateType(UnityEngineVector3ConstructorSystemSingle_SystemSingle_SystemSingle);
		static readonly UnityEngineVector3Methodop_AdditionUnityEngineVector3_UnityEngineVector3DelegateType UnityEngineVector3Methodop_AdditionUnityEngineVector3_UnityEngineVector3Delegate = new UnityEngineVector3Methodop_AdditionUnityEngineVector3_UnityEngineVector3DelegateType(UnityEngineVector3Methodop_AdditionUnityEngineVector3_UnityEngineVector3);
		static readonly BoxVector3DelegateType BoxVector3Delegate = new BoxVector3DelegateType(BoxVector3);
		static readonly UnboxVector3DelegateType UnboxVector3Delegate = new UnboxVector3DelegateType(UnboxVector3);
		static readonly UnityEngineObjectPropertyGetNameDelegateType UnityEngineObjectPropertyGetNameDelegate = new UnityEngineObjectPropertyGetNameDelegateType(UnityEngineObjectPropertyGetName);
		static readonly UnityEngineObjectPropertySetNameDelegateType UnityEngineObjectPropertySetNameDelegate = new UnityEngineObjectPropertySetNameDelegateType(UnityEngineObjectPropertySetName);
		static readonly UnityEngineComponentPropertyGetTransformDelegateType UnityEngineComponentPropertyGetTransformDelegate = new UnityEngineComponentPropertyGetTransformDelegateType(UnityEngineComponentPropertyGetTransform);
		static readonly UnityEngineTransformPropertyGetPositionDelegateType UnityEngineTransformPropertyGetPositionDelegate = new UnityEngineTransformPropertyGetPositionDelegateType(UnityEngineTransformPropertyGetPosition);
		static readonly UnityEngineTransformPropertySetPositionDelegateType UnityEngineTransformPropertySetPositionDelegate = new UnityEngineTransformPropertySetPositionDelegateType(UnityEngineTransformPropertySetPosition);
		static readonly SystemCollectionsIEnumeratorPropertyGetCurrentDelegateType SystemCollectionsIEnumeratorPropertyGetCurrentDelegate = new SystemCollectionsIEnumeratorPropertyGetCurrentDelegateType(SystemCollectionsIEnumeratorPropertyGetCurrent);
		static readonly SystemCollectionsIEnumeratorMethodMoveNextDelegateType SystemCollectionsIEnumeratorMethodMoveNextDelegate = new SystemCollectionsIEnumeratorMethodMoveNextDelegateType(SystemCollectionsIEnumeratorMethodMoveNext);
		static readonly UnityEngineGameObjectMethodAddComponentMyGameBaseBallScriptDelegateType UnityEngineGameObjectMethodAddComponentMyGameBaseBallScriptDelegate = new UnityEngineGameObjectMethodAddComponentMyGameBaseBallScriptDelegateType(UnityEngineGameObjectMethodAddComponentMyGameBaseBallScript);
		static readonly UnityEngineGameObjectMethodCreatePrimitiveUnityEnginePrimitiveTypeDelegateType UnityEngineGameObjectMethodCreatePrimitiveUnityEnginePrimitiveTypeDelegate = new UnityEngineGameObjectMethodCreatePrimitiveUnityEnginePrimitiveTypeDelegateType(UnityEngineGameObjectMethodCreatePrimitiveUnityEnginePrimitiveType);
		static readonly UnityEngineDebugMethodLogSystemObjectDelegateType UnityEngineDebugMethodLogSystemObjectDelegate = new UnityEngineDebugMethodLogSystemObjectDelegateType(UnityEngineDebugMethodLogSystemObject);
		static readonly UnityEngineMonoBehaviourPropertyGetTransformDelegateType UnityEngineMonoBehaviourPropertyGetTransformDelegate = new UnityEngineMonoBehaviourPropertyGetTransformDelegateType(UnityEngineMonoBehaviourPropertyGetTransform);
		static readonly SystemExceptionConstructorSystemStringDelegateType SystemExceptionConstructorSystemStringDelegate = new SystemExceptionConstructorSystemStringDelegateType(SystemExceptionConstructorSystemString);
		static readonly BoxPrimitiveTypeDelegateType BoxPrimitiveTypeDelegate = new BoxPrimitiveTypeDelegateType(BoxPrimitiveType);
		static readonly UnboxPrimitiveTypeDelegateType UnboxPrimitiveTypeDelegate = new UnboxPrimitiveTypeDelegateType(UnboxPrimitiveType);
		static readonly UnityEngineTimePropertyGetDeltaTimeDelegateType UnityEngineTimePropertyGetDeltaTimeDelegate = new UnityEngineTimePropertyGetDeltaTimeDelegateType(UnityEngineTimePropertyGetDeltaTime);
		static readonly ReleaseBaseBallScriptDelegateType ReleaseBaseBallScriptDelegate = new ReleaseBaseBallScriptDelegateType(ReleaseBaseBallScript);
		static readonly BaseBallScriptConstructorDelegateType BaseBallScriptConstructorDelegate = new BaseBallScriptConstructorDelegateType(BaseBallScriptConstructor);
		static readonly BoxBooleanDelegateType BoxBooleanDelegate = new BoxBooleanDelegateType(BoxBoolean);
		static readonly UnboxBooleanDelegateType UnboxBooleanDelegate = new UnboxBooleanDelegateType(UnboxBoolean);
		static readonly BoxSByteDelegateType BoxSByteDelegate = new BoxSByteDelegateType(BoxSByte);
		static readonly UnboxSByteDelegateType UnboxSByteDelegate = new UnboxSByteDelegateType(UnboxSByte);
		static readonly BoxByteDelegateType BoxByteDelegate = new BoxByteDelegateType(BoxByte);
		static readonly UnboxByteDelegateType UnboxByteDelegate = new UnboxByteDelegateType(UnboxByte);
		static readonly BoxInt16DelegateType BoxInt16Delegate = new BoxInt16DelegateType(BoxInt16);
		static readonly UnboxInt16DelegateType UnboxInt16Delegate = new UnboxInt16DelegateType(UnboxInt16);
		static readonly BoxUInt16DelegateType BoxUInt16Delegate = new BoxUInt16DelegateType(BoxUInt16);
		static readonly UnboxUInt16DelegateType UnboxUInt16Delegate = new UnboxUInt16DelegateType(UnboxUInt16);
		static readonly BoxInt32DelegateType BoxInt32Delegate = new BoxInt32DelegateType(BoxInt32);
		static readonly UnboxInt32DelegateType UnboxInt32Delegate = new UnboxInt32DelegateType(UnboxInt32);
		static readonly BoxUInt32DelegateType BoxUInt32Delegate = new BoxUInt32DelegateType(BoxUInt32);
		static readonly UnboxUInt32DelegateType UnboxUInt32Delegate = new UnboxUInt32DelegateType(UnboxUInt32);
		static readonly BoxInt64DelegateType BoxInt64Delegate = new BoxInt64DelegateType(BoxInt64);
		static readonly UnboxInt64DelegateType UnboxInt64Delegate = new UnboxInt64DelegateType(UnboxInt64);
		static readonly BoxUInt64DelegateType BoxUInt64Delegate = new BoxUInt64DelegateType(BoxUInt64);
		static readonly UnboxUInt64DelegateType UnboxUInt64Delegate = new UnboxUInt64DelegateType(UnboxUInt64);
		static readonly BoxCharDelegateType BoxCharDelegate = new BoxCharDelegateType(BoxChar);
		static readonly UnboxCharDelegateType UnboxCharDelegate = new UnboxCharDelegateType(UnboxChar);
		static readonly BoxSingleDelegateType BoxSingleDelegate = new BoxSingleDelegateType(BoxSingle);
		static readonly UnboxSingleDelegateType UnboxSingleDelegate = new UnboxSingleDelegateType(UnboxSingle);
		static readonly BoxDoubleDelegateType BoxDoubleDelegate = new BoxDoubleDelegateType(BoxDouble);
		static readonly UnboxDoubleDelegateType UnboxDoubleDelegate = new UnboxDoubleDelegateType(UnboxDouble);
		/*END CSHARP DELEGATES*/
		
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
			NativeScript.Bindings.StructStore<System.Decimal>.Init(1000);
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
			File.Copy(pluginPath, pluginTempPath, true);
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
			NewBaseBallScript = GetDelegate<NewBaseBallScriptDelegateType>(libraryHandle, "NewBaseBallScript");
			DestroyBaseBallScript = GetDelegate<DestroyBaseBallScriptDelegateType>(libraryHandle, "DestroyBaseBallScript");
			MyGameAbstractBaseBallScriptUpdate = GetDelegate<MyGameAbstractBaseBallScriptUpdateDelegateType>(libraryHandle, "MyGameAbstractBaseBallScriptUpdate");
			SetCsharpExceptionSystemNullReferenceException = GetDelegate<SetCsharpExceptionSystemNullReferenceExceptionDelegateType>(libraryHandle, "SetCsharpExceptionSystemNullReferenceException");
			/*END GETDELEGATE CALLS*/
#endif
			// Pass parameters through 'memory'
			int curMemory = 0;
			Marshal.WriteIntPtr(
				memory,
				curMemory,
				Marshal.GetFunctionPointerForDelegate(ReleaseObjectDelegate));
			curMemory += IntPtr.Size;
			Marshal.WriteIntPtr(
				memory,
				curMemory,
				Marshal.GetFunctionPointerForDelegate(StringNewDelegate));
			curMemory += IntPtr.Size;
			Marshal.WriteIntPtr(
				memory,
				curMemory,
				Marshal.GetFunctionPointerForDelegate(SetExceptionDelegate));
			curMemory += IntPtr.Size;
			Marshal.WriteIntPtr(
				memory,
				curMemory,
				Marshal.GetFunctionPointerForDelegate(ArrayGetLengthDelegate));
			curMemory += IntPtr.Size;
			Marshal.WriteIntPtr(
				memory,
				curMemory,
				Marshal.GetFunctionPointerForDelegate(EnumerableGetEnumeratorDelegate));
			curMemory += IntPtr.Size;
			
			/*BEGIN INIT CALL*/
			Marshal.WriteInt32(memory, curMemory, 1000); // max managed objects
			curMemory += sizeof(int);
 			Marshal.WriteIntPtr(memory, curMemory, Marshal.GetFunctionPointerForDelegate(ReleaseSystemDecimalDelegate));
			curMemory += IntPtr.Size;
			Marshal.WriteIntPtr(memory, curMemory, Marshal.GetFunctionPointerForDelegate(SystemDecimalConstructorSystemDoubleDelegate));
			curMemory += IntPtr.Size;
			Marshal.WriteIntPtr(memory, curMemory, Marshal.GetFunctionPointerForDelegate(SystemDecimalConstructorSystemUInt64Delegate));
			curMemory += IntPtr.Size;
			Marshal.WriteIntPtr(memory, curMemory, Marshal.GetFunctionPointerForDelegate(BoxDecimalDelegate));
			curMemory += IntPtr.Size;
			Marshal.WriteIntPtr(memory, curMemory, Marshal.GetFunctionPointerForDelegate(UnboxDecimalDelegate));
			curMemory += IntPtr.Size;
			Marshal.WriteIntPtr(memory, curMemory, Marshal.GetFunctionPointerForDelegate(UnityEngineVector3ConstructorSystemSingle_SystemSingle_SystemSingleDelegate));
			curMemory += IntPtr.Size;
			Marshal.WriteIntPtr(memory, curMemory, Marshal.GetFunctionPointerForDelegate(UnityEngineVector3Methodop_AdditionUnityEngineVector3_UnityEngineVector3Delegate));
			curMemory += IntPtr.Size;
			Marshal.WriteIntPtr(memory, curMemory, Marshal.GetFunctionPointerForDelegate(BoxVector3Delegate));
			curMemory += IntPtr.Size;
			Marshal.WriteIntPtr(memory, curMemory, Marshal.GetFunctionPointerForDelegate(UnboxVector3Delegate));
			curMemory += IntPtr.Size;
			Marshal.WriteIntPtr(memory, curMemory, Marshal.GetFunctionPointerForDelegate(UnityEngineObjectPropertyGetNameDelegate));
			curMemory += IntPtr.Size;
			Marshal.WriteIntPtr(memory, curMemory, Marshal.GetFunctionPointerForDelegate(UnityEngineObjectPropertySetNameDelegate));
			curMemory += IntPtr.Size;
			Marshal.WriteIntPtr(memory, curMemory, Marshal.GetFunctionPointerForDelegate(UnityEngineComponentPropertyGetTransformDelegate));
			curMemory += IntPtr.Size;
			Marshal.WriteIntPtr(memory, curMemory, Marshal.GetFunctionPointerForDelegate(UnityEngineTransformPropertyGetPositionDelegate));
			curMemory += IntPtr.Size;
			Marshal.WriteIntPtr(memory, curMemory, Marshal.GetFunctionPointerForDelegate(UnityEngineTransformPropertySetPositionDelegate));
			curMemory += IntPtr.Size;
			Marshal.WriteIntPtr(memory, curMemory, Marshal.GetFunctionPointerForDelegate(SystemCollectionsIEnumeratorPropertyGetCurrentDelegate));
			curMemory += IntPtr.Size;
			Marshal.WriteIntPtr(memory, curMemory, Marshal.GetFunctionPointerForDelegate(SystemCollectionsIEnumeratorMethodMoveNextDelegate));
			curMemory += IntPtr.Size;
			Marshal.WriteIntPtr(memory, curMemory, Marshal.GetFunctionPointerForDelegate(UnityEngineGameObjectMethodAddComponentMyGameBaseBallScriptDelegate));
			curMemory += IntPtr.Size;
			Marshal.WriteIntPtr(memory, curMemory, Marshal.GetFunctionPointerForDelegate(UnityEngineGameObjectMethodCreatePrimitiveUnityEnginePrimitiveTypeDelegate));
			curMemory += IntPtr.Size;
			Marshal.WriteIntPtr(memory, curMemory, Marshal.GetFunctionPointerForDelegate(UnityEngineDebugMethodLogSystemObjectDelegate));
			curMemory += IntPtr.Size;
			Marshal.WriteIntPtr(memory, curMemory, Marshal.GetFunctionPointerForDelegate(UnityEngineMonoBehaviourPropertyGetTransformDelegate));
			curMemory += IntPtr.Size;
			Marshal.WriteIntPtr(memory, curMemory, Marshal.GetFunctionPointerForDelegate(SystemExceptionConstructorSystemStringDelegate));
			curMemory += IntPtr.Size;
			Marshal.WriteIntPtr(memory, curMemory, Marshal.GetFunctionPointerForDelegate(BoxPrimitiveTypeDelegate));
			curMemory += IntPtr.Size;
			Marshal.WriteIntPtr(memory, curMemory, Marshal.GetFunctionPointerForDelegate(UnboxPrimitiveTypeDelegate));
			curMemory += IntPtr.Size;
			Marshal.WriteIntPtr(memory, curMemory, Marshal.GetFunctionPointerForDelegate(UnityEngineTimePropertyGetDeltaTimeDelegate));
			curMemory += IntPtr.Size;
			Marshal.WriteIntPtr(memory, curMemory, Marshal.GetFunctionPointerForDelegate(ReleaseBaseBallScriptDelegate));
			curMemory += IntPtr.Size;
			Marshal.WriteIntPtr(memory, curMemory, Marshal.GetFunctionPointerForDelegate(BaseBallScriptConstructorDelegate));
			curMemory += IntPtr.Size;
			Marshal.WriteIntPtr(memory, curMemory, Marshal.GetFunctionPointerForDelegate(BoxBooleanDelegate));
			curMemory += IntPtr.Size;
			Marshal.WriteIntPtr(memory, curMemory, Marshal.GetFunctionPointerForDelegate(UnboxBooleanDelegate));
			curMemory += IntPtr.Size;
			Marshal.WriteIntPtr(memory, curMemory, Marshal.GetFunctionPointerForDelegate(BoxSByteDelegate));
			curMemory += IntPtr.Size;
			Marshal.WriteIntPtr(memory, curMemory, Marshal.GetFunctionPointerForDelegate(UnboxSByteDelegate));
			curMemory += IntPtr.Size;
			Marshal.WriteIntPtr(memory, curMemory, Marshal.GetFunctionPointerForDelegate(BoxByteDelegate));
			curMemory += IntPtr.Size;
			Marshal.WriteIntPtr(memory, curMemory, Marshal.GetFunctionPointerForDelegate(UnboxByteDelegate));
			curMemory += IntPtr.Size;
			Marshal.WriteIntPtr(memory, curMemory, Marshal.GetFunctionPointerForDelegate(BoxInt16Delegate));
			curMemory += IntPtr.Size;
			Marshal.WriteIntPtr(memory, curMemory, Marshal.GetFunctionPointerForDelegate(UnboxInt16Delegate));
			curMemory += IntPtr.Size;
			Marshal.WriteIntPtr(memory, curMemory, Marshal.GetFunctionPointerForDelegate(BoxUInt16Delegate));
			curMemory += IntPtr.Size;
			Marshal.WriteIntPtr(memory, curMemory, Marshal.GetFunctionPointerForDelegate(UnboxUInt16Delegate));
			curMemory += IntPtr.Size;
			Marshal.WriteIntPtr(memory, curMemory, Marshal.GetFunctionPointerForDelegate(BoxInt32Delegate));
			curMemory += IntPtr.Size;
			Marshal.WriteIntPtr(memory, curMemory, Marshal.GetFunctionPointerForDelegate(UnboxInt32Delegate));
			curMemory += IntPtr.Size;
			Marshal.WriteIntPtr(memory, curMemory, Marshal.GetFunctionPointerForDelegate(BoxUInt32Delegate));
			curMemory += IntPtr.Size;
			Marshal.WriteIntPtr(memory, curMemory, Marshal.GetFunctionPointerForDelegate(UnboxUInt32Delegate));
			curMemory += IntPtr.Size;
			Marshal.WriteIntPtr(memory, curMemory, Marshal.GetFunctionPointerForDelegate(BoxInt64Delegate));
			curMemory += IntPtr.Size;
			Marshal.WriteIntPtr(memory, curMemory, Marshal.GetFunctionPointerForDelegate(UnboxInt64Delegate));
			curMemory += IntPtr.Size;
			Marshal.WriteIntPtr(memory, curMemory, Marshal.GetFunctionPointerForDelegate(BoxUInt64Delegate));
			curMemory += IntPtr.Size;
			Marshal.WriteIntPtr(memory, curMemory, Marshal.GetFunctionPointerForDelegate(UnboxUInt64Delegate));
			curMemory += IntPtr.Size;
			Marshal.WriteIntPtr(memory, curMemory, Marshal.GetFunctionPointerForDelegate(BoxCharDelegate));
			curMemory += IntPtr.Size;
			Marshal.WriteIntPtr(memory, curMemory, Marshal.GetFunctionPointerForDelegate(UnboxCharDelegate));
			curMemory += IntPtr.Size;
			Marshal.WriteIntPtr(memory, curMemory, Marshal.GetFunctionPointerForDelegate(BoxSingleDelegate));
			curMemory += IntPtr.Size;
			Marshal.WriteIntPtr(memory, curMemory, Marshal.GetFunctionPointerForDelegate(UnboxSingleDelegate));
			curMemory += IntPtr.Size;
			Marshal.WriteIntPtr(memory, curMemory, Marshal.GetFunctionPointerForDelegate(BoxDoubleDelegate));
			curMemory += IntPtr.Size;
			Marshal.WriteIntPtr(memory, curMemory, Marshal.GetFunctionPointerForDelegate(UnboxDoubleDelegate));
			curMemory += IntPtr.Size;
			/*END INIT CALL*/
			
			// Init C++ library
			Init(memory, memorySize, initMode);
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
		
		[MonoPInvokeCallback(typeof(ReleaseObjectDelegateType))]
		static void ReleaseObject(
			int handle)
		{
			if (handle != 0)
			{
				ObjectStore.Remove(handle);
			}
		}
		
		[MonoPInvokeCallback(typeof(StringNewDelegateType))]
		static int StringNew(
			string chars)
		{
			int handle = ObjectStore.Store(chars);
			return handle;
		}
		
		[MonoPInvokeCallback(typeof(SetExceptionDelegateType))]
		static void SetException(int handle)
		{
			UnhandledCppException = ObjectStore.Get(handle) as Exception;
		}
		
		[MonoPInvokeCallback(typeof(ArrayGetLengthDelegateType))]
		static int ArrayGetLength(int handle)
		{
			return ((Array)ObjectStore.Get(handle)).Length;
		}
		
		[MonoPInvokeCallback(typeof(EnumerableGetEnumeratorDelegateType))]
		static int EnumerableGetEnumerator(int handle)
		{
			return ObjectStore.Store(((IEnumerable)ObjectStore.Get(handle)).GetEnumerator());
		}

		/*BEGIN FUNCTIONS*/
		[MonoPInvokeCallback(typeof(ReleaseSystemDecimalDelegateType))]
		static void ReleaseSystemDecimal(int handle)
		{
			try
			{
				if (handle != 0)
			{
				NativeScript.Bindings.StructStore<System.Decimal>.Remove(handle);
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
		
		[MonoPInvokeCallback(typeof(SystemDecimalConstructorSystemDoubleDelegateType))]
		static int SystemDecimalConstructorSystemDouble(double value)
		{
			try
			{
				var returnValue = NativeScript.Bindings.StructStore<System.Decimal>.Store(new System.Decimal(value));
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
		
		[MonoPInvokeCallback(typeof(SystemDecimalConstructorSystemUInt64DelegateType))]
		static int SystemDecimalConstructorSystemUInt64(ulong value)
		{
			try
			{
				var returnValue = NativeScript.Bindings.StructStore<System.Decimal>.Store(new System.Decimal(value));
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
		
		[MonoPInvokeCallback(typeof(BoxDecimalDelegateType))]
		static int BoxDecimal(int valHandle)
		{
			try
			{
				var val = (System.Decimal)NativeScript.Bindings.StructStore<System.Decimal>.Get(valHandle);
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
		
		[MonoPInvokeCallback(typeof(UnboxDecimalDelegateType))]
		static int UnboxDecimal(int valHandle)
		{
			try
			{
				var val = NativeScript.Bindings.ObjectStore.Get(valHandle);
				var returnValue = NativeScript.Bindings.StructStore<System.Decimal>.Store((System.Decimal)val);
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
		
		[MonoPInvokeCallback(typeof(UnityEngineVector3ConstructorSystemSingle_SystemSingle_SystemSingleDelegateType))]
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
		
		[MonoPInvokeCallback(typeof(UnityEngineVector3Methodop_AdditionUnityEngineVector3_UnityEngineVector3DelegateType))]
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
		
		[MonoPInvokeCallback(typeof(BoxVector3DelegateType))]
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
		
		[MonoPInvokeCallback(typeof(UnboxVector3DelegateType))]
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
		
		[MonoPInvokeCallback(typeof(UnityEngineObjectPropertyGetNameDelegateType))]
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
		
		[MonoPInvokeCallback(typeof(UnityEngineObjectPropertySetNameDelegateType))]
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
		
		[MonoPInvokeCallback(typeof(UnityEngineComponentPropertyGetTransformDelegateType))]
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
		
		[MonoPInvokeCallback(typeof(UnityEngineTransformPropertyGetPositionDelegateType))]
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
		
		[MonoPInvokeCallback(typeof(UnityEngineTransformPropertySetPositionDelegateType))]
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
		
		[MonoPInvokeCallback(typeof(SystemCollectionsIEnumeratorPropertyGetCurrentDelegateType))]
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
		
		[MonoPInvokeCallback(typeof(SystemCollectionsIEnumeratorMethodMoveNextDelegateType))]
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
		
		[MonoPInvokeCallback(typeof(UnityEngineGameObjectMethodAddComponentMyGameBaseBallScriptDelegateType))]
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
		
		[MonoPInvokeCallback(typeof(UnityEngineGameObjectMethodCreatePrimitiveUnityEnginePrimitiveTypeDelegateType))]
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
		
		[MonoPInvokeCallback(typeof(UnityEngineDebugMethodLogSystemObjectDelegateType))]
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
		
		[MonoPInvokeCallback(typeof(UnityEngineMonoBehaviourPropertyGetTransformDelegateType))]
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
		
		[MonoPInvokeCallback(typeof(SystemExceptionConstructorSystemStringDelegateType))]
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
		
		[MonoPInvokeCallback(typeof(BoxPrimitiveTypeDelegateType))]
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
		
		[MonoPInvokeCallback(typeof(UnboxPrimitiveTypeDelegateType))]
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
		
		[MonoPInvokeCallback(typeof(UnityEngineTimePropertyGetDeltaTimeDelegateType))]
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
		
		[MonoPInvokeCallback(typeof(BaseBallScriptConstructorDelegateType))]
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
		
		[MonoPInvokeCallback(typeof(ReleaseBaseBallScriptDelegateType))]
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
		
		[MonoPInvokeCallback(typeof(BoxBooleanDelegateType))]
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
		
		[MonoPInvokeCallback(typeof(UnboxBooleanDelegateType))]
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
		
		[MonoPInvokeCallback(typeof(BoxSByteDelegateType))]
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
		
		[MonoPInvokeCallback(typeof(UnboxSByteDelegateType))]
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
		
		[MonoPInvokeCallback(typeof(BoxByteDelegateType))]
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
		
		[MonoPInvokeCallback(typeof(UnboxByteDelegateType))]
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
		
		[MonoPInvokeCallback(typeof(BoxInt16DelegateType))]
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
		
		[MonoPInvokeCallback(typeof(UnboxInt16DelegateType))]
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
		
		[MonoPInvokeCallback(typeof(BoxUInt16DelegateType))]
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
		
		[MonoPInvokeCallback(typeof(UnboxUInt16DelegateType))]
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
		
		[MonoPInvokeCallback(typeof(BoxInt32DelegateType))]
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
		
		[MonoPInvokeCallback(typeof(UnboxInt32DelegateType))]
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
		
		[MonoPInvokeCallback(typeof(BoxUInt32DelegateType))]
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
		
		[MonoPInvokeCallback(typeof(UnboxUInt32DelegateType))]
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
		
		[MonoPInvokeCallback(typeof(BoxInt64DelegateType))]
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
		
		[MonoPInvokeCallback(typeof(UnboxInt64DelegateType))]
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
		
		[MonoPInvokeCallback(typeof(BoxUInt64DelegateType))]
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
		
		[MonoPInvokeCallback(typeof(UnboxUInt64DelegateType))]
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
		
		[MonoPInvokeCallback(typeof(BoxCharDelegateType))]
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
		
		[MonoPInvokeCallback(typeof(UnboxCharDelegateType))]
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
		
		[MonoPInvokeCallback(typeof(BoxSingleDelegateType))]
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
		
		[MonoPInvokeCallback(typeof(UnboxSingleDelegateType))]
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
		
		[MonoPInvokeCallback(typeof(BoxDoubleDelegateType))]
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
		
		[MonoPInvokeCallback(typeof(UnboxDoubleDelegateType))]
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
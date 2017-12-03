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
			IntPtr boxVector3,
			IntPtr unboxVector3,
			IntPtr unityEngineMatrix4x4PropertyGetItem,
			IntPtr unityEngineMatrix4x4PropertySetItem,
			IntPtr boxMatrix4x4,
			IntPtr unboxMatrix4x4,
			IntPtr releaseUnityEngineRaycastHit,
			IntPtr unityEngineRaycastHitPropertyGetPoint,
			IntPtr unityEngineRaycastHitPropertySetPoint,
			IntPtr unityEngineRaycastHitPropertyGetTransform,
			IntPtr boxRaycastHit,
			IntPtr unboxRaycastHit,
			IntPtr boxQueryTriggerInteraction,
			IntPtr unboxQueryTriggerInteraction,
			IntPtr releaseSystemCollectionsGenericKeyValuePairSystemString_SystemDouble,
			IntPtr systemCollectionsGenericKeyValuePairSystemString_SystemDoubleConstructorSystemString_SystemDouble,
			IntPtr systemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetKey,
			IntPtr systemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetValue,
			IntPtr boxKeyValuePairSystemString_SystemDouble,
			IntPtr unboxKeyValuePairSystemString_SystemDouble,
			IntPtr systemCollectionsGenericListSystemStringConstructor,
			IntPtr systemCollectionsGenericListSystemStringPropertyGetItem,
			IntPtr systemCollectionsGenericListSystemStringPropertySetItem,
			IntPtr systemCollectionsGenericListSystemStringMethodAddSystemString,
			IntPtr systemCollectionsGenericListSystemStringMethodSortSystemCollectionsGenericIComparer,
			IntPtr systemCollectionsGenericListSystemInt32Constructor,
			IntPtr systemCollectionsGenericListSystemInt32PropertyGetItem,
			IntPtr systemCollectionsGenericListSystemInt32PropertySetItem,
			IntPtr systemCollectionsGenericListSystemInt32MethodAddSystemInt32,
			IntPtr systemCollectionsGenericListSystemInt32MethodSortSystemCollectionsGenericIComparer,
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
			IntPtr boxResolution,
			IntPtr unboxResolution,
			IntPtr unityEngineScreenPropertyGetResolutions,
			IntPtr unityEngineRayConstructorUnityEngineVector3_UnityEngineVector3,
			IntPtr boxRay,
			IntPtr unboxRay,
			IntPtr unityEnginePhysicsMethodRaycastNonAllocUnityEngineRay_UnityEngineRaycastHit,
			IntPtr unityEnginePhysicsMethodRaycastAllUnityEngineRay,
			IntPtr boxColor,
			IntPtr unboxColor,
			IntPtr boxGradientColorKey,
			IntPtr unboxGradientColorKey,
			IntPtr unityEngineGradientConstructor,
			IntPtr unityEngineGradientPropertyGetColorKeys,
			IntPtr unityEngineGradientPropertySetColorKeys,
			IntPtr systemAppDomainSetupConstructor,
			IntPtr systemAppDomainSetupPropertyGetAppDomainInitializer,
			IntPtr systemAppDomainSetupPropertySetAppDomainInitializer,
			IntPtr unityEngineApplicationAddEventOnBeforeRender,
			IntPtr unityEngineApplicationRemoveEventOnBeforeRender,
			IntPtr unityEngineSceneManagementSceneManagerAddEventSceneLoaded,
			IntPtr unityEngineSceneManagementSceneManagerRemoveEventSceneLoaded,
			IntPtr boxScene,
			IntPtr unboxScene,
			IntPtr boxLoadSceneMode,
			IntPtr unboxLoadSceneMode,
			IntPtr systemCollectionsIEnumeratorPropertyGetCurrent,
			IntPtr systemCollectionsIEnumeratorMethodMoveNext,
			IntPtr releaseSystemCollectionsGenericIComparerSystemInt32,
			IntPtr systemCollectionsGenericIComparerSystemInt32Constructor,
			IntPtr releaseSystemCollectionsGenericIComparerSystemString,
			IntPtr systemCollectionsGenericIComparerSystemStringConstructor,
			IntPtr releaseSystemStringComparer,
			IntPtr systemStringComparerConstructor,
			IntPtr releaseSystemCollectionsICollection,
			IntPtr systemCollectionsICollectionConstructor,
			IntPtr releaseSystemCollectionsIList,
			IntPtr systemCollectionsIListConstructor,
			IntPtr releaseSystemCollectionsQueue,
			IntPtr systemCollectionsQueueConstructor,
			IntPtr releaseSystemComponentModelDesignIComponentChangeService,
			IntPtr systemComponentModelDesignIComponentChangeServiceConstructor,
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
			IntPtr unboxDouble,
			IntPtr systemSystemInt32Array1Constructor1,
			IntPtr systemInt32Array1GetItem1,
			IntPtr systemInt32Array1SetItem1,
			IntPtr systemSystemSingleArray1Constructor1,
			IntPtr systemSingleArray1GetItem1,
			IntPtr systemSingleArray1SetItem1,
			IntPtr systemSystemSingleArray2Constructor2,
			IntPtr systemSystemSingleArray2GetLength2,
			IntPtr systemSingleArray2GetItem2,
			IntPtr systemSingleArray2SetItem2,
			IntPtr systemSystemSingleArray3Constructor3,
			IntPtr systemSystemSingleArray3GetLength3,
			IntPtr systemSingleArray3GetItem3,
			IntPtr systemSingleArray3SetItem3,
			IntPtr systemSystemStringArray1Constructor1,
			IntPtr systemStringArray1GetItem1,
			IntPtr systemStringArray1SetItem1,
			IntPtr unityEngineUnityEngineResolutionArray1Constructor1,
			IntPtr unityEngineResolutionArray1GetItem1,
			IntPtr unityEngineResolutionArray1SetItem1,
			IntPtr unityEngineUnityEngineRaycastHitArray1Constructor1,
			IntPtr unityEngineRaycastHitArray1GetItem1,
			IntPtr unityEngineRaycastHitArray1SetItem1,
			IntPtr unityEngineUnityEngineGradientColorKeyArray1Constructor1,
			IntPtr unityEngineGradientColorKeyArray1GetItem1,
			IntPtr unityEngineGradientColorKeyArray1SetItem1,
			IntPtr releaseSystemAction,
			IntPtr systemActionConstructor,
			IntPtr systemActionAdd,
			IntPtr systemActionRemove,
			IntPtr systemActionInvoke,
			IntPtr releaseSystemActionSystemSingle,
			IntPtr systemActionSystemSingleConstructor,
			IntPtr systemActionSystemSingleAdd,
			IntPtr systemActionSystemSingleRemove,
			IntPtr systemActionSystemSingleInvoke,
			IntPtr releaseSystemActionSystemSingle_SystemSingle,
			IntPtr systemActionSystemSingle_SystemSingleConstructor,
			IntPtr systemActionSystemSingle_SystemSingleAdd,
			IntPtr systemActionSystemSingle_SystemSingleRemove,
			IntPtr systemActionSystemSingle_SystemSingleInvoke,
			IntPtr releaseSystemFuncSystemInt32_SystemSingle_SystemDouble,
			IntPtr systemFuncSystemInt32_SystemSingle_SystemDoubleConstructor,
			IntPtr systemFuncSystemInt32_SystemSingle_SystemDoubleAdd,
			IntPtr systemFuncSystemInt32_SystemSingle_SystemDoubleRemove,
			IntPtr systemFuncSystemInt32_SystemSingle_SystemDoubleInvoke,
			IntPtr releaseSystemFuncSystemInt16_SystemInt32_SystemString,
			IntPtr systemFuncSystemInt16_SystemInt32_SystemStringConstructor,
			IntPtr systemFuncSystemInt16_SystemInt32_SystemStringAdd,
			IntPtr systemFuncSystemInt16_SystemInt32_SystemStringRemove,
			IntPtr systemFuncSystemInt16_SystemInt32_SystemStringInvoke,
			IntPtr releaseSystemAppDomainInitializer,
			IntPtr systemAppDomainInitializerConstructor,
			IntPtr systemAppDomainInitializerAdd,
			IntPtr systemAppDomainInitializerRemove,
			IntPtr systemAppDomainInitializerInvoke,
			IntPtr releaseUnityEngineEventsUnityAction,
			IntPtr unityEngineEventsUnityActionConstructor,
			IntPtr unityEngineEventsUnityActionAdd,
			IntPtr unityEngineEventsUnityActionRemove,
			IntPtr unityEngineEventsUnityActionInvoke,
			IntPtr releaseUnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneMode,
			IntPtr unityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeConstructor,
			IntPtr unityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeAdd,
			IntPtr unityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeRemove,
			IntPtr unityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeInvoke,
			IntPtr releaseSystemComponentModelDesignComponentEventHandler,
			IntPtr systemComponentModelDesignComponentEventHandlerConstructor,
			IntPtr systemComponentModelDesignComponentEventHandlerAdd,
			IntPtr systemComponentModelDesignComponentEventHandlerRemove,
			IntPtr systemComponentModelDesignComponentEventHandlerInvoke,
			IntPtr releaseSystemComponentModelDesignComponentChangingEventHandler,
			IntPtr systemComponentModelDesignComponentChangingEventHandlerConstructor,
			IntPtr systemComponentModelDesignComponentChangingEventHandlerAdd,
			IntPtr systemComponentModelDesignComponentChangingEventHandlerRemove,
			IntPtr systemComponentModelDesignComponentChangingEventHandlerInvoke,
			IntPtr releaseSystemComponentModelDesignComponentChangedEventHandler,
			IntPtr systemComponentModelDesignComponentChangedEventHandlerConstructor,
			IntPtr systemComponentModelDesignComponentChangedEventHandlerAdd,
			IntPtr systemComponentModelDesignComponentChangedEventHandlerRemove,
			IntPtr systemComponentModelDesignComponentChangedEventHandlerInvoke,
			IntPtr releaseSystemComponentModelDesignComponentRenameEventHandler,
			IntPtr systemComponentModelDesignComponentRenameEventHandlerConstructor,
			IntPtr systemComponentModelDesignComponentRenameEventHandlerAdd,
			IntPtr systemComponentModelDesignComponentRenameEventHandlerRemove,
			IntPtr systemComponentModelDesignComponentRenameEventHandlerInvoke
			/*END INIT PARAMS*/);
		
		public delegate void SetCsharpExceptionDelegate(int handle);
		
		/*BEGIN MONOBEHAVIOUR DELEGATES*/
		public delegate int SystemCollectionsGenericIComparerSystemInt32CompareDelegate(int thisHandle, int param0, int param1);
		public static SystemCollectionsGenericIComparerSystemInt32CompareDelegate SystemCollectionsGenericIComparerSystemInt32Compare;
		
		public delegate int SystemCollectionsGenericIComparerSystemStringCompareDelegate(int thisHandle, int param0, int param1);
		public static SystemCollectionsGenericIComparerSystemStringCompareDelegate SystemCollectionsGenericIComparerSystemStringCompare;
		
		public delegate int SystemStringComparerCompareDelegate(int thisHandle, int param0, int param1);
		public static SystemStringComparerCompareDelegate SystemStringComparerCompare;
		
		public delegate bool SystemStringComparerEqualsDelegate(int thisHandle, int param0, int param1);
		public static SystemStringComparerEqualsDelegate SystemStringComparerEquals;
		
		public delegate int SystemStringComparerGetHashCodeDelegate(int thisHandle, int param0);
		public static SystemStringComparerGetHashCodeDelegate SystemStringComparerGetHashCode;
		
		public delegate void SystemCollectionsICollectionCopyToDelegate(int thisHandle, int param0, int param1);
		public static SystemCollectionsICollectionCopyToDelegate SystemCollectionsICollectionCopyTo;
		
		public delegate int SystemCollectionsICollectionGetEnumeratorDelegate(int thisHandle);
		public static SystemCollectionsICollectionGetEnumeratorDelegate SystemCollectionsICollectionGetEnumerator;
		
		public delegate int SystemCollectionsICollectionGetCountDelegate(int thisHandle);
		public static SystemCollectionsICollectionGetCountDelegate SystemCollectionsICollectionGetCount;
		
		public delegate bool SystemCollectionsICollectionGetIsSynchronizedDelegate(int thisHandle);
		public static SystemCollectionsICollectionGetIsSynchronizedDelegate SystemCollectionsICollectionGetIsSynchronized;
		
		public delegate int SystemCollectionsICollectionGetSyncRootDelegate(int thisHandle);
		public static SystemCollectionsICollectionGetSyncRootDelegate SystemCollectionsICollectionGetSyncRoot;
		
		public delegate int SystemCollectionsIListAddDelegate(int thisHandle, int param0);
		public static SystemCollectionsIListAddDelegate SystemCollectionsIListAdd;
		
		public delegate void SystemCollectionsIListClearDelegate(int thisHandle);
		public static SystemCollectionsIListClearDelegate SystemCollectionsIListClear;
		
		public delegate bool SystemCollectionsIListContainsDelegate(int thisHandle, int param0);
		public static SystemCollectionsIListContainsDelegate SystemCollectionsIListContains;
		
		public delegate int SystemCollectionsIListIndexOfDelegate(int thisHandle, int param0);
		public static SystemCollectionsIListIndexOfDelegate SystemCollectionsIListIndexOf;
		
		public delegate void SystemCollectionsIListInsertDelegate(int thisHandle, int param0, int param1);
		public static SystemCollectionsIListInsertDelegate SystemCollectionsIListInsert;
		
		public delegate void SystemCollectionsIListRemoveDelegate(int thisHandle, int param0);
		public static SystemCollectionsIListRemoveDelegate SystemCollectionsIListRemove;
		
		public delegate void SystemCollectionsIListRemoveAtDelegate(int thisHandle, int param0);
		public static SystemCollectionsIListRemoveAtDelegate SystemCollectionsIListRemoveAt;
		
		public delegate int SystemCollectionsIListGetEnumeratorDelegate(int thisHandle);
		public static SystemCollectionsIListGetEnumeratorDelegate SystemCollectionsIListGetEnumerator;
		
		public delegate void SystemCollectionsIListCopyToDelegate(int thisHandle, int param0, int param1);
		public static SystemCollectionsIListCopyToDelegate SystemCollectionsIListCopyTo;
		
		public delegate bool SystemCollectionsIListGetIsFixedSizeDelegate(int thisHandle);
		public static SystemCollectionsIListGetIsFixedSizeDelegate SystemCollectionsIListGetIsFixedSize;
		
		public delegate bool SystemCollectionsIListGetIsReadOnlyDelegate(int thisHandle);
		public static SystemCollectionsIListGetIsReadOnlyDelegate SystemCollectionsIListGetIsReadOnly;
		
		public delegate int SystemCollectionsIListGetItemDelegate(int thisHandle, int param0);
		public static SystemCollectionsIListGetItemDelegate SystemCollectionsIListGetItem;
		
		public delegate void SystemCollectionsIListSetItemDelegate(int thisHandle, int param0, int param1);
		public static SystemCollectionsIListSetItemDelegate SystemCollectionsIListSetItem;
		
		public delegate int SystemCollectionsIListGetCountDelegate(int thisHandle);
		public static SystemCollectionsIListGetCountDelegate SystemCollectionsIListGetCount;
		
		public delegate bool SystemCollectionsIListGetIsSynchronizedDelegate(int thisHandle);
		public static SystemCollectionsIListGetIsSynchronizedDelegate SystemCollectionsIListGetIsSynchronized;
		
		public delegate int SystemCollectionsIListGetSyncRootDelegate(int thisHandle);
		public static SystemCollectionsIListGetSyncRootDelegate SystemCollectionsIListGetSyncRoot;
		
		public delegate int SystemCollectionsQueueGetCountDelegate(int thisHandle);
		public static SystemCollectionsQueueGetCountDelegate SystemCollectionsQueueGetCount;
		
		public delegate void SystemComponentModelDesignIComponentChangeServiceOnComponentChangedDelegate(int thisHandle, int param0, int param1, int param2, int param3);
		public static SystemComponentModelDesignIComponentChangeServiceOnComponentChangedDelegate SystemComponentModelDesignIComponentChangeServiceOnComponentChanged;
		
		public delegate void SystemComponentModelDesignIComponentChangeServiceOnComponentChangingDelegate(int thisHandle, int param0, int param1);
		public static SystemComponentModelDesignIComponentChangeServiceOnComponentChangingDelegate SystemComponentModelDesignIComponentChangeServiceOnComponentChanging;
		
		public delegate void SystemComponentModelDesignIComponentChangeServiceAddComponentAddedDelegate(int thisHandle, int param0);
		public static SystemComponentModelDesignIComponentChangeServiceAddComponentAddedDelegate SystemComponentModelDesignIComponentChangeServiceAddComponentAdded;
		
		public delegate void SystemComponentModelDesignIComponentChangeServiceRemoveComponentAddedDelegate(int thisHandle, int param0);
		public static SystemComponentModelDesignIComponentChangeServiceRemoveComponentAddedDelegate SystemComponentModelDesignIComponentChangeServiceRemoveComponentAdded;
		
		public delegate void SystemComponentModelDesignIComponentChangeServiceAddComponentAddingDelegate(int thisHandle, int param0);
		public static SystemComponentModelDesignIComponentChangeServiceAddComponentAddingDelegate SystemComponentModelDesignIComponentChangeServiceAddComponentAdding;
		
		public delegate void SystemComponentModelDesignIComponentChangeServiceRemoveComponentAddingDelegate(int thisHandle, int param0);
		public static SystemComponentModelDesignIComponentChangeServiceRemoveComponentAddingDelegate SystemComponentModelDesignIComponentChangeServiceRemoveComponentAdding;
		
		public delegate void SystemComponentModelDesignIComponentChangeServiceAddComponentChangedDelegate(int thisHandle, int param0);
		public static SystemComponentModelDesignIComponentChangeServiceAddComponentChangedDelegate SystemComponentModelDesignIComponentChangeServiceAddComponentChanged;
		
		public delegate void SystemComponentModelDesignIComponentChangeServiceRemoveComponentChangedDelegate(int thisHandle, int param0);
		public static SystemComponentModelDesignIComponentChangeServiceRemoveComponentChangedDelegate SystemComponentModelDesignIComponentChangeServiceRemoveComponentChanged;
		
		public delegate void SystemComponentModelDesignIComponentChangeServiceAddComponentChangingDelegate(int thisHandle, int param0);
		public static SystemComponentModelDesignIComponentChangeServiceAddComponentChangingDelegate SystemComponentModelDesignIComponentChangeServiceAddComponentChanging;
		
		public delegate void SystemComponentModelDesignIComponentChangeServiceRemoveComponentChangingDelegate(int thisHandle, int param0);
		public static SystemComponentModelDesignIComponentChangeServiceRemoveComponentChangingDelegate SystemComponentModelDesignIComponentChangeServiceRemoveComponentChanging;
		
		public delegate void SystemComponentModelDesignIComponentChangeServiceAddComponentRemovedDelegate(int thisHandle, int param0);
		public static SystemComponentModelDesignIComponentChangeServiceAddComponentRemovedDelegate SystemComponentModelDesignIComponentChangeServiceAddComponentRemoved;
		
		public delegate void SystemComponentModelDesignIComponentChangeServiceRemoveComponentRemovedDelegate(int thisHandle, int param0);
		public static SystemComponentModelDesignIComponentChangeServiceRemoveComponentRemovedDelegate SystemComponentModelDesignIComponentChangeServiceRemoveComponentRemoved;
		
		public delegate void SystemComponentModelDesignIComponentChangeServiceAddComponentRemovingDelegate(int thisHandle, int param0);
		public static SystemComponentModelDesignIComponentChangeServiceAddComponentRemovingDelegate SystemComponentModelDesignIComponentChangeServiceAddComponentRemoving;
		
		public delegate void SystemComponentModelDesignIComponentChangeServiceRemoveComponentRemovingDelegate(int thisHandle, int param0);
		public static SystemComponentModelDesignIComponentChangeServiceRemoveComponentRemovingDelegate SystemComponentModelDesignIComponentChangeServiceRemoveComponentRemoving;
		
		public delegate void SystemComponentModelDesignIComponentChangeServiceAddComponentRenameDelegate(int thisHandle, int param0);
		public static SystemComponentModelDesignIComponentChangeServiceAddComponentRenameDelegate SystemComponentModelDesignIComponentChangeServiceAddComponentRename;
		
		public delegate void SystemComponentModelDesignIComponentChangeServiceRemoveComponentRenameDelegate(int thisHandle, int param0);
		public static SystemComponentModelDesignIComponentChangeServiceRemoveComponentRenameDelegate SystemComponentModelDesignIComponentChangeServiceRemoveComponentRename;
		
		public delegate void MyGameMonoBehavioursTestScriptAwakeDelegate(int thisHandle);
		public static MyGameMonoBehavioursTestScriptAwakeDelegate MyGameMonoBehavioursTestScriptAwake;
		
		public delegate void MyGameMonoBehavioursTestScriptOnAnimatorIKDelegate(int thisHandle, int param0);
		public static MyGameMonoBehavioursTestScriptOnAnimatorIKDelegate MyGameMonoBehavioursTestScriptOnAnimatorIK;
		
		public delegate void MyGameMonoBehavioursTestScriptOnCollisionEnterDelegate(int thisHandle, int param0);
		public static MyGameMonoBehavioursTestScriptOnCollisionEnterDelegate MyGameMonoBehavioursTestScriptOnCollisionEnter;
		
		public delegate void MyGameMonoBehavioursTestScriptUpdateDelegate(int thisHandle);
		public static MyGameMonoBehavioursTestScriptUpdateDelegate MyGameMonoBehavioursTestScriptUpdate;
		
		public delegate void SystemActionNativeInvokeDelegate(int thisHandle);
		public static SystemActionNativeInvokeDelegate SystemActionNativeInvoke;
		
		public delegate void SystemActionSystemSingleNativeInvokeDelegate(int thisHandle, float param0);
		public static SystemActionSystemSingleNativeInvokeDelegate SystemActionSystemSingleNativeInvoke;
		
		public delegate void SystemActionSystemSingle_SystemSingleNativeInvokeDelegate(int thisHandle, float param0, float param1);
		public static SystemActionSystemSingle_SystemSingleNativeInvokeDelegate SystemActionSystemSingle_SystemSingleNativeInvoke;
		
		public delegate double SystemFuncSystemInt32_SystemSingle_SystemDoubleNativeInvokeDelegate(int thisHandle, int param0, float param1);
		public static SystemFuncSystemInt32_SystemSingle_SystemDoubleNativeInvokeDelegate SystemFuncSystemInt32_SystemSingle_SystemDoubleNativeInvoke;
		
		public delegate int SystemFuncSystemInt16_SystemInt32_SystemStringNativeInvokeDelegate(int thisHandle, short param0, int param1);
		public static SystemFuncSystemInt16_SystemInt32_SystemStringNativeInvokeDelegate SystemFuncSystemInt16_SystemInt32_SystemStringNativeInvoke;
		
		public delegate void SystemAppDomainInitializerNativeInvokeDelegate(int thisHandle, int param0);
		public static SystemAppDomainInitializerNativeInvokeDelegate SystemAppDomainInitializerNativeInvoke;
		
		public delegate void UnityEngineEventsUnityActionNativeInvokeDelegate(int thisHandle);
		public static UnityEngineEventsUnityActionNativeInvokeDelegate UnityEngineEventsUnityActionNativeInvoke;
		
		public delegate void UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeNativeInvokeDelegate(int thisHandle, UnityEngine.SceneManagement.Scene param0, UnityEngine.SceneManagement.LoadSceneMode param1);
		public static UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeNativeInvokeDelegate UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeNativeInvoke;
		
		public delegate void SystemComponentModelDesignComponentEventHandlerNativeInvokeDelegate(int thisHandle, int param0, int param1);
		public static SystemComponentModelDesignComponentEventHandlerNativeInvokeDelegate SystemComponentModelDesignComponentEventHandlerNativeInvoke;
		
		public delegate void SystemComponentModelDesignComponentChangingEventHandlerNativeInvokeDelegate(int thisHandle, int param0, int param1);
		public static SystemComponentModelDesignComponentChangingEventHandlerNativeInvokeDelegate SystemComponentModelDesignComponentChangingEventHandlerNativeInvoke;
		
		public delegate void SystemComponentModelDesignComponentChangedEventHandlerNativeInvokeDelegate(int thisHandle, int param0, int param1);
		public static SystemComponentModelDesignComponentChangedEventHandlerNativeInvokeDelegate SystemComponentModelDesignComponentChangedEventHandlerNativeInvoke;
		
		public delegate void SystemComponentModelDesignComponentRenameEventHandlerNativeInvokeDelegate(int thisHandle, int param0, int param1);
		public static SystemComponentModelDesignComponentRenameEventHandlerNativeInvokeDelegate SystemComponentModelDesignComponentRenameEventHandlerNativeInvoke;
		
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
			IntPtr boxVector3,
			IntPtr unboxVector3,
			IntPtr unityEngineMatrix4x4PropertyGetItem,
			IntPtr unityEngineMatrix4x4PropertySetItem,
			IntPtr boxMatrix4x4,
			IntPtr unboxMatrix4x4,
			IntPtr releaseUnityEngineRaycastHit,
			IntPtr unityEngineRaycastHitPropertyGetPoint,
			IntPtr unityEngineRaycastHitPropertySetPoint,
			IntPtr unityEngineRaycastHitPropertyGetTransform,
			IntPtr boxRaycastHit,
			IntPtr unboxRaycastHit,
			IntPtr boxQueryTriggerInteraction,
			IntPtr unboxQueryTriggerInteraction,
			IntPtr releaseSystemCollectionsGenericKeyValuePairSystemString_SystemDouble,
			IntPtr systemCollectionsGenericKeyValuePairSystemString_SystemDoubleConstructorSystemString_SystemDouble,
			IntPtr systemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetKey,
			IntPtr systemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetValue,
			IntPtr boxKeyValuePairSystemString_SystemDouble,
			IntPtr unboxKeyValuePairSystemString_SystemDouble,
			IntPtr systemCollectionsGenericListSystemStringConstructor,
			IntPtr systemCollectionsGenericListSystemStringPropertyGetItem,
			IntPtr systemCollectionsGenericListSystemStringPropertySetItem,
			IntPtr systemCollectionsGenericListSystemStringMethodAddSystemString,
			IntPtr systemCollectionsGenericListSystemStringMethodSortSystemCollectionsGenericIComparer,
			IntPtr systemCollectionsGenericListSystemInt32Constructor,
			IntPtr systemCollectionsGenericListSystemInt32PropertyGetItem,
			IntPtr systemCollectionsGenericListSystemInt32PropertySetItem,
			IntPtr systemCollectionsGenericListSystemInt32MethodAddSystemInt32,
			IntPtr systemCollectionsGenericListSystemInt32MethodSortSystemCollectionsGenericIComparer,
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
			IntPtr boxResolution,
			IntPtr unboxResolution,
			IntPtr unityEngineScreenPropertyGetResolutions,
			IntPtr unityEngineRayConstructorUnityEngineVector3_UnityEngineVector3,
			IntPtr boxRay,
			IntPtr unboxRay,
			IntPtr unityEnginePhysicsMethodRaycastNonAllocUnityEngineRay_UnityEngineRaycastHit,
			IntPtr unityEnginePhysicsMethodRaycastAllUnityEngineRay,
			IntPtr boxColor,
			IntPtr unboxColor,
			IntPtr boxGradientColorKey,
			IntPtr unboxGradientColorKey,
			IntPtr unityEngineGradientConstructor,
			IntPtr unityEngineGradientPropertyGetColorKeys,
			IntPtr unityEngineGradientPropertySetColorKeys,
			IntPtr systemAppDomainSetupConstructor,
			IntPtr systemAppDomainSetupPropertyGetAppDomainInitializer,
			IntPtr systemAppDomainSetupPropertySetAppDomainInitializer,
			IntPtr unityEngineApplicationAddEventOnBeforeRender,
			IntPtr unityEngineApplicationRemoveEventOnBeforeRender,
			IntPtr unityEngineSceneManagementSceneManagerAddEventSceneLoaded,
			IntPtr unityEngineSceneManagementSceneManagerRemoveEventSceneLoaded,
			IntPtr boxScene,
			IntPtr unboxScene,
			IntPtr boxLoadSceneMode,
			IntPtr unboxLoadSceneMode,
			IntPtr systemCollectionsIEnumeratorPropertyGetCurrent,
			IntPtr systemCollectionsIEnumeratorMethodMoveNext,
			IntPtr releaseSystemCollectionsGenericIComparerSystemInt32,
			IntPtr systemCollectionsGenericIComparerSystemInt32Constructor,
			IntPtr releaseSystemCollectionsGenericIComparerSystemString,
			IntPtr systemCollectionsGenericIComparerSystemStringConstructor,
			IntPtr releaseSystemStringComparer,
			IntPtr systemStringComparerConstructor,
			IntPtr releaseSystemCollectionsICollection,
			IntPtr systemCollectionsICollectionConstructor,
			IntPtr releaseSystemCollectionsIList,
			IntPtr systemCollectionsIListConstructor,
			IntPtr releaseSystemCollectionsQueue,
			IntPtr systemCollectionsQueueConstructor,
			IntPtr releaseSystemComponentModelDesignIComponentChangeService,
			IntPtr systemComponentModelDesignIComponentChangeServiceConstructor,
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
			IntPtr unboxDouble,
			IntPtr systemSystemInt32Array1Constructor1,
			IntPtr systemInt32Array1GetItem1,
			IntPtr systemInt32Array1SetItem1,
			IntPtr systemSystemSingleArray1Constructor1,
			IntPtr systemSingleArray1GetItem1,
			IntPtr systemSingleArray1SetItem1,
			IntPtr systemSystemSingleArray2Constructor2,
			IntPtr systemSystemSingleArray2GetLength2,
			IntPtr systemSingleArray2GetItem2,
			IntPtr systemSingleArray2SetItem2,
			IntPtr systemSystemSingleArray3Constructor3,
			IntPtr systemSystemSingleArray3GetLength3,
			IntPtr systemSingleArray3GetItem3,
			IntPtr systemSingleArray3SetItem3,
			IntPtr systemSystemStringArray1Constructor1,
			IntPtr systemStringArray1GetItem1,
			IntPtr systemStringArray1SetItem1,
			IntPtr unityEngineUnityEngineResolutionArray1Constructor1,
			IntPtr unityEngineResolutionArray1GetItem1,
			IntPtr unityEngineResolutionArray1SetItem1,
			IntPtr unityEngineUnityEngineRaycastHitArray1Constructor1,
			IntPtr unityEngineRaycastHitArray1GetItem1,
			IntPtr unityEngineRaycastHitArray1SetItem1,
			IntPtr unityEngineUnityEngineGradientColorKeyArray1Constructor1,
			IntPtr unityEngineGradientColorKeyArray1GetItem1,
			IntPtr unityEngineGradientColorKeyArray1SetItem1,
			IntPtr releaseSystemAction,
			IntPtr systemActionConstructor,
			IntPtr systemActionAdd,
			IntPtr systemActionRemove,
			IntPtr systemActionInvoke,
			IntPtr releaseSystemActionSystemSingle,
			IntPtr systemActionSystemSingleConstructor,
			IntPtr systemActionSystemSingleAdd,
			IntPtr systemActionSystemSingleRemove,
			IntPtr systemActionSystemSingleInvoke,
			IntPtr releaseSystemActionSystemSingle_SystemSingle,
			IntPtr systemActionSystemSingle_SystemSingleConstructor,
			IntPtr systemActionSystemSingle_SystemSingleAdd,
			IntPtr systemActionSystemSingle_SystemSingleRemove,
			IntPtr systemActionSystemSingle_SystemSingleInvoke,
			IntPtr releaseSystemFuncSystemInt32_SystemSingle_SystemDouble,
			IntPtr systemFuncSystemInt32_SystemSingle_SystemDoubleConstructor,
			IntPtr systemFuncSystemInt32_SystemSingle_SystemDoubleAdd,
			IntPtr systemFuncSystemInt32_SystemSingle_SystemDoubleRemove,
			IntPtr systemFuncSystemInt32_SystemSingle_SystemDoubleInvoke,
			IntPtr releaseSystemFuncSystemInt16_SystemInt32_SystemString,
			IntPtr systemFuncSystemInt16_SystemInt32_SystemStringConstructor,
			IntPtr systemFuncSystemInt16_SystemInt32_SystemStringAdd,
			IntPtr systemFuncSystemInt16_SystemInt32_SystemStringRemove,
			IntPtr systemFuncSystemInt16_SystemInt32_SystemStringInvoke,
			IntPtr releaseSystemAppDomainInitializer,
			IntPtr systemAppDomainInitializerConstructor,
			IntPtr systemAppDomainInitializerAdd,
			IntPtr systemAppDomainInitializerRemove,
			IntPtr systemAppDomainInitializerInvoke,
			IntPtr releaseUnityEngineEventsUnityAction,
			IntPtr unityEngineEventsUnityActionConstructor,
			IntPtr unityEngineEventsUnityActionAdd,
			IntPtr unityEngineEventsUnityActionRemove,
			IntPtr unityEngineEventsUnityActionInvoke,
			IntPtr releaseUnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneMode,
			IntPtr unityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeConstructor,
			IntPtr unityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeAdd,
			IntPtr unityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeRemove,
			IntPtr unityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeInvoke,
			IntPtr releaseSystemComponentModelDesignComponentEventHandler,
			IntPtr systemComponentModelDesignComponentEventHandlerConstructor,
			IntPtr systemComponentModelDesignComponentEventHandlerAdd,
			IntPtr systemComponentModelDesignComponentEventHandlerRemove,
			IntPtr systemComponentModelDesignComponentEventHandlerInvoke,
			IntPtr releaseSystemComponentModelDesignComponentChangingEventHandler,
			IntPtr systemComponentModelDesignComponentChangingEventHandlerConstructor,
			IntPtr systemComponentModelDesignComponentChangingEventHandlerAdd,
			IntPtr systemComponentModelDesignComponentChangingEventHandlerRemove,
			IntPtr systemComponentModelDesignComponentChangingEventHandlerInvoke,
			IntPtr releaseSystemComponentModelDesignComponentChangedEventHandler,
			IntPtr systemComponentModelDesignComponentChangedEventHandlerConstructor,
			IntPtr systemComponentModelDesignComponentChangedEventHandlerAdd,
			IntPtr systemComponentModelDesignComponentChangedEventHandlerRemove,
			IntPtr systemComponentModelDesignComponentChangedEventHandlerInvoke,
			IntPtr releaseSystemComponentModelDesignComponentRenameEventHandler,
			IntPtr systemComponentModelDesignComponentRenameEventHandlerConstructor,
			IntPtr systemComponentModelDesignComponentRenameEventHandlerAdd,
			IntPtr systemComponentModelDesignComponentRenameEventHandlerRemove,
			IntPtr systemComponentModelDesignComponentRenameEventHandlerInvoke
			/*END INIT PARAMS*/);
		
		[DllImport(PluginName)]
		static extern void SetCsharpException(int handle);
		
		/*BEGIN MONOBEHAVIOUR IMPORTS*/
		[DllImport(Constants.PluginName)]
		public static extern void SystemCollectionsGenericIComparerSystemInt32Compare(int thisHandle, int param0, int param1);
		
		[DllImport(Constants.PluginName)]
		public static extern void SystemCollectionsGenericIComparerSystemStringCompare(int thisHandle, int param0, int param1);
		
		[DllImport(Constants.PluginName)]
		public static extern void SystemStringComparerCompare(int thisHandle, int param0, int param1);
		
		[DllImport(Constants.PluginName)]
		public static extern void SystemStringComparerEquals(int thisHandle, int param0, int param1);
		
		[DllImport(Constants.PluginName)]
		public static extern void SystemStringComparerGetHashCode(int thisHandle, int param0);
		
		[DllImport(Constants.PluginName)]
		public static extern void SystemCollectionsICollectionCopyTo(int thisHandle, int param0, int param1);
		
		[DllImport(Constants.PluginName)]
		public static extern void SystemCollectionsICollectionGetEnumerator(int thisHandle);
		
		[DllImport(Constants.PluginName)]
		public static extern void SystemCollectionsICollectionGetCount(int thisHandle);
		
		[DllImport(Constants.PluginName)]
		public static extern void SystemCollectionsICollectionGetIsSynchronized(int thisHandle);
		
		[DllImport(Constants.PluginName)]
		public static extern void SystemCollectionsICollectionGetSyncRoot(int thisHandle);
		
		[DllImport(Constants.PluginName)]
		public static extern void SystemCollectionsIListAdd(int thisHandle, int param0);
		
		[DllImport(Constants.PluginName)]
		public static extern void SystemCollectionsIListClear(int thisHandle);
		
		[DllImport(Constants.PluginName)]
		public static extern void SystemCollectionsIListContains(int thisHandle, int param0);
		
		[DllImport(Constants.PluginName)]
		public static extern void SystemCollectionsIListIndexOf(int thisHandle, int param0);
		
		[DllImport(Constants.PluginName)]
		public static extern void SystemCollectionsIListInsert(int thisHandle, int param0, int param1);
		
		[DllImport(Constants.PluginName)]
		public static extern void SystemCollectionsIListRemove(int thisHandle, int param0);
		
		[DllImport(Constants.PluginName)]
		public static extern void SystemCollectionsIListRemoveAt(int thisHandle, int param0);
		
		[DllImport(Constants.PluginName)]
		public static extern void SystemCollectionsIListGetEnumerator(int thisHandle);
		
		[DllImport(Constants.PluginName)]
		public static extern void SystemCollectionsIListCopyTo(int thisHandle, int param0, int param1);
		
		[DllImport(Constants.PluginName)]
		public static extern void SystemCollectionsIListGetIsFixedSize(int thisHandle);
		
		[DllImport(Constants.PluginName)]
		public static extern void SystemCollectionsIListGetIsReadOnly(int thisHandle);
		
		[DllImport(Constants.PluginName)]
		public static extern void SystemCollectionsIListGetItem(int thisHandle, int param0);
		
		[DllImport(Constants.PluginName)]
		public static extern void SystemCollectionsIListSetItem(int thisHandle, int param0, int param1);
		
		[DllImport(Constants.PluginName)]
		public static extern void SystemCollectionsIListGetCount(int thisHandle);
		
		[DllImport(Constants.PluginName)]
		public static extern void SystemCollectionsIListGetIsSynchronized(int thisHandle);
		
		[DllImport(Constants.PluginName)]
		public static extern void SystemCollectionsIListGetSyncRoot(int thisHandle);
		
		[DllImport(Constants.PluginName)]
		public static extern void SystemCollectionsQueueGetCount(int thisHandle);
		
		[DllImport(Constants.PluginName)]
		public static extern void SystemComponentModelDesignIComponentChangeServiceOnComponentChanged(int thisHandle, int param0, int param1, int param2, int param3);
		
		[DllImport(Constants.PluginName)]
		public static extern void SystemComponentModelDesignIComponentChangeServiceOnComponentChanging(int thisHandle, int param0, int param1);
		
		[DllImport(Constants.PluginName)]
		public static extern void SystemComponentModelDesignIComponentChangeServiceAddComponentAdded(int thisHandle, int param0);
		
		[DllImport(Constants.PluginName)]
		public static extern void SystemComponentModelDesignIComponentChangeServiceRemoveComponentAdded(int thisHandle, int param0);
		
		[DllImport(Constants.PluginName)]
		public static extern void SystemComponentModelDesignIComponentChangeServiceAddComponentAdding(int thisHandle, int param0);
		
		[DllImport(Constants.PluginName)]
		public static extern void SystemComponentModelDesignIComponentChangeServiceRemoveComponentAdding(int thisHandle, int param0);
		
		[DllImport(Constants.PluginName)]
		public static extern void SystemComponentModelDesignIComponentChangeServiceAddComponentChanged(int thisHandle, int param0);
		
		[DllImport(Constants.PluginName)]
		public static extern void SystemComponentModelDesignIComponentChangeServiceRemoveComponentChanged(int thisHandle, int param0);
		
		[DllImport(Constants.PluginName)]
		public static extern void SystemComponentModelDesignIComponentChangeServiceAddComponentChanging(int thisHandle, int param0);
		
		[DllImport(Constants.PluginName)]
		public static extern void SystemComponentModelDesignIComponentChangeServiceRemoveComponentChanging(int thisHandle, int param0);
		
		[DllImport(Constants.PluginName)]
		public static extern void SystemComponentModelDesignIComponentChangeServiceAddComponentRemoved(int thisHandle, int param0);
		
		[DllImport(Constants.PluginName)]
		public static extern void SystemComponentModelDesignIComponentChangeServiceRemoveComponentRemoved(int thisHandle, int param0);
		
		[DllImport(Constants.PluginName)]
		public static extern void SystemComponentModelDesignIComponentChangeServiceAddComponentRemoving(int thisHandle, int param0);
		
		[DllImport(Constants.PluginName)]
		public static extern void SystemComponentModelDesignIComponentChangeServiceRemoveComponentRemoving(int thisHandle, int param0);
		
		[DllImport(Constants.PluginName)]
		public static extern void SystemComponentModelDesignIComponentChangeServiceAddComponentRename(int thisHandle, int param0);
		
		[DllImport(Constants.PluginName)]
		public static extern void SystemComponentModelDesignIComponentChangeServiceRemoveComponentRename(int thisHandle, int param0);
		
		[DllImport(Constants.PluginName)]
		public static extern void MyGameMonoBehavioursTestScriptAwake(int thisHandle);
		
		[DllImport(Constants.PluginName)]
		public static extern void MyGameMonoBehavioursTestScriptOnAnimatorIK(int thisHandle, int param0);
		
		[DllImport(Constants.PluginName)]
		public static extern void MyGameMonoBehavioursTestScriptOnCollisionEnter(int thisHandle, int param0);
		
		[DllImport(Constants.PluginName)]
		public static extern void MyGameMonoBehavioursTestScriptUpdate(int thisHandle);
		
		[DllImport(Constants.PluginName)]
		public static extern void SystemActionNativeInvoke(int thisHandle);
		
		[DllImport(Constants.PluginName)]
		public static extern void SystemActionSystemSingleNativeInvoke(int thisHandle, int param0);
		
		[DllImport(Constants.PluginName)]
		public static extern void SystemActionSystemSingle_SystemSingleNativeInvoke(int thisHandle, int param0, int param1);
		
		[DllImport(Constants.PluginName)]
		public static extern void SystemFuncSystemInt32_SystemSingle_SystemDoubleNativeInvoke(int thisHandle, int param0, int param1);
		
		[DllImport(Constants.PluginName)]
		public static extern void SystemFuncSystemInt16_SystemInt32_SystemStringNativeInvoke(int thisHandle, int param0, int param1);
		
		[DllImport(Constants.PluginName)]
		public static extern void SystemAppDomainInitializerNativeInvoke(int thisHandle, int param0);
		
		[DllImport(Constants.PluginName)]
		public static extern void UnityEngineEventsUnityActionNativeInvoke(int thisHandle);
		
		[DllImport(Constants.PluginName)]
		public static extern void UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeNativeInvoke(int thisHandle, UnityEngine.SceneManagement.Scene param0, int param1);
		
		[DllImport(Constants.PluginName)]
		public static extern void SystemComponentModelDesignComponentEventHandlerNativeInvoke(int thisHandle, int param0, int param1);
		
		[DllImport(Constants.PluginName)]
		public static extern void SystemComponentModelDesignComponentChangingEventHandlerNativeInvoke(int thisHandle, int param0, int param1);
		
		[DllImport(Constants.PluginName)]
		public static extern void SystemComponentModelDesignComponentChangedEventHandlerNativeInvoke(int thisHandle, int param0, int param1);
		
		[DllImport(Constants.PluginName)]
		public static extern void SystemComponentModelDesignComponentRenameEventHandlerNativeInvoke(int thisHandle, int param0, int param1);
		
		[DllImport(Constants.PluginName)]
		public static extern void SetCsharpExceptionSystemNullReferenceException(int thisHandle, int param0);
		/*END MONOBEHAVIOUR IMPORTS*/
#endif
		
		delegate void ReleaseObjectDelegate(int handle);
		delegate int StringNewDelegate(string chars);
		delegate void SetExceptionDelegate(int handle);
		delegate int ArrayGetLengthDelegate(int handle);
		
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
		delegate int BoxVector3Delegate(ref UnityEngine.Vector3 val);
		delegate UnityEngine.Vector3 UnboxVector3Delegate(int valHandle);
		delegate float UnityEngineMatrix4x4PropertyGetItemDelegate(ref UnityEngine.Matrix4x4 thiz, int row, int column);
		delegate void UnityEngineMatrix4x4PropertySetItemDelegate(ref UnityEngine.Matrix4x4 thiz, int row, int column, float value);
		delegate int BoxMatrix4x4Delegate(ref UnityEngine.Matrix4x4 val);
		delegate UnityEngine.Matrix4x4 UnboxMatrix4x4Delegate(int valHandle);
		delegate void ReleaseUnityEngineRaycastHitDelegate(int handle);
		delegate UnityEngine.Vector3 UnityEngineRaycastHitPropertyGetPointDelegate(int thisHandle);
		delegate void UnityEngineRaycastHitPropertySetPointDelegate(int thisHandle, ref UnityEngine.Vector3 value);
		delegate int UnityEngineRaycastHitPropertyGetTransformDelegate(int thisHandle);
		delegate int BoxRaycastHitDelegate(int valHandle);
		delegate int UnboxRaycastHitDelegate(int valHandle);
		delegate int BoxQueryTriggerInteractionDelegate(UnityEngine.QueryTriggerInteraction val);
		delegate UnityEngine.QueryTriggerInteraction UnboxQueryTriggerInteractionDelegate(int valHandle);
		delegate void ReleaseSystemCollectionsGenericKeyValuePairSystemString_SystemDoubleDelegate(int handle);
		delegate int SystemCollectionsGenericKeyValuePairSystemString_SystemDoubleConstructorSystemString_SystemDoubleDelegate(int keyHandle, double value);
		delegate int SystemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetKeyDelegate(int thisHandle);
		delegate double SystemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetValueDelegate(int thisHandle);
		delegate int BoxKeyValuePairSystemString_SystemDoubleDelegate(int valHandle);
		delegate int UnboxKeyValuePairSystemString_SystemDoubleDelegate(int valHandle);
		delegate int SystemCollectionsGenericListSystemStringConstructorDelegate();
		delegate int SystemCollectionsGenericListSystemStringPropertyGetItemDelegate(int thisHandle, int index);
		delegate void SystemCollectionsGenericListSystemStringPropertySetItemDelegate(int thisHandle, int index, int valueHandle);
		delegate void SystemCollectionsGenericListSystemStringMethodAddSystemStringDelegate(int thisHandle, int itemHandle);
		delegate void SystemCollectionsGenericListSystemStringMethodSortSystemCollectionsGenericIComparerDelegate(int thisHandle, int comparerHandle);
		delegate int SystemCollectionsGenericListSystemInt32ConstructorDelegate();
		delegate int SystemCollectionsGenericListSystemInt32PropertyGetItemDelegate(int thisHandle, int index);
		delegate void SystemCollectionsGenericListSystemInt32PropertySetItemDelegate(int thisHandle, int index, int value);
		delegate void SystemCollectionsGenericListSystemInt32MethodAddSystemInt32Delegate(int thisHandle, int item);
		delegate void SystemCollectionsGenericListSystemInt32MethodSortSystemCollectionsGenericIComparerDelegate(int thisHandle, int comparerHandle);
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
		delegate int BoxResolutionDelegate(ref UnityEngine.Resolution val);
		delegate UnityEngine.Resolution UnboxResolutionDelegate(int valHandle);
		delegate int UnityEngineScreenPropertyGetResolutionsDelegate();
		delegate UnityEngine.Ray UnityEngineRayConstructorUnityEngineVector3_UnityEngineVector3Delegate(ref UnityEngine.Vector3 origin, ref UnityEngine.Vector3 direction);
		delegate int BoxRayDelegate(ref UnityEngine.Ray val);
		delegate UnityEngine.Ray UnboxRayDelegate(int valHandle);
		delegate int UnityEnginePhysicsMethodRaycastNonAllocUnityEngineRay_UnityEngineRaycastHitDelegate(ref UnityEngine.Ray ray, int resultsHandle);
		delegate int UnityEnginePhysicsMethodRaycastAllUnityEngineRayDelegate(ref UnityEngine.Ray ray);
		delegate int BoxColorDelegate(ref UnityEngine.Color val);
		delegate UnityEngine.Color UnboxColorDelegate(int valHandle);
		delegate int BoxGradientColorKeyDelegate(ref UnityEngine.GradientColorKey val);
		delegate UnityEngine.GradientColorKey UnboxGradientColorKeyDelegate(int valHandle);
		delegate int UnityEngineGradientConstructorDelegate();
		delegate int UnityEngineGradientPropertyGetColorKeysDelegate(int thisHandle);
		delegate void UnityEngineGradientPropertySetColorKeysDelegate(int thisHandle, int valueHandle);
		delegate int SystemAppDomainSetupConstructorDelegate();
		delegate int SystemAppDomainSetupPropertyGetAppDomainInitializerDelegate(int thisHandle);
		delegate void SystemAppDomainSetupPropertySetAppDomainInitializerDelegate(int thisHandle, int valueHandle);
		delegate void UnityEngineApplicationAddEventOnBeforeRenderDelegate(int delHandle);
		delegate void UnityEngineApplicationRemoveEventOnBeforeRenderDelegate(int delHandle);
		delegate void UnityEngineSceneManagementSceneManagerAddEventSceneLoadedDelegate(int delHandle);
		delegate void UnityEngineSceneManagementSceneManagerRemoveEventSceneLoadedDelegate(int delHandle);
		delegate int BoxSceneDelegate(ref UnityEngine.SceneManagement.Scene val);
		delegate UnityEngine.SceneManagement.Scene UnboxSceneDelegate(int valHandle);
		delegate int BoxLoadSceneModeDelegate(UnityEngine.SceneManagement.LoadSceneMode val);
		delegate UnityEngine.SceneManagement.LoadSceneMode UnboxLoadSceneModeDelegate(int valHandle);
		delegate int SystemCollectionsIEnumeratorPropertyGetCurrentDelegate(int thisHandle);
		delegate bool SystemCollectionsIEnumeratorMethodMoveNextDelegate(int thisHandle);
		delegate void SystemCollectionsGenericIComparerSystemInt32ConstructorDelegate(int cppHandle, ref int handle);
		delegate void ReleaseSystemCollectionsGenericIComparerSystemInt32Delegate(int handle);
		delegate void SystemCollectionsGenericIComparerSystemStringConstructorDelegate(int cppHandle, ref int handle);
		delegate void ReleaseSystemCollectionsGenericIComparerSystemStringDelegate(int handle);
		delegate void SystemStringComparerConstructorDelegate(int cppHandle, ref int handle);
		delegate void ReleaseSystemStringComparerDelegate(int handle);
		delegate void SystemCollectionsICollectionConstructorDelegate(int cppHandle, ref int handle);
		delegate void ReleaseSystemCollectionsICollectionDelegate(int handle);
		delegate void SystemCollectionsIListConstructorDelegate(int cppHandle, ref int handle);
		delegate void ReleaseSystemCollectionsIListDelegate(int handle);
		delegate void SystemCollectionsQueueConstructorDelegate(int cppHandle, ref int handle);
		delegate void ReleaseSystemCollectionsQueueDelegate(int handle);
		delegate void SystemComponentModelDesignIComponentChangeServiceConstructorDelegate(int cppHandle, ref int handle);
		delegate void ReleaseSystemComponentModelDesignIComponentChangeServiceDelegate(int handle);
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
		delegate int SystemSystemInt32Array1Constructor1Delegate(int length0);
		delegate int SystemInt32Array1GetItem1Delegate(int thisHandle, int index0);
		delegate void SystemInt32Array1SetItem1Delegate(int thisHandle, int index0, int item);
		delegate int SystemSystemSingleArray1Constructor1Delegate(int length0);
		delegate float SystemSingleArray1GetItem1Delegate(int thisHandle, int index0);
		delegate void SystemSingleArray1SetItem1Delegate(int thisHandle, int index0, float item);
		delegate int SystemSystemSingleArray2Constructor2Delegate(int length0, int length1);
		delegate int SystemSystemSingleArray2GetLength2Delegate(int thisHandle, int dimension);
		delegate float SystemSingleArray2GetItem2Delegate(int thisHandle, int index0, int index1);
		delegate void SystemSingleArray2SetItem2Delegate(int thisHandle, int index0, int index1, float item);
		delegate int SystemSystemSingleArray3Constructor3Delegate(int length0, int length1, int length2);
		delegate int SystemSystemSingleArray3GetLength3Delegate(int thisHandle, int dimension);
		delegate float SystemSingleArray3GetItem3Delegate(int thisHandle, int index0, int index1, int index2);
		delegate void SystemSingleArray3SetItem3Delegate(int thisHandle, int index0, int index1, int index2, float item);
		delegate int SystemSystemStringArray1Constructor1Delegate(int length0);
		delegate int SystemStringArray1GetItem1Delegate(int thisHandle, int index0);
		delegate void SystemStringArray1SetItem1Delegate(int thisHandle, int index0, int itemHandle);
		delegate int UnityEngineUnityEngineResolutionArray1Constructor1Delegate(int length0);
		delegate UnityEngine.Resolution UnityEngineResolutionArray1GetItem1Delegate(int thisHandle, int index0);
		delegate void UnityEngineResolutionArray1SetItem1Delegate(int thisHandle, int index0, ref UnityEngine.Resolution item);
		delegate int UnityEngineUnityEngineRaycastHitArray1Constructor1Delegate(int length0);
		delegate int UnityEngineRaycastHitArray1GetItem1Delegate(int thisHandle, int index0);
		delegate void UnityEngineRaycastHitArray1SetItem1Delegate(int thisHandle, int index0, int itemHandle);
		delegate int UnityEngineUnityEngineGradientColorKeyArray1Constructor1Delegate(int length0);
		delegate UnityEngine.GradientColorKey UnityEngineGradientColorKeyArray1GetItem1Delegate(int thisHandle, int index0);
		delegate void UnityEngineGradientColorKeyArray1SetItem1Delegate(int thisHandle, int index0, ref UnityEngine.GradientColorKey item);
		delegate void SystemActionInvokeDelegate(int thisHandle);
		delegate void SystemActionConstructorDelegate(int cppHandle, ref int handle, ref int classHandle);
		delegate void ReleaseSystemActionDelegate(int handle, int classHandle);
		delegate void SystemActionAddDelegate(int thisHandle, int delHandle);
		delegate void SystemActionRemoveDelegate(int thisHandle, int delHandle);
		delegate void SystemActionSystemSingleInvokeDelegate(int thisHandle, float obj);
		delegate void SystemActionSystemSingleConstructorDelegate(int cppHandle, ref int handle, ref int classHandle);
		delegate void ReleaseSystemActionSystemSingleDelegate(int handle, int classHandle);
		delegate void SystemActionSystemSingleAddDelegate(int thisHandle, int delHandle);
		delegate void SystemActionSystemSingleRemoveDelegate(int thisHandle, int delHandle);
		delegate void SystemActionSystemSingle_SystemSingleInvokeDelegate(int thisHandle, float arg1, float arg2);
		delegate void SystemActionSystemSingle_SystemSingleConstructorDelegate(int cppHandle, ref int handle, ref int classHandle);
		delegate void ReleaseSystemActionSystemSingle_SystemSingleDelegate(int handle, int classHandle);
		delegate void SystemActionSystemSingle_SystemSingleAddDelegate(int thisHandle, int delHandle);
		delegate void SystemActionSystemSingle_SystemSingleRemoveDelegate(int thisHandle, int delHandle);
		delegate double SystemFuncSystemInt32_SystemSingle_SystemDoubleInvokeDelegate(int thisHandle, int arg1, float arg2);
		delegate void SystemFuncSystemInt32_SystemSingle_SystemDoubleConstructorDelegate(int cppHandle, ref int handle, ref int classHandle);
		delegate void ReleaseSystemFuncSystemInt32_SystemSingle_SystemDoubleDelegate(int handle, int classHandle);
		delegate void SystemFuncSystemInt32_SystemSingle_SystemDoubleAddDelegate(int thisHandle, int delHandle);
		delegate void SystemFuncSystemInt32_SystemSingle_SystemDoubleRemoveDelegate(int thisHandle, int delHandle);
		delegate int SystemFuncSystemInt16_SystemInt32_SystemStringInvokeDelegate(int thisHandle, short arg1, int arg2);
		delegate void SystemFuncSystemInt16_SystemInt32_SystemStringConstructorDelegate(int cppHandle, ref int handle, ref int classHandle);
		delegate void ReleaseSystemFuncSystemInt16_SystemInt32_SystemStringDelegate(int handle, int classHandle);
		delegate void SystemFuncSystemInt16_SystemInt32_SystemStringAddDelegate(int thisHandle, int delHandle);
		delegate void SystemFuncSystemInt16_SystemInt32_SystemStringRemoveDelegate(int thisHandle, int delHandle);
		delegate void SystemAppDomainInitializerInvokeDelegate(int thisHandle, int argsHandle);
		delegate void SystemAppDomainInitializerConstructorDelegate(int cppHandle, ref int handle, ref int classHandle);
		delegate void ReleaseSystemAppDomainInitializerDelegate(int handle, int classHandle);
		delegate void SystemAppDomainInitializerAddDelegate(int thisHandle, int delHandle);
		delegate void SystemAppDomainInitializerRemoveDelegate(int thisHandle, int delHandle);
		delegate void UnityEngineEventsUnityActionInvokeDelegate(int thisHandle);
		delegate void UnityEngineEventsUnityActionConstructorDelegate(int cppHandle, ref int handle, ref int classHandle);
		delegate void ReleaseUnityEngineEventsUnityActionDelegate(int handle, int classHandle);
		delegate void UnityEngineEventsUnityActionAddDelegate(int thisHandle, int delHandle);
		delegate void UnityEngineEventsUnityActionRemoveDelegate(int thisHandle, int delHandle);
		delegate void UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeInvokeDelegate(int thisHandle, ref UnityEngine.SceneManagement.Scene arg0, UnityEngine.SceneManagement.LoadSceneMode arg1);
		delegate void UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeConstructorDelegate(int cppHandle, ref int handle, ref int classHandle);
		delegate void ReleaseUnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeDelegate(int handle, int classHandle);
		delegate void UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeAddDelegate(int thisHandle, int delHandle);
		delegate void UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeRemoveDelegate(int thisHandle, int delHandle);
		delegate void SystemComponentModelDesignComponentEventHandlerInvokeDelegate(int thisHandle, int senderHandle, int eHandle);
		delegate void SystemComponentModelDesignComponentEventHandlerConstructorDelegate(int cppHandle, ref int handle, ref int classHandle);
		delegate void ReleaseSystemComponentModelDesignComponentEventHandlerDelegate(int handle, int classHandle);
		delegate void SystemComponentModelDesignComponentEventHandlerAddDelegate(int thisHandle, int delHandle);
		delegate void SystemComponentModelDesignComponentEventHandlerRemoveDelegate(int thisHandle, int delHandle);
		delegate void SystemComponentModelDesignComponentChangingEventHandlerInvokeDelegate(int thisHandle, int senderHandle, int eHandle);
		delegate void SystemComponentModelDesignComponentChangingEventHandlerConstructorDelegate(int cppHandle, ref int handle, ref int classHandle);
		delegate void ReleaseSystemComponentModelDesignComponentChangingEventHandlerDelegate(int handle, int classHandle);
		delegate void SystemComponentModelDesignComponentChangingEventHandlerAddDelegate(int thisHandle, int delHandle);
		delegate void SystemComponentModelDesignComponentChangingEventHandlerRemoveDelegate(int thisHandle, int delHandle);
		delegate void SystemComponentModelDesignComponentChangedEventHandlerInvokeDelegate(int thisHandle, int senderHandle, int eHandle);
		delegate void SystemComponentModelDesignComponentChangedEventHandlerConstructorDelegate(int cppHandle, ref int handle, ref int classHandle);
		delegate void ReleaseSystemComponentModelDesignComponentChangedEventHandlerDelegate(int handle, int classHandle);
		delegate void SystemComponentModelDesignComponentChangedEventHandlerAddDelegate(int thisHandle, int delHandle);
		delegate void SystemComponentModelDesignComponentChangedEventHandlerRemoveDelegate(int thisHandle, int delHandle);
		delegate void SystemComponentModelDesignComponentRenameEventHandlerInvokeDelegate(int thisHandle, int senderHandle, int eHandle);
		delegate void SystemComponentModelDesignComponentRenameEventHandlerConstructorDelegate(int cppHandle, ref int handle, ref int classHandle);
		delegate void ReleaseSystemComponentModelDesignComponentRenameEventHandlerDelegate(int handle, int classHandle);
		delegate void SystemComponentModelDesignComponentRenameEventHandlerAddDelegate(int thisHandle, int delHandle);
		delegate void SystemComponentModelDesignComponentRenameEventHandlerRemoveDelegate(int thisHandle, int delHandle);
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
			SystemCollectionsGenericIComparerSystemInt32Compare = GetDelegate<SystemCollectionsGenericIComparerSystemInt32CompareDelegate>(libraryHandle, "SystemCollectionsGenericIComparerSystemInt32Compare");
			SystemCollectionsGenericIComparerSystemStringCompare = GetDelegate<SystemCollectionsGenericIComparerSystemStringCompareDelegate>(libraryHandle, "SystemCollectionsGenericIComparerSystemStringCompare");
			SystemStringComparerCompare = GetDelegate<SystemStringComparerCompareDelegate>(libraryHandle, "SystemStringComparerCompare");
			SystemStringComparerEquals = GetDelegate<SystemStringComparerEqualsDelegate>(libraryHandle, "SystemStringComparerEquals");
			SystemStringComparerGetHashCode = GetDelegate<SystemStringComparerGetHashCodeDelegate>(libraryHandle, "SystemStringComparerGetHashCode");
			SystemCollectionsICollectionCopyTo = GetDelegate<SystemCollectionsICollectionCopyToDelegate>(libraryHandle, "SystemCollectionsICollectionCopyTo");
			SystemCollectionsICollectionGetEnumerator = GetDelegate<SystemCollectionsICollectionGetEnumeratorDelegate>(libraryHandle, "SystemCollectionsICollectionGetEnumerator");
			SystemCollectionsICollectionGetCount = GetDelegate<SystemCollectionsICollectionGetCountDelegate>(libraryHandle, "SystemCollectionsICollectionGetCount");
			SystemCollectionsICollectionGetIsSynchronized = GetDelegate<SystemCollectionsICollectionGetIsSynchronizedDelegate>(libraryHandle, "SystemCollectionsICollectionGetIsSynchronized");
			SystemCollectionsICollectionGetSyncRoot = GetDelegate<SystemCollectionsICollectionGetSyncRootDelegate>(libraryHandle, "SystemCollectionsICollectionGetSyncRoot");
			SystemCollectionsIListAdd = GetDelegate<SystemCollectionsIListAddDelegate>(libraryHandle, "SystemCollectionsIListAdd");
			SystemCollectionsIListClear = GetDelegate<SystemCollectionsIListClearDelegate>(libraryHandle, "SystemCollectionsIListClear");
			SystemCollectionsIListContains = GetDelegate<SystemCollectionsIListContainsDelegate>(libraryHandle, "SystemCollectionsIListContains");
			SystemCollectionsIListIndexOf = GetDelegate<SystemCollectionsIListIndexOfDelegate>(libraryHandle, "SystemCollectionsIListIndexOf");
			SystemCollectionsIListInsert = GetDelegate<SystemCollectionsIListInsertDelegate>(libraryHandle, "SystemCollectionsIListInsert");
			SystemCollectionsIListRemove = GetDelegate<SystemCollectionsIListRemoveDelegate>(libraryHandle, "SystemCollectionsIListRemove");
			SystemCollectionsIListRemoveAt = GetDelegate<SystemCollectionsIListRemoveAtDelegate>(libraryHandle, "SystemCollectionsIListRemoveAt");
			SystemCollectionsIListGetEnumerator = GetDelegate<SystemCollectionsIListGetEnumeratorDelegate>(libraryHandle, "SystemCollectionsIListGetEnumerator");
			SystemCollectionsIListCopyTo = GetDelegate<SystemCollectionsIListCopyToDelegate>(libraryHandle, "SystemCollectionsIListCopyTo");
			SystemCollectionsIListGetIsFixedSize = GetDelegate<SystemCollectionsIListGetIsFixedSizeDelegate>(libraryHandle, "SystemCollectionsIListGetIsFixedSize");
			SystemCollectionsIListGetIsReadOnly = GetDelegate<SystemCollectionsIListGetIsReadOnlyDelegate>(libraryHandle, "SystemCollectionsIListGetIsReadOnly");
			SystemCollectionsIListGetItem = GetDelegate<SystemCollectionsIListGetItemDelegate>(libraryHandle, "SystemCollectionsIListGetItem");
			SystemCollectionsIListSetItem = GetDelegate<SystemCollectionsIListSetItemDelegate>(libraryHandle, "SystemCollectionsIListSetItem");
			SystemCollectionsIListGetCount = GetDelegate<SystemCollectionsIListGetCountDelegate>(libraryHandle, "SystemCollectionsIListGetCount");
			SystemCollectionsIListGetIsSynchronized = GetDelegate<SystemCollectionsIListGetIsSynchronizedDelegate>(libraryHandle, "SystemCollectionsIListGetIsSynchronized");
			SystemCollectionsIListGetSyncRoot = GetDelegate<SystemCollectionsIListGetSyncRootDelegate>(libraryHandle, "SystemCollectionsIListGetSyncRoot");
			SystemCollectionsQueueGetCount = GetDelegate<SystemCollectionsQueueGetCountDelegate>(libraryHandle, "SystemCollectionsQueueGetCount");
			SystemComponentModelDesignIComponentChangeServiceOnComponentChanged = GetDelegate<SystemComponentModelDesignIComponentChangeServiceOnComponentChangedDelegate>(libraryHandle, "SystemComponentModelDesignIComponentChangeServiceOnComponentChanged");
			SystemComponentModelDesignIComponentChangeServiceOnComponentChanging = GetDelegate<SystemComponentModelDesignIComponentChangeServiceOnComponentChangingDelegate>(libraryHandle, "SystemComponentModelDesignIComponentChangeServiceOnComponentChanging");
			SystemComponentModelDesignIComponentChangeServiceAddComponentAdded = GetDelegate<SystemComponentModelDesignIComponentChangeServiceAddComponentAddedDelegate>(libraryHandle, "SystemComponentModelDesignIComponentChangeServiceAddComponentAdded");
			SystemComponentModelDesignIComponentChangeServiceRemoveComponentAdded = GetDelegate<SystemComponentModelDesignIComponentChangeServiceRemoveComponentAddedDelegate>(libraryHandle, "SystemComponentModelDesignIComponentChangeServiceRemoveComponentAdded");
			SystemComponentModelDesignIComponentChangeServiceAddComponentAdding = GetDelegate<SystemComponentModelDesignIComponentChangeServiceAddComponentAddingDelegate>(libraryHandle, "SystemComponentModelDesignIComponentChangeServiceAddComponentAdding");
			SystemComponentModelDesignIComponentChangeServiceRemoveComponentAdding = GetDelegate<SystemComponentModelDesignIComponentChangeServiceRemoveComponentAddingDelegate>(libraryHandle, "SystemComponentModelDesignIComponentChangeServiceRemoveComponentAdding");
			SystemComponentModelDesignIComponentChangeServiceAddComponentChanged = GetDelegate<SystemComponentModelDesignIComponentChangeServiceAddComponentChangedDelegate>(libraryHandle, "SystemComponentModelDesignIComponentChangeServiceAddComponentChanged");
			SystemComponentModelDesignIComponentChangeServiceRemoveComponentChanged = GetDelegate<SystemComponentModelDesignIComponentChangeServiceRemoveComponentChangedDelegate>(libraryHandle, "SystemComponentModelDesignIComponentChangeServiceRemoveComponentChanged");
			SystemComponentModelDesignIComponentChangeServiceAddComponentChanging = GetDelegate<SystemComponentModelDesignIComponentChangeServiceAddComponentChangingDelegate>(libraryHandle, "SystemComponentModelDesignIComponentChangeServiceAddComponentChanging");
			SystemComponentModelDesignIComponentChangeServiceRemoveComponentChanging = GetDelegate<SystemComponentModelDesignIComponentChangeServiceRemoveComponentChangingDelegate>(libraryHandle, "SystemComponentModelDesignIComponentChangeServiceRemoveComponentChanging");
			SystemComponentModelDesignIComponentChangeServiceAddComponentRemoved = GetDelegate<SystemComponentModelDesignIComponentChangeServiceAddComponentRemovedDelegate>(libraryHandle, "SystemComponentModelDesignIComponentChangeServiceAddComponentRemoved");
			SystemComponentModelDesignIComponentChangeServiceRemoveComponentRemoved = GetDelegate<SystemComponentModelDesignIComponentChangeServiceRemoveComponentRemovedDelegate>(libraryHandle, "SystemComponentModelDesignIComponentChangeServiceRemoveComponentRemoved");
			SystemComponentModelDesignIComponentChangeServiceAddComponentRemoving = GetDelegate<SystemComponentModelDesignIComponentChangeServiceAddComponentRemovingDelegate>(libraryHandle, "SystemComponentModelDesignIComponentChangeServiceAddComponentRemoving");
			SystemComponentModelDesignIComponentChangeServiceRemoveComponentRemoving = GetDelegate<SystemComponentModelDesignIComponentChangeServiceRemoveComponentRemovingDelegate>(libraryHandle, "SystemComponentModelDesignIComponentChangeServiceRemoveComponentRemoving");
			SystemComponentModelDesignIComponentChangeServiceAddComponentRename = GetDelegate<SystemComponentModelDesignIComponentChangeServiceAddComponentRenameDelegate>(libraryHandle, "SystemComponentModelDesignIComponentChangeServiceAddComponentRename");
			SystemComponentModelDesignIComponentChangeServiceRemoveComponentRename = GetDelegate<SystemComponentModelDesignIComponentChangeServiceRemoveComponentRenameDelegate>(libraryHandle, "SystemComponentModelDesignIComponentChangeServiceRemoveComponentRename");
			MyGameMonoBehavioursTestScriptAwake = GetDelegate<MyGameMonoBehavioursTestScriptAwakeDelegate>(libraryHandle, "MyGameMonoBehavioursTestScriptAwake");
			MyGameMonoBehavioursTestScriptOnAnimatorIK = GetDelegate<MyGameMonoBehavioursTestScriptOnAnimatorIKDelegate>(libraryHandle, "MyGameMonoBehavioursTestScriptOnAnimatorIK");
			MyGameMonoBehavioursTestScriptOnCollisionEnter = GetDelegate<MyGameMonoBehavioursTestScriptOnCollisionEnterDelegate>(libraryHandle, "MyGameMonoBehavioursTestScriptOnCollisionEnter");
			MyGameMonoBehavioursTestScriptUpdate = GetDelegate<MyGameMonoBehavioursTestScriptUpdateDelegate>(libraryHandle, "MyGameMonoBehavioursTestScriptUpdate");
			SystemActionNativeInvoke = GetDelegate<SystemActionNativeInvokeDelegate>(libraryHandle, "SystemActionNativeInvoke");
			SystemActionSystemSingleNativeInvoke = GetDelegate<SystemActionSystemSingleNativeInvokeDelegate>(libraryHandle, "SystemActionSystemSingleNativeInvoke");
			SystemActionSystemSingle_SystemSingleNativeInvoke = GetDelegate<SystemActionSystemSingle_SystemSingleNativeInvokeDelegate>(libraryHandle, "SystemActionSystemSingle_SystemSingleNativeInvoke");
			SystemFuncSystemInt32_SystemSingle_SystemDoubleNativeInvoke = GetDelegate<SystemFuncSystemInt32_SystemSingle_SystemDoubleNativeInvokeDelegate>(libraryHandle, "SystemFuncSystemInt32_SystemSingle_SystemDoubleNativeInvoke");
			SystemFuncSystemInt16_SystemInt32_SystemStringNativeInvoke = GetDelegate<SystemFuncSystemInt16_SystemInt32_SystemStringNativeInvokeDelegate>(libraryHandle, "SystemFuncSystemInt16_SystemInt32_SystemStringNativeInvoke");
			SystemAppDomainInitializerNativeInvoke = GetDelegate<SystemAppDomainInitializerNativeInvokeDelegate>(libraryHandle, "SystemAppDomainInitializerNativeInvoke");
			UnityEngineEventsUnityActionNativeInvoke = GetDelegate<UnityEngineEventsUnityActionNativeInvokeDelegate>(libraryHandle, "UnityEngineEventsUnityActionNativeInvoke");
			UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeNativeInvoke = GetDelegate<UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeNativeInvokeDelegate>(libraryHandle, "UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeNativeInvoke");
			SystemComponentModelDesignComponentEventHandlerNativeInvoke = GetDelegate<SystemComponentModelDesignComponentEventHandlerNativeInvokeDelegate>(libraryHandle, "SystemComponentModelDesignComponentEventHandlerNativeInvoke");
			SystemComponentModelDesignComponentChangingEventHandlerNativeInvoke = GetDelegate<SystemComponentModelDesignComponentChangingEventHandlerNativeInvokeDelegate>(libraryHandle, "SystemComponentModelDesignComponentChangingEventHandlerNativeInvoke");
			SystemComponentModelDesignComponentChangedEventHandlerNativeInvoke = GetDelegate<SystemComponentModelDesignComponentChangedEventHandlerNativeInvokeDelegate>(libraryHandle, "SystemComponentModelDesignComponentChangedEventHandlerNativeInvoke");
			SystemComponentModelDesignComponentRenameEventHandlerNativeInvoke = GetDelegate<SystemComponentModelDesignComponentRenameEventHandlerNativeInvokeDelegate>(libraryHandle, "SystemComponentModelDesignComponentRenameEventHandlerNativeInvoke");
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
				Marshal.GetFunctionPointerForDelegate(new BoxVector3Delegate(BoxVector3)),
				Marshal.GetFunctionPointerForDelegate(new UnboxVector3Delegate(UnboxVector3)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineMatrix4x4PropertyGetItemDelegate(UnityEngineMatrix4x4PropertyGetItem)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineMatrix4x4PropertySetItemDelegate(UnityEngineMatrix4x4PropertySetItem)),
				Marshal.GetFunctionPointerForDelegate(new BoxMatrix4x4Delegate(BoxMatrix4x4)),
				Marshal.GetFunctionPointerForDelegate(new UnboxMatrix4x4Delegate(UnboxMatrix4x4)),
				Marshal.GetFunctionPointerForDelegate(new ReleaseUnityEngineRaycastHitDelegate(ReleaseUnityEngineRaycastHit)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineRaycastHitPropertyGetPointDelegate(UnityEngineRaycastHitPropertyGetPoint)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineRaycastHitPropertySetPointDelegate(UnityEngineRaycastHitPropertySetPoint)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineRaycastHitPropertyGetTransformDelegate(UnityEngineRaycastHitPropertyGetTransform)),
				Marshal.GetFunctionPointerForDelegate(new BoxRaycastHitDelegate(BoxRaycastHit)),
				Marshal.GetFunctionPointerForDelegate(new UnboxRaycastHitDelegate(UnboxRaycastHit)),
				Marshal.GetFunctionPointerForDelegate(new BoxQueryTriggerInteractionDelegate(BoxQueryTriggerInteraction)),
				Marshal.GetFunctionPointerForDelegate(new UnboxQueryTriggerInteractionDelegate(UnboxQueryTriggerInteraction)),
				Marshal.GetFunctionPointerForDelegate(new ReleaseSystemCollectionsGenericKeyValuePairSystemString_SystemDoubleDelegate(ReleaseSystemCollectionsGenericKeyValuePairSystemString_SystemDouble)),
				Marshal.GetFunctionPointerForDelegate(new SystemCollectionsGenericKeyValuePairSystemString_SystemDoubleConstructorSystemString_SystemDoubleDelegate(SystemCollectionsGenericKeyValuePairSystemString_SystemDoubleConstructorSystemString_SystemDouble)),
				Marshal.GetFunctionPointerForDelegate(new SystemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetKeyDelegate(SystemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetKey)),
				Marshal.GetFunctionPointerForDelegate(new SystemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetValueDelegate(SystemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetValue)),
				Marshal.GetFunctionPointerForDelegate(new BoxKeyValuePairSystemString_SystemDoubleDelegate(BoxKeyValuePairSystemString_SystemDouble)),
				Marshal.GetFunctionPointerForDelegate(new UnboxKeyValuePairSystemString_SystemDoubleDelegate(UnboxKeyValuePairSystemString_SystemDouble)),
				Marshal.GetFunctionPointerForDelegate(new SystemCollectionsGenericListSystemStringConstructorDelegate(SystemCollectionsGenericListSystemStringConstructor)),
				Marshal.GetFunctionPointerForDelegate(new SystemCollectionsGenericListSystemStringPropertyGetItemDelegate(SystemCollectionsGenericListSystemStringPropertyGetItem)),
				Marshal.GetFunctionPointerForDelegate(new SystemCollectionsGenericListSystemStringPropertySetItemDelegate(SystemCollectionsGenericListSystemStringPropertySetItem)),
				Marshal.GetFunctionPointerForDelegate(new SystemCollectionsGenericListSystemStringMethodAddSystemStringDelegate(SystemCollectionsGenericListSystemStringMethodAddSystemString)),
				Marshal.GetFunctionPointerForDelegate(new SystemCollectionsGenericListSystemStringMethodSortSystemCollectionsGenericIComparerDelegate(SystemCollectionsGenericListSystemStringMethodSortSystemCollectionsGenericIComparer)),
				Marshal.GetFunctionPointerForDelegate(new SystemCollectionsGenericListSystemInt32ConstructorDelegate(SystemCollectionsGenericListSystemInt32Constructor)),
				Marshal.GetFunctionPointerForDelegate(new SystemCollectionsGenericListSystemInt32PropertyGetItemDelegate(SystemCollectionsGenericListSystemInt32PropertyGetItem)),
				Marshal.GetFunctionPointerForDelegate(new SystemCollectionsGenericListSystemInt32PropertySetItemDelegate(SystemCollectionsGenericListSystemInt32PropertySetItem)),
				Marshal.GetFunctionPointerForDelegate(new SystemCollectionsGenericListSystemInt32MethodAddSystemInt32Delegate(SystemCollectionsGenericListSystemInt32MethodAddSystemInt32)),
				Marshal.GetFunctionPointerForDelegate(new SystemCollectionsGenericListSystemInt32MethodSortSystemCollectionsGenericIComparerDelegate(SystemCollectionsGenericListSystemInt32MethodSortSystemCollectionsGenericIComparer)),
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
				Marshal.GetFunctionPointerForDelegate(new BoxResolutionDelegate(BoxResolution)),
				Marshal.GetFunctionPointerForDelegate(new UnboxResolutionDelegate(UnboxResolution)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineScreenPropertyGetResolutionsDelegate(UnityEngineScreenPropertyGetResolutions)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineRayConstructorUnityEngineVector3_UnityEngineVector3Delegate(UnityEngineRayConstructorUnityEngineVector3_UnityEngineVector3)),
				Marshal.GetFunctionPointerForDelegate(new BoxRayDelegate(BoxRay)),
				Marshal.GetFunctionPointerForDelegate(new UnboxRayDelegate(UnboxRay)),
				Marshal.GetFunctionPointerForDelegate(new UnityEnginePhysicsMethodRaycastNonAllocUnityEngineRay_UnityEngineRaycastHitDelegate(UnityEnginePhysicsMethodRaycastNonAllocUnityEngineRay_UnityEngineRaycastHit)),
				Marshal.GetFunctionPointerForDelegate(new UnityEnginePhysicsMethodRaycastAllUnityEngineRayDelegate(UnityEnginePhysicsMethodRaycastAllUnityEngineRay)),
				Marshal.GetFunctionPointerForDelegate(new BoxColorDelegate(BoxColor)),
				Marshal.GetFunctionPointerForDelegate(new UnboxColorDelegate(UnboxColor)),
				Marshal.GetFunctionPointerForDelegate(new BoxGradientColorKeyDelegate(BoxGradientColorKey)),
				Marshal.GetFunctionPointerForDelegate(new UnboxGradientColorKeyDelegate(UnboxGradientColorKey)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineGradientConstructorDelegate(UnityEngineGradientConstructor)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineGradientPropertyGetColorKeysDelegate(UnityEngineGradientPropertyGetColorKeys)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineGradientPropertySetColorKeysDelegate(UnityEngineGradientPropertySetColorKeys)),
				Marshal.GetFunctionPointerForDelegate(new SystemAppDomainSetupConstructorDelegate(SystemAppDomainSetupConstructor)),
				Marshal.GetFunctionPointerForDelegate(new SystemAppDomainSetupPropertyGetAppDomainInitializerDelegate(SystemAppDomainSetupPropertyGetAppDomainInitializer)),
				Marshal.GetFunctionPointerForDelegate(new SystemAppDomainSetupPropertySetAppDomainInitializerDelegate(SystemAppDomainSetupPropertySetAppDomainInitializer)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineApplicationAddEventOnBeforeRenderDelegate(UnityEngineApplicationAddEventOnBeforeRender)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineApplicationRemoveEventOnBeforeRenderDelegate(UnityEngineApplicationRemoveEventOnBeforeRender)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineSceneManagementSceneManagerAddEventSceneLoadedDelegate(UnityEngineSceneManagementSceneManagerAddEventSceneLoaded)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineSceneManagementSceneManagerRemoveEventSceneLoadedDelegate(UnityEngineSceneManagementSceneManagerRemoveEventSceneLoaded)),
				Marshal.GetFunctionPointerForDelegate(new BoxSceneDelegate(BoxScene)),
				Marshal.GetFunctionPointerForDelegate(new UnboxSceneDelegate(UnboxScene)),
				Marshal.GetFunctionPointerForDelegate(new BoxLoadSceneModeDelegate(BoxLoadSceneMode)),
				Marshal.GetFunctionPointerForDelegate(new UnboxLoadSceneModeDelegate(UnboxLoadSceneMode)),
				Marshal.GetFunctionPointerForDelegate(new SystemCollectionsIEnumeratorPropertyGetCurrentDelegate(SystemCollectionsIEnumeratorPropertyGetCurrent)),
				Marshal.GetFunctionPointerForDelegate(new SystemCollectionsIEnumeratorMethodMoveNextDelegate(SystemCollectionsIEnumeratorMethodMoveNext)),
				Marshal.GetFunctionPointerForDelegate(new ReleaseSystemCollectionsGenericIComparerSystemInt32Delegate(ReleaseSystemCollectionsGenericIComparerSystemInt32)),
				Marshal.GetFunctionPointerForDelegate(new SystemCollectionsGenericIComparerSystemInt32ConstructorDelegate(SystemCollectionsGenericIComparerSystemInt32Constructor)),
				Marshal.GetFunctionPointerForDelegate(new ReleaseSystemCollectionsGenericIComparerSystemStringDelegate(ReleaseSystemCollectionsGenericIComparerSystemString)),
				Marshal.GetFunctionPointerForDelegate(new SystemCollectionsGenericIComparerSystemStringConstructorDelegate(SystemCollectionsGenericIComparerSystemStringConstructor)),
				Marshal.GetFunctionPointerForDelegate(new ReleaseSystemStringComparerDelegate(ReleaseSystemStringComparer)),
				Marshal.GetFunctionPointerForDelegate(new SystemStringComparerConstructorDelegate(SystemStringComparerConstructor)),
				Marshal.GetFunctionPointerForDelegate(new ReleaseSystemCollectionsICollectionDelegate(ReleaseSystemCollectionsICollection)),
				Marshal.GetFunctionPointerForDelegate(new SystemCollectionsICollectionConstructorDelegate(SystemCollectionsICollectionConstructor)),
				Marshal.GetFunctionPointerForDelegate(new ReleaseSystemCollectionsIListDelegate(ReleaseSystemCollectionsIList)),
				Marshal.GetFunctionPointerForDelegate(new SystemCollectionsIListConstructorDelegate(SystemCollectionsIListConstructor)),
				Marshal.GetFunctionPointerForDelegate(new ReleaseSystemCollectionsQueueDelegate(ReleaseSystemCollectionsQueue)),
				Marshal.GetFunctionPointerForDelegate(new SystemCollectionsQueueConstructorDelegate(SystemCollectionsQueueConstructor)),
				Marshal.GetFunctionPointerForDelegate(new ReleaseSystemComponentModelDesignIComponentChangeServiceDelegate(ReleaseSystemComponentModelDesignIComponentChangeService)),
				Marshal.GetFunctionPointerForDelegate(new SystemComponentModelDesignIComponentChangeServiceConstructorDelegate(SystemComponentModelDesignIComponentChangeServiceConstructor)),
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
				Marshal.GetFunctionPointerForDelegate(new UnboxDoubleDelegate(UnboxDouble)),
				Marshal.GetFunctionPointerForDelegate(new SystemSystemInt32Array1Constructor1Delegate(SystemSystemInt32Array1Constructor1)),
				Marshal.GetFunctionPointerForDelegate(new SystemInt32Array1GetItem1Delegate(SystemInt32Array1GetItem1)),
				Marshal.GetFunctionPointerForDelegate(new SystemInt32Array1SetItem1Delegate(SystemInt32Array1SetItem1)),
				Marshal.GetFunctionPointerForDelegate(new SystemSystemSingleArray1Constructor1Delegate(SystemSystemSingleArray1Constructor1)),
				Marshal.GetFunctionPointerForDelegate(new SystemSingleArray1GetItem1Delegate(SystemSingleArray1GetItem1)),
				Marshal.GetFunctionPointerForDelegate(new SystemSingleArray1SetItem1Delegate(SystemSingleArray1SetItem1)),
				Marshal.GetFunctionPointerForDelegate(new SystemSystemSingleArray2Constructor2Delegate(SystemSystemSingleArray2Constructor2)),
				Marshal.GetFunctionPointerForDelegate(new SystemSystemSingleArray2GetLength2Delegate(SystemSystemSingleArray2GetLength2)),
				Marshal.GetFunctionPointerForDelegate(new SystemSingleArray2GetItem2Delegate(SystemSingleArray2GetItem2)),
				Marshal.GetFunctionPointerForDelegate(new SystemSingleArray2SetItem2Delegate(SystemSingleArray2SetItem2)),
				Marshal.GetFunctionPointerForDelegate(new SystemSystemSingleArray3Constructor3Delegate(SystemSystemSingleArray3Constructor3)),
				Marshal.GetFunctionPointerForDelegate(new SystemSystemSingleArray3GetLength3Delegate(SystemSystemSingleArray3GetLength3)),
				Marshal.GetFunctionPointerForDelegate(new SystemSingleArray3GetItem3Delegate(SystemSingleArray3GetItem3)),
				Marshal.GetFunctionPointerForDelegate(new SystemSingleArray3SetItem3Delegate(SystemSingleArray3SetItem3)),
				Marshal.GetFunctionPointerForDelegate(new SystemSystemStringArray1Constructor1Delegate(SystemSystemStringArray1Constructor1)),
				Marshal.GetFunctionPointerForDelegate(new SystemStringArray1GetItem1Delegate(SystemStringArray1GetItem1)),
				Marshal.GetFunctionPointerForDelegate(new SystemStringArray1SetItem1Delegate(SystemStringArray1SetItem1)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineUnityEngineResolutionArray1Constructor1Delegate(UnityEngineUnityEngineResolutionArray1Constructor1)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineResolutionArray1GetItem1Delegate(UnityEngineResolutionArray1GetItem1)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineResolutionArray1SetItem1Delegate(UnityEngineResolutionArray1SetItem1)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineUnityEngineRaycastHitArray1Constructor1Delegate(UnityEngineUnityEngineRaycastHitArray1Constructor1)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineRaycastHitArray1GetItem1Delegate(UnityEngineRaycastHitArray1GetItem1)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineRaycastHitArray1SetItem1Delegate(UnityEngineRaycastHitArray1SetItem1)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineUnityEngineGradientColorKeyArray1Constructor1Delegate(UnityEngineUnityEngineGradientColorKeyArray1Constructor1)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineGradientColorKeyArray1GetItem1Delegate(UnityEngineGradientColorKeyArray1GetItem1)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineGradientColorKeyArray1SetItem1Delegate(UnityEngineGradientColorKeyArray1SetItem1)),
				Marshal.GetFunctionPointerForDelegate(new ReleaseSystemActionDelegate(ReleaseSystemAction)),
				Marshal.GetFunctionPointerForDelegate(new SystemActionConstructorDelegate(SystemActionConstructor)),
				Marshal.GetFunctionPointerForDelegate(new SystemActionAddDelegate(SystemActionAdd)),
				Marshal.GetFunctionPointerForDelegate(new SystemActionRemoveDelegate(SystemActionRemove)),
				Marshal.GetFunctionPointerForDelegate(new SystemActionInvokeDelegate(SystemActionInvoke)),
				Marshal.GetFunctionPointerForDelegate(new ReleaseSystemActionSystemSingleDelegate(ReleaseSystemActionSystemSingle)),
				Marshal.GetFunctionPointerForDelegate(new SystemActionSystemSingleConstructorDelegate(SystemActionSystemSingleConstructor)),
				Marshal.GetFunctionPointerForDelegate(new SystemActionSystemSingleAddDelegate(SystemActionSystemSingleAdd)),
				Marshal.GetFunctionPointerForDelegate(new SystemActionSystemSingleRemoveDelegate(SystemActionSystemSingleRemove)),
				Marshal.GetFunctionPointerForDelegate(new SystemActionSystemSingleInvokeDelegate(SystemActionSystemSingleInvoke)),
				Marshal.GetFunctionPointerForDelegate(new ReleaseSystemActionSystemSingle_SystemSingleDelegate(ReleaseSystemActionSystemSingle_SystemSingle)),
				Marshal.GetFunctionPointerForDelegate(new SystemActionSystemSingle_SystemSingleConstructorDelegate(SystemActionSystemSingle_SystemSingleConstructor)),
				Marshal.GetFunctionPointerForDelegate(new SystemActionSystemSingle_SystemSingleAddDelegate(SystemActionSystemSingle_SystemSingleAdd)),
				Marshal.GetFunctionPointerForDelegate(new SystemActionSystemSingle_SystemSingleRemoveDelegate(SystemActionSystemSingle_SystemSingleRemove)),
				Marshal.GetFunctionPointerForDelegate(new SystemActionSystemSingle_SystemSingleInvokeDelegate(SystemActionSystemSingle_SystemSingleInvoke)),
				Marshal.GetFunctionPointerForDelegate(new ReleaseSystemFuncSystemInt32_SystemSingle_SystemDoubleDelegate(ReleaseSystemFuncSystemInt32_SystemSingle_SystemDouble)),
				Marshal.GetFunctionPointerForDelegate(new SystemFuncSystemInt32_SystemSingle_SystemDoubleConstructorDelegate(SystemFuncSystemInt32_SystemSingle_SystemDoubleConstructor)),
				Marshal.GetFunctionPointerForDelegate(new SystemFuncSystemInt32_SystemSingle_SystemDoubleAddDelegate(SystemFuncSystemInt32_SystemSingle_SystemDoubleAdd)),
				Marshal.GetFunctionPointerForDelegate(new SystemFuncSystemInt32_SystemSingle_SystemDoubleRemoveDelegate(SystemFuncSystemInt32_SystemSingle_SystemDoubleRemove)),
				Marshal.GetFunctionPointerForDelegate(new SystemFuncSystemInt32_SystemSingle_SystemDoubleInvokeDelegate(SystemFuncSystemInt32_SystemSingle_SystemDoubleInvoke)),
				Marshal.GetFunctionPointerForDelegate(new ReleaseSystemFuncSystemInt16_SystemInt32_SystemStringDelegate(ReleaseSystemFuncSystemInt16_SystemInt32_SystemString)),
				Marshal.GetFunctionPointerForDelegate(new SystemFuncSystemInt16_SystemInt32_SystemStringConstructorDelegate(SystemFuncSystemInt16_SystemInt32_SystemStringConstructor)),
				Marshal.GetFunctionPointerForDelegate(new SystemFuncSystemInt16_SystemInt32_SystemStringAddDelegate(SystemFuncSystemInt16_SystemInt32_SystemStringAdd)),
				Marshal.GetFunctionPointerForDelegate(new SystemFuncSystemInt16_SystemInt32_SystemStringRemoveDelegate(SystemFuncSystemInt16_SystemInt32_SystemStringRemove)),
				Marshal.GetFunctionPointerForDelegate(new SystemFuncSystemInt16_SystemInt32_SystemStringInvokeDelegate(SystemFuncSystemInt16_SystemInt32_SystemStringInvoke)),
				Marshal.GetFunctionPointerForDelegate(new ReleaseSystemAppDomainInitializerDelegate(ReleaseSystemAppDomainInitializer)),
				Marshal.GetFunctionPointerForDelegate(new SystemAppDomainInitializerConstructorDelegate(SystemAppDomainInitializerConstructor)),
				Marshal.GetFunctionPointerForDelegate(new SystemAppDomainInitializerAddDelegate(SystemAppDomainInitializerAdd)),
				Marshal.GetFunctionPointerForDelegate(new SystemAppDomainInitializerRemoveDelegate(SystemAppDomainInitializerRemove)),
				Marshal.GetFunctionPointerForDelegate(new SystemAppDomainInitializerInvokeDelegate(SystemAppDomainInitializerInvoke)),
				Marshal.GetFunctionPointerForDelegate(new ReleaseUnityEngineEventsUnityActionDelegate(ReleaseUnityEngineEventsUnityAction)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineEventsUnityActionConstructorDelegate(UnityEngineEventsUnityActionConstructor)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineEventsUnityActionAddDelegate(UnityEngineEventsUnityActionAdd)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineEventsUnityActionRemoveDelegate(UnityEngineEventsUnityActionRemove)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineEventsUnityActionInvokeDelegate(UnityEngineEventsUnityActionInvoke)),
				Marshal.GetFunctionPointerForDelegate(new ReleaseUnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeDelegate(ReleaseUnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneMode)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeConstructorDelegate(UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeConstructor)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeAddDelegate(UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeAdd)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeRemoveDelegate(UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeRemove)),
				Marshal.GetFunctionPointerForDelegate(new UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeInvokeDelegate(UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeInvoke)),
				Marshal.GetFunctionPointerForDelegate(new ReleaseSystemComponentModelDesignComponentEventHandlerDelegate(ReleaseSystemComponentModelDesignComponentEventHandler)),
				Marshal.GetFunctionPointerForDelegate(new SystemComponentModelDesignComponentEventHandlerConstructorDelegate(SystemComponentModelDesignComponentEventHandlerConstructor)),
				Marshal.GetFunctionPointerForDelegate(new SystemComponentModelDesignComponentEventHandlerAddDelegate(SystemComponentModelDesignComponentEventHandlerAdd)),
				Marshal.GetFunctionPointerForDelegate(new SystemComponentModelDesignComponentEventHandlerRemoveDelegate(SystemComponentModelDesignComponentEventHandlerRemove)),
				Marshal.GetFunctionPointerForDelegate(new SystemComponentModelDesignComponentEventHandlerInvokeDelegate(SystemComponentModelDesignComponentEventHandlerInvoke)),
				Marshal.GetFunctionPointerForDelegate(new ReleaseSystemComponentModelDesignComponentChangingEventHandlerDelegate(ReleaseSystemComponentModelDesignComponentChangingEventHandler)),
				Marshal.GetFunctionPointerForDelegate(new SystemComponentModelDesignComponentChangingEventHandlerConstructorDelegate(SystemComponentModelDesignComponentChangingEventHandlerConstructor)),
				Marshal.GetFunctionPointerForDelegate(new SystemComponentModelDesignComponentChangingEventHandlerAddDelegate(SystemComponentModelDesignComponentChangingEventHandlerAdd)),
				Marshal.GetFunctionPointerForDelegate(new SystemComponentModelDesignComponentChangingEventHandlerRemoveDelegate(SystemComponentModelDesignComponentChangingEventHandlerRemove)),
				Marshal.GetFunctionPointerForDelegate(new SystemComponentModelDesignComponentChangingEventHandlerInvokeDelegate(SystemComponentModelDesignComponentChangingEventHandlerInvoke)),
				Marshal.GetFunctionPointerForDelegate(new ReleaseSystemComponentModelDesignComponentChangedEventHandlerDelegate(ReleaseSystemComponentModelDesignComponentChangedEventHandler)),
				Marshal.GetFunctionPointerForDelegate(new SystemComponentModelDesignComponentChangedEventHandlerConstructorDelegate(SystemComponentModelDesignComponentChangedEventHandlerConstructor)),
				Marshal.GetFunctionPointerForDelegate(new SystemComponentModelDesignComponentChangedEventHandlerAddDelegate(SystemComponentModelDesignComponentChangedEventHandlerAdd)),
				Marshal.GetFunctionPointerForDelegate(new SystemComponentModelDesignComponentChangedEventHandlerRemoveDelegate(SystemComponentModelDesignComponentChangedEventHandlerRemove)),
				Marshal.GetFunctionPointerForDelegate(new SystemComponentModelDesignComponentChangedEventHandlerInvokeDelegate(SystemComponentModelDesignComponentChangedEventHandlerInvoke)),
				Marshal.GetFunctionPointerForDelegate(new ReleaseSystemComponentModelDesignComponentRenameEventHandlerDelegate(ReleaseSystemComponentModelDesignComponentRenameEventHandler)),
				Marshal.GetFunctionPointerForDelegate(new SystemComponentModelDesignComponentRenameEventHandlerConstructorDelegate(SystemComponentModelDesignComponentRenameEventHandlerConstructor)),
				Marshal.GetFunctionPointerForDelegate(new SystemComponentModelDesignComponentRenameEventHandlerAddDelegate(SystemComponentModelDesignComponentRenameEventHandlerAdd)),
				Marshal.GetFunctionPointerForDelegate(new SystemComponentModelDesignComponentRenameEventHandlerRemoveDelegate(SystemComponentModelDesignComponentRenameEventHandlerRemove)),
				Marshal.GetFunctionPointerForDelegate(new SystemComponentModelDesignComponentRenameEventHandlerInvokeDelegate(SystemComponentModelDesignComponentRenameEventHandlerInvoke))
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
		
		/*BEGIN BASE TYPES*/
		class SystemCollectionsGenericIComparerSystemInt32 : System.Collections.Generic.IComparer<int>
		{
			public int CppHandle;
			
			public SystemCollectionsGenericIComparerSystemInt32(int cppHandle)
			{
				CppHandle = cppHandle;
			}
			
			public int Compare(int x, int y)
			{
				if (CppHandle != 0)
				{
					int thisHandle = CppHandle;
					var returnVal = NativeScript.Bindings.SystemCollectionsGenericIComparerSystemInt32Compare(thisHandle, x, y);
					if (NativeScript.Bindings.UnhandledCppException != null)
					{
						Exception ex = NativeScript.Bindings.UnhandledCppException;
						NativeScript.Bindings.UnhandledCppException = null;
						throw ex;
					}
					return returnVal;
				}
				return default(int);
			}
			
		}
		
		class SystemCollectionsGenericIComparerSystemString : System.Collections.Generic.IComparer<string>
		{
			public int CppHandle;
			
			public SystemCollectionsGenericIComparerSystemString(int cppHandle)
			{
				CppHandle = cppHandle;
			}
			
			public int Compare(string x, string y)
			{
				if (CppHandle != 0)
				{
					int thisHandle = CppHandle;
					int xHandle = NativeScript.Bindings.ObjectStore.GetHandle(x);
					int yHandle = NativeScript.Bindings.ObjectStore.GetHandle(y);
					var returnVal = NativeScript.Bindings.SystemCollectionsGenericIComparerSystemStringCompare(thisHandle, xHandle, yHandle);
					if (NativeScript.Bindings.UnhandledCppException != null)
					{
						Exception ex = NativeScript.Bindings.UnhandledCppException;
						NativeScript.Bindings.UnhandledCppException = null;
						throw ex;
					}
					return returnVal;
				}
				return default(int);
			}
			
		}
		
		class SystemStringComparer : System.StringComparer
		{
			public int CppHandle;
			
			public SystemStringComparer(int cppHandle)
			{
				CppHandle = cppHandle;
			}
			
			public override int Compare(string x, string y)
			{
				if (CppHandle != 0)
				{
					int thisHandle = CppHandle;
					int xHandle = NativeScript.Bindings.ObjectStore.GetHandle(x);
					int yHandle = NativeScript.Bindings.ObjectStore.GetHandle(y);
					var returnVal = NativeScript.Bindings.SystemStringComparerCompare(thisHandle, xHandle, yHandle);
					if (NativeScript.Bindings.UnhandledCppException != null)
					{
						Exception ex = NativeScript.Bindings.UnhandledCppException;
						NativeScript.Bindings.UnhandledCppException = null;
						throw ex;
					}
					return returnVal;
				}
				return default(int);
			}
			
			public override bool Equals(string x, string y)
			{
				if (CppHandle != 0)
				{
					int thisHandle = CppHandle;
					int xHandle = NativeScript.Bindings.ObjectStore.GetHandle(x);
					int yHandle = NativeScript.Bindings.ObjectStore.GetHandle(y);
					var returnVal = NativeScript.Bindings.SystemStringComparerEquals(thisHandle, xHandle, yHandle);
					if (NativeScript.Bindings.UnhandledCppException != null)
					{
						Exception ex = NativeScript.Bindings.UnhandledCppException;
						NativeScript.Bindings.UnhandledCppException = null;
						throw ex;
					}
					return returnVal;
				}
				return default(bool);
			}
			
			public override int GetHashCode(string obj)
			{
				if (CppHandle != 0)
				{
					int thisHandle = CppHandle;
					int objHandle = NativeScript.Bindings.ObjectStore.GetHandle(obj);
					var returnVal = NativeScript.Bindings.SystemStringComparerGetHashCode(thisHandle, objHandle);
					if (NativeScript.Bindings.UnhandledCppException != null)
					{
						Exception ex = NativeScript.Bindings.UnhandledCppException;
						NativeScript.Bindings.UnhandledCppException = null;
						throw ex;
					}
					return returnVal;
				}
				return default(int);
			}
			
		}
		
		class SystemCollectionsICollection : System.Collections.ICollection
		{
			public int CppHandle;
			
			public SystemCollectionsICollection(int cppHandle)
			{
				CppHandle = cppHandle;
			}
			
			public void CopyTo(System.Array array, int index)
			{
				if (CppHandle != 0)
				{
					int thisHandle = CppHandle;
					int arrayHandle = NativeScript.Bindings.ObjectStore.GetHandle(array);
					NativeScript.Bindings.SystemCollectionsICollectionCopyTo(thisHandle, arrayHandle, index);
					if (NativeScript.Bindings.UnhandledCppException != null)
					{
						Exception ex = NativeScript.Bindings.UnhandledCppException;
						NativeScript.Bindings.UnhandledCppException = null;
						throw ex;
					}
				}
			}
			
			public System.Collections.IEnumerator GetEnumerator()
			{
				if (CppHandle != 0)
				{
					int thisHandle = CppHandle;
					var returnVal = NativeScript.Bindings.SystemCollectionsICollectionGetEnumerator(thisHandle);
					if (NativeScript.Bindings.UnhandledCppException != null)
					{
						Exception ex = NativeScript.Bindings.UnhandledCppException;
						NativeScript.Bindings.UnhandledCppException = null;
						throw ex;
					}
					return (System.Collections.IEnumerator)NativeScript.Bindings.ObjectStore.Get(returnVal);
				}
				return default(System.Collections.IEnumerator);
			}
			
			public int Count
			{
				get
				{
					if (CppHandle != 0)
					{
						int thisHandle = CppHandle;
						var returnVal = NativeScript.Bindings.SystemCollectionsICollectionGetCount(thisHandle);
						if (NativeScript.Bindings.UnhandledCppException != null)
						{
							Exception ex = NativeScript.Bindings.UnhandledCppException;
							NativeScript.Bindings.UnhandledCppException = null;
							throw ex;
						}
						return returnVal;
					}
					return default(int);
				}
			}
			
			public bool IsSynchronized
			{
				get
				{
					if (CppHandle != 0)
					{
						int thisHandle = CppHandle;
						var returnVal = NativeScript.Bindings.SystemCollectionsICollectionGetIsSynchronized(thisHandle);
						if (NativeScript.Bindings.UnhandledCppException != null)
						{
							Exception ex = NativeScript.Bindings.UnhandledCppException;
							NativeScript.Bindings.UnhandledCppException = null;
							throw ex;
						}
						return returnVal;
					}
					return default(bool);
				}
			}
			
			public object SyncRoot
			{
				get
				{
					if (CppHandle != 0)
					{
						int thisHandle = CppHandle;
						var returnVal = NativeScript.Bindings.SystemCollectionsICollectionGetSyncRoot(thisHandle);
						if (NativeScript.Bindings.UnhandledCppException != null)
						{
							Exception ex = NativeScript.Bindings.UnhandledCppException;
							NativeScript.Bindings.UnhandledCppException = null;
							throw ex;
						}
						return NativeScript.Bindings.ObjectStore.Get(returnVal);
					}
					return default(object);
				}
			}
			
		}
		
		class SystemCollectionsIList : System.Collections.IList
		{
			public int CppHandle;
			
			public SystemCollectionsIList(int cppHandle)
			{
				CppHandle = cppHandle;
			}
			
			public int Add(object value)
			{
				if (CppHandle != 0)
				{
					int thisHandle = CppHandle;
					int valueHandle = NativeScript.Bindings.ObjectStore.GetHandle(value);
					var returnVal = NativeScript.Bindings.SystemCollectionsIListAdd(thisHandle, valueHandle);
					if (NativeScript.Bindings.UnhandledCppException != null)
					{
						Exception ex = NativeScript.Bindings.UnhandledCppException;
						NativeScript.Bindings.UnhandledCppException = null;
						throw ex;
					}
					return returnVal;
				}
				return default(int);
			}
			
			public void Clear()
			{
				if (CppHandle != 0)
				{
					int thisHandle = CppHandle;
					NativeScript.Bindings.SystemCollectionsIListClear(thisHandle);
					if (NativeScript.Bindings.UnhandledCppException != null)
					{
						Exception ex = NativeScript.Bindings.UnhandledCppException;
						NativeScript.Bindings.UnhandledCppException = null;
						throw ex;
					}
				}
			}
			
			public bool Contains(object value)
			{
				if (CppHandle != 0)
				{
					int thisHandle = CppHandle;
					int valueHandle = NativeScript.Bindings.ObjectStore.GetHandle(value);
					var returnVal = NativeScript.Bindings.SystemCollectionsIListContains(thisHandle, valueHandle);
					if (NativeScript.Bindings.UnhandledCppException != null)
					{
						Exception ex = NativeScript.Bindings.UnhandledCppException;
						NativeScript.Bindings.UnhandledCppException = null;
						throw ex;
					}
					return returnVal;
				}
				return default(bool);
			}
			
			public int IndexOf(object value)
			{
				if (CppHandle != 0)
				{
					int thisHandle = CppHandle;
					int valueHandle = NativeScript.Bindings.ObjectStore.GetHandle(value);
					var returnVal = NativeScript.Bindings.SystemCollectionsIListIndexOf(thisHandle, valueHandle);
					if (NativeScript.Bindings.UnhandledCppException != null)
					{
						Exception ex = NativeScript.Bindings.UnhandledCppException;
						NativeScript.Bindings.UnhandledCppException = null;
						throw ex;
					}
					return returnVal;
				}
				return default(int);
			}
			
			public void Insert(int index, object value)
			{
				if (CppHandle != 0)
				{
					int thisHandle = CppHandle;
					int valueHandle = NativeScript.Bindings.ObjectStore.GetHandle(value);
					NativeScript.Bindings.SystemCollectionsIListInsert(thisHandle, index, valueHandle);
					if (NativeScript.Bindings.UnhandledCppException != null)
					{
						Exception ex = NativeScript.Bindings.UnhandledCppException;
						NativeScript.Bindings.UnhandledCppException = null;
						throw ex;
					}
				}
			}
			
			public void Remove(object value)
			{
				if (CppHandle != 0)
				{
					int thisHandle = CppHandle;
					int valueHandle = NativeScript.Bindings.ObjectStore.GetHandle(value);
					NativeScript.Bindings.SystemCollectionsIListRemove(thisHandle, valueHandle);
					if (NativeScript.Bindings.UnhandledCppException != null)
					{
						Exception ex = NativeScript.Bindings.UnhandledCppException;
						NativeScript.Bindings.UnhandledCppException = null;
						throw ex;
					}
				}
			}
			
			public void RemoveAt(int index)
			{
				if (CppHandle != 0)
				{
					int thisHandle = CppHandle;
					NativeScript.Bindings.SystemCollectionsIListRemoveAt(thisHandle, index);
					if (NativeScript.Bindings.UnhandledCppException != null)
					{
						Exception ex = NativeScript.Bindings.UnhandledCppException;
						NativeScript.Bindings.UnhandledCppException = null;
						throw ex;
					}
				}
			}
			
			public System.Collections.IEnumerator GetEnumerator()
			{
				if (CppHandle != 0)
				{
					int thisHandle = CppHandle;
					var returnVal = NativeScript.Bindings.SystemCollectionsIListGetEnumerator(thisHandle);
					if (NativeScript.Bindings.UnhandledCppException != null)
					{
						Exception ex = NativeScript.Bindings.UnhandledCppException;
						NativeScript.Bindings.UnhandledCppException = null;
						throw ex;
					}
					return (System.Collections.IEnumerator)NativeScript.Bindings.ObjectStore.Get(returnVal);
				}
				return default(System.Collections.IEnumerator);
			}
			
			public void CopyTo(System.Array array, int index)
			{
				if (CppHandle != 0)
				{
					int thisHandle = CppHandle;
					int arrayHandle = NativeScript.Bindings.ObjectStore.GetHandle(array);
					NativeScript.Bindings.SystemCollectionsIListCopyTo(thisHandle, arrayHandle, index);
					if (NativeScript.Bindings.UnhandledCppException != null)
					{
						Exception ex = NativeScript.Bindings.UnhandledCppException;
						NativeScript.Bindings.UnhandledCppException = null;
						throw ex;
					}
				}
			}
			
			public bool IsFixedSize
			{
				get
				{
					if (CppHandle != 0)
					{
						int thisHandle = CppHandle;
						var returnVal = NativeScript.Bindings.SystemCollectionsIListGetIsFixedSize(thisHandle);
						if (NativeScript.Bindings.UnhandledCppException != null)
						{
							Exception ex = NativeScript.Bindings.UnhandledCppException;
							NativeScript.Bindings.UnhandledCppException = null;
							throw ex;
						}
						return returnVal;
					}
					return default(bool);
				}
			}
			
			public bool IsReadOnly
			{
				get
				{
					if (CppHandle != 0)
					{
						int thisHandle = CppHandle;
						var returnVal = NativeScript.Bindings.SystemCollectionsIListGetIsReadOnly(thisHandle);
						if (NativeScript.Bindings.UnhandledCppException != null)
						{
							Exception ex = NativeScript.Bindings.UnhandledCppException;
							NativeScript.Bindings.UnhandledCppException = null;
							throw ex;
						}
						return returnVal;
					}
					return default(bool);
				}
			}
			
			public object this[int index]
			{
				get
				{
					if (CppHandle != 0)
					{
						int thisHandle = CppHandle;
						var returnVal = NativeScript.Bindings.SystemCollectionsIListGetItem(thisHandle, index);
						if (NativeScript.Bindings.UnhandledCppException != null)
						{
							Exception ex = NativeScript.Bindings.UnhandledCppException;
							NativeScript.Bindings.UnhandledCppException = null;
							throw ex;
						}
						return NativeScript.Bindings.ObjectStore.Get(returnVal);
					}
					return default(object);
				}
				set
				{
					if (CppHandle != 0)
					{
						int thisHandle = CppHandle;
						int valueHandle = NativeScript.Bindings.ObjectStore.GetHandle(value);
						NativeScript.Bindings.SystemCollectionsIListSetItem(thisHandle, index, valueHandle);
						if (NativeScript.Bindings.UnhandledCppException != null)
						{
							Exception ex = NativeScript.Bindings.UnhandledCppException;
							NativeScript.Bindings.UnhandledCppException = null;
							throw ex;
						}
					}
				}
			}
			
			public int Count
			{
				get
				{
					if (CppHandle != 0)
					{
						int thisHandle = CppHandle;
						var returnVal = NativeScript.Bindings.SystemCollectionsIListGetCount(thisHandle);
						if (NativeScript.Bindings.UnhandledCppException != null)
						{
							Exception ex = NativeScript.Bindings.UnhandledCppException;
							NativeScript.Bindings.UnhandledCppException = null;
							throw ex;
						}
						return returnVal;
					}
					return default(int);
				}
			}
			
			public bool IsSynchronized
			{
				get
				{
					if (CppHandle != 0)
					{
						int thisHandle = CppHandle;
						var returnVal = NativeScript.Bindings.SystemCollectionsIListGetIsSynchronized(thisHandle);
						if (NativeScript.Bindings.UnhandledCppException != null)
						{
							Exception ex = NativeScript.Bindings.UnhandledCppException;
							NativeScript.Bindings.UnhandledCppException = null;
							throw ex;
						}
						return returnVal;
					}
					return default(bool);
				}
			}
			
			public object SyncRoot
			{
				get
				{
					if (CppHandle != 0)
					{
						int thisHandle = CppHandle;
						var returnVal = NativeScript.Bindings.SystemCollectionsIListGetSyncRoot(thisHandle);
						if (NativeScript.Bindings.UnhandledCppException != null)
						{
							Exception ex = NativeScript.Bindings.UnhandledCppException;
							NativeScript.Bindings.UnhandledCppException = null;
							throw ex;
						}
						return NativeScript.Bindings.ObjectStore.Get(returnVal);
					}
					return default(object);
				}
			}
			
		}
		
		class SystemCollectionsQueue : System.Collections.Queue
		{
			public int CppHandle;
			
			public SystemCollectionsQueue(int cppHandle)
			{
				CppHandle = cppHandle;
			}
			
			public override int Count
			{
				get
				{
					if (CppHandle != 0)
					{
						int thisHandle = CppHandle;
						var returnVal = NativeScript.Bindings.SystemCollectionsQueueGetCount(thisHandle);
						if (NativeScript.Bindings.UnhandledCppException != null)
						{
							Exception ex = NativeScript.Bindings.UnhandledCppException;
							NativeScript.Bindings.UnhandledCppException = null;
							throw ex;
						}
						return returnVal;
					}
					return default(int);
				}
			}
			
		}
		
		class SystemComponentModelDesignIComponentChangeService : System.ComponentModel.Design.IComponentChangeService
		{
			public int CppHandle;
			
			public SystemComponentModelDesignIComponentChangeService(int cppHandle)
			{
				CppHandle = cppHandle;
			}
			
			public void OnComponentChanged(object component, System.ComponentModel.MemberDescriptor member, object oldValue, object newValue)
			{
				if (CppHandle != 0)
				{
					int thisHandle = CppHandle;
					int componentHandle = NativeScript.Bindings.ObjectStore.GetHandle(component);
					int memberHandle = NativeScript.Bindings.ObjectStore.GetHandle(member);
					int oldValueHandle = NativeScript.Bindings.ObjectStore.GetHandle(oldValue);
					int newValueHandle = NativeScript.Bindings.ObjectStore.GetHandle(newValue);
					NativeScript.Bindings.SystemComponentModelDesignIComponentChangeServiceOnComponentChanged(thisHandle, componentHandle, memberHandle, oldValueHandle, newValueHandle);
					if (NativeScript.Bindings.UnhandledCppException != null)
					{
						Exception ex = NativeScript.Bindings.UnhandledCppException;
						NativeScript.Bindings.UnhandledCppException = null;
						throw ex;
					}
				}
			}
			
			public void OnComponentChanging(object component, System.ComponentModel.MemberDescriptor member)
			{
				if (CppHandle != 0)
				{
					int thisHandle = CppHandle;
					int componentHandle = NativeScript.Bindings.ObjectStore.GetHandle(component);
					int memberHandle = NativeScript.Bindings.ObjectStore.GetHandle(member);
					NativeScript.Bindings.SystemComponentModelDesignIComponentChangeServiceOnComponentChanging(thisHandle, componentHandle, memberHandle);
					if (NativeScript.Bindings.UnhandledCppException != null)
					{
						Exception ex = NativeScript.Bindings.UnhandledCppException;
						NativeScript.Bindings.UnhandledCppException = null;
						throw ex;
					}
				}
			}
			
			public event System.ComponentModel.Design.ComponentEventHandler ComponentAdded
			{
				add
				{
					if (CppHandle != 0)
					{
						int thisHandle = CppHandle;
						int valueHandle = NativeScript.Bindings.ObjectStore.GetHandle(value);
						NativeScript.Bindings.SystemComponentModelDesignIComponentChangeServiceAddComponentAdded(thisHandle, valueHandle);
						if (NativeScript.Bindings.UnhandledCppException != null)
						{
							Exception ex = NativeScript.Bindings.UnhandledCppException;
							NativeScript.Bindings.UnhandledCppException = null;
							throw ex;
						}
					}
				}
				remove
				{
					if (CppHandle != 0)
					{
						int thisHandle = CppHandle;
						int valueHandle = NativeScript.Bindings.ObjectStore.GetHandle(value);
						NativeScript.Bindings.SystemComponentModelDesignIComponentChangeServiceRemoveComponentAdded(thisHandle, valueHandle);
						if (NativeScript.Bindings.UnhandledCppException != null)
						{
							Exception ex = NativeScript.Bindings.UnhandledCppException;
							NativeScript.Bindings.UnhandledCppException = null;
							throw ex;
						}
					}
				}
			}
			
			public event System.ComponentModel.Design.ComponentEventHandler ComponentAdding
			{
				add
				{
					if (CppHandle != 0)
					{
						int thisHandle = CppHandle;
						int valueHandle = NativeScript.Bindings.ObjectStore.GetHandle(value);
						NativeScript.Bindings.SystemComponentModelDesignIComponentChangeServiceAddComponentAdding(thisHandle, valueHandle);
						if (NativeScript.Bindings.UnhandledCppException != null)
						{
							Exception ex = NativeScript.Bindings.UnhandledCppException;
							NativeScript.Bindings.UnhandledCppException = null;
							throw ex;
						}
					}
				}
				remove
				{
					if (CppHandle != 0)
					{
						int thisHandle = CppHandle;
						int valueHandle = NativeScript.Bindings.ObjectStore.GetHandle(value);
						NativeScript.Bindings.SystemComponentModelDesignIComponentChangeServiceRemoveComponentAdding(thisHandle, valueHandle);
						if (NativeScript.Bindings.UnhandledCppException != null)
						{
							Exception ex = NativeScript.Bindings.UnhandledCppException;
							NativeScript.Bindings.UnhandledCppException = null;
							throw ex;
						}
					}
				}
			}
			
			public event System.ComponentModel.Design.ComponentChangedEventHandler ComponentChanged
			{
				add
				{
					if (CppHandle != 0)
					{
						int thisHandle = CppHandle;
						int valueHandle = NativeScript.Bindings.ObjectStore.GetHandle(value);
						NativeScript.Bindings.SystemComponentModelDesignIComponentChangeServiceAddComponentChanged(thisHandle, valueHandle);
						if (NativeScript.Bindings.UnhandledCppException != null)
						{
							Exception ex = NativeScript.Bindings.UnhandledCppException;
							NativeScript.Bindings.UnhandledCppException = null;
							throw ex;
						}
					}
				}
				remove
				{
					if (CppHandle != 0)
					{
						int thisHandle = CppHandle;
						int valueHandle = NativeScript.Bindings.ObjectStore.GetHandle(value);
						NativeScript.Bindings.SystemComponentModelDesignIComponentChangeServiceRemoveComponentChanged(thisHandle, valueHandle);
						if (NativeScript.Bindings.UnhandledCppException != null)
						{
							Exception ex = NativeScript.Bindings.UnhandledCppException;
							NativeScript.Bindings.UnhandledCppException = null;
							throw ex;
						}
					}
				}
			}
			
			public event System.ComponentModel.Design.ComponentChangingEventHandler ComponentChanging
			{
				add
				{
					if (CppHandle != 0)
					{
						int thisHandle = CppHandle;
						int valueHandle = NativeScript.Bindings.ObjectStore.GetHandle(value);
						NativeScript.Bindings.SystemComponentModelDesignIComponentChangeServiceAddComponentChanging(thisHandle, valueHandle);
						if (NativeScript.Bindings.UnhandledCppException != null)
						{
							Exception ex = NativeScript.Bindings.UnhandledCppException;
							NativeScript.Bindings.UnhandledCppException = null;
							throw ex;
						}
					}
				}
				remove
				{
					if (CppHandle != 0)
					{
						int thisHandle = CppHandle;
						int valueHandle = NativeScript.Bindings.ObjectStore.GetHandle(value);
						NativeScript.Bindings.SystemComponentModelDesignIComponentChangeServiceRemoveComponentChanging(thisHandle, valueHandle);
						if (NativeScript.Bindings.UnhandledCppException != null)
						{
							Exception ex = NativeScript.Bindings.UnhandledCppException;
							NativeScript.Bindings.UnhandledCppException = null;
							throw ex;
						}
					}
				}
			}
			
			public event System.ComponentModel.Design.ComponentEventHandler ComponentRemoved
			{
				add
				{
					if (CppHandle != 0)
					{
						int thisHandle = CppHandle;
						int valueHandle = NativeScript.Bindings.ObjectStore.GetHandle(value);
						NativeScript.Bindings.SystemComponentModelDesignIComponentChangeServiceAddComponentRemoved(thisHandle, valueHandle);
						if (NativeScript.Bindings.UnhandledCppException != null)
						{
							Exception ex = NativeScript.Bindings.UnhandledCppException;
							NativeScript.Bindings.UnhandledCppException = null;
							throw ex;
						}
					}
				}
				remove
				{
					if (CppHandle != 0)
					{
						int thisHandle = CppHandle;
						int valueHandle = NativeScript.Bindings.ObjectStore.GetHandle(value);
						NativeScript.Bindings.SystemComponentModelDesignIComponentChangeServiceRemoveComponentRemoved(thisHandle, valueHandle);
						if (NativeScript.Bindings.UnhandledCppException != null)
						{
							Exception ex = NativeScript.Bindings.UnhandledCppException;
							NativeScript.Bindings.UnhandledCppException = null;
							throw ex;
						}
					}
				}
			}
			
			public event System.ComponentModel.Design.ComponentEventHandler ComponentRemoving
			{
				add
				{
					if (CppHandle != 0)
					{
						int thisHandle = CppHandle;
						int valueHandle = NativeScript.Bindings.ObjectStore.GetHandle(value);
						NativeScript.Bindings.SystemComponentModelDesignIComponentChangeServiceAddComponentRemoving(thisHandle, valueHandle);
						if (NativeScript.Bindings.UnhandledCppException != null)
						{
							Exception ex = NativeScript.Bindings.UnhandledCppException;
							NativeScript.Bindings.UnhandledCppException = null;
							throw ex;
						}
					}
				}
				remove
				{
					if (CppHandle != 0)
					{
						int thisHandle = CppHandle;
						int valueHandle = NativeScript.Bindings.ObjectStore.GetHandle(value);
						NativeScript.Bindings.SystemComponentModelDesignIComponentChangeServiceRemoveComponentRemoving(thisHandle, valueHandle);
						if (NativeScript.Bindings.UnhandledCppException != null)
						{
							Exception ex = NativeScript.Bindings.UnhandledCppException;
							NativeScript.Bindings.UnhandledCppException = null;
							throw ex;
						}
					}
				}
			}
			
			public event System.ComponentModel.Design.ComponentRenameEventHandler ComponentRename
			{
				add
				{
					if (CppHandle != 0)
					{
						int thisHandle = CppHandle;
						int valueHandle = NativeScript.Bindings.ObjectStore.GetHandle(value);
						NativeScript.Bindings.SystemComponentModelDesignIComponentChangeServiceAddComponentRename(thisHandle, valueHandle);
						if (NativeScript.Bindings.UnhandledCppException != null)
						{
							Exception ex = NativeScript.Bindings.UnhandledCppException;
							NativeScript.Bindings.UnhandledCppException = null;
							throw ex;
						}
					}
				}
				remove
				{
					if (CppHandle != 0)
					{
						int thisHandle = CppHandle;
						int valueHandle = NativeScript.Bindings.ObjectStore.GetHandle(value);
						NativeScript.Bindings.SystemComponentModelDesignIComponentChangeServiceRemoveComponentRename(thisHandle, valueHandle);
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
		
		class SystemAction
		{
			public int CppHandle;
			public System.Action Delegate;
			
			public SystemAction(int cppHandle)
			{
				CppHandle = cppHandle;
				Delegate = NativeInvoke;
			}
			
			public void NativeInvoke()
			{
				if (CppHandle != 0)
				{
					int thisHandle = CppHandle;
					NativeScript.Bindings.SystemActionNativeInvoke(thisHandle);
					if (NativeScript.Bindings.UnhandledCppException != null)
					{
						Exception ex = NativeScript.Bindings.UnhandledCppException;
						NativeScript.Bindings.UnhandledCppException = null;
						throw ex;
					}
				}
			}
			
		}
		
		class SystemActionSystemSingle
		{
			public int CppHandle;
			public System.Action<float> Delegate;
			
			public SystemActionSystemSingle(int cppHandle)
			{
				CppHandle = cppHandle;
				Delegate = NativeInvoke;
			}
			
			public void NativeInvoke(float obj)
			{
				if (CppHandle != 0)
				{
					int thisHandle = CppHandle;
					NativeScript.Bindings.SystemActionSystemSingleNativeInvoke(thisHandle, obj);
					if (NativeScript.Bindings.UnhandledCppException != null)
					{
						Exception ex = NativeScript.Bindings.UnhandledCppException;
						NativeScript.Bindings.UnhandledCppException = null;
						throw ex;
					}
				}
			}
			
		}
		
		class SystemActionSystemSingle_SystemSingle
		{
			public int CppHandle;
			public System.Action<float, float> Delegate;
			
			public SystemActionSystemSingle_SystemSingle(int cppHandle)
			{
				CppHandle = cppHandle;
				Delegate = NativeInvoke;
			}
			
			public void NativeInvoke(float arg1, float arg2)
			{
				if (CppHandle != 0)
				{
					int thisHandle = CppHandle;
					NativeScript.Bindings.SystemActionSystemSingle_SystemSingleNativeInvoke(thisHandle, arg1, arg2);
					if (NativeScript.Bindings.UnhandledCppException != null)
					{
						Exception ex = NativeScript.Bindings.UnhandledCppException;
						NativeScript.Bindings.UnhandledCppException = null;
						throw ex;
					}
				}
			}
			
		}
		
		class SystemFuncSystemInt32_SystemSingle_SystemDouble
		{
			public int CppHandle;
			public System.Func<int, float, double> Delegate;
			
			public SystemFuncSystemInt32_SystemSingle_SystemDouble(int cppHandle)
			{
				CppHandle = cppHandle;
				Delegate = NativeInvoke;
			}
			
			public double NativeInvoke(int arg1, float arg2)
			{
				if (CppHandle != 0)
				{
					int thisHandle = CppHandle;
					var returnVal = NativeScript.Bindings.SystemFuncSystemInt32_SystemSingle_SystemDoubleNativeInvoke(thisHandle, arg1, arg2);
					if (NativeScript.Bindings.UnhandledCppException != null)
					{
						Exception ex = NativeScript.Bindings.UnhandledCppException;
						NativeScript.Bindings.UnhandledCppException = null;
						throw ex;
					}
					return returnVal;
				}
				return default(double);
			}
			
		}
		
		class SystemFuncSystemInt16_SystemInt32_SystemString
		{
			public int CppHandle;
			public System.Func<short, int, string> Delegate;
			
			public SystemFuncSystemInt16_SystemInt32_SystemString(int cppHandle)
			{
				CppHandle = cppHandle;
				Delegate = NativeInvoke;
			}
			
			public string NativeInvoke(short arg1, int arg2)
			{
				if (CppHandle != 0)
				{
					int thisHandle = CppHandle;
					var returnVal = NativeScript.Bindings.SystemFuncSystemInt16_SystemInt32_SystemStringNativeInvoke(thisHandle, arg1, arg2);
					if (NativeScript.Bindings.UnhandledCppException != null)
					{
						Exception ex = NativeScript.Bindings.UnhandledCppException;
						NativeScript.Bindings.UnhandledCppException = null;
						throw ex;
					}
					return (string)NativeScript.Bindings.ObjectStore.Get(returnVal);
				}
				return default(string);
			}
			
		}
		
		class SystemAppDomainInitializer
		{
			public int CppHandle;
			public System.AppDomainInitializer Delegate;
			
			public SystemAppDomainInitializer(int cppHandle)
			{
				CppHandle = cppHandle;
				Delegate = NativeInvoke;
			}
			
			public void NativeInvoke(string[] args)
			{
				if (CppHandle != 0)
				{
					int thisHandle = CppHandle;
					int argsHandle = NativeScript.Bindings.ObjectStore.GetHandle(args);
					NativeScript.Bindings.SystemAppDomainInitializerNativeInvoke(thisHandle, argsHandle);
					if (NativeScript.Bindings.UnhandledCppException != null)
					{
						Exception ex = NativeScript.Bindings.UnhandledCppException;
						NativeScript.Bindings.UnhandledCppException = null;
						throw ex;
					}
				}
			}
			
		}
		
		class UnityEngineEventsUnityAction
		{
			public int CppHandle;
			public UnityEngine.Events.UnityAction Delegate;
			
			public UnityEngineEventsUnityAction(int cppHandle)
			{
				CppHandle = cppHandle;
				Delegate = NativeInvoke;
			}
			
			public void NativeInvoke()
			{
				if (CppHandle != 0)
				{
					int thisHandle = CppHandle;
					NativeScript.Bindings.UnityEngineEventsUnityActionNativeInvoke(thisHandle);
					if (NativeScript.Bindings.UnhandledCppException != null)
					{
						Exception ex = NativeScript.Bindings.UnhandledCppException;
						NativeScript.Bindings.UnhandledCppException = null;
						throw ex;
					}
				}
			}
			
		}
		
		class UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneMode
		{
			public int CppHandle;
			public UnityEngine.Events.UnityAction<UnityEngine.SceneManagement.Scene, UnityEngine.SceneManagement.LoadSceneMode> Delegate;
			
			public UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneMode(int cppHandle)
			{
				CppHandle = cppHandle;
				Delegate = NativeInvoke;
			}
			
			public void NativeInvoke(UnityEngine.SceneManagement.Scene arg0, UnityEngine.SceneManagement.LoadSceneMode arg1)
			{
				if (CppHandle != 0)
				{
					int thisHandle = CppHandle;
					NativeScript.Bindings.UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeNativeInvoke(thisHandle, arg0, arg1);
					if (NativeScript.Bindings.UnhandledCppException != null)
					{
						Exception ex = NativeScript.Bindings.UnhandledCppException;
						NativeScript.Bindings.UnhandledCppException = null;
						throw ex;
					}
				}
			}
			
		}
		
		class SystemComponentModelDesignComponentEventHandler
		{
			public int CppHandle;
			public System.ComponentModel.Design.ComponentEventHandler Delegate;
			
			public SystemComponentModelDesignComponentEventHandler(int cppHandle)
			{
				CppHandle = cppHandle;
				Delegate = NativeInvoke;
			}
			
			public void NativeInvoke(object sender, System.ComponentModel.Design.ComponentEventArgs e)
			{
				if (CppHandle != 0)
				{
					int thisHandle = CppHandle;
					int senderHandle = NativeScript.Bindings.ObjectStore.GetHandle(sender);
					int eHandle = NativeScript.Bindings.ObjectStore.GetHandle(e);
					NativeScript.Bindings.SystemComponentModelDesignComponentEventHandlerNativeInvoke(thisHandle, senderHandle, eHandle);
					if (NativeScript.Bindings.UnhandledCppException != null)
					{
						Exception ex = NativeScript.Bindings.UnhandledCppException;
						NativeScript.Bindings.UnhandledCppException = null;
						throw ex;
					}
				}
			}
			
		}
		
		class SystemComponentModelDesignComponentChangingEventHandler
		{
			public int CppHandle;
			public System.ComponentModel.Design.ComponentChangingEventHandler Delegate;
			
			public SystemComponentModelDesignComponentChangingEventHandler(int cppHandle)
			{
				CppHandle = cppHandle;
				Delegate = NativeInvoke;
			}
			
			public void NativeInvoke(object sender, System.ComponentModel.Design.ComponentChangingEventArgs e)
			{
				if (CppHandle != 0)
				{
					int thisHandle = CppHandle;
					int senderHandle = NativeScript.Bindings.ObjectStore.GetHandle(sender);
					int eHandle = NativeScript.Bindings.ObjectStore.GetHandle(e);
					NativeScript.Bindings.SystemComponentModelDesignComponentChangingEventHandlerNativeInvoke(thisHandle, senderHandle, eHandle);
					if (NativeScript.Bindings.UnhandledCppException != null)
					{
						Exception ex = NativeScript.Bindings.UnhandledCppException;
						NativeScript.Bindings.UnhandledCppException = null;
						throw ex;
					}
				}
			}
			
		}
		
		class SystemComponentModelDesignComponentChangedEventHandler
		{
			public int CppHandle;
			public System.ComponentModel.Design.ComponentChangedEventHandler Delegate;
			
			public SystemComponentModelDesignComponentChangedEventHandler(int cppHandle)
			{
				CppHandle = cppHandle;
				Delegate = NativeInvoke;
			}
			
			public void NativeInvoke(object sender, System.ComponentModel.Design.ComponentChangedEventArgs e)
			{
				if (CppHandle != 0)
				{
					int thisHandle = CppHandle;
					int senderHandle = NativeScript.Bindings.ObjectStore.GetHandle(sender);
					int eHandle = NativeScript.Bindings.ObjectStore.GetHandle(e);
					NativeScript.Bindings.SystemComponentModelDesignComponentChangedEventHandlerNativeInvoke(thisHandle, senderHandle, eHandle);
					if (NativeScript.Bindings.UnhandledCppException != null)
					{
						Exception ex = NativeScript.Bindings.UnhandledCppException;
						NativeScript.Bindings.UnhandledCppException = null;
						throw ex;
					}
				}
			}
			
		}
		
		class SystemComponentModelDesignComponentRenameEventHandler
		{
			public int CppHandle;
			public System.ComponentModel.Design.ComponentRenameEventHandler Delegate;
			
			public SystemComponentModelDesignComponentRenameEventHandler(int cppHandle)
			{
				CppHandle = cppHandle;
				Delegate = NativeInvoke;
			}
			
			public void NativeInvoke(object sender, System.ComponentModel.Design.ComponentRenameEventArgs e)
			{
				if (CppHandle != 0)
				{
					int thisHandle = CppHandle;
					int senderHandle = NativeScript.Bindings.ObjectStore.GetHandle(sender);
					int eHandle = NativeScript.Bindings.ObjectStore.GetHandle(e);
					NativeScript.Bindings.SystemComponentModelDesignComponentRenameEventHandlerNativeInvoke(thisHandle, senderHandle, eHandle);
					if (NativeScript.Bindings.UnhandledCppException != null)
					{
						Exception ex = NativeScript.Bindings.UnhandledCppException;
						NativeScript.Bindings.UnhandledCppException = null;
						throw ex;
					}
				}
			}
			
		}
		/*END BASE TYPES*/
		
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
		
		[MonoPInvokeCallback(typeof(BoxMatrix4x4Delegate))]
		static int BoxMatrix4x4(ref UnityEngine.Matrix4x4 val)
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
		
		[MonoPInvokeCallback(typeof(UnboxMatrix4x4Delegate))]
		static UnityEngine.Matrix4x4 UnboxMatrix4x4(int valHandle)
		{
			try
			{
				var val = NativeScript.Bindings.ObjectStore.Get(valHandle);
				var returnValue = (UnityEngine.Matrix4x4)val;
				return returnValue;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(UnityEngine.Matrix4x4);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(UnityEngine.Matrix4x4);
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
		
		[MonoPInvokeCallback(typeof(BoxRaycastHitDelegate))]
		static int BoxRaycastHit(int valHandle)
		{
			try
			{
				var val = (UnityEngine.RaycastHit)NativeScript.Bindings.StructStore<UnityEngine.RaycastHit>.Get(valHandle);
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
		
		[MonoPInvokeCallback(typeof(UnboxRaycastHitDelegate))]
		static int UnboxRaycastHit(int valHandle)
		{
			try
			{
				var val = NativeScript.Bindings.ObjectStore.Get(valHandle);
				var returnValue = NativeScript.Bindings.StructStore<UnityEngine.RaycastHit>.Store((UnityEngine.RaycastHit)val);
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
		
		[MonoPInvokeCallback(typeof(BoxQueryTriggerInteractionDelegate))]
		static int BoxQueryTriggerInteraction(UnityEngine.QueryTriggerInteraction val)
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
		
		[MonoPInvokeCallback(typeof(UnboxQueryTriggerInteractionDelegate))]
		static UnityEngine.QueryTriggerInteraction UnboxQueryTriggerInteraction(int valHandle)
		{
			try
			{
				var val = NativeScript.Bindings.ObjectStore.Get(valHandle);
				var returnValue = (UnityEngine.QueryTriggerInteraction)val;
				return returnValue;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(UnityEngine.QueryTriggerInteraction);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(UnityEngine.QueryTriggerInteraction);
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
		
		[MonoPInvokeCallback(typeof(BoxKeyValuePairSystemString_SystemDoubleDelegate))]
		static int BoxKeyValuePairSystemString_SystemDouble(int valHandle)
		{
			try
			{
				var val = (System.Collections.Generic.KeyValuePair<string, double>)NativeScript.Bindings.StructStore<System.Collections.Generic.KeyValuePair<string, double>>.Get(valHandle);
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
		
		[MonoPInvokeCallback(typeof(UnboxKeyValuePairSystemString_SystemDoubleDelegate))]
		static int UnboxKeyValuePairSystemString_SystemDouble(int valHandle)
		{
			try
			{
				var val = NativeScript.Bindings.ObjectStore.Get(valHandle);
				var returnValue = NativeScript.Bindings.StructStore<System.Collections.Generic.KeyValuePair<string, double>>.Store((System.Collections.Generic.KeyValuePair<string, double>)val);
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
		
		[MonoPInvokeCallback(typeof(SystemCollectionsGenericListSystemStringMethodSortSystemCollectionsGenericIComparerDelegate))]
		static void SystemCollectionsGenericListSystemStringMethodSortSystemCollectionsGenericIComparer(int thisHandle, int comparerHandle)
		{
			try
			{
				var thiz = (System.Collections.Generic.List<string>)NativeScript.Bindings.ObjectStore.Get(thisHandle);
				var comparer = (System.Collections.Generic.IComparer<string>)NativeScript.Bindings.ObjectStore.Get(comparerHandle);
				thiz.Sort(comparer);
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
		
		[MonoPInvokeCallback(typeof(SystemCollectionsGenericListSystemInt32ConstructorDelegate))]
		static int SystemCollectionsGenericListSystemInt32Constructor()
		{
			try
			{
				var returnValue = NativeScript.Bindings.ObjectStore.Store(new System.Collections.Generic.List<int>());
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
		
		[MonoPInvokeCallback(typeof(SystemCollectionsGenericListSystemInt32PropertyGetItemDelegate))]
		static int SystemCollectionsGenericListSystemInt32PropertyGetItem(int thisHandle, int index)
		{
			try
			{
				var thiz = (System.Collections.Generic.List<int>)NativeScript.Bindings.ObjectStore.Get(thisHandle);
				var returnValue = thiz[index];
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
		
		[MonoPInvokeCallback(typeof(SystemCollectionsGenericListSystemInt32PropertySetItemDelegate))]
		static void SystemCollectionsGenericListSystemInt32PropertySetItem(int thisHandle, int index, int value)
		{
			try
			{
				var thiz = (System.Collections.Generic.List<int>)NativeScript.Bindings.ObjectStore.Get(thisHandle);
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
		
		[MonoPInvokeCallback(typeof(SystemCollectionsGenericListSystemInt32MethodAddSystemInt32Delegate))]
		static void SystemCollectionsGenericListSystemInt32MethodAddSystemInt32(int thisHandle, int item)
		{
			try
			{
				var thiz = (System.Collections.Generic.List<int>)NativeScript.Bindings.ObjectStore.Get(thisHandle);
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
		
		[MonoPInvokeCallback(typeof(SystemCollectionsGenericListSystemInt32MethodSortSystemCollectionsGenericIComparerDelegate))]
		static void SystemCollectionsGenericListSystemInt32MethodSortSystemCollectionsGenericIComparer(int thisHandle, int comparerHandle)
		{
			try
			{
				var thiz = (System.Collections.Generic.List<int>)NativeScript.Bindings.ObjectStore.Get(thisHandle);
				var comparer = (System.Collections.Generic.IComparer<int>)NativeScript.Bindings.ObjectStore.Get(comparerHandle);
				thiz.Sort(comparer);
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
		
		[MonoPInvokeCallback(typeof(BoxResolutionDelegate))]
		static int BoxResolution(ref UnityEngine.Resolution val)
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
		
		[MonoPInvokeCallback(typeof(UnboxResolutionDelegate))]
		static UnityEngine.Resolution UnboxResolution(int valHandle)
		{
			try
			{
				var val = NativeScript.Bindings.ObjectStore.Get(valHandle);
				var returnValue = (UnityEngine.Resolution)val;
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
		
		[MonoPInvokeCallback(typeof(BoxRayDelegate))]
		static int BoxRay(ref UnityEngine.Ray val)
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
		
		[MonoPInvokeCallback(typeof(UnboxRayDelegate))]
		static UnityEngine.Ray UnboxRay(int valHandle)
		{
			try
			{
				var val = NativeScript.Bindings.ObjectStore.Get(valHandle);
				var returnValue = (UnityEngine.Ray)val;
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
		
		[MonoPInvokeCallback(typeof(BoxColorDelegate))]
		static int BoxColor(ref UnityEngine.Color val)
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
		
		[MonoPInvokeCallback(typeof(UnboxColorDelegate))]
		static UnityEngine.Color UnboxColor(int valHandle)
		{
			try
			{
				var val = NativeScript.Bindings.ObjectStore.Get(valHandle);
				var returnValue = (UnityEngine.Color)val;
				return returnValue;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(UnityEngine.Color);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(UnityEngine.Color);
			}
		}
		
		[MonoPInvokeCallback(typeof(BoxGradientColorKeyDelegate))]
		static int BoxGradientColorKey(ref UnityEngine.GradientColorKey val)
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
		
		[MonoPInvokeCallback(typeof(UnboxGradientColorKeyDelegate))]
		static UnityEngine.GradientColorKey UnboxGradientColorKey(int valHandle)
		{
			try
			{
				var val = NativeScript.Bindings.ObjectStore.Get(valHandle);
				var returnValue = (UnityEngine.GradientColorKey)val;
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
		
		[MonoPInvokeCallback(typeof(SystemAppDomainSetupConstructorDelegate))]
		static int SystemAppDomainSetupConstructor()
		{
			try
			{
				var returnValue = NativeScript.Bindings.ObjectStore.Store(new System.AppDomainSetup());
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
		
		[MonoPInvokeCallback(typeof(SystemAppDomainSetupPropertyGetAppDomainInitializerDelegate))]
		static int SystemAppDomainSetupPropertyGetAppDomainInitializer(int thisHandle)
		{
			try
			{
				var thiz = (System.AppDomainSetup)NativeScript.Bindings.ObjectStore.Get(thisHandle);
				var returnValue = thiz.AppDomainInitializer;
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
		
		[MonoPInvokeCallback(typeof(SystemAppDomainSetupPropertySetAppDomainInitializerDelegate))]
		static void SystemAppDomainSetupPropertySetAppDomainInitializer(int thisHandle, int valueHandle)
		{
			try
			{
				var thiz = (System.AppDomainSetup)NativeScript.Bindings.ObjectStore.Get(thisHandle);
				var value = (System.AppDomainInitializer)NativeScript.Bindings.ObjectStore.Get(valueHandle);
				thiz.AppDomainInitializer = value;
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
		
		[MonoPInvokeCallback(typeof(UnityEngineApplicationAddEventOnBeforeRenderDelegate))]
		static void UnityEngineApplicationAddEventOnBeforeRender(int delHandle)
		{
			try
			{
				var del = (UnityEngine.Events.UnityAction)NativeScript.Bindings.ObjectStore.Get(delHandle);
				UnityEngine.Application.onBeforeRender += del;
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
		
		[MonoPInvokeCallback(typeof(UnityEngineApplicationRemoveEventOnBeforeRenderDelegate))]
		static void UnityEngineApplicationRemoveEventOnBeforeRender(int delHandle)
		{
			try
			{
				var del = (UnityEngine.Events.UnityAction)NativeScript.Bindings.ObjectStore.Get(delHandle);
				UnityEngine.Application.onBeforeRender += del;
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
		
		[MonoPInvokeCallback(typeof(UnityEngineSceneManagementSceneManagerAddEventSceneLoadedDelegate))]
		static void UnityEngineSceneManagementSceneManagerAddEventSceneLoaded(int delHandle)
		{
			try
			{
				var del = (UnityEngine.Events.UnityAction<UnityEngine.SceneManagement.Scene, UnityEngine.SceneManagement.LoadSceneMode>)NativeScript.Bindings.ObjectStore.Get(delHandle);
				UnityEngine.SceneManagement.SceneManager.sceneLoaded += del;
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
		
		[MonoPInvokeCallback(typeof(UnityEngineSceneManagementSceneManagerRemoveEventSceneLoadedDelegate))]
		static void UnityEngineSceneManagementSceneManagerRemoveEventSceneLoaded(int delHandle)
		{
			try
			{
				var del = (UnityEngine.Events.UnityAction<UnityEngine.SceneManagement.Scene, UnityEngine.SceneManagement.LoadSceneMode>)NativeScript.Bindings.ObjectStore.Get(delHandle);
				UnityEngine.SceneManagement.SceneManager.sceneLoaded += del;
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
		
		[MonoPInvokeCallback(typeof(BoxSceneDelegate))]
		static int BoxScene(ref UnityEngine.SceneManagement.Scene val)
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
		
		[MonoPInvokeCallback(typeof(UnboxSceneDelegate))]
		static UnityEngine.SceneManagement.Scene UnboxScene(int valHandle)
		{
			try
			{
				var val = NativeScript.Bindings.ObjectStore.Get(valHandle);
				var returnValue = (UnityEngine.SceneManagement.Scene)val;
				return returnValue;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(UnityEngine.SceneManagement.Scene);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(UnityEngine.SceneManagement.Scene);
			}
		}
		
		[MonoPInvokeCallback(typeof(BoxLoadSceneModeDelegate))]
		static int BoxLoadSceneMode(UnityEngine.SceneManagement.LoadSceneMode val)
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
		
		[MonoPInvokeCallback(typeof(UnboxLoadSceneModeDelegate))]
		static UnityEngine.SceneManagement.LoadSceneMode UnboxLoadSceneMode(int valHandle)
		{
			try
			{
				var val = NativeScript.Bindings.ObjectStore.Get(valHandle);
				var returnValue = (UnityEngine.SceneManagement.LoadSceneMode)val;
				return returnValue;
			}
			catch (System.NullReferenceException ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpExceptionSystemNullReferenceException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(UnityEngine.SceneManagement.LoadSceneMode);
			}
			catch (System.Exception ex)
			{
				UnityEngine.Debug.LogException(ex);
				NativeScript.Bindings.SetCsharpException(NativeScript.Bindings.ObjectStore.Store(ex));
				return default(UnityEngine.SceneManagement.LoadSceneMode);
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
		
		[MonoPInvokeCallback(typeof(SystemCollectionsGenericIComparerSystemInt32ConstructorDelegate))]
		static void SystemCollectionsGenericIComparerSystemInt32Constructor(int cppHandle, ref int handle)
		{
			try
			{
				var thiz = new SystemCollectionsGenericIComparerSystemInt32(cppHandle);
				handle = NativeScript.Bindings.ObjectStore.Store(thiz);
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
		
		[MonoPInvokeCallback(typeof(ReleaseSystemCollectionsGenericIComparerSystemInt32Delegate))]
		static void ReleaseSystemCollectionsGenericIComparerSystemInt32(int handle)
		{
			try
			{
				NativeScript.Bindings.ObjectStore.Remove(handle);
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
		
		[MonoPInvokeCallback(typeof(SystemCollectionsGenericIComparerSystemStringConstructorDelegate))]
		static void SystemCollectionsGenericIComparerSystemStringConstructor(int cppHandle, ref int handle)
		{
			try
			{
				var thiz = new SystemCollectionsGenericIComparerSystemString(cppHandle);
				handle = NativeScript.Bindings.ObjectStore.Store(thiz);
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
		
		[MonoPInvokeCallback(typeof(ReleaseSystemCollectionsGenericIComparerSystemStringDelegate))]
		static void ReleaseSystemCollectionsGenericIComparerSystemString(int handle)
		{
			try
			{
				NativeScript.Bindings.ObjectStore.Remove(handle);
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
		
		[MonoPInvokeCallback(typeof(SystemStringComparerConstructorDelegate))]
		static void SystemStringComparerConstructor(int cppHandle, ref int handle)
		{
			try
			{
				var thiz = new SystemStringComparer(cppHandle);
				handle = NativeScript.Bindings.ObjectStore.Store(thiz);
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
		
		[MonoPInvokeCallback(typeof(ReleaseSystemStringComparerDelegate))]
		static void ReleaseSystemStringComparer(int handle)
		{
			try
			{
				NativeScript.Bindings.ObjectStore.Remove(handle);
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
		
		[MonoPInvokeCallback(typeof(SystemCollectionsICollectionConstructorDelegate))]
		static void SystemCollectionsICollectionConstructor(int cppHandle, ref int handle)
		{
			try
			{
				var thiz = new SystemCollectionsICollection(cppHandle);
				handle = NativeScript.Bindings.ObjectStore.Store(thiz);
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
		
		[MonoPInvokeCallback(typeof(ReleaseSystemCollectionsICollectionDelegate))]
		static void ReleaseSystemCollectionsICollection(int handle)
		{
			try
			{
				NativeScript.Bindings.ObjectStore.Remove(handle);
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
		
		[MonoPInvokeCallback(typeof(SystemCollectionsIListConstructorDelegate))]
		static void SystemCollectionsIListConstructor(int cppHandle, ref int handle)
		{
			try
			{
				var thiz = new SystemCollectionsIList(cppHandle);
				handle = NativeScript.Bindings.ObjectStore.Store(thiz);
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
		
		[MonoPInvokeCallback(typeof(ReleaseSystemCollectionsIListDelegate))]
		static void ReleaseSystemCollectionsIList(int handle)
		{
			try
			{
				NativeScript.Bindings.ObjectStore.Remove(handle);
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
		
		[MonoPInvokeCallback(typeof(SystemCollectionsQueueConstructorDelegate))]
		static void SystemCollectionsQueueConstructor(int cppHandle, ref int handle)
		{
			try
			{
				var thiz = new SystemCollectionsQueue(cppHandle);
				handle = NativeScript.Bindings.ObjectStore.Store(thiz);
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
		
		[MonoPInvokeCallback(typeof(ReleaseSystemCollectionsQueueDelegate))]
		static void ReleaseSystemCollectionsQueue(int handle)
		{
			try
			{
				NativeScript.Bindings.ObjectStore.Remove(handle);
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
		
		[MonoPInvokeCallback(typeof(SystemComponentModelDesignIComponentChangeServiceConstructorDelegate))]
		static void SystemComponentModelDesignIComponentChangeServiceConstructor(int cppHandle, ref int handle)
		{
			try
			{
				var thiz = new SystemComponentModelDesignIComponentChangeService(cppHandle);
				handle = NativeScript.Bindings.ObjectStore.Store(thiz);
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
		
		[MonoPInvokeCallback(typeof(ReleaseSystemComponentModelDesignIComponentChangeServiceDelegate))]
		static void ReleaseSystemComponentModelDesignIComponentChangeService(int handle)
		{
			try
			{
				NativeScript.Bindings.ObjectStore.Remove(handle);
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
		
		[MonoPInvokeCallback(typeof(SystemSystemInt32Array1Constructor1Delegate))]
		static int SystemSystemInt32Array1Constructor1(int length0)
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
		
		[MonoPInvokeCallback(typeof(SystemSystemSingleArray1Constructor1Delegate))]
		static int SystemSystemSingleArray1Constructor1(int length0)
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
		
		[MonoPInvokeCallback(typeof(SystemSystemSingleArray2Constructor2Delegate))]
		static int SystemSystemSingleArray2Constructor2(int length0, int length1)
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
		
		[MonoPInvokeCallback(typeof(SystemSystemSingleArray2GetLength2Delegate))]
		static int SystemSystemSingleArray2GetLength2(int thisHandle, int dimension)
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
		
		[MonoPInvokeCallback(typeof(SystemSystemSingleArray3Constructor3Delegate))]
		static int SystemSystemSingleArray3Constructor3(int length0, int length1, int length2)
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
		
		[MonoPInvokeCallback(typeof(SystemSystemSingleArray3GetLength3Delegate))]
		static int SystemSystemSingleArray3GetLength3(int thisHandle, int dimension)
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
		
		[MonoPInvokeCallback(typeof(SystemSystemStringArray1Constructor1Delegate))]
		static int SystemSystemStringArray1Constructor1(int length0)
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
		
		[MonoPInvokeCallback(typeof(UnityEngineUnityEngineResolutionArray1Constructor1Delegate))]
		static int UnityEngineUnityEngineResolutionArray1Constructor1(int length0)
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
		
		[MonoPInvokeCallback(typeof(UnityEngineUnityEngineRaycastHitArray1Constructor1Delegate))]
		static int UnityEngineUnityEngineRaycastHitArray1Constructor1(int length0)
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
		
		[MonoPInvokeCallback(typeof(UnityEngineUnityEngineGradientColorKeyArray1Constructor1Delegate))]
		static int UnityEngineUnityEngineGradientColorKeyArray1Constructor1(int length0)
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
		
		[MonoPInvokeCallback(typeof(SystemActionInvokeDelegate))]
		static void SystemActionInvoke(int thisHandle)
		{
			try
			{
				((System.Action)NativeScript.Bindings.ObjectStore.Get(thisHandle))();
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
		
		[MonoPInvokeCallback(typeof(SystemActionConstructorDelegate))]
		static void SystemActionConstructor(int cppHandle, ref int handle, ref int classHandle)
		{
			try
			{
				var thiz = new SystemAction(cppHandle);
				handle = NativeScript.Bindings.ObjectStore.Store(thiz);
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
		static void ReleaseSystemAction(int handle, int classHandle)
		{
			try
			{
				if (classHandle != 0)
				{
					var thiz = (SystemAction)NativeScript.Bindings.ObjectStore.Remove(classHandle);
					thiz.CppHandle = 0;
				}
				NativeScript.Bindings.ObjectStore.Remove(handle);
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
				var thiz = (System.Action)NativeScript.Bindings.ObjectStore.Get(thisHandle);
				var del = (System.Action)NativeScript.Bindings.ObjectStore.Get(delHandle);
				thiz += del;
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
				var thiz = (System.Action)NativeScript.Bindings.ObjectStore.Get(thisHandle);
				var del = (System.Action)NativeScript.Bindings.ObjectStore.Get(delHandle);
				thiz -= del;
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
				((System.Action<float>)NativeScript.Bindings.ObjectStore.Get(thisHandle))(obj);
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
		
		[MonoPInvokeCallback(typeof(SystemActionSystemSingleConstructorDelegate))]
		static void SystemActionSystemSingleConstructor(int cppHandle, ref int handle, ref int classHandle)
		{
			try
			{
				var thiz = new SystemActionSystemSingle(cppHandle);
				handle = NativeScript.Bindings.ObjectStore.Store(thiz);
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
		static void ReleaseSystemActionSystemSingle(int handle, int classHandle)
		{
			try
			{
				if (classHandle != 0)
				{
					var thiz = (SystemActionSystemSingle)NativeScript.Bindings.ObjectStore.Remove(classHandle);
					thiz.CppHandle = 0;
				}
				NativeScript.Bindings.ObjectStore.Remove(handle);
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
				var thiz = (System.Action<float>)NativeScript.Bindings.ObjectStore.Get(thisHandle);
				var del = (System.Action<float>)NativeScript.Bindings.ObjectStore.Get(delHandle);
				thiz += del;
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
				var thiz = (System.Action<float>)NativeScript.Bindings.ObjectStore.Get(thisHandle);
				var del = (System.Action<float>)NativeScript.Bindings.ObjectStore.Get(delHandle);
				thiz -= del;
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
				((System.Action<float, float>)NativeScript.Bindings.ObjectStore.Get(thisHandle))(arg1, arg2);
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
		
		[MonoPInvokeCallback(typeof(SystemActionSystemSingle_SystemSingleConstructorDelegate))]
		static void SystemActionSystemSingle_SystemSingleConstructor(int cppHandle, ref int handle, ref int classHandle)
		{
			try
			{
				var thiz = new SystemActionSystemSingle_SystemSingle(cppHandle);
				handle = NativeScript.Bindings.ObjectStore.Store(thiz);
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
		static void ReleaseSystemActionSystemSingle_SystemSingle(int handle, int classHandle)
		{
			try
			{
				if (classHandle != 0)
				{
					var thiz = (SystemActionSystemSingle_SystemSingle)NativeScript.Bindings.ObjectStore.Remove(classHandle);
					thiz.CppHandle = 0;
				}
				NativeScript.Bindings.ObjectStore.Remove(handle);
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
				var thiz = (System.Action<float, float>)NativeScript.Bindings.ObjectStore.Get(thisHandle);
				var del = (System.Action<float, float>)NativeScript.Bindings.ObjectStore.Get(delHandle);
				thiz += del;
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
				var thiz = (System.Action<float, float>)NativeScript.Bindings.ObjectStore.Get(thisHandle);
				var del = (System.Action<float, float>)NativeScript.Bindings.ObjectStore.Get(delHandle);
				thiz -= del;
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
				var returnValue = ((System.Func<int, float, double>)NativeScript.Bindings.ObjectStore.Get(thisHandle))(arg1, arg2);
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
		
		[MonoPInvokeCallback(typeof(SystemFuncSystemInt32_SystemSingle_SystemDoubleConstructorDelegate))]
		static void SystemFuncSystemInt32_SystemSingle_SystemDoubleConstructor(int cppHandle, ref int handle, ref int classHandle)
		{
			try
			{
				var thiz = new SystemFuncSystemInt32_SystemSingle_SystemDouble(cppHandle);
				handle = NativeScript.Bindings.ObjectStore.Store(thiz);
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
		static void ReleaseSystemFuncSystemInt32_SystemSingle_SystemDouble(int handle, int classHandle)
		{
			try
			{
				if (classHandle != 0)
				{
					var thiz = (SystemFuncSystemInt32_SystemSingle_SystemDouble)NativeScript.Bindings.ObjectStore.Remove(classHandle);
					thiz.CppHandle = 0;
				}
				NativeScript.Bindings.ObjectStore.Remove(handle);
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
		
		[MonoPInvokeCallback(typeof(SystemFuncSystemInt32_SystemSingle_SystemDoubleAddDelegate))]
		static void SystemFuncSystemInt32_SystemSingle_SystemDoubleAdd(int thisHandle, int delHandle)
		{
			try
			{
				var thiz = (System.Func<int, float, double>)NativeScript.Bindings.ObjectStore.Get(thisHandle);
				var del = (System.Func<int, float, double>)NativeScript.Bindings.ObjectStore.Get(delHandle);
				thiz += del;
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
				var thiz = (System.Func<int, float, double>)NativeScript.Bindings.ObjectStore.Get(thisHandle);
				var del = (System.Func<int, float, double>)NativeScript.Bindings.ObjectStore.Get(delHandle);
				thiz -= del;
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
				var returnValue = ((System.Func<short, int, string>)NativeScript.Bindings.ObjectStore.Get(thisHandle))(arg1, arg2);
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
		
		[MonoPInvokeCallback(typeof(SystemFuncSystemInt16_SystemInt32_SystemStringConstructorDelegate))]
		static void SystemFuncSystemInt16_SystemInt32_SystemStringConstructor(int cppHandle, ref int handle, ref int classHandle)
		{
			try
			{
				var thiz = new SystemFuncSystemInt16_SystemInt32_SystemString(cppHandle);
				handle = NativeScript.Bindings.ObjectStore.Store(thiz);
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
		static void ReleaseSystemFuncSystemInt16_SystemInt32_SystemString(int handle, int classHandle)
		{
			try
			{
				if (classHandle != 0)
				{
					var thiz = (SystemFuncSystemInt16_SystemInt32_SystemString)NativeScript.Bindings.ObjectStore.Remove(classHandle);
					thiz.CppHandle = 0;
				}
				NativeScript.Bindings.ObjectStore.Remove(handle);
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
		
		[MonoPInvokeCallback(typeof(SystemFuncSystemInt16_SystemInt32_SystemStringAddDelegate))]
		static void SystemFuncSystemInt16_SystemInt32_SystemStringAdd(int thisHandle, int delHandle)
		{
			try
			{
				var thiz = (System.Func<short, int, string>)NativeScript.Bindings.ObjectStore.Get(thisHandle);
				var del = (System.Func<short, int, string>)NativeScript.Bindings.ObjectStore.Get(delHandle);
				thiz += del;
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
				var thiz = (System.Func<short, int, string>)NativeScript.Bindings.ObjectStore.Get(thisHandle);
				var del = (System.Func<short, int, string>)NativeScript.Bindings.ObjectStore.Get(delHandle);
				thiz -= del;
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
		
		[MonoPInvokeCallback(typeof(SystemAppDomainInitializerInvokeDelegate))]
		static void SystemAppDomainInitializerInvoke(int thisHandle, int argsHandle)
		{
			try
			{
				var args = (string[])NativeScript.Bindings.ObjectStore.Get(argsHandle);
				((System.AppDomainInitializer)NativeScript.Bindings.ObjectStore.Get(thisHandle))(args);
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
		
		[MonoPInvokeCallback(typeof(SystemAppDomainInitializerConstructorDelegate))]
		static void SystemAppDomainInitializerConstructor(int cppHandle, ref int handle, ref int classHandle)
		{
			try
			{
				var thiz = new SystemAppDomainInitializer(cppHandle);
				handle = NativeScript.Bindings.ObjectStore.Store(thiz);
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
		
		[MonoPInvokeCallback(typeof(ReleaseSystemAppDomainInitializerDelegate))]
		static void ReleaseSystemAppDomainInitializer(int handle, int classHandle)
		{
			try
			{
				if (classHandle != 0)
				{
					var thiz = (SystemAppDomainInitializer)NativeScript.Bindings.ObjectStore.Remove(classHandle);
					thiz.CppHandle = 0;
				}
				NativeScript.Bindings.ObjectStore.Remove(handle);
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
		
		[MonoPInvokeCallback(typeof(SystemAppDomainInitializerAddDelegate))]
		static void SystemAppDomainInitializerAdd(int thisHandle, int delHandle)
		{
			try
			{
				var thiz = (System.AppDomainInitializer)NativeScript.Bindings.ObjectStore.Get(thisHandle);
				var del = (System.AppDomainInitializer)NativeScript.Bindings.ObjectStore.Get(delHandle);
				thiz += del;
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
		
		[MonoPInvokeCallback(typeof(SystemAppDomainInitializerRemoveDelegate))]
		static void SystemAppDomainInitializerRemove(int thisHandle, int delHandle)
		{
			try
			{
				var thiz = (System.AppDomainInitializer)NativeScript.Bindings.ObjectStore.Get(thisHandle);
				var del = (System.AppDomainInitializer)NativeScript.Bindings.ObjectStore.Get(delHandle);
				thiz -= del;
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
		
		[MonoPInvokeCallback(typeof(UnityEngineEventsUnityActionInvokeDelegate))]
		static void UnityEngineEventsUnityActionInvoke(int thisHandle)
		{
			try
			{
				((UnityEngine.Events.UnityAction)NativeScript.Bindings.ObjectStore.Get(thisHandle))();
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
		
		[MonoPInvokeCallback(typeof(UnityEngineEventsUnityActionConstructorDelegate))]
		static void UnityEngineEventsUnityActionConstructor(int cppHandle, ref int handle, ref int classHandle)
		{
			try
			{
				var thiz = new UnityEngineEventsUnityAction(cppHandle);
				handle = NativeScript.Bindings.ObjectStore.Store(thiz);
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
		
		[MonoPInvokeCallback(typeof(ReleaseUnityEngineEventsUnityActionDelegate))]
		static void ReleaseUnityEngineEventsUnityAction(int handle, int classHandle)
		{
			try
			{
				if (classHandle != 0)
				{
					var thiz = (UnityEngineEventsUnityAction)NativeScript.Bindings.ObjectStore.Remove(classHandle);
					thiz.CppHandle = 0;
				}
				NativeScript.Bindings.ObjectStore.Remove(handle);
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
		
		[MonoPInvokeCallback(typeof(UnityEngineEventsUnityActionAddDelegate))]
		static void UnityEngineEventsUnityActionAdd(int thisHandle, int delHandle)
		{
			try
			{
				var thiz = (UnityEngine.Events.UnityAction)NativeScript.Bindings.ObjectStore.Get(thisHandle);
				var del = (UnityEngine.Events.UnityAction)NativeScript.Bindings.ObjectStore.Get(delHandle);
				thiz += del;
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
		
		[MonoPInvokeCallback(typeof(UnityEngineEventsUnityActionRemoveDelegate))]
		static void UnityEngineEventsUnityActionRemove(int thisHandle, int delHandle)
		{
			try
			{
				var thiz = (UnityEngine.Events.UnityAction)NativeScript.Bindings.ObjectStore.Get(thisHandle);
				var del = (UnityEngine.Events.UnityAction)NativeScript.Bindings.ObjectStore.Get(delHandle);
				thiz -= del;
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
		
		[MonoPInvokeCallback(typeof(UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeInvokeDelegate))]
		static void UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeInvoke(int thisHandle, ref UnityEngine.SceneManagement.Scene arg0, UnityEngine.SceneManagement.LoadSceneMode arg1)
		{
			try
			{
				((UnityEngine.Events.UnityAction<UnityEngine.SceneManagement.Scene, UnityEngine.SceneManagement.LoadSceneMode>)NativeScript.Bindings.ObjectStore.Get(thisHandle))(arg0, arg1);
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
		
		[MonoPInvokeCallback(typeof(UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeConstructorDelegate))]
		static void UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeConstructor(int cppHandle, ref int handle, ref int classHandle)
		{
			try
			{
				var thiz = new UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneMode(cppHandle);
				handle = NativeScript.Bindings.ObjectStore.Store(thiz);
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
		
		[MonoPInvokeCallback(typeof(ReleaseUnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeDelegate))]
		static void ReleaseUnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneMode(int handle, int classHandle)
		{
			try
			{
				if (classHandle != 0)
				{
					var thiz = (UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneMode)NativeScript.Bindings.ObjectStore.Remove(classHandle);
					thiz.CppHandle = 0;
				}
				NativeScript.Bindings.ObjectStore.Remove(handle);
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
		
		[MonoPInvokeCallback(typeof(UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeAddDelegate))]
		static void UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeAdd(int thisHandle, int delHandle)
		{
			try
			{
				var thiz = (UnityEngine.Events.UnityAction<UnityEngine.SceneManagement.Scene, UnityEngine.SceneManagement.LoadSceneMode>)NativeScript.Bindings.ObjectStore.Get(thisHandle);
				var del = (UnityEngine.Events.UnityAction<UnityEngine.SceneManagement.Scene, UnityEngine.SceneManagement.LoadSceneMode>)NativeScript.Bindings.ObjectStore.Get(delHandle);
				thiz += del;
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
		
		[MonoPInvokeCallback(typeof(UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeRemoveDelegate))]
		static void UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeRemove(int thisHandle, int delHandle)
		{
			try
			{
				var thiz = (UnityEngine.Events.UnityAction<UnityEngine.SceneManagement.Scene, UnityEngine.SceneManagement.LoadSceneMode>)NativeScript.Bindings.ObjectStore.Get(thisHandle);
				var del = (UnityEngine.Events.UnityAction<UnityEngine.SceneManagement.Scene, UnityEngine.SceneManagement.LoadSceneMode>)NativeScript.Bindings.ObjectStore.Get(delHandle);
				thiz -= del;
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
		
		[MonoPInvokeCallback(typeof(SystemComponentModelDesignComponentEventHandlerInvokeDelegate))]
		static void SystemComponentModelDesignComponentEventHandlerInvoke(int thisHandle, int senderHandle, int eHandle)
		{
			try
			{
				var sender = NativeScript.Bindings.ObjectStore.Get(senderHandle);
				var e = (System.ComponentModel.Design.ComponentEventArgs)NativeScript.Bindings.ObjectStore.Get(eHandle);
				((System.ComponentModel.Design.ComponentEventHandler)NativeScript.Bindings.ObjectStore.Get(thisHandle))(sender, e);
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
		
		[MonoPInvokeCallback(typeof(SystemComponentModelDesignComponentEventHandlerConstructorDelegate))]
		static void SystemComponentModelDesignComponentEventHandlerConstructor(int cppHandle, ref int handle, ref int classHandle)
		{
			try
			{
				var thiz = new SystemComponentModelDesignComponentEventHandler(cppHandle);
				handle = NativeScript.Bindings.ObjectStore.Store(thiz);
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
		
		[MonoPInvokeCallback(typeof(ReleaseSystemComponentModelDesignComponentEventHandlerDelegate))]
		static void ReleaseSystemComponentModelDesignComponentEventHandler(int handle, int classHandle)
		{
			try
			{
				if (classHandle != 0)
				{
					var thiz = (SystemComponentModelDesignComponentEventHandler)NativeScript.Bindings.ObjectStore.Remove(classHandle);
					thiz.CppHandle = 0;
				}
				NativeScript.Bindings.ObjectStore.Remove(handle);
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
		
		[MonoPInvokeCallback(typeof(SystemComponentModelDesignComponentEventHandlerAddDelegate))]
		static void SystemComponentModelDesignComponentEventHandlerAdd(int thisHandle, int delHandle)
		{
			try
			{
				var thiz = (System.ComponentModel.Design.ComponentEventHandler)NativeScript.Bindings.ObjectStore.Get(thisHandle);
				var del = (System.ComponentModel.Design.ComponentEventHandler)NativeScript.Bindings.ObjectStore.Get(delHandle);
				thiz += del;
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
		
		[MonoPInvokeCallback(typeof(SystemComponentModelDesignComponentEventHandlerRemoveDelegate))]
		static void SystemComponentModelDesignComponentEventHandlerRemove(int thisHandle, int delHandle)
		{
			try
			{
				var thiz = (System.ComponentModel.Design.ComponentEventHandler)NativeScript.Bindings.ObjectStore.Get(thisHandle);
				var del = (System.ComponentModel.Design.ComponentEventHandler)NativeScript.Bindings.ObjectStore.Get(delHandle);
				thiz -= del;
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
		
		[MonoPInvokeCallback(typeof(SystemComponentModelDesignComponentChangingEventHandlerInvokeDelegate))]
		static void SystemComponentModelDesignComponentChangingEventHandlerInvoke(int thisHandle, int senderHandle, int eHandle)
		{
			try
			{
				var sender = NativeScript.Bindings.ObjectStore.Get(senderHandle);
				var e = (System.ComponentModel.Design.ComponentChangingEventArgs)NativeScript.Bindings.ObjectStore.Get(eHandle);
				((System.ComponentModel.Design.ComponentChangingEventHandler)NativeScript.Bindings.ObjectStore.Get(thisHandle))(sender, e);
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
		
		[MonoPInvokeCallback(typeof(SystemComponentModelDesignComponentChangingEventHandlerConstructorDelegate))]
		static void SystemComponentModelDesignComponentChangingEventHandlerConstructor(int cppHandle, ref int handle, ref int classHandle)
		{
			try
			{
				var thiz = new SystemComponentModelDesignComponentChangingEventHandler(cppHandle);
				handle = NativeScript.Bindings.ObjectStore.Store(thiz);
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
		
		[MonoPInvokeCallback(typeof(ReleaseSystemComponentModelDesignComponentChangingEventHandlerDelegate))]
		static void ReleaseSystemComponentModelDesignComponentChangingEventHandler(int handle, int classHandle)
		{
			try
			{
				if (classHandle != 0)
				{
					var thiz = (SystemComponentModelDesignComponentChangingEventHandler)NativeScript.Bindings.ObjectStore.Remove(classHandle);
					thiz.CppHandle = 0;
				}
				NativeScript.Bindings.ObjectStore.Remove(handle);
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
		
		[MonoPInvokeCallback(typeof(SystemComponentModelDesignComponentChangingEventHandlerAddDelegate))]
		static void SystemComponentModelDesignComponentChangingEventHandlerAdd(int thisHandle, int delHandle)
		{
			try
			{
				var thiz = (System.ComponentModel.Design.ComponentChangingEventHandler)NativeScript.Bindings.ObjectStore.Get(thisHandle);
				var del = (System.ComponentModel.Design.ComponentChangingEventHandler)NativeScript.Bindings.ObjectStore.Get(delHandle);
				thiz += del;
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
		
		[MonoPInvokeCallback(typeof(SystemComponentModelDesignComponentChangingEventHandlerRemoveDelegate))]
		static void SystemComponentModelDesignComponentChangingEventHandlerRemove(int thisHandle, int delHandle)
		{
			try
			{
				var thiz = (System.ComponentModel.Design.ComponentChangingEventHandler)NativeScript.Bindings.ObjectStore.Get(thisHandle);
				var del = (System.ComponentModel.Design.ComponentChangingEventHandler)NativeScript.Bindings.ObjectStore.Get(delHandle);
				thiz -= del;
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
		
		[MonoPInvokeCallback(typeof(SystemComponentModelDesignComponentChangedEventHandlerInvokeDelegate))]
		static void SystemComponentModelDesignComponentChangedEventHandlerInvoke(int thisHandle, int senderHandle, int eHandle)
		{
			try
			{
				var sender = NativeScript.Bindings.ObjectStore.Get(senderHandle);
				var e = (System.ComponentModel.Design.ComponentChangedEventArgs)NativeScript.Bindings.ObjectStore.Get(eHandle);
				((System.ComponentModel.Design.ComponentChangedEventHandler)NativeScript.Bindings.ObjectStore.Get(thisHandle))(sender, e);
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
		
		[MonoPInvokeCallback(typeof(SystemComponentModelDesignComponentChangedEventHandlerConstructorDelegate))]
		static void SystemComponentModelDesignComponentChangedEventHandlerConstructor(int cppHandle, ref int handle, ref int classHandle)
		{
			try
			{
				var thiz = new SystemComponentModelDesignComponentChangedEventHandler(cppHandle);
				handle = NativeScript.Bindings.ObjectStore.Store(thiz);
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
		
		[MonoPInvokeCallback(typeof(ReleaseSystemComponentModelDesignComponentChangedEventHandlerDelegate))]
		static void ReleaseSystemComponentModelDesignComponentChangedEventHandler(int handle, int classHandle)
		{
			try
			{
				if (classHandle != 0)
				{
					var thiz = (SystemComponentModelDesignComponentChangedEventHandler)NativeScript.Bindings.ObjectStore.Remove(classHandle);
					thiz.CppHandle = 0;
				}
				NativeScript.Bindings.ObjectStore.Remove(handle);
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
		
		[MonoPInvokeCallback(typeof(SystemComponentModelDesignComponentChangedEventHandlerAddDelegate))]
		static void SystemComponentModelDesignComponentChangedEventHandlerAdd(int thisHandle, int delHandle)
		{
			try
			{
				var thiz = (System.ComponentModel.Design.ComponentChangedEventHandler)NativeScript.Bindings.ObjectStore.Get(thisHandle);
				var del = (System.ComponentModel.Design.ComponentChangedEventHandler)NativeScript.Bindings.ObjectStore.Get(delHandle);
				thiz += del;
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
		
		[MonoPInvokeCallback(typeof(SystemComponentModelDesignComponentChangedEventHandlerRemoveDelegate))]
		static void SystemComponentModelDesignComponentChangedEventHandlerRemove(int thisHandle, int delHandle)
		{
			try
			{
				var thiz = (System.ComponentModel.Design.ComponentChangedEventHandler)NativeScript.Bindings.ObjectStore.Get(thisHandle);
				var del = (System.ComponentModel.Design.ComponentChangedEventHandler)NativeScript.Bindings.ObjectStore.Get(delHandle);
				thiz -= del;
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
		
		[MonoPInvokeCallback(typeof(SystemComponentModelDesignComponentRenameEventHandlerInvokeDelegate))]
		static void SystemComponentModelDesignComponentRenameEventHandlerInvoke(int thisHandle, int senderHandle, int eHandle)
		{
			try
			{
				var sender = NativeScript.Bindings.ObjectStore.Get(senderHandle);
				var e = (System.ComponentModel.Design.ComponentRenameEventArgs)NativeScript.Bindings.ObjectStore.Get(eHandle);
				((System.ComponentModel.Design.ComponentRenameEventHandler)NativeScript.Bindings.ObjectStore.Get(thisHandle))(sender, e);
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
		
		[MonoPInvokeCallback(typeof(SystemComponentModelDesignComponentRenameEventHandlerConstructorDelegate))]
		static void SystemComponentModelDesignComponentRenameEventHandlerConstructor(int cppHandle, ref int handle, ref int classHandle)
		{
			try
			{
				var thiz = new SystemComponentModelDesignComponentRenameEventHandler(cppHandle);
				handle = NativeScript.Bindings.ObjectStore.Store(thiz);
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
		
		[MonoPInvokeCallback(typeof(ReleaseSystemComponentModelDesignComponentRenameEventHandlerDelegate))]
		static void ReleaseSystemComponentModelDesignComponentRenameEventHandler(int handle, int classHandle)
		{
			try
			{
				if (classHandle != 0)
				{
					var thiz = (SystemComponentModelDesignComponentRenameEventHandler)NativeScript.Bindings.ObjectStore.Remove(classHandle);
					thiz.CppHandle = 0;
				}
				NativeScript.Bindings.ObjectStore.Remove(handle);
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
		
		[MonoPInvokeCallback(typeof(SystemComponentModelDesignComponentRenameEventHandlerAddDelegate))]
		static void SystemComponentModelDesignComponentRenameEventHandlerAdd(int thisHandle, int delHandle)
		{
			try
			{
				var thiz = (System.ComponentModel.Design.ComponentRenameEventHandler)NativeScript.Bindings.ObjectStore.Get(thisHandle);
				var del = (System.ComponentModel.Design.ComponentRenameEventHandler)NativeScript.Bindings.ObjectStore.Get(delHandle);
				thiz += del;
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
		
		[MonoPInvokeCallback(typeof(SystemComponentModelDesignComponentRenameEventHandlerRemoveDelegate))]
		static void SystemComponentModelDesignComponentRenameEventHandlerRemove(int thisHandle, int delHandle)
		{
			try
			{
				var thiz = (System.ComponentModel.Design.ComponentRenameEventHandler)NativeScript.Bindings.ObjectStore.Get(thisHandle);
				var del = (System.ComponentModel.Design.ComponentRenameEventHandler)NativeScript.Bindings.ObjectStore.Get(delHandle);
				thiz -= del;
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
				int param0Handle = NativeScript.Bindings.ObjectStore.GetHandle(param0);
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
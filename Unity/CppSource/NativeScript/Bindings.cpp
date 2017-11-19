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

// Type definitions
#include "Bindings.h"

// For assert()
#include <assert.h>

// For int32_t, etc.
#include <stdint.h>

// For malloc(), etc.
#include <stdlib.h>

// Macro to put before functions that need to be exposed to C#
#ifdef _WIN32
	#define DLLEXPORT extern "C" __declspec(dllexport)
#else
	#define DLLEXPORT extern "C"
#endif

////////////////////////////////////////////////////////////////
// C# functions for C++ to call
////////////////////////////////////////////////////////////////

namespace Plugin
{
	void (*ReleaseObject)(int32_t handle);
	int32_t (*StringNew)(const char* chars);
	void (*SetException)(int32_t handle);
	int32_t (*ArrayGetLength)(int32_t handle);
	int32_t (*ArrayGetRank)(int32_t handle);
	
	/*BEGIN FUNCTION POINTERS*/
	int32_t (*SystemDiagnosticsStopwatchConstructor)();
	int64_t (*SystemDiagnosticsStopwatchPropertyGetElapsedMilliseconds)(int32_t thisHandle);
	void (*SystemDiagnosticsStopwatchMethodStart)(int32_t thisHandle);
	void (*SystemDiagnosticsStopwatchMethodReset)(int32_t thisHandle);
	int32_t (*UnityEngineObjectPropertyGetName)(int32_t thisHandle);
	void (*UnityEngineObjectPropertySetName)(int32_t thisHandle, int32_t valueHandle);
	System::Boolean (*UnityEngineObjectMethodop_EqualityUnityEngineObject_UnityEngineObject)(int32_t xHandle, int32_t yHandle);
	System::Boolean (*UnityEngineObjectMethodop_ImplicitUnityEngineObject)(int32_t existsHandle);
	int32_t (*UnityEngineGameObjectConstructor)();
	int32_t (*UnityEngineGameObjectConstructorSystemString)(int32_t nameHandle);
	int32_t (*UnityEngineGameObjectPropertyGetTransform)(int32_t thisHandle);
	int32_t (*UnityEngineGameObjectMethodAddComponentMyGameMonoBehavioursTestScript)(int32_t thisHandle);
	int32_t (*UnityEngineComponentPropertyGetTransform)(int32_t thisHandle);
	UnityEngine::Vector3 (*UnityEngineTransformPropertyGetPosition)(int32_t thisHandle);
	void (*UnityEngineTransformPropertySetPosition)(int32_t thisHandle, UnityEngine::Vector3& value);
	void (*UnityEngineDebugMethodLogSystemObject)(int32_t messageHandle);
	System::Boolean (*UnityEngineAssertionsAssertFieldGetRaiseExceptions)();
	void (*UnityEngineAssertionsAssertFieldSetRaiseExceptions)(System::Boolean value);
	void (*UnityEngineAssertionsAssertMethodAreEqualSystemStringSystemString_SystemString)(int32_t expectedHandle, int32_t actualHandle);
	void (*UnityEngineAssertionsAssertMethodAreEqualUnityEngineGameObjectUnityEngineGameObject_UnityEngineGameObject)(int32_t expectedHandle, int32_t actualHandle);
	void (*UnityEngineAudioSettingsMethodGetDSPBufferSizeSystemInt32_SystemInt32)(int32_t* bufferLength, int32_t* numBuffers);
	void (*UnityEngineNetworkingNetworkTransportMethodGetBroadcastConnectionInfoSystemInt32_SystemString_SystemInt32_SystemByte)(int32_t hostId, int32_t* addressHandle, int32_t* port, uint8_t* error);
	void (*UnityEngineNetworkingNetworkTransportMethodInit)();
	UnityEngine::Vector3 (*UnityEngineVector3ConstructorSystemSingle_SystemSingle_SystemSingle)(float x, float y, float z);
	float (*UnityEngineVector3PropertyGetMagnitude)(UnityEngine::Vector3* thiz);
	void (*UnityEngineVector3MethodSetSystemSingle_SystemSingle_SystemSingle)(UnityEngine::Vector3* thiz, float newX, float newY, float newZ);
	UnityEngine::Vector3 (*UnityEngineVector3Methodop_AdditionUnityEngineVector3_UnityEngineVector3)(UnityEngine::Vector3& a, UnityEngine::Vector3& b);
	UnityEngine::Vector3 (*UnityEngineVector3Methodop_UnaryNegationUnityEngineVector3)(UnityEngine::Vector3& a);
	int32_t (*BoxVector3)(UnityEngine::Vector3& val);
	UnityEngine::Vector3 (*UnboxVector3)(int32_t valHandle);
	float (*UnityEngineMatrix4x4PropertyGetItem)(UnityEngine::Matrix4x4* thiz, int32_t row, int32_t column);
	void (*UnityEngineMatrix4x4PropertySetItem)(UnityEngine::Matrix4x4* thiz, int32_t row, int32_t column, float value);
	int32_t (*BoxMatrix4x4)(UnityEngine::Matrix4x4& val);
	UnityEngine::Matrix4x4 (*UnboxMatrix4x4)(int32_t valHandle);
	void (*ReleaseUnityEngineRaycastHit)(int32_t handle);
	UnityEngine::Vector3 (*UnityEngineRaycastHitPropertyGetPoint)(int32_t thisHandle);
	void (*UnityEngineRaycastHitPropertySetPoint)(int32_t thisHandle, UnityEngine::Vector3& value);
	int32_t (*UnityEngineRaycastHitPropertyGetTransform)(int32_t thisHandle);
	int32_t (*BoxRaycastHit)(int32_t valHandle);
	int32_t (*UnboxRaycastHit)(int32_t valHandle);
	int32_t (*BoxQueryTriggerInteraction)(UnityEngine::QueryTriggerInteraction val);
	UnityEngine::QueryTriggerInteraction (*UnboxQueryTriggerInteraction)(int32_t valHandle);
	void (*ReleaseSystemCollectionsGenericKeyValuePairSystemString_SystemDouble)(int32_t handle);
	int32_t (*SystemCollectionsGenericKeyValuePairSystemString_SystemDoubleConstructorSystemString_SystemDouble)(int32_t keyHandle, double value);
	int32_t (*SystemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetKey)(int32_t thisHandle);
	double (*SystemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetValue)(int32_t thisHandle);
	int32_t (*BoxKeyValuePairSystemString_SystemDouble)(int32_t valHandle);
	int32_t (*UnboxKeyValuePairSystemString_SystemDouble)(int32_t valHandle);
	int32_t (*SystemCollectionsGenericListSystemStringConstructor)();
	int32_t (*SystemCollectionsGenericListSystemStringPropertyGetItem)(int32_t thisHandle, int32_t index);
	void (*SystemCollectionsGenericListSystemStringPropertySetItem)(int32_t thisHandle, int32_t index, int32_t valueHandle);
	void (*SystemCollectionsGenericListSystemStringMethodAddSystemString)(int32_t thisHandle, int32_t itemHandle);
	void (*SystemCollectionsGenericListSystemStringMethodSortSystemCollectionsGenericIComparer)(int32_t thisHandle, int32_t comparerHandle);
	int32_t (*SystemCollectionsGenericListSystemInt32Constructor)();
	int32_t (*SystemCollectionsGenericListSystemInt32PropertyGetItem)(int32_t thisHandle, int32_t index);
	void (*SystemCollectionsGenericListSystemInt32PropertySetItem)(int32_t thisHandle, int32_t index, int32_t value);
	void (*SystemCollectionsGenericListSystemInt32MethodAddSystemInt32)(int32_t thisHandle, int32_t item);
	void (*SystemCollectionsGenericListSystemInt32MethodSortSystemCollectionsGenericIComparer)(int32_t thisHandle, int32_t comparerHandle);
	int32_t (*SystemCollectionsGenericLinkedListNodeSystemStringConstructorSystemString)(int32_t valueHandle);
	int32_t (*SystemCollectionsGenericLinkedListNodeSystemStringPropertyGetValue)(int32_t thisHandle);
	void (*SystemCollectionsGenericLinkedListNodeSystemStringPropertySetValue)(int32_t thisHandle, int32_t valueHandle);
	int32_t (*SystemRuntimeCompilerServicesStrongBoxSystemStringConstructorSystemString)(int32_t valueHandle);
	int32_t (*SystemRuntimeCompilerServicesStrongBoxSystemStringFieldGetValue)(int32_t thisHandle);
	void (*SystemRuntimeCompilerServicesStrongBoxSystemStringFieldSetValue)(int32_t thisHandle, int32_t valueHandle);
	int32_t (*SystemExceptionConstructorSystemString)(int32_t messageHandle);
	int32_t (*UnityEngineResolutionPropertyGetWidth)(UnityEngine::Resolution* thiz);
	void (*UnityEngineResolutionPropertySetWidth)(UnityEngine::Resolution* thiz, int32_t value);
	int32_t (*UnityEngineResolutionPropertyGetHeight)(UnityEngine::Resolution* thiz);
	void (*UnityEngineResolutionPropertySetHeight)(UnityEngine::Resolution* thiz, int32_t value);
	int32_t (*UnityEngineResolutionPropertyGetRefreshRate)(UnityEngine::Resolution* thiz);
	void (*UnityEngineResolutionPropertySetRefreshRate)(UnityEngine::Resolution* thiz, int32_t value);
	int32_t (*BoxResolution)(UnityEngine::Resolution& val);
	UnityEngine::Resolution (*UnboxResolution)(int32_t valHandle);
	int32_t (*UnityEngineScreenPropertyGetResolutions)();
	UnityEngine::Ray (*UnityEngineRayConstructorUnityEngineVector3_UnityEngineVector3)(UnityEngine::Vector3& origin, UnityEngine::Vector3& direction);
	int32_t (*BoxRay)(UnityEngine::Ray& val);
	UnityEngine::Ray (*UnboxRay)(int32_t valHandle);
	int32_t (*UnityEnginePhysicsMethodRaycastNonAllocUnityEngineRay_UnityEngineRaycastHit)(UnityEngine::Ray& ray, int32_t resultsHandle);
	int32_t (*UnityEnginePhysicsMethodRaycastAllUnityEngineRay)(UnityEngine::Ray& ray);
	int32_t (*BoxColor)(UnityEngine::Color& val);
	UnityEngine::Color (*UnboxColor)(int32_t valHandle);
	int32_t (*BoxGradientColorKey)(UnityEngine::GradientColorKey& val);
	UnityEngine::GradientColorKey (*UnboxGradientColorKey)(int32_t valHandle);
	int32_t (*UnityEngineGradientConstructor)();
	int32_t (*UnityEngineGradientPropertyGetColorKeys)(int32_t thisHandle);
	void (*UnityEngineGradientPropertySetColorKeys)(int32_t thisHandle, int32_t valueHandle);
	int32_t (*SystemAppDomainSetupConstructor)();
	int32_t (*SystemAppDomainSetupPropertyGetAppDomainInitializer)(int32_t thisHandle);
	void (*SystemAppDomainSetupPropertySetAppDomainInitializer)(int32_t thisHandle, int32_t valueHandle);
	void (*UnityEngineApplicationAddEventOnBeforeRender)(int32_t delHandle);
	void (*UnityEngineApplicationRemoveEventOnBeforeRender)(int32_t delHandle);
	void (*UnityEngineSceneManagementSceneManagerAddEventSceneLoaded)(int32_t delHandle);
	void (*UnityEngineSceneManagementSceneManagerRemoveEventSceneLoaded)(int32_t delHandle);
	int32_t (*BoxScene)(UnityEngine::SceneManagement::Scene& val);
	UnityEngine::SceneManagement::Scene (*UnboxScene)(int32_t valHandle);
	int32_t (*BoxLoadSceneMode)(UnityEngine::SceneManagement::LoadSceneMode val);
	UnityEngine::SceneManagement::LoadSceneMode (*UnboxLoadSceneMode)(int32_t valHandle);
	void (*ReleaseSystemCollectionsGenericIComparerSystemInt32)(int32_t handle);
	void (*SystemCollectionsGenericIComparerSystemInt32Constructor)(int32_t cppHandle, int32_t* handle);
	void (*ReleaseSystemCollectionsGenericIComparerSystemString)(int32_t handle);
	void (*SystemCollectionsGenericIComparerSystemStringConstructor)(int32_t cppHandle, int32_t* handle);
	void (*ReleaseSystemStringComparer)(int32_t handle);
	void (*SystemStringComparerConstructor)(int32_t cppHandle, int32_t* handle);
	void (*ReleaseSystemEventArgs)(int32_t handle);
	void (*SystemEventArgsConstructor)(int32_t cppHandle, int32_t* handle);
	int32_t (*BoxBoolean)(System::Boolean val);
	System::Boolean (*UnboxBoolean)(int32_t valHandle);
	int32_t (*BoxSByte)(int8_t val);
	int8_t (*UnboxSByte)(int32_t valHandle);
	int32_t (*BoxByte)(uint8_t val);
	uint8_t (*UnboxByte)(int32_t valHandle);
	int32_t (*BoxInt16)(int16_t val);
	int16_t (*UnboxInt16)(int32_t valHandle);
	int32_t (*BoxUInt16)(uint16_t val);
	uint16_t (*UnboxUInt16)(int32_t valHandle);
	int32_t (*BoxInt32)(int32_t val);
	int32_t (*UnboxInt32)(int32_t valHandle);
	int32_t (*BoxUInt32)(uint32_t val);
	uint32_t (*UnboxUInt32)(int32_t valHandle);
	int32_t (*BoxInt64)(int64_t val);
	int64_t (*UnboxInt64)(int32_t valHandle);
	int32_t (*BoxUInt64)(uint64_t val);
	uint64_t (*UnboxUInt64)(int32_t valHandle);
	int32_t (*BoxChar)(System::Char val);
	System::Char (*UnboxChar)(int32_t valHandle);
	int32_t (*BoxSingle)(float val);
	float (*UnboxSingle)(int32_t valHandle);
	int32_t (*BoxDouble)(double val);
	double (*UnboxDouble)(int32_t valHandle);
	int32_t (*SystemSystemInt32Array1Constructor1)(int32_t length0);
	int32_t (*SystemInt32Array1GetItem1)(int32_t thisHandle, int32_t index0);
	int32_t (*SystemInt32Array1SetItem1)(int32_t thisHandle, int32_t index0, int32_t item);
	int32_t (*SystemSystemSingleArray1Constructor1)(int32_t length0);
	float (*SystemSingleArray1GetItem1)(int32_t thisHandle, int32_t index0);
	int32_t (*SystemSingleArray1SetItem1)(int32_t thisHandle, int32_t index0, float item);
	int32_t (*SystemSystemSingleArray2Constructor2)(int32_t length0, int32_t length1);
	int32_t (*SystemSystemSingleArray2GetLength2)(int32_t thisHandle, int32_t dimension);
	float (*SystemSingleArray2GetItem2)(int32_t thisHandle, int32_t index0, int32_t index1);
	int32_t (*SystemSingleArray2SetItem2)(int32_t thisHandle, int32_t index0, int32_t index1, float item);
	int32_t (*SystemSystemSingleArray3Constructor3)(int32_t length0, int32_t length1, int32_t length2);
	int32_t (*SystemSystemSingleArray3GetLength3)(int32_t thisHandle, int32_t dimension);
	float (*SystemSingleArray3GetItem3)(int32_t thisHandle, int32_t index0, int32_t index1, int32_t index2);
	int32_t (*SystemSingleArray3SetItem3)(int32_t thisHandle, int32_t index0, int32_t index1, int32_t index2, float item);
	int32_t (*SystemSystemStringArray1Constructor1)(int32_t length0);
	int32_t (*SystemStringArray1GetItem1)(int32_t thisHandle, int32_t index0);
	int32_t (*SystemStringArray1SetItem1)(int32_t thisHandle, int32_t index0, int32_t itemHandle);
	int32_t (*UnityEngineUnityEngineResolutionArray1Constructor1)(int32_t length0);
	UnityEngine::Resolution (*UnityEngineResolutionArray1GetItem1)(int32_t thisHandle, int32_t index0);
	int32_t (*UnityEngineResolutionArray1SetItem1)(int32_t thisHandle, int32_t index0, UnityEngine::Resolution& item);
	int32_t (*UnityEngineUnityEngineRaycastHitArray1Constructor1)(int32_t length0);
	int32_t (*UnityEngineRaycastHitArray1GetItem1)(int32_t thisHandle, int32_t index0);
	int32_t (*UnityEngineRaycastHitArray1SetItem1)(int32_t thisHandle, int32_t index0, int32_t itemHandle);
	int32_t (*UnityEngineUnityEngineGradientColorKeyArray1Constructor1)(int32_t length0);
	UnityEngine::GradientColorKey (*UnityEngineGradientColorKeyArray1GetItem1)(int32_t thisHandle, int32_t index0);
	int32_t (*UnityEngineGradientColorKeyArray1SetItem1)(int32_t thisHandle, int32_t index0, UnityEngine::GradientColorKey& item);
	void (*ReleaseSystemAction)(int32_t handle, int32_t classHandle);
	void (*SystemActionConstructor)(int32_t cppHandle, int32_t* handle, int32_t* classHandle);
	void (*SystemActionAdd)(int32_t thisHandle, int32_t delHandle);
	void (*SystemActionRemove)(int32_t thisHandle, int32_t delHandle);
	void (*SystemActionInvoke)(int32_t thisHandle);
	void (*ReleaseSystemActionSystemSingle)(int32_t handle, int32_t classHandle);
	void (*SystemActionSystemSingleConstructor)(int32_t cppHandle, int32_t* handle, int32_t* classHandle);
	void (*SystemActionSystemSingleAdd)(int32_t thisHandle, int32_t delHandle);
	void (*SystemActionSystemSingleRemove)(int32_t thisHandle, int32_t delHandle);
	void (*SystemActionSystemSingleInvoke)(int32_t thisHandle, float obj);
	void (*ReleaseSystemActionSystemSingle_SystemSingle)(int32_t handle, int32_t classHandle);
	void (*SystemActionSystemSingle_SystemSingleConstructor)(int32_t cppHandle, int32_t* handle, int32_t* classHandle);
	void (*SystemActionSystemSingle_SystemSingleAdd)(int32_t thisHandle, int32_t delHandle);
	void (*SystemActionSystemSingle_SystemSingleRemove)(int32_t thisHandle, int32_t delHandle);
	void (*SystemActionSystemSingle_SystemSingleInvoke)(int32_t thisHandle, float arg1, float arg2);
	void (*ReleaseSystemFuncSystemInt32_SystemSingle_SystemDouble)(int32_t handle, int32_t classHandle);
	void (*SystemFuncSystemInt32_SystemSingle_SystemDoubleConstructor)(int32_t cppHandle, int32_t* handle, int32_t* classHandle);
	void (*SystemFuncSystemInt32_SystemSingle_SystemDoubleAdd)(int32_t thisHandle, int32_t delHandle);
	void (*SystemFuncSystemInt32_SystemSingle_SystemDoubleRemove)(int32_t thisHandle, int32_t delHandle);
	double (*SystemFuncSystemInt32_SystemSingle_SystemDoubleInvoke)(int32_t thisHandle, int32_t arg1, float arg2);
	void (*ReleaseSystemFuncSystemInt16_SystemInt32_SystemString)(int32_t handle, int32_t classHandle);
	void (*SystemFuncSystemInt16_SystemInt32_SystemStringConstructor)(int32_t cppHandle, int32_t* handle, int32_t* classHandle);
	void (*SystemFuncSystemInt16_SystemInt32_SystemStringAdd)(int32_t thisHandle, int32_t delHandle);
	void (*SystemFuncSystemInt16_SystemInt32_SystemStringRemove)(int32_t thisHandle, int32_t delHandle);
	int32_t (*SystemFuncSystemInt16_SystemInt32_SystemStringInvoke)(int32_t thisHandle, int16_t arg1, int32_t arg2);
	void (*ReleaseSystemAppDomainInitializer)(int32_t handle, int32_t classHandle);
	void (*SystemAppDomainInitializerConstructor)(int32_t cppHandle, int32_t* handle, int32_t* classHandle);
	void (*SystemAppDomainInitializerAdd)(int32_t thisHandle, int32_t delHandle);
	void (*SystemAppDomainInitializerRemove)(int32_t thisHandle, int32_t delHandle);
	void (*SystemAppDomainInitializerInvoke)(int32_t thisHandle, int32_t argsHandle);
	void (*ReleaseUnityEngineEventsUnityAction)(int32_t handle, int32_t classHandle);
	void (*UnityEngineEventsUnityActionConstructor)(int32_t cppHandle, int32_t* handle, int32_t* classHandle);
	void (*UnityEngineEventsUnityActionAdd)(int32_t thisHandle, int32_t delHandle);
	void (*UnityEngineEventsUnityActionRemove)(int32_t thisHandle, int32_t delHandle);
	void (*UnityEngineEventsUnityActionInvoke)(int32_t thisHandle);
	void (*ReleaseUnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneMode)(int32_t handle, int32_t classHandle);
	void (*UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeConstructor)(int32_t cppHandle, int32_t* handle, int32_t* classHandle);
	void (*UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeAdd)(int32_t thisHandle, int32_t delHandle);
	void (*UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeRemove)(int32_t thisHandle, int32_t delHandle);
	void (*UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeInvoke)(int32_t thisHandle, UnityEngine::SceneManagement::Scene& arg0, UnityEngine::SceneManagement::LoadSceneMode arg1);
	/*END FUNCTION POINTERS*/
}

////////////////////////////////////////////////////////////////
// Reference counting of managed objects
////////////////////////////////////////////////////////////////

namespace Plugin
{
	int32_t RefCountsLenClass;
	int32_t* RefCountsClass;

	void ReferenceManagedClass(int32_t handle)
	{
		assert(handle >= 0 && handle < RefCountsLenClass);
		if (handle != 0)
		{
			RefCountsClass[handle]++;
		}
	}

	void DereferenceManagedClass(int32_t handle)
	{
		assert(handle >= 0 && handle < RefCountsLenClass);
		if (handle != 0)
		{
			int32_t numRemain = --RefCountsClass[handle];
			if (numRemain == 0)
			{
				ReleaseObject(handle);
			}
		}
	}
	
	bool DereferenceManagedClassNoRelease(int32_t handle)
	{
		assert(handle >= 0 && handle < RefCountsLenClass);
		if (handle != 0)
		{
			int32_t numRemain = --RefCountsClass[handle];
			if (numRemain == 0)
			{
				return true;
			}
		}
		return false;
	}
	
	/*BEGIN GLOBAL STATE AND FUNCTIONS*/
	int32_t RefCountsLenUnityEngineRaycastHit;
	int32_t* RefCountsUnityEngineRaycastHit;
	
	void ReferenceManagedUnityEngineRaycastHit(int32_t handle)
	{
		assert(handle >= 0 && handle < RefCountsLenUnityEngineRaycastHit);
		if (handle != 0)
		{
			RefCountsUnityEngineRaycastHit[handle]++;
		}
	}
	
	void DereferenceManagedUnityEngineRaycastHit(int32_t handle)
	{
		assert(handle >= 0 && handle < RefCountsLenUnityEngineRaycastHit);
		if (handle != 0)
		{
			int32_t numRemain = --RefCountsUnityEngineRaycastHit[handle];
			if (numRemain == 0)
			{
				ReleaseUnityEngineRaycastHit(handle);
			}
		}
	}
	
	int32_t RefCountsLenSystemCollectionsGenericKeyValuePairSystemString_SystemDouble;
	int32_t* RefCountsSystemCollectionsGenericKeyValuePairSystemString_SystemDouble;
	
	void ReferenceManagedSystemCollectionsGenericKeyValuePairSystemString_SystemDouble(int32_t handle)
	{
		assert(handle >= 0 && handle < RefCountsLenSystemCollectionsGenericKeyValuePairSystemString_SystemDouble);
		if (handle != 0)
		{
			RefCountsSystemCollectionsGenericKeyValuePairSystemString_SystemDouble[handle]++;
		}
	}
	
	void DereferenceManagedSystemCollectionsGenericKeyValuePairSystemString_SystemDouble(int32_t handle)
	{
		assert(handle >= 0 && handle < RefCountsLenSystemCollectionsGenericKeyValuePairSystemString_SystemDouble);
		if (handle != 0)
		{
			int32_t numRemain = --RefCountsSystemCollectionsGenericKeyValuePairSystemString_SystemDouble[handle];
			if (numRemain == 0)
			{
				ReleaseSystemCollectionsGenericKeyValuePairSystemString_SystemDouble(handle);
			}
		}
	}
	
	int32_t SystemCollectionsGenericIComparerSystemInt32FreeListSize;
	System::Collections::Generic::IComparer<int32_t>** SystemCollectionsGenericIComparerSystemInt32FreeList;
	System::Collections::Generic::IComparer<int32_t>** NextFreeSystemCollectionsGenericIComparerSystemInt32;
	
	int32_t StoreSystemCollectionsGenericIComparerSystemInt32(System::Collections::Generic::IComparer<int32_t>* del)
	{
		assert(NextFreeSystemCollectionsGenericIComparerSystemInt32 != nullptr);
		System::Collections::Generic::IComparer<int32_t>** pNext = NextFreeSystemCollectionsGenericIComparerSystemInt32;
		NextFreeSystemCollectionsGenericIComparerSystemInt32 = (System::Collections::Generic::IComparer<int32_t>**)*pNext;
		*pNext = del;
		return (int32_t)(pNext - SystemCollectionsGenericIComparerSystemInt32FreeList);
	}
	
	System::Collections::Generic::IComparer<int32_t>* GetSystemCollectionsGenericIComparerSystemInt32(int32_t handle)
	{
		assert(handle >= 0 && handle < SystemCollectionsGenericIComparerSystemInt32FreeListSize);
		return SystemCollectionsGenericIComparerSystemInt32FreeList[handle];
	}
	
	void RemoveSystemCollectionsGenericIComparerSystemInt32(int32_t handle)
	{
		System::Collections::Generic::IComparer<int32_t>** pRelease = SystemCollectionsGenericIComparerSystemInt32FreeList + handle;
		*pRelease = (System::Collections::Generic::IComparer<int32_t>*)NextFreeSystemCollectionsGenericIComparerSystemInt32;
		NextFreeSystemCollectionsGenericIComparerSystemInt32 = pRelease;
	}
	int32_t SystemCollectionsGenericIComparerSystemStringFreeListSize;
	System::Collections::Generic::IComparer<System::String>** SystemCollectionsGenericIComparerSystemStringFreeList;
	System::Collections::Generic::IComparer<System::String>** NextFreeSystemCollectionsGenericIComparerSystemString;
	
	int32_t StoreSystemCollectionsGenericIComparerSystemString(System::Collections::Generic::IComparer<System::String>* del)
	{
		assert(NextFreeSystemCollectionsGenericIComparerSystemString != nullptr);
		System::Collections::Generic::IComparer<System::String>** pNext = NextFreeSystemCollectionsGenericIComparerSystemString;
		NextFreeSystemCollectionsGenericIComparerSystemString = (System::Collections::Generic::IComparer<System::String>**)*pNext;
		*pNext = del;
		return (int32_t)(pNext - SystemCollectionsGenericIComparerSystemStringFreeList);
	}
	
	System::Collections::Generic::IComparer<System::String>* GetSystemCollectionsGenericIComparerSystemString(int32_t handle)
	{
		assert(handle >= 0 && handle < SystemCollectionsGenericIComparerSystemStringFreeListSize);
		return SystemCollectionsGenericIComparerSystemStringFreeList[handle];
	}
	
	void RemoveSystemCollectionsGenericIComparerSystemString(int32_t handle)
	{
		System::Collections::Generic::IComparer<System::String>** pRelease = SystemCollectionsGenericIComparerSystemStringFreeList + handle;
		*pRelease = (System::Collections::Generic::IComparer<System::String>*)NextFreeSystemCollectionsGenericIComparerSystemString;
		NextFreeSystemCollectionsGenericIComparerSystemString = pRelease;
	}
	int32_t SystemStringComparerFreeListSize;
	System::StringComparer** SystemStringComparerFreeList;
	System::StringComparer** NextFreeSystemStringComparer;
	
	int32_t StoreSystemStringComparer(System::StringComparer* del)
	{
		assert(NextFreeSystemStringComparer != nullptr);
		System::StringComparer** pNext = NextFreeSystemStringComparer;
		NextFreeSystemStringComparer = (System::StringComparer**)*pNext;
		*pNext = del;
		return (int32_t)(pNext - SystemStringComparerFreeList);
	}
	
	System::StringComparer* GetSystemStringComparer(int32_t handle)
	{
		assert(handle >= 0 && handle < SystemStringComparerFreeListSize);
		return SystemStringComparerFreeList[handle];
	}
	
	void RemoveSystemStringComparer(int32_t handle)
	{
		System::StringComparer** pRelease = SystemStringComparerFreeList + handle;
		*pRelease = (System::StringComparer*)NextFreeSystemStringComparer;
		NextFreeSystemStringComparer = pRelease;
	}
	int32_t SystemEventArgsFreeListSize;
	System::EventArgs** SystemEventArgsFreeList;
	System::EventArgs** NextFreeSystemEventArgs;
	
	int32_t StoreSystemEventArgs(System::EventArgs* del)
	{
		assert(NextFreeSystemEventArgs != nullptr);
		System::EventArgs** pNext = NextFreeSystemEventArgs;
		NextFreeSystemEventArgs = (System::EventArgs**)*pNext;
		*pNext = del;
		return (int32_t)(pNext - SystemEventArgsFreeList);
	}
	
	System::EventArgs* GetSystemEventArgs(int32_t handle)
	{
		assert(handle >= 0 && handle < SystemEventArgsFreeListSize);
		return SystemEventArgsFreeList[handle];
	}
	
	void RemoveSystemEventArgs(int32_t handle)
	{
		System::EventArgs** pRelease = SystemEventArgsFreeList + handle;
		*pRelease = (System::EventArgs*)NextFreeSystemEventArgs;
		NextFreeSystemEventArgs = pRelease;
	}
	int32_t SystemActionFreeListSize;
	System::Action** SystemActionFreeList;
	System::Action** NextFreeSystemAction;
	
	int32_t StoreSystemAction(System::Action* del)
	{
		assert(NextFreeSystemAction != nullptr);
		System::Action** pNext = NextFreeSystemAction;
		NextFreeSystemAction = (System::Action**)*pNext;
		*pNext = del;
		return (int32_t)(pNext - SystemActionFreeList);
	}
	
	System::Action* GetSystemAction(int32_t handle)
	{
		assert(handle >= 0 && handle < SystemActionFreeListSize);
		return SystemActionFreeList[handle];
	}
	
	void RemoveSystemAction(int32_t handle)
	{
		System::Action** pRelease = SystemActionFreeList + handle;
		*pRelease = (System::Action*)NextFreeSystemAction;
		NextFreeSystemAction = pRelease;
	}
	int32_t SystemActionSystemSingleFreeListSize;
	System::Action1<float>** SystemActionSystemSingleFreeList;
	System::Action1<float>** NextFreeSystemActionSystemSingle;
	
	int32_t StoreSystemActionSystemSingle(System::Action1<float>* del)
	{
		assert(NextFreeSystemActionSystemSingle != nullptr);
		System::Action1<float>** pNext = NextFreeSystemActionSystemSingle;
		NextFreeSystemActionSystemSingle = (System::Action1<float>**)*pNext;
		*pNext = del;
		return (int32_t)(pNext - SystemActionSystemSingleFreeList);
	}
	
	System::Action1<float>* GetSystemActionSystemSingle(int32_t handle)
	{
		assert(handle >= 0 && handle < SystemActionSystemSingleFreeListSize);
		return SystemActionSystemSingleFreeList[handle];
	}
	
	void RemoveSystemActionSystemSingle(int32_t handle)
	{
		System::Action1<float>** pRelease = SystemActionSystemSingleFreeList + handle;
		*pRelease = (System::Action1<float>*)NextFreeSystemActionSystemSingle;
		NextFreeSystemActionSystemSingle = pRelease;
	}
	int32_t SystemActionSystemSingle_SystemSingleFreeListSize;
	System::Action2<float, float>** SystemActionSystemSingle_SystemSingleFreeList;
	System::Action2<float, float>** NextFreeSystemActionSystemSingle_SystemSingle;
	
	int32_t StoreSystemActionSystemSingle_SystemSingle(System::Action2<float, float>* del)
	{
		assert(NextFreeSystemActionSystemSingle_SystemSingle != nullptr);
		System::Action2<float, float>** pNext = NextFreeSystemActionSystemSingle_SystemSingle;
		NextFreeSystemActionSystemSingle_SystemSingle = (System::Action2<float, float>**)*pNext;
		*pNext = del;
		return (int32_t)(pNext - SystemActionSystemSingle_SystemSingleFreeList);
	}
	
	System::Action2<float, float>* GetSystemActionSystemSingle_SystemSingle(int32_t handle)
	{
		assert(handle >= 0 && handle < SystemActionSystemSingle_SystemSingleFreeListSize);
		return SystemActionSystemSingle_SystemSingleFreeList[handle];
	}
	
	void RemoveSystemActionSystemSingle_SystemSingle(int32_t handle)
	{
		System::Action2<float, float>** pRelease = SystemActionSystemSingle_SystemSingleFreeList + handle;
		*pRelease = (System::Action2<float, float>*)NextFreeSystemActionSystemSingle_SystemSingle;
		NextFreeSystemActionSystemSingle_SystemSingle = pRelease;
	}
	int32_t SystemFuncSystemInt32_SystemSingle_SystemDoubleFreeListSize;
	System::Func3<int32_t, float, double>** SystemFuncSystemInt32_SystemSingle_SystemDoubleFreeList;
	System::Func3<int32_t, float, double>** NextFreeSystemFuncSystemInt32_SystemSingle_SystemDouble;
	
	int32_t StoreSystemFuncSystemInt32_SystemSingle_SystemDouble(System::Func3<int32_t, float, double>* del)
	{
		assert(NextFreeSystemFuncSystemInt32_SystemSingle_SystemDouble != nullptr);
		System::Func3<int32_t, float, double>** pNext = NextFreeSystemFuncSystemInt32_SystemSingle_SystemDouble;
		NextFreeSystemFuncSystemInt32_SystemSingle_SystemDouble = (System::Func3<int32_t, float, double>**)*pNext;
		*pNext = del;
		return (int32_t)(pNext - SystemFuncSystemInt32_SystemSingle_SystemDoubleFreeList);
	}
	
	System::Func3<int32_t, float, double>* GetSystemFuncSystemInt32_SystemSingle_SystemDouble(int32_t handle)
	{
		assert(handle >= 0 && handle < SystemFuncSystemInt32_SystemSingle_SystemDoubleFreeListSize);
		return SystemFuncSystemInt32_SystemSingle_SystemDoubleFreeList[handle];
	}
	
	void RemoveSystemFuncSystemInt32_SystemSingle_SystemDouble(int32_t handle)
	{
		System::Func3<int32_t, float, double>** pRelease = SystemFuncSystemInt32_SystemSingle_SystemDoubleFreeList + handle;
		*pRelease = (System::Func3<int32_t, float, double>*)NextFreeSystemFuncSystemInt32_SystemSingle_SystemDouble;
		NextFreeSystemFuncSystemInt32_SystemSingle_SystemDouble = pRelease;
	}
	int32_t SystemFuncSystemInt16_SystemInt32_SystemStringFreeListSize;
	System::Func3<int16_t, int32_t, System::String>** SystemFuncSystemInt16_SystemInt32_SystemStringFreeList;
	System::Func3<int16_t, int32_t, System::String>** NextFreeSystemFuncSystemInt16_SystemInt32_SystemString;
	
	int32_t StoreSystemFuncSystemInt16_SystemInt32_SystemString(System::Func3<int16_t, int32_t, System::String>* del)
	{
		assert(NextFreeSystemFuncSystemInt16_SystemInt32_SystemString != nullptr);
		System::Func3<int16_t, int32_t, System::String>** pNext = NextFreeSystemFuncSystemInt16_SystemInt32_SystemString;
		NextFreeSystemFuncSystemInt16_SystemInt32_SystemString = (System::Func3<int16_t, int32_t, System::String>**)*pNext;
		*pNext = del;
		return (int32_t)(pNext - SystemFuncSystemInt16_SystemInt32_SystemStringFreeList);
	}
	
	System::Func3<int16_t, int32_t, System::String>* GetSystemFuncSystemInt16_SystemInt32_SystemString(int32_t handle)
	{
		assert(handle >= 0 && handle < SystemFuncSystemInt16_SystemInt32_SystemStringFreeListSize);
		return SystemFuncSystemInt16_SystemInt32_SystemStringFreeList[handle];
	}
	
	void RemoveSystemFuncSystemInt16_SystemInt32_SystemString(int32_t handle)
	{
		System::Func3<int16_t, int32_t, System::String>** pRelease = SystemFuncSystemInt16_SystemInt32_SystemStringFreeList + handle;
		*pRelease = (System::Func3<int16_t, int32_t, System::String>*)NextFreeSystemFuncSystemInt16_SystemInt32_SystemString;
		NextFreeSystemFuncSystemInt16_SystemInt32_SystemString = pRelease;
	}
	int32_t SystemAppDomainInitializerFreeListSize;
	System::AppDomainInitializer** SystemAppDomainInitializerFreeList;
	System::AppDomainInitializer** NextFreeSystemAppDomainInitializer;
	
	int32_t StoreSystemAppDomainInitializer(System::AppDomainInitializer* del)
	{
		assert(NextFreeSystemAppDomainInitializer != nullptr);
		System::AppDomainInitializer** pNext = NextFreeSystemAppDomainInitializer;
		NextFreeSystemAppDomainInitializer = (System::AppDomainInitializer**)*pNext;
		*pNext = del;
		return (int32_t)(pNext - SystemAppDomainInitializerFreeList);
	}
	
	System::AppDomainInitializer* GetSystemAppDomainInitializer(int32_t handle)
	{
		assert(handle >= 0 && handle < SystemAppDomainInitializerFreeListSize);
		return SystemAppDomainInitializerFreeList[handle];
	}
	
	void RemoveSystemAppDomainInitializer(int32_t handle)
	{
		System::AppDomainInitializer** pRelease = SystemAppDomainInitializerFreeList + handle;
		*pRelease = (System::AppDomainInitializer*)NextFreeSystemAppDomainInitializer;
		NextFreeSystemAppDomainInitializer = pRelease;
	}
	int32_t UnityEngineEventsUnityActionFreeListSize;
	UnityEngine::Events::UnityAction** UnityEngineEventsUnityActionFreeList;
	UnityEngine::Events::UnityAction** NextFreeUnityEngineEventsUnityAction;
	
	int32_t StoreUnityEngineEventsUnityAction(UnityEngine::Events::UnityAction* del)
	{
		assert(NextFreeUnityEngineEventsUnityAction != nullptr);
		UnityEngine::Events::UnityAction** pNext = NextFreeUnityEngineEventsUnityAction;
		NextFreeUnityEngineEventsUnityAction = (UnityEngine::Events::UnityAction**)*pNext;
		*pNext = del;
		return (int32_t)(pNext - UnityEngineEventsUnityActionFreeList);
	}
	
	UnityEngine::Events::UnityAction* GetUnityEngineEventsUnityAction(int32_t handle)
	{
		assert(handle >= 0 && handle < UnityEngineEventsUnityActionFreeListSize);
		return UnityEngineEventsUnityActionFreeList[handle];
	}
	
	void RemoveUnityEngineEventsUnityAction(int32_t handle)
	{
		UnityEngine::Events::UnityAction** pRelease = UnityEngineEventsUnityActionFreeList + handle;
		*pRelease = (UnityEngine::Events::UnityAction*)NextFreeUnityEngineEventsUnityAction;
		NextFreeUnityEngineEventsUnityAction = pRelease;
	}
	int32_t UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeFreeListSize;
	UnityEngine::Events::UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>** UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeFreeList;
	UnityEngine::Events::UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>** NextFreeUnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneMode;
	
	int32_t StoreUnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneMode(UnityEngine::Events::UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>* del)
	{
		assert(NextFreeUnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneMode != nullptr);
		UnityEngine::Events::UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>** pNext = NextFreeUnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneMode;
		NextFreeUnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneMode = (UnityEngine::Events::UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>**)*pNext;
		*pNext = del;
		return (int32_t)(pNext - UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeFreeList);
	}
	
	UnityEngine::Events::UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>* GetUnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneMode(int32_t handle)
	{
		assert(handle >= 0 && handle < UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeFreeListSize);
		return UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeFreeList[handle];
	}
	
	void RemoveUnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneMode(int32_t handle)
	{
		UnityEngine::Events::UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>** pRelease = UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeFreeList + handle;
		*pRelease = (UnityEngine::Events::UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>*)NextFreeUnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneMode;
		NextFreeUnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneMode = pRelease;
	}
	/*END GLOBAL STATE AND FUNCTIONS*/
}

namespace Plugin
{
	// An unhandled exception caused by C++ calling into C#
	System::Exception* unhandledCsharpException = nullptr;
}

////////////////////////////////////////////////////////////////
// Mirrors of C# types. These wrap the C# functions to present
// a similiar API as in C#.
////////////////////////////////////////////////////////////////

namespace System
{
	Object::Object(Plugin::InternalUse iu, int32_t handle)
		: Handle(handle)
	{
	}
	
	Object::Object(decltype(nullptr) n)
		: Handle(0)
	{
	}
	
	bool Object::operator==(decltype(nullptr) other) const
	{
		return Handle == 0;
	}
	
	bool Object::operator!=(decltype(nullptr) other) const
	{
		return Handle != 0;
	}
	
	void Object::ThrowReferenceToThis()
	{
		throw *this;
	}
	
	ValueType::ValueType(Plugin::InternalUse iu, int32_t handle)
	{
		Handle = handle;
	}
	
	ValueType::ValueType(decltype(nullptr) n)
	{
		Handle = 0;
	}
	
	String::String(decltype(nullptr) n)
		: Object(Plugin::InternalUse::Only, 0)
	{
	}
	
	String::String(Plugin::InternalUse iu, int32_t handle)
		: Object(iu, handle)
	{
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
	}
	
	String::String(const String& other)
		: Object(Plugin::InternalUse::Only, other.Handle)
	{
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
	}
	
	String::String(String&& other)
		: Object(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
	}
	
	String::~String()
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
	}
	
	String& String::operator=(const String& other)
	{
		if (Handle != other.Handle)
		{
			if (Handle)
			{
				Plugin::DereferenceManagedClass(Handle);
			}
			Handle = other.Handle;
			if (Handle)
			{
				Plugin::ReferenceManagedClass(Handle);
			}
		}
		return *this;
	}
	
	String& String::operator=(decltype(nullptr) other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
		return *this;
	}
	
	String& String::operator=(String&& other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
		}
		Handle = other.Handle;
		other.Handle = 0;
		return *this;
	}
	
	String::String()
		: Object(nullptr)
	{
	}
	
	String::String(const char* chars)
		: Object(Plugin::InternalUse::Only, Plugin::StringNew(chars))
	{
	}
	
	Array::Array(Plugin::InternalUse iu, int32_t handle)
		: Object(iu, handle)
	{
	}
	
	Array::Array(decltype(nullptr) n)
		: Object(Plugin::InternalUse::Only, 0)
	{
	}
	
	int32_t Array::GetLength()
	{
		return Plugin::ArrayGetLength(Handle);
	}
	
	int32_t Array::GetRank()
	{
		return Plugin::ArrayGetRank(Handle);
	}
}

/*BEGIN METHOD DEFINITIONS*/
namespace System
{
	namespace Diagnostics
	{
		Stopwatch::Stopwatch(decltype(nullptr) n)
			: Stopwatch(Plugin::InternalUse::Only, 0)
		{
		}
		
		Stopwatch::Stopwatch(Plugin::InternalUse iu, int32_t handle)
			: System::Object(iu, handle)
		{
			if (handle)
			{
				Plugin::ReferenceManagedClass(handle);
			}
		}
		
		Stopwatch::Stopwatch(const Stopwatch& other)
			: Stopwatch(Plugin::InternalUse::Only, other.Handle)
		{
		}
		
		Stopwatch::Stopwatch(Stopwatch&& other)
			: Stopwatch(Plugin::InternalUse::Only, other.Handle)
		{
			other.Handle = 0;
		}
		
		Stopwatch::~Stopwatch()
		{
			if (Handle)
			{
				Plugin::DereferenceManagedClass(Handle);
				Handle = 0;
			}
		}
		
		Stopwatch& Stopwatch::operator=(const Stopwatch& other)
		{
			if (this->Handle)
			{
				Plugin::DereferenceManagedClass(this->Handle);
			}
			this->Handle = other.Handle;
			if (this->Handle)
			{
				Plugin::ReferenceManagedClass(this->Handle);
			}
			return *this;
		}
		
		Stopwatch& Stopwatch::operator=(decltype(nullptr) other)
		{
			if (Handle)
			{
				Plugin::DereferenceManagedClass(Handle);
				Handle = 0;
			}
			return *this;
		}
		
		Stopwatch& Stopwatch::operator=(Stopwatch&& other)
		{
			if (Handle)
			{
				Plugin::DereferenceManagedClass(Handle);
			}
			Handle = other.Handle;
			other.Handle = 0;
			return *this;
		}
		
		bool Stopwatch::operator==(const Stopwatch& other) const
		{
			return Handle == other.Handle;
		}
		
		bool Stopwatch::operator!=(const Stopwatch& other) const
		{
			return Handle != other.Handle;
		}
		
		Stopwatch::Stopwatch()
			 : System::Object(nullptr)
		{
			auto returnValue = Plugin::SystemDiagnosticsStopwatchConstructor();
			if (Plugin::unhandledCsharpException)
			{
				System::Exception* ex = Plugin::unhandledCsharpException;
				Plugin::unhandledCsharpException = nullptr;
				ex->ThrowReferenceToThis();
				delete ex;
			}
			Handle = returnValue;
			if (returnValue)
			{
				Plugin::ReferenceManagedClass(returnValue);
			}
		}
		
		int64_t Stopwatch::GetElapsedMilliseconds()
		{
			auto returnValue = Plugin::SystemDiagnosticsStopwatchPropertyGetElapsedMilliseconds(Handle);
			if (Plugin::unhandledCsharpException)
			{
				System::Exception* ex = Plugin::unhandledCsharpException;
				Plugin::unhandledCsharpException = nullptr;
				ex->ThrowReferenceToThis();
				delete ex;
			}
			return returnValue;
		}
		
		void Stopwatch::Start()
		{
			Plugin::SystemDiagnosticsStopwatchMethodStart(Handle);
			if (Plugin::unhandledCsharpException)
			{
				System::Exception* ex = Plugin::unhandledCsharpException;
				Plugin::unhandledCsharpException = nullptr;
				ex->ThrowReferenceToThis();
				delete ex;
			}
		}
	
		void Stopwatch::Reset()
		{
			Plugin::SystemDiagnosticsStopwatchMethodReset(Handle);
			if (Plugin::unhandledCsharpException)
			{
				System::Exception* ex = Plugin::unhandledCsharpException;
				Plugin::unhandledCsharpException = nullptr;
				ex->ThrowReferenceToThis();
				delete ex;
			}
		}
	}
}

namespace UnityEngine
{
	Object::Object(decltype(nullptr) n)
		: Object(Plugin::InternalUse::Only, 0)
	{
	}
	
	Object::Object(Plugin::InternalUse iu, int32_t handle)
		: System::Object(iu, handle)
	{
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
	}
	
	Object::Object(const Object& other)
		: Object(Plugin::InternalUse::Only, other.Handle)
	{
	}
	
	Object::Object(Object&& other)
		: Object(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
	}
	
	Object::~Object()
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
	}
	
	Object& Object::operator=(const Object& other)
	{
		if (this->Handle)
		{
			Plugin::DereferenceManagedClass(this->Handle);
		}
		this->Handle = other.Handle;
		if (this->Handle)
		{
			Plugin::ReferenceManagedClass(this->Handle);
		}
		return *this;
	}
	
	Object& Object::operator=(decltype(nullptr) other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
		return *this;
	}
	
	Object& Object::operator=(Object&& other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
		}
		Handle = other.Handle;
		other.Handle = 0;
		return *this;
	}
	
	bool Object::operator==(const Object& other) const
	{
		return Handle == other.Handle;
	}
	
	bool Object::operator!=(const Object& other) const
	{
		return Handle != other.Handle;
	}
	
	System::String Object::GetName()
	{
		auto returnValue = Plugin::UnityEngineObjectPropertyGetName(Handle);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return System::String(Plugin::InternalUse::Only, returnValue);
	}
	
	void Object::SetName(System::String value)
	{
		Plugin::UnityEngineObjectPropertySetName(Handle, value.Handle);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
	}
	
	System::Boolean Object::operator==(UnityEngine::Object x)
	{
		auto returnValue = Plugin::UnityEngineObjectMethodop_EqualityUnityEngineObject_UnityEngineObject(Handle, x.Handle);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnValue;
	}
	
	Object::operator System::Boolean()
	{
		auto returnValue = Plugin::UnityEngineObjectMethodop_ImplicitUnityEngineObject(Handle);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnValue;
	}
}

namespace UnityEngine
{
	GameObject::GameObject(decltype(nullptr) n)
		: GameObject(Plugin::InternalUse::Only, 0)
	{
	}
	
	GameObject::GameObject(Plugin::InternalUse iu, int32_t handle)
		: UnityEngine::Object(iu, handle)
	{
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
	}
	
	GameObject::GameObject(const GameObject& other)
		: GameObject(Plugin::InternalUse::Only, other.Handle)
	{
	}
	
	GameObject::GameObject(GameObject&& other)
		: GameObject(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
	}
	
	GameObject::~GameObject()
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
	}
	
	GameObject& GameObject::operator=(const GameObject& other)
	{
		if (this->Handle)
		{
			Plugin::DereferenceManagedClass(this->Handle);
		}
		this->Handle = other.Handle;
		if (this->Handle)
		{
			Plugin::ReferenceManagedClass(this->Handle);
		}
		return *this;
	}
	
	GameObject& GameObject::operator=(decltype(nullptr) other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
		return *this;
	}
	
	GameObject& GameObject::operator=(GameObject&& other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
		}
		Handle = other.Handle;
		other.Handle = 0;
		return *this;
	}
	
	bool GameObject::operator==(const GameObject& other) const
	{
		return Handle == other.Handle;
	}
	
	bool GameObject::operator!=(const GameObject& other) const
	{
		return Handle != other.Handle;
	}
	
	GameObject::GameObject()
		 : UnityEngine::Object(nullptr)
	{
		auto returnValue = Plugin::UnityEngineGameObjectConstructor();
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		Handle = returnValue;
		if (returnValue)
		{
			Plugin::ReferenceManagedClass(returnValue);
		}
	}
	
	GameObject::GameObject(System::String name)
		 : UnityEngine::Object(nullptr)
	{
		auto returnValue = Plugin::UnityEngineGameObjectConstructorSystemString(name.Handle);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		Handle = returnValue;
		if (returnValue)
		{
			Plugin::ReferenceManagedClass(returnValue);
		}
	}
	
	UnityEngine::Transform GameObject::GetTransform()
	{
		auto returnValue = Plugin::UnityEngineGameObjectPropertyGetTransform(Handle);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return UnityEngine::Transform(Plugin::InternalUse::Only, returnValue);
	}
	
	template<> MyGame::MonoBehaviours::TestScript GameObject::AddComponent<MyGame::MonoBehaviours::TestScript>()
	{
		auto returnValue = Plugin::UnityEngineGameObjectMethodAddComponentMyGameMonoBehavioursTestScript(Handle);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return MyGame::MonoBehaviours::TestScript(Plugin::InternalUse::Only, returnValue);
	}
}

namespace UnityEngine
{
	Component::Component(decltype(nullptr) n)
		: Component(Plugin::InternalUse::Only, 0)
	{
	}
	
	Component::Component(Plugin::InternalUse iu, int32_t handle)
		: UnityEngine::Object(iu, handle)
	{
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
	}
	
	Component::Component(const Component& other)
		: Component(Plugin::InternalUse::Only, other.Handle)
	{
	}
	
	Component::Component(Component&& other)
		: Component(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
	}
	
	Component::~Component()
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
	}
	
	Component& Component::operator=(const Component& other)
	{
		if (this->Handle)
		{
			Plugin::DereferenceManagedClass(this->Handle);
		}
		this->Handle = other.Handle;
		if (this->Handle)
		{
			Plugin::ReferenceManagedClass(this->Handle);
		}
		return *this;
	}
	
	Component& Component::operator=(decltype(nullptr) other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
		return *this;
	}
	
	Component& Component::operator=(Component&& other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
		}
		Handle = other.Handle;
		other.Handle = 0;
		return *this;
	}
	
	bool Component::operator==(const Component& other) const
	{
		return Handle == other.Handle;
	}
	
	bool Component::operator!=(const Component& other) const
	{
		return Handle != other.Handle;
	}
	
	UnityEngine::Transform Component::GetTransform()
	{
		auto returnValue = Plugin::UnityEngineComponentPropertyGetTransform(Handle);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return UnityEngine::Transform(Plugin::InternalUse::Only, returnValue);
	}
}

namespace UnityEngine
{
	Transform::Transform(decltype(nullptr) n)
		: Transform(Plugin::InternalUse::Only, 0)
	{
	}
	
	Transform::Transform(Plugin::InternalUse iu, int32_t handle)
		: UnityEngine::Component(iu, handle)
	{
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
	}
	
	Transform::Transform(const Transform& other)
		: Transform(Plugin::InternalUse::Only, other.Handle)
	{
	}
	
	Transform::Transform(Transform&& other)
		: Transform(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
	}
	
	Transform::~Transform()
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
	}
	
	Transform& Transform::operator=(const Transform& other)
	{
		if (this->Handle)
		{
			Plugin::DereferenceManagedClass(this->Handle);
		}
		this->Handle = other.Handle;
		if (this->Handle)
		{
			Plugin::ReferenceManagedClass(this->Handle);
		}
		return *this;
	}
	
	Transform& Transform::operator=(decltype(nullptr) other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
		return *this;
	}
	
	Transform& Transform::operator=(Transform&& other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
		}
		Handle = other.Handle;
		other.Handle = 0;
		return *this;
	}
	
	bool Transform::operator==(const Transform& other) const
	{
		return Handle == other.Handle;
	}
	
	bool Transform::operator!=(const Transform& other) const
	{
		return Handle != other.Handle;
	}
	
	UnityEngine::Vector3 Transform::GetPosition()
	{
		auto returnValue = Plugin::UnityEngineTransformPropertyGetPosition(Handle);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnValue;
	}
	
	void Transform::SetPosition(UnityEngine::Vector3& value)
	{
		Plugin::UnityEngineTransformPropertySetPosition(Handle, value);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
	}
}

namespace UnityEngine
{
	Debug::Debug(decltype(nullptr) n)
		: Debug(Plugin::InternalUse::Only, 0)
	{
	}
	
	Debug::Debug(Plugin::InternalUse iu, int32_t handle)
		: System::Object(iu, handle)
	{
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
	}
	
	Debug::Debug(const Debug& other)
		: Debug(Plugin::InternalUse::Only, other.Handle)
	{
	}
	
	Debug::Debug(Debug&& other)
		: Debug(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
	}
	
	Debug::~Debug()
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
	}
	
	Debug& Debug::operator=(const Debug& other)
	{
		if (this->Handle)
		{
			Plugin::DereferenceManagedClass(this->Handle);
		}
		this->Handle = other.Handle;
		if (this->Handle)
		{
			Plugin::ReferenceManagedClass(this->Handle);
		}
		return *this;
	}
	
	Debug& Debug::operator=(decltype(nullptr) other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
		return *this;
	}
	
	Debug& Debug::operator=(Debug&& other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
		}
		Handle = other.Handle;
		other.Handle = 0;
		return *this;
	}
	
	bool Debug::operator==(const Debug& other) const
	{
		return Handle == other.Handle;
	}
	
	bool Debug::operator!=(const Debug& other) const
	{
		return Handle != other.Handle;
	}
	
	void Debug::Log(System::Object message)
	{
		Plugin::UnityEngineDebugMethodLogSystemObject(message.Handle);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
	}
}

namespace UnityEngine
{
	namespace Assertions
	{
		System::Boolean Assert::GetRaiseExceptions()
		{
			auto returnValue = Plugin::UnityEngineAssertionsAssertFieldGetRaiseExceptions();
			if (Plugin::unhandledCsharpException)
			{
				System::Exception* ex = Plugin::unhandledCsharpException;
				Plugin::unhandledCsharpException = nullptr;
				ex->ThrowReferenceToThis();
				delete ex;
			}
			return returnValue;
		}
		
		void Assert::SetRaiseExceptions(System::Boolean value)
		{
			Plugin::UnityEngineAssertionsAssertFieldSetRaiseExceptions(value);
			if (Plugin::unhandledCsharpException)
			{
				System::Exception* ex = Plugin::unhandledCsharpException;
				Plugin::unhandledCsharpException = nullptr;
				ex->ThrowReferenceToThis();
				delete ex;
			}
		}
		
		template<> void Assert::AreEqual<System::String>(System::String expected, System::String actual)
		{
			Plugin::UnityEngineAssertionsAssertMethodAreEqualSystemStringSystemString_SystemString(expected.Handle, actual.Handle);
			if (Plugin::unhandledCsharpException)
			{
				System::Exception* ex = Plugin::unhandledCsharpException;
				Plugin::unhandledCsharpException = nullptr;
				ex->ThrowReferenceToThis();
				delete ex;
			}
		}
	
		template<> void Assert::AreEqual<UnityEngine::GameObject>(UnityEngine::GameObject expected, UnityEngine::GameObject actual)
		{
			Plugin::UnityEngineAssertionsAssertMethodAreEqualUnityEngineGameObjectUnityEngineGameObject_UnityEngineGameObject(expected.Handle, actual.Handle);
			if (Plugin::unhandledCsharpException)
			{
				System::Exception* ex = Plugin::unhandledCsharpException;
				Plugin::unhandledCsharpException = nullptr;
				ex->ThrowReferenceToThis();
				delete ex;
			}
		}
	}
}

namespace UnityEngine
{
	Collision::Collision(decltype(nullptr) n)
		: Collision(Plugin::InternalUse::Only, 0)
	{
	}
	
	Collision::Collision(Plugin::InternalUse iu, int32_t handle)
		: System::Object(iu, handle)
	{
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
	}
	
	Collision::Collision(const Collision& other)
		: Collision(Plugin::InternalUse::Only, other.Handle)
	{
	}
	
	Collision::Collision(Collision&& other)
		: Collision(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
	}
	
	Collision::~Collision()
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
	}
	
	Collision& Collision::operator=(const Collision& other)
	{
		if (this->Handle)
		{
			Plugin::DereferenceManagedClass(this->Handle);
		}
		this->Handle = other.Handle;
		if (this->Handle)
		{
			Plugin::ReferenceManagedClass(this->Handle);
		}
		return *this;
	}
	
	Collision& Collision::operator=(decltype(nullptr) other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
		return *this;
	}
	
	Collision& Collision::operator=(Collision&& other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
		}
		Handle = other.Handle;
		other.Handle = 0;
		return *this;
	}
	
	bool Collision::operator==(const Collision& other) const
	{
		return Handle == other.Handle;
	}
	
	bool Collision::operator!=(const Collision& other) const
	{
		return Handle != other.Handle;
	}
}

namespace UnityEngine
{
	Behaviour::Behaviour(decltype(nullptr) n)
		: Behaviour(Plugin::InternalUse::Only, 0)
	{
	}
	
	Behaviour::Behaviour(Plugin::InternalUse iu, int32_t handle)
		: UnityEngine::Component(iu, handle)
	{
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
	}
	
	Behaviour::Behaviour(const Behaviour& other)
		: Behaviour(Plugin::InternalUse::Only, other.Handle)
	{
	}
	
	Behaviour::Behaviour(Behaviour&& other)
		: Behaviour(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
	}
	
	Behaviour::~Behaviour()
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
	}
	
	Behaviour& Behaviour::operator=(const Behaviour& other)
	{
		if (this->Handle)
		{
			Plugin::DereferenceManagedClass(this->Handle);
		}
		this->Handle = other.Handle;
		if (this->Handle)
		{
			Plugin::ReferenceManagedClass(this->Handle);
		}
		return *this;
	}
	
	Behaviour& Behaviour::operator=(decltype(nullptr) other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
		return *this;
	}
	
	Behaviour& Behaviour::operator=(Behaviour&& other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
		}
		Handle = other.Handle;
		other.Handle = 0;
		return *this;
	}
	
	bool Behaviour::operator==(const Behaviour& other) const
	{
		return Handle == other.Handle;
	}
	
	bool Behaviour::operator!=(const Behaviour& other) const
	{
		return Handle != other.Handle;
	}
}

namespace UnityEngine
{
	MonoBehaviour::MonoBehaviour(decltype(nullptr) n)
		: MonoBehaviour(Plugin::InternalUse::Only, 0)
	{
	}
	
	MonoBehaviour::MonoBehaviour(Plugin::InternalUse iu, int32_t handle)
		: UnityEngine::Behaviour(iu, handle)
	{
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
	}
	
	MonoBehaviour::MonoBehaviour(const MonoBehaviour& other)
		: MonoBehaviour(Plugin::InternalUse::Only, other.Handle)
	{
	}
	
	MonoBehaviour::MonoBehaviour(MonoBehaviour&& other)
		: MonoBehaviour(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
	}
	
	MonoBehaviour::~MonoBehaviour()
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
	}
	
	MonoBehaviour& MonoBehaviour::operator=(const MonoBehaviour& other)
	{
		if (this->Handle)
		{
			Plugin::DereferenceManagedClass(this->Handle);
		}
		this->Handle = other.Handle;
		if (this->Handle)
		{
			Plugin::ReferenceManagedClass(this->Handle);
		}
		return *this;
	}
	
	MonoBehaviour& MonoBehaviour::operator=(decltype(nullptr) other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
		return *this;
	}
	
	MonoBehaviour& MonoBehaviour::operator=(MonoBehaviour&& other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
		}
		Handle = other.Handle;
		other.Handle = 0;
		return *this;
	}
	
	bool MonoBehaviour::operator==(const MonoBehaviour& other) const
	{
		return Handle == other.Handle;
	}
	
	bool MonoBehaviour::operator!=(const MonoBehaviour& other) const
	{
		return Handle != other.Handle;
	}
}

namespace UnityEngine
{
	AudioSettings::AudioSettings(decltype(nullptr) n)
		: AudioSettings(Plugin::InternalUse::Only, 0)
	{
	}
	
	AudioSettings::AudioSettings(Plugin::InternalUse iu, int32_t handle)
		: System::Object(iu, handle)
	{
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
	}
	
	AudioSettings::AudioSettings(const AudioSettings& other)
		: AudioSettings(Plugin::InternalUse::Only, other.Handle)
	{
	}
	
	AudioSettings::AudioSettings(AudioSettings&& other)
		: AudioSettings(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
	}
	
	AudioSettings::~AudioSettings()
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
	}
	
	AudioSettings& AudioSettings::operator=(const AudioSettings& other)
	{
		if (this->Handle)
		{
			Plugin::DereferenceManagedClass(this->Handle);
		}
		this->Handle = other.Handle;
		if (this->Handle)
		{
			Plugin::ReferenceManagedClass(this->Handle);
		}
		return *this;
	}
	
	AudioSettings& AudioSettings::operator=(decltype(nullptr) other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
		return *this;
	}
	
	AudioSettings& AudioSettings::operator=(AudioSettings&& other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
		}
		Handle = other.Handle;
		other.Handle = 0;
		return *this;
	}
	
	bool AudioSettings::operator==(const AudioSettings& other) const
	{
		return Handle == other.Handle;
	}
	
	bool AudioSettings::operator!=(const AudioSettings& other) const
	{
		return Handle != other.Handle;
	}
	
	void AudioSettings::GetDSPBufferSize(int32_t* bufferLength, int32_t* numBuffers)
	{
		Plugin::UnityEngineAudioSettingsMethodGetDSPBufferSizeSystemInt32_SystemInt32(bufferLength, numBuffers);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
	}
}

namespace UnityEngine
{
	namespace Networking
	{
		NetworkTransport::NetworkTransport(decltype(nullptr) n)
			: NetworkTransport(Plugin::InternalUse::Only, 0)
		{
		}
		
		NetworkTransport::NetworkTransport(Plugin::InternalUse iu, int32_t handle)
			: System::Object(iu, handle)
		{
			if (handle)
			{
				Plugin::ReferenceManagedClass(handle);
			}
		}
		
		NetworkTransport::NetworkTransport(const NetworkTransport& other)
			: NetworkTransport(Plugin::InternalUse::Only, other.Handle)
		{
		}
		
		NetworkTransport::NetworkTransport(NetworkTransport&& other)
			: NetworkTransport(Plugin::InternalUse::Only, other.Handle)
		{
			other.Handle = 0;
		}
		
		NetworkTransport::~NetworkTransport()
		{
			if (Handle)
			{
				Plugin::DereferenceManagedClass(Handle);
				Handle = 0;
			}
		}
		
		NetworkTransport& NetworkTransport::operator=(const NetworkTransport& other)
		{
			if (this->Handle)
			{
				Plugin::DereferenceManagedClass(this->Handle);
			}
			this->Handle = other.Handle;
			if (this->Handle)
			{
				Plugin::ReferenceManagedClass(this->Handle);
			}
			return *this;
		}
		
		NetworkTransport& NetworkTransport::operator=(decltype(nullptr) other)
		{
			if (Handle)
			{
				Plugin::DereferenceManagedClass(Handle);
				Handle = 0;
			}
			return *this;
		}
		
		NetworkTransport& NetworkTransport::operator=(NetworkTransport&& other)
		{
			if (Handle)
			{
				Plugin::DereferenceManagedClass(Handle);
			}
			Handle = other.Handle;
			other.Handle = 0;
			return *this;
		}
		
		bool NetworkTransport::operator==(const NetworkTransport& other) const
		{
			return Handle == other.Handle;
		}
		
		bool NetworkTransport::operator!=(const NetworkTransport& other) const
		{
			return Handle != other.Handle;
		}
		
		void NetworkTransport::GetBroadcastConnectionInfo(int32_t hostId, System::String* address, int32_t* port, uint8_t* error)
		{
			int32_t addressHandle = address->Handle;
			Plugin::UnityEngineNetworkingNetworkTransportMethodGetBroadcastConnectionInfoSystemInt32_SystemString_SystemInt32_SystemByte(hostId, &addressHandle, port, error);
			if (Plugin::unhandledCsharpException)
			{
				System::Exception* ex = Plugin::unhandledCsharpException;
				Plugin::unhandledCsharpException = nullptr;
				ex->ThrowReferenceToThis();
				delete ex;
			}
			if (address->Handle)
			{
				Plugin::DereferenceManagedClass(address->Handle);
			}
			address->Handle = addressHandle;
			if (address->Handle)
			{
				Plugin::ReferenceManagedClass(address->Handle);
			}
		}
	
		void NetworkTransport::Init()
		{
			Plugin::UnityEngineNetworkingNetworkTransportMethodInit();
			if (Plugin::unhandledCsharpException)
			{
				System::Exception* ex = Plugin::unhandledCsharpException;
				Plugin::unhandledCsharpException = nullptr;
				ex->ThrowReferenceToThis();
				delete ex;
			}
		}
	}
}

namespace UnityEngine
{
	Vector3::Vector3()
	{
	}
	
	Vector3::Vector3(float x, float y, float z)
	{
		auto returnValue = Plugin::UnityEngineVector3ConstructorSystemSingle_SystemSingle_SystemSingle(x, y, z);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		*this = returnValue;
	}
	
	float Vector3::GetMagnitude()
	{
		auto returnValue = Plugin::UnityEngineVector3PropertyGetMagnitude(this);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnValue;
	}
	
	void Vector3::Set(float newX, float newY, float newZ)
	{
		Plugin::UnityEngineVector3MethodSetSystemSingle_SystemSingle_SystemSingle(this, newX, newY, newZ);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
	}
	
	UnityEngine::Vector3 Vector3::operator+(UnityEngine::Vector3& a)
	{
		auto returnValue = Plugin::UnityEngineVector3Methodop_AdditionUnityEngineVector3_UnityEngineVector3(*this, a);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnValue;
	}
	
	UnityEngine::Vector3 Vector3::operator-()
	{
		auto returnValue = Plugin::UnityEngineVector3Methodop_UnaryNegationUnityEngineVector3(*this);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnValue;
	}
}

namespace System
{
	Object::Object(UnityEngine::Vector3& val)
	{
		int32_t handle = Plugin::BoxVector3(val);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
			Handle = handle;
		}
	}
	
	Object::operator UnityEngine::Vector3()
	{
		UnityEngine::Vector3 returnVal(Plugin::UnboxVector3(Handle));
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnVal;
	}
}

namespace UnityEngine
{
	Matrix4x4::Matrix4x4()
	{
	}
	
	float Matrix4x4::GetItem(int32_t row, int32_t column)
	{
		auto returnValue = Plugin::UnityEngineMatrix4x4PropertyGetItem(this, row, column);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnValue;
	}
	
	void Matrix4x4::SetItem(int32_t row, int32_t column, float value)
	{
		Plugin::UnityEngineMatrix4x4PropertySetItem(this, row, column, value);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
	}
}

namespace System
{
	Object::Object(UnityEngine::Matrix4x4& val)
	{
		int32_t handle = Plugin::BoxMatrix4x4(val);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
			Handle = handle;
		}
	}
	
	Object::operator UnityEngine::Matrix4x4()
	{
		UnityEngine::Matrix4x4 returnVal(Plugin::UnboxMatrix4x4(Handle));
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnVal;
	}
}

namespace UnityEngine
{
	RaycastHit::RaycastHit(decltype(nullptr) n)
		: RaycastHit(Plugin::InternalUse::Only, 0)
	{
	}
	
	RaycastHit::RaycastHit(Plugin::InternalUse iu, int32_t handle)
		: System::ValueType(iu, handle)
	{
		if (handle)
		{
			Plugin::ReferenceManagedUnityEngineRaycastHit(Handle);
		}
	}
	
	RaycastHit::RaycastHit(const RaycastHit& other)
		: RaycastHit(Plugin::InternalUse::Only, other.Handle)
	{
	}
	
	RaycastHit::RaycastHit(RaycastHit&& other)
		: RaycastHit(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
	}
	
	RaycastHit::~RaycastHit()
	{
		if (Handle)
		{
			Plugin::DereferenceManagedUnityEngineRaycastHit(Handle);
			Handle = 0;
		}
	}
	
	RaycastHit& RaycastHit::operator=(const RaycastHit& other)
	{
		if (this->Handle)
		{
			Plugin::DereferenceManagedUnityEngineRaycastHit(Handle);
		}
		this->Handle = other.Handle;
		if (this->Handle)
		{
			Plugin::ReferenceManagedUnityEngineRaycastHit(Handle);
		}
		return *this;
	}
	
	RaycastHit& RaycastHit::operator=(decltype(nullptr) other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedUnityEngineRaycastHit(Handle);
			Handle = 0;
		}
		return *this;
	}
	
	RaycastHit& RaycastHit::operator=(RaycastHit&& other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedUnityEngineRaycastHit(Handle);
		}
		Handle = other.Handle;
		other.Handle = 0;
		return *this;
	}
	
	bool RaycastHit::operator==(const RaycastHit& other) const
	{
		return Handle == other.Handle;
	}
	
	bool RaycastHit::operator!=(const RaycastHit& other) const
	{
		return Handle != other.Handle;
	}
	
	UnityEngine::Vector3 RaycastHit::GetPoint()
	{
		auto returnValue = Plugin::UnityEngineRaycastHitPropertyGetPoint(Handle);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnValue;
	}
	
	void RaycastHit::SetPoint(UnityEngine::Vector3& value)
	{
		Plugin::UnityEngineRaycastHitPropertySetPoint(Handle, value);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
	}
	
	UnityEngine::Transform RaycastHit::GetTransform()
	{
		auto returnValue = Plugin::UnityEngineRaycastHitPropertyGetTransform(Handle);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return UnityEngine::Transform(Plugin::InternalUse::Only, returnValue);
	}
}

namespace System
{
	Object::Object(UnityEngine::RaycastHit val)
	{
		int32_t handle = Plugin::BoxRaycastHit(val.Handle);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
			Handle = handle;
		}
	}
	
	Object::operator UnityEngine::RaycastHit()
	{
		UnityEngine::RaycastHit returnVal(Plugin::InternalUse::Only, Plugin::UnboxRaycastHit(Handle));
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnVal;
	}
}

namespace System
{
	Object::Object(UnityEngine::QueryTriggerInteraction val)
	{
		int32_t handle = Plugin::BoxQueryTriggerInteraction(val);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
			Handle = handle;
		}
	}
	
	Object::operator UnityEngine::QueryTriggerInteraction()
	{
		UnityEngine::QueryTriggerInteraction returnVal(Plugin::UnboxQueryTriggerInteraction(Handle));
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnVal;
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			KeyValuePair<System::String, double>::KeyValuePair(decltype(nullptr) n)
				: KeyValuePair(Plugin::InternalUse::Only, 0)
			{
			}
			
			KeyValuePair<System::String, double>::KeyValuePair(Plugin::InternalUse iu, int32_t handle)
				: System::ValueType(iu, handle)
			{
				if (handle)
				{
					Plugin::ReferenceManagedSystemCollectionsGenericKeyValuePairSystemString_SystemDouble(Handle);
				}
			}
			
			KeyValuePair<System::String, double>::KeyValuePair(const KeyValuePair<System::String, double>& other)
				: KeyValuePair(Plugin::InternalUse::Only, other.Handle)
			{
			}
			
			KeyValuePair<System::String, double>::KeyValuePair(KeyValuePair<System::String, double>&& other)
				: KeyValuePair(Plugin::InternalUse::Only, other.Handle)
			{
				other.Handle = 0;
			}
			
			KeyValuePair<System::String, double>::~KeyValuePair<System::String, double>()
			{
				if (Handle)
				{
					Plugin::DereferenceManagedSystemCollectionsGenericKeyValuePairSystemString_SystemDouble(Handle);
					Handle = 0;
				}
			}
			
			KeyValuePair<System::String, double>& KeyValuePair<System::String, double>::operator=(const KeyValuePair<System::String, double>& other)
			{
				if (this->Handle)
				{
					Plugin::DereferenceManagedSystemCollectionsGenericKeyValuePairSystemString_SystemDouble(Handle);
				}
				this->Handle = other.Handle;
				if (this->Handle)
				{
					Plugin::ReferenceManagedSystemCollectionsGenericKeyValuePairSystemString_SystemDouble(Handle);
				}
				return *this;
			}
			
			KeyValuePair<System::String, double>& KeyValuePair<System::String, double>::operator=(decltype(nullptr) other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedSystemCollectionsGenericKeyValuePairSystemString_SystemDouble(Handle);
					Handle = 0;
				}
				return *this;
			}
			
			KeyValuePair<System::String, double>& KeyValuePair<System::String, double>::operator=(KeyValuePair<System::String, double>&& other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedSystemCollectionsGenericKeyValuePairSystemString_SystemDouble(Handle);
				}
				Handle = other.Handle;
				other.Handle = 0;
				return *this;
			}
			
			bool KeyValuePair<System::String, double>::operator==(const KeyValuePair<System::String, double>& other) const
			{
				return Handle == other.Handle;
			}
			
			bool KeyValuePair<System::String, double>::operator!=(const KeyValuePair<System::String, double>& other) const
			{
				return Handle != other.Handle;
			}
			
			KeyValuePair<System::String, double>::KeyValuePair(System::String key, double value)
				 : System::ValueType(nullptr)
			{
				auto returnValue = Plugin::SystemCollectionsGenericKeyValuePairSystemString_SystemDoubleConstructorSystemString_SystemDouble(key.Handle, value);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
				Handle = returnValue;
				if (returnValue)
				{
					Plugin::ReferenceManagedSystemCollectionsGenericKeyValuePairSystemString_SystemDouble(Handle);
				}
			}
			
			System::String KeyValuePair<System::String, double>::GetKey()
			{
				auto returnValue = Plugin::SystemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetKey(Handle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
				return System::String(Plugin::InternalUse::Only, returnValue);
			}
			
			double KeyValuePair<System::String, double>::GetValue()
			{
				auto returnValue = Plugin::SystemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetValue(Handle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
				return returnValue;
			}
		}
	}
}

namespace System
{
	Object::Object(System::Collections::Generic::KeyValuePair<System::String, double> val)
	{
		int32_t handle = Plugin::BoxKeyValuePairSystemString_SystemDouble(val.Handle);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
			Handle = handle;
		}
	}
	
	Object::operator System::Collections::Generic::KeyValuePair<System::String, double>()
	{
		System::Collections::Generic::KeyValuePair<System::String, double> returnVal(Plugin::InternalUse::Only, Plugin::UnboxKeyValuePairSystemString_SystemDouble(Handle));
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnVal;
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			List<System::String>::List(decltype(nullptr) n)
				: List(Plugin::InternalUse::Only, 0)
			{
			}
			
			List<System::String>::List(Plugin::InternalUse iu, int32_t handle)
				: System::Object(iu, handle)
			{
				if (handle)
				{
					Plugin::ReferenceManagedClass(handle);
				}
			}
			
			List<System::String>::List(const List<System::String>& other)
				: List(Plugin::InternalUse::Only, other.Handle)
			{
			}
			
			List<System::String>::List(List<System::String>&& other)
				: List(Plugin::InternalUse::Only, other.Handle)
			{
				other.Handle = 0;
			}
			
			List<System::String>::~List<System::String>()
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
			}
			
			List<System::String>& List<System::String>::operator=(const List<System::String>& other)
			{
				if (this->Handle)
				{
					Plugin::DereferenceManagedClass(this->Handle);
				}
				this->Handle = other.Handle;
				if (this->Handle)
				{
					Plugin::ReferenceManagedClass(this->Handle);
				}
				return *this;
			}
			
			List<System::String>& List<System::String>::operator=(decltype(nullptr) other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
				return *this;
			}
			
			List<System::String>& List<System::String>::operator=(List<System::String>&& other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
				}
				Handle = other.Handle;
				other.Handle = 0;
				return *this;
			}
			
			bool List<System::String>::operator==(const List<System::String>& other) const
			{
				return Handle == other.Handle;
			}
			
			bool List<System::String>::operator!=(const List<System::String>& other) const
			{
				return Handle != other.Handle;
			}
			
			List<System::String>::List()
				 : System::Object(nullptr)
			{
				auto returnValue = Plugin::SystemCollectionsGenericListSystemStringConstructor();
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
				Handle = returnValue;
				if (returnValue)
				{
					Plugin::ReferenceManagedClass(returnValue);
				}
			}
			
			System::String List<System::String>::GetItem(int32_t index)
			{
				auto returnValue = Plugin::SystemCollectionsGenericListSystemStringPropertyGetItem(Handle, index);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
				return System::String(Plugin::InternalUse::Only, returnValue);
			}
			
			void List<System::String>::SetItem(int32_t index, System::String value)
			{
				Plugin::SystemCollectionsGenericListSystemStringPropertySetItem(Handle, index, value.Handle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
			}
			
			void List<System::String>::Add(System::String item)
			{
				Plugin::SystemCollectionsGenericListSystemStringMethodAddSystemString(Handle, item.Handle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
			}
	
			void List<System::String>::Sort(System::Collections::Generic::IComparer<System::String> comparer)
			{
				Plugin::SystemCollectionsGenericListSystemStringMethodSortSystemCollectionsGenericIComparer(Handle, comparer.Handle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
			}
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			List<int32_t>::List(decltype(nullptr) n)
				: List(Plugin::InternalUse::Only, 0)
			{
			}
			
			List<int32_t>::List(Plugin::InternalUse iu, int32_t handle)
				: System::Object(iu, handle)
			{
				if (handle)
				{
					Plugin::ReferenceManagedClass(handle);
				}
			}
			
			List<int32_t>::List(const List<int32_t>& other)
				: List(Plugin::InternalUse::Only, other.Handle)
			{
			}
			
			List<int32_t>::List(List<int32_t>&& other)
				: List(Plugin::InternalUse::Only, other.Handle)
			{
				other.Handle = 0;
			}
			
			List<int32_t>::~List<int32_t>()
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
			}
			
			List<int32_t>& List<int32_t>::operator=(const List<int32_t>& other)
			{
				if (this->Handle)
				{
					Plugin::DereferenceManagedClass(this->Handle);
				}
				this->Handle = other.Handle;
				if (this->Handle)
				{
					Plugin::ReferenceManagedClass(this->Handle);
				}
				return *this;
			}
			
			List<int32_t>& List<int32_t>::operator=(decltype(nullptr) other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
				return *this;
			}
			
			List<int32_t>& List<int32_t>::operator=(List<int32_t>&& other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
				}
				Handle = other.Handle;
				other.Handle = 0;
				return *this;
			}
			
			bool List<int32_t>::operator==(const List<int32_t>& other) const
			{
				return Handle == other.Handle;
			}
			
			bool List<int32_t>::operator!=(const List<int32_t>& other) const
			{
				return Handle != other.Handle;
			}
			
			List<int32_t>::List()
				 : System::Object(nullptr)
			{
				auto returnValue = Plugin::SystemCollectionsGenericListSystemInt32Constructor();
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
				Handle = returnValue;
				if (returnValue)
				{
					Plugin::ReferenceManagedClass(returnValue);
				}
			}
			
			int32_t List<int32_t>::GetItem(int32_t index)
			{
				auto returnValue = Plugin::SystemCollectionsGenericListSystemInt32PropertyGetItem(Handle, index);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
				return returnValue;
			}
			
			void List<int32_t>::SetItem(int32_t index, int32_t value)
			{
				Plugin::SystemCollectionsGenericListSystemInt32PropertySetItem(Handle, index, value);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
			}
			
			void List<int32_t>::Add(int32_t item)
			{
				Plugin::SystemCollectionsGenericListSystemInt32MethodAddSystemInt32(Handle, item);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
			}
	
			void List<int32_t>::Sort(System::Collections::Generic::IComparer<int32_t> comparer)
			{
				Plugin::SystemCollectionsGenericListSystemInt32MethodSortSystemCollectionsGenericIComparer(Handle, comparer.Handle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
			}
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			LinkedListNode<System::String>::LinkedListNode(decltype(nullptr) n)
				: LinkedListNode(Plugin::InternalUse::Only, 0)
			{
			}
			
			LinkedListNode<System::String>::LinkedListNode(Plugin::InternalUse iu, int32_t handle)
				: System::Object(iu, handle)
			{
				if (handle)
				{
					Plugin::ReferenceManagedClass(handle);
				}
			}
			
			LinkedListNode<System::String>::LinkedListNode(const LinkedListNode<System::String>& other)
				: LinkedListNode(Plugin::InternalUse::Only, other.Handle)
			{
			}
			
			LinkedListNode<System::String>::LinkedListNode(LinkedListNode<System::String>&& other)
				: LinkedListNode(Plugin::InternalUse::Only, other.Handle)
			{
				other.Handle = 0;
			}
			
			LinkedListNode<System::String>::~LinkedListNode<System::String>()
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
			}
			
			LinkedListNode<System::String>& LinkedListNode<System::String>::operator=(const LinkedListNode<System::String>& other)
			{
				if (this->Handle)
				{
					Plugin::DereferenceManagedClass(this->Handle);
				}
				this->Handle = other.Handle;
				if (this->Handle)
				{
					Plugin::ReferenceManagedClass(this->Handle);
				}
				return *this;
			}
			
			LinkedListNode<System::String>& LinkedListNode<System::String>::operator=(decltype(nullptr) other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
				return *this;
			}
			
			LinkedListNode<System::String>& LinkedListNode<System::String>::operator=(LinkedListNode<System::String>&& other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
				}
				Handle = other.Handle;
				other.Handle = 0;
				return *this;
			}
			
			bool LinkedListNode<System::String>::operator==(const LinkedListNode<System::String>& other) const
			{
				return Handle == other.Handle;
			}
			
			bool LinkedListNode<System::String>::operator!=(const LinkedListNode<System::String>& other) const
			{
				return Handle != other.Handle;
			}
			
			LinkedListNode<System::String>::LinkedListNode(System::String value)
				 : System::Object(nullptr)
			{
				auto returnValue = Plugin::SystemCollectionsGenericLinkedListNodeSystemStringConstructorSystemString(value.Handle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
				Handle = returnValue;
				if (returnValue)
				{
					Plugin::ReferenceManagedClass(returnValue);
				}
			}
			
			System::String LinkedListNode<System::String>::GetValue()
			{
				auto returnValue = Plugin::SystemCollectionsGenericLinkedListNodeSystemStringPropertyGetValue(Handle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
				return System::String(Plugin::InternalUse::Only, returnValue);
			}
			
			void LinkedListNode<System::String>::SetValue(System::String value)
			{
				Plugin::SystemCollectionsGenericLinkedListNodeSystemStringPropertySetValue(Handle, value.Handle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
			}
		}
	}
}

namespace System
{
	namespace Runtime
	{
		namespace CompilerServices
		{
			StrongBox<System::String>::StrongBox(decltype(nullptr) n)
				: StrongBox(Plugin::InternalUse::Only, 0)
			{
			}
			
			StrongBox<System::String>::StrongBox(Plugin::InternalUse iu, int32_t handle)
				: System::Object(iu, handle)
			{
				if (handle)
				{
					Plugin::ReferenceManagedClass(handle);
				}
			}
			
			StrongBox<System::String>::StrongBox(const StrongBox<System::String>& other)
				: StrongBox(Plugin::InternalUse::Only, other.Handle)
			{
			}
			
			StrongBox<System::String>::StrongBox(StrongBox<System::String>&& other)
				: StrongBox(Plugin::InternalUse::Only, other.Handle)
			{
				other.Handle = 0;
			}
			
			StrongBox<System::String>::~StrongBox<System::String>()
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
			}
			
			StrongBox<System::String>& StrongBox<System::String>::operator=(const StrongBox<System::String>& other)
			{
				if (this->Handle)
				{
					Plugin::DereferenceManagedClass(this->Handle);
				}
				this->Handle = other.Handle;
				if (this->Handle)
				{
					Plugin::ReferenceManagedClass(this->Handle);
				}
				return *this;
			}
			
			StrongBox<System::String>& StrongBox<System::String>::operator=(decltype(nullptr) other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
				return *this;
			}
			
			StrongBox<System::String>& StrongBox<System::String>::operator=(StrongBox<System::String>&& other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
				}
				Handle = other.Handle;
				other.Handle = 0;
				return *this;
			}
			
			bool StrongBox<System::String>::operator==(const StrongBox<System::String>& other) const
			{
				return Handle == other.Handle;
			}
			
			bool StrongBox<System::String>::operator!=(const StrongBox<System::String>& other) const
			{
				return Handle != other.Handle;
			}
			
			StrongBox<System::String>::StrongBox(System::String value)
				 : System::Object(nullptr)
			{
				auto returnValue = Plugin::SystemRuntimeCompilerServicesStrongBoxSystemStringConstructorSystemString(value.Handle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
				Handle = returnValue;
				if (returnValue)
				{
					Plugin::ReferenceManagedClass(returnValue);
				}
			}
			
			System::String StrongBox<System::String>::GetValue()
			{
				auto returnValue = Plugin::SystemRuntimeCompilerServicesStrongBoxSystemStringFieldGetValue(Handle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
				return System::String(Plugin::InternalUse::Only, returnValue);
			}
			
			void StrongBox<System::String>::SetValue(System::String value)
			{
				Plugin::SystemRuntimeCompilerServicesStrongBoxSystemStringFieldSetValue(Handle, value.Handle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
			}
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace ObjectModel
		{
			Collection<int32_t>::Collection(decltype(nullptr) n)
				: Collection(Plugin::InternalUse::Only, 0)
			{
			}
			
			Collection<int32_t>::Collection(Plugin::InternalUse iu, int32_t handle)
				: System::Object(iu, handle)
			{
				if (handle)
				{
					Plugin::ReferenceManagedClass(handle);
				}
			}
			
			Collection<int32_t>::Collection(const Collection<int32_t>& other)
				: Collection(Plugin::InternalUse::Only, other.Handle)
			{
			}
			
			Collection<int32_t>::Collection(Collection<int32_t>&& other)
				: Collection(Plugin::InternalUse::Only, other.Handle)
			{
				other.Handle = 0;
			}
			
			Collection<int32_t>::~Collection<int32_t>()
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
			}
			
			Collection<int32_t>& Collection<int32_t>::operator=(const Collection<int32_t>& other)
			{
				if (this->Handle)
				{
					Plugin::DereferenceManagedClass(this->Handle);
				}
				this->Handle = other.Handle;
				if (this->Handle)
				{
					Plugin::ReferenceManagedClass(this->Handle);
				}
				return *this;
			}
			
			Collection<int32_t>& Collection<int32_t>::operator=(decltype(nullptr) other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
				return *this;
			}
			
			Collection<int32_t>& Collection<int32_t>::operator=(Collection<int32_t>&& other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
				}
				Handle = other.Handle;
				other.Handle = 0;
				return *this;
			}
			
			bool Collection<int32_t>::operator==(const Collection<int32_t>& other) const
			{
				return Handle == other.Handle;
			}
			
			bool Collection<int32_t>::operator!=(const Collection<int32_t>& other) const
			{
				return Handle != other.Handle;
			}
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace ObjectModel
		{
			KeyedCollection<System::String, int32_t>::KeyedCollection(decltype(nullptr) n)
				: KeyedCollection(Plugin::InternalUse::Only, 0)
			{
			}
			
			KeyedCollection<System::String, int32_t>::KeyedCollection(Plugin::InternalUse iu, int32_t handle)
				: System::Collections::ObjectModel::Collection<int32_t>(iu, handle)
			{
				if (handle)
				{
					Plugin::ReferenceManagedClass(handle);
				}
			}
			
			KeyedCollection<System::String, int32_t>::KeyedCollection(const KeyedCollection<System::String, int32_t>& other)
				: KeyedCollection(Plugin::InternalUse::Only, other.Handle)
			{
			}
			
			KeyedCollection<System::String, int32_t>::KeyedCollection(KeyedCollection<System::String, int32_t>&& other)
				: KeyedCollection(Plugin::InternalUse::Only, other.Handle)
			{
				other.Handle = 0;
			}
			
			KeyedCollection<System::String, int32_t>::~KeyedCollection<System::String, int32_t>()
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
			}
			
			KeyedCollection<System::String, int32_t>& KeyedCollection<System::String, int32_t>::operator=(const KeyedCollection<System::String, int32_t>& other)
			{
				if (this->Handle)
				{
					Plugin::DereferenceManagedClass(this->Handle);
				}
				this->Handle = other.Handle;
				if (this->Handle)
				{
					Plugin::ReferenceManagedClass(this->Handle);
				}
				return *this;
			}
			
			KeyedCollection<System::String, int32_t>& KeyedCollection<System::String, int32_t>::operator=(decltype(nullptr) other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
				return *this;
			}
			
			KeyedCollection<System::String, int32_t>& KeyedCollection<System::String, int32_t>::operator=(KeyedCollection<System::String, int32_t>&& other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
				}
				Handle = other.Handle;
				other.Handle = 0;
				return *this;
			}
			
			bool KeyedCollection<System::String, int32_t>::operator==(const KeyedCollection<System::String, int32_t>& other) const
			{
				return Handle == other.Handle;
			}
			
			bool KeyedCollection<System::String, int32_t>::operator!=(const KeyedCollection<System::String, int32_t>& other) const
			{
				return Handle != other.Handle;
			}
		}
	}
}

namespace System
{
	Exception::Exception(decltype(nullptr) n)
		: Exception(Plugin::InternalUse::Only, 0)
	{
	}
	
	Exception::Exception(Plugin::InternalUse iu, int32_t handle)
		: System::Object(iu, handle)
	{
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
	}
	
	Exception::Exception(const Exception& other)
		: Exception(Plugin::InternalUse::Only, other.Handle)
	{
	}
	
	Exception::Exception(Exception&& other)
		: Exception(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
	}
	
	Exception::~Exception()
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
	}
	
	Exception& Exception::operator=(const Exception& other)
	{
		if (this->Handle)
		{
			Plugin::DereferenceManagedClass(this->Handle);
		}
		this->Handle = other.Handle;
		if (this->Handle)
		{
			Plugin::ReferenceManagedClass(this->Handle);
		}
		return *this;
	}
	
	Exception& Exception::operator=(decltype(nullptr) other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
		return *this;
	}
	
	Exception& Exception::operator=(Exception&& other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
		}
		Handle = other.Handle;
		other.Handle = 0;
		return *this;
	}
	
	bool Exception::operator==(const Exception& other) const
	{
		return Handle == other.Handle;
	}
	
	bool Exception::operator!=(const Exception& other) const
	{
		return Handle != other.Handle;
	}
	
	Exception::Exception(System::String message)
		 : System::Object(nullptr)
	{
		auto returnValue = Plugin::SystemExceptionConstructorSystemString(message.Handle);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		Handle = returnValue;
		if (returnValue)
		{
			Plugin::ReferenceManagedClass(returnValue);
		}
	}
}

namespace System
{
	SystemException::SystemException(decltype(nullptr) n)
		: SystemException(Plugin::InternalUse::Only, 0)
	{
	}
	
	SystemException::SystemException(Plugin::InternalUse iu, int32_t handle)
		: System::Exception(iu, handle)
	{
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
	}
	
	SystemException::SystemException(const SystemException& other)
		: SystemException(Plugin::InternalUse::Only, other.Handle)
	{
	}
	
	SystemException::SystemException(SystemException&& other)
		: SystemException(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
	}
	
	SystemException::~SystemException()
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
	}
	
	SystemException& SystemException::operator=(const SystemException& other)
	{
		if (this->Handle)
		{
			Plugin::DereferenceManagedClass(this->Handle);
		}
		this->Handle = other.Handle;
		if (this->Handle)
		{
			Plugin::ReferenceManagedClass(this->Handle);
		}
		return *this;
	}
	
	SystemException& SystemException::operator=(decltype(nullptr) other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
		return *this;
	}
	
	SystemException& SystemException::operator=(SystemException&& other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
		}
		Handle = other.Handle;
		other.Handle = 0;
		return *this;
	}
	
	bool SystemException::operator==(const SystemException& other) const
	{
		return Handle == other.Handle;
	}
	
	bool SystemException::operator!=(const SystemException& other) const
	{
		return Handle != other.Handle;
	}
}

namespace System
{
	NullReferenceException::NullReferenceException(decltype(nullptr) n)
		: NullReferenceException(Plugin::InternalUse::Only, 0)
	{
	}
	
	NullReferenceException::NullReferenceException(Plugin::InternalUse iu, int32_t handle)
		: System::SystemException(iu, handle)
	{
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
	}
	
	NullReferenceException::NullReferenceException(const NullReferenceException& other)
		: NullReferenceException(Plugin::InternalUse::Only, other.Handle)
	{
	}
	
	NullReferenceException::NullReferenceException(NullReferenceException&& other)
		: NullReferenceException(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
	}
	
	NullReferenceException::~NullReferenceException()
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
	}
	
	NullReferenceException& NullReferenceException::operator=(const NullReferenceException& other)
	{
		if (this->Handle)
		{
			Plugin::DereferenceManagedClass(this->Handle);
		}
		this->Handle = other.Handle;
		if (this->Handle)
		{
			Plugin::ReferenceManagedClass(this->Handle);
		}
		return *this;
	}
	
	NullReferenceException& NullReferenceException::operator=(decltype(nullptr) other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
		return *this;
	}
	
	NullReferenceException& NullReferenceException::operator=(NullReferenceException&& other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
		}
		Handle = other.Handle;
		other.Handle = 0;
		return *this;
	}
	
	bool NullReferenceException::operator==(const NullReferenceException& other) const
	{
		return Handle == other.Handle;
	}
	
	bool NullReferenceException::operator!=(const NullReferenceException& other) const
	{
		return Handle != other.Handle;
	}
}

namespace UnityEngine
{
	Resolution::Resolution()
	{
	}
	
	int32_t Resolution::GetWidth()
	{
		auto returnValue = Plugin::UnityEngineResolutionPropertyGetWidth(this);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnValue;
	}
	
	void Resolution::SetWidth(int32_t value)
	{
		Plugin::UnityEngineResolutionPropertySetWidth(this, value);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
	}
	
	int32_t Resolution::GetHeight()
	{
		auto returnValue = Plugin::UnityEngineResolutionPropertyGetHeight(this);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnValue;
	}
	
	void Resolution::SetHeight(int32_t value)
	{
		Plugin::UnityEngineResolutionPropertySetHeight(this, value);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
	}
	
	int32_t Resolution::GetRefreshRate()
	{
		auto returnValue = Plugin::UnityEngineResolutionPropertyGetRefreshRate(this);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnValue;
	}
	
	void Resolution::SetRefreshRate(int32_t value)
	{
		Plugin::UnityEngineResolutionPropertySetRefreshRate(this, value);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
	}
}

namespace System
{
	Object::Object(UnityEngine::Resolution& val)
	{
		int32_t handle = Plugin::BoxResolution(val);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
			Handle = handle;
		}
	}
	
	Object::operator UnityEngine::Resolution()
	{
		UnityEngine::Resolution returnVal(Plugin::UnboxResolution(Handle));
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnVal;
	}
}

namespace UnityEngine
{
	Screen::Screen(decltype(nullptr) n)
		: Screen(Plugin::InternalUse::Only, 0)
	{
	}
	
	Screen::Screen(Plugin::InternalUse iu, int32_t handle)
		: System::Object(iu, handle)
	{
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
	}
	
	Screen::Screen(const Screen& other)
		: Screen(Plugin::InternalUse::Only, other.Handle)
	{
	}
	
	Screen::Screen(Screen&& other)
		: Screen(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
	}
	
	Screen::~Screen()
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
	}
	
	Screen& Screen::operator=(const Screen& other)
	{
		if (this->Handle)
		{
			Plugin::DereferenceManagedClass(this->Handle);
		}
		this->Handle = other.Handle;
		if (this->Handle)
		{
			Plugin::ReferenceManagedClass(this->Handle);
		}
		return *this;
	}
	
	Screen& Screen::operator=(decltype(nullptr) other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
		return *this;
	}
	
	Screen& Screen::operator=(Screen&& other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
		}
		Handle = other.Handle;
		other.Handle = 0;
		return *this;
	}
	
	bool Screen::operator==(const Screen& other) const
	{
		return Handle == other.Handle;
	}
	
	bool Screen::operator!=(const Screen& other) const
	{
		return Handle != other.Handle;
	}
	
	System::Array1<UnityEngine::Resolution> Screen::GetResolutions()
	{
		auto returnValue = Plugin::UnityEngineScreenPropertyGetResolutions();
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return System::Array1<UnityEngine::Resolution>(Plugin::InternalUse::Only, returnValue);
	}
}

namespace UnityEngine
{
	Ray::Ray()
	{
	}
	
	Ray::Ray(UnityEngine::Vector3& origin, UnityEngine::Vector3& direction)
	{
		auto returnValue = Plugin::UnityEngineRayConstructorUnityEngineVector3_UnityEngineVector3(origin, direction);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		*this = returnValue;
	}
}

namespace System
{
	Object::Object(UnityEngine::Ray& val)
	{
		int32_t handle = Plugin::BoxRay(val);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
			Handle = handle;
		}
	}
	
	Object::operator UnityEngine::Ray()
	{
		UnityEngine::Ray returnVal(Plugin::UnboxRay(Handle));
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnVal;
	}
}

namespace UnityEngine
{
	Physics::Physics(decltype(nullptr) n)
		: Physics(Plugin::InternalUse::Only, 0)
	{
	}
	
	Physics::Physics(Plugin::InternalUse iu, int32_t handle)
		: System::Object(iu, handle)
	{
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
	}
	
	Physics::Physics(const Physics& other)
		: Physics(Plugin::InternalUse::Only, other.Handle)
	{
	}
	
	Physics::Physics(Physics&& other)
		: Physics(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
	}
	
	Physics::~Physics()
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
	}
	
	Physics& Physics::operator=(const Physics& other)
	{
		if (this->Handle)
		{
			Plugin::DereferenceManagedClass(this->Handle);
		}
		this->Handle = other.Handle;
		if (this->Handle)
		{
			Plugin::ReferenceManagedClass(this->Handle);
		}
		return *this;
	}
	
	Physics& Physics::operator=(decltype(nullptr) other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
		return *this;
	}
	
	Physics& Physics::operator=(Physics&& other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
		}
		Handle = other.Handle;
		other.Handle = 0;
		return *this;
	}
	
	bool Physics::operator==(const Physics& other) const
	{
		return Handle == other.Handle;
	}
	
	bool Physics::operator!=(const Physics& other) const
	{
		return Handle != other.Handle;
	}
	
	int32_t Physics::RaycastNonAlloc(UnityEngine::Ray& ray, System::Array1<UnityEngine::RaycastHit> results)
	{
		auto returnValue = Plugin::UnityEnginePhysicsMethodRaycastNonAllocUnityEngineRay_UnityEngineRaycastHit(ray, results.Handle);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnValue;
	}
	
	System::Array1<UnityEngine::RaycastHit> Physics::RaycastAll(UnityEngine::Ray& ray)
	{
		auto returnValue = Plugin::UnityEnginePhysicsMethodRaycastAllUnityEngineRay(ray);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return System::Array1<UnityEngine::RaycastHit>(Plugin::InternalUse::Only, returnValue);
	}
}

namespace UnityEngine
{
	Color::Color()
	{
	}
}

namespace System
{
	Object::Object(UnityEngine::Color& val)
	{
		int32_t handle = Plugin::BoxColor(val);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
			Handle = handle;
		}
	}
	
	Object::operator UnityEngine::Color()
	{
		UnityEngine::Color returnVal(Plugin::UnboxColor(Handle));
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnVal;
	}
}

namespace UnityEngine
{
	GradientColorKey::GradientColorKey()
	{
	}
}

namespace System
{
	Object::Object(UnityEngine::GradientColorKey& val)
	{
		int32_t handle = Plugin::BoxGradientColorKey(val);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
			Handle = handle;
		}
	}
	
	Object::operator UnityEngine::GradientColorKey()
	{
		UnityEngine::GradientColorKey returnVal(Plugin::UnboxGradientColorKey(Handle));
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnVal;
	}
}

namespace UnityEngine
{
	Gradient::Gradient(decltype(nullptr) n)
		: Gradient(Plugin::InternalUse::Only, 0)
	{
	}
	
	Gradient::Gradient(Plugin::InternalUse iu, int32_t handle)
		: System::Object(iu, handle)
	{
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
	}
	
	Gradient::Gradient(const Gradient& other)
		: Gradient(Plugin::InternalUse::Only, other.Handle)
	{
	}
	
	Gradient::Gradient(Gradient&& other)
		: Gradient(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
	}
	
	Gradient::~Gradient()
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
	}
	
	Gradient& Gradient::operator=(const Gradient& other)
	{
		if (this->Handle)
		{
			Plugin::DereferenceManagedClass(this->Handle);
		}
		this->Handle = other.Handle;
		if (this->Handle)
		{
			Plugin::ReferenceManagedClass(this->Handle);
		}
		return *this;
	}
	
	Gradient& Gradient::operator=(decltype(nullptr) other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
		return *this;
	}
	
	Gradient& Gradient::operator=(Gradient&& other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
		}
		Handle = other.Handle;
		other.Handle = 0;
		return *this;
	}
	
	bool Gradient::operator==(const Gradient& other) const
	{
		return Handle == other.Handle;
	}
	
	bool Gradient::operator!=(const Gradient& other) const
	{
		return Handle != other.Handle;
	}
	
	Gradient::Gradient()
		 : System::Object(nullptr)
	{
		auto returnValue = Plugin::UnityEngineGradientConstructor();
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		Handle = returnValue;
		if (returnValue)
		{
			Plugin::ReferenceManagedClass(returnValue);
		}
	}
	
	System::Array1<UnityEngine::GradientColorKey> Gradient::GetColorKeys()
	{
		auto returnValue = Plugin::UnityEngineGradientPropertyGetColorKeys(Handle);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return System::Array1<UnityEngine::GradientColorKey>(Plugin::InternalUse::Only, returnValue);
	}
	
	void Gradient::SetColorKeys(System::Array1<UnityEngine::GradientColorKey> value)
	{
		Plugin::UnityEngineGradientPropertySetColorKeys(Handle, value.Handle);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
	}
}

namespace System
{
	AppDomainSetup::AppDomainSetup(decltype(nullptr) n)
		: AppDomainSetup(Plugin::InternalUse::Only, 0)
	{
	}
	
	AppDomainSetup::AppDomainSetup(Plugin::InternalUse iu, int32_t handle)
		: System::Object(iu, handle)
	{
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
	}
	
	AppDomainSetup::AppDomainSetup(const AppDomainSetup& other)
		: AppDomainSetup(Plugin::InternalUse::Only, other.Handle)
	{
	}
	
	AppDomainSetup::AppDomainSetup(AppDomainSetup&& other)
		: AppDomainSetup(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
	}
	
	AppDomainSetup::~AppDomainSetup()
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
	}
	
	AppDomainSetup& AppDomainSetup::operator=(const AppDomainSetup& other)
	{
		if (this->Handle)
		{
			Plugin::DereferenceManagedClass(this->Handle);
		}
		this->Handle = other.Handle;
		if (this->Handle)
		{
			Plugin::ReferenceManagedClass(this->Handle);
		}
		return *this;
	}
	
	AppDomainSetup& AppDomainSetup::operator=(decltype(nullptr) other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
		return *this;
	}
	
	AppDomainSetup& AppDomainSetup::operator=(AppDomainSetup&& other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
		}
		Handle = other.Handle;
		other.Handle = 0;
		return *this;
	}
	
	bool AppDomainSetup::operator==(const AppDomainSetup& other) const
	{
		return Handle == other.Handle;
	}
	
	bool AppDomainSetup::operator!=(const AppDomainSetup& other) const
	{
		return Handle != other.Handle;
	}
	
	AppDomainSetup::AppDomainSetup()
		 : System::Object(nullptr)
	{
		auto returnValue = Plugin::SystemAppDomainSetupConstructor();
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		Handle = returnValue;
		if (returnValue)
		{
			Plugin::ReferenceManagedClass(returnValue);
		}
	}
	
	System::AppDomainInitializer AppDomainSetup::GetAppDomainInitializer()
	{
		auto returnValue = Plugin::SystemAppDomainSetupPropertyGetAppDomainInitializer(Handle);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return System::AppDomainInitializer(Plugin::InternalUse::Only, returnValue);
	}
	
	void AppDomainSetup::SetAppDomainInitializer(System::AppDomainInitializer value)
	{
		Plugin::SystemAppDomainSetupPropertySetAppDomainInitializer(Handle, value.Handle);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
	}
}

namespace UnityEngine
{
	Application::Application(decltype(nullptr) n)
		: Application(Plugin::InternalUse::Only, 0)
	{
	}
	
	Application::Application(Plugin::InternalUse iu, int32_t handle)
		: System::Object(iu, handle)
	{
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
	}
	
	Application::Application(const Application& other)
		: Application(Plugin::InternalUse::Only, other.Handle)
	{
	}
	
	Application::Application(Application&& other)
		: Application(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
	}
	
	Application::~Application()
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
	}
	
	Application& Application::operator=(const Application& other)
	{
		if (this->Handle)
		{
			Plugin::DereferenceManagedClass(this->Handle);
		}
		this->Handle = other.Handle;
		if (this->Handle)
		{
			Plugin::ReferenceManagedClass(this->Handle);
		}
		return *this;
	}
	
	Application& Application::operator=(decltype(nullptr) other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
		return *this;
	}
	
	Application& Application::operator=(Application&& other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
		}
		Handle = other.Handle;
		other.Handle = 0;
		return *this;
	}
	
	bool Application::operator==(const Application& other) const
	{
		return Handle == other.Handle;
	}
	
	bool Application::operator!=(const Application& other) const
	{
		return Handle != other.Handle;
	}
	
	void Application::AddOnBeforeRender(UnityEngine::Events::UnityAction del)
	{
		Plugin::UnityEngineApplicationAddEventOnBeforeRender(del.Handle);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
	}
	
	void Application::RemoveOnBeforeRender(UnityEngine::Events::UnityAction del)
	{
		Plugin::UnityEngineApplicationRemoveEventOnBeforeRender(del.Handle);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
	}
}

namespace UnityEngine
{
	namespace SceneManagement
	{
		SceneManager::SceneManager(decltype(nullptr) n)
			: SceneManager(Plugin::InternalUse::Only, 0)
		{
		}
		
		SceneManager::SceneManager(Plugin::InternalUse iu, int32_t handle)
			: System::Object(iu, handle)
		{
			if (handle)
			{
				Plugin::ReferenceManagedClass(handle);
			}
		}
		
		SceneManager::SceneManager(const SceneManager& other)
			: SceneManager(Plugin::InternalUse::Only, other.Handle)
		{
		}
		
		SceneManager::SceneManager(SceneManager&& other)
			: SceneManager(Plugin::InternalUse::Only, other.Handle)
		{
			other.Handle = 0;
		}
		
		SceneManager::~SceneManager()
		{
			if (Handle)
			{
				Plugin::DereferenceManagedClass(Handle);
				Handle = 0;
			}
		}
		
		SceneManager& SceneManager::operator=(const SceneManager& other)
		{
			if (this->Handle)
			{
				Plugin::DereferenceManagedClass(this->Handle);
			}
			this->Handle = other.Handle;
			if (this->Handle)
			{
				Plugin::ReferenceManagedClass(this->Handle);
			}
			return *this;
		}
		
		SceneManager& SceneManager::operator=(decltype(nullptr) other)
		{
			if (Handle)
			{
				Plugin::DereferenceManagedClass(Handle);
				Handle = 0;
			}
			return *this;
		}
		
		SceneManager& SceneManager::operator=(SceneManager&& other)
		{
			if (Handle)
			{
				Plugin::DereferenceManagedClass(Handle);
			}
			Handle = other.Handle;
			other.Handle = 0;
			return *this;
		}
		
		bool SceneManager::operator==(const SceneManager& other) const
		{
			return Handle == other.Handle;
		}
		
		bool SceneManager::operator!=(const SceneManager& other) const
		{
			return Handle != other.Handle;
		}
		
		void SceneManager::AddSceneLoaded(UnityEngine::Events::UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode> del)
		{
			Plugin::UnityEngineSceneManagementSceneManagerAddEventSceneLoaded(del.Handle);
			if (Plugin::unhandledCsharpException)
			{
				System::Exception* ex = Plugin::unhandledCsharpException;
				Plugin::unhandledCsharpException = nullptr;
				ex->ThrowReferenceToThis();
				delete ex;
			}
		}
	
		void SceneManager::RemoveSceneLoaded(UnityEngine::Events::UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode> del)
		{
			Plugin::UnityEngineSceneManagementSceneManagerRemoveEventSceneLoaded(del.Handle);
			if (Plugin::unhandledCsharpException)
			{
				System::Exception* ex = Plugin::unhandledCsharpException;
				Plugin::unhandledCsharpException = nullptr;
				ex->ThrowReferenceToThis();
				delete ex;
			}
		}
	}
}

namespace UnityEngine
{
	namespace SceneManagement
	{
		Scene::Scene()
		{
		}
	}
}

namespace System
{
	Object::Object(UnityEngine::SceneManagement::Scene& val)
	{
		int32_t handle = Plugin::BoxScene(val);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
			Handle = handle;
		}
	}
	
	Object::operator UnityEngine::SceneManagement::Scene()
	{
		UnityEngine::SceneManagement::Scene returnVal(Plugin::UnboxScene(Handle));
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnVal;
	}
}

namespace System
{
	Object::Object(UnityEngine::SceneManagement::LoadSceneMode val)
	{
		int32_t handle = Plugin::BoxLoadSceneMode(val);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
			Handle = handle;
		}
	}
	
	Object::operator UnityEngine::SceneManagement::LoadSceneMode()
	{
		UnityEngine::SceneManagement::LoadSceneMode returnVal(Plugin::UnboxLoadSceneMode(Handle));
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnVal;
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			IComparer<int32_t>::IComparer()
				 : System::Object(nullptr)
			{
				CppHandle = Plugin::StoreSystemCollectionsGenericIComparerSystemInt32(this);
				Plugin::SystemCollectionsGenericIComparerSystemInt32Constructor(CppHandle, &Handle);
				if (Handle)
				{
					Plugin::ReferenceManagedClass(Handle);
				}
				else
				{
					Plugin::RemoveSystemCollectionsGenericIComparerSystemInt32(CppHandle);
					CppHandle = 0;
				}
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
			}
			
			IComparer<int32_t>::IComparer(decltype(nullptr) n)
				: System::Object(Plugin::InternalUse::Only, 0)
			{
				CppHandle = Plugin::StoreSystemCollectionsGenericIComparerSystemInt32(this);
			}
			
			IComparer<int32_t>::IComparer(const IComparer<int32_t>& other)
				: System::Object(Plugin::InternalUse::Only, other.Handle)
			{
				CppHandle = Plugin::StoreSystemCollectionsGenericIComparerSystemInt32(this);
				if (Handle)
				{
					Plugin::ReferenceManagedClass(Handle);
				}
			}
			
			IComparer<int32_t>::IComparer(IComparer<int32_t>&& other)
				: System::Object(Plugin::InternalUse::Only, other.Handle)
			{
				CppHandle = other.CppHandle;
				other.Handle = 0;
				other.CppHandle = 0;
			}
			
			IComparer<int32_t>::IComparer(Plugin::InternalUse iu, int32_t handle)
				: System::Object(iu, handle)
			{
				CppHandle = Plugin::StoreSystemCollectionsGenericIComparerSystemInt32(this);
				if (Handle)
				{
					Plugin::ReferenceManagedClass(Handle);
				}
			}
			
			IComparer<int32_t>::~IComparer()
			{
				Plugin::RemoveSystemCollectionsGenericIComparerSystemInt32(CppHandle);
				CppHandle = 0;
				if (Handle)
				{
					int32_t handle = Handle;
					Handle = 0;
					if (Plugin::DereferenceManagedClassNoRelease(handle))
					{
						Plugin::ReleaseSystemCollectionsGenericIComparerSystemInt32(handle);
						if (Plugin::unhandledCsharpException)
						{
							System::Exception* ex = Plugin::unhandledCsharpException;
							Plugin::unhandledCsharpException = nullptr;
							ex->ThrowReferenceToThis();
							delete ex;
						}
					}
				}
			}
			
			IComparer<int32_t>& IComparer<int32_t>::operator=(const IComparer<int32_t>& other)
			{
				if (this->Handle)
				{
					Plugin::DereferenceManagedClass(this->Handle);
				}
				this->Handle = other.Handle;
				if (this->Handle)
				{
					Plugin::ReferenceManagedClass(this->Handle);
				}
				return *this;
			}
			
			IComparer<int32_t>& IComparer<int32_t>::operator=(decltype(nullptr) other)
			{
				if (Handle)
				{
					int32_t handle = Handle;
					Handle = 0;
					if (Plugin::DereferenceManagedClassNoRelease(handle))
					{
						Plugin::ReleaseSystemCollectionsGenericIComparerSystemInt32(handle);
						if (Plugin::unhandledCsharpException)
						{
							System::Exception* ex = Plugin::unhandledCsharpException;
							Plugin::unhandledCsharpException = nullptr;
							ex->ThrowReferenceToThis();
							delete ex;
						}
					}
				}
				Handle = 0;
				return *this;
			}
			
			IComparer<int32_t>& IComparer<int32_t>::operator=(IComparer<int32_t>&& other)
			{
				Plugin::RemoveSystemCollectionsGenericIComparerSystemInt32(CppHandle);
				CppHandle = 0;
				if (Handle)
				{
					int32_t handle = Handle;
					Handle = 0;
					if (Plugin::DereferenceManagedClassNoRelease(handle))
					{
						Plugin::ReleaseSystemCollectionsGenericIComparerSystemInt32(handle);
						if (Plugin::unhandledCsharpException)
						{
							System::Exception* ex = Plugin::unhandledCsharpException;
							Plugin::unhandledCsharpException = nullptr;
							ex->ThrowReferenceToThis();
							delete ex;
						}
					}
				}
				Handle = other.Handle;
				other.Handle = 0;
				return *this;
			}
			
			bool IComparer<int32_t>::operator==(const IComparer<int32_t>& other) const
			{
				return Handle == other.Handle;
			}
			
			bool IComparer<int32_t>::operator!=(const IComparer<int32_t>& other) const
			{
				return Handle != other.Handle;
			}
			
			int32_t IComparer<int32_t>::Compare(int32_t x, int32_t y)
			{
				return {};
			}
			
			DLLEXPORT int32_t SystemCollectionsGenericIComparerSystemInt32Compare(int32_t cppHandle, int32_t x, int32_t y)
			{
				try
				{
					return Plugin::GetSystemCollectionsGenericIComparerSystemInt32(cppHandle)->Compare(x, y);
				}
				catch (System::Exception ex)
				{
					Plugin::SetException(ex.Handle);
					return {};
				}
				catch (...)
				{
					System::String msg = "Unhandled exception invoking System::Collections::Generic::IComparer<int32_t>";
					System::Exception ex(msg);
					Plugin::SetException(ex.Handle);
					return {};
				}
			}
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			IComparer<System::String>::IComparer()
				 : System::Object(nullptr)
			{
				CppHandle = Plugin::StoreSystemCollectionsGenericIComparerSystemString(this);
				Plugin::SystemCollectionsGenericIComparerSystemStringConstructor(CppHandle, &Handle);
				if (Handle)
				{
					Plugin::ReferenceManagedClass(Handle);
				}
				else
				{
					Plugin::RemoveSystemCollectionsGenericIComparerSystemString(CppHandle);
					CppHandle = 0;
				}
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
			}
			
			IComparer<System::String>::IComparer(decltype(nullptr) n)
				: System::Object(Plugin::InternalUse::Only, 0)
			{
				CppHandle = Plugin::StoreSystemCollectionsGenericIComparerSystemString(this);
			}
			
			IComparer<System::String>::IComparer(const IComparer<System::String>& other)
				: System::Object(Plugin::InternalUse::Only, other.Handle)
			{
				CppHandle = Plugin::StoreSystemCollectionsGenericIComparerSystemString(this);
				if (Handle)
				{
					Plugin::ReferenceManagedClass(Handle);
				}
			}
			
			IComparer<System::String>::IComparer(IComparer<System::String>&& other)
				: System::Object(Plugin::InternalUse::Only, other.Handle)
			{
				CppHandle = other.CppHandle;
				other.Handle = 0;
				other.CppHandle = 0;
			}
			
			IComparer<System::String>::IComparer(Plugin::InternalUse iu, int32_t handle)
				: System::Object(iu, handle)
			{
				CppHandle = Plugin::StoreSystemCollectionsGenericIComparerSystemString(this);
				if (Handle)
				{
					Plugin::ReferenceManagedClass(Handle);
				}
			}
			
			IComparer<System::String>::~IComparer()
			{
				Plugin::RemoveSystemCollectionsGenericIComparerSystemString(CppHandle);
				CppHandle = 0;
				if (Handle)
				{
					int32_t handle = Handle;
					Handle = 0;
					if (Plugin::DereferenceManagedClassNoRelease(handle))
					{
						Plugin::ReleaseSystemCollectionsGenericIComparerSystemString(handle);
						if (Plugin::unhandledCsharpException)
						{
							System::Exception* ex = Plugin::unhandledCsharpException;
							Plugin::unhandledCsharpException = nullptr;
							ex->ThrowReferenceToThis();
							delete ex;
						}
					}
				}
			}
			
			IComparer<System::String>& IComparer<System::String>::operator=(const IComparer<System::String>& other)
			{
				if (this->Handle)
				{
					Plugin::DereferenceManagedClass(this->Handle);
				}
				this->Handle = other.Handle;
				if (this->Handle)
				{
					Plugin::ReferenceManagedClass(this->Handle);
				}
				return *this;
			}
			
			IComparer<System::String>& IComparer<System::String>::operator=(decltype(nullptr) other)
			{
				if (Handle)
				{
					int32_t handle = Handle;
					Handle = 0;
					if (Plugin::DereferenceManagedClassNoRelease(handle))
					{
						Plugin::ReleaseSystemCollectionsGenericIComparerSystemString(handle);
						if (Plugin::unhandledCsharpException)
						{
							System::Exception* ex = Plugin::unhandledCsharpException;
							Plugin::unhandledCsharpException = nullptr;
							ex->ThrowReferenceToThis();
							delete ex;
						}
					}
				}
				Handle = 0;
				return *this;
			}
			
			IComparer<System::String>& IComparer<System::String>::operator=(IComparer<System::String>&& other)
			{
				Plugin::RemoveSystemCollectionsGenericIComparerSystemString(CppHandle);
				CppHandle = 0;
				if (Handle)
				{
					int32_t handle = Handle;
					Handle = 0;
					if (Plugin::DereferenceManagedClassNoRelease(handle))
					{
						Plugin::ReleaseSystemCollectionsGenericIComparerSystemString(handle);
						if (Plugin::unhandledCsharpException)
						{
							System::Exception* ex = Plugin::unhandledCsharpException;
							Plugin::unhandledCsharpException = nullptr;
							ex->ThrowReferenceToThis();
							delete ex;
						}
					}
				}
				Handle = other.Handle;
				other.Handle = 0;
				return *this;
			}
			
			bool IComparer<System::String>::operator==(const IComparer<System::String>& other) const
			{
				return Handle == other.Handle;
			}
			
			bool IComparer<System::String>::operator!=(const IComparer<System::String>& other) const
			{
				return Handle != other.Handle;
			}
			
			int32_t IComparer<System::String>::Compare(System::String x, System::String y)
			{
				return {};
			}
			
			DLLEXPORT int32_t SystemCollectionsGenericIComparerSystemStringCompare(int32_t cppHandle, int32_t xHandle, int32_t yHandle)
			{
				try
				{
					return Plugin::GetSystemCollectionsGenericIComparerSystemString(cppHandle)->Compare(System::String(Plugin::InternalUse::Only, xHandle), System::String(Plugin::InternalUse::Only, yHandle));
				}
				catch (System::Exception ex)
				{
					Plugin::SetException(ex.Handle);
					return {};
				}
				catch (...)
				{
					System::String msg = "Unhandled exception invoking System::Collections::Generic::IComparer<System::String>";
					System::Exception ex(msg);
					Plugin::SetException(ex.Handle);
					return {};
				}
			}
		}
	}
}

namespace System
{
	StringComparer::StringComparer()
		 : System::Object(nullptr)
	{
		CppHandle = Plugin::StoreSystemStringComparer(this);
		Plugin::SystemStringComparerConstructor(CppHandle, &Handle);
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
		else
		{
			Plugin::RemoveSystemStringComparer(CppHandle);
			CppHandle = 0;
		}
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
	}
	
	StringComparer::StringComparer(decltype(nullptr) n)
		: System::Object(Plugin::InternalUse::Only, 0)
	{
		CppHandle = Plugin::StoreSystemStringComparer(this);
	}
	
	StringComparer::StringComparer(const StringComparer& other)
		: System::Object(Plugin::InternalUse::Only, other.Handle)
	{
		CppHandle = Plugin::StoreSystemStringComparer(this);
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
	}
	
	StringComparer::StringComparer(StringComparer&& other)
		: System::Object(Plugin::InternalUse::Only, other.Handle)
	{
		CppHandle = other.CppHandle;
		other.Handle = 0;
		other.CppHandle = 0;
	}
	
	StringComparer::StringComparer(Plugin::InternalUse iu, int32_t handle)
		: System::Object(iu, handle)
	{
		CppHandle = Plugin::StoreSystemStringComparer(this);
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
	}
	
	StringComparer::~StringComparer()
	{
		Plugin::RemoveSystemStringComparer(CppHandle);
		CppHandle = 0;
		if (Handle)
		{
			int32_t handle = Handle;
			Handle = 0;
			if (Plugin::DereferenceManagedClassNoRelease(handle))
			{
				Plugin::ReleaseSystemStringComparer(handle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
			}
		}
	}
	
	StringComparer& StringComparer::operator=(const StringComparer& other)
	{
		if (this->Handle)
		{
			Plugin::DereferenceManagedClass(this->Handle);
		}
		this->Handle = other.Handle;
		if (this->Handle)
		{
			Plugin::ReferenceManagedClass(this->Handle);
		}
		return *this;
	}
	
	StringComparer& StringComparer::operator=(decltype(nullptr) other)
	{
		if (Handle)
		{
			int32_t handle = Handle;
			Handle = 0;
			if (Plugin::DereferenceManagedClassNoRelease(handle))
			{
				Plugin::ReleaseSystemStringComparer(handle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
			}
		}
		Handle = 0;
		return *this;
	}
	
	StringComparer& StringComparer::operator=(StringComparer&& other)
	{
		Plugin::RemoveSystemStringComparer(CppHandle);
		CppHandle = 0;
		if (Handle)
		{
			int32_t handle = Handle;
			Handle = 0;
			if (Plugin::DereferenceManagedClassNoRelease(handle))
			{
				Plugin::ReleaseSystemStringComparer(handle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
			}
		}
		Handle = other.Handle;
		other.Handle = 0;
		return *this;
	}
	
	bool StringComparer::operator==(const StringComparer& other) const
	{
		return Handle == other.Handle;
	}
	
	bool StringComparer::operator!=(const StringComparer& other) const
	{
		return Handle != other.Handle;
	}
	
	int32_t StringComparer::Compare(System::String x, System::String y)
	{
		return {};
	}
	
	DLLEXPORT int32_t SystemStringComparerCompare(int32_t cppHandle, int32_t xHandle, int32_t yHandle)
	{
		try
		{
			return Plugin::GetSystemStringComparer(cppHandle)->Compare(System::String(Plugin::InternalUse::Only, xHandle), System::String(Plugin::InternalUse::Only, yHandle));
		}
		catch (System::Exception ex)
		{
			Plugin::SetException(ex.Handle);
			return {};
		}
		catch (...)
		{
			System::String msg = "Unhandled exception invoking System::StringComparer";
			System::Exception ex(msg);
			Plugin::SetException(ex.Handle);
			return {};
		}
	}
	
	System::Boolean StringComparer::Equals(System::String x, System::String y)
	{
		return {};
	}
	
	DLLEXPORT System::Boolean SystemStringComparerEquals(int32_t cppHandle, int32_t xHandle, int32_t yHandle)
	{
		try
		{
			return Plugin::GetSystemStringComparer(cppHandle)->Equals(System::String(Plugin::InternalUse::Only, xHandle), System::String(Plugin::InternalUse::Only, yHandle));
		}
		catch (System::Exception ex)
		{
			Plugin::SetException(ex.Handle);
			return {};
		}
		catch (...)
		{
			System::String msg = "Unhandled exception invoking System::StringComparer";
			System::Exception ex(msg);
			Plugin::SetException(ex.Handle);
			return {};
		}
	}
	
	int32_t StringComparer::GetHashCode(System::String obj)
	{
		return {};
	}
	
	DLLEXPORT int32_t SystemStringComparerGetHashCode(int32_t cppHandle, int32_t objHandle)
	{
		try
		{
			return Plugin::GetSystemStringComparer(cppHandle)->GetHashCode(System::String(Plugin::InternalUse::Only, objHandle));
		}
		catch (System::Exception ex)
		{
			Plugin::SetException(ex.Handle);
			return {};
		}
		catch (...)
		{
			System::String msg = "Unhandled exception invoking System::StringComparer";
			System::Exception ex(msg);
			Plugin::SetException(ex.Handle);
			return {};
		}
	}
}

namespace System
{
	EventArgs::EventArgs()
		 : System::Object(nullptr)
	{
		CppHandle = Plugin::StoreSystemEventArgs(this);
		Plugin::SystemEventArgsConstructor(CppHandle, &Handle);
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
		else
		{
			Plugin::RemoveSystemEventArgs(CppHandle);
			CppHandle = 0;
		}
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
	}
	
	EventArgs::EventArgs(decltype(nullptr) n)
		: System::Object(Plugin::InternalUse::Only, 0)
	{
		CppHandle = Plugin::StoreSystemEventArgs(this);
	}
	
	EventArgs::EventArgs(const EventArgs& other)
		: System::Object(Plugin::InternalUse::Only, other.Handle)
	{
		CppHandle = Plugin::StoreSystemEventArgs(this);
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
	}
	
	EventArgs::EventArgs(EventArgs&& other)
		: System::Object(Plugin::InternalUse::Only, other.Handle)
	{
		CppHandle = other.CppHandle;
		other.Handle = 0;
		other.CppHandle = 0;
	}
	
	EventArgs::EventArgs(Plugin::InternalUse iu, int32_t handle)
		: System::Object(iu, handle)
	{
		CppHandle = Plugin::StoreSystemEventArgs(this);
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
	}
	
	EventArgs::~EventArgs()
	{
		Plugin::RemoveSystemEventArgs(CppHandle);
		CppHandle = 0;
		if (Handle)
		{
			int32_t handle = Handle;
			Handle = 0;
			if (Plugin::DereferenceManagedClassNoRelease(handle))
			{
				Plugin::ReleaseSystemEventArgs(handle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
			}
		}
	}
	
	EventArgs& EventArgs::operator=(const EventArgs& other)
	{
		if (this->Handle)
		{
			Plugin::DereferenceManagedClass(this->Handle);
		}
		this->Handle = other.Handle;
		if (this->Handle)
		{
			Plugin::ReferenceManagedClass(this->Handle);
		}
		return *this;
	}
	
	EventArgs& EventArgs::operator=(decltype(nullptr) other)
	{
		if (Handle)
		{
			int32_t handle = Handle;
			Handle = 0;
			if (Plugin::DereferenceManagedClassNoRelease(handle))
			{
				Plugin::ReleaseSystemEventArgs(handle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
			}
		}
		Handle = 0;
		return *this;
	}
	
	EventArgs& EventArgs::operator=(EventArgs&& other)
	{
		Plugin::RemoveSystemEventArgs(CppHandle);
		CppHandle = 0;
		if (Handle)
		{
			int32_t handle = Handle;
			Handle = 0;
			if (Plugin::DereferenceManagedClassNoRelease(handle))
			{
				Plugin::ReleaseSystemEventArgs(handle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
			}
		}
		Handle = other.Handle;
		other.Handle = 0;
		return *this;
	}
	
	bool EventArgs::operator==(const EventArgs& other) const
	{
		return Handle == other.Handle;
	}
	
	bool EventArgs::operator!=(const EventArgs& other) const
	{
		return Handle != other.Handle;
	}
	
	System::String EventArgs::ToString()
	{
		return {};
	}
	
	DLLEXPORT int32_t SystemEventArgsToString(int32_t cppHandle)
	{
		try
		{
			return Plugin::GetSystemEventArgs(cppHandle)->ToString().Handle;
		}
		catch (System::Exception ex)
		{
			Plugin::SetException(ex.Handle);
			return {};
		}
		catch (...)
		{
			System::String msg = "Unhandled exception invoking System::EventArgs";
			System::Exception ex(msg);
			Plugin::SetException(ex.Handle);
			return {};
		}
	}
}

namespace System
{
	Object::Object(System::Boolean val)
	{
		int32_t handle = Plugin::BoxBoolean(val);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
			Handle = handle;
		}
	}
	
	Object::operator System::Boolean()
	{
		System::Boolean returnVal(Plugin::UnboxBoolean(Handle));
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnVal;
	}
}

namespace System
{
	Object::Object(int8_t val)
	{
		int32_t handle = Plugin::BoxSByte(val);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
			Handle = handle;
		}
	}
	
	Object::operator int8_t()
	{
		int8_t returnVal(Plugin::UnboxSByte(Handle));
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnVal;
	}
}

namespace System
{
	Object::Object(uint8_t val)
	{
		int32_t handle = Plugin::BoxByte(val);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
			Handle = handle;
		}
	}
	
	Object::operator uint8_t()
	{
		uint8_t returnVal(Plugin::UnboxByte(Handle));
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnVal;
	}
}

namespace System
{
	Object::Object(int16_t val)
	{
		int32_t handle = Plugin::BoxInt16(val);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
			Handle = handle;
		}
	}
	
	Object::operator int16_t()
	{
		int16_t returnVal(Plugin::UnboxInt16(Handle));
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnVal;
	}
}

namespace System
{
	Object::Object(uint16_t val)
	{
		int32_t handle = Plugin::BoxUInt16(val);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
			Handle = handle;
		}
	}
	
	Object::operator uint16_t()
	{
		uint16_t returnVal(Plugin::UnboxUInt16(Handle));
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnVal;
	}
}

namespace System
{
	Object::Object(int32_t val)
	{
		int32_t handle = Plugin::BoxInt32(val);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
			Handle = handle;
		}
	}
	
	Object::operator int32_t()
	{
		int32_t returnVal(Plugin::UnboxInt32(Handle));
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnVal;
	}
}

namespace System
{
	Object::Object(uint32_t val)
	{
		int32_t handle = Plugin::BoxUInt32(val);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
			Handle = handle;
		}
	}
	
	Object::operator uint32_t()
	{
		uint32_t returnVal(Plugin::UnboxUInt32(Handle));
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnVal;
	}
}

namespace System
{
	Object::Object(int64_t val)
	{
		int32_t handle = Plugin::BoxInt64(val);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
			Handle = handle;
		}
	}
	
	Object::operator int64_t()
	{
		int64_t returnVal(Plugin::UnboxInt64(Handle));
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnVal;
	}
}

namespace System
{
	Object::Object(uint64_t val)
	{
		int32_t handle = Plugin::BoxUInt64(val);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
			Handle = handle;
		}
	}
	
	Object::operator uint64_t()
	{
		uint64_t returnVal(Plugin::UnboxUInt64(Handle));
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnVal;
	}
}

namespace System
{
	Object::Object(System::Char val)
	{
		int32_t handle = Plugin::BoxChar(val);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
			Handle = handle;
		}
	}
	
	Object::operator System::Char()
	{
		System::Char returnVal(Plugin::UnboxChar(Handle));
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnVal;
	}
}

namespace System
{
	Object::Object(float val)
	{
		int32_t handle = Plugin::BoxSingle(val);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
			Handle = handle;
		}
	}
	
	Object::operator float()
	{
		float returnVal(Plugin::UnboxSingle(Handle));
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnVal;
	}
}

namespace System
{
	Object::Object(double val)
	{
		int32_t handle = Plugin::BoxDouble(val);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
			Handle = handle;
		}
	}
	
	Object::operator double()
	{
		double returnVal(Plugin::UnboxDouble(Handle));
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnVal;
	}
}

namespace MyGame
{
	namespace MonoBehaviours
	{
		TestScript::TestScript(decltype(nullptr) n)
			: TestScript(Plugin::InternalUse::Only, 0)
		{
		}
		
		TestScript::TestScript(Plugin::InternalUse iu, int32_t handle)
			: UnityEngine::MonoBehaviour(iu, handle)
		{
			if (handle)
			{
				Plugin::ReferenceManagedClass(handle);
			}
		}
		
		TestScript::TestScript(const TestScript& other)
			: TestScript(Plugin::InternalUse::Only, other.Handle)
		{
		}
		
		TestScript::TestScript(TestScript&& other)
			: TestScript(Plugin::InternalUse::Only, other.Handle)
		{
			other.Handle = 0;
		}
		
		TestScript::~TestScript()
		{
			if (Handle)
			{
				Plugin::DereferenceManagedClass(Handle);
				Handle = 0;
			}
		}
		
		TestScript& TestScript::operator=(const TestScript& other)
		{
			if (this->Handle)
			{
				Plugin::DereferenceManagedClass(this->Handle);
			}
			this->Handle = other.Handle;
			if (this->Handle)
			{
				Plugin::ReferenceManagedClass(this->Handle);
			}
			return *this;
		}
		
		TestScript& TestScript::operator=(decltype(nullptr) other)
		{
			if (Handle)
			{
				Plugin::DereferenceManagedClass(Handle);
				Handle = 0;
			}
			return *this;
		}
		
		TestScript& TestScript::operator=(TestScript&& other)
		{
			if (Handle)
			{
				Plugin::DereferenceManagedClass(Handle);
			}
			Handle = other.Handle;
			other.Handle = 0;
			return *this;
		}
		
		bool TestScript::operator==(const TestScript& other) const
		{
			return Handle == other.Handle;
		}
		
		bool TestScript::operator!=(const TestScript& other) const
		{
			return Handle != other.Handle;
		}
	}
}

namespace Plugin
{
	ArrayElementProxy1_1<int32_t>::ArrayElementProxy1_1(Plugin::InternalUse iu, int32_t handle, int32_t index0)
	{
		Handle = handle;
		Index0 = index0;
	}
	
	void ArrayElementProxy1_1<int32_t>::operator=(int32_t item)
	{
		Plugin::SystemInt32Array1SetItem1(Handle, Index0, item);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
	}
	
	ArrayElementProxy1_1<int32_t>::operator int32_t()
	{
		auto returnValue = Plugin::SystemInt32Array1GetItem1(Handle, Index0);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnValue;
	}
}

namespace System
{
	Array1<int32_t>::Array1(decltype(nullptr) n)
		: Array1(Plugin::InternalUse::Only, 0)
	{
		this->InternalLength = 0;
		this->InternalRank = 0;
	}
	
	Array1<int32_t>::Array1(Plugin::InternalUse iu, int32_t handle)
		: System::Array(iu, handle)
	{
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
		this->InternalLength = 0;
		this->InternalRank = 0;
	}
	
	Array1<int32_t>::Array1(const Array1<int32_t>& other)
		: Array1(Plugin::InternalUse::Only, other.Handle)
	{
		InternalLength = other.InternalLength;
		InternalRank = other.InternalRank;
	}
	
	Array1<int32_t>::Array1(Array1<int32_t>&& other)
		: Array1(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
		InternalLength = other.InternalLength;
		InternalRank = other.InternalRank;
		other.InternalLength = 0;
		other.InternalRank = 0;
	}
	
	Array1<int32_t>::~Array1<int32_t>()
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
	}
	
	Array1<int32_t>& Array1<int32_t>::operator=(const Array1<int32_t>& other)
	{
		if (this->Handle)
		{
			Plugin::DereferenceManagedClass(this->Handle);
		}
		this->Handle = other.Handle;
		if (this->Handle)
		{
			Plugin::ReferenceManagedClass(this->Handle);
		}
		InternalLength = other.InternalLength;
		InternalRank = other.InternalRank;
		return *this;
	}
	
	Array1<int32_t>& Array1<int32_t>::operator=(decltype(nullptr) other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
		return *this;
	}
	
	Array1<int32_t>& Array1<int32_t>::operator=(Array1<int32_t>&& other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
		}
		Handle = other.Handle;
		InternalLength = other.InternalLength;
		InternalRank = other.InternalRank;
		other.Handle = 0;
		other.InternalLength = 0;
		other.InternalRank = 0;
		return *this;
	}
	
	bool Array1<int32_t>::operator==(const Array1<int32_t>& other) const
	{
		return Handle == other.Handle;
	}
	
	bool Array1<int32_t>::operator!=(const Array1<int32_t>& other) const
	{
		return Handle != other.Handle;
	}
	
	Array1<int32_t>::Array1(int32_t length0)
		 : System::Array(nullptr)
	{
		auto returnValue = Plugin::SystemSystemInt32Array1Constructor1(length0);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		Handle = returnValue;
		if (returnValue)
		{
			Plugin::ReferenceManagedClass(returnValue);
			InternalLength = length0;
		}
	}
	
	int32_t Array1<int32_t>::GetLength()
	{
		int32_t returnVal = InternalLength;
		if (returnVal == 0)
		{
			returnVal = Array::GetLength();
			InternalLength = returnVal;
		};
		return returnVal;
	}
	
	int32_t Array1<int32_t>::GetRank()
	{
		int32_t returnVal = InternalRank;
		if (returnVal == 0)
		{
			returnVal = Array::GetRank();
			InternalRank = returnVal;
		};
		return returnVal;
	}
	
	Plugin::ArrayElementProxy1_1<int32_t> System::Array1<int32_t>::operator[](int32_t index)
	{
		return Plugin::ArrayElementProxy1_1<int32_t>(Plugin::InternalUse::Only, Handle, index);
	}
}

namespace Plugin
{
	ArrayElementProxy1_1<float>::ArrayElementProxy1_1(Plugin::InternalUse iu, int32_t handle, int32_t index0)
	{
		Handle = handle;
		Index0 = index0;
	}
	
	void ArrayElementProxy1_1<float>::operator=(float item)
	{
		Plugin::SystemSingleArray1SetItem1(Handle, Index0, item);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
	}
	
	ArrayElementProxy1_1<float>::operator float()
	{
		auto returnValue = Plugin::SystemSingleArray1GetItem1(Handle, Index0);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnValue;
	}
}

namespace Plugin
{
	ArrayElementProxy1_2<float>::ArrayElementProxy1_2(Plugin::InternalUse iu, int32_t handle, int32_t index0)
	{
		Handle = handle;
		Index0 = index0;
	}
	
	Plugin::ArrayElementProxy2_2<float> Plugin::ArrayElementProxy1_2<float>::operator[](int32_t index)
	{
		return Plugin::ArrayElementProxy2_2<float>(Plugin::InternalUse::Only, Handle, Index0, index);
	}
}

namespace Plugin
{
	ArrayElementProxy2_2<float>::ArrayElementProxy2_2(Plugin::InternalUse iu, int32_t handle, int32_t index0, int32_t index1)
	{
		Handle = handle;
		Index0 = index0;
		Index1 = index1;
	}
	
	void ArrayElementProxy2_2<float>::operator=(float item)
	{
		Plugin::SystemSingleArray2SetItem2(Handle, Index0, Index1, item);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
	}
	
	ArrayElementProxy2_2<float>::operator float()
	{
		auto returnValue = Plugin::SystemSingleArray2GetItem2(Handle, Index0, Index1);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnValue;
	}
}

namespace Plugin
{
	ArrayElementProxy1_3<float>::ArrayElementProxy1_3(Plugin::InternalUse iu, int32_t handle, int32_t index0)
	{
		Handle = handle;
		Index0 = index0;
	}
	
	Plugin::ArrayElementProxy2_3<float> Plugin::ArrayElementProxy1_3<float>::operator[](int32_t index)
	{
		return Plugin::ArrayElementProxy2_3<float>(Plugin::InternalUse::Only, Handle, Index0, index);
	}
}

namespace Plugin
{
	ArrayElementProxy2_3<float>::ArrayElementProxy2_3(Plugin::InternalUse iu, int32_t handle, int32_t index0, int32_t index1)
	{
		Handle = handle;
		Index0 = index0;
		Index1 = index1;
	}
	
	Plugin::ArrayElementProxy3_3<float> Plugin::ArrayElementProxy2_3<float>::operator[](int32_t index)
	{
		return Plugin::ArrayElementProxy3_3<float>(Plugin::InternalUse::Only, Handle, Index0, Index1, index);
	}
}

namespace Plugin
{
	ArrayElementProxy3_3<float>::ArrayElementProxy3_3(Plugin::InternalUse iu, int32_t handle, int32_t index0, int32_t index1, int32_t index2)
	{
		Handle = handle;
		Index0 = index0;
		Index1 = index1;
		Index2 = index2;
	}
	
	void ArrayElementProxy3_3<float>::operator=(float item)
	{
		Plugin::SystemSingleArray3SetItem3(Handle, Index0, Index1, Index2, item);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
	}
	
	ArrayElementProxy3_3<float>::operator float()
	{
		auto returnValue = Plugin::SystemSingleArray3GetItem3(Handle, Index0, Index1, Index2);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnValue;
	}
}

namespace System
{
	Array1<float>::Array1(decltype(nullptr) n)
		: Array1(Plugin::InternalUse::Only, 0)
	{
		this->InternalLength = 0;
		this->InternalRank = 0;
	}
	
	Array1<float>::Array1(Plugin::InternalUse iu, int32_t handle)
		: System::Array(iu, handle)
	{
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
		this->InternalLength = 0;
		this->InternalRank = 0;
	}
	
	Array1<float>::Array1(const Array1<float>& other)
		: Array1(Plugin::InternalUse::Only, other.Handle)
	{
		InternalLength = other.InternalLength;
		InternalRank = other.InternalRank;
	}
	
	Array1<float>::Array1(Array1<float>&& other)
		: Array1(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
		InternalLength = other.InternalLength;
		InternalRank = other.InternalRank;
		other.InternalLength = 0;
		other.InternalRank = 0;
	}
	
	Array1<float>::~Array1<float>()
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
	}
	
	Array1<float>& Array1<float>::operator=(const Array1<float>& other)
	{
		if (this->Handle)
		{
			Plugin::DereferenceManagedClass(this->Handle);
		}
		this->Handle = other.Handle;
		if (this->Handle)
		{
			Plugin::ReferenceManagedClass(this->Handle);
		}
		InternalLength = other.InternalLength;
		InternalRank = other.InternalRank;
		return *this;
	}
	
	Array1<float>& Array1<float>::operator=(decltype(nullptr) other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
		return *this;
	}
	
	Array1<float>& Array1<float>::operator=(Array1<float>&& other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
		}
		Handle = other.Handle;
		InternalLength = other.InternalLength;
		InternalRank = other.InternalRank;
		other.Handle = 0;
		other.InternalLength = 0;
		other.InternalRank = 0;
		return *this;
	}
	
	bool Array1<float>::operator==(const Array1<float>& other) const
	{
		return Handle == other.Handle;
	}
	
	bool Array1<float>::operator!=(const Array1<float>& other) const
	{
		return Handle != other.Handle;
	}
	
	Array1<float>::Array1(int32_t length0)
		 : System::Array(nullptr)
	{
		auto returnValue = Plugin::SystemSystemSingleArray1Constructor1(length0);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		Handle = returnValue;
		if (returnValue)
		{
			Plugin::ReferenceManagedClass(returnValue);
			InternalLength = length0;
		}
	}
	
	int32_t Array1<float>::GetLength()
	{
		int32_t returnVal = InternalLength;
		if (returnVal == 0)
		{
			returnVal = Array::GetLength();
			InternalLength = returnVal;
		};
		return returnVal;
	}
	
	int32_t Array1<float>::GetRank()
	{
		int32_t returnVal = InternalRank;
		if (returnVal == 0)
		{
			returnVal = Array::GetRank();
			InternalRank = returnVal;
		};
		return returnVal;
	}
	
	Plugin::ArrayElementProxy1_1<float> System::Array1<float>::operator[](int32_t index)
	{
		return Plugin::ArrayElementProxy1_1<float>(Plugin::InternalUse::Only, Handle, index);
	}
}

namespace System
{
	Array2<float>::Array2(decltype(nullptr) n)
		: Array2(Plugin::InternalUse::Only, 0)
	{
		this->InternalLength = 0;
		this->InternalRank = 0;
		this->InternalLengths[0] = 0;
		this->InternalLengths[1] = 0;
	}
	
	Array2<float>::Array2(Plugin::InternalUse iu, int32_t handle)
		: System::Array(iu, handle)
	{
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
		this->InternalLength = 0;
		this->InternalRank = 0;
		this->InternalLengths[0] = 0;
		this->InternalLengths[1] = 0;
	}
	
	Array2<float>::Array2(const Array2<float>& other)
		: Array2(Plugin::InternalUse::Only, other.Handle)
	{
		InternalLength = other.InternalLength;
		InternalRank = other.InternalRank;
		InternalLengths[0] = other.InternalLengths[0];
		InternalLengths[1] = other.InternalLengths[1];
	}
	
	Array2<float>::Array2(Array2<float>&& other)
		: Array2(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
		InternalLength = other.InternalLength;
		InternalRank = other.InternalRank;
		InternalLengths[0] = other.InternalLengths[0];
		InternalLengths[1] = other.InternalLengths[1];
		other.InternalLength = 0;
		other.InternalRank = 0;
		other.InternalLengths[0] = 0;
		other.InternalLengths[1] = 0;
	}
	
	Array2<float>::~Array2<float>()
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
	}
	
	Array2<float>& Array2<float>::operator=(const Array2<float>& other)
	{
		if (this->Handle)
		{
			Plugin::DereferenceManagedClass(this->Handle);
		}
		this->Handle = other.Handle;
		if (this->Handle)
		{
			Plugin::ReferenceManagedClass(this->Handle);
		}
		InternalLength = other.InternalLength;
		InternalRank = other.InternalRank;
		InternalLengths[0] = other.InternalLengths[0];
		InternalLengths[1] = other.InternalLengths[1];
		return *this;
	}
	
	Array2<float>& Array2<float>::operator=(decltype(nullptr) other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
		return *this;
	}
	
	Array2<float>& Array2<float>::operator=(Array2<float>&& other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
		}
		Handle = other.Handle;
		InternalLength = other.InternalLength;
		InternalRank = other.InternalRank;
		InternalLengths[0] = other.InternalLengths[0];
		InternalLengths[1] = other.InternalLengths[1];
		other.Handle = 0;
		other.InternalLength = 0;
		other.InternalRank = 0;
		other.InternalLengths[0] = 0;
		other.InternalLengths[1] = 0;
		return *this;
	}
	
	bool Array2<float>::operator==(const Array2<float>& other) const
	{
		return Handle == other.Handle;
	}
	
	bool Array2<float>::operator!=(const Array2<float>& other) const
	{
		return Handle != other.Handle;
	}
	
	Array2<float>::Array2(int32_t length0, int32_t length1)
		 : System::Array(nullptr)
	{
		auto returnValue = Plugin::SystemSystemSingleArray2Constructor2(length0, length1);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		Handle = returnValue;
		if (returnValue)
		{
			Plugin::ReferenceManagedClass(returnValue);
			InternalLength = length0 * length1;
			InternalLengths[0] = length0;
			InternalLengths[1] = length1;
		}
	}
	
	int32_t Array2<float>::GetLength()
	{
		int32_t returnVal = InternalLength;
		if (returnVal == 0)
		{
			returnVal = Array::GetLength();
			InternalLength = returnVal;
		};
		return returnVal;
	}
	
	int32_t Array2<float>::GetLength(int32_t dimension)
	{
		int32_t length = InternalLengths[dimension];
		if (length)
		{
			return length;
		}
		auto returnValue = Plugin::SystemSystemSingleArray2GetLength2(Handle, dimension);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		InternalLengths[dimension] = returnValue;
		return returnValue;
	}
	
	int32_t Array2<float>::GetRank()
	{
		int32_t returnVal = InternalRank;
		if (returnVal == 0)
		{
			returnVal = Array::GetRank();
			InternalRank = returnVal;
		};
		return returnVal;
	}
	
	Plugin::ArrayElementProxy1_2<float> System::Array2<float>::operator[](int32_t index)
	{
		return Plugin::ArrayElementProxy1_2<float>(Plugin::InternalUse::Only, Handle, index);
	}
}

namespace System
{
	Array3<float>::Array3(decltype(nullptr) n)
		: Array3(Plugin::InternalUse::Only, 0)
	{
		this->InternalLength = 0;
		this->InternalRank = 0;
		this->InternalLengths[0] = 0;
		this->InternalLengths[1] = 0;
		this->InternalLengths[2] = 0;
	}
	
	Array3<float>::Array3(Plugin::InternalUse iu, int32_t handle)
		: System::Array(iu, handle)
	{
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
		this->InternalLength = 0;
		this->InternalRank = 0;
		this->InternalLengths[0] = 0;
		this->InternalLengths[1] = 0;
		this->InternalLengths[2] = 0;
	}
	
	Array3<float>::Array3(const Array3<float>& other)
		: Array3(Plugin::InternalUse::Only, other.Handle)
	{
		InternalLength = other.InternalLength;
		InternalRank = other.InternalRank;
		InternalLengths[0] = other.InternalLengths[0];
		InternalLengths[1] = other.InternalLengths[1];
		InternalLengths[2] = other.InternalLengths[2];
	}
	
	Array3<float>::Array3(Array3<float>&& other)
		: Array3(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
		InternalLength = other.InternalLength;
		InternalRank = other.InternalRank;
		InternalLengths[0] = other.InternalLengths[0];
		InternalLengths[1] = other.InternalLengths[1];
		InternalLengths[2] = other.InternalLengths[2];
		other.InternalLength = 0;
		other.InternalRank = 0;
		other.InternalLengths[0] = 0;
		other.InternalLengths[1] = 0;
		other.InternalLengths[2] = 0;
	}
	
	Array3<float>::~Array3<float>()
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
	}
	
	Array3<float>& Array3<float>::operator=(const Array3<float>& other)
	{
		if (this->Handle)
		{
			Plugin::DereferenceManagedClass(this->Handle);
		}
		this->Handle = other.Handle;
		if (this->Handle)
		{
			Plugin::ReferenceManagedClass(this->Handle);
		}
		InternalLength = other.InternalLength;
		InternalRank = other.InternalRank;
		InternalLengths[0] = other.InternalLengths[0];
		InternalLengths[1] = other.InternalLengths[1];
		InternalLengths[2] = other.InternalLengths[2];
		return *this;
	}
	
	Array3<float>& Array3<float>::operator=(decltype(nullptr) other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
		return *this;
	}
	
	Array3<float>& Array3<float>::operator=(Array3<float>&& other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
		}
		Handle = other.Handle;
		InternalLength = other.InternalLength;
		InternalRank = other.InternalRank;
		InternalLengths[0] = other.InternalLengths[0];
		InternalLengths[1] = other.InternalLengths[1];
		InternalLengths[2] = other.InternalLengths[2];
		other.Handle = 0;
		other.InternalLength = 0;
		other.InternalRank = 0;
		other.InternalLengths[0] = 0;
		other.InternalLengths[1] = 0;
		other.InternalLengths[2] = 0;
		return *this;
	}
	
	bool Array3<float>::operator==(const Array3<float>& other) const
	{
		return Handle == other.Handle;
	}
	
	bool Array3<float>::operator!=(const Array3<float>& other) const
	{
		return Handle != other.Handle;
	}
	
	Array3<float>::Array3(int32_t length0, int32_t length1, int32_t length2)
		 : System::Array(nullptr)
	{
		auto returnValue = Plugin::SystemSystemSingleArray3Constructor3(length0, length1, length2);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		Handle = returnValue;
		if (returnValue)
		{
			Plugin::ReferenceManagedClass(returnValue);
			InternalLength = length0 * length1 * length2;
			InternalLengths[0] = length0;
			InternalLengths[1] = length1;
			InternalLengths[2] = length2;
		}
	}
	
	int32_t Array3<float>::GetLength()
	{
		int32_t returnVal = InternalLength;
		if (returnVal == 0)
		{
			returnVal = Array::GetLength();
			InternalLength = returnVal;
		};
		return returnVal;
	}
	
	int32_t Array3<float>::GetLength(int32_t dimension)
	{
		int32_t length = InternalLengths[dimension];
		if (length)
		{
			return length;
		}
		auto returnValue = Plugin::SystemSystemSingleArray3GetLength3(Handle, dimension);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		InternalLengths[dimension] = returnValue;
		return returnValue;
	}
	
	int32_t Array3<float>::GetRank()
	{
		int32_t returnVal = InternalRank;
		if (returnVal == 0)
		{
			returnVal = Array::GetRank();
			InternalRank = returnVal;
		};
		return returnVal;
	}
	
	Plugin::ArrayElementProxy1_3<float> System::Array3<float>::operator[](int32_t index)
	{
		return Plugin::ArrayElementProxy1_3<float>(Plugin::InternalUse::Only, Handle, index);
	}
}

namespace Plugin
{
	ArrayElementProxy1_1<System::String>::ArrayElementProxy1_1(Plugin::InternalUse iu, int32_t handle, int32_t index0)
	{
		Handle = handle;
		Index0 = index0;
	}
	
	void ArrayElementProxy1_1<System::String>::operator=(System::String item)
	{
		Plugin::SystemStringArray1SetItem1(Handle, Index0, item.Handle);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
	}
	
	ArrayElementProxy1_1<System::String>::operator System::String()
	{
		auto returnValue = Plugin::SystemStringArray1GetItem1(Handle, Index0);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return System::String(Plugin::InternalUse::Only, returnValue);
	}
}

namespace System
{
	Array1<System::String>::Array1(decltype(nullptr) n)
		: Array1(Plugin::InternalUse::Only, 0)
	{
		this->InternalLength = 0;
		this->InternalRank = 0;
	}
	
	Array1<System::String>::Array1(Plugin::InternalUse iu, int32_t handle)
		: System::Array(iu, handle)
	{
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
		this->InternalLength = 0;
		this->InternalRank = 0;
	}
	
	Array1<System::String>::Array1(const Array1<System::String>& other)
		: Array1(Plugin::InternalUse::Only, other.Handle)
	{
		InternalLength = other.InternalLength;
		InternalRank = other.InternalRank;
	}
	
	Array1<System::String>::Array1(Array1<System::String>&& other)
		: Array1(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
		InternalLength = other.InternalLength;
		InternalRank = other.InternalRank;
		other.InternalLength = 0;
		other.InternalRank = 0;
	}
	
	Array1<System::String>::~Array1<System::String>()
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
	}
	
	Array1<System::String>& Array1<System::String>::operator=(const Array1<System::String>& other)
	{
		if (this->Handle)
		{
			Plugin::DereferenceManagedClass(this->Handle);
		}
		this->Handle = other.Handle;
		if (this->Handle)
		{
			Plugin::ReferenceManagedClass(this->Handle);
		}
		InternalLength = other.InternalLength;
		InternalRank = other.InternalRank;
		return *this;
	}
	
	Array1<System::String>& Array1<System::String>::operator=(decltype(nullptr) other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
		return *this;
	}
	
	Array1<System::String>& Array1<System::String>::operator=(Array1<System::String>&& other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
		}
		Handle = other.Handle;
		InternalLength = other.InternalLength;
		InternalRank = other.InternalRank;
		other.Handle = 0;
		other.InternalLength = 0;
		other.InternalRank = 0;
		return *this;
	}
	
	bool Array1<System::String>::operator==(const Array1<System::String>& other) const
	{
		return Handle == other.Handle;
	}
	
	bool Array1<System::String>::operator!=(const Array1<System::String>& other) const
	{
		return Handle != other.Handle;
	}
	
	Array1<System::String>::Array1(int32_t length0)
		 : System::Array(nullptr)
	{
		auto returnValue = Plugin::SystemSystemStringArray1Constructor1(length0);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		Handle = returnValue;
		if (returnValue)
		{
			Plugin::ReferenceManagedClass(returnValue);
			InternalLength = length0;
		}
	}
	
	int32_t Array1<System::String>::GetLength()
	{
		int32_t returnVal = InternalLength;
		if (returnVal == 0)
		{
			returnVal = Array::GetLength();
			InternalLength = returnVal;
		};
		return returnVal;
	}
	
	int32_t Array1<System::String>::GetRank()
	{
		int32_t returnVal = InternalRank;
		if (returnVal == 0)
		{
			returnVal = Array::GetRank();
			InternalRank = returnVal;
		};
		return returnVal;
	}
	
	Plugin::ArrayElementProxy1_1<System::String> System::Array1<System::String>::operator[](int32_t index)
	{
		return Plugin::ArrayElementProxy1_1<System::String>(Plugin::InternalUse::Only, Handle, index);
	}
}

namespace Plugin
{
	ArrayElementProxy1_1<UnityEngine::Resolution>::ArrayElementProxy1_1(Plugin::InternalUse iu, int32_t handle, int32_t index0)
	{
		Handle = handle;
		Index0 = index0;
	}
	
	void ArrayElementProxy1_1<UnityEngine::Resolution>::operator=(UnityEngine::Resolution item)
	{
		Plugin::UnityEngineResolutionArray1SetItem1(Handle, Index0, item);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
	}
	
	ArrayElementProxy1_1<UnityEngine::Resolution>::operator UnityEngine::Resolution()
	{
		auto returnValue = Plugin::UnityEngineResolutionArray1GetItem1(Handle, Index0);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnValue;
	}
}

namespace System
{
	Array1<UnityEngine::Resolution>::Array1(decltype(nullptr) n)
		: Array1(Plugin::InternalUse::Only, 0)
	{
		this->InternalLength = 0;
		this->InternalRank = 0;
	}
	
	Array1<UnityEngine::Resolution>::Array1(Plugin::InternalUse iu, int32_t handle)
		: System::Array(iu, handle)
	{
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
		this->InternalLength = 0;
		this->InternalRank = 0;
	}
	
	Array1<UnityEngine::Resolution>::Array1(const Array1<UnityEngine::Resolution>& other)
		: Array1(Plugin::InternalUse::Only, other.Handle)
	{
		InternalLength = other.InternalLength;
		InternalRank = other.InternalRank;
	}
	
	Array1<UnityEngine::Resolution>::Array1(Array1<UnityEngine::Resolution>&& other)
		: Array1(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
		InternalLength = other.InternalLength;
		InternalRank = other.InternalRank;
		other.InternalLength = 0;
		other.InternalRank = 0;
	}
	
	Array1<UnityEngine::Resolution>::~Array1<UnityEngine::Resolution>()
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
	}
	
	Array1<UnityEngine::Resolution>& Array1<UnityEngine::Resolution>::operator=(const Array1<UnityEngine::Resolution>& other)
	{
		if (this->Handle)
		{
			Plugin::DereferenceManagedClass(this->Handle);
		}
		this->Handle = other.Handle;
		if (this->Handle)
		{
			Plugin::ReferenceManagedClass(this->Handle);
		}
		InternalLength = other.InternalLength;
		InternalRank = other.InternalRank;
		return *this;
	}
	
	Array1<UnityEngine::Resolution>& Array1<UnityEngine::Resolution>::operator=(decltype(nullptr) other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
		return *this;
	}
	
	Array1<UnityEngine::Resolution>& Array1<UnityEngine::Resolution>::operator=(Array1<UnityEngine::Resolution>&& other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
		}
		Handle = other.Handle;
		InternalLength = other.InternalLength;
		InternalRank = other.InternalRank;
		other.Handle = 0;
		other.InternalLength = 0;
		other.InternalRank = 0;
		return *this;
	}
	
	bool Array1<UnityEngine::Resolution>::operator==(const Array1<UnityEngine::Resolution>& other) const
	{
		return Handle == other.Handle;
	}
	
	bool Array1<UnityEngine::Resolution>::operator!=(const Array1<UnityEngine::Resolution>& other) const
	{
		return Handle != other.Handle;
	}
	
	Array1<UnityEngine::Resolution>::Array1(int32_t length0)
		 : System::Array(nullptr)
	{
		auto returnValue = Plugin::UnityEngineUnityEngineResolutionArray1Constructor1(length0);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		Handle = returnValue;
		if (returnValue)
		{
			Plugin::ReferenceManagedClass(returnValue);
			InternalLength = length0;
		}
	}
	
	int32_t Array1<UnityEngine::Resolution>::GetLength()
	{
		int32_t returnVal = InternalLength;
		if (returnVal == 0)
		{
			returnVal = Array::GetLength();
			InternalLength = returnVal;
		};
		return returnVal;
	}
	
	int32_t Array1<UnityEngine::Resolution>::GetRank()
	{
		int32_t returnVal = InternalRank;
		if (returnVal == 0)
		{
			returnVal = Array::GetRank();
			InternalRank = returnVal;
		};
		return returnVal;
	}
	
	Plugin::ArrayElementProxy1_1<UnityEngine::Resolution> System::Array1<UnityEngine::Resolution>::operator[](int32_t index)
	{
		return Plugin::ArrayElementProxy1_1<UnityEngine::Resolution>(Plugin::InternalUse::Only, Handle, index);
	}
}

namespace Plugin
{
	ArrayElementProxy1_1<UnityEngine::RaycastHit>::ArrayElementProxy1_1(Plugin::InternalUse iu, int32_t handle, int32_t index0)
	{
		Handle = handle;
		Index0 = index0;
	}
	
	void ArrayElementProxy1_1<UnityEngine::RaycastHit>::operator=(UnityEngine::RaycastHit item)
	{
		Plugin::UnityEngineRaycastHitArray1SetItem1(Handle, Index0, item.Handle);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
	}
	
	ArrayElementProxy1_1<UnityEngine::RaycastHit>::operator UnityEngine::RaycastHit()
	{
		auto returnValue = Plugin::UnityEngineRaycastHitArray1GetItem1(Handle, Index0);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return UnityEngine::RaycastHit(Plugin::InternalUse::Only, returnValue);
	}
}

namespace System
{
	Array1<UnityEngine::RaycastHit>::Array1(decltype(nullptr) n)
		: Array1(Plugin::InternalUse::Only, 0)
	{
		this->InternalLength = 0;
		this->InternalRank = 0;
	}
	
	Array1<UnityEngine::RaycastHit>::Array1(Plugin::InternalUse iu, int32_t handle)
		: System::Array(iu, handle)
	{
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
		this->InternalLength = 0;
		this->InternalRank = 0;
	}
	
	Array1<UnityEngine::RaycastHit>::Array1(const Array1<UnityEngine::RaycastHit>& other)
		: Array1(Plugin::InternalUse::Only, other.Handle)
	{
		InternalLength = other.InternalLength;
		InternalRank = other.InternalRank;
	}
	
	Array1<UnityEngine::RaycastHit>::Array1(Array1<UnityEngine::RaycastHit>&& other)
		: Array1(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
		InternalLength = other.InternalLength;
		InternalRank = other.InternalRank;
		other.InternalLength = 0;
		other.InternalRank = 0;
	}
	
	Array1<UnityEngine::RaycastHit>::~Array1<UnityEngine::RaycastHit>()
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
	}
	
	Array1<UnityEngine::RaycastHit>& Array1<UnityEngine::RaycastHit>::operator=(const Array1<UnityEngine::RaycastHit>& other)
	{
		if (this->Handle)
		{
			Plugin::DereferenceManagedClass(this->Handle);
		}
		this->Handle = other.Handle;
		if (this->Handle)
		{
			Plugin::ReferenceManagedClass(this->Handle);
		}
		InternalLength = other.InternalLength;
		InternalRank = other.InternalRank;
		return *this;
	}
	
	Array1<UnityEngine::RaycastHit>& Array1<UnityEngine::RaycastHit>::operator=(decltype(nullptr) other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
		return *this;
	}
	
	Array1<UnityEngine::RaycastHit>& Array1<UnityEngine::RaycastHit>::operator=(Array1<UnityEngine::RaycastHit>&& other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
		}
		Handle = other.Handle;
		InternalLength = other.InternalLength;
		InternalRank = other.InternalRank;
		other.Handle = 0;
		other.InternalLength = 0;
		other.InternalRank = 0;
		return *this;
	}
	
	bool Array1<UnityEngine::RaycastHit>::operator==(const Array1<UnityEngine::RaycastHit>& other) const
	{
		return Handle == other.Handle;
	}
	
	bool Array1<UnityEngine::RaycastHit>::operator!=(const Array1<UnityEngine::RaycastHit>& other) const
	{
		return Handle != other.Handle;
	}
	
	Array1<UnityEngine::RaycastHit>::Array1(int32_t length0)
		 : System::Array(nullptr)
	{
		auto returnValue = Plugin::UnityEngineUnityEngineRaycastHitArray1Constructor1(length0);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		Handle = returnValue;
		if (returnValue)
		{
			Plugin::ReferenceManagedClass(returnValue);
			InternalLength = length0;
		}
	}
	
	int32_t Array1<UnityEngine::RaycastHit>::GetLength()
	{
		int32_t returnVal = InternalLength;
		if (returnVal == 0)
		{
			returnVal = Array::GetLength();
			InternalLength = returnVal;
		};
		return returnVal;
	}
	
	int32_t Array1<UnityEngine::RaycastHit>::GetRank()
	{
		int32_t returnVal = InternalRank;
		if (returnVal == 0)
		{
			returnVal = Array::GetRank();
			InternalRank = returnVal;
		};
		return returnVal;
	}
	
	Plugin::ArrayElementProxy1_1<UnityEngine::RaycastHit> System::Array1<UnityEngine::RaycastHit>::operator[](int32_t index)
	{
		return Plugin::ArrayElementProxy1_1<UnityEngine::RaycastHit>(Plugin::InternalUse::Only, Handle, index);
	}
}

namespace Plugin
{
	ArrayElementProxy1_1<UnityEngine::GradientColorKey>::ArrayElementProxy1_1(Plugin::InternalUse iu, int32_t handle, int32_t index0)
	{
		Handle = handle;
		Index0 = index0;
	}
	
	void ArrayElementProxy1_1<UnityEngine::GradientColorKey>::operator=(UnityEngine::GradientColorKey item)
	{
		Plugin::UnityEngineGradientColorKeyArray1SetItem1(Handle, Index0, item);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
	}
	
	ArrayElementProxy1_1<UnityEngine::GradientColorKey>::operator UnityEngine::GradientColorKey()
	{
		auto returnValue = Plugin::UnityEngineGradientColorKeyArray1GetItem1(Handle, Index0);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnValue;
	}
}

namespace System
{
	Array1<UnityEngine::GradientColorKey>::Array1(decltype(nullptr) n)
		: Array1(Plugin::InternalUse::Only, 0)
	{
		this->InternalLength = 0;
		this->InternalRank = 0;
	}
	
	Array1<UnityEngine::GradientColorKey>::Array1(Plugin::InternalUse iu, int32_t handle)
		: System::Array(iu, handle)
	{
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
		this->InternalLength = 0;
		this->InternalRank = 0;
	}
	
	Array1<UnityEngine::GradientColorKey>::Array1(const Array1<UnityEngine::GradientColorKey>& other)
		: Array1(Plugin::InternalUse::Only, other.Handle)
	{
		InternalLength = other.InternalLength;
		InternalRank = other.InternalRank;
	}
	
	Array1<UnityEngine::GradientColorKey>::Array1(Array1<UnityEngine::GradientColorKey>&& other)
		: Array1(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
		InternalLength = other.InternalLength;
		InternalRank = other.InternalRank;
		other.InternalLength = 0;
		other.InternalRank = 0;
	}
	
	Array1<UnityEngine::GradientColorKey>::~Array1<UnityEngine::GradientColorKey>()
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
	}
	
	Array1<UnityEngine::GradientColorKey>& Array1<UnityEngine::GradientColorKey>::operator=(const Array1<UnityEngine::GradientColorKey>& other)
	{
		if (this->Handle)
		{
			Plugin::DereferenceManagedClass(this->Handle);
		}
		this->Handle = other.Handle;
		if (this->Handle)
		{
			Plugin::ReferenceManagedClass(this->Handle);
		}
		InternalLength = other.InternalLength;
		InternalRank = other.InternalRank;
		return *this;
	}
	
	Array1<UnityEngine::GradientColorKey>& Array1<UnityEngine::GradientColorKey>::operator=(decltype(nullptr) other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
		return *this;
	}
	
	Array1<UnityEngine::GradientColorKey>& Array1<UnityEngine::GradientColorKey>::operator=(Array1<UnityEngine::GradientColorKey>&& other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
		}
		Handle = other.Handle;
		InternalLength = other.InternalLength;
		InternalRank = other.InternalRank;
		other.Handle = 0;
		other.InternalLength = 0;
		other.InternalRank = 0;
		return *this;
	}
	
	bool Array1<UnityEngine::GradientColorKey>::operator==(const Array1<UnityEngine::GradientColorKey>& other) const
	{
		return Handle == other.Handle;
	}
	
	bool Array1<UnityEngine::GradientColorKey>::operator!=(const Array1<UnityEngine::GradientColorKey>& other) const
	{
		return Handle != other.Handle;
	}
	
	Array1<UnityEngine::GradientColorKey>::Array1(int32_t length0)
		 : System::Array(nullptr)
	{
		auto returnValue = Plugin::UnityEngineUnityEngineGradientColorKeyArray1Constructor1(length0);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		Handle = returnValue;
		if (returnValue)
		{
			Plugin::ReferenceManagedClass(returnValue);
			InternalLength = length0;
		}
	}
	
	int32_t Array1<UnityEngine::GradientColorKey>::GetLength()
	{
		int32_t returnVal = InternalLength;
		if (returnVal == 0)
		{
			returnVal = Array::GetLength();
			InternalLength = returnVal;
		};
		return returnVal;
	}
	
	int32_t Array1<UnityEngine::GradientColorKey>::GetRank()
	{
		int32_t returnVal = InternalRank;
		if (returnVal == 0)
		{
			returnVal = Array::GetRank();
			InternalRank = returnVal;
		};
		return returnVal;
	}
	
	Plugin::ArrayElementProxy1_1<UnityEngine::GradientColorKey> System::Array1<UnityEngine::GradientColorKey>::operator[](int32_t index)
	{
		return Plugin::ArrayElementProxy1_1<UnityEngine::GradientColorKey>(Plugin::InternalUse::Only, Handle, index);
	}
}

namespace System
{
	Action::Action()
		 : System::Object(nullptr)
	{
		CppHandle = Plugin::StoreSystemAction(this);
		Plugin::SystemActionConstructor(CppHandle, &Handle, &ClassHandle);
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
		else
		{
			Plugin::RemoveSystemAction(CppHandle);
			ClassHandle = 0;
			CppHandle = 0;
		}
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
	}
	
	Action::Action(decltype(nullptr) n)
		: System::Object(Plugin::InternalUse::Only, 0)
	{
		CppHandle = Plugin::StoreSystemAction(this);
		ClassHandle = 0;
	}
	
	Action::Action(const Action& other)
		: System::Object(Plugin::InternalUse::Only, other.Handle)
	{
		CppHandle = Plugin::StoreSystemAction(this);
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
		ClassHandle = other.ClassHandle;
	}
	
	Action::Action(Action&& other)
		: System::Object(Plugin::InternalUse::Only, other.Handle)
	{
		CppHandle = other.CppHandle;
		ClassHandle = other.ClassHandle;
		other.Handle = 0;
		other.CppHandle = 0;
		other.ClassHandle = 0;
	}
	
	Action::Action(Plugin::InternalUse iu, int32_t handle)
		: System::Object(iu, handle)
	{
		CppHandle = Plugin::StoreSystemAction(this);
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
		ClassHandle = 0;
	}
	
	Action::~Action()
	{
		Plugin::RemoveSystemAction(CppHandle);
		CppHandle = 0;
		if (Handle)
		{
			int32_t handle = Handle;
			int32_t classHandle = ClassHandle;
			Handle = 0;
			ClassHandle = 0;
			if (Plugin::DereferenceManagedClassNoRelease(handle))
			{
				Plugin::ReleaseSystemAction(handle, classHandle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
			}
		}
	}
	
	Action& Action::operator=(const Action& other)
	{
		if (this->Handle)
		{
			Plugin::DereferenceManagedClass(this->Handle);
		}
		this->Handle = other.Handle;
		if (this->Handle)
		{
			Plugin::ReferenceManagedClass(this->Handle);
		}
		ClassHandle = other.ClassHandle;
		return *this;
	}
	
	Action& Action::operator=(decltype(nullptr) other)
	{
		if (Handle)
		{
			int32_t handle = Handle;
			int32_t classHandle = ClassHandle;
			Handle = 0;
			ClassHandle = 0;
			if (Plugin::DereferenceManagedClassNoRelease(handle))
			{
				Plugin::ReleaseSystemAction(handle, classHandle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
			}
		}
		ClassHandle = 0;
		Handle = 0;
		return *this;
	}
	
	Action& Action::operator=(Action&& other)
	{
		Plugin::RemoveSystemAction(CppHandle);
		CppHandle = 0;
		if (Handle)
		{
			int32_t handle = Handle;
			int32_t classHandle = ClassHandle;
			Handle = 0;
			ClassHandle = 0;
			if (Plugin::DereferenceManagedClassNoRelease(handle))
			{
				Plugin::ReleaseSystemAction(handle, classHandle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
			}
		}
		ClassHandle = other.ClassHandle;
		other.ClassHandle = 0;
		Handle = other.Handle;
		other.Handle = 0;
		return *this;
	}
	
	bool Action::operator==(const Action& other) const
	{
		return Handle == other.Handle;
	}
	
	bool Action::operator!=(const Action& other) const
	{
		return Handle != other.Handle;
	}
	
	void Action::operator+=(System::Action& del)
	{
		Plugin::SystemActionAdd(Handle, del.Handle);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
	}
	
	void Action::operator-=(System::Action& del)
	{
		Plugin::SystemActionRemove(Handle, del.Handle);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
	}
	
	void Action::operator()()
	{
	}
	
	DLLEXPORT void SystemActionNativeInvoke(int32_t cppHandle)
	{
		try
		{
			Plugin::GetSystemAction(cppHandle)->operator()();
		}
		catch (System::Exception ex)
		{
			Plugin::SetException(ex.Handle);
		}
		catch (...)
		{
			System::String msg = "Unhandled exception invoking System::Action";
			System::Exception ex(msg);
			Plugin::SetException(ex.Handle);
		}
	}
	
	void Action::Invoke()
	{
		Plugin::SystemActionInvoke(Handle);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
	}
}

namespace System
{
	Action1<float>::Action1()
		 : System::Object(nullptr)
	{
		CppHandle = Plugin::StoreSystemActionSystemSingle(this);
		Plugin::SystemActionSystemSingleConstructor(CppHandle, &Handle, &ClassHandle);
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
		else
		{
			Plugin::RemoveSystemActionSystemSingle(CppHandle);
			ClassHandle = 0;
			CppHandle = 0;
		}
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
	}
	
	Action1<float>::Action1(decltype(nullptr) n)
		: System::Object(Plugin::InternalUse::Only, 0)
	{
		CppHandle = Plugin::StoreSystemActionSystemSingle(this);
		ClassHandle = 0;
	}
	
	Action1<float>::Action1(const Action1<float>& other)
		: System::Object(Plugin::InternalUse::Only, other.Handle)
	{
		CppHandle = Plugin::StoreSystemActionSystemSingle(this);
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
		ClassHandle = other.ClassHandle;
	}
	
	Action1<float>::Action1(Action1<float>&& other)
		: System::Object(Plugin::InternalUse::Only, other.Handle)
	{
		CppHandle = other.CppHandle;
		ClassHandle = other.ClassHandle;
		other.Handle = 0;
		other.CppHandle = 0;
		other.ClassHandle = 0;
	}
	
	Action1<float>::Action1(Plugin::InternalUse iu, int32_t handle)
		: System::Object(iu, handle)
	{
		CppHandle = Plugin::StoreSystemActionSystemSingle(this);
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
		ClassHandle = 0;
	}
	
	Action1<float>::~Action1()
	{
		Plugin::RemoveSystemActionSystemSingle(CppHandle);
		CppHandle = 0;
		if (Handle)
		{
			int32_t handle = Handle;
			int32_t classHandle = ClassHandle;
			Handle = 0;
			ClassHandle = 0;
			if (Plugin::DereferenceManagedClassNoRelease(handle))
			{
				Plugin::ReleaseSystemActionSystemSingle(handle, classHandle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
			}
		}
	}
	
	Action1<float>& Action1<float>::operator=(const Action1<float>& other)
	{
		if (this->Handle)
		{
			Plugin::DereferenceManagedClass(this->Handle);
		}
		this->Handle = other.Handle;
		if (this->Handle)
		{
			Plugin::ReferenceManagedClass(this->Handle);
		}
		ClassHandle = other.ClassHandle;
		return *this;
	}
	
	Action1<float>& Action1<float>::operator=(decltype(nullptr) other)
	{
		if (Handle)
		{
			int32_t handle = Handle;
			int32_t classHandle = ClassHandle;
			Handle = 0;
			ClassHandle = 0;
			if (Plugin::DereferenceManagedClassNoRelease(handle))
			{
				Plugin::ReleaseSystemActionSystemSingle(handle, classHandle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
			}
		}
		ClassHandle = 0;
		Handle = 0;
		return *this;
	}
	
	Action1<float>& Action1<float>::operator=(Action1<float>&& other)
	{
		Plugin::RemoveSystemActionSystemSingle(CppHandle);
		CppHandle = 0;
		if (Handle)
		{
			int32_t handle = Handle;
			int32_t classHandle = ClassHandle;
			Handle = 0;
			ClassHandle = 0;
			if (Plugin::DereferenceManagedClassNoRelease(handle))
			{
				Plugin::ReleaseSystemActionSystemSingle(handle, classHandle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
			}
		}
		ClassHandle = other.ClassHandle;
		other.ClassHandle = 0;
		Handle = other.Handle;
		other.Handle = 0;
		return *this;
	}
	
	bool Action1<float>::operator==(const Action1<float>& other) const
	{
		return Handle == other.Handle;
	}
	
	bool Action1<float>::operator!=(const Action1<float>& other) const
	{
		return Handle != other.Handle;
	}
	
	void Action1<float>::operator+=(System::Action1<float>& del)
	{
		Plugin::SystemActionSystemSingleAdd(Handle, del.Handle);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
	}
	
	void Action1<float>::operator-=(System::Action1<float>& del)
	{
		Plugin::SystemActionSystemSingleRemove(Handle, del.Handle);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
	}
	
	void Action1<float>::operator()(float obj)
	{
	}
	
	DLLEXPORT void SystemActionSystemSingleNativeInvoke(int32_t cppHandle, float obj)
	{
		try
		{
			Plugin::GetSystemActionSystemSingle(cppHandle)->operator()(obj);
		}
		catch (System::Exception ex)
		{
			Plugin::SetException(ex.Handle);
		}
		catch (...)
		{
			System::String msg = "Unhandled exception invoking System::Action1<float>";
			System::Exception ex(msg);
			Plugin::SetException(ex.Handle);
		}
	}
	
	void Action1<float>::Invoke(float obj)
	{
		Plugin::SystemActionSystemSingleInvoke(Handle, obj);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
	}
}

namespace System
{
	Action2<float, float>::Action2()
		 : System::Object(nullptr)
	{
		CppHandle = Plugin::StoreSystemActionSystemSingle_SystemSingle(this);
		Plugin::SystemActionSystemSingle_SystemSingleConstructor(CppHandle, &Handle, &ClassHandle);
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
		else
		{
			Plugin::RemoveSystemActionSystemSingle_SystemSingle(CppHandle);
			ClassHandle = 0;
			CppHandle = 0;
		}
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
	}
	
	Action2<float, float>::Action2(decltype(nullptr) n)
		: System::Object(Plugin::InternalUse::Only, 0)
	{
		CppHandle = Plugin::StoreSystemActionSystemSingle_SystemSingle(this);
		ClassHandle = 0;
	}
	
	Action2<float, float>::Action2(const Action2<float, float>& other)
		: System::Object(Plugin::InternalUse::Only, other.Handle)
	{
		CppHandle = Plugin::StoreSystemActionSystemSingle_SystemSingle(this);
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
		ClassHandle = other.ClassHandle;
	}
	
	Action2<float, float>::Action2(Action2<float, float>&& other)
		: System::Object(Plugin::InternalUse::Only, other.Handle)
	{
		CppHandle = other.CppHandle;
		ClassHandle = other.ClassHandle;
		other.Handle = 0;
		other.CppHandle = 0;
		other.ClassHandle = 0;
	}
	
	Action2<float, float>::Action2(Plugin::InternalUse iu, int32_t handle)
		: System::Object(iu, handle)
	{
		CppHandle = Plugin::StoreSystemActionSystemSingle_SystemSingle(this);
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
		ClassHandle = 0;
	}
	
	Action2<float, float>::~Action2()
	{
		Plugin::RemoveSystemActionSystemSingle_SystemSingle(CppHandle);
		CppHandle = 0;
		if (Handle)
		{
			int32_t handle = Handle;
			int32_t classHandle = ClassHandle;
			Handle = 0;
			ClassHandle = 0;
			if (Plugin::DereferenceManagedClassNoRelease(handle))
			{
				Plugin::ReleaseSystemActionSystemSingle_SystemSingle(handle, classHandle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
			}
		}
	}
	
	Action2<float, float>& Action2<float, float>::operator=(const Action2<float, float>& other)
	{
		if (this->Handle)
		{
			Plugin::DereferenceManagedClass(this->Handle);
		}
		this->Handle = other.Handle;
		if (this->Handle)
		{
			Plugin::ReferenceManagedClass(this->Handle);
		}
		ClassHandle = other.ClassHandle;
		return *this;
	}
	
	Action2<float, float>& Action2<float, float>::operator=(decltype(nullptr) other)
	{
		if (Handle)
		{
			int32_t handle = Handle;
			int32_t classHandle = ClassHandle;
			Handle = 0;
			ClassHandle = 0;
			if (Plugin::DereferenceManagedClassNoRelease(handle))
			{
				Plugin::ReleaseSystemActionSystemSingle_SystemSingle(handle, classHandle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
			}
		}
		ClassHandle = 0;
		Handle = 0;
		return *this;
	}
	
	Action2<float, float>& Action2<float, float>::operator=(Action2<float, float>&& other)
	{
		Plugin::RemoveSystemActionSystemSingle_SystemSingle(CppHandle);
		CppHandle = 0;
		if (Handle)
		{
			int32_t handle = Handle;
			int32_t classHandle = ClassHandle;
			Handle = 0;
			ClassHandle = 0;
			if (Plugin::DereferenceManagedClassNoRelease(handle))
			{
				Plugin::ReleaseSystemActionSystemSingle_SystemSingle(handle, classHandle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
			}
		}
		ClassHandle = other.ClassHandle;
		other.ClassHandle = 0;
		Handle = other.Handle;
		other.Handle = 0;
		return *this;
	}
	
	bool Action2<float, float>::operator==(const Action2<float, float>& other) const
	{
		return Handle == other.Handle;
	}
	
	bool Action2<float, float>::operator!=(const Action2<float, float>& other) const
	{
		return Handle != other.Handle;
	}
	
	void Action2<float, float>::operator+=(System::Action2<float, float>& del)
	{
		Plugin::SystemActionSystemSingle_SystemSingleAdd(Handle, del.Handle);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
	}
	
	void Action2<float, float>::operator-=(System::Action2<float, float>& del)
	{
		Plugin::SystemActionSystemSingle_SystemSingleRemove(Handle, del.Handle);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
	}
	
	void Action2<float, float>::operator()(float arg1, float arg2)
	{
	}
	
	DLLEXPORT void SystemActionSystemSingle_SystemSingleNativeInvoke(int32_t cppHandle, float arg1, float arg2)
	{
		try
		{
			Plugin::GetSystemActionSystemSingle_SystemSingle(cppHandle)->operator()(arg1, arg2);
		}
		catch (System::Exception ex)
		{
			Plugin::SetException(ex.Handle);
		}
		catch (...)
		{
			System::String msg = "Unhandled exception invoking System::Action2<float, float>";
			System::Exception ex(msg);
			Plugin::SetException(ex.Handle);
		}
	}
	
	void Action2<float, float>::Invoke(float arg1, float arg2)
	{
		Plugin::SystemActionSystemSingle_SystemSingleInvoke(Handle, arg1, arg2);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
	}
}

namespace System
{
	Func3<int32_t, float, double>::Func3()
		 : System::Object(nullptr)
	{
		CppHandle = Plugin::StoreSystemFuncSystemInt32_SystemSingle_SystemDouble(this);
		Plugin::SystemFuncSystemInt32_SystemSingle_SystemDoubleConstructor(CppHandle, &Handle, &ClassHandle);
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
		else
		{
			Plugin::RemoveSystemFuncSystemInt32_SystemSingle_SystemDouble(CppHandle);
			ClassHandle = 0;
			CppHandle = 0;
		}
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
	}
	
	Func3<int32_t, float, double>::Func3(decltype(nullptr) n)
		: System::Object(Plugin::InternalUse::Only, 0)
	{
		CppHandle = Plugin::StoreSystemFuncSystemInt32_SystemSingle_SystemDouble(this);
		ClassHandle = 0;
	}
	
	Func3<int32_t, float, double>::Func3(const Func3<int32_t, float, double>& other)
		: System::Object(Plugin::InternalUse::Only, other.Handle)
	{
		CppHandle = Plugin::StoreSystemFuncSystemInt32_SystemSingle_SystemDouble(this);
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
		ClassHandle = other.ClassHandle;
	}
	
	Func3<int32_t, float, double>::Func3(Func3<int32_t, float, double>&& other)
		: System::Object(Plugin::InternalUse::Only, other.Handle)
	{
		CppHandle = other.CppHandle;
		ClassHandle = other.ClassHandle;
		other.Handle = 0;
		other.CppHandle = 0;
		other.ClassHandle = 0;
	}
	
	Func3<int32_t, float, double>::Func3(Plugin::InternalUse iu, int32_t handle)
		: System::Object(iu, handle)
	{
		CppHandle = Plugin::StoreSystemFuncSystemInt32_SystemSingle_SystemDouble(this);
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
		ClassHandle = 0;
	}
	
	Func3<int32_t, float, double>::~Func3()
	{
		Plugin::RemoveSystemFuncSystemInt32_SystemSingle_SystemDouble(CppHandle);
		CppHandle = 0;
		if (Handle)
		{
			int32_t handle = Handle;
			int32_t classHandle = ClassHandle;
			Handle = 0;
			ClassHandle = 0;
			if (Plugin::DereferenceManagedClassNoRelease(handle))
			{
				Plugin::ReleaseSystemFuncSystemInt32_SystemSingle_SystemDouble(handle, classHandle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
			}
		}
	}
	
	Func3<int32_t, float, double>& Func3<int32_t, float, double>::operator=(const Func3<int32_t, float, double>& other)
	{
		if (this->Handle)
		{
			Plugin::DereferenceManagedClass(this->Handle);
		}
		this->Handle = other.Handle;
		if (this->Handle)
		{
			Plugin::ReferenceManagedClass(this->Handle);
		}
		ClassHandle = other.ClassHandle;
		return *this;
	}
	
	Func3<int32_t, float, double>& Func3<int32_t, float, double>::operator=(decltype(nullptr) other)
	{
		if (Handle)
		{
			int32_t handle = Handle;
			int32_t classHandle = ClassHandle;
			Handle = 0;
			ClassHandle = 0;
			if (Plugin::DereferenceManagedClassNoRelease(handle))
			{
				Plugin::ReleaseSystemFuncSystemInt32_SystemSingle_SystemDouble(handle, classHandle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
			}
		}
		ClassHandle = 0;
		Handle = 0;
		return *this;
	}
	
	Func3<int32_t, float, double>& Func3<int32_t, float, double>::operator=(Func3<int32_t, float, double>&& other)
	{
		Plugin::RemoveSystemFuncSystemInt32_SystemSingle_SystemDouble(CppHandle);
		CppHandle = 0;
		if (Handle)
		{
			int32_t handle = Handle;
			int32_t classHandle = ClassHandle;
			Handle = 0;
			ClassHandle = 0;
			if (Plugin::DereferenceManagedClassNoRelease(handle))
			{
				Plugin::ReleaseSystemFuncSystemInt32_SystemSingle_SystemDouble(handle, classHandle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
			}
		}
		ClassHandle = other.ClassHandle;
		other.ClassHandle = 0;
		Handle = other.Handle;
		other.Handle = 0;
		return *this;
	}
	
	bool Func3<int32_t, float, double>::operator==(const Func3<int32_t, float, double>& other) const
	{
		return Handle == other.Handle;
	}
	
	bool Func3<int32_t, float, double>::operator!=(const Func3<int32_t, float, double>& other) const
	{
		return Handle != other.Handle;
	}
	
	void Func3<int32_t, float, double>::operator+=(System::Func3<int32_t, float, double>& del)
	{
		Plugin::SystemFuncSystemInt32_SystemSingle_SystemDoubleAdd(Handle, del.Handle);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
	}
	
	void Func3<int32_t, float, double>::operator-=(System::Func3<int32_t, float, double>& del)
	{
		Plugin::SystemFuncSystemInt32_SystemSingle_SystemDoubleRemove(Handle, del.Handle);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
	}
	
	double Func3<int32_t, float, double>::operator()(int32_t arg1, float arg2)
	{
		return {};
	}
	
	DLLEXPORT double SystemFuncSystemInt32_SystemSingle_SystemDoubleNativeInvoke(int32_t cppHandle, int32_t arg1, float arg2)
	{
		try
		{
			return Plugin::GetSystemFuncSystemInt32_SystemSingle_SystemDouble(cppHandle)->operator()(arg1, arg2);
		}
		catch (System::Exception ex)
		{
			Plugin::SetException(ex.Handle);
			return {};
		}
		catch (...)
		{
			System::String msg = "Unhandled exception invoking System::Func3<int32_t, float, double>";
			System::Exception ex(msg);
			Plugin::SetException(ex.Handle);
			return {};
		}
	}
	
	double Func3<int32_t, float, double>::Invoke(int32_t arg1, float arg2)
	{
		auto returnValue = Plugin::SystemFuncSystemInt32_SystemSingle_SystemDoubleInvoke(Handle, arg1, arg2);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnValue;
	}
}

namespace System
{
	Func3<int16_t, int32_t, System::String>::Func3()
		 : System::Object(nullptr)
	{
		CppHandle = Plugin::StoreSystemFuncSystemInt16_SystemInt32_SystemString(this);
		Plugin::SystemFuncSystemInt16_SystemInt32_SystemStringConstructor(CppHandle, &Handle, &ClassHandle);
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
		else
		{
			Plugin::RemoveSystemFuncSystemInt16_SystemInt32_SystemString(CppHandle);
			ClassHandle = 0;
			CppHandle = 0;
		}
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
	}
	
	Func3<int16_t, int32_t, System::String>::Func3(decltype(nullptr) n)
		: System::Object(Plugin::InternalUse::Only, 0)
	{
		CppHandle = Plugin::StoreSystemFuncSystemInt16_SystemInt32_SystemString(this);
		ClassHandle = 0;
	}
	
	Func3<int16_t, int32_t, System::String>::Func3(const Func3<int16_t, int32_t, System::String>& other)
		: System::Object(Plugin::InternalUse::Only, other.Handle)
	{
		CppHandle = Plugin::StoreSystemFuncSystemInt16_SystemInt32_SystemString(this);
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
		ClassHandle = other.ClassHandle;
	}
	
	Func3<int16_t, int32_t, System::String>::Func3(Func3<int16_t, int32_t, System::String>&& other)
		: System::Object(Plugin::InternalUse::Only, other.Handle)
	{
		CppHandle = other.CppHandle;
		ClassHandle = other.ClassHandle;
		other.Handle = 0;
		other.CppHandle = 0;
		other.ClassHandle = 0;
	}
	
	Func3<int16_t, int32_t, System::String>::Func3(Plugin::InternalUse iu, int32_t handle)
		: System::Object(iu, handle)
	{
		CppHandle = Plugin::StoreSystemFuncSystemInt16_SystemInt32_SystemString(this);
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
		ClassHandle = 0;
	}
	
	Func3<int16_t, int32_t, System::String>::~Func3()
	{
		Plugin::RemoveSystemFuncSystemInt16_SystemInt32_SystemString(CppHandle);
		CppHandle = 0;
		if (Handle)
		{
			int32_t handle = Handle;
			int32_t classHandle = ClassHandle;
			Handle = 0;
			ClassHandle = 0;
			if (Plugin::DereferenceManagedClassNoRelease(handle))
			{
				Plugin::ReleaseSystemFuncSystemInt16_SystemInt32_SystemString(handle, classHandle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
			}
		}
	}
	
	Func3<int16_t, int32_t, System::String>& Func3<int16_t, int32_t, System::String>::operator=(const Func3<int16_t, int32_t, System::String>& other)
	{
		if (this->Handle)
		{
			Plugin::DereferenceManagedClass(this->Handle);
		}
		this->Handle = other.Handle;
		if (this->Handle)
		{
			Plugin::ReferenceManagedClass(this->Handle);
		}
		ClassHandle = other.ClassHandle;
		return *this;
	}
	
	Func3<int16_t, int32_t, System::String>& Func3<int16_t, int32_t, System::String>::operator=(decltype(nullptr) other)
	{
		if (Handle)
		{
			int32_t handle = Handle;
			int32_t classHandle = ClassHandle;
			Handle = 0;
			ClassHandle = 0;
			if (Plugin::DereferenceManagedClassNoRelease(handle))
			{
				Plugin::ReleaseSystemFuncSystemInt16_SystemInt32_SystemString(handle, classHandle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
			}
		}
		ClassHandle = 0;
		Handle = 0;
		return *this;
	}
	
	Func3<int16_t, int32_t, System::String>& Func3<int16_t, int32_t, System::String>::operator=(Func3<int16_t, int32_t, System::String>&& other)
	{
		Plugin::RemoveSystemFuncSystemInt16_SystemInt32_SystemString(CppHandle);
		CppHandle = 0;
		if (Handle)
		{
			int32_t handle = Handle;
			int32_t classHandle = ClassHandle;
			Handle = 0;
			ClassHandle = 0;
			if (Plugin::DereferenceManagedClassNoRelease(handle))
			{
				Plugin::ReleaseSystemFuncSystemInt16_SystemInt32_SystemString(handle, classHandle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
			}
		}
		ClassHandle = other.ClassHandle;
		other.ClassHandle = 0;
		Handle = other.Handle;
		other.Handle = 0;
		return *this;
	}
	
	bool Func3<int16_t, int32_t, System::String>::operator==(const Func3<int16_t, int32_t, System::String>& other) const
	{
		return Handle == other.Handle;
	}
	
	bool Func3<int16_t, int32_t, System::String>::operator!=(const Func3<int16_t, int32_t, System::String>& other) const
	{
		return Handle != other.Handle;
	}
	
	void Func3<int16_t, int32_t, System::String>::operator+=(System::Func3<int16_t, int32_t, System::String>& del)
	{
		Plugin::SystemFuncSystemInt16_SystemInt32_SystemStringAdd(Handle, del.Handle);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
	}
	
	void Func3<int16_t, int32_t, System::String>::operator-=(System::Func3<int16_t, int32_t, System::String>& del)
	{
		Plugin::SystemFuncSystemInt16_SystemInt32_SystemStringRemove(Handle, del.Handle);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
	}
	
	System::String Func3<int16_t, int32_t, System::String>::operator()(int16_t arg1, int32_t arg2)
	{
		return {};
	}
	
	DLLEXPORT int32_t SystemFuncSystemInt16_SystemInt32_SystemStringNativeInvoke(int32_t cppHandle, int16_t arg1, int32_t arg2)
	{
		try
		{
			return Plugin::GetSystemFuncSystemInt16_SystemInt32_SystemString(cppHandle)->operator()(arg1, arg2).Handle;
		}
		catch (System::Exception ex)
		{
			Plugin::SetException(ex.Handle);
			return {};
		}
		catch (...)
		{
			System::String msg = "Unhandled exception invoking System::Func3<int16_t, int32_t, System::String>";
			System::Exception ex(msg);
			Plugin::SetException(ex.Handle);
			return {};
		}
	}
	
	System::String Func3<int16_t, int32_t, System::String>::Invoke(int16_t arg1, int32_t arg2)
	{
		auto returnValue = Plugin::SystemFuncSystemInt16_SystemInt32_SystemStringInvoke(Handle, arg1, arg2);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return System::String(Plugin::InternalUse::Only, returnValue);
	}
}

namespace System
{
	AppDomainInitializer::AppDomainInitializer()
		 : System::Object(nullptr)
	{
		CppHandle = Plugin::StoreSystemAppDomainInitializer(this);
		Plugin::SystemAppDomainInitializerConstructor(CppHandle, &Handle, &ClassHandle);
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
		else
		{
			Plugin::RemoveSystemAppDomainInitializer(CppHandle);
			ClassHandle = 0;
			CppHandle = 0;
		}
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
	}
	
	AppDomainInitializer::AppDomainInitializer(decltype(nullptr) n)
		: System::Object(Plugin::InternalUse::Only, 0)
	{
		CppHandle = Plugin::StoreSystemAppDomainInitializer(this);
		ClassHandle = 0;
	}
	
	AppDomainInitializer::AppDomainInitializer(const AppDomainInitializer& other)
		: System::Object(Plugin::InternalUse::Only, other.Handle)
	{
		CppHandle = Plugin::StoreSystemAppDomainInitializer(this);
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
		ClassHandle = other.ClassHandle;
	}
	
	AppDomainInitializer::AppDomainInitializer(AppDomainInitializer&& other)
		: System::Object(Plugin::InternalUse::Only, other.Handle)
	{
		CppHandle = other.CppHandle;
		ClassHandle = other.ClassHandle;
		other.Handle = 0;
		other.CppHandle = 0;
		other.ClassHandle = 0;
	}
	
	AppDomainInitializer::AppDomainInitializer(Plugin::InternalUse iu, int32_t handle)
		: System::Object(iu, handle)
	{
		CppHandle = Plugin::StoreSystemAppDomainInitializer(this);
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
		ClassHandle = 0;
	}
	
	AppDomainInitializer::~AppDomainInitializer()
	{
		Plugin::RemoveSystemAppDomainInitializer(CppHandle);
		CppHandle = 0;
		if (Handle)
		{
			int32_t handle = Handle;
			int32_t classHandle = ClassHandle;
			Handle = 0;
			ClassHandle = 0;
			if (Plugin::DereferenceManagedClassNoRelease(handle))
			{
				Plugin::ReleaseSystemAppDomainInitializer(handle, classHandle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
			}
		}
	}
	
	AppDomainInitializer& AppDomainInitializer::operator=(const AppDomainInitializer& other)
	{
		if (this->Handle)
		{
			Plugin::DereferenceManagedClass(this->Handle);
		}
		this->Handle = other.Handle;
		if (this->Handle)
		{
			Plugin::ReferenceManagedClass(this->Handle);
		}
		ClassHandle = other.ClassHandle;
		return *this;
	}
	
	AppDomainInitializer& AppDomainInitializer::operator=(decltype(nullptr) other)
	{
		if (Handle)
		{
			int32_t handle = Handle;
			int32_t classHandle = ClassHandle;
			Handle = 0;
			ClassHandle = 0;
			if (Plugin::DereferenceManagedClassNoRelease(handle))
			{
				Plugin::ReleaseSystemAppDomainInitializer(handle, classHandle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
			}
		}
		ClassHandle = 0;
		Handle = 0;
		return *this;
	}
	
	AppDomainInitializer& AppDomainInitializer::operator=(AppDomainInitializer&& other)
	{
		Plugin::RemoveSystemAppDomainInitializer(CppHandle);
		CppHandle = 0;
		if (Handle)
		{
			int32_t handle = Handle;
			int32_t classHandle = ClassHandle;
			Handle = 0;
			ClassHandle = 0;
			if (Plugin::DereferenceManagedClassNoRelease(handle))
			{
				Plugin::ReleaseSystemAppDomainInitializer(handle, classHandle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
			}
		}
		ClassHandle = other.ClassHandle;
		other.ClassHandle = 0;
		Handle = other.Handle;
		other.Handle = 0;
		return *this;
	}
	
	bool AppDomainInitializer::operator==(const AppDomainInitializer& other) const
	{
		return Handle == other.Handle;
	}
	
	bool AppDomainInitializer::operator!=(const AppDomainInitializer& other) const
	{
		return Handle != other.Handle;
	}
	
	void AppDomainInitializer::operator+=(System::AppDomainInitializer& del)
	{
		Plugin::SystemAppDomainInitializerAdd(Handle, del.Handle);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
	}
	
	void AppDomainInitializer::operator-=(System::AppDomainInitializer& del)
	{
		Plugin::SystemAppDomainInitializerRemove(Handle, del.Handle);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
	}
	
	void AppDomainInitializer::operator()(System::Array1<System::String> args)
	{
	}
	
	DLLEXPORT void SystemAppDomainInitializerNativeInvoke(int32_t cppHandle, int32_t argsHandle)
	{
		try
		{
			Plugin::GetSystemAppDomainInitializer(cppHandle)->operator()(System::Array1<System::String>(Plugin::InternalUse::Only, argsHandle));
		}
		catch (System::Exception ex)
		{
			Plugin::SetException(ex.Handle);
		}
		catch (...)
		{
			System::String msg = "Unhandled exception invoking System::AppDomainInitializer";
			System::Exception ex(msg);
			Plugin::SetException(ex.Handle);
		}
	}
	
	void AppDomainInitializer::Invoke(System::Array1<System::String> args)
	{
		Plugin::SystemAppDomainInitializerInvoke(Handle, args.Handle);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
	}
}

namespace UnityEngine
{
	namespace Events
	{
		UnityAction::UnityAction()
			 : System::Object(nullptr)
		{
			CppHandle = Plugin::StoreUnityEngineEventsUnityAction(this);
			Plugin::UnityEngineEventsUnityActionConstructor(CppHandle, &Handle, &ClassHandle);
			if (Handle)
			{
				Plugin::ReferenceManagedClass(Handle);
			}
			else
			{
				Plugin::RemoveUnityEngineEventsUnityAction(CppHandle);
				ClassHandle = 0;
				CppHandle = 0;
			}
			if (Plugin::unhandledCsharpException)
			{
				System::Exception* ex = Plugin::unhandledCsharpException;
				Plugin::unhandledCsharpException = nullptr;
				ex->ThrowReferenceToThis();
				delete ex;
			}
		}
		
		UnityAction::UnityAction(decltype(nullptr) n)
			: System::Object(Plugin::InternalUse::Only, 0)
		{
			CppHandle = Plugin::StoreUnityEngineEventsUnityAction(this);
			ClassHandle = 0;
		}
		
		UnityAction::UnityAction(const UnityAction& other)
			: System::Object(Plugin::InternalUse::Only, other.Handle)
		{
			CppHandle = Plugin::StoreUnityEngineEventsUnityAction(this);
			if (Handle)
			{
				Plugin::ReferenceManagedClass(Handle);
			}
			ClassHandle = other.ClassHandle;
		}
		
		UnityAction::UnityAction(UnityAction&& other)
			: System::Object(Plugin::InternalUse::Only, other.Handle)
		{
			CppHandle = other.CppHandle;
			ClassHandle = other.ClassHandle;
			other.Handle = 0;
			other.CppHandle = 0;
			other.ClassHandle = 0;
		}
		
		UnityAction::UnityAction(Plugin::InternalUse iu, int32_t handle)
			: System::Object(iu, handle)
		{
			CppHandle = Plugin::StoreUnityEngineEventsUnityAction(this);
			if (Handle)
			{
				Plugin::ReferenceManagedClass(Handle);
			}
			ClassHandle = 0;
		}
		
		UnityAction::~UnityAction()
		{
			Plugin::RemoveUnityEngineEventsUnityAction(CppHandle);
			CppHandle = 0;
			if (Handle)
			{
				int32_t handle = Handle;
				int32_t classHandle = ClassHandle;
				Handle = 0;
				ClassHandle = 0;
				if (Plugin::DereferenceManagedClassNoRelease(handle))
				{
					Plugin::ReleaseUnityEngineEventsUnityAction(handle, classHandle);
					if (Plugin::unhandledCsharpException)
					{
						System::Exception* ex = Plugin::unhandledCsharpException;
						Plugin::unhandledCsharpException = nullptr;
						ex->ThrowReferenceToThis();
						delete ex;
					}
				}
			}
		}
		
		UnityAction& UnityAction::operator=(const UnityAction& other)
		{
			if (this->Handle)
			{
				Plugin::DereferenceManagedClass(this->Handle);
			}
			this->Handle = other.Handle;
			if (this->Handle)
			{
				Plugin::ReferenceManagedClass(this->Handle);
			}
			ClassHandle = other.ClassHandle;
			return *this;
		}
		
		UnityAction& UnityAction::operator=(decltype(nullptr) other)
		{
			if (Handle)
			{
				int32_t handle = Handle;
				int32_t classHandle = ClassHandle;
				Handle = 0;
				ClassHandle = 0;
				if (Plugin::DereferenceManagedClassNoRelease(handle))
				{
					Plugin::ReleaseUnityEngineEventsUnityAction(handle, classHandle);
					if (Plugin::unhandledCsharpException)
					{
						System::Exception* ex = Plugin::unhandledCsharpException;
						Plugin::unhandledCsharpException = nullptr;
						ex->ThrowReferenceToThis();
						delete ex;
					}
				}
			}
			ClassHandle = 0;
			Handle = 0;
			return *this;
		}
		
		UnityAction& UnityAction::operator=(UnityAction&& other)
		{
			Plugin::RemoveUnityEngineEventsUnityAction(CppHandle);
			CppHandle = 0;
			if (Handle)
			{
				int32_t handle = Handle;
				int32_t classHandle = ClassHandle;
				Handle = 0;
				ClassHandle = 0;
				if (Plugin::DereferenceManagedClassNoRelease(handle))
				{
					Plugin::ReleaseUnityEngineEventsUnityAction(handle, classHandle);
					if (Plugin::unhandledCsharpException)
					{
						System::Exception* ex = Plugin::unhandledCsharpException;
						Plugin::unhandledCsharpException = nullptr;
						ex->ThrowReferenceToThis();
						delete ex;
					}
				}
			}
			ClassHandle = other.ClassHandle;
			other.ClassHandle = 0;
			Handle = other.Handle;
			other.Handle = 0;
			return *this;
		}
		
		bool UnityAction::operator==(const UnityAction& other) const
		{
			return Handle == other.Handle;
		}
		
		bool UnityAction::operator!=(const UnityAction& other) const
		{
			return Handle != other.Handle;
		}
		
		void UnityAction::operator+=(UnityEngine::Events::UnityAction& del)
		{
			Plugin::UnityEngineEventsUnityActionAdd(Handle, del.Handle);
			if (Plugin::unhandledCsharpException)
			{
				System::Exception* ex = Plugin::unhandledCsharpException;
				Plugin::unhandledCsharpException = nullptr;
				ex->ThrowReferenceToThis();
				delete ex;
			}
		}
		
		void UnityAction::operator-=(UnityEngine::Events::UnityAction& del)
		{
			Plugin::UnityEngineEventsUnityActionRemove(Handle, del.Handle);
			if (Plugin::unhandledCsharpException)
			{
				System::Exception* ex = Plugin::unhandledCsharpException;
				Plugin::unhandledCsharpException = nullptr;
				ex->ThrowReferenceToThis();
				delete ex;
			}
		}
		
		void UnityAction::operator()()
		{
		}
		
		DLLEXPORT void UnityEngineEventsUnityActionNativeInvoke(int32_t cppHandle)
		{
			try
			{
				Plugin::GetUnityEngineEventsUnityAction(cppHandle)->operator()();
			}
			catch (System::Exception ex)
			{
				Plugin::SetException(ex.Handle);
			}
			catch (...)
			{
				System::String msg = "Unhandled exception invoking UnityEngine::Events::UnityAction";
				System::Exception ex(msg);
				Plugin::SetException(ex.Handle);
			}
		}
		
		void UnityAction::Invoke()
		{
			Plugin::UnityEngineEventsUnityActionInvoke(Handle);
			if (Plugin::unhandledCsharpException)
			{
				System::Exception* ex = Plugin::unhandledCsharpException;
				Plugin::unhandledCsharpException = nullptr;
				ex->ThrowReferenceToThis();
				delete ex;
			}
		}
	}
}

namespace UnityEngine
{
	namespace Events
	{
		UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>::UnityAction2()
			 : System::Object(nullptr)
		{
			CppHandle = Plugin::StoreUnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneMode(this);
			Plugin::UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeConstructor(CppHandle, &Handle, &ClassHandle);
			if (Handle)
			{
				Plugin::ReferenceManagedClass(Handle);
			}
			else
			{
				Plugin::RemoveUnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneMode(CppHandle);
				ClassHandle = 0;
				CppHandle = 0;
			}
			if (Plugin::unhandledCsharpException)
			{
				System::Exception* ex = Plugin::unhandledCsharpException;
				Plugin::unhandledCsharpException = nullptr;
				ex->ThrowReferenceToThis();
				delete ex;
			}
		}
		
		UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>::UnityAction2(decltype(nullptr) n)
			: System::Object(Plugin::InternalUse::Only, 0)
		{
			CppHandle = Plugin::StoreUnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneMode(this);
			ClassHandle = 0;
		}
		
		UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>::UnityAction2(const UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>& other)
			: System::Object(Plugin::InternalUse::Only, other.Handle)
		{
			CppHandle = Plugin::StoreUnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneMode(this);
			if (Handle)
			{
				Plugin::ReferenceManagedClass(Handle);
			}
			ClassHandle = other.ClassHandle;
		}
		
		UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>::UnityAction2(UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>&& other)
			: System::Object(Plugin::InternalUse::Only, other.Handle)
		{
			CppHandle = other.CppHandle;
			ClassHandle = other.ClassHandle;
			other.Handle = 0;
			other.CppHandle = 0;
			other.ClassHandle = 0;
		}
		
		UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>::UnityAction2(Plugin::InternalUse iu, int32_t handle)
			: System::Object(iu, handle)
		{
			CppHandle = Plugin::StoreUnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneMode(this);
			if (Handle)
			{
				Plugin::ReferenceManagedClass(Handle);
			}
			ClassHandle = 0;
		}
		
		UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>::~UnityAction2()
		{
			Plugin::RemoveUnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneMode(CppHandle);
			CppHandle = 0;
			if (Handle)
			{
				int32_t handle = Handle;
				int32_t classHandle = ClassHandle;
				Handle = 0;
				ClassHandle = 0;
				if (Plugin::DereferenceManagedClassNoRelease(handle))
				{
					Plugin::ReleaseUnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneMode(handle, classHandle);
					if (Plugin::unhandledCsharpException)
					{
						System::Exception* ex = Plugin::unhandledCsharpException;
						Plugin::unhandledCsharpException = nullptr;
						ex->ThrowReferenceToThis();
						delete ex;
					}
				}
			}
		}
		
		UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>& UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>::operator=(const UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>& other)
		{
			if (this->Handle)
			{
				Plugin::DereferenceManagedClass(this->Handle);
			}
			this->Handle = other.Handle;
			if (this->Handle)
			{
				Plugin::ReferenceManagedClass(this->Handle);
			}
			ClassHandle = other.ClassHandle;
			return *this;
		}
		
		UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>& UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>::operator=(decltype(nullptr) other)
		{
			if (Handle)
			{
				int32_t handle = Handle;
				int32_t classHandle = ClassHandle;
				Handle = 0;
				ClassHandle = 0;
				if (Plugin::DereferenceManagedClassNoRelease(handle))
				{
					Plugin::ReleaseUnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneMode(handle, classHandle);
					if (Plugin::unhandledCsharpException)
					{
						System::Exception* ex = Plugin::unhandledCsharpException;
						Plugin::unhandledCsharpException = nullptr;
						ex->ThrowReferenceToThis();
						delete ex;
					}
				}
			}
			ClassHandle = 0;
			Handle = 0;
			return *this;
		}
		
		UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>& UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>::operator=(UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>&& other)
		{
			Plugin::RemoveUnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneMode(CppHandle);
			CppHandle = 0;
			if (Handle)
			{
				int32_t handle = Handle;
				int32_t classHandle = ClassHandle;
				Handle = 0;
				ClassHandle = 0;
				if (Plugin::DereferenceManagedClassNoRelease(handle))
				{
					Plugin::ReleaseUnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneMode(handle, classHandle);
					if (Plugin::unhandledCsharpException)
					{
						System::Exception* ex = Plugin::unhandledCsharpException;
						Plugin::unhandledCsharpException = nullptr;
						ex->ThrowReferenceToThis();
						delete ex;
					}
				}
			}
			ClassHandle = other.ClassHandle;
			other.ClassHandle = 0;
			Handle = other.Handle;
			other.Handle = 0;
			return *this;
		}
		
		bool UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>::operator==(const UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>& other) const
		{
			return Handle == other.Handle;
		}
		
		bool UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>::operator!=(const UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>& other) const
		{
			return Handle != other.Handle;
		}
		
		void UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>::operator+=(UnityEngine::Events::UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>& del)
		{
			Plugin::UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeAdd(Handle, del.Handle);
			if (Plugin::unhandledCsharpException)
			{
				System::Exception* ex = Plugin::unhandledCsharpException;
				Plugin::unhandledCsharpException = nullptr;
				ex->ThrowReferenceToThis();
				delete ex;
			}
		}
		
		void UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>::operator-=(UnityEngine::Events::UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>& del)
		{
			Plugin::UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeRemove(Handle, del.Handle);
			if (Plugin::unhandledCsharpException)
			{
				System::Exception* ex = Plugin::unhandledCsharpException;
				Plugin::unhandledCsharpException = nullptr;
				ex->ThrowReferenceToThis();
				delete ex;
			}
		}
		
		void UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>::operator()(UnityEngine::SceneManagement::Scene& arg0, UnityEngine::SceneManagement::LoadSceneMode arg1)
		{
		}
		
		DLLEXPORT void UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeNativeInvoke(int32_t cppHandle, UnityEngine::SceneManagement::Scene arg0, UnityEngine::SceneManagement::LoadSceneMode arg1)
		{
			try
			{
				Plugin::GetUnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneMode(cppHandle)->operator()(arg0, arg1);
			}
			catch (System::Exception ex)
			{
				Plugin::SetException(ex.Handle);
			}
			catch (...)
			{
				System::String msg = "Unhandled exception invoking UnityEngine::Events::UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>";
				System::Exception ex(msg);
				Plugin::SetException(ex.Handle);
			}
		}
		
		void UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>::Invoke(UnityEngine::SceneManagement::Scene& arg0, UnityEngine::SceneManagement::LoadSceneMode arg1)
		{
			Plugin::UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeInvoke(Handle, arg0, arg1);
			if (Plugin::unhandledCsharpException)
			{
				System::Exception* ex = Plugin::unhandledCsharpException;
				Plugin::unhandledCsharpException = nullptr;
				ex->ThrowReferenceToThis();
				delete ex;
			}
		}
	}
}

namespace System
{
	struct NullReferenceExceptionThrower : System::NullReferenceException
	{
		NullReferenceExceptionThrower(int32_t handle)
			: System::NullReferenceException(Plugin::InternalUse::Only, handle)
		{
		}
	
		virtual void ThrowReferenceToThis()
		{
			throw *this;
		}
	};
}

DLLEXPORT void SetCsharpExceptionSystemNullReferenceException(int32_t handle)
{
	delete Plugin::unhandledCsharpException;
	Plugin::unhandledCsharpException = new System::NullReferenceExceptionThrower(handle);
}
/*END METHOD DEFINITIONS*/

////////////////////////////////////////////////////////////////
// App-specific functions for this file to call
////////////////////////////////////////////////////////////////

// Called when the plugin is initialized
extern void PluginMain();

////////////////////////////////////////////////////////////////
// C++ functions for C# to call
////////////////////////////////////////////////////////////////

// Init the plugin
DLLEXPORT void Init(
	int32_t maxManagedObjects,
	void (*releaseObject)(int32_t handle),
	int32_t (*stringNew)(const char* chars),
	void (*setException)(int32_t handle),
	int32_t (*arrayGetLength)(int32_t handle),
	int32_t (*arrayGetRank)(int32_t handle),
	/*BEGIN INIT PARAMS*/
	int32_t (*systemDiagnosticsStopwatchConstructor)(),
	int64_t (*systemDiagnosticsStopwatchPropertyGetElapsedMilliseconds)(int32_t thisHandle),
	void (*systemDiagnosticsStopwatchMethodStart)(int32_t thisHandle),
	void (*systemDiagnosticsStopwatchMethodReset)(int32_t thisHandle),
	int32_t (*unityEngineObjectPropertyGetName)(int32_t thisHandle),
	void (*unityEngineObjectPropertySetName)(int32_t thisHandle, int32_t valueHandle),
	System::Boolean (*unityEngineObjectMethodop_EqualityUnityEngineObject_UnityEngineObject)(int32_t xHandle, int32_t yHandle),
	System::Boolean (*unityEngineObjectMethodop_ImplicitUnityEngineObject)(int32_t existsHandle),
	int32_t (*unityEngineGameObjectConstructor)(),
	int32_t (*unityEngineGameObjectConstructorSystemString)(int32_t nameHandle),
	int32_t (*unityEngineGameObjectPropertyGetTransform)(int32_t thisHandle),
	int32_t (*unityEngineGameObjectMethodAddComponentMyGameMonoBehavioursTestScript)(int32_t thisHandle),
	int32_t (*unityEngineComponentPropertyGetTransform)(int32_t thisHandle),
	UnityEngine::Vector3 (*unityEngineTransformPropertyGetPosition)(int32_t thisHandle),
	void (*unityEngineTransformPropertySetPosition)(int32_t thisHandle, UnityEngine::Vector3& value),
	void (*unityEngineDebugMethodLogSystemObject)(int32_t messageHandle),
	System::Boolean (*unityEngineAssertionsAssertFieldGetRaiseExceptions)(),
	void (*unityEngineAssertionsAssertFieldSetRaiseExceptions)(System::Boolean value),
	void (*unityEngineAssertionsAssertMethodAreEqualSystemStringSystemString_SystemString)(int32_t expectedHandle, int32_t actualHandle),
	void (*unityEngineAssertionsAssertMethodAreEqualUnityEngineGameObjectUnityEngineGameObject_UnityEngineGameObject)(int32_t expectedHandle, int32_t actualHandle),
	void (*unityEngineAudioSettingsMethodGetDSPBufferSizeSystemInt32_SystemInt32)(int32_t* bufferLength, int32_t* numBuffers),
	void (*unityEngineNetworkingNetworkTransportMethodGetBroadcastConnectionInfoSystemInt32_SystemString_SystemInt32_SystemByte)(int32_t hostId, int32_t* addressHandle, int32_t* port, uint8_t* error),
	void (*unityEngineNetworkingNetworkTransportMethodInit)(),
	UnityEngine::Vector3 (*unityEngineVector3ConstructorSystemSingle_SystemSingle_SystemSingle)(float x, float y, float z),
	float (*unityEngineVector3PropertyGetMagnitude)(UnityEngine::Vector3* thiz),
	void (*unityEngineVector3MethodSetSystemSingle_SystemSingle_SystemSingle)(UnityEngine::Vector3* thiz, float newX, float newY, float newZ),
	UnityEngine::Vector3 (*unityEngineVector3Methodop_AdditionUnityEngineVector3_UnityEngineVector3)(UnityEngine::Vector3& a, UnityEngine::Vector3& b),
	UnityEngine::Vector3 (*unityEngineVector3Methodop_UnaryNegationUnityEngineVector3)(UnityEngine::Vector3& a),
	int32_t (*boxVector3)(UnityEngine::Vector3& val),
	UnityEngine::Vector3 (*unboxVector3)(int32_t valHandle),
	float (*unityEngineMatrix4x4PropertyGetItem)(UnityEngine::Matrix4x4* thiz, int32_t row, int32_t column),
	void (*unityEngineMatrix4x4PropertySetItem)(UnityEngine::Matrix4x4* thiz, int32_t row, int32_t column, float value),
	int32_t (*boxMatrix4x4)(UnityEngine::Matrix4x4& val),
	UnityEngine::Matrix4x4 (*unboxMatrix4x4)(int32_t valHandle),
	void (*releaseUnityEngineRaycastHit)(int32_t handle),
	UnityEngine::Vector3 (*unityEngineRaycastHitPropertyGetPoint)(int32_t thisHandle),
	void (*unityEngineRaycastHitPropertySetPoint)(int32_t thisHandle, UnityEngine::Vector3& value),
	int32_t (*unityEngineRaycastHitPropertyGetTransform)(int32_t thisHandle),
	int32_t (*boxRaycastHit)(int32_t valHandle),
	int32_t (*unboxRaycastHit)(int32_t valHandle),
	int32_t (*boxQueryTriggerInteraction)(UnityEngine::QueryTriggerInteraction val),
	UnityEngine::QueryTriggerInteraction (*unboxQueryTriggerInteraction)(int32_t valHandle),
	void (*releaseSystemCollectionsGenericKeyValuePairSystemString_SystemDouble)(int32_t handle),
	int32_t (*systemCollectionsGenericKeyValuePairSystemString_SystemDoubleConstructorSystemString_SystemDouble)(int32_t keyHandle, double value),
	int32_t (*systemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetKey)(int32_t thisHandle),
	double (*systemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetValue)(int32_t thisHandle),
	int32_t (*boxKeyValuePairSystemString_SystemDouble)(int32_t valHandle),
	int32_t (*unboxKeyValuePairSystemString_SystemDouble)(int32_t valHandle),
	int32_t (*systemCollectionsGenericListSystemStringConstructor)(),
	int32_t (*systemCollectionsGenericListSystemStringPropertyGetItem)(int32_t thisHandle, int32_t index),
	void (*systemCollectionsGenericListSystemStringPropertySetItem)(int32_t thisHandle, int32_t index, int32_t valueHandle),
	void (*systemCollectionsGenericListSystemStringMethodAddSystemString)(int32_t thisHandle, int32_t itemHandle),
	void (*systemCollectionsGenericListSystemStringMethodSortSystemCollectionsGenericIComparer)(int32_t thisHandle, int32_t comparerHandle),
	int32_t (*systemCollectionsGenericListSystemInt32Constructor)(),
	int32_t (*systemCollectionsGenericListSystemInt32PropertyGetItem)(int32_t thisHandle, int32_t index),
	void (*systemCollectionsGenericListSystemInt32PropertySetItem)(int32_t thisHandle, int32_t index, int32_t value),
	void (*systemCollectionsGenericListSystemInt32MethodAddSystemInt32)(int32_t thisHandle, int32_t item),
	void (*systemCollectionsGenericListSystemInt32MethodSortSystemCollectionsGenericIComparer)(int32_t thisHandle, int32_t comparerHandle),
	int32_t (*systemCollectionsGenericLinkedListNodeSystemStringConstructorSystemString)(int32_t valueHandle),
	int32_t (*systemCollectionsGenericLinkedListNodeSystemStringPropertyGetValue)(int32_t thisHandle),
	void (*systemCollectionsGenericLinkedListNodeSystemStringPropertySetValue)(int32_t thisHandle, int32_t valueHandle),
	int32_t (*systemRuntimeCompilerServicesStrongBoxSystemStringConstructorSystemString)(int32_t valueHandle),
	int32_t (*systemRuntimeCompilerServicesStrongBoxSystemStringFieldGetValue)(int32_t thisHandle),
	void (*systemRuntimeCompilerServicesStrongBoxSystemStringFieldSetValue)(int32_t thisHandle, int32_t valueHandle),
	int32_t (*systemExceptionConstructorSystemString)(int32_t messageHandle),
	int32_t (*unityEngineResolutionPropertyGetWidth)(UnityEngine::Resolution* thiz),
	void (*unityEngineResolutionPropertySetWidth)(UnityEngine::Resolution* thiz, int32_t value),
	int32_t (*unityEngineResolutionPropertyGetHeight)(UnityEngine::Resolution* thiz),
	void (*unityEngineResolutionPropertySetHeight)(UnityEngine::Resolution* thiz, int32_t value),
	int32_t (*unityEngineResolutionPropertyGetRefreshRate)(UnityEngine::Resolution* thiz),
	void (*unityEngineResolutionPropertySetRefreshRate)(UnityEngine::Resolution* thiz, int32_t value),
	int32_t (*boxResolution)(UnityEngine::Resolution& val),
	UnityEngine::Resolution (*unboxResolution)(int32_t valHandle),
	int32_t (*unityEngineScreenPropertyGetResolutions)(),
	UnityEngine::Ray (*unityEngineRayConstructorUnityEngineVector3_UnityEngineVector3)(UnityEngine::Vector3& origin, UnityEngine::Vector3& direction),
	int32_t (*boxRay)(UnityEngine::Ray& val),
	UnityEngine::Ray (*unboxRay)(int32_t valHandle),
	int32_t (*unityEnginePhysicsMethodRaycastNonAllocUnityEngineRay_UnityEngineRaycastHit)(UnityEngine::Ray& ray, int32_t resultsHandle),
	int32_t (*unityEnginePhysicsMethodRaycastAllUnityEngineRay)(UnityEngine::Ray& ray),
	int32_t (*boxColor)(UnityEngine::Color& val),
	UnityEngine::Color (*unboxColor)(int32_t valHandle),
	int32_t (*boxGradientColorKey)(UnityEngine::GradientColorKey& val),
	UnityEngine::GradientColorKey (*unboxGradientColorKey)(int32_t valHandle),
	int32_t (*unityEngineGradientConstructor)(),
	int32_t (*unityEngineGradientPropertyGetColorKeys)(int32_t thisHandle),
	void (*unityEngineGradientPropertySetColorKeys)(int32_t thisHandle, int32_t valueHandle),
	int32_t (*systemAppDomainSetupConstructor)(),
	int32_t (*systemAppDomainSetupPropertyGetAppDomainInitializer)(int32_t thisHandle),
	void (*systemAppDomainSetupPropertySetAppDomainInitializer)(int32_t thisHandle, int32_t valueHandle),
	void (*unityEngineApplicationAddEventOnBeforeRender)(int32_t delHandle),
	void (*unityEngineApplicationRemoveEventOnBeforeRender)(int32_t delHandle),
	void (*unityEngineSceneManagementSceneManagerAddEventSceneLoaded)(int32_t delHandle),
	void (*unityEngineSceneManagementSceneManagerRemoveEventSceneLoaded)(int32_t delHandle),
	int32_t (*boxScene)(UnityEngine::SceneManagement::Scene& val),
	UnityEngine::SceneManagement::Scene (*unboxScene)(int32_t valHandle),
	int32_t (*boxLoadSceneMode)(UnityEngine::SceneManagement::LoadSceneMode val),
	UnityEngine::SceneManagement::LoadSceneMode (*unboxLoadSceneMode)(int32_t valHandle),
	void (*releaseSystemCollectionsGenericIComparerSystemInt32)(int32_t handle),
	void (*systemCollectionsGenericIComparerSystemInt32Constructor)(int32_t cppHandle, int32_t* handle),
	void (*releaseSystemCollectionsGenericIComparerSystemString)(int32_t handle),
	void (*systemCollectionsGenericIComparerSystemStringConstructor)(int32_t cppHandle, int32_t* handle),
	void (*releaseSystemStringComparer)(int32_t handle),
	void (*systemStringComparerConstructor)(int32_t cppHandle, int32_t* handle),
	void (*releaseSystemEventArgs)(int32_t handle),
	void (*systemEventArgsConstructor)(int32_t cppHandle, int32_t* handle),
	int32_t (*boxBoolean)(System::Boolean val),
	System::Boolean (*unboxBoolean)(int32_t valHandle),
	int32_t (*boxSByte)(int8_t val),
	int8_t (*unboxSByte)(int32_t valHandle),
	int32_t (*boxByte)(uint8_t val),
	uint8_t (*unboxByte)(int32_t valHandle),
	int32_t (*boxInt16)(int16_t val),
	int16_t (*unboxInt16)(int32_t valHandle),
	int32_t (*boxUInt16)(uint16_t val),
	uint16_t (*unboxUInt16)(int32_t valHandle),
	int32_t (*boxInt32)(int32_t val),
	int32_t (*unboxInt32)(int32_t valHandle),
	int32_t (*boxUInt32)(uint32_t val),
	uint32_t (*unboxUInt32)(int32_t valHandle),
	int32_t (*boxInt64)(int64_t val),
	int64_t (*unboxInt64)(int32_t valHandle),
	int32_t (*boxUInt64)(uint64_t val),
	uint64_t (*unboxUInt64)(int32_t valHandle),
	int32_t (*boxChar)(System::Char val),
	System::Char (*unboxChar)(int32_t valHandle),
	int32_t (*boxSingle)(float val),
	float (*unboxSingle)(int32_t valHandle),
	int32_t (*boxDouble)(double val),
	double (*unboxDouble)(int32_t valHandle),
	int32_t (*systemSystemInt32Array1Constructor1)(int32_t length0),
	int32_t (*systemInt32Array1GetItem1)(int32_t thisHandle, int32_t index0),
	int32_t (*systemInt32Array1SetItem1)(int32_t thisHandle, int32_t index0, int32_t item),
	int32_t (*systemSystemSingleArray1Constructor1)(int32_t length0),
	float (*systemSingleArray1GetItem1)(int32_t thisHandle, int32_t index0),
	int32_t (*systemSingleArray1SetItem1)(int32_t thisHandle, int32_t index0, float item),
	int32_t (*systemSystemSingleArray2Constructor2)(int32_t length0, int32_t length1),
	int32_t (*systemSystemSingleArray2GetLength2)(int32_t thisHandle, int32_t dimension),
	float (*systemSingleArray2GetItem2)(int32_t thisHandle, int32_t index0, int32_t index1),
	int32_t (*systemSingleArray2SetItem2)(int32_t thisHandle, int32_t index0, int32_t index1, float item),
	int32_t (*systemSystemSingleArray3Constructor3)(int32_t length0, int32_t length1, int32_t length2),
	int32_t (*systemSystemSingleArray3GetLength3)(int32_t thisHandle, int32_t dimension),
	float (*systemSingleArray3GetItem3)(int32_t thisHandle, int32_t index0, int32_t index1, int32_t index2),
	int32_t (*systemSingleArray3SetItem3)(int32_t thisHandle, int32_t index0, int32_t index1, int32_t index2, float item),
	int32_t (*systemSystemStringArray1Constructor1)(int32_t length0),
	int32_t (*systemStringArray1GetItem1)(int32_t thisHandle, int32_t index0),
	int32_t (*systemStringArray1SetItem1)(int32_t thisHandle, int32_t index0, int32_t itemHandle),
	int32_t (*unityEngineUnityEngineResolutionArray1Constructor1)(int32_t length0),
	UnityEngine::Resolution (*unityEngineResolutionArray1GetItem1)(int32_t thisHandle, int32_t index0),
	int32_t (*unityEngineResolutionArray1SetItem1)(int32_t thisHandle, int32_t index0, UnityEngine::Resolution& item),
	int32_t (*unityEngineUnityEngineRaycastHitArray1Constructor1)(int32_t length0),
	int32_t (*unityEngineRaycastHitArray1GetItem1)(int32_t thisHandle, int32_t index0),
	int32_t (*unityEngineRaycastHitArray1SetItem1)(int32_t thisHandle, int32_t index0, int32_t itemHandle),
	int32_t (*unityEngineUnityEngineGradientColorKeyArray1Constructor1)(int32_t length0),
	UnityEngine::GradientColorKey (*unityEngineGradientColorKeyArray1GetItem1)(int32_t thisHandle, int32_t index0),
	int32_t (*unityEngineGradientColorKeyArray1SetItem1)(int32_t thisHandle, int32_t index0, UnityEngine::GradientColorKey& item),
	void (*releaseSystemAction)(int32_t handle, int32_t classHandle),
	void (*systemActionConstructor)(int32_t cppHandle, int32_t* handle, int32_t* classHandle),
	void (*systemActionAdd)(int32_t thisHandle, int32_t delHandle),
	void (*systemActionRemove)(int32_t thisHandle, int32_t delHandle),
	void (*systemActionInvoke)(int32_t thisHandle),
	void (*releaseSystemActionSystemSingle)(int32_t handle, int32_t classHandle),
	void (*systemActionSystemSingleConstructor)(int32_t cppHandle, int32_t* handle, int32_t* classHandle),
	void (*systemActionSystemSingleAdd)(int32_t thisHandle, int32_t delHandle),
	void (*systemActionSystemSingleRemove)(int32_t thisHandle, int32_t delHandle),
	void (*systemActionSystemSingleInvoke)(int32_t thisHandle, float obj),
	void (*releaseSystemActionSystemSingle_SystemSingle)(int32_t handle, int32_t classHandle),
	void (*systemActionSystemSingle_SystemSingleConstructor)(int32_t cppHandle, int32_t* handle, int32_t* classHandle),
	void (*systemActionSystemSingle_SystemSingleAdd)(int32_t thisHandle, int32_t delHandle),
	void (*systemActionSystemSingle_SystemSingleRemove)(int32_t thisHandle, int32_t delHandle),
	void (*systemActionSystemSingle_SystemSingleInvoke)(int32_t thisHandle, float arg1, float arg2),
	void (*releaseSystemFuncSystemInt32_SystemSingle_SystemDouble)(int32_t handle, int32_t classHandle),
	void (*systemFuncSystemInt32_SystemSingle_SystemDoubleConstructor)(int32_t cppHandle, int32_t* handle, int32_t* classHandle),
	void (*systemFuncSystemInt32_SystemSingle_SystemDoubleAdd)(int32_t thisHandle, int32_t delHandle),
	void (*systemFuncSystemInt32_SystemSingle_SystemDoubleRemove)(int32_t thisHandle, int32_t delHandle),
	double (*systemFuncSystemInt32_SystemSingle_SystemDoubleInvoke)(int32_t thisHandle, int32_t arg1, float arg2),
	void (*releaseSystemFuncSystemInt16_SystemInt32_SystemString)(int32_t handle, int32_t classHandle),
	void (*systemFuncSystemInt16_SystemInt32_SystemStringConstructor)(int32_t cppHandle, int32_t* handle, int32_t* classHandle),
	void (*systemFuncSystemInt16_SystemInt32_SystemStringAdd)(int32_t thisHandle, int32_t delHandle),
	void (*systemFuncSystemInt16_SystemInt32_SystemStringRemove)(int32_t thisHandle, int32_t delHandle),
	int32_t (*systemFuncSystemInt16_SystemInt32_SystemStringInvoke)(int32_t thisHandle, int16_t arg1, int32_t arg2),
	void (*releaseSystemAppDomainInitializer)(int32_t handle, int32_t classHandle),
	void (*systemAppDomainInitializerConstructor)(int32_t cppHandle, int32_t* handle, int32_t* classHandle),
	void (*systemAppDomainInitializerAdd)(int32_t thisHandle, int32_t delHandle),
	void (*systemAppDomainInitializerRemove)(int32_t thisHandle, int32_t delHandle),
	void (*systemAppDomainInitializerInvoke)(int32_t thisHandle, int32_t argsHandle),
	void (*releaseUnityEngineEventsUnityAction)(int32_t handle, int32_t classHandle),
	void (*unityEngineEventsUnityActionConstructor)(int32_t cppHandle, int32_t* handle, int32_t* classHandle),
	void (*unityEngineEventsUnityActionAdd)(int32_t thisHandle, int32_t delHandle),
	void (*unityEngineEventsUnityActionRemove)(int32_t thisHandle, int32_t delHandle),
	void (*unityEngineEventsUnityActionInvoke)(int32_t thisHandle),
	void (*releaseUnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneMode)(int32_t handle, int32_t classHandle),
	void (*unityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeConstructor)(int32_t cppHandle, int32_t* handle, int32_t* classHandle),
	void (*unityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeAdd)(int32_t thisHandle, int32_t delHandle),
	void (*unityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeRemove)(int32_t thisHandle, int32_t delHandle),
	void (*unityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeInvoke)(int32_t thisHandle, UnityEngine::SceneManagement::Scene& arg0, UnityEngine::SceneManagement::LoadSceneMode arg1)
	/*END INIT PARAMS*/)
{
	using namespace Plugin;
	
	// Init managed object ref counting
	Plugin::RefCountsLenClass = maxManagedObjects;
	Plugin::RefCountsClass = new int32_t[maxManagedObjects]();
	
	// Init pointers to C# functions
	Plugin::StringNew = stringNew;
	Plugin::ReleaseObject = releaseObject;
	Plugin::SetException = setException;
	Plugin::ArrayGetLength = arrayGetLength;
	Plugin::ArrayGetRank = arrayGetRank;
	/*BEGIN INIT BODY*/
	Plugin::SystemDiagnosticsStopwatchConstructor = systemDiagnosticsStopwatchConstructor;
	Plugin::SystemDiagnosticsStopwatchPropertyGetElapsedMilliseconds = systemDiagnosticsStopwatchPropertyGetElapsedMilliseconds;
	Plugin::SystemDiagnosticsStopwatchMethodStart = systemDiagnosticsStopwatchMethodStart;
	Plugin::SystemDiagnosticsStopwatchMethodReset = systemDiagnosticsStopwatchMethodReset;
	Plugin::UnityEngineObjectPropertyGetName = unityEngineObjectPropertyGetName;
	Plugin::UnityEngineObjectPropertySetName = unityEngineObjectPropertySetName;
	Plugin::UnityEngineObjectMethodop_EqualityUnityEngineObject_UnityEngineObject = unityEngineObjectMethodop_EqualityUnityEngineObject_UnityEngineObject;
	Plugin::UnityEngineObjectMethodop_ImplicitUnityEngineObject = unityEngineObjectMethodop_ImplicitUnityEngineObject;
	Plugin::UnityEngineGameObjectConstructor = unityEngineGameObjectConstructor;
	Plugin::UnityEngineGameObjectConstructorSystemString = unityEngineGameObjectConstructorSystemString;
	Plugin::UnityEngineGameObjectPropertyGetTransform = unityEngineGameObjectPropertyGetTransform;
	Plugin::UnityEngineGameObjectMethodAddComponentMyGameMonoBehavioursTestScript = unityEngineGameObjectMethodAddComponentMyGameMonoBehavioursTestScript;
	Plugin::UnityEngineComponentPropertyGetTransform = unityEngineComponentPropertyGetTransform;
	Plugin::UnityEngineTransformPropertyGetPosition = unityEngineTransformPropertyGetPosition;
	Plugin::UnityEngineTransformPropertySetPosition = unityEngineTransformPropertySetPosition;
	Plugin::UnityEngineDebugMethodLogSystemObject = unityEngineDebugMethodLogSystemObject;
	Plugin::UnityEngineAssertionsAssertFieldGetRaiseExceptions = unityEngineAssertionsAssertFieldGetRaiseExceptions;
	Plugin::UnityEngineAssertionsAssertFieldSetRaiseExceptions = unityEngineAssertionsAssertFieldSetRaiseExceptions;
	Plugin::UnityEngineAssertionsAssertMethodAreEqualSystemStringSystemString_SystemString = unityEngineAssertionsAssertMethodAreEqualSystemStringSystemString_SystemString;
	Plugin::UnityEngineAssertionsAssertMethodAreEqualUnityEngineGameObjectUnityEngineGameObject_UnityEngineGameObject = unityEngineAssertionsAssertMethodAreEqualUnityEngineGameObjectUnityEngineGameObject_UnityEngineGameObject;
	Plugin::UnityEngineAudioSettingsMethodGetDSPBufferSizeSystemInt32_SystemInt32 = unityEngineAudioSettingsMethodGetDSPBufferSizeSystemInt32_SystemInt32;
	Plugin::UnityEngineNetworkingNetworkTransportMethodGetBroadcastConnectionInfoSystemInt32_SystemString_SystemInt32_SystemByte = unityEngineNetworkingNetworkTransportMethodGetBroadcastConnectionInfoSystemInt32_SystemString_SystemInt32_SystemByte;
	Plugin::UnityEngineNetworkingNetworkTransportMethodInit = unityEngineNetworkingNetworkTransportMethodInit;
	Plugin::UnityEngineVector3ConstructorSystemSingle_SystemSingle_SystemSingle = unityEngineVector3ConstructorSystemSingle_SystemSingle_SystemSingle;
	Plugin::UnityEngineVector3PropertyGetMagnitude = unityEngineVector3PropertyGetMagnitude;
	Plugin::UnityEngineVector3MethodSetSystemSingle_SystemSingle_SystemSingle = unityEngineVector3MethodSetSystemSingle_SystemSingle_SystemSingle;
	Plugin::UnityEngineVector3Methodop_AdditionUnityEngineVector3_UnityEngineVector3 = unityEngineVector3Methodop_AdditionUnityEngineVector3_UnityEngineVector3;
	Plugin::UnityEngineVector3Methodop_UnaryNegationUnityEngineVector3 = unityEngineVector3Methodop_UnaryNegationUnityEngineVector3;
	Plugin::BoxVector3 = boxVector3;
	Plugin::UnboxVector3 = unboxVector3;
	Plugin::UnityEngineMatrix4x4PropertyGetItem = unityEngineMatrix4x4PropertyGetItem;
	Plugin::UnityEngineMatrix4x4PropertySetItem = unityEngineMatrix4x4PropertySetItem;
	Plugin::BoxMatrix4x4 = boxMatrix4x4;
	Plugin::UnboxMatrix4x4 = unboxMatrix4x4;
	Plugin::ReleaseUnityEngineRaycastHit = releaseUnityEngineRaycastHit;
	Plugin::RefCountsUnityEngineRaycastHit = new int32_t[1000]();
	Plugin::UnityEngineRaycastHitPropertyGetPoint = unityEngineRaycastHitPropertyGetPoint;
	Plugin::UnityEngineRaycastHitPropertySetPoint = unityEngineRaycastHitPropertySetPoint;
	Plugin::UnityEngineRaycastHitPropertyGetTransform = unityEngineRaycastHitPropertyGetTransform;
	Plugin::BoxRaycastHit = boxRaycastHit;
	Plugin::UnboxRaycastHit = unboxRaycastHit;
	Plugin::BoxQueryTriggerInteraction = boxQueryTriggerInteraction;
	Plugin::UnboxQueryTriggerInteraction = unboxQueryTriggerInteraction;
	Plugin::ReleaseSystemCollectionsGenericKeyValuePairSystemString_SystemDouble = releaseSystemCollectionsGenericKeyValuePairSystemString_SystemDouble;
	Plugin::RefCountsSystemCollectionsGenericKeyValuePairSystemString_SystemDouble = new int32_t[maxManagedObjects]();
	Plugin::SystemCollectionsGenericKeyValuePairSystemString_SystemDoubleConstructorSystemString_SystemDouble = systemCollectionsGenericKeyValuePairSystemString_SystemDoubleConstructorSystemString_SystemDouble;
	Plugin::SystemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetKey = systemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetKey;
	Plugin::SystemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetValue = systemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetValue;
	Plugin::BoxKeyValuePairSystemString_SystemDouble = boxKeyValuePairSystemString_SystemDouble;
	Plugin::UnboxKeyValuePairSystemString_SystemDouble = unboxKeyValuePairSystemString_SystemDouble;
	Plugin::SystemCollectionsGenericListSystemStringConstructor = systemCollectionsGenericListSystemStringConstructor;
	Plugin::SystemCollectionsGenericListSystemStringPropertyGetItem = systemCollectionsGenericListSystemStringPropertyGetItem;
	Plugin::SystemCollectionsGenericListSystemStringPropertySetItem = systemCollectionsGenericListSystemStringPropertySetItem;
	Plugin::SystemCollectionsGenericListSystemStringMethodAddSystemString = systemCollectionsGenericListSystemStringMethodAddSystemString;
	Plugin::SystemCollectionsGenericListSystemStringMethodSortSystemCollectionsGenericIComparer = systemCollectionsGenericListSystemStringMethodSortSystemCollectionsGenericIComparer;
	Plugin::SystemCollectionsGenericListSystemInt32Constructor = systemCollectionsGenericListSystemInt32Constructor;
	Plugin::SystemCollectionsGenericListSystemInt32PropertyGetItem = systemCollectionsGenericListSystemInt32PropertyGetItem;
	Plugin::SystemCollectionsGenericListSystemInt32PropertySetItem = systemCollectionsGenericListSystemInt32PropertySetItem;
	Plugin::SystemCollectionsGenericListSystemInt32MethodAddSystemInt32 = systemCollectionsGenericListSystemInt32MethodAddSystemInt32;
	Plugin::SystemCollectionsGenericListSystemInt32MethodSortSystemCollectionsGenericIComparer = systemCollectionsGenericListSystemInt32MethodSortSystemCollectionsGenericIComparer;
	Plugin::SystemCollectionsGenericLinkedListNodeSystemStringConstructorSystemString = systemCollectionsGenericLinkedListNodeSystemStringConstructorSystemString;
	Plugin::SystemCollectionsGenericLinkedListNodeSystemStringPropertyGetValue = systemCollectionsGenericLinkedListNodeSystemStringPropertyGetValue;
	Plugin::SystemCollectionsGenericLinkedListNodeSystemStringPropertySetValue = systemCollectionsGenericLinkedListNodeSystemStringPropertySetValue;
	Plugin::SystemRuntimeCompilerServicesStrongBoxSystemStringConstructorSystemString = systemRuntimeCompilerServicesStrongBoxSystemStringConstructorSystemString;
	Plugin::SystemRuntimeCompilerServicesStrongBoxSystemStringFieldGetValue = systemRuntimeCompilerServicesStrongBoxSystemStringFieldGetValue;
	Plugin::SystemRuntimeCompilerServicesStrongBoxSystemStringFieldSetValue = systemRuntimeCompilerServicesStrongBoxSystemStringFieldSetValue;
	Plugin::SystemExceptionConstructorSystemString = systemExceptionConstructorSystemString;
	Plugin::UnityEngineResolutionPropertyGetWidth = unityEngineResolutionPropertyGetWidth;
	Plugin::UnityEngineResolutionPropertySetWidth = unityEngineResolutionPropertySetWidth;
	Plugin::UnityEngineResolutionPropertyGetHeight = unityEngineResolutionPropertyGetHeight;
	Plugin::UnityEngineResolutionPropertySetHeight = unityEngineResolutionPropertySetHeight;
	Plugin::UnityEngineResolutionPropertyGetRefreshRate = unityEngineResolutionPropertyGetRefreshRate;
	Plugin::UnityEngineResolutionPropertySetRefreshRate = unityEngineResolutionPropertySetRefreshRate;
	Plugin::BoxResolution = boxResolution;
	Plugin::UnboxResolution = unboxResolution;
	Plugin::UnityEngineScreenPropertyGetResolutions = unityEngineScreenPropertyGetResolutions;
	Plugin::UnityEngineRayConstructorUnityEngineVector3_UnityEngineVector3 = unityEngineRayConstructorUnityEngineVector3_UnityEngineVector3;
	Plugin::BoxRay = boxRay;
	Plugin::UnboxRay = unboxRay;
	Plugin::UnityEnginePhysicsMethodRaycastNonAllocUnityEngineRay_UnityEngineRaycastHit = unityEnginePhysicsMethodRaycastNonAllocUnityEngineRay_UnityEngineRaycastHit;
	Plugin::UnityEnginePhysicsMethodRaycastAllUnityEngineRay = unityEnginePhysicsMethodRaycastAllUnityEngineRay;
	Plugin::BoxColor = boxColor;
	Plugin::UnboxColor = unboxColor;
	Plugin::BoxGradientColorKey = boxGradientColorKey;
	Plugin::UnboxGradientColorKey = unboxGradientColorKey;
	Plugin::UnityEngineGradientConstructor = unityEngineGradientConstructor;
	Plugin::UnityEngineGradientPropertyGetColorKeys = unityEngineGradientPropertyGetColorKeys;
	Plugin::UnityEngineGradientPropertySetColorKeys = unityEngineGradientPropertySetColorKeys;
	Plugin::SystemAppDomainSetupConstructor = systemAppDomainSetupConstructor;
	Plugin::SystemAppDomainSetupPropertyGetAppDomainInitializer = systemAppDomainSetupPropertyGetAppDomainInitializer;
	Plugin::SystemAppDomainSetupPropertySetAppDomainInitializer = systemAppDomainSetupPropertySetAppDomainInitializer;
	Plugin::UnityEngineApplicationAddEventOnBeforeRender = unityEngineApplicationAddEventOnBeforeRender;
	Plugin::UnityEngineApplicationRemoveEventOnBeforeRender = unityEngineApplicationRemoveEventOnBeforeRender;
	Plugin::UnityEngineSceneManagementSceneManagerAddEventSceneLoaded = unityEngineSceneManagementSceneManagerAddEventSceneLoaded;
	Plugin::UnityEngineSceneManagementSceneManagerRemoveEventSceneLoaded = unityEngineSceneManagementSceneManagerRemoveEventSceneLoaded;
	Plugin::BoxScene = boxScene;
	Plugin::UnboxScene = unboxScene;
	Plugin::BoxLoadSceneMode = boxLoadSceneMode;
	Plugin::UnboxLoadSceneMode = unboxLoadSceneMode;
	SystemCollectionsGenericIComparerSystemInt32FreeListSize = maxManagedObjects;
	SystemCollectionsGenericIComparerSystemInt32FreeList = new System::Collections::Generic::IComparer<int32_t>*[SystemCollectionsGenericIComparerSystemInt32FreeListSize];
	for (int32_t i = 0, end = SystemCollectionsGenericIComparerSystemInt32FreeListSize - 1; i < end; ++i)
	{
		SystemCollectionsGenericIComparerSystemInt32FreeList[i] = (System::Collections::Generic::IComparer<int32_t>*)(SystemCollectionsGenericIComparerSystemInt32FreeList + i + 1);
	}
	SystemCollectionsGenericIComparerSystemInt32FreeList[SystemCollectionsGenericIComparerSystemInt32FreeListSize - 1] = nullptr;
	NextFreeSystemCollectionsGenericIComparerSystemInt32 = SystemCollectionsGenericIComparerSystemInt32FreeList + 1;
	Plugin::ReleaseSystemCollectionsGenericIComparerSystemInt32 = releaseSystemCollectionsGenericIComparerSystemInt32;
	Plugin::SystemCollectionsGenericIComparerSystemInt32Constructor = systemCollectionsGenericIComparerSystemInt32Constructor;
	SystemCollectionsGenericIComparerSystemStringFreeListSize = maxManagedObjects;
	SystemCollectionsGenericIComparerSystemStringFreeList = new System::Collections::Generic::IComparer<System::String>*[SystemCollectionsGenericIComparerSystemStringFreeListSize];
	for (int32_t i = 0, end = SystemCollectionsGenericIComparerSystemStringFreeListSize - 1; i < end; ++i)
	{
		SystemCollectionsGenericIComparerSystemStringFreeList[i] = (System::Collections::Generic::IComparer<System::String>*)(SystemCollectionsGenericIComparerSystemStringFreeList + i + 1);
	}
	SystemCollectionsGenericIComparerSystemStringFreeList[SystemCollectionsGenericIComparerSystemStringFreeListSize - 1] = nullptr;
	NextFreeSystemCollectionsGenericIComparerSystemString = SystemCollectionsGenericIComparerSystemStringFreeList + 1;
	Plugin::ReleaseSystemCollectionsGenericIComparerSystemString = releaseSystemCollectionsGenericIComparerSystemString;
	Plugin::SystemCollectionsGenericIComparerSystemStringConstructor = systemCollectionsGenericIComparerSystemStringConstructor;
	SystemStringComparerFreeListSize = maxManagedObjects;
	SystemStringComparerFreeList = new System::StringComparer*[SystemStringComparerFreeListSize];
	for (int32_t i = 0, end = SystemStringComparerFreeListSize - 1; i < end; ++i)
	{
		SystemStringComparerFreeList[i] = (System::StringComparer*)(SystemStringComparerFreeList + i + 1);
	}
	SystemStringComparerFreeList[SystemStringComparerFreeListSize - 1] = nullptr;
	NextFreeSystemStringComparer = SystemStringComparerFreeList + 1;
	Plugin::ReleaseSystemStringComparer = releaseSystemStringComparer;
	Plugin::SystemStringComparerConstructor = systemStringComparerConstructor;
	SystemEventArgsFreeListSize = maxManagedObjects;
	SystemEventArgsFreeList = new System::EventArgs*[SystemEventArgsFreeListSize];
	for (int32_t i = 0, end = SystemEventArgsFreeListSize - 1; i < end; ++i)
	{
		SystemEventArgsFreeList[i] = (System::EventArgs*)(SystemEventArgsFreeList + i + 1);
	}
	SystemEventArgsFreeList[SystemEventArgsFreeListSize - 1] = nullptr;
	NextFreeSystemEventArgs = SystemEventArgsFreeList + 1;
	Plugin::ReleaseSystemEventArgs = releaseSystemEventArgs;
	Plugin::SystemEventArgsConstructor = systemEventArgsConstructor;
	Plugin::BoxBoolean = boxBoolean;
	Plugin::UnboxBoolean = unboxBoolean;
	Plugin::BoxSByte = boxSByte;
	Plugin::UnboxSByte = unboxSByte;
	Plugin::BoxByte = boxByte;
	Plugin::UnboxByte = unboxByte;
	Plugin::BoxInt16 = boxInt16;
	Plugin::UnboxInt16 = unboxInt16;
	Plugin::BoxUInt16 = boxUInt16;
	Plugin::UnboxUInt16 = unboxUInt16;
	Plugin::BoxInt32 = boxInt32;
	Plugin::UnboxInt32 = unboxInt32;
	Plugin::BoxUInt32 = boxUInt32;
	Plugin::UnboxUInt32 = unboxUInt32;
	Plugin::BoxInt64 = boxInt64;
	Plugin::UnboxInt64 = unboxInt64;
	Plugin::BoxUInt64 = boxUInt64;
	Plugin::UnboxUInt64 = unboxUInt64;
	Plugin::BoxChar = boxChar;
	Plugin::UnboxChar = unboxChar;
	Plugin::BoxSingle = boxSingle;
	Plugin::UnboxSingle = unboxSingle;
	Plugin::BoxDouble = boxDouble;
	Plugin::UnboxDouble = unboxDouble;
	Plugin::SystemSystemInt32Array1Constructor1 = systemSystemInt32Array1Constructor1;
	Plugin::SystemInt32Array1GetItem1 = systemInt32Array1GetItem1;
	Plugin::SystemInt32Array1SetItem1 = systemInt32Array1SetItem1;
	Plugin::SystemSystemSingleArray1Constructor1 = systemSystemSingleArray1Constructor1;
	Plugin::SystemSingleArray1GetItem1 = systemSingleArray1GetItem1;
	Plugin::SystemSingleArray1SetItem1 = systemSingleArray1SetItem1;
	Plugin::SystemSystemSingleArray2Constructor2 = systemSystemSingleArray2Constructor2;
	Plugin::SystemSystemSingleArray2GetLength2 = systemSystemSingleArray2GetLength2;
	Plugin::SystemSingleArray2GetItem2 = systemSingleArray2GetItem2;
	Plugin::SystemSingleArray2SetItem2 = systemSingleArray2SetItem2;
	Plugin::SystemSystemSingleArray3Constructor3 = systemSystemSingleArray3Constructor3;
	Plugin::SystemSystemSingleArray3GetLength3 = systemSystemSingleArray3GetLength3;
	Plugin::SystemSingleArray3GetItem3 = systemSingleArray3GetItem3;
	Plugin::SystemSingleArray3SetItem3 = systemSingleArray3SetItem3;
	Plugin::SystemSystemStringArray1Constructor1 = systemSystemStringArray1Constructor1;
	Plugin::SystemStringArray1GetItem1 = systemStringArray1GetItem1;
	Plugin::SystemStringArray1SetItem1 = systemStringArray1SetItem1;
	Plugin::UnityEngineUnityEngineResolutionArray1Constructor1 = unityEngineUnityEngineResolutionArray1Constructor1;
	Plugin::UnityEngineResolutionArray1GetItem1 = unityEngineResolutionArray1GetItem1;
	Plugin::UnityEngineResolutionArray1SetItem1 = unityEngineResolutionArray1SetItem1;
	Plugin::UnityEngineUnityEngineRaycastHitArray1Constructor1 = unityEngineUnityEngineRaycastHitArray1Constructor1;
	Plugin::UnityEngineRaycastHitArray1GetItem1 = unityEngineRaycastHitArray1GetItem1;
	Plugin::UnityEngineRaycastHitArray1SetItem1 = unityEngineRaycastHitArray1SetItem1;
	Plugin::UnityEngineUnityEngineGradientColorKeyArray1Constructor1 = unityEngineUnityEngineGradientColorKeyArray1Constructor1;
	Plugin::UnityEngineGradientColorKeyArray1GetItem1 = unityEngineGradientColorKeyArray1GetItem1;
	Plugin::UnityEngineGradientColorKeyArray1SetItem1 = unityEngineGradientColorKeyArray1SetItem1;
	SystemActionFreeListSize = maxManagedObjects;
	SystemActionFreeList = new System::Action*[SystemActionFreeListSize];
	for (int32_t i = 0, end = SystemActionFreeListSize - 1; i < end; ++i)
	{
		SystemActionFreeList[i] = (System::Action*)(SystemActionFreeList + i + 1);
	}
	SystemActionFreeList[SystemActionFreeListSize - 1] = nullptr;
	NextFreeSystemAction = SystemActionFreeList + 1;
	Plugin::ReleaseSystemAction = releaseSystemAction;
	Plugin::SystemActionConstructor = systemActionConstructor;
	Plugin::SystemActionAdd = systemActionAdd;
	Plugin::SystemActionRemove = systemActionRemove;
	Plugin::SystemActionInvoke = systemActionInvoke;
	SystemActionSystemSingleFreeListSize = maxManagedObjects;
	SystemActionSystemSingleFreeList = new System::Action1<float>*[SystemActionSystemSingleFreeListSize];
	for (int32_t i = 0, end = SystemActionSystemSingleFreeListSize - 1; i < end; ++i)
	{
		SystemActionSystemSingleFreeList[i] = (System::Action1<float>*)(SystemActionSystemSingleFreeList + i + 1);
	}
	SystemActionSystemSingleFreeList[SystemActionSystemSingleFreeListSize - 1] = nullptr;
	NextFreeSystemActionSystemSingle = SystemActionSystemSingleFreeList + 1;
	Plugin::ReleaseSystemActionSystemSingle = releaseSystemActionSystemSingle;
	Plugin::SystemActionSystemSingleConstructor = systemActionSystemSingleConstructor;
	Plugin::SystemActionSystemSingleAdd = systemActionSystemSingleAdd;
	Plugin::SystemActionSystemSingleRemove = systemActionSystemSingleRemove;
	Plugin::SystemActionSystemSingleInvoke = systemActionSystemSingleInvoke;
	SystemActionSystemSingle_SystemSingleFreeListSize = 100;
	SystemActionSystemSingle_SystemSingleFreeList = new System::Action2<float, float>*[SystemActionSystemSingle_SystemSingleFreeListSize];
	for (int32_t i = 0, end = SystemActionSystemSingle_SystemSingleFreeListSize - 1; i < end; ++i)
	{
		SystemActionSystemSingle_SystemSingleFreeList[i] = (System::Action2<float, float>*)(SystemActionSystemSingle_SystemSingleFreeList + i + 1);
	}
	SystemActionSystemSingle_SystemSingleFreeList[SystemActionSystemSingle_SystemSingleFreeListSize - 1] = nullptr;
	NextFreeSystemActionSystemSingle_SystemSingle = SystemActionSystemSingle_SystemSingleFreeList + 1;
	Plugin::ReleaseSystemActionSystemSingle_SystemSingle = releaseSystemActionSystemSingle_SystemSingle;
	Plugin::SystemActionSystemSingle_SystemSingleConstructor = systemActionSystemSingle_SystemSingleConstructor;
	Plugin::SystemActionSystemSingle_SystemSingleAdd = systemActionSystemSingle_SystemSingleAdd;
	Plugin::SystemActionSystemSingle_SystemSingleRemove = systemActionSystemSingle_SystemSingleRemove;
	Plugin::SystemActionSystemSingle_SystemSingleInvoke = systemActionSystemSingle_SystemSingleInvoke;
	SystemFuncSystemInt32_SystemSingle_SystemDoubleFreeListSize = 50;
	SystemFuncSystemInt32_SystemSingle_SystemDoubleFreeList = new System::Func3<int32_t, float, double>*[SystemFuncSystemInt32_SystemSingle_SystemDoubleFreeListSize];
	for (int32_t i = 0, end = SystemFuncSystemInt32_SystemSingle_SystemDoubleFreeListSize - 1; i < end; ++i)
	{
		SystemFuncSystemInt32_SystemSingle_SystemDoubleFreeList[i] = (System::Func3<int32_t, float, double>*)(SystemFuncSystemInt32_SystemSingle_SystemDoubleFreeList + i + 1);
	}
	SystemFuncSystemInt32_SystemSingle_SystemDoubleFreeList[SystemFuncSystemInt32_SystemSingle_SystemDoubleFreeListSize - 1] = nullptr;
	NextFreeSystemFuncSystemInt32_SystemSingle_SystemDouble = SystemFuncSystemInt32_SystemSingle_SystemDoubleFreeList + 1;
	Plugin::ReleaseSystemFuncSystemInt32_SystemSingle_SystemDouble = releaseSystemFuncSystemInt32_SystemSingle_SystemDouble;
	Plugin::SystemFuncSystemInt32_SystemSingle_SystemDoubleConstructor = systemFuncSystemInt32_SystemSingle_SystemDoubleConstructor;
	Plugin::SystemFuncSystemInt32_SystemSingle_SystemDoubleAdd = systemFuncSystemInt32_SystemSingle_SystemDoubleAdd;
	Plugin::SystemFuncSystemInt32_SystemSingle_SystemDoubleRemove = systemFuncSystemInt32_SystemSingle_SystemDoubleRemove;
	Plugin::SystemFuncSystemInt32_SystemSingle_SystemDoubleInvoke = systemFuncSystemInt32_SystemSingle_SystemDoubleInvoke;
	SystemFuncSystemInt16_SystemInt32_SystemStringFreeListSize = 25;
	SystemFuncSystemInt16_SystemInt32_SystemStringFreeList = new System::Func3<int16_t, int32_t, System::String>*[SystemFuncSystemInt16_SystemInt32_SystemStringFreeListSize];
	for (int32_t i = 0, end = SystemFuncSystemInt16_SystemInt32_SystemStringFreeListSize - 1; i < end; ++i)
	{
		SystemFuncSystemInt16_SystemInt32_SystemStringFreeList[i] = (System::Func3<int16_t, int32_t, System::String>*)(SystemFuncSystemInt16_SystemInt32_SystemStringFreeList + i + 1);
	}
	SystemFuncSystemInt16_SystemInt32_SystemStringFreeList[SystemFuncSystemInt16_SystemInt32_SystemStringFreeListSize - 1] = nullptr;
	NextFreeSystemFuncSystemInt16_SystemInt32_SystemString = SystemFuncSystemInt16_SystemInt32_SystemStringFreeList + 1;
	Plugin::ReleaseSystemFuncSystemInt16_SystemInt32_SystemString = releaseSystemFuncSystemInt16_SystemInt32_SystemString;
	Plugin::SystemFuncSystemInt16_SystemInt32_SystemStringConstructor = systemFuncSystemInt16_SystemInt32_SystemStringConstructor;
	Plugin::SystemFuncSystemInt16_SystemInt32_SystemStringAdd = systemFuncSystemInt16_SystemInt32_SystemStringAdd;
	Plugin::SystemFuncSystemInt16_SystemInt32_SystemStringRemove = systemFuncSystemInt16_SystemInt32_SystemStringRemove;
	Plugin::SystemFuncSystemInt16_SystemInt32_SystemStringInvoke = systemFuncSystemInt16_SystemInt32_SystemStringInvoke;
	SystemAppDomainInitializerFreeListSize = maxManagedObjects;
	SystemAppDomainInitializerFreeList = new System::AppDomainInitializer*[SystemAppDomainInitializerFreeListSize];
	for (int32_t i = 0, end = SystemAppDomainInitializerFreeListSize - 1; i < end; ++i)
	{
		SystemAppDomainInitializerFreeList[i] = (System::AppDomainInitializer*)(SystemAppDomainInitializerFreeList + i + 1);
	}
	SystemAppDomainInitializerFreeList[SystemAppDomainInitializerFreeListSize - 1] = nullptr;
	NextFreeSystemAppDomainInitializer = SystemAppDomainInitializerFreeList + 1;
	Plugin::ReleaseSystemAppDomainInitializer = releaseSystemAppDomainInitializer;
	Plugin::SystemAppDomainInitializerConstructor = systemAppDomainInitializerConstructor;
	Plugin::SystemAppDomainInitializerAdd = systemAppDomainInitializerAdd;
	Plugin::SystemAppDomainInitializerRemove = systemAppDomainInitializerRemove;
	Plugin::SystemAppDomainInitializerInvoke = systemAppDomainInitializerInvoke;
	UnityEngineEventsUnityActionFreeListSize = maxManagedObjects;
	UnityEngineEventsUnityActionFreeList = new UnityEngine::Events::UnityAction*[UnityEngineEventsUnityActionFreeListSize];
	for (int32_t i = 0, end = UnityEngineEventsUnityActionFreeListSize - 1; i < end; ++i)
	{
		UnityEngineEventsUnityActionFreeList[i] = (UnityEngine::Events::UnityAction*)(UnityEngineEventsUnityActionFreeList + i + 1);
	}
	UnityEngineEventsUnityActionFreeList[UnityEngineEventsUnityActionFreeListSize - 1] = nullptr;
	NextFreeUnityEngineEventsUnityAction = UnityEngineEventsUnityActionFreeList + 1;
	Plugin::ReleaseUnityEngineEventsUnityAction = releaseUnityEngineEventsUnityAction;
	Plugin::UnityEngineEventsUnityActionConstructor = unityEngineEventsUnityActionConstructor;
	Plugin::UnityEngineEventsUnityActionAdd = unityEngineEventsUnityActionAdd;
	Plugin::UnityEngineEventsUnityActionRemove = unityEngineEventsUnityActionRemove;
	Plugin::UnityEngineEventsUnityActionInvoke = unityEngineEventsUnityActionInvoke;
	UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeFreeListSize = 10;
	UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeFreeList = new UnityEngine::Events::UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>*[UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeFreeListSize];
	for (int32_t i = 0, end = UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeFreeListSize - 1; i < end; ++i)
	{
		UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeFreeList[i] = (UnityEngine::Events::UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>*)(UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeFreeList + i + 1);
	}
	UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeFreeList[UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeFreeListSize - 1] = nullptr;
	NextFreeUnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneMode = UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeFreeList + 1;
	Plugin::ReleaseUnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneMode = releaseUnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneMode;
	Plugin::UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeConstructor = unityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeConstructor;
	Plugin::UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeAdd = unityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeAdd;
	Plugin::UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeRemove = unityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeRemove;
	Plugin::UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeInvoke = unityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeInvoke;
	/*END INIT BODY*/
	
	try
	{
		PluginMain();
	}
	catch (System::Exception ex)
	{
		Plugin::SetException(ex.Handle);
	}
	catch (...)
	{
		System::String msg = "Unhandled exception in PluginMain";
		System::Exception ex(msg);
		Plugin::SetException(ex.Handle);
	}
}

// Receive an unhandled exception from C#
DLLEXPORT void SetCsharpException(int32_t handle)
{
	Plugin::unhandledCsharpException = new System::Exception(
		Plugin::InternalUse::Only,
		handle);
}

/*BEGIN MONOBEHAVIOUR MESSAGES*/
DLLEXPORT void MyGameMonoBehavioursTestScriptAwake(int32_t thisHandle)
{
	MyGame::MonoBehaviours::TestScript thiz(Plugin::InternalUse::Only, thisHandle);
	try
	{
		thiz.Awake();
	}
	catch (System::Exception ex)
	{
		Plugin::SetException(ex.Handle);
	}
	catch (...)
	{
		System::String msg = "Unhandled exception in MyGame::MonoBehaviours::TestScript::Awake";
		System::Exception ex(msg);
		Plugin::SetException(ex.Handle);
	}
}


DLLEXPORT void MyGameMonoBehavioursTestScriptOnAnimatorIK(int32_t thisHandle, int32_t param0)
{
	MyGame::MonoBehaviours::TestScript thiz(Plugin::InternalUse::Only, thisHandle);
	try
	{
		thiz.OnAnimatorIK(param0);
	}
	catch (System::Exception ex)
	{
		Plugin::SetException(ex.Handle);
	}
	catch (...)
	{
		System::String msg = "Unhandled exception in MyGame::MonoBehaviours::TestScript::OnAnimatorIK";
		System::Exception ex(msg);
		Plugin::SetException(ex.Handle);
	}
}


DLLEXPORT void MyGameMonoBehavioursTestScriptOnCollisionEnter(int32_t thisHandle, int32_t param0Handle)
{
	MyGame::MonoBehaviours::TestScript thiz(Plugin::InternalUse::Only, thisHandle);
	UnityEngine::Collision param0(Plugin::InternalUse::Only, param0Handle);
	try
	{
		thiz.OnCollisionEnter(param0);
	}
	catch (System::Exception ex)
	{
		Plugin::SetException(ex.Handle);
	}
	catch (...)
	{
		System::String msg = "Unhandled exception in MyGame::MonoBehaviours::TestScript::OnCollisionEnter";
		System::Exception ex(msg);
		Plugin::SetException(ex.Handle);
	}
}


DLLEXPORT void MyGameMonoBehavioursTestScriptUpdate(int32_t thisHandle)
{
	MyGame::MonoBehaviours::TestScript thiz(Plugin::InternalUse::Only, thisHandle);
	try
	{
		thiz.Update();
	}
	catch (System::Exception ex)
	{
		Plugin::SetException(ex.Handle);
	}
	catch (...)
	{
		System::String msg = "Unhandled exception in MyGame::MonoBehaviours::TestScript::Update";
		System::Exception ex(msg);
		Plugin::SetException(ex.Handle);
	}
}
/*END MONOBEHAVIOUR MESSAGES*/

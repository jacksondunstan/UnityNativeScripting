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

// For memset(), etc.
#include <string.h>

// Macro to put before functions that need to be exposed to C#
#ifdef _WIN32
	#define DLLEXPORT extern "C" __declspec(dllexport)
#else
	#define DLLEXPORT extern "C"
#endif

////////////////////////////////////////////////////////////////
// Global variables
////////////////////////////////////////////////////////////////

namespace Plugin
{
	System::String NullString(nullptr);
}

////////////////////////////////////////////////////////////////
// Support for using IEnumerable with range for loops
////////////////////////////////////////////////////////////////

namespace Plugin
{
	// End iterators are dummies full of null
	EnumerableIterator::EnumerableIterator(decltype(nullptr))
		: enumerator(nullptr)
		, hasMore(false)
	{
	}
	
	// Begin iterators keep track of an IEnumerator
	EnumerableIterator::EnumerableIterator(
		System::Collections::IEnumerable& enumerable)
		: enumerator(enumerable.GetEnumerator())
	{
		hasMore = enumerator.MoveNext();
	}
	
	EnumerableIterator& EnumerableIterator::operator++()
	{
		hasMore = enumerator.MoveNext();
		return *this;
	}
	
	bool EnumerableIterator::operator!=(const EnumerableIterator& other)
	{
		return hasMore;
	}
	
	System::Object EnumerableIterator::operator*()
	{
		return enumerator.GetCurrent();
	}
}

////////////////////////////////////////////////////////////////
// C# functions for C++ to call
////////////////////////////////////////////////////////////////

namespace Plugin
{
	void (*ReleaseObject)(int32_t handle);
	int32_t (*StringNew)(const char* chars);
	void (*SetException)(int32_t handle);
	int32_t (*ArrayGetLength)(int32_t handle);
	int32_t (*EnumerableGetEnumerator)(int32_t handle);
	
	/*BEGIN FUNCTION POINTERS*/
	void (*SystemIDisposableMethodDispose)(int32_t thisHandle);
	UnityEngine::Vector3 (*UnityEngineVector3ConstructorSystemSingle_SystemSingle_SystemSingle)(float x, float y, float z);
	float (*UnityEngineVector3PropertyGetMagnitude)(UnityEngine::Vector3* thiz);
	void (*UnityEngineVector3MethodSetSystemSingle_SystemSingle_SystemSingle)(UnityEngine::Vector3* thiz, float newX, float newY, float newZ);
	UnityEngine::Vector3 (*UnityEngineVector3Methodop_AdditionUnityEngineVector3_UnityEngineVector3)(UnityEngine::Vector3& a, UnityEngine::Vector3& b);
	UnityEngine::Vector3 (*UnityEngineVector3Methodop_UnaryNegationUnityEngineVector3)(UnityEngine::Vector3& a);
	int32_t (*BoxVector3)(UnityEngine::Vector3& val);
	UnityEngine::Vector3 (*UnboxVector3)(int32_t valHandle);
	int32_t (*UnityEngineObjectPropertyGetName)(int32_t thisHandle);
	void (*UnityEngineObjectPropertySetName)(int32_t thisHandle, int32_t valueHandle);
	int32_t (*UnityEngineObjectMethodop_EqualityUnityEngineObject_UnityEngineObject)(int32_t xHandle, int32_t yHandle);
	int32_t (*UnityEngineObjectMethodop_ImplicitUnityEngineObject)(int32_t existsHandle);
	int32_t (*UnityEngineComponentPropertyGetTransform)(int32_t thisHandle);
	UnityEngine::Vector3 (*UnityEngineTransformPropertyGetPosition)(int32_t thisHandle);
	void (*UnityEngineTransformPropertySetPosition)(int32_t thisHandle, UnityEngine::Vector3& value);
	void (*UnityEngineTransformMethodSetParentUnityEngineTransform)(int32_t thisHandle, int32_t parentHandle);
	int32_t (*BoxColor)(UnityEngine::Color& val);
	UnityEngine::Color (*UnboxColor)(int32_t valHandle);
	int32_t (*BoxGradientColorKey)(UnityEngine::GradientColorKey& val);
	UnityEngine::GradientColorKey (*UnboxGradientColorKey)(int32_t valHandle);
	void (*ReleaseUnityEngineResolution)(int32_t handle);
	int32_t (*UnityEngineResolutionPropertyGetWidth)(int32_t thisHandle);
	void (*UnityEngineResolutionPropertySetWidth)(int32_t thisHandle, int32_t value);
	int32_t (*UnityEngineResolutionPropertyGetHeight)(int32_t thisHandle);
	void (*UnityEngineResolutionPropertySetHeight)(int32_t thisHandle, int32_t value);
	int32_t (*UnityEngineResolutionPropertyGetRefreshRate)(int32_t thisHandle);
	void (*UnityEngineResolutionPropertySetRefreshRate)(int32_t thisHandle, int32_t value);
	int32_t (*BoxResolution)(int32_t valHandle);
	int32_t (*UnboxResolution)(int32_t valHandle);
	void (*ReleaseUnityEngineRaycastHit)(int32_t handle);
	UnityEngine::Vector3 (*UnityEngineRaycastHitPropertyGetPoint)(int32_t thisHandle);
	void (*UnityEngineRaycastHitPropertySetPoint)(int32_t thisHandle, UnityEngine::Vector3& value);
	int32_t (*UnityEngineRaycastHitPropertyGetTransform)(int32_t thisHandle);
	int32_t (*BoxRaycastHit)(int32_t valHandle);
	int32_t (*UnboxRaycastHit)(int32_t valHandle);
	int32_t (*SystemCollectionsIEnumeratorPropertyGetCurrent)(int32_t thisHandle);
	int32_t (*SystemCollectionsIEnumeratorMethodMoveNext)(int32_t thisHandle);
	int32_t (*SystemCollectionsGenericIEnumeratorSystemStringPropertyGetCurrent)(int32_t thisHandle);
	int32_t (*SystemCollectionsGenericIEnumeratorSystemInt32PropertyGetCurrent)(int32_t thisHandle);
	float (*SystemCollectionsGenericIEnumeratorSystemSinglePropertyGetCurrent)(int32_t thisHandle);
	int32_t (*SystemCollectionsGenericIEnumeratorUnityEngineRaycastHitPropertyGetCurrent)(int32_t thisHandle);
	UnityEngine::GradientColorKey (*SystemCollectionsGenericIEnumeratorUnityEngineGradientColorKeyPropertyGetCurrent)(int32_t thisHandle);
	int32_t (*SystemCollectionsGenericIEnumeratorUnityEngineResolutionPropertyGetCurrent)(int32_t thisHandle);
	int32_t (*SystemCollectionsGenericIEnumerableSystemStringMethodGetEnumerator)(int32_t thisHandle);
	int32_t (*SystemCollectionsGenericIEnumerableSystemInt32MethodGetEnumerator)(int32_t thisHandle);
	int32_t (*SystemCollectionsGenericIEnumerableSystemSingleMethodGetEnumerator)(int32_t thisHandle);
	int32_t (*SystemCollectionsGenericIEnumerableUnityEngineRaycastHitMethodGetEnumerator)(int32_t thisHandle);
	int32_t (*SystemCollectionsGenericIEnumerableUnityEngineGradientColorKeyMethodGetEnumerator)(int32_t thisHandle);
	int32_t (*SystemCollectionsGenericIEnumerableUnityEngineResolutionMethodGetEnumerator)(int32_t thisHandle);
	void (*ReleaseUnityEnginePlayablesPlayableGraph)(int32_t handle);
	int32_t (*BoxPlayableGraph)(int32_t valHandle);
	int32_t (*UnboxPlayableGraph)(int32_t valHandle);
	void (*ReleaseUnityEngineAnimationsAnimationMixerPlayable)(int32_t handle);
	int32_t (*UnityEngineAnimationsAnimationMixerPlayableMethodCreateUnityEnginePlayablesPlayableGraph_SystemInt32_SystemBoolean)(int32_t graphHandle, int32_t inputCount, System::Boolean normalizeWeights);
	int32_t (*BoxAnimationMixerPlayable)(int32_t valHandle);
	int32_t (*UnboxAnimationMixerPlayable)(int32_t valHandle);
	int32_t (*SystemDiagnosticsStopwatchConstructor)();
	int64_t (*SystemDiagnosticsStopwatchPropertyGetElapsedMilliseconds)(int32_t thisHandle);
	void (*SystemDiagnosticsStopwatchMethodStart)(int32_t thisHandle);
	void (*SystemDiagnosticsStopwatchMethodReset)(int32_t thisHandle);
	int32_t (*UnityEngineGameObjectConstructor)();
	int32_t (*UnityEngineGameObjectConstructorSystemString)(int32_t nameHandle);
	int32_t (*UnityEngineGameObjectPropertyGetTransform)(int32_t thisHandle);
	int32_t (*UnityEngineGameObjectMethodAddComponentMyGameMonoBehavioursTestScript)(int32_t thisHandle);
	int32_t (*UnityEngineGameObjectMethodAddComponentMyGameMonoBehavioursAnotherScript)(int32_t thisHandle);
	int32_t (*UnityEngineGameObjectMethodCreatePrimitiveUnityEnginePrimitiveType)(UnityEngine::PrimitiveType type);
	void (*UnityEngineDebugMethodLogSystemObject)(int32_t messageHandle);
	int32_t (*UnityEngineAssertionsAssertFieldGetRaiseExceptions)();
	void (*UnityEngineAssertionsAssertFieldSetRaiseExceptions)(System::Boolean value);
	void (*UnityEngineAssertionsAssertMethodAreEqualSystemStringSystemString_SystemString)(int32_t expectedHandle, int32_t actualHandle);
	void (*UnityEngineAssertionsAssertMethodAreEqualUnityEngineGameObjectUnityEngineGameObject_UnityEngineGameObject)(int32_t expectedHandle, int32_t actualHandle);
	int32_t (*UnityEngineMonoBehaviourPropertyGetTransform)(int32_t thisHandle);
	void (*UnityEngineAudioSettingsMethodGetDSPBufferSizeSystemInt32_SystemInt32)(int32_t* bufferLength, int32_t* numBuffers);
	void (*UnityEngineNetworkingNetworkTransportMethodGetBroadcastConnectionInfoSystemInt32_SystemString_SystemInt32_SystemByte)(int32_t hostId, int32_t* addressHandle, int32_t* port, uint8_t* error);
	void (*UnityEngineNetworkingNetworkTransportMethodInit)();
	int32_t (*BoxQuaternion)(UnityEngine::Quaternion& val);
	UnityEngine::Quaternion (*UnboxQuaternion)(int32_t valHandle);
	float (*UnityEngineMatrix4x4PropertyGetItem)(UnityEngine::Matrix4x4* thiz, int32_t row, int32_t column);
	void (*UnityEngineMatrix4x4PropertySetItem)(UnityEngine::Matrix4x4* thiz, int32_t row, int32_t column, float value);
	int32_t (*BoxMatrix4x4)(UnityEngine::Matrix4x4& val);
	UnityEngine::Matrix4x4 (*UnboxMatrix4x4)(int32_t valHandle);
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
	int32_t (*UnityEngineScreenPropertyGetResolutions)();
	void (*ReleaseUnityEngineRay)(int32_t handle);
	int32_t (*UnityEngineRayConstructorUnityEngineVector3_UnityEngineVector3)(UnityEngine::Vector3& origin, UnityEngine::Vector3& direction);
	int32_t (*BoxRay)(int32_t valHandle);
	int32_t (*UnboxRay)(int32_t valHandle);
	int32_t (*UnityEnginePhysicsMethodRaycastNonAllocUnityEngineRay_UnityEngineRaycastHitArray1)(int32_t rayHandle, int32_t resultsHandle);
	int32_t (*UnityEnginePhysicsMethodRaycastAllUnityEngineRay)(int32_t rayHandle);
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
	void (*ReleaseUnityEngineSceneManagementScene)(int32_t handle);
	int32_t (*BoxScene)(int32_t valHandle);
	int32_t (*UnboxScene)(int32_t valHandle);
	int32_t (*BoxLoadSceneMode)(UnityEngine::SceneManagement::LoadSceneMode val);
	UnityEngine::SceneManagement::LoadSceneMode (*UnboxLoadSceneMode)(int32_t valHandle);
	int32_t (*BoxPrimitiveType)(UnityEngine::PrimitiveType val);
	UnityEngine::PrimitiveType (*UnboxPrimitiveType)(int32_t valHandle);
	float (*UnityEngineTimePropertyGetDeltaTime)();
	int32_t (*BoxFileMode)(System::IO::FileMode val);
	System::IO::FileMode (*UnboxFileMode)(int32_t valHandle);
	void (*ReleaseSystemCollectionsGenericBaseIComparerSystemInt32)(int32_t handle);
	void (*SystemCollectionsGenericBaseIComparerSystemInt32Constructor)(int32_t cppHandle, int32_t* handle);
	void (*ReleaseSystemCollectionsGenericBaseIComparerSystemString)(int32_t handle);
	void (*SystemCollectionsGenericBaseIComparerSystemStringConstructor)(int32_t cppHandle, int32_t* handle);
	void (*ReleaseSystemBaseStringComparer)(int32_t handle);
	void (*SystemBaseStringComparerConstructor)(int32_t cppHandle, int32_t* handle);
	int32_t (*SystemCollectionsQueuePropertyGetCount)(int32_t thisHandle);
	void (*ReleaseSystemCollectionsBaseQueue)(int32_t handle);
	void (*SystemCollectionsBaseQueueConstructor)(int32_t cppHandle, int32_t* handle);
	void (*ReleaseSystemComponentModelDesignBaseIComponentChangeService)(int32_t handle);
	void (*SystemComponentModelDesignBaseIComponentChangeServiceConstructor)(int32_t cppHandle, int32_t* handle);
	int32_t (*SystemIOFileStreamConstructorSystemString_SystemIOFileMode)(int32_t pathHandle, System::IO::FileMode mode);
	void (*SystemIOFileStreamMethodWriteByteSystemByte)(int32_t thisHandle, uint8_t value);
	void (*ReleaseSystemIOBaseFileStream)(int32_t handle);
	void (*SystemIOBaseFileStreamConstructorSystemString_SystemIOFileMode)(int32_t cppHandle, int32_t* handle, int32_t pathHandle, System::IO::FileMode mode);
	void (*ReleaseUnityEnginePlayablesPlayableHandle)(int32_t handle);
	int32_t (*BoxPlayableHandle)(int32_t valHandle);
	int32_t (*UnboxPlayableHandle)(int32_t valHandle);
	int32_t (*UnityEngineExperimentalUIElementsUQueryExtensionsMethodQUnityEngineExperimentalUIElementsVisualElement_SystemString_SystemStringArray1)(int32_t eHandle, int32_t nameHandle, int32_t classesHandle);
	int32_t (*UnityEngineExperimentalUIElementsUQueryExtensionsMethodQUnityEngineExperimentalUIElementsVisualElement_SystemString_SystemString)(int32_t eHandle, int32_t nameHandle, int32_t classNameHandle);
	int32_t (*BoxInteractionSourcePositionAccuracy)(UnityEngine::XR::WSA::Input::InteractionSourcePositionAccuracy val);
	UnityEngine::XR::WSA::Input::InteractionSourcePositionAccuracy (*UnboxInteractionSourcePositionAccuracy)(int32_t valHandle);
	int32_t (*BoxInteractionSourceNode)(UnityEngine::XR::WSA::Input::InteractionSourceNode val);
	UnityEngine::XR::WSA::Input::InteractionSourceNode (*UnboxInteractionSourceNode)(int32_t valHandle);
	void (*ReleaseUnityEngineXRWSAInputInteractionSourcePose)(int32_t handle);
	int32_t (*UnityEngineXRWSAInputInteractionSourcePoseMethodTryGetRotationUnityEngineQuaternion_UnityEngineXRWSAInputInteractionSourceNode)(int32_t thisHandle, UnityEngine::Quaternion* rotation, UnityEngine::XR::WSA::Input::InteractionSourceNode node);
	int32_t (*BoxInteractionSourcePose)(int32_t valHandle);
	int32_t (*UnboxInteractionSourcePose)(int32_t valHandle);
	int32_t (*BoxBoolean)(System::Boolean val);
	int32_t (*UnboxBoolean)(int32_t valHandle);
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
	int16_t (*UnboxChar)(int32_t valHandle);
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
	int32_t (*UnityEngineResolutionArray1GetItem1)(int32_t thisHandle, int32_t index0);
	int32_t (*UnityEngineResolutionArray1SetItem1)(int32_t thisHandle, int32_t index0, int32_t itemHandle);
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
	void (*UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeInvoke)(int32_t thisHandle, int32_t arg0Handle, UnityEngine::SceneManagement::LoadSceneMode arg1);
	void (*ReleaseSystemComponentModelDesignComponentEventHandler)(int32_t handle, int32_t classHandle);
	void (*SystemComponentModelDesignComponentEventHandlerConstructor)(int32_t cppHandle, int32_t* handle, int32_t* classHandle);
	void (*SystemComponentModelDesignComponentEventHandlerAdd)(int32_t thisHandle, int32_t delHandle);
	void (*SystemComponentModelDesignComponentEventHandlerRemove)(int32_t thisHandle, int32_t delHandle);
	void (*SystemComponentModelDesignComponentEventHandlerInvoke)(int32_t thisHandle, int32_t senderHandle, int32_t eHandle);
	void (*ReleaseSystemComponentModelDesignComponentChangingEventHandler)(int32_t handle, int32_t classHandle);
	void (*SystemComponentModelDesignComponentChangingEventHandlerConstructor)(int32_t cppHandle, int32_t* handle, int32_t* classHandle);
	void (*SystemComponentModelDesignComponentChangingEventHandlerAdd)(int32_t thisHandle, int32_t delHandle);
	void (*SystemComponentModelDesignComponentChangingEventHandlerRemove)(int32_t thisHandle, int32_t delHandle);
	void (*SystemComponentModelDesignComponentChangingEventHandlerInvoke)(int32_t thisHandle, int32_t senderHandle, int32_t eHandle);
	void (*ReleaseSystemComponentModelDesignComponentChangedEventHandler)(int32_t handle, int32_t classHandle);
	void (*SystemComponentModelDesignComponentChangedEventHandlerConstructor)(int32_t cppHandle, int32_t* handle, int32_t* classHandle);
	void (*SystemComponentModelDesignComponentChangedEventHandlerAdd)(int32_t thisHandle, int32_t delHandle);
	void (*SystemComponentModelDesignComponentChangedEventHandlerRemove)(int32_t thisHandle, int32_t delHandle);
	void (*SystemComponentModelDesignComponentChangedEventHandlerInvoke)(int32_t thisHandle, int32_t senderHandle, int32_t eHandle);
	void (*ReleaseSystemComponentModelDesignComponentRenameEventHandler)(int32_t handle, int32_t classHandle);
	void (*SystemComponentModelDesignComponentRenameEventHandlerConstructor)(int32_t cppHandle, int32_t* handle, int32_t* classHandle);
	void (*SystemComponentModelDesignComponentRenameEventHandlerAdd)(int32_t thisHandle, int32_t delHandle);
	void (*SystemComponentModelDesignComponentRenameEventHandlerRemove)(int32_t thisHandle, int32_t delHandle);
	void (*SystemComponentModelDesignComponentRenameEventHandlerInvoke)(int32_t thisHandle, int32_t senderHandle, int32_t eHandle);
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
	int32_t RefCountsLenUnityEngineResolution;
	int32_t* RefCountsUnityEngineResolution;
	
	void ReferenceManagedUnityEngineResolution(int32_t handle)
	{
		assert(handle >= 0 && handle < RefCountsLenUnityEngineResolution);
		if (handle != 0)
		{
			RefCountsUnityEngineResolution[handle]++;
		}
	}
	
	void DereferenceManagedUnityEngineResolution(int32_t handle)
	{
		assert(handle >= 0 && handle < RefCountsLenUnityEngineResolution);
		if (handle != 0)
		{
			int32_t numRemain = --RefCountsUnityEngineResolution[handle];
			if (numRemain == 0)
			{
				ReleaseUnityEngineResolution(handle);
			}
		}
	}
	
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
	
	int32_t RefCountsLenUnityEnginePlayablesPlayableGraph;
	int32_t* RefCountsUnityEnginePlayablesPlayableGraph;
	
	void ReferenceManagedUnityEnginePlayablesPlayableGraph(int32_t handle)
	{
		assert(handle >= 0 && handle < RefCountsLenUnityEnginePlayablesPlayableGraph);
		if (handle != 0)
		{
			RefCountsUnityEnginePlayablesPlayableGraph[handle]++;
		}
	}
	
	void DereferenceManagedUnityEnginePlayablesPlayableGraph(int32_t handle)
	{
		assert(handle >= 0 && handle < RefCountsLenUnityEnginePlayablesPlayableGraph);
		if (handle != 0)
		{
			int32_t numRemain = --RefCountsUnityEnginePlayablesPlayableGraph[handle];
			if (numRemain == 0)
			{
				ReleaseUnityEnginePlayablesPlayableGraph(handle);
			}
		}
	}
	
	int32_t RefCountsLenUnityEngineAnimationsAnimationMixerPlayable;
	int32_t* RefCountsUnityEngineAnimationsAnimationMixerPlayable;
	
	void ReferenceManagedUnityEngineAnimationsAnimationMixerPlayable(int32_t handle)
	{
		assert(handle >= 0 && handle < RefCountsLenUnityEngineAnimationsAnimationMixerPlayable);
		if (handle != 0)
		{
			RefCountsUnityEngineAnimationsAnimationMixerPlayable[handle]++;
		}
	}
	
	void DereferenceManagedUnityEngineAnimationsAnimationMixerPlayable(int32_t handle)
	{
		assert(handle >= 0 && handle < RefCountsLenUnityEngineAnimationsAnimationMixerPlayable);
		if (handle != 0)
		{
			int32_t numRemain = --RefCountsUnityEngineAnimationsAnimationMixerPlayable[handle];
			if (numRemain == 0)
			{
				ReleaseUnityEngineAnimationsAnimationMixerPlayable(handle);
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
	
	int32_t RefCountsLenUnityEngineRay;
	int32_t* RefCountsUnityEngineRay;
	
	void ReferenceManagedUnityEngineRay(int32_t handle)
	{
		assert(handle >= 0 && handle < RefCountsLenUnityEngineRay);
		if (handle != 0)
		{
			RefCountsUnityEngineRay[handle]++;
		}
	}
	
	void DereferenceManagedUnityEngineRay(int32_t handle)
	{
		assert(handle >= 0 && handle < RefCountsLenUnityEngineRay);
		if (handle != 0)
		{
			int32_t numRemain = --RefCountsUnityEngineRay[handle];
			if (numRemain == 0)
			{
				ReleaseUnityEngineRay(handle);
			}
		}
	}
	
	int32_t RefCountsLenUnityEngineSceneManagementScene;
	int32_t* RefCountsUnityEngineSceneManagementScene;
	
	void ReferenceManagedUnityEngineSceneManagementScene(int32_t handle)
	{
		assert(handle >= 0 && handle < RefCountsLenUnityEngineSceneManagementScene);
		if (handle != 0)
		{
			RefCountsUnityEngineSceneManagementScene[handle]++;
		}
	}
	
	void DereferenceManagedUnityEngineSceneManagementScene(int32_t handle)
	{
		assert(handle >= 0 && handle < RefCountsLenUnityEngineSceneManagementScene);
		if (handle != 0)
		{
			int32_t numRemain = --RefCountsUnityEngineSceneManagementScene[handle];
			if (numRemain == 0)
			{
				ReleaseUnityEngineSceneManagementScene(handle);
			}
		}
	}
	
	int32_t SystemCollectionsGenericBaseIComparerSystemInt32FreeListSize;
	System::Collections::Generic::BaseIComparer<int32_t>** SystemCollectionsGenericBaseIComparerSystemInt32FreeList;
	System::Collections::Generic::BaseIComparer<int32_t>** NextFreeSystemCollectionsGenericBaseIComparerSystemInt32;
	
	int32_t StoreSystemCollectionsGenericBaseIComparerSystemInt32(System::Collections::Generic::BaseIComparer<int32_t>* del)
	{
		assert(NextFreeSystemCollectionsGenericBaseIComparerSystemInt32 != nullptr);
		System::Collections::Generic::BaseIComparer<int32_t>** pNext = NextFreeSystemCollectionsGenericBaseIComparerSystemInt32;
		NextFreeSystemCollectionsGenericBaseIComparerSystemInt32 = (System::Collections::Generic::BaseIComparer<int32_t>**)*pNext;
		*pNext = del;
		return (int32_t)(pNext - SystemCollectionsGenericBaseIComparerSystemInt32FreeList);
	}
	
	System::Collections::Generic::BaseIComparer<int32_t>* GetSystemCollectionsGenericBaseIComparerSystemInt32(int32_t handle)
	{
		assert(handle >= 0 && handle < SystemCollectionsGenericBaseIComparerSystemInt32FreeListSize);
		return SystemCollectionsGenericBaseIComparerSystemInt32FreeList[handle];
	}
	
	void RemoveSystemCollectionsGenericBaseIComparerSystemInt32(int32_t handle)
	{
		System::Collections::Generic::BaseIComparer<int32_t>** pRelease = SystemCollectionsGenericBaseIComparerSystemInt32FreeList + handle;
		*pRelease = (System::Collections::Generic::BaseIComparer<int32_t>*)NextFreeSystemCollectionsGenericBaseIComparerSystemInt32;
		NextFreeSystemCollectionsGenericBaseIComparerSystemInt32 = pRelease;
	}
	int32_t SystemCollectionsGenericBaseIComparerSystemStringFreeListSize;
	System::Collections::Generic::BaseIComparer<System::String>** SystemCollectionsGenericBaseIComparerSystemStringFreeList;
	System::Collections::Generic::BaseIComparer<System::String>** NextFreeSystemCollectionsGenericBaseIComparerSystemString;
	
	int32_t StoreSystemCollectionsGenericBaseIComparerSystemString(System::Collections::Generic::BaseIComparer<System::String>* del)
	{
		assert(NextFreeSystemCollectionsGenericBaseIComparerSystemString != nullptr);
		System::Collections::Generic::BaseIComparer<System::String>** pNext = NextFreeSystemCollectionsGenericBaseIComparerSystemString;
		NextFreeSystemCollectionsGenericBaseIComparerSystemString = (System::Collections::Generic::BaseIComparer<System::String>**)*pNext;
		*pNext = del;
		return (int32_t)(pNext - SystemCollectionsGenericBaseIComparerSystemStringFreeList);
	}
	
	System::Collections::Generic::BaseIComparer<System::String>* GetSystemCollectionsGenericBaseIComparerSystemString(int32_t handle)
	{
		assert(handle >= 0 && handle < SystemCollectionsGenericBaseIComparerSystemStringFreeListSize);
		return SystemCollectionsGenericBaseIComparerSystemStringFreeList[handle];
	}
	
	void RemoveSystemCollectionsGenericBaseIComparerSystemString(int32_t handle)
	{
		System::Collections::Generic::BaseIComparer<System::String>** pRelease = SystemCollectionsGenericBaseIComparerSystemStringFreeList + handle;
		*pRelease = (System::Collections::Generic::BaseIComparer<System::String>*)NextFreeSystemCollectionsGenericBaseIComparerSystemString;
		NextFreeSystemCollectionsGenericBaseIComparerSystemString = pRelease;
	}
	int32_t SystemBaseStringComparerFreeListSize;
	System::BaseStringComparer** SystemBaseStringComparerFreeList;
	System::BaseStringComparer** NextFreeSystemBaseStringComparer;
	
	int32_t StoreSystemBaseStringComparer(System::BaseStringComparer* del)
	{
		assert(NextFreeSystemBaseStringComparer != nullptr);
		System::BaseStringComparer** pNext = NextFreeSystemBaseStringComparer;
		NextFreeSystemBaseStringComparer = (System::BaseStringComparer**)*pNext;
		*pNext = del;
		return (int32_t)(pNext - SystemBaseStringComparerFreeList);
	}
	
	System::BaseStringComparer* GetSystemBaseStringComparer(int32_t handle)
	{
		assert(handle >= 0 && handle < SystemBaseStringComparerFreeListSize);
		return SystemBaseStringComparerFreeList[handle];
	}
	
	void RemoveSystemBaseStringComparer(int32_t handle)
	{
		System::BaseStringComparer** pRelease = SystemBaseStringComparerFreeList + handle;
		*pRelease = (System::BaseStringComparer*)NextFreeSystemBaseStringComparer;
		NextFreeSystemBaseStringComparer = pRelease;
	}
	int32_t SystemCollectionsBaseQueueFreeListSize;
	System::Collections::BaseQueue** SystemCollectionsBaseQueueFreeList;
	System::Collections::BaseQueue** NextFreeSystemCollectionsBaseQueue;
	
	int32_t StoreSystemCollectionsBaseQueue(System::Collections::BaseQueue* del)
	{
		assert(NextFreeSystemCollectionsBaseQueue != nullptr);
		System::Collections::BaseQueue** pNext = NextFreeSystemCollectionsBaseQueue;
		NextFreeSystemCollectionsBaseQueue = (System::Collections::BaseQueue**)*pNext;
		*pNext = del;
		return (int32_t)(pNext - SystemCollectionsBaseQueueFreeList);
	}
	
	System::Collections::BaseQueue* GetSystemCollectionsBaseQueue(int32_t handle)
	{
		assert(handle >= 0 && handle < SystemCollectionsBaseQueueFreeListSize);
		return SystemCollectionsBaseQueueFreeList[handle];
	}
	
	void RemoveSystemCollectionsBaseQueue(int32_t handle)
	{
		System::Collections::BaseQueue** pRelease = SystemCollectionsBaseQueueFreeList + handle;
		*pRelease = (System::Collections::BaseQueue*)NextFreeSystemCollectionsBaseQueue;
		NextFreeSystemCollectionsBaseQueue = pRelease;
	}
	int32_t SystemComponentModelDesignBaseIComponentChangeServiceFreeListSize;
	System::ComponentModel::Design::BaseIComponentChangeService** SystemComponentModelDesignBaseIComponentChangeServiceFreeList;
	System::ComponentModel::Design::BaseIComponentChangeService** NextFreeSystemComponentModelDesignBaseIComponentChangeService;
	
	int32_t StoreSystemComponentModelDesignBaseIComponentChangeService(System::ComponentModel::Design::BaseIComponentChangeService* del)
	{
		assert(NextFreeSystemComponentModelDesignBaseIComponentChangeService != nullptr);
		System::ComponentModel::Design::BaseIComponentChangeService** pNext = NextFreeSystemComponentModelDesignBaseIComponentChangeService;
		NextFreeSystemComponentModelDesignBaseIComponentChangeService = (System::ComponentModel::Design::BaseIComponentChangeService**)*pNext;
		*pNext = del;
		return (int32_t)(pNext - SystemComponentModelDesignBaseIComponentChangeServiceFreeList);
	}
	
	System::ComponentModel::Design::BaseIComponentChangeService* GetSystemComponentModelDesignBaseIComponentChangeService(int32_t handle)
	{
		assert(handle >= 0 && handle < SystemComponentModelDesignBaseIComponentChangeServiceFreeListSize);
		return SystemComponentModelDesignBaseIComponentChangeServiceFreeList[handle];
	}
	
	void RemoveSystemComponentModelDesignBaseIComponentChangeService(int32_t handle)
	{
		System::ComponentModel::Design::BaseIComponentChangeService** pRelease = SystemComponentModelDesignBaseIComponentChangeServiceFreeList + handle;
		*pRelease = (System::ComponentModel::Design::BaseIComponentChangeService*)NextFreeSystemComponentModelDesignBaseIComponentChangeService;
		NextFreeSystemComponentModelDesignBaseIComponentChangeService = pRelease;
	}
	int32_t SystemIOBaseFileStreamFreeListSize;
	System::IO::BaseFileStream** SystemIOBaseFileStreamFreeList;
	System::IO::BaseFileStream** NextFreeSystemIOBaseFileStream;
	
	int32_t StoreSystemIOBaseFileStream(System::IO::BaseFileStream* del)
	{
		assert(NextFreeSystemIOBaseFileStream != nullptr);
		System::IO::BaseFileStream** pNext = NextFreeSystemIOBaseFileStream;
		NextFreeSystemIOBaseFileStream = (System::IO::BaseFileStream**)*pNext;
		*pNext = del;
		return (int32_t)(pNext - SystemIOBaseFileStreamFreeList);
	}
	
	System::IO::BaseFileStream* GetSystemIOBaseFileStream(int32_t handle)
	{
		assert(handle >= 0 && handle < SystemIOBaseFileStreamFreeListSize);
		return SystemIOBaseFileStreamFreeList[handle];
	}
	
	void RemoveSystemIOBaseFileStream(int32_t handle)
	{
		System::IO::BaseFileStream** pRelease = SystemIOBaseFileStreamFreeList + handle;
		*pRelease = (System::IO::BaseFileStream*)NextFreeSystemIOBaseFileStream;
		NextFreeSystemIOBaseFileStream = pRelease;
	}
	int32_t RefCountsLenUnityEnginePlayablesPlayableHandle;
	int32_t* RefCountsUnityEnginePlayablesPlayableHandle;
	
	void ReferenceManagedUnityEnginePlayablesPlayableHandle(int32_t handle)
	{
		assert(handle >= 0 && handle < RefCountsLenUnityEnginePlayablesPlayableHandle);
		if (handle != 0)
		{
			RefCountsUnityEnginePlayablesPlayableHandle[handle]++;
		}
	}
	
	void DereferenceManagedUnityEnginePlayablesPlayableHandle(int32_t handle)
	{
		assert(handle >= 0 && handle < RefCountsLenUnityEnginePlayablesPlayableHandle);
		if (handle != 0)
		{
			int32_t numRemain = --RefCountsUnityEnginePlayablesPlayableHandle[handle];
			if (numRemain == 0)
			{
				ReleaseUnityEnginePlayablesPlayableHandle(handle);
			}
		}
	}
	
	int32_t RefCountsLenUnityEngineXRWSAInputInteractionSourcePose;
	int32_t* RefCountsUnityEngineXRWSAInputInteractionSourcePose;
	
	void ReferenceManagedUnityEngineXRWSAInputInteractionSourcePose(int32_t handle)
	{
		assert(handle >= 0 && handle < RefCountsLenUnityEngineXRWSAInputInteractionSourcePose);
		if (handle != 0)
		{
			RefCountsUnityEngineXRWSAInputInteractionSourcePose[handle]++;
		}
	}
	
	void DereferenceManagedUnityEngineXRWSAInputInteractionSourcePose(int32_t handle)
	{
		assert(handle >= 0 && handle < RefCountsLenUnityEngineXRWSAInputInteractionSourcePose);
		if (handle != 0)
		{
			int32_t numRemain = --RefCountsUnityEngineXRWSAInputInteractionSourcePose[handle];
			if (numRemain == 0)
			{
				ReleaseUnityEngineXRWSAInputInteractionSourcePose(handle);
			}
		}
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
	int32_t SystemComponentModelDesignComponentEventHandlerFreeListSize;
	System::ComponentModel::Design::ComponentEventHandler** SystemComponentModelDesignComponentEventHandlerFreeList;
	System::ComponentModel::Design::ComponentEventHandler** NextFreeSystemComponentModelDesignComponentEventHandler;
	
	int32_t StoreSystemComponentModelDesignComponentEventHandler(System::ComponentModel::Design::ComponentEventHandler* del)
	{
		assert(NextFreeSystemComponentModelDesignComponentEventHandler != nullptr);
		System::ComponentModel::Design::ComponentEventHandler** pNext = NextFreeSystemComponentModelDesignComponentEventHandler;
		NextFreeSystemComponentModelDesignComponentEventHandler = (System::ComponentModel::Design::ComponentEventHandler**)*pNext;
		*pNext = del;
		return (int32_t)(pNext - SystemComponentModelDesignComponentEventHandlerFreeList);
	}
	
	System::ComponentModel::Design::ComponentEventHandler* GetSystemComponentModelDesignComponentEventHandler(int32_t handle)
	{
		assert(handle >= 0 && handle < SystemComponentModelDesignComponentEventHandlerFreeListSize);
		return SystemComponentModelDesignComponentEventHandlerFreeList[handle];
	}
	
	void RemoveSystemComponentModelDesignComponentEventHandler(int32_t handle)
	{
		System::ComponentModel::Design::ComponentEventHandler** pRelease = SystemComponentModelDesignComponentEventHandlerFreeList + handle;
		*pRelease = (System::ComponentModel::Design::ComponentEventHandler*)NextFreeSystemComponentModelDesignComponentEventHandler;
		NextFreeSystemComponentModelDesignComponentEventHandler = pRelease;
	}
	int32_t SystemComponentModelDesignComponentChangingEventHandlerFreeListSize;
	System::ComponentModel::Design::ComponentChangingEventHandler** SystemComponentModelDesignComponentChangingEventHandlerFreeList;
	System::ComponentModel::Design::ComponentChangingEventHandler** NextFreeSystemComponentModelDesignComponentChangingEventHandler;
	
	int32_t StoreSystemComponentModelDesignComponentChangingEventHandler(System::ComponentModel::Design::ComponentChangingEventHandler* del)
	{
		assert(NextFreeSystemComponentModelDesignComponentChangingEventHandler != nullptr);
		System::ComponentModel::Design::ComponentChangingEventHandler** pNext = NextFreeSystemComponentModelDesignComponentChangingEventHandler;
		NextFreeSystemComponentModelDesignComponentChangingEventHandler = (System::ComponentModel::Design::ComponentChangingEventHandler**)*pNext;
		*pNext = del;
		return (int32_t)(pNext - SystemComponentModelDesignComponentChangingEventHandlerFreeList);
	}
	
	System::ComponentModel::Design::ComponentChangingEventHandler* GetSystemComponentModelDesignComponentChangingEventHandler(int32_t handle)
	{
		assert(handle >= 0 && handle < SystemComponentModelDesignComponentChangingEventHandlerFreeListSize);
		return SystemComponentModelDesignComponentChangingEventHandlerFreeList[handle];
	}
	
	void RemoveSystemComponentModelDesignComponentChangingEventHandler(int32_t handle)
	{
		System::ComponentModel::Design::ComponentChangingEventHandler** pRelease = SystemComponentModelDesignComponentChangingEventHandlerFreeList + handle;
		*pRelease = (System::ComponentModel::Design::ComponentChangingEventHandler*)NextFreeSystemComponentModelDesignComponentChangingEventHandler;
		NextFreeSystemComponentModelDesignComponentChangingEventHandler = pRelease;
	}
	int32_t SystemComponentModelDesignComponentChangedEventHandlerFreeListSize;
	System::ComponentModel::Design::ComponentChangedEventHandler** SystemComponentModelDesignComponentChangedEventHandlerFreeList;
	System::ComponentModel::Design::ComponentChangedEventHandler** NextFreeSystemComponentModelDesignComponentChangedEventHandler;
	
	int32_t StoreSystemComponentModelDesignComponentChangedEventHandler(System::ComponentModel::Design::ComponentChangedEventHandler* del)
	{
		assert(NextFreeSystemComponentModelDesignComponentChangedEventHandler != nullptr);
		System::ComponentModel::Design::ComponentChangedEventHandler** pNext = NextFreeSystemComponentModelDesignComponentChangedEventHandler;
		NextFreeSystemComponentModelDesignComponentChangedEventHandler = (System::ComponentModel::Design::ComponentChangedEventHandler**)*pNext;
		*pNext = del;
		return (int32_t)(pNext - SystemComponentModelDesignComponentChangedEventHandlerFreeList);
	}
	
	System::ComponentModel::Design::ComponentChangedEventHandler* GetSystemComponentModelDesignComponentChangedEventHandler(int32_t handle)
	{
		assert(handle >= 0 && handle < SystemComponentModelDesignComponentChangedEventHandlerFreeListSize);
		return SystemComponentModelDesignComponentChangedEventHandlerFreeList[handle];
	}
	
	void RemoveSystemComponentModelDesignComponentChangedEventHandler(int32_t handle)
	{
		System::ComponentModel::Design::ComponentChangedEventHandler** pRelease = SystemComponentModelDesignComponentChangedEventHandlerFreeList + handle;
		*pRelease = (System::ComponentModel::Design::ComponentChangedEventHandler*)NextFreeSystemComponentModelDesignComponentChangedEventHandler;
		NextFreeSystemComponentModelDesignComponentChangedEventHandler = pRelease;
	}
	int32_t SystemComponentModelDesignComponentRenameEventHandlerFreeListSize;
	System::ComponentModel::Design::ComponentRenameEventHandler** SystemComponentModelDesignComponentRenameEventHandlerFreeList;
	System::ComponentModel::Design::ComponentRenameEventHandler** NextFreeSystemComponentModelDesignComponentRenameEventHandler;
	
	int32_t StoreSystemComponentModelDesignComponentRenameEventHandler(System::ComponentModel::Design::ComponentRenameEventHandler* del)
	{
		assert(NextFreeSystemComponentModelDesignComponentRenameEventHandler != nullptr);
		System::ComponentModel::Design::ComponentRenameEventHandler** pNext = NextFreeSystemComponentModelDesignComponentRenameEventHandler;
		NextFreeSystemComponentModelDesignComponentRenameEventHandler = (System::ComponentModel::Design::ComponentRenameEventHandler**)*pNext;
		*pNext = del;
		return (int32_t)(pNext - SystemComponentModelDesignComponentRenameEventHandlerFreeList);
	}
	
	System::ComponentModel::Design::ComponentRenameEventHandler* GetSystemComponentModelDesignComponentRenameEventHandler(int32_t handle)
	{
		assert(handle >= 0 && handle < SystemComponentModelDesignComponentRenameEventHandlerFreeListSize);
		return SystemComponentModelDesignComponentRenameEventHandlerFreeList[handle];
	}
	
	void RemoveSystemComponentModelDesignComponentRenameEventHandler(int32_t handle)
	{
		System::ComponentModel::Design::ComponentRenameEventHandler** pRelease = SystemComponentModelDesignComponentRenameEventHandlerFreeList + handle;
		*pRelease = (System::ComponentModel::Design::ComponentRenameEventHandler*)NextFreeSystemComponentModelDesignComponentRenameEventHandler;
		NextFreeSystemComponentModelDesignComponentRenameEventHandler = pRelease;
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
	Object::Object()
		: Handle(0)
	{
	}
	
	Object::Object(Plugin::InternalUse iu, int32_t handle)
		: Handle(handle)
	{
	}
	
	Object::Object(decltype(nullptr))
		: Handle(0)
	{
	}
	
	bool Object::operator==(decltype(nullptr)) const
	{
		return Handle == 0;
	}
	
	bool Object::operator!=(decltype(nullptr)) const
	{
		return Handle != 0;
	}
	
	void Object::ThrowReferenceToThis()
	{
		throw *this;
	}
	
	ValueType::ValueType(Plugin::InternalUse iu, int32_t handle)
		: Object(iu, handle)
	{
	}
	
	ValueType::ValueType(decltype(nullptr))
		: Object(nullptr)
	{
	}
	
	String::String(decltype(nullptr))
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
	
	String& String::operator=(decltype(nullptr))
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
	
	String::String(const char* chars)
		: Object(Plugin::InternalUse::Only, Plugin::StringNew(chars))
	{
	}
	
	ICloneable::ICloneable(Plugin::InternalUse iu, int32_t handle)
		: Object(iu, handle)
	{
	}
	
	ICloneable::ICloneable(decltype(nullptr))
		: Object(nullptr)
	{
	}
	
	namespace Collections
	{
		IEnumerable::IEnumerable(Plugin::InternalUse iu, int32_t handle)
			: Object(iu, handle)
		{
		}
		
		IEnumerable::IEnumerable(decltype(nullptr))
			: Object(nullptr)
		{
		}
		
		IEnumerator IEnumerable::GetEnumerator()
		{
			return IEnumerator(
				Plugin::InternalUse::Only,
				Plugin::EnumerableGetEnumerator(Handle));
		}
		
		Plugin::EnumerableIterator begin(
			System::Collections::IEnumerable& enumerable)
		{
			return Plugin::EnumerableIterator(enumerable);
		}
		
		Plugin::EnumerableIterator end(
			System::Collections::IEnumerable& enumerable)
		{
			return Plugin::EnumerableIterator(nullptr);
		}
		
		ICollection::ICollection(Plugin::InternalUse iu, int32_t handle)
			: Object(iu, handle)
			, IEnumerable(nullptr)
		{
		}
		
		ICollection::ICollection(decltype(nullptr))
			: Object(nullptr)
			, IEnumerable(nullptr)
		{
		}
		
		IList::IList(Plugin::InternalUse iu, int32_t handle)
			: Object(iu, handle)
			, IEnumerable(nullptr)
			, ICollection(nullptr)
		{
		}
		
		IList::IList(decltype(nullptr))
			: Object(nullptr)
			, IEnumerable(nullptr)
			, ICollection(nullptr)
		{
		}
	}
	
	Array::Array(Plugin::InternalUse iu, int32_t handle)
		: Object(iu, handle)
		, ICloneable(nullptr)
		, Collections::IEnumerable(nullptr)
		, Collections::ICollection(nullptr)
		, Collections::IList(nullptr)
	{
	}
	
	Array::Array(decltype(nullptr))
		: Object(nullptr)
		, ICloneable(nullptr)
		, Collections::IEnumerable(nullptr)
		, Collections::ICollection(nullptr)
		, Collections::IList(nullptr)
	{
	}
	
	int32_t Array::GetLength()
	{
		return Plugin::ArrayGetLength(Handle);
	}
	
	int32_t Array::GetRank()
	{
		return 0;
	}
}

/*BEGIN METHOD DEFINITIONS*/
namespace System
{
	IDisposable::IDisposable(decltype(nullptr))
	{
	}
	
	IDisposable::IDisposable(Plugin::InternalUse iu, int32_t handle)
	{
		Handle = handle;
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
	}
	
	IDisposable::IDisposable(const IDisposable& other)
		: IDisposable(Plugin::InternalUse::Only, other.Handle)
	{
	}
	
	IDisposable::IDisposable(IDisposable&& other)
		: IDisposable(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
	}
	
	IDisposable::~IDisposable()
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
	}
	
	IDisposable& IDisposable::operator=(const IDisposable& other)
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
	
	IDisposable& IDisposable::operator=(decltype(nullptr))
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
		return *this;
	}
	
	IDisposable& IDisposable::operator=(IDisposable&& other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
		}
		Handle = other.Handle;
		other.Handle = 0;
		return *this;
	}
	
	bool IDisposable::operator==(const IDisposable& other) const
	{
		return Handle == other.Handle;
	}
	
	bool IDisposable::operator!=(const IDisposable& other) const
	{
		return Handle != other.Handle;
	}
	
	void IDisposable::Dispose()
	{
		Plugin::SystemIDisposableMethodDispose(Handle);
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
	Object::Object(decltype(nullptr))
	{
	}
	
	Object::Object(Plugin::InternalUse iu, int32_t handle)
	{
		Handle = handle;
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
	
	Object& Object::operator=(decltype(nullptr))
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
	
	void Object::SetName(System::String& value)
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
	
	System::Boolean Object::operator==(UnityEngine::Object& x)
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
	Component::Component(decltype(nullptr))
		: UnityEngine::Object(nullptr)
	{
	}
	
	Component::Component(Plugin::InternalUse iu, int32_t handle)
		: UnityEngine::Object(nullptr)
	{
		Handle = handle;
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
	
	Component& Component::operator=(decltype(nullptr))
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
	Transform::Transform(decltype(nullptr))
		: UnityEngine::Object(nullptr)
		, UnityEngine::Component(nullptr)
		, System::Collections::IEnumerable(nullptr)
	{
	}
	
	Transform::Transform(Plugin::InternalUse iu, int32_t handle)
		: UnityEngine::Object(nullptr)
		, UnityEngine::Component(nullptr)
		, System::Collections::IEnumerable(nullptr)
	{
		Handle = handle;
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
	
	Transform& Transform::operator=(decltype(nullptr))
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
	
	void Transform::SetParent(UnityEngine::Transform& parent)
	{
		Plugin::UnityEngineTransformMethodSetParentUnityEngineTransform(Handle, parent.Handle);
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
	Resolution::Resolution(decltype(nullptr))
		: System::ValueType(nullptr)
	{
	}
	
	Resolution::Resolution(Plugin::InternalUse iu, int32_t handle)
		: System::ValueType(nullptr)
	{
		Handle = handle;
		if (handle)
		{
			Plugin::ReferenceManagedUnityEngineResolution(Handle);
		}
	}
	
	Resolution::Resolution(const Resolution& other)
		: Resolution(Plugin::InternalUse::Only, other.Handle)
	{
	}
	
	Resolution::Resolution(Resolution&& other)
		: Resolution(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
	}
	
	Resolution::~Resolution()
	{
		if (Handle)
		{
			Plugin::DereferenceManagedUnityEngineResolution(Handle);
			Handle = 0;
		}
	}
	
	Resolution& Resolution::operator=(const Resolution& other)
	{
		if (this->Handle)
		{
			Plugin::DereferenceManagedUnityEngineResolution(Handle);
		}
		this->Handle = other.Handle;
		if (this->Handle)
		{
			Plugin::ReferenceManagedUnityEngineResolution(Handle);
		}
		return *this;
	}
	
	Resolution& Resolution::operator=(decltype(nullptr))
	{
		if (Handle)
		{
			Plugin::DereferenceManagedUnityEngineResolution(Handle);
			Handle = 0;
		}
		return *this;
	}
	
	Resolution& Resolution::operator=(Resolution&& other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedUnityEngineResolution(Handle);
		}
		Handle = other.Handle;
		other.Handle = 0;
		return *this;
	}
	
	bool Resolution::operator==(const Resolution& other) const
	{
		return Handle == other.Handle;
	}
	
	bool Resolution::operator!=(const Resolution& other) const
	{
		return Handle != other.Handle;
	}
	
	int32_t Resolution::GetWidth()
	{
		auto returnValue = Plugin::UnityEngineResolutionPropertyGetWidth(Handle);
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
		Plugin::UnityEngineResolutionPropertySetWidth(Handle, value);
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
		auto returnValue = Plugin::UnityEngineResolutionPropertyGetHeight(Handle);
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
		Plugin::UnityEngineResolutionPropertySetHeight(Handle, value);
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
		auto returnValue = Plugin::UnityEngineResolutionPropertyGetRefreshRate(Handle);
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
		Plugin::UnityEngineResolutionPropertySetRefreshRate(Handle, value);
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
		int32_t handle = Plugin::BoxResolution(val.Handle);
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
		UnityEngine::Resolution returnVal(Plugin::InternalUse::Only, Plugin::UnboxResolution(Handle));
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
	RaycastHit::RaycastHit(decltype(nullptr))
		: System::ValueType(nullptr)
	{
	}
	
	RaycastHit::RaycastHit(Plugin::InternalUse iu, int32_t handle)
		: System::ValueType(nullptr)
	{
		Handle = handle;
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
	
	RaycastHit& RaycastHit::operator=(decltype(nullptr))
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
	Object::Object(UnityEngine::RaycastHit& val)
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
	namespace Collections
	{
		IEnumerator::IEnumerator(decltype(nullptr))
		{
		}
		
		IEnumerator::IEnumerator(Plugin::InternalUse iu, int32_t handle)
		{
			Handle = handle;
			if (handle)
			{
				Plugin::ReferenceManagedClass(handle);
			}
		}
		
		IEnumerator::IEnumerator(const IEnumerator& other)
			: IEnumerator(Plugin::InternalUse::Only, other.Handle)
		{
		}
		
		IEnumerator::IEnumerator(IEnumerator&& other)
			: IEnumerator(Plugin::InternalUse::Only, other.Handle)
		{
			other.Handle = 0;
		}
		
		IEnumerator::~IEnumerator()
		{
			if (Handle)
			{
				Plugin::DereferenceManagedClass(Handle);
				Handle = 0;
			}
		}
		
		IEnumerator& IEnumerator::operator=(const IEnumerator& other)
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
		
		IEnumerator& IEnumerator::operator=(decltype(nullptr))
		{
			if (Handle)
			{
				Plugin::DereferenceManagedClass(Handle);
				Handle = 0;
			}
			return *this;
		}
		
		IEnumerator& IEnumerator::operator=(IEnumerator&& other)
		{
			if (Handle)
			{
				Plugin::DereferenceManagedClass(Handle);
			}
			Handle = other.Handle;
			other.Handle = 0;
			return *this;
		}
		
		bool IEnumerator::operator==(const IEnumerator& other) const
		{
			return Handle == other.Handle;
		}
		
		bool IEnumerator::operator!=(const IEnumerator& other) const
		{
			return Handle != other.Handle;
		}
		
		System::Object IEnumerator::GetCurrent()
		{
			auto returnValue = Plugin::SystemCollectionsIEnumeratorPropertyGetCurrent(Handle);
			if (Plugin::unhandledCsharpException)
			{
				System::Exception* ex = Plugin::unhandledCsharpException;
				Plugin::unhandledCsharpException = nullptr;
				ex->ThrowReferenceToThis();
				delete ex;
			}
			return System::Object(Plugin::InternalUse::Only, returnValue);
		}
		
		System::Boolean IEnumerator::MoveNext()
		{
			auto returnValue = Plugin::SystemCollectionsIEnumeratorMethodMoveNext(Handle);
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

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			IEnumerator<System::String>::IEnumerator(decltype(nullptr))
				: System::IDisposable(nullptr)
				, System::Collections::IEnumerator(nullptr)
			{
			}
			
			IEnumerator<System::String>::IEnumerator(Plugin::InternalUse iu, int32_t handle)
				: System::IDisposable(nullptr)
				, System::Collections::IEnumerator(nullptr)
			{
				Handle = handle;
				if (handle)
				{
					Plugin::ReferenceManagedClass(handle);
				}
			}
			
			IEnumerator<System::String>::IEnumerator(const IEnumerator<System::String>& other)
				: IEnumerator(Plugin::InternalUse::Only, other.Handle)
			{
			}
			
			IEnumerator<System::String>::IEnumerator(IEnumerator<System::String>&& other)
				: IEnumerator(Plugin::InternalUse::Only, other.Handle)
			{
				other.Handle = 0;
			}
			
			IEnumerator<System::String>::~IEnumerator<System::String>()
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
			}
			
			IEnumerator<System::String>& IEnumerator<System::String>::operator=(const IEnumerator<System::String>& other)
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
			
			IEnumerator<System::String>& IEnumerator<System::String>::operator=(decltype(nullptr))
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
				return *this;
			}
			
			IEnumerator<System::String>& IEnumerator<System::String>::operator=(IEnumerator<System::String>&& other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
				}
				Handle = other.Handle;
				other.Handle = 0;
				return *this;
			}
			
			bool IEnumerator<System::String>::operator==(const IEnumerator<System::String>& other) const
			{
				return Handle == other.Handle;
			}
			
			bool IEnumerator<System::String>::operator!=(const IEnumerator<System::String>& other) const
			{
				return Handle != other.Handle;
			}
			
			System::String IEnumerator<System::String>::GetCurrent()
			{
				auto returnValue = Plugin::SystemCollectionsGenericIEnumeratorSystemStringPropertyGetCurrent(Handle);
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
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			IEnumerator<int32_t>::IEnumerator(decltype(nullptr))
				: System::IDisposable(nullptr)
				, System::Collections::IEnumerator(nullptr)
			{
			}
			
			IEnumerator<int32_t>::IEnumerator(Plugin::InternalUse iu, int32_t handle)
				: System::IDisposable(nullptr)
				, System::Collections::IEnumerator(nullptr)
			{
				Handle = handle;
				if (handle)
				{
					Plugin::ReferenceManagedClass(handle);
				}
			}
			
			IEnumerator<int32_t>::IEnumerator(const IEnumerator<int32_t>& other)
				: IEnumerator(Plugin::InternalUse::Only, other.Handle)
			{
			}
			
			IEnumerator<int32_t>::IEnumerator(IEnumerator<int32_t>&& other)
				: IEnumerator(Plugin::InternalUse::Only, other.Handle)
			{
				other.Handle = 0;
			}
			
			IEnumerator<int32_t>::~IEnumerator<int32_t>()
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
			}
			
			IEnumerator<int32_t>& IEnumerator<int32_t>::operator=(const IEnumerator<int32_t>& other)
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
			
			IEnumerator<int32_t>& IEnumerator<int32_t>::operator=(decltype(nullptr))
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
				return *this;
			}
			
			IEnumerator<int32_t>& IEnumerator<int32_t>::operator=(IEnumerator<int32_t>&& other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
				}
				Handle = other.Handle;
				other.Handle = 0;
				return *this;
			}
			
			bool IEnumerator<int32_t>::operator==(const IEnumerator<int32_t>& other) const
			{
				return Handle == other.Handle;
			}
			
			bool IEnumerator<int32_t>::operator!=(const IEnumerator<int32_t>& other) const
			{
				return Handle != other.Handle;
			}
			
			int32_t IEnumerator<int32_t>::GetCurrent()
			{
				auto returnValue = Plugin::SystemCollectionsGenericIEnumeratorSystemInt32PropertyGetCurrent(Handle);
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
	namespace Collections
	{
		namespace Generic
		{
			IEnumerator<float>::IEnumerator(decltype(nullptr))
				: System::IDisposable(nullptr)
				, System::Collections::IEnumerator(nullptr)
			{
			}
			
			IEnumerator<float>::IEnumerator(Plugin::InternalUse iu, int32_t handle)
				: System::IDisposable(nullptr)
				, System::Collections::IEnumerator(nullptr)
			{
				Handle = handle;
				if (handle)
				{
					Plugin::ReferenceManagedClass(handle);
				}
			}
			
			IEnumerator<float>::IEnumerator(const IEnumerator<float>& other)
				: IEnumerator(Plugin::InternalUse::Only, other.Handle)
			{
			}
			
			IEnumerator<float>::IEnumerator(IEnumerator<float>&& other)
				: IEnumerator(Plugin::InternalUse::Only, other.Handle)
			{
				other.Handle = 0;
			}
			
			IEnumerator<float>::~IEnumerator<float>()
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
			}
			
			IEnumerator<float>& IEnumerator<float>::operator=(const IEnumerator<float>& other)
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
			
			IEnumerator<float>& IEnumerator<float>::operator=(decltype(nullptr))
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
				return *this;
			}
			
			IEnumerator<float>& IEnumerator<float>::operator=(IEnumerator<float>&& other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
				}
				Handle = other.Handle;
				other.Handle = 0;
				return *this;
			}
			
			bool IEnumerator<float>::operator==(const IEnumerator<float>& other) const
			{
				return Handle == other.Handle;
			}
			
			bool IEnumerator<float>::operator!=(const IEnumerator<float>& other) const
			{
				return Handle != other.Handle;
			}
			
			float IEnumerator<float>::GetCurrent()
			{
				auto returnValue = Plugin::SystemCollectionsGenericIEnumeratorSystemSinglePropertyGetCurrent(Handle);
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
	namespace Collections
	{
		namespace Generic
		{
			IEnumerator<UnityEngine::RaycastHit>::IEnumerator(decltype(nullptr))
				: System::IDisposable(nullptr)
				, System::Collections::IEnumerator(nullptr)
			{
			}
			
			IEnumerator<UnityEngine::RaycastHit>::IEnumerator(Plugin::InternalUse iu, int32_t handle)
				: System::IDisposable(nullptr)
				, System::Collections::IEnumerator(nullptr)
			{
				Handle = handle;
				if (handle)
				{
					Plugin::ReferenceManagedClass(handle);
				}
			}
			
			IEnumerator<UnityEngine::RaycastHit>::IEnumerator(const IEnumerator<UnityEngine::RaycastHit>& other)
				: IEnumerator(Plugin::InternalUse::Only, other.Handle)
			{
			}
			
			IEnumerator<UnityEngine::RaycastHit>::IEnumerator(IEnumerator<UnityEngine::RaycastHit>&& other)
				: IEnumerator(Plugin::InternalUse::Only, other.Handle)
			{
				other.Handle = 0;
			}
			
			IEnumerator<UnityEngine::RaycastHit>::~IEnumerator<UnityEngine::RaycastHit>()
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
			}
			
			IEnumerator<UnityEngine::RaycastHit>& IEnumerator<UnityEngine::RaycastHit>::operator=(const IEnumerator<UnityEngine::RaycastHit>& other)
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
			
			IEnumerator<UnityEngine::RaycastHit>& IEnumerator<UnityEngine::RaycastHit>::operator=(decltype(nullptr))
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
				return *this;
			}
			
			IEnumerator<UnityEngine::RaycastHit>& IEnumerator<UnityEngine::RaycastHit>::operator=(IEnumerator<UnityEngine::RaycastHit>&& other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
				}
				Handle = other.Handle;
				other.Handle = 0;
				return *this;
			}
			
			bool IEnumerator<UnityEngine::RaycastHit>::operator==(const IEnumerator<UnityEngine::RaycastHit>& other) const
			{
				return Handle == other.Handle;
			}
			
			bool IEnumerator<UnityEngine::RaycastHit>::operator!=(const IEnumerator<UnityEngine::RaycastHit>& other) const
			{
				return Handle != other.Handle;
			}
			
			UnityEngine::RaycastHit IEnumerator<UnityEngine::RaycastHit>::GetCurrent()
			{
				auto returnValue = Plugin::SystemCollectionsGenericIEnumeratorUnityEngineRaycastHitPropertyGetCurrent(Handle);
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
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			IEnumerator<UnityEngine::GradientColorKey>::IEnumerator(decltype(nullptr))
				: System::IDisposable(nullptr)
				, System::Collections::IEnumerator(nullptr)
			{
			}
			
			IEnumerator<UnityEngine::GradientColorKey>::IEnumerator(Plugin::InternalUse iu, int32_t handle)
				: System::IDisposable(nullptr)
				, System::Collections::IEnumerator(nullptr)
			{
				Handle = handle;
				if (handle)
				{
					Plugin::ReferenceManagedClass(handle);
				}
			}
			
			IEnumerator<UnityEngine::GradientColorKey>::IEnumerator(const IEnumerator<UnityEngine::GradientColorKey>& other)
				: IEnumerator(Plugin::InternalUse::Only, other.Handle)
			{
			}
			
			IEnumerator<UnityEngine::GradientColorKey>::IEnumerator(IEnumerator<UnityEngine::GradientColorKey>&& other)
				: IEnumerator(Plugin::InternalUse::Only, other.Handle)
			{
				other.Handle = 0;
			}
			
			IEnumerator<UnityEngine::GradientColorKey>::~IEnumerator<UnityEngine::GradientColorKey>()
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
			}
			
			IEnumerator<UnityEngine::GradientColorKey>& IEnumerator<UnityEngine::GradientColorKey>::operator=(const IEnumerator<UnityEngine::GradientColorKey>& other)
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
			
			IEnumerator<UnityEngine::GradientColorKey>& IEnumerator<UnityEngine::GradientColorKey>::operator=(decltype(nullptr))
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
				return *this;
			}
			
			IEnumerator<UnityEngine::GradientColorKey>& IEnumerator<UnityEngine::GradientColorKey>::operator=(IEnumerator<UnityEngine::GradientColorKey>&& other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
				}
				Handle = other.Handle;
				other.Handle = 0;
				return *this;
			}
			
			bool IEnumerator<UnityEngine::GradientColorKey>::operator==(const IEnumerator<UnityEngine::GradientColorKey>& other) const
			{
				return Handle == other.Handle;
			}
			
			bool IEnumerator<UnityEngine::GradientColorKey>::operator!=(const IEnumerator<UnityEngine::GradientColorKey>& other) const
			{
				return Handle != other.Handle;
			}
			
			UnityEngine::GradientColorKey IEnumerator<UnityEngine::GradientColorKey>::GetCurrent()
			{
				auto returnValue = Plugin::SystemCollectionsGenericIEnumeratorUnityEngineGradientColorKeyPropertyGetCurrent(Handle);
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
	namespace Collections
	{
		namespace Generic
		{
			IEnumerator<UnityEngine::Resolution>::IEnumerator(decltype(nullptr))
				: System::IDisposable(nullptr)
				, System::Collections::IEnumerator(nullptr)
			{
			}
			
			IEnumerator<UnityEngine::Resolution>::IEnumerator(Plugin::InternalUse iu, int32_t handle)
				: System::IDisposable(nullptr)
				, System::Collections::IEnumerator(nullptr)
			{
				Handle = handle;
				if (handle)
				{
					Plugin::ReferenceManagedClass(handle);
				}
			}
			
			IEnumerator<UnityEngine::Resolution>::IEnumerator(const IEnumerator<UnityEngine::Resolution>& other)
				: IEnumerator(Plugin::InternalUse::Only, other.Handle)
			{
			}
			
			IEnumerator<UnityEngine::Resolution>::IEnumerator(IEnumerator<UnityEngine::Resolution>&& other)
				: IEnumerator(Plugin::InternalUse::Only, other.Handle)
			{
				other.Handle = 0;
			}
			
			IEnumerator<UnityEngine::Resolution>::~IEnumerator<UnityEngine::Resolution>()
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
			}
			
			IEnumerator<UnityEngine::Resolution>& IEnumerator<UnityEngine::Resolution>::operator=(const IEnumerator<UnityEngine::Resolution>& other)
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
			
			IEnumerator<UnityEngine::Resolution>& IEnumerator<UnityEngine::Resolution>::operator=(decltype(nullptr))
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
				return *this;
			}
			
			IEnumerator<UnityEngine::Resolution>& IEnumerator<UnityEngine::Resolution>::operator=(IEnumerator<UnityEngine::Resolution>&& other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
				}
				Handle = other.Handle;
				other.Handle = 0;
				return *this;
			}
			
			bool IEnumerator<UnityEngine::Resolution>::operator==(const IEnumerator<UnityEngine::Resolution>& other) const
			{
				return Handle == other.Handle;
			}
			
			bool IEnumerator<UnityEngine::Resolution>::operator!=(const IEnumerator<UnityEngine::Resolution>& other) const
			{
				return Handle != other.Handle;
			}
			
			UnityEngine::Resolution IEnumerator<UnityEngine::Resolution>::GetCurrent()
			{
				auto returnValue = Plugin::SystemCollectionsGenericIEnumeratorUnityEngineResolutionPropertyGetCurrent(Handle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
				return UnityEngine::Resolution(Plugin::InternalUse::Only, returnValue);
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
			IEnumerable<System::String>::IEnumerable(decltype(nullptr))
				: System::Collections::IEnumerable(nullptr)
			{
			}
			
			IEnumerable<System::String>::IEnumerable(Plugin::InternalUse iu, int32_t handle)
				: System::Collections::IEnumerable(nullptr)
			{
				Handle = handle;
				if (handle)
				{
					Plugin::ReferenceManagedClass(handle);
				}
			}
			
			IEnumerable<System::String>::IEnumerable(const IEnumerable<System::String>& other)
				: IEnumerable(Plugin::InternalUse::Only, other.Handle)
			{
			}
			
			IEnumerable<System::String>::IEnumerable(IEnumerable<System::String>&& other)
				: IEnumerable(Plugin::InternalUse::Only, other.Handle)
			{
				other.Handle = 0;
			}
			
			IEnumerable<System::String>::~IEnumerable<System::String>()
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
			}
			
			IEnumerable<System::String>& IEnumerable<System::String>::operator=(const IEnumerable<System::String>& other)
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
			
			IEnumerable<System::String>& IEnumerable<System::String>::operator=(decltype(nullptr))
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
				return *this;
			}
			
			IEnumerable<System::String>& IEnumerable<System::String>::operator=(IEnumerable<System::String>&& other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
				}
				Handle = other.Handle;
				other.Handle = 0;
				return *this;
			}
			
			bool IEnumerable<System::String>::operator==(const IEnumerable<System::String>& other) const
			{
				return Handle == other.Handle;
			}
			
			bool IEnumerable<System::String>::operator!=(const IEnumerable<System::String>& other) const
			{
				return Handle != other.Handle;
			}
			
			System::Collections::Generic::IEnumerator<System::String> IEnumerable<System::String>::GetEnumerator()
			{
				auto returnValue = Plugin::SystemCollectionsGenericIEnumerableSystemStringMethodGetEnumerator(Handle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
				return System::Collections::Generic::IEnumerator<System::String>(Plugin::InternalUse::Only, returnValue);
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
			IEnumerable<int32_t>::IEnumerable(decltype(nullptr))
				: System::Collections::IEnumerable(nullptr)
			{
			}
			
			IEnumerable<int32_t>::IEnumerable(Plugin::InternalUse iu, int32_t handle)
				: System::Collections::IEnumerable(nullptr)
			{
				Handle = handle;
				if (handle)
				{
					Plugin::ReferenceManagedClass(handle);
				}
			}
			
			IEnumerable<int32_t>::IEnumerable(const IEnumerable<int32_t>& other)
				: IEnumerable(Plugin::InternalUse::Only, other.Handle)
			{
			}
			
			IEnumerable<int32_t>::IEnumerable(IEnumerable<int32_t>&& other)
				: IEnumerable(Plugin::InternalUse::Only, other.Handle)
			{
				other.Handle = 0;
			}
			
			IEnumerable<int32_t>::~IEnumerable<int32_t>()
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
			}
			
			IEnumerable<int32_t>& IEnumerable<int32_t>::operator=(const IEnumerable<int32_t>& other)
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
			
			IEnumerable<int32_t>& IEnumerable<int32_t>::operator=(decltype(nullptr))
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
				return *this;
			}
			
			IEnumerable<int32_t>& IEnumerable<int32_t>::operator=(IEnumerable<int32_t>&& other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
				}
				Handle = other.Handle;
				other.Handle = 0;
				return *this;
			}
			
			bool IEnumerable<int32_t>::operator==(const IEnumerable<int32_t>& other) const
			{
				return Handle == other.Handle;
			}
			
			bool IEnumerable<int32_t>::operator!=(const IEnumerable<int32_t>& other) const
			{
				return Handle != other.Handle;
			}
			
			System::Collections::Generic::IEnumerator<int32_t> IEnumerable<int32_t>::GetEnumerator()
			{
				auto returnValue = Plugin::SystemCollectionsGenericIEnumerableSystemInt32MethodGetEnumerator(Handle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
				return System::Collections::Generic::IEnumerator<int32_t>(Plugin::InternalUse::Only, returnValue);
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
			IEnumerable<float>::IEnumerable(decltype(nullptr))
				: System::Collections::IEnumerable(nullptr)
			{
			}
			
			IEnumerable<float>::IEnumerable(Plugin::InternalUse iu, int32_t handle)
				: System::Collections::IEnumerable(nullptr)
			{
				Handle = handle;
				if (handle)
				{
					Plugin::ReferenceManagedClass(handle);
				}
			}
			
			IEnumerable<float>::IEnumerable(const IEnumerable<float>& other)
				: IEnumerable(Plugin::InternalUse::Only, other.Handle)
			{
			}
			
			IEnumerable<float>::IEnumerable(IEnumerable<float>&& other)
				: IEnumerable(Plugin::InternalUse::Only, other.Handle)
			{
				other.Handle = 0;
			}
			
			IEnumerable<float>::~IEnumerable<float>()
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
			}
			
			IEnumerable<float>& IEnumerable<float>::operator=(const IEnumerable<float>& other)
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
			
			IEnumerable<float>& IEnumerable<float>::operator=(decltype(nullptr))
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
				return *this;
			}
			
			IEnumerable<float>& IEnumerable<float>::operator=(IEnumerable<float>&& other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
				}
				Handle = other.Handle;
				other.Handle = 0;
				return *this;
			}
			
			bool IEnumerable<float>::operator==(const IEnumerable<float>& other) const
			{
				return Handle == other.Handle;
			}
			
			bool IEnumerable<float>::operator!=(const IEnumerable<float>& other) const
			{
				return Handle != other.Handle;
			}
			
			System::Collections::Generic::IEnumerator<float> IEnumerable<float>::GetEnumerator()
			{
				auto returnValue = Plugin::SystemCollectionsGenericIEnumerableSystemSingleMethodGetEnumerator(Handle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
				return System::Collections::Generic::IEnumerator<float>(Plugin::InternalUse::Only, returnValue);
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
			IEnumerable<UnityEngine::RaycastHit>::IEnumerable(decltype(nullptr))
				: System::Collections::IEnumerable(nullptr)
			{
			}
			
			IEnumerable<UnityEngine::RaycastHit>::IEnumerable(Plugin::InternalUse iu, int32_t handle)
				: System::Collections::IEnumerable(nullptr)
			{
				Handle = handle;
				if (handle)
				{
					Plugin::ReferenceManagedClass(handle);
				}
			}
			
			IEnumerable<UnityEngine::RaycastHit>::IEnumerable(const IEnumerable<UnityEngine::RaycastHit>& other)
				: IEnumerable(Plugin::InternalUse::Only, other.Handle)
			{
			}
			
			IEnumerable<UnityEngine::RaycastHit>::IEnumerable(IEnumerable<UnityEngine::RaycastHit>&& other)
				: IEnumerable(Plugin::InternalUse::Only, other.Handle)
			{
				other.Handle = 0;
			}
			
			IEnumerable<UnityEngine::RaycastHit>::~IEnumerable<UnityEngine::RaycastHit>()
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
			}
			
			IEnumerable<UnityEngine::RaycastHit>& IEnumerable<UnityEngine::RaycastHit>::operator=(const IEnumerable<UnityEngine::RaycastHit>& other)
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
			
			IEnumerable<UnityEngine::RaycastHit>& IEnumerable<UnityEngine::RaycastHit>::operator=(decltype(nullptr))
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
				return *this;
			}
			
			IEnumerable<UnityEngine::RaycastHit>& IEnumerable<UnityEngine::RaycastHit>::operator=(IEnumerable<UnityEngine::RaycastHit>&& other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
				}
				Handle = other.Handle;
				other.Handle = 0;
				return *this;
			}
			
			bool IEnumerable<UnityEngine::RaycastHit>::operator==(const IEnumerable<UnityEngine::RaycastHit>& other) const
			{
				return Handle == other.Handle;
			}
			
			bool IEnumerable<UnityEngine::RaycastHit>::operator!=(const IEnumerable<UnityEngine::RaycastHit>& other) const
			{
				return Handle != other.Handle;
			}
			
			System::Collections::Generic::IEnumerator<UnityEngine::RaycastHit> IEnumerable<UnityEngine::RaycastHit>::GetEnumerator()
			{
				auto returnValue = Plugin::SystemCollectionsGenericIEnumerableUnityEngineRaycastHitMethodGetEnumerator(Handle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
				return System::Collections::Generic::IEnumerator<UnityEngine::RaycastHit>(Plugin::InternalUse::Only, returnValue);
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
			IEnumerable<UnityEngine::GradientColorKey>::IEnumerable(decltype(nullptr))
				: System::Collections::IEnumerable(nullptr)
			{
			}
			
			IEnumerable<UnityEngine::GradientColorKey>::IEnumerable(Plugin::InternalUse iu, int32_t handle)
				: System::Collections::IEnumerable(nullptr)
			{
				Handle = handle;
				if (handle)
				{
					Plugin::ReferenceManagedClass(handle);
				}
			}
			
			IEnumerable<UnityEngine::GradientColorKey>::IEnumerable(const IEnumerable<UnityEngine::GradientColorKey>& other)
				: IEnumerable(Plugin::InternalUse::Only, other.Handle)
			{
			}
			
			IEnumerable<UnityEngine::GradientColorKey>::IEnumerable(IEnumerable<UnityEngine::GradientColorKey>&& other)
				: IEnumerable(Plugin::InternalUse::Only, other.Handle)
			{
				other.Handle = 0;
			}
			
			IEnumerable<UnityEngine::GradientColorKey>::~IEnumerable<UnityEngine::GradientColorKey>()
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
			}
			
			IEnumerable<UnityEngine::GradientColorKey>& IEnumerable<UnityEngine::GradientColorKey>::operator=(const IEnumerable<UnityEngine::GradientColorKey>& other)
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
			
			IEnumerable<UnityEngine::GradientColorKey>& IEnumerable<UnityEngine::GradientColorKey>::operator=(decltype(nullptr))
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
				return *this;
			}
			
			IEnumerable<UnityEngine::GradientColorKey>& IEnumerable<UnityEngine::GradientColorKey>::operator=(IEnumerable<UnityEngine::GradientColorKey>&& other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
				}
				Handle = other.Handle;
				other.Handle = 0;
				return *this;
			}
			
			bool IEnumerable<UnityEngine::GradientColorKey>::operator==(const IEnumerable<UnityEngine::GradientColorKey>& other) const
			{
				return Handle == other.Handle;
			}
			
			bool IEnumerable<UnityEngine::GradientColorKey>::operator!=(const IEnumerable<UnityEngine::GradientColorKey>& other) const
			{
				return Handle != other.Handle;
			}
			
			System::Collections::Generic::IEnumerator<UnityEngine::GradientColorKey> IEnumerable<UnityEngine::GradientColorKey>::GetEnumerator()
			{
				auto returnValue = Plugin::SystemCollectionsGenericIEnumerableUnityEngineGradientColorKeyMethodGetEnumerator(Handle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
				return System::Collections::Generic::IEnumerator<UnityEngine::GradientColorKey>(Plugin::InternalUse::Only, returnValue);
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
			IEnumerable<UnityEngine::Resolution>::IEnumerable(decltype(nullptr))
				: System::Collections::IEnumerable(nullptr)
			{
			}
			
			IEnumerable<UnityEngine::Resolution>::IEnumerable(Plugin::InternalUse iu, int32_t handle)
				: System::Collections::IEnumerable(nullptr)
			{
				Handle = handle;
				if (handle)
				{
					Plugin::ReferenceManagedClass(handle);
				}
			}
			
			IEnumerable<UnityEngine::Resolution>::IEnumerable(const IEnumerable<UnityEngine::Resolution>& other)
				: IEnumerable(Plugin::InternalUse::Only, other.Handle)
			{
			}
			
			IEnumerable<UnityEngine::Resolution>::IEnumerable(IEnumerable<UnityEngine::Resolution>&& other)
				: IEnumerable(Plugin::InternalUse::Only, other.Handle)
			{
				other.Handle = 0;
			}
			
			IEnumerable<UnityEngine::Resolution>::~IEnumerable<UnityEngine::Resolution>()
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
			}
			
			IEnumerable<UnityEngine::Resolution>& IEnumerable<UnityEngine::Resolution>::operator=(const IEnumerable<UnityEngine::Resolution>& other)
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
			
			IEnumerable<UnityEngine::Resolution>& IEnumerable<UnityEngine::Resolution>::operator=(decltype(nullptr))
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
				return *this;
			}
			
			IEnumerable<UnityEngine::Resolution>& IEnumerable<UnityEngine::Resolution>::operator=(IEnumerable<UnityEngine::Resolution>&& other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
				}
				Handle = other.Handle;
				other.Handle = 0;
				return *this;
			}
			
			bool IEnumerable<UnityEngine::Resolution>::operator==(const IEnumerable<UnityEngine::Resolution>& other) const
			{
				return Handle == other.Handle;
			}
			
			bool IEnumerable<UnityEngine::Resolution>::operator!=(const IEnumerable<UnityEngine::Resolution>& other) const
			{
				return Handle != other.Handle;
			}
			
			System::Collections::Generic::IEnumerator<UnityEngine::Resolution> IEnumerable<UnityEngine::Resolution>::GetEnumerator()
			{
				auto returnValue = Plugin::SystemCollectionsGenericIEnumerableUnityEngineResolutionMethodGetEnumerator(Handle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
				return System::Collections::Generic::IEnumerator<UnityEngine::Resolution>(Plugin::InternalUse::Only, returnValue);
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
			ICollection<System::String>::ICollection(decltype(nullptr))
				: System::Collections::IEnumerable(nullptr)
				, System::Collections::Generic::IEnumerable<System::String>(nullptr)
			{
			}
			
			ICollection<System::String>::ICollection(Plugin::InternalUse iu, int32_t handle)
				: System::Collections::IEnumerable(nullptr)
				, System::Collections::Generic::IEnumerable<System::String>(nullptr)
			{
				Handle = handle;
				if (handle)
				{
					Plugin::ReferenceManagedClass(handle);
				}
			}
			
			ICollection<System::String>::ICollection(const ICollection<System::String>& other)
				: ICollection(Plugin::InternalUse::Only, other.Handle)
			{
			}
			
			ICollection<System::String>::ICollection(ICollection<System::String>&& other)
				: ICollection(Plugin::InternalUse::Only, other.Handle)
			{
				other.Handle = 0;
			}
			
			ICollection<System::String>::~ICollection<System::String>()
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
			}
			
			ICollection<System::String>& ICollection<System::String>::operator=(const ICollection<System::String>& other)
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
			
			ICollection<System::String>& ICollection<System::String>::operator=(decltype(nullptr))
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
				return *this;
			}
			
			ICollection<System::String>& ICollection<System::String>::operator=(ICollection<System::String>&& other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
				}
				Handle = other.Handle;
				other.Handle = 0;
				return *this;
			}
			
			bool ICollection<System::String>::operator==(const ICollection<System::String>& other) const
			{
				return Handle == other.Handle;
			}
			
			bool ICollection<System::String>::operator!=(const ICollection<System::String>& other) const
			{
				return Handle != other.Handle;
			}
		}
	}
}

namespace Plugin
{
	SystemCollectionsGenericICollectionSystemStringIterator::SystemCollectionsGenericICollectionSystemStringIterator(decltype(nullptr))
		: enumerator(nullptr)
		, hasMore(false)
	{
	}
	
	SystemCollectionsGenericICollectionSystemStringIterator::SystemCollectionsGenericICollectionSystemStringIterator(System::Collections::Generic::ICollection<System::String>& enumerable)
		: enumerator(enumerable.GetEnumerator())
	{
		hasMore = enumerator.MoveNext();
	}
	
	SystemCollectionsGenericICollectionSystemStringIterator::~SystemCollectionsGenericICollectionSystemStringIterator()
	{
		if (enumerator != nullptr)
		{
			enumerator.Dispose();
		}
	}
	
	SystemCollectionsGenericICollectionSystemStringIterator& SystemCollectionsGenericICollectionSystemStringIterator::operator++()
	{
		hasMore = enumerator.MoveNext();
		return *this;
	}
	
	bool SystemCollectionsGenericICollectionSystemStringIterator::operator!=(const SystemCollectionsGenericICollectionSystemStringIterator& other)
	{
		return hasMore;
	}
	
	System::String SystemCollectionsGenericICollectionSystemStringIterator::operator*()
	{
		return enumerator.GetCurrent();
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			Plugin::SystemCollectionsGenericICollectionSystemStringIterator begin(System::Collections::Generic::ICollection<System::String>& enumerable)
			{
				return Plugin::SystemCollectionsGenericICollectionSystemStringIterator(enumerable);
			}
			
			Plugin::SystemCollectionsGenericICollectionSystemStringIterator end(System::Collections::Generic::ICollection<System::String>& enumerable)
			{
				return Plugin::SystemCollectionsGenericICollectionSystemStringIterator(nullptr);
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
			ICollection<int32_t>::ICollection(decltype(nullptr))
				: System::Collections::IEnumerable(nullptr)
				, System::Collections::Generic::IEnumerable<int32_t>(nullptr)
			{
			}
			
			ICollection<int32_t>::ICollection(Plugin::InternalUse iu, int32_t handle)
				: System::Collections::IEnumerable(nullptr)
				, System::Collections::Generic::IEnumerable<int32_t>(nullptr)
			{
				Handle = handle;
				if (handle)
				{
					Plugin::ReferenceManagedClass(handle);
				}
			}
			
			ICollection<int32_t>::ICollection(const ICollection<int32_t>& other)
				: ICollection(Plugin::InternalUse::Only, other.Handle)
			{
			}
			
			ICollection<int32_t>::ICollection(ICollection<int32_t>&& other)
				: ICollection(Plugin::InternalUse::Only, other.Handle)
			{
				other.Handle = 0;
			}
			
			ICollection<int32_t>::~ICollection<int32_t>()
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
			}
			
			ICollection<int32_t>& ICollection<int32_t>::operator=(const ICollection<int32_t>& other)
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
			
			ICollection<int32_t>& ICollection<int32_t>::operator=(decltype(nullptr))
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
				return *this;
			}
			
			ICollection<int32_t>& ICollection<int32_t>::operator=(ICollection<int32_t>&& other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
				}
				Handle = other.Handle;
				other.Handle = 0;
				return *this;
			}
			
			bool ICollection<int32_t>::operator==(const ICollection<int32_t>& other) const
			{
				return Handle == other.Handle;
			}
			
			bool ICollection<int32_t>::operator!=(const ICollection<int32_t>& other) const
			{
				return Handle != other.Handle;
			}
		}
	}
}

namespace Plugin
{
	SystemCollectionsGenericICollectionSystemInt32Iterator::SystemCollectionsGenericICollectionSystemInt32Iterator(decltype(nullptr))
		: enumerator(nullptr)
		, hasMore(false)
	{
	}
	
	SystemCollectionsGenericICollectionSystemInt32Iterator::SystemCollectionsGenericICollectionSystemInt32Iterator(System::Collections::Generic::ICollection<int32_t>& enumerable)
		: enumerator(enumerable.GetEnumerator())
	{
		hasMore = enumerator.MoveNext();
	}
	
	SystemCollectionsGenericICollectionSystemInt32Iterator::~SystemCollectionsGenericICollectionSystemInt32Iterator()
	{
		if (enumerator != nullptr)
		{
			enumerator.Dispose();
		}
	}
	
	SystemCollectionsGenericICollectionSystemInt32Iterator& SystemCollectionsGenericICollectionSystemInt32Iterator::operator++()
	{
		hasMore = enumerator.MoveNext();
		return *this;
	}
	
	bool SystemCollectionsGenericICollectionSystemInt32Iterator::operator!=(const SystemCollectionsGenericICollectionSystemInt32Iterator& other)
	{
		return hasMore;
	}
	
	int32_t SystemCollectionsGenericICollectionSystemInt32Iterator::operator*()
	{
		return enumerator.GetCurrent();
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			Plugin::SystemCollectionsGenericICollectionSystemInt32Iterator begin(System::Collections::Generic::ICollection<int32_t>& enumerable)
			{
				return Plugin::SystemCollectionsGenericICollectionSystemInt32Iterator(enumerable);
			}
			
			Plugin::SystemCollectionsGenericICollectionSystemInt32Iterator end(System::Collections::Generic::ICollection<int32_t>& enumerable)
			{
				return Plugin::SystemCollectionsGenericICollectionSystemInt32Iterator(nullptr);
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
			ICollection<float>::ICollection(decltype(nullptr))
				: System::Collections::IEnumerable(nullptr)
				, System::Collections::Generic::IEnumerable<float>(nullptr)
			{
			}
			
			ICollection<float>::ICollection(Plugin::InternalUse iu, int32_t handle)
				: System::Collections::IEnumerable(nullptr)
				, System::Collections::Generic::IEnumerable<float>(nullptr)
			{
				Handle = handle;
				if (handle)
				{
					Plugin::ReferenceManagedClass(handle);
				}
			}
			
			ICollection<float>::ICollection(const ICollection<float>& other)
				: ICollection(Plugin::InternalUse::Only, other.Handle)
			{
			}
			
			ICollection<float>::ICollection(ICollection<float>&& other)
				: ICollection(Plugin::InternalUse::Only, other.Handle)
			{
				other.Handle = 0;
			}
			
			ICollection<float>::~ICollection<float>()
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
			}
			
			ICollection<float>& ICollection<float>::operator=(const ICollection<float>& other)
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
			
			ICollection<float>& ICollection<float>::operator=(decltype(nullptr))
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
				return *this;
			}
			
			ICollection<float>& ICollection<float>::operator=(ICollection<float>&& other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
				}
				Handle = other.Handle;
				other.Handle = 0;
				return *this;
			}
			
			bool ICollection<float>::operator==(const ICollection<float>& other) const
			{
				return Handle == other.Handle;
			}
			
			bool ICollection<float>::operator!=(const ICollection<float>& other) const
			{
				return Handle != other.Handle;
			}
		}
	}
}

namespace Plugin
{
	SystemCollectionsGenericICollectionSystemSingleIterator::SystemCollectionsGenericICollectionSystemSingleIterator(decltype(nullptr))
		: enumerator(nullptr)
		, hasMore(false)
	{
	}
	
	SystemCollectionsGenericICollectionSystemSingleIterator::SystemCollectionsGenericICollectionSystemSingleIterator(System::Collections::Generic::ICollection<float>& enumerable)
		: enumerator(enumerable.GetEnumerator())
	{
		hasMore = enumerator.MoveNext();
	}
	
	SystemCollectionsGenericICollectionSystemSingleIterator::~SystemCollectionsGenericICollectionSystemSingleIterator()
	{
		if (enumerator != nullptr)
		{
			enumerator.Dispose();
		}
	}
	
	SystemCollectionsGenericICollectionSystemSingleIterator& SystemCollectionsGenericICollectionSystemSingleIterator::operator++()
	{
		hasMore = enumerator.MoveNext();
		return *this;
	}
	
	bool SystemCollectionsGenericICollectionSystemSingleIterator::operator!=(const SystemCollectionsGenericICollectionSystemSingleIterator& other)
	{
		return hasMore;
	}
	
	float SystemCollectionsGenericICollectionSystemSingleIterator::operator*()
	{
		return enumerator.GetCurrent();
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			Plugin::SystemCollectionsGenericICollectionSystemSingleIterator begin(System::Collections::Generic::ICollection<float>& enumerable)
			{
				return Plugin::SystemCollectionsGenericICollectionSystemSingleIterator(enumerable);
			}
			
			Plugin::SystemCollectionsGenericICollectionSystemSingleIterator end(System::Collections::Generic::ICollection<float>& enumerable)
			{
				return Plugin::SystemCollectionsGenericICollectionSystemSingleIterator(nullptr);
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
			ICollection<UnityEngine::RaycastHit>::ICollection(decltype(nullptr))
				: System::Collections::IEnumerable(nullptr)
				, System::Collections::Generic::IEnumerable<UnityEngine::RaycastHit>(nullptr)
			{
			}
			
			ICollection<UnityEngine::RaycastHit>::ICollection(Plugin::InternalUse iu, int32_t handle)
				: System::Collections::IEnumerable(nullptr)
				, System::Collections::Generic::IEnumerable<UnityEngine::RaycastHit>(nullptr)
			{
				Handle = handle;
				if (handle)
				{
					Plugin::ReferenceManagedClass(handle);
				}
			}
			
			ICollection<UnityEngine::RaycastHit>::ICollection(const ICollection<UnityEngine::RaycastHit>& other)
				: ICollection(Plugin::InternalUse::Only, other.Handle)
			{
			}
			
			ICollection<UnityEngine::RaycastHit>::ICollection(ICollection<UnityEngine::RaycastHit>&& other)
				: ICollection(Plugin::InternalUse::Only, other.Handle)
			{
				other.Handle = 0;
			}
			
			ICollection<UnityEngine::RaycastHit>::~ICollection<UnityEngine::RaycastHit>()
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
			}
			
			ICollection<UnityEngine::RaycastHit>& ICollection<UnityEngine::RaycastHit>::operator=(const ICollection<UnityEngine::RaycastHit>& other)
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
			
			ICollection<UnityEngine::RaycastHit>& ICollection<UnityEngine::RaycastHit>::operator=(decltype(nullptr))
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
				return *this;
			}
			
			ICollection<UnityEngine::RaycastHit>& ICollection<UnityEngine::RaycastHit>::operator=(ICollection<UnityEngine::RaycastHit>&& other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
				}
				Handle = other.Handle;
				other.Handle = 0;
				return *this;
			}
			
			bool ICollection<UnityEngine::RaycastHit>::operator==(const ICollection<UnityEngine::RaycastHit>& other) const
			{
				return Handle == other.Handle;
			}
			
			bool ICollection<UnityEngine::RaycastHit>::operator!=(const ICollection<UnityEngine::RaycastHit>& other) const
			{
				return Handle != other.Handle;
			}
		}
	}
}

namespace Plugin
{
	SystemCollectionsGenericICollectionUnityEngineRaycastHitIterator::SystemCollectionsGenericICollectionUnityEngineRaycastHitIterator(decltype(nullptr))
		: enumerator(nullptr)
		, hasMore(false)
	{
	}
	
	SystemCollectionsGenericICollectionUnityEngineRaycastHitIterator::SystemCollectionsGenericICollectionUnityEngineRaycastHitIterator(System::Collections::Generic::ICollection<UnityEngine::RaycastHit>& enumerable)
		: enumerator(enumerable.GetEnumerator())
	{
		hasMore = enumerator.MoveNext();
	}
	
	SystemCollectionsGenericICollectionUnityEngineRaycastHitIterator::~SystemCollectionsGenericICollectionUnityEngineRaycastHitIterator()
	{
		if (enumerator != nullptr)
		{
			enumerator.Dispose();
		}
	}
	
	SystemCollectionsGenericICollectionUnityEngineRaycastHitIterator& SystemCollectionsGenericICollectionUnityEngineRaycastHitIterator::operator++()
	{
		hasMore = enumerator.MoveNext();
		return *this;
	}
	
	bool SystemCollectionsGenericICollectionUnityEngineRaycastHitIterator::operator!=(const SystemCollectionsGenericICollectionUnityEngineRaycastHitIterator& other)
	{
		return hasMore;
	}
	
	UnityEngine::RaycastHit SystemCollectionsGenericICollectionUnityEngineRaycastHitIterator::operator*()
	{
		return enumerator.GetCurrent();
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			Plugin::SystemCollectionsGenericICollectionUnityEngineRaycastHitIterator begin(System::Collections::Generic::ICollection<UnityEngine::RaycastHit>& enumerable)
			{
				return Plugin::SystemCollectionsGenericICollectionUnityEngineRaycastHitIterator(enumerable);
			}
			
			Plugin::SystemCollectionsGenericICollectionUnityEngineRaycastHitIterator end(System::Collections::Generic::ICollection<UnityEngine::RaycastHit>& enumerable)
			{
				return Plugin::SystemCollectionsGenericICollectionUnityEngineRaycastHitIterator(nullptr);
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
			ICollection<UnityEngine::GradientColorKey>::ICollection(decltype(nullptr))
				: System::Collections::IEnumerable(nullptr)
				, System::Collections::Generic::IEnumerable<UnityEngine::GradientColorKey>(nullptr)
			{
			}
			
			ICollection<UnityEngine::GradientColorKey>::ICollection(Plugin::InternalUse iu, int32_t handle)
				: System::Collections::IEnumerable(nullptr)
				, System::Collections::Generic::IEnumerable<UnityEngine::GradientColorKey>(nullptr)
			{
				Handle = handle;
				if (handle)
				{
					Plugin::ReferenceManagedClass(handle);
				}
			}
			
			ICollection<UnityEngine::GradientColorKey>::ICollection(const ICollection<UnityEngine::GradientColorKey>& other)
				: ICollection(Plugin::InternalUse::Only, other.Handle)
			{
			}
			
			ICollection<UnityEngine::GradientColorKey>::ICollection(ICollection<UnityEngine::GradientColorKey>&& other)
				: ICollection(Plugin::InternalUse::Only, other.Handle)
			{
				other.Handle = 0;
			}
			
			ICollection<UnityEngine::GradientColorKey>::~ICollection<UnityEngine::GradientColorKey>()
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
			}
			
			ICollection<UnityEngine::GradientColorKey>& ICollection<UnityEngine::GradientColorKey>::operator=(const ICollection<UnityEngine::GradientColorKey>& other)
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
			
			ICollection<UnityEngine::GradientColorKey>& ICollection<UnityEngine::GradientColorKey>::operator=(decltype(nullptr))
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
				return *this;
			}
			
			ICollection<UnityEngine::GradientColorKey>& ICollection<UnityEngine::GradientColorKey>::operator=(ICollection<UnityEngine::GradientColorKey>&& other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
				}
				Handle = other.Handle;
				other.Handle = 0;
				return *this;
			}
			
			bool ICollection<UnityEngine::GradientColorKey>::operator==(const ICollection<UnityEngine::GradientColorKey>& other) const
			{
				return Handle == other.Handle;
			}
			
			bool ICollection<UnityEngine::GradientColorKey>::operator!=(const ICollection<UnityEngine::GradientColorKey>& other) const
			{
				return Handle != other.Handle;
			}
		}
	}
}

namespace Plugin
{
	SystemCollectionsGenericICollectionUnityEngineGradientColorKeyIterator::SystemCollectionsGenericICollectionUnityEngineGradientColorKeyIterator(decltype(nullptr))
		: enumerator(nullptr)
		, hasMore(false)
	{
	}
	
	SystemCollectionsGenericICollectionUnityEngineGradientColorKeyIterator::SystemCollectionsGenericICollectionUnityEngineGradientColorKeyIterator(System::Collections::Generic::ICollection<UnityEngine::GradientColorKey>& enumerable)
		: enumerator(enumerable.GetEnumerator())
	{
		hasMore = enumerator.MoveNext();
	}
	
	SystemCollectionsGenericICollectionUnityEngineGradientColorKeyIterator::~SystemCollectionsGenericICollectionUnityEngineGradientColorKeyIterator()
	{
		if (enumerator != nullptr)
		{
			enumerator.Dispose();
		}
	}
	
	SystemCollectionsGenericICollectionUnityEngineGradientColorKeyIterator& SystemCollectionsGenericICollectionUnityEngineGradientColorKeyIterator::operator++()
	{
		hasMore = enumerator.MoveNext();
		return *this;
	}
	
	bool SystemCollectionsGenericICollectionUnityEngineGradientColorKeyIterator::operator!=(const SystemCollectionsGenericICollectionUnityEngineGradientColorKeyIterator& other)
	{
		return hasMore;
	}
	
	UnityEngine::GradientColorKey SystemCollectionsGenericICollectionUnityEngineGradientColorKeyIterator::operator*()
	{
		return enumerator.GetCurrent();
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			Plugin::SystemCollectionsGenericICollectionUnityEngineGradientColorKeyIterator begin(System::Collections::Generic::ICollection<UnityEngine::GradientColorKey>& enumerable)
			{
				return Plugin::SystemCollectionsGenericICollectionUnityEngineGradientColorKeyIterator(enumerable);
			}
			
			Plugin::SystemCollectionsGenericICollectionUnityEngineGradientColorKeyIterator end(System::Collections::Generic::ICollection<UnityEngine::GradientColorKey>& enumerable)
			{
				return Plugin::SystemCollectionsGenericICollectionUnityEngineGradientColorKeyIterator(nullptr);
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
			ICollection<UnityEngine::Resolution>::ICollection(decltype(nullptr))
				: System::Collections::IEnumerable(nullptr)
				, System::Collections::Generic::IEnumerable<UnityEngine::Resolution>(nullptr)
			{
			}
			
			ICollection<UnityEngine::Resolution>::ICollection(Plugin::InternalUse iu, int32_t handle)
				: System::Collections::IEnumerable(nullptr)
				, System::Collections::Generic::IEnumerable<UnityEngine::Resolution>(nullptr)
			{
				Handle = handle;
				if (handle)
				{
					Plugin::ReferenceManagedClass(handle);
				}
			}
			
			ICollection<UnityEngine::Resolution>::ICollection(const ICollection<UnityEngine::Resolution>& other)
				: ICollection(Plugin::InternalUse::Only, other.Handle)
			{
			}
			
			ICollection<UnityEngine::Resolution>::ICollection(ICollection<UnityEngine::Resolution>&& other)
				: ICollection(Plugin::InternalUse::Only, other.Handle)
			{
				other.Handle = 0;
			}
			
			ICollection<UnityEngine::Resolution>::~ICollection<UnityEngine::Resolution>()
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
			}
			
			ICollection<UnityEngine::Resolution>& ICollection<UnityEngine::Resolution>::operator=(const ICollection<UnityEngine::Resolution>& other)
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
			
			ICollection<UnityEngine::Resolution>& ICollection<UnityEngine::Resolution>::operator=(decltype(nullptr))
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
				return *this;
			}
			
			ICollection<UnityEngine::Resolution>& ICollection<UnityEngine::Resolution>::operator=(ICollection<UnityEngine::Resolution>&& other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
				}
				Handle = other.Handle;
				other.Handle = 0;
				return *this;
			}
			
			bool ICollection<UnityEngine::Resolution>::operator==(const ICollection<UnityEngine::Resolution>& other) const
			{
				return Handle == other.Handle;
			}
			
			bool ICollection<UnityEngine::Resolution>::operator!=(const ICollection<UnityEngine::Resolution>& other) const
			{
				return Handle != other.Handle;
			}
		}
	}
}

namespace Plugin
{
	SystemCollectionsGenericICollectionUnityEngineResolutionIterator::SystemCollectionsGenericICollectionUnityEngineResolutionIterator(decltype(nullptr))
		: enumerator(nullptr)
		, hasMore(false)
	{
	}
	
	SystemCollectionsGenericICollectionUnityEngineResolutionIterator::SystemCollectionsGenericICollectionUnityEngineResolutionIterator(System::Collections::Generic::ICollection<UnityEngine::Resolution>& enumerable)
		: enumerator(enumerable.GetEnumerator())
	{
		hasMore = enumerator.MoveNext();
	}
	
	SystemCollectionsGenericICollectionUnityEngineResolutionIterator::~SystemCollectionsGenericICollectionUnityEngineResolutionIterator()
	{
		if (enumerator != nullptr)
		{
			enumerator.Dispose();
		}
	}
	
	SystemCollectionsGenericICollectionUnityEngineResolutionIterator& SystemCollectionsGenericICollectionUnityEngineResolutionIterator::operator++()
	{
		hasMore = enumerator.MoveNext();
		return *this;
	}
	
	bool SystemCollectionsGenericICollectionUnityEngineResolutionIterator::operator!=(const SystemCollectionsGenericICollectionUnityEngineResolutionIterator& other)
	{
		return hasMore;
	}
	
	UnityEngine::Resolution SystemCollectionsGenericICollectionUnityEngineResolutionIterator::operator*()
	{
		return enumerator.GetCurrent();
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			Plugin::SystemCollectionsGenericICollectionUnityEngineResolutionIterator begin(System::Collections::Generic::ICollection<UnityEngine::Resolution>& enumerable)
			{
				return Plugin::SystemCollectionsGenericICollectionUnityEngineResolutionIterator(enumerable);
			}
			
			Plugin::SystemCollectionsGenericICollectionUnityEngineResolutionIterator end(System::Collections::Generic::ICollection<UnityEngine::Resolution>& enumerable)
			{
				return Plugin::SystemCollectionsGenericICollectionUnityEngineResolutionIterator(nullptr);
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
			IList<System::String>::IList(decltype(nullptr))
				: System::Collections::IEnumerable(nullptr)
				, System::Collections::Generic::IEnumerable<System::String>(nullptr)
				, System::Collections::Generic::ICollection<System::String>(nullptr)
			{
			}
			
			IList<System::String>::IList(Plugin::InternalUse iu, int32_t handle)
				: System::Collections::IEnumerable(nullptr)
				, System::Collections::Generic::IEnumerable<System::String>(nullptr)
				, System::Collections::Generic::ICollection<System::String>(nullptr)
			{
				Handle = handle;
				if (handle)
				{
					Plugin::ReferenceManagedClass(handle);
				}
			}
			
			IList<System::String>::IList(const IList<System::String>& other)
				: IList(Plugin::InternalUse::Only, other.Handle)
			{
			}
			
			IList<System::String>::IList(IList<System::String>&& other)
				: IList(Plugin::InternalUse::Only, other.Handle)
			{
				other.Handle = 0;
			}
			
			IList<System::String>::~IList<System::String>()
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
			}
			
			IList<System::String>& IList<System::String>::operator=(const IList<System::String>& other)
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
			
			IList<System::String>& IList<System::String>::operator=(decltype(nullptr))
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
				return *this;
			}
			
			IList<System::String>& IList<System::String>::operator=(IList<System::String>&& other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
				}
				Handle = other.Handle;
				other.Handle = 0;
				return *this;
			}
			
			bool IList<System::String>::operator==(const IList<System::String>& other) const
			{
				return Handle == other.Handle;
			}
			
			bool IList<System::String>::operator!=(const IList<System::String>& other) const
			{
				return Handle != other.Handle;
			}
		}
	}
}

namespace Plugin
{
	SystemCollectionsGenericIListSystemStringIterator::SystemCollectionsGenericIListSystemStringIterator(decltype(nullptr))
		: enumerator(nullptr)
		, hasMore(false)
	{
	}
	
	SystemCollectionsGenericIListSystemStringIterator::SystemCollectionsGenericIListSystemStringIterator(System::Collections::Generic::IList<System::String>& enumerable)
		: enumerator(enumerable.GetEnumerator())
	{
		hasMore = enumerator.MoveNext();
	}
	
	SystemCollectionsGenericIListSystemStringIterator::~SystemCollectionsGenericIListSystemStringIterator()
	{
		if (enumerator != nullptr)
		{
			enumerator.Dispose();
		}
	}
	
	SystemCollectionsGenericIListSystemStringIterator& SystemCollectionsGenericIListSystemStringIterator::operator++()
	{
		hasMore = enumerator.MoveNext();
		return *this;
	}
	
	bool SystemCollectionsGenericIListSystemStringIterator::operator!=(const SystemCollectionsGenericIListSystemStringIterator& other)
	{
		return hasMore;
	}
	
	System::String SystemCollectionsGenericIListSystemStringIterator::operator*()
	{
		return enumerator.GetCurrent();
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			Plugin::SystemCollectionsGenericIListSystemStringIterator begin(System::Collections::Generic::IList<System::String>& enumerable)
			{
				return Plugin::SystemCollectionsGenericIListSystemStringIterator(enumerable);
			}
			
			Plugin::SystemCollectionsGenericIListSystemStringIterator end(System::Collections::Generic::IList<System::String>& enumerable)
			{
				return Plugin::SystemCollectionsGenericIListSystemStringIterator(nullptr);
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
			IList<int32_t>::IList(decltype(nullptr))
				: System::Collections::IEnumerable(nullptr)
				, System::Collections::Generic::IEnumerable<int32_t>(nullptr)
				, System::Collections::Generic::ICollection<int32_t>(nullptr)
			{
			}
			
			IList<int32_t>::IList(Plugin::InternalUse iu, int32_t handle)
				: System::Collections::IEnumerable(nullptr)
				, System::Collections::Generic::IEnumerable<int32_t>(nullptr)
				, System::Collections::Generic::ICollection<int32_t>(nullptr)
			{
				Handle = handle;
				if (handle)
				{
					Plugin::ReferenceManagedClass(handle);
				}
			}
			
			IList<int32_t>::IList(const IList<int32_t>& other)
				: IList(Plugin::InternalUse::Only, other.Handle)
			{
			}
			
			IList<int32_t>::IList(IList<int32_t>&& other)
				: IList(Plugin::InternalUse::Only, other.Handle)
			{
				other.Handle = 0;
			}
			
			IList<int32_t>::~IList<int32_t>()
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
			}
			
			IList<int32_t>& IList<int32_t>::operator=(const IList<int32_t>& other)
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
			
			IList<int32_t>& IList<int32_t>::operator=(decltype(nullptr))
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
				return *this;
			}
			
			IList<int32_t>& IList<int32_t>::operator=(IList<int32_t>&& other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
				}
				Handle = other.Handle;
				other.Handle = 0;
				return *this;
			}
			
			bool IList<int32_t>::operator==(const IList<int32_t>& other) const
			{
				return Handle == other.Handle;
			}
			
			bool IList<int32_t>::operator!=(const IList<int32_t>& other) const
			{
				return Handle != other.Handle;
			}
		}
	}
}

namespace Plugin
{
	SystemCollectionsGenericIListSystemInt32Iterator::SystemCollectionsGenericIListSystemInt32Iterator(decltype(nullptr))
		: enumerator(nullptr)
		, hasMore(false)
	{
	}
	
	SystemCollectionsGenericIListSystemInt32Iterator::SystemCollectionsGenericIListSystemInt32Iterator(System::Collections::Generic::IList<int32_t>& enumerable)
		: enumerator(enumerable.GetEnumerator())
	{
		hasMore = enumerator.MoveNext();
	}
	
	SystemCollectionsGenericIListSystemInt32Iterator::~SystemCollectionsGenericIListSystemInt32Iterator()
	{
		if (enumerator != nullptr)
		{
			enumerator.Dispose();
		}
	}
	
	SystemCollectionsGenericIListSystemInt32Iterator& SystemCollectionsGenericIListSystemInt32Iterator::operator++()
	{
		hasMore = enumerator.MoveNext();
		return *this;
	}
	
	bool SystemCollectionsGenericIListSystemInt32Iterator::operator!=(const SystemCollectionsGenericIListSystemInt32Iterator& other)
	{
		return hasMore;
	}
	
	int32_t SystemCollectionsGenericIListSystemInt32Iterator::operator*()
	{
		return enumerator.GetCurrent();
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			Plugin::SystemCollectionsGenericIListSystemInt32Iterator begin(System::Collections::Generic::IList<int32_t>& enumerable)
			{
				return Plugin::SystemCollectionsGenericIListSystemInt32Iterator(enumerable);
			}
			
			Plugin::SystemCollectionsGenericIListSystemInt32Iterator end(System::Collections::Generic::IList<int32_t>& enumerable)
			{
				return Plugin::SystemCollectionsGenericIListSystemInt32Iterator(nullptr);
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
			IList<float>::IList(decltype(nullptr))
				: System::Collections::IEnumerable(nullptr)
				, System::Collections::Generic::IEnumerable<float>(nullptr)
				, System::Collections::Generic::ICollection<float>(nullptr)
			{
			}
			
			IList<float>::IList(Plugin::InternalUse iu, int32_t handle)
				: System::Collections::IEnumerable(nullptr)
				, System::Collections::Generic::IEnumerable<float>(nullptr)
				, System::Collections::Generic::ICollection<float>(nullptr)
			{
				Handle = handle;
				if (handle)
				{
					Plugin::ReferenceManagedClass(handle);
				}
			}
			
			IList<float>::IList(const IList<float>& other)
				: IList(Plugin::InternalUse::Only, other.Handle)
			{
			}
			
			IList<float>::IList(IList<float>&& other)
				: IList(Plugin::InternalUse::Only, other.Handle)
			{
				other.Handle = 0;
			}
			
			IList<float>::~IList<float>()
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
			}
			
			IList<float>& IList<float>::operator=(const IList<float>& other)
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
			
			IList<float>& IList<float>::operator=(decltype(nullptr))
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
				return *this;
			}
			
			IList<float>& IList<float>::operator=(IList<float>&& other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
				}
				Handle = other.Handle;
				other.Handle = 0;
				return *this;
			}
			
			bool IList<float>::operator==(const IList<float>& other) const
			{
				return Handle == other.Handle;
			}
			
			bool IList<float>::operator!=(const IList<float>& other) const
			{
				return Handle != other.Handle;
			}
		}
	}
}

namespace Plugin
{
	SystemCollectionsGenericIListSystemSingleIterator::SystemCollectionsGenericIListSystemSingleIterator(decltype(nullptr))
		: enumerator(nullptr)
		, hasMore(false)
	{
	}
	
	SystemCollectionsGenericIListSystemSingleIterator::SystemCollectionsGenericIListSystemSingleIterator(System::Collections::Generic::IList<float>& enumerable)
		: enumerator(enumerable.GetEnumerator())
	{
		hasMore = enumerator.MoveNext();
	}
	
	SystemCollectionsGenericIListSystemSingleIterator::~SystemCollectionsGenericIListSystemSingleIterator()
	{
		if (enumerator != nullptr)
		{
			enumerator.Dispose();
		}
	}
	
	SystemCollectionsGenericIListSystemSingleIterator& SystemCollectionsGenericIListSystemSingleIterator::operator++()
	{
		hasMore = enumerator.MoveNext();
		return *this;
	}
	
	bool SystemCollectionsGenericIListSystemSingleIterator::operator!=(const SystemCollectionsGenericIListSystemSingleIterator& other)
	{
		return hasMore;
	}
	
	float SystemCollectionsGenericIListSystemSingleIterator::operator*()
	{
		return enumerator.GetCurrent();
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			Plugin::SystemCollectionsGenericIListSystemSingleIterator begin(System::Collections::Generic::IList<float>& enumerable)
			{
				return Plugin::SystemCollectionsGenericIListSystemSingleIterator(enumerable);
			}
			
			Plugin::SystemCollectionsGenericIListSystemSingleIterator end(System::Collections::Generic::IList<float>& enumerable)
			{
				return Plugin::SystemCollectionsGenericIListSystemSingleIterator(nullptr);
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
			IList<UnityEngine::RaycastHit>::IList(decltype(nullptr))
				: System::Collections::IEnumerable(nullptr)
				, System::Collections::Generic::IEnumerable<UnityEngine::RaycastHit>(nullptr)
				, System::Collections::Generic::ICollection<UnityEngine::RaycastHit>(nullptr)
			{
			}
			
			IList<UnityEngine::RaycastHit>::IList(Plugin::InternalUse iu, int32_t handle)
				: System::Collections::IEnumerable(nullptr)
				, System::Collections::Generic::IEnumerable<UnityEngine::RaycastHit>(nullptr)
				, System::Collections::Generic::ICollection<UnityEngine::RaycastHit>(nullptr)
			{
				Handle = handle;
				if (handle)
				{
					Plugin::ReferenceManagedClass(handle);
				}
			}
			
			IList<UnityEngine::RaycastHit>::IList(const IList<UnityEngine::RaycastHit>& other)
				: IList(Plugin::InternalUse::Only, other.Handle)
			{
			}
			
			IList<UnityEngine::RaycastHit>::IList(IList<UnityEngine::RaycastHit>&& other)
				: IList(Plugin::InternalUse::Only, other.Handle)
			{
				other.Handle = 0;
			}
			
			IList<UnityEngine::RaycastHit>::~IList<UnityEngine::RaycastHit>()
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
			}
			
			IList<UnityEngine::RaycastHit>& IList<UnityEngine::RaycastHit>::operator=(const IList<UnityEngine::RaycastHit>& other)
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
			
			IList<UnityEngine::RaycastHit>& IList<UnityEngine::RaycastHit>::operator=(decltype(nullptr))
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
				return *this;
			}
			
			IList<UnityEngine::RaycastHit>& IList<UnityEngine::RaycastHit>::operator=(IList<UnityEngine::RaycastHit>&& other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
				}
				Handle = other.Handle;
				other.Handle = 0;
				return *this;
			}
			
			bool IList<UnityEngine::RaycastHit>::operator==(const IList<UnityEngine::RaycastHit>& other) const
			{
				return Handle == other.Handle;
			}
			
			bool IList<UnityEngine::RaycastHit>::operator!=(const IList<UnityEngine::RaycastHit>& other) const
			{
				return Handle != other.Handle;
			}
		}
	}
}

namespace Plugin
{
	SystemCollectionsGenericIListUnityEngineRaycastHitIterator::SystemCollectionsGenericIListUnityEngineRaycastHitIterator(decltype(nullptr))
		: enumerator(nullptr)
		, hasMore(false)
	{
	}
	
	SystemCollectionsGenericIListUnityEngineRaycastHitIterator::SystemCollectionsGenericIListUnityEngineRaycastHitIterator(System::Collections::Generic::IList<UnityEngine::RaycastHit>& enumerable)
		: enumerator(enumerable.GetEnumerator())
	{
		hasMore = enumerator.MoveNext();
	}
	
	SystemCollectionsGenericIListUnityEngineRaycastHitIterator::~SystemCollectionsGenericIListUnityEngineRaycastHitIterator()
	{
		if (enumerator != nullptr)
		{
			enumerator.Dispose();
		}
	}
	
	SystemCollectionsGenericIListUnityEngineRaycastHitIterator& SystemCollectionsGenericIListUnityEngineRaycastHitIterator::operator++()
	{
		hasMore = enumerator.MoveNext();
		return *this;
	}
	
	bool SystemCollectionsGenericIListUnityEngineRaycastHitIterator::operator!=(const SystemCollectionsGenericIListUnityEngineRaycastHitIterator& other)
	{
		return hasMore;
	}
	
	UnityEngine::RaycastHit SystemCollectionsGenericIListUnityEngineRaycastHitIterator::operator*()
	{
		return enumerator.GetCurrent();
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			Plugin::SystemCollectionsGenericIListUnityEngineRaycastHitIterator begin(System::Collections::Generic::IList<UnityEngine::RaycastHit>& enumerable)
			{
				return Plugin::SystemCollectionsGenericIListUnityEngineRaycastHitIterator(enumerable);
			}
			
			Plugin::SystemCollectionsGenericIListUnityEngineRaycastHitIterator end(System::Collections::Generic::IList<UnityEngine::RaycastHit>& enumerable)
			{
				return Plugin::SystemCollectionsGenericIListUnityEngineRaycastHitIterator(nullptr);
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
			IList<UnityEngine::GradientColorKey>::IList(decltype(nullptr))
				: System::Collections::IEnumerable(nullptr)
				, System::Collections::Generic::IEnumerable<UnityEngine::GradientColorKey>(nullptr)
				, System::Collections::Generic::ICollection<UnityEngine::GradientColorKey>(nullptr)
			{
			}
			
			IList<UnityEngine::GradientColorKey>::IList(Plugin::InternalUse iu, int32_t handle)
				: System::Collections::IEnumerable(nullptr)
				, System::Collections::Generic::IEnumerable<UnityEngine::GradientColorKey>(nullptr)
				, System::Collections::Generic::ICollection<UnityEngine::GradientColorKey>(nullptr)
			{
				Handle = handle;
				if (handle)
				{
					Plugin::ReferenceManagedClass(handle);
				}
			}
			
			IList<UnityEngine::GradientColorKey>::IList(const IList<UnityEngine::GradientColorKey>& other)
				: IList(Plugin::InternalUse::Only, other.Handle)
			{
			}
			
			IList<UnityEngine::GradientColorKey>::IList(IList<UnityEngine::GradientColorKey>&& other)
				: IList(Plugin::InternalUse::Only, other.Handle)
			{
				other.Handle = 0;
			}
			
			IList<UnityEngine::GradientColorKey>::~IList<UnityEngine::GradientColorKey>()
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
			}
			
			IList<UnityEngine::GradientColorKey>& IList<UnityEngine::GradientColorKey>::operator=(const IList<UnityEngine::GradientColorKey>& other)
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
			
			IList<UnityEngine::GradientColorKey>& IList<UnityEngine::GradientColorKey>::operator=(decltype(nullptr))
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
				return *this;
			}
			
			IList<UnityEngine::GradientColorKey>& IList<UnityEngine::GradientColorKey>::operator=(IList<UnityEngine::GradientColorKey>&& other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
				}
				Handle = other.Handle;
				other.Handle = 0;
				return *this;
			}
			
			bool IList<UnityEngine::GradientColorKey>::operator==(const IList<UnityEngine::GradientColorKey>& other) const
			{
				return Handle == other.Handle;
			}
			
			bool IList<UnityEngine::GradientColorKey>::operator!=(const IList<UnityEngine::GradientColorKey>& other) const
			{
				return Handle != other.Handle;
			}
		}
	}
}

namespace Plugin
{
	SystemCollectionsGenericIListUnityEngineGradientColorKeyIterator::SystemCollectionsGenericIListUnityEngineGradientColorKeyIterator(decltype(nullptr))
		: enumerator(nullptr)
		, hasMore(false)
	{
	}
	
	SystemCollectionsGenericIListUnityEngineGradientColorKeyIterator::SystemCollectionsGenericIListUnityEngineGradientColorKeyIterator(System::Collections::Generic::IList<UnityEngine::GradientColorKey>& enumerable)
		: enumerator(enumerable.GetEnumerator())
	{
		hasMore = enumerator.MoveNext();
	}
	
	SystemCollectionsGenericIListUnityEngineGradientColorKeyIterator::~SystemCollectionsGenericIListUnityEngineGradientColorKeyIterator()
	{
		if (enumerator != nullptr)
		{
			enumerator.Dispose();
		}
	}
	
	SystemCollectionsGenericIListUnityEngineGradientColorKeyIterator& SystemCollectionsGenericIListUnityEngineGradientColorKeyIterator::operator++()
	{
		hasMore = enumerator.MoveNext();
		return *this;
	}
	
	bool SystemCollectionsGenericIListUnityEngineGradientColorKeyIterator::operator!=(const SystemCollectionsGenericIListUnityEngineGradientColorKeyIterator& other)
	{
		return hasMore;
	}
	
	UnityEngine::GradientColorKey SystemCollectionsGenericIListUnityEngineGradientColorKeyIterator::operator*()
	{
		return enumerator.GetCurrent();
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			Plugin::SystemCollectionsGenericIListUnityEngineGradientColorKeyIterator begin(System::Collections::Generic::IList<UnityEngine::GradientColorKey>& enumerable)
			{
				return Plugin::SystemCollectionsGenericIListUnityEngineGradientColorKeyIterator(enumerable);
			}
			
			Plugin::SystemCollectionsGenericIListUnityEngineGradientColorKeyIterator end(System::Collections::Generic::IList<UnityEngine::GradientColorKey>& enumerable)
			{
				return Plugin::SystemCollectionsGenericIListUnityEngineGradientColorKeyIterator(nullptr);
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
			IList<UnityEngine::Resolution>::IList(decltype(nullptr))
				: System::Collections::IEnumerable(nullptr)
				, System::Collections::Generic::IEnumerable<UnityEngine::Resolution>(nullptr)
				, System::Collections::Generic::ICollection<UnityEngine::Resolution>(nullptr)
			{
			}
			
			IList<UnityEngine::Resolution>::IList(Plugin::InternalUse iu, int32_t handle)
				: System::Collections::IEnumerable(nullptr)
				, System::Collections::Generic::IEnumerable<UnityEngine::Resolution>(nullptr)
				, System::Collections::Generic::ICollection<UnityEngine::Resolution>(nullptr)
			{
				Handle = handle;
				if (handle)
				{
					Plugin::ReferenceManagedClass(handle);
				}
			}
			
			IList<UnityEngine::Resolution>::IList(const IList<UnityEngine::Resolution>& other)
				: IList(Plugin::InternalUse::Only, other.Handle)
			{
			}
			
			IList<UnityEngine::Resolution>::IList(IList<UnityEngine::Resolution>&& other)
				: IList(Plugin::InternalUse::Only, other.Handle)
			{
				other.Handle = 0;
			}
			
			IList<UnityEngine::Resolution>::~IList<UnityEngine::Resolution>()
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
			}
			
			IList<UnityEngine::Resolution>& IList<UnityEngine::Resolution>::operator=(const IList<UnityEngine::Resolution>& other)
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
			
			IList<UnityEngine::Resolution>& IList<UnityEngine::Resolution>::operator=(decltype(nullptr))
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
				return *this;
			}
			
			IList<UnityEngine::Resolution>& IList<UnityEngine::Resolution>::operator=(IList<UnityEngine::Resolution>&& other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
				}
				Handle = other.Handle;
				other.Handle = 0;
				return *this;
			}
			
			bool IList<UnityEngine::Resolution>::operator==(const IList<UnityEngine::Resolution>& other) const
			{
				return Handle == other.Handle;
			}
			
			bool IList<UnityEngine::Resolution>::operator!=(const IList<UnityEngine::Resolution>& other) const
			{
				return Handle != other.Handle;
			}
		}
	}
}

namespace Plugin
{
	SystemCollectionsGenericIListUnityEngineResolutionIterator::SystemCollectionsGenericIListUnityEngineResolutionIterator(decltype(nullptr))
		: enumerator(nullptr)
		, hasMore(false)
	{
	}
	
	SystemCollectionsGenericIListUnityEngineResolutionIterator::SystemCollectionsGenericIListUnityEngineResolutionIterator(System::Collections::Generic::IList<UnityEngine::Resolution>& enumerable)
		: enumerator(enumerable.GetEnumerator())
	{
		hasMore = enumerator.MoveNext();
	}
	
	SystemCollectionsGenericIListUnityEngineResolutionIterator::~SystemCollectionsGenericIListUnityEngineResolutionIterator()
	{
		if (enumerator != nullptr)
		{
			enumerator.Dispose();
		}
	}
	
	SystemCollectionsGenericIListUnityEngineResolutionIterator& SystemCollectionsGenericIListUnityEngineResolutionIterator::operator++()
	{
		hasMore = enumerator.MoveNext();
		return *this;
	}
	
	bool SystemCollectionsGenericIListUnityEngineResolutionIterator::operator!=(const SystemCollectionsGenericIListUnityEngineResolutionIterator& other)
	{
		return hasMore;
	}
	
	UnityEngine::Resolution SystemCollectionsGenericIListUnityEngineResolutionIterator::operator*()
	{
		return enumerator.GetCurrent();
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			Plugin::SystemCollectionsGenericIListUnityEngineResolutionIterator begin(System::Collections::Generic::IList<UnityEngine::Resolution>& enumerable)
			{
				return Plugin::SystemCollectionsGenericIListUnityEngineResolutionIterator(enumerable);
			}
			
			Plugin::SystemCollectionsGenericIListUnityEngineResolutionIterator end(System::Collections::Generic::IList<UnityEngine::Resolution>& enumerable)
			{
				return Plugin::SystemCollectionsGenericIListUnityEngineResolutionIterator(nullptr);
			}
		}
	}
}

namespace System
{
	namespace Runtime
	{
		namespace Serialization
		{
			ISerializable::ISerializable(decltype(nullptr))
			{
			}
			
			ISerializable::ISerializable(Plugin::InternalUse iu, int32_t handle)
			{
				Handle = handle;
				if (handle)
				{
					Plugin::ReferenceManagedClass(handle);
				}
			}
			
			ISerializable::ISerializable(const ISerializable& other)
				: ISerializable(Plugin::InternalUse::Only, other.Handle)
			{
			}
			
			ISerializable::ISerializable(ISerializable&& other)
				: ISerializable(Plugin::InternalUse::Only, other.Handle)
			{
				other.Handle = 0;
			}
			
			ISerializable::~ISerializable()
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
			}
			
			ISerializable& ISerializable::operator=(const ISerializable& other)
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
			
			ISerializable& ISerializable::operator=(decltype(nullptr))
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
				return *this;
			}
			
			ISerializable& ISerializable::operator=(ISerializable&& other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
				}
				Handle = other.Handle;
				other.Handle = 0;
				return *this;
			}
			
			bool ISerializable::operator==(const ISerializable& other) const
			{
				return Handle == other.Handle;
			}
			
			bool ISerializable::operator!=(const ISerializable& other) const
			{
				return Handle != other.Handle;
			}
		}
	}
}

namespace System
{
	namespace Runtime
	{
		namespace InteropServices
		{
			_Exception::_Exception(decltype(nullptr))
			{
			}
			
			_Exception::_Exception(Plugin::InternalUse iu, int32_t handle)
			{
				Handle = handle;
				if (handle)
				{
					Plugin::ReferenceManagedClass(handle);
				}
			}
			
			_Exception::_Exception(const _Exception& other)
				: _Exception(Plugin::InternalUse::Only, other.Handle)
			{
			}
			
			_Exception::_Exception(_Exception&& other)
				: _Exception(Plugin::InternalUse::Only, other.Handle)
			{
				other.Handle = 0;
			}
			
			_Exception::~_Exception()
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
			}
			
			_Exception& _Exception::operator=(const _Exception& other)
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
			
			_Exception& _Exception::operator=(decltype(nullptr))
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
				return *this;
			}
			
			_Exception& _Exception::operator=(_Exception&& other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
				}
				Handle = other.Handle;
				other.Handle = 0;
				return *this;
			}
			
			bool _Exception::operator==(const _Exception& other) const
			{
				return Handle == other.Handle;
			}
			
			bool _Exception::operator!=(const _Exception& other) const
			{
				return Handle != other.Handle;
			}
		}
	}
}

namespace System
{
	IAppDomainSetup::IAppDomainSetup(decltype(nullptr))
	{
	}
	
	IAppDomainSetup::IAppDomainSetup(Plugin::InternalUse iu, int32_t handle)
	{
		Handle = handle;
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
	}
	
	IAppDomainSetup::IAppDomainSetup(const IAppDomainSetup& other)
		: IAppDomainSetup(Plugin::InternalUse::Only, other.Handle)
	{
	}
	
	IAppDomainSetup::IAppDomainSetup(IAppDomainSetup&& other)
		: IAppDomainSetup(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
	}
	
	IAppDomainSetup::~IAppDomainSetup()
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
	}
	
	IAppDomainSetup& IAppDomainSetup::operator=(const IAppDomainSetup& other)
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
	
	IAppDomainSetup& IAppDomainSetup::operator=(decltype(nullptr))
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
		return *this;
	}
	
	IAppDomainSetup& IAppDomainSetup::operator=(IAppDomainSetup&& other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
		}
		Handle = other.Handle;
		other.Handle = 0;
		return *this;
	}
	
	bool IAppDomainSetup::operator==(const IAppDomainSetup& other) const
	{
		return Handle == other.Handle;
	}
	
	bool IAppDomainSetup::operator!=(const IAppDomainSetup& other) const
	{
		return Handle != other.Handle;
	}
}

namespace System
{
	namespace Collections
	{
		IComparer::IComparer(decltype(nullptr))
		{
		}
		
		IComparer::IComparer(Plugin::InternalUse iu, int32_t handle)
		{
			Handle = handle;
			if (handle)
			{
				Plugin::ReferenceManagedClass(handle);
			}
		}
		
		IComparer::IComparer(const IComparer& other)
			: IComparer(Plugin::InternalUse::Only, other.Handle)
		{
		}
		
		IComparer::IComparer(IComparer&& other)
			: IComparer(Plugin::InternalUse::Only, other.Handle)
		{
			other.Handle = 0;
		}
		
		IComparer::~IComparer()
		{
			if (Handle)
			{
				Plugin::DereferenceManagedClass(Handle);
				Handle = 0;
			}
		}
		
		IComparer& IComparer::operator=(const IComparer& other)
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
		
		IComparer& IComparer::operator=(decltype(nullptr))
		{
			if (Handle)
			{
				Plugin::DereferenceManagedClass(Handle);
				Handle = 0;
			}
			return *this;
		}
		
		IComparer& IComparer::operator=(IComparer&& other)
		{
			if (Handle)
			{
				Plugin::DereferenceManagedClass(Handle);
			}
			Handle = other.Handle;
			other.Handle = 0;
			return *this;
		}
		
		bool IComparer::operator==(const IComparer& other) const
		{
			return Handle == other.Handle;
		}
		
		bool IComparer::operator!=(const IComparer& other) const
		{
			return Handle != other.Handle;
		}
	}
}

namespace System
{
	namespace Collections
	{
		IEqualityComparer::IEqualityComparer(decltype(nullptr))
		{
		}
		
		IEqualityComparer::IEqualityComparer(Plugin::InternalUse iu, int32_t handle)
		{
			Handle = handle;
			if (handle)
			{
				Plugin::ReferenceManagedClass(handle);
			}
		}
		
		IEqualityComparer::IEqualityComparer(const IEqualityComparer& other)
			: IEqualityComparer(Plugin::InternalUse::Only, other.Handle)
		{
		}
		
		IEqualityComparer::IEqualityComparer(IEqualityComparer&& other)
			: IEqualityComparer(Plugin::InternalUse::Only, other.Handle)
		{
			other.Handle = 0;
		}
		
		IEqualityComparer::~IEqualityComparer()
		{
			if (Handle)
			{
				Plugin::DereferenceManagedClass(Handle);
				Handle = 0;
			}
		}
		
		IEqualityComparer& IEqualityComparer::operator=(const IEqualityComparer& other)
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
		
		IEqualityComparer& IEqualityComparer::operator=(decltype(nullptr))
		{
			if (Handle)
			{
				Plugin::DereferenceManagedClass(Handle);
				Handle = 0;
			}
			return *this;
		}
		
		IEqualityComparer& IEqualityComparer::operator=(IEqualityComparer&& other)
		{
			if (Handle)
			{
				Plugin::DereferenceManagedClass(Handle);
			}
			Handle = other.Handle;
			other.Handle = 0;
			return *this;
		}
		
		bool IEqualityComparer::operator==(const IEqualityComparer& other) const
		{
			return Handle == other.Handle;
		}
		
		bool IEqualityComparer::operator!=(const IEqualityComparer& other) const
		{
			return Handle != other.Handle;
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			IEqualityComparer<System::String>::IEqualityComparer(decltype(nullptr))
			{
			}
			
			IEqualityComparer<System::String>::IEqualityComparer(Plugin::InternalUse iu, int32_t handle)
			{
				Handle = handle;
				if (handle)
				{
					Plugin::ReferenceManagedClass(handle);
				}
			}
			
			IEqualityComparer<System::String>::IEqualityComparer(const IEqualityComparer<System::String>& other)
				: IEqualityComparer(Plugin::InternalUse::Only, other.Handle)
			{
			}
			
			IEqualityComparer<System::String>::IEqualityComparer(IEqualityComparer<System::String>&& other)
				: IEqualityComparer(Plugin::InternalUse::Only, other.Handle)
			{
				other.Handle = 0;
			}
			
			IEqualityComparer<System::String>::~IEqualityComparer<System::String>()
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
			}
			
			IEqualityComparer<System::String>& IEqualityComparer<System::String>::operator=(const IEqualityComparer<System::String>& other)
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
			
			IEqualityComparer<System::String>& IEqualityComparer<System::String>::operator=(decltype(nullptr))
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
				return *this;
			}
			
			IEqualityComparer<System::String>& IEqualityComparer<System::String>::operator=(IEqualityComparer<System::String>&& other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
				}
				Handle = other.Handle;
				other.Handle = 0;
				return *this;
			}
			
			bool IEqualityComparer<System::String>::operator==(const IEqualityComparer<System::String>& other) const
			{
				return Handle == other.Handle;
			}
			
			bool IEqualityComparer<System::String>::operator!=(const IEqualityComparer<System::String>& other) const
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
		namespace Generic
		{
			IEqualityComparer<int32_t>::IEqualityComparer(decltype(nullptr))
			{
			}
			
			IEqualityComparer<int32_t>::IEqualityComparer(Plugin::InternalUse iu, int32_t handle)
			{
				Handle = handle;
				if (handle)
				{
					Plugin::ReferenceManagedClass(handle);
				}
			}
			
			IEqualityComparer<int32_t>::IEqualityComparer(const IEqualityComparer<int32_t>& other)
				: IEqualityComparer(Plugin::InternalUse::Only, other.Handle)
			{
			}
			
			IEqualityComparer<int32_t>::IEqualityComparer(IEqualityComparer<int32_t>&& other)
				: IEqualityComparer(Plugin::InternalUse::Only, other.Handle)
			{
				other.Handle = 0;
			}
			
			IEqualityComparer<int32_t>::~IEqualityComparer<int32_t>()
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
			}
			
			IEqualityComparer<int32_t>& IEqualityComparer<int32_t>::operator=(const IEqualityComparer<int32_t>& other)
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
			
			IEqualityComparer<int32_t>& IEqualityComparer<int32_t>::operator=(decltype(nullptr))
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
				return *this;
			}
			
			IEqualityComparer<int32_t>& IEqualityComparer<int32_t>::operator=(IEqualityComparer<int32_t>&& other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
				}
				Handle = other.Handle;
				other.Handle = 0;
				return *this;
			}
			
			bool IEqualityComparer<int32_t>::operator==(const IEqualityComparer<int32_t>& other) const
			{
				return Handle == other.Handle;
			}
			
			bool IEqualityComparer<int32_t>::operator!=(const IEqualityComparer<int32_t>& other) const
			{
				return Handle != other.Handle;
			}
		}
	}
}

namespace UnityEngine
{
	namespace Playables
	{
		PlayableGraph::PlayableGraph(decltype(nullptr))
			: System::ValueType(nullptr)
		{
		}
		
		PlayableGraph::PlayableGraph(Plugin::InternalUse iu, int32_t handle)
			: System::ValueType(nullptr)
		{
			Handle = handle;
			if (handle)
			{
				Plugin::ReferenceManagedUnityEnginePlayablesPlayableGraph(Handle);
			}
		}
		
		PlayableGraph::PlayableGraph(const PlayableGraph& other)
			: PlayableGraph(Plugin::InternalUse::Only, other.Handle)
		{
		}
		
		PlayableGraph::PlayableGraph(PlayableGraph&& other)
			: PlayableGraph(Plugin::InternalUse::Only, other.Handle)
		{
			other.Handle = 0;
		}
		
		PlayableGraph::~PlayableGraph()
		{
			if (Handle)
			{
				Plugin::DereferenceManagedUnityEnginePlayablesPlayableGraph(Handle);
				Handle = 0;
			}
		}
		
		PlayableGraph& PlayableGraph::operator=(const PlayableGraph& other)
		{
			if (this->Handle)
			{
				Plugin::DereferenceManagedUnityEnginePlayablesPlayableGraph(Handle);
			}
			this->Handle = other.Handle;
			if (this->Handle)
			{
				Plugin::ReferenceManagedUnityEnginePlayablesPlayableGraph(Handle);
			}
			return *this;
		}
		
		PlayableGraph& PlayableGraph::operator=(decltype(nullptr))
		{
			if (Handle)
			{
				Plugin::DereferenceManagedUnityEnginePlayablesPlayableGraph(Handle);
				Handle = 0;
			}
			return *this;
		}
		
		PlayableGraph& PlayableGraph::operator=(PlayableGraph&& other)
		{
			if (Handle)
			{
				Plugin::DereferenceManagedUnityEnginePlayablesPlayableGraph(Handle);
			}
			Handle = other.Handle;
			other.Handle = 0;
			return *this;
		}
		
		bool PlayableGraph::operator==(const PlayableGraph& other) const
		{
			return Handle == other.Handle;
		}
		
		bool PlayableGraph::operator!=(const PlayableGraph& other) const
		{
			return Handle != other.Handle;
		}
	}
}

namespace System
{
	Object::Object(UnityEngine::Playables::PlayableGraph& val)
	{
		int32_t handle = Plugin::BoxPlayableGraph(val.Handle);
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
	
	Object::operator UnityEngine::Playables::PlayableGraph()
	{
		UnityEngine::Playables::PlayableGraph returnVal(Plugin::InternalUse::Only, Plugin::UnboxPlayableGraph(Handle));
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
	namespace Playables
	{
		IPlayable::IPlayable(decltype(nullptr))
		{
		}
		
		IPlayable::IPlayable(Plugin::InternalUse iu, int32_t handle)
		{
			Handle = handle;
			if (handle)
			{
				Plugin::ReferenceManagedClass(handle);
			}
		}
		
		IPlayable::IPlayable(const IPlayable& other)
			: IPlayable(Plugin::InternalUse::Only, other.Handle)
		{
		}
		
		IPlayable::IPlayable(IPlayable&& other)
			: IPlayable(Plugin::InternalUse::Only, other.Handle)
		{
			other.Handle = 0;
		}
		
		IPlayable::~IPlayable()
		{
			if (Handle)
			{
				Plugin::DereferenceManagedClass(Handle);
				Handle = 0;
			}
		}
		
		IPlayable& IPlayable::operator=(const IPlayable& other)
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
		
		IPlayable& IPlayable::operator=(decltype(nullptr))
		{
			if (Handle)
			{
				Plugin::DereferenceManagedClass(Handle);
				Handle = 0;
			}
			return *this;
		}
		
		IPlayable& IPlayable::operator=(IPlayable&& other)
		{
			if (Handle)
			{
				Plugin::DereferenceManagedClass(Handle);
			}
			Handle = other.Handle;
			other.Handle = 0;
			return *this;
		}
		
		bool IPlayable::operator==(const IPlayable& other) const
		{
			return Handle == other.Handle;
		}
		
		bool IPlayable::operator!=(const IPlayable& other) const
		{
			return Handle != other.Handle;
		}
	}
}

namespace System
{
	IEquatable<UnityEngine::Animations::AnimationMixerPlayable>::IEquatable(decltype(nullptr))
	{
	}
	
	IEquatable<UnityEngine::Animations::AnimationMixerPlayable>::IEquatable(Plugin::InternalUse iu, int32_t handle)
	{
		Handle = handle;
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
	}
	
	IEquatable<UnityEngine::Animations::AnimationMixerPlayable>::IEquatable(const IEquatable<UnityEngine::Animations::AnimationMixerPlayable>& other)
		: IEquatable(Plugin::InternalUse::Only, other.Handle)
	{
	}
	
	IEquatable<UnityEngine::Animations::AnimationMixerPlayable>::IEquatable(IEquatable<UnityEngine::Animations::AnimationMixerPlayable>&& other)
		: IEquatable(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
	}
	
	IEquatable<UnityEngine::Animations::AnimationMixerPlayable>::~IEquatable<UnityEngine::Animations::AnimationMixerPlayable>()
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
	}
	
	IEquatable<UnityEngine::Animations::AnimationMixerPlayable>& IEquatable<UnityEngine::Animations::AnimationMixerPlayable>::operator=(const IEquatable<UnityEngine::Animations::AnimationMixerPlayable>& other)
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
	
	IEquatable<UnityEngine::Animations::AnimationMixerPlayable>& IEquatable<UnityEngine::Animations::AnimationMixerPlayable>::operator=(decltype(nullptr))
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
		return *this;
	}
	
	IEquatable<UnityEngine::Animations::AnimationMixerPlayable>& IEquatable<UnityEngine::Animations::AnimationMixerPlayable>::operator=(IEquatable<UnityEngine::Animations::AnimationMixerPlayable>&& other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
		}
		Handle = other.Handle;
		other.Handle = 0;
		return *this;
	}
	
	bool IEquatable<UnityEngine::Animations::AnimationMixerPlayable>::operator==(const IEquatable<UnityEngine::Animations::AnimationMixerPlayable>& other) const
	{
		return Handle == other.Handle;
	}
	
	bool IEquatable<UnityEngine::Animations::AnimationMixerPlayable>::operator!=(const IEquatable<UnityEngine::Animations::AnimationMixerPlayable>& other) const
	{
		return Handle != other.Handle;
	}
}

namespace UnityEngine
{
	namespace Animations
	{
		AnimationMixerPlayable::AnimationMixerPlayable(decltype(nullptr))
			: System::ValueType(nullptr)
			, System::IEquatable<UnityEngine::Animations::AnimationMixerPlayable>(nullptr)
			, UnityEngine::Playables::IPlayable(nullptr)
		{
		}
		
		AnimationMixerPlayable::AnimationMixerPlayable(Plugin::InternalUse iu, int32_t handle)
			: System::ValueType(nullptr)
			, System::IEquatable<UnityEngine::Animations::AnimationMixerPlayable>(nullptr)
			, UnityEngine::Playables::IPlayable(nullptr)
		{
			Handle = handle;
			if (handle)
			{
				Plugin::ReferenceManagedUnityEngineAnimationsAnimationMixerPlayable(Handle);
			}
		}
		
		AnimationMixerPlayable::AnimationMixerPlayable(const AnimationMixerPlayable& other)
			: AnimationMixerPlayable(Plugin::InternalUse::Only, other.Handle)
		{
		}
		
		AnimationMixerPlayable::AnimationMixerPlayable(AnimationMixerPlayable&& other)
			: AnimationMixerPlayable(Plugin::InternalUse::Only, other.Handle)
		{
			other.Handle = 0;
		}
		
		AnimationMixerPlayable::~AnimationMixerPlayable()
		{
			if (Handle)
			{
				Plugin::DereferenceManagedUnityEngineAnimationsAnimationMixerPlayable(Handle);
				Handle = 0;
			}
		}
		
		AnimationMixerPlayable& AnimationMixerPlayable::operator=(const AnimationMixerPlayable& other)
		{
			if (this->Handle)
			{
				Plugin::DereferenceManagedUnityEngineAnimationsAnimationMixerPlayable(Handle);
			}
			this->Handle = other.Handle;
			if (this->Handle)
			{
				Plugin::ReferenceManagedUnityEngineAnimationsAnimationMixerPlayable(Handle);
			}
			return *this;
		}
		
		AnimationMixerPlayable& AnimationMixerPlayable::operator=(decltype(nullptr))
		{
			if (Handle)
			{
				Plugin::DereferenceManagedUnityEngineAnimationsAnimationMixerPlayable(Handle);
				Handle = 0;
			}
			return *this;
		}
		
		AnimationMixerPlayable& AnimationMixerPlayable::operator=(AnimationMixerPlayable&& other)
		{
			if (Handle)
			{
				Plugin::DereferenceManagedUnityEngineAnimationsAnimationMixerPlayable(Handle);
			}
			Handle = other.Handle;
			other.Handle = 0;
			return *this;
		}
		
		bool AnimationMixerPlayable::operator==(const AnimationMixerPlayable& other) const
		{
			return Handle == other.Handle;
		}
		
		bool AnimationMixerPlayable::operator!=(const AnimationMixerPlayable& other) const
		{
			return Handle != other.Handle;
		}
		
		UnityEngine::Animations::AnimationMixerPlayable AnimationMixerPlayable::Create(UnityEngine::Playables::PlayableGraph& graph, int32_t inputCount, System::Boolean normalizeWeights)
		{
			auto returnValue = Plugin::UnityEngineAnimationsAnimationMixerPlayableMethodCreateUnityEnginePlayablesPlayableGraph_SystemInt32_SystemBoolean(graph.Handle, inputCount, normalizeWeights);
			if (Plugin::unhandledCsharpException)
			{
				System::Exception* ex = Plugin::unhandledCsharpException;
				Plugin::unhandledCsharpException = nullptr;
				ex->ThrowReferenceToThis();
				delete ex;
			}
			return UnityEngine::Animations::AnimationMixerPlayable(Plugin::InternalUse::Only, returnValue);
		}
	}
}

namespace System
{
	Object::Object(UnityEngine::Animations::AnimationMixerPlayable& val)
	{
		int32_t handle = Plugin::BoxAnimationMixerPlayable(val.Handle);
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
	
	Object::operator UnityEngine::Animations::AnimationMixerPlayable()
	{
		UnityEngine::Animations::AnimationMixerPlayable returnVal(Plugin::InternalUse::Only, Plugin::UnboxAnimationMixerPlayable(Handle));
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
	namespace Runtime
	{
		namespace CompilerServices
		{
			IStrongBox::IStrongBox(decltype(nullptr))
			{
			}
			
			IStrongBox::IStrongBox(Plugin::InternalUse iu, int32_t handle)
			{
				Handle = handle;
				if (handle)
				{
					Plugin::ReferenceManagedClass(handle);
				}
			}
			
			IStrongBox::IStrongBox(const IStrongBox& other)
				: IStrongBox(Plugin::InternalUse::Only, other.Handle)
			{
			}
			
			IStrongBox::IStrongBox(IStrongBox&& other)
				: IStrongBox(Plugin::InternalUse::Only, other.Handle)
			{
				other.Handle = 0;
			}
			
			IStrongBox::~IStrongBox()
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
			}
			
			IStrongBox& IStrongBox::operator=(const IStrongBox& other)
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
			
			IStrongBox& IStrongBox::operator=(decltype(nullptr))
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
				return *this;
			}
			
			IStrongBox& IStrongBox::operator=(IStrongBox&& other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
				}
				Handle = other.Handle;
				other.Handle = 0;
				return *this;
			}
			
			bool IStrongBox::operator==(const IStrongBox& other) const
			{
				return Handle == other.Handle;
			}
			
			bool IStrongBox::operator!=(const IStrongBox& other) const
			{
				return Handle != other.Handle;
			}
		}
	}
}

namespace UnityEngine
{
	namespace Experimental
	{
		namespace UIElements
		{
			IEventHandler::IEventHandler(decltype(nullptr))
			{
			}
			
			IEventHandler::IEventHandler(Plugin::InternalUse iu, int32_t handle)
			{
				Handle = handle;
				if (handle)
				{
					Plugin::ReferenceManagedClass(handle);
				}
			}
			
			IEventHandler::IEventHandler(const IEventHandler& other)
				: IEventHandler(Plugin::InternalUse::Only, other.Handle)
			{
			}
			
			IEventHandler::IEventHandler(IEventHandler&& other)
				: IEventHandler(Plugin::InternalUse::Only, other.Handle)
			{
				other.Handle = 0;
			}
			
			IEventHandler::~IEventHandler()
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
			}
			
			IEventHandler& IEventHandler::operator=(const IEventHandler& other)
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
			
			IEventHandler& IEventHandler::operator=(decltype(nullptr))
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
				return *this;
			}
			
			IEventHandler& IEventHandler::operator=(IEventHandler&& other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
				}
				Handle = other.Handle;
				other.Handle = 0;
				return *this;
			}
			
			bool IEventHandler::operator==(const IEventHandler& other) const
			{
				return Handle == other.Handle;
			}
			
			bool IEventHandler::operator!=(const IEventHandler& other) const
			{
				return Handle != other.Handle;
			}
		}
	}
}

namespace UnityEngine
{
	namespace Experimental
	{
		namespace UIElements
		{
			IStyle::IStyle(decltype(nullptr))
			{
			}
			
			IStyle::IStyle(Plugin::InternalUse iu, int32_t handle)
			{
				Handle = handle;
				if (handle)
				{
					Plugin::ReferenceManagedClass(handle);
				}
			}
			
			IStyle::IStyle(const IStyle& other)
				: IStyle(Plugin::InternalUse::Only, other.Handle)
			{
			}
			
			IStyle::IStyle(IStyle&& other)
				: IStyle(Plugin::InternalUse::Only, other.Handle)
			{
				other.Handle = 0;
			}
			
			IStyle::~IStyle()
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
			}
			
			IStyle& IStyle::operator=(const IStyle& other)
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
			
			IStyle& IStyle::operator=(decltype(nullptr))
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
				return *this;
			}
			
			IStyle& IStyle::operator=(IStyle&& other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
				}
				Handle = other.Handle;
				other.Handle = 0;
				return *this;
			}
			
			bool IStyle::operator==(const IStyle& other) const
			{
				return Handle == other.Handle;
			}
			
			bool IStyle::operator!=(const IStyle& other) const
			{
				return Handle != other.Handle;
			}
		}
	}
}

namespace System
{
	namespace Diagnostics
	{
		Stopwatch::Stopwatch(decltype(nullptr))
		{
		}
		
		Stopwatch::Stopwatch(Plugin::InternalUse iu, int32_t handle)
		{
			Handle = handle;
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
		
		Stopwatch& Stopwatch::operator=(decltype(nullptr))
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
	GameObject::GameObject(decltype(nullptr))
		: UnityEngine::Object(nullptr)
	{
	}
	
	GameObject::GameObject(Plugin::InternalUse iu, int32_t handle)
		: UnityEngine::Object(nullptr)
	{
		Handle = handle;
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
	
	GameObject& GameObject::operator=(decltype(nullptr))
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
	
	GameObject::GameObject(System::String& name)
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
	
	template<> MyGame::MonoBehaviours::AnotherScript GameObject::AddComponent<MyGame::MonoBehaviours::AnotherScript>()
	{
		auto returnValue = Plugin::UnityEngineGameObjectMethodAddComponentMyGameMonoBehavioursAnotherScript(Handle);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return MyGame::MonoBehaviours::AnotherScript(Plugin::InternalUse::Only, returnValue);
	}
	
	UnityEngine::GameObject GameObject::CreatePrimitive(UnityEngine::PrimitiveType type)
	{
		auto returnValue = Plugin::UnityEngineGameObjectMethodCreatePrimitiveUnityEnginePrimitiveType(type);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return UnityEngine::GameObject(Plugin::InternalUse::Only, returnValue);
	}
}

namespace UnityEngine
{
	Debug::Debug(decltype(nullptr))
	{
	}
	
	Debug::Debug(Plugin::InternalUse iu, int32_t handle)
	{
		Handle = handle;
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
	
	Debug& Debug::operator=(decltype(nullptr))
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
	
	void Debug::Log(System::Object& message)
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
		
		template<> void Assert::AreEqual<System::String>(System::String& expected, System::String& actual)
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
	
		template<> void Assert::AreEqual<UnityEngine::GameObject>(UnityEngine::GameObject& expected, UnityEngine::GameObject& actual)
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
	Collision::Collision(decltype(nullptr))
	{
	}
	
	Collision::Collision(Plugin::InternalUse iu, int32_t handle)
	{
		Handle = handle;
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
	
	Collision& Collision::operator=(decltype(nullptr))
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
	Behaviour::Behaviour(decltype(nullptr))
		: UnityEngine::Object(nullptr)
		, UnityEngine::Component(nullptr)
	{
	}
	
	Behaviour::Behaviour(Plugin::InternalUse iu, int32_t handle)
		: UnityEngine::Object(nullptr)
		, UnityEngine::Component(nullptr)
	{
		Handle = handle;
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
	
	Behaviour& Behaviour::operator=(decltype(nullptr))
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
	MonoBehaviour::MonoBehaviour(decltype(nullptr))
		: UnityEngine::Object(nullptr)
		, UnityEngine::Component(nullptr)
		, UnityEngine::Behaviour(nullptr)
	{
	}
	
	MonoBehaviour::MonoBehaviour(Plugin::InternalUse iu, int32_t handle)
		: UnityEngine::Object(nullptr)
		, UnityEngine::Component(nullptr)
		, UnityEngine::Behaviour(nullptr)
	{
		Handle = handle;
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
	
	MonoBehaviour& MonoBehaviour::operator=(decltype(nullptr))
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
	
	UnityEngine::Transform MonoBehaviour::GetTransform()
	{
		auto returnValue = Plugin::UnityEngineMonoBehaviourPropertyGetTransform(Handle);
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
	AudioSettings::AudioSettings(decltype(nullptr))
	{
	}
	
	AudioSettings::AudioSettings(Plugin::InternalUse iu, int32_t handle)
	{
		Handle = handle;
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
	
	AudioSettings& AudioSettings::operator=(decltype(nullptr))
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
		NetworkTransport::NetworkTransport(decltype(nullptr))
		{
		}
		
		NetworkTransport::NetworkTransport(Plugin::InternalUse iu, int32_t handle)
		{
			Handle = handle;
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
		
		NetworkTransport& NetworkTransport::operator=(decltype(nullptr))
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
	Quaternion::Quaternion()
	{
	}
}

namespace System
{
	Object::Object(UnityEngine::Quaternion& val)
	{
		int32_t handle = Plugin::BoxQuaternion(val);
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
	
	Object::operator UnityEngine::Quaternion()
	{
		UnityEngine::Quaternion returnVal(Plugin::UnboxQuaternion(Handle));
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
			KeyValuePair<System::String, double>::KeyValuePair(decltype(nullptr))
				: System::ValueType(nullptr)
			{
			}
			
			KeyValuePair<System::String, double>::KeyValuePair(Plugin::InternalUse iu, int32_t handle)
				: System::ValueType(nullptr)
			{
				Handle = handle;
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
			
			KeyValuePair<System::String, double>& KeyValuePair<System::String, double>::operator=(decltype(nullptr))
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
			
			KeyValuePair<System::String, double>::KeyValuePair(System::String& key, double value)
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
	Object::Object(System::Collections::Generic::KeyValuePair<System::String, double>& val)
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
			List<System::String>::List(decltype(nullptr))
				: System::Collections::IEnumerable(nullptr)
				, System::Collections::ICollection(nullptr)
				, System::Collections::IList(nullptr)
				, System::Collections::Generic::IEnumerable<System::String>(nullptr)
				, System::Collections::Generic::ICollection<System::String>(nullptr)
				, System::Collections::Generic::IList<System::String>(nullptr)
			{
			}
			
			List<System::String>::List(Plugin::InternalUse iu, int32_t handle)
				: System::Collections::IEnumerable(nullptr)
				, System::Collections::ICollection(nullptr)
				, System::Collections::IList(nullptr)
				, System::Collections::Generic::IEnumerable<System::String>(nullptr)
				, System::Collections::Generic::ICollection<System::String>(nullptr)
				, System::Collections::Generic::IList<System::String>(nullptr)
			{
				Handle = handle;
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
			
			List<System::String>& List<System::String>::operator=(decltype(nullptr))
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
				: System::Collections::IEnumerable(nullptr)
				, System::Collections::ICollection(nullptr)
				, System::Collections::IList(nullptr)
				, System::Collections::Generic::IEnumerable<System::String>(nullptr)
				, System::Collections::Generic::ICollection<System::String>(nullptr)
				, System::Collections::Generic::IList<System::String>(nullptr)
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
			
			void List<System::String>::SetItem(int32_t index, System::String& value)
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
			
			void List<System::String>::Add(System::String& item)
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
	
			void List<System::String>::Sort(System::Collections::Generic::IComparer<System::String>& comparer)
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

namespace Plugin
{
	SystemCollectionsGenericListSystemStringIterator::SystemCollectionsGenericListSystemStringIterator(decltype(nullptr))
		: enumerator(nullptr)
		, hasMore(false)
	{
	}
	
	SystemCollectionsGenericListSystemStringIterator::SystemCollectionsGenericListSystemStringIterator(System::Collections::Generic::List<System::String>& enumerable)
		: enumerator(enumerable.GetEnumerator())
	{
		hasMore = enumerator.MoveNext();
	}
	
	SystemCollectionsGenericListSystemStringIterator::~SystemCollectionsGenericListSystemStringIterator()
	{
		if (enumerator != nullptr)
		{
			enumerator.Dispose();
		}
	}
	
	SystemCollectionsGenericListSystemStringIterator& SystemCollectionsGenericListSystemStringIterator::operator++()
	{
		hasMore = enumerator.MoveNext();
		return *this;
	}
	
	bool SystemCollectionsGenericListSystemStringIterator::operator!=(const SystemCollectionsGenericListSystemStringIterator& other)
	{
		return hasMore;
	}
	
	System::String SystemCollectionsGenericListSystemStringIterator::operator*()
	{
		return enumerator.GetCurrent();
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			Plugin::SystemCollectionsGenericListSystemStringIterator begin(System::Collections::Generic::List<System::String>& enumerable)
			{
				return Plugin::SystemCollectionsGenericListSystemStringIterator(enumerable);
			}
			
			Plugin::SystemCollectionsGenericListSystemStringIterator end(System::Collections::Generic::List<System::String>& enumerable)
			{
				return Plugin::SystemCollectionsGenericListSystemStringIterator(nullptr);
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
			List<int32_t>::List(decltype(nullptr))
				: System::Collections::IEnumerable(nullptr)
				, System::Collections::ICollection(nullptr)
				, System::Collections::IList(nullptr)
				, System::Collections::Generic::IEnumerable<int32_t>(nullptr)
				, System::Collections::Generic::ICollection<int32_t>(nullptr)
				, System::Collections::Generic::IList<int32_t>(nullptr)
			{
			}
			
			List<int32_t>::List(Plugin::InternalUse iu, int32_t handle)
				: System::Collections::IEnumerable(nullptr)
				, System::Collections::ICollection(nullptr)
				, System::Collections::IList(nullptr)
				, System::Collections::Generic::IEnumerable<int32_t>(nullptr)
				, System::Collections::Generic::ICollection<int32_t>(nullptr)
				, System::Collections::Generic::IList<int32_t>(nullptr)
			{
				Handle = handle;
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
			
			List<int32_t>& List<int32_t>::operator=(decltype(nullptr))
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
				: System::Collections::IEnumerable(nullptr)
				, System::Collections::ICollection(nullptr)
				, System::Collections::IList(nullptr)
				, System::Collections::Generic::IEnumerable<int32_t>(nullptr)
				, System::Collections::Generic::ICollection<int32_t>(nullptr)
				, System::Collections::Generic::IList<int32_t>(nullptr)
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
	
			void List<int32_t>::Sort(System::Collections::Generic::IComparer<int32_t>& comparer)
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

namespace Plugin
{
	SystemCollectionsGenericListSystemInt32Iterator::SystemCollectionsGenericListSystemInt32Iterator(decltype(nullptr))
		: enumerator(nullptr)
		, hasMore(false)
	{
	}
	
	SystemCollectionsGenericListSystemInt32Iterator::SystemCollectionsGenericListSystemInt32Iterator(System::Collections::Generic::List<int32_t>& enumerable)
		: enumerator(enumerable.GetEnumerator())
	{
		hasMore = enumerator.MoveNext();
	}
	
	SystemCollectionsGenericListSystemInt32Iterator::~SystemCollectionsGenericListSystemInt32Iterator()
	{
		if (enumerator != nullptr)
		{
			enumerator.Dispose();
		}
	}
	
	SystemCollectionsGenericListSystemInt32Iterator& SystemCollectionsGenericListSystemInt32Iterator::operator++()
	{
		hasMore = enumerator.MoveNext();
		return *this;
	}
	
	bool SystemCollectionsGenericListSystemInt32Iterator::operator!=(const SystemCollectionsGenericListSystemInt32Iterator& other)
	{
		return hasMore;
	}
	
	int32_t SystemCollectionsGenericListSystemInt32Iterator::operator*()
	{
		return enumerator.GetCurrent();
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			Plugin::SystemCollectionsGenericListSystemInt32Iterator begin(System::Collections::Generic::List<int32_t>& enumerable)
			{
				return Plugin::SystemCollectionsGenericListSystemInt32Iterator(enumerable);
			}
			
			Plugin::SystemCollectionsGenericListSystemInt32Iterator end(System::Collections::Generic::List<int32_t>& enumerable)
			{
				return Plugin::SystemCollectionsGenericListSystemInt32Iterator(nullptr);
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
			LinkedListNode<System::String>::LinkedListNode(decltype(nullptr))
			{
			}
			
			LinkedListNode<System::String>::LinkedListNode(Plugin::InternalUse iu, int32_t handle)
			{
				Handle = handle;
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
			
			LinkedListNode<System::String>& LinkedListNode<System::String>::operator=(decltype(nullptr))
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
			
			LinkedListNode<System::String>::LinkedListNode(System::String& value)
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
			
			void LinkedListNode<System::String>::SetValue(System::String& value)
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
			StrongBox<System::String>::StrongBox(decltype(nullptr))
				: System::Runtime::CompilerServices::IStrongBox(nullptr)
			{
			}
			
			StrongBox<System::String>::StrongBox(Plugin::InternalUse iu, int32_t handle)
				: System::Runtime::CompilerServices::IStrongBox(nullptr)
			{
				Handle = handle;
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
			
			StrongBox<System::String>& StrongBox<System::String>::operator=(decltype(nullptr))
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
			
			StrongBox<System::String>::StrongBox(System::String& value)
				: System::Runtime::CompilerServices::IStrongBox(nullptr)
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
			
			void StrongBox<System::String>::SetValue(System::String& value)
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
			Collection<int32_t>::Collection(decltype(nullptr))
				: System::Collections::IEnumerable(nullptr)
				, System::Collections::ICollection(nullptr)
				, System::Collections::IList(nullptr)
				, System::Collections::Generic::IEnumerable<int32_t>(nullptr)
				, System::Collections::Generic::ICollection<int32_t>(nullptr)
				, System::Collections::Generic::IList<int32_t>(nullptr)
			{
			}
			
			Collection<int32_t>::Collection(Plugin::InternalUse iu, int32_t handle)
				: System::Collections::IEnumerable(nullptr)
				, System::Collections::ICollection(nullptr)
				, System::Collections::IList(nullptr)
				, System::Collections::Generic::IEnumerable<int32_t>(nullptr)
				, System::Collections::Generic::ICollection<int32_t>(nullptr)
				, System::Collections::Generic::IList<int32_t>(nullptr)
			{
				Handle = handle;
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
			
			Collection<int32_t>& Collection<int32_t>::operator=(decltype(nullptr))
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

namespace Plugin
{
	SystemCollectionsObjectModelCollectionSystemInt32Iterator::SystemCollectionsObjectModelCollectionSystemInt32Iterator(decltype(nullptr))
		: enumerator(nullptr)
		, hasMore(false)
	{
	}
	
	SystemCollectionsObjectModelCollectionSystemInt32Iterator::SystemCollectionsObjectModelCollectionSystemInt32Iterator(System::Collections::ObjectModel::Collection<int32_t>& enumerable)
		: enumerator(enumerable.GetEnumerator())
	{
		hasMore = enumerator.MoveNext();
	}
	
	SystemCollectionsObjectModelCollectionSystemInt32Iterator::~SystemCollectionsObjectModelCollectionSystemInt32Iterator()
	{
		if (enumerator != nullptr)
		{
			enumerator.Dispose();
		}
	}
	
	SystemCollectionsObjectModelCollectionSystemInt32Iterator& SystemCollectionsObjectModelCollectionSystemInt32Iterator::operator++()
	{
		hasMore = enumerator.MoveNext();
		return *this;
	}
	
	bool SystemCollectionsObjectModelCollectionSystemInt32Iterator::operator!=(const SystemCollectionsObjectModelCollectionSystemInt32Iterator& other)
	{
		return hasMore;
	}
	
	int32_t SystemCollectionsObjectModelCollectionSystemInt32Iterator::operator*()
	{
		return enumerator.GetCurrent();
	}
}

namespace System
{
	namespace Collections
	{
		namespace ObjectModel
		{
			Plugin::SystemCollectionsObjectModelCollectionSystemInt32Iterator begin(System::Collections::ObjectModel::Collection<int32_t>& enumerable)
			{
				return Plugin::SystemCollectionsObjectModelCollectionSystemInt32Iterator(enumerable);
			}
			
			Plugin::SystemCollectionsObjectModelCollectionSystemInt32Iterator end(System::Collections::ObjectModel::Collection<int32_t>& enumerable)
			{
				return Plugin::SystemCollectionsObjectModelCollectionSystemInt32Iterator(nullptr);
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
			KeyedCollection<System::String, int32_t>::KeyedCollection(decltype(nullptr))
				: System::Collections::IEnumerable(nullptr)
				, System::Collections::ICollection(nullptr)
				, System::Collections::IList(nullptr)
				, System::Collections::Generic::IEnumerable<int32_t>(nullptr)
				, System::Collections::Generic::ICollection<int32_t>(nullptr)
				, System::Collections::Generic::IList<int32_t>(nullptr)
				, System::Collections::ObjectModel::Collection<int32_t>(nullptr)
			{
			}
			
			KeyedCollection<System::String, int32_t>::KeyedCollection(Plugin::InternalUse iu, int32_t handle)
				: System::Collections::IEnumerable(nullptr)
				, System::Collections::ICollection(nullptr)
				, System::Collections::IList(nullptr)
				, System::Collections::Generic::IEnumerable<int32_t>(nullptr)
				, System::Collections::Generic::ICollection<int32_t>(nullptr)
				, System::Collections::Generic::IList<int32_t>(nullptr)
				, System::Collections::ObjectModel::Collection<int32_t>(nullptr)
			{
				Handle = handle;
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
			
			KeyedCollection<System::String, int32_t>& KeyedCollection<System::String, int32_t>::operator=(decltype(nullptr))
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

namespace Plugin
{
	SystemCollectionsObjectModelKeyedCollectionSystemString_SystemInt32Iterator::SystemCollectionsObjectModelKeyedCollectionSystemString_SystemInt32Iterator(decltype(nullptr))
		: enumerator(nullptr)
		, hasMore(false)
	{
	}
	
	SystemCollectionsObjectModelKeyedCollectionSystemString_SystemInt32Iterator::SystemCollectionsObjectModelKeyedCollectionSystemString_SystemInt32Iterator(System::Collections::ObjectModel::KeyedCollection<System::String, int32_t>& enumerable)
		: enumerator(enumerable.GetEnumerator())
	{
		hasMore = enumerator.MoveNext();
	}
	
	SystemCollectionsObjectModelKeyedCollectionSystemString_SystemInt32Iterator::~SystemCollectionsObjectModelKeyedCollectionSystemString_SystemInt32Iterator()
	{
		if (enumerator != nullptr)
		{
			enumerator.Dispose();
		}
	}
	
	SystemCollectionsObjectModelKeyedCollectionSystemString_SystemInt32Iterator& SystemCollectionsObjectModelKeyedCollectionSystemString_SystemInt32Iterator::operator++()
	{
		hasMore = enumerator.MoveNext();
		return *this;
	}
	
	bool SystemCollectionsObjectModelKeyedCollectionSystemString_SystemInt32Iterator::operator!=(const SystemCollectionsObjectModelKeyedCollectionSystemString_SystemInt32Iterator& other)
	{
		return hasMore;
	}
	
	int32_t SystemCollectionsObjectModelKeyedCollectionSystemString_SystemInt32Iterator::operator*()
	{
		return enumerator.GetCurrent();
	}
}

namespace System
{
	namespace Collections
	{
		namespace ObjectModel
		{
			Plugin::SystemCollectionsObjectModelKeyedCollectionSystemString_SystemInt32Iterator begin(System::Collections::ObjectModel::KeyedCollection<System::String, int32_t>& enumerable)
			{
				return Plugin::SystemCollectionsObjectModelKeyedCollectionSystemString_SystemInt32Iterator(enumerable);
			}
			
			Plugin::SystemCollectionsObjectModelKeyedCollectionSystemString_SystemInt32Iterator end(System::Collections::ObjectModel::KeyedCollection<System::String, int32_t>& enumerable)
			{
				return Plugin::SystemCollectionsObjectModelKeyedCollectionSystemString_SystemInt32Iterator(nullptr);
			}
		}
	}
}

namespace System
{
	Exception::Exception(decltype(nullptr))
		: System::Runtime::InteropServices::_Exception(nullptr)
		, System::Runtime::Serialization::ISerializable(nullptr)
	{
	}
	
	Exception::Exception(Plugin::InternalUse iu, int32_t handle)
		: System::Runtime::InteropServices::_Exception(nullptr)
		, System::Runtime::Serialization::ISerializable(nullptr)
	{
		Handle = handle;
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
	
	Exception& Exception::operator=(decltype(nullptr))
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
	
	Exception::Exception(System::String& message)
		: System::Runtime::InteropServices::_Exception(nullptr)
		, System::Runtime::Serialization::ISerializable(nullptr)
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
	SystemException::SystemException(decltype(nullptr))
		: System::Runtime::InteropServices::_Exception(nullptr)
		, System::Runtime::Serialization::ISerializable(nullptr)
		, System::Exception(nullptr)
	{
	}
	
	SystemException::SystemException(Plugin::InternalUse iu, int32_t handle)
		: System::Runtime::InteropServices::_Exception(nullptr)
		, System::Runtime::Serialization::ISerializable(nullptr)
		, System::Exception(nullptr)
	{
		Handle = handle;
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
	
	SystemException& SystemException::operator=(decltype(nullptr))
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
	NullReferenceException::NullReferenceException(decltype(nullptr))
		: System::Runtime::InteropServices::_Exception(nullptr)
		, System::Runtime::Serialization::ISerializable(nullptr)
		, System::Exception(nullptr)
		, System::SystemException(nullptr)
	{
	}
	
	NullReferenceException::NullReferenceException(Plugin::InternalUse iu, int32_t handle)
		: System::Runtime::InteropServices::_Exception(nullptr)
		, System::Runtime::Serialization::ISerializable(nullptr)
		, System::Exception(nullptr)
		, System::SystemException(nullptr)
	{
		Handle = handle;
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
	
	NullReferenceException& NullReferenceException::operator=(decltype(nullptr))
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
	Screen::Screen(decltype(nullptr))
	{
	}
	
	Screen::Screen(Plugin::InternalUse iu, int32_t handle)
	{
		Handle = handle;
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
	
	Screen& Screen::operator=(decltype(nullptr))
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
	Ray::Ray(decltype(nullptr))
		: System::ValueType(nullptr)
	{
	}
	
	Ray::Ray(Plugin::InternalUse iu, int32_t handle)
		: System::ValueType(nullptr)
	{
		Handle = handle;
		if (handle)
		{
			Plugin::ReferenceManagedUnityEngineRay(Handle);
		}
	}
	
	Ray::Ray(const Ray& other)
		: Ray(Plugin::InternalUse::Only, other.Handle)
	{
	}
	
	Ray::Ray(Ray&& other)
		: Ray(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
	}
	
	Ray::~Ray()
	{
		if (Handle)
		{
			Plugin::DereferenceManagedUnityEngineRay(Handle);
			Handle = 0;
		}
	}
	
	Ray& Ray::operator=(const Ray& other)
	{
		if (this->Handle)
		{
			Plugin::DereferenceManagedUnityEngineRay(Handle);
		}
		this->Handle = other.Handle;
		if (this->Handle)
		{
			Plugin::ReferenceManagedUnityEngineRay(Handle);
		}
		return *this;
	}
	
	Ray& Ray::operator=(decltype(nullptr))
	{
		if (Handle)
		{
			Plugin::DereferenceManagedUnityEngineRay(Handle);
			Handle = 0;
		}
		return *this;
	}
	
	Ray& Ray::operator=(Ray&& other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedUnityEngineRay(Handle);
		}
		Handle = other.Handle;
		other.Handle = 0;
		return *this;
	}
	
	bool Ray::operator==(const Ray& other) const
	{
		return Handle == other.Handle;
	}
	
	bool Ray::operator!=(const Ray& other) const
	{
		return Handle != other.Handle;
	}
	
	Ray::Ray(UnityEngine::Vector3& origin, UnityEngine::Vector3& direction)
		: System::ValueType(nullptr)
	{
		auto returnValue = Plugin::UnityEngineRayConstructorUnityEngineVector3_UnityEngineVector3(origin, direction);
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
			Plugin::ReferenceManagedUnityEngineRay(Handle);
		}
	}
}

namespace System
{
	Object::Object(UnityEngine::Ray& val)
	{
		int32_t handle = Plugin::BoxRay(val.Handle);
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
		UnityEngine::Ray returnVal(Plugin::InternalUse::Only, Plugin::UnboxRay(Handle));
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
	Physics::Physics(decltype(nullptr))
	{
	}
	
	Physics::Physics(Plugin::InternalUse iu, int32_t handle)
	{
		Handle = handle;
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
	
	Physics& Physics::operator=(decltype(nullptr))
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
	
	int32_t Physics::RaycastNonAlloc(UnityEngine::Ray& ray, System::Array1<UnityEngine::RaycastHit>& results)
	{
		auto returnValue = Plugin::UnityEnginePhysicsMethodRaycastNonAllocUnityEngineRay_UnityEngineRaycastHitArray1(ray.Handle, results.Handle);
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
		auto returnValue = Plugin::UnityEnginePhysicsMethodRaycastAllUnityEngineRay(ray.Handle);
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
	Gradient::Gradient(decltype(nullptr))
	{
	}
	
	Gradient::Gradient(Plugin::InternalUse iu, int32_t handle)
	{
		Handle = handle;
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
	
	Gradient& Gradient::operator=(decltype(nullptr))
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
	
	void Gradient::SetColorKeys(System::Array1<UnityEngine::GradientColorKey>& value)
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
	AppDomainSetup::AppDomainSetup(decltype(nullptr))
		: System::IAppDomainSetup(nullptr)
	{
	}
	
	AppDomainSetup::AppDomainSetup(Plugin::InternalUse iu, int32_t handle)
		: System::IAppDomainSetup(nullptr)
	{
		Handle = handle;
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
	
	AppDomainSetup& AppDomainSetup::operator=(decltype(nullptr))
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
		: System::IAppDomainSetup(nullptr)
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
	
	void AppDomainSetup::SetAppDomainInitializer(System::AppDomainInitializer& value)
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
	Application::Application(decltype(nullptr))
	{
	}
	
	Application::Application(Plugin::InternalUse iu, int32_t handle)
	{
		Handle = handle;
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
	
	Application& Application::operator=(decltype(nullptr))
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
	
	void Application::AddOnBeforeRender(UnityEngine::Events::UnityAction& del)
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
	
	void Application::RemoveOnBeforeRender(UnityEngine::Events::UnityAction& del)
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
		SceneManager::SceneManager(decltype(nullptr))
		{
		}
		
		SceneManager::SceneManager(Plugin::InternalUse iu, int32_t handle)
		{
			Handle = handle;
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
		
		SceneManager& SceneManager::operator=(decltype(nullptr))
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
		
		void SceneManager::AddSceneLoaded(UnityEngine::Events::UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>& del)
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
	
		void SceneManager::RemoveSceneLoaded(UnityEngine::Events::UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>& del)
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
		Scene::Scene(decltype(nullptr))
			: System::ValueType(nullptr)
		{
		}
		
		Scene::Scene(Plugin::InternalUse iu, int32_t handle)
			: System::ValueType(nullptr)
		{
			Handle = handle;
			if (handle)
			{
				Plugin::ReferenceManagedUnityEngineSceneManagementScene(Handle);
			}
		}
		
		Scene::Scene(const Scene& other)
			: Scene(Plugin::InternalUse::Only, other.Handle)
		{
		}
		
		Scene::Scene(Scene&& other)
			: Scene(Plugin::InternalUse::Only, other.Handle)
		{
			other.Handle = 0;
		}
		
		Scene::~Scene()
		{
			if (Handle)
			{
				Plugin::DereferenceManagedUnityEngineSceneManagementScene(Handle);
				Handle = 0;
			}
		}
		
		Scene& Scene::operator=(const Scene& other)
		{
			if (this->Handle)
			{
				Plugin::DereferenceManagedUnityEngineSceneManagementScene(Handle);
			}
			this->Handle = other.Handle;
			if (this->Handle)
			{
				Plugin::ReferenceManagedUnityEngineSceneManagementScene(Handle);
			}
			return *this;
		}
		
		Scene& Scene::operator=(decltype(nullptr))
		{
			if (Handle)
			{
				Plugin::DereferenceManagedUnityEngineSceneManagementScene(Handle);
				Handle = 0;
			}
			return *this;
		}
		
		Scene& Scene::operator=(Scene&& other)
		{
			if (Handle)
			{
				Plugin::DereferenceManagedUnityEngineSceneManagementScene(Handle);
			}
			Handle = other.Handle;
			other.Handle = 0;
			return *this;
		}
		
		bool Scene::operator==(const Scene& other) const
		{
			return Handle == other.Handle;
		}
		
		bool Scene::operator!=(const Scene& other) const
		{
			return Handle != other.Handle;
		}
	}
}

namespace System
{
	Object::Object(UnityEngine::SceneManagement::Scene& val)
	{
		int32_t handle = Plugin::BoxScene(val.Handle);
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
		UnityEngine::SceneManagement::Scene returnVal(Plugin::InternalUse::Only, Plugin::UnboxScene(Handle));
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
	EventArgs::EventArgs(decltype(nullptr))
	{
	}
	
	EventArgs::EventArgs(Plugin::InternalUse iu, int32_t handle)
	{
		Handle = handle;
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
	}
	
	EventArgs::EventArgs(const EventArgs& other)
		: EventArgs(Plugin::InternalUse::Only, other.Handle)
	{
	}
	
	EventArgs::EventArgs(EventArgs&& other)
		: EventArgs(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
	}
	
	EventArgs::~EventArgs()
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
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
	
	EventArgs& EventArgs::operator=(decltype(nullptr))
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
		return *this;
	}
	
	EventArgs& EventArgs::operator=(EventArgs&& other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
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
}

namespace System
{
	namespace ComponentModel
	{
		namespace Design
		{
			ComponentEventArgs::ComponentEventArgs(decltype(nullptr))
				: System::EventArgs(nullptr)
			{
			}
			
			ComponentEventArgs::ComponentEventArgs(Plugin::InternalUse iu, int32_t handle)
				: System::EventArgs(nullptr)
			{
				Handle = handle;
				if (handle)
				{
					Plugin::ReferenceManagedClass(handle);
				}
			}
			
			ComponentEventArgs::ComponentEventArgs(const ComponentEventArgs& other)
				: ComponentEventArgs(Plugin::InternalUse::Only, other.Handle)
			{
			}
			
			ComponentEventArgs::ComponentEventArgs(ComponentEventArgs&& other)
				: ComponentEventArgs(Plugin::InternalUse::Only, other.Handle)
			{
				other.Handle = 0;
			}
			
			ComponentEventArgs::~ComponentEventArgs()
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
			}
			
			ComponentEventArgs& ComponentEventArgs::operator=(const ComponentEventArgs& other)
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
			
			ComponentEventArgs& ComponentEventArgs::operator=(decltype(nullptr))
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
				return *this;
			}
			
			ComponentEventArgs& ComponentEventArgs::operator=(ComponentEventArgs&& other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
				}
				Handle = other.Handle;
				other.Handle = 0;
				return *this;
			}
			
			bool ComponentEventArgs::operator==(const ComponentEventArgs& other) const
			{
				return Handle == other.Handle;
			}
			
			bool ComponentEventArgs::operator!=(const ComponentEventArgs& other) const
			{
				return Handle != other.Handle;
			}
		}
	}
}

namespace System
{
	namespace ComponentModel
	{
		namespace Design
		{
			ComponentChangingEventArgs::ComponentChangingEventArgs(decltype(nullptr))
				: System::EventArgs(nullptr)
			{
			}
			
			ComponentChangingEventArgs::ComponentChangingEventArgs(Plugin::InternalUse iu, int32_t handle)
				: System::EventArgs(nullptr)
			{
				Handle = handle;
				if (handle)
				{
					Plugin::ReferenceManagedClass(handle);
				}
			}
			
			ComponentChangingEventArgs::ComponentChangingEventArgs(const ComponentChangingEventArgs& other)
				: ComponentChangingEventArgs(Plugin::InternalUse::Only, other.Handle)
			{
			}
			
			ComponentChangingEventArgs::ComponentChangingEventArgs(ComponentChangingEventArgs&& other)
				: ComponentChangingEventArgs(Plugin::InternalUse::Only, other.Handle)
			{
				other.Handle = 0;
			}
			
			ComponentChangingEventArgs::~ComponentChangingEventArgs()
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
			}
			
			ComponentChangingEventArgs& ComponentChangingEventArgs::operator=(const ComponentChangingEventArgs& other)
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
			
			ComponentChangingEventArgs& ComponentChangingEventArgs::operator=(decltype(nullptr))
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
				return *this;
			}
			
			ComponentChangingEventArgs& ComponentChangingEventArgs::operator=(ComponentChangingEventArgs&& other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
				}
				Handle = other.Handle;
				other.Handle = 0;
				return *this;
			}
			
			bool ComponentChangingEventArgs::operator==(const ComponentChangingEventArgs& other) const
			{
				return Handle == other.Handle;
			}
			
			bool ComponentChangingEventArgs::operator!=(const ComponentChangingEventArgs& other) const
			{
				return Handle != other.Handle;
			}
		}
	}
}

namespace System
{
	namespace ComponentModel
	{
		namespace Design
		{
			ComponentChangedEventArgs::ComponentChangedEventArgs(decltype(nullptr))
				: System::EventArgs(nullptr)
			{
			}
			
			ComponentChangedEventArgs::ComponentChangedEventArgs(Plugin::InternalUse iu, int32_t handle)
				: System::EventArgs(nullptr)
			{
				Handle = handle;
				if (handle)
				{
					Plugin::ReferenceManagedClass(handle);
				}
			}
			
			ComponentChangedEventArgs::ComponentChangedEventArgs(const ComponentChangedEventArgs& other)
				: ComponentChangedEventArgs(Plugin::InternalUse::Only, other.Handle)
			{
			}
			
			ComponentChangedEventArgs::ComponentChangedEventArgs(ComponentChangedEventArgs&& other)
				: ComponentChangedEventArgs(Plugin::InternalUse::Only, other.Handle)
			{
				other.Handle = 0;
			}
			
			ComponentChangedEventArgs::~ComponentChangedEventArgs()
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
			}
			
			ComponentChangedEventArgs& ComponentChangedEventArgs::operator=(const ComponentChangedEventArgs& other)
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
			
			ComponentChangedEventArgs& ComponentChangedEventArgs::operator=(decltype(nullptr))
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
				return *this;
			}
			
			ComponentChangedEventArgs& ComponentChangedEventArgs::operator=(ComponentChangedEventArgs&& other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
				}
				Handle = other.Handle;
				other.Handle = 0;
				return *this;
			}
			
			bool ComponentChangedEventArgs::operator==(const ComponentChangedEventArgs& other) const
			{
				return Handle == other.Handle;
			}
			
			bool ComponentChangedEventArgs::operator!=(const ComponentChangedEventArgs& other) const
			{
				return Handle != other.Handle;
			}
		}
	}
}

namespace System
{
	namespace ComponentModel
	{
		namespace Design
		{
			ComponentRenameEventArgs::ComponentRenameEventArgs(decltype(nullptr))
				: System::EventArgs(nullptr)
			{
			}
			
			ComponentRenameEventArgs::ComponentRenameEventArgs(Plugin::InternalUse iu, int32_t handle)
				: System::EventArgs(nullptr)
			{
				Handle = handle;
				if (handle)
				{
					Plugin::ReferenceManagedClass(handle);
				}
			}
			
			ComponentRenameEventArgs::ComponentRenameEventArgs(const ComponentRenameEventArgs& other)
				: ComponentRenameEventArgs(Plugin::InternalUse::Only, other.Handle)
			{
			}
			
			ComponentRenameEventArgs::ComponentRenameEventArgs(ComponentRenameEventArgs&& other)
				: ComponentRenameEventArgs(Plugin::InternalUse::Only, other.Handle)
			{
				other.Handle = 0;
			}
			
			ComponentRenameEventArgs::~ComponentRenameEventArgs()
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
			}
			
			ComponentRenameEventArgs& ComponentRenameEventArgs::operator=(const ComponentRenameEventArgs& other)
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
			
			ComponentRenameEventArgs& ComponentRenameEventArgs::operator=(decltype(nullptr))
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
				return *this;
			}
			
			ComponentRenameEventArgs& ComponentRenameEventArgs::operator=(ComponentRenameEventArgs&& other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
				}
				Handle = other.Handle;
				other.Handle = 0;
				return *this;
			}
			
			bool ComponentRenameEventArgs::operator==(const ComponentRenameEventArgs& other) const
			{
				return Handle == other.Handle;
			}
			
			bool ComponentRenameEventArgs::operator!=(const ComponentRenameEventArgs& other) const
			{
				return Handle != other.Handle;
			}
		}
	}
}

namespace System
{
	namespace ComponentModel
	{
		MemberDescriptor::MemberDescriptor(decltype(nullptr))
		{
		}
		
		MemberDescriptor::MemberDescriptor(Plugin::InternalUse iu, int32_t handle)
		{
			Handle = handle;
			if (handle)
			{
				Plugin::ReferenceManagedClass(handle);
			}
		}
		
		MemberDescriptor::MemberDescriptor(const MemberDescriptor& other)
			: MemberDescriptor(Plugin::InternalUse::Only, other.Handle)
		{
		}
		
		MemberDescriptor::MemberDescriptor(MemberDescriptor&& other)
			: MemberDescriptor(Plugin::InternalUse::Only, other.Handle)
		{
			other.Handle = 0;
		}
		
		MemberDescriptor::~MemberDescriptor()
		{
			if (Handle)
			{
				Plugin::DereferenceManagedClass(Handle);
				Handle = 0;
			}
		}
		
		MemberDescriptor& MemberDescriptor::operator=(const MemberDescriptor& other)
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
		
		MemberDescriptor& MemberDescriptor::operator=(decltype(nullptr))
		{
			if (Handle)
			{
				Plugin::DereferenceManagedClass(Handle);
				Handle = 0;
			}
			return *this;
		}
		
		MemberDescriptor& MemberDescriptor::operator=(MemberDescriptor&& other)
		{
			if (Handle)
			{
				Plugin::DereferenceManagedClass(Handle);
			}
			Handle = other.Handle;
			other.Handle = 0;
			return *this;
		}
		
		bool MemberDescriptor::operator==(const MemberDescriptor& other) const
		{
			return Handle == other.Handle;
		}
		
		bool MemberDescriptor::operator!=(const MemberDescriptor& other) const
		{
			return Handle != other.Handle;
		}
	}
}

namespace System
{
	Object::Object(UnityEngine::PrimitiveType val)
	{
		int32_t handle = Plugin::BoxPrimitiveType(val);
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
	
	Object::operator UnityEngine::PrimitiveType()
	{
		UnityEngine::PrimitiveType returnVal(Plugin::UnboxPrimitiveType(Handle));
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
	Time::Time(decltype(nullptr))
	{
	}
	
	Time::Time(Plugin::InternalUse iu, int32_t handle)
	{
		Handle = handle;
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
	}
	
	Time::Time(const Time& other)
		: Time(Plugin::InternalUse::Only, other.Handle)
	{
	}
	
	Time::Time(Time&& other)
		: Time(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
	}
	
	Time::~Time()
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
	}
	
	Time& Time::operator=(const Time& other)
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
	
	Time& Time::operator=(decltype(nullptr))
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
		return *this;
	}
	
	Time& Time::operator=(Time&& other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
		}
		Handle = other.Handle;
		other.Handle = 0;
		return *this;
	}
	
	bool Time::operator==(const Time& other) const
	{
		return Handle == other.Handle;
	}
	
	bool Time::operator!=(const Time& other) const
	{
		return Handle != other.Handle;
	}
	
	float Time::GetDeltaTime()
	{
		auto returnValue = Plugin::UnityEngineTimePropertyGetDeltaTime();
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
	Object::Object(System::IO::FileMode val)
	{
		int32_t handle = Plugin::BoxFileMode(val);
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
	
	Object::operator System::IO::FileMode()
	{
		System::IO::FileMode returnVal(Plugin::UnboxFileMode(Handle));
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
	MarshalByRefObject::MarshalByRefObject(decltype(nullptr))
	{
	}
	
	MarshalByRefObject::MarshalByRefObject(Plugin::InternalUse iu, int32_t handle)
	{
		Handle = handle;
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
	}
	
	MarshalByRefObject::MarshalByRefObject(const MarshalByRefObject& other)
		: MarshalByRefObject(Plugin::InternalUse::Only, other.Handle)
	{
	}
	
	MarshalByRefObject::MarshalByRefObject(MarshalByRefObject&& other)
		: MarshalByRefObject(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
	}
	
	MarshalByRefObject::~MarshalByRefObject()
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
	}
	
	MarshalByRefObject& MarshalByRefObject::operator=(const MarshalByRefObject& other)
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
	
	MarshalByRefObject& MarshalByRefObject::operator=(decltype(nullptr))
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
		return *this;
	}
	
	MarshalByRefObject& MarshalByRefObject::operator=(MarshalByRefObject&& other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
		}
		Handle = other.Handle;
		other.Handle = 0;
		return *this;
	}
	
	bool MarshalByRefObject::operator==(const MarshalByRefObject& other) const
	{
		return Handle == other.Handle;
	}
	
	bool MarshalByRefObject::operator!=(const MarshalByRefObject& other) const
	{
		return Handle != other.Handle;
	}
}

namespace System
{
	namespace IO
	{
		Stream::Stream(decltype(nullptr))
			: System::MarshalByRefObject(nullptr)
			, System::IDisposable(nullptr)
		{
		}
		
		Stream::Stream(Plugin::InternalUse iu, int32_t handle)
			: System::MarshalByRefObject(nullptr)
			, System::IDisposable(nullptr)
		{
			Handle = handle;
			if (handle)
			{
				Plugin::ReferenceManagedClass(handle);
			}
		}
		
		Stream::Stream(const Stream& other)
			: Stream(Plugin::InternalUse::Only, other.Handle)
		{
		}
		
		Stream::Stream(Stream&& other)
			: Stream(Plugin::InternalUse::Only, other.Handle)
		{
			other.Handle = 0;
		}
		
		Stream::~Stream()
		{
			if (Handle)
			{
				Plugin::DereferenceManagedClass(Handle);
				Handle = 0;
			}
		}
		
		Stream& Stream::operator=(const Stream& other)
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
		
		Stream& Stream::operator=(decltype(nullptr))
		{
			if (Handle)
			{
				Plugin::DereferenceManagedClass(Handle);
				Handle = 0;
			}
			return *this;
		}
		
		Stream& Stream::operator=(Stream&& other)
		{
			if (Handle)
			{
				Plugin::DereferenceManagedClass(Handle);
			}
			Handle = other.Handle;
			other.Handle = 0;
			return *this;
		}
		
		bool Stream::operator==(const Stream& other) const
		{
			return Handle == other.Handle;
		}
		
		bool Stream::operator!=(const Stream& other) const
		{
			return Handle != other.Handle;
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			IComparer<int32_t>::IComparer(decltype(nullptr))
			{
			}
			
			IComparer<int32_t>::IComparer(Plugin::InternalUse iu, int32_t handle)
			{
				Handle = handle;
				if (handle)
				{
					Plugin::ReferenceManagedClass(handle);
				}
			}
			
			IComparer<int32_t>::IComparer(const IComparer<int32_t>& other)
				: IComparer(Plugin::InternalUse::Only, other.Handle)
			{
			}
			
			IComparer<int32_t>::IComparer(IComparer<int32_t>&& other)
				: IComparer(Plugin::InternalUse::Only, other.Handle)
			{
				other.Handle = 0;
			}
			
			IComparer<int32_t>::~IComparer<int32_t>()
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
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
			
			IComparer<int32_t>& IComparer<int32_t>::operator=(decltype(nullptr))
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
				return *this;
			}
			
			IComparer<int32_t>& IComparer<int32_t>::operator=(IComparer<int32_t>&& other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
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
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			IComparer<System::String>::IComparer(decltype(nullptr))
			{
			}
			
			IComparer<System::String>::IComparer(Plugin::InternalUse iu, int32_t handle)
			{
				Handle = handle;
				if (handle)
				{
					Plugin::ReferenceManagedClass(handle);
				}
			}
			
			IComparer<System::String>::IComparer(const IComparer<System::String>& other)
				: IComparer(Plugin::InternalUse::Only, other.Handle)
			{
			}
			
			IComparer<System::String>::IComparer(IComparer<System::String>&& other)
				: IComparer(Plugin::InternalUse::Only, other.Handle)
			{
				other.Handle = 0;
			}
			
			IComparer<System::String>::~IComparer<System::String>()
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
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
			
			IComparer<System::String>& IComparer<System::String>::operator=(decltype(nullptr))
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
				return *this;
			}
			
			IComparer<System::String>& IComparer<System::String>::operator=(IComparer<System::String>&& other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
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
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			BaseIComparer<int32_t>::BaseIComparer()
				: System::Collections::Generic::IComparer<int32_t>(nullptr)
			{
				CppHandle = Plugin::StoreSystemCollectionsGenericBaseIComparerSystemInt32(this);
				int32_t* handle = &Handle;
				int32_t cppHandle = CppHandle;
				Plugin::SystemCollectionsGenericBaseIComparerSystemInt32Constructor(cppHandle, handle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
				if (Handle)
				{
					Plugin::ReferenceManagedClass(Handle);
				}
				else
				{
					Plugin::RemoveSystemCollectionsGenericBaseIComparerSystemInt32(CppHandle);
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
			
			BaseIComparer<int32_t>::BaseIComparer(decltype(nullptr))
				: System::Collections::Generic::IComparer<int32_t>(nullptr)
			{
				CppHandle = Plugin::StoreSystemCollectionsGenericBaseIComparerSystemInt32(this);
			}
			
			BaseIComparer<int32_t>::BaseIComparer(const BaseIComparer<int32_t>& other)
				: System::Collections::Generic::IComparer<int32_t>(nullptr)
			{
				Handle = other.Handle;
				CppHandle = Plugin::StoreSystemCollectionsGenericBaseIComparerSystemInt32(this);
				if (Handle)
				{
					Plugin::ReferenceManagedClass(Handle);
				}
			}
			
			BaseIComparer<int32_t>::BaseIComparer(BaseIComparer<int32_t>&& other)
				: System::Collections::Generic::IComparer<int32_t>(nullptr)
			{
				Handle = other.Handle;
				CppHandle = other.CppHandle;
				other.Handle = 0;
				other.CppHandle = 0;
			}
			
			BaseIComparer<int32_t>::BaseIComparer(Plugin::InternalUse iu, int32_t handle)
				: System::Collections::Generic::IComparer<int32_t>(nullptr)
			{
				Handle = handle;
				CppHandle = Plugin::StoreSystemCollectionsGenericBaseIComparerSystemInt32(this);
				if (Handle)
				{
					Plugin::ReferenceManagedClass(Handle);
				}
			}
			
			BaseIComparer<int32_t>::~BaseIComparer()
			{
				Plugin::RemoveSystemCollectionsGenericBaseIComparerSystemInt32(CppHandle);
				CppHandle = 0;
				if (Handle)
				{
					int32_t handle = Handle;
					Handle = 0;
					if (Plugin::DereferenceManagedClassNoRelease(handle))
					{
						Plugin::ReleaseSystemCollectionsGenericBaseIComparerSystemInt32(handle);
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
			
			BaseIComparer<int32_t>& BaseIComparer<int32_t>::operator=(const BaseIComparer<int32_t>& other)
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
			
			BaseIComparer<int32_t>& BaseIComparer<int32_t>::operator=(decltype(nullptr))
			{
				if (Handle)
				{
					int32_t handle = Handle;
					Handle = 0;
					if (Plugin::DereferenceManagedClassNoRelease(handle))
					{
						Plugin::ReleaseSystemCollectionsGenericBaseIComparerSystemInt32(handle);
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
			
			BaseIComparer<int32_t>& BaseIComparer<int32_t>::operator=(BaseIComparer<int32_t>&& other)
			{
				Plugin::RemoveSystemCollectionsGenericBaseIComparerSystemInt32(CppHandle);
				CppHandle = 0;
				if (Handle)
				{
					int32_t handle = Handle;
					Handle = 0;
					if (Plugin::DereferenceManagedClassNoRelease(handle))
					{
						Plugin::ReleaseSystemCollectionsGenericBaseIComparerSystemInt32(handle);
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
			
			bool BaseIComparer<int32_t>::operator==(const BaseIComparer<int32_t>& other) const
			{
				return Handle == other.Handle;
			}
			
			bool BaseIComparer<int32_t>::operator!=(const BaseIComparer<int32_t>& other) const
			{
				return Handle != other.Handle;
			}
			
			int32_t BaseIComparer<int32_t>::Compare(int32_t x, int32_t y)
			{
				return {};
			}
			
			DLLEXPORT int32_t SystemCollectionsGenericIComparerSystemInt32Compare(int32_t cppHandle, int32_t x, int32_t y)
			{
				try
				{
					return Plugin::GetSystemCollectionsGenericBaseIComparerSystemInt32(cppHandle)->Compare(x, y);
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
			BaseIComparer<System::String>::BaseIComparer()
				: System::Collections::Generic::IComparer<System::String>(nullptr)
			{
				CppHandle = Plugin::StoreSystemCollectionsGenericBaseIComparerSystemString(this);
				int32_t* handle = &Handle;
				int32_t cppHandle = CppHandle;
				Plugin::SystemCollectionsGenericBaseIComparerSystemStringConstructor(cppHandle, handle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
				if (Handle)
				{
					Plugin::ReferenceManagedClass(Handle);
				}
				else
				{
					Plugin::RemoveSystemCollectionsGenericBaseIComparerSystemString(CppHandle);
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
			
			BaseIComparer<System::String>::BaseIComparer(decltype(nullptr))
				: System::Collections::Generic::IComparer<System::String>(nullptr)
			{
				CppHandle = Plugin::StoreSystemCollectionsGenericBaseIComparerSystemString(this);
			}
			
			BaseIComparer<System::String>::BaseIComparer(const BaseIComparer<System::String>& other)
				: System::Collections::Generic::IComparer<System::String>(nullptr)
			{
				Handle = other.Handle;
				CppHandle = Plugin::StoreSystemCollectionsGenericBaseIComparerSystemString(this);
				if (Handle)
				{
					Plugin::ReferenceManagedClass(Handle);
				}
			}
			
			BaseIComparer<System::String>::BaseIComparer(BaseIComparer<System::String>&& other)
				: System::Collections::Generic::IComparer<System::String>(nullptr)
			{
				Handle = other.Handle;
				CppHandle = other.CppHandle;
				other.Handle = 0;
				other.CppHandle = 0;
			}
			
			BaseIComparer<System::String>::BaseIComparer(Plugin::InternalUse iu, int32_t handle)
				: System::Collections::Generic::IComparer<System::String>(nullptr)
			{
				Handle = handle;
				CppHandle = Plugin::StoreSystemCollectionsGenericBaseIComparerSystemString(this);
				if (Handle)
				{
					Plugin::ReferenceManagedClass(Handle);
				}
			}
			
			BaseIComparer<System::String>::~BaseIComparer()
			{
				Plugin::RemoveSystemCollectionsGenericBaseIComparerSystemString(CppHandle);
				CppHandle = 0;
				if (Handle)
				{
					int32_t handle = Handle;
					Handle = 0;
					if (Plugin::DereferenceManagedClassNoRelease(handle))
					{
						Plugin::ReleaseSystemCollectionsGenericBaseIComparerSystemString(handle);
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
			
			BaseIComparer<System::String>& BaseIComparer<System::String>::operator=(const BaseIComparer<System::String>& other)
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
			
			BaseIComparer<System::String>& BaseIComparer<System::String>::operator=(decltype(nullptr))
			{
				if (Handle)
				{
					int32_t handle = Handle;
					Handle = 0;
					if (Plugin::DereferenceManagedClassNoRelease(handle))
					{
						Plugin::ReleaseSystemCollectionsGenericBaseIComparerSystemString(handle);
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
			
			BaseIComparer<System::String>& BaseIComparer<System::String>::operator=(BaseIComparer<System::String>&& other)
			{
				Plugin::RemoveSystemCollectionsGenericBaseIComparerSystemString(CppHandle);
				CppHandle = 0;
				if (Handle)
				{
					int32_t handle = Handle;
					Handle = 0;
					if (Plugin::DereferenceManagedClassNoRelease(handle))
					{
						Plugin::ReleaseSystemCollectionsGenericBaseIComparerSystemString(handle);
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
			
			bool BaseIComparer<System::String>::operator==(const BaseIComparer<System::String>& other) const
			{
				return Handle == other.Handle;
			}
			
			bool BaseIComparer<System::String>::operator!=(const BaseIComparer<System::String>& other) const
			{
				return Handle != other.Handle;
			}
			
			int32_t BaseIComparer<System::String>::Compare(System::String& x, System::String& y)
			{
				return {};
			}
			
			DLLEXPORT int32_t SystemCollectionsGenericIComparerSystemStringCompare(int32_t cppHandle, int32_t xHandle, int32_t yHandle)
			{
				try
				{
					auto x = System::String(Plugin::InternalUse::Only, xHandle);
					auto y = System::String(Plugin::InternalUse::Only, yHandle);
					return Plugin::GetSystemCollectionsGenericBaseIComparerSystemString(cppHandle)->Compare(x, y);
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
	StringComparer::StringComparer(decltype(nullptr))
		: System::Collections::IComparer(nullptr)
		, System::Collections::Generic::IComparer<System::String>(nullptr)
		, System::Collections::IEqualityComparer(nullptr)
		, System::Collections::Generic::IEqualityComparer<System::String>(nullptr)
	{
	}
	
	StringComparer::StringComparer(Plugin::InternalUse iu, int32_t handle)
		: System::Collections::IComparer(nullptr)
		, System::Collections::Generic::IComparer<System::String>(nullptr)
		, System::Collections::IEqualityComparer(nullptr)
		, System::Collections::Generic::IEqualityComparer<System::String>(nullptr)
	{
		Handle = handle;
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
	}
	
	StringComparer::StringComparer(const StringComparer& other)
		: StringComparer(Plugin::InternalUse::Only, other.Handle)
	{
	}
	
	StringComparer::StringComparer(StringComparer&& other)
		: StringComparer(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
	}
	
	StringComparer::~StringComparer()
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
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
	
	StringComparer& StringComparer::operator=(decltype(nullptr))
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
		return *this;
	}
	
	StringComparer& StringComparer::operator=(StringComparer&& other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
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
}

namespace System
{
	BaseStringComparer::BaseStringComparer()
		: System::Collections::IComparer(nullptr)
		, System::Collections::Generic::IComparer<System::String>(nullptr)
		, System::Collections::IEqualityComparer(nullptr)
		, System::Collections::Generic::IEqualityComparer<System::String>(nullptr)
		, System::StringComparer(nullptr)
	{
		CppHandle = Plugin::StoreSystemBaseStringComparer(this);
		int32_t* handle = &Handle;
		int32_t cppHandle = CppHandle;
		Plugin::SystemBaseStringComparerConstructor(cppHandle, handle);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
		else
		{
			Plugin::RemoveSystemBaseStringComparer(CppHandle);
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
	
	BaseStringComparer::BaseStringComparer(decltype(nullptr))
		: System::Collections::IComparer(nullptr)
		, System::Collections::Generic::IComparer<System::String>(nullptr)
		, System::Collections::IEqualityComparer(nullptr)
		, System::Collections::Generic::IEqualityComparer<System::String>(nullptr)
		, System::StringComparer(nullptr)
	{
		CppHandle = Plugin::StoreSystemBaseStringComparer(this);
	}
	
	BaseStringComparer::BaseStringComparer(const BaseStringComparer& other)
		: System::Collections::IComparer(nullptr)
		, System::Collections::Generic::IComparer<System::String>(nullptr)
		, System::Collections::IEqualityComparer(nullptr)
		, System::Collections::Generic::IEqualityComparer<System::String>(nullptr)
		, System::StringComparer(nullptr)
	{
		Handle = other.Handle;
		CppHandle = Plugin::StoreSystemBaseStringComparer(this);
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
	}
	
	BaseStringComparer::BaseStringComparer(BaseStringComparer&& other)
		: System::Collections::IComparer(nullptr)
		, System::Collections::Generic::IComparer<System::String>(nullptr)
		, System::Collections::IEqualityComparer(nullptr)
		, System::Collections::Generic::IEqualityComparer<System::String>(nullptr)
		, System::StringComparer(nullptr)
	{
		Handle = other.Handle;
		CppHandle = other.CppHandle;
		other.Handle = 0;
		other.CppHandle = 0;
	}
	
	BaseStringComparer::BaseStringComparer(Plugin::InternalUse iu, int32_t handle)
		: System::Collections::IComparer(nullptr)
		, System::Collections::Generic::IComparer<System::String>(nullptr)
		, System::Collections::IEqualityComparer(nullptr)
		, System::Collections::Generic::IEqualityComparer<System::String>(nullptr)
		, System::StringComparer(nullptr)
	{
		Handle = handle;
		CppHandle = Plugin::StoreSystemBaseStringComparer(this);
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
	}
	
	BaseStringComparer::~BaseStringComparer()
	{
		Plugin::RemoveSystemBaseStringComparer(CppHandle);
		CppHandle = 0;
		if (Handle)
		{
			int32_t handle = Handle;
			Handle = 0;
			if (Plugin::DereferenceManagedClassNoRelease(handle))
			{
				Plugin::ReleaseSystemBaseStringComparer(handle);
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
	
	BaseStringComparer& BaseStringComparer::operator=(const BaseStringComparer& other)
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
	
	BaseStringComparer& BaseStringComparer::operator=(decltype(nullptr))
	{
		if (Handle)
		{
			int32_t handle = Handle;
			Handle = 0;
			if (Plugin::DereferenceManagedClassNoRelease(handle))
			{
				Plugin::ReleaseSystemBaseStringComparer(handle);
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
	
	BaseStringComparer& BaseStringComparer::operator=(BaseStringComparer&& other)
	{
		Plugin::RemoveSystemBaseStringComparer(CppHandle);
		CppHandle = 0;
		if (Handle)
		{
			int32_t handle = Handle;
			Handle = 0;
			if (Plugin::DereferenceManagedClassNoRelease(handle))
			{
				Plugin::ReleaseSystemBaseStringComparer(handle);
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
	
	bool BaseStringComparer::operator==(const BaseStringComparer& other) const
	{
		return Handle == other.Handle;
	}
	
	bool BaseStringComparer::operator!=(const BaseStringComparer& other) const
	{
		return Handle != other.Handle;
	}
	
	int32_t BaseStringComparer::Compare(System::String& x, System::String& y)
	{
		return {};
	}
	
	DLLEXPORT int32_t SystemStringComparerCompare(int32_t cppHandle, int32_t xHandle, int32_t yHandle)
	{
		try
		{
			auto x = System::String(Plugin::InternalUse::Only, xHandle);
			auto y = System::String(Plugin::InternalUse::Only, yHandle);
			return Plugin::GetSystemBaseStringComparer(cppHandle)->Compare(x, y);
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
	
	System::Boolean BaseStringComparer::Equals(System::String& x, System::String& y)
	{
		return {};
	}
	
	DLLEXPORT int32_t SystemStringComparerEquals(int32_t cppHandle, int32_t xHandle, int32_t yHandle)
	{
		try
		{
			auto x = System::String(Plugin::InternalUse::Only, xHandle);
			auto y = System::String(Plugin::InternalUse::Only, yHandle);
			return Plugin::GetSystemBaseStringComparer(cppHandle)->Equals(x, y);
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
	
	int32_t BaseStringComparer::GetHashCode(System::String& obj)
	{
		return {};
	}
	
	DLLEXPORT int32_t SystemStringComparerGetHashCode(int32_t cppHandle, int32_t objHandle)
	{
		try
		{
			auto obj = System::String(Plugin::InternalUse::Only, objHandle);
			return Plugin::GetSystemBaseStringComparer(cppHandle)->GetHashCode(obj);
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
	namespace Collections
	{
		Queue::Queue(decltype(nullptr))
			: System::ICloneable(nullptr)
			, System::Collections::IEnumerable(nullptr)
			, System::Collections::ICollection(nullptr)
		{
		}
		
		Queue::Queue(Plugin::InternalUse iu, int32_t handle)
			: System::ICloneable(nullptr)
			, System::Collections::IEnumerable(nullptr)
			, System::Collections::ICollection(nullptr)
		{
			Handle = handle;
			if (handle)
			{
				Plugin::ReferenceManagedClass(handle);
			}
		}
		
		Queue::Queue(const Queue& other)
			: Queue(Plugin::InternalUse::Only, other.Handle)
		{
		}
		
		Queue::Queue(Queue&& other)
			: Queue(Plugin::InternalUse::Only, other.Handle)
		{
			other.Handle = 0;
		}
		
		Queue::~Queue()
		{
			if (Handle)
			{
				Plugin::DereferenceManagedClass(Handle);
				Handle = 0;
			}
		}
		
		Queue& Queue::operator=(const Queue& other)
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
		
		Queue& Queue::operator=(decltype(nullptr))
		{
			if (Handle)
			{
				Plugin::DereferenceManagedClass(Handle);
				Handle = 0;
			}
			return *this;
		}
		
		Queue& Queue::operator=(Queue&& other)
		{
			if (Handle)
			{
				Plugin::DereferenceManagedClass(Handle);
			}
			Handle = other.Handle;
			other.Handle = 0;
			return *this;
		}
		
		bool Queue::operator==(const Queue& other) const
		{
			return Handle == other.Handle;
		}
		
		bool Queue::operator!=(const Queue& other) const
		{
			return Handle != other.Handle;
		}
		
		int32_t Queue::GetCount()
		{
			auto returnValue = Plugin::SystemCollectionsQueuePropertyGetCount(Handle);
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

namespace System
{
	namespace Collections
	{
		BaseQueue::BaseQueue()
			: System::ICloneable(nullptr)
			, System::Collections::IEnumerable(nullptr)
			, System::Collections::ICollection(nullptr)
			, System::Collections::Queue(nullptr)
		{
			CppHandle = Plugin::StoreSystemCollectionsBaseQueue(this);
			int32_t* handle = &Handle;
			int32_t cppHandle = CppHandle;
			Plugin::SystemCollectionsBaseQueueConstructor(cppHandle, handle);
			if (Plugin::unhandledCsharpException)
			{
				System::Exception* ex = Plugin::unhandledCsharpException;
				Plugin::unhandledCsharpException = nullptr;
				ex->ThrowReferenceToThis();
				delete ex;
			}
			if (Handle)
			{
				Plugin::ReferenceManagedClass(Handle);
			}
			else
			{
				Plugin::RemoveSystemCollectionsBaseQueue(CppHandle);
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
		
		BaseQueue::BaseQueue(decltype(nullptr))
			: System::ICloneable(nullptr)
			, System::Collections::IEnumerable(nullptr)
			, System::Collections::ICollection(nullptr)
			, System::Collections::Queue(nullptr)
		{
			CppHandle = Plugin::StoreSystemCollectionsBaseQueue(this);
		}
		
		BaseQueue::BaseQueue(const BaseQueue& other)
			: System::ICloneable(nullptr)
			, System::Collections::IEnumerable(nullptr)
			, System::Collections::ICollection(nullptr)
			, System::Collections::Queue(nullptr)
		{
			Handle = other.Handle;
			CppHandle = Plugin::StoreSystemCollectionsBaseQueue(this);
			if (Handle)
			{
				Plugin::ReferenceManagedClass(Handle);
			}
		}
		
		BaseQueue::BaseQueue(BaseQueue&& other)
			: System::ICloneable(nullptr)
			, System::Collections::IEnumerable(nullptr)
			, System::Collections::ICollection(nullptr)
			, System::Collections::Queue(nullptr)
		{
			Handle = other.Handle;
			CppHandle = other.CppHandle;
			other.Handle = 0;
			other.CppHandle = 0;
		}
		
		BaseQueue::BaseQueue(Plugin::InternalUse iu, int32_t handle)
			: System::ICloneable(nullptr)
			, System::Collections::IEnumerable(nullptr)
			, System::Collections::ICollection(nullptr)
			, System::Collections::Queue(nullptr)
		{
			Handle = handle;
			CppHandle = Plugin::StoreSystemCollectionsBaseQueue(this);
			if (Handle)
			{
				Plugin::ReferenceManagedClass(Handle);
			}
		}
		
		BaseQueue::~BaseQueue()
		{
			Plugin::RemoveSystemCollectionsBaseQueue(CppHandle);
			CppHandle = 0;
			if (Handle)
			{
				int32_t handle = Handle;
				Handle = 0;
				if (Plugin::DereferenceManagedClassNoRelease(handle))
				{
					Plugin::ReleaseSystemCollectionsBaseQueue(handle);
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
		
		BaseQueue& BaseQueue::operator=(const BaseQueue& other)
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
		
		BaseQueue& BaseQueue::operator=(decltype(nullptr))
		{
			if (Handle)
			{
				int32_t handle = Handle;
				Handle = 0;
				if (Plugin::DereferenceManagedClassNoRelease(handle))
				{
					Plugin::ReleaseSystemCollectionsBaseQueue(handle);
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
		
		BaseQueue& BaseQueue::operator=(BaseQueue&& other)
		{
			Plugin::RemoveSystemCollectionsBaseQueue(CppHandle);
			CppHandle = 0;
			if (Handle)
			{
				int32_t handle = Handle;
				Handle = 0;
				if (Plugin::DereferenceManagedClassNoRelease(handle))
				{
					Plugin::ReleaseSystemCollectionsBaseQueue(handle);
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
		
		bool BaseQueue::operator==(const BaseQueue& other) const
		{
			return Handle == other.Handle;
		}
		
		bool BaseQueue::operator!=(const BaseQueue& other) const
		{
			return Handle != other.Handle;
		}
		
		int32_t BaseQueue::GetCount()
		{
			return {};
		}
		
		DLLEXPORT int32_t SystemCollectionsQueueGetCount(int32_t cppHandle)
		{
			try
			{
				return Plugin::GetSystemCollectionsBaseQueue(cppHandle)->GetCount();
			}
			catch (System::Exception ex)
			{
				Plugin::SetException(ex.Handle);
				return {};
			}
			catch (...)
			{
				System::String msg = "Unhandled exception invoking System::Collections::Queue";
				System::Exception ex(msg);
				Plugin::SetException(ex.Handle);
				return {};
			}
		}
	}
}

namespace System
{
	namespace ComponentModel
	{
		namespace Design
		{
			IComponentChangeService::IComponentChangeService(decltype(nullptr))
			{
			}
			
			IComponentChangeService::IComponentChangeService(Plugin::InternalUse iu, int32_t handle)
			{
				Handle = handle;
				if (handle)
				{
					Plugin::ReferenceManagedClass(handle);
				}
			}
			
			IComponentChangeService::IComponentChangeService(const IComponentChangeService& other)
				: IComponentChangeService(Plugin::InternalUse::Only, other.Handle)
			{
			}
			
			IComponentChangeService::IComponentChangeService(IComponentChangeService&& other)
				: IComponentChangeService(Plugin::InternalUse::Only, other.Handle)
			{
				other.Handle = 0;
			}
			
			IComponentChangeService::~IComponentChangeService()
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
			}
			
			IComponentChangeService& IComponentChangeService::operator=(const IComponentChangeService& other)
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
			
			IComponentChangeService& IComponentChangeService::operator=(decltype(nullptr))
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
				return *this;
			}
			
			IComponentChangeService& IComponentChangeService::operator=(IComponentChangeService&& other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
				}
				Handle = other.Handle;
				other.Handle = 0;
				return *this;
			}
			
			bool IComponentChangeService::operator==(const IComponentChangeService& other) const
			{
				return Handle == other.Handle;
			}
			
			bool IComponentChangeService::operator!=(const IComponentChangeService& other) const
			{
				return Handle != other.Handle;
			}
		}
	}
}

namespace System
{
	namespace ComponentModel
	{
		namespace Design
		{
			BaseIComponentChangeService::BaseIComponentChangeService()
				: System::ComponentModel::Design::IComponentChangeService(nullptr)
			{
				CppHandle = Plugin::StoreSystemComponentModelDesignBaseIComponentChangeService(this);
				int32_t* handle = &Handle;
				int32_t cppHandle = CppHandle;
				Plugin::SystemComponentModelDesignBaseIComponentChangeServiceConstructor(cppHandle, handle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
				if (Handle)
				{
					Plugin::ReferenceManagedClass(Handle);
				}
				else
				{
					Plugin::RemoveSystemComponentModelDesignBaseIComponentChangeService(CppHandle);
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
			
			BaseIComponentChangeService::BaseIComponentChangeService(decltype(nullptr))
				: System::ComponentModel::Design::IComponentChangeService(nullptr)
			{
				CppHandle = Plugin::StoreSystemComponentModelDesignBaseIComponentChangeService(this);
			}
			
			BaseIComponentChangeService::BaseIComponentChangeService(const BaseIComponentChangeService& other)
				: System::ComponentModel::Design::IComponentChangeService(nullptr)
			{
				Handle = other.Handle;
				CppHandle = Plugin::StoreSystemComponentModelDesignBaseIComponentChangeService(this);
				if (Handle)
				{
					Plugin::ReferenceManagedClass(Handle);
				}
			}
			
			BaseIComponentChangeService::BaseIComponentChangeService(BaseIComponentChangeService&& other)
				: System::ComponentModel::Design::IComponentChangeService(nullptr)
			{
				Handle = other.Handle;
				CppHandle = other.CppHandle;
				other.Handle = 0;
				other.CppHandle = 0;
			}
			
			BaseIComponentChangeService::BaseIComponentChangeService(Plugin::InternalUse iu, int32_t handle)
				: System::ComponentModel::Design::IComponentChangeService(nullptr)
			{
				Handle = handle;
				CppHandle = Plugin::StoreSystemComponentModelDesignBaseIComponentChangeService(this);
				if (Handle)
				{
					Plugin::ReferenceManagedClass(Handle);
				}
			}
			
			BaseIComponentChangeService::~BaseIComponentChangeService()
			{
				Plugin::RemoveSystemComponentModelDesignBaseIComponentChangeService(CppHandle);
				CppHandle = 0;
				if (Handle)
				{
					int32_t handle = Handle;
					Handle = 0;
					if (Plugin::DereferenceManagedClassNoRelease(handle))
					{
						Plugin::ReleaseSystemComponentModelDesignBaseIComponentChangeService(handle);
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
			
			BaseIComponentChangeService& BaseIComponentChangeService::operator=(const BaseIComponentChangeService& other)
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
			
			BaseIComponentChangeService& BaseIComponentChangeService::operator=(decltype(nullptr))
			{
				if (Handle)
				{
					int32_t handle = Handle;
					Handle = 0;
					if (Plugin::DereferenceManagedClassNoRelease(handle))
					{
						Plugin::ReleaseSystemComponentModelDesignBaseIComponentChangeService(handle);
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
			
			BaseIComponentChangeService& BaseIComponentChangeService::operator=(BaseIComponentChangeService&& other)
			{
				Plugin::RemoveSystemComponentModelDesignBaseIComponentChangeService(CppHandle);
				CppHandle = 0;
				if (Handle)
				{
					int32_t handle = Handle;
					Handle = 0;
					if (Plugin::DereferenceManagedClassNoRelease(handle))
					{
						Plugin::ReleaseSystemComponentModelDesignBaseIComponentChangeService(handle);
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
			
			bool BaseIComponentChangeService::operator==(const BaseIComponentChangeService& other) const
			{
				return Handle == other.Handle;
			}
			
			bool BaseIComponentChangeService::operator!=(const BaseIComponentChangeService& other) const
			{
				return Handle != other.Handle;
			}
			
			void BaseIComponentChangeService::OnComponentChanged(System::Object& component, System::ComponentModel::MemberDescriptor& member, System::Object& oldValue, System::Object& newValue)
			{
			}
			
			DLLEXPORT void SystemComponentModelDesignIComponentChangeServiceOnComponentChanged(int32_t cppHandle, int32_t componentHandle, int32_t memberHandle, int32_t oldValueHandle, int32_t newValueHandle)
			{
				try
				{
					auto component = System::Object(Plugin::InternalUse::Only, componentHandle);
					auto member = System::ComponentModel::MemberDescriptor(Plugin::InternalUse::Only, memberHandle);
					auto oldValue = System::Object(Plugin::InternalUse::Only, oldValueHandle);
					auto newValue = System::Object(Plugin::InternalUse::Only, newValueHandle);
					Plugin::GetSystemComponentModelDesignBaseIComponentChangeService(cppHandle)->OnComponentChanged(component, member, oldValue, newValue);
				}
				catch (System::Exception ex)
				{
					Plugin::SetException(ex.Handle);
				}
				catch (...)
				{
					System::String msg = "Unhandled exception invoking System::ComponentModel::Design::IComponentChangeService";
					System::Exception ex(msg);
					Plugin::SetException(ex.Handle);
				}
			}
			
			void BaseIComponentChangeService::OnComponentChanging(System::Object& component, System::ComponentModel::MemberDescriptor& member)
			{
			}
			
			DLLEXPORT void SystemComponentModelDesignIComponentChangeServiceOnComponentChanging(int32_t cppHandle, int32_t componentHandle, int32_t memberHandle)
			{
				try
				{
					auto component = System::Object(Plugin::InternalUse::Only, componentHandle);
					auto member = System::ComponentModel::MemberDescriptor(Plugin::InternalUse::Only, memberHandle);
					Plugin::GetSystemComponentModelDesignBaseIComponentChangeService(cppHandle)->OnComponentChanging(component, member);
				}
				catch (System::Exception ex)
				{
					Plugin::SetException(ex.Handle);
				}
				catch (...)
				{
					System::String msg = "Unhandled exception invoking System::ComponentModel::Design::IComponentChangeService";
					System::Exception ex(msg);
					Plugin::SetException(ex.Handle);
				}
			}
			
			void BaseIComponentChangeService::AddComponentAdded(System::ComponentModel::Design::ComponentEventHandler& value)
			{
			}
			
			DLLEXPORT void SystemComponentModelDesignIComponentChangeServiceAddComponentAdded(int32_t cppHandle, int32_t valueHandle)
			{
				try
				{
					auto value = System::ComponentModel::Design::ComponentEventHandler(Plugin::InternalUse::Only, valueHandle);
					Plugin::GetSystemComponentModelDesignBaseIComponentChangeService(cppHandle)->AddComponentAdded(value);
				}
				catch (System::Exception ex)
				{
					Plugin::SetException(ex.Handle);
				}
				catch (...)
				{
					System::String msg = "Unhandled exception invoking System::ComponentModel::Design::IComponentChangeService";
					System::Exception ex(msg);
					Plugin::SetException(ex.Handle);
				}
			}
			
			void BaseIComponentChangeService::RemoveComponentAdded(System::ComponentModel::Design::ComponentEventHandler& value)
			{
			}
			
			DLLEXPORT void SystemComponentModelDesignIComponentChangeServiceRemoveComponentAdded(int32_t cppHandle, int32_t valueHandle)
			{
				try
				{
					auto value = System::ComponentModel::Design::ComponentEventHandler(Plugin::InternalUse::Only, valueHandle);
					Plugin::GetSystemComponentModelDesignBaseIComponentChangeService(cppHandle)->RemoveComponentAdded(value);
				}
				catch (System::Exception ex)
				{
					Plugin::SetException(ex.Handle);
				}
				catch (...)
				{
					System::String msg = "Unhandled exception invoking System::ComponentModel::Design::IComponentChangeService";
					System::Exception ex(msg);
					Plugin::SetException(ex.Handle);
				}
			}
			
			void BaseIComponentChangeService::AddComponentAdding(System::ComponentModel::Design::ComponentEventHandler& value)
			{
			}
			
			DLLEXPORT void SystemComponentModelDesignIComponentChangeServiceAddComponentAdding(int32_t cppHandle, int32_t valueHandle)
			{
				try
				{
					auto value = System::ComponentModel::Design::ComponentEventHandler(Plugin::InternalUse::Only, valueHandle);
					Plugin::GetSystemComponentModelDesignBaseIComponentChangeService(cppHandle)->AddComponentAdding(value);
				}
				catch (System::Exception ex)
				{
					Plugin::SetException(ex.Handle);
				}
				catch (...)
				{
					System::String msg = "Unhandled exception invoking System::ComponentModel::Design::IComponentChangeService";
					System::Exception ex(msg);
					Plugin::SetException(ex.Handle);
				}
			}
			
			void BaseIComponentChangeService::RemoveComponentAdding(System::ComponentModel::Design::ComponentEventHandler& value)
			{
			}
			
			DLLEXPORT void SystemComponentModelDesignIComponentChangeServiceRemoveComponentAdding(int32_t cppHandle, int32_t valueHandle)
			{
				try
				{
					auto value = System::ComponentModel::Design::ComponentEventHandler(Plugin::InternalUse::Only, valueHandle);
					Plugin::GetSystemComponentModelDesignBaseIComponentChangeService(cppHandle)->RemoveComponentAdding(value);
				}
				catch (System::Exception ex)
				{
					Plugin::SetException(ex.Handle);
				}
				catch (...)
				{
					System::String msg = "Unhandled exception invoking System::ComponentModel::Design::IComponentChangeService";
					System::Exception ex(msg);
					Plugin::SetException(ex.Handle);
				}
			}
			
			void BaseIComponentChangeService::AddComponentChanged(System::ComponentModel::Design::ComponentChangedEventHandler& value)
			{
			}
			
			DLLEXPORT void SystemComponentModelDesignIComponentChangeServiceAddComponentChanged(int32_t cppHandle, int32_t valueHandle)
			{
				try
				{
					auto value = System::ComponentModel::Design::ComponentChangedEventHandler(Plugin::InternalUse::Only, valueHandle);
					Plugin::GetSystemComponentModelDesignBaseIComponentChangeService(cppHandle)->AddComponentChanged(value);
				}
				catch (System::Exception ex)
				{
					Plugin::SetException(ex.Handle);
				}
				catch (...)
				{
					System::String msg = "Unhandled exception invoking System::ComponentModel::Design::IComponentChangeService";
					System::Exception ex(msg);
					Plugin::SetException(ex.Handle);
				}
			}
			
			void BaseIComponentChangeService::RemoveComponentChanged(System::ComponentModel::Design::ComponentChangedEventHandler& value)
			{
			}
			
			DLLEXPORT void SystemComponentModelDesignIComponentChangeServiceRemoveComponentChanged(int32_t cppHandle, int32_t valueHandle)
			{
				try
				{
					auto value = System::ComponentModel::Design::ComponentChangedEventHandler(Plugin::InternalUse::Only, valueHandle);
					Plugin::GetSystemComponentModelDesignBaseIComponentChangeService(cppHandle)->RemoveComponentChanged(value);
				}
				catch (System::Exception ex)
				{
					Plugin::SetException(ex.Handle);
				}
				catch (...)
				{
					System::String msg = "Unhandled exception invoking System::ComponentModel::Design::IComponentChangeService";
					System::Exception ex(msg);
					Plugin::SetException(ex.Handle);
				}
			}
			
			void BaseIComponentChangeService::AddComponentChanging(System::ComponentModel::Design::ComponentChangingEventHandler& value)
			{
			}
			
			DLLEXPORT void SystemComponentModelDesignIComponentChangeServiceAddComponentChanging(int32_t cppHandle, int32_t valueHandle)
			{
				try
				{
					auto value = System::ComponentModel::Design::ComponentChangingEventHandler(Plugin::InternalUse::Only, valueHandle);
					Plugin::GetSystemComponentModelDesignBaseIComponentChangeService(cppHandle)->AddComponentChanging(value);
				}
				catch (System::Exception ex)
				{
					Plugin::SetException(ex.Handle);
				}
				catch (...)
				{
					System::String msg = "Unhandled exception invoking System::ComponentModel::Design::IComponentChangeService";
					System::Exception ex(msg);
					Plugin::SetException(ex.Handle);
				}
			}
			
			void BaseIComponentChangeService::RemoveComponentChanging(System::ComponentModel::Design::ComponentChangingEventHandler& value)
			{
			}
			
			DLLEXPORT void SystemComponentModelDesignIComponentChangeServiceRemoveComponentChanging(int32_t cppHandle, int32_t valueHandle)
			{
				try
				{
					auto value = System::ComponentModel::Design::ComponentChangingEventHandler(Plugin::InternalUse::Only, valueHandle);
					Plugin::GetSystemComponentModelDesignBaseIComponentChangeService(cppHandle)->RemoveComponentChanging(value);
				}
				catch (System::Exception ex)
				{
					Plugin::SetException(ex.Handle);
				}
				catch (...)
				{
					System::String msg = "Unhandled exception invoking System::ComponentModel::Design::IComponentChangeService";
					System::Exception ex(msg);
					Plugin::SetException(ex.Handle);
				}
			}
			
			void BaseIComponentChangeService::AddComponentRemoved(System::ComponentModel::Design::ComponentEventHandler& value)
			{
			}
			
			DLLEXPORT void SystemComponentModelDesignIComponentChangeServiceAddComponentRemoved(int32_t cppHandle, int32_t valueHandle)
			{
				try
				{
					auto value = System::ComponentModel::Design::ComponentEventHandler(Plugin::InternalUse::Only, valueHandle);
					Plugin::GetSystemComponentModelDesignBaseIComponentChangeService(cppHandle)->AddComponentRemoved(value);
				}
				catch (System::Exception ex)
				{
					Plugin::SetException(ex.Handle);
				}
				catch (...)
				{
					System::String msg = "Unhandled exception invoking System::ComponentModel::Design::IComponentChangeService";
					System::Exception ex(msg);
					Plugin::SetException(ex.Handle);
				}
			}
			
			void BaseIComponentChangeService::RemoveComponentRemoved(System::ComponentModel::Design::ComponentEventHandler& value)
			{
			}
			
			DLLEXPORT void SystemComponentModelDesignIComponentChangeServiceRemoveComponentRemoved(int32_t cppHandle, int32_t valueHandle)
			{
				try
				{
					auto value = System::ComponentModel::Design::ComponentEventHandler(Plugin::InternalUse::Only, valueHandle);
					Plugin::GetSystemComponentModelDesignBaseIComponentChangeService(cppHandle)->RemoveComponentRemoved(value);
				}
				catch (System::Exception ex)
				{
					Plugin::SetException(ex.Handle);
				}
				catch (...)
				{
					System::String msg = "Unhandled exception invoking System::ComponentModel::Design::IComponentChangeService";
					System::Exception ex(msg);
					Plugin::SetException(ex.Handle);
				}
			}
			
			void BaseIComponentChangeService::AddComponentRemoving(System::ComponentModel::Design::ComponentEventHandler& value)
			{
			}
			
			DLLEXPORT void SystemComponentModelDesignIComponentChangeServiceAddComponentRemoving(int32_t cppHandle, int32_t valueHandle)
			{
				try
				{
					auto value = System::ComponentModel::Design::ComponentEventHandler(Plugin::InternalUse::Only, valueHandle);
					Plugin::GetSystemComponentModelDesignBaseIComponentChangeService(cppHandle)->AddComponentRemoving(value);
				}
				catch (System::Exception ex)
				{
					Plugin::SetException(ex.Handle);
				}
				catch (...)
				{
					System::String msg = "Unhandled exception invoking System::ComponentModel::Design::IComponentChangeService";
					System::Exception ex(msg);
					Plugin::SetException(ex.Handle);
				}
			}
			
			void BaseIComponentChangeService::RemoveComponentRemoving(System::ComponentModel::Design::ComponentEventHandler& value)
			{
			}
			
			DLLEXPORT void SystemComponentModelDesignIComponentChangeServiceRemoveComponentRemoving(int32_t cppHandle, int32_t valueHandle)
			{
				try
				{
					auto value = System::ComponentModel::Design::ComponentEventHandler(Plugin::InternalUse::Only, valueHandle);
					Plugin::GetSystemComponentModelDesignBaseIComponentChangeService(cppHandle)->RemoveComponentRemoving(value);
				}
				catch (System::Exception ex)
				{
					Plugin::SetException(ex.Handle);
				}
				catch (...)
				{
					System::String msg = "Unhandled exception invoking System::ComponentModel::Design::IComponentChangeService";
					System::Exception ex(msg);
					Plugin::SetException(ex.Handle);
				}
			}
			
			void BaseIComponentChangeService::AddComponentRename(System::ComponentModel::Design::ComponentRenameEventHandler& value)
			{
			}
			
			DLLEXPORT void SystemComponentModelDesignIComponentChangeServiceAddComponentRename(int32_t cppHandle, int32_t valueHandle)
			{
				try
				{
					auto value = System::ComponentModel::Design::ComponentRenameEventHandler(Plugin::InternalUse::Only, valueHandle);
					Plugin::GetSystemComponentModelDesignBaseIComponentChangeService(cppHandle)->AddComponentRename(value);
				}
				catch (System::Exception ex)
				{
					Plugin::SetException(ex.Handle);
				}
				catch (...)
				{
					System::String msg = "Unhandled exception invoking System::ComponentModel::Design::IComponentChangeService";
					System::Exception ex(msg);
					Plugin::SetException(ex.Handle);
				}
			}
			
			void BaseIComponentChangeService::RemoveComponentRename(System::ComponentModel::Design::ComponentRenameEventHandler& value)
			{
			}
			
			DLLEXPORT void SystemComponentModelDesignIComponentChangeServiceRemoveComponentRename(int32_t cppHandle, int32_t valueHandle)
			{
				try
				{
					auto value = System::ComponentModel::Design::ComponentRenameEventHandler(Plugin::InternalUse::Only, valueHandle);
					Plugin::GetSystemComponentModelDesignBaseIComponentChangeService(cppHandle)->RemoveComponentRename(value);
				}
				catch (System::Exception ex)
				{
					Plugin::SetException(ex.Handle);
				}
				catch (...)
				{
					System::String msg = "Unhandled exception invoking System::ComponentModel::Design::IComponentChangeService";
					System::Exception ex(msg);
					Plugin::SetException(ex.Handle);
				}
			}
		}
	}
}

namespace System
{
	namespace IO
	{
		FileStream::FileStream(decltype(nullptr))
			: System::MarshalByRefObject(nullptr)
			, System::IDisposable(nullptr)
			, System::IO::Stream(nullptr)
		{
		}
		
		FileStream::FileStream(Plugin::InternalUse iu, int32_t handle)
			: System::MarshalByRefObject(nullptr)
			, System::IDisposable(nullptr)
			, System::IO::Stream(nullptr)
		{
			Handle = handle;
			if (handle)
			{
				Plugin::ReferenceManagedClass(handle);
			}
		}
		
		FileStream::FileStream(const FileStream& other)
			: FileStream(Plugin::InternalUse::Only, other.Handle)
		{
		}
		
		FileStream::FileStream(FileStream&& other)
			: FileStream(Plugin::InternalUse::Only, other.Handle)
		{
			other.Handle = 0;
		}
		
		FileStream::~FileStream()
		{
			if (Handle)
			{
				Plugin::DereferenceManagedClass(Handle);
				Handle = 0;
			}
		}
		
		FileStream& FileStream::operator=(const FileStream& other)
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
		
		FileStream& FileStream::operator=(decltype(nullptr))
		{
			if (Handle)
			{
				Plugin::DereferenceManagedClass(Handle);
				Handle = 0;
			}
			return *this;
		}
		
		FileStream& FileStream::operator=(FileStream&& other)
		{
			if (Handle)
			{
				Plugin::DereferenceManagedClass(Handle);
			}
			Handle = other.Handle;
			other.Handle = 0;
			return *this;
		}
		
		bool FileStream::operator==(const FileStream& other) const
		{
			return Handle == other.Handle;
		}
		
		bool FileStream::operator!=(const FileStream& other) const
		{
			return Handle != other.Handle;
		}
		
		FileStream::FileStream(System::String& path, System::IO::FileMode mode)
			: System::MarshalByRefObject(nullptr)
			, System::IDisposable(nullptr)
			, System::IO::Stream(nullptr)
		{
			auto returnValue = Plugin::SystemIOFileStreamConstructorSystemString_SystemIOFileMode(path.Handle, mode);
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
		
		void FileStream::WriteByte(uint8_t value)
		{
			Plugin::SystemIOFileStreamMethodWriteByteSystemByte(Handle, value);
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
	namespace IO
	{
		BaseFileStream::BaseFileStream(System::String& path, System::IO::FileMode mode)
			: System::MarshalByRefObject(nullptr)
			, System::IDisposable(nullptr)
			, System::IO::Stream(nullptr)
			, System::IO::FileStream(nullptr)
		{
			CppHandle = Plugin::StoreSystemIOBaseFileStream(this);
			int32_t* handle = &Handle;
			int32_t cppHandle = CppHandle;
			Plugin::SystemIOBaseFileStreamConstructorSystemString_SystemIOFileMode(cppHandle, handle, path.Handle, mode);
			if (Plugin::unhandledCsharpException)
			{
				System::Exception* ex = Plugin::unhandledCsharpException;
				Plugin::unhandledCsharpException = nullptr;
				ex->ThrowReferenceToThis();
				delete ex;
			}
			if (Handle)
			{
				Plugin::ReferenceManagedClass(Handle);
			}
			else
			{
				Plugin::RemoveSystemIOBaseFileStream(CppHandle);
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
		
		BaseFileStream::BaseFileStream(decltype(nullptr))
			: System::MarshalByRefObject(nullptr)
			, System::IDisposable(nullptr)
			, System::IO::Stream(nullptr)
			, System::IO::FileStream(nullptr)
		{
			CppHandle = Plugin::StoreSystemIOBaseFileStream(this);
		}
		
		BaseFileStream::BaseFileStream(const BaseFileStream& other)
			: System::MarshalByRefObject(nullptr)
			, System::IDisposable(nullptr)
			, System::IO::Stream(nullptr)
			, System::IO::FileStream(nullptr)
		{
			Handle = other.Handle;
			CppHandle = Plugin::StoreSystemIOBaseFileStream(this);
			if (Handle)
			{
				Plugin::ReferenceManagedClass(Handle);
			}
		}
		
		BaseFileStream::BaseFileStream(BaseFileStream&& other)
			: System::MarshalByRefObject(nullptr)
			, System::IDisposable(nullptr)
			, System::IO::Stream(nullptr)
			, System::IO::FileStream(nullptr)
		{
			Handle = other.Handle;
			CppHandle = other.CppHandle;
			other.Handle = 0;
			other.CppHandle = 0;
		}
		
		BaseFileStream::BaseFileStream(Plugin::InternalUse iu, int32_t handle)
			: System::MarshalByRefObject(nullptr)
			, System::IDisposable(nullptr)
			, System::IO::Stream(nullptr)
			, System::IO::FileStream(nullptr)
		{
			Handle = handle;
			CppHandle = Plugin::StoreSystemIOBaseFileStream(this);
			if (Handle)
			{
				Plugin::ReferenceManagedClass(Handle);
			}
		}
		
		BaseFileStream::~BaseFileStream()
		{
			Plugin::RemoveSystemIOBaseFileStream(CppHandle);
			CppHandle = 0;
			if (Handle)
			{
				int32_t handle = Handle;
				Handle = 0;
				if (Plugin::DereferenceManagedClassNoRelease(handle))
				{
					Plugin::ReleaseSystemIOBaseFileStream(handle);
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
		
		BaseFileStream& BaseFileStream::operator=(const BaseFileStream& other)
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
		
		BaseFileStream& BaseFileStream::operator=(decltype(nullptr))
		{
			if (Handle)
			{
				int32_t handle = Handle;
				Handle = 0;
				if (Plugin::DereferenceManagedClassNoRelease(handle))
				{
					Plugin::ReleaseSystemIOBaseFileStream(handle);
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
		
		BaseFileStream& BaseFileStream::operator=(BaseFileStream&& other)
		{
			Plugin::RemoveSystemIOBaseFileStream(CppHandle);
			CppHandle = 0;
			if (Handle)
			{
				int32_t handle = Handle;
				Handle = 0;
				if (Plugin::DereferenceManagedClassNoRelease(handle))
				{
					Plugin::ReleaseSystemIOBaseFileStream(handle);
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
		
		bool BaseFileStream::operator==(const BaseFileStream& other) const
		{
			return Handle == other.Handle;
		}
		
		bool BaseFileStream::operator!=(const BaseFileStream& other) const
		{
			return Handle != other.Handle;
		}
		
		void BaseFileStream::WriteByte(uint8_t value)
		{
		}
		
		DLLEXPORT void SystemIOFileStreamWriteByte(int32_t cppHandle, uint8_t value)
		{
			try
			{
				Plugin::GetSystemIOBaseFileStream(cppHandle)->WriteByte(value);
			}
			catch (System::Exception ex)
			{
				Plugin::SetException(ex.Handle);
			}
			catch (...)
			{
				System::String msg = "Unhandled exception invoking System::IO::FileStream";
				System::Exception ex(msg);
				Plugin::SetException(ex.Handle);
			}
		}
	}
}

namespace UnityEngine
{
	namespace Playables
	{
		PlayableHandle::PlayableHandle(decltype(nullptr))
			: System::ValueType(nullptr)
		{
		}
		
		PlayableHandle::PlayableHandle(Plugin::InternalUse iu, int32_t handle)
			: System::ValueType(nullptr)
		{
			Handle = handle;
			if (handle)
			{
				Plugin::ReferenceManagedUnityEnginePlayablesPlayableHandle(Handle);
			}
		}
		
		PlayableHandle::PlayableHandle(const PlayableHandle& other)
			: PlayableHandle(Plugin::InternalUse::Only, other.Handle)
		{
		}
		
		PlayableHandle::PlayableHandle(PlayableHandle&& other)
			: PlayableHandle(Plugin::InternalUse::Only, other.Handle)
		{
			other.Handle = 0;
		}
		
		PlayableHandle::~PlayableHandle()
		{
			if (Handle)
			{
				Plugin::DereferenceManagedUnityEnginePlayablesPlayableHandle(Handle);
				Handle = 0;
			}
		}
		
		PlayableHandle& PlayableHandle::operator=(const PlayableHandle& other)
		{
			if (this->Handle)
			{
				Plugin::DereferenceManagedUnityEnginePlayablesPlayableHandle(Handle);
			}
			this->Handle = other.Handle;
			if (this->Handle)
			{
				Plugin::ReferenceManagedUnityEnginePlayablesPlayableHandle(Handle);
			}
			return *this;
		}
		
		PlayableHandle& PlayableHandle::operator=(decltype(nullptr))
		{
			if (Handle)
			{
				Plugin::DereferenceManagedUnityEnginePlayablesPlayableHandle(Handle);
				Handle = 0;
			}
			return *this;
		}
		
		PlayableHandle& PlayableHandle::operator=(PlayableHandle&& other)
		{
			if (Handle)
			{
				Plugin::DereferenceManagedUnityEnginePlayablesPlayableHandle(Handle);
			}
			Handle = other.Handle;
			other.Handle = 0;
			return *this;
		}
		
		bool PlayableHandle::operator==(const PlayableHandle& other) const
		{
			return Handle == other.Handle;
		}
		
		bool PlayableHandle::operator!=(const PlayableHandle& other) const
		{
			return Handle != other.Handle;
		}
	}
}

namespace System
{
	Object::Object(UnityEngine::Playables::PlayableHandle& val)
	{
		int32_t handle = Plugin::BoxPlayableHandle(val.Handle);
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
	
	Object::operator UnityEngine::Playables::PlayableHandle()
	{
		UnityEngine::Playables::PlayableHandle returnVal(Plugin::InternalUse::Only, Plugin::UnboxPlayableHandle(Handle));
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
	namespace Experimental
	{
		namespace UIElements
		{
			CallbackEventHandler::CallbackEventHandler(decltype(nullptr))
				: UnityEngine::Experimental::UIElements::IEventHandler(nullptr)
			{
			}
			
			CallbackEventHandler::CallbackEventHandler(Plugin::InternalUse iu, int32_t handle)
				: UnityEngine::Experimental::UIElements::IEventHandler(nullptr)
			{
				Handle = handle;
				if (handle)
				{
					Plugin::ReferenceManagedClass(handle);
				}
			}
			
			CallbackEventHandler::CallbackEventHandler(const CallbackEventHandler& other)
				: CallbackEventHandler(Plugin::InternalUse::Only, other.Handle)
			{
			}
			
			CallbackEventHandler::CallbackEventHandler(CallbackEventHandler&& other)
				: CallbackEventHandler(Plugin::InternalUse::Only, other.Handle)
			{
				other.Handle = 0;
			}
			
			CallbackEventHandler::~CallbackEventHandler()
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
			}
			
			CallbackEventHandler& CallbackEventHandler::operator=(const CallbackEventHandler& other)
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
			
			CallbackEventHandler& CallbackEventHandler::operator=(decltype(nullptr))
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
				return *this;
			}
			
			CallbackEventHandler& CallbackEventHandler::operator=(CallbackEventHandler&& other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
				}
				Handle = other.Handle;
				other.Handle = 0;
				return *this;
			}
			
			bool CallbackEventHandler::operator==(const CallbackEventHandler& other) const
			{
				return Handle == other.Handle;
			}
			
			bool CallbackEventHandler::operator!=(const CallbackEventHandler& other) const
			{
				return Handle != other.Handle;
			}
		}
	}
}

namespace UnityEngine
{
	namespace Experimental
	{
		namespace UIElements
		{
			VisualElement::VisualElement(decltype(nullptr))
				: UnityEngine::Experimental::UIElements::IEventHandler(nullptr)
				, UnityEngine::Experimental::UIElements::CallbackEventHandler(nullptr)
				, UnityEngine::Experimental::UIElements::IStyle(nullptr)
			{
			}
			
			VisualElement::VisualElement(Plugin::InternalUse iu, int32_t handle)
				: UnityEngine::Experimental::UIElements::IEventHandler(nullptr)
				, UnityEngine::Experimental::UIElements::CallbackEventHandler(nullptr)
				, UnityEngine::Experimental::UIElements::IStyle(nullptr)
			{
				Handle = handle;
				if (handle)
				{
					Plugin::ReferenceManagedClass(handle);
				}
			}
			
			VisualElement::VisualElement(const VisualElement& other)
				: VisualElement(Plugin::InternalUse::Only, other.Handle)
			{
			}
			
			VisualElement::VisualElement(VisualElement&& other)
				: VisualElement(Plugin::InternalUse::Only, other.Handle)
			{
				other.Handle = 0;
			}
			
			VisualElement::~VisualElement()
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
			}
			
			VisualElement& VisualElement::operator=(const VisualElement& other)
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
			
			VisualElement& VisualElement::operator=(decltype(nullptr))
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
				return *this;
			}
			
			VisualElement& VisualElement::operator=(VisualElement&& other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
				}
				Handle = other.Handle;
				other.Handle = 0;
				return *this;
			}
			
			bool VisualElement::operator==(const VisualElement& other) const
			{
				return Handle == other.Handle;
			}
			
			bool VisualElement::operator!=(const VisualElement& other) const
			{
				return Handle != other.Handle;
			}
		}
	}
}

namespace UnityEngine
{
	namespace Experimental
	{
		namespace UIElements
		{
			UnityEngine::Experimental::UIElements::VisualElement UQueryExtensions::Q(UnityEngine::Experimental::UIElements::VisualElement& e, System::String& name, System::Array1<System::String>& classes)
			{
				auto returnValue = Plugin::UnityEngineExperimentalUIElementsUQueryExtensionsMethodQUnityEngineExperimentalUIElementsVisualElement_SystemString_SystemStringArray1(e.Handle, name.Handle, classes.Handle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
				return UnityEngine::Experimental::UIElements::VisualElement(Plugin::InternalUse::Only, returnValue);
			}
	
			UnityEngine::Experimental::UIElements::VisualElement UQueryExtensions::Q(UnityEngine::Experimental::UIElements::VisualElement& e, System::String& name, System::String& className)
			{
				auto returnValue = Plugin::UnityEngineExperimentalUIElementsUQueryExtensionsMethodQUnityEngineExperimentalUIElementsVisualElement_SystemString_SystemString(e.Handle, name.Handle, className.Handle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
				return UnityEngine::Experimental::UIElements::VisualElement(Plugin::InternalUse::Only, returnValue);
			}
		}
	}
}

namespace System
{
	Object::Object(UnityEngine::XR::WSA::Input::InteractionSourcePositionAccuracy val)
	{
		int32_t handle = Plugin::BoxInteractionSourcePositionAccuracy(val);
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
	
	Object::operator UnityEngine::XR::WSA::Input::InteractionSourcePositionAccuracy()
	{
		UnityEngine::XR::WSA::Input::InteractionSourcePositionAccuracy returnVal(Plugin::UnboxInteractionSourcePositionAccuracy(Handle));
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
	Object::Object(UnityEngine::XR::WSA::Input::InteractionSourceNode val)
	{
		int32_t handle = Plugin::BoxInteractionSourceNode(val);
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
	
	Object::operator UnityEngine::XR::WSA::Input::InteractionSourceNode()
	{
		UnityEngine::XR::WSA::Input::InteractionSourceNode returnVal(Plugin::UnboxInteractionSourceNode(Handle));
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
	namespace XR
	{
		namespace WSA
		{
			namespace Input
			{
				InteractionSourcePose::InteractionSourcePose(decltype(nullptr))
					: System::ValueType(nullptr)
				{
				}
				
				InteractionSourcePose::InteractionSourcePose(Plugin::InternalUse iu, int32_t handle)
					: System::ValueType(nullptr)
				{
					Handle = handle;
					if (handle)
					{
						Plugin::ReferenceManagedUnityEngineXRWSAInputInteractionSourcePose(Handle);
					}
				}
				
				InteractionSourcePose::InteractionSourcePose(const InteractionSourcePose& other)
					: InteractionSourcePose(Plugin::InternalUse::Only, other.Handle)
				{
				}
				
				InteractionSourcePose::InteractionSourcePose(InteractionSourcePose&& other)
					: InteractionSourcePose(Plugin::InternalUse::Only, other.Handle)
				{
					other.Handle = 0;
				}
				
				InteractionSourcePose::~InteractionSourcePose()
				{
					if (Handle)
					{
						Plugin::DereferenceManagedUnityEngineXRWSAInputInteractionSourcePose(Handle);
						Handle = 0;
					}
				}
				
				InteractionSourcePose& InteractionSourcePose::operator=(const InteractionSourcePose& other)
				{
					if (this->Handle)
					{
						Plugin::DereferenceManagedUnityEngineXRWSAInputInteractionSourcePose(Handle);
					}
					this->Handle = other.Handle;
					if (this->Handle)
					{
						Plugin::ReferenceManagedUnityEngineXRWSAInputInteractionSourcePose(Handle);
					}
					return *this;
				}
				
				InteractionSourcePose& InteractionSourcePose::operator=(decltype(nullptr))
				{
					if (Handle)
					{
						Plugin::DereferenceManagedUnityEngineXRWSAInputInteractionSourcePose(Handle);
						Handle = 0;
					}
					return *this;
				}
				
				InteractionSourcePose& InteractionSourcePose::operator=(InteractionSourcePose&& other)
				{
					if (Handle)
					{
						Plugin::DereferenceManagedUnityEngineXRWSAInputInteractionSourcePose(Handle);
					}
					Handle = other.Handle;
					other.Handle = 0;
					return *this;
				}
				
				bool InteractionSourcePose::operator==(const InteractionSourcePose& other) const
				{
					return Handle == other.Handle;
				}
				
				bool InteractionSourcePose::operator!=(const InteractionSourcePose& other) const
				{
					return Handle != other.Handle;
				}
				
				System::Boolean InteractionSourcePose::TryGetRotation(UnityEngine::Quaternion* rotation, UnityEngine::XR::WSA::Input::InteractionSourceNode node)
				{
					auto returnValue = Plugin::UnityEngineXRWSAInputInteractionSourcePoseMethodTryGetRotationUnityEngineQuaternion_UnityEngineXRWSAInputInteractionSourceNode(Handle, rotation, node);
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
}

namespace System
{
	Object::Object(UnityEngine::XR::WSA::Input::InteractionSourcePose& val)
	{
		int32_t handle = Plugin::BoxInteractionSourcePose(val.Handle);
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
	
	Object::operator UnityEngine::XR::WSA::Input::InteractionSourcePose()
	{
		UnityEngine::XR::WSA::Input::InteractionSourcePose returnVal(Plugin::InternalUse::Only, Plugin::UnboxInteractionSourcePose(Handle));
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
		TestScript::TestScript(decltype(nullptr))
			: UnityEngine::Object(nullptr)
			, UnityEngine::Component(nullptr)
			, UnityEngine::Behaviour(nullptr)
			, UnityEngine::MonoBehaviour(nullptr)
		{
		}
		
		TestScript::TestScript(Plugin::InternalUse iu, int32_t handle)
			: UnityEngine::Object(nullptr)
			, UnityEngine::Component(nullptr)
			, UnityEngine::Behaviour(nullptr)
			, UnityEngine::MonoBehaviour(nullptr)
		{
			Handle = handle;
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
		
		TestScript& TestScript::operator=(decltype(nullptr))
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

namespace MyGame
{
	namespace MonoBehaviours
	{
		AnotherScript::AnotherScript(decltype(nullptr))
			: UnityEngine::Object(nullptr)
			, UnityEngine::Component(nullptr)
			, UnityEngine::Behaviour(nullptr)
			, UnityEngine::MonoBehaviour(nullptr)
		{
		}
		
		AnotherScript::AnotherScript(Plugin::InternalUse iu, int32_t handle)
			: UnityEngine::Object(nullptr)
			, UnityEngine::Component(nullptr)
			, UnityEngine::Behaviour(nullptr)
			, UnityEngine::MonoBehaviour(nullptr)
		{
			Handle = handle;
			if (handle)
			{
				Plugin::ReferenceManagedClass(handle);
			}
		}
		
		AnotherScript::AnotherScript(const AnotherScript& other)
			: AnotherScript(Plugin::InternalUse::Only, other.Handle)
		{
		}
		
		AnotherScript::AnotherScript(AnotherScript&& other)
			: AnotherScript(Plugin::InternalUse::Only, other.Handle)
		{
			other.Handle = 0;
		}
		
		AnotherScript::~AnotherScript()
		{
			if (Handle)
			{
				Plugin::DereferenceManagedClass(Handle);
				Handle = 0;
			}
		}
		
		AnotherScript& AnotherScript::operator=(const AnotherScript& other)
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
		
		AnotherScript& AnotherScript::operator=(decltype(nullptr))
		{
			if (Handle)
			{
				Plugin::DereferenceManagedClass(Handle);
				Handle = 0;
			}
			return *this;
		}
		
		AnotherScript& AnotherScript::operator=(AnotherScript&& other)
		{
			if (Handle)
			{
				Plugin::DereferenceManagedClass(Handle);
			}
			Handle = other.Handle;
			other.Handle = 0;
			return *this;
		}
		
		bool AnotherScript::operator==(const AnotherScript& other) const
		{
			return Handle == other.Handle;
		}
		
		bool AnotherScript::operator!=(const AnotherScript& other) const
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
	Array1<int32_t>::Array1(decltype(nullptr))
		: System::ICloneable(nullptr)
		, System::Collections::IEnumerable(nullptr)
		, System::Collections::ICollection(nullptr)
		, System::Collections::IList(nullptr)
		, System::Array(nullptr)
		, System::Collections::Generic::IEnumerable<int32_t>(nullptr)
		, System::Collections::Generic::ICollection<int32_t>(nullptr)
		, System::Collections::Generic::IList<int32_t>(nullptr)
	{
		this->InternalLength = 0;
	}
	
	Array1<int32_t>::Array1(Plugin::InternalUse iu, int32_t handle)
		: System::ICloneable(nullptr)
		, System::Collections::IEnumerable(nullptr)
		, System::Collections::ICollection(nullptr)
		, System::Collections::IList(nullptr)
		, System::Array(nullptr)
		, System::Collections::Generic::IEnumerable<int32_t>(nullptr)
		, System::Collections::Generic::ICollection<int32_t>(nullptr)
		, System::Collections::Generic::IList<int32_t>(nullptr)
	{
		Handle = handle;
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
		this->InternalLength = 0;
	}
	
	Array1<int32_t>::Array1(const Array1<int32_t>& other)
		: Array1(Plugin::InternalUse::Only, other.Handle)
	{
		InternalLength = other.InternalLength;
	}
	
	Array1<int32_t>::Array1(Array1<int32_t>&& other)
		: Array1(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
		InternalLength = other.InternalLength;
		other.InternalLength = 0;
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
		return *this;
	}
	
	Array1<int32_t>& Array1<int32_t>::operator=(decltype(nullptr))
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
		other.Handle = 0;
		other.InternalLength = 0;
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
		: System::ICloneable(nullptr)
		, System::Collections::IEnumerable(nullptr)
		, System::Collections::ICollection(nullptr)
		, System::Collections::IList(nullptr)
		, System::Array(nullptr)
		, System::Collections::Generic::IEnumerable<int32_t>(nullptr)
		, System::Collections::Generic::ICollection<int32_t>(nullptr)
		, System::Collections::Generic::IList<int32_t>(nullptr)
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
		return 1;
	}
	
	Plugin::ArrayElementProxy1_1<int32_t> System::Array1<int32_t>::operator[](int32_t index)
	{
		return Plugin::ArrayElementProxy1_1<int32_t>(Plugin::InternalUse::Only, Handle, index);
	}
}

namespace Plugin
{
	SystemInt32Array1Iterator::SystemInt32Array1Iterator(System::Array1<int32_t>& array, int32_t index)
		: array(array)
		, index(index)
	{
	}
	
	SystemInt32Array1Iterator& SystemInt32Array1Iterator::operator++()
	{
		index++;
		return *this;
	}
	
	bool SystemInt32Array1Iterator::operator!=(const SystemInt32Array1Iterator& other)
	{
		return index != other.index;
	}
	
	int32_t SystemInt32Array1Iterator::operator*()
	{
		return array[index];
	}
}

namespace System
{
	Plugin::SystemInt32Array1Iterator begin(System::Array1<int32_t>& array)
	{
		return Plugin::SystemInt32Array1Iterator(array, 0);
	}
	
	Plugin::SystemInt32Array1Iterator end(System::Array1<int32_t>& array)
	{
		return Plugin::SystemInt32Array1Iterator(array, array.GetLength() - 1);
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
	Array1<float>::Array1(decltype(nullptr))
		: System::ICloneable(nullptr)
		, System::Collections::IEnumerable(nullptr)
		, System::Collections::ICollection(nullptr)
		, System::Collections::IList(nullptr)
		, System::Array(nullptr)
		, System::Collections::Generic::IEnumerable<float>(nullptr)
		, System::Collections::Generic::ICollection<float>(nullptr)
		, System::Collections::Generic::IList<float>(nullptr)
	{
		this->InternalLength = 0;
	}
	
	Array1<float>::Array1(Plugin::InternalUse iu, int32_t handle)
		: System::ICloneable(nullptr)
		, System::Collections::IEnumerable(nullptr)
		, System::Collections::ICollection(nullptr)
		, System::Collections::IList(nullptr)
		, System::Array(nullptr)
		, System::Collections::Generic::IEnumerable<float>(nullptr)
		, System::Collections::Generic::ICollection<float>(nullptr)
		, System::Collections::Generic::IList<float>(nullptr)
	{
		Handle = handle;
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
		this->InternalLength = 0;
	}
	
	Array1<float>::Array1(const Array1<float>& other)
		: Array1(Plugin::InternalUse::Only, other.Handle)
	{
		InternalLength = other.InternalLength;
	}
	
	Array1<float>::Array1(Array1<float>&& other)
		: Array1(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
		InternalLength = other.InternalLength;
		other.InternalLength = 0;
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
		return *this;
	}
	
	Array1<float>& Array1<float>::operator=(decltype(nullptr))
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
		other.Handle = 0;
		other.InternalLength = 0;
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
		: System::ICloneable(nullptr)
		, System::Collections::IEnumerable(nullptr)
		, System::Collections::ICollection(nullptr)
		, System::Collections::IList(nullptr)
		, System::Array(nullptr)
		, System::Collections::Generic::IEnumerable<float>(nullptr)
		, System::Collections::Generic::ICollection<float>(nullptr)
		, System::Collections::Generic::IList<float>(nullptr)
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
		return 1;
	}
	
	Plugin::ArrayElementProxy1_1<float> System::Array1<float>::operator[](int32_t index)
	{
		return Plugin::ArrayElementProxy1_1<float>(Plugin::InternalUse::Only, Handle, index);
	}
}

namespace Plugin
{
	SystemSingleArray1Iterator::SystemSingleArray1Iterator(System::Array1<float>& array, int32_t index)
		: array(array)
		, index(index)
	{
	}
	
	SystemSingleArray1Iterator& SystemSingleArray1Iterator::operator++()
	{
		index++;
		return *this;
	}
	
	bool SystemSingleArray1Iterator::operator!=(const SystemSingleArray1Iterator& other)
	{
		return index != other.index;
	}
	
	float SystemSingleArray1Iterator::operator*()
	{
		return array[index];
	}
}

namespace System
{
	Plugin::SystemSingleArray1Iterator begin(System::Array1<float>& array)
	{
		return Plugin::SystemSingleArray1Iterator(array, 0);
	}
	
	Plugin::SystemSingleArray1Iterator end(System::Array1<float>& array)
	{
		return Plugin::SystemSingleArray1Iterator(array, array.GetLength() - 1);
	}
}

namespace System
{
	Array2<float>::Array2(decltype(nullptr))
		: System::ICloneable(nullptr)
		, System::Collections::IEnumerable(nullptr)
		, System::Collections::ICollection(nullptr)
		, System::Collections::IList(nullptr)
		, System::Array(nullptr)
	{
		this->InternalLength = 0;
		this->InternalLengths[0] = 0;
		this->InternalLengths[1] = 0;
	}
	
	Array2<float>::Array2(Plugin::InternalUse iu, int32_t handle)
		: System::ICloneable(nullptr)
		, System::Collections::IEnumerable(nullptr)
		, System::Collections::ICollection(nullptr)
		, System::Collections::IList(nullptr)
		, System::Array(nullptr)
	{
		Handle = handle;
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
		this->InternalLength = 0;
		this->InternalLengths[0] = 0;
		this->InternalLengths[1] = 0;
	}
	
	Array2<float>::Array2(const Array2<float>& other)
		: Array2(Plugin::InternalUse::Only, other.Handle)
	{
		InternalLength = other.InternalLength;
		InternalLengths[0] = other.InternalLengths[0];
		InternalLengths[1] = other.InternalLengths[1];
	}
	
	Array2<float>::Array2(Array2<float>&& other)
		: Array2(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
		InternalLength = other.InternalLength;
		InternalLengths[0] = other.InternalLengths[0];
		InternalLengths[1] = other.InternalLengths[1];
		other.InternalLength = 0;
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
		InternalLengths[0] = other.InternalLengths[0];
		InternalLengths[1] = other.InternalLengths[1];
		return *this;
	}
	
	Array2<float>& Array2<float>::operator=(decltype(nullptr))
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
		InternalLengths[0] = other.InternalLengths[0];
		InternalLengths[1] = other.InternalLengths[1];
		other.Handle = 0;
		other.InternalLength = 0;
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
		: System::ICloneable(nullptr)
		, System::Collections::IEnumerable(nullptr)
		, System::Collections::ICollection(nullptr)
		, System::Collections::IList(nullptr)
		, System::Array(nullptr)
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
		assert(dimension >= 0 && dimension < 2);
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
		return 2;
	}
	
	Plugin::ArrayElementProxy1_2<float> System::Array2<float>::operator[](int32_t index)
	{
		return Plugin::ArrayElementProxy1_2<float>(Plugin::InternalUse::Only, Handle, index);
	}
}

namespace System
{
	Array3<float>::Array3(decltype(nullptr))
		: System::ICloneable(nullptr)
		, System::Collections::IEnumerable(nullptr)
		, System::Collections::ICollection(nullptr)
		, System::Collections::IList(nullptr)
		, System::Array(nullptr)
	{
		this->InternalLength = 0;
		this->InternalLengths[0] = 0;
		this->InternalLengths[1] = 0;
		this->InternalLengths[2] = 0;
	}
	
	Array3<float>::Array3(Plugin::InternalUse iu, int32_t handle)
		: System::ICloneable(nullptr)
		, System::Collections::IEnumerable(nullptr)
		, System::Collections::ICollection(nullptr)
		, System::Collections::IList(nullptr)
		, System::Array(nullptr)
	{
		Handle = handle;
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
		this->InternalLength = 0;
		this->InternalLengths[0] = 0;
		this->InternalLengths[1] = 0;
		this->InternalLengths[2] = 0;
	}
	
	Array3<float>::Array3(const Array3<float>& other)
		: Array3(Plugin::InternalUse::Only, other.Handle)
	{
		InternalLength = other.InternalLength;
		InternalLengths[0] = other.InternalLengths[0];
		InternalLengths[1] = other.InternalLengths[1];
		InternalLengths[2] = other.InternalLengths[2];
	}
	
	Array3<float>::Array3(Array3<float>&& other)
		: Array3(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
		InternalLength = other.InternalLength;
		InternalLengths[0] = other.InternalLengths[0];
		InternalLengths[1] = other.InternalLengths[1];
		InternalLengths[2] = other.InternalLengths[2];
		other.InternalLength = 0;
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
		InternalLengths[0] = other.InternalLengths[0];
		InternalLengths[1] = other.InternalLengths[1];
		InternalLengths[2] = other.InternalLengths[2];
		return *this;
	}
	
	Array3<float>& Array3<float>::operator=(decltype(nullptr))
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
		InternalLengths[0] = other.InternalLengths[0];
		InternalLengths[1] = other.InternalLengths[1];
		InternalLengths[2] = other.InternalLengths[2];
		other.Handle = 0;
		other.InternalLength = 0;
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
		: System::ICloneable(nullptr)
		, System::Collections::IEnumerable(nullptr)
		, System::Collections::ICollection(nullptr)
		, System::Collections::IList(nullptr)
		, System::Array(nullptr)
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
		assert(dimension >= 0 && dimension < 3);
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
		return 3;
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
	Array1<System::String>::Array1(decltype(nullptr))
		: System::ICloneable(nullptr)
		, System::Collections::IEnumerable(nullptr)
		, System::Collections::ICollection(nullptr)
		, System::Collections::IList(nullptr)
		, System::Array(nullptr)
		, System::Collections::Generic::IEnumerable<System::String>(nullptr)
		, System::Collections::Generic::ICollection<System::String>(nullptr)
		, System::Collections::Generic::IList<System::String>(nullptr)
	{
		this->InternalLength = 0;
	}
	
	Array1<System::String>::Array1(Plugin::InternalUse iu, int32_t handle)
		: System::ICloneable(nullptr)
		, System::Collections::IEnumerable(nullptr)
		, System::Collections::ICollection(nullptr)
		, System::Collections::IList(nullptr)
		, System::Array(nullptr)
		, System::Collections::Generic::IEnumerable<System::String>(nullptr)
		, System::Collections::Generic::ICollection<System::String>(nullptr)
		, System::Collections::Generic::IList<System::String>(nullptr)
	{
		Handle = handle;
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
		this->InternalLength = 0;
	}
	
	Array1<System::String>::Array1(const Array1<System::String>& other)
		: Array1(Plugin::InternalUse::Only, other.Handle)
	{
		InternalLength = other.InternalLength;
	}
	
	Array1<System::String>::Array1(Array1<System::String>&& other)
		: Array1(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
		InternalLength = other.InternalLength;
		other.InternalLength = 0;
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
		return *this;
	}
	
	Array1<System::String>& Array1<System::String>::operator=(decltype(nullptr))
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
		other.Handle = 0;
		other.InternalLength = 0;
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
		: System::ICloneable(nullptr)
		, System::Collections::IEnumerable(nullptr)
		, System::Collections::ICollection(nullptr)
		, System::Collections::IList(nullptr)
		, System::Array(nullptr)
		, System::Collections::Generic::IEnumerable<System::String>(nullptr)
		, System::Collections::Generic::ICollection<System::String>(nullptr)
		, System::Collections::Generic::IList<System::String>(nullptr)
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
		return 1;
	}
	
	Plugin::ArrayElementProxy1_1<System::String> System::Array1<System::String>::operator[](int32_t index)
	{
		return Plugin::ArrayElementProxy1_1<System::String>(Plugin::InternalUse::Only, Handle, index);
	}
}

namespace Plugin
{
	SystemStringArray1Iterator::SystemStringArray1Iterator(System::Array1<System::String>& array, int32_t index)
		: array(array)
		, index(index)
	{
	}
	
	SystemStringArray1Iterator& SystemStringArray1Iterator::operator++()
	{
		index++;
		return *this;
	}
	
	bool SystemStringArray1Iterator::operator!=(const SystemStringArray1Iterator& other)
	{
		return index != other.index;
	}
	
	System::String SystemStringArray1Iterator::operator*()
	{
		return array[index];
	}
}

namespace System
{
	Plugin::SystemStringArray1Iterator begin(System::Array1<System::String>& array)
	{
		return Plugin::SystemStringArray1Iterator(array, 0);
	}
	
	Plugin::SystemStringArray1Iterator end(System::Array1<System::String>& array)
	{
		return Plugin::SystemStringArray1Iterator(array, array.GetLength() - 1);
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
		Plugin::UnityEngineResolutionArray1SetItem1(Handle, Index0, item.Handle);
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
		return UnityEngine::Resolution(Plugin::InternalUse::Only, returnValue);
	}
}

namespace System
{
	Array1<UnityEngine::Resolution>::Array1(decltype(nullptr))
		: System::ICloneable(nullptr)
		, System::Collections::IEnumerable(nullptr)
		, System::Collections::ICollection(nullptr)
		, System::Collections::IList(nullptr)
		, System::Array(nullptr)
		, System::Collections::Generic::IEnumerable<UnityEngine::Resolution>(nullptr)
		, System::Collections::Generic::ICollection<UnityEngine::Resolution>(nullptr)
		, System::Collections::Generic::IList<UnityEngine::Resolution>(nullptr)
	{
		this->InternalLength = 0;
	}
	
	Array1<UnityEngine::Resolution>::Array1(Plugin::InternalUse iu, int32_t handle)
		: System::ICloneable(nullptr)
		, System::Collections::IEnumerable(nullptr)
		, System::Collections::ICollection(nullptr)
		, System::Collections::IList(nullptr)
		, System::Array(nullptr)
		, System::Collections::Generic::IEnumerable<UnityEngine::Resolution>(nullptr)
		, System::Collections::Generic::ICollection<UnityEngine::Resolution>(nullptr)
		, System::Collections::Generic::IList<UnityEngine::Resolution>(nullptr)
	{
		Handle = handle;
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
		this->InternalLength = 0;
	}
	
	Array1<UnityEngine::Resolution>::Array1(const Array1<UnityEngine::Resolution>& other)
		: Array1(Plugin::InternalUse::Only, other.Handle)
	{
		InternalLength = other.InternalLength;
	}
	
	Array1<UnityEngine::Resolution>::Array1(Array1<UnityEngine::Resolution>&& other)
		: Array1(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
		InternalLength = other.InternalLength;
		other.InternalLength = 0;
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
		return *this;
	}
	
	Array1<UnityEngine::Resolution>& Array1<UnityEngine::Resolution>::operator=(decltype(nullptr))
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
		other.Handle = 0;
		other.InternalLength = 0;
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
		: System::ICloneable(nullptr)
		, System::Collections::IEnumerable(nullptr)
		, System::Collections::ICollection(nullptr)
		, System::Collections::IList(nullptr)
		, System::Array(nullptr)
		, System::Collections::Generic::IEnumerable<UnityEngine::Resolution>(nullptr)
		, System::Collections::Generic::ICollection<UnityEngine::Resolution>(nullptr)
		, System::Collections::Generic::IList<UnityEngine::Resolution>(nullptr)
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
		return 1;
	}
	
	Plugin::ArrayElementProxy1_1<UnityEngine::Resolution> System::Array1<UnityEngine::Resolution>::operator[](int32_t index)
	{
		return Plugin::ArrayElementProxy1_1<UnityEngine::Resolution>(Plugin::InternalUse::Only, Handle, index);
	}
}

namespace Plugin
{
	UnityEngineResolutionArray1Iterator::UnityEngineResolutionArray1Iterator(System::Array1<UnityEngine::Resolution>& array, int32_t index)
		: array(array)
		, index(index)
	{
	}
	
	UnityEngineResolutionArray1Iterator& UnityEngineResolutionArray1Iterator::operator++()
	{
		index++;
		return *this;
	}
	
	bool UnityEngineResolutionArray1Iterator::operator!=(const UnityEngineResolutionArray1Iterator& other)
	{
		return index != other.index;
	}
	
	UnityEngine::Resolution UnityEngineResolutionArray1Iterator::operator*()
	{
		return array[index];
	}
}

namespace System
{
	Plugin::UnityEngineResolutionArray1Iterator begin(System::Array1<UnityEngine::Resolution>& array)
	{
		return Plugin::UnityEngineResolutionArray1Iterator(array, 0);
	}
	
	Plugin::UnityEngineResolutionArray1Iterator end(System::Array1<UnityEngine::Resolution>& array)
	{
		return Plugin::UnityEngineResolutionArray1Iterator(array, array.GetLength() - 1);
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
	Array1<UnityEngine::RaycastHit>::Array1(decltype(nullptr))
		: System::ICloneable(nullptr)
		, System::Collections::IEnumerable(nullptr)
		, System::Collections::ICollection(nullptr)
		, System::Collections::IList(nullptr)
		, System::Array(nullptr)
		, System::Collections::Generic::IEnumerable<UnityEngine::RaycastHit>(nullptr)
		, System::Collections::Generic::ICollection<UnityEngine::RaycastHit>(nullptr)
		, System::Collections::Generic::IList<UnityEngine::RaycastHit>(nullptr)
	{
		this->InternalLength = 0;
	}
	
	Array1<UnityEngine::RaycastHit>::Array1(Plugin::InternalUse iu, int32_t handle)
		: System::ICloneable(nullptr)
		, System::Collections::IEnumerable(nullptr)
		, System::Collections::ICollection(nullptr)
		, System::Collections::IList(nullptr)
		, System::Array(nullptr)
		, System::Collections::Generic::IEnumerable<UnityEngine::RaycastHit>(nullptr)
		, System::Collections::Generic::ICollection<UnityEngine::RaycastHit>(nullptr)
		, System::Collections::Generic::IList<UnityEngine::RaycastHit>(nullptr)
	{
		Handle = handle;
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
		this->InternalLength = 0;
	}
	
	Array1<UnityEngine::RaycastHit>::Array1(const Array1<UnityEngine::RaycastHit>& other)
		: Array1(Plugin::InternalUse::Only, other.Handle)
	{
		InternalLength = other.InternalLength;
	}
	
	Array1<UnityEngine::RaycastHit>::Array1(Array1<UnityEngine::RaycastHit>&& other)
		: Array1(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
		InternalLength = other.InternalLength;
		other.InternalLength = 0;
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
		return *this;
	}
	
	Array1<UnityEngine::RaycastHit>& Array1<UnityEngine::RaycastHit>::operator=(decltype(nullptr))
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
		other.Handle = 0;
		other.InternalLength = 0;
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
		: System::ICloneable(nullptr)
		, System::Collections::IEnumerable(nullptr)
		, System::Collections::ICollection(nullptr)
		, System::Collections::IList(nullptr)
		, System::Array(nullptr)
		, System::Collections::Generic::IEnumerable<UnityEngine::RaycastHit>(nullptr)
		, System::Collections::Generic::ICollection<UnityEngine::RaycastHit>(nullptr)
		, System::Collections::Generic::IList<UnityEngine::RaycastHit>(nullptr)
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
		return 1;
	}
	
	Plugin::ArrayElementProxy1_1<UnityEngine::RaycastHit> System::Array1<UnityEngine::RaycastHit>::operator[](int32_t index)
	{
		return Plugin::ArrayElementProxy1_1<UnityEngine::RaycastHit>(Plugin::InternalUse::Only, Handle, index);
	}
}

namespace Plugin
{
	UnityEngineRaycastHitArray1Iterator::UnityEngineRaycastHitArray1Iterator(System::Array1<UnityEngine::RaycastHit>& array, int32_t index)
		: array(array)
		, index(index)
	{
	}
	
	UnityEngineRaycastHitArray1Iterator& UnityEngineRaycastHitArray1Iterator::operator++()
	{
		index++;
		return *this;
	}
	
	bool UnityEngineRaycastHitArray1Iterator::operator!=(const UnityEngineRaycastHitArray1Iterator& other)
	{
		return index != other.index;
	}
	
	UnityEngine::RaycastHit UnityEngineRaycastHitArray1Iterator::operator*()
	{
		return array[index];
	}
}

namespace System
{
	Plugin::UnityEngineRaycastHitArray1Iterator begin(System::Array1<UnityEngine::RaycastHit>& array)
	{
		return Plugin::UnityEngineRaycastHitArray1Iterator(array, 0);
	}
	
	Plugin::UnityEngineRaycastHitArray1Iterator end(System::Array1<UnityEngine::RaycastHit>& array)
	{
		return Plugin::UnityEngineRaycastHitArray1Iterator(array, array.GetLength() - 1);
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
	Array1<UnityEngine::GradientColorKey>::Array1(decltype(nullptr))
		: System::ICloneable(nullptr)
		, System::Collections::IEnumerable(nullptr)
		, System::Collections::ICollection(nullptr)
		, System::Collections::IList(nullptr)
		, System::Array(nullptr)
		, System::Collections::Generic::IEnumerable<UnityEngine::GradientColorKey>(nullptr)
		, System::Collections::Generic::ICollection<UnityEngine::GradientColorKey>(nullptr)
		, System::Collections::Generic::IList<UnityEngine::GradientColorKey>(nullptr)
	{
		this->InternalLength = 0;
	}
	
	Array1<UnityEngine::GradientColorKey>::Array1(Plugin::InternalUse iu, int32_t handle)
		: System::ICloneable(nullptr)
		, System::Collections::IEnumerable(nullptr)
		, System::Collections::ICollection(nullptr)
		, System::Collections::IList(nullptr)
		, System::Array(nullptr)
		, System::Collections::Generic::IEnumerable<UnityEngine::GradientColorKey>(nullptr)
		, System::Collections::Generic::ICollection<UnityEngine::GradientColorKey>(nullptr)
		, System::Collections::Generic::IList<UnityEngine::GradientColorKey>(nullptr)
	{
		Handle = handle;
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
		this->InternalLength = 0;
	}
	
	Array1<UnityEngine::GradientColorKey>::Array1(const Array1<UnityEngine::GradientColorKey>& other)
		: Array1(Plugin::InternalUse::Only, other.Handle)
	{
		InternalLength = other.InternalLength;
	}
	
	Array1<UnityEngine::GradientColorKey>::Array1(Array1<UnityEngine::GradientColorKey>&& other)
		: Array1(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
		InternalLength = other.InternalLength;
		other.InternalLength = 0;
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
		return *this;
	}
	
	Array1<UnityEngine::GradientColorKey>& Array1<UnityEngine::GradientColorKey>::operator=(decltype(nullptr))
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
		other.Handle = 0;
		other.InternalLength = 0;
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
		: System::ICloneable(nullptr)
		, System::Collections::IEnumerable(nullptr)
		, System::Collections::ICollection(nullptr)
		, System::Collections::IList(nullptr)
		, System::Array(nullptr)
		, System::Collections::Generic::IEnumerable<UnityEngine::GradientColorKey>(nullptr)
		, System::Collections::Generic::ICollection<UnityEngine::GradientColorKey>(nullptr)
		, System::Collections::Generic::IList<UnityEngine::GradientColorKey>(nullptr)
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
		return 1;
	}
	
	Plugin::ArrayElementProxy1_1<UnityEngine::GradientColorKey> System::Array1<UnityEngine::GradientColorKey>::operator[](int32_t index)
	{
		return Plugin::ArrayElementProxy1_1<UnityEngine::GradientColorKey>(Plugin::InternalUse::Only, Handle, index);
	}
}

namespace Plugin
{
	UnityEngineGradientColorKeyArray1Iterator::UnityEngineGradientColorKeyArray1Iterator(System::Array1<UnityEngine::GradientColorKey>& array, int32_t index)
		: array(array)
		, index(index)
	{
	}
	
	UnityEngineGradientColorKeyArray1Iterator& UnityEngineGradientColorKeyArray1Iterator::operator++()
	{
		index++;
		return *this;
	}
	
	bool UnityEngineGradientColorKeyArray1Iterator::operator!=(const UnityEngineGradientColorKeyArray1Iterator& other)
	{
		return index != other.index;
	}
	
	UnityEngine::GradientColorKey UnityEngineGradientColorKeyArray1Iterator::operator*()
	{
		return array[index];
	}
}

namespace System
{
	Plugin::UnityEngineGradientColorKeyArray1Iterator begin(System::Array1<UnityEngine::GradientColorKey>& array)
	{
		return Plugin::UnityEngineGradientColorKeyArray1Iterator(array, 0);
	}
	
	Plugin::UnityEngineGradientColorKeyArray1Iterator end(System::Array1<UnityEngine::GradientColorKey>& array)
	{
		return Plugin::UnityEngineGradientColorKeyArray1Iterator(array, array.GetLength() - 1);
	}
}

namespace System
{
	Action::Action()
	{
		CppHandle = Plugin::StoreSystemAction(this);
		int32_t* handle = &Handle;
		int32_t cppHandle = CppHandle;
		int32_t* classHandle = &ClassHandle;
		Plugin::SystemActionConstructor(cppHandle, handle, classHandle);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
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
	
	Action::Action(decltype(nullptr))
	{
		CppHandle = Plugin::StoreSystemAction(this);
		ClassHandle = 0;
	}
	
	Action::Action(const Action& other)
	{
		Handle = other.Handle;
		CppHandle = Plugin::StoreSystemAction(this);
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
		ClassHandle = other.ClassHandle;
	}
	
	Action::Action(Action&& other)
	{
		Handle = other.Handle;
		CppHandle = other.CppHandle;
		ClassHandle = other.ClassHandle;
		other.Handle = 0;
		other.CppHandle = 0;
		other.ClassHandle = 0;
	}
	
	Action::Action(Plugin::InternalUse iu, int32_t handle)
	{
		Handle = handle;
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
	
	Action& Action::operator=(decltype(nullptr))
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
	{
		CppHandle = Plugin::StoreSystemActionSystemSingle(this);
		int32_t* handle = &Handle;
		int32_t cppHandle = CppHandle;
		int32_t* classHandle = &ClassHandle;
		Plugin::SystemActionSystemSingleConstructor(cppHandle, handle, classHandle);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
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
	
	Action1<float>::Action1(decltype(nullptr))
	{
		CppHandle = Plugin::StoreSystemActionSystemSingle(this);
		ClassHandle = 0;
	}
	
	Action1<float>::Action1(const Action1<float>& other)
	{
		Handle = other.Handle;
		CppHandle = Plugin::StoreSystemActionSystemSingle(this);
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
		ClassHandle = other.ClassHandle;
	}
	
	Action1<float>::Action1(Action1<float>&& other)
	{
		Handle = other.Handle;
		CppHandle = other.CppHandle;
		ClassHandle = other.ClassHandle;
		other.Handle = 0;
		other.CppHandle = 0;
		other.ClassHandle = 0;
	}
	
	Action1<float>::Action1(Plugin::InternalUse iu, int32_t handle)
	{
		Handle = handle;
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
	
	Action1<float>& Action1<float>::operator=(decltype(nullptr))
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
	{
		CppHandle = Plugin::StoreSystemActionSystemSingle_SystemSingle(this);
		int32_t* handle = &Handle;
		int32_t cppHandle = CppHandle;
		int32_t* classHandle = &ClassHandle;
		Plugin::SystemActionSystemSingle_SystemSingleConstructor(cppHandle, handle, classHandle);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
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
	
	Action2<float, float>::Action2(decltype(nullptr))
	{
		CppHandle = Plugin::StoreSystemActionSystemSingle_SystemSingle(this);
		ClassHandle = 0;
	}
	
	Action2<float, float>::Action2(const Action2<float, float>& other)
	{
		Handle = other.Handle;
		CppHandle = Plugin::StoreSystemActionSystemSingle_SystemSingle(this);
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
		ClassHandle = other.ClassHandle;
	}
	
	Action2<float, float>::Action2(Action2<float, float>&& other)
	{
		Handle = other.Handle;
		CppHandle = other.CppHandle;
		ClassHandle = other.ClassHandle;
		other.Handle = 0;
		other.CppHandle = 0;
		other.ClassHandle = 0;
	}
	
	Action2<float, float>::Action2(Plugin::InternalUse iu, int32_t handle)
	{
		Handle = handle;
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
	
	Action2<float, float>& Action2<float, float>::operator=(decltype(nullptr))
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
	{
		CppHandle = Plugin::StoreSystemFuncSystemInt32_SystemSingle_SystemDouble(this);
		int32_t* handle = &Handle;
		int32_t cppHandle = CppHandle;
		int32_t* classHandle = &ClassHandle;
		Plugin::SystemFuncSystemInt32_SystemSingle_SystemDoubleConstructor(cppHandle, handle, classHandle);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
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
	
	Func3<int32_t, float, double>::Func3(decltype(nullptr))
	{
		CppHandle = Plugin::StoreSystemFuncSystemInt32_SystemSingle_SystemDouble(this);
		ClassHandle = 0;
	}
	
	Func3<int32_t, float, double>::Func3(const Func3<int32_t, float, double>& other)
	{
		Handle = other.Handle;
		CppHandle = Plugin::StoreSystemFuncSystemInt32_SystemSingle_SystemDouble(this);
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
		ClassHandle = other.ClassHandle;
	}
	
	Func3<int32_t, float, double>::Func3(Func3<int32_t, float, double>&& other)
	{
		Handle = other.Handle;
		CppHandle = other.CppHandle;
		ClassHandle = other.ClassHandle;
		other.Handle = 0;
		other.CppHandle = 0;
		other.ClassHandle = 0;
	}
	
	Func3<int32_t, float, double>::Func3(Plugin::InternalUse iu, int32_t handle)
	{
		Handle = handle;
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
	
	Func3<int32_t, float, double>& Func3<int32_t, float, double>::operator=(decltype(nullptr))
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
	{
		CppHandle = Plugin::StoreSystemFuncSystemInt16_SystemInt32_SystemString(this);
		int32_t* handle = &Handle;
		int32_t cppHandle = CppHandle;
		int32_t* classHandle = &ClassHandle;
		Plugin::SystemFuncSystemInt16_SystemInt32_SystemStringConstructor(cppHandle, handle, classHandle);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
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
	
	Func3<int16_t, int32_t, System::String>::Func3(decltype(nullptr))
	{
		CppHandle = Plugin::StoreSystemFuncSystemInt16_SystemInt32_SystemString(this);
		ClassHandle = 0;
	}
	
	Func3<int16_t, int32_t, System::String>::Func3(const Func3<int16_t, int32_t, System::String>& other)
	{
		Handle = other.Handle;
		CppHandle = Plugin::StoreSystemFuncSystemInt16_SystemInt32_SystemString(this);
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
		ClassHandle = other.ClassHandle;
	}
	
	Func3<int16_t, int32_t, System::String>::Func3(Func3<int16_t, int32_t, System::String>&& other)
	{
		Handle = other.Handle;
		CppHandle = other.CppHandle;
		ClassHandle = other.ClassHandle;
		other.Handle = 0;
		other.CppHandle = 0;
		other.ClassHandle = 0;
	}
	
	Func3<int16_t, int32_t, System::String>::Func3(Plugin::InternalUse iu, int32_t handle)
	{
		Handle = handle;
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
	
	Func3<int16_t, int32_t, System::String>& Func3<int16_t, int32_t, System::String>::operator=(decltype(nullptr))
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
		return nullptr;
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
	{
		CppHandle = Plugin::StoreSystemAppDomainInitializer(this);
		int32_t* handle = &Handle;
		int32_t cppHandle = CppHandle;
		int32_t* classHandle = &ClassHandle;
		Plugin::SystemAppDomainInitializerConstructor(cppHandle, handle, classHandle);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
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
	
	AppDomainInitializer::AppDomainInitializer(decltype(nullptr))
	{
		CppHandle = Plugin::StoreSystemAppDomainInitializer(this);
		ClassHandle = 0;
	}
	
	AppDomainInitializer::AppDomainInitializer(const AppDomainInitializer& other)
	{
		Handle = other.Handle;
		CppHandle = Plugin::StoreSystemAppDomainInitializer(this);
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
		ClassHandle = other.ClassHandle;
	}
	
	AppDomainInitializer::AppDomainInitializer(AppDomainInitializer&& other)
	{
		Handle = other.Handle;
		CppHandle = other.CppHandle;
		ClassHandle = other.ClassHandle;
		other.Handle = 0;
		other.CppHandle = 0;
		other.ClassHandle = 0;
	}
	
	AppDomainInitializer::AppDomainInitializer(Plugin::InternalUse iu, int32_t handle)
	{
		Handle = handle;
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
	
	AppDomainInitializer& AppDomainInitializer::operator=(decltype(nullptr))
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
	
	void AppDomainInitializer::operator()(System::Array1<System::String>& args)
	{
	}
	
	DLLEXPORT void SystemAppDomainInitializerNativeInvoke(int32_t cppHandle, int32_t argsHandle)
	{
		try
		{
			auto args = System::Array1<System::String>(Plugin::InternalUse::Only, argsHandle);
			Plugin::GetSystemAppDomainInitializer(cppHandle)->operator()(args);
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
	
	void AppDomainInitializer::Invoke(System::Array1<System::String>& args)
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
		{
			CppHandle = Plugin::StoreUnityEngineEventsUnityAction(this);
			int32_t* handle = &Handle;
			int32_t cppHandle = CppHandle;
			int32_t* classHandle = &ClassHandle;
			Plugin::UnityEngineEventsUnityActionConstructor(cppHandle, handle, classHandle);
			if (Plugin::unhandledCsharpException)
			{
				System::Exception* ex = Plugin::unhandledCsharpException;
				Plugin::unhandledCsharpException = nullptr;
				ex->ThrowReferenceToThis();
				delete ex;
			}
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
		
		UnityAction::UnityAction(decltype(nullptr))
		{
			CppHandle = Plugin::StoreUnityEngineEventsUnityAction(this);
			ClassHandle = 0;
		}
		
		UnityAction::UnityAction(const UnityAction& other)
		{
			Handle = other.Handle;
			CppHandle = Plugin::StoreUnityEngineEventsUnityAction(this);
			if (Handle)
			{
				Plugin::ReferenceManagedClass(Handle);
			}
			ClassHandle = other.ClassHandle;
		}
		
		UnityAction::UnityAction(UnityAction&& other)
		{
			Handle = other.Handle;
			CppHandle = other.CppHandle;
			ClassHandle = other.ClassHandle;
			other.Handle = 0;
			other.CppHandle = 0;
			other.ClassHandle = 0;
		}
		
		UnityAction::UnityAction(Plugin::InternalUse iu, int32_t handle)
		{
			Handle = handle;
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
		
		UnityAction& UnityAction::operator=(decltype(nullptr))
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
		{
			CppHandle = Plugin::StoreUnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneMode(this);
			int32_t* handle = &Handle;
			int32_t cppHandle = CppHandle;
			int32_t* classHandle = &ClassHandle;
			Plugin::UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeConstructor(cppHandle, handle, classHandle);
			if (Plugin::unhandledCsharpException)
			{
				System::Exception* ex = Plugin::unhandledCsharpException;
				Plugin::unhandledCsharpException = nullptr;
				ex->ThrowReferenceToThis();
				delete ex;
			}
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
		
		UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>::UnityAction2(decltype(nullptr))
		{
			CppHandle = Plugin::StoreUnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneMode(this);
			ClassHandle = 0;
		}
		
		UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>::UnityAction2(const UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>& other)
		{
			Handle = other.Handle;
			CppHandle = Plugin::StoreUnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneMode(this);
			if (Handle)
			{
				Plugin::ReferenceManagedClass(Handle);
			}
			ClassHandle = other.ClassHandle;
		}
		
		UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>::UnityAction2(UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>&& other)
		{
			Handle = other.Handle;
			CppHandle = other.CppHandle;
			ClassHandle = other.ClassHandle;
			other.Handle = 0;
			other.CppHandle = 0;
			other.ClassHandle = 0;
		}
		
		UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>::UnityAction2(Plugin::InternalUse iu, int32_t handle)
		{
			Handle = handle;
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
		
		UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>& UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>::operator=(decltype(nullptr))
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
		
		DLLEXPORT void UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeNativeInvoke(int32_t cppHandle, int32_t arg0Handle, UnityEngine::SceneManagement::LoadSceneMode arg1)
		{
			try
			{
				auto arg0 = UnityEngine::SceneManagement::Scene(Plugin::InternalUse::Only, arg0Handle);
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
			Plugin::UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeInvoke(Handle, arg0.Handle, arg1);
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
	namespace ComponentModel
	{
		namespace Design
		{
			ComponentEventHandler::ComponentEventHandler()
			{
				CppHandle = Plugin::StoreSystemComponentModelDesignComponentEventHandler(this);
				int32_t* handle = &Handle;
				int32_t cppHandle = CppHandle;
				int32_t* classHandle = &ClassHandle;
				Plugin::SystemComponentModelDesignComponentEventHandlerConstructor(cppHandle, handle, classHandle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
				if (Handle)
				{
					Plugin::ReferenceManagedClass(Handle);
				}
				else
				{
					Plugin::RemoveSystemComponentModelDesignComponentEventHandler(CppHandle);
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
			
			ComponentEventHandler::ComponentEventHandler(decltype(nullptr))
			{
				CppHandle = Plugin::StoreSystemComponentModelDesignComponentEventHandler(this);
				ClassHandle = 0;
			}
			
			ComponentEventHandler::ComponentEventHandler(const ComponentEventHandler& other)
			{
				Handle = other.Handle;
				CppHandle = Plugin::StoreSystemComponentModelDesignComponentEventHandler(this);
				if (Handle)
				{
					Plugin::ReferenceManagedClass(Handle);
				}
				ClassHandle = other.ClassHandle;
			}
			
			ComponentEventHandler::ComponentEventHandler(ComponentEventHandler&& other)
			{
				Handle = other.Handle;
				CppHandle = other.CppHandle;
				ClassHandle = other.ClassHandle;
				other.Handle = 0;
				other.CppHandle = 0;
				other.ClassHandle = 0;
			}
			
			ComponentEventHandler::ComponentEventHandler(Plugin::InternalUse iu, int32_t handle)
			{
				Handle = handle;
				CppHandle = Plugin::StoreSystemComponentModelDesignComponentEventHandler(this);
				if (Handle)
				{
					Plugin::ReferenceManagedClass(Handle);
				}
				ClassHandle = 0;
			}
			
			ComponentEventHandler::~ComponentEventHandler()
			{
				Plugin::RemoveSystemComponentModelDesignComponentEventHandler(CppHandle);
				CppHandle = 0;
				if (Handle)
				{
					int32_t handle = Handle;
					int32_t classHandle = ClassHandle;
					Handle = 0;
					ClassHandle = 0;
					if (Plugin::DereferenceManagedClassNoRelease(handle))
					{
						Plugin::ReleaseSystemComponentModelDesignComponentEventHandler(handle, classHandle);
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
			
			ComponentEventHandler& ComponentEventHandler::operator=(const ComponentEventHandler& other)
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
			
			ComponentEventHandler& ComponentEventHandler::operator=(decltype(nullptr))
			{
				if (Handle)
				{
					int32_t handle = Handle;
					int32_t classHandle = ClassHandle;
					Handle = 0;
					ClassHandle = 0;
					if (Plugin::DereferenceManagedClassNoRelease(handle))
					{
						Plugin::ReleaseSystemComponentModelDesignComponentEventHandler(handle, classHandle);
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
			
			ComponentEventHandler& ComponentEventHandler::operator=(ComponentEventHandler&& other)
			{
				Plugin::RemoveSystemComponentModelDesignComponentEventHandler(CppHandle);
				CppHandle = 0;
				if (Handle)
				{
					int32_t handle = Handle;
					int32_t classHandle = ClassHandle;
					Handle = 0;
					ClassHandle = 0;
					if (Plugin::DereferenceManagedClassNoRelease(handle))
					{
						Plugin::ReleaseSystemComponentModelDesignComponentEventHandler(handle, classHandle);
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
			
			bool ComponentEventHandler::operator==(const ComponentEventHandler& other) const
			{
				return Handle == other.Handle;
			}
			
			bool ComponentEventHandler::operator!=(const ComponentEventHandler& other) const
			{
				return Handle != other.Handle;
			}
			
			void ComponentEventHandler::operator+=(System::ComponentModel::Design::ComponentEventHandler& del)
			{
				Plugin::SystemComponentModelDesignComponentEventHandlerAdd(Handle, del.Handle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
			}
			
			void ComponentEventHandler::operator-=(System::ComponentModel::Design::ComponentEventHandler& del)
			{
				Plugin::SystemComponentModelDesignComponentEventHandlerRemove(Handle, del.Handle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
			}
			
			void ComponentEventHandler::operator()(System::Object& sender, System::ComponentModel::Design::ComponentEventArgs& e)
			{
			}
			
			DLLEXPORT void SystemComponentModelDesignComponentEventHandlerNativeInvoke(int32_t cppHandle, int32_t senderHandle, int32_t eHandle)
			{
				try
				{
					auto sender = System::Object(Plugin::InternalUse::Only, senderHandle);
					auto e = System::ComponentModel::Design::ComponentEventArgs(Plugin::InternalUse::Only, eHandle);
					Plugin::GetSystemComponentModelDesignComponentEventHandler(cppHandle)->operator()(sender, e);
				}
				catch (System::Exception ex)
				{
					Plugin::SetException(ex.Handle);
				}
				catch (...)
				{
					System::String msg = "Unhandled exception invoking System::ComponentModel::Design::ComponentEventHandler";
					System::Exception ex(msg);
					Plugin::SetException(ex.Handle);
				}
			}
			
			void ComponentEventHandler::Invoke(System::Object& sender, System::ComponentModel::Design::ComponentEventArgs& e)
			{
				Plugin::SystemComponentModelDesignComponentEventHandlerInvoke(Handle, sender.Handle, e.Handle);
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
	namespace ComponentModel
	{
		namespace Design
		{
			ComponentChangingEventHandler::ComponentChangingEventHandler()
			{
				CppHandle = Plugin::StoreSystemComponentModelDesignComponentChangingEventHandler(this);
				int32_t* handle = &Handle;
				int32_t cppHandle = CppHandle;
				int32_t* classHandle = &ClassHandle;
				Plugin::SystemComponentModelDesignComponentChangingEventHandlerConstructor(cppHandle, handle, classHandle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
				if (Handle)
				{
					Plugin::ReferenceManagedClass(Handle);
				}
				else
				{
					Plugin::RemoveSystemComponentModelDesignComponentChangingEventHandler(CppHandle);
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
			
			ComponentChangingEventHandler::ComponentChangingEventHandler(decltype(nullptr))
			{
				CppHandle = Plugin::StoreSystemComponentModelDesignComponentChangingEventHandler(this);
				ClassHandle = 0;
			}
			
			ComponentChangingEventHandler::ComponentChangingEventHandler(const ComponentChangingEventHandler& other)
			{
				Handle = other.Handle;
				CppHandle = Plugin::StoreSystemComponentModelDesignComponentChangingEventHandler(this);
				if (Handle)
				{
					Plugin::ReferenceManagedClass(Handle);
				}
				ClassHandle = other.ClassHandle;
			}
			
			ComponentChangingEventHandler::ComponentChangingEventHandler(ComponentChangingEventHandler&& other)
			{
				Handle = other.Handle;
				CppHandle = other.CppHandle;
				ClassHandle = other.ClassHandle;
				other.Handle = 0;
				other.CppHandle = 0;
				other.ClassHandle = 0;
			}
			
			ComponentChangingEventHandler::ComponentChangingEventHandler(Plugin::InternalUse iu, int32_t handle)
			{
				Handle = handle;
				CppHandle = Plugin::StoreSystemComponentModelDesignComponentChangingEventHandler(this);
				if (Handle)
				{
					Plugin::ReferenceManagedClass(Handle);
				}
				ClassHandle = 0;
			}
			
			ComponentChangingEventHandler::~ComponentChangingEventHandler()
			{
				Plugin::RemoveSystemComponentModelDesignComponentChangingEventHandler(CppHandle);
				CppHandle = 0;
				if (Handle)
				{
					int32_t handle = Handle;
					int32_t classHandle = ClassHandle;
					Handle = 0;
					ClassHandle = 0;
					if (Plugin::DereferenceManagedClassNoRelease(handle))
					{
						Plugin::ReleaseSystemComponentModelDesignComponentChangingEventHandler(handle, classHandle);
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
			
			ComponentChangingEventHandler& ComponentChangingEventHandler::operator=(const ComponentChangingEventHandler& other)
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
			
			ComponentChangingEventHandler& ComponentChangingEventHandler::operator=(decltype(nullptr))
			{
				if (Handle)
				{
					int32_t handle = Handle;
					int32_t classHandle = ClassHandle;
					Handle = 0;
					ClassHandle = 0;
					if (Plugin::DereferenceManagedClassNoRelease(handle))
					{
						Plugin::ReleaseSystemComponentModelDesignComponentChangingEventHandler(handle, classHandle);
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
			
			ComponentChangingEventHandler& ComponentChangingEventHandler::operator=(ComponentChangingEventHandler&& other)
			{
				Plugin::RemoveSystemComponentModelDesignComponentChangingEventHandler(CppHandle);
				CppHandle = 0;
				if (Handle)
				{
					int32_t handle = Handle;
					int32_t classHandle = ClassHandle;
					Handle = 0;
					ClassHandle = 0;
					if (Plugin::DereferenceManagedClassNoRelease(handle))
					{
						Plugin::ReleaseSystemComponentModelDesignComponentChangingEventHandler(handle, classHandle);
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
			
			bool ComponentChangingEventHandler::operator==(const ComponentChangingEventHandler& other) const
			{
				return Handle == other.Handle;
			}
			
			bool ComponentChangingEventHandler::operator!=(const ComponentChangingEventHandler& other) const
			{
				return Handle != other.Handle;
			}
			
			void ComponentChangingEventHandler::operator+=(System::ComponentModel::Design::ComponentChangingEventHandler& del)
			{
				Plugin::SystemComponentModelDesignComponentChangingEventHandlerAdd(Handle, del.Handle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
			}
			
			void ComponentChangingEventHandler::operator-=(System::ComponentModel::Design::ComponentChangingEventHandler& del)
			{
				Plugin::SystemComponentModelDesignComponentChangingEventHandlerRemove(Handle, del.Handle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
			}
			
			void ComponentChangingEventHandler::operator()(System::Object& sender, System::ComponentModel::Design::ComponentChangingEventArgs& e)
			{
			}
			
			DLLEXPORT void SystemComponentModelDesignComponentChangingEventHandlerNativeInvoke(int32_t cppHandle, int32_t senderHandle, int32_t eHandle)
			{
				try
				{
					auto sender = System::Object(Plugin::InternalUse::Only, senderHandle);
					auto e = System::ComponentModel::Design::ComponentChangingEventArgs(Plugin::InternalUse::Only, eHandle);
					Plugin::GetSystemComponentModelDesignComponentChangingEventHandler(cppHandle)->operator()(sender, e);
				}
				catch (System::Exception ex)
				{
					Plugin::SetException(ex.Handle);
				}
				catch (...)
				{
					System::String msg = "Unhandled exception invoking System::ComponentModel::Design::ComponentChangingEventHandler";
					System::Exception ex(msg);
					Plugin::SetException(ex.Handle);
				}
			}
			
			void ComponentChangingEventHandler::Invoke(System::Object& sender, System::ComponentModel::Design::ComponentChangingEventArgs& e)
			{
				Plugin::SystemComponentModelDesignComponentChangingEventHandlerInvoke(Handle, sender.Handle, e.Handle);
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
	namespace ComponentModel
	{
		namespace Design
		{
			ComponentChangedEventHandler::ComponentChangedEventHandler()
			{
				CppHandle = Plugin::StoreSystemComponentModelDesignComponentChangedEventHandler(this);
				int32_t* handle = &Handle;
				int32_t cppHandle = CppHandle;
				int32_t* classHandle = &ClassHandle;
				Plugin::SystemComponentModelDesignComponentChangedEventHandlerConstructor(cppHandle, handle, classHandle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
				if (Handle)
				{
					Plugin::ReferenceManagedClass(Handle);
				}
				else
				{
					Plugin::RemoveSystemComponentModelDesignComponentChangedEventHandler(CppHandle);
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
			
			ComponentChangedEventHandler::ComponentChangedEventHandler(decltype(nullptr))
			{
				CppHandle = Plugin::StoreSystemComponentModelDesignComponentChangedEventHandler(this);
				ClassHandle = 0;
			}
			
			ComponentChangedEventHandler::ComponentChangedEventHandler(const ComponentChangedEventHandler& other)
			{
				Handle = other.Handle;
				CppHandle = Plugin::StoreSystemComponentModelDesignComponentChangedEventHandler(this);
				if (Handle)
				{
					Plugin::ReferenceManagedClass(Handle);
				}
				ClassHandle = other.ClassHandle;
			}
			
			ComponentChangedEventHandler::ComponentChangedEventHandler(ComponentChangedEventHandler&& other)
			{
				Handle = other.Handle;
				CppHandle = other.CppHandle;
				ClassHandle = other.ClassHandle;
				other.Handle = 0;
				other.CppHandle = 0;
				other.ClassHandle = 0;
			}
			
			ComponentChangedEventHandler::ComponentChangedEventHandler(Plugin::InternalUse iu, int32_t handle)
			{
				Handle = handle;
				CppHandle = Plugin::StoreSystemComponentModelDesignComponentChangedEventHandler(this);
				if (Handle)
				{
					Plugin::ReferenceManagedClass(Handle);
				}
				ClassHandle = 0;
			}
			
			ComponentChangedEventHandler::~ComponentChangedEventHandler()
			{
				Plugin::RemoveSystemComponentModelDesignComponentChangedEventHandler(CppHandle);
				CppHandle = 0;
				if (Handle)
				{
					int32_t handle = Handle;
					int32_t classHandle = ClassHandle;
					Handle = 0;
					ClassHandle = 0;
					if (Plugin::DereferenceManagedClassNoRelease(handle))
					{
						Plugin::ReleaseSystemComponentModelDesignComponentChangedEventHandler(handle, classHandle);
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
			
			ComponentChangedEventHandler& ComponentChangedEventHandler::operator=(const ComponentChangedEventHandler& other)
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
			
			ComponentChangedEventHandler& ComponentChangedEventHandler::operator=(decltype(nullptr))
			{
				if (Handle)
				{
					int32_t handle = Handle;
					int32_t classHandle = ClassHandle;
					Handle = 0;
					ClassHandle = 0;
					if (Plugin::DereferenceManagedClassNoRelease(handle))
					{
						Plugin::ReleaseSystemComponentModelDesignComponentChangedEventHandler(handle, classHandle);
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
			
			ComponentChangedEventHandler& ComponentChangedEventHandler::operator=(ComponentChangedEventHandler&& other)
			{
				Plugin::RemoveSystemComponentModelDesignComponentChangedEventHandler(CppHandle);
				CppHandle = 0;
				if (Handle)
				{
					int32_t handle = Handle;
					int32_t classHandle = ClassHandle;
					Handle = 0;
					ClassHandle = 0;
					if (Plugin::DereferenceManagedClassNoRelease(handle))
					{
						Plugin::ReleaseSystemComponentModelDesignComponentChangedEventHandler(handle, classHandle);
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
			
			bool ComponentChangedEventHandler::operator==(const ComponentChangedEventHandler& other) const
			{
				return Handle == other.Handle;
			}
			
			bool ComponentChangedEventHandler::operator!=(const ComponentChangedEventHandler& other) const
			{
				return Handle != other.Handle;
			}
			
			void ComponentChangedEventHandler::operator+=(System::ComponentModel::Design::ComponentChangedEventHandler& del)
			{
				Plugin::SystemComponentModelDesignComponentChangedEventHandlerAdd(Handle, del.Handle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
			}
			
			void ComponentChangedEventHandler::operator-=(System::ComponentModel::Design::ComponentChangedEventHandler& del)
			{
				Plugin::SystemComponentModelDesignComponentChangedEventHandlerRemove(Handle, del.Handle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
			}
			
			void ComponentChangedEventHandler::operator()(System::Object& sender, System::ComponentModel::Design::ComponentChangedEventArgs& e)
			{
			}
			
			DLLEXPORT void SystemComponentModelDesignComponentChangedEventHandlerNativeInvoke(int32_t cppHandle, int32_t senderHandle, int32_t eHandle)
			{
				try
				{
					auto sender = System::Object(Plugin::InternalUse::Only, senderHandle);
					auto e = System::ComponentModel::Design::ComponentChangedEventArgs(Plugin::InternalUse::Only, eHandle);
					Plugin::GetSystemComponentModelDesignComponentChangedEventHandler(cppHandle)->operator()(sender, e);
				}
				catch (System::Exception ex)
				{
					Plugin::SetException(ex.Handle);
				}
				catch (...)
				{
					System::String msg = "Unhandled exception invoking System::ComponentModel::Design::ComponentChangedEventHandler";
					System::Exception ex(msg);
					Plugin::SetException(ex.Handle);
				}
			}
			
			void ComponentChangedEventHandler::Invoke(System::Object& sender, System::ComponentModel::Design::ComponentChangedEventArgs& e)
			{
				Plugin::SystemComponentModelDesignComponentChangedEventHandlerInvoke(Handle, sender.Handle, e.Handle);
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
	namespace ComponentModel
	{
		namespace Design
		{
			ComponentRenameEventHandler::ComponentRenameEventHandler()
			{
				CppHandle = Plugin::StoreSystemComponentModelDesignComponentRenameEventHandler(this);
				int32_t* handle = &Handle;
				int32_t cppHandle = CppHandle;
				int32_t* classHandle = &ClassHandle;
				Plugin::SystemComponentModelDesignComponentRenameEventHandlerConstructor(cppHandle, handle, classHandle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
				if (Handle)
				{
					Plugin::ReferenceManagedClass(Handle);
				}
				else
				{
					Plugin::RemoveSystemComponentModelDesignComponentRenameEventHandler(CppHandle);
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
			
			ComponentRenameEventHandler::ComponentRenameEventHandler(decltype(nullptr))
			{
				CppHandle = Plugin::StoreSystemComponentModelDesignComponentRenameEventHandler(this);
				ClassHandle = 0;
			}
			
			ComponentRenameEventHandler::ComponentRenameEventHandler(const ComponentRenameEventHandler& other)
			{
				Handle = other.Handle;
				CppHandle = Plugin::StoreSystemComponentModelDesignComponentRenameEventHandler(this);
				if (Handle)
				{
					Plugin::ReferenceManagedClass(Handle);
				}
				ClassHandle = other.ClassHandle;
			}
			
			ComponentRenameEventHandler::ComponentRenameEventHandler(ComponentRenameEventHandler&& other)
			{
				Handle = other.Handle;
				CppHandle = other.CppHandle;
				ClassHandle = other.ClassHandle;
				other.Handle = 0;
				other.CppHandle = 0;
				other.ClassHandle = 0;
			}
			
			ComponentRenameEventHandler::ComponentRenameEventHandler(Plugin::InternalUse iu, int32_t handle)
			{
				Handle = handle;
				CppHandle = Plugin::StoreSystemComponentModelDesignComponentRenameEventHandler(this);
				if (Handle)
				{
					Plugin::ReferenceManagedClass(Handle);
				}
				ClassHandle = 0;
			}
			
			ComponentRenameEventHandler::~ComponentRenameEventHandler()
			{
				Plugin::RemoveSystemComponentModelDesignComponentRenameEventHandler(CppHandle);
				CppHandle = 0;
				if (Handle)
				{
					int32_t handle = Handle;
					int32_t classHandle = ClassHandle;
					Handle = 0;
					ClassHandle = 0;
					if (Plugin::DereferenceManagedClassNoRelease(handle))
					{
						Plugin::ReleaseSystemComponentModelDesignComponentRenameEventHandler(handle, classHandle);
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
			
			ComponentRenameEventHandler& ComponentRenameEventHandler::operator=(const ComponentRenameEventHandler& other)
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
			
			ComponentRenameEventHandler& ComponentRenameEventHandler::operator=(decltype(nullptr))
			{
				if (Handle)
				{
					int32_t handle = Handle;
					int32_t classHandle = ClassHandle;
					Handle = 0;
					ClassHandle = 0;
					if (Plugin::DereferenceManagedClassNoRelease(handle))
					{
						Plugin::ReleaseSystemComponentModelDesignComponentRenameEventHandler(handle, classHandle);
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
			
			ComponentRenameEventHandler& ComponentRenameEventHandler::operator=(ComponentRenameEventHandler&& other)
			{
				Plugin::RemoveSystemComponentModelDesignComponentRenameEventHandler(CppHandle);
				CppHandle = 0;
				if (Handle)
				{
					int32_t handle = Handle;
					int32_t classHandle = ClassHandle;
					Handle = 0;
					ClassHandle = 0;
					if (Plugin::DereferenceManagedClassNoRelease(handle))
					{
						Plugin::ReleaseSystemComponentModelDesignComponentRenameEventHandler(handle, classHandle);
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
			
			bool ComponentRenameEventHandler::operator==(const ComponentRenameEventHandler& other) const
			{
				return Handle == other.Handle;
			}
			
			bool ComponentRenameEventHandler::operator!=(const ComponentRenameEventHandler& other) const
			{
				return Handle != other.Handle;
			}
			
			void ComponentRenameEventHandler::operator+=(System::ComponentModel::Design::ComponentRenameEventHandler& del)
			{
				Plugin::SystemComponentModelDesignComponentRenameEventHandlerAdd(Handle, del.Handle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
			}
			
			void ComponentRenameEventHandler::operator-=(System::ComponentModel::Design::ComponentRenameEventHandler& del)
			{
				Plugin::SystemComponentModelDesignComponentRenameEventHandlerRemove(Handle, del.Handle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
			}
			
			void ComponentRenameEventHandler::operator()(System::Object& sender, System::ComponentModel::Design::ComponentRenameEventArgs& e)
			{
			}
			
			DLLEXPORT void SystemComponentModelDesignComponentRenameEventHandlerNativeInvoke(int32_t cppHandle, int32_t senderHandle, int32_t eHandle)
			{
				try
				{
					auto sender = System::Object(Plugin::InternalUse::Only, senderHandle);
					auto e = System::ComponentModel::Design::ComponentRenameEventArgs(Plugin::InternalUse::Only, eHandle);
					Plugin::GetSystemComponentModelDesignComponentRenameEventHandler(cppHandle)->operator()(sender, e);
				}
				catch (System::Exception ex)
				{
					Plugin::SetException(ex.Handle);
				}
				catch (...)
				{
					System::String msg = "Unhandled exception invoking System::ComponentModel::Design::ComponentRenameEventHandler";
					System::Exception ex(msg);
					Plugin::SetException(ex.Handle);
				}
			}
			
			void ComponentRenameEventHandler::Invoke(System::Object& sender, System::ComponentModel::Design::ComponentRenameEventArgs& e)
			{
				Plugin::SystemComponentModelDesignComponentRenameEventHandlerInvoke(Handle, sender.Handle, e.Handle);
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
	struct NullReferenceExceptionThrower : System::NullReferenceException
	{
		NullReferenceExceptionThrower(int32_t handle)
			: System::Runtime::InteropServices::_Exception(nullptr)
			, System::Runtime::Serialization::ISerializable(nullptr)
			, System::Exception(nullptr)
			, System::SystemException(nullptr)
			, System::NullReferenceException(Plugin::InternalUse::Only, handle)
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
extern void PluginMain(
	void* memory,
	int32_t memorySize,
	bool isFirstBoot);

////////////////////////////////////////////////////////////////
// C++ functions for C# to call
////////////////////////////////////////////////////////////////

enum class InitMode : uint8_t
{
	FirstBoot,
	Reload
};

// Init the plugin
DLLEXPORT void Init(
	uint8_t* memory,
	int32_t memorySize,
	InitMode initMode,
	void (*releaseObject)(int32_t handle),
	int32_t (*stringNew)(const char* chars),
	void (*setException)(int32_t handle),
	int32_t (*arrayGetLength)(int32_t handle),
	int32_t (*enumerableGetEnumerator)(int32_t handle),
	/*BEGIN INIT PARAMS*/
	int32_t maxManagedObjects,
	void (*systemIDisposableMethodDispose)(int32_t thisHandle),
	UnityEngine::Vector3 (*unityEngineVector3ConstructorSystemSingle_SystemSingle_SystemSingle)(float x, float y, float z),
	float (*unityEngineVector3PropertyGetMagnitude)(UnityEngine::Vector3* thiz),
	void (*unityEngineVector3MethodSetSystemSingle_SystemSingle_SystemSingle)(UnityEngine::Vector3* thiz, float newX, float newY, float newZ),
	UnityEngine::Vector3 (*unityEngineVector3Methodop_AdditionUnityEngineVector3_UnityEngineVector3)(UnityEngine::Vector3& a, UnityEngine::Vector3& b),
	UnityEngine::Vector3 (*unityEngineVector3Methodop_UnaryNegationUnityEngineVector3)(UnityEngine::Vector3& a),
	int32_t (*boxVector3)(UnityEngine::Vector3& val),
	UnityEngine::Vector3 (*unboxVector3)(int32_t valHandle),
	int32_t (*unityEngineObjectPropertyGetName)(int32_t thisHandle),
	void (*unityEngineObjectPropertySetName)(int32_t thisHandle, int32_t valueHandle),
	int32_t (*unityEngineObjectMethodop_EqualityUnityEngineObject_UnityEngineObject)(int32_t xHandle, int32_t yHandle),
	int32_t (*unityEngineObjectMethodop_ImplicitUnityEngineObject)(int32_t existsHandle),
	int32_t (*unityEngineComponentPropertyGetTransform)(int32_t thisHandle),
	UnityEngine::Vector3 (*unityEngineTransformPropertyGetPosition)(int32_t thisHandle),
	void (*unityEngineTransformPropertySetPosition)(int32_t thisHandle, UnityEngine::Vector3& value),
	void (*unityEngineTransformMethodSetParentUnityEngineTransform)(int32_t thisHandle, int32_t parentHandle),
	int32_t (*boxColor)(UnityEngine::Color& val),
	UnityEngine::Color (*unboxColor)(int32_t valHandle),
	int32_t (*boxGradientColorKey)(UnityEngine::GradientColorKey& val),
	UnityEngine::GradientColorKey (*unboxGradientColorKey)(int32_t valHandle),
	void (*releaseUnityEngineResolution)(int32_t handle),
	int32_t (*unityEngineResolutionPropertyGetWidth)(int32_t thisHandle),
	void (*unityEngineResolutionPropertySetWidth)(int32_t thisHandle, int32_t value),
	int32_t (*unityEngineResolutionPropertyGetHeight)(int32_t thisHandle),
	void (*unityEngineResolutionPropertySetHeight)(int32_t thisHandle, int32_t value),
	int32_t (*unityEngineResolutionPropertyGetRefreshRate)(int32_t thisHandle),
	void (*unityEngineResolutionPropertySetRefreshRate)(int32_t thisHandle, int32_t value),
	int32_t (*boxResolution)(int32_t valHandle),
	int32_t (*unboxResolution)(int32_t valHandle),
	void (*releaseUnityEngineRaycastHit)(int32_t handle),
	UnityEngine::Vector3 (*unityEngineRaycastHitPropertyGetPoint)(int32_t thisHandle),
	void (*unityEngineRaycastHitPropertySetPoint)(int32_t thisHandle, UnityEngine::Vector3& value),
	int32_t (*unityEngineRaycastHitPropertyGetTransform)(int32_t thisHandle),
	int32_t (*boxRaycastHit)(int32_t valHandle),
	int32_t (*unboxRaycastHit)(int32_t valHandle),
	int32_t (*systemCollectionsIEnumeratorPropertyGetCurrent)(int32_t thisHandle),
	int32_t (*systemCollectionsIEnumeratorMethodMoveNext)(int32_t thisHandle),
	int32_t (*systemCollectionsGenericIEnumeratorSystemStringPropertyGetCurrent)(int32_t thisHandle),
	int32_t (*systemCollectionsGenericIEnumeratorSystemInt32PropertyGetCurrent)(int32_t thisHandle),
	float (*systemCollectionsGenericIEnumeratorSystemSinglePropertyGetCurrent)(int32_t thisHandle),
	int32_t (*systemCollectionsGenericIEnumeratorUnityEngineRaycastHitPropertyGetCurrent)(int32_t thisHandle),
	UnityEngine::GradientColorKey (*systemCollectionsGenericIEnumeratorUnityEngineGradientColorKeyPropertyGetCurrent)(int32_t thisHandle),
	int32_t (*systemCollectionsGenericIEnumeratorUnityEngineResolutionPropertyGetCurrent)(int32_t thisHandle),
	int32_t (*systemCollectionsGenericIEnumerableSystemStringMethodGetEnumerator)(int32_t thisHandle),
	int32_t (*systemCollectionsGenericIEnumerableSystemInt32MethodGetEnumerator)(int32_t thisHandle),
	int32_t (*systemCollectionsGenericIEnumerableSystemSingleMethodGetEnumerator)(int32_t thisHandle),
	int32_t (*systemCollectionsGenericIEnumerableUnityEngineRaycastHitMethodGetEnumerator)(int32_t thisHandle),
	int32_t (*systemCollectionsGenericIEnumerableUnityEngineGradientColorKeyMethodGetEnumerator)(int32_t thisHandle),
	int32_t (*systemCollectionsGenericIEnumerableUnityEngineResolutionMethodGetEnumerator)(int32_t thisHandle),
	void (*releaseUnityEnginePlayablesPlayableGraph)(int32_t handle),
	int32_t (*boxPlayableGraph)(int32_t valHandle),
	int32_t (*unboxPlayableGraph)(int32_t valHandle),
	void (*releaseUnityEngineAnimationsAnimationMixerPlayable)(int32_t handle),
	int32_t (*unityEngineAnimationsAnimationMixerPlayableMethodCreateUnityEnginePlayablesPlayableGraph_SystemInt32_SystemBoolean)(int32_t graphHandle, int32_t inputCount, System::Boolean normalizeWeights),
	int32_t (*boxAnimationMixerPlayable)(int32_t valHandle),
	int32_t (*unboxAnimationMixerPlayable)(int32_t valHandle),
	int32_t (*systemDiagnosticsStopwatchConstructor)(),
	int64_t (*systemDiagnosticsStopwatchPropertyGetElapsedMilliseconds)(int32_t thisHandle),
	void (*systemDiagnosticsStopwatchMethodStart)(int32_t thisHandle),
	void (*systemDiagnosticsStopwatchMethodReset)(int32_t thisHandle),
	int32_t (*unityEngineGameObjectConstructor)(),
	int32_t (*unityEngineGameObjectConstructorSystemString)(int32_t nameHandle),
	int32_t (*unityEngineGameObjectPropertyGetTransform)(int32_t thisHandle),
	int32_t (*unityEngineGameObjectMethodAddComponentMyGameMonoBehavioursTestScript)(int32_t thisHandle),
	int32_t (*unityEngineGameObjectMethodAddComponentMyGameMonoBehavioursAnotherScript)(int32_t thisHandle),
	int32_t (*unityEngineGameObjectMethodCreatePrimitiveUnityEnginePrimitiveType)(UnityEngine::PrimitiveType type),
	void (*unityEngineDebugMethodLogSystemObject)(int32_t messageHandle),
	int32_t (*unityEngineAssertionsAssertFieldGetRaiseExceptions)(),
	void (*unityEngineAssertionsAssertFieldSetRaiseExceptions)(System::Boolean value),
	void (*unityEngineAssertionsAssertMethodAreEqualSystemStringSystemString_SystemString)(int32_t expectedHandle, int32_t actualHandle),
	void (*unityEngineAssertionsAssertMethodAreEqualUnityEngineGameObjectUnityEngineGameObject_UnityEngineGameObject)(int32_t expectedHandle, int32_t actualHandle),
	int32_t (*unityEngineMonoBehaviourPropertyGetTransform)(int32_t thisHandle),
	void (*unityEngineAudioSettingsMethodGetDSPBufferSizeSystemInt32_SystemInt32)(int32_t* bufferLength, int32_t* numBuffers),
	void (*unityEngineNetworkingNetworkTransportMethodGetBroadcastConnectionInfoSystemInt32_SystemString_SystemInt32_SystemByte)(int32_t hostId, int32_t* addressHandle, int32_t* port, uint8_t* error),
	void (*unityEngineNetworkingNetworkTransportMethodInit)(),
	int32_t (*boxQuaternion)(UnityEngine::Quaternion& val),
	UnityEngine::Quaternion (*unboxQuaternion)(int32_t valHandle),
	float (*unityEngineMatrix4x4PropertyGetItem)(UnityEngine::Matrix4x4* thiz, int32_t row, int32_t column),
	void (*unityEngineMatrix4x4PropertySetItem)(UnityEngine::Matrix4x4* thiz, int32_t row, int32_t column, float value),
	int32_t (*boxMatrix4x4)(UnityEngine::Matrix4x4& val),
	UnityEngine::Matrix4x4 (*unboxMatrix4x4)(int32_t valHandle),
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
	int32_t (*unityEngineScreenPropertyGetResolutions)(),
	void (*releaseUnityEngineRay)(int32_t handle),
	int32_t (*unityEngineRayConstructorUnityEngineVector3_UnityEngineVector3)(UnityEngine::Vector3& origin, UnityEngine::Vector3& direction),
	int32_t (*boxRay)(int32_t valHandle),
	int32_t (*unboxRay)(int32_t valHandle),
	int32_t (*unityEnginePhysicsMethodRaycastNonAllocUnityEngineRay_UnityEngineRaycastHitArray1)(int32_t rayHandle, int32_t resultsHandle),
	int32_t (*unityEnginePhysicsMethodRaycastAllUnityEngineRay)(int32_t rayHandle),
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
	void (*releaseUnityEngineSceneManagementScene)(int32_t handle),
	int32_t (*boxScene)(int32_t valHandle),
	int32_t (*unboxScene)(int32_t valHandle),
	int32_t (*boxLoadSceneMode)(UnityEngine::SceneManagement::LoadSceneMode val),
	UnityEngine::SceneManagement::LoadSceneMode (*unboxLoadSceneMode)(int32_t valHandle),
	int32_t (*boxPrimitiveType)(UnityEngine::PrimitiveType val),
	UnityEngine::PrimitiveType (*unboxPrimitiveType)(int32_t valHandle),
	float (*unityEngineTimePropertyGetDeltaTime)(),
	int32_t (*boxFileMode)(System::IO::FileMode val),
	System::IO::FileMode (*unboxFileMode)(int32_t valHandle),
	void (*releaseSystemCollectionsGenericBaseIComparerSystemInt32)(int32_t handle),
	void (*systemCollectionsGenericBaseIComparerSystemInt32Constructor)(int32_t cppHandle, int32_t* handle),
	void (*releaseSystemCollectionsGenericBaseIComparerSystemString)(int32_t handle),
	void (*systemCollectionsGenericBaseIComparerSystemStringConstructor)(int32_t cppHandle, int32_t* handle),
	void (*releaseSystemBaseStringComparer)(int32_t handle),
	void (*systemBaseStringComparerConstructor)(int32_t cppHandle, int32_t* handle),
	int32_t (*systemCollectionsQueuePropertyGetCount)(int32_t thisHandle),
	void (*releaseSystemCollectionsBaseQueue)(int32_t handle),
	void (*systemCollectionsBaseQueueConstructor)(int32_t cppHandle, int32_t* handle),
	void (*releaseSystemComponentModelDesignBaseIComponentChangeService)(int32_t handle),
	void (*systemComponentModelDesignBaseIComponentChangeServiceConstructor)(int32_t cppHandle, int32_t* handle),
	int32_t (*systemIOFileStreamConstructorSystemString_SystemIOFileMode)(int32_t pathHandle, System::IO::FileMode mode),
	void (*systemIOFileStreamMethodWriteByteSystemByte)(int32_t thisHandle, uint8_t value),
	void (*releaseSystemIOBaseFileStream)(int32_t handle),
	void (*systemIOBaseFileStreamConstructorSystemString_SystemIOFileMode)(int32_t cppHandle, int32_t* handle, int32_t pathHandle, System::IO::FileMode mode),
	void (*releaseUnityEnginePlayablesPlayableHandle)(int32_t handle),
	int32_t (*boxPlayableHandle)(int32_t valHandle),
	int32_t (*unboxPlayableHandle)(int32_t valHandle),
	int32_t (*unityEngineExperimentalUIElementsUQueryExtensionsMethodQUnityEngineExperimentalUIElementsVisualElement_SystemString_SystemStringArray1)(int32_t eHandle, int32_t nameHandle, int32_t classesHandle),
	int32_t (*unityEngineExperimentalUIElementsUQueryExtensionsMethodQUnityEngineExperimentalUIElementsVisualElement_SystemString_SystemString)(int32_t eHandle, int32_t nameHandle, int32_t classNameHandle),
	int32_t (*boxInteractionSourcePositionAccuracy)(UnityEngine::XR::WSA::Input::InteractionSourcePositionAccuracy val),
	UnityEngine::XR::WSA::Input::InteractionSourcePositionAccuracy (*unboxInteractionSourcePositionAccuracy)(int32_t valHandle),
	int32_t (*boxInteractionSourceNode)(UnityEngine::XR::WSA::Input::InteractionSourceNode val),
	UnityEngine::XR::WSA::Input::InteractionSourceNode (*unboxInteractionSourceNode)(int32_t valHandle),
	void (*releaseUnityEngineXRWSAInputInteractionSourcePose)(int32_t handle),
	int32_t (*unityEngineXRWSAInputInteractionSourcePoseMethodTryGetRotationUnityEngineQuaternion_UnityEngineXRWSAInputInteractionSourceNode)(int32_t thisHandle, UnityEngine::Quaternion* rotation, UnityEngine::XR::WSA::Input::InteractionSourceNode node),
	int32_t (*boxInteractionSourcePose)(int32_t valHandle),
	int32_t (*unboxInteractionSourcePose)(int32_t valHandle),
	int32_t (*boxBoolean)(System::Boolean val),
	int32_t (*unboxBoolean)(int32_t valHandle),
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
	int16_t (*unboxChar)(int32_t valHandle),
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
	int32_t (*unityEngineResolutionArray1GetItem1)(int32_t thisHandle, int32_t index0),
	int32_t (*unityEngineResolutionArray1SetItem1)(int32_t thisHandle, int32_t index0, int32_t itemHandle),
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
	void (*unityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeInvoke)(int32_t thisHandle, int32_t arg0Handle, UnityEngine::SceneManagement::LoadSceneMode arg1),
	void (*releaseSystemComponentModelDesignComponentEventHandler)(int32_t handle, int32_t classHandle),
	void (*systemComponentModelDesignComponentEventHandlerConstructor)(int32_t cppHandle, int32_t* handle, int32_t* classHandle),
	void (*systemComponentModelDesignComponentEventHandlerAdd)(int32_t thisHandle, int32_t delHandle),
	void (*systemComponentModelDesignComponentEventHandlerRemove)(int32_t thisHandle, int32_t delHandle),
	void (*systemComponentModelDesignComponentEventHandlerInvoke)(int32_t thisHandle, int32_t senderHandle, int32_t eHandle),
	void (*releaseSystemComponentModelDesignComponentChangingEventHandler)(int32_t handle, int32_t classHandle),
	void (*systemComponentModelDesignComponentChangingEventHandlerConstructor)(int32_t cppHandle, int32_t* handle, int32_t* classHandle),
	void (*systemComponentModelDesignComponentChangingEventHandlerAdd)(int32_t thisHandle, int32_t delHandle),
	void (*systemComponentModelDesignComponentChangingEventHandlerRemove)(int32_t thisHandle, int32_t delHandle),
	void (*systemComponentModelDesignComponentChangingEventHandlerInvoke)(int32_t thisHandle, int32_t senderHandle, int32_t eHandle),
	void (*releaseSystemComponentModelDesignComponentChangedEventHandler)(int32_t handle, int32_t classHandle),
	void (*systemComponentModelDesignComponentChangedEventHandlerConstructor)(int32_t cppHandle, int32_t* handle, int32_t* classHandle),
	void (*systemComponentModelDesignComponentChangedEventHandlerAdd)(int32_t thisHandle, int32_t delHandle),
	void (*systemComponentModelDesignComponentChangedEventHandlerRemove)(int32_t thisHandle, int32_t delHandle),
	void (*systemComponentModelDesignComponentChangedEventHandlerInvoke)(int32_t thisHandle, int32_t senderHandle, int32_t eHandle),
	void (*releaseSystemComponentModelDesignComponentRenameEventHandler)(int32_t handle, int32_t classHandle),
	void (*systemComponentModelDesignComponentRenameEventHandlerConstructor)(int32_t cppHandle, int32_t* handle, int32_t* classHandle),
	void (*systemComponentModelDesignComponentRenameEventHandlerAdd)(int32_t thisHandle, int32_t delHandle),
	void (*systemComponentModelDesignComponentRenameEventHandlerRemove)(int32_t thisHandle, int32_t delHandle),
	void (*systemComponentModelDesignComponentRenameEventHandlerInvoke)(int32_t thisHandle, int32_t senderHandle, int32_t eHandle)
	/*END INIT PARAMS*/)
{
	uint8_t* curMemory = memory;
	
	// Init managed object ref counting
	Plugin::RefCountsLenClass = maxManagedObjects;
	Plugin::RefCountsClass = (int32_t*)curMemory;
	curMemory += maxManagedObjects * sizeof(int32_t);
	
	// Init pointers to C# functions
	Plugin::StringNew = stringNew;
	Plugin::ReleaseObject = releaseObject;
	Plugin::SetException = setException;
	Plugin::ArrayGetLength = arrayGetLength;
	Plugin::EnumerableGetEnumerator = enumerableGetEnumerator;
	/*BEGIN INIT BODY*/
	Plugin::SystemIDisposableMethodDispose = systemIDisposableMethodDispose;
	Plugin::UnityEngineVector3ConstructorSystemSingle_SystemSingle_SystemSingle = unityEngineVector3ConstructorSystemSingle_SystemSingle_SystemSingle;
	Plugin::UnityEngineVector3PropertyGetMagnitude = unityEngineVector3PropertyGetMagnitude;
	Plugin::UnityEngineVector3MethodSetSystemSingle_SystemSingle_SystemSingle = unityEngineVector3MethodSetSystemSingle_SystemSingle_SystemSingle;
	Plugin::UnityEngineVector3Methodop_AdditionUnityEngineVector3_UnityEngineVector3 = unityEngineVector3Methodop_AdditionUnityEngineVector3_UnityEngineVector3;
	Plugin::UnityEngineVector3Methodop_UnaryNegationUnityEngineVector3 = unityEngineVector3Methodop_UnaryNegationUnityEngineVector3;
	Plugin::BoxVector3 = boxVector3;
	Plugin::UnboxVector3 = unboxVector3;
	Plugin::UnityEngineObjectPropertyGetName = unityEngineObjectPropertyGetName;
	Plugin::UnityEngineObjectPropertySetName = unityEngineObjectPropertySetName;
	Plugin::UnityEngineObjectMethodop_EqualityUnityEngineObject_UnityEngineObject = unityEngineObjectMethodop_EqualityUnityEngineObject_UnityEngineObject;
	Plugin::UnityEngineObjectMethodop_ImplicitUnityEngineObject = unityEngineObjectMethodop_ImplicitUnityEngineObject;
	Plugin::UnityEngineComponentPropertyGetTransform = unityEngineComponentPropertyGetTransform;
	Plugin::UnityEngineTransformPropertyGetPosition = unityEngineTransformPropertyGetPosition;
	Plugin::UnityEngineTransformPropertySetPosition = unityEngineTransformPropertySetPosition;
	Plugin::UnityEngineTransformMethodSetParentUnityEngineTransform = unityEngineTransformMethodSetParentUnityEngineTransform;
	Plugin::BoxColor = boxColor;
	Plugin::UnboxColor = unboxColor;
	Plugin::BoxGradientColorKey = boxGradientColorKey;
	Plugin::UnboxGradientColorKey = unboxGradientColorKey;
	Plugin::ReleaseUnityEngineResolution = releaseUnityEngineResolution;
	Plugin::RefCountsUnityEngineResolution = (int32_t*)curMemory;
	curMemory += 1000 * sizeof(int32_t);
	Plugin::RefCountsLenUnityEngineResolution = 1000;
	Plugin::UnityEngineResolutionPropertyGetWidth = unityEngineResolutionPropertyGetWidth;
	Plugin::UnityEngineResolutionPropertySetWidth = unityEngineResolutionPropertySetWidth;
	Plugin::UnityEngineResolutionPropertyGetHeight = unityEngineResolutionPropertyGetHeight;
	Plugin::UnityEngineResolutionPropertySetHeight = unityEngineResolutionPropertySetHeight;
	Plugin::UnityEngineResolutionPropertyGetRefreshRate = unityEngineResolutionPropertyGetRefreshRate;
	Plugin::UnityEngineResolutionPropertySetRefreshRate = unityEngineResolutionPropertySetRefreshRate;
	Plugin::BoxResolution = boxResolution;
	Plugin::UnboxResolution = unboxResolution;
	Plugin::ReleaseUnityEngineRaycastHit = releaseUnityEngineRaycastHit;
	Plugin::RefCountsUnityEngineRaycastHit = (int32_t*)curMemory;
	curMemory += 1000 * sizeof(int32_t);
	Plugin::RefCountsLenUnityEngineRaycastHit = 1000;
	Plugin::UnityEngineRaycastHitPropertyGetPoint = unityEngineRaycastHitPropertyGetPoint;
	Plugin::UnityEngineRaycastHitPropertySetPoint = unityEngineRaycastHitPropertySetPoint;
	Plugin::UnityEngineRaycastHitPropertyGetTransform = unityEngineRaycastHitPropertyGetTransform;
	Plugin::BoxRaycastHit = boxRaycastHit;
	Plugin::UnboxRaycastHit = unboxRaycastHit;
	Plugin::SystemCollectionsIEnumeratorPropertyGetCurrent = systemCollectionsIEnumeratorPropertyGetCurrent;
	Plugin::SystemCollectionsIEnumeratorMethodMoveNext = systemCollectionsIEnumeratorMethodMoveNext;
	Plugin::SystemCollectionsGenericIEnumeratorSystemStringPropertyGetCurrent = systemCollectionsGenericIEnumeratorSystemStringPropertyGetCurrent;
	Plugin::SystemCollectionsGenericIEnumeratorSystemInt32PropertyGetCurrent = systemCollectionsGenericIEnumeratorSystemInt32PropertyGetCurrent;
	Plugin::SystemCollectionsGenericIEnumeratorSystemSinglePropertyGetCurrent = systemCollectionsGenericIEnumeratorSystemSinglePropertyGetCurrent;
	Plugin::SystemCollectionsGenericIEnumeratorUnityEngineRaycastHitPropertyGetCurrent = systemCollectionsGenericIEnumeratorUnityEngineRaycastHitPropertyGetCurrent;
	Plugin::SystemCollectionsGenericIEnumeratorUnityEngineGradientColorKeyPropertyGetCurrent = systemCollectionsGenericIEnumeratorUnityEngineGradientColorKeyPropertyGetCurrent;
	Plugin::SystemCollectionsGenericIEnumeratorUnityEngineResolutionPropertyGetCurrent = systemCollectionsGenericIEnumeratorUnityEngineResolutionPropertyGetCurrent;
	Plugin::SystemCollectionsGenericIEnumerableSystemStringMethodGetEnumerator = systemCollectionsGenericIEnumerableSystemStringMethodGetEnumerator;
	Plugin::SystemCollectionsGenericIEnumerableSystemInt32MethodGetEnumerator = systemCollectionsGenericIEnumerableSystemInt32MethodGetEnumerator;
	Plugin::SystemCollectionsGenericIEnumerableSystemSingleMethodGetEnumerator = systemCollectionsGenericIEnumerableSystemSingleMethodGetEnumerator;
	Plugin::SystemCollectionsGenericIEnumerableUnityEngineRaycastHitMethodGetEnumerator = systemCollectionsGenericIEnumerableUnityEngineRaycastHitMethodGetEnumerator;
	Plugin::SystemCollectionsGenericIEnumerableUnityEngineGradientColorKeyMethodGetEnumerator = systemCollectionsGenericIEnumerableUnityEngineGradientColorKeyMethodGetEnumerator;
	Plugin::SystemCollectionsGenericIEnumerableUnityEngineResolutionMethodGetEnumerator = systemCollectionsGenericIEnumerableUnityEngineResolutionMethodGetEnumerator;
	Plugin::ReleaseUnityEnginePlayablesPlayableGraph = releaseUnityEnginePlayablesPlayableGraph;
	Plugin::RefCountsUnityEnginePlayablesPlayableGraph = (int32_t*)curMemory;
	curMemory += 1000 * sizeof(int32_t);
	Plugin::RefCountsLenUnityEnginePlayablesPlayableGraph = 1000;
	Plugin::BoxPlayableGraph = boxPlayableGraph;
	Plugin::UnboxPlayableGraph = unboxPlayableGraph;
	Plugin::ReleaseUnityEngineAnimationsAnimationMixerPlayable = releaseUnityEngineAnimationsAnimationMixerPlayable;
	Plugin::RefCountsUnityEngineAnimationsAnimationMixerPlayable = (int32_t*)curMemory;
	curMemory += 1000 * sizeof(int32_t);
	Plugin::RefCountsLenUnityEngineAnimationsAnimationMixerPlayable = 1000;
	Plugin::UnityEngineAnimationsAnimationMixerPlayableMethodCreateUnityEnginePlayablesPlayableGraph_SystemInt32_SystemBoolean = unityEngineAnimationsAnimationMixerPlayableMethodCreateUnityEnginePlayablesPlayableGraph_SystemInt32_SystemBoolean;
	Plugin::BoxAnimationMixerPlayable = boxAnimationMixerPlayable;
	Plugin::UnboxAnimationMixerPlayable = unboxAnimationMixerPlayable;
	Plugin::SystemDiagnosticsStopwatchConstructor = systemDiagnosticsStopwatchConstructor;
	Plugin::SystemDiagnosticsStopwatchPropertyGetElapsedMilliseconds = systemDiagnosticsStopwatchPropertyGetElapsedMilliseconds;
	Plugin::SystemDiagnosticsStopwatchMethodStart = systemDiagnosticsStopwatchMethodStart;
	Plugin::SystemDiagnosticsStopwatchMethodReset = systemDiagnosticsStopwatchMethodReset;
	Plugin::UnityEngineGameObjectConstructor = unityEngineGameObjectConstructor;
	Plugin::UnityEngineGameObjectConstructorSystemString = unityEngineGameObjectConstructorSystemString;
	Plugin::UnityEngineGameObjectPropertyGetTransform = unityEngineGameObjectPropertyGetTransform;
	Plugin::UnityEngineGameObjectMethodAddComponentMyGameMonoBehavioursTestScript = unityEngineGameObjectMethodAddComponentMyGameMonoBehavioursTestScript;
	Plugin::UnityEngineGameObjectMethodAddComponentMyGameMonoBehavioursAnotherScript = unityEngineGameObjectMethodAddComponentMyGameMonoBehavioursAnotherScript;
	Plugin::UnityEngineGameObjectMethodCreatePrimitiveUnityEnginePrimitiveType = unityEngineGameObjectMethodCreatePrimitiveUnityEnginePrimitiveType;
	Plugin::UnityEngineDebugMethodLogSystemObject = unityEngineDebugMethodLogSystemObject;
	Plugin::UnityEngineAssertionsAssertFieldGetRaiseExceptions = unityEngineAssertionsAssertFieldGetRaiseExceptions;
	Plugin::UnityEngineAssertionsAssertFieldSetRaiseExceptions = unityEngineAssertionsAssertFieldSetRaiseExceptions;
	Plugin::UnityEngineAssertionsAssertMethodAreEqualSystemStringSystemString_SystemString = unityEngineAssertionsAssertMethodAreEqualSystemStringSystemString_SystemString;
	Plugin::UnityEngineAssertionsAssertMethodAreEqualUnityEngineGameObjectUnityEngineGameObject_UnityEngineGameObject = unityEngineAssertionsAssertMethodAreEqualUnityEngineGameObjectUnityEngineGameObject_UnityEngineGameObject;
	Plugin::UnityEngineMonoBehaviourPropertyGetTransform = unityEngineMonoBehaviourPropertyGetTransform;
	Plugin::UnityEngineAudioSettingsMethodGetDSPBufferSizeSystemInt32_SystemInt32 = unityEngineAudioSettingsMethodGetDSPBufferSizeSystemInt32_SystemInt32;
	Plugin::UnityEngineNetworkingNetworkTransportMethodGetBroadcastConnectionInfoSystemInt32_SystemString_SystemInt32_SystemByte = unityEngineNetworkingNetworkTransportMethodGetBroadcastConnectionInfoSystemInt32_SystemString_SystemInt32_SystemByte;
	Plugin::UnityEngineNetworkingNetworkTransportMethodInit = unityEngineNetworkingNetworkTransportMethodInit;
	Plugin::BoxQuaternion = boxQuaternion;
	Plugin::UnboxQuaternion = unboxQuaternion;
	Plugin::UnityEngineMatrix4x4PropertyGetItem = unityEngineMatrix4x4PropertyGetItem;
	Plugin::UnityEngineMatrix4x4PropertySetItem = unityEngineMatrix4x4PropertySetItem;
	Plugin::BoxMatrix4x4 = boxMatrix4x4;
	Plugin::UnboxMatrix4x4 = unboxMatrix4x4;
	Plugin::BoxQueryTriggerInteraction = boxQueryTriggerInteraction;
	Plugin::UnboxQueryTriggerInteraction = unboxQueryTriggerInteraction;
	Plugin::ReleaseSystemCollectionsGenericKeyValuePairSystemString_SystemDouble = releaseSystemCollectionsGenericKeyValuePairSystemString_SystemDouble;
	Plugin::RefCountsSystemCollectionsGenericKeyValuePairSystemString_SystemDouble = (int32_t*)curMemory;
	curMemory += 20 * sizeof(int32_t);
	Plugin::RefCountsLenSystemCollectionsGenericKeyValuePairSystemString_SystemDouble = 20;
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
	Plugin::UnityEngineScreenPropertyGetResolutions = unityEngineScreenPropertyGetResolutions;
	Plugin::ReleaseUnityEngineRay = releaseUnityEngineRay;
	Plugin::RefCountsUnityEngineRay = (int32_t*)curMemory;
	curMemory += 10 * sizeof(int32_t);
	Plugin::RefCountsLenUnityEngineRay = 10;
	Plugin::UnityEngineRayConstructorUnityEngineVector3_UnityEngineVector3 = unityEngineRayConstructorUnityEngineVector3_UnityEngineVector3;
	Plugin::BoxRay = boxRay;
	Plugin::UnboxRay = unboxRay;
	Plugin::UnityEnginePhysicsMethodRaycastNonAllocUnityEngineRay_UnityEngineRaycastHitArray1 = unityEnginePhysicsMethodRaycastNonAllocUnityEngineRay_UnityEngineRaycastHitArray1;
	Plugin::UnityEnginePhysicsMethodRaycastAllUnityEngineRay = unityEnginePhysicsMethodRaycastAllUnityEngineRay;
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
	Plugin::ReleaseUnityEngineSceneManagementScene = releaseUnityEngineSceneManagementScene;
	Plugin::RefCountsUnityEngineSceneManagementScene = (int32_t*)curMemory;
	curMemory += 1000 * sizeof(int32_t);
	Plugin::RefCountsLenUnityEngineSceneManagementScene = 1000;
	Plugin::BoxScene = boxScene;
	Plugin::UnboxScene = unboxScene;
	Plugin::BoxLoadSceneMode = boxLoadSceneMode;
	Plugin::UnboxLoadSceneMode = unboxLoadSceneMode;
	Plugin::BoxPrimitiveType = boxPrimitiveType;
	Plugin::UnboxPrimitiveType = unboxPrimitiveType;
	Plugin::UnityEngineTimePropertyGetDeltaTime = unityEngineTimePropertyGetDeltaTime;
	Plugin::BoxFileMode = boxFileMode;
	Plugin::UnboxFileMode = unboxFileMode;
	Plugin::SystemCollectionsGenericBaseIComparerSystemInt32FreeListSize = 1000;
	Plugin::SystemCollectionsGenericBaseIComparerSystemInt32FreeList = (System::Collections::Generic::BaseIComparer<int32_t>**)curMemory;
	curMemory += 1000 * sizeof(System::Collections::Generic::BaseIComparer<int32_t>*);
	
	Plugin::ReleaseSystemCollectionsGenericBaseIComparerSystemInt32 = releaseSystemCollectionsGenericBaseIComparerSystemInt32;
	Plugin::SystemCollectionsGenericBaseIComparerSystemInt32Constructor = systemCollectionsGenericBaseIComparerSystemInt32Constructor;
	Plugin::SystemCollectionsGenericBaseIComparerSystemStringFreeListSize = 1000;
	Plugin::SystemCollectionsGenericBaseIComparerSystemStringFreeList = (System::Collections::Generic::BaseIComparer<System::String>**)curMemory;
	curMemory += 1000 * sizeof(System::Collections::Generic::BaseIComparer<System::String>*);
	
	Plugin::ReleaseSystemCollectionsGenericBaseIComparerSystemString = releaseSystemCollectionsGenericBaseIComparerSystemString;
	Plugin::SystemCollectionsGenericBaseIComparerSystemStringConstructor = systemCollectionsGenericBaseIComparerSystemStringConstructor;
	Plugin::SystemBaseStringComparerFreeListSize = 1000;
	Plugin::SystemBaseStringComparerFreeList = (System::BaseStringComparer**)curMemory;
	curMemory += 1000 * sizeof(System::BaseStringComparer*);
	
	Plugin::ReleaseSystemBaseStringComparer = releaseSystemBaseStringComparer;
	Plugin::SystemBaseStringComparerConstructor = systemBaseStringComparerConstructor;
	Plugin::SystemCollectionsQueuePropertyGetCount = systemCollectionsQueuePropertyGetCount;
	Plugin::SystemCollectionsBaseQueueFreeListSize = 1000;
	Plugin::SystemCollectionsBaseQueueFreeList = (System::Collections::BaseQueue**)curMemory;
	curMemory += 1000 * sizeof(System::Collections::BaseQueue*);
	
	Plugin::ReleaseSystemCollectionsBaseQueue = releaseSystemCollectionsBaseQueue;
	Plugin::SystemCollectionsBaseQueueConstructor = systemCollectionsBaseQueueConstructor;
	Plugin::SystemComponentModelDesignBaseIComponentChangeServiceFreeListSize = 1000;
	Plugin::SystemComponentModelDesignBaseIComponentChangeServiceFreeList = (System::ComponentModel::Design::BaseIComponentChangeService**)curMemory;
	curMemory += 1000 * sizeof(System::ComponentModel::Design::BaseIComponentChangeService*);
	
	Plugin::ReleaseSystemComponentModelDesignBaseIComponentChangeService = releaseSystemComponentModelDesignBaseIComponentChangeService;
	Plugin::SystemComponentModelDesignBaseIComponentChangeServiceConstructor = systemComponentModelDesignBaseIComponentChangeServiceConstructor;
	Plugin::SystemIOFileStreamConstructorSystemString_SystemIOFileMode = systemIOFileStreamConstructorSystemString_SystemIOFileMode;
	Plugin::SystemIOFileStreamMethodWriteByteSystemByte = systemIOFileStreamMethodWriteByteSystemByte;
	Plugin::SystemIOBaseFileStreamFreeListSize = 1000;
	Plugin::SystemIOBaseFileStreamFreeList = (System::IO::BaseFileStream**)curMemory;
	curMemory += 1000 * sizeof(System::IO::BaseFileStream*);
	
	Plugin::ReleaseSystemIOBaseFileStream = releaseSystemIOBaseFileStream;
	Plugin::SystemIOBaseFileStreamConstructorSystemString_SystemIOFileMode = systemIOBaseFileStreamConstructorSystemString_SystemIOFileMode;
	Plugin::ReleaseUnityEnginePlayablesPlayableHandle = releaseUnityEnginePlayablesPlayableHandle;
	Plugin::RefCountsUnityEnginePlayablesPlayableHandle = (int32_t*)curMemory;
	curMemory += 1000 * sizeof(int32_t);
	Plugin::RefCountsLenUnityEnginePlayablesPlayableHandle = 1000;
	Plugin::BoxPlayableHandle = boxPlayableHandle;
	Plugin::UnboxPlayableHandle = unboxPlayableHandle;
	Plugin::UnityEngineExperimentalUIElementsUQueryExtensionsMethodQUnityEngineExperimentalUIElementsVisualElement_SystemString_SystemStringArray1 = unityEngineExperimentalUIElementsUQueryExtensionsMethodQUnityEngineExperimentalUIElementsVisualElement_SystemString_SystemStringArray1;
	Plugin::UnityEngineExperimentalUIElementsUQueryExtensionsMethodQUnityEngineExperimentalUIElementsVisualElement_SystemString_SystemString = unityEngineExperimentalUIElementsUQueryExtensionsMethodQUnityEngineExperimentalUIElementsVisualElement_SystemString_SystemString;
	Plugin::BoxInteractionSourcePositionAccuracy = boxInteractionSourcePositionAccuracy;
	Plugin::UnboxInteractionSourcePositionAccuracy = unboxInteractionSourcePositionAccuracy;
	Plugin::BoxInteractionSourceNode = boxInteractionSourceNode;
	Plugin::UnboxInteractionSourceNode = unboxInteractionSourceNode;
	Plugin::ReleaseUnityEngineXRWSAInputInteractionSourcePose = releaseUnityEngineXRWSAInputInteractionSourcePose;
	Plugin::RefCountsUnityEngineXRWSAInputInteractionSourcePose = (int32_t*)curMemory;
	curMemory += 1000 * sizeof(int32_t);
	Plugin::RefCountsLenUnityEngineXRWSAInputInteractionSourcePose = 1000;
	Plugin::UnityEngineXRWSAInputInteractionSourcePoseMethodTryGetRotationUnityEngineQuaternion_UnityEngineXRWSAInputInteractionSourceNode = unityEngineXRWSAInputInteractionSourcePoseMethodTryGetRotationUnityEngineQuaternion_UnityEngineXRWSAInputInteractionSourceNode;
	Plugin::BoxInteractionSourcePose = boxInteractionSourcePose;
	Plugin::UnboxInteractionSourcePose = unboxInteractionSourcePose;
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
	Plugin::SystemActionFreeListSize = 1000;
	Plugin::SystemActionFreeList = (System::Action**)curMemory;
	curMemory += 1000 * sizeof(System::Action*);
	
	Plugin::ReleaseSystemAction = releaseSystemAction;
	Plugin::SystemActionConstructor = systemActionConstructor;
	Plugin::SystemActionAdd = systemActionAdd;
	Plugin::SystemActionRemove = systemActionRemove;
	Plugin::SystemActionInvoke = systemActionInvoke;
	Plugin::SystemActionSystemSingleFreeListSize = 1000;
	Plugin::SystemActionSystemSingleFreeList = (System::Action1<float>**)curMemory;
	curMemory += 1000 * sizeof(System::Action1<float>*);
	
	Plugin::ReleaseSystemActionSystemSingle = releaseSystemActionSystemSingle;
	Plugin::SystemActionSystemSingleConstructor = systemActionSystemSingleConstructor;
	Plugin::SystemActionSystemSingleAdd = systemActionSystemSingleAdd;
	Plugin::SystemActionSystemSingleRemove = systemActionSystemSingleRemove;
	Plugin::SystemActionSystemSingleInvoke = systemActionSystemSingleInvoke;
	Plugin::SystemActionSystemSingle_SystemSingleFreeListSize = 100;
	Plugin::SystemActionSystemSingle_SystemSingleFreeList = (System::Action2<float, float>**)curMemory;
	curMemory += 100 * sizeof(System::Action2<float, float>*);
	
	Plugin::ReleaseSystemActionSystemSingle_SystemSingle = releaseSystemActionSystemSingle_SystemSingle;
	Plugin::SystemActionSystemSingle_SystemSingleConstructor = systemActionSystemSingle_SystemSingleConstructor;
	Plugin::SystemActionSystemSingle_SystemSingleAdd = systemActionSystemSingle_SystemSingleAdd;
	Plugin::SystemActionSystemSingle_SystemSingleRemove = systemActionSystemSingle_SystemSingleRemove;
	Plugin::SystemActionSystemSingle_SystemSingleInvoke = systemActionSystemSingle_SystemSingleInvoke;
	Plugin::SystemFuncSystemInt32_SystemSingle_SystemDoubleFreeListSize = 50;
	Plugin::SystemFuncSystemInt32_SystemSingle_SystemDoubleFreeList = (System::Func3<int32_t, float, double>**)curMemory;
	curMemory += 50 * sizeof(System::Func3<int32_t, float, double>*);
	
	Plugin::ReleaseSystemFuncSystemInt32_SystemSingle_SystemDouble = releaseSystemFuncSystemInt32_SystemSingle_SystemDouble;
	Plugin::SystemFuncSystemInt32_SystemSingle_SystemDoubleConstructor = systemFuncSystemInt32_SystemSingle_SystemDoubleConstructor;
	Plugin::SystemFuncSystemInt32_SystemSingle_SystemDoubleAdd = systemFuncSystemInt32_SystemSingle_SystemDoubleAdd;
	Plugin::SystemFuncSystemInt32_SystemSingle_SystemDoubleRemove = systemFuncSystemInt32_SystemSingle_SystemDoubleRemove;
	Plugin::SystemFuncSystemInt32_SystemSingle_SystemDoubleInvoke = systemFuncSystemInt32_SystemSingle_SystemDoubleInvoke;
	Plugin::SystemFuncSystemInt16_SystemInt32_SystemStringFreeListSize = 25;
	Plugin::SystemFuncSystemInt16_SystemInt32_SystemStringFreeList = (System::Func3<int16_t, int32_t, System::String>**)curMemory;
	curMemory += 25 * sizeof(System::Func3<int16_t, int32_t, System::String>*);
	
	Plugin::ReleaseSystemFuncSystemInt16_SystemInt32_SystemString = releaseSystemFuncSystemInt16_SystemInt32_SystemString;
	Plugin::SystemFuncSystemInt16_SystemInt32_SystemStringConstructor = systemFuncSystemInt16_SystemInt32_SystemStringConstructor;
	Plugin::SystemFuncSystemInt16_SystemInt32_SystemStringAdd = systemFuncSystemInt16_SystemInt32_SystemStringAdd;
	Plugin::SystemFuncSystemInt16_SystemInt32_SystemStringRemove = systemFuncSystemInt16_SystemInt32_SystemStringRemove;
	Plugin::SystemFuncSystemInt16_SystemInt32_SystemStringInvoke = systemFuncSystemInt16_SystemInt32_SystemStringInvoke;
	Plugin::SystemAppDomainInitializerFreeListSize = 1000;
	Plugin::SystemAppDomainInitializerFreeList = (System::AppDomainInitializer**)curMemory;
	curMemory += 1000 * sizeof(System::AppDomainInitializer*);
	
	Plugin::ReleaseSystemAppDomainInitializer = releaseSystemAppDomainInitializer;
	Plugin::SystemAppDomainInitializerConstructor = systemAppDomainInitializerConstructor;
	Plugin::SystemAppDomainInitializerAdd = systemAppDomainInitializerAdd;
	Plugin::SystemAppDomainInitializerRemove = systemAppDomainInitializerRemove;
	Plugin::SystemAppDomainInitializerInvoke = systemAppDomainInitializerInvoke;
	Plugin::UnityEngineEventsUnityActionFreeListSize = 1000;
	Plugin::UnityEngineEventsUnityActionFreeList = (UnityEngine::Events::UnityAction**)curMemory;
	curMemory += 1000 * sizeof(UnityEngine::Events::UnityAction*);
	
	Plugin::ReleaseUnityEngineEventsUnityAction = releaseUnityEngineEventsUnityAction;
	Plugin::UnityEngineEventsUnityActionConstructor = unityEngineEventsUnityActionConstructor;
	Plugin::UnityEngineEventsUnityActionAdd = unityEngineEventsUnityActionAdd;
	Plugin::UnityEngineEventsUnityActionRemove = unityEngineEventsUnityActionRemove;
	Plugin::UnityEngineEventsUnityActionInvoke = unityEngineEventsUnityActionInvoke;
	Plugin::UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeFreeListSize = 10;
	Plugin::UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeFreeList = (UnityEngine::Events::UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>**)curMemory;
	curMemory += 10 * sizeof(UnityEngine::Events::UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>*);
	
	Plugin::ReleaseUnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneMode = releaseUnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneMode;
	Plugin::UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeConstructor = unityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeConstructor;
	Plugin::UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeAdd = unityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeAdd;
	Plugin::UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeRemove = unityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeRemove;
	Plugin::UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeInvoke = unityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeInvoke;
	Plugin::SystemComponentModelDesignComponentEventHandlerFreeListSize = 1000;
	Plugin::SystemComponentModelDesignComponentEventHandlerFreeList = (System::ComponentModel::Design::ComponentEventHandler**)curMemory;
	curMemory += 1000 * sizeof(System::ComponentModel::Design::ComponentEventHandler*);
	
	Plugin::ReleaseSystemComponentModelDesignComponentEventHandler = releaseSystemComponentModelDesignComponentEventHandler;
	Plugin::SystemComponentModelDesignComponentEventHandlerConstructor = systemComponentModelDesignComponentEventHandlerConstructor;
	Plugin::SystemComponentModelDesignComponentEventHandlerAdd = systemComponentModelDesignComponentEventHandlerAdd;
	Plugin::SystemComponentModelDesignComponentEventHandlerRemove = systemComponentModelDesignComponentEventHandlerRemove;
	Plugin::SystemComponentModelDesignComponentEventHandlerInvoke = systemComponentModelDesignComponentEventHandlerInvoke;
	Plugin::SystemComponentModelDesignComponentChangingEventHandlerFreeListSize = 1000;
	Plugin::SystemComponentModelDesignComponentChangingEventHandlerFreeList = (System::ComponentModel::Design::ComponentChangingEventHandler**)curMemory;
	curMemory += 1000 * sizeof(System::ComponentModel::Design::ComponentChangingEventHandler*);
	
	Plugin::ReleaseSystemComponentModelDesignComponentChangingEventHandler = releaseSystemComponentModelDesignComponentChangingEventHandler;
	Plugin::SystemComponentModelDesignComponentChangingEventHandlerConstructor = systemComponentModelDesignComponentChangingEventHandlerConstructor;
	Plugin::SystemComponentModelDesignComponentChangingEventHandlerAdd = systemComponentModelDesignComponentChangingEventHandlerAdd;
	Plugin::SystemComponentModelDesignComponentChangingEventHandlerRemove = systemComponentModelDesignComponentChangingEventHandlerRemove;
	Plugin::SystemComponentModelDesignComponentChangingEventHandlerInvoke = systemComponentModelDesignComponentChangingEventHandlerInvoke;
	Plugin::SystemComponentModelDesignComponentChangedEventHandlerFreeListSize = 1000;
	Plugin::SystemComponentModelDesignComponentChangedEventHandlerFreeList = (System::ComponentModel::Design::ComponentChangedEventHandler**)curMemory;
	curMemory += 1000 * sizeof(System::ComponentModel::Design::ComponentChangedEventHandler*);
	
	Plugin::ReleaseSystemComponentModelDesignComponentChangedEventHandler = releaseSystemComponentModelDesignComponentChangedEventHandler;
	Plugin::SystemComponentModelDesignComponentChangedEventHandlerConstructor = systemComponentModelDesignComponentChangedEventHandlerConstructor;
	Plugin::SystemComponentModelDesignComponentChangedEventHandlerAdd = systemComponentModelDesignComponentChangedEventHandlerAdd;
	Plugin::SystemComponentModelDesignComponentChangedEventHandlerRemove = systemComponentModelDesignComponentChangedEventHandlerRemove;
	Plugin::SystemComponentModelDesignComponentChangedEventHandlerInvoke = systemComponentModelDesignComponentChangedEventHandlerInvoke;
	Plugin::SystemComponentModelDesignComponentRenameEventHandlerFreeListSize = 1000;
	Plugin::SystemComponentModelDesignComponentRenameEventHandlerFreeList = (System::ComponentModel::Design::ComponentRenameEventHandler**)curMemory;
	curMemory += 1000 * sizeof(System::ComponentModel::Design::ComponentRenameEventHandler*);
	
	Plugin::ReleaseSystemComponentModelDesignComponentRenameEventHandler = releaseSystemComponentModelDesignComponentRenameEventHandler;
	Plugin::SystemComponentModelDesignComponentRenameEventHandlerConstructor = systemComponentModelDesignComponentRenameEventHandlerConstructor;
	Plugin::SystemComponentModelDesignComponentRenameEventHandlerAdd = systemComponentModelDesignComponentRenameEventHandlerAdd;
	Plugin::SystemComponentModelDesignComponentRenameEventHandlerRemove = systemComponentModelDesignComponentRenameEventHandlerRemove;
	Plugin::SystemComponentModelDesignComponentRenameEventHandlerInvoke = systemComponentModelDesignComponentRenameEventHandlerInvoke;
	/*END INIT BODY*/
	
	// Make sure there was enough memory
	int32_t usedMemory = curMemory - (uint8_t*)memory;
	if (usedMemory > memorySize)
	{
		System::String msg = "Plugin memory size is too low";
		System::Exception ex(msg);
		Plugin::SetException(ex.Handle);
		return;
	}
	
	if (initMode == InitMode::FirstBoot)
	{
		memset(memory, 0, memorySize);
		
		/*BEGIN INIT BODY FIRST BOOT*/
		for (int32_t i = 0, end = Plugin::SystemCollectionsGenericBaseIComparerSystemInt32FreeListSize - 1; i < end; ++i)
		{
			Plugin::SystemCollectionsGenericBaseIComparerSystemInt32FreeList[i] = (System::Collections::Generic::BaseIComparer<int32_t>*)(Plugin::SystemCollectionsGenericBaseIComparerSystemInt32FreeList + i + 1);
		}
		Plugin::SystemCollectionsGenericBaseIComparerSystemInt32FreeList[Plugin::SystemCollectionsGenericBaseIComparerSystemInt32FreeListSize - 1] = nullptr;
		Plugin::NextFreeSystemCollectionsGenericBaseIComparerSystemInt32 = Plugin::SystemCollectionsGenericBaseIComparerSystemInt32FreeList + 1;
		
		for (int32_t i = 0, end = Plugin::SystemCollectionsGenericBaseIComparerSystemStringFreeListSize - 1; i < end; ++i)
		{
			Plugin::SystemCollectionsGenericBaseIComparerSystemStringFreeList[i] = (System::Collections::Generic::BaseIComparer<System::String>*)(Plugin::SystemCollectionsGenericBaseIComparerSystemStringFreeList + i + 1);
		}
		Plugin::SystemCollectionsGenericBaseIComparerSystemStringFreeList[Plugin::SystemCollectionsGenericBaseIComparerSystemStringFreeListSize - 1] = nullptr;
		Plugin::NextFreeSystemCollectionsGenericBaseIComparerSystemString = Plugin::SystemCollectionsGenericBaseIComparerSystemStringFreeList + 1;
		
		for (int32_t i = 0, end = Plugin::SystemBaseStringComparerFreeListSize - 1; i < end; ++i)
		{
			Plugin::SystemBaseStringComparerFreeList[i] = (System::BaseStringComparer*)(Plugin::SystemBaseStringComparerFreeList + i + 1);
		}
		Plugin::SystemBaseStringComparerFreeList[Plugin::SystemBaseStringComparerFreeListSize - 1] = nullptr;
		Plugin::NextFreeSystemBaseStringComparer = Plugin::SystemBaseStringComparerFreeList + 1;
		
		for (int32_t i = 0, end = Plugin::SystemCollectionsBaseQueueFreeListSize - 1; i < end; ++i)
		{
			Plugin::SystemCollectionsBaseQueueFreeList[i] = (System::Collections::BaseQueue*)(Plugin::SystemCollectionsBaseQueueFreeList + i + 1);
		}
		Plugin::SystemCollectionsBaseQueueFreeList[Plugin::SystemCollectionsBaseQueueFreeListSize - 1] = nullptr;
		Plugin::NextFreeSystemCollectionsBaseQueue = Plugin::SystemCollectionsBaseQueueFreeList + 1;
		
		for (int32_t i = 0, end = Plugin::SystemComponentModelDesignBaseIComponentChangeServiceFreeListSize - 1; i < end; ++i)
		{
			Plugin::SystemComponentModelDesignBaseIComponentChangeServiceFreeList[i] = (System::ComponentModel::Design::BaseIComponentChangeService*)(Plugin::SystemComponentModelDesignBaseIComponentChangeServiceFreeList + i + 1);
		}
		Plugin::SystemComponentModelDesignBaseIComponentChangeServiceFreeList[Plugin::SystemComponentModelDesignBaseIComponentChangeServiceFreeListSize - 1] = nullptr;
		Plugin::NextFreeSystemComponentModelDesignBaseIComponentChangeService = Plugin::SystemComponentModelDesignBaseIComponentChangeServiceFreeList + 1;
		
		for (int32_t i = 0, end = Plugin::SystemIOBaseFileStreamFreeListSize - 1; i < end; ++i)
		{
			Plugin::SystemIOBaseFileStreamFreeList[i] = (System::IO::BaseFileStream*)(Plugin::SystemIOBaseFileStreamFreeList + i + 1);
		}
		Plugin::SystemIOBaseFileStreamFreeList[Plugin::SystemIOBaseFileStreamFreeListSize - 1] = nullptr;
		Plugin::NextFreeSystemIOBaseFileStream = Plugin::SystemIOBaseFileStreamFreeList + 1;
		
		for (int32_t i = 0, end = Plugin::SystemActionFreeListSize - 1; i < end; ++i)
		{
			Plugin::SystemActionFreeList[i] = (System::Action*)(Plugin::SystemActionFreeList + i + 1);
		}
		Plugin::SystemActionFreeList[Plugin::SystemActionFreeListSize - 1] = nullptr;
		Plugin::NextFreeSystemAction = Plugin::SystemActionFreeList + 1;
		
		for (int32_t i = 0, end = Plugin::SystemActionSystemSingleFreeListSize - 1; i < end; ++i)
		{
			Plugin::SystemActionSystemSingleFreeList[i] = (System::Action1<float>*)(Plugin::SystemActionSystemSingleFreeList + i + 1);
		}
		Plugin::SystemActionSystemSingleFreeList[Plugin::SystemActionSystemSingleFreeListSize - 1] = nullptr;
		Plugin::NextFreeSystemActionSystemSingle = Plugin::SystemActionSystemSingleFreeList + 1;
		
		for (int32_t i = 0, end = Plugin::SystemActionSystemSingle_SystemSingleFreeListSize - 1; i < end; ++i)
		{
			Plugin::SystemActionSystemSingle_SystemSingleFreeList[i] = (System::Action2<float, float>*)(Plugin::SystemActionSystemSingle_SystemSingleFreeList + i + 1);
		}
		Plugin::SystemActionSystemSingle_SystemSingleFreeList[Plugin::SystemActionSystemSingle_SystemSingleFreeListSize - 1] = nullptr;
		Plugin::NextFreeSystemActionSystemSingle_SystemSingle = Plugin::SystemActionSystemSingle_SystemSingleFreeList + 1;
		
		for (int32_t i = 0, end = Plugin::SystemFuncSystemInt32_SystemSingle_SystemDoubleFreeListSize - 1; i < end; ++i)
		{
			Plugin::SystemFuncSystemInt32_SystemSingle_SystemDoubleFreeList[i] = (System::Func3<int32_t, float, double>*)(Plugin::SystemFuncSystemInt32_SystemSingle_SystemDoubleFreeList + i + 1);
		}
		Plugin::SystemFuncSystemInt32_SystemSingle_SystemDoubleFreeList[Plugin::SystemFuncSystemInt32_SystemSingle_SystemDoubleFreeListSize - 1] = nullptr;
		Plugin::NextFreeSystemFuncSystemInt32_SystemSingle_SystemDouble = Plugin::SystemFuncSystemInt32_SystemSingle_SystemDoubleFreeList + 1;
		
		for (int32_t i = 0, end = Plugin::SystemFuncSystemInt16_SystemInt32_SystemStringFreeListSize - 1; i < end; ++i)
		{
			Plugin::SystemFuncSystemInt16_SystemInt32_SystemStringFreeList[i] = (System::Func3<int16_t, int32_t, System::String>*)(Plugin::SystemFuncSystemInt16_SystemInt32_SystemStringFreeList + i + 1);
		}
		Plugin::SystemFuncSystemInt16_SystemInt32_SystemStringFreeList[Plugin::SystemFuncSystemInt16_SystemInt32_SystemStringFreeListSize - 1] = nullptr;
		Plugin::NextFreeSystemFuncSystemInt16_SystemInt32_SystemString = Plugin::SystemFuncSystemInt16_SystemInt32_SystemStringFreeList + 1;
		
		for (int32_t i = 0, end = Plugin::SystemAppDomainInitializerFreeListSize - 1; i < end; ++i)
		{
			Plugin::SystemAppDomainInitializerFreeList[i] = (System::AppDomainInitializer*)(Plugin::SystemAppDomainInitializerFreeList + i + 1);
		}
		Plugin::SystemAppDomainInitializerFreeList[Plugin::SystemAppDomainInitializerFreeListSize - 1] = nullptr;
		Plugin::NextFreeSystemAppDomainInitializer = Plugin::SystemAppDomainInitializerFreeList + 1;
		
		for (int32_t i = 0, end = Plugin::UnityEngineEventsUnityActionFreeListSize - 1; i < end; ++i)
		{
			Plugin::UnityEngineEventsUnityActionFreeList[i] = (UnityEngine::Events::UnityAction*)(Plugin::UnityEngineEventsUnityActionFreeList + i + 1);
		}
		Plugin::UnityEngineEventsUnityActionFreeList[Plugin::UnityEngineEventsUnityActionFreeListSize - 1] = nullptr;
		Plugin::NextFreeUnityEngineEventsUnityAction = Plugin::UnityEngineEventsUnityActionFreeList + 1;
		
		for (int32_t i = 0, end = Plugin::UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeFreeListSize - 1; i < end; ++i)
		{
			Plugin::UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeFreeList[i] = (UnityEngine::Events::UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>*)(Plugin::UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeFreeList + i + 1);
		}
		Plugin::UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeFreeList[Plugin::UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeFreeListSize - 1] = nullptr;
		Plugin::NextFreeUnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneMode = Plugin::UnityEngineEventsUnityActionUnityEngineSceneManagementScene_UnityEngineSceneManagementLoadSceneModeFreeList + 1;
		
		for (int32_t i = 0, end = Plugin::SystemComponentModelDesignComponentEventHandlerFreeListSize - 1; i < end; ++i)
		{
			Plugin::SystemComponentModelDesignComponentEventHandlerFreeList[i] = (System::ComponentModel::Design::ComponentEventHandler*)(Plugin::SystemComponentModelDesignComponentEventHandlerFreeList + i + 1);
		}
		Plugin::SystemComponentModelDesignComponentEventHandlerFreeList[Plugin::SystemComponentModelDesignComponentEventHandlerFreeListSize - 1] = nullptr;
		Plugin::NextFreeSystemComponentModelDesignComponentEventHandler = Plugin::SystemComponentModelDesignComponentEventHandlerFreeList + 1;
		
		for (int32_t i = 0, end = Plugin::SystemComponentModelDesignComponentChangingEventHandlerFreeListSize - 1; i < end; ++i)
		{
			Plugin::SystemComponentModelDesignComponentChangingEventHandlerFreeList[i] = (System::ComponentModel::Design::ComponentChangingEventHandler*)(Plugin::SystemComponentModelDesignComponentChangingEventHandlerFreeList + i + 1);
		}
		Plugin::SystemComponentModelDesignComponentChangingEventHandlerFreeList[Plugin::SystemComponentModelDesignComponentChangingEventHandlerFreeListSize - 1] = nullptr;
		Plugin::NextFreeSystemComponentModelDesignComponentChangingEventHandler = Plugin::SystemComponentModelDesignComponentChangingEventHandlerFreeList + 1;
		
		for (int32_t i = 0, end = Plugin::SystemComponentModelDesignComponentChangedEventHandlerFreeListSize - 1; i < end; ++i)
		{
			Plugin::SystemComponentModelDesignComponentChangedEventHandlerFreeList[i] = (System::ComponentModel::Design::ComponentChangedEventHandler*)(Plugin::SystemComponentModelDesignComponentChangedEventHandlerFreeList + i + 1);
		}
		Plugin::SystemComponentModelDesignComponentChangedEventHandlerFreeList[Plugin::SystemComponentModelDesignComponentChangedEventHandlerFreeListSize - 1] = nullptr;
		Plugin::NextFreeSystemComponentModelDesignComponentChangedEventHandler = Plugin::SystemComponentModelDesignComponentChangedEventHandlerFreeList + 1;
		
		for (int32_t i = 0, end = Plugin::SystemComponentModelDesignComponentRenameEventHandlerFreeListSize - 1; i < end; ++i)
		{
			Plugin::SystemComponentModelDesignComponentRenameEventHandlerFreeList[i] = (System::ComponentModel::Design::ComponentRenameEventHandler*)(Plugin::SystemComponentModelDesignComponentRenameEventHandlerFreeList + i + 1);
		}
		Plugin::SystemComponentModelDesignComponentRenameEventHandlerFreeList[Plugin::SystemComponentModelDesignComponentRenameEventHandlerFreeListSize - 1] = nullptr;
		Plugin::NextFreeSystemComponentModelDesignComponentRenameEventHandler = Plugin::SystemComponentModelDesignComponentRenameEventHandlerFreeList + 1;
		/*END INIT BODY FIRST BOOT*/
	}
	
	try
	{
		PluginMain(
			curMemory,
			memorySize - usedMemory,
			initMode == InitMode::FirstBoot);
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


DLLEXPORT void MyGameMonoBehavioursAnotherScriptAwake(int32_t thisHandle)
{
	MyGame::MonoBehaviours::AnotherScript thiz(Plugin::InternalUse::Only, thisHandle);
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
		System::String msg = "Unhandled exception in MyGame::MonoBehaviours::AnotherScript::Awake";
		System::Exception ex(msg);
		Plugin::SetException(ex.Handle);
	}
}


DLLEXPORT void MyGameMonoBehavioursAnotherScriptUpdate(int32_t thisHandle)
{
	MyGame::MonoBehaviours::AnotherScript thiz(Plugin::InternalUse::Only, thisHandle);
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
		System::String msg = "Unhandled exception in MyGame::MonoBehaviours::AnotherScript::Update";
		System::Exception ex(msg);
		Plugin::SetException(ex.Handle);
	}
}
/*END MONOBEHAVIOUR MESSAGES*/

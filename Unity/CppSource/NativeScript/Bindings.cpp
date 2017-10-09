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
	float (*UnityEngineMatrix4x4PropertyGetItem)(UnityEngine::Matrix4x4* thiz, int32_t row, int32_t column);
	void (*UnityEngineMatrix4x4PropertySetItem)(UnityEngine::Matrix4x4* thiz, int32_t row, int32_t column, float value);
	void (*ReleaseUnityEngineRaycastHit)(int32_t handle);
	UnityEngine::Vector3 (*UnityEngineRaycastHitPropertyGetPoint)(int32_t thisHandle);
	void (*UnityEngineRaycastHitPropertySetPoint)(int32_t thisHandle, UnityEngine::Vector3& value);
	int32_t (*UnityEngineRaycastHitPropertyGetTransform)(int32_t thisHandle);
	void (*ReleaseSystemCollectionsGenericKeyValuePairSystemString_SystemDouble)(int32_t handle);
	int32_t (*SystemCollectionsGenericKeyValuePairSystemString_SystemDoubleConstructorSystemString_SystemDouble)(int32_t keyHandle, double value);
	int32_t (*SystemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetKey)(int32_t thisHandle);
	double (*SystemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetValue)(int32_t thisHandle);
	int32_t (*SystemCollectionsGenericListSystemStringConstructor)();
	int32_t (*SystemCollectionsGenericListSystemStringPropertyGetItem)(int32_t thisHandle, int32_t index);
	void (*SystemCollectionsGenericListSystemStringPropertySetItem)(int32_t thisHandle, int32_t index, int32_t valueHandle);
	void (*SystemCollectionsGenericListSystemStringMethodAddSystemString)(int32_t thisHandle, int32_t itemHandle);
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
	int32_t (*UnityEngineScreenPropertyGetResolutions)();
	UnityEngine::Ray (*UnityEngineRayConstructorUnityEngineVector3_UnityEngineVector3)(UnityEngine::Vector3& origin, UnityEngine::Vector3& direction);
	int32_t (*UnityEnginePhysicsMethodRaycastNonAllocUnityEngineRay_UnityEngineRaycastHit)(UnityEngine::Ray& ray, int32_t resultsHandle);
	int32_t (*UnityEnginePhysicsMethodRaycastAllUnityEngineRay)(UnityEngine::Ray& ray);
	int32_t (*UnityEngineGradientConstructor)();
	int32_t (*UnityEngineGradientPropertyGetColorKeys)(int32_t thisHandle);
	void (*UnityEngineGradientPropertySetColorKeys)(int32_t thisHandle, int32_t valueHandle);
	int32_t (*SystemInt32Array1Constructor1)(int32_t length0);
	int32_t (*SystemInt32Array1GetItem1)(int32_t thisHandle, int32_t index0);
	int32_t (*SystemInt32Array1SetItem1)(int32_t thisHandle, int32_t index0, int32_t item);
	int32_t (*SystemSingleArray1Constructor1)(int32_t length0);
	float (*SystemSingleArray1GetItem1)(int32_t thisHandle, int32_t index0);
	int32_t (*SystemSingleArray1SetItem1)(int32_t thisHandle, int32_t index0, float item);
	int32_t (*SystemSingleArray2Constructor2)(int32_t length0, int32_t length1);
	int32_t (*SystemSingleArray2GetLength2)(int32_t thisHandle, int32_t dimension);
	float (*SystemSingleArray2GetItem2)(int32_t thisHandle, int32_t index0, int32_t index1);
	int32_t (*SystemSingleArray2SetItem2)(int32_t thisHandle, int32_t index0, int32_t index1, float item);
	int32_t (*SystemSingleArray3Constructor3)(int32_t length0, int32_t length1, int32_t length2);
	int32_t (*SystemSingleArray3GetLength3)(int32_t thisHandle, int32_t dimension);
	float (*SystemSingleArray3GetItem3)(int32_t thisHandle, int32_t index0, int32_t index1, int32_t index2);
	int32_t (*SystemSingleArray3SetItem3)(int32_t thisHandle, int32_t index0, int32_t index1, int32_t index2, float item);
	int32_t (*SystemStringArray1Constructor1)(int32_t length0);
	int32_t (*SystemStringArray1GetItem1)(int32_t thisHandle, int32_t index0);
	int32_t (*SystemStringArray1SetItem1)(int32_t thisHandle, int32_t index0, int32_t itemHandle);
	int32_t (*UnityEngineResolutionArray1Constructor1)(int32_t length0);
	UnityEngine::Resolution (*UnityEngineResolutionArray1GetItem1)(int32_t thisHandle, int32_t index0);
	int32_t (*UnityEngineResolutionArray1SetItem1)(int32_t thisHandle, int32_t index0, UnityEngine::Resolution& item);
	int32_t (*UnityEngineRaycastHitArray1Constructor1)(int32_t length0);
	int32_t (*UnityEngineRaycastHitArray1GetItem1)(int32_t thisHandle, int32_t index0);
	int32_t (*UnityEngineRaycastHitArray1SetItem1)(int32_t thisHandle, int32_t index0, int32_t itemHandle);
	int32_t (*UnityEngineGradientColorKeyArray1Constructor1)(int32_t length0);
	UnityEngine::GradientColorKey (*UnityEngineGradientColorKeyArray1GetItem1)(int32_t thisHandle, int32_t index0);
	int32_t (*UnityEngineGradientColorKeyArray1SetItem1)(int32_t thisHandle, int32_t index0, UnityEngine::GradientColorKey& item);
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
	
	/*BEGIN REF COUNTS STATE AND FUNCTIONS*/
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
	

	/*END REF COUNTS STATE AND FUNCTIONS*/
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
	
	Object::Object(std::nullptr_t n)
		: Handle(0)
	{
	}
	
	bool Object::operator==(std::nullptr_t other) const
	{
		return Handle == 0;
	}
	
	bool Object::operator!=(std::nullptr_t other) const
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
	
	ValueType::ValueType(std::nullptr_t n)
	{
		Handle = 0;
	}
	
	String::String(std::nullptr_t n)
		: Object(0)
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
	
	String& String::operator=(std::nullptr_t other)
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
	
	Array::Array(Plugin::InternalUse iu, int32_t handle)
		: Object(iu, handle)
	{
	}
	
	Array::Array(std::nullptr_t n)
		: Object(0)
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
		Stopwatch::Stopwatch(std::nullptr_t n)
			: System::Object(nullptr)
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
			: System::Object(Plugin::InternalUse::Only, other.Handle)
		{
			if (Handle)
			{
				Plugin::ReferenceManagedClass(Handle);
			}
		}
		
		Stopwatch::Stopwatch(Stopwatch&& other)
			: System::Object(Plugin::InternalUse::Only, other.Handle)
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
			if (this->Handle != other.Handle)
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
			}
			return *this;
		}
		
		Stopwatch& Stopwatch::operator=(std::nullptr_t other)
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
	Object::Object(std::nullptr_t n)
		: System::Object(nullptr)
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
		: System::Object(Plugin::InternalUse::Only, other.Handle)
	{
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
	}
	
	Object::Object(Object&& other)
		: System::Object(Plugin::InternalUse::Only, other.Handle)
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
		if (this->Handle != other.Handle)
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
		}
		return *this;
	}
	
	Object& Object::operator=(std::nullptr_t other)
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
	GameObject::GameObject(std::nullptr_t n)
		: UnityEngine::Object(nullptr)
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
		: UnityEngine::Object(Plugin::InternalUse::Only, other.Handle)
	{
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
	}
	
	GameObject::GameObject(GameObject&& other)
		: UnityEngine::Object(Plugin::InternalUse::Only, other.Handle)
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
		if (this->Handle != other.Handle)
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
		}
		return *this;
	}
	
	GameObject& GameObject::operator=(std::nullptr_t other)
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
	Component::Component(std::nullptr_t n)
		: UnityEngine::Object(nullptr)
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
		: UnityEngine::Object(Plugin::InternalUse::Only, other.Handle)
	{
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
	}
	
	Component::Component(Component&& other)
		: UnityEngine::Object(Plugin::InternalUse::Only, other.Handle)
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
		if (this->Handle != other.Handle)
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
		}
		return *this;
	}
	
	Component& Component::operator=(std::nullptr_t other)
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
	Transform::Transform(std::nullptr_t n)
		: UnityEngine::Component(nullptr)
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
		: UnityEngine::Component(Plugin::InternalUse::Only, other.Handle)
	{
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
	}
	
	Transform::Transform(Transform&& other)
		: UnityEngine::Component(Plugin::InternalUse::Only, other.Handle)
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
		if (this->Handle != other.Handle)
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
		}
		return *this;
	}
	
	Transform& Transform::operator=(std::nullptr_t other)
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
	Debug::Debug(std::nullptr_t n)
		: System::Object(nullptr)
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
		: System::Object(Plugin::InternalUse::Only, other.Handle)
	{
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
	}
	
	Debug::Debug(Debug&& other)
		: System::Object(Plugin::InternalUse::Only, other.Handle)
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
		if (this->Handle != other.Handle)
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
		}
		return *this;
	}
	
	Debug& Debug::operator=(std::nullptr_t other)
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
	Collision::Collision(std::nullptr_t n)
		: System::Object(nullptr)
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
		: System::Object(Plugin::InternalUse::Only, other.Handle)
	{
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
	}
	
	Collision::Collision(Collision&& other)
		: System::Object(Plugin::InternalUse::Only, other.Handle)
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
		if (this->Handle != other.Handle)
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
		}
		return *this;
	}
	
	Collision& Collision::operator=(std::nullptr_t other)
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
	Behaviour::Behaviour(std::nullptr_t n)
		: UnityEngine::Component(nullptr)
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
		: UnityEngine::Component(Plugin::InternalUse::Only, other.Handle)
	{
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
	}
	
	Behaviour::Behaviour(Behaviour&& other)
		: UnityEngine::Component(Plugin::InternalUse::Only, other.Handle)
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
		if (this->Handle != other.Handle)
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
		}
		return *this;
	}
	
	Behaviour& Behaviour::operator=(std::nullptr_t other)
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
	MonoBehaviour::MonoBehaviour(std::nullptr_t n)
		: UnityEngine::Behaviour(nullptr)
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
		: UnityEngine::Behaviour(Plugin::InternalUse::Only, other.Handle)
	{
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
	}
	
	MonoBehaviour::MonoBehaviour(MonoBehaviour&& other)
		: UnityEngine::Behaviour(Plugin::InternalUse::Only, other.Handle)
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
		if (this->Handle != other.Handle)
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
		}
		return *this;
	}
	
	MonoBehaviour& MonoBehaviour::operator=(std::nullptr_t other)
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
	AudioSettings::AudioSettings(std::nullptr_t n)
		: System::Object(nullptr)
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
		: System::Object(Plugin::InternalUse::Only, other.Handle)
	{
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
	}
	
	AudioSettings::AudioSettings(AudioSettings&& other)
		: System::Object(Plugin::InternalUse::Only, other.Handle)
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
		if (this->Handle != other.Handle)
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
		}
		return *this;
	}
	
	AudioSettings& AudioSettings::operator=(std::nullptr_t other)
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
		NetworkTransport::NetworkTransport(std::nullptr_t n)
			: System::Object(nullptr)
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
			: System::Object(Plugin::InternalUse::Only, other.Handle)
		{
			if (Handle)
			{
				Plugin::ReferenceManagedClass(Handle);
			}
		}
		
		NetworkTransport::NetworkTransport(NetworkTransport&& other)
			: System::Object(Plugin::InternalUse::Only, other.Handle)
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
			if (this->Handle != other.Handle)
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
			}
			return *this;
		}
		
		NetworkTransport& NetworkTransport::operator=(std::nullptr_t other)
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
			if (address->Handle != addressHandle)
			{
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

namespace UnityEngine
{
	RaycastHit::RaycastHit(std::nullptr_t n)
		: System::ValueType(nullptr)
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
		: System::ValueType(Plugin::InternalUse::Only, other.Handle)
	{
		if (Handle)
		{
			Plugin::ReferenceManagedUnityEngineRaycastHit(Handle);
		}
	}
	
	RaycastHit::RaycastHit(RaycastHit&& other)
		: System::ValueType(Plugin::InternalUse::Only, other.Handle)
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
		if (this->Handle != other.Handle)
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
		}
		return *this;
	}
	
	RaycastHit& RaycastHit::operator=(std::nullptr_t other)
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
	namespace Collections
	{
		namespace Generic
		{
			KeyValuePair<System::String, double>::KeyValuePair(std::nullptr_t n)
				: System::ValueType(nullptr)
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
				: System::ValueType(Plugin::InternalUse::Only, other.Handle)
			{
				if (Handle)
				{
					Plugin::ReferenceManagedSystemCollectionsGenericKeyValuePairSystemString_SystemDouble(Handle);
				}
			}
			
			KeyValuePair<System::String, double>::KeyValuePair(KeyValuePair<System::String, double>&& other)
				: System::ValueType(Plugin::InternalUse::Only, other.Handle)
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
				if (this->Handle != other.Handle)
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
				}
				return *this;
			}
			
			KeyValuePair<System::String, double>& KeyValuePair<System::String, double>::operator=(std::nullptr_t other)
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
	namespace Collections
	{
		namespace Generic
		{
			List<System::String>::List(std::nullptr_t n)
				: System::Object(nullptr)
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
				: System::Object(Plugin::InternalUse::Only, other.Handle)
			{
				if (Handle)
				{
					Plugin::ReferenceManagedClass(Handle);
				}
			}
			
			List<System::String>::List(List<System::String>&& other)
				: System::Object(Plugin::InternalUse::Only, other.Handle)
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
				if (this->Handle != other.Handle)
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
				}
				return *this;
			}
			
			List<System::String>& List<System::String>::operator=(std::nullptr_t other)
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
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			LinkedListNode<System::String>::LinkedListNode(std::nullptr_t n)
				: System::Object(nullptr)
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
				: System::Object(Plugin::InternalUse::Only, other.Handle)
			{
				if (Handle)
				{
					Plugin::ReferenceManagedClass(Handle);
				}
			}
			
			LinkedListNode<System::String>::LinkedListNode(LinkedListNode<System::String>&& other)
				: System::Object(Plugin::InternalUse::Only, other.Handle)
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
				if (this->Handle != other.Handle)
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
				}
				return *this;
			}
			
			LinkedListNode<System::String>& LinkedListNode<System::String>::operator=(std::nullptr_t other)
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
			StrongBox<System::String>::StrongBox(std::nullptr_t n)
				: System::Object(nullptr)
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
				: System::Object(Plugin::InternalUse::Only, other.Handle)
			{
				if (Handle)
				{
					Plugin::ReferenceManagedClass(Handle);
				}
			}
			
			StrongBox<System::String>::StrongBox(StrongBox<System::String>&& other)
				: System::Object(Plugin::InternalUse::Only, other.Handle)
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
				if (this->Handle != other.Handle)
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
				}
				return *this;
			}
			
			StrongBox<System::String>& StrongBox<System::String>::operator=(std::nullptr_t other)
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
			Collection<int32_t>::Collection(std::nullptr_t n)
				: System::Object(nullptr)
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
				: System::Object(Plugin::InternalUse::Only, other.Handle)
			{
				if (Handle)
				{
					Plugin::ReferenceManagedClass(Handle);
				}
			}
			
			Collection<int32_t>::Collection(Collection<int32_t>&& other)
				: System::Object(Plugin::InternalUse::Only, other.Handle)
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
				if (this->Handle != other.Handle)
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
				}
				return *this;
			}
			
			Collection<int32_t>& Collection<int32_t>::operator=(std::nullptr_t other)
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
			KeyedCollection<System::String, int32_t>::KeyedCollection(std::nullptr_t n)
				: System::Collections::ObjectModel::Collection<int32_t>(nullptr)
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
				: System::Collections::ObjectModel::Collection<int32_t>(Plugin::InternalUse::Only, other.Handle)
			{
				if (Handle)
				{
					Plugin::ReferenceManagedClass(Handle);
				}
			}
			
			KeyedCollection<System::String, int32_t>::KeyedCollection(KeyedCollection<System::String, int32_t>&& other)
				: System::Collections::ObjectModel::Collection<int32_t>(Plugin::InternalUse::Only, other.Handle)
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
				if (this->Handle != other.Handle)
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
				}
				return *this;
			}
			
			KeyedCollection<System::String, int32_t>& KeyedCollection<System::String, int32_t>::operator=(std::nullptr_t other)
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
	Exception::Exception(std::nullptr_t n)
		: System::Object(nullptr)
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
		: System::Object(Plugin::InternalUse::Only, other.Handle)
	{
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
	}
	
	Exception::Exception(Exception&& other)
		: System::Object(Plugin::InternalUse::Only, other.Handle)
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
		if (this->Handle != other.Handle)
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
		}
		return *this;
	}
	
	Exception& Exception::operator=(std::nullptr_t other)
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
	SystemException::SystemException(std::nullptr_t n)
		: System::Exception(nullptr)
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
		: System::Exception(Plugin::InternalUse::Only, other.Handle)
	{
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
	}
	
	SystemException::SystemException(SystemException&& other)
		: System::Exception(Plugin::InternalUse::Only, other.Handle)
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
		if (this->Handle != other.Handle)
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
		}
		return *this;
	}
	
	SystemException& SystemException::operator=(std::nullptr_t other)
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
	NullReferenceException::NullReferenceException(std::nullptr_t n)
		: System::SystemException(nullptr)
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
		: System::SystemException(Plugin::InternalUse::Only, other.Handle)
	{
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
	}
	
	NullReferenceException::NullReferenceException(NullReferenceException&& other)
		: System::SystemException(Plugin::InternalUse::Only, other.Handle)
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
		if (this->Handle != other.Handle)
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
		}
		return *this;
	}
	
	NullReferenceException& NullReferenceException::operator=(std::nullptr_t other)
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

namespace UnityEngine
{
	Screen::Screen(std::nullptr_t n)
		: System::Object(nullptr)
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
		: System::Object(Plugin::InternalUse::Only, other.Handle)
	{
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
	}
	
	Screen::Screen(Screen&& other)
		: System::Object(Plugin::InternalUse::Only, other.Handle)
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
		if (this->Handle != other.Handle)
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
		}
		return *this;
	}
	
	Screen& Screen::operator=(std::nullptr_t other)
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
	
	System::Array1<Resolution> Screen::GetResolutions()
	{
		auto returnValue = Plugin::UnityEngineScreenPropertyGetResolutions();
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return System::Array1<Resolution>(Plugin::InternalUse::Only, returnValue);
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

namespace UnityEngine
{
	Physics::Physics(std::nullptr_t n)
		: System::Object(nullptr)
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
		: System::Object(Plugin::InternalUse::Only, other.Handle)
	{
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
	}
	
	Physics::Physics(Physics&& other)
		: System::Object(Plugin::InternalUse::Only, other.Handle)
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
		if (this->Handle != other.Handle)
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
		}
		return *this;
	}
	
	Physics& Physics::operator=(std::nullptr_t other)
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
	
	int32_t Physics::RaycastNonAlloc(UnityEngine::Ray& ray, System::Array1<RaycastHit> results)
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
	
	System::Array1<RaycastHit> Physics::RaycastAll(UnityEngine::Ray& ray)
	{
		auto returnValue = Plugin::UnityEnginePhysicsMethodRaycastAllUnityEngineRay(ray);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return System::Array1<RaycastHit>(Plugin::InternalUse::Only, returnValue);
	}
}

namespace UnityEngine
{
	Color::Color()
	{
	}
}

namespace UnityEngine
{
	GradientColorKey::GradientColorKey()
	{
	}
}

namespace UnityEngine
{
	Gradient::Gradient(std::nullptr_t n)
		: System::Object(nullptr)
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
		: System::Object(Plugin::InternalUse::Only, other.Handle)
	{
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
	}
	
	Gradient::Gradient(Gradient&& other)
		: System::Object(Plugin::InternalUse::Only, other.Handle)
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
		if (this->Handle != other.Handle)
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
		}
		return *this;
	}
	
	Gradient& Gradient::operator=(std::nullptr_t other)
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
	
	System::Array1<GradientColorKey> Gradient::GetColorKeys()
	{
		auto returnValue = Plugin::UnityEngineGradientPropertyGetColorKeys(Handle);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return System::Array1<GradientColorKey>(Plugin::InternalUse::Only, returnValue);
	}
	
	void Gradient::SetColorKeys(System::Array1<GradientColorKey> value)
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

namespace MyGame
{
	namespace MonoBehaviours
	{
		TestScript::TestScript(std::nullptr_t n)
			: UnityEngine::MonoBehaviour(nullptr)
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
			: UnityEngine::MonoBehaviour(Plugin::InternalUse::Only, other.Handle)
		{
			if (Handle)
			{
				Plugin::ReferenceManagedClass(Handle);
			}
		}
		
		TestScript::TestScript(TestScript&& other)
			: UnityEngine::MonoBehaviour(Plugin::InternalUse::Only, other.Handle)
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
			if (this->Handle != other.Handle)
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
			}
			return *this;
		}
		
		TestScript& TestScript::operator=(std::nullptr_t other)
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

namespace System
{
	Array1<int32_t>::Array1(std::nullptr_t n)
		: System::Array(nullptr)
	{
	}
	
	Array1<int32_t>::Array1(Plugin::InternalUse iu, int32_t handle)
		: System::Array(iu, handle)
	{
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
	}
	
	Array1<int32_t>::Array1(const Array1<int32_t>& other)
		: System::Array(Plugin::InternalUse::Only, other.Handle)
	{
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
	}
	
	Array1<int32_t>::Array1(Array1<int32_t>&& other)
		: System::Array(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
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
		if (this->Handle != other.Handle)
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
		}
		return *this;
	}
	
	Array1<int32_t>& Array1<int32_t>::operator=(std::nullptr_t other)
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
		other.Handle = 0;
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
		auto returnValue = Plugin::SystemInt32Array1Constructor1(length0);
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
	
	int32_t Array1<int32_t>::GetLength()
	{
		return Array::GetLength();
	}
	
	int32_t Array1<int32_t>::GetRank()
	{
		return Array::GetRank();
	}
	
	int32_t Array1<int32_t>::GetItem(int32_t index0)
	{
		auto returnValue = Plugin::SystemInt32Array1GetItem1(Handle, index0);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnValue;
	}
	
	void Array1<int32_t>::SetItem(int32_t index0, int32_t item)
	{
		Plugin::SystemInt32Array1SetItem1(Handle, index0, item);
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
	Array1<float>::Array1(std::nullptr_t n)
		: System::Array(nullptr)
	{
	}
	
	Array1<float>::Array1(Plugin::InternalUse iu, int32_t handle)
		: System::Array(iu, handle)
	{
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
	}
	
	Array1<float>::Array1(const Array1<float>& other)
		: System::Array(Plugin::InternalUse::Only, other.Handle)
	{
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
	}
	
	Array1<float>::Array1(Array1<float>&& other)
		: System::Array(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
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
		if (this->Handle != other.Handle)
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
		}
		return *this;
	}
	
	Array1<float>& Array1<float>::operator=(std::nullptr_t other)
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
		other.Handle = 0;
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
		auto returnValue = Plugin::SystemSingleArray1Constructor1(length0);
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
	
	int32_t Array1<float>::GetLength()
	{
		return Array::GetLength();
	}
	
	int32_t Array1<float>::GetRank()
	{
		return Array::GetRank();
	}
	
	float Array1<float>::GetItem(int32_t index0)
	{
		auto returnValue = Plugin::SystemSingleArray1GetItem1(Handle, index0);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnValue;
	}
	
	void Array1<float>::SetItem(int32_t index0, float item)
	{
		Plugin::SystemSingleArray1SetItem1(Handle, index0, item);
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
	Array2<float>::Array2(std::nullptr_t n)
		: System::Array(nullptr)
	{
	}
	
	Array2<float>::Array2(Plugin::InternalUse iu, int32_t handle)
		: System::Array(iu, handle)
	{
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
	}
	
	Array2<float>::Array2(const Array2<float>& other)
		: System::Array(Plugin::InternalUse::Only, other.Handle)
	{
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
	}
	
	Array2<float>::Array2(Array2<float>&& other)
		: System::Array(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
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
		if (this->Handle != other.Handle)
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
		}
		return *this;
	}
	
	Array2<float>& Array2<float>::operator=(std::nullptr_t other)
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
		other.Handle = 0;
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
		auto returnValue = Plugin::SystemSingleArray2Constructor2(length0, length1);
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
	
	int32_t Array2<float>::GetLength()
	{
		return Array::GetLength();
	}
	
	int32_t Array2<float>::GetLength(int32_t dimension)
	{
		auto returnValue = Plugin::SystemSingleArray2GetLength2(Handle, dimension);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnValue;
	}
	
	int32_t Array2<float>::GetRank()
	{
		return Array::GetRank();
	}
	
	float Array2<float>::GetItem(int32_t index0, int32_t index1)
	{
		auto returnValue = Plugin::SystemSingleArray2GetItem2(Handle, index0, index1);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnValue;
	}
	
	void Array2<float>::SetItem(int32_t index0, int32_t index1, float item)
	{
		Plugin::SystemSingleArray2SetItem2(Handle, index0, index1, item);
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
	Array3<float>::Array3(std::nullptr_t n)
		: System::Array(nullptr)
	{
	}
	
	Array3<float>::Array3(Plugin::InternalUse iu, int32_t handle)
		: System::Array(iu, handle)
	{
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
	}
	
	Array3<float>::Array3(const Array3<float>& other)
		: System::Array(Plugin::InternalUse::Only, other.Handle)
	{
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
	}
	
	Array3<float>::Array3(Array3<float>&& other)
		: System::Array(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
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
		if (this->Handle != other.Handle)
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
		}
		return *this;
	}
	
	Array3<float>& Array3<float>::operator=(std::nullptr_t other)
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
		other.Handle = 0;
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
		auto returnValue = Plugin::SystemSingleArray3Constructor3(length0, length1, length2);
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
	
	int32_t Array3<float>::GetLength()
	{
		return Array::GetLength();
	}
	
	int32_t Array3<float>::GetLength(int32_t dimension)
	{
		auto returnValue = Plugin::SystemSingleArray3GetLength3(Handle, dimension);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnValue;
	}
	
	int32_t Array3<float>::GetRank()
	{
		return Array::GetRank();
	}
	
	float Array3<float>::GetItem(int32_t index0, int32_t index1, int32_t index2)
	{
		auto returnValue = Plugin::SystemSingleArray3GetItem3(Handle, index0, index1, index2);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnValue;
	}
	
	void Array3<float>::SetItem(int32_t index0, int32_t index1, int32_t index2, float item)
	{
		Plugin::SystemSingleArray3SetItem3(Handle, index0, index1, index2, item);
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
	Array1<System::String>::Array1(std::nullptr_t n)
		: System::Array(nullptr)
	{
	}
	
	Array1<System::String>::Array1(Plugin::InternalUse iu, int32_t handle)
		: System::Array(iu, handle)
	{
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
	}
	
	Array1<System::String>::Array1(const Array1<System::String>& other)
		: System::Array(Plugin::InternalUse::Only, other.Handle)
	{
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
	}
	
	Array1<System::String>::Array1(Array1<System::String>&& other)
		: System::Array(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
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
		if (this->Handle != other.Handle)
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
		}
		return *this;
	}
	
	Array1<System::String>& Array1<System::String>::operator=(std::nullptr_t other)
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
		other.Handle = 0;
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
		auto returnValue = Plugin::SystemStringArray1Constructor1(length0);
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
	
	int32_t Array1<System::String>::GetLength()
	{
		return Array::GetLength();
	}
	
	int32_t Array1<System::String>::GetRank()
	{
		return Array::GetRank();
	}
	
	System::String Array1<System::String>::GetItem(int32_t index0)
	{
		auto returnValue = Plugin::SystemStringArray1GetItem1(Handle, index0);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return System::String(Plugin::InternalUse::Only, returnValue);
	}
	
	void Array1<System::String>::SetItem(int32_t index0, System::String item)
	{
		Plugin::SystemStringArray1SetItem1(Handle, index0, item.Handle);
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
	Array1<UnityEngine::Resolution>::Array1(std::nullptr_t n)
		: System::Array(nullptr)
	{
	}
	
	Array1<UnityEngine::Resolution>::Array1(Plugin::InternalUse iu, int32_t handle)
		: System::Array(iu, handle)
	{
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
	}
	
	Array1<UnityEngine::Resolution>::Array1(const Array1<UnityEngine::Resolution>& other)
		: System::Array(Plugin::InternalUse::Only, other.Handle)
	{
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
	}
	
	Array1<UnityEngine::Resolution>::Array1(Array1<UnityEngine::Resolution>&& other)
		: System::Array(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
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
		if (this->Handle != other.Handle)
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
		}
		return *this;
	}
	
	Array1<UnityEngine::Resolution>& Array1<UnityEngine::Resolution>::operator=(std::nullptr_t other)
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
		other.Handle = 0;
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
		auto returnValue = Plugin::UnityEngineResolutionArray1Constructor1(length0);
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
	
	int32_t Array1<UnityEngine::Resolution>::GetLength()
	{
		return Array::GetLength();
	}
	
	int32_t Array1<UnityEngine::Resolution>::GetRank()
	{
		return Array::GetRank();
	}
	
	UnityEngine::Resolution Array1<UnityEngine::Resolution>::GetItem(int32_t index0)
	{
		auto returnValue = Plugin::UnityEngineResolutionArray1GetItem1(Handle, index0);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnValue;
	}
	
	void Array1<UnityEngine::Resolution>::SetItem(int32_t index0, UnityEngine::Resolution& item)
	{
		Plugin::UnityEngineResolutionArray1SetItem1(Handle, index0, item);
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
	Array1<UnityEngine::RaycastHit>::Array1(std::nullptr_t n)
		: System::Array(nullptr)
	{
	}
	
	Array1<UnityEngine::RaycastHit>::Array1(Plugin::InternalUse iu, int32_t handle)
		: System::Array(iu, handle)
	{
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
	}
	
	Array1<UnityEngine::RaycastHit>::Array1(const Array1<UnityEngine::RaycastHit>& other)
		: System::Array(Plugin::InternalUse::Only, other.Handle)
	{
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
	}
	
	Array1<UnityEngine::RaycastHit>::Array1(Array1<UnityEngine::RaycastHit>&& other)
		: System::Array(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
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
		if (this->Handle != other.Handle)
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
		}
		return *this;
	}
	
	Array1<UnityEngine::RaycastHit>& Array1<UnityEngine::RaycastHit>::operator=(std::nullptr_t other)
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
		other.Handle = 0;
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
		auto returnValue = Plugin::UnityEngineRaycastHitArray1Constructor1(length0);
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
	
	int32_t Array1<UnityEngine::RaycastHit>::GetLength()
	{
		return Array::GetLength();
	}
	
	int32_t Array1<UnityEngine::RaycastHit>::GetRank()
	{
		return Array::GetRank();
	}
	
	UnityEngine::RaycastHit Array1<UnityEngine::RaycastHit>::GetItem(int32_t index0)
	{
		auto returnValue = Plugin::UnityEngineRaycastHitArray1GetItem1(Handle, index0);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return UnityEngine::RaycastHit(Plugin::InternalUse::Only, returnValue);
	}
	
	void Array1<UnityEngine::RaycastHit>::SetItem(int32_t index0, UnityEngine::RaycastHit item)
	{
		Plugin::UnityEngineRaycastHitArray1SetItem1(Handle, index0, item.Handle);
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
	Array1<UnityEngine::GradientColorKey>::Array1(std::nullptr_t n)
		: System::Array(nullptr)
	{
	}
	
	Array1<UnityEngine::GradientColorKey>::Array1(Plugin::InternalUse iu, int32_t handle)
		: System::Array(iu, handle)
	{
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
	}
	
	Array1<UnityEngine::GradientColorKey>::Array1(const Array1<UnityEngine::GradientColorKey>& other)
		: System::Array(Plugin::InternalUse::Only, other.Handle)
	{
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
	}
	
	Array1<UnityEngine::GradientColorKey>::Array1(Array1<UnityEngine::GradientColorKey>&& other)
		: System::Array(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
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
		if (this->Handle != other.Handle)
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
		}
		return *this;
	}
	
	Array1<UnityEngine::GradientColorKey>& Array1<UnityEngine::GradientColorKey>::operator=(std::nullptr_t other)
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
		other.Handle = 0;
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
		auto returnValue = Plugin::UnityEngineGradientColorKeyArray1Constructor1(length0);
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
	
	int32_t Array1<UnityEngine::GradientColorKey>::GetLength()
	{
		return Array::GetLength();
	}
	
	int32_t Array1<UnityEngine::GradientColorKey>::GetRank()
	{
		return Array::GetRank();
	}
	
	UnityEngine::GradientColorKey Array1<UnityEngine::GradientColorKey>::GetItem(int32_t index0)
	{
		auto returnValue = Plugin::UnityEngineGradientColorKeyArray1GetItem1(Handle, index0);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnValue;
	}
	
	void Array1<UnityEngine::GradientColorKey>::SetItem(int32_t index0, UnityEngine::GradientColorKey& item)
	{
		Plugin::UnityEngineGradientColorKeyArray1SetItem1(Handle, index0, item);
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
	float (*unityEngineMatrix4x4PropertyGetItem)(UnityEngine::Matrix4x4* thiz, int32_t row, int32_t column),
	void (*unityEngineMatrix4x4PropertySetItem)(UnityEngine::Matrix4x4* thiz, int32_t row, int32_t column, float value),
	void (*releaseUnityEngineRaycastHit)(int32_t handle),
	int32_t refCountsLenUnityEngineRaycastHit,
	UnityEngine::Vector3 (*unityEngineRaycastHitPropertyGetPoint)(int32_t thisHandle),
	void (*unityEngineRaycastHitPropertySetPoint)(int32_t thisHandle, UnityEngine::Vector3& value),
	int32_t (*unityEngineRaycastHitPropertyGetTransform)(int32_t thisHandle),
	void (*releaseSystemCollectionsGenericKeyValuePairSystemString_SystemDouble)(int32_t handle),
	int32_t refCountsLenSystemCollectionsGenericKeyValuePairSystemString_SystemDouble,
	int32_t (*systemCollectionsGenericKeyValuePairSystemString_SystemDoubleConstructorSystemString_SystemDouble)(int32_t keyHandle, double value),
	int32_t (*systemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetKey)(int32_t thisHandle),
	double (*systemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetValue)(int32_t thisHandle),
	int32_t (*systemCollectionsGenericListSystemStringConstructor)(),
	int32_t (*systemCollectionsGenericListSystemStringPropertyGetItem)(int32_t thisHandle, int32_t index),
	void (*systemCollectionsGenericListSystemStringPropertySetItem)(int32_t thisHandle, int32_t index, int32_t valueHandle),
	void (*systemCollectionsGenericListSystemStringMethodAddSystemString)(int32_t thisHandle, int32_t itemHandle),
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
	int32_t (*unityEngineScreenPropertyGetResolutions)(),
	UnityEngine::Ray (*unityEngineRayConstructorUnityEngineVector3_UnityEngineVector3)(UnityEngine::Vector3& origin, UnityEngine::Vector3& direction),
	int32_t (*unityEnginePhysicsMethodRaycastNonAllocUnityEngineRay_UnityEngineRaycastHit)(UnityEngine::Ray& ray, int32_t resultsHandle),
	int32_t (*unityEnginePhysicsMethodRaycastAllUnityEngineRay)(UnityEngine::Ray& ray),
	int32_t (*unityEngineGradientConstructor)(),
	int32_t (*unityEngineGradientPropertyGetColorKeys)(int32_t thisHandle),
	void (*unityEngineGradientPropertySetColorKeys)(int32_t thisHandle, int32_t valueHandle),
	int32_t (*systemInt32Array1Constructor1)(int32_t length0),
	int32_t (*systemInt32Array1GetItem1)(int32_t thisHandle, int32_t index0),
	int32_t (*systemInt32Array1SetItem1)(int32_t thisHandle, int32_t index0, int32_t item),
	int32_t (*systemSingleArray1Constructor1)(int32_t length0),
	float (*systemSingleArray1GetItem1)(int32_t thisHandle, int32_t index0),
	int32_t (*systemSingleArray1SetItem1)(int32_t thisHandle, int32_t index0, float item),
	int32_t (*systemSingleArray2Constructor2)(int32_t length0, int32_t length1),
	int32_t (*systemSingleArray2GetLength2)(int32_t thisHandle, int32_t dimension),
	float (*systemSingleArray2GetItem2)(int32_t thisHandle, int32_t index0, int32_t index1),
	int32_t (*systemSingleArray2SetItem2)(int32_t thisHandle, int32_t index0, int32_t index1, float item),
	int32_t (*systemSingleArray3Constructor3)(int32_t length0, int32_t length1, int32_t length2),
	int32_t (*systemSingleArray3GetLength3)(int32_t thisHandle, int32_t dimension),
	float (*systemSingleArray3GetItem3)(int32_t thisHandle, int32_t index0, int32_t index1, int32_t index2),
	int32_t (*systemSingleArray3SetItem3)(int32_t thisHandle, int32_t index0, int32_t index1, int32_t index2, float item),
	int32_t (*systemStringArray1Constructor1)(int32_t length0),
	int32_t (*systemStringArray1GetItem1)(int32_t thisHandle, int32_t index0),
	int32_t (*systemStringArray1SetItem1)(int32_t thisHandle, int32_t index0, int32_t itemHandle),
	int32_t (*unityEngineResolutionArray1Constructor1)(int32_t length0),
	UnityEngine::Resolution (*unityEngineResolutionArray1GetItem1)(int32_t thisHandle, int32_t index0),
	int32_t (*unityEngineResolutionArray1SetItem1)(int32_t thisHandle, int32_t index0, UnityEngine::Resolution& item),
	int32_t (*unityEngineRaycastHitArray1Constructor1)(int32_t length0),
	int32_t (*unityEngineRaycastHitArray1GetItem1)(int32_t thisHandle, int32_t index0),
	int32_t (*unityEngineRaycastHitArray1SetItem1)(int32_t thisHandle, int32_t index0, int32_t itemHandle),
	int32_t (*unityEngineGradientColorKeyArray1Constructor1)(int32_t length0),
	UnityEngine::GradientColorKey (*unityEngineGradientColorKeyArray1GetItem1)(int32_t thisHandle, int32_t index0),
	int32_t (*unityEngineGradientColorKeyArray1SetItem1)(int32_t thisHandle, int32_t index0, UnityEngine::GradientColorKey& item)
	/*END INIT PARAMS*/)
{
	using namespace Plugin;
	
	// Init managed object ref counting
	Plugin::RefCountsLenClass = maxManagedObjects;
	Plugin::RefCountsClass = new int32_t[maxManagedObjects];
	
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
	Plugin::UnityEngineMatrix4x4PropertyGetItem = unityEngineMatrix4x4PropertyGetItem;
	Plugin::UnityEngineMatrix4x4PropertySetItem = unityEngineMatrix4x4PropertySetItem;
	Plugin::ReleaseUnityEngineRaycastHit = releaseUnityEngineRaycastHit;
	Plugin::RefCountsLenUnityEngineRaycastHit = refCountsLenUnityEngineRaycastHit;
	Plugin::RefCountsUnityEngineRaycastHit = new int32_t[refCountsLenUnityEngineRaycastHit]();
	Plugin::UnityEngineRaycastHitPropertyGetPoint = unityEngineRaycastHitPropertyGetPoint;
	Plugin::UnityEngineRaycastHitPropertySetPoint = unityEngineRaycastHitPropertySetPoint;
	Plugin::UnityEngineRaycastHitPropertyGetTransform = unityEngineRaycastHitPropertyGetTransform;
	Plugin::ReleaseSystemCollectionsGenericKeyValuePairSystemString_SystemDouble = releaseSystemCollectionsGenericKeyValuePairSystemString_SystemDouble;
	Plugin::RefCountsLenSystemCollectionsGenericKeyValuePairSystemString_SystemDouble = refCountsLenSystemCollectionsGenericKeyValuePairSystemString_SystemDouble;
	Plugin::RefCountsSystemCollectionsGenericKeyValuePairSystemString_SystemDouble = new int32_t[refCountsLenSystemCollectionsGenericKeyValuePairSystemString_SystemDouble]();
	Plugin::SystemCollectionsGenericKeyValuePairSystemString_SystemDoubleConstructorSystemString_SystemDouble = systemCollectionsGenericKeyValuePairSystemString_SystemDoubleConstructorSystemString_SystemDouble;
	Plugin::SystemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetKey = systemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetKey;
	Plugin::SystemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetValue = systemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetValue;
	Plugin::SystemCollectionsGenericListSystemStringConstructor = systemCollectionsGenericListSystemStringConstructor;
	Plugin::SystemCollectionsGenericListSystemStringPropertyGetItem = systemCollectionsGenericListSystemStringPropertyGetItem;
	Plugin::SystemCollectionsGenericListSystemStringPropertySetItem = systemCollectionsGenericListSystemStringPropertySetItem;
	Plugin::SystemCollectionsGenericListSystemStringMethodAddSystemString = systemCollectionsGenericListSystemStringMethodAddSystemString;
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
	Plugin::UnityEngineScreenPropertyGetResolutions = unityEngineScreenPropertyGetResolutions;
	Plugin::UnityEngineRayConstructorUnityEngineVector3_UnityEngineVector3 = unityEngineRayConstructorUnityEngineVector3_UnityEngineVector3;
	Plugin::UnityEnginePhysicsMethodRaycastNonAllocUnityEngineRay_UnityEngineRaycastHit = unityEnginePhysicsMethodRaycastNonAllocUnityEngineRay_UnityEngineRaycastHit;
	Plugin::UnityEnginePhysicsMethodRaycastAllUnityEngineRay = unityEnginePhysicsMethodRaycastAllUnityEngineRay;
	Plugin::UnityEngineGradientConstructor = unityEngineGradientConstructor;
	Plugin::UnityEngineGradientPropertyGetColorKeys = unityEngineGradientPropertyGetColorKeys;
	Plugin::UnityEngineGradientPropertySetColorKeys = unityEngineGradientPropertySetColorKeys;
	Plugin::SystemInt32Array1Constructor1 = systemInt32Array1Constructor1;
	Plugin::SystemInt32Array1GetItem1 = systemInt32Array1GetItem1;
	Plugin::SystemInt32Array1SetItem1 = systemInt32Array1SetItem1;
	Plugin::SystemSingleArray1Constructor1 = systemSingleArray1Constructor1;
	Plugin::SystemSingleArray1GetItem1 = systemSingleArray1GetItem1;
	Plugin::SystemSingleArray1SetItem1 = systemSingleArray1SetItem1;
	Plugin::SystemSingleArray2Constructor2 = systemSingleArray2Constructor2;
	Plugin::SystemSingleArray2GetLength2 = systemSingleArray2GetLength2;
	Plugin::SystemSingleArray2GetItem2 = systemSingleArray2GetItem2;
	Plugin::SystemSingleArray2SetItem2 = systemSingleArray2SetItem2;
	Plugin::SystemSingleArray3Constructor3 = systemSingleArray3Constructor3;
	Plugin::SystemSingleArray3GetLength3 = systemSingleArray3GetLength3;
	Plugin::SystemSingleArray3GetItem3 = systemSingleArray3GetItem3;
	Plugin::SystemSingleArray3SetItem3 = systemSingleArray3SetItem3;
	Plugin::SystemStringArray1Constructor1 = systemStringArray1Constructor1;
	Plugin::SystemStringArray1GetItem1 = systemStringArray1GetItem1;
	Plugin::SystemStringArray1SetItem1 = systemStringArray1SetItem1;
	Plugin::UnityEngineResolutionArray1Constructor1 = unityEngineResolutionArray1Constructor1;
	Plugin::UnityEngineResolutionArray1GetItem1 = unityEngineResolutionArray1GetItem1;
	Plugin::UnityEngineResolutionArray1SetItem1 = unityEngineResolutionArray1SetItem1;
	Plugin::UnityEngineRaycastHitArray1Constructor1 = unityEngineRaycastHitArray1Constructor1;
	Plugin::UnityEngineRaycastHitArray1GetItem1 = unityEngineRaycastHitArray1GetItem1;
	Plugin::UnityEngineRaycastHitArray1SetItem1 = unityEngineRaycastHitArray1SetItem1;
	Plugin::UnityEngineGradientColorKeyArray1Constructor1 = unityEngineGradientColorKeyArray1Constructor1;
	Plugin::UnityEngineGradientColorKeyArray1GetItem1 = unityEngineGradientColorKeyArray1GetItem1;
	Plugin::UnityEngineGradientColorKeyArray1SetItem1 = unityEngineGradientColorKeyArray1SetItem1;
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
		System::Exception ex(System::String("Unhandled exception in PluginMain"));
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
DLLEXPORT void TestScriptAwake(int32_t thisHandle)
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
		System::Exception ex(System::String("Unhandled exception in MyGame::MonoBehaviours::TestScript::Awake"));
		Plugin::SetException(ex.Handle);
	}
}


DLLEXPORT void TestScriptOnAnimatorIK(int32_t thisHandle, int32_t param0)
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
		System::Exception ex(System::String("Unhandled exception in MyGame::MonoBehaviours::TestScript::OnAnimatorIK"));
		Plugin::SetException(ex.Handle);
	}
}


DLLEXPORT void TestScriptOnCollisionEnter(int32_t thisHandle, int32_t param0Handle)
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
		System::Exception ex(System::String("Unhandled exception in MyGame::MonoBehaviours::TestScript::OnCollisionEnter"));
		Plugin::SetException(ex.Handle);
	}
}


DLLEXPORT void TestScriptUpdate(int32_t thisHandle)
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
		System::Exception ex(System::String("Unhandled exception in MyGame::MonoBehaviours::TestScript::Update"));
		Plugin::SetException(ex.Handle);
	}
}
/*END MONOBEHAVIOUR MESSAGES*/

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
	void (*SetException)(int32_t handle);
	
	int32_t (*StringNew)(const char* chars);
	
	/*BEGIN FUNCTION POINTERS*/
	int32_t (*SystemDiagnosticsStopwatchConstructor)();
	int64_t (*SystemDiagnosticsStopwatchPropertyGetElapsedMilliseconds)(int32_t thisHandle);
	void (*SystemDiagnosticsStopwatchMethodStart)(int32_t thisHandle);
	void (*SystemDiagnosticsStopwatchMethodReset)(int32_t thisHandle);
	int32_t (*UnityEngineObjectPropertyGetName)(int32_t thisHandle);
	void (*UnityEngineObjectPropertySetName)(int32_t thisHandle, int32_t valueHandle);
	int32_t (*UnityEngineGameObjectConstructor)();
	int32_t (*UnityEngineGameObjectConstructorSystemString)(int32_t nameHandle);
	int32_t (*UnityEngineGameObjectPropertyGetTransform)(int32_t thisHandle);
	int32_t (*UnityEngineGameObjectMethodFindSystemString)(int32_t nameHandle);
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
	void (*ReleaseUnityEngineRaycastHit)(int32_t handle);
	UnityEngine::Vector3 (*UnityEngineRaycastHitPropertyGetPoint)(int32_t thisHandle);
	void (*UnityEngineRaycastHitPropertySetPoint)(int32_t thisHandle, UnityEngine::Vector3& value);
	int32_t (*UnityEngineRaycastHitPropertyGetTransform)(int32_t thisHandle);
	void (*ReleaseSystemCollectionsGenericKeyValuePairSystemString_SystemDouble)(int32_t handle);
	int32_t (*SystemCollectionsGenericKeyValuePairSystemString_SystemDoubleConstructorSystemString_SystemDouble)(int32_t keyHandle, double value);
	int32_t (*SystemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetKey)(int32_t thisHandle);
	double (*SystemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetValue)(int32_t thisHandle);
	int32_t (*SystemCollectionsGenericListSystemStringConstructor)();
	void (*SystemCollectionsGenericListSystemStringMethodAddSystemString)(int32_t thisHandle, int32_t itemHandle);
	int32_t (*SystemCollectionsGenericLinkedListNodeSystemStringConstructorSystemString)(int32_t valueHandle);
	int32_t (*SystemCollectionsGenericLinkedListNodeSystemStringPropertyGetValue)(int32_t thisHandle);
	void (*SystemCollectionsGenericLinkedListNodeSystemStringPropertySetValue)(int32_t thisHandle, int32_t valueHandle);
	int32_t (*SystemRuntimeCompilerServicesStrongBoxSystemStringConstructorSystemString)(int32_t valueHandle);
	int32_t (*SystemRuntimeCompilerServicesStrongBoxSystemStringFieldGetValue)(int32_t thisHandle);
	void (*SystemRuntimeCompilerServicesStrongBoxSystemStringFieldSetValue)(int32_t thisHandle, int32_t valueHandle);
	int32_t (*SystemExceptionConstructorSystemString)(int32_t messageHandle);
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
	System::Exception unhandledCsharpException(nullptr);
}

////////////////////////////////////////////////////////////////
// Mirrors of C# types. These wrap the C# functions to present
// a similiar API as in C#.
////////////////////////////////////////////////////////////////

namespace System
{
	Object::Object(std::nullptr_t n)
		: Handle(0)
	{
	}
	
	Object::Object(int32_t handle)
		: Handle(handle)
	{
	}
	
	Object::operator bool() const
	{
		return Handle != 0;
	}
	
	bool Object::operator==(std::nullptr_t other) const
	{
		return Handle == 0;
	}
	
	bool Object::operator!=(std::nullptr_t other) const
	{
		return Handle != 0;
	}
	
	ValueType::ValueType(std::nullptr_t n)
		: Object(0)
	{
	}
	
	ValueType::ValueType(int32_t handle)
		: Object(handle)
	{
	}
	
	String::String(std::nullptr_t n)
		: Object(0)
	{
	}
	
	String::String(int32_t handle)
		: Object(handle)
	{
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
	}
	
	String::String(const String& other)
		: Object(other.Handle)
	{
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
	}
	
	String::String(String&& other)
		: Object(other.Handle)
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
		: String(Plugin::StringNew(chars))
	{
	}
}

/*BEGIN METHOD DEFINITIONS*/
namespace System
{
	namespace Diagnostics
	{
		Stopwatch::Stopwatch(std::nullptr_t n)
			: System::Object(0)
		{
		}
		
		Stopwatch::Stopwatch(int32_t handle)
			: System::Object(handle)
		{
			if (handle)
			{
				Plugin::ReferenceManagedClass(handle);
			}
		}
		
		Stopwatch::Stopwatch(const Stopwatch& other)
			: System::Object(other.Handle)
		{
			if (Handle)
			{
				Plugin::ReferenceManagedClass(Handle);
			}
		}
		
		Stopwatch::Stopwatch(Stopwatch&& other)
			: System::Object(other.Handle)
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
			 : System::Object(0)
		{
			auto returnValue = Plugin::SystemDiagnosticsStopwatchConstructor();
			if (Plugin::unhandledCsharpException)
			{
				System::Exception ex(Plugin::unhandledCsharpException);
				Plugin::unhandledCsharpException = nullptr;
				throw ex;
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
				System::Exception ex(Plugin::unhandledCsharpException);
				Plugin::unhandledCsharpException = nullptr;
				throw ex;
			}
			return returnValue;
		}
		
		void Stopwatch::Start()
		{
			Plugin::SystemDiagnosticsStopwatchMethodStart(Handle);
			if (Plugin::unhandledCsharpException)
			{
				System::Exception ex(Plugin::unhandledCsharpException);
				Plugin::unhandledCsharpException = nullptr;
				throw ex;
			}
		}
	
		void Stopwatch::Reset()
		{
			Plugin::SystemDiagnosticsStopwatchMethodReset(Handle);
			if (Plugin::unhandledCsharpException)
			{
				System::Exception ex(Plugin::unhandledCsharpException);
				Plugin::unhandledCsharpException = nullptr;
				throw ex;
			}
		}
	}
}

namespace UnityEngine
{
	Object::Object(std::nullptr_t n)
		: System::Object(0)
	{
	}
	
	Object::Object(int32_t handle)
		: System::Object(handle)
	{
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
	}
	
	Object::Object(const Object& other)
		: System::Object(other.Handle)
	{
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
	}
	
	Object::Object(Object&& other)
		: System::Object(other.Handle)
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
			System::Exception ex(Plugin::unhandledCsharpException);
			Plugin::unhandledCsharpException = nullptr;
			throw ex;
		}
		return returnValue;
	}
	
	void Object::SetName(System::String value)
	{
		Plugin::UnityEngineObjectPropertySetName(Handle, value.Handle);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception ex(Plugin::unhandledCsharpException);
			Plugin::unhandledCsharpException = nullptr;
			throw ex;
		}
	}
}

namespace UnityEngine
{
	GameObject::GameObject(std::nullptr_t n)
		: UnityEngine::Object(0)
	{
	}
	
	GameObject::GameObject(int32_t handle)
		: UnityEngine::Object(handle)
	{
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
	}
	
	GameObject::GameObject(const GameObject& other)
		: UnityEngine::Object(other.Handle)
	{
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
	}
	
	GameObject::GameObject(GameObject&& other)
		: UnityEngine::Object(other.Handle)
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
		 : UnityEngine::Object(0)
	{
		auto returnValue = Plugin::UnityEngineGameObjectConstructor();
		if (Plugin::unhandledCsharpException)
		{
			System::Exception ex(Plugin::unhandledCsharpException);
			Plugin::unhandledCsharpException = nullptr;
			throw ex;
		}
		Handle = returnValue;
		if (returnValue)
		{
			Plugin::ReferenceManagedClass(returnValue);
		}
	}
	
	GameObject::GameObject(System::String name)
		 : UnityEngine::Object(0)
	{
		auto returnValue = Plugin::UnityEngineGameObjectConstructorSystemString(name.Handle);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception ex(Plugin::unhandledCsharpException);
			Plugin::unhandledCsharpException = nullptr;
			throw ex;
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
			System::Exception ex(Plugin::unhandledCsharpException);
			Plugin::unhandledCsharpException = nullptr;
			throw ex;
		}
		return returnValue;
	}
	
	UnityEngine::GameObject GameObject::Find(System::String name)
	{
		auto returnValue = Plugin::UnityEngineGameObjectMethodFindSystemString(name.Handle);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception ex(Plugin::unhandledCsharpException);
			Plugin::unhandledCsharpException = nullptr;
			throw ex;
		}
		return returnValue;
	}
	
	template<> MyGame::MonoBehaviours::TestScript GameObject::AddComponent<MyGame::MonoBehaviours::TestScript>()
	{
		auto returnValue = Plugin::UnityEngineGameObjectMethodAddComponentMyGameMonoBehavioursTestScript(Handle);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception ex(Plugin::unhandledCsharpException);
			Plugin::unhandledCsharpException = nullptr;
			throw ex;
		}
		return returnValue;
	}
}

namespace UnityEngine
{
	Component::Component(std::nullptr_t n)
		: UnityEngine::Object(0)
	{
	}
	
	Component::Component(int32_t handle)
		: UnityEngine::Object(handle)
	{
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
	}
	
	Component::Component(const Component& other)
		: UnityEngine::Object(other.Handle)
	{
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
	}
	
	Component::Component(Component&& other)
		: UnityEngine::Object(other.Handle)
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
			System::Exception ex(Plugin::unhandledCsharpException);
			Plugin::unhandledCsharpException = nullptr;
			throw ex;
		}
		return returnValue;
	}
}

namespace UnityEngine
{
	Transform::Transform(std::nullptr_t n)
		: UnityEngine::Component(0)
	{
	}
	
	Transform::Transform(int32_t handle)
		: UnityEngine::Component(handle)
	{
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
	}
	
	Transform::Transform(const Transform& other)
		: UnityEngine::Component(other.Handle)
	{
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
	}
	
	Transform::Transform(Transform&& other)
		: UnityEngine::Component(other.Handle)
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
			System::Exception ex(Plugin::unhandledCsharpException);
			Plugin::unhandledCsharpException = nullptr;
			throw ex;
		}
		return returnValue;
	}
	
	void Transform::SetPosition(UnityEngine::Vector3& value)
	{
		Plugin::UnityEngineTransformPropertySetPosition(Handle, value);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception ex(Plugin::unhandledCsharpException);
			Plugin::unhandledCsharpException = nullptr;
			throw ex;
		}
	}
}

namespace UnityEngine
{
	Debug::Debug(std::nullptr_t n)
		: System::Object(0)
	{
	}
	
	Debug::Debug(int32_t handle)
		: System::Object(handle)
	{
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
	}
	
	Debug::Debug(const Debug& other)
		: System::Object(other.Handle)
	{
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
	}
	
	Debug::Debug(Debug&& other)
		: System::Object(other.Handle)
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
			System::Exception ex(Plugin::unhandledCsharpException);
			Plugin::unhandledCsharpException = nullptr;
			throw ex;
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
				System::Exception ex(Plugin::unhandledCsharpException);
				Plugin::unhandledCsharpException = nullptr;
				throw ex;
			}
			return returnValue;
		}
		
		void Assert::SetRaiseExceptions(System::Boolean value)
		{
			Plugin::UnityEngineAssertionsAssertFieldSetRaiseExceptions(value);
			if (Plugin::unhandledCsharpException)
			{
				System::Exception ex(Plugin::unhandledCsharpException);
				Plugin::unhandledCsharpException = nullptr;
				throw ex;
			}
		}
		
		template<> void Assert::AreEqual<System::String>(System::String expected, System::String actual)
		{
			Plugin::UnityEngineAssertionsAssertMethodAreEqualSystemStringSystemString_SystemString(expected.Handle, actual.Handle);
			if (Plugin::unhandledCsharpException)
			{
				System::Exception ex(Plugin::unhandledCsharpException);
				Plugin::unhandledCsharpException = nullptr;
				throw ex;
			}
		}
	
		template<> void Assert::AreEqual<UnityEngine::GameObject>(UnityEngine::GameObject expected, UnityEngine::GameObject actual)
		{
			Plugin::UnityEngineAssertionsAssertMethodAreEqualUnityEngineGameObjectUnityEngineGameObject_UnityEngineGameObject(expected.Handle, actual.Handle);
			if (Plugin::unhandledCsharpException)
			{
				System::Exception ex(Plugin::unhandledCsharpException);
				Plugin::unhandledCsharpException = nullptr;
				throw ex;
			}
		}
	}
}

namespace UnityEngine
{
	Collision::Collision(std::nullptr_t n)
		: System::Object(0)
	{
	}
	
	Collision::Collision(int32_t handle)
		: System::Object(handle)
	{
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
	}
	
	Collision::Collision(const Collision& other)
		: System::Object(other.Handle)
	{
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
	}
	
	Collision::Collision(Collision&& other)
		: System::Object(other.Handle)
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
		: UnityEngine::Component(0)
	{
	}
	
	Behaviour::Behaviour(int32_t handle)
		: UnityEngine::Component(handle)
	{
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
	}
	
	Behaviour::Behaviour(const Behaviour& other)
		: UnityEngine::Component(other.Handle)
	{
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
	}
	
	Behaviour::Behaviour(Behaviour&& other)
		: UnityEngine::Component(other.Handle)
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
		: UnityEngine::Behaviour(0)
	{
	}
	
	MonoBehaviour::MonoBehaviour(int32_t handle)
		: UnityEngine::Behaviour(handle)
	{
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
	}
	
	MonoBehaviour::MonoBehaviour(const MonoBehaviour& other)
		: UnityEngine::Behaviour(other.Handle)
	{
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
	}
	
	MonoBehaviour::MonoBehaviour(MonoBehaviour&& other)
		: UnityEngine::Behaviour(other.Handle)
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
		: System::Object(0)
	{
	}
	
	AudioSettings::AudioSettings(int32_t handle)
		: System::Object(handle)
	{
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
	}
	
	AudioSettings::AudioSettings(const AudioSettings& other)
		: System::Object(other.Handle)
	{
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
	}
	
	AudioSettings::AudioSettings(AudioSettings&& other)
		: System::Object(other.Handle)
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
			System::Exception ex(Plugin::unhandledCsharpException);
			Plugin::unhandledCsharpException = nullptr;
			throw ex;
		}
	}
}

namespace UnityEngine
{
	namespace Networking
	{
		NetworkTransport::NetworkTransport(std::nullptr_t n)
			: System::Object(0)
		{
		}
		
		NetworkTransport::NetworkTransport(int32_t handle)
			: System::Object(handle)
		{
			if (handle)
			{
				Plugin::ReferenceManagedClass(handle);
			}
		}
		
		NetworkTransport::NetworkTransport(const NetworkTransport& other)
			: System::Object(other.Handle)
		{
			if (Handle)
			{
				Plugin::ReferenceManagedClass(Handle);
			}
		}
		
		NetworkTransport::NetworkTransport(NetworkTransport&& other)
			: System::Object(other.Handle)
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
				System::Exception ex(Plugin::unhandledCsharpException);
				Plugin::unhandledCsharpException = nullptr;
				throw ex;
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
				System::Exception ex(Plugin::unhandledCsharpException);
				Plugin::unhandledCsharpException = nullptr;
				throw ex;
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
			System::Exception ex(Plugin::unhandledCsharpException);
			Plugin::unhandledCsharpException = nullptr;
			throw ex;
		}
		*this = returnValue;
	}
	
	float Vector3::GetMagnitude()
	{
		auto returnValue = Plugin::UnityEngineVector3PropertyGetMagnitude(this);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception ex(Plugin::unhandledCsharpException);
			Plugin::unhandledCsharpException = nullptr;
			throw ex;
		}
		return returnValue;
	}
	
	void Vector3::Set(float newX, float newY, float newZ)
	{
		Plugin::UnityEngineVector3MethodSetSystemSingle_SystemSingle_SystemSingle(this, newX, newY, newZ);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception ex(Plugin::unhandledCsharpException);
			Plugin::unhandledCsharpException = nullptr;
			throw ex;
		}
	}
}

namespace UnityEngine
{
	RaycastHit::RaycastHit(std::nullptr_t n)
		: System::ValueType(0)
	{
	}
	
	RaycastHit::RaycastHit(int32_t handle)
		: System::ValueType(handle)
	{
		if (handle)
		{
			Plugin::ReferenceManagedUnityEngineRaycastHit(Handle);
		}
	}
	
	RaycastHit::RaycastHit(const RaycastHit& other)
		: System::ValueType(other.Handle)
	{
		if (Handle)
		{
			Plugin::ReferenceManagedUnityEngineRaycastHit(Handle);
		}
	}
	
	RaycastHit::RaycastHit(RaycastHit&& other)
		: System::ValueType(other.Handle)
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
			System::Exception ex(Plugin::unhandledCsharpException);
			Plugin::unhandledCsharpException = nullptr;
			throw ex;
		}
		return returnValue;
	}
	
	void RaycastHit::SetPoint(UnityEngine::Vector3& value)
	{
		Plugin::UnityEngineRaycastHitPropertySetPoint(Handle, value);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception ex(Plugin::unhandledCsharpException);
			Plugin::unhandledCsharpException = nullptr;
			throw ex;
		}
	}
	
	UnityEngine::Transform RaycastHit::GetTransform()
	{
		auto returnValue = Plugin::UnityEngineRaycastHitPropertyGetTransform(Handle);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception ex(Plugin::unhandledCsharpException);
			Plugin::unhandledCsharpException = nullptr;
			throw ex;
		}
		return returnValue;
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			KeyValuePair<System::String, double>::KeyValuePair(std::nullptr_t n)
				: System::ValueType(0)
			{
			}
			
			KeyValuePair<System::String, double>::KeyValuePair(int32_t handle)
				: System::ValueType(handle)
			{
				if (handle)
				{
					Plugin::ReferenceManagedSystemCollectionsGenericKeyValuePairSystemString_SystemDouble(Handle);
				}
			}
			
			KeyValuePair<System::String, double>::KeyValuePair(const KeyValuePair<System::String, double>& other)
				: System::ValueType(other.Handle)
			{
				if (Handle)
				{
					Plugin::ReferenceManagedSystemCollectionsGenericKeyValuePairSystemString_SystemDouble(Handle);
				}
			}
			
			KeyValuePair<System::String, double>::KeyValuePair(KeyValuePair<System::String, double>&& other)
				: System::ValueType(other.Handle)
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
				 : System::ValueType(0)
			{
				auto returnValue = Plugin::SystemCollectionsGenericKeyValuePairSystemString_SystemDoubleConstructorSystemString_SystemDouble(key.Handle, value);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception ex(Plugin::unhandledCsharpException);
					Plugin::unhandledCsharpException = nullptr;
					throw ex;
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
					System::Exception ex(Plugin::unhandledCsharpException);
					Plugin::unhandledCsharpException = nullptr;
					throw ex;
				}
				return returnValue;
			}
			
			double KeyValuePair<System::String, double>::GetValue()
			{
				auto returnValue = Plugin::SystemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetValue(Handle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception ex(Plugin::unhandledCsharpException);
					Plugin::unhandledCsharpException = nullptr;
					throw ex;
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
				: System::Object(0)
			{
			}
			
			List<System::String>::List(int32_t handle)
				: System::Object(handle)
			{
				if (handle)
				{
					Plugin::ReferenceManagedClass(handle);
				}
			}
			
			List<System::String>::List(const List<System::String>& other)
				: System::Object(other.Handle)
			{
				if (Handle)
				{
					Plugin::ReferenceManagedClass(Handle);
				}
			}
			
			List<System::String>::List(List<System::String>&& other)
				: System::Object(other.Handle)
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
				 : System::Object(0)
			{
				auto returnValue = Plugin::SystemCollectionsGenericListSystemStringConstructor();
				if (Plugin::unhandledCsharpException)
				{
					System::Exception ex(Plugin::unhandledCsharpException);
					Plugin::unhandledCsharpException = nullptr;
					throw ex;
				}
				Handle = returnValue;
				if (returnValue)
				{
					Plugin::ReferenceManagedClass(returnValue);
				}
			}
			
			void List<System::String>::Add(System::String item)
			{
				Plugin::SystemCollectionsGenericListSystemStringMethodAddSystemString(Handle, item.Handle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception ex(Plugin::unhandledCsharpException);
					Plugin::unhandledCsharpException = nullptr;
					throw ex;
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
				: System::Object(0)
			{
			}
			
			LinkedListNode<System::String>::LinkedListNode(int32_t handle)
				: System::Object(handle)
			{
				if (handle)
				{
					Plugin::ReferenceManagedClass(handle);
				}
			}
			
			LinkedListNode<System::String>::LinkedListNode(const LinkedListNode<System::String>& other)
				: System::Object(other.Handle)
			{
				if (Handle)
				{
					Plugin::ReferenceManagedClass(Handle);
				}
			}
			
			LinkedListNode<System::String>::LinkedListNode(LinkedListNode<System::String>&& other)
				: System::Object(other.Handle)
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
				 : System::Object(0)
			{
				auto returnValue = Plugin::SystemCollectionsGenericLinkedListNodeSystemStringConstructorSystemString(value.Handle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception ex(Plugin::unhandledCsharpException);
					Plugin::unhandledCsharpException = nullptr;
					throw ex;
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
					System::Exception ex(Plugin::unhandledCsharpException);
					Plugin::unhandledCsharpException = nullptr;
					throw ex;
				}
				return returnValue;
			}
			
			void LinkedListNode<System::String>::SetValue(System::String value)
			{
				Plugin::SystemCollectionsGenericLinkedListNodeSystemStringPropertySetValue(Handle, value.Handle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception ex(Plugin::unhandledCsharpException);
					Plugin::unhandledCsharpException = nullptr;
					throw ex;
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
				: System::Object(0)
			{
			}
			
			StrongBox<System::String>::StrongBox(int32_t handle)
				: System::Object(handle)
			{
				if (handle)
				{
					Plugin::ReferenceManagedClass(handle);
				}
			}
			
			StrongBox<System::String>::StrongBox(const StrongBox<System::String>& other)
				: System::Object(other.Handle)
			{
				if (Handle)
				{
					Plugin::ReferenceManagedClass(Handle);
				}
			}
			
			StrongBox<System::String>::StrongBox(StrongBox<System::String>&& other)
				: System::Object(other.Handle)
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
				 : System::Object(0)
			{
				auto returnValue = Plugin::SystemRuntimeCompilerServicesStrongBoxSystemStringConstructorSystemString(value.Handle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception ex(Plugin::unhandledCsharpException);
					Plugin::unhandledCsharpException = nullptr;
					throw ex;
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
					System::Exception ex(Plugin::unhandledCsharpException);
					Plugin::unhandledCsharpException = nullptr;
					throw ex;
				}
				return returnValue;
			}
			
			void StrongBox<System::String>::SetValue(System::String value)
			{
				Plugin::SystemRuntimeCompilerServicesStrongBoxSystemStringFieldSetValue(Handle, value.Handle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception ex(Plugin::unhandledCsharpException);
					Plugin::unhandledCsharpException = nullptr;
					throw ex;
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
				: System::Object(0)
			{
			}
			
			Collection<int32_t>::Collection(int32_t handle)
				: System::Object(handle)
			{
				if (handle)
				{
					Plugin::ReferenceManagedClass(handle);
				}
			}
			
			Collection<int32_t>::Collection(const Collection<int32_t>& other)
				: System::Object(other.Handle)
			{
				if (Handle)
				{
					Plugin::ReferenceManagedClass(Handle);
				}
			}
			
			Collection<int32_t>::Collection(Collection<int32_t>&& other)
				: System::Object(other.Handle)
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
				: System::Collections::ObjectModel::Collection<int32_t>(0)
			{
			}
			
			KeyedCollection<System::String, int32_t>::KeyedCollection(int32_t handle)
				: System::Collections::ObjectModel::Collection<int32_t>(handle)
			{
				if (handle)
				{
					Plugin::ReferenceManagedClass(handle);
				}
			}
			
			KeyedCollection<System::String, int32_t>::KeyedCollection(const KeyedCollection<System::String, int32_t>& other)
				: System::Collections::ObjectModel::Collection<int32_t>(other.Handle)
			{
				if (Handle)
				{
					Plugin::ReferenceManagedClass(Handle);
				}
			}
			
			KeyedCollection<System::String, int32_t>::KeyedCollection(KeyedCollection<System::String, int32_t>&& other)
				: System::Collections::ObjectModel::Collection<int32_t>(other.Handle)
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
		: System::Object(0)
	{
	}
	
	Exception::Exception(int32_t handle)
		: System::Object(handle)
	{
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
	}
	
	Exception::Exception(const Exception& other)
		: System::Object(other.Handle)
	{
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
	}
	
	Exception::Exception(Exception&& other)
		: System::Object(other.Handle)
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
		 : System::Object(0)
	{
		auto returnValue = Plugin::SystemExceptionConstructorSystemString(message.Handle);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception ex(Plugin::unhandledCsharpException);
			Plugin::unhandledCsharpException = nullptr;
			throw ex;
		}
		Handle = returnValue;
		if (returnValue)
		{
			Plugin::ReferenceManagedClass(returnValue);
		}
	}
}

namespace MyGame
{
	namespace MonoBehaviours
	{
		TestScript::TestScript(std::nullptr_t n)
			: UnityEngine::MonoBehaviour(0)
		{
		}
		
		TestScript::TestScript(int32_t handle)
			: UnityEngine::MonoBehaviour(handle)
		{
			if (handle)
			{
				Plugin::ReferenceManagedClass(handle);
			}
		}
		
		TestScript::TestScript(const TestScript& other)
			: UnityEngine::MonoBehaviour(other.Handle)
		{
			if (Handle)
			{
				Plugin::ReferenceManagedClass(Handle);
			}
		}
		
		TestScript::TestScript(TestScript&& other)
			: UnityEngine::MonoBehaviour(other.Handle)
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
	/*BEGIN INIT PARAMS*/
	int32_t (*systemDiagnosticsStopwatchConstructor)(),
	int64_t (*systemDiagnosticsStopwatchPropertyGetElapsedMilliseconds)(int32_t thisHandle),
	void (*systemDiagnosticsStopwatchMethodStart)(int32_t thisHandle),
	void (*systemDiagnosticsStopwatchMethodReset)(int32_t thisHandle),
	int32_t (*unityEngineObjectPropertyGetName)(int32_t thisHandle),
	void (*unityEngineObjectPropertySetName)(int32_t thisHandle, int32_t valueHandle),
	int32_t (*unityEngineGameObjectConstructor)(),
	int32_t (*unityEngineGameObjectConstructorSystemString)(int32_t nameHandle),
	int32_t (*unityEngineGameObjectPropertyGetTransform)(int32_t thisHandle),
	int32_t (*unityEngineGameObjectMethodFindSystemString)(int32_t nameHandle),
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
	void (*systemCollectionsGenericListSystemStringMethodAddSystemString)(int32_t thisHandle, int32_t itemHandle),
	int32_t (*systemCollectionsGenericLinkedListNodeSystemStringConstructorSystemString)(int32_t valueHandle),
	int32_t (*systemCollectionsGenericLinkedListNodeSystemStringPropertyGetValue)(int32_t thisHandle),
	void (*systemCollectionsGenericLinkedListNodeSystemStringPropertySetValue)(int32_t thisHandle, int32_t valueHandle),
	int32_t (*systemRuntimeCompilerServicesStrongBoxSystemStringConstructorSystemString)(int32_t valueHandle),
	int32_t (*systemRuntimeCompilerServicesStrongBoxSystemStringFieldGetValue)(int32_t thisHandle),
	void (*systemRuntimeCompilerServicesStrongBoxSystemStringFieldSetValue)(int32_t thisHandle, int32_t valueHandle),
	int32_t (*systemExceptionConstructorSystemString)(int32_t messageHandle)
	/*END INIT PARAMS*/)
{
	using namespace Plugin;
	
	// Init managed object ref counting
	Plugin::RefCountsLenClass = maxManagedObjects;
	Plugin::RefCountsClass = (int32_t*)calloc(
		maxManagedObjects,
		sizeof(int32_t));
	
	// Init pointers to C# functions
	Plugin::StringNew = stringNew;
	Plugin::ReleaseObject = releaseObject;
	Plugin::SetException = setException;
	/*BEGIN INIT BODY*/
	Plugin::SystemDiagnosticsStopwatchConstructor = systemDiagnosticsStopwatchConstructor;
	Plugin::SystemDiagnosticsStopwatchPropertyGetElapsedMilliseconds = systemDiagnosticsStopwatchPropertyGetElapsedMilliseconds;
	Plugin::SystemDiagnosticsStopwatchMethodStart = systemDiagnosticsStopwatchMethodStart;
	Plugin::SystemDiagnosticsStopwatchMethodReset = systemDiagnosticsStopwatchMethodReset;
	Plugin::UnityEngineObjectPropertyGetName = unityEngineObjectPropertyGetName;
	Plugin::UnityEngineObjectPropertySetName = unityEngineObjectPropertySetName;
	Plugin::UnityEngineGameObjectConstructor = unityEngineGameObjectConstructor;
	Plugin::UnityEngineGameObjectConstructorSystemString = unityEngineGameObjectConstructorSystemString;
	Plugin::UnityEngineGameObjectPropertyGetTransform = unityEngineGameObjectPropertyGetTransform;
	Plugin::UnityEngineGameObjectMethodFindSystemString = unityEngineGameObjectMethodFindSystemString;
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
	Plugin::ReleaseUnityEngineRaycastHit = releaseUnityEngineRaycastHit;
	Plugin::RefCountsLenUnityEngineRaycastHit = refCountsLenUnityEngineRaycastHit;
	Plugin::RefCountsUnityEngineRaycastHit = (int32_t*)calloc(refCountsLenUnityEngineRaycastHit, sizeof(int32_t));
	Plugin::UnityEngineRaycastHitPropertyGetPoint = unityEngineRaycastHitPropertyGetPoint;
	Plugin::UnityEngineRaycastHitPropertySetPoint = unityEngineRaycastHitPropertySetPoint;
	Plugin::UnityEngineRaycastHitPropertyGetTransform = unityEngineRaycastHitPropertyGetTransform;
	Plugin::ReleaseSystemCollectionsGenericKeyValuePairSystemString_SystemDouble = releaseSystemCollectionsGenericKeyValuePairSystemString_SystemDouble;
	Plugin::RefCountsLenSystemCollectionsGenericKeyValuePairSystemString_SystemDouble = refCountsLenSystemCollectionsGenericKeyValuePairSystemString_SystemDouble;
	Plugin::RefCountsSystemCollectionsGenericKeyValuePairSystemString_SystemDouble = (int32_t*)calloc(refCountsLenSystemCollectionsGenericKeyValuePairSystemString_SystemDouble, sizeof(int32_t));
	Plugin::SystemCollectionsGenericKeyValuePairSystemString_SystemDoubleConstructorSystemString_SystemDouble = systemCollectionsGenericKeyValuePairSystemString_SystemDoubleConstructorSystemString_SystemDouble;
	Plugin::SystemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetKey = systemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetKey;
	Plugin::SystemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetValue = systemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetValue;
	Plugin::SystemCollectionsGenericListSystemStringConstructor = systemCollectionsGenericListSystemStringConstructor;
	Plugin::SystemCollectionsGenericListSystemStringMethodAddSystemString = systemCollectionsGenericListSystemStringMethodAddSystemString;
	Plugin::SystemCollectionsGenericLinkedListNodeSystemStringConstructorSystemString = systemCollectionsGenericLinkedListNodeSystemStringConstructorSystemString;
	Plugin::SystemCollectionsGenericLinkedListNodeSystemStringPropertyGetValue = systemCollectionsGenericLinkedListNodeSystemStringPropertyGetValue;
	Plugin::SystemCollectionsGenericLinkedListNodeSystemStringPropertySetValue = systemCollectionsGenericLinkedListNodeSystemStringPropertySetValue;
	Plugin::SystemRuntimeCompilerServicesStrongBoxSystemStringConstructorSystemString = systemRuntimeCompilerServicesStrongBoxSystemStringConstructorSystemString;
	Plugin::SystemRuntimeCompilerServicesStrongBoxSystemStringFieldGetValue = systemRuntimeCompilerServicesStrongBoxSystemStringFieldGetValue;
	Plugin::SystemRuntimeCompilerServicesStrongBoxSystemStringFieldSetValue = systemRuntimeCompilerServicesStrongBoxSystemStringFieldSetValue;
	Plugin::SystemExceptionConstructorSystemString = systemExceptionConstructorSystemString;
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
	Plugin::unhandledCsharpException = System::Exception(handle);
}

/*BEGIN MONOBEHAVIOUR MESSAGES*/
DLLEXPORT void TestScriptAwake(int32_t thisHandle)
{
	MyGame::MonoBehaviours::TestScript thiz(thisHandle);
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
	MyGame::MonoBehaviours::TestScript thiz(thisHandle);
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
	MyGame::MonoBehaviours::TestScript thiz(thisHandle);
	UnityEngine::Collision param0(param0Handle);
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
	MyGame::MonoBehaviours::TestScript thiz(thisHandle);
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

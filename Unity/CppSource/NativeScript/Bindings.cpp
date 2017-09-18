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

// For std::forward
#include <utility>

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
	UnityEngine::Vector3 (*UnityEngineRaycastHitPropertyGetPoint)(int32_t thisHandle);
	void (*UnityEngineRaycastHitPropertySetPoint)(int32_t thisHandle, UnityEngine::Vector3& value);
	int32_t (*UnityEngineRaycastHitPropertyGetTransform)(int32_t thisHandle);
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
	/*END FUNCTION POINTERS*/
}

////////////////////////////////////////////////////////////////
// Reference counting of managed objects
////////////////////////////////////////////////////////////////

namespace Plugin
{
	int32_t managedObjectsRefCountLen;
	int32_t* managedObjectRefCounts;

	void ReferenceManagedObject(int32_t handle)
	{
		assert(handle >= 0 && handle < managedObjectsRefCountLen);
		if (handle != 0)
		{
			managedObjectRefCounts[handle]++;
		}
	}

	void DereferenceManagedObject(int32_t handle)
	{
		assert(handle >= 0 && handle < managedObjectsRefCountLen);
		if (handle != 0)
		{
			int32_t numRemain = --managedObjectRefCounts[handle];
			if (numRemain == 0)
			{
				ReleaseObject(handle);
			}
		}
	}
}

////////////////////////////////////////////////////////////////
// Mirrors of C# types. These wrap the C# functions to present
// a similiar API as in C#.
////////////////////////////////////////////////////////////////

namespace System
{
	Object::Object(int32_t handle)
	{
		Handle = handle;
		if (handle)
		{
			Plugin::ReferenceManagedObject(handle);
		}
	}
	
	Object::Object(const Object& other)
	{
		Handle = other.Handle;
		if (Handle)
		{
			Plugin::ReferenceManagedObject(Handle);
		}
	}
	
	Object::Object(Object&& other)
	{
		Handle = other.Handle;
		other.Handle = 0;
	}
	
	void Object::SetHandle(int32_t handle)
	{
		if (Handle != handle)
		{
			if (Handle)
			{
				Plugin::DereferenceManagedObject(Handle);
			}
			Handle = handle;
			if (handle)
			{
				Plugin::ReferenceManagedObject(handle);
			}
		}
	}
	
	Object::operator bool() const
	{
		return Handle != 0;
	}
	
	bool Object::operator==(const Object& other) const
	{
		return Handle == other.Handle;
	}
	
	bool Object::operator!=(const Object& other) const
	{
		return Handle != other.Handle;
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
	
	ValueType::ValueType(const ValueType& other)
		: Object(other)
	{
	}
	
	ValueType::ValueType(ValueType&& other)
		: Object(std::forward<ValueType>(other))
	{
	}
	
	ValueType::~ValueType()
	{
		if (Handle)
		{
			Plugin::DereferenceManagedObject(Handle);
		}
	}
	
	ValueType& ValueType::operator=(const ValueType& other)
	{
		SetHandle(other.Handle);
		return *this;
	}
	ValueType& ValueType::operator=(std::nullptr_t other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedObject(Handle);
			Handle = 0;
		}
		return *this;
	}
	
	ValueType& ValueType::operator=(ValueType&& other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedObject(Handle);
		}
		Handle = other.Handle;
		other.Handle = 0;
		return *this;
	}
	
	String::String(std::nullptr_t n)
		: Object(0)
	{
	}
	
	String::String(int32_t handle)
		: Object(handle)
	{
	}
	
	String::String(const String& other)
		: Object(other)
	{
	}
	
	String::String(String&& other)
		: Object(std::forward<String>(other))
	{
	}
	
	String::~String()
	{
		if (Handle)
		{
			Plugin::DereferenceManagedObject(Handle);
		}
	}
	
	String& String::operator=(const String& other)
	{
		SetHandle(other.Handle);
		return *this;
	}
	String& String::operator=(std::nullptr_t other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedObject(Handle);
			Handle = 0;
		}
		return *this;
	}
	
	String& String::operator=(String&& other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedObject(Handle);
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
		}
		
		Stopwatch::Stopwatch(const Stopwatch& other)
			: System::Object(other)
		{
		}
		
		Stopwatch::Stopwatch(Stopwatch&& other)
			: System::Object(std::forward<Stopwatch>(other))
		{
		}
		
		Stopwatch::~Stopwatch()
		{
			if (Handle)
			{
				Plugin::DereferenceManagedObject(Handle);
			}
		}
		
		Stopwatch& Stopwatch::operator=(const Stopwatch& other)
		{
			SetHandle(other.Handle);
			return *this;
		}
		
		Stopwatch& Stopwatch::operator=(std::nullptr_t other)
		{
			if (Handle)
			{
				Plugin::DereferenceManagedObject(Handle);
				Handle = 0;
			}
			return *this;
		}
		
		Stopwatch& Stopwatch::operator=(Stopwatch&& other)
		{
			if (Handle)
			{
				Plugin::DereferenceManagedObject(Handle);
			}
			Handle = other.Handle;
			other.Handle = 0;
			return *this;
		}
		
		Stopwatch::Stopwatch()
			 : System::Object(0)
		{
			auto returnValue = Plugin::SystemDiagnosticsStopwatchConstructor();
			SetHandle(returnValue);
		}
		
		int64_t Stopwatch::GetElapsedMilliseconds()
		{
			auto returnValue = Plugin::SystemDiagnosticsStopwatchPropertyGetElapsedMilliseconds(Handle);
			return returnValue;
		}
		
		void Stopwatch::Start()
		{
			Plugin::SystemDiagnosticsStopwatchMethodStart(Handle);
		}
	
		void Stopwatch::Reset()
		{
			Plugin::SystemDiagnosticsStopwatchMethodReset(Handle);
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
	}
	
	Object::Object(const Object& other)
		: System::Object(other)
	{
	}
	
	Object::Object(Object&& other)
		: System::Object(std::forward<Object>(other))
	{
	}
	
	Object::~Object()
	{
		if (Handle)
		{
			Plugin::DereferenceManagedObject(Handle);
		}
	}
	
	Object& Object::operator=(const Object& other)
	{
		SetHandle(other.Handle);
		return *this;
	}
	
	Object& Object::operator=(std::nullptr_t other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedObject(Handle);
			Handle = 0;
		}
		return *this;
	}
	
	Object& Object::operator=(Object&& other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedObject(Handle);
		}
		Handle = other.Handle;
		other.Handle = 0;
		return *this;
	}
	
	System::String Object::GetName()
	{
		auto returnValue = Plugin::UnityEngineObjectPropertyGetName(Handle);
		return returnValue;
	}
	
	void Object::SetName(System::String value)
	{
		Plugin::UnityEngineObjectPropertySetName(Handle, value.Handle);
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
	}
	
	GameObject::GameObject(const GameObject& other)
		: UnityEngine::Object(other)
	{
	}
	
	GameObject::GameObject(GameObject&& other)
		: UnityEngine::Object(std::forward<GameObject>(other))
	{
	}
	
	GameObject::~GameObject()
	{
		if (Handle)
		{
			Plugin::DereferenceManagedObject(Handle);
		}
	}
	
	GameObject& GameObject::operator=(const GameObject& other)
	{
		SetHandle(other.Handle);
		return *this;
	}
	
	GameObject& GameObject::operator=(std::nullptr_t other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedObject(Handle);
			Handle = 0;
		}
		return *this;
	}
	
	GameObject& GameObject::operator=(GameObject&& other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedObject(Handle);
		}
		Handle = other.Handle;
		other.Handle = 0;
		return *this;
	}
	
	GameObject::GameObject()
		 : UnityEngine::Object(0)
	{
		auto returnValue = Plugin::UnityEngineGameObjectConstructor();
		SetHandle(returnValue);
	}
	
	GameObject::GameObject(System::String name)
		 : UnityEngine::Object(0)
	{
		auto returnValue = Plugin::UnityEngineGameObjectConstructorSystemString(name.Handle);
		SetHandle(returnValue);
	}
	
	UnityEngine::Transform GameObject::GetTransform()
	{
		auto returnValue = Plugin::UnityEngineGameObjectPropertyGetTransform(Handle);
		return returnValue;
	}
	
	UnityEngine::GameObject GameObject::Find(System::String name)
	{
		auto returnValue = Plugin::UnityEngineGameObjectMethodFindSystemString(name.Handle);
		return returnValue;
	}
	
	template<> MyGame::MonoBehaviours::TestScript GameObject::AddComponent<MyGame::MonoBehaviours::TestScript>()
	{
		auto returnValue = Plugin::UnityEngineGameObjectMethodAddComponentMyGameMonoBehavioursTestScript(Handle);
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
	}
	
	Component::Component(const Component& other)
		: UnityEngine::Object(other)
	{
	}
	
	Component::Component(Component&& other)
		: UnityEngine::Object(std::forward<Component>(other))
	{
	}
	
	Component::~Component()
	{
		if (Handle)
		{
			Plugin::DereferenceManagedObject(Handle);
		}
	}
	
	Component& Component::operator=(const Component& other)
	{
		SetHandle(other.Handle);
		return *this;
	}
	
	Component& Component::operator=(std::nullptr_t other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedObject(Handle);
			Handle = 0;
		}
		return *this;
	}
	
	Component& Component::operator=(Component&& other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedObject(Handle);
		}
		Handle = other.Handle;
		other.Handle = 0;
		return *this;
	}
	
	UnityEngine::Transform Component::GetTransform()
	{
		auto returnValue = Plugin::UnityEngineComponentPropertyGetTransform(Handle);
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
	}
	
	Transform::Transform(const Transform& other)
		: UnityEngine::Component(other)
	{
	}
	
	Transform::Transform(Transform&& other)
		: UnityEngine::Component(std::forward<Transform>(other))
	{
	}
	
	Transform::~Transform()
	{
		if (Handle)
		{
			Plugin::DereferenceManagedObject(Handle);
		}
	}
	
	Transform& Transform::operator=(const Transform& other)
	{
		SetHandle(other.Handle);
		return *this;
	}
	
	Transform& Transform::operator=(std::nullptr_t other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedObject(Handle);
			Handle = 0;
		}
		return *this;
	}
	
	Transform& Transform::operator=(Transform&& other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedObject(Handle);
		}
		Handle = other.Handle;
		other.Handle = 0;
		return *this;
	}
	
	UnityEngine::Vector3 Transform::GetPosition()
	{
		auto returnValue = Plugin::UnityEngineTransformPropertyGetPosition(Handle);
		return returnValue;
	}
	
	void Transform::SetPosition(UnityEngine::Vector3& value)
	{
		Plugin::UnityEngineTransformPropertySetPosition(Handle, value);
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
	}
	
	Debug::Debug(const Debug& other)
		: System::Object(other)
	{
	}
	
	Debug::Debug(Debug&& other)
		: System::Object(std::forward<Debug>(other))
	{
	}
	
	Debug::~Debug()
	{
		if (Handle)
		{
			Plugin::DereferenceManagedObject(Handle);
		}
	}
	
	Debug& Debug::operator=(const Debug& other)
	{
		SetHandle(other.Handle);
		return *this;
	}
	
	Debug& Debug::operator=(std::nullptr_t other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedObject(Handle);
			Handle = 0;
		}
		return *this;
	}
	
	Debug& Debug::operator=(Debug&& other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedObject(Handle);
		}
		Handle = other.Handle;
		other.Handle = 0;
		return *this;
	}
	
	void Debug::Log(System::Object message)
	{
		Plugin::UnityEngineDebugMethodLogSystemObject(message.Handle);
	}
}

namespace UnityEngine
{
	namespace Assertions
	{
		System::Boolean Assert::GetRaiseExceptions()
		{
			auto returnValue = Plugin::UnityEngineAssertionsAssertFieldGetRaiseExceptions();
			return returnValue;
		}
		
		void Assert::SetRaiseExceptions(System::Boolean value)
		{
			Plugin::UnityEngineAssertionsAssertFieldSetRaiseExceptions(value);
		}
		
		template<> void Assert::AreEqual<System::String>(System::String expected, System::String actual)
		{
			Plugin::UnityEngineAssertionsAssertMethodAreEqualSystemStringSystemString_SystemString(expected.Handle, actual.Handle);
		}
	
		template<> void Assert::AreEqual<UnityEngine::GameObject>(UnityEngine::GameObject expected, UnityEngine::GameObject actual)
		{
			Plugin::UnityEngineAssertionsAssertMethodAreEqualUnityEngineGameObjectUnityEngineGameObject_UnityEngineGameObject(expected.Handle, actual.Handle);
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
	}
	
	Collision::Collision(const Collision& other)
		: System::Object(other)
	{
	}
	
	Collision::Collision(Collision&& other)
		: System::Object(std::forward<Collision>(other))
	{
	}
	
	Collision::~Collision()
	{
		if (Handle)
		{
			Plugin::DereferenceManagedObject(Handle);
		}
	}
	
	Collision& Collision::operator=(const Collision& other)
	{
		SetHandle(other.Handle);
		return *this;
	}
	
	Collision& Collision::operator=(std::nullptr_t other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedObject(Handle);
			Handle = 0;
		}
		return *this;
	}
	
	Collision& Collision::operator=(Collision&& other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedObject(Handle);
		}
		Handle = other.Handle;
		other.Handle = 0;
		return *this;
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
	}
	
	Behaviour::Behaviour(const Behaviour& other)
		: UnityEngine::Component(other)
	{
	}
	
	Behaviour::Behaviour(Behaviour&& other)
		: UnityEngine::Component(std::forward<Behaviour>(other))
	{
	}
	
	Behaviour::~Behaviour()
	{
		if (Handle)
		{
			Plugin::DereferenceManagedObject(Handle);
		}
	}
	
	Behaviour& Behaviour::operator=(const Behaviour& other)
	{
		SetHandle(other.Handle);
		return *this;
	}
	
	Behaviour& Behaviour::operator=(std::nullptr_t other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedObject(Handle);
			Handle = 0;
		}
		return *this;
	}
	
	Behaviour& Behaviour::operator=(Behaviour&& other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedObject(Handle);
		}
		Handle = other.Handle;
		other.Handle = 0;
		return *this;
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
	}
	
	MonoBehaviour::MonoBehaviour(const MonoBehaviour& other)
		: UnityEngine::Behaviour(other)
	{
	}
	
	MonoBehaviour::MonoBehaviour(MonoBehaviour&& other)
		: UnityEngine::Behaviour(std::forward<MonoBehaviour>(other))
	{
	}
	
	MonoBehaviour::~MonoBehaviour()
	{
		if (Handle)
		{
			Plugin::DereferenceManagedObject(Handle);
		}
	}
	
	MonoBehaviour& MonoBehaviour::operator=(const MonoBehaviour& other)
	{
		SetHandle(other.Handle);
		return *this;
	}
	
	MonoBehaviour& MonoBehaviour::operator=(std::nullptr_t other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedObject(Handle);
			Handle = 0;
		}
		return *this;
	}
	
	MonoBehaviour& MonoBehaviour::operator=(MonoBehaviour&& other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedObject(Handle);
		}
		Handle = other.Handle;
		other.Handle = 0;
		return *this;
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
	}
	
	AudioSettings::AudioSettings(const AudioSettings& other)
		: System::Object(other)
	{
	}
	
	AudioSettings::AudioSettings(AudioSettings&& other)
		: System::Object(std::forward<AudioSettings>(other))
	{
	}
	
	AudioSettings::~AudioSettings()
	{
		if (Handle)
		{
			Plugin::DereferenceManagedObject(Handle);
		}
	}
	
	AudioSettings& AudioSettings::operator=(const AudioSettings& other)
	{
		SetHandle(other.Handle);
		return *this;
	}
	
	AudioSettings& AudioSettings::operator=(std::nullptr_t other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedObject(Handle);
			Handle = 0;
		}
		return *this;
	}
	
	AudioSettings& AudioSettings::operator=(AudioSettings&& other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedObject(Handle);
		}
		Handle = other.Handle;
		other.Handle = 0;
		return *this;
	}
	
	void AudioSettings::GetDSPBufferSize(int32_t* bufferLength, int32_t* numBuffers)
	{
		Plugin::UnityEngineAudioSettingsMethodGetDSPBufferSizeSystemInt32_SystemInt32(bufferLength, numBuffers);
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
		}
		
		NetworkTransport::NetworkTransport(const NetworkTransport& other)
			: System::Object(other)
		{
		}
		
		NetworkTransport::NetworkTransport(NetworkTransport&& other)
			: System::Object(std::forward<NetworkTransport>(other))
		{
		}
		
		NetworkTransport::~NetworkTransport()
		{
			if (Handle)
			{
				Plugin::DereferenceManagedObject(Handle);
			}
		}
		
		NetworkTransport& NetworkTransport::operator=(const NetworkTransport& other)
		{
			SetHandle(other.Handle);
			return *this;
		}
		
		NetworkTransport& NetworkTransport::operator=(std::nullptr_t other)
		{
			if (Handle)
			{
				Plugin::DereferenceManagedObject(Handle);
				Handle = 0;
			}
			return *this;
		}
		
		NetworkTransport& NetworkTransport::operator=(NetworkTransport&& other)
		{
			if (Handle)
			{
				Plugin::DereferenceManagedObject(Handle);
			}
			Handle = other.Handle;
			other.Handle = 0;
			return *this;
		}
		
		void NetworkTransport::GetBroadcastConnectionInfo(int32_t hostId, System::String* address, int32_t* port, uint8_t* error)
		{
			int32_t addressHandle = address->Handle;
			Plugin::UnityEngineNetworkingNetworkTransportMethodGetBroadcastConnectionInfoSystemInt32_SystemString_SystemInt32_SystemByte(hostId, &addressHandle, port, error);
			address->SetHandle(addressHandle);
		}
	
		void NetworkTransport::Init()
		{
			Plugin::UnityEngineNetworkingNetworkTransportMethodInit();
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
		*this = returnValue;
	}
	
	float Vector3::GetMagnitude()
	{
		auto returnValue = Plugin::UnityEngineVector3PropertyGetMagnitude(this);
		return returnValue;
	}
	
	void Vector3::Set(float newX, float newY, float newZ)
	{
		Plugin::UnityEngineVector3MethodSetSystemSingle_SystemSingle_SystemSingle(this, newX, newY, newZ);
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
	}
	
	RaycastHit::RaycastHit(const RaycastHit& other)
		: System::ValueType(other)
	{
	}
	
	RaycastHit::RaycastHit(RaycastHit&& other)
		: System::ValueType(std::forward<RaycastHit>(other))
	{
	}
	
	RaycastHit::~RaycastHit()
	{
		if (Handle)
		{
			Plugin::DereferenceManagedObject(Handle);
		}
	}
	
	RaycastHit& RaycastHit::operator=(const RaycastHit& other)
	{
		SetHandle(other.Handle);
		return *this;
	}
	
	RaycastHit& RaycastHit::operator=(std::nullptr_t other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedObject(Handle);
			Handle = 0;
		}
		return *this;
	}
	
	RaycastHit& RaycastHit::operator=(RaycastHit&& other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedObject(Handle);
		}
		Handle = other.Handle;
		other.Handle = 0;
		return *this;
	}
	
	UnityEngine::Vector3 RaycastHit::GetPoint()
	{
		auto returnValue = Plugin::UnityEngineRaycastHitPropertyGetPoint(Handle);
		return returnValue;
	}
	
	void RaycastHit::SetPoint(UnityEngine::Vector3& value)
	{
		Plugin::UnityEngineRaycastHitPropertySetPoint(Handle, value);
	}
	
	UnityEngine::Transform RaycastHit::GetTransform()
	{
		auto returnValue = Plugin::UnityEngineRaycastHitPropertyGetTransform(Handle);
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
			}
			
			KeyValuePair<System::String, double>::KeyValuePair(const KeyValuePair<System::String, double>& other)
				: System::ValueType(other)
			{
			}
			
			KeyValuePair<System::String, double>::KeyValuePair(KeyValuePair<System::String, double>&& other)
				: System::ValueType(std::forward<KeyValuePair<System::String, double>>(other))
			{
			}
			
			KeyValuePair<System::String, double>::~KeyValuePair<System::String, double>()
			{
				if (Handle)
				{
					Plugin::DereferenceManagedObject(Handle);
				}
			}
			
			KeyValuePair<System::String, double>& KeyValuePair<System::String, double>::operator=(const KeyValuePair<System::String, double>& other)
			{
				SetHandle(other.Handle);
				return *this;
			}
			
			KeyValuePair<System::String, double>& KeyValuePair<System::String, double>::operator=(std::nullptr_t other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedObject(Handle);
					Handle = 0;
				}
				return *this;
			}
			
			KeyValuePair<System::String, double>& KeyValuePair<System::String, double>::operator=(KeyValuePair<System::String, double>&& other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedObject(Handle);
				}
				Handle = other.Handle;
				other.Handle = 0;
				return *this;
			}
			
			KeyValuePair<System::String, double>::KeyValuePair(System::String key, double value)
				 : System::ValueType(0)
			{
				auto returnValue = Plugin::SystemCollectionsGenericKeyValuePairSystemString_SystemDoubleConstructorSystemString_SystemDouble(key.Handle, value);
				SetHandle(returnValue);
			}
			
			System::String KeyValuePair<System::String, double>::GetKey()
			{
				auto returnValue = Plugin::SystemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetKey(Handle);
				return returnValue;
			}
			
			double KeyValuePair<System::String, double>::GetValue()
			{
				auto returnValue = Plugin::SystemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetValue(Handle);
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
			}
			
			List<System::String>::List(const List<System::String>& other)
				: System::Object(other)
			{
			}
			
			List<System::String>::List(List<System::String>&& other)
				: System::Object(std::forward<List<System::String>>(other))
			{
			}
			
			List<System::String>::~List<System::String>()
			{
				if (Handle)
				{
					Plugin::DereferenceManagedObject(Handle);
				}
			}
			
			List<System::String>& List<System::String>::operator=(const List<System::String>& other)
			{
				SetHandle(other.Handle);
				return *this;
			}
			
			List<System::String>& List<System::String>::operator=(std::nullptr_t other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedObject(Handle);
					Handle = 0;
				}
				return *this;
			}
			
			List<System::String>& List<System::String>::operator=(List<System::String>&& other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedObject(Handle);
				}
				Handle = other.Handle;
				other.Handle = 0;
				return *this;
			}
			
			List<System::String>::List()
				 : System::Object(0)
			{
				auto returnValue = Plugin::SystemCollectionsGenericListSystemStringConstructor();
				SetHandle(returnValue);
			}
			
			void List<System::String>::Add(System::String item)
			{
				Plugin::SystemCollectionsGenericListSystemStringMethodAddSystemString(Handle, item.Handle);
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
			}
			
			LinkedListNode<System::String>::LinkedListNode(const LinkedListNode<System::String>& other)
				: System::Object(other)
			{
			}
			
			LinkedListNode<System::String>::LinkedListNode(LinkedListNode<System::String>&& other)
				: System::Object(std::forward<LinkedListNode<System::String>>(other))
			{
			}
			
			LinkedListNode<System::String>::~LinkedListNode<System::String>()
			{
				if (Handle)
				{
					Plugin::DereferenceManagedObject(Handle);
				}
			}
			
			LinkedListNode<System::String>& LinkedListNode<System::String>::operator=(const LinkedListNode<System::String>& other)
			{
				SetHandle(other.Handle);
				return *this;
			}
			
			LinkedListNode<System::String>& LinkedListNode<System::String>::operator=(std::nullptr_t other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedObject(Handle);
					Handle = 0;
				}
				return *this;
			}
			
			LinkedListNode<System::String>& LinkedListNode<System::String>::operator=(LinkedListNode<System::String>&& other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedObject(Handle);
				}
				Handle = other.Handle;
				other.Handle = 0;
				return *this;
			}
			
			LinkedListNode<System::String>::LinkedListNode(System::String value)
				 : System::Object(0)
			{
				auto returnValue = Plugin::SystemCollectionsGenericLinkedListNodeSystemStringConstructorSystemString(value.Handle);
				SetHandle(returnValue);
			}
			
			System::String LinkedListNode<System::String>::GetValue()
			{
				auto returnValue = Plugin::SystemCollectionsGenericLinkedListNodeSystemStringPropertyGetValue(Handle);
				return returnValue;
			}
			
			void LinkedListNode<System::String>::SetValue(System::String value)
			{
				Plugin::SystemCollectionsGenericLinkedListNodeSystemStringPropertySetValue(Handle, value.Handle);
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
			}
			
			StrongBox<System::String>::StrongBox(const StrongBox<System::String>& other)
				: System::Object(other)
			{
			}
			
			StrongBox<System::String>::StrongBox(StrongBox<System::String>&& other)
				: System::Object(std::forward<StrongBox<System::String>>(other))
			{
			}
			
			StrongBox<System::String>::~StrongBox<System::String>()
			{
				if (Handle)
				{
					Plugin::DereferenceManagedObject(Handle);
				}
			}
			
			StrongBox<System::String>& StrongBox<System::String>::operator=(const StrongBox<System::String>& other)
			{
				SetHandle(other.Handle);
				return *this;
			}
			
			StrongBox<System::String>& StrongBox<System::String>::operator=(std::nullptr_t other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedObject(Handle);
					Handle = 0;
				}
				return *this;
			}
			
			StrongBox<System::String>& StrongBox<System::String>::operator=(StrongBox<System::String>&& other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedObject(Handle);
				}
				Handle = other.Handle;
				other.Handle = 0;
				return *this;
			}
			
			StrongBox<System::String>::StrongBox(System::String value)
				 : System::Object(0)
			{
				auto returnValue = Plugin::SystemRuntimeCompilerServicesStrongBoxSystemStringConstructorSystemString(value.Handle);
				SetHandle(returnValue);
			}
			
			System::String StrongBox<System::String>::GetValue()
			{
				auto returnValue = Plugin::SystemRuntimeCompilerServicesStrongBoxSystemStringFieldGetValue(Handle);
				return returnValue;
			}
			
			void StrongBox<System::String>::SetValue(System::String value)
			{
				Plugin::SystemRuntimeCompilerServicesStrongBoxSystemStringFieldSetValue(Handle, value.Handle);
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
			}
			
			Collection<int32_t>::Collection(const Collection<int32_t>& other)
				: System::Object(other)
			{
			}
			
			Collection<int32_t>::Collection(Collection<int32_t>&& other)
				: System::Object(std::forward<Collection<int32_t>>(other))
			{
			}
			
			Collection<int32_t>::~Collection<int32_t>()
			{
				if (Handle)
				{
					Plugin::DereferenceManagedObject(Handle);
				}
			}
			
			Collection<int32_t>& Collection<int32_t>::operator=(const Collection<int32_t>& other)
			{
				SetHandle(other.Handle);
				return *this;
			}
			
			Collection<int32_t>& Collection<int32_t>::operator=(std::nullptr_t other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedObject(Handle);
					Handle = 0;
				}
				return *this;
			}
			
			Collection<int32_t>& Collection<int32_t>::operator=(Collection<int32_t>&& other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedObject(Handle);
				}
				Handle = other.Handle;
				other.Handle = 0;
				return *this;
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
			}
			
			KeyedCollection<System::String, int32_t>::KeyedCollection(const KeyedCollection<System::String, int32_t>& other)
				: System::Collections::ObjectModel::Collection<int32_t>(other)
			{
			}
			
			KeyedCollection<System::String, int32_t>::KeyedCollection(KeyedCollection<System::String, int32_t>&& other)
				: System::Collections::ObjectModel::Collection<int32_t>(std::forward<KeyedCollection<System::String, int32_t>>(other))
			{
			}
			
			KeyedCollection<System::String, int32_t>::~KeyedCollection<System::String, int32_t>()
			{
				if (Handle)
				{
					Plugin::DereferenceManagedObject(Handle);
				}
			}
			
			KeyedCollection<System::String, int32_t>& KeyedCollection<System::String, int32_t>::operator=(const KeyedCollection<System::String, int32_t>& other)
			{
				SetHandle(other.Handle);
				return *this;
			}
			
			KeyedCollection<System::String, int32_t>& KeyedCollection<System::String, int32_t>::operator=(std::nullptr_t other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedObject(Handle);
					Handle = 0;
				}
				return *this;
			}
			
			KeyedCollection<System::String, int32_t>& KeyedCollection<System::String, int32_t>::operator=(KeyedCollection<System::String, int32_t>&& other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedObject(Handle);
				}
				Handle = other.Handle;
				other.Handle = 0;
				return *this;
			}
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
		}
		
		TestScript::TestScript(const TestScript& other)
			: UnityEngine::MonoBehaviour(other)
		{
		}
		
		TestScript::TestScript(TestScript&& other)
			: UnityEngine::MonoBehaviour(std::forward<TestScript>(other))
		{
		}
		
		TestScript::~TestScript()
		{
			if (Handle)
			{
				Plugin::DereferenceManagedObject(Handle);
			}
		}
		
		TestScript& TestScript::operator=(const TestScript& other)
		{
			SetHandle(other.Handle);
			return *this;
		}
		
		TestScript& TestScript::operator=(std::nullptr_t other)
		{
			if (Handle)
			{
				Plugin::DereferenceManagedObject(Handle);
				Handle = 0;
			}
			return *this;
		}
		
		TestScript& TestScript::operator=(TestScript&& other)
		{
			if (Handle)
			{
				Plugin::DereferenceManagedObject(Handle);
			}
			Handle = other.Handle;
			other.Handle = 0;
			return *this;
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
	UnityEngine::Vector3 (*unityEngineRaycastHitPropertyGetPoint)(int32_t thisHandle),
	void (*unityEngineRaycastHitPropertySetPoint)(int32_t thisHandle, UnityEngine::Vector3& value),
	int32_t (*unityEngineRaycastHitPropertyGetTransform)(int32_t thisHandle),
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
	void (*systemRuntimeCompilerServicesStrongBoxSystemStringFieldSetValue)(int32_t thisHandle, int32_t valueHandle)
	/*END INIT PARAMS*/)
{
	using namespace Plugin;
	
	// Init managed object ref counting
	managedObjectsRefCountLen = maxManagedObjects;
	managedObjectRefCounts = (int32_t*)calloc(
		maxManagedObjects,
		sizeof(int32_t));
	
	// Init pointers to C# functions
	StringNew = stringNew;
	ReleaseObject = releaseObject;
	/*BEGIN INIT BODY*/
	SystemDiagnosticsStopwatchConstructor = systemDiagnosticsStopwatchConstructor;
	SystemDiagnosticsStopwatchPropertyGetElapsedMilliseconds = systemDiagnosticsStopwatchPropertyGetElapsedMilliseconds;
	SystemDiagnosticsStopwatchMethodStart = systemDiagnosticsStopwatchMethodStart;
	SystemDiagnosticsStopwatchMethodReset = systemDiagnosticsStopwatchMethodReset;
	UnityEngineObjectPropertyGetName = unityEngineObjectPropertyGetName;
	UnityEngineObjectPropertySetName = unityEngineObjectPropertySetName;
	UnityEngineGameObjectConstructor = unityEngineGameObjectConstructor;
	UnityEngineGameObjectConstructorSystemString = unityEngineGameObjectConstructorSystemString;
	UnityEngineGameObjectPropertyGetTransform = unityEngineGameObjectPropertyGetTransform;
	UnityEngineGameObjectMethodFindSystemString = unityEngineGameObjectMethodFindSystemString;
	UnityEngineGameObjectMethodAddComponentMyGameMonoBehavioursTestScript = unityEngineGameObjectMethodAddComponentMyGameMonoBehavioursTestScript;
	UnityEngineComponentPropertyGetTransform = unityEngineComponentPropertyGetTransform;
	UnityEngineTransformPropertyGetPosition = unityEngineTransformPropertyGetPosition;
	UnityEngineTransformPropertySetPosition = unityEngineTransformPropertySetPosition;
	UnityEngineDebugMethodLogSystemObject = unityEngineDebugMethodLogSystemObject;
	UnityEngineAssertionsAssertFieldGetRaiseExceptions = unityEngineAssertionsAssertFieldGetRaiseExceptions;
	UnityEngineAssertionsAssertFieldSetRaiseExceptions = unityEngineAssertionsAssertFieldSetRaiseExceptions;
	UnityEngineAssertionsAssertMethodAreEqualSystemStringSystemString_SystemString = unityEngineAssertionsAssertMethodAreEqualSystemStringSystemString_SystemString;
	UnityEngineAssertionsAssertMethodAreEqualUnityEngineGameObjectUnityEngineGameObject_UnityEngineGameObject = unityEngineAssertionsAssertMethodAreEqualUnityEngineGameObjectUnityEngineGameObject_UnityEngineGameObject;
	UnityEngineAudioSettingsMethodGetDSPBufferSizeSystemInt32_SystemInt32 = unityEngineAudioSettingsMethodGetDSPBufferSizeSystemInt32_SystemInt32;
	UnityEngineNetworkingNetworkTransportMethodGetBroadcastConnectionInfoSystemInt32_SystemString_SystemInt32_SystemByte = unityEngineNetworkingNetworkTransportMethodGetBroadcastConnectionInfoSystemInt32_SystemString_SystemInt32_SystemByte;
	UnityEngineNetworkingNetworkTransportMethodInit = unityEngineNetworkingNetworkTransportMethodInit;
	UnityEngineVector3ConstructorSystemSingle_SystemSingle_SystemSingle = unityEngineVector3ConstructorSystemSingle_SystemSingle_SystemSingle;
	UnityEngineVector3PropertyGetMagnitude = unityEngineVector3PropertyGetMagnitude;
	UnityEngineVector3MethodSetSystemSingle_SystemSingle_SystemSingle = unityEngineVector3MethodSetSystemSingle_SystemSingle_SystemSingle;
	UnityEngineRaycastHitPropertyGetPoint = unityEngineRaycastHitPropertyGetPoint;
	UnityEngineRaycastHitPropertySetPoint = unityEngineRaycastHitPropertySetPoint;
	UnityEngineRaycastHitPropertyGetTransform = unityEngineRaycastHitPropertyGetTransform;
	SystemCollectionsGenericKeyValuePairSystemString_SystemDoubleConstructorSystemString_SystemDouble = systemCollectionsGenericKeyValuePairSystemString_SystemDoubleConstructorSystemString_SystemDouble;
	SystemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetKey = systemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetKey;
	SystemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetValue = systemCollectionsGenericKeyValuePairSystemString_SystemDoublePropertyGetValue;
	SystemCollectionsGenericListSystemStringConstructor = systemCollectionsGenericListSystemStringConstructor;
	SystemCollectionsGenericListSystemStringMethodAddSystemString = systemCollectionsGenericListSystemStringMethodAddSystemString;
	SystemCollectionsGenericLinkedListNodeSystemStringConstructorSystemString = systemCollectionsGenericLinkedListNodeSystemStringConstructorSystemString;
	SystemCollectionsGenericLinkedListNodeSystemStringPropertyGetValue = systemCollectionsGenericLinkedListNodeSystemStringPropertyGetValue;
	SystemCollectionsGenericLinkedListNodeSystemStringPropertySetValue = systemCollectionsGenericLinkedListNodeSystemStringPropertySetValue;
	SystemRuntimeCompilerServicesStrongBoxSystemStringConstructorSystemString = systemRuntimeCompilerServicesStrongBoxSystemStringConstructorSystemString;
	SystemRuntimeCompilerServicesStrongBoxSystemStringFieldGetValue = systemRuntimeCompilerServicesStrongBoxSystemStringFieldGetValue;
	SystemRuntimeCompilerServicesStrongBoxSystemStringFieldSetValue = systemRuntimeCompilerServicesStrongBoxSystemStringFieldSetValue;
	/*END INIT BODY*/
	
	PluginMain();
}

/*BEGIN MONOBEHAVIOUR MESSAGES*/
DLLEXPORT void TestScriptAwake(int32_t thisHandle)
{
	MyGame::MonoBehaviours::TestScript thiz(thisHandle);
	thiz.Awake();
}

DLLEXPORT void TestScriptOnAnimatorIK(int32_t thisHandle, int32_t param0)
{
	MyGame::MonoBehaviours::TestScript thiz(thisHandle);
	thiz.OnAnimatorIK(param0);
}

DLLEXPORT void TestScriptOnCollisionEnter(int32_t thisHandle, int32_t param0Handle)
{
	MyGame::MonoBehaviours::TestScript thiz(thisHandle);
	UnityEngine::Collision param0(param0Handle);
	thiz.OnCollisionEnter(param0);
}

DLLEXPORT void TestScriptUpdate(int32_t thisHandle)
{
	MyGame::MonoBehaviours::TestScript thiz(thisHandle);
	thiz.Update();
}
/*END MONOBEHAVIOUR MESSAGES*/

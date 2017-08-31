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
	int32_t (*StopwatchConstructor)();
	int64_t (*StopwatchPropertyGetElapsedMilliseconds)(int32_t thisHandle);
	void (*StopwatchMethodStart)(int32_t thisHandle);
	void (*StopwatchMethodReset)(int32_t thisHandle);
	int32_t (*ObjectPropertyGetName)(int32_t thisHandle);
	void (*ObjectPropertySetName)(int32_t thisHandle, int32_t valueHandle);
	int32_t (*GameObjectConstructor)();
	int32_t (*GameObjectConstructorSystemString)(int32_t nameHandle);
	int32_t (*GameObjectPropertyGetTransform)(int32_t thisHandle);
	int32_t (*GameObjectMethodFindSystemString)(int32_t nameHandle);
	int32_t (*GameObjectMethodAddComponentMyGameMonoBehavioursTestScript)(int32_t thisHandle);
	int32_t (*ComponentPropertyGetTransform)(int32_t thisHandle);
	UnityEngine::Vector3 (*TransformPropertyGetPosition)(int32_t thisHandle);
	void (*TransformPropertySetPosition)(int32_t thisHandle, UnityEngine::Vector3 value);
	void (*DebugMethodLogSystemObject)(int32_t messageHandle);
	System::Boolean (*AssertFieldGetRaiseExceptions)();
	void (*AssertFieldSetRaiseExceptions)(System::Boolean value);
	void (*AudioSettingsMethodGetDSPBufferSizeSystemInt32_SystemInt32)(int32_t* bufferLength, int32_t* numBuffers);
	void (*NetworkTransportMethodGetBroadcastConnectionInfoSystemInt32_SystemString_SystemInt32_SystemByte)(int32_t hostId, int32_t* addressHandle, int32_t* port, uint8_t* error);
	void (*NetworkTransportMethodInit)();
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
	
	#define SYSTEM_OBJECT_LIFECYCLE_DEFINITION(ClassName, BaseClassName) \
		ClassName::ClassName(std::nullptr_t n) \
			: BaseClassName(0) \
		{ \
		} \
		\
		ClassName::ClassName(int32_t handle) \
			: BaseClassName(handle) \
		{ \
		} \
		\
		ClassName::ClassName(const ClassName& other) \
			: BaseClassName(other) \
		{ \
		} \
		\
		ClassName::ClassName(ClassName&& other) \
			: BaseClassName(std::forward<ClassName>(other)) \
		{ \
		} \
		\
		ClassName::~ClassName() \
		{ \
			if (Handle) \
			{ \
				Plugin::DereferenceManagedObject(Handle); \
			} \
		} \
		\
		ClassName& ClassName::operator=(const ClassName& other) \
		{ \
			SetHandle(other.Handle); \
			return *this; \
		} \
		ClassName& ClassName::operator=(std::nullptr_t other) \
		{ \
			if (Handle) \
			{ \
				Plugin::DereferenceManagedObject(Handle); \
				Handle = 0; \
			} \
			return *this; \
		} \
		\
		ClassName& ClassName::operator=(ClassName&& other) \
		{ \
			if (Handle) \
			{ \
				Plugin::DereferenceManagedObject(Handle); \
			} \
			Handle = other.Handle; \
			other.Handle = 0; \
			return *this; \
		}
	
	SYSTEM_OBJECT_LIFECYCLE_DEFINITION(String, System::Object)
	
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
		SYSTEM_OBJECT_LIFECYCLE_DEFINITION(Stopwatch, System::Object)
		
		Stopwatch::Stopwatch()
			 : System::Object(0)
		{
			auto returnValue = Plugin::StopwatchConstructor();
			SetHandle(returnValue);
		}
		
		int64_t Stopwatch::GetElapsedMilliseconds()
		{
			auto returnValue = Plugin::StopwatchPropertyGetElapsedMilliseconds(Handle);
			return returnValue;
		}
		
		void Stopwatch::Start()
		{
			Plugin::StopwatchMethodStart(Handle);
		}
	
		void Stopwatch::Reset()
		{
			Plugin::StopwatchMethodReset(Handle);
		}
	}
}

namespace UnityEngine
{
	SYSTEM_OBJECT_LIFECYCLE_DEFINITION(Object, System::Object)
	
	System::String Object::GetName()
	{
		auto returnValue = Plugin::ObjectPropertyGetName(Handle);
		return returnValue;
	}
	
	void Object::SetName(System::String value)
	{
		Plugin::ObjectPropertySetName(Handle, value.Handle);
	}
}

namespace UnityEngine
{
	SYSTEM_OBJECT_LIFECYCLE_DEFINITION(GameObject, UnityEngine::Object)
	
	GameObject::GameObject()
		 : UnityEngine::Object(0)
	{
		auto returnValue = Plugin::GameObjectConstructor();
		SetHandle(returnValue);
	}
	
	GameObject::GameObject(System::String name)
		 : UnityEngine::Object(0)
	{
		auto returnValue = Plugin::GameObjectConstructorSystemString(name.Handle);
		SetHandle(returnValue);
	}
	
	UnityEngine::Transform GameObject::GetTransform()
	{
		auto returnValue = Plugin::GameObjectPropertyGetTransform(Handle);
		return returnValue;
	}
	
	UnityEngine::GameObject GameObject::Find(System::String name)
	{
		auto returnValue = Plugin::GameObjectMethodFindSystemString(name.Handle);
		return returnValue;
	}
	
	template<> MyGame::MonoBehaviours::TestScript GameObject::AddComponent<MyGame::MonoBehaviours::TestScript>()
	{
		auto returnValue = Plugin::GameObjectMethodAddComponentMyGameMonoBehavioursTestScript(Handle);
		return returnValue;
	}
}

namespace UnityEngine
{
	SYSTEM_OBJECT_LIFECYCLE_DEFINITION(Component, UnityEngine::Object)
	
	UnityEngine::Transform Component::GetTransform()
	{
		auto returnValue = Plugin::ComponentPropertyGetTransform(Handle);
		return returnValue;
	}
}

namespace UnityEngine
{
	SYSTEM_OBJECT_LIFECYCLE_DEFINITION(Transform, UnityEngine::Component)
	
	UnityEngine::Vector3 Transform::GetPosition()
	{
		auto returnValue = Plugin::TransformPropertyGetPosition(Handle);
		return returnValue;
	}
	
	void Transform::SetPosition(UnityEngine::Vector3 value)
	{
		Plugin::TransformPropertySetPosition(Handle, value);
	}
}

namespace UnityEngine
{
	SYSTEM_OBJECT_LIFECYCLE_DEFINITION(Debug, System::Object)
	
	void Debug::Log(System::Object message)
	{
		Plugin::DebugMethodLogSystemObject(message.Handle);
	}
}

namespace UnityEngine
{
	namespace Assertions
	{
		System::Boolean Assert::GetRaiseExceptions()
		{
			auto returnValue = Plugin::AssertFieldGetRaiseExceptions();
			return returnValue;
		}
		
		void Assert::SetRaiseExceptions(System::Boolean value)
		{
			Plugin::AssertFieldSetRaiseExceptions(value);
		}
	}
}

namespace UnityEngine
{
	SYSTEM_OBJECT_LIFECYCLE_DEFINITION(Collision, System::Object)
}

namespace UnityEngine
{
	SYSTEM_OBJECT_LIFECYCLE_DEFINITION(Behaviour, UnityEngine::Component)
}

namespace UnityEngine
{
	SYSTEM_OBJECT_LIFECYCLE_DEFINITION(MonoBehaviour, UnityEngine::Behaviour)
}

namespace UnityEngine
{
	SYSTEM_OBJECT_LIFECYCLE_DEFINITION(AudioSettings, System::Object)
	
	void AudioSettings::GetDSPBufferSize(int32_t* bufferLength, int32_t* numBuffers)
	{
		Plugin::AudioSettingsMethodGetDSPBufferSizeSystemInt32_SystemInt32(bufferLength, numBuffers);
	}
}

namespace UnityEngine
{
	namespace Networking
	{
		SYSTEM_OBJECT_LIFECYCLE_DEFINITION(NetworkTransport, System::Object)
		
		void NetworkTransport::GetBroadcastConnectionInfo(int32_t hostId, System::String* address, int32_t* port, uint8_t* error)
		{
			int32_t addressHandle = address->Handle;
			Plugin::NetworkTransportMethodGetBroadcastConnectionInfoSystemInt32_SystemString_SystemInt32_SystemByte(hostId, &addressHandle, port, error);
			address->SetHandle(addressHandle);
		}
	
		void NetworkTransport::Init()
		{
			Plugin::NetworkTransportMethodInit();
		}
	}
}

namespace MyGame
{
	namespace MonoBehaviours
	{
		SYSTEM_OBJECT_LIFECYCLE_DEFINITION(TestScript, UnityEngine::MonoBehaviour)
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
	int32_t (*stopwatchConstructor)(),
	int64_t (*stopwatchPropertyGetElapsedMilliseconds)(int32_t thisHandle),
	void (*stopwatchMethodStart)(int32_t thisHandle),
	void (*stopwatchMethodReset)(int32_t thisHandle),
	int32_t (*objectPropertyGetName)(int32_t thisHandle),
	void (*objectPropertySetName)(int32_t thisHandle, int32_t valueHandle),
	int32_t (*gameObjectConstructor)(),
	int32_t (*gameObjectConstructorSystemString)(int32_t nameHandle),
	int32_t (*gameObjectPropertyGetTransform)(int32_t thisHandle),
	int32_t (*gameObjectMethodFindSystemString)(int32_t nameHandle),
	int32_t (*gameObjectMethodAddComponentMyGameMonoBehavioursTestScript)(int32_t thisHandle),
	int32_t (*componentPropertyGetTransform)(int32_t thisHandle),
	UnityEngine::Vector3 (*transformPropertyGetPosition)(int32_t thisHandle),
	void (*transformPropertySetPosition)(int32_t thisHandle, UnityEngine::Vector3 value),
	void (*debugMethodLogSystemObject)(int32_t messageHandle),
	System::Boolean (*assertFieldGetRaiseExceptions)(),
	void (*assertFieldSetRaiseExceptions)(System::Boolean value),
	void (*audioSettingsMethodGetDSPBufferSizeSystemInt32_SystemInt32)(int32_t* bufferLength, int32_t* numBuffers),
	void (*networkTransportMethodGetBroadcastConnectionInfoSystemInt32_SystemString_SystemInt32_SystemByte)(int32_t hostId, int32_t* addressHandle, int32_t* port, uint8_t* error),
	void (*networkTransportMethodInit)()
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
	StopwatchConstructor = stopwatchConstructor;
	StopwatchPropertyGetElapsedMilliseconds = stopwatchPropertyGetElapsedMilliseconds;
	StopwatchMethodStart = stopwatchMethodStart;
	StopwatchMethodReset = stopwatchMethodReset;
	ObjectPropertyGetName = objectPropertyGetName;
	ObjectPropertySetName = objectPropertySetName;
	GameObjectConstructor = gameObjectConstructor;
	GameObjectConstructorSystemString = gameObjectConstructorSystemString;
	GameObjectPropertyGetTransform = gameObjectPropertyGetTransform;
	GameObjectMethodFindSystemString = gameObjectMethodFindSystemString;
	GameObjectMethodAddComponentMyGameMonoBehavioursTestScript = gameObjectMethodAddComponentMyGameMonoBehavioursTestScript;
	ComponentPropertyGetTransform = componentPropertyGetTransform;
	TransformPropertyGetPosition = transformPropertyGetPosition;
	TransformPropertySetPosition = transformPropertySetPosition;
	DebugMethodLogSystemObject = debugMethodLogSystemObject;
	AssertFieldGetRaiseExceptions = assertFieldGetRaiseExceptions;
	AssertFieldSetRaiseExceptions = assertFieldSetRaiseExceptions;
	AudioSettingsMethodGetDSPBufferSizeSystemInt32_SystemInt32 = audioSettingsMethodGetDSPBufferSizeSystemInt32_SystemInt32;
	NetworkTransportMethodGetBroadcastConnectionInfoSystemInt32_SystemString_SystemInt32_SystemByte = networkTransportMethodGetBroadcastConnectionInfoSystemInt32_SystemString_SystemInt32_SystemByte;
	NetworkTransportMethodInit = networkTransportMethodInit;
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

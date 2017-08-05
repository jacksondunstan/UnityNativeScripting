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
	int32_t (*ObjectPropertyGetName)(int32_t thisHandle);
	
	void (*ObjectPropertySetName)(int32_t thisHandle, int32_t valueHandle);
	
	int32_t (*GameObjectConstructor)();
	
	int32_t (*GameObjectPropertyGetTransform)(int32_t thisHandle);
	
	int32_t (*GameObjectMethodFindSystemString)(int32_t nameHandle);
	
	int32_t (*ComponentPropertyGetTransform)(int32_t thisHandle);
	
	UnityEngine::Vector3 (*TransformPropertyGetPosition)(int32_t thisHandle);
	
	void (*TransformPropertySetPosition)(int32_t thisHandle, UnityEngine::Vector3 value);
	
	void (*DebugMethodLogSystemObject)(int32_t messageHandle);
	
	System::Boolean (*AssertFieldGetRaiseExceptions)();
	
	void (*AssertFieldSetRaiseExceptions)(System::Boolean value);
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
		Plugin::ReferenceManagedObject(handle);
	}
	
	Object::Object(const Object& other)
	{
		Handle = other.Handle;
		Plugin::ReferenceManagedObject(Handle);
	}
	
	Object::Object(Object&& other)
	{
		Handle = other.Handle;
		other.Handle = 0;
	}
	
	#define SYSTEM_OBJECT_LIFECYCLE_DEFINITION(ClassName, BaseClassName) \
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
			Plugin::DereferenceManagedObject(Handle); \
		} \
		\
		ClassName& ClassName::operator=(const ClassName& other) \
		{ \
			Plugin::DereferenceManagedObject(Handle); \
			Handle = other.Handle; \
			Plugin::ReferenceManagedObject(Handle); \
			return *this; \
		} \
		\
		ClassName& ClassName::operator=(ClassName&& other) \
		{ \
			Plugin::DereferenceManagedObject(Handle); \
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
namespace UnityEngine
{
	SYSTEM_OBJECT_LIFECYCLE_DEFINITION(Object, System::Object)
	
	System::String Object::GetName()
	{
		return System::String(Plugin::ObjectPropertyGetName(Handle));
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
		: GameObject(GameObject(Plugin::GameObjectConstructor()))
	{
	}
	
	UnityEngine::Transform GameObject::GetTransform()
	{
		return UnityEngine::Transform(Plugin::GameObjectPropertyGetTransform(Handle));
	}
	
	UnityEngine::GameObject GameObject::Find(System::String name)
	{
		return UnityEngine::GameObject(Plugin::GameObjectMethodFindSystemString(name.Handle));
	}
}

namespace UnityEngine
{
	SYSTEM_OBJECT_LIFECYCLE_DEFINITION(Component, UnityEngine::Object)
	
	UnityEngine::Transform Component::GetTransform()
	{
		return UnityEngine::Transform(Plugin::ComponentPropertyGetTransform(Handle));
	}
}

namespace UnityEngine
{
	SYSTEM_OBJECT_LIFECYCLE_DEFINITION(Transform, UnityEngine::Component)
	
	UnityEngine::Vector3 Transform::GetPosition()
	{
		return Plugin::TransformPropertyGetPosition(Handle);
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
			return Plugin::AssertFieldGetRaiseExceptions();
		}
		
		void Assert::SetRaiseExceptions(System::Boolean value)
		{
			Plugin::AssertFieldSetRaiseExceptions(value);
		}
	}
}
/*END METHOD DEFINITIONS*/

////////////////////////////////////////////////////////////////
// App-specific functions for this file to call
////////////////////////////////////////////////////////////////

// Called when the plugin is initialized
extern void PluginMain();

// Called for MonoBehaviour.Update
extern void PluginUpdate();

////////////////////////////////////////////////////////////////
// C++ functions for C# to call
////////////////////////////////////////////////////////////////

// Init the plugin
DLLEXPORT void Init(
	int32_t maxManagedObjects,
	void (*releaseObject)(
		int32_t handle),
	int32_t (*stringNew)(
		const char* chars),
	/*BEGIN INIT PARAMS*/
	int32_t (*objectPropertyGetName)(int32_t thisHandle),
	void (*objectPropertySetName)(int32_t thisHandle, int32_t valueHandle),
	int32_t (*gameObjectConstructor)(),
	int32_t (*gameObjectPropertyGetTransform)(int32_t thisHandle),
	int32_t (*gameObjectMethodFindSystemString)(int32_t nameHandle),
	int32_t (*componentPropertyGetTransform)(int32_t thisHandle),
	UnityEngine::Vector3 (*transformPropertyGetPosition)(int32_t thisHandle),
	void (*transformPropertySetPosition)(int32_t thisHandle, UnityEngine::Vector3 value),
	void (*debugMethodLogSystemObject)(int32_t messageHandle),
	System::Boolean (*assertFieldGetRaiseExceptions)(),
	void (*assertFieldSetRaiseExceptions)(System::Boolean value)
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
	ObjectPropertyGetName = objectPropertyGetName;
	ObjectPropertySetName = objectPropertySetName;
	GameObjectConstructor = gameObjectConstructor;
	GameObjectPropertyGetTransform = gameObjectPropertyGetTransform;
	GameObjectMethodFindSystemString = gameObjectMethodFindSystemString;
	ComponentPropertyGetTransform = componentPropertyGetTransform;
	TransformPropertyGetPosition = transformPropertyGetPosition;
	TransformPropertySetPosition = transformPropertySetPosition;
	DebugMethodLogSystemObject = debugMethodLogSystemObject;
	AssertFieldGetRaiseExceptions = assertFieldGetRaiseExceptions;
	AssertFieldSetRaiseExceptions = assertFieldSetRaiseExceptions;
	/*END INIT BODY*/
	
	PluginMain();
}

// Called by MonoBehaviour.Update
DLLEXPORT void MonoBehaviourUpdate()
{
	PluginUpdate();
}

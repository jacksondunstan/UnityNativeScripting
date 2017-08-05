/// <summary>
/// Declaration of the various .NET types exposed to C++
/// </summary>
/// <author>
/// Jackson Dunstan, 2017, http://JacksonDunstan.com
/// </author>
/// <license>
/// MIT
/// </license>

#pragma once

// For int32_t, etc.
#include <stdint.h>

////////////////////////////////////////////////////////////////
// C# struct types
////////////////////////////////////////////////////////////////

namespace System
{
	// .NET booleans are four bytes long
	typedef int32_t Boolean;
}

namespace UnityEngine
{
	struct Vector3
	{
		float x;
		float y;
		float z;
		
		Vector3()
			: x(0.0f)
			, y(0.0f)
			, z(0.0f)
		{
		}
		
		Vector3(
			float x,
			float y,
			float z)
			: x(x)
			, y(y)
			, z(z)
		{
		}
		
		Vector3 operator+(const Vector3& other)
		{
			return {
				x + other.x,
				y + other.y,
				z + other.z };
		}
		
		Vector3& operator=(const Vector3& other)
		{
			x = other.x;
			y = other.y;
			z = other.z;
			return *this;
		}
		
		Vector3& operator+=(const Vector3& other)
		{
			x += other.x;
			y += other.y;
			z += other.z;
			return *this;
		}
	};
}

////////////////////////////////////////////////////////////////
// C# type declarations
////////////////////////////////////////////////////////////////

namespace System
{
	struct Object
	{
		int32_t Handle;
		Object(int32_t handle);
		Object(const Object& other);
		Object(Object&& other);
	};
	
#define SYSTEM_OBJECT_LIFECYCLE_DECLARATION(ClassName, BaseClassName) \
	ClassName(int32_t handle); \
	ClassName(const ClassName& other); \
	ClassName(ClassName&& other); \
	~ClassName(); \
	ClassName& operator=(const ClassName& other); \
	ClassName& operator=(ClassName&& other);
	
	struct String : Object
	{
		SYSTEM_OBJECT_LIFECYCLE_DECLARATION(String, Object);
		String(const char* chars);
	};
}

/*BEGIN TYPE DECLARATIONS*/
namespace System
{
	namespace Diagnostics
	{
		struct Stopwatch;
	}
}

namespace UnityEngine
{
	struct Object;
}

namespace UnityEngine
{
	struct GameObject;
}

namespace UnityEngine
{
	struct Component;
}

namespace UnityEngine
{
	struct Transform;
}

namespace UnityEngine
{
	struct Debug;
}

namespace UnityEngine
{
	namespace Assertions
	{
		namespace Assert
		{
		}
	}
}
/*END TYPE DECLARATIONS*/

/*BEGIN TYPE DEFINITIONS*/
namespace System
{
	namespace Diagnostics
	{
		struct Stopwatch : System::Object
		{
			SYSTEM_OBJECT_LIFECYCLE_DECLARATION(Stopwatch, System::Object)
			Stopwatch();
			int64_t GetElapsedMilliseconds();
			void Start();
			void Reset();
		};
	}
}

namespace UnityEngine
{
	struct Object : System::Object
	{
		SYSTEM_OBJECT_LIFECYCLE_DECLARATION(Object, System::Object)
		System::String GetName();
		void SetName(System::String value);
	};
}

namespace UnityEngine
{
	struct GameObject : UnityEngine::Object
	{
		SYSTEM_OBJECT_LIFECYCLE_DECLARATION(GameObject, UnityEngine::Object)
		GameObject();
		UnityEngine::Transform GetTransform();
		static UnityEngine::GameObject Find(System::String name);
	};
}

namespace UnityEngine
{
	struct Component : UnityEngine::Object
	{
		SYSTEM_OBJECT_LIFECYCLE_DECLARATION(Component, UnityEngine::Object)
		UnityEngine::Transform GetTransform();
	};
}

namespace UnityEngine
{
	struct Transform : UnityEngine::Component
	{
		SYSTEM_OBJECT_LIFECYCLE_DECLARATION(Transform, UnityEngine::Component)
		UnityEngine::Vector3 GetPosition();
		void SetPosition(UnityEngine::Vector3 value);
	};
}

namespace UnityEngine
{
	struct Debug : System::Object
	{
		SYSTEM_OBJECT_LIFECYCLE_DECLARATION(Debug, System::Object)
		static void Log(System::Object message);
	};
}

namespace UnityEngine
{
	namespace Assertions
	{
		namespace Assert
		{
			static System::Boolean GetRaiseExceptions();
			static void SetRaiseExceptions(System::Boolean value);
		}
	}
}
/*END TYPE DEFINITIONS*/
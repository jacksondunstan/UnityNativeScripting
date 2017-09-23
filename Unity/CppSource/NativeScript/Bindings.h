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

// For nullptr_t
#include <cstddef>

////////////////////////////////////////////////////////////////
// C# struct types
////////////////////////////////////////////////////////////////

namespace System
{
	// .NET booleans are four bytes long
	// This struct makes them feel like C++'s bool type
	struct Boolean
	{
		int32_t Value;
		
		Boolean()
			: Value(0)
		{
		}
		
		Boolean(const Boolean& other)
			: Value(other.Value)
		{
		}
		
		Boolean(const Boolean&& other)
			: Value(other.Value)
		{
		}
		
		Boolean(bool value)
			: Value((int32_t)value)
		{
		}
		
		operator bool() const
		{
			return (bool)Value;
		}
		
		bool operator==(const Boolean other) const
		{
			return Value == other.Value;
		}
		
		bool operator!=(const Boolean other) const
		{
			return Value != other.Value;
		}
		
		bool operator==(const bool other) const
		{
			return Value == other;
		}
		
		bool operator!=(const bool other) const
		{
			return Value != other;
		}
	};
	
	// .NET chars are two bytes long
	// This struct helps them interoperate with C++'s char type
	struct Char
	{
		int16_t Value;
		
		Char()
			: Value(0)
		{
		}
		
		Char(const Char& other)
			: Value(other.Value)
		{
		}
		
		Char(const Char&& other)
			: Value(other.Value)
		{
		}
		
		Char(char value)
			: Value(value)
		{
		}
		
		Char(int16_t value)
			: Value(value)
		{
		}
		
		operator bool() const
		{
			return (bool)Value;
		}
		
		bool operator==(const Char other) const
		{
			return Value == other.Value;
		}
		
		bool operator!=(const Char other) const
		{
			return Value != other.Value;
		}
		
		bool operator==(const char other) const
		{
			return Value == other;
		}
		
		bool operator!=(const char other) const
		{
			return Value != other;
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
		Object(std::nullptr_t n);
		Object(int32_t handle);
		operator bool() const;
		bool operator==(std::nullptr_t other) const;
		bool operator!=(std::nullptr_t other) const;
	};
	
	struct ValueType : Object
	{
		ValueType(std::nullptr_t n);
		ValueType(int32_t handle);
	};
	
	struct String : Object
	{
		String(std::nullptr_t n);
		String(int32_t handle);
		String(const String& other);
		String(String&& other);
		~String();
		String& operator=(const String& other);
		String& operator=(std::nullptr_t other);
		String& operator=(String&& other);
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

namespace UnityEngine
{
	struct Collision;
}

namespace UnityEngine
{
	struct Behaviour;
}

namespace UnityEngine
{
	struct MonoBehaviour;
}

namespace UnityEngine
{
	struct AudioSettings;
}

namespace UnityEngine
{
	namespace Networking
	{
		struct NetworkTransport;
	}
}

namespace UnityEngine
{
	struct Vector3;
}

namespace UnityEngine
{
	struct RaycastHit;
}

namespace UnityEngine
{
	enum struct QueryTriggerInteraction : int32_t
	{
		UseGlobal = 0,
		Ignore = 1,
		Collide = 2
	};
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			template<typename T0, typename T1> struct KeyValuePair;
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			template<> struct KeyValuePair<System::String, double>;
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			template<typename T0> struct List;
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			template<> struct List<System::String>;
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			template<typename T0> struct LinkedListNode;
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			template<> struct LinkedListNode<System::String>;
		}
	}
}

namespace System
{
	namespace Runtime
	{
		namespace CompilerServices
		{
			template<typename T0> struct StrongBox;
		}
	}
}

namespace System
{
	namespace Runtime
	{
		namespace CompilerServices
		{
			template<> struct StrongBox<System::String>;
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace ObjectModel
		{
			template<typename T0> struct Collection;
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace ObjectModel
		{
			template<> struct Collection<int32_t>;
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace ObjectModel
		{
			template<typename T0, typename T1> struct KeyedCollection;
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace ObjectModel
		{
			template<> struct KeyedCollection<System::String, int32_t>;
		}
	}
}

namespace System
{
	struct Exception;
}

namespace MyGame
{
	namespace MonoBehaviours
	{
		struct TestScript;
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
			Stopwatch(std::nullptr_t n);
			Stopwatch(int32_t handle);
			Stopwatch(const Stopwatch& other);
			Stopwatch(Stopwatch&& other);
			~Stopwatch();
			Stopwatch& operator=(const Stopwatch& other);
			Stopwatch& operator=(std::nullptr_t other);
			Stopwatch& operator=(Stopwatch&& other);
			bool operator==(const Stopwatch& other) const;
			bool operator!=(const Stopwatch& other) const;
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
		Object(std::nullptr_t n);
		Object(int32_t handle);
		Object(const Object& other);
		Object(Object&& other);
		~Object();
		Object& operator=(const Object& other);
		Object& operator=(std::nullptr_t other);
		Object& operator=(Object&& other);
		bool operator==(const Object& other) const;
		bool operator!=(const Object& other) const;
		System::String GetName();
		void SetName(System::String value);
	};
}

namespace UnityEngine
{
	struct GameObject : UnityEngine::Object
	{
		GameObject(std::nullptr_t n);
		GameObject(int32_t handle);
		GameObject(const GameObject& other);
		GameObject(GameObject&& other);
		~GameObject();
		GameObject& operator=(const GameObject& other);
		GameObject& operator=(std::nullptr_t other);
		GameObject& operator=(GameObject&& other);
		bool operator==(const GameObject& other) const;
		bool operator!=(const GameObject& other) const;
		GameObject();
		GameObject(System::String name);
		UnityEngine::Transform GetTransform();
		static UnityEngine::GameObject Find(System::String name);
		template<typename T0> MyGame::MonoBehaviours::TestScript AddComponent();
	};
}

namespace UnityEngine
{
	struct Component : UnityEngine::Object
	{
		Component(std::nullptr_t n);
		Component(int32_t handle);
		Component(const Component& other);
		Component(Component&& other);
		~Component();
		Component& operator=(const Component& other);
		Component& operator=(std::nullptr_t other);
		Component& operator=(Component&& other);
		bool operator==(const Component& other) const;
		bool operator!=(const Component& other) const;
		UnityEngine::Transform GetTransform();
	};
}

namespace UnityEngine
{
	struct Transform : UnityEngine::Component
	{
		Transform(std::nullptr_t n);
		Transform(int32_t handle);
		Transform(const Transform& other);
		Transform(Transform&& other);
		~Transform();
		Transform& operator=(const Transform& other);
		Transform& operator=(std::nullptr_t other);
		Transform& operator=(Transform&& other);
		bool operator==(const Transform& other) const;
		bool operator!=(const Transform& other) const;
		UnityEngine::Vector3 GetPosition();
		void SetPosition(UnityEngine::Vector3& value);
	};
}

namespace UnityEngine
{
	struct Debug : System::Object
	{
		Debug(std::nullptr_t n);
		Debug(int32_t handle);
		Debug(const Debug& other);
		Debug(Debug&& other);
		~Debug();
		Debug& operator=(const Debug& other);
		Debug& operator=(std::nullptr_t other);
		Debug& operator=(Debug&& other);
		bool operator==(const Debug& other) const;
		bool operator!=(const Debug& other) const;
		static void Log(System::Object message);
	};
}

namespace UnityEngine
{
	namespace Assertions
	{
		namespace Assert
		{
			System::Boolean GetRaiseExceptions();
			void SetRaiseExceptions(System::Boolean value);
			template<typename T0> void AreEqual(System::String expected, System::String actual);
			template<typename T0> void AreEqual(UnityEngine::GameObject expected, UnityEngine::GameObject actual);
		}
	}
}

namespace UnityEngine
{
	struct Collision : System::Object
	{
		Collision(std::nullptr_t n);
		Collision(int32_t handle);
		Collision(const Collision& other);
		Collision(Collision&& other);
		~Collision();
		Collision& operator=(const Collision& other);
		Collision& operator=(std::nullptr_t other);
		Collision& operator=(Collision&& other);
		bool operator==(const Collision& other) const;
		bool operator!=(const Collision& other) const;
	};
}

namespace UnityEngine
{
	struct Behaviour : UnityEngine::Component
	{
		Behaviour(std::nullptr_t n);
		Behaviour(int32_t handle);
		Behaviour(const Behaviour& other);
		Behaviour(Behaviour&& other);
		~Behaviour();
		Behaviour& operator=(const Behaviour& other);
		Behaviour& operator=(std::nullptr_t other);
		Behaviour& operator=(Behaviour&& other);
		bool operator==(const Behaviour& other) const;
		bool operator!=(const Behaviour& other) const;
	};
}

namespace UnityEngine
{
	struct MonoBehaviour : UnityEngine::Behaviour
	{
		MonoBehaviour(std::nullptr_t n);
		MonoBehaviour(int32_t handle);
		MonoBehaviour(const MonoBehaviour& other);
		MonoBehaviour(MonoBehaviour&& other);
		~MonoBehaviour();
		MonoBehaviour& operator=(const MonoBehaviour& other);
		MonoBehaviour& operator=(std::nullptr_t other);
		MonoBehaviour& operator=(MonoBehaviour&& other);
		bool operator==(const MonoBehaviour& other) const;
		bool operator!=(const MonoBehaviour& other) const;
	};
}

namespace UnityEngine
{
	struct AudioSettings : System::Object
	{
		AudioSettings(std::nullptr_t n);
		AudioSettings(int32_t handle);
		AudioSettings(const AudioSettings& other);
		AudioSettings(AudioSettings&& other);
		~AudioSettings();
		AudioSettings& operator=(const AudioSettings& other);
		AudioSettings& operator=(std::nullptr_t other);
		AudioSettings& operator=(AudioSettings&& other);
		bool operator==(const AudioSettings& other) const;
		bool operator!=(const AudioSettings& other) const;
		static void GetDSPBufferSize(int32_t* bufferLength, int32_t* numBuffers);
	};
}

namespace UnityEngine
{
	namespace Networking
	{
		struct NetworkTransport : System::Object
		{
			NetworkTransport(std::nullptr_t n);
			NetworkTransport(int32_t handle);
			NetworkTransport(const NetworkTransport& other);
			NetworkTransport(NetworkTransport&& other);
			~NetworkTransport();
			NetworkTransport& operator=(const NetworkTransport& other);
			NetworkTransport& operator=(std::nullptr_t other);
			NetworkTransport& operator=(NetworkTransport&& other);
			bool operator==(const NetworkTransport& other) const;
			bool operator!=(const NetworkTransport& other) const;
			static void GetBroadcastConnectionInfo(int32_t hostId, System::String* address, int32_t* port, uint8_t* error);
			static void Init();
		};
	}
}

namespace UnityEngine
{
	struct Vector3
	{
		Vector3();
		Vector3(float x, float y, float z);
		float GetMagnitude();
		float x;
		float y;
		float z;
		void Set(float newX, float newY, float newZ);
	};
}

namespace UnityEngine
{
	struct RaycastHit : System::ValueType
	{
		RaycastHit(std::nullptr_t n);
		RaycastHit(int32_t handle);
		RaycastHit(const RaycastHit& other);
		RaycastHit(RaycastHit&& other);
		~RaycastHit();
		RaycastHit& operator=(const RaycastHit& other);
		RaycastHit& operator=(std::nullptr_t other);
		RaycastHit& operator=(RaycastHit&& other);
		bool operator==(const RaycastHit& other) const;
		bool operator!=(const RaycastHit& other) const;
		UnityEngine::Vector3 GetPoint();
		void SetPoint(UnityEngine::Vector3& value);
		UnityEngine::Transform GetTransform();
	};
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			template<> struct KeyValuePair<System::String, double> : System::ValueType
			{
				KeyValuePair<System::String, double>(std::nullptr_t n);
				KeyValuePair<System::String, double>(int32_t handle);
				KeyValuePair<System::String, double>(const KeyValuePair<System::String, double>& other);
				KeyValuePair<System::String, double>(KeyValuePair<System::String, double>&& other);
				~KeyValuePair<System::String, double>();
				KeyValuePair<System::String, double>& operator=(const KeyValuePair<System::String, double>& other);
				KeyValuePair<System::String, double>& operator=(std::nullptr_t other);
				KeyValuePair<System::String, double>& operator=(KeyValuePair<System::String, double>&& other);
				bool operator==(const KeyValuePair<System::String, double>& other) const;
				bool operator!=(const KeyValuePair<System::String, double>& other) const;
				KeyValuePair(System::String key, double value);
				System::String GetKey();
				double GetValue();
			};
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			template<> struct List<System::String> : System::Object
			{
				List<System::String>(std::nullptr_t n);
				List<System::String>(int32_t handle);
				List<System::String>(const List<System::String>& other);
				List<System::String>(List<System::String>&& other);
				~List<System::String>();
				List<System::String>& operator=(const List<System::String>& other);
				List<System::String>& operator=(std::nullptr_t other);
				List<System::String>& operator=(List<System::String>&& other);
				bool operator==(const List<System::String>& other) const;
				bool operator!=(const List<System::String>& other) const;
				List();
				void Add(System::String item);
			};
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			template<> struct LinkedListNode<System::String> : System::Object
			{
				LinkedListNode<System::String>(std::nullptr_t n);
				LinkedListNode<System::String>(int32_t handle);
				LinkedListNode<System::String>(const LinkedListNode<System::String>& other);
				LinkedListNode<System::String>(LinkedListNode<System::String>&& other);
				~LinkedListNode<System::String>();
				LinkedListNode<System::String>& operator=(const LinkedListNode<System::String>& other);
				LinkedListNode<System::String>& operator=(std::nullptr_t other);
				LinkedListNode<System::String>& operator=(LinkedListNode<System::String>&& other);
				bool operator==(const LinkedListNode<System::String>& other) const;
				bool operator!=(const LinkedListNode<System::String>& other) const;
				LinkedListNode(System::String value);
				System::String GetValue();
				void SetValue(System::String value);
			};
		}
	}
}

namespace System
{
	namespace Runtime
	{
		namespace CompilerServices
		{
			template<> struct StrongBox<System::String> : System::Object
			{
				StrongBox<System::String>(std::nullptr_t n);
				StrongBox<System::String>(int32_t handle);
				StrongBox<System::String>(const StrongBox<System::String>& other);
				StrongBox<System::String>(StrongBox<System::String>&& other);
				~StrongBox<System::String>();
				StrongBox<System::String>& operator=(const StrongBox<System::String>& other);
				StrongBox<System::String>& operator=(std::nullptr_t other);
				StrongBox<System::String>& operator=(StrongBox<System::String>&& other);
				bool operator==(const StrongBox<System::String>& other) const;
				bool operator!=(const StrongBox<System::String>& other) const;
				StrongBox(System::String value);
				System::String GetValue();
				void SetValue(System::String value);
			};
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace ObjectModel
		{
			template<> struct Collection<int32_t> : System::Object
			{
				Collection<int32_t>(std::nullptr_t n);
				Collection<int32_t>(int32_t handle);
				Collection<int32_t>(const Collection<int32_t>& other);
				Collection<int32_t>(Collection<int32_t>&& other);
				~Collection<int32_t>();
				Collection<int32_t>& operator=(const Collection<int32_t>& other);
				Collection<int32_t>& operator=(std::nullptr_t other);
				Collection<int32_t>& operator=(Collection<int32_t>&& other);
				bool operator==(const Collection<int32_t>& other) const;
				bool operator!=(const Collection<int32_t>& other) const;
			};
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace ObjectModel
		{
			template<> struct KeyedCollection<System::String, int32_t> : System::Collections::ObjectModel::Collection<int32_t>
			{
				KeyedCollection<System::String, int32_t>(std::nullptr_t n);
				KeyedCollection<System::String, int32_t>(int32_t handle);
				KeyedCollection<System::String, int32_t>(const KeyedCollection<System::String, int32_t>& other);
				KeyedCollection<System::String, int32_t>(KeyedCollection<System::String, int32_t>&& other);
				~KeyedCollection<System::String, int32_t>();
				KeyedCollection<System::String, int32_t>& operator=(const KeyedCollection<System::String, int32_t>& other);
				KeyedCollection<System::String, int32_t>& operator=(std::nullptr_t other);
				KeyedCollection<System::String, int32_t>& operator=(KeyedCollection<System::String, int32_t>&& other);
				bool operator==(const KeyedCollection<System::String, int32_t>& other) const;
				bool operator!=(const KeyedCollection<System::String, int32_t>& other) const;
			};
		}
	}
}

namespace System
{
	struct Exception : System::Object
	{
		Exception(std::nullptr_t n);
		Exception(int32_t handle);
		Exception(const Exception& other);
		Exception(Exception&& other);
		~Exception();
		Exception& operator=(const Exception& other);
		Exception& operator=(std::nullptr_t other);
		Exception& operator=(Exception&& other);
		bool operator==(const Exception& other) const;
		bool operator!=(const Exception& other) const;
		Exception(System::String message);
	};
}

namespace MyGame
{
	namespace MonoBehaviours
	{
		struct TestScript : UnityEngine::MonoBehaviour
		{
			TestScript(std::nullptr_t n);
			TestScript(int32_t handle);
			TestScript(const TestScript& other);
			TestScript(TestScript&& other);
			~TestScript();
			TestScript& operator=(const TestScript& other);
			TestScript& operator=(std::nullptr_t other);
			TestScript& operator=(TestScript&& other);
			bool operator==(const TestScript& other) const;
			bool operator!=(const TestScript& other) const;
			void Awake();
			void OnAnimatorIK(int32_t param0);
			void OnCollisionEnter(UnityEngine::Collision param0);
			void Update();
		};
	}
}
/*END TYPE DEFINITIONS*/

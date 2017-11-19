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
// Plugin internals
////////////////////////////////////////////////////////////////

namespace Plugin
{
	enum class InternalUse
	{
		Only
	};
	
	template <typename TElement> struct ArrayElementProxy1_1;
	
	template <typename TElement> struct ArrayElementProxy1_2;
	template <typename TElement> struct ArrayElementProxy2_2;
	
	template <typename TElement> struct ArrayElementProxy1_3;
	template <typename TElement> struct ArrayElementProxy2_3;
	template <typename TElement> struct ArrayElementProxy3_3;
	
	template <typename TElement> struct ArrayElementProxy1_4;
	template <typename TElement> struct ArrayElementProxy2_4;
	template <typename TElement> struct ArrayElementProxy3_4;
	template <typename TElement> struct ArrayElementProxy4_4;
	
	template <typename TElement> struct ArrayElementProxy1_5;
	template <typename TElement> struct ArrayElementProxy2_5;
	template <typename TElement> struct ArrayElementProxy3_5;
	template <typename TElement> struct ArrayElementProxy4_5;
	template <typename TElement> struct ArrayElementProxy5_5;
}

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
		
		operator int32_t() const
		{
			return Value;
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
		
		operator int16_t() const
		{
			return Value;
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
	struct Object;
	struct ValueType;
	struct String;
	struct Array;
	template <typename TElement> struct Array1;
	template <typename TElement> struct Array2;
	template <typename TElement> struct Array3;
	template <typename TElement> struct Array4;
	template <typename TElement> struct Array5;
}

////////////////////////////////////////////////////////////////
// C# type aliases
////////////////////////////////////////////////////////////////

namespace System
{
	using SByte = int8_t;
	using Byte = uint8_t;
	using Int16 = int16_t;
	using UInt16 = uint16_t;
	using Int32 = int32_t;
	using UInt32 = uint32_t;
	using Int64 = int64_t;
	using UInt64 = uint64_t;
	using Boolean = Boolean;
	using Single = float;
	using Double = double;
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
	struct Matrix4x4;
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
			template<> struct List<int32_t>;
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

namespace System
{
	struct SystemException;
}

namespace System
{
	struct NullReferenceException;
}

namespace UnityEngine
{
	struct Resolution;
}

namespace UnityEngine
{
	struct Screen;
}

namespace UnityEngine
{
	struct Ray;
}

namespace UnityEngine
{
	struct Physics;
}

namespace UnityEngine
{
	struct Color;
}

namespace UnityEngine
{
	struct GradientColorKey;
}

namespace UnityEngine
{
	struct Gradient;
}

namespace System
{
	struct AppDomainSetup;
}

namespace UnityEngine
{
	struct Application;
}

namespace UnityEngine
{
	namespace SceneManagement
	{
		struct SceneManager;
	}
}

namespace UnityEngine
{
	namespace SceneManagement
	{
		struct Scene;
	}
}

namespace UnityEngine
{
	namespace SceneManagement
	{
		enum struct LoadSceneMode : int32_t
		{
			Single = 0,
			Additive = 1
		};
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			template<typename T0> struct IComparer;
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			template<> struct IComparer<int32_t>;
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			template<> struct IComparer<System::String>;
		}
	}
}

namespace System
{
	struct StringComparer;
}

namespace System
{
	struct EventArgs;
}

namespace MyGame
{
	namespace MonoBehaviours
	{
		struct TestScript;
	}
}

namespace Plugin
{
	template<> struct ArrayElementProxy1_1<int32_t>;
}

namespace System
{
	template<> struct Array1<int32_t>;
}

namespace Plugin
{
	template<> struct ArrayElementProxy1_1<float>;
}

namespace Plugin
{
	template<> struct ArrayElementProxy1_2<float>;
}

namespace Plugin
{
	template<> struct ArrayElementProxy2_2<float>;
}

namespace Plugin
{
	template<> struct ArrayElementProxy1_3<float>;
}

namespace Plugin
{
	template<> struct ArrayElementProxy2_3<float>;
}

namespace Plugin
{
	template<> struct ArrayElementProxy3_3<float>;
}

namespace System
{
	template<> struct Array1<float>;
}

namespace System
{
	template<> struct Array2<float>;
}

namespace System
{
	template<> struct Array3<float>;
}

namespace Plugin
{
	template<> struct ArrayElementProxy1_1<System::String>;
}

namespace System
{
	template<> struct Array1<System::String>;
}

namespace Plugin
{
	template<> struct ArrayElementProxy1_1<UnityEngine::Resolution>;
}

namespace System
{
	template<> struct Array1<UnityEngine::Resolution>;
}

namespace Plugin
{
	template<> struct ArrayElementProxy1_1<UnityEngine::RaycastHit>;
}

namespace System
{
	template<> struct Array1<UnityEngine::RaycastHit>;
}

namespace Plugin
{
	template<> struct ArrayElementProxy1_1<UnityEngine::GradientColorKey>;
}

namespace System
{
	template<> struct Array1<UnityEngine::GradientColorKey>;
}

namespace System
{
	struct Action;
}

namespace System
{
	template<typename T0> struct Action1;
}

namespace System
{
	template<> struct Action1<float>;
}

namespace System
{
	template<typename T0, typename T1> struct Action2;
}

namespace System
{
	template<> struct Action2<float, float>;
}

namespace System
{
	template<typename T0, typename T1, typename T2> struct Func3;
}

namespace System
{
	template<typename T0, typename T1, typename T2> struct Func3;
}

namespace System
{
	template<> struct Func3<int32_t, float, double>;
}

namespace System
{
	template<> struct Func3<int16_t, int32_t, System::String>;
}

namespace System
{
	struct AppDomainInitializer;
}

namespace UnityEngine
{
	namespace Events
	{
		struct UnityAction;
	}
}

namespace UnityEngine
{
	namespace Events
	{
		template<typename T0, typename T1> struct UnityAction2;
	}
}

namespace UnityEngine
{
	namespace Events
	{
		template<> struct UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>;
	}
}
/*END TYPE DECLARATIONS*/

////////////////////////////////////////////////////////////////
// C# type definitions
////////////////////////////////////////////////////////////////

namespace System
{
	struct Object
	{
		int32_t Handle;
		Object(Plugin::InternalUse iu, int32_t handle);
		Object(decltype(nullptr) n);
		virtual ~Object() = default;
		bool operator==(decltype(nullptr) other) const;
		bool operator!=(decltype(nullptr) other) const;
		virtual void ThrowReferenceToThis();
		
		/*BEGIN BOXING METHOD DECLARATIONS*/
		Object(const UnityEngine::Vector3& val);
		explicit operator UnityEngine::Vector3();
		Object(const UnityEngine::Matrix4x4& val);
		explicit operator UnityEngine::Matrix4x4();
		Object(const UnityEngine::RaycastHit& val);
		explicit operator UnityEngine::RaycastHit();
		Object(UnityEngine::QueryTriggerInteraction val);
		explicit operator UnityEngine::QueryTriggerInteraction();
		Object(const System::Collections::Generic::KeyValuePair<System::String, double>& val);
		explicit operator System::Collections::Generic::KeyValuePair<System::String, double>();
		Object(const UnityEngine::Resolution& val);
		explicit operator UnityEngine::Resolution();
		Object(const UnityEngine::Ray& val);
		explicit operator UnityEngine::Ray();
		Object(const UnityEngine::Color& val);
		explicit operator UnityEngine::Color();
		Object(const UnityEngine::GradientColorKey& val);
		explicit operator UnityEngine::GradientColorKey();
		Object(const UnityEngine::SceneManagement::Scene& val);
		explicit operator UnityEngine::SceneManagement::Scene();
		Object(UnityEngine::SceneManagement::LoadSceneMode val);
		explicit operator UnityEngine::SceneManagement::LoadSceneMode();
		Object(System::Boolean val);
		explicit operator System::Boolean();
		Object(int8_t val);
		explicit operator int8_t();
		Object(uint8_t val);
		explicit operator uint8_t();
		Object(int16_t val);
		explicit operator int16_t();
		Object(uint16_t val);
		explicit operator uint16_t();
		Object(int32_t val);
		explicit operator int32_t();
		Object(uint32_t val);
		explicit operator uint32_t();
		Object(int64_t val);
		explicit operator int64_t();
		Object(uint64_t val);
		explicit operator uint64_t();
		Object(System::Char val);
		explicit operator System::Char();
		Object(float val);
		explicit operator float();
		Object(double val);
		explicit operator double();
		/*END BOXING METHOD DECLARATIONS*/
	};
	
	struct ValueType
	{
		int32_t Handle;
		ValueType(Plugin::InternalUse iu, int32_t handle);
		ValueType(decltype(nullptr) n);
	};
	
	struct String : Object
	{
		String(Plugin::InternalUse iu, int32_t handle);
		String(decltype(nullptr) n);
		String(const String& other);
		String(String&& other);
		virtual ~String();
		String& operator=(const String& other);
		String& operator=(decltype(nullptr) other);
		String& operator=(String&& other);
		String();
		String(const char* chars);
	};
	
	struct Array : Object
	{
		Array(Plugin::InternalUse iu, int32_t handle);
		Array(decltype(nullptr) n);
		int32_t GetLength();
		int32_t GetRank();
	};
}

/*BEGIN TYPE DEFINITIONS*/
namespace System
{
	namespace Diagnostics
	{
		struct Stopwatch : System::Object
		{
			Stopwatch(decltype(nullptr) n);
			Stopwatch(Plugin::InternalUse iu, int32_t handle);
			Stopwatch(const Stopwatch& other);
			Stopwatch(Stopwatch&& other);
			virtual ~Stopwatch();
			Stopwatch& operator=(const Stopwatch& other);
			Stopwatch& operator=(decltype(nullptr) other);
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
		Object(decltype(nullptr) n);
		Object(Plugin::InternalUse iu, int32_t handle);
		Object(const Object& other);
		Object(Object&& other);
		virtual ~Object();
		Object& operator=(const Object& other);
		Object& operator=(decltype(nullptr) other);
		Object& operator=(Object&& other);
		bool operator==(const Object& other) const;
		bool operator!=(const Object& other) const;
		System::String GetName();
		void SetName(const System::String& value);
		System::Boolean operator==(const UnityEngine::Object& x);
		operator System::Boolean();
	};
}

namespace UnityEngine
{
	struct GameObject : UnityEngine::Object
	{
		GameObject(decltype(nullptr) n);
		GameObject(Plugin::InternalUse iu, int32_t handle);
		GameObject(const GameObject& other);
		GameObject(GameObject&& other);
		virtual ~GameObject();
		GameObject& operator=(const GameObject& other);
		GameObject& operator=(decltype(nullptr) other);
		GameObject& operator=(GameObject&& other);
		bool operator==(const GameObject& other) const;
		bool operator!=(const GameObject& other) const;
		GameObject();
		GameObject(const System::String& name);
		UnityEngine::Transform GetTransform();
		template<typename T0> MyGame::MonoBehaviours::TestScript AddComponent();
	};
}

namespace UnityEngine
{
	struct Component : UnityEngine::Object
	{
		Component(decltype(nullptr) n);
		Component(Plugin::InternalUse iu, int32_t handle);
		Component(const Component& other);
		Component(Component&& other);
		virtual ~Component();
		Component& operator=(const Component& other);
		Component& operator=(decltype(nullptr) other);
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
		Transform(decltype(nullptr) n);
		Transform(Plugin::InternalUse iu, int32_t handle);
		Transform(const Transform& other);
		Transform(Transform&& other);
		virtual ~Transform();
		Transform& operator=(const Transform& other);
		Transform& operator=(decltype(nullptr) other);
		Transform& operator=(Transform&& other);
		bool operator==(const Transform& other) const;
		bool operator!=(const Transform& other) const;
		UnityEngine::Vector3 GetPosition();
		void SetPosition(const UnityEngine::Vector3& value);
	};
}

namespace UnityEngine
{
	struct Debug : System::Object
	{
		Debug(decltype(nullptr) n);
		Debug(Plugin::InternalUse iu, int32_t handle);
		Debug(const Debug& other);
		Debug(Debug&& other);
		virtual ~Debug();
		Debug& operator=(const Debug& other);
		Debug& operator=(decltype(nullptr) other);
		Debug& operator=(Debug&& other);
		bool operator==(const Debug& other) const;
		bool operator!=(const Debug& other) const;
		static void Log(const System::Object& message);
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
			template<typename T0> void AreEqual(const System::String& expected, const System::String& actual);
			template<typename T0> void AreEqual(const UnityEngine::GameObject& expected, const UnityEngine::GameObject& actual);
		}
	}
}

namespace UnityEngine
{
	struct Collision : System::Object
	{
		Collision(decltype(nullptr) n);
		Collision(Plugin::InternalUse iu, int32_t handle);
		Collision(const Collision& other);
		Collision(Collision&& other);
		virtual ~Collision();
		Collision& operator=(const Collision& other);
		Collision& operator=(decltype(nullptr) other);
		Collision& operator=(Collision&& other);
		bool operator==(const Collision& other) const;
		bool operator!=(const Collision& other) const;
	};
}

namespace UnityEngine
{
	struct Behaviour : UnityEngine::Component
	{
		Behaviour(decltype(nullptr) n);
		Behaviour(Plugin::InternalUse iu, int32_t handle);
		Behaviour(const Behaviour& other);
		Behaviour(Behaviour&& other);
		virtual ~Behaviour();
		Behaviour& operator=(const Behaviour& other);
		Behaviour& operator=(decltype(nullptr) other);
		Behaviour& operator=(Behaviour&& other);
		bool operator==(const Behaviour& other) const;
		bool operator!=(const Behaviour& other) const;
	};
}

namespace UnityEngine
{
	struct MonoBehaviour : UnityEngine::Behaviour
	{
		MonoBehaviour(decltype(nullptr) n);
		MonoBehaviour(Plugin::InternalUse iu, int32_t handle);
		MonoBehaviour(const MonoBehaviour& other);
		MonoBehaviour(MonoBehaviour&& other);
		virtual ~MonoBehaviour();
		MonoBehaviour& operator=(const MonoBehaviour& other);
		MonoBehaviour& operator=(decltype(nullptr) other);
		MonoBehaviour& operator=(MonoBehaviour&& other);
		bool operator==(const MonoBehaviour& other) const;
		bool operator!=(const MonoBehaviour& other) const;
	};
}

namespace UnityEngine
{
	struct AudioSettings : System::Object
	{
		AudioSettings(decltype(nullptr) n);
		AudioSettings(Plugin::InternalUse iu, int32_t handle);
		AudioSettings(const AudioSettings& other);
		AudioSettings(AudioSettings&& other);
		virtual ~AudioSettings();
		AudioSettings& operator=(const AudioSettings& other);
		AudioSettings& operator=(decltype(nullptr) other);
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
			NetworkTransport(decltype(nullptr) n);
			NetworkTransport(Plugin::InternalUse iu, int32_t handle);
			NetworkTransport(const NetworkTransport& other);
			NetworkTransport(NetworkTransport&& other);
			virtual ~NetworkTransport();
			NetworkTransport& operator=(const NetworkTransport& other);
			NetworkTransport& operator=(decltype(nullptr) other);
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
		UnityEngine::Vector3 operator+(const UnityEngine::Vector3& a);
		UnityEngine::Vector3 operator-();
	};
}

namespace UnityEngine
{
	struct Matrix4x4
	{
		Matrix4x4();
		float GetItem(int32_t row, int32_t column);
		void SetItem(int32_t row, int32_t column, float value);
		float m00;
		float m10;
		float m20;
		float m30;
		float m01;
		float m11;
		float m21;
		float m31;
		float m02;
		float m12;
		float m22;
		float m32;
		float m03;
		float m13;
		float m23;
		float m33;
	};
}

namespace UnityEngine
{
	struct RaycastHit : System::ValueType
	{
		RaycastHit(decltype(nullptr) n);
		RaycastHit(Plugin::InternalUse iu, int32_t handle);
		RaycastHit(const RaycastHit& other);
		RaycastHit(RaycastHit&& other);
		virtual ~RaycastHit();
		RaycastHit& operator=(const RaycastHit& other);
		RaycastHit& operator=(decltype(nullptr) other);
		RaycastHit& operator=(RaycastHit&& other);
		bool operator==(const RaycastHit& other) const;
		bool operator!=(const RaycastHit& other) const;
		UnityEngine::Vector3 GetPoint();
		void SetPoint(const UnityEngine::Vector3& value);
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
				KeyValuePair<System::String, double>(decltype(nullptr) n);
				KeyValuePair<System::String, double>(Plugin::InternalUse iu, int32_t handle);
				KeyValuePair<System::String, double>(const KeyValuePair<System::String, double>& other);
				KeyValuePair<System::String, double>(KeyValuePair<System::String, double>&& other);
				virtual ~KeyValuePair<System::String, double>();
				KeyValuePair<System::String, double>& operator=(const KeyValuePair<System::String, double>& other);
				KeyValuePair<System::String, double>& operator=(decltype(nullptr) other);
				KeyValuePair<System::String, double>& operator=(KeyValuePair<System::String, double>&& other);
				bool operator==(const KeyValuePair<System::String, double>& other) const;
				bool operator!=(const KeyValuePair<System::String, double>& other) const;
				KeyValuePair(const System::String& key, double value);
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
				List<System::String>(decltype(nullptr) n);
				List<System::String>(Plugin::InternalUse iu, int32_t handle);
				List<System::String>(const List<System::String>& other);
				List<System::String>(List<System::String>&& other);
				virtual ~List<System::String>();
				List<System::String>& operator=(const List<System::String>& other);
				List<System::String>& operator=(decltype(nullptr) other);
				List<System::String>& operator=(List<System::String>&& other);
				bool operator==(const List<System::String>& other) const;
				bool operator!=(const List<System::String>& other) const;
				List();
				System::String GetItem(int32_t index);
				void SetItem(int32_t index, const System::String& value);
				void Add(const System::String& item);
				void Sort(const System::Collections::Generic::IComparer<System::String>& comparer);
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
			template<> struct List<int32_t> : System::Object
			{
				List<int32_t>(decltype(nullptr) n);
				List<int32_t>(Plugin::InternalUse iu, int32_t handle);
				List<int32_t>(const List<int32_t>& other);
				List<int32_t>(List<int32_t>&& other);
				virtual ~List<int32_t>();
				List<int32_t>& operator=(const List<int32_t>& other);
				List<int32_t>& operator=(decltype(nullptr) other);
				List<int32_t>& operator=(List<int32_t>&& other);
				bool operator==(const List<int32_t>& other) const;
				bool operator!=(const List<int32_t>& other) const;
				List();
				int32_t GetItem(int32_t index);
				void SetItem(int32_t index, int32_t value);
				void Add(int32_t item);
				void Sort(const System::Collections::Generic::IComparer<int32_t>& comparer);
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
				LinkedListNode<System::String>(decltype(nullptr) n);
				LinkedListNode<System::String>(Plugin::InternalUse iu, int32_t handle);
				LinkedListNode<System::String>(const LinkedListNode<System::String>& other);
				LinkedListNode<System::String>(LinkedListNode<System::String>&& other);
				virtual ~LinkedListNode<System::String>();
				LinkedListNode<System::String>& operator=(const LinkedListNode<System::String>& other);
				LinkedListNode<System::String>& operator=(decltype(nullptr) other);
				LinkedListNode<System::String>& operator=(LinkedListNode<System::String>&& other);
				bool operator==(const LinkedListNode<System::String>& other) const;
				bool operator!=(const LinkedListNode<System::String>& other) const;
				LinkedListNode(const System::String& value);
				System::String GetValue();
				void SetValue(const System::String& value);
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
				StrongBox<System::String>(decltype(nullptr) n);
				StrongBox<System::String>(Plugin::InternalUse iu, int32_t handle);
				StrongBox<System::String>(const StrongBox<System::String>& other);
				StrongBox<System::String>(StrongBox<System::String>&& other);
				virtual ~StrongBox<System::String>();
				StrongBox<System::String>& operator=(const StrongBox<System::String>& other);
				StrongBox<System::String>& operator=(decltype(nullptr) other);
				StrongBox<System::String>& operator=(StrongBox<System::String>&& other);
				bool operator==(const StrongBox<System::String>& other) const;
				bool operator!=(const StrongBox<System::String>& other) const;
				StrongBox(const System::String& value);
				System::String GetValue();
				void SetValue(const System::String& value);
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
				Collection<int32_t>(decltype(nullptr) n);
				Collection<int32_t>(Plugin::InternalUse iu, int32_t handle);
				Collection<int32_t>(const Collection<int32_t>& other);
				Collection<int32_t>(Collection<int32_t>&& other);
				virtual ~Collection<int32_t>();
				Collection<int32_t>& operator=(const Collection<int32_t>& other);
				Collection<int32_t>& operator=(decltype(nullptr) other);
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
				KeyedCollection<System::String, int32_t>(decltype(nullptr) n);
				KeyedCollection<System::String, int32_t>(Plugin::InternalUse iu, int32_t handle);
				KeyedCollection<System::String, int32_t>(const KeyedCollection<System::String, int32_t>& other);
				KeyedCollection<System::String, int32_t>(KeyedCollection<System::String, int32_t>&& other);
				virtual ~KeyedCollection<System::String, int32_t>();
				KeyedCollection<System::String, int32_t>& operator=(const KeyedCollection<System::String, int32_t>& other);
				KeyedCollection<System::String, int32_t>& operator=(decltype(nullptr) other);
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
		Exception(decltype(nullptr) n);
		Exception(Plugin::InternalUse iu, int32_t handle);
		Exception(const Exception& other);
		Exception(Exception&& other);
		virtual ~Exception();
		Exception& operator=(const Exception& other);
		Exception& operator=(decltype(nullptr) other);
		Exception& operator=(Exception&& other);
		bool operator==(const Exception& other) const;
		bool operator!=(const Exception& other) const;
		Exception(const System::String& message);
	};
}

namespace System
{
	struct SystemException : System::Exception
	{
		SystemException(decltype(nullptr) n);
		SystemException(Plugin::InternalUse iu, int32_t handle);
		SystemException(const SystemException& other);
		SystemException(SystemException&& other);
		virtual ~SystemException();
		SystemException& operator=(const SystemException& other);
		SystemException& operator=(decltype(nullptr) other);
		SystemException& operator=(SystemException&& other);
		bool operator==(const SystemException& other) const;
		bool operator!=(const SystemException& other) const;
	};
}

namespace System
{
	struct NullReferenceException : System::SystemException
	{
		NullReferenceException(decltype(nullptr) n);
		NullReferenceException(Plugin::InternalUse iu, int32_t handle);
		NullReferenceException(const NullReferenceException& other);
		NullReferenceException(NullReferenceException&& other);
		virtual ~NullReferenceException();
		NullReferenceException& operator=(const NullReferenceException& other);
		NullReferenceException& operator=(decltype(nullptr) other);
		NullReferenceException& operator=(NullReferenceException&& other);
		bool operator==(const NullReferenceException& other) const;
		bool operator!=(const NullReferenceException& other) const;
	};
}

namespace UnityEngine
{
	struct Resolution
	{
		Resolution();
		int32_t GetWidth();
		void SetWidth(int32_t value);
		int32_t GetHeight();
		void SetHeight(int32_t value);
		int32_t GetRefreshRate();
		void SetRefreshRate(int32_t value);
		int32_t m_Width;
		int32_t m_Height;
		int32_t m_RefreshRate;
	};
}

namespace UnityEngine
{
	struct Screen : System::Object
	{
		Screen(decltype(nullptr) n);
		Screen(Plugin::InternalUse iu, int32_t handle);
		Screen(const Screen& other);
		Screen(Screen&& other);
		virtual ~Screen();
		Screen& operator=(const Screen& other);
		Screen& operator=(decltype(nullptr) other);
		Screen& operator=(Screen&& other);
		bool operator==(const Screen& other) const;
		bool operator!=(const Screen& other) const;
		static System::Array1<UnityEngine::Resolution> GetResolutions();
	};
}

namespace UnityEngine
{
	struct Ray
	{
		Ray();
		Ray(const UnityEngine::Vector3& origin, const UnityEngine::Vector3& direction);
		UnityEngine::Vector3 m_Origin;
		UnityEngine::Vector3 m_Direction;
	};
}

namespace UnityEngine
{
	struct Physics : System::Object
	{
		Physics(decltype(nullptr) n);
		Physics(Plugin::InternalUse iu, int32_t handle);
		Physics(const Physics& other);
		Physics(Physics&& other);
		virtual ~Physics();
		Physics& operator=(const Physics& other);
		Physics& operator=(decltype(nullptr) other);
		Physics& operator=(Physics&& other);
		bool operator==(const Physics& other) const;
		bool operator!=(const Physics& other) const;
		static int32_t RaycastNonAlloc(const UnityEngine::Ray& ray, const System::Array1<UnityEngine::RaycastHit>& results);
		static System::Array1<UnityEngine::RaycastHit> RaycastAll(const UnityEngine::Ray& ray);
	};
}

namespace UnityEngine
{
	struct Color
	{
		Color();
		float r;
		float g;
		float b;
		float a;
	};
}

namespace UnityEngine
{
	struct GradientColorKey
	{
		GradientColorKey();
		UnityEngine::Color color;
		float time;
	};
}

namespace UnityEngine
{
	struct Gradient : System::Object
	{
		Gradient(decltype(nullptr) n);
		Gradient(Plugin::InternalUse iu, int32_t handle);
		Gradient(const Gradient& other);
		Gradient(Gradient&& other);
		virtual ~Gradient();
		Gradient& operator=(const Gradient& other);
		Gradient& operator=(decltype(nullptr) other);
		Gradient& operator=(Gradient&& other);
		bool operator==(const Gradient& other) const;
		bool operator!=(const Gradient& other) const;
		Gradient();
		System::Array1<UnityEngine::GradientColorKey> GetColorKeys();
		void SetColorKeys(const System::Array1<UnityEngine::GradientColorKey>& value);
	};
}

namespace System
{
	struct AppDomainSetup : System::Object
	{
		AppDomainSetup(decltype(nullptr) n);
		AppDomainSetup(Plugin::InternalUse iu, int32_t handle);
		AppDomainSetup(const AppDomainSetup& other);
		AppDomainSetup(AppDomainSetup&& other);
		virtual ~AppDomainSetup();
		AppDomainSetup& operator=(const AppDomainSetup& other);
		AppDomainSetup& operator=(decltype(nullptr) other);
		AppDomainSetup& operator=(AppDomainSetup&& other);
		bool operator==(const AppDomainSetup& other) const;
		bool operator!=(const AppDomainSetup& other) const;
		AppDomainSetup();
		System::AppDomainInitializer GetAppDomainInitializer();
		void SetAppDomainInitializer(const System::AppDomainInitializer& value);
	};
}

namespace UnityEngine
{
	struct Application : System::Object
	{
		Application(decltype(nullptr) n);
		Application(Plugin::InternalUse iu, int32_t handle);
		Application(const Application& other);
		Application(Application&& other);
		virtual ~Application();
		Application& operator=(const Application& other);
		Application& operator=(decltype(nullptr) other);
		Application& operator=(Application&& other);
		bool operator==(const Application& other) const;
		bool operator!=(const Application& other) const;
		static void AddOnBeforeRender(const UnityEngine::Events::UnityAction& del);
		static void RemoveOnBeforeRender(const UnityEngine::Events::UnityAction& del);
	};
}

namespace UnityEngine
{
	namespace SceneManagement
	{
		struct SceneManager : System::Object
		{
			SceneManager(decltype(nullptr) n);
			SceneManager(Plugin::InternalUse iu, int32_t handle);
			SceneManager(const SceneManager& other);
			SceneManager(SceneManager&& other);
			virtual ~SceneManager();
			SceneManager& operator=(const SceneManager& other);
			SceneManager& operator=(decltype(nullptr) other);
			SceneManager& operator=(SceneManager&& other);
			bool operator==(const SceneManager& other) const;
			bool operator!=(const SceneManager& other) const;
			static void AddSceneLoaded(const UnityEngine::Events::UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>& del);
			static void RemoveSceneLoaded(const UnityEngine::Events::UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>& del);
		};
	}
}

namespace UnityEngine
{
	namespace SceneManagement
	{
		struct Scene
		{
			Scene();
			int32_t m_Handle;
		};
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			template<> struct IComparer<int32_t> : System::Object
			{
				IComparer<int32_t>(decltype(nullptr) n);
				IComparer<int32_t>(Plugin::InternalUse iu, int32_t handle);
				IComparer<int32_t>(const IComparer<int32_t>& other);
				IComparer<int32_t>(IComparer<int32_t>&& other);
				virtual ~IComparer<int32_t>();
				IComparer<int32_t>& operator=(const IComparer<int32_t>& other);
				IComparer<int32_t>& operator=(decltype(nullptr) other);
				IComparer<int32_t>& operator=(IComparer<int32_t>&& other);
				bool operator==(const IComparer<int32_t>& other) const;
				bool operator!=(const IComparer<int32_t>& other) const;
				int32_t CppHandle;
				IComparer();
				virtual int32_t Compare(int32_t x, int32_t y);
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
			template<> struct IComparer<System::String> : System::Object
			{
				IComparer<System::String>(decltype(nullptr) n);
				IComparer<System::String>(Plugin::InternalUse iu, int32_t handle);
				IComparer<System::String>(const IComparer<System::String>& other);
				IComparer<System::String>(IComparer<System::String>&& other);
				virtual ~IComparer<System::String>();
				IComparer<System::String>& operator=(const IComparer<System::String>& other);
				IComparer<System::String>& operator=(decltype(nullptr) other);
				IComparer<System::String>& operator=(IComparer<System::String>&& other);
				bool operator==(const IComparer<System::String>& other) const;
				bool operator!=(const IComparer<System::String>& other) const;
				int32_t CppHandle;
				IComparer();
				virtual int32_t Compare(const System::String& x, const System::String& y);
			};
		}
	}
}

namespace System
{
	struct StringComparer : System::Object
	{
		StringComparer(decltype(nullptr) n);
		StringComparer(Plugin::InternalUse iu, int32_t handle);
		StringComparer(const StringComparer& other);
		StringComparer(StringComparer&& other);
		virtual ~StringComparer();
		StringComparer& operator=(const StringComparer& other);
		StringComparer& operator=(decltype(nullptr) other);
		StringComparer& operator=(StringComparer&& other);
		bool operator==(const StringComparer& other) const;
		bool operator!=(const StringComparer& other) const;
		int32_t CppHandle;
		StringComparer();
		virtual int32_t Compare(const System::String& x, const System::String& y);
		virtual System::Boolean Equals(const System::String& x, const System::String& y);
		virtual int32_t GetHashCode(const System::String& obj);
	};
}

namespace System
{
	struct EventArgs : System::Object
	{
		EventArgs(decltype(nullptr) n);
		EventArgs(Plugin::InternalUse iu, int32_t handle);
		EventArgs(const EventArgs& other);
		EventArgs(EventArgs&& other);
		virtual ~EventArgs();
		EventArgs& operator=(const EventArgs& other);
		EventArgs& operator=(decltype(nullptr) other);
		EventArgs& operator=(EventArgs&& other);
		bool operator==(const EventArgs& other) const;
		bool operator!=(const EventArgs& other) const;
		int32_t CppHandle;
		EventArgs();
		virtual System::String ToString();
	};
}

namespace MyGame
{
	namespace MonoBehaviours
	{
		struct TestScript : UnityEngine::MonoBehaviour
		{
			TestScript(decltype(nullptr) n);
			TestScript(Plugin::InternalUse iu, int32_t handle);
			TestScript(const TestScript& other);
			TestScript(TestScript&& other);
			virtual ~TestScript();
			TestScript& operator=(const TestScript& other);
			TestScript& operator=(decltype(nullptr) other);
			TestScript& operator=(TestScript&& other);
			bool operator==(const TestScript& other) const;
			bool operator!=(const TestScript& other) const;
			void Awake();
			void OnAnimatorIK(int32_t param0);
			void OnCollisionEnter(const UnityEngine::Collision& param0);
			void Update();
		};
	}
}

namespace Plugin
{
	template<> struct ArrayElementProxy1_1<int32_t>
	{
		int32_t Handle;
		int32_t Index0;
		ArrayElementProxy1_1<int32_t>(Plugin::InternalUse iu, int32_t handle, int32_t index0);
		void operator=(int32_t item);
		operator int32_t();
	};
}

namespace System
{
	template<> struct Array1<int32_t> : System::Array
	{
		Array1<int32_t>(decltype(nullptr) n);
		Array1<int32_t>(Plugin::InternalUse iu, int32_t handle);
		Array1<int32_t>(const Array1<int32_t>& other);
		Array1<int32_t>(Array1<int32_t>&& other);
		virtual ~Array1<int32_t>();
		Array1<int32_t>& operator=(const Array1<int32_t>& other);
		Array1<int32_t>& operator=(decltype(nullptr) other);
		Array1<int32_t>& operator=(Array1<int32_t>&& other);
		bool operator==(const Array1<int32_t>& other) const;
		bool operator!=(const Array1<int32_t>& other) const;
		int32_t InternalLength;
		int32_t InternalRank;
		Array1(int32_t length0);
		int32_t GetLength();
		int32_t GetRank();
		Plugin::ArrayElementProxy1_1<int32_t> operator[](int32_t index);
	};
}

namespace Plugin
{
	template<> struct ArrayElementProxy1_1<float>
	{
		int32_t Handle;
		int32_t Index0;
		ArrayElementProxy1_1<float>(Plugin::InternalUse iu, int32_t handle, int32_t index0);
		void operator=(float item);
		operator float();
	};
}

namespace Plugin
{
	template<> struct ArrayElementProxy1_2<float>
	{
		int32_t Handle;
		int32_t Index0;
		ArrayElementProxy1_2<float>(Plugin::InternalUse iu, int32_t handle, int32_t index0);
		Plugin::ArrayElementProxy2_2<float> operator[](int32_t index);
	};
}

namespace Plugin
{
	template<> struct ArrayElementProxy2_2<float>
	{
		int32_t Handle;
		int32_t Index0;
		int32_t Index1;
		ArrayElementProxy2_2<float>(Plugin::InternalUse iu, int32_t handle, int32_t index0, int32_t index1);
		void operator=(float item);
		operator float();
	};
}

namespace Plugin
{
	template<> struct ArrayElementProxy1_3<float>
	{
		int32_t Handle;
		int32_t Index0;
		ArrayElementProxy1_3<float>(Plugin::InternalUse iu, int32_t handle, int32_t index0);
		Plugin::ArrayElementProxy2_3<float> operator[](int32_t index);
	};
}

namespace Plugin
{
	template<> struct ArrayElementProxy2_3<float>
	{
		int32_t Handle;
		int32_t Index0;
		int32_t Index1;
		ArrayElementProxy2_3<float>(Plugin::InternalUse iu, int32_t handle, int32_t index0, int32_t index1);
		Plugin::ArrayElementProxy3_3<float> operator[](int32_t index);
	};
}

namespace Plugin
{
	template<> struct ArrayElementProxy3_3<float>
	{
		int32_t Handle;
		int32_t Index0;
		int32_t Index1;
		int32_t Index2;
		ArrayElementProxy3_3<float>(Plugin::InternalUse iu, int32_t handle, int32_t index0, int32_t index1, int32_t index2);
		void operator=(float item);
		operator float();
	};
}

namespace System
{
	template<> struct Array1<float> : System::Array
	{
		Array1<float>(decltype(nullptr) n);
		Array1<float>(Plugin::InternalUse iu, int32_t handle);
		Array1<float>(const Array1<float>& other);
		Array1<float>(Array1<float>&& other);
		virtual ~Array1<float>();
		Array1<float>& operator=(const Array1<float>& other);
		Array1<float>& operator=(decltype(nullptr) other);
		Array1<float>& operator=(Array1<float>&& other);
		bool operator==(const Array1<float>& other) const;
		bool operator!=(const Array1<float>& other) const;
		int32_t InternalLength;
		int32_t InternalRank;
		Array1(int32_t length0);
		int32_t GetLength();
		int32_t GetRank();
		Plugin::ArrayElementProxy1_1<float> operator[](int32_t index);
	};
}

namespace System
{
	template<> struct Array2<float> : System::Array
	{
		Array2<float>(decltype(nullptr) n);
		Array2<float>(Plugin::InternalUse iu, int32_t handle);
		Array2<float>(const Array2<float>& other);
		Array2<float>(Array2<float>&& other);
		virtual ~Array2<float>();
		Array2<float>& operator=(const Array2<float>& other);
		Array2<float>& operator=(decltype(nullptr) other);
		Array2<float>& operator=(Array2<float>&& other);
		bool operator==(const Array2<float>& other) const;
		bool operator!=(const Array2<float>& other) const;
		int32_t InternalLength;
		int32_t InternalRank;
		int32_t InternalLengths[2];
		Array2(int32_t length0, int32_t length1);
		int32_t GetLength();
		int32_t GetLength(int32_t dimension);
		int32_t GetRank();
		Plugin::ArrayElementProxy1_2<float> operator[](int32_t index);
	};
}

namespace System
{
	template<> struct Array3<float> : System::Array
	{
		Array3<float>(decltype(nullptr) n);
		Array3<float>(Plugin::InternalUse iu, int32_t handle);
		Array3<float>(const Array3<float>& other);
		Array3<float>(Array3<float>&& other);
		virtual ~Array3<float>();
		Array3<float>& operator=(const Array3<float>& other);
		Array3<float>& operator=(decltype(nullptr) other);
		Array3<float>& operator=(Array3<float>&& other);
		bool operator==(const Array3<float>& other) const;
		bool operator!=(const Array3<float>& other) const;
		int32_t InternalLength;
		int32_t InternalRank;
		int32_t InternalLengths[3];
		Array3(int32_t length0, int32_t length1, int32_t length2);
		int32_t GetLength();
		int32_t GetLength(int32_t dimension);
		int32_t GetRank();
		Plugin::ArrayElementProxy1_3<float> operator[](int32_t index);
	};
}

namespace Plugin
{
	template<> struct ArrayElementProxy1_1<System::String>
	{
		int32_t Handle;
		int32_t Index0;
		ArrayElementProxy1_1<System::String>(Plugin::InternalUse iu, int32_t handle, int32_t index0);
		void operator=(System::String item);
		operator System::String();
	};
}

namespace System
{
	template<> struct Array1<System::String> : System::Array
	{
		Array1<System::String>(decltype(nullptr) n);
		Array1<System::String>(Plugin::InternalUse iu, int32_t handle);
		Array1<System::String>(const Array1<System::String>& other);
		Array1<System::String>(Array1<System::String>&& other);
		virtual ~Array1<System::String>();
		Array1<System::String>& operator=(const Array1<System::String>& other);
		Array1<System::String>& operator=(decltype(nullptr) other);
		Array1<System::String>& operator=(Array1<System::String>&& other);
		bool operator==(const Array1<System::String>& other) const;
		bool operator!=(const Array1<System::String>& other) const;
		int32_t InternalLength;
		int32_t InternalRank;
		Array1(int32_t length0);
		int32_t GetLength();
		int32_t GetRank();
		Plugin::ArrayElementProxy1_1<System::String> operator[](int32_t index);
	};
}

namespace Plugin
{
	template<> struct ArrayElementProxy1_1<UnityEngine::Resolution>
	{
		int32_t Handle;
		int32_t Index0;
		ArrayElementProxy1_1<UnityEngine::Resolution>(Plugin::InternalUse iu, int32_t handle, int32_t index0);
		void operator=(UnityEngine::Resolution item);
		operator UnityEngine::Resolution();
	};
}

namespace System
{
	template<> struct Array1<UnityEngine::Resolution> : System::Array
	{
		Array1<UnityEngine::Resolution>(decltype(nullptr) n);
		Array1<UnityEngine::Resolution>(Plugin::InternalUse iu, int32_t handle);
		Array1<UnityEngine::Resolution>(const Array1<UnityEngine::Resolution>& other);
		Array1<UnityEngine::Resolution>(Array1<UnityEngine::Resolution>&& other);
		virtual ~Array1<UnityEngine::Resolution>();
		Array1<UnityEngine::Resolution>& operator=(const Array1<UnityEngine::Resolution>& other);
		Array1<UnityEngine::Resolution>& operator=(decltype(nullptr) other);
		Array1<UnityEngine::Resolution>& operator=(Array1<UnityEngine::Resolution>&& other);
		bool operator==(const Array1<UnityEngine::Resolution>& other) const;
		bool operator!=(const Array1<UnityEngine::Resolution>& other) const;
		int32_t InternalLength;
		int32_t InternalRank;
		Array1(int32_t length0);
		int32_t GetLength();
		int32_t GetRank();
		Plugin::ArrayElementProxy1_1<UnityEngine::Resolution> operator[](int32_t index);
	};
}

namespace Plugin
{
	template<> struct ArrayElementProxy1_1<UnityEngine::RaycastHit>
	{
		int32_t Handle;
		int32_t Index0;
		ArrayElementProxy1_1<UnityEngine::RaycastHit>(Plugin::InternalUse iu, int32_t handle, int32_t index0);
		void operator=(UnityEngine::RaycastHit item);
		operator UnityEngine::RaycastHit();
	};
}

namespace System
{
	template<> struct Array1<UnityEngine::RaycastHit> : System::Array
	{
		Array1<UnityEngine::RaycastHit>(decltype(nullptr) n);
		Array1<UnityEngine::RaycastHit>(Plugin::InternalUse iu, int32_t handle);
		Array1<UnityEngine::RaycastHit>(const Array1<UnityEngine::RaycastHit>& other);
		Array1<UnityEngine::RaycastHit>(Array1<UnityEngine::RaycastHit>&& other);
		virtual ~Array1<UnityEngine::RaycastHit>();
		Array1<UnityEngine::RaycastHit>& operator=(const Array1<UnityEngine::RaycastHit>& other);
		Array1<UnityEngine::RaycastHit>& operator=(decltype(nullptr) other);
		Array1<UnityEngine::RaycastHit>& operator=(Array1<UnityEngine::RaycastHit>&& other);
		bool operator==(const Array1<UnityEngine::RaycastHit>& other) const;
		bool operator!=(const Array1<UnityEngine::RaycastHit>& other) const;
		int32_t InternalLength;
		int32_t InternalRank;
		Array1(int32_t length0);
		int32_t GetLength();
		int32_t GetRank();
		Plugin::ArrayElementProxy1_1<UnityEngine::RaycastHit> operator[](int32_t index);
	};
}

namespace Plugin
{
	template<> struct ArrayElementProxy1_1<UnityEngine::GradientColorKey>
	{
		int32_t Handle;
		int32_t Index0;
		ArrayElementProxy1_1<UnityEngine::GradientColorKey>(Plugin::InternalUse iu, int32_t handle, int32_t index0);
		void operator=(UnityEngine::GradientColorKey item);
		operator UnityEngine::GradientColorKey();
	};
}

namespace System
{
	template<> struct Array1<UnityEngine::GradientColorKey> : System::Array
	{
		Array1<UnityEngine::GradientColorKey>(decltype(nullptr) n);
		Array1<UnityEngine::GradientColorKey>(Plugin::InternalUse iu, int32_t handle);
		Array1<UnityEngine::GradientColorKey>(const Array1<UnityEngine::GradientColorKey>& other);
		Array1<UnityEngine::GradientColorKey>(Array1<UnityEngine::GradientColorKey>&& other);
		virtual ~Array1<UnityEngine::GradientColorKey>();
		Array1<UnityEngine::GradientColorKey>& operator=(const Array1<UnityEngine::GradientColorKey>& other);
		Array1<UnityEngine::GradientColorKey>& operator=(decltype(nullptr) other);
		Array1<UnityEngine::GradientColorKey>& operator=(Array1<UnityEngine::GradientColorKey>&& other);
		bool operator==(const Array1<UnityEngine::GradientColorKey>& other) const;
		bool operator!=(const Array1<UnityEngine::GradientColorKey>& other) const;
		int32_t InternalLength;
		int32_t InternalRank;
		Array1(int32_t length0);
		int32_t GetLength();
		int32_t GetRank();
		Plugin::ArrayElementProxy1_1<UnityEngine::GradientColorKey> operator[](int32_t index);
	};
}

namespace System
{
	struct Action : System::Object
	{
		Action(decltype(nullptr) n);
		Action(Plugin::InternalUse iu, int32_t handle);
		Action(const Action& other);
		Action(Action&& other);
		virtual ~Action();
		Action& operator=(const Action& other);
		Action& operator=(decltype(nullptr) other);
		Action& operator=(Action&& other);
		bool operator==(const Action& other) const;
		bool operator!=(const Action& other) const;
		int32_t CppHandle;
		int32_t ClassHandle;
		Action();
		void operator+=(const System::Action& del);
		void operator-=(const System::Action& del);
		virtual void operator()();
		void Invoke();
	};
}

namespace System
{
	template<> struct Action1<float> : System::Object
	{
		Action1<float>(decltype(nullptr) n);
		Action1<float>(Plugin::InternalUse iu, int32_t handle);
		Action1<float>(const Action1<float>& other);
		Action1<float>(Action1<float>&& other);
		virtual ~Action1<float>();
		Action1<float>& operator=(const Action1<float>& other);
		Action1<float>& operator=(decltype(nullptr) other);
		Action1<float>& operator=(Action1<float>&& other);
		bool operator==(const Action1<float>& other) const;
		bool operator!=(const Action1<float>& other) const;
		int32_t CppHandle;
		int32_t ClassHandle;
		Action1();
		void operator+=(const System::Action1<float>& del);
		void operator-=(const System::Action1<float>& del);
		virtual void operator()(float obj);
		void Invoke(float obj);
	};
}

namespace System
{
	template<> struct Action2<float, float> : System::Object
	{
		Action2<float, float>(decltype(nullptr) n);
		Action2<float, float>(Plugin::InternalUse iu, int32_t handle);
		Action2<float, float>(const Action2<float, float>& other);
		Action2<float, float>(Action2<float, float>&& other);
		virtual ~Action2<float, float>();
		Action2<float, float>& operator=(const Action2<float, float>& other);
		Action2<float, float>& operator=(decltype(nullptr) other);
		Action2<float, float>& operator=(Action2<float, float>&& other);
		bool operator==(const Action2<float, float>& other) const;
		bool operator!=(const Action2<float, float>& other) const;
		int32_t CppHandle;
		int32_t ClassHandle;
		Action2();
		void operator+=(const System::Action2<float, float>& del);
		void operator-=(const System::Action2<float, float>& del);
		virtual void operator()(float arg1, float arg2);
		void Invoke(float arg1, float arg2);
	};
}

namespace System
{
	template<> struct Func3<int32_t, float, double> : System::Object
	{
		Func3<int32_t, float, double>(decltype(nullptr) n);
		Func3<int32_t, float, double>(Plugin::InternalUse iu, int32_t handle);
		Func3<int32_t, float, double>(const Func3<int32_t, float, double>& other);
		Func3<int32_t, float, double>(Func3<int32_t, float, double>&& other);
		virtual ~Func3<int32_t, float, double>();
		Func3<int32_t, float, double>& operator=(const Func3<int32_t, float, double>& other);
		Func3<int32_t, float, double>& operator=(decltype(nullptr) other);
		Func3<int32_t, float, double>& operator=(Func3<int32_t, float, double>&& other);
		bool operator==(const Func3<int32_t, float, double>& other) const;
		bool operator!=(const Func3<int32_t, float, double>& other) const;
		int32_t CppHandle;
		int32_t ClassHandle;
		Func3();
		void operator+=(const System::Func3<int32_t, float, double>& del);
		void operator-=(const System::Func3<int32_t, float, double>& del);
		virtual double operator()(int32_t arg1, float arg2);
		double Invoke(int32_t arg1, float arg2);
	};
}

namespace System
{
	template<> struct Func3<int16_t, int32_t, System::String> : System::Object
	{
		Func3<int16_t, int32_t, System::String>(decltype(nullptr) n);
		Func3<int16_t, int32_t, System::String>(Plugin::InternalUse iu, int32_t handle);
		Func3<int16_t, int32_t, System::String>(const Func3<int16_t, int32_t, System::String>& other);
		Func3<int16_t, int32_t, System::String>(Func3<int16_t, int32_t, System::String>&& other);
		virtual ~Func3<int16_t, int32_t, System::String>();
		Func3<int16_t, int32_t, System::String>& operator=(const Func3<int16_t, int32_t, System::String>& other);
		Func3<int16_t, int32_t, System::String>& operator=(decltype(nullptr) other);
		Func3<int16_t, int32_t, System::String>& operator=(Func3<int16_t, int32_t, System::String>&& other);
		bool operator==(const Func3<int16_t, int32_t, System::String>& other) const;
		bool operator!=(const Func3<int16_t, int32_t, System::String>& other) const;
		int32_t CppHandle;
		int32_t ClassHandle;
		Func3();
		void operator+=(const System::Func3<int16_t, int32_t, System::String>& del);
		void operator-=(const System::Func3<int16_t, int32_t, System::String>& del);
		virtual System::String operator()(int16_t arg1, int32_t arg2);
		System::String Invoke(int16_t arg1, int32_t arg2);
	};
}

namespace System
{
	struct AppDomainInitializer : System::Object
	{
		AppDomainInitializer(decltype(nullptr) n);
		AppDomainInitializer(Plugin::InternalUse iu, int32_t handle);
		AppDomainInitializer(const AppDomainInitializer& other);
		AppDomainInitializer(AppDomainInitializer&& other);
		virtual ~AppDomainInitializer();
		AppDomainInitializer& operator=(const AppDomainInitializer& other);
		AppDomainInitializer& operator=(decltype(nullptr) other);
		AppDomainInitializer& operator=(AppDomainInitializer&& other);
		bool operator==(const AppDomainInitializer& other) const;
		bool operator!=(const AppDomainInitializer& other) const;
		int32_t CppHandle;
		int32_t ClassHandle;
		AppDomainInitializer();
		void operator+=(const System::AppDomainInitializer& del);
		void operator-=(const System::AppDomainInitializer& del);
		virtual void operator()(const System::Array1<System::String>& args);
		void Invoke(const System::Array1<System::String>& args);
	};
}

namespace UnityEngine
{
	namespace Events
	{
		struct UnityAction : System::Object
		{
			UnityAction(decltype(nullptr) n);
			UnityAction(Plugin::InternalUse iu, int32_t handle);
			UnityAction(const UnityAction& other);
			UnityAction(UnityAction&& other);
			virtual ~UnityAction();
			UnityAction& operator=(const UnityAction& other);
			UnityAction& operator=(decltype(nullptr) other);
			UnityAction& operator=(UnityAction&& other);
			bool operator==(const UnityAction& other) const;
			bool operator!=(const UnityAction& other) const;
			int32_t CppHandle;
			int32_t ClassHandle;
			UnityAction();
			void operator+=(const UnityEngine::Events::UnityAction& del);
			void operator-=(const UnityEngine::Events::UnityAction& del);
			virtual void operator()();
			void Invoke();
		};
	}
}

namespace UnityEngine
{
	namespace Events
	{
		template<> struct UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode> : System::Object
		{
			UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>(decltype(nullptr) n);
			UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>(Plugin::InternalUse iu, int32_t handle);
			UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>(const UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>& other);
			UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>(UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>&& other);
			virtual ~UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>();
			UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>& operator=(const UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>& other);
			UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>& operator=(decltype(nullptr) other);
			UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>& operator=(UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>&& other);
			bool operator==(const UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>& other) const;
			bool operator!=(const UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>& other) const;
			int32_t CppHandle;
			int32_t ClassHandle;
			UnityAction2();
			void operator+=(const UnityEngine::Events::UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>& del);
			void operator-=(const UnityEngine::Events::UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>& del);
			virtual void operator()(const UnityEngine::SceneManagement::Scene& arg0, UnityEngine::SceneManagement::LoadSceneMode arg1);
			void Invoke(const UnityEngine::SceneManagement::Scene& arg0, UnityEngine::SceneManagement::LoadSceneMode arg1);
		};
	}
}
/*END TYPE DEFINITIONS*/

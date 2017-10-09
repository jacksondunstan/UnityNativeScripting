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
// Plugin internals
////////////////////////////////////////////////////////////////

namespace Plugin
{
	enum class InternalUse
	{
		Only
	};
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
		Object(Plugin::InternalUse iu, int32_t handle);
		Object(std::nullptr_t n);
		virtual ~Object() = default;
		bool operator==(std::nullptr_t other) const;
		bool operator!=(std::nullptr_t other) const;
		virtual void ThrowReferenceToThis();
	};
	
	struct ValueType
	{
		int32_t Handle;
		ValueType(Plugin::InternalUse iu, int32_t handle);
		ValueType(std::nullptr_t n);
	};
	
	struct String : Object
	{
		String(Plugin::InternalUse iu, int32_t handle);
		String(std::nullptr_t n);
		String(const String& other);
		String(String&& other);
		virtual ~String();
		String& operator=(const String& other);
		String& operator=(std::nullptr_t other);
		String& operator=(String&& other);
		String(const char* chars);
	};
	
	struct Array : Object
	{
		Array(Plugin::InternalUse iu, int32_t handle);
		Array(std::nullptr_t n);
		int32_t GetLength();
		int32_t GetRank();
	};
	
	template <typename TElement> struct Array1;
	template <typename TElement> struct Array2;
	template <typename TElement> struct Array3;
	template <typename TElement> struct Array4;
	template <typename TElement> struct Array5;
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

namespace MyGame
{
	namespace MonoBehaviours
	{
		struct TestScript;
	}
}

namespace System
{
	template<> struct Array1<int32_t>;
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

namespace System
{
	template<> struct Array1<System::String>;
}

namespace System
{
	template<> struct Array1<UnityEngine::Resolution>;
}

namespace System
{
	template<> struct Array1<UnityEngine::RaycastHit>;
}

namespace System
{
	template<> struct Array1<UnityEngine::GradientColorKey>;
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
			Stopwatch(Plugin::InternalUse iu, int32_t handle);
			Stopwatch(const Stopwatch& other);
			Stopwatch(Stopwatch&& other);
			virtual ~Stopwatch();
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
		Object(Plugin::InternalUse iu, int32_t handle);
		Object(const Object& other);
		Object(Object&& other);
		virtual ~Object();
		Object& operator=(const Object& other);
		Object& operator=(std::nullptr_t other);
		Object& operator=(Object&& other);
		bool operator==(const Object& other) const;
		bool operator!=(const Object& other) const;
		System::String GetName();
		void SetName(System::String value);
		System::Boolean operator==(UnityEngine::Object x);
		operator System::Boolean();
	};
}

namespace UnityEngine
{
	struct GameObject : UnityEngine::Object
	{
		GameObject(std::nullptr_t n);
		GameObject(Plugin::InternalUse iu, int32_t handle);
		GameObject(const GameObject& other);
		GameObject(GameObject&& other);
		virtual ~GameObject();
		GameObject& operator=(const GameObject& other);
		GameObject& operator=(std::nullptr_t other);
		GameObject& operator=(GameObject&& other);
		bool operator==(const GameObject& other) const;
		bool operator!=(const GameObject& other) const;
		GameObject();
		GameObject(System::String name);
		UnityEngine::Transform GetTransform();
		template<typename T0> MyGame::MonoBehaviours::TestScript AddComponent();
	};
}

namespace UnityEngine
{
	struct Component : UnityEngine::Object
	{
		Component(std::nullptr_t n);
		Component(Plugin::InternalUse iu, int32_t handle);
		Component(const Component& other);
		Component(Component&& other);
		virtual ~Component();
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
		Transform(Plugin::InternalUse iu, int32_t handle);
		Transform(const Transform& other);
		Transform(Transform&& other);
		virtual ~Transform();
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
		Debug(Plugin::InternalUse iu, int32_t handle);
		Debug(const Debug& other);
		Debug(Debug&& other);
		virtual ~Debug();
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
		Collision(Plugin::InternalUse iu, int32_t handle);
		Collision(const Collision& other);
		Collision(Collision&& other);
		virtual ~Collision();
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
		Behaviour(Plugin::InternalUse iu, int32_t handle);
		Behaviour(const Behaviour& other);
		Behaviour(Behaviour&& other);
		virtual ~Behaviour();
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
		MonoBehaviour(Plugin::InternalUse iu, int32_t handle);
		MonoBehaviour(const MonoBehaviour& other);
		MonoBehaviour(MonoBehaviour&& other);
		virtual ~MonoBehaviour();
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
		AudioSettings(Plugin::InternalUse iu, int32_t handle);
		AudioSettings(const AudioSettings& other);
		AudioSettings(AudioSettings&& other);
		virtual ~AudioSettings();
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
			NetworkTransport(Plugin::InternalUse iu, int32_t handle);
			NetworkTransport(const NetworkTransport& other);
			NetworkTransport(NetworkTransport&& other);
			virtual ~NetworkTransport();
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
		UnityEngine::Vector3 operator+(UnityEngine::Vector3& a);
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
		RaycastHit(std::nullptr_t n);
		RaycastHit(Plugin::InternalUse iu, int32_t handle);
		RaycastHit(const RaycastHit& other);
		RaycastHit(RaycastHit&& other);
		virtual ~RaycastHit();
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
				KeyValuePair<System::String, double>(Plugin::InternalUse iu, int32_t handle);
				KeyValuePair<System::String, double>(const KeyValuePair<System::String, double>& other);
				KeyValuePair<System::String, double>(KeyValuePair<System::String, double>&& other);
				virtual ~KeyValuePair<System::String, double>();
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
				List<System::String>(Plugin::InternalUse iu, int32_t handle);
				List<System::String>(const List<System::String>& other);
				List<System::String>(List<System::String>&& other);
				virtual ~List<System::String>();
				List<System::String>& operator=(const List<System::String>& other);
				List<System::String>& operator=(std::nullptr_t other);
				List<System::String>& operator=(List<System::String>&& other);
				bool operator==(const List<System::String>& other) const;
				bool operator!=(const List<System::String>& other) const;
				List();
				System::String GetItem(int32_t index);
				void SetItem(int32_t index, System::String value);
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
				LinkedListNode<System::String>(Plugin::InternalUse iu, int32_t handle);
				LinkedListNode<System::String>(const LinkedListNode<System::String>& other);
				LinkedListNode<System::String>(LinkedListNode<System::String>&& other);
				virtual ~LinkedListNode<System::String>();
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
				StrongBox<System::String>(Plugin::InternalUse iu, int32_t handle);
				StrongBox<System::String>(const StrongBox<System::String>& other);
				StrongBox<System::String>(StrongBox<System::String>&& other);
				virtual ~StrongBox<System::String>();
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
				Collection<int32_t>(Plugin::InternalUse iu, int32_t handle);
				Collection<int32_t>(const Collection<int32_t>& other);
				Collection<int32_t>(Collection<int32_t>&& other);
				virtual ~Collection<int32_t>();
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
				KeyedCollection<System::String, int32_t>(Plugin::InternalUse iu, int32_t handle);
				KeyedCollection<System::String, int32_t>(const KeyedCollection<System::String, int32_t>& other);
				KeyedCollection<System::String, int32_t>(KeyedCollection<System::String, int32_t>&& other);
				virtual ~KeyedCollection<System::String, int32_t>();
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
		Exception(Plugin::InternalUse iu, int32_t handle);
		Exception(const Exception& other);
		Exception(Exception&& other);
		virtual ~Exception();
		Exception& operator=(const Exception& other);
		Exception& operator=(std::nullptr_t other);
		Exception& operator=(Exception&& other);
		bool operator==(const Exception& other) const;
		bool operator!=(const Exception& other) const;
		Exception(System::String message);
	};
}

namespace System
{
	struct SystemException : System::Exception
	{
		SystemException(std::nullptr_t n);
		SystemException(Plugin::InternalUse iu, int32_t handle);
		SystemException(const SystemException& other);
		SystemException(SystemException&& other);
		virtual ~SystemException();
		SystemException& operator=(const SystemException& other);
		SystemException& operator=(std::nullptr_t other);
		SystemException& operator=(SystemException&& other);
		bool operator==(const SystemException& other) const;
		bool operator!=(const SystemException& other) const;
	};
}

namespace System
{
	struct NullReferenceException : System::SystemException
	{
		NullReferenceException(std::nullptr_t n);
		NullReferenceException(Plugin::InternalUse iu, int32_t handle);
		NullReferenceException(const NullReferenceException& other);
		NullReferenceException(NullReferenceException&& other);
		virtual ~NullReferenceException();
		NullReferenceException& operator=(const NullReferenceException& other);
		NullReferenceException& operator=(std::nullptr_t other);
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
		Screen(std::nullptr_t n);
		Screen(Plugin::InternalUse iu, int32_t handle);
		Screen(const Screen& other);
		Screen(Screen&& other);
		virtual ~Screen();
		Screen& operator=(const Screen& other);
		Screen& operator=(std::nullptr_t other);
		Screen& operator=(Screen&& other);
		bool operator==(const Screen& other) const;
		bool operator!=(const Screen& other) const;
		static System::Array1<Resolution> GetResolutions();
	};
}

namespace UnityEngine
{
	struct Ray
	{
		Ray();
		Ray(UnityEngine::Vector3& origin, UnityEngine::Vector3& direction);
		UnityEngine::Vector3 m_Origin;
		UnityEngine::Vector3 m_Direction;
	};
}

namespace UnityEngine
{
	struct Physics : System::Object
	{
		Physics(std::nullptr_t n);
		Physics(Plugin::InternalUse iu, int32_t handle);
		Physics(const Physics& other);
		Physics(Physics&& other);
		virtual ~Physics();
		Physics& operator=(const Physics& other);
		Physics& operator=(std::nullptr_t other);
		Physics& operator=(Physics&& other);
		bool operator==(const Physics& other) const;
		bool operator!=(const Physics& other) const;
		static int32_t RaycastNonAlloc(UnityEngine::Ray& ray, System::Array1<RaycastHit> results);
		static System::Array1<RaycastHit> RaycastAll(UnityEngine::Ray& ray);
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
		Gradient(std::nullptr_t n);
		Gradient(Plugin::InternalUse iu, int32_t handle);
		Gradient(const Gradient& other);
		Gradient(Gradient&& other);
		virtual ~Gradient();
		Gradient& operator=(const Gradient& other);
		Gradient& operator=(std::nullptr_t other);
		Gradient& operator=(Gradient&& other);
		bool operator==(const Gradient& other) const;
		bool operator!=(const Gradient& other) const;
		Gradient();
		System::Array1<GradientColorKey> GetColorKeys();
		void SetColorKeys(System::Array1<GradientColorKey> value);
	};
}

namespace MyGame
{
	namespace MonoBehaviours
	{
		struct TestScript : UnityEngine::MonoBehaviour
		{
			TestScript(std::nullptr_t n);
			TestScript(Plugin::InternalUse iu, int32_t handle);
			TestScript(const TestScript& other);
			TestScript(TestScript&& other);
			virtual ~TestScript();
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

namespace System
{
	template<> struct Array1<int32_t> : System::Array
	{
		Array1<int32_t>(std::nullptr_t n);
		Array1<int32_t>(Plugin::InternalUse iu, int32_t handle);
		Array1<int32_t>(const Array1<int32_t>& other);
		Array1<int32_t>(Array1<int32_t>&& other);
		virtual ~Array1<int32_t>();
		Array1<int32_t>& operator=(const Array1<int32_t>& other);
		Array1<int32_t>& operator=(std::nullptr_t other);
		Array1<int32_t>& operator=(Array1<int32_t>&& other);
		bool operator==(const Array1<int32_t>& other) const;
		bool operator!=(const Array1<int32_t>& other) const;
		Array1(int32_t length0);
		int32_t GetLength();
		int32_t GetRank();
		int32_t GetItem(int32_t index0);
		void SetItem(int32_t index0, int32_t item);
	};
}

namespace System
{
	template<> struct Array1<float> : System::Array
	{
		Array1<float>(std::nullptr_t n);
		Array1<float>(Plugin::InternalUse iu, int32_t handle);
		Array1<float>(const Array1<float>& other);
		Array1<float>(Array1<float>&& other);
		virtual ~Array1<float>();
		Array1<float>& operator=(const Array1<float>& other);
		Array1<float>& operator=(std::nullptr_t other);
		Array1<float>& operator=(Array1<float>&& other);
		bool operator==(const Array1<float>& other) const;
		bool operator!=(const Array1<float>& other) const;
		Array1(int32_t length0);
		int32_t GetLength();
		int32_t GetRank();
		float GetItem(int32_t index0);
		void SetItem(int32_t index0, float item);
	};
}

namespace System
{
	template<> struct Array2<float> : System::Array
	{
		Array2<float>(std::nullptr_t n);
		Array2<float>(Plugin::InternalUse iu, int32_t handle);
		Array2<float>(const Array2<float>& other);
		Array2<float>(Array2<float>&& other);
		virtual ~Array2<float>();
		Array2<float>& operator=(const Array2<float>& other);
		Array2<float>& operator=(std::nullptr_t other);
		Array2<float>& operator=(Array2<float>&& other);
		bool operator==(const Array2<float>& other) const;
		bool operator!=(const Array2<float>& other) const;
		Array2(int32_t length0, int32_t length1);
		int32_t GetLength();
		int32_t GetLength(int32_t dimension);
		int32_t GetRank();
		float GetItem(int32_t index0, int32_t index1);
		void SetItem(int32_t index0, int32_t index1, float item);
	};
}

namespace System
{
	template<> struct Array3<float> : System::Array
	{
		Array3<float>(std::nullptr_t n);
		Array3<float>(Plugin::InternalUse iu, int32_t handle);
		Array3<float>(const Array3<float>& other);
		Array3<float>(Array3<float>&& other);
		virtual ~Array3<float>();
		Array3<float>& operator=(const Array3<float>& other);
		Array3<float>& operator=(std::nullptr_t other);
		Array3<float>& operator=(Array3<float>&& other);
		bool operator==(const Array3<float>& other) const;
		bool operator!=(const Array3<float>& other) const;
		Array3(int32_t length0, int32_t length1, int32_t length2);
		int32_t GetLength();
		int32_t GetLength(int32_t dimension);
		int32_t GetRank();
		float GetItem(int32_t index0, int32_t index1, int32_t index2);
		void SetItem(int32_t index0, int32_t index1, int32_t index2, float item);
	};
}

namespace System
{
	template<> struct Array1<System::String> : System::Array
	{
		Array1<System::String>(std::nullptr_t n);
		Array1<System::String>(Plugin::InternalUse iu, int32_t handle);
		Array1<System::String>(const Array1<System::String>& other);
		Array1<System::String>(Array1<System::String>&& other);
		virtual ~Array1<System::String>();
		Array1<System::String>& operator=(const Array1<System::String>& other);
		Array1<System::String>& operator=(std::nullptr_t other);
		Array1<System::String>& operator=(Array1<System::String>&& other);
		bool operator==(const Array1<System::String>& other) const;
		bool operator!=(const Array1<System::String>& other) const;
		Array1(int32_t length0);
		int32_t GetLength();
		int32_t GetRank();
		System::String GetItem(int32_t index0);
		void SetItem(int32_t index0, System::String item);
	};
}

namespace System
{
	template<> struct Array1<UnityEngine::Resolution> : System::Array
	{
		Array1<UnityEngine::Resolution>(std::nullptr_t n);
		Array1<UnityEngine::Resolution>(Plugin::InternalUse iu, int32_t handle);
		Array1<UnityEngine::Resolution>(const Array1<UnityEngine::Resolution>& other);
		Array1<UnityEngine::Resolution>(Array1<UnityEngine::Resolution>&& other);
		virtual ~Array1<UnityEngine::Resolution>();
		Array1<UnityEngine::Resolution>& operator=(const Array1<UnityEngine::Resolution>& other);
		Array1<UnityEngine::Resolution>& operator=(std::nullptr_t other);
		Array1<UnityEngine::Resolution>& operator=(Array1<UnityEngine::Resolution>&& other);
		bool operator==(const Array1<UnityEngine::Resolution>& other) const;
		bool operator!=(const Array1<UnityEngine::Resolution>& other) const;
		Array1(int32_t length0);
		int32_t GetLength();
		int32_t GetRank();
		UnityEngine::Resolution GetItem(int32_t index0);
		void SetItem(int32_t index0, UnityEngine::Resolution& item);
	};
}

namespace System
{
	template<> struct Array1<UnityEngine::RaycastHit> : System::Array
	{
		Array1<UnityEngine::RaycastHit>(std::nullptr_t n);
		Array1<UnityEngine::RaycastHit>(Plugin::InternalUse iu, int32_t handle);
		Array1<UnityEngine::RaycastHit>(const Array1<UnityEngine::RaycastHit>& other);
		Array1<UnityEngine::RaycastHit>(Array1<UnityEngine::RaycastHit>&& other);
		virtual ~Array1<UnityEngine::RaycastHit>();
		Array1<UnityEngine::RaycastHit>& operator=(const Array1<UnityEngine::RaycastHit>& other);
		Array1<UnityEngine::RaycastHit>& operator=(std::nullptr_t other);
		Array1<UnityEngine::RaycastHit>& operator=(Array1<UnityEngine::RaycastHit>&& other);
		bool operator==(const Array1<UnityEngine::RaycastHit>& other) const;
		bool operator!=(const Array1<UnityEngine::RaycastHit>& other) const;
		Array1(int32_t length0);
		int32_t GetLength();
		int32_t GetRank();
		UnityEngine::RaycastHit GetItem(int32_t index0);
		void SetItem(int32_t index0, UnityEngine::RaycastHit item);
	};
}

namespace System
{
	template<> struct Array1<UnityEngine::GradientColorKey> : System::Array
	{
		Array1<UnityEngine::GradientColorKey>(std::nullptr_t n);
		Array1<UnityEngine::GradientColorKey>(Plugin::InternalUse iu, int32_t handle);
		Array1<UnityEngine::GradientColorKey>(const Array1<UnityEngine::GradientColorKey>& other);
		Array1<UnityEngine::GradientColorKey>(Array1<UnityEngine::GradientColorKey>&& other);
		virtual ~Array1<UnityEngine::GradientColorKey>();
		Array1<UnityEngine::GradientColorKey>& operator=(const Array1<UnityEngine::GradientColorKey>& other);
		Array1<UnityEngine::GradientColorKey>& operator=(std::nullptr_t other);
		Array1<UnityEngine::GradientColorKey>& operator=(Array1<UnityEngine::GradientColorKey>&& other);
		bool operator==(const Array1<UnityEngine::GradientColorKey>& other) const;
		bool operator!=(const Array1<UnityEngine::GradientColorKey>& other) const;
		Array1(int32_t length0);
		int32_t GetLength();
		int32_t GetRank();
		UnityEngine::GradientColorKey GetItem(int32_t index0);
		void SetItem(int32_t index0, UnityEngine::GradientColorKey& item);
	};
}
/*END TYPE DEFINITIONS*/

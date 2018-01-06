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

/*BEGIN TEMPLATE DECLARATIONS*/
namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			template<typename TT0> struct IEnumerable;
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			template<typename TT0> struct ICollection;
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			template<typename TT0> struct IList;
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			template<typename TT0> struct IEqualityComparer;
		}
	}
}

namespace System
{
	template<typename TT0> struct IEquatable;
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			template<typename TT0, typename TT1> struct KeyValuePair;
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			template<typename TT0> struct List;
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			template<typename TT0> struct LinkedListNode;
		}
	}
}

namespace System
{
	namespace Runtime
	{
		namespace CompilerServices
		{
			template<typename TT0> struct StrongBox;
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace ObjectModel
		{
			template<typename TT0> struct Collection;
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace ObjectModel
		{
			template<typename TT0, typename TT1> struct KeyedCollection;
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			template<typename TT0> struct IComparer;
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			template<typename TT0> struct BaseIComparer;
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			template<typename TT0> struct BaseIComparer;
		}
	}
}

namespace System
{
	template<typename TT0> struct Action1;
}

namespace System
{
	template<typename TT0, typename TT1> struct Action2;
}

namespace System
{
	template<typename TT0, typename TT1, typename TT2> struct Func3;
}

namespace System
{
	template<typename TT0, typename TT1, typename TT2> struct Func3;
}

namespace UnityEngine
{
	namespace Events
	{
		template<typename TT0, typename TT1> struct UnityAction2;
	}
}
/*END TEMPLATE DECLARATIONS*/

/*BEGIN TYPE DECLARATIONS*/
namespace System
{
	struct IDisposable;
}

namespace UnityEngine
{
	struct Vector3;
}

namespace UnityEngine
{
	struct Object;
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
	struct Color;
}

namespace UnityEngine
{
	struct GradientColorKey;
}

namespace UnityEngine
{
	struct Resolution;
}

namespace UnityEngine
{
	struct RaycastHit;
}

namespace System
{
	namespace Runtime
	{
		namespace Serialization
		{
			struct ISerializable;
		}
	}
}

namespace System
{
	namespace Runtime
	{
		namespace InteropServices
		{
			struct _Exception;
		}
	}
}

namespace System
{
	struct IAppDomainSetup;
}

namespace System
{
	namespace Collections
	{
		struct IComparer;
	}
}

namespace System
{
	namespace Collections
	{
		struct IEqualityComparer;
	}
}

namespace UnityEngine
{
	namespace Playables
	{
		struct PlayableGraph;
	}
}

namespace UnityEngine
{
	namespace Playables
	{
		struct IPlayable;
	}
}

namespace UnityEngine
{
	namespace Animations
	{
		struct AnimationMixerPlayable;
	}
}

namespace System
{
	namespace Runtime
	{
		namespace CompilerServices
		{
			struct IStrongBox;
		}
	}
}

namespace UnityEngine
{
	namespace Experimental
	{
		namespace UIElements
		{
			struct IEventHandler;
		}
	}
}

namespace UnityEngine
{
	namespace Experimental
	{
		namespace UIElements
		{
			struct IStyle;
		}
	}
}

namespace System
{
	namespace Diagnostics
	{
		struct Stopwatch;
	}
}

namespace UnityEngine
{
	struct GameObject;
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
	struct Quaternion;
}

namespace UnityEngine
{
	struct Matrix4x4;
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
		struct IEnumerator;
	}
}

namespace System
{
	struct EventArgs;
}

namespace System
{
	namespace ComponentModel
	{
		namespace Design
		{
			struct ComponentEventArgs;
		}
	}
}

namespace System
{
	namespace ComponentModel
	{
		namespace Design
		{
			struct ComponentChangingEventArgs;
		}
	}
}

namespace System
{
	namespace ComponentModel
	{
		namespace Design
		{
			struct ComponentChangedEventArgs;
		}
	}
}

namespace System
{
	namespace ComponentModel
	{
		namespace Design
		{
			struct ComponentRenameEventArgs;
		}
	}
}

namespace System
{
	namespace ComponentModel
	{
		struct MemberDescriptor;
	}
}

namespace UnityEngine
{
	enum struct PrimitiveType : int32_t
	{
		Sphere = 0,
		Capsule = 1,
		Cylinder = 2,
		Cube = 3,
		Plane = 4,
		Quad = 5
	};
}

namespace UnityEngine
{
	struct Time;
}

namespace System
{
	namespace IO
	{
		enum struct FileMode : int32_t
		{
			CreateNew = 1,
			Create = 2,
			Open = 3,
			OpenOrCreate = 4,
			Truncate = 5,
			Append = 6
		};
	}
}

namespace System
{
	struct MarshalByRefObject;
}

namespace System
{
	namespace IO
	{
		struct Stream;
	}
}

namespace System
{
	struct StringComparer;
}

namespace System
{
	struct BaseStringComparer;
}

namespace System
{
	namespace Collections
	{
		struct Queue;
	}
}

namespace System
{
	namespace Collections
	{
		struct BaseQueue;
	}
}

namespace System
{
	namespace ComponentModel
	{
		namespace Design
		{
			struct IComponentChangeService;
		}
	}
}

namespace System
{
	namespace ComponentModel
	{
		namespace Design
		{
			struct BaseIComponentChangeService;
		}
	}
}

namespace System
{
	namespace IO
	{
		struct FileStream;
	}
}

namespace System
{
	namespace IO
	{
		struct BaseFileStream;
	}
}

namespace UnityEngine
{
	namespace Playables
	{
		struct PlayableHandle;
	}
}

namespace UnityEngine
{
	namespace Experimental
	{
		namespace UIElements
		{
			struct CallbackEventHandler;
		}
	}
}

namespace UnityEngine
{
	namespace Experimental
	{
		namespace UIElements
		{
			struct VisualElement;
		}
	}
}

namespace UnityEngine
{
	namespace Experimental
	{
		namespace UIElements
		{
			namespace UQueryExtensions
			{
			}
		}
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
				enum struct InteractionSourcePositionAccuracy : int32_t
				{
					None = 0,
					Approximate = 1,
					High = 2
				};
			}
		}
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
				enum struct InteractionSourceNode : int32_t
				{
					Grip = 0,
					Pointer = 1
				};
			}
		}
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
				struct InteractionSourcePose;
			}
		}
	}
}

namespace MyGame
{
	namespace MonoBehaviours
	{
		struct TestScript;
	}
}

namespace MyGame
{
	namespace MonoBehaviours
	{
		struct AnotherScript;
	}
}

namespace System
{
	struct Action;
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

namespace System
{
	namespace ComponentModel
	{
		namespace Design
		{
			struct ComponentEventHandler;
		}
	}
}

namespace System
{
	namespace ComponentModel
	{
		namespace Design
		{
			struct ComponentChangingEventHandler;
		}
	}
}

namespace System
{
	namespace ComponentModel
	{
		namespace Design
		{
			struct ComponentChangedEventHandler;
		}
	}
}

namespace System
{
	namespace ComponentModel
	{
		namespace Design
		{
			struct ComponentRenameEventHandler;
		}
	}
}
/*END TYPE DECLARATIONS*/

/*BEGIN TEMPLATE SPECIALIZATION DECLARATIONS*/
namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			template<> struct IEnumerable<System::String>;
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			template<> struct IEnumerable<int32_t>;
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			template<> struct IEnumerable<float>;
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			template<> struct IEnumerable<UnityEngine::RaycastHit>;
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			template<> struct IEnumerable<UnityEngine::GradientColorKey>;
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			template<> struct IEnumerable<UnityEngine::Resolution>;
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			template<> struct ICollection<System::String>;
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			template<> struct ICollection<int32_t>;
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			template<> struct ICollection<float>;
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			template<> struct ICollection<UnityEngine::RaycastHit>;
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			template<> struct ICollection<UnityEngine::GradientColorKey>;
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			template<> struct ICollection<UnityEngine::Resolution>;
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			template<> struct IList<System::String>;
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			template<> struct IList<int32_t>;
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			template<> struct IList<float>;
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			template<> struct IList<UnityEngine::RaycastHit>;
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			template<> struct IList<UnityEngine::GradientColorKey>;
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			template<> struct IList<UnityEngine::Resolution>;
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			template<> struct IEqualityComparer<System::String>;
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			template<> struct IEqualityComparer<int32_t>;
		}
	}
}

namespace System
{
	template<> struct IEquatable<UnityEngine::Animations::AnimationMixerPlayable>;
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
			template<> struct KeyedCollection<System::String, int32_t>;
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
	namespace Collections
	{
		namespace Generic
		{
			template<> struct BaseIComparer<int32_t>;
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			template<> struct BaseIComparer<System::String>;
		}
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
	template<> struct Action1<float>;
}

namespace System
{
	template<> struct Action2<float, float>;
}

namespace System
{
	template<> struct Func3<int32_t, float, double>;
}

namespace System
{
	template<> struct Func3<int16_t, int32_t, System::String>;
}

namespace UnityEngine
{
	namespace Events
	{
		template<> struct UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>;
	}
}
/*END TEMPLATE SPECIALIZATION DECLARATIONS*/

////////////////////////////////////////////////////////////////
// C# type definitions
////////////////////////////////////////////////////////////////

namespace System
{
	struct Object
	{
		int32_t Handle;
		Object();
		Object(Plugin::InternalUse iu, int32_t handle);
		Object(decltype(nullptr) n);
		virtual ~Object() = default;
		bool operator==(decltype(nullptr) other) const;
		bool operator!=(decltype(nullptr) other) const;
		virtual void ThrowReferenceToThis();
		
		/*BEGIN BOXING METHOD DECLARATIONS*/
		Object(UnityEngine::Vector3& val);
		explicit operator UnityEngine::Vector3();
		Object(UnityEngine::Color& val);
		explicit operator UnityEngine::Color();
		Object(UnityEngine::GradientColorKey& val);
		explicit operator UnityEngine::GradientColorKey();
		Object(UnityEngine::Resolution& val);
		explicit operator UnityEngine::Resolution();
		Object(UnityEngine::RaycastHit& val);
		explicit operator UnityEngine::RaycastHit();
		Object(UnityEngine::Playables::PlayableGraph& val);
		explicit operator UnityEngine::Playables::PlayableGraph();
		Object(UnityEngine::Animations::AnimationMixerPlayable& val);
		explicit operator UnityEngine::Animations::AnimationMixerPlayable();
		Object(UnityEngine::Quaternion& val);
		explicit operator UnityEngine::Quaternion();
		Object(UnityEngine::Matrix4x4& val);
		explicit operator UnityEngine::Matrix4x4();
		Object(UnityEngine::QueryTriggerInteraction val);
		explicit operator UnityEngine::QueryTriggerInteraction();
		Object(System::Collections::Generic::KeyValuePair<System::String, double>& val);
		explicit operator System::Collections::Generic::KeyValuePair<System::String, double>();
		Object(UnityEngine::Ray& val);
		explicit operator UnityEngine::Ray();
		Object(UnityEngine::SceneManagement::Scene& val);
		explicit operator UnityEngine::SceneManagement::Scene();
		Object(UnityEngine::SceneManagement::LoadSceneMode val);
		explicit operator UnityEngine::SceneManagement::LoadSceneMode();
		Object(UnityEngine::PrimitiveType val);
		explicit operator UnityEngine::PrimitiveType();
		Object(System::IO::FileMode val);
		explicit operator System::IO::FileMode();
		Object(UnityEngine::Playables::PlayableHandle& val);
		explicit operator UnityEngine::Playables::PlayableHandle();
		Object(UnityEngine::XR::WSA::Input::InteractionSourcePositionAccuracy val);
		explicit operator UnityEngine::XR::WSA::Input::InteractionSourcePositionAccuracy();
		Object(UnityEngine::XR::WSA::Input::InteractionSourceNode val);
		explicit operator UnityEngine::XR::WSA::Input::InteractionSourceNode();
		Object(UnityEngine::XR::WSA::Input::InteractionSourcePose& val);
		explicit operator UnityEngine::XR::WSA::Input::InteractionSourcePose();
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
	
	struct ValueType : virtual Object
	{
		ValueType(Plugin::InternalUse iu, int32_t handle);
		ValueType(decltype(nullptr) n);
	};
	
	struct String : virtual Object
	{
		String(Plugin::InternalUse iu, int32_t handle);
		String(decltype(nullptr) n);
		String(const String& other);
		String(String&& other);
		virtual ~String();
		String& operator=(const String& other);
		String& operator=(decltype(nullptr) other);
		String& operator=(String&& other);
		String(const char* chars);
	};
	
	struct ICloneable : virtual Object
	{
		ICloneable(Plugin::InternalUse iu, int32_t handle);
		ICloneable(decltype(nullptr) n);
	};
	
	namespace Collections
	{
		struct IEnumerable : virtual Object
		{
			IEnumerable(Plugin::InternalUse iu, int32_t handle);
			IEnumerable(decltype(nullptr) n);
		};
		
		struct ICollection : virtual IEnumerable
		{
			ICollection(Plugin::InternalUse iu, int32_t handle);
			ICollection(decltype(nullptr) n);
		};
		
		struct IList : virtual ICollection, virtual IEnumerable
		{
			IList(Plugin::InternalUse iu, int32_t handle);
			IList(decltype(nullptr) n);
		};
	}
	
	struct Array : virtual ICloneable, virtual Collections::IList
	{
		Array(Plugin::InternalUse iu, int32_t handle);
		Array(decltype(nullptr) n);
		int32_t GetLength();
		int32_t GetRank();
	};
}

////////////////////////////////////////////////////////////////
// Global variables
////////////////////////////////////////////////////////////////

namespace Plugin
{
	extern System::String NullString;
}

/*BEGIN TYPE DEFINITIONS*/
namespace System
{
	struct IDisposable : virtual System::Object
	{
		IDisposable(decltype(nullptr) n);
		IDisposable(Plugin::InternalUse iu, int32_t handle);
		IDisposable(const IDisposable& other);
		IDisposable(IDisposable&& other);
		virtual ~IDisposable();
		IDisposable& operator=(const IDisposable& other);
		IDisposable& operator=(decltype(nullptr) other);
		IDisposable& operator=(IDisposable&& other);
		bool operator==(const IDisposable& other) const;
		bool operator!=(const IDisposable& other) const;
	};
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
	struct Object : virtual System::Object
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
		void SetName(System::String& value);
		System::Boolean operator==(UnityEngine::Object& x);
		operator System::Boolean();
	};
}

namespace UnityEngine
{
	struct Component : virtual UnityEngine::Object
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
	struct Transform : virtual UnityEngine::Component, virtual System::Collections::IEnumerable
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
		void SetPosition(UnityEngine::Vector3& value);
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
	struct Resolution : virtual System::ValueType
	{
		Resolution(decltype(nullptr) n);
		Resolution(Plugin::InternalUse iu, int32_t handle);
		Resolution(const Resolution& other);
		Resolution(Resolution&& other);
		virtual ~Resolution();
		Resolution& operator=(const Resolution& other);
		Resolution& operator=(decltype(nullptr) other);
		Resolution& operator=(Resolution&& other);
		bool operator==(const Resolution& other) const;
		bool operator!=(const Resolution& other) const;
		int32_t GetWidth();
		void SetWidth(int32_t value);
		int32_t GetHeight();
		void SetHeight(int32_t value);
		int32_t GetRefreshRate();
		void SetRefreshRate(int32_t value);
	};
}

namespace UnityEngine
{
	struct RaycastHit : virtual System::ValueType
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
			template<> struct IEnumerable<System::String> : virtual System::Collections::IEnumerable
			{
				IEnumerable<System::String>(decltype(nullptr) n);
				IEnumerable<System::String>(Plugin::InternalUse iu, int32_t handle);
				IEnumerable<System::String>(const IEnumerable<System::String>& other);
				IEnumerable<System::String>(IEnumerable<System::String>&& other);
				virtual ~IEnumerable<System::String>();
				IEnumerable<System::String>& operator=(const IEnumerable<System::String>& other);
				IEnumerable<System::String>& operator=(decltype(nullptr) other);
				IEnumerable<System::String>& operator=(IEnumerable<System::String>&& other);
				bool operator==(const IEnumerable<System::String>& other) const;
				bool operator!=(const IEnumerable<System::String>& other) const;
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
			template<> struct IEnumerable<int32_t> : virtual System::Collections::IEnumerable
			{
				IEnumerable<int32_t>(decltype(nullptr) n);
				IEnumerable<int32_t>(Plugin::InternalUse iu, int32_t handle);
				IEnumerable<int32_t>(const IEnumerable<int32_t>& other);
				IEnumerable<int32_t>(IEnumerable<int32_t>&& other);
				virtual ~IEnumerable<int32_t>();
				IEnumerable<int32_t>& operator=(const IEnumerable<int32_t>& other);
				IEnumerable<int32_t>& operator=(decltype(nullptr) other);
				IEnumerable<int32_t>& operator=(IEnumerable<int32_t>&& other);
				bool operator==(const IEnumerable<int32_t>& other) const;
				bool operator!=(const IEnumerable<int32_t>& other) const;
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
			template<> struct IEnumerable<float> : virtual System::Collections::IEnumerable
			{
				IEnumerable<float>(decltype(nullptr) n);
				IEnumerable<float>(Plugin::InternalUse iu, int32_t handle);
				IEnumerable<float>(const IEnumerable<float>& other);
				IEnumerable<float>(IEnumerable<float>&& other);
				virtual ~IEnumerable<float>();
				IEnumerable<float>& operator=(const IEnumerable<float>& other);
				IEnumerable<float>& operator=(decltype(nullptr) other);
				IEnumerable<float>& operator=(IEnumerable<float>&& other);
				bool operator==(const IEnumerable<float>& other) const;
				bool operator!=(const IEnumerable<float>& other) const;
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
			template<> struct IEnumerable<UnityEngine::RaycastHit> : virtual System::Collections::IEnumerable
			{
				IEnumerable<UnityEngine::RaycastHit>(decltype(nullptr) n);
				IEnumerable<UnityEngine::RaycastHit>(Plugin::InternalUse iu, int32_t handle);
				IEnumerable<UnityEngine::RaycastHit>(const IEnumerable<UnityEngine::RaycastHit>& other);
				IEnumerable<UnityEngine::RaycastHit>(IEnumerable<UnityEngine::RaycastHit>&& other);
				virtual ~IEnumerable<UnityEngine::RaycastHit>();
				IEnumerable<UnityEngine::RaycastHit>& operator=(const IEnumerable<UnityEngine::RaycastHit>& other);
				IEnumerable<UnityEngine::RaycastHit>& operator=(decltype(nullptr) other);
				IEnumerable<UnityEngine::RaycastHit>& operator=(IEnumerable<UnityEngine::RaycastHit>&& other);
				bool operator==(const IEnumerable<UnityEngine::RaycastHit>& other) const;
				bool operator!=(const IEnumerable<UnityEngine::RaycastHit>& other) const;
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
			template<> struct IEnumerable<UnityEngine::GradientColorKey> : virtual System::Collections::IEnumerable
			{
				IEnumerable<UnityEngine::GradientColorKey>(decltype(nullptr) n);
				IEnumerable<UnityEngine::GradientColorKey>(Plugin::InternalUse iu, int32_t handle);
				IEnumerable<UnityEngine::GradientColorKey>(const IEnumerable<UnityEngine::GradientColorKey>& other);
				IEnumerable<UnityEngine::GradientColorKey>(IEnumerable<UnityEngine::GradientColorKey>&& other);
				virtual ~IEnumerable<UnityEngine::GradientColorKey>();
				IEnumerable<UnityEngine::GradientColorKey>& operator=(const IEnumerable<UnityEngine::GradientColorKey>& other);
				IEnumerable<UnityEngine::GradientColorKey>& operator=(decltype(nullptr) other);
				IEnumerable<UnityEngine::GradientColorKey>& operator=(IEnumerable<UnityEngine::GradientColorKey>&& other);
				bool operator==(const IEnumerable<UnityEngine::GradientColorKey>& other) const;
				bool operator!=(const IEnumerable<UnityEngine::GradientColorKey>& other) const;
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
			template<> struct IEnumerable<UnityEngine::Resolution> : virtual System::Collections::IEnumerable
			{
				IEnumerable<UnityEngine::Resolution>(decltype(nullptr) n);
				IEnumerable<UnityEngine::Resolution>(Plugin::InternalUse iu, int32_t handle);
				IEnumerable<UnityEngine::Resolution>(const IEnumerable<UnityEngine::Resolution>& other);
				IEnumerable<UnityEngine::Resolution>(IEnumerable<UnityEngine::Resolution>&& other);
				virtual ~IEnumerable<UnityEngine::Resolution>();
				IEnumerable<UnityEngine::Resolution>& operator=(const IEnumerable<UnityEngine::Resolution>& other);
				IEnumerable<UnityEngine::Resolution>& operator=(decltype(nullptr) other);
				IEnumerable<UnityEngine::Resolution>& operator=(IEnumerable<UnityEngine::Resolution>&& other);
				bool operator==(const IEnumerable<UnityEngine::Resolution>& other) const;
				bool operator!=(const IEnumerable<UnityEngine::Resolution>& other) const;
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
			template<> struct ICollection<System::String> : virtual System::Collections::Generic::IEnumerable<System::String>
			{
				ICollection<System::String>(decltype(nullptr) n);
				ICollection<System::String>(Plugin::InternalUse iu, int32_t handle);
				ICollection<System::String>(const ICollection<System::String>& other);
				ICollection<System::String>(ICollection<System::String>&& other);
				virtual ~ICollection<System::String>();
				ICollection<System::String>& operator=(const ICollection<System::String>& other);
				ICollection<System::String>& operator=(decltype(nullptr) other);
				ICollection<System::String>& operator=(ICollection<System::String>&& other);
				bool operator==(const ICollection<System::String>& other) const;
				bool operator!=(const ICollection<System::String>& other) const;
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
			template<> struct ICollection<int32_t> : virtual System::Collections::Generic::IEnumerable<int32_t>
			{
				ICollection<int32_t>(decltype(nullptr) n);
				ICollection<int32_t>(Plugin::InternalUse iu, int32_t handle);
				ICollection<int32_t>(const ICollection<int32_t>& other);
				ICollection<int32_t>(ICollection<int32_t>&& other);
				virtual ~ICollection<int32_t>();
				ICollection<int32_t>& operator=(const ICollection<int32_t>& other);
				ICollection<int32_t>& operator=(decltype(nullptr) other);
				ICollection<int32_t>& operator=(ICollection<int32_t>&& other);
				bool operator==(const ICollection<int32_t>& other) const;
				bool operator!=(const ICollection<int32_t>& other) const;
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
			template<> struct ICollection<float> : virtual System::Collections::Generic::IEnumerable<float>
			{
				ICollection<float>(decltype(nullptr) n);
				ICollection<float>(Plugin::InternalUse iu, int32_t handle);
				ICollection<float>(const ICollection<float>& other);
				ICollection<float>(ICollection<float>&& other);
				virtual ~ICollection<float>();
				ICollection<float>& operator=(const ICollection<float>& other);
				ICollection<float>& operator=(decltype(nullptr) other);
				ICollection<float>& operator=(ICollection<float>&& other);
				bool operator==(const ICollection<float>& other) const;
				bool operator!=(const ICollection<float>& other) const;
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
			template<> struct ICollection<UnityEngine::RaycastHit> : virtual System::Collections::Generic::IEnumerable<UnityEngine::RaycastHit>
			{
				ICollection<UnityEngine::RaycastHit>(decltype(nullptr) n);
				ICollection<UnityEngine::RaycastHit>(Plugin::InternalUse iu, int32_t handle);
				ICollection<UnityEngine::RaycastHit>(const ICollection<UnityEngine::RaycastHit>& other);
				ICollection<UnityEngine::RaycastHit>(ICollection<UnityEngine::RaycastHit>&& other);
				virtual ~ICollection<UnityEngine::RaycastHit>();
				ICollection<UnityEngine::RaycastHit>& operator=(const ICollection<UnityEngine::RaycastHit>& other);
				ICollection<UnityEngine::RaycastHit>& operator=(decltype(nullptr) other);
				ICollection<UnityEngine::RaycastHit>& operator=(ICollection<UnityEngine::RaycastHit>&& other);
				bool operator==(const ICollection<UnityEngine::RaycastHit>& other) const;
				bool operator!=(const ICollection<UnityEngine::RaycastHit>& other) const;
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
			template<> struct ICollection<UnityEngine::GradientColorKey> : virtual System::Collections::Generic::IEnumerable<UnityEngine::GradientColorKey>
			{
				ICollection<UnityEngine::GradientColorKey>(decltype(nullptr) n);
				ICollection<UnityEngine::GradientColorKey>(Plugin::InternalUse iu, int32_t handle);
				ICollection<UnityEngine::GradientColorKey>(const ICollection<UnityEngine::GradientColorKey>& other);
				ICollection<UnityEngine::GradientColorKey>(ICollection<UnityEngine::GradientColorKey>&& other);
				virtual ~ICollection<UnityEngine::GradientColorKey>();
				ICollection<UnityEngine::GradientColorKey>& operator=(const ICollection<UnityEngine::GradientColorKey>& other);
				ICollection<UnityEngine::GradientColorKey>& operator=(decltype(nullptr) other);
				ICollection<UnityEngine::GradientColorKey>& operator=(ICollection<UnityEngine::GradientColorKey>&& other);
				bool operator==(const ICollection<UnityEngine::GradientColorKey>& other) const;
				bool operator!=(const ICollection<UnityEngine::GradientColorKey>& other) const;
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
			template<> struct ICollection<UnityEngine::Resolution> : virtual System::Collections::Generic::IEnumerable<UnityEngine::Resolution>
			{
				ICollection<UnityEngine::Resolution>(decltype(nullptr) n);
				ICollection<UnityEngine::Resolution>(Plugin::InternalUse iu, int32_t handle);
				ICollection<UnityEngine::Resolution>(const ICollection<UnityEngine::Resolution>& other);
				ICollection<UnityEngine::Resolution>(ICollection<UnityEngine::Resolution>&& other);
				virtual ~ICollection<UnityEngine::Resolution>();
				ICollection<UnityEngine::Resolution>& operator=(const ICollection<UnityEngine::Resolution>& other);
				ICollection<UnityEngine::Resolution>& operator=(decltype(nullptr) other);
				ICollection<UnityEngine::Resolution>& operator=(ICollection<UnityEngine::Resolution>&& other);
				bool operator==(const ICollection<UnityEngine::Resolution>& other) const;
				bool operator!=(const ICollection<UnityEngine::Resolution>& other) const;
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
			template<> struct IList<System::String> : virtual System::Collections::Generic::ICollection<System::String>
			{
				IList<System::String>(decltype(nullptr) n);
				IList<System::String>(Plugin::InternalUse iu, int32_t handle);
				IList<System::String>(const IList<System::String>& other);
				IList<System::String>(IList<System::String>&& other);
				virtual ~IList<System::String>();
				IList<System::String>& operator=(const IList<System::String>& other);
				IList<System::String>& operator=(decltype(nullptr) other);
				IList<System::String>& operator=(IList<System::String>&& other);
				bool operator==(const IList<System::String>& other) const;
				bool operator!=(const IList<System::String>& other) const;
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
			template<> struct IList<int32_t> : virtual System::Collections::Generic::ICollection<int32_t>
			{
				IList<int32_t>(decltype(nullptr) n);
				IList<int32_t>(Plugin::InternalUse iu, int32_t handle);
				IList<int32_t>(const IList<int32_t>& other);
				IList<int32_t>(IList<int32_t>&& other);
				virtual ~IList<int32_t>();
				IList<int32_t>& operator=(const IList<int32_t>& other);
				IList<int32_t>& operator=(decltype(nullptr) other);
				IList<int32_t>& operator=(IList<int32_t>&& other);
				bool operator==(const IList<int32_t>& other) const;
				bool operator!=(const IList<int32_t>& other) const;
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
			template<> struct IList<float> : virtual System::Collections::Generic::ICollection<float>
			{
				IList<float>(decltype(nullptr) n);
				IList<float>(Plugin::InternalUse iu, int32_t handle);
				IList<float>(const IList<float>& other);
				IList<float>(IList<float>&& other);
				virtual ~IList<float>();
				IList<float>& operator=(const IList<float>& other);
				IList<float>& operator=(decltype(nullptr) other);
				IList<float>& operator=(IList<float>&& other);
				bool operator==(const IList<float>& other) const;
				bool operator!=(const IList<float>& other) const;
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
			template<> struct IList<UnityEngine::RaycastHit> : virtual System::Collections::Generic::ICollection<UnityEngine::RaycastHit>
			{
				IList<UnityEngine::RaycastHit>(decltype(nullptr) n);
				IList<UnityEngine::RaycastHit>(Plugin::InternalUse iu, int32_t handle);
				IList<UnityEngine::RaycastHit>(const IList<UnityEngine::RaycastHit>& other);
				IList<UnityEngine::RaycastHit>(IList<UnityEngine::RaycastHit>&& other);
				virtual ~IList<UnityEngine::RaycastHit>();
				IList<UnityEngine::RaycastHit>& operator=(const IList<UnityEngine::RaycastHit>& other);
				IList<UnityEngine::RaycastHit>& operator=(decltype(nullptr) other);
				IList<UnityEngine::RaycastHit>& operator=(IList<UnityEngine::RaycastHit>&& other);
				bool operator==(const IList<UnityEngine::RaycastHit>& other) const;
				bool operator!=(const IList<UnityEngine::RaycastHit>& other) const;
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
			template<> struct IList<UnityEngine::GradientColorKey> : virtual System::Collections::Generic::ICollection<UnityEngine::GradientColorKey>
			{
				IList<UnityEngine::GradientColorKey>(decltype(nullptr) n);
				IList<UnityEngine::GradientColorKey>(Plugin::InternalUse iu, int32_t handle);
				IList<UnityEngine::GradientColorKey>(const IList<UnityEngine::GradientColorKey>& other);
				IList<UnityEngine::GradientColorKey>(IList<UnityEngine::GradientColorKey>&& other);
				virtual ~IList<UnityEngine::GradientColorKey>();
				IList<UnityEngine::GradientColorKey>& operator=(const IList<UnityEngine::GradientColorKey>& other);
				IList<UnityEngine::GradientColorKey>& operator=(decltype(nullptr) other);
				IList<UnityEngine::GradientColorKey>& operator=(IList<UnityEngine::GradientColorKey>&& other);
				bool operator==(const IList<UnityEngine::GradientColorKey>& other) const;
				bool operator!=(const IList<UnityEngine::GradientColorKey>& other) const;
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
			template<> struct IList<UnityEngine::Resolution> : virtual System::Collections::Generic::ICollection<UnityEngine::Resolution>
			{
				IList<UnityEngine::Resolution>(decltype(nullptr) n);
				IList<UnityEngine::Resolution>(Plugin::InternalUse iu, int32_t handle);
				IList<UnityEngine::Resolution>(const IList<UnityEngine::Resolution>& other);
				IList<UnityEngine::Resolution>(IList<UnityEngine::Resolution>&& other);
				virtual ~IList<UnityEngine::Resolution>();
				IList<UnityEngine::Resolution>& operator=(const IList<UnityEngine::Resolution>& other);
				IList<UnityEngine::Resolution>& operator=(decltype(nullptr) other);
				IList<UnityEngine::Resolution>& operator=(IList<UnityEngine::Resolution>&& other);
				bool operator==(const IList<UnityEngine::Resolution>& other) const;
				bool operator!=(const IList<UnityEngine::Resolution>& other) const;
			};
		}
	}
}

namespace System
{
	namespace Runtime
	{
		namespace Serialization
		{
			struct ISerializable : virtual System::Object
			{
				ISerializable(decltype(nullptr) n);
				ISerializable(Plugin::InternalUse iu, int32_t handle);
				ISerializable(const ISerializable& other);
				ISerializable(ISerializable&& other);
				virtual ~ISerializable();
				ISerializable& operator=(const ISerializable& other);
				ISerializable& operator=(decltype(nullptr) other);
				ISerializable& operator=(ISerializable&& other);
				bool operator==(const ISerializable& other) const;
				bool operator!=(const ISerializable& other) const;
			};
		}
	}
}

namespace System
{
	namespace Runtime
	{
		namespace InteropServices
		{
			struct _Exception : virtual System::Object
			{
				_Exception(decltype(nullptr) n);
				_Exception(Plugin::InternalUse iu, int32_t handle);
				_Exception(const _Exception& other);
				_Exception(_Exception&& other);
				virtual ~_Exception();
				_Exception& operator=(const _Exception& other);
				_Exception& operator=(decltype(nullptr) other);
				_Exception& operator=(_Exception&& other);
				bool operator==(const _Exception& other) const;
				bool operator!=(const _Exception& other) const;
			};
		}
	}
}

namespace System
{
	struct IAppDomainSetup : virtual System::Object
	{
		IAppDomainSetup(decltype(nullptr) n);
		IAppDomainSetup(Plugin::InternalUse iu, int32_t handle);
		IAppDomainSetup(const IAppDomainSetup& other);
		IAppDomainSetup(IAppDomainSetup&& other);
		virtual ~IAppDomainSetup();
		IAppDomainSetup& operator=(const IAppDomainSetup& other);
		IAppDomainSetup& operator=(decltype(nullptr) other);
		IAppDomainSetup& operator=(IAppDomainSetup&& other);
		bool operator==(const IAppDomainSetup& other) const;
		bool operator!=(const IAppDomainSetup& other) const;
	};
}

namespace System
{
	namespace Collections
	{
		struct IComparer : virtual System::Object
		{
			IComparer(decltype(nullptr) n);
			IComparer(Plugin::InternalUse iu, int32_t handle);
			IComparer(const IComparer& other);
			IComparer(IComparer&& other);
			virtual ~IComparer();
			IComparer& operator=(const IComparer& other);
			IComparer& operator=(decltype(nullptr) other);
			IComparer& operator=(IComparer&& other);
			bool operator==(const IComparer& other) const;
			bool operator!=(const IComparer& other) const;
		};
	}
}

namespace System
{
	namespace Collections
	{
		struct IEqualityComparer : virtual System::Object
		{
			IEqualityComparer(decltype(nullptr) n);
			IEqualityComparer(Plugin::InternalUse iu, int32_t handle);
			IEqualityComparer(const IEqualityComparer& other);
			IEqualityComparer(IEqualityComparer&& other);
			virtual ~IEqualityComparer();
			IEqualityComparer& operator=(const IEqualityComparer& other);
			IEqualityComparer& operator=(decltype(nullptr) other);
			IEqualityComparer& operator=(IEqualityComparer&& other);
			bool operator==(const IEqualityComparer& other) const;
			bool operator!=(const IEqualityComparer& other) const;
		};
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			template<> struct IEqualityComparer<System::String> : virtual System::Object
			{
				IEqualityComparer<System::String>(decltype(nullptr) n);
				IEqualityComparer<System::String>(Plugin::InternalUse iu, int32_t handle);
				IEqualityComparer<System::String>(const IEqualityComparer<System::String>& other);
				IEqualityComparer<System::String>(IEqualityComparer<System::String>&& other);
				virtual ~IEqualityComparer<System::String>();
				IEqualityComparer<System::String>& operator=(const IEqualityComparer<System::String>& other);
				IEqualityComparer<System::String>& operator=(decltype(nullptr) other);
				IEqualityComparer<System::String>& operator=(IEqualityComparer<System::String>&& other);
				bool operator==(const IEqualityComparer<System::String>& other) const;
				bool operator!=(const IEqualityComparer<System::String>& other) const;
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
			template<> struct IEqualityComparer<int32_t> : virtual System::Object
			{
				IEqualityComparer<int32_t>(decltype(nullptr) n);
				IEqualityComparer<int32_t>(Plugin::InternalUse iu, int32_t handle);
				IEqualityComparer<int32_t>(const IEqualityComparer<int32_t>& other);
				IEqualityComparer<int32_t>(IEqualityComparer<int32_t>&& other);
				virtual ~IEqualityComparer<int32_t>();
				IEqualityComparer<int32_t>& operator=(const IEqualityComparer<int32_t>& other);
				IEqualityComparer<int32_t>& operator=(decltype(nullptr) other);
				IEqualityComparer<int32_t>& operator=(IEqualityComparer<int32_t>&& other);
				bool operator==(const IEqualityComparer<int32_t>& other) const;
				bool operator!=(const IEqualityComparer<int32_t>& other) const;
			};
		}
	}
}

namespace UnityEngine
{
	namespace Playables
	{
		struct PlayableGraph : virtual System::ValueType
		{
			PlayableGraph(decltype(nullptr) n);
			PlayableGraph(Plugin::InternalUse iu, int32_t handle);
			PlayableGraph(const PlayableGraph& other);
			PlayableGraph(PlayableGraph&& other);
			virtual ~PlayableGraph();
			PlayableGraph& operator=(const PlayableGraph& other);
			PlayableGraph& operator=(decltype(nullptr) other);
			PlayableGraph& operator=(PlayableGraph&& other);
			bool operator==(const PlayableGraph& other) const;
			bool operator!=(const PlayableGraph& other) const;
		};
	}
}

namespace UnityEngine
{
	namespace Playables
	{
		struct IPlayable : virtual System::Object
		{
			IPlayable(decltype(nullptr) n);
			IPlayable(Plugin::InternalUse iu, int32_t handle);
			IPlayable(const IPlayable& other);
			IPlayable(IPlayable&& other);
			virtual ~IPlayable();
			IPlayable& operator=(const IPlayable& other);
			IPlayable& operator=(decltype(nullptr) other);
			IPlayable& operator=(IPlayable&& other);
			bool operator==(const IPlayable& other) const;
			bool operator!=(const IPlayable& other) const;
		};
	}
}

namespace System
{
	template<> struct IEquatable<UnityEngine::Animations::AnimationMixerPlayable> : virtual System::Object
	{
		IEquatable<UnityEngine::Animations::AnimationMixerPlayable>(decltype(nullptr) n);
		IEquatable<UnityEngine::Animations::AnimationMixerPlayable>(Plugin::InternalUse iu, int32_t handle);
		IEquatable<UnityEngine::Animations::AnimationMixerPlayable>(const IEquatable<UnityEngine::Animations::AnimationMixerPlayable>& other);
		IEquatable<UnityEngine::Animations::AnimationMixerPlayable>(IEquatable<UnityEngine::Animations::AnimationMixerPlayable>&& other);
		virtual ~IEquatable<UnityEngine::Animations::AnimationMixerPlayable>();
		IEquatable<UnityEngine::Animations::AnimationMixerPlayable>& operator=(const IEquatable<UnityEngine::Animations::AnimationMixerPlayable>& other);
		IEquatable<UnityEngine::Animations::AnimationMixerPlayable>& operator=(decltype(nullptr) other);
		IEquatable<UnityEngine::Animations::AnimationMixerPlayable>& operator=(IEquatable<UnityEngine::Animations::AnimationMixerPlayable>&& other);
		bool operator==(const IEquatable<UnityEngine::Animations::AnimationMixerPlayable>& other) const;
		bool operator!=(const IEquatable<UnityEngine::Animations::AnimationMixerPlayable>& other) const;
	};
}

namespace UnityEngine
{
	namespace Animations
	{
		struct AnimationMixerPlayable : virtual System::ValueType, virtual System::IEquatable<UnityEngine::Animations::AnimationMixerPlayable>, virtual UnityEngine::Playables::IPlayable
		{
			AnimationMixerPlayable(decltype(nullptr) n);
			AnimationMixerPlayable(Plugin::InternalUse iu, int32_t handle);
			AnimationMixerPlayable(const AnimationMixerPlayable& other);
			AnimationMixerPlayable(AnimationMixerPlayable&& other);
			virtual ~AnimationMixerPlayable();
			AnimationMixerPlayable& operator=(const AnimationMixerPlayable& other);
			AnimationMixerPlayable& operator=(decltype(nullptr) other);
			AnimationMixerPlayable& operator=(AnimationMixerPlayable&& other);
			bool operator==(const AnimationMixerPlayable& other) const;
			bool operator!=(const AnimationMixerPlayable& other) const;
			static UnityEngine::Animations::AnimationMixerPlayable Create(UnityEngine::Playables::PlayableGraph& graph, int32_t inputCount = 0, System::Boolean normalizeWeights = false);
		};
	}
}

namespace System
{
	namespace Runtime
	{
		namespace CompilerServices
		{
			struct IStrongBox : virtual System::Object
			{
				IStrongBox(decltype(nullptr) n);
				IStrongBox(Plugin::InternalUse iu, int32_t handle);
				IStrongBox(const IStrongBox& other);
				IStrongBox(IStrongBox&& other);
				virtual ~IStrongBox();
				IStrongBox& operator=(const IStrongBox& other);
				IStrongBox& operator=(decltype(nullptr) other);
				IStrongBox& operator=(IStrongBox&& other);
				bool operator==(const IStrongBox& other) const;
				bool operator!=(const IStrongBox& other) const;
			};
		}
	}
}

namespace UnityEngine
{
	namespace Experimental
	{
		namespace UIElements
		{
			struct IEventHandler : virtual System::Object
			{
				IEventHandler(decltype(nullptr) n);
				IEventHandler(Plugin::InternalUse iu, int32_t handle);
				IEventHandler(const IEventHandler& other);
				IEventHandler(IEventHandler&& other);
				virtual ~IEventHandler();
				IEventHandler& operator=(const IEventHandler& other);
				IEventHandler& operator=(decltype(nullptr) other);
				IEventHandler& operator=(IEventHandler&& other);
				bool operator==(const IEventHandler& other) const;
				bool operator!=(const IEventHandler& other) const;
			};
		}
	}
}

namespace UnityEngine
{
	namespace Experimental
	{
		namespace UIElements
		{
			struct IStyle : virtual System::Object
			{
				IStyle(decltype(nullptr) n);
				IStyle(Plugin::InternalUse iu, int32_t handle);
				IStyle(const IStyle& other);
				IStyle(IStyle&& other);
				virtual ~IStyle();
				IStyle& operator=(const IStyle& other);
				IStyle& operator=(decltype(nullptr) other);
				IStyle& operator=(IStyle&& other);
				bool operator==(const IStyle& other) const;
				bool operator!=(const IStyle& other) const;
			};
		}
	}
}

namespace System
{
	namespace Diagnostics
	{
		struct Stopwatch : virtual System::Object
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
	struct GameObject : virtual UnityEngine::Object
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
		GameObject(System::String& name);
		UnityEngine::Transform GetTransform();
		template<typename MT0> MT0 AddComponent();
		static UnityEngine::GameObject CreatePrimitive(UnityEngine::PrimitiveType type);
	};
}

namespace UnityEngine
{
	struct Debug : virtual System::Object
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
		static void Log(System::Object& message);
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
			template<typename MT0> void AreEqual(MT0& expected, MT0& actual);
		}
	}
}

namespace UnityEngine
{
	struct Collision : virtual System::Object
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
	struct Behaviour : virtual UnityEngine::Component
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
	struct MonoBehaviour : virtual UnityEngine::Behaviour
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
		UnityEngine::Transform GetTransform();
	};
}

namespace UnityEngine
{
	struct AudioSettings : virtual System::Object
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
		struct NetworkTransport : virtual System::Object
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
	struct Quaternion
	{
		Quaternion();
		float x;
		float y;
		float z;
		float w;
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

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			template<> struct KeyValuePair<System::String, double> : virtual System::ValueType
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
				KeyValuePair(System::String& key, double value);
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
			template<> struct List<System::String> : virtual System::Collections::IList, virtual System::Collections::Generic::IList<System::String>
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
				void SetItem(int32_t index, System::String& value);
				void Add(System::String& item);
				void Sort(System::Collections::Generic::IComparer<System::String>& comparer);
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
			template<> struct List<int32_t> : virtual System::Collections::IList, virtual System::Collections::Generic::IList<int32_t>
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
				void Sort(System::Collections::Generic::IComparer<int32_t>& comparer);
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
			template<> struct LinkedListNode<System::String> : virtual System::Object
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
				LinkedListNode(System::String& value);
				System::String GetValue();
				void SetValue(System::String& value);
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
			template<> struct StrongBox<System::String> : virtual System::Runtime::CompilerServices::IStrongBox
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
				StrongBox(System::String& value);
				System::String GetValue();
				void SetValue(System::String& value);
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
			template<> struct Collection<int32_t> : virtual System::Collections::IList, virtual System::Collections::Generic::IList<int32_t>
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
			template<> struct KeyedCollection<System::String, int32_t> : virtual System::Collections::ObjectModel::Collection<int32_t>, virtual System::Collections::IList, virtual System::Collections::Generic::IList<int32_t>
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
	struct Exception : virtual System::Runtime::InteropServices::_Exception, virtual System::Runtime::Serialization::ISerializable
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
		Exception(System::String& message);
	};
}

namespace System
{
	struct SystemException : virtual System::Exception, virtual System::Runtime::InteropServices::_Exception, virtual System::Runtime::Serialization::ISerializable
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
	struct NullReferenceException : virtual System::SystemException, virtual System::Runtime::InteropServices::_Exception, virtual System::Runtime::Serialization::ISerializable
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
	struct Screen : virtual System::Object
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
	struct Ray : virtual System::ValueType
	{
		Ray(decltype(nullptr) n);
		Ray(Plugin::InternalUse iu, int32_t handle);
		Ray(const Ray& other);
		Ray(Ray&& other);
		virtual ~Ray();
		Ray& operator=(const Ray& other);
		Ray& operator=(decltype(nullptr) other);
		Ray& operator=(Ray&& other);
		bool operator==(const Ray& other) const;
		bool operator!=(const Ray& other) const;
		Ray(UnityEngine::Vector3& origin, UnityEngine::Vector3& direction);
	};
}

namespace UnityEngine
{
	struct Physics : virtual System::Object
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
		static int32_t RaycastNonAlloc(UnityEngine::Ray& ray, System::Array1<UnityEngine::RaycastHit>& results);
		static System::Array1<UnityEngine::RaycastHit> RaycastAll(UnityEngine::Ray& ray);
	};
}

namespace UnityEngine
{
	struct Gradient : virtual System::Object
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
		void SetColorKeys(System::Array1<UnityEngine::GradientColorKey>& value);
	};
}

namespace System
{
	struct AppDomainSetup : virtual System::IAppDomainSetup
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
		void SetAppDomainInitializer(System::AppDomainInitializer& value);
	};
}

namespace UnityEngine
{
	struct Application : virtual System::Object
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
		static void AddOnBeforeRender(UnityEngine::Events::UnityAction& del);
		static void RemoveOnBeforeRender(UnityEngine::Events::UnityAction& del);
	};
}

namespace UnityEngine
{
	namespace SceneManagement
	{
		struct SceneManager : virtual System::Object
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
			static void AddSceneLoaded(UnityEngine::Events::UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>& del);
			static void RemoveSceneLoaded(UnityEngine::Events::UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>& del);
		};
	}
}

namespace UnityEngine
{
	namespace SceneManagement
	{
		struct Scene : virtual System::ValueType
		{
			Scene(decltype(nullptr) n);
			Scene(Plugin::InternalUse iu, int32_t handle);
			Scene(const Scene& other);
			Scene(Scene&& other);
			virtual ~Scene();
			Scene& operator=(const Scene& other);
			Scene& operator=(decltype(nullptr) other);
			Scene& operator=(Scene&& other);
			bool operator==(const Scene& other) const;
			bool operator!=(const Scene& other) const;
		};
	}
}

namespace System
{
	namespace Collections
	{
		struct IEnumerator : virtual System::Object
		{
			IEnumerator(decltype(nullptr) n);
			IEnumerator(Plugin::InternalUse iu, int32_t handle);
			IEnumerator(const IEnumerator& other);
			IEnumerator(IEnumerator&& other);
			virtual ~IEnumerator();
			IEnumerator& operator=(const IEnumerator& other);
			IEnumerator& operator=(decltype(nullptr) other);
			IEnumerator& operator=(IEnumerator&& other);
			bool operator==(const IEnumerator& other) const;
			bool operator!=(const IEnumerator& other) const;
			System::Object GetCurrent();
			System::Boolean MoveNext();
		};
	}
}

namespace System
{
	struct EventArgs : virtual System::Object
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
	};
}

namespace System
{
	namespace ComponentModel
	{
		namespace Design
		{
			struct ComponentEventArgs : virtual System::EventArgs
			{
				ComponentEventArgs(decltype(nullptr) n);
				ComponentEventArgs(Plugin::InternalUse iu, int32_t handle);
				ComponentEventArgs(const ComponentEventArgs& other);
				ComponentEventArgs(ComponentEventArgs&& other);
				virtual ~ComponentEventArgs();
				ComponentEventArgs& operator=(const ComponentEventArgs& other);
				ComponentEventArgs& operator=(decltype(nullptr) other);
				ComponentEventArgs& operator=(ComponentEventArgs&& other);
				bool operator==(const ComponentEventArgs& other) const;
				bool operator!=(const ComponentEventArgs& other) const;
			};
		}
	}
}

namespace System
{
	namespace ComponentModel
	{
		namespace Design
		{
			struct ComponentChangingEventArgs : virtual System::EventArgs
			{
				ComponentChangingEventArgs(decltype(nullptr) n);
				ComponentChangingEventArgs(Plugin::InternalUse iu, int32_t handle);
				ComponentChangingEventArgs(const ComponentChangingEventArgs& other);
				ComponentChangingEventArgs(ComponentChangingEventArgs&& other);
				virtual ~ComponentChangingEventArgs();
				ComponentChangingEventArgs& operator=(const ComponentChangingEventArgs& other);
				ComponentChangingEventArgs& operator=(decltype(nullptr) other);
				ComponentChangingEventArgs& operator=(ComponentChangingEventArgs&& other);
				bool operator==(const ComponentChangingEventArgs& other) const;
				bool operator!=(const ComponentChangingEventArgs& other) const;
			};
		}
	}
}

namespace System
{
	namespace ComponentModel
	{
		namespace Design
		{
			struct ComponentChangedEventArgs : virtual System::EventArgs
			{
				ComponentChangedEventArgs(decltype(nullptr) n);
				ComponentChangedEventArgs(Plugin::InternalUse iu, int32_t handle);
				ComponentChangedEventArgs(const ComponentChangedEventArgs& other);
				ComponentChangedEventArgs(ComponentChangedEventArgs&& other);
				virtual ~ComponentChangedEventArgs();
				ComponentChangedEventArgs& operator=(const ComponentChangedEventArgs& other);
				ComponentChangedEventArgs& operator=(decltype(nullptr) other);
				ComponentChangedEventArgs& operator=(ComponentChangedEventArgs&& other);
				bool operator==(const ComponentChangedEventArgs& other) const;
				bool operator!=(const ComponentChangedEventArgs& other) const;
			};
		}
	}
}

namespace System
{
	namespace ComponentModel
	{
		namespace Design
		{
			struct ComponentRenameEventArgs : virtual System::EventArgs
			{
				ComponentRenameEventArgs(decltype(nullptr) n);
				ComponentRenameEventArgs(Plugin::InternalUse iu, int32_t handle);
				ComponentRenameEventArgs(const ComponentRenameEventArgs& other);
				ComponentRenameEventArgs(ComponentRenameEventArgs&& other);
				virtual ~ComponentRenameEventArgs();
				ComponentRenameEventArgs& operator=(const ComponentRenameEventArgs& other);
				ComponentRenameEventArgs& operator=(decltype(nullptr) other);
				ComponentRenameEventArgs& operator=(ComponentRenameEventArgs&& other);
				bool operator==(const ComponentRenameEventArgs& other) const;
				bool operator!=(const ComponentRenameEventArgs& other) const;
			};
		}
	}
}

namespace System
{
	namespace ComponentModel
	{
		struct MemberDescriptor : virtual System::Object
		{
			MemberDescriptor(decltype(nullptr) n);
			MemberDescriptor(Plugin::InternalUse iu, int32_t handle);
			MemberDescriptor(const MemberDescriptor& other);
			MemberDescriptor(MemberDescriptor&& other);
			virtual ~MemberDescriptor();
			MemberDescriptor& operator=(const MemberDescriptor& other);
			MemberDescriptor& operator=(decltype(nullptr) other);
			MemberDescriptor& operator=(MemberDescriptor&& other);
			bool operator==(const MemberDescriptor& other) const;
			bool operator!=(const MemberDescriptor& other) const;
		};
	}
}

namespace UnityEngine
{
	struct Time : virtual System::Object
	{
		Time(decltype(nullptr) n);
		Time(Plugin::InternalUse iu, int32_t handle);
		Time(const Time& other);
		Time(Time&& other);
		virtual ~Time();
		Time& operator=(const Time& other);
		Time& operator=(decltype(nullptr) other);
		Time& operator=(Time&& other);
		bool operator==(const Time& other) const;
		bool operator!=(const Time& other) const;
		static float GetDeltaTime();
	};
}

namespace System
{
	struct MarshalByRefObject : virtual System::Object
	{
		MarshalByRefObject(decltype(nullptr) n);
		MarshalByRefObject(Plugin::InternalUse iu, int32_t handle);
		MarshalByRefObject(const MarshalByRefObject& other);
		MarshalByRefObject(MarshalByRefObject&& other);
		virtual ~MarshalByRefObject();
		MarshalByRefObject& operator=(const MarshalByRefObject& other);
		MarshalByRefObject& operator=(decltype(nullptr) other);
		MarshalByRefObject& operator=(MarshalByRefObject&& other);
		bool operator==(const MarshalByRefObject& other) const;
		bool operator!=(const MarshalByRefObject& other) const;
	};
}

namespace System
{
	namespace IO
	{
		struct Stream : virtual System::MarshalByRefObject, virtual System::IDisposable
		{
			Stream(decltype(nullptr) n);
			Stream(Plugin::InternalUse iu, int32_t handle);
			Stream(const Stream& other);
			Stream(Stream&& other);
			virtual ~Stream();
			Stream& operator=(const Stream& other);
			Stream& operator=(decltype(nullptr) other);
			Stream& operator=(Stream&& other);
			bool operator==(const Stream& other) const;
			bool operator!=(const Stream& other) const;
		};
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			template<> struct IComparer<int32_t> : virtual System::Object
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
			template<> struct IComparer<System::String> : virtual System::Object
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
			template<> struct BaseIComparer<int32_t> : virtual System::Collections::Generic::IComparer<int32_t>
			{
				BaseIComparer<int32_t>(decltype(nullptr) n);
				BaseIComparer<int32_t>(Plugin::InternalUse iu, int32_t handle);
				BaseIComparer<int32_t>(const BaseIComparer<int32_t>& other);
				BaseIComparer<int32_t>(BaseIComparer<int32_t>&& other);
				virtual ~BaseIComparer<int32_t>();
				BaseIComparer<int32_t>& operator=(const BaseIComparer<int32_t>& other);
				BaseIComparer<int32_t>& operator=(decltype(nullptr) other);
				BaseIComparer<int32_t>& operator=(BaseIComparer<int32_t>&& other);
				bool operator==(const BaseIComparer<int32_t>& other) const;
				bool operator!=(const BaseIComparer<int32_t>& other) const;
				int32_t CppHandle;
				BaseIComparer();
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
			template<> struct BaseIComparer<System::String> : virtual System::Collections::Generic::IComparer<System::String>
			{
				BaseIComparer<System::String>(decltype(nullptr) n);
				BaseIComparer<System::String>(Plugin::InternalUse iu, int32_t handle);
				BaseIComparer<System::String>(const BaseIComparer<System::String>& other);
				BaseIComparer<System::String>(BaseIComparer<System::String>&& other);
				virtual ~BaseIComparer<System::String>();
				BaseIComparer<System::String>& operator=(const BaseIComparer<System::String>& other);
				BaseIComparer<System::String>& operator=(decltype(nullptr) other);
				BaseIComparer<System::String>& operator=(BaseIComparer<System::String>&& other);
				bool operator==(const BaseIComparer<System::String>& other) const;
				bool operator!=(const BaseIComparer<System::String>& other) const;
				int32_t CppHandle;
				BaseIComparer();
				virtual int32_t Compare(System::String& x, System::String& y);
			};
		}
	}
}

namespace System
{
	struct StringComparer : virtual System::Collections::IComparer, virtual System::Collections::Generic::IComparer<System::String>, virtual System::Collections::IEqualityComparer, virtual System::Collections::Generic::IEqualityComparer<System::String>
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
	};
}

namespace System
{
	struct BaseStringComparer : virtual System::StringComparer
	{
		BaseStringComparer(decltype(nullptr) n);
		BaseStringComparer(Plugin::InternalUse iu, int32_t handle);
		BaseStringComparer(const BaseStringComparer& other);
		BaseStringComparer(BaseStringComparer&& other);
		virtual ~BaseStringComparer();
		BaseStringComparer& operator=(const BaseStringComparer& other);
		BaseStringComparer& operator=(decltype(nullptr) other);
		BaseStringComparer& operator=(BaseStringComparer&& other);
		bool operator==(const BaseStringComparer& other) const;
		bool operator!=(const BaseStringComparer& other) const;
		int32_t CppHandle;
		BaseStringComparer();
		virtual int32_t Compare(System::String& x, System::String& y);
		virtual System::Boolean Equals(System::String& x, System::String& y);
		virtual int32_t GetHashCode(System::String& obj);
	};
}

namespace System
{
	namespace Collections
	{
		struct Queue : virtual System::ICloneable, virtual System::Collections::ICollection
		{
			Queue(decltype(nullptr) n);
			Queue(Plugin::InternalUse iu, int32_t handle);
			Queue(const Queue& other);
			Queue(Queue&& other);
			virtual ~Queue();
			Queue& operator=(const Queue& other);
			Queue& operator=(decltype(nullptr) other);
			Queue& operator=(Queue&& other);
			bool operator==(const Queue& other) const;
			bool operator!=(const Queue& other) const;
			int32_t GetCount();
		};
	}
}

namespace System
{
	namespace Collections
	{
		struct BaseQueue : virtual System::Collections::Queue
		{
			BaseQueue(decltype(nullptr) n);
			BaseQueue(Plugin::InternalUse iu, int32_t handle);
			BaseQueue(const BaseQueue& other);
			BaseQueue(BaseQueue&& other);
			virtual ~BaseQueue();
			BaseQueue& operator=(const BaseQueue& other);
			BaseQueue& operator=(decltype(nullptr) other);
			BaseQueue& operator=(BaseQueue&& other);
			bool operator==(const BaseQueue& other) const;
			bool operator!=(const BaseQueue& other) const;
			int32_t CppHandle;
			BaseQueue();
			virtual int32_t GetCount();
		};
	}
}

namespace System
{
	namespace ComponentModel
	{
		namespace Design
		{
			struct IComponentChangeService : virtual System::Object
			{
				IComponentChangeService(decltype(nullptr) n);
				IComponentChangeService(Plugin::InternalUse iu, int32_t handle);
				IComponentChangeService(const IComponentChangeService& other);
				IComponentChangeService(IComponentChangeService&& other);
				virtual ~IComponentChangeService();
				IComponentChangeService& operator=(const IComponentChangeService& other);
				IComponentChangeService& operator=(decltype(nullptr) other);
				IComponentChangeService& operator=(IComponentChangeService&& other);
				bool operator==(const IComponentChangeService& other) const;
				bool operator!=(const IComponentChangeService& other) const;
			};
		}
	}
}

namespace System
{
	namespace ComponentModel
	{
		namespace Design
		{
			struct BaseIComponentChangeService : virtual System::ComponentModel::Design::IComponentChangeService
			{
				BaseIComponentChangeService(decltype(nullptr) n);
				BaseIComponentChangeService(Plugin::InternalUse iu, int32_t handle);
				BaseIComponentChangeService(const BaseIComponentChangeService& other);
				BaseIComponentChangeService(BaseIComponentChangeService&& other);
				virtual ~BaseIComponentChangeService();
				BaseIComponentChangeService& operator=(const BaseIComponentChangeService& other);
				BaseIComponentChangeService& operator=(decltype(nullptr) other);
				BaseIComponentChangeService& operator=(BaseIComponentChangeService&& other);
				bool operator==(const BaseIComponentChangeService& other) const;
				bool operator!=(const BaseIComponentChangeService& other) const;
				int32_t CppHandle;
				BaseIComponentChangeService();
				virtual void OnComponentChanged(System::Object& component, System::ComponentModel::MemberDescriptor& member, System::Object& oldValue, System::Object& newValue);
				virtual void OnComponentChanging(System::Object& component, System::ComponentModel::MemberDescriptor& member);
				virtual void AddComponentAdded(System::ComponentModel::Design::ComponentEventHandler& value);
				virtual void RemoveComponentAdded(System::ComponentModel::Design::ComponentEventHandler& value);
				virtual void AddComponentAdding(System::ComponentModel::Design::ComponentEventHandler& value);
				virtual void RemoveComponentAdding(System::ComponentModel::Design::ComponentEventHandler& value);
				virtual void AddComponentChanged(System::ComponentModel::Design::ComponentChangedEventHandler& value);
				virtual void RemoveComponentChanged(System::ComponentModel::Design::ComponentChangedEventHandler& value);
				virtual void AddComponentChanging(System::ComponentModel::Design::ComponentChangingEventHandler& value);
				virtual void RemoveComponentChanging(System::ComponentModel::Design::ComponentChangingEventHandler& value);
				virtual void AddComponentRemoved(System::ComponentModel::Design::ComponentEventHandler& value);
				virtual void RemoveComponentRemoved(System::ComponentModel::Design::ComponentEventHandler& value);
				virtual void AddComponentRemoving(System::ComponentModel::Design::ComponentEventHandler& value);
				virtual void RemoveComponentRemoving(System::ComponentModel::Design::ComponentEventHandler& value);
				virtual void AddComponentRename(System::ComponentModel::Design::ComponentRenameEventHandler& value);
				virtual void RemoveComponentRename(System::ComponentModel::Design::ComponentRenameEventHandler& value);
			};
		}
	}
}

namespace System
{
	namespace IO
	{
		struct FileStream : virtual System::IO::Stream, virtual System::IDisposable
		{
			FileStream(decltype(nullptr) n);
			FileStream(Plugin::InternalUse iu, int32_t handle);
			FileStream(const FileStream& other);
			FileStream(FileStream&& other);
			virtual ~FileStream();
			FileStream& operator=(const FileStream& other);
			FileStream& operator=(decltype(nullptr) other);
			FileStream& operator=(FileStream&& other);
			bool operator==(const FileStream& other) const;
			bool operator!=(const FileStream& other) const;
			FileStream(System::String& path, System::IO::FileMode mode);
			void WriteByte(uint8_t value);
		};
	}
}

namespace System
{
	namespace IO
	{
		struct BaseFileStream : virtual System::IO::FileStream
		{
			BaseFileStream(decltype(nullptr) n);
			BaseFileStream(Plugin::InternalUse iu, int32_t handle);
			BaseFileStream(const BaseFileStream& other);
			BaseFileStream(BaseFileStream&& other);
			virtual ~BaseFileStream();
			BaseFileStream& operator=(const BaseFileStream& other);
			BaseFileStream& operator=(decltype(nullptr) other);
			BaseFileStream& operator=(BaseFileStream&& other);
			bool operator==(const BaseFileStream& other) const;
			bool operator!=(const BaseFileStream& other) const;
			int32_t CppHandle;
			BaseFileStream(System::String& path, System::IO::FileMode mode);
			virtual void WriteByte(uint8_t value);
		};
	}
}

namespace UnityEngine
{
	namespace Playables
	{
		struct PlayableHandle : virtual System::ValueType
		{
			PlayableHandle(decltype(nullptr) n);
			PlayableHandle(Plugin::InternalUse iu, int32_t handle);
			PlayableHandle(const PlayableHandle& other);
			PlayableHandle(PlayableHandle&& other);
			virtual ~PlayableHandle();
			PlayableHandle& operator=(const PlayableHandle& other);
			PlayableHandle& operator=(decltype(nullptr) other);
			PlayableHandle& operator=(PlayableHandle&& other);
			bool operator==(const PlayableHandle& other) const;
			bool operator!=(const PlayableHandle& other) const;
		};
	}
}

namespace UnityEngine
{
	namespace Experimental
	{
		namespace UIElements
		{
			struct CallbackEventHandler : virtual UnityEngine::Experimental::UIElements::IEventHandler
			{
				CallbackEventHandler(decltype(nullptr) n);
				CallbackEventHandler(Plugin::InternalUse iu, int32_t handle);
				CallbackEventHandler(const CallbackEventHandler& other);
				CallbackEventHandler(CallbackEventHandler&& other);
				virtual ~CallbackEventHandler();
				CallbackEventHandler& operator=(const CallbackEventHandler& other);
				CallbackEventHandler& operator=(decltype(nullptr) other);
				CallbackEventHandler& operator=(CallbackEventHandler&& other);
				bool operator==(const CallbackEventHandler& other) const;
				bool operator!=(const CallbackEventHandler& other) const;
			};
		}
	}
}

namespace UnityEngine
{
	namespace Experimental
	{
		namespace UIElements
		{
			struct VisualElement : virtual UnityEngine::Experimental::UIElements::CallbackEventHandler, virtual UnityEngine::Experimental::UIElements::IEventHandler, virtual UnityEngine::Experimental::UIElements::IStyle
			{
				VisualElement(decltype(nullptr) n);
				VisualElement(Plugin::InternalUse iu, int32_t handle);
				VisualElement(const VisualElement& other);
				VisualElement(VisualElement&& other);
				virtual ~VisualElement();
				VisualElement& operator=(const VisualElement& other);
				VisualElement& operator=(decltype(nullptr) other);
				VisualElement& operator=(VisualElement&& other);
				bool operator==(const VisualElement& other) const;
				bool operator!=(const VisualElement& other) const;
			};
		}
	}
}

namespace UnityEngine
{
	namespace Experimental
	{
		namespace UIElements
		{
			namespace UQueryExtensions
			{
				UnityEngine::Experimental::UIElements::VisualElement Q(UnityEngine::Experimental::UIElements::VisualElement& e, System::String& name, System::Array1<System::String>& classes);
				UnityEngine::Experimental::UIElements::VisualElement Q(UnityEngine::Experimental::UIElements::VisualElement& e, System::String& name = Plugin::NullString, System::String& className = Plugin::NullString);
			}
		}
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
				struct InteractionSourcePose : virtual System::ValueType
				{
					InteractionSourcePose(decltype(nullptr) n);
					InteractionSourcePose(Plugin::InternalUse iu, int32_t handle);
					InteractionSourcePose(const InteractionSourcePose& other);
					InteractionSourcePose(InteractionSourcePose&& other);
					virtual ~InteractionSourcePose();
					InteractionSourcePose& operator=(const InteractionSourcePose& other);
					InteractionSourcePose& operator=(decltype(nullptr) other);
					InteractionSourcePose& operator=(InteractionSourcePose&& other);
					bool operator==(const InteractionSourcePose& other) const;
					bool operator!=(const InteractionSourcePose& other) const;
					System::Boolean TryGetRotation(UnityEngine::Quaternion* rotation, UnityEngine::XR::WSA::Input::InteractionSourceNode node = UnityEngine::XR::WSA::Input::InteractionSourceNode::Grip);
				};
			}
		}
	}
}

namespace MyGame
{
	namespace MonoBehaviours
	{
		struct TestScript : virtual UnityEngine::MonoBehaviour
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
			void OnCollisionEnter(UnityEngine::Collision& param0);
			void Update();
		};
	}
}

namespace MyGame
{
	namespace MonoBehaviours
	{
		struct AnotherScript : virtual UnityEngine::MonoBehaviour
		{
			AnotherScript(decltype(nullptr) n);
			AnotherScript(Plugin::InternalUse iu, int32_t handle);
			AnotherScript(const AnotherScript& other);
			AnotherScript(AnotherScript&& other);
			virtual ~AnotherScript();
			AnotherScript& operator=(const AnotherScript& other);
			AnotherScript& operator=(decltype(nullptr) other);
			AnotherScript& operator=(AnotherScript&& other);
			bool operator==(const AnotherScript& other) const;
			bool operator!=(const AnotherScript& other) const;
			void Awake();
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
	template<> struct Array1<int32_t> : virtual System::Array, virtual System::ICloneable, virtual System::Collections::IList, virtual System::Collections::Generic::IList<int32_t>
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
	template<> struct Array1<float> : virtual System::Array, virtual System::ICloneable, virtual System::Collections::IList, virtual System::Collections::Generic::IList<float>
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
		Array1(int32_t length0);
		int32_t GetLength();
		int32_t GetRank();
		Plugin::ArrayElementProxy1_1<float> operator[](int32_t index);
	};
}

namespace System
{
	template<> struct Array2<float> : virtual System::Array, virtual System::ICloneable, virtual System::Collections::IList
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
	template<> struct Array3<float> : virtual System::Array, virtual System::ICloneable, virtual System::Collections::IList
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
	template<> struct Array1<System::String> : virtual System::Array, virtual System::ICloneable, virtual System::Collections::IList, virtual System::Collections::Generic::IList<System::String>
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
	template<> struct Array1<UnityEngine::Resolution> : virtual System::Array, virtual System::ICloneable, virtual System::Collections::IList, virtual System::Collections::Generic::IList<UnityEngine::Resolution>
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
	template<> struct Array1<UnityEngine::RaycastHit> : virtual System::Array, virtual System::ICloneable, virtual System::Collections::IList, virtual System::Collections::Generic::IList<UnityEngine::RaycastHit>
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
	template<> struct Array1<UnityEngine::GradientColorKey> : virtual System::Array, virtual System::ICloneable, virtual System::Collections::IList, virtual System::Collections::Generic::IList<UnityEngine::GradientColorKey>
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
		Array1(int32_t length0);
		int32_t GetLength();
		int32_t GetRank();
		Plugin::ArrayElementProxy1_1<UnityEngine::GradientColorKey> operator[](int32_t index);
	};
}

namespace System
{
	struct Action : virtual System::Object
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
		void operator+=(System::Action& del);
		void operator-=(System::Action& del);
		virtual void operator()();
		void Invoke();
	};
}

namespace System
{
	template<> struct Action1<float> : virtual System::Object
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
		void operator+=(System::Action1<float>& del);
		void operator-=(System::Action1<float>& del);
		virtual void operator()(float obj);
		void Invoke(float obj);
	};
}

namespace System
{
	template<> struct Action2<float, float> : virtual System::Object
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
		void operator+=(System::Action2<float, float>& del);
		void operator-=(System::Action2<float, float>& del);
		virtual void operator()(float arg1, float arg2);
		void Invoke(float arg1, float arg2);
	};
}

namespace System
{
	template<> struct Func3<int32_t, float, double> : virtual System::Object
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
		void operator+=(System::Func3<int32_t, float, double>& del);
		void operator-=(System::Func3<int32_t, float, double>& del);
		virtual double operator()(int32_t arg1, float arg2);
		double Invoke(int32_t arg1, float arg2);
	};
}

namespace System
{
	template<> struct Func3<int16_t, int32_t, System::String> : virtual System::Object
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
		void operator+=(System::Func3<int16_t, int32_t, System::String>& del);
		void operator-=(System::Func3<int16_t, int32_t, System::String>& del);
		virtual System::String operator()(int16_t arg1, int32_t arg2);
		System::String Invoke(int16_t arg1, int32_t arg2);
	};
}

namespace System
{
	struct AppDomainInitializer : virtual System::Object
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
		void operator+=(System::AppDomainInitializer& del);
		void operator-=(System::AppDomainInitializer& del);
		virtual void operator()(System::Array1<System::String>& args);
		void Invoke(System::Array1<System::String>& args);
	};
}

namespace UnityEngine
{
	namespace Events
	{
		struct UnityAction : virtual System::Object
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
			void operator+=(UnityEngine::Events::UnityAction& del);
			void operator-=(UnityEngine::Events::UnityAction& del);
			virtual void operator()();
			void Invoke();
		};
	}
}

namespace UnityEngine
{
	namespace Events
	{
		template<> struct UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode> : virtual System::Object
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
			void operator+=(UnityEngine::Events::UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>& del);
			void operator-=(UnityEngine::Events::UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>& del);
			virtual void operator()(UnityEngine::SceneManagement::Scene& arg0, UnityEngine::SceneManagement::LoadSceneMode arg1);
			void Invoke(UnityEngine::SceneManagement::Scene& arg0, UnityEngine::SceneManagement::LoadSceneMode arg1);
		};
	}
}

namespace System
{
	namespace ComponentModel
	{
		namespace Design
		{
			struct ComponentEventHandler : virtual System::Object
			{
				ComponentEventHandler(decltype(nullptr) n);
				ComponentEventHandler(Plugin::InternalUse iu, int32_t handle);
				ComponentEventHandler(const ComponentEventHandler& other);
				ComponentEventHandler(ComponentEventHandler&& other);
				virtual ~ComponentEventHandler();
				ComponentEventHandler& operator=(const ComponentEventHandler& other);
				ComponentEventHandler& operator=(decltype(nullptr) other);
				ComponentEventHandler& operator=(ComponentEventHandler&& other);
				bool operator==(const ComponentEventHandler& other) const;
				bool operator!=(const ComponentEventHandler& other) const;
				int32_t CppHandle;
				int32_t ClassHandle;
				ComponentEventHandler();
				void operator+=(System::ComponentModel::Design::ComponentEventHandler& del);
				void operator-=(System::ComponentModel::Design::ComponentEventHandler& del);
				virtual void operator()(System::Object& sender, System::ComponentModel::Design::ComponentEventArgs& e);
				void Invoke(System::Object& sender, System::ComponentModel::Design::ComponentEventArgs& e);
			};
		}
	}
}

namespace System
{
	namespace ComponentModel
	{
		namespace Design
		{
			struct ComponentChangingEventHandler : virtual System::Object
			{
				ComponentChangingEventHandler(decltype(nullptr) n);
				ComponentChangingEventHandler(Plugin::InternalUse iu, int32_t handle);
				ComponentChangingEventHandler(const ComponentChangingEventHandler& other);
				ComponentChangingEventHandler(ComponentChangingEventHandler&& other);
				virtual ~ComponentChangingEventHandler();
				ComponentChangingEventHandler& operator=(const ComponentChangingEventHandler& other);
				ComponentChangingEventHandler& operator=(decltype(nullptr) other);
				ComponentChangingEventHandler& operator=(ComponentChangingEventHandler&& other);
				bool operator==(const ComponentChangingEventHandler& other) const;
				bool operator!=(const ComponentChangingEventHandler& other) const;
				int32_t CppHandle;
				int32_t ClassHandle;
				ComponentChangingEventHandler();
				void operator+=(System::ComponentModel::Design::ComponentChangingEventHandler& del);
				void operator-=(System::ComponentModel::Design::ComponentChangingEventHandler& del);
				virtual void operator()(System::Object& sender, System::ComponentModel::Design::ComponentChangingEventArgs& e);
				void Invoke(System::Object& sender, System::ComponentModel::Design::ComponentChangingEventArgs& e);
			};
		}
	}
}

namespace System
{
	namespace ComponentModel
	{
		namespace Design
		{
			struct ComponentChangedEventHandler : virtual System::Object
			{
				ComponentChangedEventHandler(decltype(nullptr) n);
				ComponentChangedEventHandler(Plugin::InternalUse iu, int32_t handle);
				ComponentChangedEventHandler(const ComponentChangedEventHandler& other);
				ComponentChangedEventHandler(ComponentChangedEventHandler&& other);
				virtual ~ComponentChangedEventHandler();
				ComponentChangedEventHandler& operator=(const ComponentChangedEventHandler& other);
				ComponentChangedEventHandler& operator=(decltype(nullptr) other);
				ComponentChangedEventHandler& operator=(ComponentChangedEventHandler&& other);
				bool operator==(const ComponentChangedEventHandler& other) const;
				bool operator!=(const ComponentChangedEventHandler& other) const;
				int32_t CppHandle;
				int32_t ClassHandle;
				ComponentChangedEventHandler();
				void operator+=(System::ComponentModel::Design::ComponentChangedEventHandler& del);
				void operator-=(System::ComponentModel::Design::ComponentChangedEventHandler& del);
				virtual void operator()(System::Object& sender, System::ComponentModel::Design::ComponentChangedEventArgs& e);
				void Invoke(System::Object& sender, System::ComponentModel::Design::ComponentChangedEventArgs& e);
			};
		}
	}
}

namespace System
{
	namespace ComponentModel
	{
		namespace Design
		{
			struct ComponentRenameEventHandler : virtual System::Object
			{
				ComponentRenameEventHandler(decltype(nullptr) n);
				ComponentRenameEventHandler(Plugin::InternalUse iu, int32_t handle);
				ComponentRenameEventHandler(const ComponentRenameEventHandler& other);
				ComponentRenameEventHandler(ComponentRenameEventHandler&& other);
				virtual ~ComponentRenameEventHandler();
				ComponentRenameEventHandler& operator=(const ComponentRenameEventHandler& other);
				ComponentRenameEventHandler& operator=(decltype(nullptr) other);
				ComponentRenameEventHandler& operator=(ComponentRenameEventHandler&& other);
				bool operator==(const ComponentRenameEventHandler& other) const;
				bool operator!=(const ComponentRenameEventHandler& other) const;
				int32_t CppHandle;
				int32_t ClassHandle;
				ComponentRenameEventHandler();
				void operator+=(System::ComponentModel::Design::ComponentRenameEventHandler& del);
				void operator-=(System::ComponentModel::Design::ComponentRenameEventHandler& del);
				virtual void operator()(System::Object& sender, System::ComponentModel::Design::ComponentRenameEventArgs& e);
				void Invoke(System::Object& sender, System::ComponentModel::Design::ComponentRenameEventArgs& e);
			};
		}
	}
}
/*END TYPE DEFINITIONS*/

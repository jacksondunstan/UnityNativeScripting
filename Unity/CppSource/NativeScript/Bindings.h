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
		
		Boolean(int32_t value)
			: Value(value)
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
			template<typename TT0> struct IEnumerator;
		}
	}
}

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
	namespace Collections
	{
		struct IEnumerator;
	}
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
			template<> struct IEnumerator<System::String>;
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			template<> struct IEnumerator<int32_t>;
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			template<> struct IEnumerator<float>;
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			template<> struct IEnumerator<UnityEngine::RaycastHit>;
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			template<> struct IEnumerator<UnityEngine::GradientColorKey>;
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			template<> struct IEnumerator<UnityEngine::Resolution>;
		}
	}
}

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
		Object(decltype(nullptr));
		virtual ~Object() = default;
		bool operator==(decltype(nullptr)) const;
		bool operator!=(decltype(nullptr)) const;
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
		ValueType(decltype(nullptr));
	};
	
	struct String : virtual Object
	{
		String(Plugin::InternalUse iu, int32_t handle);
		String(decltype(nullptr));
		String(const String& other);
		String(String&& other);
		virtual ~String();
		String& operator=(const String& other);
		String& operator=(decltype(nullptr));
		String& operator=(String&& other);
		String(const char* chars);
	};
	
	struct ICloneable : virtual Object
	{
		ICloneable(Plugin::InternalUse iu, int32_t handle);
		ICloneable(decltype(nullptr));
	};
	
	namespace Collections
	{
		struct IEnumerable : virtual Object
		{
			IEnumerable(Plugin::InternalUse iu, int32_t handle);
			IEnumerable(decltype(nullptr));
			IEnumerator GetEnumerator();
		};
		
		struct ICollection : virtual IEnumerable
		{
			ICollection(Plugin::InternalUse iu, int32_t handle);
			ICollection(decltype(nullptr));
		};
		
		struct IList : virtual ICollection, virtual IEnumerable
		{
			IList(Plugin::InternalUse iu, int32_t handle);
			IList(decltype(nullptr));
		};
	}
	
	struct Array : virtual ICloneable, virtual Collections::IList
	{
		Array(Plugin::InternalUse iu, int32_t handle);
		Array(decltype(nullptr));
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
		IDisposable(decltype(nullptr));
		IDisposable(Plugin::InternalUse iu, int32_t handle);
		IDisposable(const IDisposable& other);
		IDisposable(IDisposable&& other);
		virtual ~IDisposable();
		IDisposable& operator=(const IDisposable& other);
		IDisposable& operator=(decltype(nullptr));
		IDisposable& operator=(IDisposable&& other);
		bool operator==(const IDisposable& other) const;
		bool operator!=(const IDisposable& other) const;
		void Dispose();
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
		Object(decltype(nullptr));
		Object(Plugin::InternalUse iu, int32_t handle);
		Object(const Object& other);
		Object(Object&& other);
		virtual ~Object();
		Object& operator=(const Object& other);
		Object& operator=(decltype(nullptr));
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
		Component(decltype(nullptr));
		Component(Plugin::InternalUse iu, int32_t handle);
		Component(const Component& other);
		Component(Component&& other);
		virtual ~Component();
		Component& operator=(const Component& other);
		Component& operator=(decltype(nullptr));
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
		Transform(decltype(nullptr));
		Transform(Plugin::InternalUse iu, int32_t handle);
		Transform(const Transform& other);
		Transform(Transform&& other);
		virtual ~Transform();
		Transform& operator=(const Transform& other);
		Transform& operator=(decltype(nullptr));
		Transform& operator=(Transform&& other);
		bool operator==(const Transform& other) const;
		bool operator!=(const Transform& other) const;
		UnityEngine::Vector3 GetPosition();
		void SetPosition(UnityEngine::Vector3& value);
		void SetParent(UnityEngine::Transform& parent);
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
		Resolution(decltype(nullptr));
		Resolution(Plugin::InternalUse iu, int32_t handle);
		Resolution(const Resolution& other);
		Resolution(Resolution&& other);
		virtual ~Resolution();
		Resolution& operator=(const Resolution& other);
		Resolution& operator=(decltype(nullptr));
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
		RaycastHit(decltype(nullptr));
		RaycastHit(Plugin::InternalUse iu, int32_t handle);
		RaycastHit(const RaycastHit& other);
		RaycastHit(RaycastHit&& other);
		virtual ~RaycastHit();
		RaycastHit& operator=(const RaycastHit& other);
		RaycastHit& operator=(decltype(nullptr));
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
		struct IEnumerator : virtual System::Object
		{
			IEnumerator(decltype(nullptr));
			IEnumerator(Plugin::InternalUse iu, int32_t handle);
			IEnumerator(const IEnumerator& other);
			IEnumerator(IEnumerator&& other);
			virtual ~IEnumerator();
			IEnumerator& operator=(const IEnumerator& other);
			IEnumerator& operator=(decltype(nullptr));
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
	namespace Collections
	{
		namespace Generic
		{
			template<> struct IEnumerator<System::String> : virtual System::IDisposable, virtual System::Collections::IEnumerator
			{
				IEnumerator<System::String>(decltype(nullptr));
				IEnumerator<System::String>(Plugin::InternalUse iu, int32_t handle);
				IEnumerator<System::String>(const IEnumerator<System::String>& other);
				IEnumerator<System::String>(IEnumerator<System::String>&& other);
				virtual ~IEnumerator<System::String>();
				IEnumerator<System::String>& operator=(const IEnumerator<System::String>& other);
				IEnumerator<System::String>& operator=(decltype(nullptr));
				IEnumerator<System::String>& operator=(IEnumerator<System::String>&& other);
				bool operator==(const IEnumerator<System::String>& other) const;
				bool operator!=(const IEnumerator<System::String>& other) const;
				System::String GetCurrent();
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
			template<> struct IEnumerator<int32_t> : virtual System::IDisposable, virtual System::Collections::IEnumerator
			{
				IEnumerator<int32_t>(decltype(nullptr));
				IEnumerator<int32_t>(Plugin::InternalUse iu, int32_t handle);
				IEnumerator<int32_t>(const IEnumerator<int32_t>& other);
				IEnumerator<int32_t>(IEnumerator<int32_t>&& other);
				virtual ~IEnumerator<int32_t>();
				IEnumerator<int32_t>& operator=(const IEnumerator<int32_t>& other);
				IEnumerator<int32_t>& operator=(decltype(nullptr));
				IEnumerator<int32_t>& operator=(IEnumerator<int32_t>&& other);
				bool operator==(const IEnumerator<int32_t>& other) const;
				bool operator!=(const IEnumerator<int32_t>& other) const;
				int32_t GetCurrent();
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
			template<> struct IEnumerator<float> : virtual System::IDisposable, virtual System::Collections::IEnumerator
			{
				IEnumerator<float>(decltype(nullptr));
				IEnumerator<float>(Plugin::InternalUse iu, int32_t handle);
				IEnumerator<float>(const IEnumerator<float>& other);
				IEnumerator<float>(IEnumerator<float>&& other);
				virtual ~IEnumerator<float>();
				IEnumerator<float>& operator=(const IEnumerator<float>& other);
				IEnumerator<float>& operator=(decltype(nullptr));
				IEnumerator<float>& operator=(IEnumerator<float>&& other);
				bool operator==(const IEnumerator<float>& other) const;
				bool operator!=(const IEnumerator<float>& other) const;
				float GetCurrent();
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
			template<> struct IEnumerator<UnityEngine::RaycastHit> : virtual System::IDisposable, virtual System::Collections::IEnumerator
			{
				IEnumerator<UnityEngine::RaycastHit>(decltype(nullptr));
				IEnumerator<UnityEngine::RaycastHit>(Plugin::InternalUse iu, int32_t handle);
				IEnumerator<UnityEngine::RaycastHit>(const IEnumerator<UnityEngine::RaycastHit>& other);
				IEnumerator<UnityEngine::RaycastHit>(IEnumerator<UnityEngine::RaycastHit>&& other);
				virtual ~IEnumerator<UnityEngine::RaycastHit>();
				IEnumerator<UnityEngine::RaycastHit>& operator=(const IEnumerator<UnityEngine::RaycastHit>& other);
				IEnumerator<UnityEngine::RaycastHit>& operator=(decltype(nullptr));
				IEnumerator<UnityEngine::RaycastHit>& operator=(IEnumerator<UnityEngine::RaycastHit>&& other);
				bool operator==(const IEnumerator<UnityEngine::RaycastHit>& other) const;
				bool operator!=(const IEnumerator<UnityEngine::RaycastHit>& other) const;
				UnityEngine::RaycastHit GetCurrent();
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
			template<> struct IEnumerator<UnityEngine::GradientColorKey> : virtual System::IDisposable, virtual System::Collections::IEnumerator
			{
				IEnumerator<UnityEngine::GradientColorKey>(decltype(nullptr));
				IEnumerator<UnityEngine::GradientColorKey>(Plugin::InternalUse iu, int32_t handle);
				IEnumerator<UnityEngine::GradientColorKey>(const IEnumerator<UnityEngine::GradientColorKey>& other);
				IEnumerator<UnityEngine::GradientColorKey>(IEnumerator<UnityEngine::GradientColorKey>&& other);
				virtual ~IEnumerator<UnityEngine::GradientColorKey>();
				IEnumerator<UnityEngine::GradientColorKey>& operator=(const IEnumerator<UnityEngine::GradientColorKey>& other);
				IEnumerator<UnityEngine::GradientColorKey>& operator=(decltype(nullptr));
				IEnumerator<UnityEngine::GradientColorKey>& operator=(IEnumerator<UnityEngine::GradientColorKey>&& other);
				bool operator==(const IEnumerator<UnityEngine::GradientColorKey>& other) const;
				bool operator!=(const IEnumerator<UnityEngine::GradientColorKey>& other) const;
				UnityEngine::GradientColorKey GetCurrent();
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
			template<> struct IEnumerator<UnityEngine::Resolution> : virtual System::IDisposable, virtual System::Collections::IEnumerator
			{
				IEnumerator<UnityEngine::Resolution>(decltype(nullptr));
				IEnumerator<UnityEngine::Resolution>(Plugin::InternalUse iu, int32_t handle);
				IEnumerator<UnityEngine::Resolution>(const IEnumerator<UnityEngine::Resolution>& other);
				IEnumerator<UnityEngine::Resolution>(IEnumerator<UnityEngine::Resolution>&& other);
				virtual ~IEnumerator<UnityEngine::Resolution>();
				IEnumerator<UnityEngine::Resolution>& operator=(const IEnumerator<UnityEngine::Resolution>& other);
				IEnumerator<UnityEngine::Resolution>& operator=(decltype(nullptr));
				IEnumerator<UnityEngine::Resolution>& operator=(IEnumerator<UnityEngine::Resolution>&& other);
				bool operator==(const IEnumerator<UnityEngine::Resolution>& other) const;
				bool operator!=(const IEnumerator<UnityEngine::Resolution>& other) const;
				UnityEngine::Resolution GetCurrent();
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
			template<> struct IEnumerable<System::String> : virtual System::Collections::IEnumerable
			{
				IEnumerable<System::String>(decltype(nullptr));
				IEnumerable<System::String>(Plugin::InternalUse iu, int32_t handle);
				IEnumerable<System::String>(const IEnumerable<System::String>& other);
				IEnumerable<System::String>(IEnumerable<System::String>&& other);
				virtual ~IEnumerable<System::String>();
				IEnumerable<System::String>& operator=(const IEnumerable<System::String>& other);
				IEnumerable<System::String>& operator=(decltype(nullptr));
				IEnumerable<System::String>& operator=(IEnumerable<System::String>&& other);
				bool operator==(const IEnumerable<System::String>& other) const;
				bool operator!=(const IEnumerable<System::String>& other) const;
				System::Collections::Generic::IEnumerator<System::String> GetEnumerator();
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
				IEnumerable<int32_t>(decltype(nullptr));
				IEnumerable<int32_t>(Plugin::InternalUse iu, int32_t handle);
				IEnumerable<int32_t>(const IEnumerable<int32_t>& other);
				IEnumerable<int32_t>(IEnumerable<int32_t>&& other);
				virtual ~IEnumerable<int32_t>();
				IEnumerable<int32_t>& operator=(const IEnumerable<int32_t>& other);
				IEnumerable<int32_t>& operator=(decltype(nullptr));
				IEnumerable<int32_t>& operator=(IEnumerable<int32_t>&& other);
				bool operator==(const IEnumerable<int32_t>& other) const;
				bool operator!=(const IEnumerable<int32_t>& other) const;
				System::Collections::Generic::IEnumerator<int32_t> GetEnumerator();
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
				IEnumerable<float>(decltype(nullptr));
				IEnumerable<float>(Plugin::InternalUse iu, int32_t handle);
				IEnumerable<float>(const IEnumerable<float>& other);
				IEnumerable<float>(IEnumerable<float>&& other);
				virtual ~IEnumerable<float>();
				IEnumerable<float>& operator=(const IEnumerable<float>& other);
				IEnumerable<float>& operator=(decltype(nullptr));
				IEnumerable<float>& operator=(IEnumerable<float>&& other);
				bool operator==(const IEnumerable<float>& other) const;
				bool operator!=(const IEnumerable<float>& other) const;
				System::Collections::Generic::IEnumerator<float> GetEnumerator();
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
				IEnumerable<UnityEngine::RaycastHit>(decltype(nullptr));
				IEnumerable<UnityEngine::RaycastHit>(Plugin::InternalUse iu, int32_t handle);
				IEnumerable<UnityEngine::RaycastHit>(const IEnumerable<UnityEngine::RaycastHit>& other);
				IEnumerable<UnityEngine::RaycastHit>(IEnumerable<UnityEngine::RaycastHit>&& other);
				virtual ~IEnumerable<UnityEngine::RaycastHit>();
				IEnumerable<UnityEngine::RaycastHit>& operator=(const IEnumerable<UnityEngine::RaycastHit>& other);
				IEnumerable<UnityEngine::RaycastHit>& operator=(decltype(nullptr));
				IEnumerable<UnityEngine::RaycastHit>& operator=(IEnumerable<UnityEngine::RaycastHit>&& other);
				bool operator==(const IEnumerable<UnityEngine::RaycastHit>& other) const;
				bool operator!=(const IEnumerable<UnityEngine::RaycastHit>& other) const;
				System::Collections::Generic::IEnumerator<UnityEngine::RaycastHit> GetEnumerator();
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
				IEnumerable<UnityEngine::GradientColorKey>(decltype(nullptr));
				IEnumerable<UnityEngine::GradientColorKey>(Plugin::InternalUse iu, int32_t handle);
				IEnumerable<UnityEngine::GradientColorKey>(const IEnumerable<UnityEngine::GradientColorKey>& other);
				IEnumerable<UnityEngine::GradientColorKey>(IEnumerable<UnityEngine::GradientColorKey>&& other);
				virtual ~IEnumerable<UnityEngine::GradientColorKey>();
				IEnumerable<UnityEngine::GradientColorKey>& operator=(const IEnumerable<UnityEngine::GradientColorKey>& other);
				IEnumerable<UnityEngine::GradientColorKey>& operator=(decltype(nullptr));
				IEnumerable<UnityEngine::GradientColorKey>& operator=(IEnumerable<UnityEngine::GradientColorKey>&& other);
				bool operator==(const IEnumerable<UnityEngine::GradientColorKey>& other) const;
				bool operator!=(const IEnumerable<UnityEngine::GradientColorKey>& other) const;
				System::Collections::Generic::IEnumerator<UnityEngine::GradientColorKey> GetEnumerator();
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
				IEnumerable<UnityEngine::Resolution>(decltype(nullptr));
				IEnumerable<UnityEngine::Resolution>(Plugin::InternalUse iu, int32_t handle);
				IEnumerable<UnityEngine::Resolution>(const IEnumerable<UnityEngine::Resolution>& other);
				IEnumerable<UnityEngine::Resolution>(IEnumerable<UnityEngine::Resolution>&& other);
				virtual ~IEnumerable<UnityEngine::Resolution>();
				IEnumerable<UnityEngine::Resolution>& operator=(const IEnumerable<UnityEngine::Resolution>& other);
				IEnumerable<UnityEngine::Resolution>& operator=(decltype(nullptr));
				IEnumerable<UnityEngine::Resolution>& operator=(IEnumerable<UnityEngine::Resolution>&& other);
				bool operator==(const IEnumerable<UnityEngine::Resolution>& other) const;
				bool operator!=(const IEnumerable<UnityEngine::Resolution>& other) const;
				System::Collections::Generic::IEnumerator<UnityEngine::Resolution> GetEnumerator();
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
				ICollection<System::String>(decltype(nullptr));
				ICollection<System::String>(Plugin::InternalUse iu, int32_t handle);
				ICollection<System::String>(const ICollection<System::String>& other);
				ICollection<System::String>(ICollection<System::String>&& other);
				virtual ~ICollection<System::String>();
				ICollection<System::String>& operator=(const ICollection<System::String>& other);
				ICollection<System::String>& operator=(decltype(nullptr));
				ICollection<System::String>& operator=(ICollection<System::String>&& other);
				bool operator==(const ICollection<System::String>& other) const;
				bool operator!=(const ICollection<System::String>& other) const;
			};
		}
	}
}

namespace Plugin
{
	struct SystemCollectionsGenericICollectionSystemStringIterator
	{
		System::Collections::Generic::IEnumerator<System::String> enumerator;
		bool hasMore;
		SystemCollectionsGenericICollectionSystemStringIterator(decltype(nullptr));
		SystemCollectionsGenericICollectionSystemStringIterator(System::Collections::Generic::ICollection<System::String>& enumerable);
		~SystemCollectionsGenericICollectionSystemStringIterator();
		SystemCollectionsGenericICollectionSystemStringIterator& operator++();
		bool operator!=(const SystemCollectionsGenericICollectionSystemStringIterator& other);
		System::String operator*();
	};
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			Plugin::SystemCollectionsGenericICollectionSystemStringIterator begin(System::Collections::Generic::ICollection<System::String>& enumerable);
			Plugin::SystemCollectionsGenericICollectionSystemStringIterator end(System::Collections::Generic::ICollection<System::String>& enumerable);
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
				ICollection<int32_t>(decltype(nullptr));
				ICollection<int32_t>(Plugin::InternalUse iu, int32_t handle);
				ICollection<int32_t>(const ICollection<int32_t>& other);
				ICollection<int32_t>(ICollection<int32_t>&& other);
				virtual ~ICollection<int32_t>();
				ICollection<int32_t>& operator=(const ICollection<int32_t>& other);
				ICollection<int32_t>& operator=(decltype(nullptr));
				ICollection<int32_t>& operator=(ICollection<int32_t>&& other);
				bool operator==(const ICollection<int32_t>& other) const;
				bool operator!=(const ICollection<int32_t>& other) const;
			};
		}
	}
}

namespace Plugin
{
	struct SystemCollectionsGenericICollectionSystemInt32Iterator
	{
		System::Collections::Generic::IEnumerator<int32_t> enumerator;
		bool hasMore;
		SystemCollectionsGenericICollectionSystemInt32Iterator(decltype(nullptr));
		SystemCollectionsGenericICollectionSystemInt32Iterator(System::Collections::Generic::ICollection<int32_t>& enumerable);
		~SystemCollectionsGenericICollectionSystemInt32Iterator();
		SystemCollectionsGenericICollectionSystemInt32Iterator& operator++();
		bool operator!=(const SystemCollectionsGenericICollectionSystemInt32Iterator& other);
		int32_t operator*();
	};
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			Plugin::SystemCollectionsGenericICollectionSystemInt32Iterator begin(System::Collections::Generic::ICollection<int32_t>& enumerable);
			Plugin::SystemCollectionsGenericICollectionSystemInt32Iterator end(System::Collections::Generic::ICollection<int32_t>& enumerable);
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
				ICollection<float>(decltype(nullptr));
				ICollection<float>(Plugin::InternalUse iu, int32_t handle);
				ICollection<float>(const ICollection<float>& other);
				ICollection<float>(ICollection<float>&& other);
				virtual ~ICollection<float>();
				ICollection<float>& operator=(const ICollection<float>& other);
				ICollection<float>& operator=(decltype(nullptr));
				ICollection<float>& operator=(ICollection<float>&& other);
				bool operator==(const ICollection<float>& other) const;
				bool operator!=(const ICollection<float>& other) const;
			};
		}
	}
}

namespace Plugin
{
	struct SystemCollectionsGenericICollectionSystemSingleIterator
	{
		System::Collections::Generic::IEnumerator<float> enumerator;
		bool hasMore;
		SystemCollectionsGenericICollectionSystemSingleIterator(decltype(nullptr));
		SystemCollectionsGenericICollectionSystemSingleIterator(System::Collections::Generic::ICollection<float>& enumerable);
		~SystemCollectionsGenericICollectionSystemSingleIterator();
		SystemCollectionsGenericICollectionSystemSingleIterator& operator++();
		bool operator!=(const SystemCollectionsGenericICollectionSystemSingleIterator& other);
		float operator*();
	};
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			Plugin::SystemCollectionsGenericICollectionSystemSingleIterator begin(System::Collections::Generic::ICollection<float>& enumerable);
			Plugin::SystemCollectionsGenericICollectionSystemSingleIterator end(System::Collections::Generic::ICollection<float>& enumerable);
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
				ICollection<UnityEngine::RaycastHit>(decltype(nullptr));
				ICollection<UnityEngine::RaycastHit>(Plugin::InternalUse iu, int32_t handle);
				ICollection<UnityEngine::RaycastHit>(const ICollection<UnityEngine::RaycastHit>& other);
				ICollection<UnityEngine::RaycastHit>(ICollection<UnityEngine::RaycastHit>&& other);
				virtual ~ICollection<UnityEngine::RaycastHit>();
				ICollection<UnityEngine::RaycastHit>& operator=(const ICollection<UnityEngine::RaycastHit>& other);
				ICollection<UnityEngine::RaycastHit>& operator=(decltype(nullptr));
				ICollection<UnityEngine::RaycastHit>& operator=(ICollection<UnityEngine::RaycastHit>&& other);
				bool operator==(const ICollection<UnityEngine::RaycastHit>& other) const;
				bool operator!=(const ICollection<UnityEngine::RaycastHit>& other) const;
			};
		}
	}
}

namespace Plugin
{
	struct SystemCollectionsGenericICollectionUnityEngineRaycastHitIterator
	{
		System::Collections::Generic::IEnumerator<UnityEngine::RaycastHit> enumerator;
		bool hasMore;
		SystemCollectionsGenericICollectionUnityEngineRaycastHitIterator(decltype(nullptr));
		SystemCollectionsGenericICollectionUnityEngineRaycastHitIterator(System::Collections::Generic::ICollection<UnityEngine::RaycastHit>& enumerable);
		~SystemCollectionsGenericICollectionUnityEngineRaycastHitIterator();
		SystemCollectionsGenericICollectionUnityEngineRaycastHitIterator& operator++();
		bool operator!=(const SystemCollectionsGenericICollectionUnityEngineRaycastHitIterator& other);
		UnityEngine::RaycastHit operator*();
	};
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			Plugin::SystemCollectionsGenericICollectionUnityEngineRaycastHitIterator begin(System::Collections::Generic::ICollection<UnityEngine::RaycastHit>& enumerable);
			Plugin::SystemCollectionsGenericICollectionUnityEngineRaycastHitIterator end(System::Collections::Generic::ICollection<UnityEngine::RaycastHit>& enumerable);
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
				ICollection<UnityEngine::GradientColorKey>(decltype(nullptr));
				ICollection<UnityEngine::GradientColorKey>(Plugin::InternalUse iu, int32_t handle);
				ICollection<UnityEngine::GradientColorKey>(const ICollection<UnityEngine::GradientColorKey>& other);
				ICollection<UnityEngine::GradientColorKey>(ICollection<UnityEngine::GradientColorKey>&& other);
				virtual ~ICollection<UnityEngine::GradientColorKey>();
				ICollection<UnityEngine::GradientColorKey>& operator=(const ICollection<UnityEngine::GradientColorKey>& other);
				ICollection<UnityEngine::GradientColorKey>& operator=(decltype(nullptr));
				ICollection<UnityEngine::GradientColorKey>& operator=(ICollection<UnityEngine::GradientColorKey>&& other);
				bool operator==(const ICollection<UnityEngine::GradientColorKey>& other) const;
				bool operator!=(const ICollection<UnityEngine::GradientColorKey>& other) const;
			};
		}
	}
}

namespace Plugin
{
	struct SystemCollectionsGenericICollectionUnityEngineGradientColorKeyIterator
	{
		System::Collections::Generic::IEnumerator<UnityEngine::GradientColorKey> enumerator;
		bool hasMore;
		SystemCollectionsGenericICollectionUnityEngineGradientColorKeyIterator(decltype(nullptr));
		SystemCollectionsGenericICollectionUnityEngineGradientColorKeyIterator(System::Collections::Generic::ICollection<UnityEngine::GradientColorKey>& enumerable);
		~SystemCollectionsGenericICollectionUnityEngineGradientColorKeyIterator();
		SystemCollectionsGenericICollectionUnityEngineGradientColorKeyIterator& operator++();
		bool operator!=(const SystemCollectionsGenericICollectionUnityEngineGradientColorKeyIterator& other);
		UnityEngine::GradientColorKey operator*();
	};
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			Plugin::SystemCollectionsGenericICollectionUnityEngineGradientColorKeyIterator begin(System::Collections::Generic::ICollection<UnityEngine::GradientColorKey>& enumerable);
			Plugin::SystemCollectionsGenericICollectionUnityEngineGradientColorKeyIterator end(System::Collections::Generic::ICollection<UnityEngine::GradientColorKey>& enumerable);
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
				ICollection<UnityEngine::Resolution>(decltype(nullptr));
				ICollection<UnityEngine::Resolution>(Plugin::InternalUse iu, int32_t handle);
				ICollection<UnityEngine::Resolution>(const ICollection<UnityEngine::Resolution>& other);
				ICollection<UnityEngine::Resolution>(ICollection<UnityEngine::Resolution>&& other);
				virtual ~ICollection<UnityEngine::Resolution>();
				ICollection<UnityEngine::Resolution>& operator=(const ICollection<UnityEngine::Resolution>& other);
				ICollection<UnityEngine::Resolution>& operator=(decltype(nullptr));
				ICollection<UnityEngine::Resolution>& operator=(ICollection<UnityEngine::Resolution>&& other);
				bool operator==(const ICollection<UnityEngine::Resolution>& other) const;
				bool operator!=(const ICollection<UnityEngine::Resolution>& other) const;
			};
		}
	}
}

namespace Plugin
{
	struct SystemCollectionsGenericICollectionUnityEngineResolutionIterator
	{
		System::Collections::Generic::IEnumerator<UnityEngine::Resolution> enumerator;
		bool hasMore;
		SystemCollectionsGenericICollectionUnityEngineResolutionIterator(decltype(nullptr));
		SystemCollectionsGenericICollectionUnityEngineResolutionIterator(System::Collections::Generic::ICollection<UnityEngine::Resolution>& enumerable);
		~SystemCollectionsGenericICollectionUnityEngineResolutionIterator();
		SystemCollectionsGenericICollectionUnityEngineResolutionIterator& operator++();
		bool operator!=(const SystemCollectionsGenericICollectionUnityEngineResolutionIterator& other);
		UnityEngine::Resolution operator*();
	};
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			Plugin::SystemCollectionsGenericICollectionUnityEngineResolutionIterator begin(System::Collections::Generic::ICollection<UnityEngine::Resolution>& enumerable);
			Plugin::SystemCollectionsGenericICollectionUnityEngineResolutionIterator end(System::Collections::Generic::ICollection<UnityEngine::Resolution>& enumerable);
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
				IList<System::String>(decltype(nullptr));
				IList<System::String>(Plugin::InternalUse iu, int32_t handle);
				IList<System::String>(const IList<System::String>& other);
				IList<System::String>(IList<System::String>&& other);
				virtual ~IList<System::String>();
				IList<System::String>& operator=(const IList<System::String>& other);
				IList<System::String>& operator=(decltype(nullptr));
				IList<System::String>& operator=(IList<System::String>&& other);
				bool operator==(const IList<System::String>& other) const;
				bool operator!=(const IList<System::String>& other) const;
			};
		}
	}
}

namespace Plugin
{
	struct SystemCollectionsGenericIListSystemStringIterator
	{
		System::Collections::Generic::IEnumerator<System::String> enumerator;
		bool hasMore;
		SystemCollectionsGenericIListSystemStringIterator(decltype(nullptr));
		SystemCollectionsGenericIListSystemStringIterator(System::Collections::Generic::IList<System::String>& enumerable);
		~SystemCollectionsGenericIListSystemStringIterator();
		SystemCollectionsGenericIListSystemStringIterator& operator++();
		bool operator!=(const SystemCollectionsGenericIListSystemStringIterator& other);
		System::String operator*();
	};
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			Plugin::SystemCollectionsGenericIListSystemStringIterator begin(System::Collections::Generic::IList<System::String>& enumerable);
			Plugin::SystemCollectionsGenericIListSystemStringIterator end(System::Collections::Generic::IList<System::String>& enumerable);
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
				IList<int32_t>(decltype(nullptr));
				IList<int32_t>(Plugin::InternalUse iu, int32_t handle);
				IList<int32_t>(const IList<int32_t>& other);
				IList<int32_t>(IList<int32_t>&& other);
				virtual ~IList<int32_t>();
				IList<int32_t>& operator=(const IList<int32_t>& other);
				IList<int32_t>& operator=(decltype(nullptr));
				IList<int32_t>& operator=(IList<int32_t>&& other);
				bool operator==(const IList<int32_t>& other) const;
				bool operator!=(const IList<int32_t>& other) const;
			};
		}
	}
}

namespace Plugin
{
	struct SystemCollectionsGenericIListSystemInt32Iterator
	{
		System::Collections::Generic::IEnumerator<int32_t> enumerator;
		bool hasMore;
		SystemCollectionsGenericIListSystemInt32Iterator(decltype(nullptr));
		SystemCollectionsGenericIListSystemInt32Iterator(System::Collections::Generic::IList<int32_t>& enumerable);
		~SystemCollectionsGenericIListSystemInt32Iterator();
		SystemCollectionsGenericIListSystemInt32Iterator& operator++();
		bool operator!=(const SystemCollectionsGenericIListSystemInt32Iterator& other);
		int32_t operator*();
	};
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			Plugin::SystemCollectionsGenericIListSystemInt32Iterator begin(System::Collections::Generic::IList<int32_t>& enumerable);
			Plugin::SystemCollectionsGenericIListSystemInt32Iterator end(System::Collections::Generic::IList<int32_t>& enumerable);
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
				IList<float>(decltype(nullptr));
				IList<float>(Plugin::InternalUse iu, int32_t handle);
				IList<float>(const IList<float>& other);
				IList<float>(IList<float>&& other);
				virtual ~IList<float>();
				IList<float>& operator=(const IList<float>& other);
				IList<float>& operator=(decltype(nullptr));
				IList<float>& operator=(IList<float>&& other);
				bool operator==(const IList<float>& other) const;
				bool operator!=(const IList<float>& other) const;
			};
		}
	}
}

namespace Plugin
{
	struct SystemCollectionsGenericIListSystemSingleIterator
	{
		System::Collections::Generic::IEnumerator<float> enumerator;
		bool hasMore;
		SystemCollectionsGenericIListSystemSingleIterator(decltype(nullptr));
		SystemCollectionsGenericIListSystemSingleIterator(System::Collections::Generic::IList<float>& enumerable);
		~SystemCollectionsGenericIListSystemSingleIterator();
		SystemCollectionsGenericIListSystemSingleIterator& operator++();
		bool operator!=(const SystemCollectionsGenericIListSystemSingleIterator& other);
		float operator*();
	};
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			Plugin::SystemCollectionsGenericIListSystemSingleIterator begin(System::Collections::Generic::IList<float>& enumerable);
			Plugin::SystemCollectionsGenericIListSystemSingleIterator end(System::Collections::Generic::IList<float>& enumerable);
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
				IList<UnityEngine::RaycastHit>(decltype(nullptr));
				IList<UnityEngine::RaycastHit>(Plugin::InternalUse iu, int32_t handle);
				IList<UnityEngine::RaycastHit>(const IList<UnityEngine::RaycastHit>& other);
				IList<UnityEngine::RaycastHit>(IList<UnityEngine::RaycastHit>&& other);
				virtual ~IList<UnityEngine::RaycastHit>();
				IList<UnityEngine::RaycastHit>& operator=(const IList<UnityEngine::RaycastHit>& other);
				IList<UnityEngine::RaycastHit>& operator=(decltype(nullptr));
				IList<UnityEngine::RaycastHit>& operator=(IList<UnityEngine::RaycastHit>&& other);
				bool operator==(const IList<UnityEngine::RaycastHit>& other) const;
				bool operator!=(const IList<UnityEngine::RaycastHit>& other) const;
			};
		}
	}
}

namespace Plugin
{
	struct SystemCollectionsGenericIListUnityEngineRaycastHitIterator
	{
		System::Collections::Generic::IEnumerator<UnityEngine::RaycastHit> enumerator;
		bool hasMore;
		SystemCollectionsGenericIListUnityEngineRaycastHitIterator(decltype(nullptr));
		SystemCollectionsGenericIListUnityEngineRaycastHitIterator(System::Collections::Generic::IList<UnityEngine::RaycastHit>& enumerable);
		~SystemCollectionsGenericIListUnityEngineRaycastHitIterator();
		SystemCollectionsGenericIListUnityEngineRaycastHitIterator& operator++();
		bool operator!=(const SystemCollectionsGenericIListUnityEngineRaycastHitIterator& other);
		UnityEngine::RaycastHit operator*();
	};
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			Plugin::SystemCollectionsGenericIListUnityEngineRaycastHitIterator begin(System::Collections::Generic::IList<UnityEngine::RaycastHit>& enumerable);
			Plugin::SystemCollectionsGenericIListUnityEngineRaycastHitIterator end(System::Collections::Generic::IList<UnityEngine::RaycastHit>& enumerable);
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
				IList<UnityEngine::GradientColorKey>(decltype(nullptr));
				IList<UnityEngine::GradientColorKey>(Plugin::InternalUse iu, int32_t handle);
				IList<UnityEngine::GradientColorKey>(const IList<UnityEngine::GradientColorKey>& other);
				IList<UnityEngine::GradientColorKey>(IList<UnityEngine::GradientColorKey>&& other);
				virtual ~IList<UnityEngine::GradientColorKey>();
				IList<UnityEngine::GradientColorKey>& operator=(const IList<UnityEngine::GradientColorKey>& other);
				IList<UnityEngine::GradientColorKey>& operator=(decltype(nullptr));
				IList<UnityEngine::GradientColorKey>& operator=(IList<UnityEngine::GradientColorKey>&& other);
				bool operator==(const IList<UnityEngine::GradientColorKey>& other) const;
				bool operator!=(const IList<UnityEngine::GradientColorKey>& other) const;
			};
		}
	}
}

namespace Plugin
{
	struct SystemCollectionsGenericIListUnityEngineGradientColorKeyIterator
	{
		System::Collections::Generic::IEnumerator<UnityEngine::GradientColorKey> enumerator;
		bool hasMore;
		SystemCollectionsGenericIListUnityEngineGradientColorKeyIterator(decltype(nullptr));
		SystemCollectionsGenericIListUnityEngineGradientColorKeyIterator(System::Collections::Generic::IList<UnityEngine::GradientColorKey>& enumerable);
		~SystemCollectionsGenericIListUnityEngineGradientColorKeyIterator();
		SystemCollectionsGenericIListUnityEngineGradientColorKeyIterator& operator++();
		bool operator!=(const SystemCollectionsGenericIListUnityEngineGradientColorKeyIterator& other);
		UnityEngine::GradientColorKey operator*();
	};
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			Plugin::SystemCollectionsGenericIListUnityEngineGradientColorKeyIterator begin(System::Collections::Generic::IList<UnityEngine::GradientColorKey>& enumerable);
			Plugin::SystemCollectionsGenericIListUnityEngineGradientColorKeyIterator end(System::Collections::Generic::IList<UnityEngine::GradientColorKey>& enumerable);
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
				IList<UnityEngine::Resolution>(decltype(nullptr));
				IList<UnityEngine::Resolution>(Plugin::InternalUse iu, int32_t handle);
				IList<UnityEngine::Resolution>(const IList<UnityEngine::Resolution>& other);
				IList<UnityEngine::Resolution>(IList<UnityEngine::Resolution>&& other);
				virtual ~IList<UnityEngine::Resolution>();
				IList<UnityEngine::Resolution>& operator=(const IList<UnityEngine::Resolution>& other);
				IList<UnityEngine::Resolution>& operator=(decltype(nullptr));
				IList<UnityEngine::Resolution>& operator=(IList<UnityEngine::Resolution>&& other);
				bool operator==(const IList<UnityEngine::Resolution>& other) const;
				bool operator!=(const IList<UnityEngine::Resolution>& other) const;
			};
		}
	}
}

namespace Plugin
{
	struct SystemCollectionsGenericIListUnityEngineResolutionIterator
	{
		System::Collections::Generic::IEnumerator<UnityEngine::Resolution> enumerator;
		bool hasMore;
		SystemCollectionsGenericIListUnityEngineResolutionIterator(decltype(nullptr));
		SystemCollectionsGenericIListUnityEngineResolutionIterator(System::Collections::Generic::IList<UnityEngine::Resolution>& enumerable);
		~SystemCollectionsGenericIListUnityEngineResolutionIterator();
		SystemCollectionsGenericIListUnityEngineResolutionIterator& operator++();
		bool operator!=(const SystemCollectionsGenericIListUnityEngineResolutionIterator& other);
		UnityEngine::Resolution operator*();
	};
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			Plugin::SystemCollectionsGenericIListUnityEngineResolutionIterator begin(System::Collections::Generic::IList<UnityEngine::Resolution>& enumerable);
			Plugin::SystemCollectionsGenericIListUnityEngineResolutionIterator end(System::Collections::Generic::IList<UnityEngine::Resolution>& enumerable);
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
				ISerializable(decltype(nullptr));
				ISerializable(Plugin::InternalUse iu, int32_t handle);
				ISerializable(const ISerializable& other);
				ISerializable(ISerializable&& other);
				virtual ~ISerializable();
				ISerializable& operator=(const ISerializable& other);
				ISerializable& operator=(decltype(nullptr));
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
				_Exception(decltype(nullptr));
				_Exception(Plugin::InternalUse iu, int32_t handle);
				_Exception(const _Exception& other);
				_Exception(_Exception&& other);
				virtual ~_Exception();
				_Exception& operator=(const _Exception& other);
				_Exception& operator=(decltype(nullptr));
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
		IAppDomainSetup(decltype(nullptr));
		IAppDomainSetup(Plugin::InternalUse iu, int32_t handle);
		IAppDomainSetup(const IAppDomainSetup& other);
		IAppDomainSetup(IAppDomainSetup&& other);
		virtual ~IAppDomainSetup();
		IAppDomainSetup& operator=(const IAppDomainSetup& other);
		IAppDomainSetup& operator=(decltype(nullptr));
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
			IComparer(decltype(nullptr));
			IComparer(Plugin::InternalUse iu, int32_t handle);
			IComparer(const IComparer& other);
			IComparer(IComparer&& other);
			virtual ~IComparer();
			IComparer& operator=(const IComparer& other);
			IComparer& operator=(decltype(nullptr));
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
			IEqualityComparer(decltype(nullptr));
			IEqualityComparer(Plugin::InternalUse iu, int32_t handle);
			IEqualityComparer(const IEqualityComparer& other);
			IEqualityComparer(IEqualityComparer&& other);
			virtual ~IEqualityComparer();
			IEqualityComparer& operator=(const IEqualityComparer& other);
			IEqualityComparer& operator=(decltype(nullptr));
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
				IEqualityComparer<System::String>(decltype(nullptr));
				IEqualityComparer<System::String>(Plugin::InternalUse iu, int32_t handle);
				IEqualityComparer<System::String>(const IEqualityComparer<System::String>& other);
				IEqualityComparer<System::String>(IEqualityComparer<System::String>&& other);
				virtual ~IEqualityComparer<System::String>();
				IEqualityComparer<System::String>& operator=(const IEqualityComparer<System::String>& other);
				IEqualityComparer<System::String>& operator=(decltype(nullptr));
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
				IEqualityComparer<int32_t>(decltype(nullptr));
				IEqualityComparer<int32_t>(Plugin::InternalUse iu, int32_t handle);
				IEqualityComparer<int32_t>(const IEqualityComparer<int32_t>& other);
				IEqualityComparer<int32_t>(IEqualityComparer<int32_t>&& other);
				virtual ~IEqualityComparer<int32_t>();
				IEqualityComparer<int32_t>& operator=(const IEqualityComparer<int32_t>& other);
				IEqualityComparer<int32_t>& operator=(decltype(nullptr));
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
			PlayableGraph(decltype(nullptr));
			PlayableGraph(Plugin::InternalUse iu, int32_t handle);
			PlayableGraph(const PlayableGraph& other);
			PlayableGraph(PlayableGraph&& other);
			virtual ~PlayableGraph();
			PlayableGraph& operator=(const PlayableGraph& other);
			PlayableGraph& operator=(decltype(nullptr));
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
			IPlayable(decltype(nullptr));
			IPlayable(Plugin::InternalUse iu, int32_t handle);
			IPlayable(const IPlayable& other);
			IPlayable(IPlayable&& other);
			virtual ~IPlayable();
			IPlayable& operator=(const IPlayable& other);
			IPlayable& operator=(decltype(nullptr));
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
		IEquatable<UnityEngine::Animations::AnimationMixerPlayable>(decltype(nullptr));
		IEquatable<UnityEngine::Animations::AnimationMixerPlayable>(Plugin::InternalUse iu, int32_t handle);
		IEquatable<UnityEngine::Animations::AnimationMixerPlayable>(const IEquatable<UnityEngine::Animations::AnimationMixerPlayable>& other);
		IEquatable<UnityEngine::Animations::AnimationMixerPlayable>(IEquatable<UnityEngine::Animations::AnimationMixerPlayable>&& other);
		virtual ~IEquatable<UnityEngine::Animations::AnimationMixerPlayable>();
		IEquatable<UnityEngine::Animations::AnimationMixerPlayable>& operator=(const IEquatable<UnityEngine::Animations::AnimationMixerPlayable>& other);
		IEquatable<UnityEngine::Animations::AnimationMixerPlayable>& operator=(decltype(nullptr));
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
			AnimationMixerPlayable(decltype(nullptr));
			AnimationMixerPlayable(Plugin::InternalUse iu, int32_t handle);
			AnimationMixerPlayable(const AnimationMixerPlayable& other);
			AnimationMixerPlayable(AnimationMixerPlayable&& other);
			virtual ~AnimationMixerPlayable();
			AnimationMixerPlayable& operator=(const AnimationMixerPlayable& other);
			AnimationMixerPlayable& operator=(decltype(nullptr));
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
				IStrongBox(decltype(nullptr));
				IStrongBox(Plugin::InternalUse iu, int32_t handle);
				IStrongBox(const IStrongBox& other);
				IStrongBox(IStrongBox&& other);
				virtual ~IStrongBox();
				IStrongBox& operator=(const IStrongBox& other);
				IStrongBox& operator=(decltype(nullptr));
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
				IEventHandler(decltype(nullptr));
				IEventHandler(Plugin::InternalUse iu, int32_t handle);
				IEventHandler(const IEventHandler& other);
				IEventHandler(IEventHandler&& other);
				virtual ~IEventHandler();
				IEventHandler& operator=(const IEventHandler& other);
				IEventHandler& operator=(decltype(nullptr));
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
				IStyle(decltype(nullptr));
				IStyle(Plugin::InternalUse iu, int32_t handle);
				IStyle(const IStyle& other);
				IStyle(IStyle&& other);
				virtual ~IStyle();
				IStyle& operator=(const IStyle& other);
				IStyle& operator=(decltype(nullptr));
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
			Stopwatch(decltype(nullptr));
			Stopwatch(Plugin::InternalUse iu, int32_t handle);
			Stopwatch(const Stopwatch& other);
			Stopwatch(Stopwatch&& other);
			virtual ~Stopwatch();
			Stopwatch& operator=(const Stopwatch& other);
			Stopwatch& operator=(decltype(nullptr));
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
		GameObject(decltype(nullptr));
		GameObject(Plugin::InternalUse iu, int32_t handle);
		GameObject(const GameObject& other);
		GameObject(GameObject&& other);
		virtual ~GameObject();
		GameObject& operator=(const GameObject& other);
		GameObject& operator=(decltype(nullptr));
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
		Debug(decltype(nullptr));
		Debug(Plugin::InternalUse iu, int32_t handle);
		Debug(const Debug& other);
		Debug(Debug&& other);
		virtual ~Debug();
		Debug& operator=(const Debug& other);
		Debug& operator=(decltype(nullptr));
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
		Collision(decltype(nullptr));
		Collision(Plugin::InternalUse iu, int32_t handle);
		Collision(const Collision& other);
		Collision(Collision&& other);
		virtual ~Collision();
		Collision& operator=(const Collision& other);
		Collision& operator=(decltype(nullptr));
		Collision& operator=(Collision&& other);
		bool operator==(const Collision& other) const;
		bool operator!=(const Collision& other) const;
	};
}

namespace UnityEngine
{
	struct Behaviour : virtual UnityEngine::Component
	{
		Behaviour(decltype(nullptr));
		Behaviour(Plugin::InternalUse iu, int32_t handle);
		Behaviour(const Behaviour& other);
		Behaviour(Behaviour&& other);
		virtual ~Behaviour();
		Behaviour& operator=(const Behaviour& other);
		Behaviour& operator=(decltype(nullptr));
		Behaviour& operator=(Behaviour&& other);
		bool operator==(const Behaviour& other) const;
		bool operator!=(const Behaviour& other) const;
	};
}

namespace UnityEngine
{
	struct MonoBehaviour : virtual UnityEngine::Behaviour
	{
		MonoBehaviour(decltype(nullptr));
		MonoBehaviour(Plugin::InternalUse iu, int32_t handle);
		MonoBehaviour(const MonoBehaviour& other);
		MonoBehaviour(MonoBehaviour&& other);
		virtual ~MonoBehaviour();
		MonoBehaviour& operator=(const MonoBehaviour& other);
		MonoBehaviour& operator=(decltype(nullptr));
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
		AudioSettings(decltype(nullptr));
		AudioSettings(Plugin::InternalUse iu, int32_t handle);
		AudioSettings(const AudioSettings& other);
		AudioSettings(AudioSettings&& other);
		virtual ~AudioSettings();
		AudioSettings& operator=(const AudioSettings& other);
		AudioSettings& operator=(decltype(nullptr));
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
			NetworkTransport(decltype(nullptr));
			NetworkTransport(Plugin::InternalUse iu, int32_t handle);
			NetworkTransport(const NetworkTransport& other);
			NetworkTransport(NetworkTransport&& other);
			virtual ~NetworkTransport();
			NetworkTransport& operator=(const NetworkTransport& other);
			NetworkTransport& operator=(decltype(nullptr));
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
				KeyValuePair<System::String, double>(decltype(nullptr));
				KeyValuePair<System::String, double>(Plugin::InternalUse iu, int32_t handle);
				KeyValuePair<System::String, double>(const KeyValuePair<System::String, double>& other);
				KeyValuePair<System::String, double>(KeyValuePair<System::String, double>&& other);
				virtual ~KeyValuePair<System::String, double>();
				KeyValuePair<System::String, double>& operator=(const KeyValuePair<System::String, double>& other);
				KeyValuePair<System::String, double>& operator=(decltype(nullptr));
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
				List<System::String>(decltype(nullptr));
				List<System::String>(Plugin::InternalUse iu, int32_t handle);
				List<System::String>(const List<System::String>& other);
				List<System::String>(List<System::String>&& other);
				virtual ~List<System::String>();
				List<System::String>& operator=(const List<System::String>& other);
				List<System::String>& operator=(decltype(nullptr));
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

namespace Plugin
{
	struct SystemCollectionsGenericListSystemStringIterator
	{
		System::Collections::Generic::IEnumerator<System::String> enumerator;
		bool hasMore;
		SystemCollectionsGenericListSystemStringIterator(decltype(nullptr));
		SystemCollectionsGenericListSystemStringIterator(System::Collections::Generic::List<System::String>& enumerable);
		~SystemCollectionsGenericListSystemStringIterator();
		SystemCollectionsGenericListSystemStringIterator& operator++();
		bool operator!=(const SystemCollectionsGenericListSystemStringIterator& other);
		System::String operator*();
	};
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			Plugin::SystemCollectionsGenericListSystemStringIterator begin(System::Collections::Generic::List<System::String>& enumerable);
			Plugin::SystemCollectionsGenericListSystemStringIterator end(System::Collections::Generic::List<System::String>& enumerable);
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
				List<int32_t>(decltype(nullptr));
				List<int32_t>(Plugin::InternalUse iu, int32_t handle);
				List<int32_t>(const List<int32_t>& other);
				List<int32_t>(List<int32_t>&& other);
				virtual ~List<int32_t>();
				List<int32_t>& operator=(const List<int32_t>& other);
				List<int32_t>& operator=(decltype(nullptr));
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

namespace Plugin
{
	struct SystemCollectionsGenericListSystemInt32Iterator
	{
		System::Collections::Generic::IEnumerator<int32_t> enumerator;
		bool hasMore;
		SystemCollectionsGenericListSystemInt32Iterator(decltype(nullptr));
		SystemCollectionsGenericListSystemInt32Iterator(System::Collections::Generic::List<int32_t>& enumerable);
		~SystemCollectionsGenericListSystemInt32Iterator();
		SystemCollectionsGenericListSystemInt32Iterator& operator++();
		bool operator!=(const SystemCollectionsGenericListSystemInt32Iterator& other);
		int32_t operator*();
	};
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			Plugin::SystemCollectionsGenericListSystemInt32Iterator begin(System::Collections::Generic::List<int32_t>& enumerable);
			Plugin::SystemCollectionsGenericListSystemInt32Iterator end(System::Collections::Generic::List<int32_t>& enumerable);
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
				LinkedListNode<System::String>(decltype(nullptr));
				LinkedListNode<System::String>(Plugin::InternalUse iu, int32_t handle);
				LinkedListNode<System::String>(const LinkedListNode<System::String>& other);
				LinkedListNode<System::String>(LinkedListNode<System::String>&& other);
				virtual ~LinkedListNode<System::String>();
				LinkedListNode<System::String>& operator=(const LinkedListNode<System::String>& other);
				LinkedListNode<System::String>& operator=(decltype(nullptr));
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
				StrongBox<System::String>(decltype(nullptr));
				StrongBox<System::String>(Plugin::InternalUse iu, int32_t handle);
				StrongBox<System::String>(const StrongBox<System::String>& other);
				StrongBox<System::String>(StrongBox<System::String>&& other);
				virtual ~StrongBox<System::String>();
				StrongBox<System::String>& operator=(const StrongBox<System::String>& other);
				StrongBox<System::String>& operator=(decltype(nullptr));
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
				Collection<int32_t>(decltype(nullptr));
				Collection<int32_t>(Plugin::InternalUse iu, int32_t handle);
				Collection<int32_t>(const Collection<int32_t>& other);
				Collection<int32_t>(Collection<int32_t>&& other);
				virtual ~Collection<int32_t>();
				Collection<int32_t>& operator=(const Collection<int32_t>& other);
				Collection<int32_t>& operator=(decltype(nullptr));
				Collection<int32_t>& operator=(Collection<int32_t>&& other);
				bool operator==(const Collection<int32_t>& other) const;
				bool operator!=(const Collection<int32_t>& other) const;
			};
		}
	}
}

namespace Plugin
{
	struct SystemCollectionsObjectModelCollectionSystemInt32Iterator
	{
		System::Collections::Generic::IEnumerator<int32_t> enumerator;
		bool hasMore;
		SystemCollectionsObjectModelCollectionSystemInt32Iterator(decltype(nullptr));
		SystemCollectionsObjectModelCollectionSystemInt32Iterator(System::Collections::ObjectModel::Collection<int32_t>& enumerable);
		~SystemCollectionsObjectModelCollectionSystemInt32Iterator();
		SystemCollectionsObjectModelCollectionSystemInt32Iterator& operator++();
		bool operator!=(const SystemCollectionsObjectModelCollectionSystemInt32Iterator& other);
		int32_t operator*();
	};
}

namespace System
{
	namespace Collections
	{
		namespace ObjectModel
		{
			Plugin::SystemCollectionsObjectModelCollectionSystemInt32Iterator begin(System::Collections::ObjectModel::Collection<int32_t>& enumerable);
			Plugin::SystemCollectionsObjectModelCollectionSystemInt32Iterator end(System::Collections::ObjectModel::Collection<int32_t>& enumerable);
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
				KeyedCollection<System::String, int32_t>(decltype(nullptr));
				KeyedCollection<System::String, int32_t>(Plugin::InternalUse iu, int32_t handle);
				KeyedCollection<System::String, int32_t>(const KeyedCollection<System::String, int32_t>& other);
				KeyedCollection<System::String, int32_t>(KeyedCollection<System::String, int32_t>&& other);
				virtual ~KeyedCollection<System::String, int32_t>();
				KeyedCollection<System::String, int32_t>& operator=(const KeyedCollection<System::String, int32_t>& other);
				KeyedCollection<System::String, int32_t>& operator=(decltype(nullptr));
				KeyedCollection<System::String, int32_t>& operator=(KeyedCollection<System::String, int32_t>&& other);
				bool operator==(const KeyedCollection<System::String, int32_t>& other) const;
				bool operator!=(const KeyedCollection<System::String, int32_t>& other) const;
			};
		}
	}
}

namespace Plugin
{
	struct SystemCollectionsObjectModelKeyedCollectionSystemString_SystemInt32Iterator
	{
		System::Collections::Generic::IEnumerator<int32_t> enumerator;
		bool hasMore;
		SystemCollectionsObjectModelKeyedCollectionSystemString_SystemInt32Iterator(decltype(nullptr));
		SystemCollectionsObjectModelKeyedCollectionSystemString_SystemInt32Iterator(System::Collections::ObjectModel::KeyedCollection<System::String, int32_t>& enumerable);
		~SystemCollectionsObjectModelKeyedCollectionSystemString_SystemInt32Iterator();
		SystemCollectionsObjectModelKeyedCollectionSystemString_SystemInt32Iterator& operator++();
		bool operator!=(const SystemCollectionsObjectModelKeyedCollectionSystemString_SystemInt32Iterator& other);
		int32_t operator*();
	};
}

namespace System
{
	namespace Collections
	{
		namespace ObjectModel
		{
			Plugin::SystemCollectionsObjectModelKeyedCollectionSystemString_SystemInt32Iterator begin(System::Collections::ObjectModel::KeyedCollection<System::String, int32_t>& enumerable);
			Plugin::SystemCollectionsObjectModelKeyedCollectionSystemString_SystemInt32Iterator end(System::Collections::ObjectModel::KeyedCollection<System::String, int32_t>& enumerable);
		}
	}
}

namespace System
{
	struct Exception : virtual System::Runtime::InteropServices::_Exception, virtual System::Runtime::Serialization::ISerializable
	{
		Exception(decltype(nullptr));
		Exception(Plugin::InternalUse iu, int32_t handle);
		Exception(const Exception& other);
		Exception(Exception&& other);
		virtual ~Exception();
		Exception& operator=(const Exception& other);
		Exception& operator=(decltype(nullptr));
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
		SystemException(decltype(nullptr));
		SystemException(Plugin::InternalUse iu, int32_t handle);
		SystemException(const SystemException& other);
		SystemException(SystemException&& other);
		virtual ~SystemException();
		SystemException& operator=(const SystemException& other);
		SystemException& operator=(decltype(nullptr));
		SystemException& operator=(SystemException&& other);
		bool operator==(const SystemException& other) const;
		bool operator!=(const SystemException& other) const;
	};
}

namespace System
{
	struct NullReferenceException : virtual System::SystemException, virtual System::Runtime::InteropServices::_Exception, virtual System::Runtime::Serialization::ISerializable
	{
		NullReferenceException(decltype(nullptr));
		NullReferenceException(Plugin::InternalUse iu, int32_t handle);
		NullReferenceException(const NullReferenceException& other);
		NullReferenceException(NullReferenceException&& other);
		virtual ~NullReferenceException();
		NullReferenceException& operator=(const NullReferenceException& other);
		NullReferenceException& operator=(decltype(nullptr));
		NullReferenceException& operator=(NullReferenceException&& other);
		bool operator==(const NullReferenceException& other) const;
		bool operator!=(const NullReferenceException& other) const;
	};
}

namespace UnityEngine
{
	struct Screen : virtual System::Object
	{
		Screen(decltype(nullptr));
		Screen(Plugin::InternalUse iu, int32_t handle);
		Screen(const Screen& other);
		Screen(Screen&& other);
		virtual ~Screen();
		Screen& operator=(const Screen& other);
		Screen& operator=(decltype(nullptr));
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
		Ray(decltype(nullptr));
		Ray(Plugin::InternalUse iu, int32_t handle);
		Ray(const Ray& other);
		Ray(Ray&& other);
		virtual ~Ray();
		Ray& operator=(const Ray& other);
		Ray& operator=(decltype(nullptr));
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
		Physics(decltype(nullptr));
		Physics(Plugin::InternalUse iu, int32_t handle);
		Physics(const Physics& other);
		Physics(Physics&& other);
		virtual ~Physics();
		Physics& operator=(const Physics& other);
		Physics& operator=(decltype(nullptr));
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
		Gradient(decltype(nullptr));
		Gradient(Plugin::InternalUse iu, int32_t handle);
		Gradient(const Gradient& other);
		Gradient(Gradient&& other);
		virtual ~Gradient();
		Gradient& operator=(const Gradient& other);
		Gradient& operator=(decltype(nullptr));
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
		AppDomainSetup(decltype(nullptr));
		AppDomainSetup(Plugin::InternalUse iu, int32_t handle);
		AppDomainSetup(const AppDomainSetup& other);
		AppDomainSetup(AppDomainSetup&& other);
		virtual ~AppDomainSetup();
		AppDomainSetup& operator=(const AppDomainSetup& other);
		AppDomainSetup& operator=(decltype(nullptr));
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
		Application(decltype(nullptr));
		Application(Plugin::InternalUse iu, int32_t handle);
		Application(const Application& other);
		Application(Application&& other);
		virtual ~Application();
		Application& operator=(const Application& other);
		Application& operator=(decltype(nullptr));
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
			SceneManager(decltype(nullptr));
			SceneManager(Plugin::InternalUse iu, int32_t handle);
			SceneManager(const SceneManager& other);
			SceneManager(SceneManager&& other);
			virtual ~SceneManager();
			SceneManager& operator=(const SceneManager& other);
			SceneManager& operator=(decltype(nullptr));
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
			Scene(decltype(nullptr));
			Scene(Plugin::InternalUse iu, int32_t handle);
			Scene(const Scene& other);
			Scene(Scene&& other);
			virtual ~Scene();
			Scene& operator=(const Scene& other);
			Scene& operator=(decltype(nullptr));
			Scene& operator=(Scene&& other);
			bool operator==(const Scene& other) const;
			bool operator!=(const Scene& other) const;
		};
	}
}

namespace System
{
	struct EventArgs : virtual System::Object
	{
		EventArgs(decltype(nullptr));
		EventArgs(Plugin::InternalUse iu, int32_t handle);
		EventArgs(const EventArgs& other);
		EventArgs(EventArgs&& other);
		virtual ~EventArgs();
		EventArgs& operator=(const EventArgs& other);
		EventArgs& operator=(decltype(nullptr));
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
				ComponentEventArgs(decltype(nullptr));
				ComponentEventArgs(Plugin::InternalUse iu, int32_t handle);
				ComponentEventArgs(const ComponentEventArgs& other);
				ComponentEventArgs(ComponentEventArgs&& other);
				virtual ~ComponentEventArgs();
				ComponentEventArgs& operator=(const ComponentEventArgs& other);
				ComponentEventArgs& operator=(decltype(nullptr));
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
				ComponentChangingEventArgs(decltype(nullptr));
				ComponentChangingEventArgs(Plugin::InternalUse iu, int32_t handle);
				ComponentChangingEventArgs(const ComponentChangingEventArgs& other);
				ComponentChangingEventArgs(ComponentChangingEventArgs&& other);
				virtual ~ComponentChangingEventArgs();
				ComponentChangingEventArgs& operator=(const ComponentChangingEventArgs& other);
				ComponentChangingEventArgs& operator=(decltype(nullptr));
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
				ComponentChangedEventArgs(decltype(nullptr));
				ComponentChangedEventArgs(Plugin::InternalUse iu, int32_t handle);
				ComponentChangedEventArgs(const ComponentChangedEventArgs& other);
				ComponentChangedEventArgs(ComponentChangedEventArgs&& other);
				virtual ~ComponentChangedEventArgs();
				ComponentChangedEventArgs& operator=(const ComponentChangedEventArgs& other);
				ComponentChangedEventArgs& operator=(decltype(nullptr));
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
				ComponentRenameEventArgs(decltype(nullptr));
				ComponentRenameEventArgs(Plugin::InternalUse iu, int32_t handle);
				ComponentRenameEventArgs(const ComponentRenameEventArgs& other);
				ComponentRenameEventArgs(ComponentRenameEventArgs&& other);
				virtual ~ComponentRenameEventArgs();
				ComponentRenameEventArgs& operator=(const ComponentRenameEventArgs& other);
				ComponentRenameEventArgs& operator=(decltype(nullptr));
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
			MemberDescriptor(decltype(nullptr));
			MemberDescriptor(Plugin::InternalUse iu, int32_t handle);
			MemberDescriptor(const MemberDescriptor& other);
			MemberDescriptor(MemberDescriptor&& other);
			virtual ~MemberDescriptor();
			MemberDescriptor& operator=(const MemberDescriptor& other);
			MemberDescriptor& operator=(decltype(nullptr));
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
		Time(decltype(nullptr));
		Time(Plugin::InternalUse iu, int32_t handle);
		Time(const Time& other);
		Time(Time&& other);
		virtual ~Time();
		Time& operator=(const Time& other);
		Time& operator=(decltype(nullptr));
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
		MarshalByRefObject(decltype(nullptr));
		MarshalByRefObject(Plugin::InternalUse iu, int32_t handle);
		MarshalByRefObject(const MarshalByRefObject& other);
		MarshalByRefObject(MarshalByRefObject&& other);
		virtual ~MarshalByRefObject();
		MarshalByRefObject& operator=(const MarshalByRefObject& other);
		MarshalByRefObject& operator=(decltype(nullptr));
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
			Stream(decltype(nullptr));
			Stream(Plugin::InternalUse iu, int32_t handle);
			Stream(const Stream& other);
			Stream(Stream&& other);
			virtual ~Stream();
			Stream& operator=(const Stream& other);
			Stream& operator=(decltype(nullptr));
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
				IComparer<int32_t>(decltype(nullptr));
				IComparer<int32_t>(Plugin::InternalUse iu, int32_t handle);
				IComparer<int32_t>(const IComparer<int32_t>& other);
				IComparer<int32_t>(IComparer<int32_t>&& other);
				virtual ~IComparer<int32_t>();
				IComparer<int32_t>& operator=(const IComparer<int32_t>& other);
				IComparer<int32_t>& operator=(decltype(nullptr));
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
				IComparer<System::String>(decltype(nullptr));
				IComparer<System::String>(Plugin::InternalUse iu, int32_t handle);
				IComparer<System::String>(const IComparer<System::String>& other);
				IComparer<System::String>(IComparer<System::String>&& other);
				virtual ~IComparer<System::String>();
				IComparer<System::String>& operator=(const IComparer<System::String>& other);
				IComparer<System::String>& operator=(decltype(nullptr));
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
				BaseIComparer<int32_t>(decltype(nullptr));
				BaseIComparer<int32_t>(Plugin::InternalUse iu, int32_t handle);
				BaseIComparer<int32_t>(const BaseIComparer<int32_t>& other);
				BaseIComparer<int32_t>(BaseIComparer<int32_t>&& other);
				virtual ~BaseIComparer<int32_t>();
				BaseIComparer<int32_t>& operator=(const BaseIComparer<int32_t>& other);
				BaseIComparer<int32_t>& operator=(decltype(nullptr));
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
				BaseIComparer<System::String>(decltype(nullptr));
				BaseIComparer<System::String>(Plugin::InternalUse iu, int32_t handle);
				BaseIComparer<System::String>(const BaseIComparer<System::String>& other);
				BaseIComparer<System::String>(BaseIComparer<System::String>&& other);
				virtual ~BaseIComparer<System::String>();
				BaseIComparer<System::String>& operator=(const BaseIComparer<System::String>& other);
				BaseIComparer<System::String>& operator=(decltype(nullptr));
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
		StringComparer(decltype(nullptr));
		StringComparer(Plugin::InternalUse iu, int32_t handle);
		StringComparer(const StringComparer& other);
		StringComparer(StringComparer&& other);
		virtual ~StringComparer();
		StringComparer& operator=(const StringComparer& other);
		StringComparer& operator=(decltype(nullptr));
		StringComparer& operator=(StringComparer&& other);
		bool operator==(const StringComparer& other) const;
		bool operator!=(const StringComparer& other) const;
	};
}

namespace System
{
	struct BaseStringComparer : virtual System::StringComparer
	{
		BaseStringComparer(decltype(nullptr));
		BaseStringComparer(Plugin::InternalUse iu, int32_t handle);
		BaseStringComparer(const BaseStringComparer& other);
		BaseStringComparer(BaseStringComparer&& other);
		virtual ~BaseStringComparer();
		BaseStringComparer& operator=(const BaseStringComparer& other);
		BaseStringComparer& operator=(decltype(nullptr));
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
			Queue(decltype(nullptr));
			Queue(Plugin::InternalUse iu, int32_t handle);
			Queue(const Queue& other);
			Queue(Queue&& other);
			virtual ~Queue();
			Queue& operator=(const Queue& other);
			Queue& operator=(decltype(nullptr));
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
			BaseQueue(decltype(nullptr));
			BaseQueue(Plugin::InternalUse iu, int32_t handle);
			BaseQueue(const BaseQueue& other);
			BaseQueue(BaseQueue&& other);
			virtual ~BaseQueue();
			BaseQueue& operator=(const BaseQueue& other);
			BaseQueue& operator=(decltype(nullptr));
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
				IComponentChangeService(decltype(nullptr));
				IComponentChangeService(Plugin::InternalUse iu, int32_t handle);
				IComponentChangeService(const IComponentChangeService& other);
				IComponentChangeService(IComponentChangeService&& other);
				virtual ~IComponentChangeService();
				IComponentChangeService& operator=(const IComponentChangeService& other);
				IComponentChangeService& operator=(decltype(nullptr));
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
				BaseIComponentChangeService(decltype(nullptr));
				BaseIComponentChangeService(Plugin::InternalUse iu, int32_t handle);
				BaseIComponentChangeService(const BaseIComponentChangeService& other);
				BaseIComponentChangeService(BaseIComponentChangeService&& other);
				virtual ~BaseIComponentChangeService();
				BaseIComponentChangeService& operator=(const BaseIComponentChangeService& other);
				BaseIComponentChangeService& operator=(decltype(nullptr));
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
			FileStream(decltype(nullptr));
			FileStream(Plugin::InternalUse iu, int32_t handle);
			FileStream(const FileStream& other);
			FileStream(FileStream&& other);
			virtual ~FileStream();
			FileStream& operator=(const FileStream& other);
			FileStream& operator=(decltype(nullptr));
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
			BaseFileStream(decltype(nullptr));
			BaseFileStream(Plugin::InternalUse iu, int32_t handle);
			BaseFileStream(const BaseFileStream& other);
			BaseFileStream(BaseFileStream&& other);
			virtual ~BaseFileStream();
			BaseFileStream& operator=(const BaseFileStream& other);
			BaseFileStream& operator=(decltype(nullptr));
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
			PlayableHandle(decltype(nullptr));
			PlayableHandle(Plugin::InternalUse iu, int32_t handle);
			PlayableHandle(const PlayableHandle& other);
			PlayableHandle(PlayableHandle&& other);
			virtual ~PlayableHandle();
			PlayableHandle& operator=(const PlayableHandle& other);
			PlayableHandle& operator=(decltype(nullptr));
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
				CallbackEventHandler(decltype(nullptr));
				CallbackEventHandler(Plugin::InternalUse iu, int32_t handle);
				CallbackEventHandler(const CallbackEventHandler& other);
				CallbackEventHandler(CallbackEventHandler&& other);
				virtual ~CallbackEventHandler();
				CallbackEventHandler& operator=(const CallbackEventHandler& other);
				CallbackEventHandler& operator=(decltype(nullptr));
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
				VisualElement(decltype(nullptr));
				VisualElement(Plugin::InternalUse iu, int32_t handle);
				VisualElement(const VisualElement& other);
				VisualElement(VisualElement&& other);
				virtual ~VisualElement();
				VisualElement& operator=(const VisualElement& other);
				VisualElement& operator=(decltype(nullptr));
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
					InteractionSourcePose(decltype(nullptr));
					InteractionSourcePose(Plugin::InternalUse iu, int32_t handle);
					InteractionSourcePose(const InteractionSourcePose& other);
					InteractionSourcePose(InteractionSourcePose&& other);
					virtual ~InteractionSourcePose();
					InteractionSourcePose& operator=(const InteractionSourcePose& other);
					InteractionSourcePose& operator=(decltype(nullptr));
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
			TestScript(decltype(nullptr));
			TestScript(Plugin::InternalUse iu, int32_t handle);
			TestScript(const TestScript& other);
			TestScript(TestScript&& other);
			virtual ~TestScript();
			TestScript& operator=(const TestScript& other);
			TestScript& operator=(decltype(nullptr));
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
			AnotherScript(decltype(nullptr));
			AnotherScript(Plugin::InternalUse iu, int32_t handle);
			AnotherScript(const AnotherScript& other);
			AnotherScript(AnotherScript&& other);
			virtual ~AnotherScript();
			AnotherScript& operator=(const AnotherScript& other);
			AnotherScript& operator=(decltype(nullptr));
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
		Array1<int32_t>(decltype(nullptr));
		Array1<int32_t>(Plugin::InternalUse iu, int32_t handle);
		Array1<int32_t>(const Array1<int32_t>& other);
		Array1<int32_t>(Array1<int32_t>&& other);
		virtual ~Array1<int32_t>();
		Array1<int32_t>& operator=(const Array1<int32_t>& other);
		Array1<int32_t>& operator=(decltype(nullptr));
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
	struct SystemInt32Array1Iterator
	{
		System::Array1<int32_t>& array;
		int index;
		SystemInt32Array1Iterator(System::Array1<int32_t>& array, int32_t index);
		SystemInt32Array1Iterator& operator++();
		bool operator!=(const SystemInt32Array1Iterator& other);
		int32_t operator*();
	};
}

namespace System
{
	Plugin::SystemInt32Array1Iterator begin(System::Array1<int32_t>& array);
	Plugin::SystemInt32Array1Iterator end(System::Array1<int32_t>& array);
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
		Array1<float>(decltype(nullptr));
		Array1<float>(Plugin::InternalUse iu, int32_t handle);
		Array1<float>(const Array1<float>& other);
		Array1<float>(Array1<float>&& other);
		virtual ~Array1<float>();
		Array1<float>& operator=(const Array1<float>& other);
		Array1<float>& operator=(decltype(nullptr));
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

namespace Plugin
{
	struct SystemSingleArray1Iterator
	{
		System::Array1<float>& array;
		int index;
		SystemSingleArray1Iterator(System::Array1<float>& array, int32_t index);
		SystemSingleArray1Iterator& operator++();
		bool operator!=(const SystemSingleArray1Iterator& other);
		float operator*();
	};
}

namespace System
{
	Plugin::SystemSingleArray1Iterator begin(System::Array1<float>& array);
	Plugin::SystemSingleArray1Iterator end(System::Array1<float>& array);
}

namespace System
{
	template<> struct Array2<float> : virtual System::Array, virtual System::ICloneable, virtual System::Collections::IList
	{
		Array2<float>(decltype(nullptr));
		Array2<float>(Plugin::InternalUse iu, int32_t handle);
		Array2<float>(const Array2<float>& other);
		Array2<float>(Array2<float>&& other);
		virtual ~Array2<float>();
		Array2<float>& operator=(const Array2<float>& other);
		Array2<float>& operator=(decltype(nullptr));
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
		Array3<float>(decltype(nullptr));
		Array3<float>(Plugin::InternalUse iu, int32_t handle);
		Array3<float>(const Array3<float>& other);
		Array3<float>(Array3<float>&& other);
		virtual ~Array3<float>();
		Array3<float>& operator=(const Array3<float>& other);
		Array3<float>& operator=(decltype(nullptr));
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
		Array1<System::String>(decltype(nullptr));
		Array1<System::String>(Plugin::InternalUse iu, int32_t handle);
		Array1<System::String>(const Array1<System::String>& other);
		Array1<System::String>(Array1<System::String>&& other);
		virtual ~Array1<System::String>();
		Array1<System::String>& operator=(const Array1<System::String>& other);
		Array1<System::String>& operator=(decltype(nullptr));
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
	struct SystemStringArray1Iterator
	{
		System::Array1<System::String>& array;
		int index;
		SystemStringArray1Iterator(System::Array1<System::String>& array, int32_t index);
		SystemStringArray1Iterator& operator++();
		bool operator!=(const SystemStringArray1Iterator& other);
		System::String operator*();
	};
}

namespace System
{
	Plugin::SystemStringArray1Iterator begin(System::Array1<System::String>& array);
	Plugin::SystemStringArray1Iterator end(System::Array1<System::String>& array);
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
		Array1<UnityEngine::Resolution>(decltype(nullptr));
		Array1<UnityEngine::Resolution>(Plugin::InternalUse iu, int32_t handle);
		Array1<UnityEngine::Resolution>(const Array1<UnityEngine::Resolution>& other);
		Array1<UnityEngine::Resolution>(Array1<UnityEngine::Resolution>&& other);
		virtual ~Array1<UnityEngine::Resolution>();
		Array1<UnityEngine::Resolution>& operator=(const Array1<UnityEngine::Resolution>& other);
		Array1<UnityEngine::Resolution>& operator=(decltype(nullptr));
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
	struct UnityEngineResolutionArray1Iterator
	{
		System::Array1<UnityEngine::Resolution>& array;
		int index;
		UnityEngineResolutionArray1Iterator(System::Array1<UnityEngine::Resolution>& array, int32_t index);
		UnityEngineResolutionArray1Iterator& operator++();
		bool operator!=(const UnityEngineResolutionArray1Iterator& other);
		UnityEngine::Resolution operator*();
	};
}

namespace System
{
	Plugin::UnityEngineResolutionArray1Iterator begin(System::Array1<UnityEngine::Resolution>& array);
	Plugin::UnityEngineResolutionArray1Iterator end(System::Array1<UnityEngine::Resolution>& array);
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
		Array1<UnityEngine::RaycastHit>(decltype(nullptr));
		Array1<UnityEngine::RaycastHit>(Plugin::InternalUse iu, int32_t handle);
		Array1<UnityEngine::RaycastHit>(const Array1<UnityEngine::RaycastHit>& other);
		Array1<UnityEngine::RaycastHit>(Array1<UnityEngine::RaycastHit>&& other);
		virtual ~Array1<UnityEngine::RaycastHit>();
		Array1<UnityEngine::RaycastHit>& operator=(const Array1<UnityEngine::RaycastHit>& other);
		Array1<UnityEngine::RaycastHit>& operator=(decltype(nullptr));
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
	struct UnityEngineRaycastHitArray1Iterator
	{
		System::Array1<UnityEngine::RaycastHit>& array;
		int index;
		UnityEngineRaycastHitArray1Iterator(System::Array1<UnityEngine::RaycastHit>& array, int32_t index);
		UnityEngineRaycastHitArray1Iterator& operator++();
		bool operator!=(const UnityEngineRaycastHitArray1Iterator& other);
		UnityEngine::RaycastHit operator*();
	};
}

namespace System
{
	Plugin::UnityEngineRaycastHitArray1Iterator begin(System::Array1<UnityEngine::RaycastHit>& array);
	Plugin::UnityEngineRaycastHitArray1Iterator end(System::Array1<UnityEngine::RaycastHit>& array);
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
		Array1<UnityEngine::GradientColorKey>(decltype(nullptr));
		Array1<UnityEngine::GradientColorKey>(Plugin::InternalUse iu, int32_t handle);
		Array1<UnityEngine::GradientColorKey>(const Array1<UnityEngine::GradientColorKey>& other);
		Array1<UnityEngine::GradientColorKey>(Array1<UnityEngine::GradientColorKey>&& other);
		virtual ~Array1<UnityEngine::GradientColorKey>();
		Array1<UnityEngine::GradientColorKey>& operator=(const Array1<UnityEngine::GradientColorKey>& other);
		Array1<UnityEngine::GradientColorKey>& operator=(decltype(nullptr));
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

namespace Plugin
{
	struct UnityEngineGradientColorKeyArray1Iterator
	{
		System::Array1<UnityEngine::GradientColorKey>& array;
		int index;
		UnityEngineGradientColorKeyArray1Iterator(System::Array1<UnityEngine::GradientColorKey>& array, int32_t index);
		UnityEngineGradientColorKeyArray1Iterator& operator++();
		bool operator!=(const UnityEngineGradientColorKeyArray1Iterator& other);
		UnityEngine::GradientColorKey operator*();
	};
}

namespace System
{
	Plugin::UnityEngineGradientColorKeyArray1Iterator begin(System::Array1<UnityEngine::GradientColorKey>& array);
	Plugin::UnityEngineGradientColorKeyArray1Iterator end(System::Array1<UnityEngine::GradientColorKey>& array);
}

namespace System
{
	struct Action : virtual System::Object
	{
		Action(decltype(nullptr));
		Action(Plugin::InternalUse iu, int32_t handle);
		Action(const Action& other);
		Action(Action&& other);
		virtual ~Action();
		Action& operator=(const Action& other);
		Action& operator=(decltype(nullptr));
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
		Action1<float>(decltype(nullptr));
		Action1<float>(Plugin::InternalUse iu, int32_t handle);
		Action1<float>(const Action1<float>& other);
		Action1<float>(Action1<float>&& other);
		virtual ~Action1<float>();
		Action1<float>& operator=(const Action1<float>& other);
		Action1<float>& operator=(decltype(nullptr));
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
		Action2<float, float>(decltype(nullptr));
		Action2<float, float>(Plugin::InternalUse iu, int32_t handle);
		Action2<float, float>(const Action2<float, float>& other);
		Action2<float, float>(Action2<float, float>&& other);
		virtual ~Action2<float, float>();
		Action2<float, float>& operator=(const Action2<float, float>& other);
		Action2<float, float>& operator=(decltype(nullptr));
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
		Func3<int32_t, float, double>(decltype(nullptr));
		Func3<int32_t, float, double>(Plugin::InternalUse iu, int32_t handle);
		Func3<int32_t, float, double>(const Func3<int32_t, float, double>& other);
		Func3<int32_t, float, double>(Func3<int32_t, float, double>&& other);
		virtual ~Func3<int32_t, float, double>();
		Func3<int32_t, float, double>& operator=(const Func3<int32_t, float, double>& other);
		Func3<int32_t, float, double>& operator=(decltype(nullptr));
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
		Func3<int16_t, int32_t, System::String>(decltype(nullptr));
		Func3<int16_t, int32_t, System::String>(Plugin::InternalUse iu, int32_t handle);
		Func3<int16_t, int32_t, System::String>(const Func3<int16_t, int32_t, System::String>& other);
		Func3<int16_t, int32_t, System::String>(Func3<int16_t, int32_t, System::String>&& other);
		virtual ~Func3<int16_t, int32_t, System::String>();
		Func3<int16_t, int32_t, System::String>& operator=(const Func3<int16_t, int32_t, System::String>& other);
		Func3<int16_t, int32_t, System::String>& operator=(decltype(nullptr));
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
		AppDomainInitializer(decltype(nullptr));
		AppDomainInitializer(Plugin::InternalUse iu, int32_t handle);
		AppDomainInitializer(const AppDomainInitializer& other);
		AppDomainInitializer(AppDomainInitializer&& other);
		virtual ~AppDomainInitializer();
		AppDomainInitializer& operator=(const AppDomainInitializer& other);
		AppDomainInitializer& operator=(decltype(nullptr));
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
			UnityAction(decltype(nullptr));
			UnityAction(Plugin::InternalUse iu, int32_t handle);
			UnityAction(const UnityAction& other);
			UnityAction(UnityAction&& other);
			virtual ~UnityAction();
			UnityAction& operator=(const UnityAction& other);
			UnityAction& operator=(decltype(nullptr));
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
			UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>(decltype(nullptr));
			UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>(Plugin::InternalUse iu, int32_t handle);
			UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>(const UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>& other);
			UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>(UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>&& other);
			virtual ~UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>();
			UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>& operator=(const UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>& other);
			UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>& operator=(decltype(nullptr));
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
				ComponentEventHandler(decltype(nullptr));
				ComponentEventHandler(Plugin::InternalUse iu, int32_t handle);
				ComponentEventHandler(const ComponentEventHandler& other);
				ComponentEventHandler(ComponentEventHandler&& other);
				virtual ~ComponentEventHandler();
				ComponentEventHandler& operator=(const ComponentEventHandler& other);
				ComponentEventHandler& operator=(decltype(nullptr));
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
				ComponentChangingEventHandler(decltype(nullptr));
				ComponentChangingEventHandler(Plugin::InternalUse iu, int32_t handle);
				ComponentChangingEventHandler(const ComponentChangingEventHandler& other);
				ComponentChangingEventHandler(ComponentChangingEventHandler&& other);
				virtual ~ComponentChangingEventHandler();
				ComponentChangingEventHandler& operator=(const ComponentChangingEventHandler& other);
				ComponentChangingEventHandler& operator=(decltype(nullptr));
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
				ComponentChangedEventHandler(decltype(nullptr));
				ComponentChangedEventHandler(Plugin::InternalUse iu, int32_t handle);
				ComponentChangedEventHandler(const ComponentChangedEventHandler& other);
				ComponentChangedEventHandler(ComponentChangedEventHandler&& other);
				virtual ~ComponentChangedEventHandler();
				ComponentChangedEventHandler& operator=(const ComponentChangedEventHandler& other);
				ComponentChangedEventHandler& operator=(decltype(nullptr));
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
				ComponentRenameEventHandler(decltype(nullptr));
				ComponentRenameEventHandler(Plugin::InternalUse iu, int32_t handle);
				ComponentRenameEventHandler(const ComponentRenameEventHandler& other);
				ComponentRenameEventHandler(ComponentRenameEventHandler&& other);
				virtual ~ComponentRenameEventHandler();
				ComponentRenameEventHandler& operator=(const ComponentRenameEventHandler& other);
				ComponentRenameEventHandler& operator=(decltype(nullptr));
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

////////////////////////////////////////////////////////////////
// Support for using IEnumerable with range for loops
////////////////////////////////////////////////////////////////

namespace Plugin
{
	struct EnumerableIterator
	{
		System::Collections::IEnumerator enumerator;
		bool hasMore;
		EnumerableIterator(decltype(nullptr));
		EnumerableIterator(System::Collections::IEnumerable& enumerable);
		EnumerableIterator& operator++();
		bool operator!=(const EnumerableIterator& other);
		System::Object operator*();
	};
}

namespace System
{
	namespace Collections
	{
		Plugin::EnumerableIterator begin(IEnumerable& enumerable);
		Plugin::EnumerableIterator end(IEnumerable& enumerable);
	}
}

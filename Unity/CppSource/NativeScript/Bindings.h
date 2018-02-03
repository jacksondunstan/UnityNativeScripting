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
// Plugin internals. Do not name these in game code as they may
// change without warning. For example:
//   // Good. Uses behavior, not names.
//   int x = myArray[5];
//   // Bad. Directly uses names.
//   ArrayElementProxy1_1 proxy = myArray[5];
//   int x = proxy;
////////////////////////////////////////////////////////////////

namespace Plugin
{
	enum struct InternalUse
	{
		Only
	};
	
	struct ManagedType
	{
		int32_t Handle;
		
		ManagedType();
		ManagedType(decltype(nullptr));
		ManagedType(InternalUse, int32_t handle);
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
// C# basic types
////////////////////////////////////////////////////////////////

namespace System
{
	struct Object;
	struct ValueType;
	struct Enum;
	struct String;
	struct Array;
	template <typename TElement> struct Array1;
	template <typename TElement> struct Array2;
	template <typename TElement> struct Array3;
	template <typename TElement> struct Array4;
	template <typename TElement> struct Array5;
	struct IComparable;
	struct IFormattable;
	struct IConvertible;
	
	// .NET booleans are four bytes long
	// This struct makes them feel like C++'s bool, int32_t, and uint32_t types
	struct Boolean
	{
		int32_t Value;
		
		Boolean();
		Boolean(bool value);
		Boolean(int32_t value);
		Boolean(uint32_t value);
		operator bool() const;
		operator int32_t() const;
		operator uint32_t() const;
		explicit operator Object() const;
		explicit operator ValueType() const;
		explicit operator IComparable() const;
		explicit operator IFormattable() const;
		explicit operator IConvertible() const;
	};
	
	// .NET chars are two bytes long
	// This struct helps them interoperate with C++'s char and int16_t types
	struct Char
	{
		int16_t Value;
		
		Char();
		Char(char value);
		Char(int16_t value);
		operator int16_t() const;
		explicit operator Object() const;
		explicit operator ValueType() const;
		explicit operator IComparable() const;
		explicit operator IFormattable() const;
		explicit operator IConvertible() const;
	};
	
	struct SByte
	{
		int8_t Value;
		
		SByte();
		SByte(int8_t value);
		operator int8_t() const;
		explicit operator Object() const;
		explicit operator ValueType() const;
		explicit operator IComparable() const;
		explicit operator IFormattable() const;
		explicit operator IConvertible() const;
	};
	
	struct Byte
	{
		uint8_t Value;
		
		Byte();
		Byte(uint8_t value);
		operator uint8_t() const;
		explicit operator Object() const;
		explicit operator ValueType() const;
		explicit operator IComparable() const;
		explicit operator IFormattable() const;
		explicit operator IConvertible() const;
	};
	
	struct Int16
	{
		int16_t Value;
		
		Int16();
		Int16(int16_t value);
		operator int16_t() const;
		explicit operator Object() const;
		explicit operator ValueType() const;
		explicit operator IComparable() const;
		explicit operator IFormattable() const;
		explicit operator IConvertible() const;
	};
	
	struct UInt16
	{
		uint16_t Value;
		
		UInt16();
		UInt16(uint16_t value);
		operator uint16_t() const;
		explicit operator Object() const;
		explicit operator ValueType() const;
		explicit operator IComparable() const;
		explicit operator IFormattable() const;
		explicit operator IConvertible() const;
	};
	
	struct Int32
	{
		int32_t Value;
		
		Int32();
		Int32(int32_t value);
		operator int32_t() const;
		explicit operator Object() const;
		explicit operator ValueType() const;
		explicit operator IComparable() const;
		explicit operator IFormattable() const;
		explicit operator IConvertible() const;
	};
	
	struct UInt32
	{
		uint32_t Value;
		
		UInt32();
		UInt32(uint32_t value);
		operator uint32_t() const;
		explicit operator Object() const;
		explicit operator ValueType() const;
		explicit operator IComparable() const;
		explicit operator IFormattable() const;
		explicit operator IConvertible() const;
	};
	
	struct Int64
	{
		int64_t Value;
		
		Int64();
		Int64(int64_t value);
		operator int64_t() const;
		explicit operator Object() const;
		explicit operator ValueType() const;
		explicit operator IComparable() const;
		explicit operator IFormattable() const;
		explicit operator IConvertible() const;
	};
	
	struct UInt64
	{
		uint64_t Value;
		
		UInt64();
		UInt64(uint64_t value);
		operator uint64_t() const;
		explicit operator Object() const;
		explicit operator ValueType() const;
		explicit operator IComparable() const;
		explicit operator IFormattable() const;
		explicit operator IConvertible() const;
	};
	
	struct Single
	{
		float Value;
		
		Single();
		Single(float value);
		operator float() const;
		explicit operator Object() const;
		explicit operator ValueType() const;
		explicit operator IComparable() const;
		explicit operator IFormattable() const;
		explicit operator IConvertible() const;
	};
	
	struct Double
	{
		double Value;
		
		Double();
		Double(double value);
		operator double() const;
		explicit operator Object() const;
		explicit operator ValueType() const;
		explicit operator IComparable() const;
		explicit operator IFormattable() const;
		explicit operator IConvertible() const;
	};
}

/*BEGIN TEMPLATE DECLARATIONS*/
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
			template<typename TT0> struct List;
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
	struct IFormattable;
}

namespace System
{
	struct IConvertible;
}

namespace System
{
	struct IComparable;
}

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
			struct Focusable;
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
	struct QueryTriggerInteraction;
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
		struct LoadSceneMode;
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
	struct PrimitiveType;
}

namespace UnityEngine
{
	struct Time;
}

namespace System
{
	namespace IO
	{
		struct FileMode;
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
			struct ITransform;
		}
	}
}

namespace UnityEngine
{
	namespace Experimental
	{
		namespace UIElements
		{
			struct IUIElementDataWatch;
		}
	}
}

namespace UnityEngine
{
	namespace Experimental
	{
		namespace UIElements
		{
			struct IVisualElementScheduler;
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
				struct InteractionSourcePositionAccuracy;
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
				struct InteractionSourceNode;
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
			template<> struct IEqualityComparer<System::Int32>;
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
			template<> struct KeyValuePair<System::String, System::Double>;
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
		namespace Generic
		{
			template<> struct IComparer<System::Int32>;
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
			template<> struct BaseIComparer<System::Int32>;
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

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			template<> struct IEnumerator<UnityEngine::Experimental::UIElements::VisualElement>;
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			template<> struct IEnumerable<UnityEngine::Experimental::UIElements::VisualElement>;
		}
	}
}

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
			template<> struct IEnumerator<System::Int32>;
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			template<> struct IEnumerator<System::Single>;
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
			template<> struct IEnumerable<System::Int32>;
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			template<> struct IEnumerable<System::Single>;
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
			template<> struct ICollection<System::Int32>;
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			template<> struct ICollection<System::Single>;
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
			template<> struct IList<System::Int32>;
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			template<> struct IList<System::Single>;
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
			template<> struct List<System::Int32>;
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace ObjectModel
		{
			template<> struct Collection<System::Int32>;
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace ObjectModel
		{
			template<> struct KeyedCollection<System::String, System::Int32>;
		}
	}
}

namespace Plugin
{
	template<> struct ArrayElementProxy1_1<System::Int32>;
}

namespace System
{
	template<> struct Array1<System::Int32>;
}

namespace Plugin
{
	template<> struct ArrayElementProxy1_1<System::Single>;
}

namespace Plugin
{
	template<> struct ArrayElementProxy1_2<System::Single>;
}

namespace Plugin
{
	template<> struct ArrayElementProxy2_2<System::Single>;
}

namespace Plugin
{
	template<> struct ArrayElementProxy1_3<System::Single>;
}

namespace Plugin
{
	template<> struct ArrayElementProxy2_3<System::Single>;
}

namespace Plugin
{
	template<> struct ArrayElementProxy3_3<System::Single>;
}

namespace System
{
	template<> struct Array1<System::Single>;
}

namespace System
{
	template<> struct Array2<System::Single>;
}

namespace System
{
	template<> struct Array3<System::Single>;
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
	template<> struct Action1<System::Single>;
}

namespace System
{
	template<> struct Action2<System::Single, System::Single>;
}

namespace System
{
	template<> struct Func3<System::Int32, System::Single, System::Double>;
}

namespace System
{
	template<> struct Func3<System::Int16, System::Int32, System::String>;
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
	struct Object : Plugin::ManagedType
	{
		Object();
		Object(Plugin::InternalUse iu, int32_t handle);
		Object(decltype(nullptr));
		virtual ~Object() = default;
		bool operator==(decltype(nullptr)) const;
		bool operator!=(decltype(nullptr)) const;
		virtual void ThrowReferenceToThis();
		
		/*BEGIN UNBOXING METHOD DECLARATIONS*/
		explicit operator UnityEngine::Vector3();
		explicit operator UnityEngine::Color();
		explicit operator UnityEngine::GradientColorKey();
		explicit operator UnityEngine::Resolution();
		explicit operator UnityEngine::RaycastHit();
		explicit operator UnityEngine::Playables::PlayableGraph();
		explicit operator UnityEngine::Animations::AnimationMixerPlayable();
		explicit operator UnityEngine::Quaternion();
		explicit operator UnityEngine::Matrix4x4();
		explicit operator UnityEngine::QueryTriggerInteraction();
		explicit operator System::Collections::Generic::KeyValuePair<System::String, System::Double>();
		explicit operator UnityEngine::Ray();
		explicit operator UnityEngine::SceneManagement::Scene();
		explicit operator UnityEngine::SceneManagement::LoadSceneMode();
		explicit operator UnityEngine::PrimitiveType();
		explicit operator System::IO::FileMode();
		explicit operator UnityEngine::Playables::PlayableHandle();
		explicit operator UnityEngine::XR::WSA::Input::InteractionSourcePositionAccuracy();
		explicit operator UnityEngine::XR::WSA::Input::InteractionSourceNode();
		explicit operator UnityEngine::XR::WSA::Input::InteractionSourcePose();
		explicit operator System::Boolean();
		explicit operator System::SByte();
		explicit operator System::Byte();
		explicit operator System::Int16();
		explicit operator System::UInt16();
		explicit operator System::Int32();
		explicit operator System::UInt32();
		explicit operator System::Int64();
		explicit operator System::UInt64();
		explicit operator System::Char();
		explicit operator System::Single();
		explicit operator System::Double();
		/*END UNBOXING METHOD DECLARATIONS*/
	};
	
	struct ValueType : virtual Object
	{
		ValueType(Plugin::InternalUse iu, int32_t handle);
		ValueType(decltype(nullptr));
	};
	
	struct Enum : virtual ValueType
	{
		Enum(Plugin::InternalUse iu, int32_t handle);
		Enum(decltype(nullptr));
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
	struct IFormattable : virtual System::Object
	{
		IFormattable(decltype(nullptr));
		IFormattable(Plugin::InternalUse, int32_t handle);
		IFormattable(const IFormattable& other);
		IFormattable(IFormattable&& other);
		virtual ~IFormattable();
		IFormattable& operator=(const IFormattable& other);
		IFormattable& operator=(decltype(nullptr));
		IFormattable& operator=(IFormattable&& other);
		bool operator==(const IFormattable& other) const;
		bool operator!=(const IFormattable& other) const;
	};
}

namespace System
{
	struct IConvertible : virtual System::Object
	{
		IConvertible(decltype(nullptr));
		IConvertible(Plugin::InternalUse, int32_t handle);
		IConvertible(const IConvertible& other);
		IConvertible(IConvertible&& other);
		virtual ~IConvertible();
		IConvertible& operator=(const IConvertible& other);
		IConvertible& operator=(decltype(nullptr));
		IConvertible& operator=(IConvertible&& other);
		bool operator==(const IConvertible& other) const;
		bool operator!=(const IConvertible& other) const;
	};
}

namespace System
{
	struct IComparable : virtual System::Object
	{
		IComparable(decltype(nullptr));
		IComparable(Plugin::InternalUse, int32_t handle);
		IComparable(const IComparable& other);
		IComparable(IComparable&& other);
		virtual ~IComparable();
		IComparable& operator=(const IComparable& other);
		IComparable& operator=(decltype(nullptr));
		IComparable& operator=(IComparable&& other);
		bool operator==(const IComparable& other) const;
		bool operator!=(const IComparable& other) const;
		System::Int32 CompareTo(System::Object& obj);
	};
}

namespace System
{
	struct IDisposable : virtual System::Object
	{
		IDisposable(decltype(nullptr));
		IDisposable(Plugin::InternalUse, int32_t handle);
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
		Vector3(System::Single x, System::Single y, System::Single z);
		System::Single GetMagnitude();
		System::Single x;
		System::Single y;
		System::Single z;
		void Set(System::Single newX, System::Single newY, System::Single newZ);
		UnityEngine::Vector3 operator+(UnityEngine::Vector3& a);
		UnityEngine::Vector3 operator-();
		explicit operator System::ValueType();
		explicit operator System::Object();
	};
}

namespace UnityEngine
{
	struct Object : virtual System::Object
	{
		Object(decltype(nullptr));
		Object(Plugin::InternalUse, int32_t handle);
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
		Component(Plugin::InternalUse, int32_t handle);
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
		Transform(Plugin::InternalUse, int32_t handle);
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
		System::Single r;
		System::Single g;
		System::Single b;
		System::Single a;
		explicit operator System::ValueType();
		explicit operator System::Object();
	};
}

namespace UnityEngine
{
	struct GradientColorKey
	{
		GradientColorKey();
		UnityEngine::Color color;
		System::Single time;
		explicit operator System::ValueType();
		explicit operator System::Object();
	};
}

namespace UnityEngine
{
	struct Resolution : Plugin::ManagedType
	{
		Resolution(decltype(nullptr));
		Resolution(Plugin::InternalUse, int32_t handle);
		Resolution(const Resolution& other);
		Resolution(Resolution&& other);
		virtual ~Resolution();
		Resolution& operator=(const Resolution& other);
		Resolution& operator=(decltype(nullptr));
		Resolution& operator=(Resolution&& other);
		bool operator==(const Resolution& other) const;
		bool operator!=(const Resolution& other) const;
		Resolution();
		System::Int32 GetWidth();
		void SetWidth(System::Int32 value);
		System::Int32 GetHeight();
		void SetHeight(System::Int32 value);
		System::Int32 GetRefreshRate();
		void SetRefreshRate(System::Int32 value);
		explicit operator System::ValueType();
		explicit operator System::Object();
	};
}

namespace UnityEngine
{
	struct RaycastHit : Plugin::ManagedType
	{
		RaycastHit(decltype(nullptr));
		RaycastHit(Plugin::InternalUse, int32_t handle);
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
		explicit operator System::ValueType();
		explicit operator System::Object();
	};
}

namespace System
{
	namespace Collections
	{
		struct IEnumerator : virtual System::Object
		{
			IEnumerator(decltype(nullptr));
			IEnumerator(Plugin::InternalUse, int32_t handle);
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
	namespace Runtime
	{
		namespace Serialization
		{
			struct ISerializable : virtual System::Object
			{
				ISerializable(decltype(nullptr));
				ISerializable(Plugin::InternalUse, int32_t handle);
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
				_Exception(Plugin::InternalUse, int32_t handle);
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
		IAppDomainSetup(Plugin::InternalUse, int32_t handle);
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
			IComparer(Plugin::InternalUse, int32_t handle);
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
			IEqualityComparer(Plugin::InternalUse, int32_t handle);
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
				IEqualityComparer<System::String>(Plugin::InternalUse, int32_t handle);
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
			template<> struct IEqualityComparer<System::Int32> : virtual System::Object
			{
				IEqualityComparer<System::Int32>(decltype(nullptr));
				IEqualityComparer<System::Int32>(Plugin::InternalUse, int32_t handle);
				IEqualityComparer<System::Int32>(const IEqualityComparer<System::Int32>& other);
				IEqualityComparer<System::Int32>(IEqualityComparer<System::Int32>&& other);
				virtual ~IEqualityComparer<System::Int32>();
				IEqualityComparer<System::Int32>& operator=(const IEqualityComparer<System::Int32>& other);
				IEqualityComparer<System::Int32>& operator=(decltype(nullptr));
				IEqualityComparer<System::Int32>& operator=(IEqualityComparer<System::Int32>&& other);
				bool operator==(const IEqualityComparer<System::Int32>& other) const;
				bool operator!=(const IEqualityComparer<System::Int32>& other) const;
			};
		}
	}
}

namespace UnityEngine
{
	namespace Playables
	{
		struct PlayableGraph : Plugin::ManagedType
		{
			PlayableGraph(decltype(nullptr));
			PlayableGraph(Plugin::InternalUse, int32_t handle);
			PlayableGraph(const PlayableGraph& other);
			PlayableGraph(PlayableGraph&& other);
			virtual ~PlayableGraph();
			PlayableGraph& operator=(const PlayableGraph& other);
			PlayableGraph& operator=(decltype(nullptr));
			PlayableGraph& operator=(PlayableGraph&& other);
			bool operator==(const PlayableGraph& other) const;
			bool operator!=(const PlayableGraph& other) const;
			explicit operator System::ValueType();
			explicit operator System::Object();
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
			IPlayable(Plugin::InternalUse, int32_t handle);
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
		IEquatable<UnityEngine::Animations::AnimationMixerPlayable>(Plugin::InternalUse, int32_t handle);
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
		struct AnimationMixerPlayable : Plugin::ManagedType
		{
			AnimationMixerPlayable(decltype(nullptr));
			AnimationMixerPlayable(Plugin::InternalUse, int32_t handle);
			AnimationMixerPlayable(const AnimationMixerPlayable& other);
			AnimationMixerPlayable(AnimationMixerPlayable&& other);
			virtual ~AnimationMixerPlayable();
			AnimationMixerPlayable& operator=(const AnimationMixerPlayable& other);
			AnimationMixerPlayable& operator=(decltype(nullptr));
			AnimationMixerPlayable& operator=(AnimationMixerPlayable&& other);
			bool operator==(const AnimationMixerPlayable& other) const;
			bool operator!=(const AnimationMixerPlayable& other) const;
			static UnityEngine::Animations::AnimationMixerPlayable Create(UnityEngine::Playables::PlayableGraph& graph, System::Int32 inputCount = 0, System::Boolean normalizeWeights = false);
			explicit operator System::ValueType();
			explicit operator System::Object();
			explicit operator UnityEngine::Playables::IPlayable();
			explicit operator System::IEquatable<UnityEngine::Animations::AnimationMixerPlayable>();
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
				IStrongBox(Plugin::InternalUse, int32_t handle);
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
				IEventHandler(Plugin::InternalUse, int32_t handle);
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
			struct CallbackEventHandler : virtual UnityEngine::Experimental::UIElements::IEventHandler
			{
				CallbackEventHandler(decltype(nullptr));
				CallbackEventHandler(Plugin::InternalUse, int32_t handle);
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
			struct Focusable : virtual UnityEngine::Experimental::UIElements::CallbackEventHandler, virtual UnityEngine::Experimental::UIElements::IEventHandler
			{
				Focusable(decltype(nullptr));
				Focusable(Plugin::InternalUse, int32_t handle);
				Focusable(const Focusable& other);
				Focusable(Focusable&& other);
				virtual ~Focusable();
				Focusable& operator=(const Focusable& other);
				Focusable& operator=(decltype(nullptr));
				Focusable& operator=(Focusable&& other);
				bool operator==(const Focusable& other) const;
				bool operator!=(const Focusable& other) const;
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
				IStyle(Plugin::InternalUse, int32_t handle);
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
			Stopwatch(Plugin::InternalUse, int32_t handle);
			Stopwatch(const Stopwatch& other);
			Stopwatch(Stopwatch&& other);
			virtual ~Stopwatch();
			Stopwatch& operator=(const Stopwatch& other);
			Stopwatch& operator=(decltype(nullptr));
			Stopwatch& operator=(Stopwatch&& other);
			bool operator==(const Stopwatch& other) const;
			bool operator!=(const Stopwatch& other) const;
			Stopwatch();
			System::Int64 GetElapsedMilliseconds();
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
		GameObject(Plugin::InternalUse, int32_t handle);
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
		Debug(Plugin::InternalUse, int32_t handle);
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
		Collision(Plugin::InternalUse, int32_t handle);
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
		Behaviour(Plugin::InternalUse, int32_t handle);
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
		MonoBehaviour(Plugin::InternalUse, int32_t handle);
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
		AudioSettings(Plugin::InternalUse, int32_t handle);
		AudioSettings(const AudioSettings& other);
		AudioSettings(AudioSettings&& other);
		virtual ~AudioSettings();
		AudioSettings& operator=(const AudioSettings& other);
		AudioSettings& operator=(decltype(nullptr));
		AudioSettings& operator=(AudioSettings&& other);
		bool operator==(const AudioSettings& other) const;
		bool operator!=(const AudioSettings& other) const;
		static void GetDSPBufferSize(System::Int32* bufferLength, System::Int32* numBuffers);
	};
}

namespace UnityEngine
{
	namespace Networking
	{
		struct NetworkTransport : virtual System::Object
		{
			NetworkTransport(decltype(nullptr));
			NetworkTransport(Plugin::InternalUse, int32_t handle);
			NetworkTransport(const NetworkTransport& other);
			NetworkTransport(NetworkTransport&& other);
			virtual ~NetworkTransport();
			NetworkTransport& operator=(const NetworkTransport& other);
			NetworkTransport& operator=(decltype(nullptr));
			NetworkTransport& operator=(NetworkTransport&& other);
			bool operator==(const NetworkTransport& other) const;
			bool operator!=(const NetworkTransport& other) const;
			static void GetBroadcastConnectionInfo(System::Int32 hostId, System::String* address, System::Int32* port, System::Byte* error);
			static void Init();
		};
	}
}

namespace UnityEngine
{
	struct Quaternion
	{
		Quaternion();
		System::Single x;
		System::Single y;
		System::Single z;
		System::Single w;
		explicit operator System::ValueType();
		explicit operator System::Object();
	};
}

namespace UnityEngine
{
	struct Matrix4x4
	{
		Matrix4x4();
		System::Single GetItem(System::Int32 row, System::Int32 column);
		void SetItem(System::Int32 row, System::Int32 column, System::Single value);
		System::Single m00;
		System::Single m10;
		System::Single m20;
		System::Single m30;
		System::Single m01;
		System::Single m11;
		System::Single m21;
		System::Single m31;
		System::Single m02;
		System::Single m12;
		System::Single m22;
		System::Single m32;
		System::Single m03;
		System::Single m13;
		System::Single m23;
		System::Single m33;
		explicit operator System::ValueType();
		explicit operator System::Object();
	};
}

namespace UnityEngine
{
	struct QueryTriggerInteraction
	{
		int32_t Value;
		static const UnityEngine::QueryTriggerInteraction UseGlobal;
		static const UnityEngine::QueryTriggerInteraction Ignore;
		static const UnityEngine::QueryTriggerInteraction Collide;
		explicit QueryTriggerInteraction(int32_t value);
		explicit operator int32_t() const;
		bool operator==(QueryTriggerInteraction other);
		bool operator!=(QueryTriggerInteraction other);
		explicit operator System::Enum();
		explicit operator System::ValueType();
		explicit operator System::Object();
		explicit operator System::IFormattable();
		explicit operator System::IConvertible();
		explicit operator System::IComparable();
	};
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			template<> struct KeyValuePair<System::String, System::Double> : Plugin::ManagedType
			{
				KeyValuePair<System::String, System::Double>(decltype(nullptr));
				KeyValuePair<System::String, System::Double>(Plugin::InternalUse, int32_t handle);
				KeyValuePair<System::String, System::Double>(const KeyValuePair<System::String, System::Double>& other);
				KeyValuePair<System::String, System::Double>(KeyValuePair<System::String, System::Double>&& other);
				virtual ~KeyValuePair<System::String, System::Double>();
				KeyValuePair<System::String, System::Double>& operator=(const KeyValuePair<System::String, System::Double>& other);
				KeyValuePair<System::String, System::Double>& operator=(decltype(nullptr));
				KeyValuePair<System::String, System::Double>& operator=(KeyValuePair<System::String, System::Double>&& other);
				bool operator==(const KeyValuePair<System::String, System::Double>& other) const;
				bool operator!=(const KeyValuePair<System::String, System::Double>& other) const;
				KeyValuePair(System::String& key, System::Double value);
				System::String GetKey();
				System::Double GetValue();
				explicit operator System::ValueType();
				explicit operator System::Object();
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
				LinkedListNode<System::String>(decltype(nullptr));
				LinkedListNode<System::String>(Plugin::InternalUse, int32_t handle);
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
				StrongBox<System::String>(Plugin::InternalUse, int32_t handle);
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
	struct Exception : virtual System::Runtime::InteropServices::_Exception, virtual System::Runtime::Serialization::ISerializable
	{
		Exception(decltype(nullptr));
		Exception(Plugin::InternalUse, int32_t handle);
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
		SystemException(Plugin::InternalUse, int32_t handle);
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
		NullReferenceException(Plugin::InternalUse, int32_t handle);
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
		Screen(Plugin::InternalUse, int32_t handle);
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
	struct Ray : Plugin::ManagedType
	{
		Ray(decltype(nullptr));
		Ray(Plugin::InternalUse, int32_t handle);
		Ray(const Ray& other);
		Ray(Ray&& other);
		virtual ~Ray();
		Ray& operator=(const Ray& other);
		Ray& operator=(decltype(nullptr));
		Ray& operator=(Ray&& other);
		bool operator==(const Ray& other) const;
		bool operator!=(const Ray& other) const;
		Ray(UnityEngine::Vector3& origin, UnityEngine::Vector3& direction);
		explicit operator System::ValueType();
		explicit operator System::Object();
	};
}

namespace UnityEngine
{
	struct Physics : virtual System::Object
	{
		Physics(decltype(nullptr));
		Physics(Plugin::InternalUse, int32_t handle);
		Physics(const Physics& other);
		Physics(Physics&& other);
		virtual ~Physics();
		Physics& operator=(const Physics& other);
		Physics& operator=(decltype(nullptr));
		Physics& operator=(Physics&& other);
		bool operator==(const Physics& other) const;
		bool operator!=(const Physics& other) const;
		static System::Int32 RaycastNonAlloc(UnityEngine::Ray& ray, System::Array1<UnityEngine::RaycastHit>& results);
		static System::Array1<UnityEngine::RaycastHit> RaycastAll(UnityEngine::Ray& ray);
	};
}

namespace UnityEngine
{
	struct Gradient : virtual System::Object
	{
		Gradient(decltype(nullptr));
		Gradient(Plugin::InternalUse, int32_t handle);
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
		AppDomainSetup(Plugin::InternalUse, int32_t handle);
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
		Application(Plugin::InternalUse, int32_t handle);
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
			SceneManager(Plugin::InternalUse, int32_t handle);
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
		struct Scene : Plugin::ManagedType
		{
			Scene(decltype(nullptr));
			Scene(Plugin::InternalUse, int32_t handle);
			Scene(const Scene& other);
			Scene(Scene&& other);
			virtual ~Scene();
			Scene& operator=(const Scene& other);
			Scene& operator=(decltype(nullptr));
			Scene& operator=(Scene&& other);
			bool operator==(const Scene& other) const;
			bool operator!=(const Scene& other) const;
			explicit operator System::ValueType();
			explicit operator System::Object();
		};
	}
}

namespace UnityEngine
{
	namespace SceneManagement
	{
		struct LoadSceneMode
		{
			int32_t Value;
			static const UnityEngine::SceneManagement::LoadSceneMode Single;
			static const UnityEngine::SceneManagement::LoadSceneMode Additive;
			explicit LoadSceneMode(int32_t value);
			explicit operator int32_t() const;
			bool operator==(LoadSceneMode other);
			bool operator!=(LoadSceneMode other);
			explicit operator System::Enum();
			explicit operator System::ValueType();
			explicit operator System::Object();
			explicit operator System::IFormattable();
			explicit operator System::IConvertible();
			explicit operator System::IComparable();
		};
	}
}

namespace System
{
	struct EventArgs : virtual System::Object
	{
		EventArgs(decltype(nullptr));
		EventArgs(Plugin::InternalUse, int32_t handle);
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
				ComponentEventArgs(Plugin::InternalUse, int32_t handle);
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
				ComponentChangingEventArgs(Plugin::InternalUse, int32_t handle);
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
				ComponentChangedEventArgs(Plugin::InternalUse, int32_t handle);
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
				ComponentRenameEventArgs(Plugin::InternalUse, int32_t handle);
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
			MemberDescriptor(Plugin::InternalUse, int32_t handle);
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
	struct PrimitiveType
	{
		int32_t Value;
		static const UnityEngine::PrimitiveType Sphere;
		static const UnityEngine::PrimitiveType Capsule;
		static const UnityEngine::PrimitiveType Cylinder;
		static const UnityEngine::PrimitiveType Cube;
		static const UnityEngine::PrimitiveType Plane;
		static const UnityEngine::PrimitiveType Quad;
		explicit PrimitiveType(int32_t value);
		explicit operator int32_t() const;
		bool operator==(PrimitiveType other);
		bool operator!=(PrimitiveType other);
		explicit operator System::Enum();
		explicit operator System::ValueType();
		explicit operator System::Object();
		explicit operator System::IFormattable();
		explicit operator System::IConvertible();
		explicit operator System::IComparable();
	};
}

namespace UnityEngine
{
	struct Time : virtual System::Object
	{
		Time(decltype(nullptr));
		Time(Plugin::InternalUse, int32_t handle);
		Time(const Time& other);
		Time(Time&& other);
		virtual ~Time();
		Time& operator=(const Time& other);
		Time& operator=(decltype(nullptr));
		Time& operator=(Time&& other);
		bool operator==(const Time& other) const;
		bool operator!=(const Time& other) const;
		static System::Single GetDeltaTime();
	};
}

namespace System
{
	namespace IO
	{
		struct FileMode
		{
			int32_t Value;
			static const System::IO::FileMode CreateNew;
			static const System::IO::FileMode Create;
			static const System::IO::FileMode Open;
			static const System::IO::FileMode OpenOrCreate;
			static const System::IO::FileMode Truncate;
			static const System::IO::FileMode Append;
			explicit FileMode(int32_t value);
			explicit operator int32_t() const;
			bool operator==(FileMode other);
			bool operator!=(FileMode other);
			explicit operator System::Enum();
			explicit operator System::ValueType();
			explicit operator System::Object();
			explicit operator System::IFormattable();
			explicit operator System::IConvertible();
			explicit operator System::IComparable();
		};
	}
}

namespace System
{
	struct MarshalByRefObject : virtual System::Object
	{
		MarshalByRefObject(decltype(nullptr));
		MarshalByRefObject(Plugin::InternalUse, int32_t handle);
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
			Stream(Plugin::InternalUse, int32_t handle);
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
			template<> struct IComparer<System::Int32> : virtual System::Object
			{
				IComparer<System::Int32>(decltype(nullptr));
				IComparer<System::Int32>(Plugin::InternalUse, int32_t handle);
				IComparer<System::Int32>(const IComparer<System::Int32>& other);
				IComparer<System::Int32>(IComparer<System::Int32>&& other);
				virtual ~IComparer<System::Int32>();
				IComparer<System::Int32>& operator=(const IComparer<System::Int32>& other);
				IComparer<System::Int32>& operator=(decltype(nullptr));
				IComparer<System::Int32>& operator=(IComparer<System::Int32>&& other);
				bool operator==(const IComparer<System::Int32>& other) const;
				bool operator!=(const IComparer<System::Int32>& other) const;
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
				IComparer<System::String>(Plugin::InternalUse, int32_t handle);
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
			template<> struct BaseIComparer<System::Int32> : virtual System::Collections::Generic::IComparer<System::Int32>
			{
				BaseIComparer<System::Int32>(decltype(nullptr));
				BaseIComparer<System::Int32>(Plugin::InternalUse, int32_t handle);
				BaseIComparer<System::Int32>(const BaseIComparer<System::Int32>& other);
				BaseIComparer<System::Int32>(BaseIComparer<System::Int32>&& other);
				virtual ~BaseIComparer<System::Int32>();
				BaseIComparer<System::Int32>& operator=(const BaseIComparer<System::Int32>& other);
				BaseIComparer<System::Int32>& operator=(decltype(nullptr));
				BaseIComparer<System::Int32>& operator=(BaseIComparer<System::Int32>&& other);
				bool operator==(const BaseIComparer<System::Int32>& other) const;
				bool operator!=(const BaseIComparer<System::Int32>& other) const;
				int32_t CppHandle;
				BaseIComparer();
				virtual System::Int32 Compare(System::Int32 x, System::Int32 y);
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
				BaseIComparer<System::String>(Plugin::InternalUse, int32_t handle);
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
				virtual System::Int32 Compare(System::String& x, System::String& y);
			};
		}
	}
}

namespace System
{
	struct StringComparer : virtual System::Collections::IComparer, virtual System::Collections::Generic::IComparer<System::String>, virtual System::Collections::IEqualityComparer, virtual System::Collections::Generic::IEqualityComparer<System::String>
	{
		StringComparer(decltype(nullptr));
		StringComparer(Plugin::InternalUse, int32_t handle);
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
		BaseStringComparer(Plugin::InternalUse, int32_t handle);
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
		virtual System::Int32 Compare(System::String& x, System::String& y);
		virtual System::Boolean Equals(System::String& x, System::String& y);
		virtual System::Int32 GetHashCode(System::String& obj);
	};
}

namespace System
{
	namespace Collections
	{
		struct Queue : virtual System::ICloneable, virtual System::Collections::ICollection
		{
			Queue(decltype(nullptr));
			Queue(Plugin::InternalUse, int32_t handle);
			Queue(const Queue& other);
			Queue(Queue&& other);
			virtual ~Queue();
			Queue& operator=(const Queue& other);
			Queue& operator=(decltype(nullptr));
			Queue& operator=(Queue&& other);
			bool operator==(const Queue& other) const;
			bool operator!=(const Queue& other) const;
			System::Int32 GetCount();
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
			BaseQueue(Plugin::InternalUse, int32_t handle);
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
			virtual System::Int32 GetCount();
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
				IComponentChangeService(Plugin::InternalUse, int32_t handle);
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
				BaseIComponentChangeService(Plugin::InternalUse, int32_t handle);
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
			FileStream(Plugin::InternalUse, int32_t handle);
			FileStream(const FileStream& other);
			FileStream(FileStream&& other);
			virtual ~FileStream();
			FileStream& operator=(const FileStream& other);
			FileStream& operator=(decltype(nullptr));
			FileStream& operator=(FileStream&& other);
			bool operator==(const FileStream& other) const;
			bool operator!=(const FileStream& other) const;
			FileStream(System::String& path, System::IO::FileMode mode);
			void WriteByte(System::Byte value);
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
			BaseFileStream(Plugin::InternalUse, int32_t handle);
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
			virtual void WriteByte(System::Byte value);
		};
	}
}

namespace UnityEngine
{
	namespace Playables
	{
		struct PlayableHandle : Plugin::ManagedType
		{
			PlayableHandle(decltype(nullptr));
			PlayableHandle(Plugin::InternalUse, int32_t handle);
			PlayableHandle(const PlayableHandle& other);
			PlayableHandle(PlayableHandle&& other);
			virtual ~PlayableHandle();
			PlayableHandle& operator=(const PlayableHandle& other);
			PlayableHandle& operator=(decltype(nullptr));
			PlayableHandle& operator=(PlayableHandle&& other);
			bool operator==(const PlayableHandle& other) const;
			bool operator!=(const PlayableHandle& other) const;
			explicit operator System::ValueType();
			explicit operator System::Object();
		};
	}
}

namespace UnityEngine
{
	namespace Experimental
	{
		namespace UIElements
		{
			struct ITransform : virtual System::Object
			{
				ITransform(decltype(nullptr));
				ITransform(Plugin::InternalUse, int32_t handle);
				ITransform(const ITransform& other);
				ITransform(ITransform&& other);
				virtual ~ITransform();
				ITransform& operator=(const ITransform& other);
				ITransform& operator=(decltype(nullptr));
				ITransform& operator=(ITransform&& other);
				bool operator==(const ITransform& other) const;
				bool operator!=(const ITransform& other) const;
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
			struct IUIElementDataWatch : virtual System::Object
			{
				IUIElementDataWatch(decltype(nullptr));
				IUIElementDataWatch(Plugin::InternalUse, int32_t handle);
				IUIElementDataWatch(const IUIElementDataWatch& other);
				IUIElementDataWatch(IUIElementDataWatch&& other);
				virtual ~IUIElementDataWatch();
				IUIElementDataWatch& operator=(const IUIElementDataWatch& other);
				IUIElementDataWatch& operator=(decltype(nullptr));
				IUIElementDataWatch& operator=(IUIElementDataWatch&& other);
				bool operator==(const IUIElementDataWatch& other) const;
				bool operator!=(const IUIElementDataWatch& other) const;
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
			struct IVisualElementScheduler : virtual System::Object
			{
				IVisualElementScheduler(decltype(nullptr));
				IVisualElementScheduler(Plugin::InternalUse, int32_t handle);
				IVisualElementScheduler(const IVisualElementScheduler& other);
				IVisualElementScheduler(IVisualElementScheduler&& other);
				virtual ~IVisualElementScheduler();
				IVisualElementScheduler& operator=(const IVisualElementScheduler& other);
				IVisualElementScheduler& operator=(decltype(nullptr));
				IVisualElementScheduler& operator=(IVisualElementScheduler&& other);
				bool operator==(const IVisualElementScheduler& other) const;
				bool operator!=(const IVisualElementScheduler& other) const;
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
			template<> struct IEnumerator<UnityEngine::Experimental::UIElements::VisualElement> : virtual System::IDisposable, virtual System::Collections::IEnumerator
			{
				IEnumerator<UnityEngine::Experimental::UIElements::VisualElement>(decltype(nullptr));
				IEnumerator<UnityEngine::Experimental::UIElements::VisualElement>(Plugin::InternalUse, int32_t handle);
				IEnumerator<UnityEngine::Experimental::UIElements::VisualElement>(const IEnumerator<UnityEngine::Experimental::UIElements::VisualElement>& other);
				IEnumerator<UnityEngine::Experimental::UIElements::VisualElement>(IEnumerator<UnityEngine::Experimental::UIElements::VisualElement>&& other);
				virtual ~IEnumerator<UnityEngine::Experimental::UIElements::VisualElement>();
				IEnumerator<UnityEngine::Experimental::UIElements::VisualElement>& operator=(const IEnumerator<UnityEngine::Experimental::UIElements::VisualElement>& other);
				IEnumerator<UnityEngine::Experimental::UIElements::VisualElement>& operator=(decltype(nullptr));
				IEnumerator<UnityEngine::Experimental::UIElements::VisualElement>& operator=(IEnumerator<UnityEngine::Experimental::UIElements::VisualElement>&& other);
				bool operator==(const IEnumerator<UnityEngine::Experimental::UIElements::VisualElement>& other) const;
				bool operator!=(const IEnumerator<UnityEngine::Experimental::UIElements::VisualElement>& other) const;
				UnityEngine::Experimental::UIElements::VisualElement GetCurrent();
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
			template<> struct IEnumerable<UnityEngine::Experimental::UIElements::VisualElement> : virtual System::Collections::IEnumerable
			{
				IEnumerable<UnityEngine::Experimental::UIElements::VisualElement>(decltype(nullptr));
				IEnumerable<UnityEngine::Experimental::UIElements::VisualElement>(Plugin::InternalUse, int32_t handle);
				IEnumerable<UnityEngine::Experimental::UIElements::VisualElement>(const IEnumerable<UnityEngine::Experimental::UIElements::VisualElement>& other);
				IEnumerable<UnityEngine::Experimental::UIElements::VisualElement>(IEnumerable<UnityEngine::Experimental::UIElements::VisualElement>&& other);
				virtual ~IEnumerable<UnityEngine::Experimental::UIElements::VisualElement>();
				IEnumerable<UnityEngine::Experimental::UIElements::VisualElement>& operator=(const IEnumerable<UnityEngine::Experimental::UIElements::VisualElement>& other);
				IEnumerable<UnityEngine::Experimental::UIElements::VisualElement>& operator=(decltype(nullptr));
				IEnumerable<UnityEngine::Experimental::UIElements::VisualElement>& operator=(IEnumerable<UnityEngine::Experimental::UIElements::VisualElement>&& other);
				bool operator==(const IEnumerable<UnityEngine::Experimental::UIElements::VisualElement>& other) const;
				bool operator!=(const IEnumerable<UnityEngine::Experimental::UIElements::VisualElement>& other) const;
				System::Collections::Generic::IEnumerator<UnityEngine::Experimental::UIElements::VisualElement> GetEnumerator();
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
			struct VisualElement : virtual UnityEngine::Experimental::UIElements::Focusable, virtual System::Collections::Generic::IEnumerable<UnityEngine::Experimental::UIElements::VisualElement>, virtual UnityEngine::Experimental::UIElements::IEventHandler, virtual UnityEngine::Experimental::UIElements::IStyle, virtual UnityEngine::Experimental::UIElements::ITransform, virtual UnityEngine::Experimental::UIElements::IUIElementDataWatch, virtual UnityEngine::Experimental::UIElements::IVisualElementScheduler
			{
				VisualElement(decltype(nullptr));
				VisualElement(Plugin::InternalUse, int32_t handle);
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

namespace Plugin
{
	struct UnityEngineExperimentalUIElementsVisualElementIterator
	{
		System::Collections::Generic::IEnumerator<UnityEngine::Experimental::UIElements::VisualElement> enumerator;
		bool hasMore;
		UnityEngineExperimentalUIElementsVisualElementIterator(decltype(nullptr));
		UnityEngineExperimentalUIElementsVisualElementIterator(UnityEngine::Experimental::UIElements::VisualElement& enumerable);
		~UnityEngineExperimentalUIElementsVisualElementIterator();
		UnityEngineExperimentalUIElementsVisualElementIterator& operator++();
		bool operator!=(const UnityEngineExperimentalUIElementsVisualElementIterator& other);
		UnityEngine::Experimental::UIElements::VisualElement operator*();
	};
}

namespace UnityEngine
{
	namespace Experimental
	{
		namespace UIElements
		{
			Plugin::UnityEngineExperimentalUIElementsVisualElementIterator begin(UnityEngine::Experimental::UIElements::VisualElement& enumerable);
			Plugin::UnityEngineExperimentalUIElementsVisualElementIterator end(UnityEngine::Experimental::UIElements::VisualElement& enumerable);
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
				struct InteractionSourcePositionAccuracy
				{
					int32_t Value;
					static const UnityEngine::XR::WSA::Input::InteractionSourcePositionAccuracy None;
					static const UnityEngine::XR::WSA::Input::InteractionSourcePositionAccuracy Approximate;
					static const UnityEngine::XR::WSA::Input::InteractionSourcePositionAccuracy High;
					explicit InteractionSourcePositionAccuracy(int32_t value);
					explicit operator int32_t() const;
					bool operator==(InteractionSourcePositionAccuracy other);
					bool operator!=(InteractionSourcePositionAccuracy other);
					explicit operator System::Enum();
					explicit operator System::ValueType();
					explicit operator System::Object();
					explicit operator System::IFormattable();
					explicit operator System::IConvertible();
					explicit operator System::IComparable();
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
				struct InteractionSourceNode
				{
					int32_t Value;
					static const UnityEngine::XR::WSA::Input::InteractionSourceNode Grip;
					static const UnityEngine::XR::WSA::Input::InteractionSourceNode Pointer;
					explicit InteractionSourceNode(int32_t value);
					explicit operator int32_t() const;
					bool operator==(InteractionSourceNode other);
					bool operator!=(InteractionSourceNode other);
					explicit operator System::Enum();
					explicit operator System::ValueType();
					explicit operator System::Object();
					explicit operator System::IFormattable();
					explicit operator System::IConvertible();
					explicit operator System::IComparable();
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
				struct InteractionSourcePose : Plugin::ManagedType
				{
					InteractionSourcePose(decltype(nullptr));
					InteractionSourcePose(Plugin::InternalUse, int32_t handle);
					InteractionSourcePose(const InteractionSourcePose& other);
					InteractionSourcePose(InteractionSourcePose&& other);
					virtual ~InteractionSourcePose();
					InteractionSourcePose& operator=(const InteractionSourcePose& other);
					InteractionSourcePose& operator=(decltype(nullptr));
					InteractionSourcePose& operator=(InteractionSourcePose&& other);
					bool operator==(const InteractionSourcePose& other) const;
					bool operator!=(const InteractionSourcePose& other) const;
					System::Boolean TryGetRotation(UnityEngine::Quaternion* rotation, UnityEngine::XR::WSA::Input::InteractionSourceNode node = UnityEngine::XR::WSA::Input::InteractionSourceNode::Grip);
					explicit operator System::ValueType();
					explicit operator System::Object();
				};
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
			template<> struct IEnumerator<System::String> : virtual System::IDisposable, virtual System::Collections::IEnumerator
			{
				IEnumerator<System::String>(decltype(nullptr));
				IEnumerator<System::String>(Plugin::InternalUse, int32_t handle);
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
			template<> struct IEnumerator<System::Int32> : virtual System::IDisposable, virtual System::Collections::IEnumerator
			{
				IEnumerator<System::Int32>(decltype(nullptr));
				IEnumerator<System::Int32>(Plugin::InternalUse, int32_t handle);
				IEnumerator<System::Int32>(const IEnumerator<System::Int32>& other);
				IEnumerator<System::Int32>(IEnumerator<System::Int32>&& other);
				virtual ~IEnumerator<System::Int32>();
				IEnumerator<System::Int32>& operator=(const IEnumerator<System::Int32>& other);
				IEnumerator<System::Int32>& operator=(decltype(nullptr));
				IEnumerator<System::Int32>& operator=(IEnumerator<System::Int32>&& other);
				bool operator==(const IEnumerator<System::Int32>& other) const;
				bool operator!=(const IEnumerator<System::Int32>& other) const;
				System::Int32 GetCurrent();
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
			template<> struct IEnumerator<System::Single> : virtual System::IDisposable, virtual System::Collections::IEnumerator
			{
				IEnumerator<System::Single>(decltype(nullptr));
				IEnumerator<System::Single>(Plugin::InternalUse, int32_t handle);
				IEnumerator<System::Single>(const IEnumerator<System::Single>& other);
				IEnumerator<System::Single>(IEnumerator<System::Single>&& other);
				virtual ~IEnumerator<System::Single>();
				IEnumerator<System::Single>& operator=(const IEnumerator<System::Single>& other);
				IEnumerator<System::Single>& operator=(decltype(nullptr));
				IEnumerator<System::Single>& operator=(IEnumerator<System::Single>&& other);
				bool operator==(const IEnumerator<System::Single>& other) const;
				bool operator!=(const IEnumerator<System::Single>& other) const;
				System::Single GetCurrent();
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
				IEnumerator<UnityEngine::RaycastHit>(Plugin::InternalUse, int32_t handle);
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
				IEnumerator<UnityEngine::GradientColorKey>(Plugin::InternalUse, int32_t handle);
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
				IEnumerator<UnityEngine::Resolution>(Plugin::InternalUse, int32_t handle);
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
				IEnumerable<System::String>(Plugin::InternalUse, int32_t handle);
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
			template<> struct IEnumerable<System::Int32> : virtual System::Collections::IEnumerable
			{
				IEnumerable<System::Int32>(decltype(nullptr));
				IEnumerable<System::Int32>(Plugin::InternalUse, int32_t handle);
				IEnumerable<System::Int32>(const IEnumerable<System::Int32>& other);
				IEnumerable<System::Int32>(IEnumerable<System::Int32>&& other);
				virtual ~IEnumerable<System::Int32>();
				IEnumerable<System::Int32>& operator=(const IEnumerable<System::Int32>& other);
				IEnumerable<System::Int32>& operator=(decltype(nullptr));
				IEnumerable<System::Int32>& operator=(IEnumerable<System::Int32>&& other);
				bool operator==(const IEnumerable<System::Int32>& other) const;
				bool operator!=(const IEnumerable<System::Int32>& other) const;
				System::Collections::Generic::IEnumerator<System::Int32> GetEnumerator();
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
			template<> struct IEnumerable<System::Single> : virtual System::Collections::IEnumerable
			{
				IEnumerable<System::Single>(decltype(nullptr));
				IEnumerable<System::Single>(Plugin::InternalUse, int32_t handle);
				IEnumerable<System::Single>(const IEnumerable<System::Single>& other);
				IEnumerable<System::Single>(IEnumerable<System::Single>&& other);
				virtual ~IEnumerable<System::Single>();
				IEnumerable<System::Single>& operator=(const IEnumerable<System::Single>& other);
				IEnumerable<System::Single>& operator=(decltype(nullptr));
				IEnumerable<System::Single>& operator=(IEnumerable<System::Single>&& other);
				bool operator==(const IEnumerable<System::Single>& other) const;
				bool operator!=(const IEnumerable<System::Single>& other) const;
				System::Collections::Generic::IEnumerator<System::Single> GetEnumerator();
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
				IEnumerable<UnityEngine::RaycastHit>(Plugin::InternalUse, int32_t handle);
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
				IEnumerable<UnityEngine::GradientColorKey>(Plugin::InternalUse, int32_t handle);
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
				IEnumerable<UnityEngine::Resolution>(Plugin::InternalUse, int32_t handle);
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
				ICollection<System::String>(Plugin::InternalUse, int32_t handle);
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
			template<> struct ICollection<System::Int32> : virtual System::Collections::Generic::IEnumerable<System::Int32>
			{
				ICollection<System::Int32>(decltype(nullptr));
				ICollection<System::Int32>(Plugin::InternalUse, int32_t handle);
				ICollection<System::Int32>(const ICollection<System::Int32>& other);
				ICollection<System::Int32>(ICollection<System::Int32>&& other);
				virtual ~ICollection<System::Int32>();
				ICollection<System::Int32>& operator=(const ICollection<System::Int32>& other);
				ICollection<System::Int32>& operator=(decltype(nullptr));
				ICollection<System::Int32>& operator=(ICollection<System::Int32>&& other);
				bool operator==(const ICollection<System::Int32>& other) const;
				bool operator!=(const ICollection<System::Int32>& other) const;
			};
		}
	}
}

namespace Plugin
{
	struct SystemCollectionsGenericICollectionSystemInt32Iterator
	{
		System::Collections::Generic::IEnumerator<System::Int32> enumerator;
		bool hasMore;
		SystemCollectionsGenericICollectionSystemInt32Iterator(decltype(nullptr));
		SystemCollectionsGenericICollectionSystemInt32Iterator(System::Collections::Generic::ICollection<System::Int32>& enumerable);
		~SystemCollectionsGenericICollectionSystemInt32Iterator();
		SystemCollectionsGenericICollectionSystemInt32Iterator& operator++();
		bool operator!=(const SystemCollectionsGenericICollectionSystemInt32Iterator& other);
		System::Int32 operator*();
	};
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			Plugin::SystemCollectionsGenericICollectionSystemInt32Iterator begin(System::Collections::Generic::ICollection<System::Int32>& enumerable);
			Plugin::SystemCollectionsGenericICollectionSystemInt32Iterator end(System::Collections::Generic::ICollection<System::Int32>& enumerable);
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			template<> struct ICollection<System::Single> : virtual System::Collections::Generic::IEnumerable<System::Single>
			{
				ICollection<System::Single>(decltype(nullptr));
				ICollection<System::Single>(Plugin::InternalUse, int32_t handle);
				ICollection<System::Single>(const ICollection<System::Single>& other);
				ICollection<System::Single>(ICollection<System::Single>&& other);
				virtual ~ICollection<System::Single>();
				ICollection<System::Single>& operator=(const ICollection<System::Single>& other);
				ICollection<System::Single>& operator=(decltype(nullptr));
				ICollection<System::Single>& operator=(ICollection<System::Single>&& other);
				bool operator==(const ICollection<System::Single>& other) const;
				bool operator!=(const ICollection<System::Single>& other) const;
			};
		}
	}
}

namespace Plugin
{
	struct SystemCollectionsGenericICollectionSystemSingleIterator
	{
		System::Collections::Generic::IEnumerator<System::Single> enumerator;
		bool hasMore;
		SystemCollectionsGenericICollectionSystemSingleIterator(decltype(nullptr));
		SystemCollectionsGenericICollectionSystemSingleIterator(System::Collections::Generic::ICollection<System::Single>& enumerable);
		~SystemCollectionsGenericICollectionSystemSingleIterator();
		SystemCollectionsGenericICollectionSystemSingleIterator& operator++();
		bool operator!=(const SystemCollectionsGenericICollectionSystemSingleIterator& other);
		System::Single operator*();
	};
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			Plugin::SystemCollectionsGenericICollectionSystemSingleIterator begin(System::Collections::Generic::ICollection<System::Single>& enumerable);
			Plugin::SystemCollectionsGenericICollectionSystemSingleIterator end(System::Collections::Generic::ICollection<System::Single>& enumerable);
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
				ICollection<UnityEngine::RaycastHit>(Plugin::InternalUse, int32_t handle);
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
				ICollection<UnityEngine::GradientColorKey>(Plugin::InternalUse, int32_t handle);
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
				ICollection<UnityEngine::Resolution>(Plugin::InternalUse, int32_t handle);
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
				IList<System::String>(Plugin::InternalUse, int32_t handle);
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
			template<> struct IList<System::Int32> : virtual System::Collections::Generic::ICollection<System::Int32>
			{
				IList<System::Int32>(decltype(nullptr));
				IList<System::Int32>(Plugin::InternalUse, int32_t handle);
				IList<System::Int32>(const IList<System::Int32>& other);
				IList<System::Int32>(IList<System::Int32>&& other);
				virtual ~IList<System::Int32>();
				IList<System::Int32>& operator=(const IList<System::Int32>& other);
				IList<System::Int32>& operator=(decltype(nullptr));
				IList<System::Int32>& operator=(IList<System::Int32>&& other);
				bool operator==(const IList<System::Int32>& other) const;
				bool operator!=(const IList<System::Int32>& other) const;
			};
		}
	}
}

namespace Plugin
{
	struct SystemCollectionsGenericIListSystemInt32Iterator
	{
		System::Collections::Generic::IEnumerator<System::Int32> enumerator;
		bool hasMore;
		SystemCollectionsGenericIListSystemInt32Iterator(decltype(nullptr));
		SystemCollectionsGenericIListSystemInt32Iterator(System::Collections::Generic::IList<System::Int32>& enumerable);
		~SystemCollectionsGenericIListSystemInt32Iterator();
		SystemCollectionsGenericIListSystemInt32Iterator& operator++();
		bool operator!=(const SystemCollectionsGenericIListSystemInt32Iterator& other);
		System::Int32 operator*();
	};
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			Plugin::SystemCollectionsGenericIListSystemInt32Iterator begin(System::Collections::Generic::IList<System::Int32>& enumerable);
			Plugin::SystemCollectionsGenericIListSystemInt32Iterator end(System::Collections::Generic::IList<System::Int32>& enumerable);
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			template<> struct IList<System::Single> : virtual System::Collections::Generic::ICollection<System::Single>
			{
				IList<System::Single>(decltype(nullptr));
				IList<System::Single>(Plugin::InternalUse, int32_t handle);
				IList<System::Single>(const IList<System::Single>& other);
				IList<System::Single>(IList<System::Single>&& other);
				virtual ~IList<System::Single>();
				IList<System::Single>& operator=(const IList<System::Single>& other);
				IList<System::Single>& operator=(decltype(nullptr));
				IList<System::Single>& operator=(IList<System::Single>&& other);
				bool operator==(const IList<System::Single>& other) const;
				bool operator!=(const IList<System::Single>& other) const;
			};
		}
	}
}

namespace Plugin
{
	struct SystemCollectionsGenericIListSystemSingleIterator
	{
		System::Collections::Generic::IEnumerator<System::Single> enumerator;
		bool hasMore;
		SystemCollectionsGenericIListSystemSingleIterator(decltype(nullptr));
		SystemCollectionsGenericIListSystemSingleIterator(System::Collections::Generic::IList<System::Single>& enumerable);
		~SystemCollectionsGenericIListSystemSingleIterator();
		SystemCollectionsGenericIListSystemSingleIterator& operator++();
		bool operator!=(const SystemCollectionsGenericIListSystemSingleIterator& other);
		System::Single operator*();
	};
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			Plugin::SystemCollectionsGenericIListSystemSingleIterator begin(System::Collections::Generic::IList<System::Single>& enumerable);
			Plugin::SystemCollectionsGenericIListSystemSingleIterator end(System::Collections::Generic::IList<System::Single>& enumerable);
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
				IList<UnityEngine::RaycastHit>(Plugin::InternalUse, int32_t handle);
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
				IList<UnityEngine::GradientColorKey>(Plugin::InternalUse, int32_t handle);
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
				IList<UnityEngine::Resolution>(Plugin::InternalUse, int32_t handle);
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
	namespace Collections
	{
		namespace Generic
		{
			template<> struct List<System::String> : virtual System::Collections::IList, virtual System::Collections::Generic::IList<System::String>
			{
				List<System::String>(decltype(nullptr));
				List<System::String>(Plugin::InternalUse, int32_t handle);
				List<System::String>(const List<System::String>& other);
				List<System::String>(List<System::String>&& other);
				virtual ~List<System::String>();
				List<System::String>& operator=(const List<System::String>& other);
				List<System::String>& operator=(decltype(nullptr));
				List<System::String>& operator=(List<System::String>&& other);
				bool operator==(const List<System::String>& other) const;
				bool operator!=(const List<System::String>& other) const;
				List();
				System::String GetItem(System::Int32 index);
				void SetItem(System::Int32 index, System::String& value);
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
			template<> struct List<System::Int32> : virtual System::Collections::IList, virtual System::Collections::Generic::IList<System::Int32>
			{
				List<System::Int32>(decltype(nullptr));
				List<System::Int32>(Plugin::InternalUse, int32_t handle);
				List<System::Int32>(const List<System::Int32>& other);
				List<System::Int32>(List<System::Int32>&& other);
				virtual ~List<System::Int32>();
				List<System::Int32>& operator=(const List<System::Int32>& other);
				List<System::Int32>& operator=(decltype(nullptr));
				List<System::Int32>& operator=(List<System::Int32>&& other);
				bool operator==(const List<System::Int32>& other) const;
				bool operator!=(const List<System::Int32>& other) const;
				List();
				System::Int32 GetItem(System::Int32 index);
				void SetItem(System::Int32 index, System::Int32 value);
				void Add(System::Int32 item);
				void Sort(System::Collections::Generic::IComparer<System::Int32>& comparer);
			};
		}
	}
}

namespace Plugin
{
	struct SystemCollectionsGenericListSystemInt32Iterator
	{
		System::Collections::Generic::IEnumerator<System::Int32> enumerator;
		bool hasMore;
		SystemCollectionsGenericListSystemInt32Iterator(decltype(nullptr));
		SystemCollectionsGenericListSystemInt32Iterator(System::Collections::Generic::List<System::Int32>& enumerable);
		~SystemCollectionsGenericListSystemInt32Iterator();
		SystemCollectionsGenericListSystemInt32Iterator& operator++();
		bool operator!=(const SystemCollectionsGenericListSystemInt32Iterator& other);
		System::Int32 operator*();
	};
}

namespace System
{
	namespace Collections
	{
		namespace Generic
		{
			Plugin::SystemCollectionsGenericListSystemInt32Iterator begin(System::Collections::Generic::List<System::Int32>& enumerable);
			Plugin::SystemCollectionsGenericListSystemInt32Iterator end(System::Collections::Generic::List<System::Int32>& enumerable);
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace ObjectModel
		{
			template<> struct Collection<System::Int32> : virtual System::Collections::IList, virtual System::Collections::Generic::IList<System::Int32>
			{
				Collection<System::Int32>(decltype(nullptr));
				Collection<System::Int32>(Plugin::InternalUse, int32_t handle);
				Collection<System::Int32>(const Collection<System::Int32>& other);
				Collection<System::Int32>(Collection<System::Int32>&& other);
				virtual ~Collection<System::Int32>();
				Collection<System::Int32>& operator=(const Collection<System::Int32>& other);
				Collection<System::Int32>& operator=(decltype(nullptr));
				Collection<System::Int32>& operator=(Collection<System::Int32>&& other);
				bool operator==(const Collection<System::Int32>& other) const;
				bool operator!=(const Collection<System::Int32>& other) const;
			};
		}
	}
}

namespace Plugin
{
	struct SystemCollectionsObjectModelCollectionSystemInt32Iterator
	{
		System::Collections::Generic::IEnumerator<System::Int32> enumerator;
		bool hasMore;
		SystemCollectionsObjectModelCollectionSystemInt32Iterator(decltype(nullptr));
		SystemCollectionsObjectModelCollectionSystemInt32Iterator(System::Collections::ObjectModel::Collection<System::Int32>& enumerable);
		~SystemCollectionsObjectModelCollectionSystemInt32Iterator();
		SystemCollectionsObjectModelCollectionSystemInt32Iterator& operator++();
		bool operator!=(const SystemCollectionsObjectModelCollectionSystemInt32Iterator& other);
		System::Int32 operator*();
	};
}

namespace System
{
	namespace Collections
	{
		namespace ObjectModel
		{
			Plugin::SystemCollectionsObjectModelCollectionSystemInt32Iterator begin(System::Collections::ObjectModel::Collection<System::Int32>& enumerable);
			Plugin::SystemCollectionsObjectModelCollectionSystemInt32Iterator end(System::Collections::ObjectModel::Collection<System::Int32>& enumerable);
		}
	}
}

namespace System
{
	namespace Collections
	{
		namespace ObjectModel
		{
			template<> struct KeyedCollection<System::String, System::Int32> : virtual System::Collections::ObjectModel::Collection<System::Int32>, virtual System::Collections::IList, virtual System::Collections::Generic::IList<System::Int32>
			{
				KeyedCollection<System::String, System::Int32>(decltype(nullptr));
				KeyedCollection<System::String, System::Int32>(Plugin::InternalUse, int32_t handle);
				KeyedCollection<System::String, System::Int32>(const KeyedCollection<System::String, System::Int32>& other);
				KeyedCollection<System::String, System::Int32>(KeyedCollection<System::String, System::Int32>&& other);
				virtual ~KeyedCollection<System::String, System::Int32>();
				KeyedCollection<System::String, System::Int32>& operator=(const KeyedCollection<System::String, System::Int32>& other);
				KeyedCollection<System::String, System::Int32>& operator=(decltype(nullptr));
				KeyedCollection<System::String, System::Int32>& operator=(KeyedCollection<System::String, System::Int32>&& other);
				bool operator==(const KeyedCollection<System::String, System::Int32>& other) const;
				bool operator!=(const KeyedCollection<System::String, System::Int32>& other) const;
			};
		}
	}
}

namespace Plugin
{
	struct SystemCollectionsObjectModelKeyedCollectionSystemString_SystemInt32Iterator
	{
		System::Collections::Generic::IEnumerator<System::Int32> enumerator;
		bool hasMore;
		SystemCollectionsObjectModelKeyedCollectionSystemString_SystemInt32Iterator(decltype(nullptr));
		SystemCollectionsObjectModelKeyedCollectionSystemString_SystemInt32Iterator(System::Collections::ObjectModel::KeyedCollection<System::String, System::Int32>& enumerable);
		~SystemCollectionsObjectModelKeyedCollectionSystemString_SystemInt32Iterator();
		SystemCollectionsObjectModelKeyedCollectionSystemString_SystemInt32Iterator& operator++();
		bool operator!=(const SystemCollectionsObjectModelKeyedCollectionSystemString_SystemInt32Iterator& other);
		System::Int32 operator*();
	};
}

namespace System
{
	namespace Collections
	{
		namespace ObjectModel
		{
			Plugin::SystemCollectionsObjectModelKeyedCollectionSystemString_SystemInt32Iterator begin(System::Collections::ObjectModel::KeyedCollection<System::String, System::Int32>& enumerable);
			Plugin::SystemCollectionsObjectModelKeyedCollectionSystemString_SystemInt32Iterator end(System::Collections::ObjectModel::KeyedCollection<System::String, System::Int32>& enumerable);
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
			TestScript(Plugin::InternalUse, int32_t handle);
			TestScript(const TestScript& other);
			TestScript(TestScript&& other);
			virtual ~TestScript();
			TestScript& operator=(const TestScript& other);
			TestScript& operator=(decltype(nullptr));
			TestScript& operator=(TestScript&& other);
			bool operator==(const TestScript& other) const;
			bool operator!=(const TestScript& other) const;
			void Awake();
			void OnAnimatorIK(System::Int32 param0);
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
			AnotherScript(Plugin::InternalUse, int32_t handle);
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
	template<> struct ArrayElementProxy1_1<System::Int32>
	{
		int32_t Handle;
		int32_t Index0;
		ArrayElementProxy1_1<System::Int32>(Plugin::InternalUse, int32_t handle, int32_t index0);
		void operator=(System::Int32 item);
		operator System::Int32();
	};
}

namespace System
{
	template<> struct Array1<System::Int32> : virtual System::Array, virtual System::ICloneable, virtual System::Collections::IList, virtual System::Collections::Generic::IList<System::Int32>
	{
		Array1<System::Int32>(decltype(nullptr));
		Array1<System::Int32>(Plugin::InternalUse, int32_t handle);
		Array1<System::Int32>(const Array1<System::Int32>& other);
		Array1<System::Int32>(Array1<System::Int32>&& other);
		virtual ~Array1<System::Int32>();
		Array1<System::Int32>& operator=(const Array1<System::Int32>& other);
		Array1<System::Int32>& operator=(decltype(nullptr));
		Array1<System::Int32>& operator=(Array1<System::Int32>&& other);
		bool operator==(const Array1<System::Int32>& other) const;
		bool operator!=(const Array1<System::Int32>& other) const;
		int32_t InternalLength;
		Array1(System::Int32 length0);
		System::Int32 GetLength();
		System::Int32 GetRank();
		Plugin::ArrayElementProxy1_1<System::Int32> operator[](int32_t index);
	};
}

namespace Plugin
{
	struct SystemInt32Array1Iterator
	{
		System::Array1<System::Int32>& array;
		int index;
		SystemInt32Array1Iterator(System::Array1<System::Int32>& array, int32_t index);
		SystemInt32Array1Iterator& operator++();
		bool operator!=(const SystemInt32Array1Iterator& other);
		System::Int32 operator*();
	};
}

namespace System
{
	Plugin::SystemInt32Array1Iterator begin(System::Array1<System::Int32>& array);
	Plugin::SystemInt32Array1Iterator end(System::Array1<System::Int32>& array);
}

namespace Plugin
{
	template<> struct ArrayElementProxy1_1<System::Single>
	{
		int32_t Handle;
		int32_t Index0;
		ArrayElementProxy1_1<System::Single>(Plugin::InternalUse, int32_t handle, int32_t index0);
		void operator=(System::Single item);
		operator System::Single();
	};
}

namespace Plugin
{
	template<> struct ArrayElementProxy1_2<System::Single>
	{
		int32_t Handle;
		int32_t Index0;
		ArrayElementProxy1_2<System::Single>(Plugin::InternalUse, int32_t handle, int32_t index0);
		Plugin::ArrayElementProxy2_2<System::Single> operator[](int32_t index);
	};
}

namespace Plugin
{
	template<> struct ArrayElementProxy2_2<System::Single>
	{
		int32_t Handle;
		int32_t Index0;
		int32_t Index1;
		ArrayElementProxy2_2<System::Single>(Plugin::InternalUse, int32_t handle, int32_t index0, int32_t index1);
		void operator=(System::Single item);
		operator System::Single();
	};
}

namespace Plugin
{
	template<> struct ArrayElementProxy1_3<System::Single>
	{
		int32_t Handle;
		int32_t Index0;
		ArrayElementProxy1_3<System::Single>(Plugin::InternalUse, int32_t handle, int32_t index0);
		Plugin::ArrayElementProxy2_3<System::Single> operator[](int32_t index);
	};
}

namespace Plugin
{
	template<> struct ArrayElementProxy2_3<System::Single>
	{
		int32_t Handle;
		int32_t Index0;
		int32_t Index1;
		ArrayElementProxy2_3<System::Single>(Plugin::InternalUse, int32_t handle, int32_t index0, int32_t index1);
		Plugin::ArrayElementProxy3_3<System::Single> operator[](int32_t index);
	};
}

namespace Plugin
{
	template<> struct ArrayElementProxy3_3<System::Single>
	{
		int32_t Handle;
		int32_t Index0;
		int32_t Index1;
		int32_t Index2;
		ArrayElementProxy3_3<System::Single>(Plugin::InternalUse, int32_t handle, int32_t index0, int32_t index1, int32_t index2);
		void operator=(System::Single item);
		operator System::Single();
	};
}

namespace System
{
	template<> struct Array1<System::Single> : virtual System::Array, virtual System::ICloneable, virtual System::Collections::IList, virtual System::Collections::Generic::IList<System::Single>
	{
		Array1<System::Single>(decltype(nullptr));
		Array1<System::Single>(Plugin::InternalUse, int32_t handle);
		Array1<System::Single>(const Array1<System::Single>& other);
		Array1<System::Single>(Array1<System::Single>&& other);
		virtual ~Array1<System::Single>();
		Array1<System::Single>& operator=(const Array1<System::Single>& other);
		Array1<System::Single>& operator=(decltype(nullptr));
		Array1<System::Single>& operator=(Array1<System::Single>&& other);
		bool operator==(const Array1<System::Single>& other) const;
		bool operator!=(const Array1<System::Single>& other) const;
		int32_t InternalLength;
		Array1(System::Int32 length0);
		System::Int32 GetLength();
		System::Int32 GetRank();
		Plugin::ArrayElementProxy1_1<System::Single> operator[](int32_t index);
	};
}

namespace Plugin
{
	struct SystemSingleArray1Iterator
	{
		System::Array1<System::Single>& array;
		int index;
		SystemSingleArray1Iterator(System::Array1<System::Single>& array, int32_t index);
		SystemSingleArray1Iterator& operator++();
		bool operator!=(const SystemSingleArray1Iterator& other);
		System::Single operator*();
	};
}

namespace System
{
	Plugin::SystemSingleArray1Iterator begin(System::Array1<System::Single>& array);
	Plugin::SystemSingleArray1Iterator end(System::Array1<System::Single>& array);
}

namespace System
{
	template<> struct Array2<System::Single> : virtual System::Array, virtual System::ICloneable, virtual System::Collections::IList
	{
		Array2<System::Single>(decltype(nullptr));
		Array2<System::Single>(Plugin::InternalUse, int32_t handle);
		Array2<System::Single>(const Array2<System::Single>& other);
		Array2<System::Single>(Array2<System::Single>&& other);
		virtual ~Array2<System::Single>();
		Array2<System::Single>& operator=(const Array2<System::Single>& other);
		Array2<System::Single>& operator=(decltype(nullptr));
		Array2<System::Single>& operator=(Array2<System::Single>&& other);
		bool operator==(const Array2<System::Single>& other) const;
		bool operator!=(const Array2<System::Single>& other) const;
		int32_t InternalLength;
		int32_t InternalLengths[2];
		Array2(System::Int32 length0, System::Int32 length1);
		System::Int32 GetLength();
		System::Int32 GetLength(System::Int32 dimension);
		System::Int32 GetRank();
		Plugin::ArrayElementProxy1_2<System::Single> operator[](int32_t index);
	};
}

namespace System
{
	template<> struct Array3<System::Single> : virtual System::Array, virtual System::ICloneable, virtual System::Collections::IList
	{
		Array3<System::Single>(decltype(nullptr));
		Array3<System::Single>(Plugin::InternalUse, int32_t handle);
		Array3<System::Single>(const Array3<System::Single>& other);
		Array3<System::Single>(Array3<System::Single>&& other);
		virtual ~Array3<System::Single>();
		Array3<System::Single>& operator=(const Array3<System::Single>& other);
		Array3<System::Single>& operator=(decltype(nullptr));
		Array3<System::Single>& operator=(Array3<System::Single>&& other);
		bool operator==(const Array3<System::Single>& other) const;
		bool operator!=(const Array3<System::Single>& other) const;
		int32_t InternalLength;
		int32_t InternalLengths[3];
		Array3(System::Int32 length0, System::Int32 length1, System::Int32 length2);
		System::Int32 GetLength();
		System::Int32 GetLength(System::Int32 dimension);
		System::Int32 GetRank();
		Plugin::ArrayElementProxy1_3<System::Single> operator[](int32_t index);
	};
}

namespace Plugin
{
	template<> struct ArrayElementProxy1_1<System::String>
	{
		int32_t Handle;
		int32_t Index0;
		ArrayElementProxy1_1<System::String>(Plugin::InternalUse, int32_t handle, int32_t index0);
		void operator=(System::String item);
		operator System::String();
	};
}

namespace System
{
	template<> struct Array1<System::String> : virtual System::Array, virtual System::ICloneable, virtual System::Collections::IList, virtual System::Collections::Generic::IList<System::String>
	{
		Array1<System::String>(decltype(nullptr));
		Array1<System::String>(Plugin::InternalUse, int32_t handle);
		Array1<System::String>(const Array1<System::String>& other);
		Array1<System::String>(Array1<System::String>&& other);
		virtual ~Array1<System::String>();
		Array1<System::String>& operator=(const Array1<System::String>& other);
		Array1<System::String>& operator=(decltype(nullptr));
		Array1<System::String>& operator=(Array1<System::String>&& other);
		bool operator==(const Array1<System::String>& other) const;
		bool operator!=(const Array1<System::String>& other) const;
		int32_t InternalLength;
		Array1(System::Int32 length0);
		System::Int32 GetLength();
		System::Int32 GetRank();
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
		ArrayElementProxy1_1<UnityEngine::Resolution>(Plugin::InternalUse, int32_t handle, int32_t index0);
		void operator=(UnityEngine::Resolution item);
		operator UnityEngine::Resolution();
	};
}

namespace System
{
	template<> struct Array1<UnityEngine::Resolution> : virtual System::Array, virtual System::ICloneable, virtual System::Collections::IList, virtual System::Collections::Generic::IList<UnityEngine::Resolution>
	{
		Array1<UnityEngine::Resolution>(decltype(nullptr));
		Array1<UnityEngine::Resolution>(Plugin::InternalUse, int32_t handle);
		Array1<UnityEngine::Resolution>(const Array1<UnityEngine::Resolution>& other);
		Array1<UnityEngine::Resolution>(Array1<UnityEngine::Resolution>&& other);
		virtual ~Array1<UnityEngine::Resolution>();
		Array1<UnityEngine::Resolution>& operator=(const Array1<UnityEngine::Resolution>& other);
		Array1<UnityEngine::Resolution>& operator=(decltype(nullptr));
		Array1<UnityEngine::Resolution>& operator=(Array1<UnityEngine::Resolution>&& other);
		bool operator==(const Array1<UnityEngine::Resolution>& other) const;
		bool operator!=(const Array1<UnityEngine::Resolution>& other) const;
		int32_t InternalLength;
		Array1(System::Int32 length0);
		System::Int32 GetLength();
		System::Int32 GetRank();
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
		ArrayElementProxy1_1<UnityEngine::RaycastHit>(Plugin::InternalUse, int32_t handle, int32_t index0);
		void operator=(UnityEngine::RaycastHit item);
		operator UnityEngine::RaycastHit();
	};
}

namespace System
{
	template<> struct Array1<UnityEngine::RaycastHit> : virtual System::Array, virtual System::ICloneable, virtual System::Collections::IList, virtual System::Collections::Generic::IList<UnityEngine::RaycastHit>
	{
		Array1<UnityEngine::RaycastHit>(decltype(nullptr));
		Array1<UnityEngine::RaycastHit>(Plugin::InternalUse, int32_t handle);
		Array1<UnityEngine::RaycastHit>(const Array1<UnityEngine::RaycastHit>& other);
		Array1<UnityEngine::RaycastHit>(Array1<UnityEngine::RaycastHit>&& other);
		virtual ~Array1<UnityEngine::RaycastHit>();
		Array1<UnityEngine::RaycastHit>& operator=(const Array1<UnityEngine::RaycastHit>& other);
		Array1<UnityEngine::RaycastHit>& operator=(decltype(nullptr));
		Array1<UnityEngine::RaycastHit>& operator=(Array1<UnityEngine::RaycastHit>&& other);
		bool operator==(const Array1<UnityEngine::RaycastHit>& other) const;
		bool operator!=(const Array1<UnityEngine::RaycastHit>& other) const;
		int32_t InternalLength;
		Array1(System::Int32 length0);
		System::Int32 GetLength();
		System::Int32 GetRank();
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
		ArrayElementProxy1_1<UnityEngine::GradientColorKey>(Plugin::InternalUse, int32_t handle, int32_t index0);
		void operator=(UnityEngine::GradientColorKey item);
		operator UnityEngine::GradientColorKey();
	};
}

namespace System
{
	template<> struct Array1<UnityEngine::GradientColorKey> : virtual System::Array, virtual System::ICloneable, virtual System::Collections::IList, virtual System::Collections::Generic::IList<UnityEngine::GradientColorKey>
	{
		Array1<UnityEngine::GradientColorKey>(decltype(nullptr));
		Array1<UnityEngine::GradientColorKey>(Plugin::InternalUse, int32_t handle);
		Array1<UnityEngine::GradientColorKey>(const Array1<UnityEngine::GradientColorKey>& other);
		Array1<UnityEngine::GradientColorKey>(Array1<UnityEngine::GradientColorKey>&& other);
		virtual ~Array1<UnityEngine::GradientColorKey>();
		Array1<UnityEngine::GradientColorKey>& operator=(const Array1<UnityEngine::GradientColorKey>& other);
		Array1<UnityEngine::GradientColorKey>& operator=(decltype(nullptr));
		Array1<UnityEngine::GradientColorKey>& operator=(Array1<UnityEngine::GradientColorKey>&& other);
		bool operator==(const Array1<UnityEngine::GradientColorKey>& other) const;
		bool operator!=(const Array1<UnityEngine::GradientColorKey>& other) const;
		int32_t InternalLength;
		Array1(System::Int32 length0);
		System::Int32 GetLength();
		System::Int32 GetRank();
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
		Action(Plugin::InternalUse, int32_t handle);
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
	template<> struct Action1<System::Single> : virtual System::Object
	{
		Action1<System::Single>(decltype(nullptr));
		Action1<System::Single>(Plugin::InternalUse, int32_t handle);
		Action1<System::Single>(const Action1<System::Single>& other);
		Action1<System::Single>(Action1<System::Single>&& other);
		virtual ~Action1<System::Single>();
		Action1<System::Single>& operator=(const Action1<System::Single>& other);
		Action1<System::Single>& operator=(decltype(nullptr));
		Action1<System::Single>& operator=(Action1<System::Single>&& other);
		bool operator==(const Action1<System::Single>& other) const;
		bool operator!=(const Action1<System::Single>& other) const;
		int32_t CppHandle;
		int32_t ClassHandle;
		Action1();
		void operator+=(System::Action1<System::Single>& del);
		void operator-=(System::Action1<System::Single>& del);
		virtual void operator()(System::Single obj);
		void Invoke(System::Single obj);
	};
}

namespace System
{
	template<> struct Action2<System::Single, System::Single> : virtual System::Object
	{
		Action2<System::Single, System::Single>(decltype(nullptr));
		Action2<System::Single, System::Single>(Plugin::InternalUse, int32_t handle);
		Action2<System::Single, System::Single>(const Action2<System::Single, System::Single>& other);
		Action2<System::Single, System::Single>(Action2<System::Single, System::Single>&& other);
		virtual ~Action2<System::Single, System::Single>();
		Action2<System::Single, System::Single>& operator=(const Action2<System::Single, System::Single>& other);
		Action2<System::Single, System::Single>& operator=(decltype(nullptr));
		Action2<System::Single, System::Single>& operator=(Action2<System::Single, System::Single>&& other);
		bool operator==(const Action2<System::Single, System::Single>& other) const;
		bool operator!=(const Action2<System::Single, System::Single>& other) const;
		int32_t CppHandle;
		int32_t ClassHandle;
		Action2();
		void operator+=(System::Action2<System::Single, System::Single>& del);
		void operator-=(System::Action2<System::Single, System::Single>& del);
		virtual void operator()(System::Single arg1, System::Single arg2);
		void Invoke(System::Single arg1, System::Single arg2);
	};
}

namespace System
{
	template<> struct Func3<System::Int32, System::Single, System::Double> : virtual System::Object
	{
		Func3<System::Int32, System::Single, System::Double>(decltype(nullptr));
		Func3<System::Int32, System::Single, System::Double>(Plugin::InternalUse, int32_t handle);
		Func3<System::Int32, System::Single, System::Double>(const Func3<System::Int32, System::Single, System::Double>& other);
		Func3<System::Int32, System::Single, System::Double>(Func3<System::Int32, System::Single, System::Double>&& other);
		virtual ~Func3<System::Int32, System::Single, System::Double>();
		Func3<System::Int32, System::Single, System::Double>& operator=(const Func3<System::Int32, System::Single, System::Double>& other);
		Func3<System::Int32, System::Single, System::Double>& operator=(decltype(nullptr));
		Func3<System::Int32, System::Single, System::Double>& operator=(Func3<System::Int32, System::Single, System::Double>&& other);
		bool operator==(const Func3<System::Int32, System::Single, System::Double>& other) const;
		bool operator!=(const Func3<System::Int32, System::Single, System::Double>& other) const;
		int32_t CppHandle;
		int32_t ClassHandle;
		Func3();
		void operator+=(System::Func3<System::Int32, System::Single, System::Double>& del);
		void operator-=(System::Func3<System::Int32, System::Single, System::Double>& del);
		virtual System::Double operator()(System::Int32 arg1, System::Single arg2);
		System::Double Invoke(System::Int32 arg1, System::Single arg2);
	};
}

namespace System
{
	template<> struct Func3<System::Int16, System::Int32, System::String> : virtual System::Object
	{
		Func3<System::Int16, System::Int32, System::String>(decltype(nullptr));
		Func3<System::Int16, System::Int32, System::String>(Plugin::InternalUse, int32_t handle);
		Func3<System::Int16, System::Int32, System::String>(const Func3<System::Int16, System::Int32, System::String>& other);
		Func3<System::Int16, System::Int32, System::String>(Func3<System::Int16, System::Int32, System::String>&& other);
		virtual ~Func3<System::Int16, System::Int32, System::String>();
		Func3<System::Int16, System::Int32, System::String>& operator=(const Func3<System::Int16, System::Int32, System::String>& other);
		Func3<System::Int16, System::Int32, System::String>& operator=(decltype(nullptr));
		Func3<System::Int16, System::Int32, System::String>& operator=(Func3<System::Int16, System::Int32, System::String>&& other);
		bool operator==(const Func3<System::Int16, System::Int32, System::String>& other) const;
		bool operator!=(const Func3<System::Int16, System::Int32, System::String>& other) const;
		int32_t CppHandle;
		int32_t ClassHandle;
		Func3();
		void operator+=(System::Func3<System::Int16, System::Int32, System::String>& del);
		void operator-=(System::Func3<System::Int16, System::Int32, System::String>& del);
		virtual System::String operator()(System::Int16 arg1, System::Int32 arg2);
		System::String Invoke(System::Int16 arg1, System::Int32 arg2);
	};
}

namespace System
{
	struct AppDomainInitializer : virtual System::Object
	{
		AppDomainInitializer(decltype(nullptr));
		AppDomainInitializer(Plugin::InternalUse, int32_t handle);
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
			UnityAction(Plugin::InternalUse, int32_t handle);
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
			UnityAction2<UnityEngine::SceneManagement::Scene, UnityEngine::SceneManagement::LoadSceneMode>(Plugin::InternalUse, int32_t handle);
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
				ComponentEventHandler(Plugin::InternalUse, int32_t handle);
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
				ComponentChangingEventHandler(Plugin::InternalUse, int32_t handle);
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
				ComponentChangedEventHandler(Plugin::InternalUse, int32_t handle);
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
				ComponentRenameEventHandler(Plugin::InternalUse, int32_t handle);
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

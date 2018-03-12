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

// Game type definitions
#include "Game.h"

// For assert()
#include <assert.h>

// For int32_t, etc.
#include <stdint.h>

// For malloc(), etc.
#include <stdlib.h>

// For memset(), etc.
#include <string.h>

// Support placement new
void* operator new(size_t, void* p)
{
	return p;
}

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
	void (*SetException)(int32_t handle);
	int32_t (*ArrayGetLength)(int32_t handle);
	int32_t (*EnumerableGetEnumerator)(int32_t handle);
	
	/*BEGIN FUNCTION POINTERS*/
	UnityEngine::Vector3 (*UnityEngineVector3ConstructorSystemSingle_SystemSingle_SystemSingle)(float x, float y, float z);
	UnityEngine::Vector3 (*UnityEngineVector3Methodop_AdditionUnityEngineVector3_UnityEngineVector3)(UnityEngine::Vector3& a, UnityEngine::Vector3& b);
	int32_t (*BoxVector3)(UnityEngine::Vector3& val);
	UnityEngine::Vector3 (*UnboxVector3)(int32_t valHandle);
	int32_t (*UnityEngineObjectPropertyGetName)(int32_t thisHandle);
	void (*UnityEngineObjectPropertySetName)(int32_t thisHandle, int32_t valueHandle);
	int32_t (*UnityEngineComponentPropertyGetTransform)(int32_t thisHandle);
	UnityEngine::Vector3 (*UnityEngineTransformPropertyGetPosition)(int32_t thisHandle);
	void (*UnityEngineTransformPropertySetPosition)(int32_t thisHandle, UnityEngine::Vector3& value);
	int32_t (*SystemCollectionsIEnumeratorPropertyGetCurrent)(int32_t thisHandle);
	int32_t (*SystemCollectionsIEnumeratorMethodMoveNext)(int32_t thisHandle);
	int32_t (*UnityEngineGameObjectMethodAddComponentMyGameBaseBallScript)(int32_t thisHandle);
	int32_t (*UnityEngineGameObjectMethodCreatePrimitiveUnityEnginePrimitiveType)(UnityEngine::PrimitiveType type);
	void (*UnityEngineDebugMethodLogSystemObject)(int32_t messageHandle);
	int32_t (*UnityEngineMonoBehaviourPropertyGetTransform)(int32_t thisHandle);
	int32_t (*SystemExceptionConstructorSystemString)(int32_t messageHandle);
	int32_t (*BoxPrimitiveType)(UnityEngine::PrimitiveType val);
	UnityEngine::PrimitiveType (*UnboxPrimitiveType)(int32_t valHandle);
	System::Single (*UnityEngineTimePropertyGetDeltaTime)();
	void (*ReleaseBaseBallScript)(int32_t handle);
	void (*BaseBallScriptConstructor)(int32_t cppHandle, int32_t* handle);
	int32_t (*BoxBoolean)(uint32_t val);
	int32_t (*UnboxBoolean)(int32_t valHandle);
	int32_t (*BoxSByte)(int8_t val);
	System::SByte (*UnboxSByte)(int32_t valHandle);
	int32_t (*BoxByte)(uint8_t val);
	System::Byte (*UnboxByte)(int32_t valHandle);
	int32_t (*BoxInt16)(int16_t val);
	System::Int16 (*UnboxInt16)(int32_t valHandle);
	int32_t (*BoxUInt16)(uint16_t val);
	System::UInt16 (*UnboxUInt16)(int32_t valHandle);
	int32_t (*BoxInt32)(int32_t val);
	System::Int32 (*UnboxInt32)(int32_t valHandle);
	int32_t (*BoxUInt32)(uint32_t val);
	System::UInt32 (*UnboxUInt32)(int32_t valHandle);
	int32_t (*BoxInt64)(int64_t val);
	System::Int64 (*UnboxInt64)(int32_t valHandle);
	int32_t (*BoxUInt64)(uint64_t val);
	System::UInt64 (*UnboxUInt64)(int32_t valHandle);
	int32_t (*BoxChar)(uint16_t val);
	int16_t (*UnboxChar)(int32_t valHandle);
	int32_t (*BoxSingle)(float val);
	System::Single (*UnboxSingle)(int32_t valHandle);
	int32_t (*BoxDouble)(double val);
	System::Double (*UnboxDouble)(int32_t valHandle);
	/*END FUNCTION POINTERS*/
}

////////////////////////////////////////////////////////////////
// Global variables
////////////////////////////////////////////////////////////////

namespace Plugin
{
	System::String NullString(nullptr);
}

////////////////////////////////////////////////////////////////
// Plugin Types
////////////////////////////////////////////////////////////////

namespace Plugin
{
	ManagedType::ManagedType()
		: Handle(0)
	{
	}
	
	ManagedType::ManagedType(decltype(nullptr))
		: Handle(0)
	{
	}
	
	ManagedType::ManagedType(Plugin::InternalUse iu, int32_t handle)
		: Handle(handle)
	{
	}
}

////////////////////////////////////////////////////////////////
// C# Primitive Types
////////////////////////////////////////////////////////////////

namespace System
{
	Boolean::Boolean()
		: Value(0)
	{
	}
	
	Boolean::Boolean(bool value)
		: Value((int32_t)value)
	{
	}
	
	Boolean::Boolean(int32_t value)
		: Value(value)
	{
	}
	
	Boolean::Boolean(uint32_t value)
		: Value(value)
	{
	}
	
	Boolean::operator bool() const
	{
		return (bool)Value;
	}
	
	Boolean::operator int32_t() const
	{
		return Value;
	}
	
	Boolean::operator uint32_t() const
	{
		return Value;
	}
	
	Boolean::operator Object() const
	{
		return Object(Plugin::InternalUse::Only, Plugin::BoxBoolean(Value));
	}
	
	Boolean::operator ValueType() const
	{
		return ValueType(Plugin::InternalUse::Only, Plugin::BoxBoolean(Value));
	}
	
	Boolean::operator IComparable() const
	{
		return IComparable(Plugin::InternalUse::Only, Plugin::BoxBoolean(Value));
	}
	
	Boolean::operator IFormattable() const
	{
		return IFormattable(Plugin::InternalUse::Only, Plugin::BoxBoolean(Value));
	}
	
	Boolean::operator IConvertible() const
	{
		return IConvertible(Plugin::InternalUse::Only, Plugin::BoxBoolean(Value));
	}
	
	Char::Char()
		: Value(0)
	{
	}
	
	Char::Char(char value)
		: Value(value)
	{
	}
	
	Char::Char(int16_t value)
		: Value(value)
	{
	}
	
	Char::operator int16_t() const
	{
		return Value;
	}
	
	Char::operator Object() const
	{
		return Object(Plugin::InternalUse::Only, Plugin::BoxChar(Value));
	}
	
	Char::operator ValueType() const
	{
		return ValueType(Plugin::InternalUse::Only, Plugin::BoxChar(Value));
	}
	
	Char::operator IComparable() const
	{
		return IComparable(Plugin::InternalUse::Only, Plugin::BoxChar(Value));
	}
	
	Char::operator IFormattable() const
	{
		return IFormattable(Plugin::InternalUse::Only, Plugin::BoxChar(Value));
	}
	
	Char::operator IConvertible() const
	{
		return IConvertible(Plugin::InternalUse::Only, Plugin::BoxChar(Value));
	}

	SByte::SByte()
		: Value(0)
	{
	}
	
	SByte::SByte(int8_t val)
		: Value(val)
	{
	}
	
	SByte::operator int8_t() const
	{
		return Value;
	}
	
	SByte::operator Object() const
	{
		return Object(Plugin::InternalUse::Only, Plugin::BoxSByte(Value));
	}
	
	SByte::operator ValueType() const
	{
		return ValueType(Plugin::InternalUse::Only, Plugin::BoxSByte(Value));
	}
	
	SByte::operator IComparable() const
	{
		return IComparable(Plugin::InternalUse::Only, Plugin::BoxSByte(Value));
	}
	
	SByte::operator IFormattable() const
	{
		return IFormattable(Plugin::InternalUse::Only, Plugin::BoxSByte(Value));
	}
	
	SByte::operator IConvertible() const
	{
		return IConvertible(Plugin::InternalUse::Only, Plugin::BoxSByte(Value));
	}

	Byte::Byte()
		: Value(0)
	{
	}
	
	Byte::Byte(uint8_t value)
		: Value(value)
	{
	}
	
	Byte::operator uint8_t() const
	{
		return Value;
	}
	
	Byte::operator Object() const
	{
		return Object(Plugin::InternalUse::Only, Plugin::BoxByte(Value));
	}
	
	Byte::operator ValueType() const
	{
		return ValueType(Plugin::InternalUse::Only, Plugin::BoxByte(Value));
	}
	
	Byte::operator IComparable() const
	{
		return IComparable(Plugin::InternalUse::Only, Plugin::BoxByte(Value));
	}
	
	Byte::operator IFormattable() const
	{
		return IFormattable(Plugin::InternalUse::Only, Plugin::BoxByte(Value));
	}
	
	Byte::operator IConvertible() const
	{
		return IConvertible(Plugin::InternalUse::Only, Plugin::BoxByte(Value));
	}

	Int16::Int16()
		: Value(0)
	{
	}
	
	Int16::Int16(int16_t value)
		: Value(value)
	{
	}
	
	Int16::operator int16_t() const
	{
		return Value;
	}
	
	Int16::operator Object() const
	{
		return Object(Plugin::InternalUse::Only, Plugin::BoxInt16(Value));
	}
	
	Int16::operator ValueType() const
	{
		return ValueType(Plugin::InternalUse::Only, Plugin::BoxInt16(Value));
	}
	
	Int16::operator IComparable() const
	{
		return IComparable(Plugin::InternalUse::Only, Plugin::BoxInt16(Value));
	}
	
	Int16::operator IFormattable() const
	{
		return IFormattable(Plugin::InternalUse::Only, Plugin::BoxInt16(Value));
	}
	
	Int16::operator IConvertible() const
	{
		return IConvertible(Plugin::InternalUse::Only, Plugin::BoxInt16(Value));
	}

	UInt16::UInt16()
		: Value(0)
	{
	}
	
	UInt16::UInt16(uint16_t value)
		: Value(value)
	{
	}
	
	UInt16::operator uint16_t() const
	{
		return Value;
	}
	
	UInt16::operator Object() const
	{
		return Object(Plugin::InternalUse::Only, Plugin::BoxUInt16(Value));
	}
	
	UInt16::operator ValueType() const
	{
		return ValueType(Plugin::InternalUse::Only, Plugin::BoxUInt16(Value));
	}
	
	UInt16::operator IComparable() const
	{
		return IComparable(Plugin::InternalUse::Only, Plugin::BoxUInt16(Value));
	}
	
	UInt16::operator IFormattable() const
	{
		return IFormattable(Plugin::InternalUse::Only, Plugin::BoxUInt16(Value));
	}
	
	UInt16::operator IConvertible() const
	{
		return IConvertible(Plugin::InternalUse::Only, Plugin::BoxUInt16(Value));
	}

	Int32::Int32()
		: Value(0)
	{
	}
	
	Int32::Int32(int32_t value)
		: Value(value)
	{
	}
	
	Int32::operator int32_t() const
	{
		return Value;
	}
	
	Int32::operator Object() const
	{
		return Object(Plugin::InternalUse::Only, Plugin::BoxInt32(Value));
	}
	
	Int32::operator ValueType() const
	{
		return ValueType(Plugin::InternalUse::Only, Plugin::BoxInt32(Value));
	}
	
	Int32::operator IComparable() const
	{
		return IComparable(Plugin::InternalUse::Only, Plugin::BoxInt32(Value));
	}
	
	Int32::operator IFormattable() const
	{
		return IFormattable(Plugin::InternalUse::Only, Plugin::BoxInt32(Value));
	}
	
	Int32::operator IConvertible() const
	{
		return IConvertible(Plugin::InternalUse::Only, Plugin::BoxInt32(Value));
	}

	UInt32::UInt32()
		: Value(0)
	{
	}
	
	UInt32::UInt32(uint32_t value)
		: Value(value)
	{
	}
	
	UInt32::operator uint32_t() const
	{
		return Value;
	}
	
	UInt32::operator Object() const
	{
		return Object(Plugin::InternalUse::Only, Plugin::BoxUInt32(Value));
	}
	
	UInt32::operator ValueType() const
	{
		return ValueType(Plugin::InternalUse::Only, Plugin::BoxUInt32(Value));
	}
	
	UInt32::operator IComparable() const
	{
		return IComparable(Plugin::InternalUse::Only, Plugin::BoxUInt32(Value));
	}
	
	UInt32::operator IFormattable() const
	{
		return IFormattable(Plugin::InternalUse::Only, Plugin::BoxUInt32(Value));
	}
	
	UInt32::operator IConvertible() const
	{
		return IConvertible(Plugin::InternalUse::Only, Plugin::BoxUInt32(Value));
	}

	Int64::Int64()
		: Value(0)
	{
	}
	
	Int64::Int64(int64_t value)
		: Value(value)
	{
	}
	
	Int64::operator int64_t() const
	{
		return Value;
	}
	
	Int64::operator Object() const
	{
		return Object(Plugin::InternalUse::Only, Plugin::BoxInt64(Value));
	}
	
	Int64::operator ValueType() const
	{
		return ValueType(Plugin::InternalUse::Only, Plugin::BoxInt64(Value));
	}
	
	Int64::operator IComparable() const
	{
		return IComparable(Plugin::InternalUse::Only, Plugin::BoxInt64(Value));
	}
	
	Int64::operator IFormattable() const
	{
		return IFormattable(Plugin::InternalUse::Only, Plugin::BoxInt64(Value));
	}
	
	Int64::operator IConvertible() const
	{
		return IConvertible(Plugin::InternalUse::Only, Plugin::BoxInt64(Value));
	}

	UInt64::UInt64()
		: Value(0)
	{
	}
	
	UInt64::UInt64(uint64_t value)
		: Value(value)
	{
	}
	
	UInt64::operator uint64_t() const
	{
		return Value;
	}
	
	UInt64::operator Object() const
	{
		return Object(Plugin::InternalUse::Only, Plugin::BoxUInt64(Value));
	}
	
	UInt64::operator ValueType() const
	{
		return ValueType(Plugin::InternalUse::Only, Plugin::BoxUInt64(Value));
	}
	
	UInt64::operator IComparable() const
	{
		return IComparable(Plugin::InternalUse::Only, Plugin::BoxUInt64(Value));
	}
	
	UInt64::operator IFormattable() const
	{
		return IFormattable(Plugin::InternalUse::Only, Plugin::BoxUInt64(Value));
	}
	
	UInt64::operator IConvertible() const
	{
		return IConvertible(Plugin::InternalUse::Only, Plugin::BoxUInt64(Value));
	}

	Single::Single()
		: Value(0.0f)
	{
	}
	
	Single::Single(float value)
		: Value(value)
	{
	}
	
	Single::operator float() const
	{
		return Value;
	}
	
	Single::operator Object() const
	{
		return Object(Plugin::InternalUse::Only, Plugin::BoxSingle(Value));
	}
	
	Single::operator ValueType() const
	{
		return ValueType(Plugin::InternalUse::Only, Plugin::BoxSingle(Value));
	}
	
	Single::operator IComparable() const
	{
		return IComparable(Plugin::InternalUse::Only, Plugin::BoxSingle(Value));
	}
	
	Single::operator IFormattable() const
	{
		return IFormattable(Plugin::InternalUse::Only, Plugin::BoxSingle(Value));
	}
	
	Single::operator IConvertible() const
	{
		return IConvertible(Plugin::InternalUse::Only, Plugin::BoxSingle(Value));
	}

	Double::Double()
		: Value(0.0)
	{
	}
	
	Double::Double(double value)
		: Value(value)
	{
	}
	
	Double::operator double() const
	{
		return Value;
	}
	
	Double::operator Object() const
	{
		return Object(Plugin::InternalUse::Only, Plugin::BoxDouble(Value));
	}
	
	Double::operator ValueType() const
	{
		return ValueType(Plugin::InternalUse::Only, Plugin::BoxDouble(Value));
	}
	
	Double::operator IComparable() const
	{
		return IComparable(Plugin::InternalUse::Only, Plugin::BoxDouble(Value));
	}
	
	Double::operator IFormattable() const
	{
		return IFormattable(Plugin::InternalUse::Only, Plugin::BoxDouble(Value));
	}
	
	Double::operator IConvertible() const
	{
		return IConvertible(Plugin::InternalUse::Only, Plugin::BoxDouble(Value));
	}
}

////////////////////////////////////////////////////////////////
// Support for using IEnumerable with range for loops
////////////////////////////////////////////////////////////////

namespace Plugin
{
	// End iterators are dummies full of null
	EnumerableIterator::EnumerableIterator(decltype(nullptr))
		: enumerator(nullptr)
		, hasMore(false)
	{
	}
	
	// Begin iterators keep track of an IEnumerator
	EnumerableIterator::EnumerableIterator(
		System::Collections::IEnumerable& enumerable)
		: enumerator(enumerable.GetEnumerator())
	{
		hasMore = enumerator.MoveNext();
	}
	
	EnumerableIterator& EnumerableIterator::operator++()
	{
		hasMore = enumerator.MoveNext();
		return *this;
	}
	
	bool EnumerableIterator::operator!=(const EnumerableIterator& other)
	{
		return hasMore;
	}
	
	System::Object EnumerableIterator::operator*()
	{
		return enumerator.GetCurrent();
	}
}

////////////////////////////////////////////////////////////////
// Reference counting of managed objects
////////////////////////////////////////////////////////////////

namespace Plugin
{
	int32_t RefCountsLenClass;
	int32_t* RefCountsClass;

	void ReferenceManagedClass(int32_t handle)
	{
		assert(handle >= 0 && handle < RefCountsLenClass);
		if (handle != 0)
		{
			RefCountsClass[handle]++;
		}
	}

	void DereferenceManagedClass(int32_t handle)
	{
		assert(handle >= 0 && handle < RefCountsLenClass);
		if (handle != 0)
		{
			int32_t numRemain = --RefCountsClass[handle];
			if (numRemain == 0)
			{
				ReleaseObject(handle);
			}
		}
	}
	
	bool DereferenceManagedClassNoRelease(int32_t handle)
	{
		assert(handle >= 0 && handle < RefCountsLenClass);
		if (handle != 0)
		{
			int32_t numRemain = --RefCountsClass[handle];
			if (numRemain == 0)
			{
				return true;
			}
		}
		return false;
	}
	
	/*BEGIN GLOBAL STATE AND FUNCTIONS*/
	// Free list for MyGame::BaseBallScript pointers
	
	int32_t BaseBallScriptFreeListSize;
	MyGame::BaseBallScript** BaseBallScriptFreeList;
	MyGame::BaseBallScript** NextFreeBaseBallScript;
	
	int32_t StoreBaseBallScript(MyGame::BaseBallScript* del)
	{
		assert(NextFreeBaseBallScript != nullptr);
		MyGame::BaseBallScript** pNext = NextFreeBaseBallScript;
		NextFreeBaseBallScript = (MyGame::BaseBallScript**)*pNext;
		*pNext = del;
		return (int32_t)(pNext - BaseBallScriptFreeList);
	}
	
	MyGame::BaseBallScript* GetBaseBallScript(int32_t handle)
	{
		assert(handle >= 0 && handle < BaseBallScriptFreeListSize);
		return BaseBallScriptFreeList[handle];
	}
	
	void RemoveBaseBallScript(int32_t handle)
	{
		MyGame::BaseBallScript** pRelease = BaseBallScriptFreeList + handle;
		*pRelease = (MyGame::BaseBallScript*)NextFreeBaseBallScript;
		NextFreeBaseBallScript = pRelease;
	}
	
	// Free list for whole MyGame::BaseBallScript objects
	
	union BaseBallScriptFreeWholeListEntry
	{
		BaseBallScriptFreeWholeListEntry* Next;
		MyGame::BaseBallScript Value;
	};
	int32_t BaseBallScriptFreeWholeListSize;
	BaseBallScriptFreeWholeListEntry* BaseBallScriptFreeWholeList;
	BaseBallScriptFreeWholeListEntry* NextFreeWholeBaseBallScript;
	
	MyGame::BaseBallScript* StoreWholeBaseBallScript()
	{
		assert(NextFreeWholeBaseBallScript != nullptr);
		BaseBallScriptFreeWholeListEntry* pNext = NextFreeWholeBaseBallScript;
		NextFreeWholeBaseBallScript = pNext->Next;
		return &pNext->Value;
	}
	
	void RemoveWholeBaseBallScript(MyGame::BaseBallScript* instance)
	{
		BaseBallScriptFreeWholeListEntry* pRelease = (BaseBallScriptFreeWholeListEntry*)instance;
		if (pRelease >= BaseBallScriptFreeWholeList && pRelease < BaseBallScriptFreeWholeList + (BaseBallScriptFreeWholeListSize - 1))
		{
			pRelease->Next = NextFreeWholeBaseBallScript;
			NextFreeWholeBaseBallScript = pRelease->Next;
		}
	}
	/*END GLOBAL STATE AND FUNCTIONS*/
}

namespace Plugin
{
	// An unhandled exception caused by C++ calling into C#
	System::Exception* unhandledCsharpException = nullptr;
}

////////////////////////////////////////////////////////////////
// Mirrors of C# types. These wrap the C# functions to present
// a similiar API as in C#.
////////////////////////////////////////////////////////////////

namespace System
{
	Object::Object()
		: Plugin::ManagedType(nullptr)
	{
	}
	
	Object::Object(Plugin::InternalUse iu, int32_t handle)
		: ManagedType(Plugin::InternalUse::Only, handle)
	{
	}
	
	Object::Object(decltype(nullptr))
		: ManagedType(nullptr)
	{
	}
	
	bool Object::operator==(decltype(nullptr)) const
	{
		return Handle == 0;
	}
	
	bool Object::operator!=(decltype(nullptr)) const
	{
		return Handle != 0;
	}
	
	void Object::ThrowReferenceToThis()
	{
		throw *this;
	}
	
	ValueType::ValueType(Plugin::InternalUse iu, int32_t handle)
		: Object(iu, handle)
	{
	}
	
	ValueType::ValueType(decltype(nullptr))
		: Object(nullptr)
	{
	}
	
	Enum::Enum(Plugin::InternalUse iu, int32_t handle)
		: ValueType(iu, handle)
	{
	}
	
	Enum::Enum(decltype(nullptr))
		: ValueType(nullptr)
	{
	}
	
	String::String(decltype(nullptr))
		: Object(Plugin::InternalUse::Only, 0)
	{
	}
	
	String::String(Plugin::InternalUse iu, int32_t handle)
		: Object(iu, handle)
	{
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
	}
	
	String::String(const String& other)
		: Object(Plugin::InternalUse::Only, other.Handle)
	{
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
	}
	
	String::String(String&& other)
		: Object(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
	}
	
	String::~String()
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
	}
	
	String& String::operator=(const String& other)
	{
		if (Handle != other.Handle)
		{
			if (Handle)
			{
				Plugin::DereferenceManagedClass(Handle);
			}
			Handle = other.Handle;
			if (Handle)
			{
				Plugin::ReferenceManagedClass(Handle);
			}
		}
		return *this;
	}
	
	String& String::operator=(decltype(nullptr))
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
		return *this;
	}
	
	String& String::operator=(String&& other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
		}
		Handle = other.Handle;
		other.Handle = 0;
		return *this;
	}
	
	String::String(const char* chars)
		: Object(Plugin::InternalUse::Only, Plugin::StringNew(chars))
	{
	}
	
	ICloneable::ICloneable(Plugin::InternalUse iu, int32_t handle)
		: Object(iu, handle)
	{
	}
	
	ICloneable::ICloneable(decltype(nullptr))
		: Object(nullptr)
	{
	}
	
	namespace Collections
	{
		IEnumerable::IEnumerable(Plugin::InternalUse iu, int32_t handle)
			: Object(iu, handle)
		{
		}
		
		IEnumerable::IEnumerable(decltype(nullptr))
			: Object(nullptr)
		{
		}
		
		IEnumerator IEnumerable::GetEnumerator()
		{
			return IEnumerator(
				Plugin::InternalUse::Only,
				Plugin::EnumerableGetEnumerator(Handle));
		}
		
		Plugin::EnumerableIterator begin(
			System::Collections::IEnumerable& enumerable)
		{
			return Plugin::EnumerableIterator(enumerable);
		}
		
		Plugin::EnumerableIterator end(
			System::Collections::IEnumerable& enumerable)
		{
			return Plugin::EnumerableIterator(nullptr);
		}
		
		ICollection::ICollection(Plugin::InternalUse iu, int32_t handle)
			: Object(iu, handle)
			, IEnumerable(nullptr)
		{
		}
		
		ICollection::ICollection(decltype(nullptr))
			: Object(nullptr)
			, IEnumerable(nullptr)
		{
		}
		
		IList::IList(Plugin::InternalUse iu, int32_t handle)
			: Object(iu, handle)
			, IEnumerable(nullptr)
			, ICollection(nullptr)
		{
		}
		
		IList::IList(decltype(nullptr))
			: Object(nullptr)
			, IEnumerable(nullptr)
			, ICollection(nullptr)
		{
		}
	}
	
	Array::Array(Plugin::InternalUse iu, int32_t handle)
		: Object(iu, handle)
		, ICloneable(nullptr)
		, Collections::IEnumerable(nullptr)
		, Collections::ICollection(nullptr)
		, Collections::IList(nullptr)
	{
	}
	
	Array::Array(decltype(nullptr))
		: Object(nullptr)
		, ICloneable(nullptr)
		, Collections::IEnumerable(nullptr)
		, Collections::ICollection(nullptr)
		, Collections::IList(nullptr)
	{
	}
	
	int32_t Array::GetLength()
	{
		return Plugin::ArrayGetLength(Handle);
	}
	
	int32_t Array::GetRank()
	{
		return 0;
	}
}

/*BEGIN METHOD DEFINITIONS*/
namespace System
{
	IFormattable::IFormattable(decltype(nullptr))
	{
	}
	
	IFormattable::IFormattable(Plugin::InternalUse, int32_t handle)
	{
		Handle = handle;
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
	}
	
	IFormattable::IFormattable(const IFormattable& other)
		: IFormattable(Plugin::InternalUse::Only, other.Handle)
	{
	}
	
	IFormattable::IFormattable(IFormattable&& other)
		: IFormattable(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
	}
	
	IFormattable::~IFormattable()
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
	}
	
	IFormattable& IFormattable::operator=(const IFormattable& other)
	{
		if (this->Handle)
		{
			Plugin::DereferenceManagedClass(this->Handle);
		}
		this->Handle = other.Handle;
		if (this->Handle)
		{
			Plugin::ReferenceManagedClass(this->Handle);
		}
		return *this;
	}
	
	IFormattable& IFormattable::operator=(decltype(nullptr))
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
		return *this;
	}
	
	IFormattable& IFormattable::operator=(IFormattable&& other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
		}
		Handle = other.Handle;
		other.Handle = 0;
		return *this;
	}
	
	bool IFormattable::operator==(const IFormattable& other) const
	{
		return Handle == other.Handle;
	}
	
	bool IFormattable::operator!=(const IFormattable& other) const
	{
		return Handle != other.Handle;
	}
}

namespace System
{
	IConvertible::IConvertible(decltype(nullptr))
	{
	}
	
	IConvertible::IConvertible(Plugin::InternalUse, int32_t handle)
	{
		Handle = handle;
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
	}
	
	IConvertible::IConvertible(const IConvertible& other)
		: IConvertible(Plugin::InternalUse::Only, other.Handle)
	{
	}
	
	IConvertible::IConvertible(IConvertible&& other)
		: IConvertible(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
	}
	
	IConvertible::~IConvertible()
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
	}
	
	IConvertible& IConvertible::operator=(const IConvertible& other)
	{
		if (this->Handle)
		{
			Plugin::DereferenceManagedClass(this->Handle);
		}
		this->Handle = other.Handle;
		if (this->Handle)
		{
			Plugin::ReferenceManagedClass(this->Handle);
		}
		return *this;
	}
	
	IConvertible& IConvertible::operator=(decltype(nullptr))
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
		return *this;
	}
	
	IConvertible& IConvertible::operator=(IConvertible&& other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
		}
		Handle = other.Handle;
		other.Handle = 0;
		return *this;
	}
	
	bool IConvertible::operator==(const IConvertible& other) const
	{
		return Handle == other.Handle;
	}
	
	bool IConvertible::operator!=(const IConvertible& other) const
	{
		return Handle != other.Handle;
	}
}

namespace System
{
	IComparable::IComparable(decltype(nullptr))
	{
	}
	
	IComparable::IComparable(Plugin::InternalUse, int32_t handle)
	{
		Handle = handle;
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
	}
	
	IComparable::IComparable(const IComparable& other)
		: IComparable(Plugin::InternalUse::Only, other.Handle)
	{
	}
	
	IComparable::IComparable(IComparable&& other)
		: IComparable(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
	}
	
	IComparable::~IComparable()
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
	}
	
	IComparable& IComparable::operator=(const IComparable& other)
	{
		if (this->Handle)
		{
			Plugin::DereferenceManagedClass(this->Handle);
		}
		this->Handle = other.Handle;
		if (this->Handle)
		{
			Plugin::ReferenceManagedClass(this->Handle);
		}
		return *this;
	}
	
	IComparable& IComparable::operator=(decltype(nullptr))
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
		return *this;
	}
	
	IComparable& IComparable::operator=(IComparable&& other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
		}
		Handle = other.Handle;
		other.Handle = 0;
		return *this;
	}
	
	bool IComparable::operator==(const IComparable& other) const
	{
		return Handle == other.Handle;
	}
	
	bool IComparable::operator!=(const IComparable& other) const
	{
		return Handle != other.Handle;
	}
}

namespace UnityEngine
{
	Vector3::Vector3()
	{
	}
	
	Vector3::Vector3(System::Single x, System::Single y, System::Single z)
	{
		auto returnValue = Plugin::UnityEngineVector3ConstructorSystemSingle_SystemSingle_SystemSingle(x, y, z);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		*this = returnValue;
	}
	
	UnityEngine::Vector3 Vector3::operator+(UnityEngine::Vector3& a)
	{
		auto returnValue = Plugin::UnityEngineVector3Methodop_AdditionUnityEngineVector3_UnityEngineVector3(*this, a);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnValue;
	}
	
	Vector3::operator System::ValueType()
	{
		int32_t handle = Plugin::BoxVector3(*this);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
			return System::ValueType(Plugin::InternalUse::Only, handle);
		}
		return nullptr;
	}
	
	Vector3::operator System::Object()
	{
		int32_t handle = Plugin::BoxVector3(*this);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
			return System::Object(Plugin::InternalUse::Only, handle);
		}
		return nullptr;
	}
}

namespace System
{
	Object::operator UnityEngine::Vector3()
	{
		UnityEngine::Vector3 returnVal(Plugin::UnboxVector3(Handle));
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnVal;
	}
}

namespace UnityEngine
{
	Object::Object(decltype(nullptr))
	{
	}
	
	Object::Object(Plugin::InternalUse, int32_t handle)
	{
		Handle = handle;
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
	}
	
	Object::Object(const Object& other)
		: Object(Plugin::InternalUse::Only, other.Handle)
	{
	}
	
	Object::Object(Object&& other)
		: Object(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
	}
	
	Object::~Object()
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
	}
	
	Object& Object::operator=(const Object& other)
	{
		if (this->Handle)
		{
			Plugin::DereferenceManagedClass(this->Handle);
		}
		this->Handle = other.Handle;
		if (this->Handle)
		{
			Plugin::ReferenceManagedClass(this->Handle);
		}
		return *this;
	}
	
	Object& Object::operator=(decltype(nullptr))
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
		return *this;
	}
	
	Object& Object::operator=(Object&& other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
		}
		Handle = other.Handle;
		other.Handle = 0;
		return *this;
	}
	
	bool Object::operator==(const Object& other) const
	{
		return Handle == other.Handle;
	}
	
	bool Object::operator!=(const Object& other) const
	{
		return Handle != other.Handle;
	}
	
	System::String Object::GetName()
	{
		auto returnValue = Plugin::UnityEngineObjectPropertyGetName(Handle);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return System::String(Plugin::InternalUse::Only, returnValue);
	}
	
	void Object::SetName(System::String& value)
	{
		Plugin::UnityEngineObjectPropertySetName(Handle, value.Handle);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
	}
}

namespace UnityEngine
{
	Component::Component(decltype(nullptr))
		: UnityEngine::Object(nullptr)
	{
	}
	
	Component::Component(Plugin::InternalUse, int32_t handle)
		: UnityEngine::Object(nullptr)
	{
		Handle = handle;
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
	}
	
	Component::Component(const Component& other)
		: Component(Plugin::InternalUse::Only, other.Handle)
	{
	}
	
	Component::Component(Component&& other)
		: Component(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
	}
	
	Component::~Component()
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
	}
	
	Component& Component::operator=(const Component& other)
	{
		if (this->Handle)
		{
			Plugin::DereferenceManagedClass(this->Handle);
		}
		this->Handle = other.Handle;
		if (this->Handle)
		{
			Plugin::ReferenceManagedClass(this->Handle);
		}
		return *this;
	}
	
	Component& Component::operator=(decltype(nullptr))
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
		return *this;
	}
	
	Component& Component::operator=(Component&& other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
		}
		Handle = other.Handle;
		other.Handle = 0;
		return *this;
	}
	
	bool Component::operator==(const Component& other) const
	{
		return Handle == other.Handle;
	}
	
	bool Component::operator!=(const Component& other) const
	{
		return Handle != other.Handle;
	}
	
	UnityEngine::Transform Component::GetTransform()
	{
		auto returnValue = Plugin::UnityEngineComponentPropertyGetTransform(Handle);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return UnityEngine::Transform(Plugin::InternalUse::Only, returnValue);
	}
}

namespace UnityEngine
{
	Transform::Transform(decltype(nullptr))
		: UnityEngine::Object(nullptr)
		, UnityEngine::Component(nullptr)
		, System::Collections::IEnumerable(nullptr)
	{
	}
	
	Transform::Transform(Plugin::InternalUse, int32_t handle)
		: UnityEngine::Object(nullptr)
		, UnityEngine::Component(nullptr)
		, System::Collections::IEnumerable(nullptr)
	{
		Handle = handle;
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
	}
	
	Transform::Transform(const Transform& other)
		: Transform(Plugin::InternalUse::Only, other.Handle)
	{
	}
	
	Transform::Transform(Transform&& other)
		: Transform(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
	}
	
	Transform::~Transform()
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
	}
	
	Transform& Transform::operator=(const Transform& other)
	{
		if (this->Handle)
		{
			Plugin::DereferenceManagedClass(this->Handle);
		}
		this->Handle = other.Handle;
		if (this->Handle)
		{
			Plugin::ReferenceManagedClass(this->Handle);
		}
		return *this;
	}
	
	Transform& Transform::operator=(decltype(nullptr))
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
		return *this;
	}
	
	Transform& Transform::operator=(Transform&& other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
		}
		Handle = other.Handle;
		other.Handle = 0;
		return *this;
	}
	
	bool Transform::operator==(const Transform& other) const
	{
		return Handle == other.Handle;
	}
	
	bool Transform::operator!=(const Transform& other) const
	{
		return Handle != other.Handle;
	}
	
	UnityEngine::Vector3 Transform::GetPosition()
	{
		auto returnValue = Plugin::UnityEngineTransformPropertyGetPosition(Handle);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnValue;
	}
	
	void Transform::SetPosition(UnityEngine::Vector3& value)
	{
		Plugin::UnityEngineTransformPropertySetPosition(Handle, value);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
	}
}

namespace System
{
	namespace Collections
	{
		IEnumerator::IEnumerator(decltype(nullptr))
		{
		}
		
		IEnumerator::IEnumerator(Plugin::InternalUse, int32_t handle)
		{
			Handle = handle;
			if (handle)
			{
				Plugin::ReferenceManagedClass(handle);
			}
		}
		
		IEnumerator::IEnumerator(const IEnumerator& other)
			: IEnumerator(Plugin::InternalUse::Only, other.Handle)
		{
		}
		
		IEnumerator::IEnumerator(IEnumerator&& other)
			: IEnumerator(Plugin::InternalUse::Only, other.Handle)
		{
			other.Handle = 0;
		}
		
		IEnumerator::~IEnumerator()
		{
			if (Handle)
			{
				Plugin::DereferenceManagedClass(Handle);
				Handle = 0;
			}
		}
		
		IEnumerator& IEnumerator::operator=(const IEnumerator& other)
		{
			if (this->Handle)
			{
				Plugin::DereferenceManagedClass(this->Handle);
			}
			this->Handle = other.Handle;
			if (this->Handle)
			{
				Plugin::ReferenceManagedClass(this->Handle);
			}
			return *this;
		}
		
		IEnumerator& IEnumerator::operator=(decltype(nullptr))
		{
			if (Handle)
			{
				Plugin::DereferenceManagedClass(Handle);
				Handle = 0;
			}
			return *this;
		}
		
		IEnumerator& IEnumerator::operator=(IEnumerator&& other)
		{
			if (Handle)
			{
				Plugin::DereferenceManagedClass(Handle);
			}
			Handle = other.Handle;
			other.Handle = 0;
			return *this;
		}
		
		bool IEnumerator::operator==(const IEnumerator& other) const
		{
			return Handle == other.Handle;
		}
		
		bool IEnumerator::operator!=(const IEnumerator& other) const
		{
			return Handle != other.Handle;
		}
		
		System::Object IEnumerator::GetCurrent()
		{
			auto returnValue = Plugin::SystemCollectionsIEnumeratorPropertyGetCurrent(Handle);
			if (Plugin::unhandledCsharpException)
			{
				System::Exception* ex = Plugin::unhandledCsharpException;
				Plugin::unhandledCsharpException = nullptr;
				ex->ThrowReferenceToThis();
				delete ex;
			}
			return System::Object(Plugin::InternalUse::Only, returnValue);
		}
		
		System::Boolean IEnumerator::MoveNext()
		{
			auto returnValue = Plugin::SystemCollectionsIEnumeratorMethodMoveNext(Handle);
			if (Plugin::unhandledCsharpException)
			{
				System::Exception* ex = Plugin::unhandledCsharpException;
				Plugin::unhandledCsharpException = nullptr;
				ex->ThrowReferenceToThis();
				delete ex;
			}
			return returnValue;
		}
	}
}

namespace System
{
	namespace Runtime
	{
		namespace Serialization
		{
			ISerializable::ISerializable(decltype(nullptr))
			{
			}
			
			ISerializable::ISerializable(Plugin::InternalUse, int32_t handle)
			{
				Handle = handle;
				if (handle)
				{
					Plugin::ReferenceManagedClass(handle);
				}
			}
			
			ISerializable::ISerializable(const ISerializable& other)
				: ISerializable(Plugin::InternalUse::Only, other.Handle)
			{
			}
			
			ISerializable::ISerializable(ISerializable&& other)
				: ISerializable(Plugin::InternalUse::Only, other.Handle)
			{
				other.Handle = 0;
			}
			
			ISerializable::~ISerializable()
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
			}
			
			ISerializable& ISerializable::operator=(const ISerializable& other)
			{
				if (this->Handle)
				{
					Plugin::DereferenceManagedClass(this->Handle);
				}
				this->Handle = other.Handle;
				if (this->Handle)
				{
					Plugin::ReferenceManagedClass(this->Handle);
				}
				return *this;
			}
			
			ISerializable& ISerializable::operator=(decltype(nullptr))
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
				return *this;
			}
			
			ISerializable& ISerializable::operator=(ISerializable&& other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
				}
				Handle = other.Handle;
				other.Handle = 0;
				return *this;
			}
			
			bool ISerializable::operator==(const ISerializable& other) const
			{
				return Handle == other.Handle;
			}
			
			bool ISerializable::operator!=(const ISerializable& other) const
			{
				return Handle != other.Handle;
			}
		}
	}
}

namespace System
{
	namespace Runtime
	{
		namespace InteropServices
		{
			_Exception::_Exception(decltype(nullptr))
			{
			}
			
			_Exception::_Exception(Plugin::InternalUse, int32_t handle)
			{
				Handle = handle;
				if (handle)
				{
					Plugin::ReferenceManagedClass(handle);
				}
			}
			
			_Exception::_Exception(const _Exception& other)
				: _Exception(Plugin::InternalUse::Only, other.Handle)
			{
			}
			
			_Exception::_Exception(_Exception&& other)
				: _Exception(Plugin::InternalUse::Only, other.Handle)
			{
				other.Handle = 0;
			}
			
			_Exception::~_Exception()
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
			}
			
			_Exception& _Exception::operator=(const _Exception& other)
			{
				if (this->Handle)
				{
					Plugin::DereferenceManagedClass(this->Handle);
				}
				this->Handle = other.Handle;
				if (this->Handle)
				{
					Plugin::ReferenceManagedClass(this->Handle);
				}
				return *this;
			}
			
			_Exception& _Exception::operator=(decltype(nullptr))
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
					Handle = 0;
				}
				return *this;
			}
			
			_Exception& _Exception::operator=(_Exception&& other)
			{
				if (Handle)
				{
					Plugin::DereferenceManagedClass(Handle);
				}
				Handle = other.Handle;
				other.Handle = 0;
				return *this;
			}
			
			bool _Exception::operator==(const _Exception& other) const
			{
				return Handle == other.Handle;
			}
			
			bool _Exception::operator!=(const _Exception& other) const
			{
				return Handle != other.Handle;
			}
		}
	}
}

namespace UnityEngine
{
	GameObject::GameObject(decltype(nullptr))
		: UnityEngine::Object(nullptr)
	{
	}
	
	GameObject::GameObject(Plugin::InternalUse, int32_t handle)
		: UnityEngine::Object(nullptr)
	{
		Handle = handle;
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
	}
	
	GameObject::GameObject(const GameObject& other)
		: GameObject(Plugin::InternalUse::Only, other.Handle)
	{
	}
	
	GameObject::GameObject(GameObject&& other)
		: GameObject(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
	}
	
	GameObject::~GameObject()
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
	}
	
	GameObject& GameObject::operator=(const GameObject& other)
	{
		if (this->Handle)
		{
			Plugin::DereferenceManagedClass(this->Handle);
		}
		this->Handle = other.Handle;
		if (this->Handle)
		{
			Plugin::ReferenceManagedClass(this->Handle);
		}
		return *this;
	}
	
	GameObject& GameObject::operator=(decltype(nullptr))
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
		return *this;
	}
	
	GameObject& GameObject::operator=(GameObject&& other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
		}
		Handle = other.Handle;
		other.Handle = 0;
		return *this;
	}
	
	bool GameObject::operator==(const GameObject& other) const
	{
		return Handle == other.Handle;
	}
	
	bool GameObject::operator!=(const GameObject& other) const
	{
		return Handle != other.Handle;
	}
	
	template<> MyGame::BaseBallScript GameObject::AddComponent<MyGame::BaseBallScript>()
	{
		auto returnValue = Plugin::UnityEngineGameObjectMethodAddComponentMyGameBaseBallScript(Handle);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return MyGame::BaseBallScript(Plugin::InternalUse::Only, returnValue);
	}
	
	UnityEngine::GameObject GameObject::CreatePrimitive(UnityEngine::PrimitiveType type)
	{
		auto returnValue = Plugin::UnityEngineGameObjectMethodCreatePrimitiveUnityEnginePrimitiveType(type);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return UnityEngine::GameObject(Plugin::InternalUse::Only, returnValue);
	}
}

namespace UnityEngine
{
	Debug::Debug(decltype(nullptr))
	{
	}
	
	Debug::Debug(Plugin::InternalUse, int32_t handle)
	{
		Handle = handle;
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
	}
	
	Debug::Debug(const Debug& other)
		: Debug(Plugin::InternalUse::Only, other.Handle)
	{
	}
	
	Debug::Debug(Debug&& other)
		: Debug(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
	}
	
	Debug::~Debug()
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
	}
	
	Debug& Debug::operator=(const Debug& other)
	{
		if (this->Handle)
		{
			Plugin::DereferenceManagedClass(this->Handle);
		}
		this->Handle = other.Handle;
		if (this->Handle)
		{
			Plugin::ReferenceManagedClass(this->Handle);
		}
		return *this;
	}
	
	Debug& Debug::operator=(decltype(nullptr))
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
		return *this;
	}
	
	Debug& Debug::operator=(Debug&& other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
		}
		Handle = other.Handle;
		other.Handle = 0;
		return *this;
	}
	
	bool Debug::operator==(const Debug& other) const
	{
		return Handle == other.Handle;
	}
	
	bool Debug::operator!=(const Debug& other) const
	{
		return Handle != other.Handle;
	}
	
	void Debug::Log(System::Object& message)
	{
		Plugin::UnityEngineDebugMethodLogSystemObject(message.Handle);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
	}
}

namespace UnityEngine
{
	Behaviour::Behaviour(decltype(nullptr))
		: UnityEngine::Object(nullptr)
		, UnityEngine::Component(nullptr)
	{
	}
	
	Behaviour::Behaviour(Plugin::InternalUse, int32_t handle)
		: UnityEngine::Object(nullptr)
		, UnityEngine::Component(nullptr)
	{
		Handle = handle;
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
	}
	
	Behaviour::Behaviour(const Behaviour& other)
		: Behaviour(Plugin::InternalUse::Only, other.Handle)
	{
	}
	
	Behaviour::Behaviour(Behaviour&& other)
		: Behaviour(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
	}
	
	Behaviour::~Behaviour()
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
	}
	
	Behaviour& Behaviour::operator=(const Behaviour& other)
	{
		if (this->Handle)
		{
			Plugin::DereferenceManagedClass(this->Handle);
		}
		this->Handle = other.Handle;
		if (this->Handle)
		{
			Plugin::ReferenceManagedClass(this->Handle);
		}
		return *this;
	}
	
	Behaviour& Behaviour::operator=(decltype(nullptr))
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
		return *this;
	}
	
	Behaviour& Behaviour::operator=(Behaviour&& other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
		}
		Handle = other.Handle;
		other.Handle = 0;
		return *this;
	}
	
	bool Behaviour::operator==(const Behaviour& other) const
	{
		return Handle == other.Handle;
	}
	
	bool Behaviour::operator!=(const Behaviour& other) const
	{
		return Handle != other.Handle;
	}
}

namespace UnityEngine
{
	MonoBehaviour::MonoBehaviour(decltype(nullptr))
		: UnityEngine::Object(nullptr)
		, UnityEngine::Component(nullptr)
		, UnityEngine::Behaviour(nullptr)
	{
	}
	
	MonoBehaviour::MonoBehaviour(Plugin::InternalUse, int32_t handle)
		: UnityEngine::Object(nullptr)
		, UnityEngine::Component(nullptr)
		, UnityEngine::Behaviour(nullptr)
	{
		Handle = handle;
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
	}
	
	MonoBehaviour::MonoBehaviour(const MonoBehaviour& other)
		: MonoBehaviour(Plugin::InternalUse::Only, other.Handle)
	{
	}
	
	MonoBehaviour::MonoBehaviour(MonoBehaviour&& other)
		: MonoBehaviour(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
	}
	
	MonoBehaviour::~MonoBehaviour()
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
	}
	
	MonoBehaviour& MonoBehaviour::operator=(const MonoBehaviour& other)
	{
		if (this->Handle)
		{
			Plugin::DereferenceManagedClass(this->Handle);
		}
		this->Handle = other.Handle;
		if (this->Handle)
		{
			Plugin::ReferenceManagedClass(this->Handle);
		}
		return *this;
	}
	
	MonoBehaviour& MonoBehaviour::operator=(decltype(nullptr))
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
		return *this;
	}
	
	MonoBehaviour& MonoBehaviour::operator=(MonoBehaviour&& other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
		}
		Handle = other.Handle;
		other.Handle = 0;
		return *this;
	}
	
	bool MonoBehaviour::operator==(const MonoBehaviour& other) const
	{
		return Handle == other.Handle;
	}
	
	bool MonoBehaviour::operator!=(const MonoBehaviour& other) const
	{
		return Handle != other.Handle;
	}
	
	UnityEngine::Transform MonoBehaviour::GetTransform()
	{
		auto returnValue = Plugin::UnityEngineMonoBehaviourPropertyGetTransform(Handle);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return UnityEngine::Transform(Plugin::InternalUse::Only, returnValue);
	}
}

namespace System
{
	Exception::Exception(decltype(nullptr))
		: System::Runtime::InteropServices::_Exception(nullptr)
		, System::Runtime::Serialization::ISerializable(nullptr)
	{
	}
	
	Exception::Exception(Plugin::InternalUse, int32_t handle)
		: System::Runtime::InteropServices::_Exception(nullptr)
		, System::Runtime::Serialization::ISerializable(nullptr)
	{
		Handle = handle;
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
	}
	
	Exception::Exception(const Exception& other)
		: Exception(Plugin::InternalUse::Only, other.Handle)
	{
	}
	
	Exception::Exception(Exception&& other)
		: Exception(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
	}
	
	Exception::~Exception()
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
	}
	
	Exception& Exception::operator=(const Exception& other)
	{
		if (this->Handle)
		{
			Plugin::DereferenceManagedClass(this->Handle);
		}
		this->Handle = other.Handle;
		if (this->Handle)
		{
			Plugin::ReferenceManagedClass(this->Handle);
		}
		return *this;
	}
	
	Exception& Exception::operator=(decltype(nullptr))
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
		return *this;
	}
	
	Exception& Exception::operator=(Exception&& other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
		}
		Handle = other.Handle;
		other.Handle = 0;
		return *this;
	}
	
	bool Exception::operator==(const Exception& other) const
	{
		return Handle == other.Handle;
	}
	
	bool Exception::operator!=(const Exception& other) const
	{
		return Handle != other.Handle;
	}
	
	Exception::Exception(System::String& message)
		: System::Runtime::InteropServices::_Exception(nullptr)
		, System::Runtime::Serialization::ISerializable(nullptr)
	{
		auto returnValue = Plugin::SystemExceptionConstructorSystemString(message.Handle);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		Handle = returnValue;
		if (returnValue)
		{
			Plugin::ReferenceManagedClass(returnValue);
		}
	}
}

namespace System
{
	SystemException::SystemException(decltype(nullptr))
		: System::Runtime::InteropServices::_Exception(nullptr)
		, System::Runtime::Serialization::ISerializable(nullptr)
		, System::Exception(nullptr)
	{
	}
	
	SystemException::SystemException(Plugin::InternalUse, int32_t handle)
		: System::Runtime::InteropServices::_Exception(nullptr)
		, System::Runtime::Serialization::ISerializable(nullptr)
		, System::Exception(nullptr)
	{
		Handle = handle;
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
	}
	
	SystemException::SystemException(const SystemException& other)
		: SystemException(Plugin::InternalUse::Only, other.Handle)
	{
	}
	
	SystemException::SystemException(SystemException&& other)
		: SystemException(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
	}
	
	SystemException::~SystemException()
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
	}
	
	SystemException& SystemException::operator=(const SystemException& other)
	{
		if (this->Handle)
		{
			Plugin::DereferenceManagedClass(this->Handle);
		}
		this->Handle = other.Handle;
		if (this->Handle)
		{
			Plugin::ReferenceManagedClass(this->Handle);
		}
		return *this;
	}
	
	SystemException& SystemException::operator=(decltype(nullptr))
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
		return *this;
	}
	
	SystemException& SystemException::operator=(SystemException&& other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
		}
		Handle = other.Handle;
		other.Handle = 0;
		return *this;
	}
	
	bool SystemException::operator==(const SystemException& other) const
	{
		return Handle == other.Handle;
	}
	
	bool SystemException::operator!=(const SystemException& other) const
	{
		return Handle != other.Handle;
	}
}

namespace System
{
	NullReferenceException::NullReferenceException(decltype(nullptr))
		: System::Runtime::InteropServices::_Exception(nullptr)
		, System::Runtime::Serialization::ISerializable(nullptr)
		, System::Exception(nullptr)
		, System::SystemException(nullptr)
	{
	}
	
	NullReferenceException::NullReferenceException(Plugin::InternalUse, int32_t handle)
		: System::Runtime::InteropServices::_Exception(nullptr)
		, System::Runtime::Serialization::ISerializable(nullptr)
		, System::Exception(nullptr)
		, System::SystemException(nullptr)
	{
		Handle = handle;
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
	}
	
	NullReferenceException::NullReferenceException(const NullReferenceException& other)
		: NullReferenceException(Plugin::InternalUse::Only, other.Handle)
	{
	}
	
	NullReferenceException::NullReferenceException(NullReferenceException&& other)
		: NullReferenceException(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
	}
	
	NullReferenceException::~NullReferenceException()
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
	}
	
	NullReferenceException& NullReferenceException::operator=(const NullReferenceException& other)
	{
		if (this->Handle)
		{
			Plugin::DereferenceManagedClass(this->Handle);
		}
		this->Handle = other.Handle;
		if (this->Handle)
		{
			Plugin::ReferenceManagedClass(this->Handle);
		}
		return *this;
	}
	
	NullReferenceException& NullReferenceException::operator=(decltype(nullptr))
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
		return *this;
	}
	
	NullReferenceException& NullReferenceException::operator=(NullReferenceException&& other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
		}
		Handle = other.Handle;
		other.Handle = 0;
		return *this;
	}
	
	bool NullReferenceException::operator==(const NullReferenceException& other) const
	{
		return Handle == other.Handle;
	}
	
	bool NullReferenceException::operator!=(const NullReferenceException& other) const
	{
		return Handle != other.Handle;
	}
}

namespace UnityEngine
{
	PrimitiveType::PrimitiveType(int32_t value)
		: Value(value)
	{
	}
	
	UnityEngine::PrimitiveType::operator int32_t() const
	{
		return Value;
	}
	
	bool UnityEngine::PrimitiveType::operator==(PrimitiveType other)
	{
		return Value == other.Value;
	}
	
	bool UnityEngine::PrimitiveType::operator!=(PrimitiveType other)
	{
		return Value != other.Value;
	}
	
	PrimitiveType::operator System::Enum()
	{
		int32_t handle = Plugin::BoxPrimitiveType(*this);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
			return System::Enum(Plugin::InternalUse::Only, handle);
		}
		return nullptr;
	}
	
	PrimitiveType::operator System::ValueType()
	{
		int32_t handle = Plugin::BoxPrimitiveType(*this);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
			return System::ValueType(Plugin::InternalUse::Only, handle);
		}
		return nullptr;
	}
	
	PrimitiveType::operator System::Object()
	{
		int32_t handle = Plugin::BoxPrimitiveType(*this);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
			return System::Object(Plugin::InternalUse::Only, handle);
		}
		return nullptr;
	}
	
	PrimitiveType::operator System::IFormattable()
	{
		int32_t handle = Plugin::BoxPrimitiveType(*this);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
			return System::IFormattable(Plugin::InternalUse::Only, handle);
		}
		return nullptr;
	}
	
	PrimitiveType::operator System::IConvertible()
	{
		int32_t handle = Plugin::BoxPrimitiveType(*this);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
			return System::IConvertible(Plugin::InternalUse::Only, handle);
		}
		return nullptr;
	}
	
	PrimitiveType::operator System::IComparable()
	{
		int32_t handle = Plugin::BoxPrimitiveType(*this);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
			return System::IComparable(Plugin::InternalUse::Only, handle);
		}
		return nullptr;
	}
	
}
const UnityEngine::PrimitiveType UnityEngine::PrimitiveType::Sphere(0);
const UnityEngine::PrimitiveType UnityEngine::PrimitiveType::Capsule(1);
const UnityEngine::PrimitiveType UnityEngine::PrimitiveType::Cylinder(2);
const UnityEngine::PrimitiveType UnityEngine::PrimitiveType::Cube(3);
const UnityEngine::PrimitiveType UnityEngine::PrimitiveType::Plane(4);
const UnityEngine::PrimitiveType UnityEngine::PrimitiveType::Quad(5);

namespace System
{
	Object::operator UnityEngine::PrimitiveType()
	{
		UnityEngine::PrimitiveType returnVal(Plugin::UnboxPrimitiveType(Handle));
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnVal;
	}
}

namespace UnityEngine
{
	Time::Time(decltype(nullptr))
	{
	}
	
	Time::Time(Plugin::InternalUse, int32_t handle)
	{
		Handle = handle;
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
	}
	
	Time::Time(const Time& other)
		: Time(Plugin::InternalUse::Only, other.Handle)
	{
	}
	
	Time::Time(Time&& other)
		: Time(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
	}
	
	Time::~Time()
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
	}
	
	Time& Time::operator=(const Time& other)
	{
		if (this->Handle)
		{
			Plugin::DereferenceManagedClass(this->Handle);
		}
		this->Handle = other.Handle;
		if (this->Handle)
		{
			Plugin::ReferenceManagedClass(this->Handle);
		}
		return *this;
	}
	
	Time& Time::operator=(decltype(nullptr))
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
		return *this;
	}
	
	Time& Time::operator=(Time&& other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
		}
		Handle = other.Handle;
		other.Handle = 0;
		return *this;
	}
	
	bool Time::operator==(const Time& other) const
	{
		return Handle == other.Handle;
	}
	
	bool Time::operator!=(const Time& other) const
	{
		return Handle != other.Handle;
	}
	
	System::Single Time::GetDeltaTime()
	{
		auto returnValue = Plugin::UnityEngineTimePropertyGetDeltaTime();
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnValue;
	}
}

namespace MyGame
{
	AbstractBaseBallScript::AbstractBaseBallScript(decltype(nullptr))
		: UnityEngine::Object(nullptr)
		, UnityEngine::Component(nullptr)
		, UnityEngine::Behaviour(nullptr)
		, UnityEngine::MonoBehaviour(nullptr)
	{
	}
	
	AbstractBaseBallScript::AbstractBaseBallScript(Plugin::InternalUse, int32_t handle)
		: UnityEngine::Object(nullptr)
		, UnityEngine::Component(nullptr)
		, UnityEngine::Behaviour(nullptr)
		, UnityEngine::MonoBehaviour(nullptr)
	{
		Handle = handle;
		if (handle)
		{
			Plugin::ReferenceManagedClass(handle);
		}
	}
	
	AbstractBaseBallScript::AbstractBaseBallScript(const AbstractBaseBallScript& other)
		: AbstractBaseBallScript(Plugin::InternalUse::Only, other.Handle)
	{
	}
	
	AbstractBaseBallScript::AbstractBaseBallScript(AbstractBaseBallScript&& other)
		: AbstractBaseBallScript(Plugin::InternalUse::Only, other.Handle)
	{
		other.Handle = 0;
	}
	
	AbstractBaseBallScript::~AbstractBaseBallScript()
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
	}
	
	AbstractBaseBallScript& AbstractBaseBallScript::operator=(const AbstractBaseBallScript& other)
	{
		if (this->Handle)
		{
			Plugin::DereferenceManagedClass(this->Handle);
		}
		this->Handle = other.Handle;
		if (this->Handle)
		{
			Plugin::ReferenceManagedClass(this->Handle);
		}
		return *this;
	}
	
	AbstractBaseBallScript& AbstractBaseBallScript::operator=(decltype(nullptr))
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
			Handle = 0;
		}
		return *this;
	}
	
	AbstractBaseBallScript& AbstractBaseBallScript::operator=(AbstractBaseBallScript&& other)
	{
		if (Handle)
		{
			Plugin::DereferenceManagedClass(Handle);
		}
		Handle = other.Handle;
		other.Handle = 0;
		return *this;
	}
	
	bool AbstractBaseBallScript::operator==(const AbstractBaseBallScript& other) const
	{
		return Handle == other.Handle;
	}
	
	bool AbstractBaseBallScript::operator!=(const AbstractBaseBallScript& other) const
	{
		return Handle != other.Handle;
	}
}

namespace MyGame
{
	BaseBallScript::BaseBallScript()
		: UnityEngine::Object(nullptr)
		, UnityEngine::Component(nullptr)
		, UnityEngine::Behaviour(nullptr)
		, UnityEngine::MonoBehaviour(nullptr)
		, MyGame::AbstractBaseBallScript(nullptr)
	{
		CppHandle = Plugin::StoreBaseBallScript(this);
		System::Int32* handle = (System::Int32*)&Handle;
		int32_t cppHandle = CppHandle;
		Plugin::BaseBallScriptConstructor(cppHandle, &handle->Value);
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
		else
		{
			Plugin::RemoveBaseBallScript(CppHandle);
			CppHandle = 0;
		}
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
	}
	
	BaseBallScript::BaseBallScript(decltype(nullptr))
		: UnityEngine::Object(nullptr)
		, UnityEngine::Component(nullptr)
		, UnityEngine::Behaviour(nullptr)
		, UnityEngine::MonoBehaviour(nullptr)
		, MyGame::AbstractBaseBallScript(nullptr)
	{
		CppHandle = Plugin::StoreBaseBallScript(this);
	}
	
	BaseBallScript::BaseBallScript(const BaseBallScript& other)
		: UnityEngine::Object(nullptr)
		, UnityEngine::Component(nullptr)
		, UnityEngine::Behaviour(nullptr)
		, UnityEngine::MonoBehaviour(nullptr)
		, MyGame::AbstractBaseBallScript(nullptr)
	{
		Handle = other.Handle;
		CppHandle = Plugin::StoreBaseBallScript(this);
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
	}
	
	BaseBallScript::BaseBallScript(BaseBallScript&& other)
		: UnityEngine::Object(nullptr)
		, UnityEngine::Component(nullptr)
		, UnityEngine::Behaviour(nullptr)
		, UnityEngine::MonoBehaviour(nullptr)
		, MyGame::AbstractBaseBallScript(nullptr)
	{
		Handle = other.Handle;
		CppHandle = other.CppHandle;
		other.Handle = 0;
		other.CppHandle = 0;
	}
	
	BaseBallScript::BaseBallScript(Plugin::InternalUse, int32_t handle)
		: UnityEngine::Object(nullptr)
		, UnityEngine::Component(nullptr)
		, UnityEngine::Behaviour(nullptr)
		, UnityEngine::MonoBehaviour(nullptr)
		, MyGame::AbstractBaseBallScript(nullptr)
	{
		Handle = handle;
		CppHandle = Plugin::StoreBaseBallScript(this);
		if (Handle)
		{
			Plugin::ReferenceManagedClass(Handle);
		}
	}
	
	BaseBallScript::~BaseBallScript()
	{
		Plugin::RemoveWholeBaseBallScript(this);
		Plugin::RemoveBaseBallScript(CppHandle);
		CppHandle = 0;
		if (Handle)
		{
			int32_t handle = Handle;
			Handle = 0;
			if (Plugin::DereferenceManagedClassNoRelease(handle))
			{
				Plugin::ReleaseBaseBallScript(handle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
			}
		}
	}
	
	BaseBallScript& BaseBallScript::operator=(const BaseBallScript& other)
	{
		if (this->Handle)
		{
			Plugin::DereferenceManagedClass(this->Handle);
		}
		this->Handle = other.Handle;
		if (this->Handle)
		{
			Plugin::ReferenceManagedClass(this->Handle);
		}
		return *this;
	}
	
	BaseBallScript& BaseBallScript::operator=(decltype(nullptr))
	{
		if (Handle)
		{
			int32_t handle = Handle;
			Handle = 0;
			if (Plugin::DereferenceManagedClassNoRelease(handle))
			{
				Plugin::ReleaseBaseBallScript(handle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
			}
		}
		Handle = 0;
		return *this;
	}
	
	BaseBallScript& BaseBallScript::operator=(BaseBallScript&& other)
	{
		Plugin::RemoveBaseBallScript(CppHandle);
		CppHandle = 0;
		if (Handle)
		{
			int32_t handle = Handle;
			Handle = 0;
			if (Plugin::DereferenceManagedClassNoRelease(handle))
			{
				Plugin::ReleaseBaseBallScript(handle);
				if (Plugin::unhandledCsharpException)
				{
					System::Exception* ex = Plugin::unhandledCsharpException;
					Plugin::unhandledCsharpException = nullptr;
					ex->ThrowReferenceToThis();
					delete ex;
				}
			}
		}
		Handle = other.Handle;
		other.Handle = 0;
		return *this;
	}
	
	bool BaseBallScript::operator==(const BaseBallScript& other) const
	{
		return Handle == other.Handle;
	}
	
	bool BaseBallScript::operator!=(const BaseBallScript& other) const
	{
		return Handle != other.Handle;
	}
	
	DLLEXPORT int32_t NewBaseBallScript(int32_t handle)
	{
		MyGame::BaseBallScript* memory = Plugin::StoreWholeBaseBallScript();
		MyGame::BallScript* thiz = new (memory) MyGame::BallScript(Plugin::InternalUse::Only, handle);
		return thiz->CppHandle;
	}

	DLLEXPORT void DestroyBaseBallScript(int32_t cppHandle)
	{
		BaseBallScript* instance = Plugin::GetBaseBallScript(cppHandle);
		instance->~BaseBallScript();
	}

	void BaseBallScript::Update()
	{
	}
	
	DLLEXPORT void MyGameAbstractBaseBallScriptUpdate(int32_t cppHandle)
	{
		try
		{
			Plugin::GetBaseBallScript(cppHandle)->Update();
		}
		catch (System::Exception ex)
		{
			Plugin::SetException(ex.Handle);
		}
		catch (...)
		{
			System::String msg = "Unhandled exception invoking MyGame::AbstractBaseBallScript";
			System::Exception ex(msg);
			Plugin::SetException(ex.Handle);
		}
	}
}

namespace System
{
	Object::operator System::Boolean()
	{
		System::Boolean returnVal(Plugin::UnboxBoolean(Handle));
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnVal;
	}
}

namespace System
{
	Object::operator System::SByte()
	{
		System::SByte returnVal(Plugin::UnboxSByte(Handle));
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnVal;
	}
}

namespace System
{
	Object::operator System::Byte()
	{
		System::Byte returnVal(Plugin::UnboxByte(Handle));
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnVal;
	}
}

namespace System
{
	Object::operator System::Int16()
	{
		System::Int16 returnVal(Plugin::UnboxInt16(Handle));
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnVal;
	}
}

namespace System
{
	Object::operator System::UInt16()
	{
		System::UInt16 returnVal(Plugin::UnboxUInt16(Handle));
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnVal;
	}
}

namespace System
{
	Object::operator System::Int32()
	{
		System::Int32 returnVal(Plugin::UnboxInt32(Handle));
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnVal;
	}
}

namespace System
{
	Object::operator System::UInt32()
	{
		System::UInt32 returnVal(Plugin::UnboxUInt32(Handle));
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnVal;
	}
}

namespace System
{
	Object::operator System::Int64()
	{
		System::Int64 returnVal(Plugin::UnboxInt64(Handle));
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnVal;
	}
}

namespace System
{
	Object::operator System::UInt64()
	{
		System::UInt64 returnVal(Plugin::UnboxUInt64(Handle));
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnVal;
	}
}

namespace System
{
	Object::operator System::Char()
	{
		System::Char returnVal(Plugin::UnboxChar(Handle));
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnVal;
	}
}

namespace System
{
	Object::operator System::Single()
	{
		System::Single returnVal(Plugin::UnboxSingle(Handle));
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnVal;
	}
}

namespace System
{
	Object::operator System::Double()
	{
		System::Double returnVal(Plugin::UnboxDouble(Handle));
		if (Plugin::unhandledCsharpException)
		{
			System::Exception* ex = Plugin::unhandledCsharpException;
			Plugin::unhandledCsharpException = nullptr;
			ex->ThrowReferenceToThis();
			delete ex;
		}
		return returnVal;
	}
}

namespace System
{
	struct NullReferenceExceptionThrower : System::NullReferenceException
	{
		NullReferenceExceptionThrower(int32_t handle)
			: System::Runtime::InteropServices::_Exception(nullptr)
			, System::Runtime::Serialization::ISerializable(nullptr)
			, System::Exception(nullptr)
			, System::SystemException(nullptr)
			, System::NullReferenceException(Plugin::InternalUse::Only, handle)
		{
		}
	
		virtual void ThrowReferenceToThis()
		{
			throw *this;
		}
	};
}

DLLEXPORT void SetCsharpExceptionSystemNullReferenceException(int32_t handle)
{
	delete Plugin::unhandledCsharpException;
	Plugin::unhandledCsharpException = new System::NullReferenceExceptionThrower(handle);
}
/*END METHOD DEFINITIONS*/

////////////////////////////////////////////////////////////////
// App-specific functions for this file to call
////////////////////////////////////////////////////////////////

// Called when the plugin is initialized
extern void PluginMain(
	void* memory,
	int32_t memorySize,
	bool isFirstBoot);

////////////////////////////////////////////////////////////////
// C++ functions for C# to call
////////////////////////////////////////////////////////////////

enum class InitMode : uint8_t
{
	FirstBoot,
	Reload
};

// Init the plugin
DLLEXPORT void Init(
	uint8_t* memory,
	int32_t memorySize,
	InitMode initMode,
	void (*releaseObject)(int32_t handle),
	int32_t (*stringNew)(const char* chars),
	void (*setException)(int32_t handle),
	int32_t (*arrayGetLength)(int32_t handle),
	int32_t (*enumerableGetEnumerator)(int32_t handle),
	/*BEGIN INIT PARAMS*/
	int32_t maxManagedObjects,
	UnityEngine::Vector3 (*unityEngineVector3ConstructorSystemSingle_SystemSingle_SystemSingle)(float x, float y, float z),
	UnityEngine::Vector3 (*unityEngineVector3Methodop_AdditionUnityEngineVector3_UnityEngineVector3)(UnityEngine::Vector3& a, UnityEngine::Vector3& b),
	int32_t (*boxVector3)(UnityEngine::Vector3& val),
	UnityEngine::Vector3 (*unboxVector3)(int32_t valHandle),
	int32_t (*unityEngineObjectPropertyGetName)(int32_t thisHandle),
	void (*unityEngineObjectPropertySetName)(int32_t thisHandle, int32_t valueHandle),
	int32_t (*unityEngineComponentPropertyGetTransform)(int32_t thisHandle),
	UnityEngine::Vector3 (*unityEngineTransformPropertyGetPosition)(int32_t thisHandle),
	void (*unityEngineTransformPropertySetPosition)(int32_t thisHandle, UnityEngine::Vector3& value),
	int32_t (*systemCollectionsIEnumeratorPropertyGetCurrent)(int32_t thisHandle),
	int32_t (*systemCollectionsIEnumeratorMethodMoveNext)(int32_t thisHandle),
	int32_t (*unityEngineGameObjectMethodAddComponentMyGameBaseBallScript)(int32_t thisHandle),
	int32_t (*unityEngineGameObjectMethodCreatePrimitiveUnityEnginePrimitiveType)(UnityEngine::PrimitiveType type),
	void (*unityEngineDebugMethodLogSystemObject)(int32_t messageHandle),
	int32_t (*unityEngineMonoBehaviourPropertyGetTransform)(int32_t thisHandle),
	int32_t (*systemExceptionConstructorSystemString)(int32_t messageHandle),
	int32_t (*boxPrimitiveType)(UnityEngine::PrimitiveType val),
	UnityEngine::PrimitiveType (*unboxPrimitiveType)(int32_t valHandle),
	System::Single (*unityEngineTimePropertyGetDeltaTime)(),
	void (*releaseBaseBallScript)(int32_t handle),
	void (*baseBallScriptConstructor)(int32_t cppHandle, int32_t* handle),
	int32_t (*boxBoolean)(uint32_t val),
	int32_t (*unboxBoolean)(int32_t valHandle),
	int32_t (*boxSByte)(int8_t val),
	System::SByte (*unboxSByte)(int32_t valHandle),
	int32_t (*boxByte)(uint8_t val),
	System::Byte (*unboxByte)(int32_t valHandle),
	int32_t (*boxInt16)(int16_t val),
	System::Int16 (*unboxInt16)(int32_t valHandle),
	int32_t (*boxUInt16)(uint16_t val),
	System::UInt16 (*unboxUInt16)(int32_t valHandle),
	int32_t (*boxInt32)(int32_t val),
	System::Int32 (*unboxInt32)(int32_t valHandle),
	int32_t (*boxUInt32)(uint32_t val),
	System::UInt32 (*unboxUInt32)(int32_t valHandle),
	int32_t (*boxInt64)(int64_t val),
	System::Int64 (*unboxInt64)(int32_t valHandle),
	int32_t (*boxUInt64)(uint64_t val),
	System::UInt64 (*unboxUInt64)(int32_t valHandle),
	int32_t (*boxChar)(uint16_t val),
	int16_t (*unboxChar)(int32_t valHandle),
	int32_t (*boxSingle)(float val),
	System::Single (*unboxSingle)(int32_t valHandle),
	int32_t (*boxDouble)(double val),
	System::Double (*unboxDouble)(int32_t valHandle)
	/*END INIT PARAMS*/)
{
	uint8_t* curMemory = memory;
	
	// Init managed object ref counting
	Plugin::RefCountsLenClass = maxManagedObjects;
	Plugin::RefCountsClass = (int32_t*)curMemory;
	curMemory += maxManagedObjects * sizeof(int32_t);
	
	// Init pointers to C# functions
	Plugin::StringNew = stringNew;
	Plugin::ReleaseObject = releaseObject;
	Plugin::SetException = setException;
	Plugin::ArrayGetLength = arrayGetLength;
	Plugin::EnumerableGetEnumerator = enumerableGetEnumerator;
	/*BEGIN INIT BODY*/
	Plugin::UnityEngineVector3ConstructorSystemSingle_SystemSingle_SystemSingle = unityEngineVector3ConstructorSystemSingle_SystemSingle_SystemSingle;
	Plugin::UnityEngineVector3Methodop_AdditionUnityEngineVector3_UnityEngineVector3 = unityEngineVector3Methodop_AdditionUnityEngineVector3_UnityEngineVector3;
	Plugin::BoxVector3 = boxVector3;
	Plugin::UnboxVector3 = unboxVector3;
	Plugin::UnityEngineObjectPropertyGetName = unityEngineObjectPropertyGetName;
	Plugin::UnityEngineObjectPropertySetName = unityEngineObjectPropertySetName;
	Plugin::UnityEngineComponentPropertyGetTransform = unityEngineComponentPropertyGetTransform;
	Plugin::UnityEngineTransformPropertyGetPosition = unityEngineTransformPropertyGetPosition;
	Plugin::UnityEngineTransformPropertySetPosition = unityEngineTransformPropertySetPosition;
	Plugin::SystemCollectionsIEnumeratorPropertyGetCurrent = systemCollectionsIEnumeratorPropertyGetCurrent;
	Plugin::SystemCollectionsIEnumeratorMethodMoveNext = systemCollectionsIEnumeratorMethodMoveNext;
	Plugin::UnityEngineGameObjectMethodAddComponentMyGameBaseBallScript = unityEngineGameObjectMethodAddComponentMyGameBaseBallScript;
	Plugin::UnityEngineGameObjectMethodCreatePrimitiveUnityEnginePrimitiveType = unityEngineGameObjectMethodCreatePrimitiveUnityEnginePrimitiveType;
	Plugin::UnityEngineDebugMethodLogSystemObject = unityEngineDebugMethodLogSystemObject;
	Plugin::UnityEngineMonoBehaviourPropertyGetTransform = unityEngineMonoBehaviourPropertyGetTransform;
	Plugin::SystemExceptionConstructorSystemString = systemExceptionConstructorSystemString;
	Plugin::BoxPrimitiveType = boxPrimitiveType;
	Plugin::UnboxPrimitiveType = unboxPrimitiveType;
	Plugin::UnityEngineTimePropertyGetDeltaTime = unityEngineTimePropertyGetDeltaTime;
	Plugin::BaseBallScriptFreeListSize = 1000;
	Plugin::BaseBallScriptFreeList = (MyGame::BaseBallScript**)curMemory;
	curMemory += 1000 * sizeof(MyGame::BaseBallScript*);
	
	Plugin::ReleaseBaseBallScript = releaseBaseBallScript;
	Plugin::BaseBallScriptConstructor = baseBallScriptConstructor;
	Plugin::BaseBallScriptFreeWholeListSize = 1000;
	Plugin::BaseBallScriptFreeWholeList = (Plugin::BaseBallScriptFreeWholeListEntry*)curMemory;
	curMemory += 1000 * sizeof(Plugin::BaseBallScriptFreeWholeListEntry);
	
	Plugin::BoxBoolean = boxBoolean;
	Plugin::UnboxBoolean = unboxBoolean;
	Plugin::BoxSByte = boxSByte;
	Plugin::UnboxSByte = unboxSByte;
	Plugin::BoxByte = boxByte;
	Plugin::UnboxByte = unboxByte;
	Plugin::BoxInt16 = boxInt16;
	Plugin::UnboxInt16 = unboxInt16;
	Plugin::BoxUInt16 = boxUInt16;
	Plugin::UnboxUInt16 = unboxUInt16;
	Plugin::BoxInt32 = boxInt32;
	Plugin::UnboxInt32 = unboxInt32;
	Plugin::BoxUInt32 = boxUInt32;
	Plugin::UnboxUInt32 = unboxUInt32;
	Plugin::BoxInt64 = boxInt64;
	Plugin::UnboxInt64 = unboxInt64;
	Plugin::BoxUInt64 = boxUInt64;
	Plugin::UnboxUInt64 = unboxUInt64;
	Plugin::BoxChar = boxChar;
	Plugin::UnboxChar = unboxChar;
	Plugin::BoxSingle = boxSingle;
	Plugin::UnboxSingle = unboxSingle;
	Plugin::BoxDouble = boxDouble;
	Plugin::UnboxDouble = unboxDouble;
	/*END INIT BODY*/
	
	// Make sure there was enough memory
	int32_t usedMemory = (int32_t)(curMemory - (uint8_t*)memory);
	if (usedMemory > memorySize)
	{
		System::String msg = "Plugin memory size is too low";
		System::Exception ex(msg);
		Plugin::SetException(ex.Handle);
		return;
	}
	
	if (initMode == InitMode::FirstBoot)
	{
		memset(memory, 0, memorySize);
		
		/*BEGIN INIT BODY FIRST BOOT*/
		for (int32_t i = 0, end = Plugin::BaseBallScriptFreeListSize - 1; i < end; ++i)
		{
			Plugin::BaseBallScriptFreeList[i] = (MyGame::BaseBallScript*)(Plugin::BaseBallScriptFreeList + i + 1);
		}
		Plugin::BaseBallScriptFreeList[Plugin::BaseBallScriptFreeListSize - 1] = nullptr;
		Plugin::NextFreeBaseBallScript = Plugin::BaseBallScriptFreeList + 1;
		
		for (int32_t i = 0, end = Plugin::BaseBallScriptFreeWholeListSize - 1; i < end; ++i)
		{
			Plugin::BaseBallScriptFreeWholeList[i].Next = Plugin::BaseBallScriptFreeWholeList[i + 1].Next;
		}
		Plugin::BaseBallScriptFreeWholeList[Plugin::BaseBallScriptFreeWholeListSize - 1].Next = nullptr;
		Plugin::NextFreeWholeBaseBallScript = Plugin::BaseBallScriptFreeWholeList + 1;
		/*END INIT BODY FIRST BOOT*/
	}
	
	try
	{
		PluginMain(
			curMemory,
			(int32_t)(memorySize - usedMemory),
			initMode == InitMode::FirstBoot);
	}
	catch (System::Exception ex)
	{
		Plugin::SetException(ex.Handle);
	}
	catch (...)
	{
		System::String msg = "Unhandled exception in PluginMain";
		System::Exception ex(msg);
		Plugin::SetException(ex.Handle);
	}
}

// Receive an unhandled exception from C#
DLLEXPORT void SetCsharpException(int32_t handle)
{
	Plugin::unhandledCsharpException = new System::Exception(
		Plugin::InternalUse::Only,
		handle);
}


# Unity Native Scripting

A library to allow writing Unity scripts in native code: C, C++, assembly.

# Reasons to Prefer C++ Over C# #

By using C++ directly, you gain complete control over the code the CPU will execute. It's much easier to generate optimal code with a C++ compiler than with a C# compiler, IL2CPP, and finally a C++ compiler. You can even code with compiler intrinsics or assembly to directly write machine code and take advantage of CPU features like [SIMD](http://jacksondunstan.com/articles/3890) and hardware AES encryption for massive performance gains.

C++ is also a much larger language than C# and some developers will prefer having more tools at their disposal. Here are a few differences:

* Its template system is much more powerful than C# generics
* There are macros for extreme flexibility by generating code
* Function pointers instead of just delegates
* No-overhead [algorithms](http://en.cppreference.com/w/cpp/algorithm) instead of LINQ
* Bit fields for easy memory savings
* Pointers and never-null references instead of just managed referneces
* Much more. C++ is huge.

There are also some problems with C# code running under Unity that you'll automatically avoid. For one, Unity's garbage collector is very slow. It runs on the main thread, which blocks rendering and input handling. It collects all objects at once, which causes frame hitches. And it fragments memory, so eventually you may run out and crash.

A significant amount of effort is required to work around the GC and the resulting code is difficult to maintain and slow. This includes techniques like [object pools](http://jacksondunstan.com/articles/3829), which essentially make memory management manual. You've also got to avoid boxing value types to managed types, `foreach` loops in some situations, and various other [gotchas](http://jacksondunstan.com/articles/3850).

C++ has no required garbage collector and features optional automatic memory management via "smart pointer" types like [shared_ptr](http://en.cppreference.com/w/cpp/memory/shared_ptr). It offers an excellent alternative to Unity's primitive garbage collector.

While IL2CPP transforms C# into C++ already, it generates a lot of overhead. There are many [surprises](http://jacksondunstan.com/articles/3916) if you read through the generated C++. For example, there's overhead for any function using a static variable and an extra two pointers are stored at the beginning of every class. The same goes for all sorts of features such as `sizeof()`, mandatory null checks, and so forth. Instead, you could write C++ directly and not need to work around IL2CPP.

This project aims to give you a viable alternative to C#. Scripting in C++ isn't right for all parts of every project, but now it's an option.

# Features

* Supports Windows, macOS, iOS, and Android (editor and standalone)
* Plays nice with other C# scripts- no need to use 100% C++
* Object-oriented API just like in C#

>
	GameObject go;
	Transform transform = go.GetTransform();
	Vector3 position(1.0f, 2.0f, 3.0f);
	transform.SetPosition(position);

* No need to reload the Unity editor when changing C++
* Code generator exposes any C# API (Unity, .NET, custom DLLs) with a simple JSON config file
* Handle `MonoBehaviour` messages in C++

>
	void MyScript::Start()
	{
		Debug::Log(String("MyScript has started"));
	}

* Platform-dependent compilation via the [usual flags](https://docs.unity3d.com/Manual/PlatformDependentCompilation.html) (e.g. `#if UNITY_EDITOR`)
* [CMake](https://cmake.org/) build system sets up any IDE project or command-line build

# Performance

[Article](http://jacksondunstan.com/articles/3952).

tl;dr - Most projects will not be noticeably impacted by C++ overhead and many projects will benefit from reducing garbage collection and IL2CPP overhead.

# Project Structure

When scripting in C++, C# is used only as a "binding" layer so Unity can call C++ functions and C++ functions can call the Unity API. A code generator is used to generate most of these bindings according to the needs of your project.

All of your code, plus a few bindings, will exist in a single "native" C++ plugin. When you change your C++ code, you'll build this plugin and then play the game in the editor or in a deployed build (e.g. to an Android device). There won't be any C# code for Unity to compile unless you run the code generator, which is infrequent.

The standard C# workflow looks like this:

1. Edit C# code in a C# IDE like MonoDevelop
2. Switch to the Unity editor window
3. Wait for the compile to finish (slow, "real" games take 5+ seconds)
4. Run the game

With C++, the workflow looks like this:

1. Edit C++ code in a C++ IDE like Xcode or Visual Studio
2. Build the C++ plugin (extremely fast, often under 1 second)
3. Switch to the Unity editor window. Nothing to compile.
4. Run the game

One of the project's goals is to make it just as easy to work with C++ as it is to work with C#, if not easier.

# Getting Started

1. Download or clone this repo
2. Copy everything in `Unity/Assets` directory to your Unity project's `Assets` directory
3. Copy the `Unity/CppSource` directory to your Unity project directory
4. Edit `NativeScriptTypes.json` and specify what parts of the Unity API you want access to from C++. Some examples are provided, but feel free to delete them if you're not using those features.
5. Edit `Unity/CppSource/Game/Game.cpp` to create your game. Some example code is provided, but feel free to delete it. You can add more C++ source (`.cpp`) and header (`.h`) files here as your game grows.

# Building the C++ Plugin

## iOS

1. Install [CMake](https://cmake.org/) version 3.6 or greater
2. Create a directory for build files. Anywhere is fine.
3. Open the Terminal app in `/Applications/Utilities`
4. Execute `cd /path/to/your/build/directory`
5. Execute `cmake -G MyGenerator -DCMAKE_TOOLCHAIN_FILE=/path/to/your/project/CppSource/iOS.cmake /path/to/your/project/CppSource`. Replace `MyGenerator` with the generator of your choice. To see the options, execute `cmake --help` and look at the list at the bottom. Common choices include "Unix Makefiles" to build from command line or "Xcode" to use Apple's IDE.
6. The build scripts or IDE project files are now generated in your build directory
7. Build as appropriate for your generator. For example, execute `make` if you chose `Unix Makefiles` as your generator or open `NativeScript.xcodeproj` and click `Product > Build` if you chose Xcode.

## macOS (Editor and Standalone)

1. Install [CMake](https://cmake.org/) version 3.6 or greater
2. Create a directory for build files. Anywhere is fine.
3. Open the Terminal app in `/Applications/Utilities`
4. Execute `cd /path/to/your/build/directory`
5. Execute `cmake -G "MyGenerator" -DEDITOR=TRUE /path/to/your/project/CppSource`. Replace `MyGenerator` with the generator of your choice. To see the options, execute `cmake --help` and look at the list at the bottom. Common choices include "Unix Makefiles" to build from command line or "Xcode" to use Apple's IDE. Remove `-DEDITOR=TRUE` for standalone builds.
6. The build scripts or IDE project files are now generated in your build directory
7. Build as appropriate for your generator. For example, execute `make` if you chose `Unix Makefiles` as your generator or open `NativeScript.xcodeproj` and click `Product > Build` if you chose Xcode.

## Windows (Editor and Standalone)

1. Install [CMake](https://cmake.org/) version 3.6 or greater
2. Create a directory for build files. Anywhere is fine.
3. Open a Command Prompt by clicking the Start button, typing "Command Prompt", then clicking the app
4. Execute `cd /path/to/your/build/directory`
5. Execute `cmake -G "Visual Studio VERSION YEAR Win64" -DEDITOR=TRUE /path/to/your/project/CppSource`. Replace `VERSION` and `YEAR` with the version of Visual Studio you want to use. To see the options, execute `cmake --help` and look at the list at the bottom. For example, use `"`Visual Studio 15 2017 Win64` for Visual Studio 2017. Any version, including Community, works just fine. Remove `-DEDITOR=TRUE` for standalone builds.
6. The project files are now generated in your build directory
7. Open `NativeScript.sln` and click `Build > Build Solution`.

## Linux (Editor and Standalone)

1. Install [CMake](https://cmake.org/) version 3.6 or greater
2. Create a directory for build files. Anywhere is fine.
3. Open a terminal as appropriate for your Linux distribution
4. Execute `cd /path/to/your/build/directory`
5. Execute `cmake -G "MyGenerator" -DEDITOR=TRUE /path/to/your/project/CppSource`. Replace `MyGenerator` with the generator of your choice. To see the options, execute `cmake --help` and look at the list at the bottom. The most common choice is "Unix Makefiles" to build from command line, but there are IDE options too. Remove `-DEDITOR=TRUE` for standalone builds.
6. The build scripts or IDE project files are now generated in your build directory
7. Build as appropriate for your generator. For example, execute `make` if you chose `Unix Makefiles` as your generator.

## Android

1. Install [CMake](https://cmake.org/) version 3.6 or greater
2. Create a directory for build files. Anywhere is fine.
3. Open a terminal (macOS, Linux) or Command Prompt (Windows)
4. Execute `cd /path/to/your/build/directory`
5. Execute `cmake -G MyGenerator -DANDROID_NDK=/path/to/android/ndk /path/to/your/project/CppSource`. Replace `MyGenerator` with the generator of your choice. To see the options, execute `cmake --help` and look at the list at the bottom. To make a build for any platform other than Android, omit the `-DANDROID_NDK=/path/to/android/ndk` part.
6. The build scripts or IDE project files are now generated in your build directory
7. Build as appropriate for your generator. For example, execute `make` if you chose `Unix Makefiles` as your generator.

# The Code Generator

To run the code generator, choose `NativeScript > Generate Bindings` from the Unity editor.

To configure the code generator, open `NativeScriptTypes.json` and notice the existing examples. Add on to this file to expose more C# APIs from Unity, .NET, or custom DLLs to your C++ code.

The code generator supports:

* Class types (including generics)
* Struct types (including generics)
* Base classes (including generics)
* Constructors (including generic parameters)
* Methods (including generic parameters and return types)
* Fields (including generic types)
* Properties (getters and setters) (including generic types)
* `MonoBehaviour` classes with "message" functions like `Update` (except `OnAudioFilterRead`)
* `out` and `ref` parameters
* Enumerations
* Exceptions
* Overloaded operators (except `operator true` and `operator false`)

The code generator does not support (yet):

* Arrays (single- or multi-dimensional)
* Delegates
* `MonoBehaviour` contents (e.g. fields) except for "message" functions
* Default parameters
* Interfaces
* `decimal`
* Pointers

# Updating To A New Version

To update to a new version of this project, overwrite your Unity project's `Assets/NativeScript` directory with this project's `Unity/Assets/NativeScript` directory and re-run the code generator.

# Reference

[Articles](http://jacksondunstan.com/articles/3938) by the author describing the development of this project.

# Author

[Jackson Dunstan](http://jacksondunstan.com)

# Contributing

Please feel free to fork and send [pull requests](https://github.com/jacksondunstan/UnityNativeScripting/pulls) or simply submit an [issue](https://github.com/jacksondunstan/UnityNativeScripting/issues) for features or bug fixes.

# License

All code is licensed [MIT](https://opensource.org/licenses/MIT), which means it can usually be easily used in commercial and non-commercial applications.

All writing is licensed [CC BY 4.0](https://creativecommons.org/licenses/by/4.0/), which means it can be used as long as attribution is given.
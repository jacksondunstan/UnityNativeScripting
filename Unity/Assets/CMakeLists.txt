cmake_minimum_required(VERSION 3.6.0 FATAL_ERROR)
project(NativeScript CXX)

# Set platform-dependent compilation defines matching C#
if (EDITOR)
	add_definitions(-DUNITY_EDITOR)
	if (WIN32)
		add_definitions(-DUNITY_EDITOR_WIN)
	elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
		add_definitions(-DUNITY_EDITOR_OSX)
	elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
		add_definitions(-DUNITY_EDITOR_LINUX)
	endif()
else()
	add_definitions(-DUNITY_STANDALONE)
	if (WIN32)
		add_definitions(-DUNITY_STANDALONE_WIN)
	elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
		add_definitions(-DUNITY_STANDALONE_OSX)
	elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
		add_definitions(-DUNITY_STANDALONE_LINUX)
	endif()
endif()
if (IOS)
	add_definitions(-DUNITY_IOS)
endif()
if (ANDROID_NDK)
	add_definitions(-DUNITY_ANDROID)
endif()

# Use NDK on Android
if (ANDROID_NDK)
	set(ANDROID_ABI armeabi-v7a)
	set(CMAKE_TOOLCHAIN_FILE ${ANDROID_NDK}/build/cmake/android.toolchain.cmake)
endif()

# Set output path
if (ANDROID_NDK)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Plugins/Android)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/Plugins/Android)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/Plugins/Android)
elseif (IOS)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Plugins/iOS)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/Plugins/iOS)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/Plugins/iOS)
elseif (WIN32 OR (${CMAKE_SYSTEM_NAME} MATCHES "Darwin") OR (${CMAKE_SYSTEM_NAME} MATCHES "Linux"))
	if (EDITOR)
		set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Plugins/Editor)
		set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/Plugins/Editor)
		set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/Plugins/Editor)
	else()
		set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Plugins)
		set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/Plugins)
		set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/Plugins)
	endif()
endif()

# Set the directories for NativeScript and game C++ as include directories
include_directories(
	${CMAKE_SOURCE_DIR}/NativeScript
	${CMAKE_SOURCE_DIR}/CppSource)

# Set all .cpp files for NativeScript and the game as sources
file(GLOB GAMESOURCEFILES ${CMAKE_SOURCE_DIR}/CppSource/*.cpp)
file(GLOB GAMEHEADERFILES ${CMAKE_SOURCE_DIR}/CppSource/*.h)
file(GLOB NATIVESCRIPTSOURCEFILES ${CMAKE_SOURCE_DIR}/NativeScript/*.cpp)
file(GLOB NATIVESCRIPTHEADERFILES ${CMAKE_SOURCE_DIR}/NativeScript/*.h)
set(
	SOURCES
	${GAMESOURCEFILES}
	${GAMEHEADERFILES}
	${NATIVESCRIPTSOURCEFILES}
	${NATIVESCRIPTHEADERFILES})

# Build a library. If on an Apple platform, build it in a bundle.
add_library(${PROJECT_NAME} MODULE ${SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES BUNDLE TRUE)

# Enable C++11
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)
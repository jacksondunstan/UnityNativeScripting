cmake_minimum_required( VERSION 3.6.0 FATAL_ERROR )
set( CMAKE_INCLUDE_CURRENT_DIR ON )
if ( ANDROID_NDK )
	set( ANDROID_ABI armeabi-v7a )
	set( CMAKE_TOOLCHAIN_FILE ${ANDROID_NDK}/build/cmake/android.toolchain.cmake )
	set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Plugins/Android )
	set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/Plugins/Android )
	set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/Plugins/Android )
else()
	set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Plugins )
	set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/Plugins )
	set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/Plugins )
endif( )
include_directories( ${CMAKE_SOURCE_DIR}/NativeScript ${CMAKE_SOURCE_DIR}/CppSource )
project( NativeScript CXX )
file(GLOB GAMESOURCEFILES ${CMAKE_SOURCE_DIR}/CppSource/*.cpp)
set( SOURCES
	${GAMESOURCEFILES}
	NativeScript/Bindings.h
	NativeScript/Bindings.cpp )
add_library( ${PROJECT_NAME} MODULE ${SOURCES} )
set_target_properties( ${PROJECT_NAME} PROPERTIES BUNDLE TRUE )

# Enable C++11
target_compile_features( ${PROJECT_NAME} PRIVATE cxx_range_for )

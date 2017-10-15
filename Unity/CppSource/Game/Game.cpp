/// <summary>
/// Game-specific code for the native plugin
/// </summary>
/// <author>
/// Jackson Dunstan, 2017, http://JacksonDunstan.com
/// </author>
/// <license>
/// MIT
/// </license>

#include "Bindings.h"

using namespace System;
using namespace UnityEngine;

void PrintPlatformDefines();

// Called when the plugin is initialized
// This is mostly full of test code. Feel free to remove it all.
void PluginMain()
{
	PrintPlatformDefines();
	Debug::Log(String("Game booted up"));
	
	GameObject go(String("GameObject with a TestScript"));
	go.AddComponent<MyGame::MonoBehaviours::TestScript>();
}

void MyGame::MonoBehaviours::TestScript::Awake()
{
	Debug::Log(String("C++ TestScript Awake"));
}

void MyGame::MonoBehaviours::TestScript::OnAnimatorIK(int32_t param0)
{
	Debug::Log(String("C++ TestScript OnAnimatorIK"));
}

void MyGame::MonoBehaviours::TestScript::OnCollisionEnter(UnityEngine::Collision param0)
{
	Debug::Log(String("C++ TestScript OnCollisionEnter"));
}

void MyGame::MonoBehaviours::TestScript::Update()
{
	static int32_t numCreated = 0;
	if (numCreated < 10)
	{
		GameObject go;
		Transform transform = go.GetTransform();
		float comp = (float)numCreated;
		Vector3 position(comp, comp*10.0f, comp*100.0f);
		transform.SetPosition(position);
		numCreated++;
		if (numCreated == 10)
		{
			Debug::Log(String("Done spawning game objects"));
		}
	}
}

void PrintPlatformDefines()
{
#if defined(UNITY_EDITOR)
	Debug::Log(String("UNITY_EDITOR"));
#endif
#if defined(UNITY_STANDALONE)
	Debug::Log(String("UNITY_STANDALONE"));
#endif
#if defined(UNITY_IOS)
	Debug::Log(String("UNITY_IOS"));
#endif
#if defined(UNITY_ANDROID)
	Debug::Log(String("UNITY_ANDROID"));
#endif
#if defined(UNITY_EDITOR_WIN)
	Debug::Log(String("UNITY_EDITOR_WIN"));
#endif
#if defined(UNITY_EDITOR_OSX)
	Debug::Log(String("UNITY_EDITOR_OSX"));
#endif
#if defined(UNITY_EDITOR_LINUX)
	Debug::Log(String("UNITY_EDITOR_LINUX"));
#endif
#if defined(UNITY_STANDALONE_OSX)
	Debug::Log(String("UNITY_STANDALONE_OSX"));
#endif
#if defined(UNITY_STANDALONE_WIN)
	Debug::Log(String("UNITY_STANDALONE_WIN"));
#endif
#if defined(UNITY_STANDALONE_LINUX)
	Debug::Log(String("UNITY_STANDALONE_LINUX"));
#endif
}

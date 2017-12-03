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

// Called when the plugin is initialized
// This is mostly full of test code. Feel free to remove it all.
void PluginMain()
{
	String message("Game booted up");
	Debug::Log(message);
	
	String name("GameObject with a TestScript");
	GameObject go(name);
	go.AddComponent<MyGame::MonoBehaviours::TestScript>();
}

void MyGame::MonoBehaviours::TestScript::Awake()
{
	String message("C++ TestScript Awake");
	Debug::Log(message);
}

void MyGame::MonoBehaviours::TestScript::OnAnimatorIK(int32_t param0)
{
	String message("C++ TestScript OnAnimatorIK");
	Debug::Log(message);
}

void MyGame::MonoBehaviours::TestScript::OnCollisionEnter(UnityEngine::Collision& param0)
{
	String message("C++ TestScript OnCollisionEnter");
	Debug::Log(message);
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
			String message("Done spawning game objects");
			Debug::Log(message);
			
			GameObject go = GameObject::CreatePrimitive(PrimitiveType::Sphere);
			String name("GameObject with an AnotherScript");
			go.SetName(name);
			go.AddComponent<MyGame::MonoBehaviours::AnotherScript>();
		}
	}
}

void MyGame::MonoBehaviours::AnotherScript::Awake()
{
	String message("C++ AnotherScript Awake");
	Debug::Log(message);
}

void MyGame::MonoBehaviours::AnotherScript::Update()
{
	Transform transform = GetTransform();
	Vector3 pos = transform.GetPosition();
	const float speed = 1.2f;
	static float dir = 1.0f;
	static float min = -1.5f;
	static float max = 1.5f;
	Vector3 offset(Time::GetDeltaTime() * speed * dir, 0, 0);
	Vector3 newPos = pos + offset;
	if (newPos.x > max)
	{
		dir = -dir;
		newPos.x = max - (newPos.x - max);
	}
	else if (newPos.x < min)
	{
		dir = -dir;
		newPos.x = min + (min - newPos.x);
	}
	transform.SetPosition(newPos);
}

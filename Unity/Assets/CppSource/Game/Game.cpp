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
#include "Game.h"

using namespace System;
using namespace UnityEngine;

namespace
{
	struct GameState
	{
		float BallDir;
	};
	
	GameState* gameState;
}

namespace MyGame
{
	void BallScript::Update()
	{
		Transform transform = GetTransform();
		Vector3 pos = transform.GetPosition();
		const float speed = 1.2f;
		const float min = -1.5f;
		const float max = 1.5f;
		float distance = Time::GetDeltaTime() * speed * gameState->BallDir;
		Vector3 offset(distance, 0, 0);
		Vector3 newPos = pos + offset;
		if (newPos.x > max)
		{
			gameState->BallDir *= -1.0f;
			newPos.x = max - (newPos.x - max);
			if (newPos.x < min)
			{
				newPos.x = min;
			}
		}
		else if (newPos.x < min)
		{
			gameState->BallDir *= -1.0f;
			newPos.x = min + (min - newPos.x);
			if (newPos.x > max)
			{
				newPos.x = max;
			}
		}
		transform.SetPosition(newPos);
	}
}

// Called when the plugin is initialized
// This is mostly full of test code. Feel free to remove it all.
void PluginMain(
	void* memory,
	int32_t memorySize,
	bool isFirstBoot)
{
	gameState = (GameState*)memory;
	if (isFirstBoot)
	{
		String message("Game booted up");
		Debug::Log(message);
		
		// The ball initially goes right
		gameState->BallDir = 1.0f;
		
		// Create the ball game object out of a sphere primitive
		GameObject go = GameObject::CreatePrimitive(PrimitiveType::Sphere);
		String name("GameObject with a BallScript");
		go.SetName(name);
		
		// Attach the ball script to make it bounce back and forth
		go.AddComponent<MyGame::BaseBallScript>();
	}
}

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
void PluginMain()
{
	Debug::Log(String("Game booted up"));
}

// Called for MonoBehaviour.Update
void PluginUpdate()
{
	static int32_t numCreated = 0;
	if (numCreated < 10)
	{
		GameObject go;
		Transform transform = go.GetTransform();
		float comp = (float)numCreated;
		Vector3 position(comp, comp, comp);
		transform.SetPosition(position);
		numCreated++;
		if (numCreated == 10)
		{
			Debug::Log(String("Done spawning game objects"));
		}
	}
}

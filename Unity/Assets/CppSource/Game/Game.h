/// <summary>
/// Declaration of the game types the bindings layer needs to know about
/// </summary>
/// <author>
/// Jackson Dunstan, 2018, http://JacksonDunstan.com
/// </author>
/// <license>
/// MIT
/// </license>

#pragma once

#include "Bindings.h"

namespace MyGame
{
	struct BallScript : MyGame::BaseBallScript
	{
		MY_GAME_BALL_SCRIPT_DEFAULT_CONTENTS
		MY_GAME_BALL_SCRIPT_DEFAULT_CONSTRUCTOR
		void Update() override;
	};
}

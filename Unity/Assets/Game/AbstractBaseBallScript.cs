using UnityEngine;

namespace MyGame
{
	/// <summary>
	/// Base class of a script used in the example code to make a "ball" bounce
	/// back and forth on the screen
	/// </summary>
	/// <author>
	/// Jackson Dunstan, 2018, http://JacksonDunstan.com
	/// </author>
	/// <license>
	/// MIT
	/// </license>
	public abstract class AbstractBaseBallScript : MonoBehaviour
	{
		public abstract void Update();
	}
}

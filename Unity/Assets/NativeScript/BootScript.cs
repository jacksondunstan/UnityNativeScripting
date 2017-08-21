using UnityEngine;

namespace NativeScript
{
	/// <summary>
	/// Script to run at app startup that initializes and runs the native plugin
	/// </summary>
	/// <author>
	/// Jackson Dunstan, 2017, http://JacksonDunstan.com
	/// </author>
	/// <license>
	/// MIT
	/// </license>
	class BootScript : MonoBehaviour
	{
		void Awake()
		{
			DontDestroyOnLoad(gameObject);
			Bindings.Open();
		}
		
		void OnApplicationQuit()
		{
			Bindings.Close();
		}
	}
}
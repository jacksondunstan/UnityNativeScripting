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
		public int MaxManagedObjects = 1024;
		
		void Awake()
		{
			DontDestroyOnLoad(gameObject);
			Bindings.Open(MaxManagedObjects);
		}
		
		void OnApplicationQuit()
		{
			Bindings.Close();
		}
	}
}
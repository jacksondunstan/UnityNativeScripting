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
		Bindings bindings;
		
		void Awake()
		{
			DontDestroyOnLoad(gameObject);
			bindings = new Bindings();
			bindings.Open();
		}
		
		void Update()
		{
			bindings.MonoBehaviourUpdate();
		}

		void OnApplicationQuit()
		{
			bindings.Close();
		}
	}
}
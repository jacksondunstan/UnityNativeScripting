using UnityEngine;

namespace NativeScript
{
	/// <summary>
	/// Script to run at app startup that initializes and runs the native plugin
	/// </summary>
	/// 
	/// <author>
	/// Jackson Dunstan, 2017, http://JacksonDunstan.com
	/// </author>
	/// 
	/// <license>
	/// MIT
	/// </license>
	public class BootScript : MonoBehaviour
	{
		public int MemorySize = 1024 * 1024 * 16;
		
		// Reloading requires dynamic loading of the C++ plugin, which is only
		// available in the editor
#if UNITY_EDITOR
		public bool AutoReload;
		
		public float AutoReloadPollTime = 1.0f;
		private float lastAutoReloadPollTime;
		private Coroutine autoReloadCoroutine;
#endif
		
		void Start()
		{
#if UNITY_EDITOR
			lastAutoReloadPollTime = AutoReloadPollTime;
#endif
			DontDestroyOnLoad(gameObject);
			Bindings.Open(MemorySize);
		}
		
#if UNITY_EDITOR
		void Update()
		{
			Bindings.Update();

			if (AutoReload)
			{
				if (AutoReloadPollTime > 0)
				{
					// Not started yet. Start.
					if (autoReloadCoroutine == null)
					{
						lastAutoReloadPollTime = AutoReloadPollTime;
						autoReloadCoroutine = StartCoroutine(
							Bindings.AutoReload(
								AutoReloadPollTime));
					}
					// Poll time changed. Restart.
					else if (AutoReloadPollTime != lastAutoReloadPollTime)
					{
						StopCoroutine(autoReloadCoroutine);
						lastAutoReloadPollTime = AutoReloadPollTime;
						autoReloadCoroutine = StartCoroutine(
							Bindings.AutoReload(
								AutoReloadPollTime));
					}
				}
			}
			else
			{
				// Not stopped yet. Stop.
				if (autoReloadCoroutine != null)
				{
					StopCoroutine(autoReloadCoroutine);
					autoReloadCoroutine = null;
				}
			}
		}
#endif
		
		void OnApplicationQuit()
		{
			Bindings.Close();
		}
	}
}
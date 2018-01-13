using UnityEngine;
using UnityEditor;

namespace NativeScript
{
	/// <summary>
	/// Menus for the Unity Editor
	/// </summary>
	/// 
	/// <author>
	/// Jackson Dunstan, 2018, http://JacksonDunstan.com
	/// </author>
	/// 
	/// <license>
	/// MIT
	/// </license>
	public static class EditorMenus
	{
		[MenuItem("NativeScript/Generate Bindings #%g")]
		public static void Generate()
		{
			GenerateBindings.Generate();
		}
		
		[MenuItem("NativeScript/Reload Plugin #%r")]
		public static void Reload()
		{
			Bindings.Reload();
		}
	}
}

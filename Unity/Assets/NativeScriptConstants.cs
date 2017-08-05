/// <summary>
/// Constants used by the C++ scripting system. Redefine these with values
/// specific to your project.
/// </summary>
/// <author>
/// Jackson Dunstan, 2017, http://JacksonDunstan.com
/// </author>
/// <license>
/// MIT
/// </license>
public static class NativeScriptConstants
{
	/// <summary>
	/// Name of the plugin used by [DllImport] when running outside the editor
	/// </summary>
	public const string PluginName = "NativeScript";
	
	/// <summary>
	/// Path to load the plugin from when running inside the editor
	/// </summary>
#if UNITY_EDITOR_OSX
	public const string PluginPath = "/NativeScript.bundle/Contents/MacOS/NativeScript";
#elif UNITY_EDITOR_LINUX
	public const string PluginPath = "/NativeScript.so";
#elif UNITY_EDITOR_WIN
	public const string PluginPath = "/NativeScript.dll";
#endif
	
	/// <summary>
	/// Maximum number of simultaneous managed objects that the C++ plugin uses
	/// </summary>
	public const int MaxManagedObjects = 1024;
	
	/// <summary>
	/// Path within the Unity project to the Bindings JSON file
	/// </summary>
	public const string BindingsJsonPath = "Bindings.json";
}
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
	/// Path within the Unity project to the exposed types JSON file
	/// </summary>
	public const string JSON_CONFIG_PATH = "NativeScriptTypes.json";
}
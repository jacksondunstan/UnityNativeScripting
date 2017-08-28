using System;
using System.IO;
using System.Reflection;
using System.Text;

using UnityEditor;
using UnityEngine;

namespace NativeScript
{
	/// <summary>
	/// Code generator that reads a JSON file and outputs C# and C++ code
	/// bindings so C++ can call managed functions.
	/// 
	/// Supports:
	/// * Constructors
	/// * Properties (get and set)
	/// * Fields
	/// * Methods
	/// * Class types (static and regular)
	/// 
	/// Does Not Support:
	/// * Arrays (single- or multi-dimensional)
	/// * out or ref parameters
	/// * Struct types
	/// * Generic functions
	/// * Generic types
	/// * Delegates
	/// 
	/// TODO:
	/// * Prefix binding function names with namespaces
	/// </summary>
	/// <author>
	/// Jackson Dunstan, 2017, http://JacksonDunstan.com
	/// </author>
	/// <license>
	/// MIT
	/// </license>
	public static class GenerateBindings
	{
		// Disable unused field types. JsonUtility actually uses them, but it
		// does so with reflection.
		#pragma warning disable CS0649
		
		[Serializable]
		private class JsonConstructor
		{
			public string[] Types;
		}
		
		[Serializable]
		private class JsonGenericType
		{
			public string Name;
			public string Type;
		}
		
		[Serializable]
		private class JsonMethod
		{
			public string Name;
			public string ReturnType;
			public string[] ParamTypes;
			public JsonGenericType[] GenericTypes;
		}
		
		[Serializable]
		private class JsonType
		{
			public string Name;
			public JsonConstructor[] Constructors;
			public JsonMethod[] Methods;
			public string[] Properties;
			public string[] Fields;
		}
		
		[Serializable]
		private class JsonAssembly
		{
			public string Path;
			public JsonType[] Types;
		}
		
		[Serializable]
		private class JsonMonoBehaviour
		{
			public string Name;
			public string Namespace;
			public string[] Messages;
		}
		
		[Serializable]
		private class JsonDocument
		{
			public JsonAssembly[] Assemblies;
			public JsonMonoBehaviour[] MonoBehaviours;
		}
		
		private class StringBuilders
		{
			public StringBuilder CsharpInitParams = new StringBuilder();
			public StringBuilder CsharpDelegateTypes = new StringBuilder();
			public StringBuilder CsharpInitCall = new StringBuilder();
			public StringBuilder CsharpFunctions = new StringBuilder();
			public StringBuilder CsharpMonoBehaviours = new StringBuilder();
			public StringBuilder CsharpMonoBehaviourDelegates = new StringBuilder();
			public StringBuilder CsharpMonoBehaviourImports = new StringBuilder();
			public StringBuilder CsharpMonoBehaviourGetDelegateCalls = new StringBuilder();
			public StringBuilder CppFunctionPointers = new StringBuilder();
			public StringBuilder CppTypeDeclarations = new StringBuilder();
			public StringBuilder CppTypeDefinitions = new StringBuilder();
			public StringBuilder CppMethodDefinitions = new StringBuilder();
			public StringBuilder CppInitParams = new StringBuilder();
			public StringBuilder CppInitBody = new StringBuilder();
			public StringBuilder CppMonoBehaviourMessages = new StringBuilder();
			public StringBuilder TempStrBuilder = new StringBuilder();
		}
		
		private class ParameterInfo
		{
			public string Name;
			public Type ParameterType;
		}
		
		private class MessageInfo
		{
			public string Name;
			public Type[] ParameterTypes;
			public bool Selected;
			
			public MessageInfo(
				string name,
				params Type[] parameterTypes)
			{
				Name = name;
				ParameterTypes = parameterTypes;
			}
		}
		
		private static readonly MessageInfo[] messageInfos = new[] {
			new MessageInfo("Awake"),
			new MessageInfo("FixedUpdate"),
			new MessageInfo("LateUpdate"),
			new MessageInfo("OnAnimatorIK",typeof(int)),
			new MessageInfo("OnAnimatorMove"),
			new MessageInfo("OnApplicationFocus",typeof(bool)),
			new MessageInfo("OnApplicationPause",typeof(bool)),
			new MessageInfo("OnApplicationQuit"),
			// TODO re-enable when arrays are supported
			// new MessageInfo("OnAudioFilterRead", typeof(float[]), typeof(int)),
			new MessageInfo("OnBecameInvisible"),
			new MessageInfo("OnBecameVisible"),
			new MessageInfo("OnCollisionEnter", typeof(Collision)),
			new MessageInfo("OnCollisionEnter2D", typeof(Collision2D)),
			new MessageInfo("OnCollisionExit", typeof(Collision)),
			new MessageInfo("OnCollisionExit2D", typeof(Collision2D)),
			new MessageInfo("OnCollisionStay", typeof(Collision)),
			new MessageInfo("OnCollisionStay2D", typeof(Collision2D)),
			new MessageInfo("OnConnectedToServer"),
			new MessageInfo("OnControllerColliderHit", typeof(ControllerColliderHit)),
			new MessageInfo("OnDestroy"),
			new MessageInfo("OnDisable"),
			new MessageInfo("OnDisconnectedFromServer", typeof(NetworkDisconnection)),
			new MessageInfo("OnDrawGizmos"),
			new MessageInfo("OnDrawGizmosSelected"),
			new MessageInfo("OnEnable"),
			new MessageInfo("OnFailedToConnect", typeof(NetworkConnectionError)),
			new MessageInfo("OnFailedToConnectToMasterServer", typeof(NetworkConnectionError)),
			new MessageInfo("OnGUI"),
			new MessageInfo("OnJointBreak", typeof(float)),
			new MessageInfo("OnJointBreak2D", typeof(Joint2D)),
			new MessageInfo("OnMasterServerEvent", typeof(MasterServerEvent)),
			new MessageInfo("OnMouseDown"),
			new MessageInfo("OnMouseDrag"),
			new MessageInfo("OnMouseEnter"),
			new MessageInfo("OnMouseExit"),
			new MessageInfo("OnMouseOver"),
			new MessageInfo("OnMouseUp"),
			new MessageInfo("OnMouseUpAsButton"),
			new MessageInfo("OnNetworkInstantiate", typeof(NetworkMessageInfo)),
			new MessageInfo("OnParticleCollision", typeof(GameObject)),
			new MessageInfo("OnParticleTrigger"),
			new MessageInfo("OnPlayerConnected", typeof(NetworkPlayer)),
			new MessageInfo("OnPlayerDisconnected", typeof(NetworkPlayer)),
			new MessageInfo("OnPostRender"),
			new MessageInfo("OnPreCull"),
			new MessageInfo("OnPreRender"),
			new MessageInfo("OnRenderImage", typeof(RenderTexture), typeof(RenderTexture)),
			new MessageInfo("OnRenderObject"),
			new MessageInfo("OnSerializeNetworkView", typeof(BitStream), typeof(NetworkMessageInfo)),
			new MessageInfo("OnServerInitialized"),
			new MessageInfo("OnTransformChildrenChanged"),
			new MessageInfo("OnTransformParentChanged"),
			new MessageInfo("OnTriggerEnter", typeof(Collider)),
			new MessageInfo("OnTriggerEnter2D", typeof(Collider2D)),
			new MessageInfo("OnTriggerExit", typeof(Collider)),
			new MessageInfo("OnTriggerExit2D", typeof(Collider2D)),
			new MessageInfo("OnTriggerStay", typeof(Collider)),
			new MessageInfo("OnTriggerStay2D", typeof(Collider2D)),
			new MessageInfo("OnValidate"),
			new MessageInfo("OnWillRenderObject"),
			new MessageInfo("Reset"),
			new MessageInfo("Start"),
			new MessageInfo("Update"),
		};
		
		const string PostCompileWorkPref = "NativeScriptGenerateBindingsPostCompileWork";
		const string DryRunPref = "NativeScriptGenerateBindingsDryRun";
		
		static readonly string CppDirPath =
			Path.Combine(
				Path.Combine(
					new DirectoryInfo(Application.dataPath)
						.Parent
						.FullName,
					"CppSource"),
				"NativeScript");
		static readonly string CsharpPath = Path.Combine(
			Application.dataPath,
			Path.Combine(
				"NativeScript",
				"Bindings.cs"));
		static readonly string CppHeaderPath = Path.Combine(
			CppDirPath,
			"Bindings.h");
		static readonly string CppSourcePath = Path.Combine(
			CppDirPath,
			"Bindings.cpp");
		
		// Restore unused field types
		#pragma warning restore CS0649
		
		[MenuItem("NativeScript/Generate Bindings #%g")]
		public static void Generate()
		{
			Generate(false);
		}
		
		[MenuItem("NativeScript/Generate Bindings (dry run) #%&g")]
		public static void GenerateDryRun()
		{
			Generate(true);
		}
		
		static void Generate(bool dryRun)
		{
			EditorPrefs.DeleteKey(PostCompileWorkPref);
			EditorPrefs.SetBool(DryRunPref, dryRun);
			if (dryRun)
			{
				DoPostCompileWork();
			}
			else
			{
				JsonDocument doc = LoadJson();
				
				// Generate stub classes extending MonoBehaviour
				// We'll need to be able to get these via reflection later
				StringBuilder output = new StringBuilder(1024*5);
				string timestamp = DateTime.Now.ToLongTimeString();
				foreach (JsonMonoBehaviour monoBehaviour in doc.MonoBehaviours)
				{
					int csharpIndent = AppendNamespaceBeginning(
						monoBehaviour.Namespace,
						output);
					AppendIndent(csharpIndent, output);
					output.Append("public class ");
					output.Append(monoBehaviour.Name);
					output.Append(" : UnityEngine.MonoBehaviour\n");
					AppendIndent(csharpIndent, output);
					output.Append("{\n");
					AppendIndent(csharpIndent + 1, output);
					output.Append("// Stub version. GenerateBindings is still in progress. ");
					output.Append(timestamp);
					output.Append('\n');
					AppendIndent(csharpIndent, output);
					output.Append("}\n");
					AppendNamespaceEnding(csharpIndent, output);
				}
				
				// Inject
				File.WriteAllText(
					CsharpPath,
					InjectIntoString(
						File.ReadAllText(CsharpPath),
						"/*BEGIN MONOBEHAVIOURS*/\n",
						"\n/*END MONOBEHAVIOURS*/",
						output.ToString()));
				
				// Compile and continue after scripts are refreshed
				Debug.Log("Waiting for compile...");
				AssetDatabase.Refresh();
				EditorPrefs.SetBool(PostCompileWorkPref, true);
			}
		}
		
		[UnityEditor.Callbacks.DidReloadScripts]
		private static void OnScriptsReloaded()
		{
			// Scripts get reloaded for many reasons, not just our work
			// Check if this reload is due to us refreshing the asset DB
			bool doWork = EditorPrefs.GetBool(PostCompileWorkPref, false);
			EditorPrefs.DeleteKey(PostCompileWorkPref);
			if (doWork)
			{
				DoPostCompileWork();
			}
		}
		
		static void DoPostCompileWork()
		{
			bool dryRun = EditorPrefs.GetBool(DryRunPref);
			EditorPrefs.DeleteKey(DryRunPref);
			
			JsonDocument doc = LoadJson();
			
			// Build binding strings
			StringBuilders builders = new StringBuilders();
			StringBuilder csharpInitParams = builders.CsharpInitParams;
			StringBuilder csharpDelegateTypes = builders.CsharpDelegateTypes;
			StringBuilder csharpInitCall = builders.CsharpInitCall;
			StringBuilder csharpFunctions = builders.CsharpFunctions;
			StringBuilder csharpMonoBehaviours = builders.CsharpMonoBehaviours;
			StringBuilder csharpMonoBehaviourDelegates = builders.CsharpMonoBehaviourDelegates;
			StringBuilder csharpMonoBehaviourImports = builders.CsharpMonoBehaviourImports;
			StringBuilder csharpMonoBehaviourGetDelegateCalls = builders.CsharpMonoBehaviourGetDelegateCalls;
			StringBuilder cppFunctionPointers = builders.CppFunctionPointers;
			StringBuilder cppTypeDeclarations = builders.CppTypeDeclarations;
			StringBuilder cppTypeDefinitions = builders.CppTypeDefinitions;
			StringBuilder cppMethodDefinitions = builders.CppMethodDefinitions;
			StringBuilder cppInitParams = builders.CppInitParams;
			StringBuilder cppInitBody = builders.CppInitBody;
			StringBuilder cppMonoBehaviourMessages = builders.CppMonoBehaviourMessages;
			StringBuilder tempStrBuilder = builders.TempStrBuilder;
			foreach (JsonAssembly jsonAssembly in doc.Assemblies)
			{
				Assembly assembly = Assembly.LoadFrom(jsonAssembly.Path);
				foreach (JsonType jsonType in jsonAssembly.Types)
				{
					Type type = assembly.GetType(jsonType.Name);
					string typeNameLower = char.ToLower(type.Name[0])
						+ type.Name.Substring(1);
					bool isStatic = type.IsAbstract && type.IsSealed;
					
					// C++ type declaration
					int indent = AppendCppTypeDeclaration(
						type.Namespace,
						type.Name,
						isStatic,
						cppTypeDeclarations);
					
					// C++ type definition (beginning)
					AppendCppTypeDefinitionBegin(
						type.Namespace,
						type.Name,
						type.BaseType.Namespace,
						type.BaseType.Name,
						isStatic,
						indent,
						cppTypeDefinitions);
					
					// C++ method definition
					int cppMethodDefinitionsIndent = AppendCppMethodDefinitionBegin(
						type.Namespace,
						type.Name,
						type.BaseType.Namespace,
						type.BaseType.Name,
						isStatic,
						indent,
						cppMethodDefinitions);
					
					// Constructors
					foreach (JsonConstructor jsonCtor in jsonType.Constructors)
					{
						Type[] paramTypes = GetTypes(jsonCtor.Types, assembly);
						ConstructorInfo ctor = type.GetConstructor(paramTypes);
						ParameterInfo[] parameters = ConvertParameters(
							ctor.GetParameters());
						
						// Build uppercase function name
						tempStrBuilder.Length = 0;
						tempStrBuilder.Append(type.Name);
						tempStrBuilder.Append("Constructor");
						AppendTypeNames(paramTypes, tempStrBuilder);
						string funcName = tempStrBuilder.ToString();
						
						// Build lowercase function name
						tempStrBuilder.Length = 0;
						tempStrBuilder.Append(typeNameLower);
						tempStrBuilder.Append("Constructor");
						AppendTypeNames(paramTypes, tempStrBuilder);
						string funcNameLower = tempStrBuilder.ToString();
						
						// C# init param declaration
						AppendCsharpInitParam(funcNameLower, csharpInitParams);

						// C# delegate type
						AppendCsharpDelegateType(
							funcName,
							true,
							typeof(int),
							parameters,
							csharpDelegateTypes);

						// C# init call param
						AppendCsharpInitCallArg(funcName, csharpInitCall);

						// C# function
						AppendCsharpFunctionBeginning(
							type,
							funcName,
							true,
							typeof(int),
							null,
							parameters,
							csharpFunctions);
						csharpFunctions.Append("ObjectStore.Store(");
						csharpFunctions.Append("new ");
						AppendCsharpTypeName(
							type,
							csharpFunctions);
						AppendCsharpFunctionCallParameters(
							true,
							parameters,
							csharpFunctions);
						csharpFunctions.Append(");");
						AppendCsharpFunctionReturn(
							typeof(int),
							csharpFunctions);
						
						// C++ function pointer
						AppendCppFunctionPointerDefinition(
							funcName,
							true,
							parameters,
							type,
							cppFunctionPointers);
						
						// C++ type declaration
						AppendIndent(
							indent + 1,
							cppTypeDefinitions);
						AppendCppMethodDeclaration(
							type.Name,
							false,
							null,
							null,
							parameters,
							cppTypeDefinitions);
						
						// C++ method definition
						AppendCppMethodDefinition(
							type,
							null,
							type.Name,
							null,
							parameters,
							indent,
							cppMethodDefinitions);
						AppendIndent(indent + 1, cppMethodDefinitions);
						cppMethodDefinitions.Append(": ");
						cppMethodDefinitions.Append(type.Name);
						cppMethodDefinitions.Append('(');
						cppMethodDefinitions.Append(type.Name);
						cppMethodDefinitions.Append('(');
						AppendCppPluginFunctionCall(
							true,
							type,
							funcName,
							parameters,
							cppMethodDefinitions);
						cppMethodDefinitions.Append(")\n");
						AppendIndent(indent, cppMethodDefinitions);
						cppMethodDefinitions.Append("{\n");
						AppendIndent(indent, cppMethodDefinitions);
						cppMethodDefinitions.Append("}\n");
						AppendIndent(indent, cppMethodDefinitions);
						cppMethodDefinitions.Append("\n");

						// C++ init params
						AppendCppInitParam(
							funcNameLower,
							true,
							parameters,
							type,
							cppInitParams);
						
						// C++ init body
						AppendCppInitBody(funcName, funcNameLower, cppInitBody);
					}
					
					// Properties
					foreach (string jsonPropertyName in jsonType.Properties)
					{
						PropertyInfo property = type.GetProperty(
							jsonPropertyName);
						MethodInfo getMethod = property.GetGetMethod();
						if (getMethod != null && getMethod.IsPublic)
						{
							AppendGetter(
								property.Name,
								typeNameLower,
								"Property",
								ConvertParameters(getMethod.GetParameters()),
								getMethod.IsStatic,
								type,
								property.PropertyType,
								indent,
								builders);
						}
						MethodInfo setMethod = property.GetSetMethod();
						if (setMethod != null && setMethod.IsPublic)
						{
							AppendSetter(
								property.Name,
								"Property",
								typeNameLower,
								ConvertParameters(setMethod.GetParameters()),
								setMethod.IsStatic,
								type,
								property.PropertyType,
								indent,
								builders);
						}
					}
					
					// Fields
					foreach (string jsonFieldName in jsonType.Fields)
					{
						FieldInfo field = type.GetField(jsonFieldName);
						AppendGetter(
							field.Name,
							typeNameLower,
							"Field",
							new ParameterInfo[0],
							field.IsStatic,
							type,
							field.FieldType,
							indent,
							builders);
						ParameterInfo setParam = new ParameterInfo();
						setParam.Name = "value";
						setParam.ParameterType = field.FieldType;
						ParameterInfo[] parameters = new []{ setParam };
						AppendSetter(
							field.Name,
							"Field",
							typeNameLower,
							parameters,
							field.IsStatic,
							type,
							field.FieldType,
							indent,
							builders);
					}
					
					// Methods
					foreach (JsonMethod jsonMethod in jsonType.Methods)
					{
						MethodInfo method = GetMethod(
							type,
							jsonMethod.Name,
							jsonMethod.ReturnType,
							jsonMethod.ParamTypes);
						ParameterInfo[] parameters = ConvertParameters(
							method.GetParameters());
						Type[] paramTypes = GetTypes(
							jsonMethod.ParamTypes,
							assembly);
						
						if (jsonMethod.GenericTypes != null)
						{
							foreach (JsonGenericType genericType in jsonMethod.GenericTypes)
							{
								Type returnType;
								if (genericType.Name == method.ReturnType.Name)
								{
									returnType = GetType(genericType.Type, assembly);
								}
								else
								{
									returnType = method.ReturnType;
								}
								Type[] typeParams = new[] { returnType };
								
								AppendMethod(
									type,
									typeNameLower,
									method.Name,
									method.IsStatic,
									returnType,
									typeParams,
									parameters,
									paramTypes,
									indent,
									builders);
							}
						}
						else
						{
							AppendMethod(
								type,
								typeNameLower,
								method.Name,
								method.IsStatic,
								method.ReturnType,
								null,
								parameters,
								paramTypes,
								indent,
								builders);
						}
					}
					
					// C++ type definition (ending)
					AppendCppTypeDefinitionEnd(
						isStatic,
						indent,
						cppTypeDefinitions);
					
					// C++ method definition (ending)
					AppendCppMethodDefinitionEnd(
						cppMethodDefinitionsIndent,
						cppMethodDefinitions);
				}
			}
			foreach (JsonMonoBehaviour monoBehaviour in doc.MonoBehaviours)
			{
				// C++ Type Declaration
				int cppIndent = AppendCppTypeDeclaration(
					monoBehaviour.Namespace,
					monoBehaviour.Name,
					false,
					cppTypeDeclarations);
				
				// C++ Type Definition (begin)
				AppendCppTypeDefinitionBegin(
					monoBehaviour.Namespace,
					monoBehaviour.Name,
					"UnityEngine",
					"MonoBehaviour",
					false,
					cppIndent,
					cppTypeDefinitions
				);
				
				// C++ method definition
				int cppMethodDefinitionsIndent = AppendCppMethodDefinitionBegin(
					monoBehaviour.Namespace,
					monoBehaviour.Name,
					"UnityEngine",
					"MonoBehaviour",
					false,
					cppIndent,
					cppMethodDefinitions);
				AppendCppMethodDefinitionEnd(
					cppMethodDefinitionsIndent,
					cppMethodDefinitions);
				
				// C# Class extending MonoBehaviour
				int csharpIndent = AppendNamespaceBeginning(
					monoBehaviour.Namespace,
					csharpMonoBehaviours);
				AppendIndent(csharpIndent, csharpMonoBehaviours);
				csharpMonoBehaviours.Append("public class ");
				csharpMonoBehaviours.Append(monoBehaviour.Name);
				csharpMonoBehaviours.Append(" : UnityEngine.MonoBehaviour\n");
				AppendIndent(csharpIndent, csharpMonoBehaviours);
				csharpMonoBehaviours.Append("{\n");
				AppendIndent(csharpIndent + 1, csharpMonoBehaviours);
				csharpMonoBehaviours.Append("private int thisHandle;\n");
				AppendIndent(csharpIndent + 1, csharpMonoBehaviours);
				csharpMonoBehaviours.Append('\n');
				AppendIndent(csharpIndent + 1, csharpMonoBehaviours);
				csharpMonoBehaviours.Append("public ");
				csharpMonoBehaviours.Append(monoBehaviour.Name);
				csharpMonoBehaviours.Append("()\n");
				AppendIndent(csharpIndent + 1, csharpMonoBehaviours);
				csharpMonoBehaviours.Append("{\n");
				AppendIndent(csharpIndent + 2, csharpMonoBehaviours);
				csharpMonoBehaviours.Append("thisHandle = NativeScript.ObjectStore.Store(this);\n");
				AppendIndent(csharpIndent + 1, csharpMonoBehaviours);
				csharpMonoBehaviours.Append("}\n");
				if (monoBehaviour.Messages.Length > 0)
				{
					AppendIndent(csharpIndent + 1, csharpMonoBehaviours);
					csharpMonoBehaviours.Append('\n');
				}
				for (int messageIndex = 0; messageIndex < monoBehaviour.Messages.Length; ++messageIndex)
				{
					string message = monoBehaviour.Messages[messageIndex];
					MessageInfo messageInfo = null;
					foreach (MessageInfo mi in messageInfos)
					{
						if (mi.Name == message)
						{
							messageInfo = mi;
							break;
						}
					}
					Type[] paramTypes = messageInfo.ParameterTypes;
					int numParams = paramTypes.Length;
					ParameterInfo[] parameters = ConvertParameters(
						paramTypes);
					
					// C++ Method Declaration
					AppendIndent(
						cppIndent + 1,
						cppTypeDefinitions);
					AppendCppMethodDeclaration(
						messageInfo.Name,
						false,
						typeof(void),
						null,
						parameters,
						cppTypeDefinitions);
					
					AppendIndent(csharpIndent + 1, csharpMonoBehaviours);
					csharpMonoBehaviours.Append("public ");
					AppendCsharpTypeName(
						typeof(void),
						csharpMonoBehaviours);
					csharpMonoBehaviours.Append(' ');
					csharpMonoBehaviours.Append(messageInfo.Name);
					csharpMonoBehaviours.Append('(');
					for (int i = 0; i < numParams; ++i)
					{
						Type paramType = paramTypes[i];
						AppendCsharpTypeName(
							paramType,
							csharpMonoBehaviours);
						csharpMonoBehaviours.Append(' ');
						csharpMonoBehaviours.Append("param");
						csharpMonoBehaviours.Append(i);
						if (i != numParams - 1)
						{
							csharpMonoBehaviours.Append(", ");
						}
					}
					csharpMonoBehaviours.Append(")\n");
					AppendIndent(csharpIndent + 1, csharpMonoBehaviours);
					csharpMonoBehaviours.Append("{\n");
					for (int i = 0; i < numParams; ++i)
					{
						Type paramType = paramTypes[i];
						if (!paramType.IsValueType)
						{
							AppendIndent(csharpIndent + 2, csharpMonoBehaviours);
							csharpMonoBehaviours.Append("int param");
							csharpMonoBehaviours.Append(i);
							csharpMonoBehaviours.Append("Handle = NativeScript.ObjectStore.Store(");
							csharpMonoBehaviours.Append("param");
							csharpMonoBehaviours.Append(i);
							csharpMonoBehaviours.Append(");\n");
						}
					}
					AppendIndent(csharpIndent + 2, csharpMonoBehaviours);
					csharpMonoBehaviours.Append("NativeScript.Bindings.");
					csharpMonoBehaviours.Append(monoBehaviour.Name);
					csharpMonoBehaviours.Append(messageInfo.Name);
					csharpMonoBehaviours.Append("(thisHandle");
					if (numParams > 0)
					{
						csharpMonoBehaviours.Append(", ");
					}
					for (int i = 0; i < numParams; ++i)
					{
						csharpMonoBehaviours.Append("param");
						csharpMonoBehaviours.Append(i);
						Type paramType = paramTypes[i];
						if (!paramType.IsValueType)
						{
							csharpMonoBehaviours.Append("Handle");
						}
						if (i != numParams - 1)
						{
							csharpMonoBehaviours.Append(", ");
						}
					}
					csharpMonoBehaviours.Append(");\n");
					for (int i = 0; i < numParams; ++i)
					{
						Type paramType = paramTypes[i];
						if (!paramType.IsValueType)
						{
							AppendIndent(csharpIndent + 2, csharpMonoBehaviours);
							csharpMonoBehaviours.Append("NativeScript.ObjectStore.Remove(param");
							csharpMonoBehaviours.Append(i);
							if (!paramType.IsValueType)
							{
								csharpMonoBehaviours.Append("Handle");
							}
							csharpMonoBehaviours.Append(");\n");
						}
					}
					AppendIndent(csharpIndent + 1, csharpMonoBehaviours);
					csharpMonoBehaviours.Append("}\n");
					if (messageIndex != monoBehaviour.Messages.Length - 1)
					{
						AppendIndent(csharpIndent + 1, csharpMonoBehaviours);
						csharpMonoBehaviours.Append('\n');
					}
					
					// C# Delegate
					csharpMonoBehaviourDelegates.Append("\t\tpublic delegate void ");
					csharpMonoBehaviourDelegates.Append(monoBehaviour.Name);
					csharpMonoBehaviourDelegates.Append(messageInfo.Name);
					csharpMonoBehaviourDelegates.Append("Delegate(int thisHandle");
					if (numParams > 0)
					{
						csharpMonoBehaviourDelegates.Append(", ");
					}
					for (int i = 0; i < numParams; ++i)
					{
						Type paramType = paramTypes[i];
						if (paramType.IsValueType)
						{
							AppendCsharpTypeName(
								paramType,
								csharpMonoBehaviourDelegates);
							csharpMonoBehaviourDelegates.Append(" param");
							csharpMonoBehaviourDelegates.Append(i);
						}
						else
						{
							csharpMonoBehaviourDelegates.Append("int param");
							csharpMonoBehaviourDelegates.Append(i);
						}
						if (i != numParams-1)
						{
							csharpMonoBehaviourDelegates.Append(", ");
						}
					}
					csharpMonoBehaviourDelegates.Append(");\n");
					csharpMonoBehaviourDelegates.Append("\t\tpublic static ");
					csharpMonoBehaviourDelegates.Append(monoBehaviour.Name);
					csharpMonoBehaviourDelegates.Append(messageInfo.Name);
					csharpMonoBehaviourDelegates.Append("Delegate ");
					csharpMonoBehaviourDelegates.Append(monoBehaviour.Name);
					csharpMonoBehaviourDelegates.Append(messageInfo.Name);
					csharpMonoBehaviourDelegates.Append(";\n\t\t\n");
					
					// C# Import
					csharpMonoBehaviourImports.Append("\t\t[DllImport(Constants.PluginName)]\n");
					csharpMonoBehaviourImports.Append("\t\tpublic static extern void ");
					csharpMonoBehaviourImports.Append(monoBehaviour.Name);
					csharpMonoBehaviourImports.Append(messageInfo.Name);
					csharpMonoBehaviourImports.Append("(int thisHandle");
					if (numParams > 0)
					{
						csharpMonoBehaviourImports.Append(", ");
					}
					for (int i = 0; i < numParams; ++i)
					{
						Type paramType = paramTypes[i];
						if (paramType.IsValueType)
						{
							AppendCsharpTypeName(
								paramType,
								csharpMonoBehaviourImports);
							csharpMonoBehaviourImports.Append(" param");
							csharpMonoBehaviourImports.Append(i);
						}
						else
						{
							csharpMonoBehaviourImports.Append("int param");
							csharpMonoBehaviourImports.Append(i);
						}
						if (i != numParams-1)
						{
							csharpMonoBehaviourImports.Append(", ");
						}
					}
					csharpMonoBehaviourImports.Append(");\n\t\t\n");
					
					// C# GetDelegate Call
					csharpMonoBehaviourGetDelegateCalls.Append("\t\t\t");
					csharpMonoBehaviourGetDelegateCalls.Append(monoBehaviour.Name);
					csharpMonoBehaviourGetDelegateCalls.Append(messageInfo.Name);
					csharpMonoBehaviourGetDelegateCalls.Append(" = GetDelegate<");
					csharpMonoBehaviourGetDelegateCalls.Append(monoBehaviour.Name);
					csharpMonoBehaviourGetDelegateCalls.Append(messageInfo.Name);
					csharpMonoBehaviourGetDelegateCalls.Append("Delegate>(libraryHandle, \"");
					csharpMonoBehaviourGetDelegateCalls.Append(monoBehaviour.Name);
					csharpMonoBehaviourGetDelegateCalls.Append(messageInfo.Name);
					csharpMonoBehaviourGetDelegateCalls.Append("\");\n");
					
					// C++ Message
					cppMonoBehaviourMessages.Append("DLLEXPORT void ");
					cppMonoBehaviourMessages.Append(monoBehaviour.Name);
					cppMonoBehaviourMessages.Append(messageInfo.Name);
					cppMonoBehaviourMessages.Append("(int32_t thisHandle");
					if (numParams > 0)
					{
						cppMonoBehaviourMessages.Append(", ");
					}
					for (int i = 0; i < numParams; ++i)
					{
						Type paramType = paramTypes[i];
						if (paramType.IsValueType)
						{
							AppendCppTypeName(
								paramType,
								cppMonoBehaviourMessages);
							cppMonoBehaviourMessages.Append(" param");
							cppMonoBehaviourMessages.Append(i);
						}
						else
						{
							cppMonoBehaviourMessages.Append("int32_t param");
							cppMonoBehaviourMessages.Append(i);
							cppMonoBehaviourMessages.Append("Handle");
						}
						if (i != numParams-1)
						{
							cppMonoBehaviourMessages.Append(", ");
						}
					}
					cppMonoBehaviourMessages.Append(")\n{\n\t");
					AppendCppTypeName(
						monoBehaviour.Namespace,
						monoBehaviour.Name,
						cppMonoBehaviourMessages);
					cppMonoBehaviourMessages.Append(" thiz(thisHandle);\n");
					for (int i = 0; i < numParams; ++i)
					{
						Type paramType = paramTypes[i];
						if (!paramType.IsValueType)
						{
							cppMonoBehaviourMessages.Append('\t');
							AppendCppTypeName(
								paramType,
								cppMonoBehaviourMessages);
							cppMonoBehaviourMessages.Append(" param");
							cppMonoBehaviourMessages.Append(i);
							cppMonoBehaviourMessages.Append("(param");
							cppMonoBehaviourMessages.Append(i);
							cppMonoBehaviourMessages.Append("Handle);\n");
						}
					}
					cppMonoBehaviourMessages.Append("\tthiz.");
					cppMonoBehaviourMessages.Append(messageInfo.Name);
					cppMonoBehaviourMessages.Append("(");
					for (int i = 0; i < numParams; ++i)
					{
						cppMonoBehaviourMessages.Append("param");
						cppMonoBehaviourMessages.Append(i);
						if (i != numParams-1)
						{
							cppMonoBehaviourMessages.Append(", ");
						}
					}
					cppMonoBehaviourMessages.Append(");\n}\n\n");
				}
				
				// C# Class extending MonoBehaviour (end)
				AppendIndent(csharpIndent, csharpMonoBehaviours);
				csharpMonoBehaviours.Append("}\n");
				AppendNamespaceEnding(csharpIndent, csharpMonoBehaviours);
				
				// C++ Type Definition (end)
				AppendCppTypeDefinitionEnd(
					false,
					cppIndent,
					cppTypeDefinitions);
			}
			
			// Remove trailing chars (e.g. commas) for last elements
			RemoveTrailingChars(csharpInitParams);
			RemoveTrailingChars(csharpDelegateTypes);
			RemoveTrailingChars(csharpInitCall);
			RemoveTrailingChars(csharpFunctions);
			RemoveTrailingChars(csharpMonoBehaviours);
			RemoveTrailingChars(csharpMonoBehaviourDelegates);
			RemoveTrailingChars(csharpMonoBehaviourImports);
			RemoveTrailingChars(csharpMonoBehaviourGetDelegateCalls);
			RemoveTrailingChars(cppFunctionPointers);
			RemoveTrailingChars(cppTypeDeclarations);
			RemoveTrailingChars(cppMethodDefinitions);
			RemoveTrailingChars(cppTypeDefinitions);
			RemoveTrailingChars(cppInitParams);
			RemoveTrailingChars(cppInitBody);
			RemoveTrailingChars(cppMonoBehaviourMessages);
			
			if (dryRun)
			{
				LogStringBuilder("C# init params", csharpInitParams);
				LogStringBuilder("C# delegates", csharpDelegateTypes);
				LogStringBuilder("C# init call", csharpInitCall);
				LogStringBuilder("C# functions", csharpFunctions);
				LogStringBuilder("C# MonoBehaviours", csharpMonoBehaviours);
				LogStringBuilder("C# MonoBehaviour Delegates", csharpMonoBehaviourDelegates);
				LogStringBuilder("C# MonoBehaviour Imports", csharpMonoBehaviourImports);
				LogStringBuilder("C# MonoBehaviour GetDelegate Calls", csharpMonoBehaviourGetDelegateCalls);
				LogStringBuilder("C++ function pointers", cppFunctionPointers);
				LogStringBuilder("C++ type declarations", cppTypeDeclarations);
				LogStringBuilder("C++ type definitions", cppTypeDefinitions);
				LogStringBuilder("C++ method definitions", cppMethodDefinitions);
				LogStringBuilder("C++ init params", cppInitParams);
				LogStringBuilder("C++ init body", cppInitBody);
				LogStringBuilder("C++ MonoBehaviour messages", cppMonoBehaviourMessages);
			}
			else
			{
				// Inject into source files
				string csharpContents = File.ReadAllText(CsharpPath);
				string cppHeaderContents = File.ReadAllText(CppHeaderPath);
				string cppSourceContents = File.ReadAllText(CppSourcePath);
				csharpContents = InjectIntoString(
					csharpContents,
					"/*BEGIN INIT PARAMS*/\n",
					"\n\t\t\t/*END INIT PARAMS*/",
					csharpInitParams.ToString());
				csharpContents = InjectIntoString(
					csharpContents,
					"/*BEGIN DELEGATE TYPES*/\n",
					"\n\t\t/*END DELEGATE TYPES*/",
					csharpDelegateTypes.ToString());
				csharpContents = InjectIntoString(
					csharpContents,
					"/*BEGIN INIT CALL*/\n",
					"\n\t\t\t\t/*END INIT CALL*/",
					csharpInitCall.ToString());
				csharpContents = InjectIntoString(
					csharpContents,
					"/*BEGIN FUNCTIONS*/\n",
					"\n\t\t/*END FUNCTIONS*/",
					csharpFunctions.ToString());
				csharpContents = InjectIntoString(
					csharpContents,
					"/*BEGIN MONOBEHAVIOURS*/\n",
					"\n/*END MONOBEHAVIOURS*/",
					csharpMonoBehaviours.ToString());
				csharpContents = InjectIntoString(
					csharpContents,
					"/*BEGIN MONOBEHAVIOUR DELEGATES*/\n",
					"\n\t\t/*END MONOBEHAVIOUR DELEGATES*/",
					csharpMonoBehaviourDelegates.ToString());
				csharpContents = InjectIntoString(
					csharpContents,
					"/*BEGIN MONOBEHAVIOUR IMPORTS*/\n",
					"\n\t\t/*END MONOBEHAVIOUR IMPORTS*/",
					csharpMonoBehaviourImports.ToString());
				csharpContents = InjectIntoString(
					csharpContents,
					"/*BEGIN MONOBEHAVIOUR GETDELEGATE CALLS*/\n",
					"\n\t\t\t/*END MONOBEHAVIOUR GETDELEGATE CALLS*/",
					csharpMonoBehaviourGetDelegateCalls.ToString());
				cppSourceContents = InjectIntoString(
					cppSourceContents,
					"/*BEGIN FUNCTION POINTERS*/\n",
					"\n\t/*END FUNCTION POINTERS*/",
					cppFunctionPointers.ToString());
				cppHeaderContents = InjectIntoString(
					cppHeaderContents,
					"/*BEGIN TYPE DECLARATIONS*/\n",
					"\n/*END TYPE DECLARATIONS*/",
					cppTypeDeclarations.ToString());
				cppHeaderContents = InjectIntoString(
					cppHeaderContents,
					"/*BEGIN TYPE DEFINITIONS*/\n",
					"\n/*END TYPE DEFINITIONS*/",
					cppTypeDefinitions.ToString());
				cppSourceContents = InjectIntoString(
					cppSourceContents,
					"/*BEGIN METHOD DEFINITIONS*/\n",
					"\n/*END METHOD DEFINITIONS*/",
					cppMethodDefinitions.ToString());
				cppSourceContents = InjectIntoString(
					cppSourceContents,
					"/*BEGIN INIT PARAMS*/\n",
					"\n\t/*END INIT PARAMS*/",
					cppInitParams.ToString());
				cppSourceContents = InjectIntoString(
					cppSourceContents,
					"/*BEGIN INIT BODY*/\n",
					"\n\t/*END INIT BODY*/",
					cppInitBody.ToString());
				cppSourceContents = InjectIntoString(
					cppSourceContents,
					"/*BEGIN MONOBEHAVIOUR MESSAGES*/\n",
					"\n/*END MONOBEHAVIOUR MESSAGES*/",
					cppMonoBehaviourMessages.ToString());
				
				File.WriteAllText(CsharpPath, csharpContents);
				File.WriteAllText(CppHeaderPath, cppHeaderContents);
				File.WriteAllText(CppSourcePath, cppSourceContents);
				Debug.Log(
					"Can't auto-refresh due to a bug in Unity. " +
					"Please manually refresh assets with Assets -> Refresh.");
			}
		}
		
		static JsonDocument LoadJson()
		{
			string jsonPath = Path.Combine(
				Application.dataPath,
				NativeScriptConstants.ExposedTypesJsonPath);
			string json = File.ReadAllText(jsonPath);
			return JsonUtility.FromJson<JsonDocument>(json);
		}
		
		static Type[] GetTypes(
			string[] typeNames,
			Assembly assembly)
		{
			Assembly systemAssembly = typeof(string).Assembly;
			Type[] types = new Type[typeNames.Length];
			for (int i = 0; i < typeNames.Length; ++i)
			{
				types[i] = GetType(typeNames[i], assembly);
			}
			return types;
		}
		
		static Type GetType(
			string typeName,
			Assembly assembly)
		{
			return assembly.GetType(typeName)
				?? typeof(string).Assembly.GetType(typeName)
				?? typeof(Bindings).Assembly.GetType(typeName);
		}
		
		static MethodInfo GetMethod(
			Type type,
			string methodName,
			string returnTypeName,
			string[] paramTypeNames)
		{
			foreach (MethodInfo method in type.GetMethods())
			{
				if (method.Name == methodName)
				{
					if (returnTypeName != null)
					{
						if (string.IsNullOrEmpty(method.ReturnType.Namespace))
						{
							if (method.ReturnType.Name != returnTypeName)
							{
								continue;
							}
						}
						else
						{
							if (method.ReturnType.Namespace + "." + method.ReturnType.Name != returnTypeName)
							{
								continue;
							}
						}
					}
					System.Reflection.ParameterInfo[] parameters = method.GetParameters();
					for (int i = 0; i < parameters.Length; ++i)
					{
						Type paramType = parameters[i].ParameterType;
						if (string.IsNullOrEmpty(paramType.Namespace))
						{
							if (paramType.Name != paramTypeNames[i])
							{
								goto mismatch;
							}
						}
						else
						{
							if (paramType.Namespace + "." + paramType.Name != paramTypeNames[i])
							{
								goto mismatch;
							}
						}
					}
					return method;
					mismatch:;
				}
			}
			return null;
		}
		
		static void AppendTypeNames(
			Type[] types,
			StringBuilder output)
		{
			for (int i = 0, len = types.Length; i < len; ++i)
			{
				Type type = types[i];
				AppendNamespace(type.Namespace, string.Empty, output);
				output.Append(type.Name);
				if (i != len - 1)
				{
					output.Append('_');
				}
			}
		}
		
		static void AppendNamespace(
			string namespaceName,
			string separator,
			StringBuilder output)
		{
			int startIndex = 0;
			if (!string.IsNullOrEmpty(namespaceName))
			{
				do
				{
					int dotIndex = namespaceName.IndexOf(
						'.',
						startIndex);
					if (dotIndex < 0)
					{
						break;
					}
					output.Append(
						namespaceName,
						startIndex,
						dotIndex - startIndex);
					output.Append(separator);
					startIndex = dotIndex + 1;
				}
				while (true);
				output.Append(
					namespaceName,
					startIndex,
					namespaceName.Length - startIndex);
			}
		}
		
		static ParameterInfo[] ConvertParameters(
			System.Reflection.ParameterInfo[] reflectionParameters)
		{
			int num = reflectionParameters.Length;
			ParameterInfo[] parameters = new ParameterInfo[num];
			for (int i = 0; i < num; ++i)
			{
				var reflectionInfo = reflectionParameters[i];
				ParameterInfo info = new ParameterInfo();
				info.Name = reflectionInfo.Name;
				info.ParameterType = reflectionInfo.ParameterType;
				parameters[i] = info;
			}
			return parameters;
		}
		
		static ParameterInfo[] ConvertParameters(
			Type[] paramTypes)
		{
			int num = paramTypes.Length;
			ParameterInfo[] parameters = new ParameterInfo[num];
			for (int i = 0; i < num; ++i)
			{
				Type paramType = paramTypes[i];
				ParameterInfo info = new ParameterInfo();
				info.Name = "param" + i;
				info.ParameterType = paramType;
				parameters[i] = info;
			}
			return parameters;
		}
		
		static void AppendGetter(
			string fieldName,
			string enclosingTypeNameLower,
			string syntaxType,
			ParameterInfo[] parameters,
			bool isStatic,
			Type enclosingType,
			Type fieldType,
			int indent,
			StringBuilders stringBuilders)
		{
			// Build uppercased field name
			stringBuilders.TempStrBuilder.Length = 0;
			stringBuilders.TempStrBuilder.Append(char.ToUpper(fieldName[0]));
			stringBuilders.TempStrBuilder.Append(
				fieldName,
				1,
				fieldName.Length-1);
			string fieldNameUpper = stringBuilders.TempStrBuilder.ToString();
			
			// Build uppercase function name
			stringBuilders.TempStrBuilder.Length = 0;
			stringBuilders.TempStrBuilder.Append(enclosingType.Name);
			stringBuilders.TempStrBuilder.Append(syntaxType);
			stringBuilders.TempStrBuilder.Append("Get");
			stringBuilders.TempStrBuilder.Append(fieldNameUpper);
			string funcName = stringBuilders.TempStrBuilder.ToString();
			
			// Build lowercase function name
			stringBuilders.TempStrBuilder.Length = 0;
			stringBuilders.TempStrBuilder.Append(enclosingTypeNameLower);
			stringBuilders.TempStrBuilder.Append(syntaxType);
			stringBuilders.TempStrBuilder.Append("Get");
			stringBuilders.TempStrBuilder.Append(fieldNameUpper);
			string funcNameLower = stringBuilders.TempStrBuilder.ToString();
			
			// Build method name
			stringBuilders.TempStrBuilder.Length = 0;
			stringBuilders.TempStrBuilder.Append("Get");
			stringBuilders.TempStrBuilder.Append(fieldNameUpper);
			string methodName = stringBuilders.TempStrBuilder.ToString();
			
			// C# init param declaration
			AppendCsharpInitParam(
				funcNameLower,
				stringBuilders.CsharpInitParams);

			// C# delegate type
			AppendCsharpDelegateType(
				funcName,
				isStatic,
				fieldType,
				parameters,
				stringBuilders.CsharpDelegateTypes);

			// C# init call param
			AppendCsharpInitCallArg(
				funcName,
				stringBuilders.CsharpInitCall);

			// C# function
			AppendCsharpFunctionBeginning(
				enclosingType,
				funcName,
				isStatic,
				fieldType,
				null,
				parameters,
				stringBuilders.CsharpFunctions);
			AppendCsharpFunctionCallSubject(
				enclosingType,
				isStatic,
				stringBuilders.CsharpFunctions);
			stringBuilders.CsharpFunctions.Append(fieldName);
			stringBuilders.CsharpFunctions.Append(';');
			AppendCsharpFunctionReturn(
				fieldType,
				stringBuilders.CsharpFunctions);

			// C++ function pointer
			AppendCppFunctionPointerDefinition(
				funcName,
				isStatic,
				parameters,
				fieldType,
				stringBuilders.CppFunctionPointers);

			// C++ method declaration
			AppendIndent(indent + 1, stringBuilders.CppTypeDefinitions);
			AppendCppMethodDeclaration(
				methodName,
				isStatic,
				fieldType,
				null,
				parameters,
				stringBuilders.CppTypeDefinitions);
			
			// C++ method definition
			AppendCppMethodDefinition(
				enclosingType,
				fieldType,
				methodName,
				null,
				parameters,
				indent,
				stringBuilders.CppMethodDefinitions);
			AppendIndent(indent, stringBuilders.CppMethodDefinitions);
			stringBuilders.CppMethodDefinitions.Append("{\n");
			AppendIndent(indent + 1, stringBuilders.CppMethodDefinitions);
			AppendCppMethodReturn(
				fieldType,
				stringBuilders.CppMethodDefinitions);
			AppendCppPluginFunctionCall(
				isStatic,
				fieldType,
				funcName,
				parameters,
				stringBuilders.CppMethodDefinitions);
			stringBuilders.CppMethodDefinitions.Append(";\n");
			AppendIndent(indent, stringBuilders.CppMethodDefinitions);
			stringBuilders.CppMethodDefinitions.Append("}\n");
			AppendIndent(indent, stringBuilders.CppMethodDefinitions);
			stringBuilders.CppMethodDefinitions.Append("\n");

			// C++ init params
			AppendCppInitParam(
				funcNameLower,
				isStatic,
				parameters,
				fieldType,
				stringBuilders.CppInitParams);
			
			// C++ init body
			AppendCppInitBody(
				funcName,
				funcNameLower,
				stringBuilders.CppInitBody);
		}
		
		static void AppendSetter(
			string fieldName,
			string syntaxType,
			string enclosingTypeNameLower,
			ParameterInfo[] parameters,
			bool isStatic,
			Type enclosingType,
			Type fieldType,
			int indent,
			StringBuilders stringBuilders)
		{
			// Build uppercased field name
			stringBuilders.TempStrBuilder.Length = 0;
			stringBuilders.TempStrBuilder.Append(char.ToUpper(fieldName[0]));
			stringBuilders.TempStrBuilder.Append(
				fieldName,
				1,
				fieldName.Length-1);
			string fieldNameUpper = stringBuilders.TempStrBuilder.ToString();
			
			// Build uppercase function name
			stringBuilders.TempStrBuilder.Length = 0;
			stringBuilders.TempStrBuilder.Append(enclosingType.Name);
			stringBuilders.TempStrBuilder.Append(syntaxType);
			stringBuilders.TempStrBuilder.Append("Set");
			stringBuilders.TempStrBuilder.Append(fieldNameUpper);
			string funcName = stringBuilders.TempStrBuilder.ToString();
			
			// Build lowercase function name
			stringBuilders.TempStrBuilder.Length = 0;
			stringBuilders.TempStrBuilder.Append(enclosingTypeNameLower);
			stringBuilders.TempStrBuilder.Append(syntaxType);
			stringBuilders.TempStrBuilder.Append("Set");
			stringBuilders.TempStrBuilder.Append(fieldNameUpper);
			string funcNameLower = stringBuilders.TempStrBuilder.ToString();
			
			// Build method name
			stringBuilders.TempStrBuilder.Length = 0;
			stringBuilders.TempStrBuilder.Append("Set");
			stringBuilders.TempStrBuilder.Append(fieldNameUpper);
			string methodName = stringBuilders.TempStrBuilder.ToString();
			
			// C# init param declaration
			AppendCsharpInitParam(
				funcNameLower,
				stringBuilders.CsharpInitParams);
			
			// C# delegate type
			AppendCsharpDelegateType(
				funcName,
				isStatic,
				typeof(void),
				parameters,
				stringBuilders.CsharpDelegateTypes);
			
			// C# init call param
			AppendCsharpInitCallArg(
				funcName,
				stringBuilders.CsharpInitCall);
			
			// C# function
			AppendCsharpFunctionBeginning(
				enclosingType,
				funcName,
				isStatic,
				typeof(void),
				null,
				parameters,
				stringBuilders.CsharpFunctions);
			AppendCsharpFunctionCallSubject(
				enclosingType,
				isStatic,
				stringBuilders.CsharpFunctions);
			stringBuilders.CsharpFunctions.Append(fieldName);
			stringBuilders.CsharpFunctions.Append(" = ");
			if (fieldType.IsValueType)
			{
				stringBuilders.CsharpFunctions.Append("value;");
			}
			else
			{
				stringBuilders.CsharpFunctions.Append('(');
				AppendCsharpTypeName(
					fieldType,
					stringBuilders.CsharpFunctions);
				stringBuilders.CsharpFunctions.Append(
					")ObjectStore.Get(valueHandle);");
			}
			AppendCsharpFunctionReturn(
				typeof(void),
				stringBuilders.CsharpFunctions);
			
			// C++ function pointer
			AppendCppFunctionPointerDefinition(
				funcName,
				isStatic,
				parameters,
				typeof(void),
				stringBuilders.CppFunctionPointers);
			
			// C++ method declaration
			AppendIndent(indent + 1, stringBuilders.CppTypeDefinitions);
			AppendCppMethodDeclaration(
				methodName,
				isStatic,
				typeof(void),
				null,
				parameters,
				stringBuilders.CppTypeDefinitions);
			
			// C++ method definition
			AppendCppMethodDefinition(
				enclosingType,
				typeof(void),
				methodName,
				null,
				parameters,
				indent,
				stringBuilders.CppMethodDefinitions);
			AppendIndent(indent, stringBuilders.CppMethodDefinitions);
			stringBuilders.CppMethodDefinitions.Append("{\n");
			AppendIndent(indent + 1, stringBuilders.CppMethodDefinitions);
			AppendCppPluginFunctionCall(
				isStatic,
				typeof(void),
				funcName,
				parameters,
				stringBuilders.CppMethodDefinitions);
			stringBuilders.CppMethodDefinitions.Append(";\n");
			AppendIndent(indent, stringBuilders.CppMethodDefinitions);
			stringBuilders.CppMethodDefinitions.Append("}\n");
			AppendIndent(indent, stringBuilders.CppMethodDefinitions);
			stringBuilders.CppMethodDefinitions.Append('\n');
			
			// C++ init params
			AppendCppInitParam(
				funcNameLower,
				isStatic,
				parameters,
				typeof(void),
				stringBuilders.CppInitParams);
			
			// C++ init body
			AppendCppInitBody(
				funcName,
				funcNameLower,
				stringBuilders.CppInitBody);
		}
		
		static void AppendMethod(
			Type type,
			string typeNameLower,
			string methodName,
			bool isStatic,
			Type returnType,
			Type[] typeParameters,
			ParameterInfo[] parameters,
			Type[] paramTypes,
			int indent,
			StringBuilders stringBuilders)
		{
			// Build uppercase function name
			stringBuilders.TempStrBuilder.Length = 0;
			stringBuilders.TempStrBuilder.Append(type.Name);
			stringBuilders.TempStrBuilder.Append("Method");
			stringBuilders.TempStrBuilder.Append(methodName);
			AppendTypeNames(paramTypes, stringBuilders.TempStrBuilder);
			if (typeParameters != null)
			{
				foreach (Type typeParam in typeParameters)
				{
					AppendNamespace(
						typeParam.Namespace,
						string.Empty,
						stringBuilders.TempStrBuilder);
					stringBuilders.TempStrBuilder.Append(typeParam.Name);
				}
			}
			string funcName = stringBuilders.TempStrBuilder.ToString();
			
			// Build lowercase function name
			stringBuilders.TempStrBuilder.Length = 0;
			stringBuilders.TempStrBuilder.Append(typeNameLower);
			stringBuilders.TempStrBuilder.Append("Method");
			stringBuilders.TempStrBuilder.Append(methodName);
			AppendTypeNames(paramTypes, stringBuilders.TempStrBuilder);
			if (typeParameters != null)
			{
				foreach (Type typeParam in typeParameters)
				{
					AppendNamespace(
						typeParam.Namespace,
						string.Empty,
						stringBuilders.TempStrBuilder);
					stringBuilders.TempStrBuilder.Append(typeParam.Name);
				}
			}
			string funcNameLower = stringBuilders.TempStrBuilder.ToString();
			
			// C# init param declaration
			AppendCsharpInitParam(
				funcNameLower,
				stringBuilders.CsharpInitParams);

			// C# delegate type
			AppendCsharpDelegateType(
				funcName,
				isStatic,
				returnType,
				parameters,
				stringBuilders.CsharpDelegateTypes);

			// C# init call param
			AppendCsharpInitCallArg(
				funcName,
				stringBuilders.CsharpInitCall);

			// C# function
			AppendCsharpFunctionBeginning(
				type,
				funcName,
				isStatic,
				returnType,
				typeParameters,
				parameters,
				stringBuilders.CsharpFunctions);
			AppendCsharpFunctionCallSubject(
				type,
				isStatic,
				stringBuilders.CsharpFunctions);
			stringBuilders.CsharpFunctions.Append(methodName);
			if (typeParameters != null)
			{
				stringBuilders.CsharpFunctions.Append('<');
				for (int i = 0; i < typeParameters.Length; ++i)
				{
					Type typeParam = typeParameters[i];
					AppendCsharpTypeName(
						typeParam,
						stringBuilders.CsharpFunctions);
					if (i != typeParameters.Length - 1)
					{
						stringBuilders.CsharpFunctions.Append(", ");
					}
				}
				stringBuilders.CsharpFunctions.Append('>');
			}
			AppendCsharpFunctionCallParameters(
				isStatic,
				parameters,
				stringBuilders.CsharpFunctions);
			stringBuilders.CsharpFunctions.Append(';');
			AppendCsharpFunctionReturn(
				returnType,
				stringBuilders.CsharpFunctions);
			
			// C++ function pointer
			AppendCppFunctionPointerDefinition(
				funcName,
				isStatic,
				parameters,
				returnType,
				stringBuilders.CppFunctionPointers);
			
			// C++ method declaration
			AppendIndent(
				indent + 1,
				stringBuilders.CppTypeDefinitions);
			AppendCppMethodDeclaration(
				methodName,
				isStatic,
				returnType,
				typeParameters,
				parameters,
				stringBuilders.CppTypeDefinitions);
			
			// C++ method definition
			AppendCppMethodDefinition(
				type,
				returnType,
				methodName,
				typeParameters,
				parameters,
				indent,
				stringBuilders.CppMethodDefinitions);
			AppendIndent(
				indent,
				stringBuilders.CppMethodDefinitions);
			stringBuilders.CppMethodDefinitions.Append("{\n");
			AppendIndent(
				indent + 1,
				stringBuilders.CppMethodDefinitions);
			AppendCppMethodReturn(
				returnType,
				stringBuilders.CppMethodDefinitions);
			AppendCppPluginFunctionCall(
				isStatic,
				returnType,
				funcName,
				parameters,
				stringBuilders.CppMethodDefinitions);
			stringBuilders.CppMethodDefinitions.Append(";\n");
			AppendIndent(
				indent,
				stringBuilders.CppMethodDefinitions);
			stringBuilders.CppMethodDefinitions.Append("}\n\t\n");
			
			// C++ init params
			AppendCppInitParam(
				funcNameLower,
				isStatic,
				parameters,
				returnType,
				stringBuilders.CppInitParams);
			
			// C++ init body
			AppendCppInitBody(
				funcName,
				funcNameLower,
				stringBuilders.CppInitBody);
		}
		
		static int AppendCppTypeDeclaration(
			string typeNamespace,
			string typeName,
			bool isStatic,
			StringBuilder output
		)
		{
			int indent = AppendNamespaceBeginning(
				typeNamespace,
				output);
			AppendIndent(indent, output);
			if (isStatic)
			{
				output.Append("namespace ");
				output.Append(typeName);
				output.Append('\n');
				AppendIndent(indent, output);
				output.Append("{\n");
				AppendIndent(indent, output);
				output.Append('}');
			}
			else
			{
				output.Append("struct ");
				output.Append(typeName);
				output.Append(";");
			}
			output.Append('\n');
			AppendNamespaceEnding(
				indent,
				output);
			output.Append('\n');
			return indent;
		}
		
		static void AppendCppTypeDefinitionBegin(
			string typeNamespace,
			string typeName,
			string baseTypeNamespace,
			string baseTypeName,
			bool isStatic,
			int indent,
			StringBuilder output
		)
		{
			AppendNamespaceBeginning(
				typeNamespace,
				output);
			AppendIndent(
				indent,
				output);
			if (isStatic)
			{
				output.Append("namespace ");
				output.Append(typeName);
			}
			else
			{
				output.Append("struct ");
				output.Append(typeName);
				output.Append(" : ");
				output.Append(baseTypeNamespace);
				output.Append("::");
				output.Append(baseTypeName);
			}
			output.Append('\n');
			AppendIndent(
				indent,
				output);
			output.Append("{\n");
			if (!isStatic)
			{
				AppendIndent(
					indent + 1,
					output);
				AppendSystemObjectLifecycleCall(
					"SYSTEM_OBJECT_LIFECYCLE_DECLARATION",
					typeName,
					baseTypeNamespace,
					baseTypeName,
					output);
				output.Append('\n');
			}
		}
		
		static void AppendCppTypeDefinitionEnd(
			bool isStatic,
			int indent,
			StringBuilder cppTypeDefinitions
		)
		{
			AppendIndent(
				indent,
				cppTypeDefinitions);
			cppTypeDefinitions.Append('}');
			if (!isStatic)
			{
				cppTypeDefinitions.Append(';');
			}
			cppTypeDefinitions.Append('\n');
			AppendNamespaceEnding(
				indent,
				cppTypeDefinitions);
			cppTypeDefinitions.Append('\n');
		}
		
		static int AppendCppMethodDefinitionBegin(
			string typeNamespace,
			string typeName,
			string baseTypeNamespace,
			string baseTypeName,
			bool isStatic,
			int indent,
			StringBuilder output)
		{
			int cppMethodDefinitionsIndent = AppendNamespaceBeginning(
				typeNamespace,
				output);
			if (!isStatic)
			{
				AppendIndent(indent, output);
				AppendSystemObjectLifecycleCall(
					"SYSTEM_OBJECT_LIFECYCLE_DEFINITION",
					typeName,
					baseTypeNamespace,
					baseTypeName,
					output);
				output.Append('\n');
				AppendIndent(indent, output);
				output.Append('\n');
			}
			return cppMethodDefinitionsIndent;
		}
		
		static void AppendCppMethodDefinitionEnd(
			int indent,
			StringBuilder output)
		{
			RemoveTrailingChars(output);
			output.Append('\n');
			AppendNamespaceEnding(
				indent,
				output);
			output.Append('\n');
		}
		
		static void AppendSystemObjectLifecycleCall(
			string macroName,
			string typeName,
			string baseTypeNamespace,
			string baseTypeName,
			StringBuilder output)
		{
			output.Append(macroName);
			output.Append('(');
			output.Append(typeName);
			output.Append(", ");
			output.Append(baseTypeNamespace);
			output.Append("::");
			output.Append(baseTypeName);
			output.Append(")");
		}
		
		static int AppendNamespaceBeginning(
			string namespaceName,
			StringBuilder output)
		{
			int startIndex = 0;
			int indent = 0;
			do
			{
				int separatorIndex = namespaceName.IndexOf(
					'.',
					startIndex);
				int endIndex = separatorIndex < 0
					? namespaceName.Length - 1
					: separatorIndex - 1;
				int len = 1 + endIndex - startIndex;
				AppendIndent(indent, output);
				output.Append("namespace ");
				output.Append(namespaceName, startIndex, len);
				output.Append('\n');
				AppendIndent(indent, output);
				output.Append("{\n");
				if (separatorIndex < 0)
				{
					break;
				}
				startIndex = separatorIndex + 1;
				indent++;
			}
			while (true);
			return indent + 1;
		}
		
		static void AppendNamespaceEnding(
			int indent,
			StringBuilder output)
		{
			indent--;
			for (; indent >= 0; --indent)
			{
				AppendIndent(indent, output);
				output.Append("}\n");
			}
		}
		
		static void AppendIndent(
			int indent,
			StringBuilder output)
		{
			output.Append('\t', indent);
		}
		
		static void AppendCsharpInitParam(
			string funcName,
			StringBuilder output)
		{
			output.Append("\t\t\tIntPtr ");
			output.Append(funcName);
			output.Append(",\n");
		}
		
		static void AppendCsharpInitCallArg(
			string funcName,
			StringBuilder output)
		{
			output.Append(
				"\t\t\t\tMarshal.GetFunctionPointerForDelegate(new ");
			output.Append(funcName);
			output.Append("Delegate(");
			output.Append(funcName);
			output.Append(")),\n");
		}
		
		static void AppendCsharpDelegateType(
			string funcName,
			bool isStatic,
			Type returnType,
			ParameterInfo[] parameters,
			StringBuilder output)
		{
			output.Append("\t\tdelegate ");
			
			// Return type
			if (returnType.IsValueType)
			{
				AppendCsharpTypeName(
					returnType,
					output);
			}
			else
			{
				output.Append("int");
			}
			
			output.Append(' ');
			output.Append(funcName);
			output.Append("Delegate(");
			if (!isStatic)
			{
				output.Append("int thisHandle");
				if (parameters.Length > 0)
				{
					output.Append(", ");
				}
			}
			AppendParameterDeclaration(
				parameters,
				"int",
				AppendCsharpTypeName,
				output);
			output.Append(");\n");
		}
		
		static void AppendCsharpFunctionBeginning(
			Type enclosingType,
			string funcName,
			bool isStatic,
			Type returnType,
			Type[] typeParameters,
			ParameterInfo[] parameters,
			StringBuilder output)
		{
			output.Append("\t\t[MonoPInvokeCallback(typeof(");
			output.Append(funcName);
			output.Append("Delegate))]\n\t\tstatic ");
			
			// Return type
			if (returnType != null)
			{
				if (returnType.IsValueType)
				{
					AppendCsharpTypeName(
						returnType,
						output);
				}
				else
				{
					output.Append("int");
				}
				output.Append(' ');
			}
			
			// Function name
			output.Append(funcName);
			
			// Parameters
			output.Append("(");
			if (!isStatic)
			{
				output.Append("int thisHandle");
				if (parameters.Length > 0)
				{
					output.Append(", ");
				}
			}
			AppendParameterDeclaration(
				parameters,
				"int",
				AppendCsharpTypeName,
				output);
			output.Append(")\n\t\t{\n\t\t\t");
			
			// Get "this"
			if (!isStatic)
			{
				output.Append("var thiz = (");
				AppendCsharpTypeName(
					enclosingType,
					output);
				output.Append(
					")ObjectStore.Get(thisHandle);\n\t\t\t");
			}
			
			// Save return value as local variable
			if (!returnType.Equals(typeof(void)))
			{
				output.Append("var obj = ");
			};
		}
		
		static void AppendCsharpFunctionCallSubject(
			Type enclosingType,
			bool isStatic,
			StringBuilder output)
		{
			if (isStatic)
			{
				AppendCsharpTypeName(
					enclosingType,
					output);
			}
			else
			{
				output.Append("thiz");
			}
			output.Append('.');
		}
		
		static void AppendCsharpFunctionCallParameters(
			bool isStatic,
			ParameterInfo[] parameters,
			StringBuilder output)
		{
			output.Append('(');
			for (int i = 0; i < parameters.Length; ++i)
			{
				ParameterInfo parameter = parameters[i];
				if (parameter.ParameterType.IsValueType)
				{
					output.Append(parameter.Name);
				}
				else
				{
					if (!parameter.ParameterType.Equals(typeof(object)))
					{
						output.Append('(');
						output.Append(parameter.ParameterType);
						output.Append(')');
					}
					output.Append("ObjectStore.Get(");
					output.Append(parameter.Name);
					output.Append("Handle)");
				}
				if (i != parameters.Length - 1)
				{
					output.Append(", ");
				}
			}
			output.Append(')');
		}
		
		static void AppendCsharpFunctionReturn(
			Type returnType,
			StringBuilder output)
		{
			if (!returnType.Equals(typeof(void)))
			{
				output.Append("\n\t\t\t");
				if (returnType.IsValueType)
				{
					output.Append("return obj;");
				}
				else
				{
					output.Append(
						"int handle = ObjectStore.Store(obj);\n");
					output.Append("\t\t\treturn handle;");
				}
			}
			output.Append("\n\t\t}\n\t\t\n");
		}
		
		static void AppendParameterDeclaration(
			ParameterInfo[] parameters,
			string handleType,
			Action<Type, StringBuilder> appendTypeName,
			StringBuilder output
		)
		{
			for (int i = 0; i < parameters.Length; ++i)
			{
				ParameterInfo parameter = parameters[i];
				if (handleType == null || parameter.ParameterType.IsValueType)
				{
					appendTypeName(parameter.ParameterType, output);
				}
				else
				{
					output.Append(handleType);
				}
				output.Append(' ');
				output.Append(parameter.Name);
				if (handleType != null && !parameter.ParameterType.IsValueType)
				{
					output.Append("Handle");
				}
				if (i != parameters.Length - 1)
				{
					output.Append(", ");
				}
			}
		}
		
		static void AppendParameterCall(
			ParameterInfo[] parameters,
			string separator,
			StringBuilder output)
		{
			for (int i = 0; i < parameters.Length; ++i)
			{
				ParameterInfo parameter = parameters[i];
				output.Append(parameter.Name);
				if (!parameter.ParameterType.IsValueType)
				{
					output.Append("Handle");
				}
				if (i != parameters.Length - 1)
				{
					output.Append(',');
					output.Append(separator);
				}
			}
		}
		
		static void AppendCppInitBody(
			string funcName,
			string funcNameLower,
			StringBuilder output)
		{
			output.Append('\t');
			output.Append(funcName);
			output.Append(" = ");
			output.Append(funcNameLower);
			output.Append(";\n");
		}
		
		static void AppendCppMethodDefinition(
			Type enclosingType,
			Type returnType,
			string methodName,
			Type[] typeParameters,
			ParameterInfo[] parameters,
			int indent,
			StringBuilder output)
		{
			AppendIndent(indent, output);
			if (typeParameters != null)
			{
				output.Append("template<> ");
			}
			if (returnType != null)
			{
				AppendCppTypeName(returnType, output);
				output.Append(' ');
			}
			output.Append(enclosingType.Name);
			output.Append("::");
			output.Append(methodName);
			if (typeParameters != null)
			{
				output.Append("<");
				for (int i = 0; i < typeParameters.Length; ++i)
				{
					Type typeParam = typeParameters[i];
					AppendCppTypeName(typeParam, output);
					if (i != typeParameters.Length - 1)
					{
						output.Append(", ");
					}
				}
				output.Append(">");
			}
			output.Append('(');
			AppendParameterDeclaration(
				parameters,
				null,
				AppendCppTypeName,
				output);
			output.Append(")\n");
		}
		
		static void AppendCppMethodReturn(
			Type returnType,
			StringBuilder output
		)
		{
			if (returnType != null && !returnType.Equals(typeof(void)))
			{
				output.Append("return ");
				if (!returnType.IsValueType)
				{
					AppendCppTypeName(returnType, output);
					output.Append('(');
				}
			}
		}
		
		static void AppendCppPluginFunctionCall(
			bool isStatic,
			Type returnType,
			string funcName,
			ParameterInfo[] parameters,
			StringBuilder output)
		{
			output.Append("Plugin::");
			output.Append(funcName);
			output.Append("(");
			if (!isStatic)
			{
				output.Append("Handle");
				if (parameters.Length > 0)
				{
					output.Append(", ");
				}
			}
			for (int i = 0; i < parameters.Length; ++i)
			{
				Type paramType = parameters[i].ParameterType;
				output.Append(parameters[i].Name);
				if (!paramType.IsValueType)
				{
					output.Append(".Handle");
				}
				if (i != parameters.Length - 1)
				{
					output.Append(", ");
				}
			}
			output.Append(")");
			if (returnType != null
				&& !returnType.Equals(typeof(void))
				&& !returnType.IsValueType)
			{
				output.Append(')');
			}
		}
		
		static void AppendCppInitParam(
			string funcName,
			bool isStatic,
			ParameterInfo[] parameters,
			Type returnType,
			StringBuilder output
		)
		{
			output.Append('\t');
			AppendCppFunctionPointer(
				funcName,
				isStatic,
				parameters,
				returnType,
				',',
				output
			);
			output.Append('\n');
		}
		
		static void AppendCppFunctionPointerDefinition(
			string funcName,
			bool isStatic,
			ParameterInfo[] parameters,
			Type returnType,
			StringBuilder output
		)
		{
			output.Append('\t');
			AppendCppFunctionPointer(
				funcName,
				isStatic,
				parameters,
				returnType,
				';',
				output
			);
			output.Append('\n');
		}
		
		static void AppendCppFunctionPointer(
			string funcName,
			bool isStatic,
			ParameterInfo[] parameters,
			Type returnType,
			char separator,
			StringBuilder output
		)
		{
			// Return type
			if (returnType.IsValueType)
			{
				AppendCppTypeName(returnType, output);
			}
			else
			{
				output.Append("int32_t");
			}
			
			output.Append(" (*");
			output.Append(funcName);
			output.Append(")(");
			if (!isStatic)
			{
				output.Append("int32_t thisHandle");
				if (parameters.Length > 0)
				{
					output.Append(", ");
				}
			}
			AppendParameterDeclaration(
				parameters,
				"int32_t",
				AppendCppTypeName,
				output);
			output.Append(")");
			output.Append(separator);
		}
		
		static void AppendCppMethodDeclaration(
			string methodName,
			bool isStatic,
			Type returnType,
			Type[] typeParameters,
			ParameterInfo[] parameters,
			StringBuilder output)
		{
			if (typeParameters != null)
			{
				output.Append("template<typename ");
				for (int i = 0; i < typeParameters.Length; ++i)
				{
					output.Append('T');
					output.Append(i);
					if (i != typeParameters.Length - 1)
					{
						output.Append(", typename ");
					}
				}
				output.Append("> ");
			}
			
			if (isStatic)
			{
				output.Append("static ");
			}
			
			// Return type
			if (typeParameters != null)
			{
				output.Append("T0 ");
			}
			else if (returnType != null)
			{
				AppendCppTypeName(returnType, output);
				output.Append(' ');
			}
			
			output.Append(methodName);
			output.Append('(');
			
			// Parameters
			AppendParameterDeclaration(
				parameters,
				null,
				AppendCppTypeName,
				output);
			output.Append(");\n");
		}
		
		static void AppendCsharpTypeName(
			Type type,
			StringBuilder output)
		{
			if (type.Equals(typeof(void)))
			{
				output.Append("void");
			}
			else if (type.Equals(typeof(bool)))
			{
				output.Append("bool");
			}
			else if (type.Equals(typeof(sbyte)))
			{
				output.Append("sbyte");
			}
			else if (type.Equals(typeof(byte)))
			{
				output.Append("byte");
			}
			else if (type.Equals(typeof(short)))
			{
				output.Append("short");
			}
			else if (type.Equals(typeof(ushort)))
			{
				output.Append("ushort");
			}
			else if (type.Equals(typeof(int)))
			{
				output.Append("int");
			}
			else if (type.Equals(typeof(uint)))
			{
				output.Append("uint");
			}
			else if (type.Equals(typeof(long)))
			{
				output.Append("long");
			}
			else if (type.Equals(typeof(ulong)))
			{
				output.Append("ulong");
			}
			else if (type.Equals(typeof(string)))
			{
				output.Append("string");
			}
			else
			{
				output.Append(type.Namespace);
				output.Append('.');
				output.Append(type.Name);
			}
		}
		
		static void AppendCppTypeName(
			Type type,
			StringBuilder output)
		{
			if (type.Equals(typeof(void)))
			{
				output.Append("void");
			}
			else if (type.Equals(typeof(bool)))
			{
				output.Append("System::Boolean");
			}
			else if (type.Equals(typeof(sbyte)))
			{
				output.Append("int8_t");
			}
			else if (type.Equals(typeof(byte)))
			{
				output.Append("uint8_t");
			}
			else if (type.Equals(typeof(short)))
			{
				output.Append("int16_t");
			}
			else if (type.Equals(typeof(ushort)))
			{
				output.Append("uint16_t");
			}
			else if (type.Equals(typeof(int)))
			{
				output.Append("int32_t");
			}
			else if (type.Equals(typeof(uint)))
			{
				output.Append("uint32_t");
			}
			else if (type.Equals(typeof(long)))
			{
				output.Append("int64_t");
			}
			else if (type.Equals(typeof(ulong)))
			{
				output.Append("uint64_t");
			}
			else if (type.Equals(typeof(string)))
			{
				output.Append("System::String");
			}
			else
			{
				AppendCppTypeName(
					type.Namespace,
					type.Name,
					output);
			}
		}
		
		static void AppendCppTypeName(
			string namespaceName,
			string name,
			StringBuilder output)
		{
			AppendNamespace(namespaceName, "::", output);
			output.Append("::");
			output.Append(name);
		}
		
		static void LogStringBuilder(
			string title,
			StringBuilder builder)
		{
			Debug.LogFormat(
				"{0}:\n\n{1}\n\n",
				title,
				builder);
		}
		
		static void RemoveTrailingChars(
			StringBuilder builder)
		{
			int len = builder.Length;
			int i;
			for (i = len - 1; i >= 0; --i)
			{
				char cur = builder[i];
				switch (cur)
				{
					case '\n':
					case '\t':
					case ',':
						break;
					default:
						goto after;
				}
			}
			after:
			if (i < len - 1)
			{
				builder.Remove(i + 1, len - i - 1);
			}
		}
		
		static string InjectIntoString(
			string contents,
			string beginMarker,
			string endMarker,
			string text)
		{
			for (int startIndex = 0; true; )
			{
				int beginIndex = contents.IndexOf(beginMarker, startIndex);
				if (beginIndex < 0)
				{
					return contents;
				}
				int afterBeginIndex = beginIndex + beginMarker.Length;
				int endIndex = contents.IndexOf(endMarker, afterBeginIndex);
				if (endIndex < 0)
				{
					throw new Exception(
						string.Format(
							"No end ({0}) for begin ({1}) at {2} after {3}",
							endMarker,
							beginMarker,
							beginIndex,
							startIndex));
				}
				string begin = contents.Substring(0, afterBeginIndex);
				string end = contents.Substring(endIndex);
				contents = begin + text + end;
				startIndex = beginIndex + 1;
			}
		}
	}
}
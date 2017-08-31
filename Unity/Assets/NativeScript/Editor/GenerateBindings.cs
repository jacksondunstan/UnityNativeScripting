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
	/// * Generic return values
	/// * out and ref parameters
	/// 
	/// Does Not Support:
	/// * Arrays (single- or multi-dimensional)
	/// * Struct types
	/// * Generic method parameters
	/// * Generic types
	/// * Delegates
	/// * MonoBehaviour contents (e.g. fields) except for "message" functions
	/// * Overloaded operators
	/// * Exceptions
	/// * Default parameters
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
		class JsonConstructor
		{
			public string[] Types;
		}
		
		[Serializable]
		class JsonGenericType
		{
			public string Name;
			public string Type;
		}
		
		[Serializable]
		class JsonMethod
		{
			public string Name;
			public string ReturnType;
			public string[] ParamTypes;
			public JsonGenericType[] GenericTypes;
		}
		
		[Serializable]
		class JsonType
		{
			public string Name;
			public JsonConstructor[] Constructors;
			public JsonMethod[] Methods;
			public string[] Properties;
			public string[] Fields;
		}
		
		[Serializable]
		class JsonAssembly
		{
			public string Path;
			public JsonType[] Types;
		}
		
		[Serializable]
		class JsonMonoBehaviour
		{
			public string Name;
			public string Namespace;
			public string[] Messages;
		}
		
		[Serializable]
		class JsonDocument
		{
			public JsonAssembly[] Assemblies;
			public JsonMonoBehaviour[] MonoBehaviours;
		}
		
		const int InitialStringBuilderCapacity = 1024 * 5;
		
		class StringBuilders
		{
			public StringBuilder CsharpInitParams =
				new StringBuilder(InitialStringBuilderCapacity);
			public StringBuilder CsharpDelegateTypes =
				new StringBuilder(InitialStringBuilderCapacity);
			public StringBuilder CsharpInitCall =
				new StringBuilder(InitialStringBuilderCapacity);
			public StringBuilder CsharpFunctions =
				new StringBuilder(InitialStringBuilderCapacity);
			public StringBuilder CsharpMonoBehaviours =
				new StringBuilder(InitialStringBuilderCapacity);
			public StringBuilder CsharpMonoBehaviourDelegates =
				new StringBuilder(InitialStringBuilderCapacity);
			public StringBuilder CsharpMonoBehaviourImports =
				new StringBuilder(InitialStringBuilderCapacity);
			public StringBuilder CsharpMonoBehaviourGetDelegateCalls =
				new StringBuilder(InitialStringBuilderCapacity);
			public StringBuilder CppFunctionPointers =
				new StringBuilder(InitialStringBuilderCapacity);
			public StringBuilder CppTypeDeclarations =
				new StringBuilder(InitialStringBuilderCapacity);
			public StringBuilder CppTypeDefinitions =
				new StringBuilder(InitialStringBuilderCapacity);
			public StringBuilder CppMethodDefinitions =
				new StringBuilder(InitialStringBuilderCapacity);
			public StringBuilder CppInitParams =
				new StringBuilder(InitialStringBuilderCapacity);
			public StringBuilder CppInitBody =
				new StringBuilder(InitialStringBuilderCapacity);
			public StringBuilder CppMonoBehaviourMessages =
				new StringBuilder(InitialStringBuilderCapacity);
			public StringBuilder TempStrBuilder =
				new StringBuilder(InitialStringBuilderCapacity);
		}
		
		class ParameterInfo
		{
			public string Name;
			public Type ParameterType;
			public Type DereferencedParameterType;
			public bool IsOut;
			public bool IsRef;
			public bool IsStruct;
		}
		
		class MessageInfo
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
		
		static readonly MessageInfo[] messageInfos = new[] {
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
				
				// Generate stub types
				// We'll need to be able to get these via reflection later
				StringBuilder csharpMonoBehaviours = new StringBuilder(
					InitialStringBuilderCapacity);
				string timestamp = DateTime.Now.ToLongTimeString();
				AppendStubMonoBehaviours(
					doc.MonoBehaviours,
					timestamp,
					csharpMonoBehaviours);
				
				// Inject
				string csharpContents = File.ReadAllText(CsharpPath);
				csharpContents = InjectIntoString(
					csharpContents,
					"/*BEGIN MONOBEHAVIOURS*/\n",
					"\n/*END MONOBEHAVIOURS*/",
					csharpMonoBehaviours.ToString());
				File.WriteAllText(CsharpPath, csharpContents);
				
				// Compile and continue after scripts are refreshed
				Debug.Log("Waiting for compile...");
				AssetDatabase.Refresh();
				EditorPrefs.SetBool(PostCompileWorkPref, true);
			}
		}
		
		static void AppendStubMonoBehaviours(
			JsonMonoBehaviour[] monoBehaviours,
			string timestamp,
			StringBuilder output)
		{
			if (monoBehaviours != null)
			{
				foreach (JsonMonoBehaviour monoBehaviour in monoBehaviours)
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
			}
		}
		
		[UnityEditor.Callbacks.DidReloadScripts]
		static void OnScriptsReloaded()
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
			
			StringBuilders builders = new StringBuilders();
			if (doc.Assemblies != null)
			{
				foreach (JsonAssembly jsonAssembly in doc.Assemblies)
				{
					AppendAssembly(
						jsonAssembly,
						builders);
				}
			}
			if (doc.MonoBehaviours != null)
			{
				foreach (JsonMonoBehaviour jsonMonoBehaviour in doc.MonoBehaviours)
				{
					AppendMonoBehaviour(
						jsonMonoBehaviour,
						builders);
				}
			}
			
			RemoveTrailingChars(builders);
			
			if (dryRun)
			{
				LogStringBuilders(builders);
			}
			else
			{
				InjectBuilders(builders);
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
			Type type = assembly.GetType(typeName)
				?? typeof(string).Assembly.GetType(typeName)
				?? typeof(Vector3).Assembly.GetType(typeName)
				?? typeof(Bindings).Assembly.GetType(typeName);
			if (type == null)
			{
				StringBuilder errorBuilder = new StringBuilder(1024);
				errorBuilder.Append("Couldn't find type \"");
				errorBuilder.Append(typeName);
				errorBuilder.Append('"');
				throw new Exception(errorBuilder.ToString());
			}
			return type;
		}
		
		static MethodInfo GetMethod(
			Type type,
			string methodName,
			string returnTypeName,
			string[] paramTypeNames)
		{
			foreach (MethodInfo method in type.GetMethods())
			{
				if (method.Name != methodName)
				{
					continue;
				}
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
						if (
							method.ReturnType.Namespace + "." + method.ReturnType.Name
							!= returnTypeName)
						{
							continue;
						}
					}
				}
				ParameterInfo[] parameters = ConvertParameters(
					method.GetParameters());
				for (int i = 0; i < parameters.Length; ++i)
				{
					Type paramType = parameters[i].DereferencedParameterType;
					if (string.IsNullOrEmpty(paramType.Namespace))
					{
						if (paramType.Name != paramTypeNames[i])
						{
							goto mismatch;
						}
					}
					else
					{
						if (
							paramType.Namespace + "." + paramType.Name
							!= paramTypeNames[i])
						{
							goto mismatch;
						}
					}
				}
				return method;
				mismatch:;
			}
			
			// Throw an exception so the user knows what to fix in the JSON
			StringBuilder errorBuilder = new StringBuilder(1024);
			errorBuilder.Append("Method \"");
			errorBuilder.Append(returnTypeName ?? "void");
			errorBuilder.Append(' ');
			AppendCsharpTypeName(type, errorBuilder);
			errorBuilder.Append('.');
			errorBuilder.Append(methodName);
			errorBuilder.Append('(');
			for (int i = 0; i < paramTypeNames.Length; ++i)
			{
				errorBuilder.Append(paramTypeNames[i]);
				if (i != paramTypeNames.Length - 1)
				{
					errorBuilder.Append(", ");
				}
			}
			errorBuilder.Append(")\" not found");
			throw new Exception(errorBuilder.ToString());
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
					int separatorIndex = namespaceName.IndexOf(
						'.',
						startIndex);
					if (separatorIndex < 0)
					{
						separatorIndex = namespaceName.IndexOf(
							'+',
							startIndex);
						if (separatorIndex < 0)
						{
							break;
						}
						break;
					}
					output.Append(
						namespaceName,
						startIndex,
						separatorIndex - startIndex);
					output.Append(separator);
					startIndex = separatorIndex + 1;
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
				info.IsOut = reflectionInfo.IsOut;
				info.IsRef = !info.IsOut && info.ParameterType.IsByRef;
				info.DereferencedParameterType = info.IsRef || info.IsOut
					? info.ParameterType.GetElementType()
					: info.ParameterType;
				info.IsStruct = info.DereferencedParameterType.IsValueType;
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
				info.IsOut = false;
				info.IsRef = false;
				info.DereferencedParameterType = paramType;
				info.IsStruct = info.DereferencedParameterType.IsValueType;
				parameters[i] = info;
			}
			return parameters;
		}
		
		static string GetTypeNameLower(Type type)
		{
			return char.ToLower(type.Name[0]) + type.Name.Substring(1);
		}
		
		static bool IsStatic(Type type)
		{
			return type.IsAbstract && type.IsSealed;
		}
		
		static void AppendAssembly(
			JsonAssembly jsonAssembly,
			StringBuilders builders)
		{
			Assembly assembly = Assembly.LoadFrom(jsonAssembly.Path);
			foreach (JsonType jsonType in jsonAssembly.Types)
			{
				AppendType(
					jsonType,
					assembly,
					builders);
			}
		}
		
		static void AppendType(
			JsonType jsonType,
			Assembly assembly,
			StringBuilders builders)
		{
			Type type = GetType(jsonType.Name, assembly);
			string typeNameLower = GetTypeNameLower(type);
			bool isStatic = IsStatic(type);
			
			// C++ type declaration
			int indent = AppendCppTypeDeclaration(
				type.Namespace,
				type.Name,
				isStatic,
				builders.CppTypeDeclarations);
			
			// C++ type definition (beginning)
			AppendCppTypeDefinitionBegin(
				type.Namespace,
				type.Name,
				type.BaseType.Namespace,
				type.BaseType.Name,
				isStatic,
				indent,
				builders.CppTypeDefinitions);
			
			// C++ method definition
			int cppMethodDefinitionsIndent = AppendCppMethodDefinitionBegin(
				type.Namespace,
				type.Name,
				type.BaseType.Namespace,
				type.BaseType.Name,
				isStatic,
				indent,
				builders.CppMethodDefinitions);
			
			// Constructors
			if (jsonType.Constructors != null)
			{
				foreach (JsonConstructor jsonCtor in jsonType.Constructors)
				{
					AppendConstructor(
						jsonCtor,
						assembly,
						type,
						typeNameLower,
						indent,
						builders);
				}
			}
			
			// Properties
			if (jsonType.Properties != null)
			{
				foreach (string jsonPropertyName in jsonType.Properties)
				{
					AppendProperty(
						jsonPropertyName,
						type,
						typeNameLower,
						indent,
						builders);
				}
			}
			
			// Fields
			if (jsonType.Fields != null)
			{
				foreach (string jsonFieldName in jsonType.Fields)
				{
					AppendField(
						jsonFieldName,
						type,
						typeNameLower,
						indent,
						builders
					);
				}
			}
			
			// Methods
			if (jsonType.Methods != null)
			{
				foreach (JsonMethod jsonMethod in jsonType.Methods)
				{
					AppendMethod(
						jsonMethod,
						assembly,
						type,
						typeNameLower,
						indent,
						builders);
				}
			}
			
			// C++ type definition (ending)
			AppendCppTypeDefinitionEnd(
				isStatic,
				indent,
				builders.CppTypeDefinitions);
			
			// C++ method definition (ending)
			AppendCppMethodDefinitionEnd(
				cppMethodDefinitionsIndent,
				builders.CppMethodDefinitions);
		}
		
		static void AppendConstructor(
			JsonConstructor jsonCtor,
			Assembly assembly,
			Type enclosingType,
			string typeNameLower,
			int indent,
			StringBuilders builders)
		{
			Type[] paramTypes = GetTypes(jsonCtor.Types, assembly);
			ConstructorInfo ctor = enclosingType.GetConstructor(paramTypes);
			ParameterInfo[] parameters = ConvertParameters(
				ctor.GetParameters());
			
			// Build uppercase function name
			builders.TempStrBuilder.Length = 0;
			builders.TempStrBuilder.Append(enclosingType.Name);
			builders.TempStrBuilder.Append("Constructor");
			AppendTypeNames(paramTypes, builders.TempStrBuilder);
			string funcName = builders.TempStrBuilder.ToString();
			
			// Build lowercase function name
			builders.TempStrBuilder.Length = 0;
			builders.TempStrBuilder.Append(typeNameLower);
			builders.TempStrBuilder.Append("Constructor");
			AppendTypeNames(paramTypes, builders.TempStrBuilder);
			string funcNameLower = builders.TempStrBuilder.ToString();
			
			// C# init param declaration
			AppendCsharpInitParam(funcNameLower, builders.CsharpInitParams);

			// C# delegate type
			AppendCsharpDelegateType(
				funcName,
				true,
				typeof(int),
				parameters,
				builders.CsharpDelegateTypes);

			// C# init call param
			AppendCsharpInitCallArg(funcName, builders.CsharpInitCall);

			// C# function
			AppendCsharpFunctionBeginning(
				enclosingType,
				funcName,
				true,
				typeof(int),
				null,
				parameters,
				builders.CsharpFunctions);
			builders.CsharpFunctions.Append("NativeScript.Bindings.StoreObject(");
			builders.CsharpFunctions.Append("new ");
			AppendCsharpTypeName(
				enclosingType,
				builders.CsharpFunctions);
			AppendCsharpFunctionCallParameters(
				true,
				parameters,
				builders.CsharpFunctions);
			builders.CsharpFunctions.Append(");");
			AppendCsharpFunctionReturn(
				parameters,
				typeof(int),
				builders.CsharpFunctions);
			
			// C++ function pointer
			AppendCppFunctionPointerDefinition(
				funcName,
				true,
				parameters,
				enclosingType,
				builders.CppFunctionPointers);
			
			// C++ type declaration
			AppendIndent(
				indent + 1,
				builders.CppTypeDefinitions);
			AppendCppMethodDeclaration(
				enclosingType.Name,
				false,
				null,
				null,
				parameters,
				builders.CppTypeDefinitions);
			
			// C++ method definition
			AppendCppMethodDefinition(
				enclosingType,
				null,
				enclosingType.Name,
				null,
				parameters,
				indent,
				builders.CppMethodDefinitions);
			AppendIndent(indent + 1, builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append(" : ");
			AppendCppTypeName(
				enclosingType.BaseType,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("(0)\n");
			AppendIndent(indent, builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("{\n");
			AppendCppPluginFunctionCall(
				true,
				enclosingType,
				funcName,
				parameters,
				indent + 1,
				builders.CppMethodDefinitions);
			AppendIndent(indent + 1, builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("SetHandle(returnValue);\n");
			AppendIndent(indent, builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("}\n");
			AppendIndent(indent, builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("\n");

			// C++ init params
			AppendCppInitParam(
				funcNameLower,
				true,
				parameters,
				enclosingType,
				builders.CppInitParams);
			
			// C++ init body
			AppendCppInitBody(
				funcName,
				funcNameLower,
				builders.CppInitBody);
		}
		
		static void AppendProperty(
			string jsonPropertyName,
			Type type,
			string typeNameLower,
			int indent,
			StringBuilders builders)
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
		
		static void AppendField(
			string jsonFieldName,
			Type type,
			string typeNameLower,
			int indent,
			StringBuilders builders
		)
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
			setParam.IsOut = false;
			setParam.IsRef = false;
			setParam.DereferencedParameterType = setParam.ParameterType;
			setParam.IsStruct = setParam.DereferencedParameterType.IsValueType;
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
		
		static void AppendMethod(
			JsonMethod jsonMethod,
			Assembly assembly,
			Type type,
			string typeNameLower,
			int indent,
			StringBuilders builders)
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
						assembly,
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
					assembly,
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
		
		static void AppendMethod(
			Type type,
			Assembly assembly,
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
			AppendMethodFuncName(
				type.Name,
				methodName,
				paramTypes,
				typeParameters,
				stringBuilders.TempStrBuilder);
			string funcName = stringBuilders.TempStrBuilder.ToString();
			
			// Build lowercase function name
			stringBuilders.TempStrBuilder.Length = 0;
			AppendMethodFuncName(
				typeNameLower,
				methodName,
				paramTypes,
				typeParameters,
				stringBuilders.TempStrBuilder);
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
			AppendCSharpTypeParameters(
				typeParameters,
				stringBuilders.CsharpFunctions);
			AppendCsharpFunctionCallParameters(
				isStatic,
				parameters,
				stringBuilders.CsharpFunctions);
			stringBuilders.CsharpFunctions.Append(';');
			AppendCsharpFunctionReturn(
				parameters,
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
			AppendCppPluginFunctionCall(
				isStatic,
				returnType,
				funcName,
				parameters,
				indent + 1,
				stringBuilders.CppMethodDefinitions);
			AppendCppMethodReturn(
				returnType,
				indent + 1,
				stringBuilders.CppMethodDefinitions);
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
		
		static void AppendMethodFuncName(
			string typeName,
			string methodName,
			Type[] paramTypes,
			Type[] typeParameters,
			StringBuilder output)
		{
			output.Append(typeName);
			output.Append("Method");
			output.Append(methodName);
			AppendTypeNames(paramTypes, output);
			if (typeParameters != null)
			{
				foreach (Type typeParam in typeParameters)
				{
					AppendNamespace(
						typeParam.Namespace,
						string.Empty,
						output);
					output.Append(typeParam.Name);
				}
			}
		}
		
		static void AppendCSharpTypeParameters(
			Type[] typeParameters,
			StringBuilder output
		)
		{
			if (typeParameters != null)
			{
				output.Append('<');
				for (int i = 0; i < typeParameters.Length; ++i)
				{
					Type typeParam = typeParameters[i];
					AppendCsharpTypeName(typeParam, output);
					if (i != typeParameters.Length - 1)
					{
						output.Append(", ");
					}
				}
				output.Append('>');
			}
		}
		
		static void AppendMonoBehaviour(
			JsonMonoBehaviour jsonMonoBehaviour,
			StringBuilders builders)
		{
			// C++ Type Declaration
			int cppIndent = AppendCppTypeDeclaration(
				jsonMonoBehaviour.Namespace,
				jsonMonoBehaviour.Name,
				false,
				builders.CppTypeDeclarations);
			
			// C++ Type Definition (begin)
			AppendCppTypeDefinitionBegin(
				jsonMonoBehaviour.Namespace,
				jsonMonoBehaviour.Name,
				"UnityEngine",
				"MonoBehaviour",
				false,
				cppIndent,
				builders.CppTypeDefinitions
			);
			
			// C++ method definition
			int cppMethodDefinitionsIndent = AppendCppMethodDefinitionBegin(
				jsonMonoBehaviour.Namespace,
				jsonMonoBehaviour.Name,
				"UnityEngine",
				"MonoBehaviour",
				false,
				cppIndent,
				builders.CppMethodDefinitions);
			AppendCppMethodDefinitionEnd(
				cppMethodDefinitionsIndent,
				builders.CppMethodDefinitions);
			
			// C# Class extending MonoBehaviour
			int csharpIndent = AppendNamespaceBeginning(
				jsonMonoBehaviour.Namespace,
				builders.CsharpMonoBehaviours);
			AppendIndent(csharpIndent, builders.CsharpMonoBehaviours);
			builders.CsharpMonoBehaviours.Append("public class ");
			builders.CsharpMonoBehaviours.Append(jsonMonoBehaviour.Name);
			builders.CsharpMonoBehaviours.Append(" : UnityEngine.MonoBehaviour\n");
			AppendIndent(csharpIndent, builders.CsharpMonoBehaviours);
			builders.CsharpMonoBehaviours.Append("{\n");
			AppendIndent(csharpIndent + 1, builders.CsharpMonoBehaviours);
			builders.CsharpMonoBehaviours.Append("int thisHandle;\n");
			AppendIndent(csharpIndent + 1, builders.CsharpMonoBehaviours);
			builders.CsharpMonoBehaviours.Append('\n');
			AppendIndent(csharpIndent + 1, builders.CsharpMonoBehaviours);
			builders.CsharpMonoBehaviours.Append("public ");
			builders.CsharpMonoBehaviours.Append(jsonMonoBehaviour.Name);
			builders.CsharpMonoBehaviours.Append("()\n");
			AppendIndent(csharpIndent + 1, builders.CsharpMonoBehaviours);
			builders.CsharpMonoBehaviours.Append("{\n");
			AppendIndent(csharpIndent + 2, builders.CsharpMonoBehaviours);
			builders.CsharpMonoBehaviours.Append("thisHandle = NativeScript.Bindings.StoreObject(this);\n");
			AppendIndent(csharpIndent + 1, builders.CsharpMonoBehaviours);
			builders.CsharpMonoBehaviours.Append("}\n");
			if (jsonMonoBehaviour.Messages.Length > 0)
			{
				AppendIndent(csharpIndent + 1, builders.CsharpMonoBehaviours);
				builders.CsharpMonoBehaviours.Append('\n');
			}
			for (
				int messageIndex = 0;
				messageIndex < jsonMonoBehaviour.Messages.Length;
				++messageIndex)
			{
				string message = jsonMonoBehaviour.Messages[messageIndex];
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
					builders.CppTypeDefinitions);
				AppendCppMethodDeclaration(
					messageInfo.Name,
					false,
					typeof(void),
					null,
					parameters,
					builders.CppTypeDefinitions);
				
				AppendIndent(
					csharpIndent + 1,
					builders.CsharpMonoBehaviours);
				builders.CsharpMonoBehaviours.Append("public ");
				AppendCsharpTypeName(
					typeof(void),
					builders.CsharpMonoBehaviours);
				builders.CsharpMonoBehaviours.Append(' ');
				builders.CsharpMonoBehaviours.Append(messageInfo.Name);
				builders.CsharpMonoBehaviours.Append('(');
				for (int i = 0; i < numParams; ++i)
				{
					Type paramType = paramTypes[i];
					AppendCsharpTypeName(
						paramType,
						builders.CsharpMonoBehaviours);
					builders.CsharpMonoBehaviours.Append(' ');
					builders.CsharpMonoBehaviours.Append("param");
					builders.CsharpMonoBehaviours.Append(i);
					if (i != numParams - 1)
					{
						builders.CsharpMonoBehaviours.Append(", ");
					}
				}
				builders.CsharpMonoBehaviours.Append(")\n");
				AppendIndent(
					csharpIndent + 1,
					builders.CsharpMonoBehaviours);
				builders.CsharpMonoBehaviours.Append("{\n");
				for (int i = 0; i < numParams; ++i)
				{
					if (!parameters[i].IsStruct)
					{
						AppendIndent(
							csharpIndent + 2,
							builders.CsharpMonoBehaviours);
						builders.CsharpMonoBehaviours.Append("int param");
						builders.CsharpMonoBehaviours.Append(i);
						builders.CsharpMonoBehaviours.Append("Handle = ");
						builders.CsharpMonoBehaviours.Append("NativeScript.Bindings.GetHandle(");
						builders.CsharpMonoBehaviours.Append("param");
						builders.CsharpMonoBehaviours.Append(i);
						builders.CsharpMonoBehaviours.Append(");\n");
						AppendIndent(
							csharpIndent + 2,
							builders.CsharpMonoBehaviours);
						builders.CsharpMonoBehaviours.Append("if (param");
						builders.CsharpMonoBehaviours.Append(i);
						builders.CsharpMonoBehaviours.Append("Handle < 0)\n");
						AppendIndent(
							csharpIndent + 2,
							builders.CsharpMonoBehaviours);
						builders.CsharpMonoBehaviours.Append("{\n");
						AppendIndent(
							csharpIndent + 3,
							builders.CsharpMonoBehaviours);
						builders.CsharpMonoBehaviours.Append("param");
						builders.CsharpMonoBehaviours.Append(i);
						builders.CsharpMonoBehaviours.Append("Handle = NativeScript.Bindings.StoreObject(");
						builders.CsharpMonoBehaviours.Append("param");
						builders.CsharpMonoBehaviours.Append(i);
						builders.CsharpMonoBehaviours.Append(");\n");
						AppendIndent(
							csharpIndent + 2,
							builders.CsharpMonoBehaviours);
						builders.CsharpMonoBehaviours.Append("}\n");
					}
				}
				AppendIndent(
					csharpIndent + 2,
					builders.CsharpMonoBehaviours);
				builders.CsharpMonoBehaviours.Append("NativeScript.Bindings.");
				builders.CsharpMonoBehaviours.Append(jsonMonoBehaviour.Name);
				builders.CsharpMonoBehaviours.Append(messageInfo.Name);
				builders.CsharpMonoBehaviours.Append("(thisHandle");
				if (numParams > 0)
				{
					builders.CsharpMonoBehaviours.Append(", ");
				}
				for (int i = 0; i < numParams; ++i)
				{
					builders.CsharpMonoBehaviours.Append("param");
					builders.CsharpMonoBehaviours.Append(i);
					if (!parameters[i].IsStruct)
					{
						builders.CsharpMonoBehaviours.Append("Handle");
					}
					if (i != numParams - 1)
					{
						builders.CsharpMonoBehaviours.Append(", ");
					}
				}
				builders.CsharpMonoBehaviours.Append(");\n");
				AppendIndent(
					csharpIndent + 1,
					builders.CsharpMonoBehaviours);
				builders.CsharpMonoBehaviours.Append("}\n");
				if (messageIndex != jsonMonoBehaviour.Messages.Length - 1)
				{
					AppendIndent(
						csharpIndent + 1,
						builders.CsharpMonoBehaviours);
					builders.CsharpMonoBehaviours.Append('\n');
				}
				
				// C# Delegate
				builders.CsharpMonoBehaviourDelegates.Append(
					"\t\tpublic delegate void ");
				builders.CsharpMonoBehaviourDelegates.Append(
					jsonMonoBehaviour.Name);
				builders.CsharpMonoBehaviourDelegates.Append(
					messageInfo.Name);
				builders.CsharpMonoBehaviourDelegates.Append(
					"Delegate(int thisHandle");
				if (numParams > 0)
				{
					builders.CsharpMonoBehaviourDelegates.Append(", ");
				}
				for (int i = 0; i < numParams; ++i)
				{
					ParameterInfo param = parameters[i];
					if (param.IsStruct)
					{
						AppendCsharpTypeName(
							param.ParameterType,
							builders.CsharpMonoBehaviourDelegates);
						builders.CsharpMonoBehaviourDelegates.Append(" param");
						builders.CsharpMonoBehaviourDelegates.Append(i);
					}
					else
					{
						builders.CsharpMonoBehaviourDelegates.Append("int param");
						builders.CsharpMonoBehaviourDelegates.Append(i);
					}
					if (i != numParams-1)
					{
						builders.CsharpMonoBehaviourDelegates.Append(", ");
					}
				}
				builders.CsharpMonoBehaviourDelegates.Append(");\n");
				builders.CsharpMonoBehaviourDelegates.Append("\t\tpublic static ");
				builders.CsharpMonoBehaviourDelegates.Append(jsonMonoBehaviour.Name);
				builders.CsharpMonoBehaviourDelegates.Append(messageInfo.Name);
				builders.CsharpMonoBehaviourDelegates.Append("Delegate ");
				builders.CsharpMonoBehaviourDelegates.Append(jsonMonoBehaviour.Name);
				builders.CsharpMonoBehaviourDelegates.Append(messageInfo.Name);
				builders.CsharpMonoBehaviourDelegates.Append(";\n\t\t\n");
				
				// C# Import
				builders.CsharpMonoBehaviourImports.Append("\t\t[DllImport(Constants.PluginName)]\n");
				builders.CsharpMonoBehaviourImports.Append("\t\tpublic static extern void ");
				builders.CsharpMonoBehaviourImports.Append(jsonMonoBehaviour.Name);
				builders.CsharpMonoBehaviourImports.Append(messageInfo.Name);
				builders.CsharpMonoBehaviourImports.Append("(int thisHandle");
				if (numParams > 0)
				{
					builders.CsharpMonoBehaviourImports.Append(", ");
				}
				for (int i = 0; i < numParams; ++i)
				{
					ParameterInfo param = parameters[i];
					if (param.IsStruct)
					{
						AppendCsharpTypeName(
							param.ParameterType,
							builders.CsharpMonoBehaviourImports);
						builders.CsharpMonoBehaviourImports.Append(" param");
						builders.CsharpMonoBehaviourImports.Append(i);
					}
					else
					{
						builders.CsharpMonoBehaviourImports.Append("int param");
						builders.CsharpMonoBehaviourImports.Append(i);
					}
					if (i != numParams-1)
					{
						builders.CsharpMonoBehaviourImports.Append(", ");
					}
				}
				builders.CsharpMonoBehaviourImports.Append(");\n\t\t\n");
				
				// C# GetDelegate Call
				builders.CsharpMonoBehaviourGetDelegateCalls.Append("\t\t\t");
				builders.CsharpMonoBehaviourGetDelegateCalls.Append(jsonMonoBehaviour.Name);
				builders.CsharpMonoBehaviourGetDelegateCalls.Append(messageInfo.Name);
				builders.CsharpMonoBehaviourGetDelegateCalls.Append(" = GetDelegate<");
				builders.CsharpMonoBehaviourGetDelegateCalls.Append(jsonMonoBehaviour.Name);
				builders.CsharpMonoBehaviourGetDelegateCalls.Append(messageInfo.Name);
				builders.CsharpMonoBehaviourGetDelegateCalls.Append("Delegate>(libraryHandle, \"");
				builders.CsharpMonoBehaviourGetDelegateCalls.Append(jsonMonoBehaviour.Name);
				builders.CsharpMonoBehaviourGetDelegateCalls.Append(messageInfo.Name);
				builders.CsharpMonoBehaviourGetDelegateCalls.Append("\");\n");
				
				// C++ Message
				builders.CppMonoBehaviourMessages.Append("DLLEXPORT void ");
				builders.CppMonoBehaviourMessages.Append(jsonMonoBehaviour.Name);
				builders.CppMonoBehaviourMessages.Append(messageInfo.Name);
				builders.CppMonoBehaviourMessages.Append("(int32_t thisHandle");
				if (numParams > 0)
				{
					builders.CppMonoBehaviourMessages.Append(", ");
				}
				for (int i = 0; i < numParams; ++i)
				{
					ParameterInfo param = parameters[i];
					if (param.IsStruct)
					{
						AppendCppTypeName(
							param.ParameterType,
							builders.CppMonoBehaviourMessages);
						builders.CppMonoBehaviourMessages.Append(" param");
						builders.CppMonoBehaviourMessages.Append(i);
					}
					else
					{
						builders.CppMonoBehaviourMessages.Append("int32_t param");
						builders.CppMonoBehaviourMessages.Append(i);
						builders.CppMonoBehaviourMessages.Append("Handle");
					}
					if (i != numParams-1)
					{
						builders.CppMonoBehaviourMessages.Append(", ");
					}
				}
				builders.CppMonoBehaviourMessages.Append(")\n{\n\t");
				AppendCppTypeName(
					jsonMonoBehaviour.Namespace,
					jsonMonoBehaviour.Name,
					builders.CppMonoBehaviourMessages);
				builders.CppMonoBehaviourMessages.Append(" thiz(thisHandle);\n");
				for (int i = 0; i < numParams; ++i)
				{
					ParameterInfo param = parameters[i];
					if (!param.IsStruct)
					{
						builders.CppMonoBehaviourMessages.Append('\t');
						AppendCppTypeName(
							param.ParameterType,
							builders.CppMonoBehaviourMessages);
						builders.CppMonoBehaviourMessages.Append(" param");
						builders.CppMonoBehaviourMessages.Append(i);
						builders.CppMonoBehaviourMessages.Append("(param");
						builders.CppMonoBehaviourMessages.Append(i);
						builders.CppMonoBehaviourMessages.Append("Handle);\n");
					}
				}
				builders.CppMonoBehaviourMessages.Append("\tthiz.");
				builders.CppMonoBehaviourMessages.Append(messageInfo.Name);
				builders.CppMonoBehaviourMessages.Append("(");
				for (int i = 0; i < numParams; ++i)
				{
					builders.CppMonoBehaviourMessages.Append("param");
					builders.CppMonoBehaviourMessages.Append(i);
					if (i != numParams-1)
					{
						builders.CppMonoBehaviourMessages.Append(", ");
					}
				}
				builders.CppMonoBehaviourMessages.Append(");\n}\n\n");
			}
			
			// C# Class extending MonoBehaviour (end)
			AppendIndent(csharpIndent, builders.CsharpMonoBehaviours);
			builders.CsharpMonoBehaviours.Append("}\n");
			AppendNamespaceEnding(csharpIndent, builders.CsharpMonoBehaviours);
			
			// C++ Type Definition (end)
			AppendCppTypeDefinitionEnd(
				false,
				cppIndent,
				builders.CppTypeDefinitions);
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
				parameters,
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
			AppendCppPluginFunctionCall(
				isStatic,
				fieldType,
				funcName,
				parameters,
				indent + 1,
				stringBuilders.CppMethodDefinitions);
			AppendCppMethodReturn(
				fieldType,
				indent + 1,
				stringBuilders.CppMethodDefinitions);
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
			stringBuilders.CsharpFunctions.Append("value;");
			AppendCsharpFunctionReturn(
				parameters,
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
			AppendCppPluginFunctionCall(
				isStatic,
				null,
				funcName,
				parameters,
				indent + 1,
				stringBuilders.CppMethodDefinitions);
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
		
		static int AppendCppTypeDeclaration(
			string typeNamespace,
			string typeName,
			bool isStatic,
			StringBuilder output)
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
				if (baseTypeNamespace != null && baseTypeName != null)
				{
					output.Append(" : ");
					output.Append(baseTypeNamespace);
					output.Append("::");
					output.Append(baseTypeName);
				}
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
			StringBuilder output)
		{
			AppendIndent(
				indent,
				output);
			output.Append('}');
			if (!isStatic)
			{
				output.Append(';');
			}
			output.Append('\n');
			AppendNamespaceEnding(
				indent,
				output);
			output.Append('\n');
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
			if (baseTypeNamespace != null && baseTypeName != null)
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
			AppendCsharpParameterDeclaration(
				parameters,
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
			AppendCsharpParameterDeclaration(
				parameters,
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
					")NativeScript.Bindings.GetObject(thisHandle);\n\t\t\t");
			}
			
			// Get reference type params from ObjectStore
			foreach (ParameterInfo param in parameters)
			{
				Type paramType = param.DereferencedParameterType;
				if (!param.IsStruct)
				{
					output.Append("var ");
					output.Append(param.Name);
					output.Append(" = ");
					if (!paramType.Equals(typeof(object)))
					{
						output.Append('(');
						output.Append(paramType);
						output.Append(')');
					}
					output.Append("NativeScript.Bindings.GetObject(");
					output.Append(param.Name);
					output.Append("Handle);\n\t\t\t");
				}
			}
			
			// Save return value as local variable
			if (!returnType.Equals(typeof(void)))
			{
				output.Append("var returnValue = ");
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
				ParameterInfo param = parameters[i];
				if (param.IsOut)
				{
					output.Append("out ");
				}
				else if (param.IsRef)
				{
					output.Append("ref ");
				}
				output.Append(param.Name);
				if (i != parameters.Length - 1)
				{
					output.Append(", ");
				}
			}
			output.Append(')');
		}
		
		static void AppendCsharpFunctionReturn(
			ParameterInfo[] parameters,
			Type returnType,
			StringBuilder output)
		{
			// Store reference out and ref params and overwrite handles
			foreach (ParameterInfo param in parameters)
			{
				if (!param.IsStruct && (param.IsOut || param.IsRef))
				{
					output.Append("\n\t\t\tint ");
					output.Append(param.Name);
					output.Append("HandleNew = NativeScript.Bindings.GetHandle(");
					output.Append(param.Name);
					output.Append(");\n\t\t\tif (");
					output.Append(param.Name);
					output.Append("HandleNew < 0)\n\t\t\t{\n\t\t\t\t");
					output.Append(param.Name);
					output.Append("Handle = NativeScript.Bindings.StoreObject(");
					output.Append(param.Name);
					output.Append(");\n\t\t\t}\n\t\t\telse\n\t\t\t{\n\t\t\t\t");
					output.Append(param.Name);
					output.Append("Handle = ");
					output.Append(param.Name);
					output.Append("HandleNew;\n\t\t\t}");
				}
			}
			if (!returnType.Equals(typeof(void)))
			{
				output.Append('\n');
				if (returnType.IsValueType)
				{
					output.Append("\t\t\treturn returnValue;");
				}
				else
				{
					output.Append("\t\t\tint returnValueHandle = NativeScript.Bindings.GetHandle(returnValue);\n");
					output.Append("\t\t\tif (returnValueHandle < 0)\n");
					output.Append("\t\t\t{\n");
					output.Append("\t\t\t\treturn NativeScript.Bindings.StoreObject(returnValue);\n");
					output.Append("\t\t\t}\n");
					output.Append("\t\t\telse\n");
					output.Append("\t\t\t{\n");
					output.Append("\t\t\t\treturn returnValueHandle;\n");
					output.Append("\t\t\t}");
				}
			}
			output.Append("\n\t\t}\n\t\t\n");
		}
		
		static void AppendCsharpParameterDeclaration(
			ParameterInfo[] parameters,
			StringBuilder output)
		{
			for (int i = 0; i < parameters.Length; ++i)
			{
				ParameterInfo param = parameters[i];
				if (param.IsOut)
				{
					if (param.IsStruct)
					{
						output.Append("out ");
					}
					else
					{
						output.Append("ref ");
					}
				}
				if (param.IsRef)
				{
					output.Append("ref ");
				}
				if (param.IsStruct)
				{
					AppendCsharpTypeName(
						param.DereferencedParameterType,
						output);
				}
				else
				{
					output.Append("int");
				}
				output.Append(' ');
				output.Append(param.Name);
				if (!param.IsStruct)
				{
					output.Append("Handle");
				}
				if (i != parameters.Length - 1)
				{
					output.Append(", ");
				}
			}
		}
		
		static void AppendCppParameterDeclaration(
			ParameterInfo[] parameters,
			StringBuilder output)
		{
			for (int i = 0; i < parameters.Length; ++i)
			{
				ParameterInfo param = parameters[i];
				AppendCppTypeName(
					param.DereferencedParameterType,
					output);
				if (param.IsOut || param.IsRef)
				{
					output.Append('*');
				}
				output.Append(' ');
				output.Append(param.Name);
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
				if (!parameter.IsStruct)
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
			AppendCppParameterDeclaration(
				parameters,
				output);
			output.Append(")\n");
		}
		
		static void AppendCppMethodReturn(
			Type returnType,
			int indent,
			StringBuilder output)
		{
			if (returnType != null && !returnType.Equals(typeof(void)))
			{
				AppendIndent(indent, output);
				output.Append("return returnValue;\n");
			}
		}
		
		static void AppendCppPluginFunctionCall(
			bool isStatic,
			Type returnType,
			string funcName,
			ParameterInfo[] parameters,
			int indent,
			StringBuilder output)
		{
			// Gather handles for out and ref parameters
			foreach (ParameterInfo param in parameters)
			{
				if (!param.IsStruct && (param.IsOut || param.IsRef))
				{
					AppendIndent(indent, output);
					output.Append("int32_t ");
					output.Append(param.Name);
					output.Append("Handle = ");
					output.Append(param.Name);
					output.Append("->Handle;\n");
				}
			}
			
			// Call the function
			AppendIndent(indent, output);
			if (returnType != null && returnType != typeof(void))
			{
				output.Append("auto returnValue = ");
			}
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
				ParameterInfo param = parameters[i];
				if (param.IsStruct)
				{
					output.Append(param.Name);
				}
				else
				{
					if (param.IsOut || param.IsRef)
					{
						output.Append('&');
						output.Append(param.Name);
					}
					else
					{
						output.Append(param.Name);
						output.Append('.');
					}
					output.Append("Handle");
				}
				if (i != parameters.Length - 1)
				{
					output.Append(", ");
				}
			}
			output.Append(");\n");
			
			// Set out and ref parameters
			foreach (ParameterInfo param in parameters)
			{
				if (!param.IsStruct && (param.IsOut || param.IsRef))
				{
					AppendIndent(indent, output);
					output.Append(param.Name);
					output.Append("->SetHandle(");
					output.Append(param.Name);
					output.Append("Handle);\n");
				}
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
			StringBuilder output)
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
			for (int i = 0; i < parameters.Length; ++i)
			{
				ParameterInfo param = parameters[i];
				if (param.IsStruct)
				{
					AppendCppTypeName(
						param.DereferencedParameterType,
						output);
				}
				else
				{
					output.Append("int32_t");
				}
				if (param.IsOut || param.IsRef)
				{
					output.Append('*');
				}
				output.Append(' ');
				output.Append(param.Name);
				if (!param.IsStruct)
				{
					output.Append("Handle");
				}
				if (i != parameters.Length - 1)
				{
					output.Append(", ");
				}
			}
			output.Append(')');
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
			AppendCppParameterDeclaration(
				parameters,
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
		
		static void LogStringBuilders(
			StringBuilders builders)
		{
			LogStringBuilder(
				"C# init params",
				builders.CsharpInitParams);
			LogStringBuilder(
				"C# delegates",
				builders.CsharpDelegateTypes);
			LogStringBuilder(
				"C# init call",
				builders.CsharpInitCall);
			LogStringBuilder(
				"C# functions",
				builders.CsharpFunctions);
			LogStringBuilder(
				"C# MonoBehaviours",
				builders.CsharpMonoBehaviours);
			LogStringBuilder(
				"C# MonoBehaviour Delegates",
				builders.CsharpMonoBehaviourDelegates);
			LogStringBuilder(
				"C# MonoBehaviour Imports",
				builders.CsharpMonoBehaviourImports);
			LogStringBuilder(
				"C# MonoBehaviour GetDelegate Calls",
				builders.CsharpMonoBehaviourGetDelegateCalls);
			LogStringBuilder(
				"C++ function pointers",
				builders.CppFunctionPointers);
			LogStringBuilder(
				"C++ type declarations",
				builders.CppTypeDeclarations);
			LogStringBuilder(
				"C++ type definitions",
				builders.CppTypeDefinitions);
			LogStringBuilder(
				"C++ method definitions",
				builders.CppMethodDefinitions);
			LogStringBuilder(
				"C++ init params",
				builders.CppInitParams);
			LogStringBuilder(
				"C++ init body",
				builders.CppInitBody);
			LogStringBuilder(
				"C++ MonoBehaviour messages",
				builders.CppMonoBehaviourMessages);
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
			StringBuilders builders)
		{
			RemoveTrailingChars(builders.CsharpInitParams);
			RemoveTrailingChars(builders.CsharpDelegateTypes);
			RemoveTrailingChars(builders.CsharpInitCall);
			RemoveTrailingChars(builders.CsharpFunctions);
			RemoveTrailingChars(builders.CsharpMonoBehaviours);
			RemoveTrailingChars(builders.CsharpMonoBehaviourDelegates);
			RemoveTrailingChars(builders.CsharpMonoBehaviourImports);
			RemoveTrailingChars(builders.CsharpMonoBehaviourGetDelegateCalls);
			RemoveTrailingChars(builders.CppFunctionPointers);
			RemoveTrailingChars(builders.CppTypeDeclarations);
			RemoveTrailingChars(builders.CppMethodDefinitions);
			RemoveTrailingChars(builders.CppTypeDefinitions);
			RemoveTrailingChars(builders.CppInitParams);
			RemoveTrailingChars(builders.CppInitBody);
			RemoveTrailingChars(builders.CppMonoBehaviourMessages);
		}
		
		// Remove trailing chars (e.g. commas) for last elements
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
		
		static void InjectBuilders(
			StringBuilders builders)
		{
			// Inject into source files
			string csharpContents = File.ReadAllText(CsharpPath);
			string cppHeaderContents = File.ReadAllText(CppHeaderPath);
			string cppSourceContents = File.ReadAllText(CppSourcePath);
			csharpContents = InjectIntoString(
				csharpContents,
				"/*BEGIN INIT PARAMS*/\n",
				"\n\t\t\t/*END INIT PARAMS*/",
				builders.CsharpInitParams.ToString());
			csharpContents = InjectIntoString(
				csharpContents,
				"/*BEGIN DELEGATE TYPES*/\n",
				"\n\t\t/*END DELEGATE TYPES*/",
				builders.CsharpDelegateTypes.ToString());
			csharpContents = InjectIntoString(
				csharpContents,
				"/*BEGIN INIT CALL*/\n",
				"\n\t\t\t\t/*END INIT CALL*/",
				builders.CsharpInitCall.ToString());
			csharpContents = InjectIntoString(
				csharpContents,
				"/*BEGIN FUNCTIONS*/\n",
				"\n\t\t/*END FUNCTIONS*/",
				builders.CsharpFunctions.ToString());
			csharpContents = InjectIntoString(
				csharpContents,
				"/*BEGIN MONOBEHAVIOURS*/\n",
				"\n/*END MONOBEHAVIOURS*/",
				builders.CsharpMonoBehaviours.ToString());
			csharpContents = InjectIntoString(
				csharpContents,
				"/*BEGIN MONOBEHAVIOUR DELEGATES*/\n",
				"\n\t\t/*END MONOBEHAVIOUR DELEGATES*/",
				builders.CsharpMonoBehaviourDelegates.ToString());
			csharpContents = InjectIntoString(
				csharpContents,
				"/*BEGIN MONOBEHAVIOUR IMPORTS*/\n",
				"\n\t\t/*END MONOBEHAVIOUR IMPORTS*/",
				builders.CsharpMonoBehaviourImports.ToString());
			csharpContents = InjectIntoString(
				csharpContents,
				"/*BEGIN MONOBEHAVIOUR GETDELEGATE CALLS*/\n",
				"\n\t\t\t/*END MONOBEHAVIOUR GETDELEGATE CALLS*/",
				builders.CsharpMonoBehaviourGetDelegateCalls.ToString());
			cppSourceContents = InjectIntoString(
				cppSourceContents,
				"/*BEGIN FUNCTION POINTERS*/\n",
				"\n\t/*END FUNCTION POINTERS*/",
				builders.CppFunctionPointers.ToString());
			cppHeaderContents = InjectIntoString(
				cppHeaderContents,
				"/*BEGIN TYPE DECLARATIONS*/\n",
				"\n/*END TYPE DECLARATIONS*/",
				builders.CppTypeDeclarations.ToString());
			cppHeaderContents = InjectIntoString(
				cppHeaderContents,
				"/*BEGIN TYPE DEFINITIONS*/\n",
				"\n/*END TYPE DEFINITIONS*/",
				builders.CppTypeDefinitions.ToString());
			cppSourceContents = InjectIntoString(
				cppSourceContents,
				"/*BEGIN METHOD DEFINITIONS*/\n",
				"\n/*END METHOD DEFINITIONS*/",
				builders.CppMethodDefinitions.ToString());
			cppSourceContents = InjectIntoString(
				cppSourceContents,
				"/*BEGIN INIT PARAMS*/\n",
				"\n\t/*END INIT PARAMS*/",
				builders.CppInitParams.ToString());
			cppSourceContents = InjectIntoString(
				cppSourceContents,
				"/*BEGIN INIT BODY*/\n",
				"\n\t/*END INIT BODY*/",
				builders.CppInitBody.ToString());
			cppSourceContents = InjectIntoString(
				cppSourceContents,
				"/*BEGIN MONOBEHAVIOUR MESSAGES*/\n",
				"\n/*END MONOBEHAVIOUR MESSAGES*/",
				builders.CppMonoBehaviourMessages.ToString());
			
			File.WriteAllText(CsharpPath, csharpContents);
			File.WriteAllText(CppHeaderPath, cppHeaderContents);
			File.WriteAllText(CppSourcePath, cppSourceContents);
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
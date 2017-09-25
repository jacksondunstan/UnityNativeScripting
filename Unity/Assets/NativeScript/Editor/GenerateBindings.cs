using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Reflection;
using System.Text;

using UnityEditor;
using UnityEngine;

namespace NativeScript
{
	/// <summary>
	/// Code generator that reads a JSON file and outputs C# and C++ code
	/// bindings so C++ can call managed functions and MonoBehaviour "messages"
	/// like Update() can call their C++ counterparts.
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
			public string[] ParamTypes;
			public string[] Exceptions;
		}
		
		[Serializable]
		class JsonGenericParams
		{
			public string[] Types;
		}
		
		[Serializable]
		class JsonMethod
		{
			public string Name;
			public string[] ParamTypes;
			public JsonGenericParams[] GenericParams;
			public bool IsReadOnly;
			public string[] Exceptions;
		}
		
		[Serializable]
		class JsonProperty
		{
			public string Name;
			public bool GetIsReadOnly = true;
			public bool SetIsReadOnly;
			public string[] GetExceptions;
			public string[] SetExceptions;
		}
		
		[Serializable]
		class JsonType
		{
			public string Name;
			public JsonConstructor[] Constructors;
			public JsonMethod[] Methods;
			public JsonProperty[] Properties;
			public string[] Fields;
			public JsonGenericParams[] GenericParams;
			public int MaxSimultaneous;
		}
		
		[Serializable]
		class JsonMonoBehaviour
		{
			public string Name;
			public string[] Messages;
		}
		
		[Serializable]
		class JsonDocument
		{
			public string[] Assemblies;
			public JsonType[] Types;
			public JsonMonoBehaviour[] MonoBehaviours;
		}
		
		const int InitialStringBuilderCapacity = 1024 * 10;
		
		class StringBuilders
		{
			public StringBuilder CsharpInitParams =
				new StringBuilder(InitialStringBuilderCapacity);
			public StringBuilder CsharpDelegateTypes =
				new StringBuilder(InitialStringBuilderCapacity);
			public StringBuilder CsharpStructStoreInitCalls =
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
			public StringBuilder CppRefCountsStateAndFunctions =
				new StringBuilder(InitialStringBuilderCapacity);
			public StringBuilder CppRefCountsInit =
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
			public TypeKind Kind;
		}
		
		enum TypeKind
		{
			// No type (e.g. a global function)
			None,
			
			// An instance of any class
			Class,
			
			// A struct that must be managed. This includes types like
			// RaycastHit which have class fields (Transform) and types with no
			// C++ equivalent like decimal.
			ManagedStruct,
			
			// A struct that can be copied between C#/C++. These are types like
			// Vector3 with only non-class fields and a C++ equivalent can be
			// generated.
			FullStruct,
			
			// Any enum
			Enum,
			
			// Any primitive (e.g. int) except pointers
			Primitive,
			
			// A pointer to any type, either X*, IntPtr, or UIntPtr
			Pointer,
			
			// The decimal type
			Decimal
		}
		
		// Compares by field declaration order
		// This uses MetadataToken, which isn't guaranteed to match field
		// declaration order. It just happens to on Mono and .NET.
		class FieldOrderComparer : IComparer
		{
			int IComparer.Compare(object x, object y)
			{
				FieldInfo xField = (FieldInfo)x;
				FieldInfo yField = (FieldInfo)y;
				return xField.MetadataToken < yField.MetadataToken
					? -1
					: xField.MetadataToken > yField.MetadataToken
						? 1
						: 0;
			}
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
			new MessageInfo("OnAnimatorIK", typeof(int)),
			new MessageInfo("OnAnimatorMove"),
			new MessageInfo("OnApplicationFocus", typeof(bool)),
			new MessageInfo("OnApplicationPause", typeof(bool)),
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
		
		static readonly string DotNetDllsDirPath = new FileInfo(
				new Uri(typeof(string).Assembly.CodeBase).LocalPath
			).DirectoryName;
		static readonly string UnityDllsDirPath = new FileInfo(
				new Uri(typeof(GameObject).Assembly.CodeBase).LocalPath
			).DirectoryName;
		static readonly string AssetsDirPath = Application.dataPath;
		static readonly string ProjectDirPath =
			new DirectoryInfo(AssetsDirPath)
				.Parent
				.FullName;
		static readonly string CppDirPath =
			Path.Combine(
				Path.Combine(
					ProjectDirPath,
					"CppSource"),
				"NativeScript");
		static readonly string CsharpPath = Path.Combine(
			AssetsDirPath,
			Path.Combine(
				"NativeScript",
				"Bindings.cs"));
		static readonly string CppHeaderPath = Path.Combine(
			CppDirPath,
			"Bindings.h");
		static readonly string CppSourcePath = Path.Combine(
			CppDirPath,
			"Bindings.cpp");
		
		static readonly FieldOrderComparer DefaultFieldOrderComparer
			= new FieldOrderComparer();
		
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
				DoPostCompileWork(true);
			}
			else
			{
				JsonDocument doc = LoadJson();
				Assembly[] assemblies = GetAssemblies(doc.Assemblies);
				
				// Determine whether we need to generate stubs
				// We can skip this step if we've already generated all the
				// required MonoBehaviour classes and their messages
				bool needStubs = false;
				foreach (JsonMonoBehaviour monoBehaviour in doc.MonoBehaviours)
				{
					// Check if the MonoBehaviour type is already generated
					Type type = TryGetType(
						monoBehaviour.Name,
						assemblies);
					if (type == null)
					{
						needStubs = true;
						break;
					}
					
					// Check if all the messages are already generated
					foreach (string message in monoBehaviour.Messages)
					{
						MethodInfo methodInfo = type.GetMethod(message);
						if (methodInfo == null)
						{
							needStubs = true;
							goto determinedNeedStubs;
						}
					}
				}
				determinedNeedStubs:;
				
				if (needStubs)
				{
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
				else
				{
					DoPostCompileWork(true);
				}
			}
		}
		
		static void AppendStubMonoBehaviours(
			JsonMonoBehaviour[] monoBehaviours,
			string timestamp,
			StringBuilder output)
		{
			if (monoBehaviours != null)
			{
				foreach (JsonMonoBehaviour jsonMonoBehaviour in monoBehaviours)
				{
					// Split namespace from name
					string fullName = jsonMonoBehaviour.Name;
					string monoBehaviourName;
					string monoBehaviourNamespace;
					int index = fullName.LastIndexOf('.');
					if (index >= 0)
					{
						monoBehaviourNamespace = fullName.Substring(
							0,
							index);
						monoBehaviourName = fullName.Substring(
							index + 1);
					}
					else
					{
						monoBehaviourName = fullName;
						monoBehaviourNamespace = string.Empty;
					}
					
					int indent = AppendNamespaceBeginning(
						monoBehaviourNamespace,
						output);
					AppendIndent(indent, output);
					output.Append("public class ");
					output.Append(monoBehaviourName);
					output.Append(" : UnityEngine.MonoBehaviour\n");
					AppendIndent(indent, output);
					output.Append("{\n");
					AppendIndent(indent + 1, output);
					output.Append("// Stub version. GenerateBindings is still in progress. ");
					output.Append(timestamp);
					output.Append('\n');
					AppendIndent(indent, output);
					output.Append("}\n");
					AppendNamespaceEnding(indent, output);
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
				DoPostCompileWork(false);
			}
		}
		
		static void DoPostCompileWork(bool canRefreshAssetDb)
		{
			bool dryRun = EditorPrefs.GetBool(DryRunPref);
			EditorPrefs.DeleteKey(DryRunPref);
			
			JsonDocument doc = LoadJson();
			Assembly[] assemblies = GetAssemblies(doc.Assemblies);
			StringBuilders builders = new StringBuilders();
			
			// Generate types
			foreach (JsonType jsonType in doc.Types)
			{
				AppendType(
					jsonType,
					assemblies,
					builders);
			}
			
			// Generate MonoBehaviours
			if (doc.MonoBehaviours != null)
			{
				foreach (JsonMonoBehaviour monoBehaviour in doc.MonoBehaviours)
				{
					AppendMonoBehaviour(
						monoBehaviour,
						assemblies,
						builders);
				}
			}
			
			// Generate exception setters
			AppendExceptions(
				doc,
				assemblies,
				builders);
			
			RemoveTrailingChars(builders);
			
			if (dryRun)
			{
				LogStringBuilders(builders);
			}
			else
			{
				InjectBuilders(builders);
				if (canRefreshAssetDb)
				{
					AssetDatabase.Refresh();
					Debug.Log("Done generating bindings.");
				}
				else
				{
					Debug.LogWarning(
						"Can't auto-refresh due to a bug in Unity. " +
						"Please manually refresh assets with " +
						"Assets -> Refresh to finish generating bindings");
				}
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
		
		static Assembly[] GetAssemblies(string[] assemblyNames)
		{
			const int numDefaultAssemblies = 7;
			int numAssemblies;
			Assembly[] assemblies;
			if (assemblyNames == null)
			{
				numAssemblies = numDefaultAssemblies;
				assemblies = new Assembly[numAssemblies];
			}
			else
			{
				numAssemblies = numDefaultAssemblies + assemblyNames.Length;
				assemblies = new Assembly[numAssemblies];
				
				for (int i = 0; i < assemblyNames.Length; ++i)
				{
					string path = assemblyNames[i]
						.Replace("UNITY_PROJECT", ProjectDirPath)
						.Replace("UNITY_ASSETS", AssetsDirPath)
						.Replace("DOTNET_DLLS", DotNetDllsDirPath)
						.Replace("UNITY_DLLS", UnityDllsDirPath);
					Assembly assembly = Assembly.LoadFrom(path);
					assemblies[numDefaultAssemblies + i] = assembly;
				}
			}
			assemblies[0] = typeof(string).Assembly; // .NET: mscorlib
			assemblies[1] = typeof(Uri).Assembly; // .NET: System
			assemblies[2] = typeof(Action).Assembly; // .NET: System.Core
			assemblies[3] = typeof(Vector3).Assembly; // UnityEngine
			assemblies[4] = typeof(Bindings).Assembly; // Runtime scripts
			assemblies[5] = typeof(GenerateBindings).Assembly; // Editor scripts
			assemblies[6] = typeof(EditorPrefs).Assembly; // UnityEditor
			return assemblies;
		}
		
		static Type[] GetTypes(
			string[] typeNames,
			Assembly[] assemblies)
		{
			if (typeNames == null)
			{
				return new Type[0];
			}
			Type[] types = new Type[typeNames.Length];
			for (int i = 0; i < typeNames.Length; ++i)
			{
				types[i] = GetType(typeNames[i], assemblies);
			}
			return types;
		}
		
		static Type GetType(
			string typeName,
			Assembly[] assemblies)
		{
			Type type = TryGetType(
				typeName,
				assemblies);
			if (type != null)
			{
				return type;
			}
			
			// Not finding a type is a fatal error
			StringBuilder errorBuilder = new StringBuilder(1024);
			errorBuilder.Append("Couldn't find type \"");
			errorBuilder.Append(typeName);
			errorBuilder.Append('"');
			throw new Exception(errorBuilder.ToString());
		}
		
		static Type TryGetType(
			string typeName,
			Assembly[] assemblies)
		{
			// Search all assemblies for the type
			foreach (Assembly assembly in assemblies)
			{
				Type type = assembly.GetType(typeName);
				if (type != null)
				{
					return type;
				}
			}
			return null;
		}
		
		static TypeKind GetTypeKind(Type type)
		{
			if (type.IsPointer)
			{
				return TypeKind.Pointer;
			}
			
			if (type.IsEnum)
			{
				return TypeKind.Enum;
			}
			
			if (type.IsPrimitive)
			{
				return TypeKind.Primitive;
			}
			
			if (!type.IsValueType)
			{
				return TypeKind.Class;
			}
			
			// Decimal (currently) can't be represented on the C++ side, so
			// don't count it as a full struct
			if (type != typeof(decimal) && IsFullValueType(type))
			{
				return TypeKind.FullStruct;
			}
			
			return TypeKind.ManagedStruct;
		}
		
		static ParameterInfo[] GetConstructorParameters(
			Type type,
			string[] paramTypeNames)
		{
			foreach (ConstructorInfo ctor in type.GetConstructors())
			{
				System.Reflection.ParameterInfo[] reflectionParams
					= ctor.GetParameters();
				if (CheckParametersMatch(
					paramTypeNames,
					reflectionParams))
				{
					return ConvertParameters(reflectionParams);
				}
			}
			
			// Throw an exception so the user knows what to fix in the JSON
			StringBuilder errorBuilder = new StringBuilder(1024);
			errorBuilder.Append("Constructor \"");
			AppendCsharpTypeName(type, errorBuilder);
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
		
		static MethodInfo GetMethod(
			Type type,
			MethodInfo[] methods,
			string methodName,
			string[] paramTypeNames)
		{
			foreach (MethodInfo method in methods)
			{
				// Name must match
				if (method.Name != methodName)
				{
					continue;
				}
				
				// All parameters must match
				if (CheckParametersMatch(
					paramTypeNames,
					method.GetParameters()))
				{
					return method;
				}
			}
			
			// Throw an exception so the user knows what to fix in the JSON
			StringBuilder errorBuilder = new StringBuilder(1024);
			errorBuilder.Append("Method \"");
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
		
		static bool CheckParametersMatch(
			string[] paramTypeNames,
			System.Reflection.ParameterInfo[] reflectionParams)
		{
			// Length must match
			if (reflectionParams.Length != paramTypeNames.Length)
			{
				return false;
			}
			
			// All params must match
			for (int i = 0; i < reflectionParams.Length; ++i)
			{
				Type type = DereferenceParameterType(
					reflectionParams[i]);
				string typeName = paramTypeNames[i];
				if (!CheckTypeNameMatches(typeName, type))
				{
					return false;
				}
			}
			
			return true;
		}
		
		static bool CheckTypeNameMatches(
			string typeName,
			Type type)
		{
			// No namespace. Only name must match.
			if (string.IsNullOrEmpty(type.Namespace))
			{
				if (type.Name != typeName)
				{
					return false;
				}
			}
			// Must be: Namespace.Name
			else
			{
				// Length must be the same as (namespace + '.' + name)
				if (
					typeName.Length !=
					type.Namespace.Length
						+ 1
						+ type.Name.Length)
				{
					return false;
				}
				
				// Must start with namespace
				if (!typeName.StartsWith(type.Namespace))
				{
					return false;
				}
				
				// Namespace must be followed by '.'
				if (typeName[type.Namespace.Length] != '.')
				{
					return false;
				}
				
				// Must end with name
				if (!typeName.EndsWith(type.Name))
				{
					return false;
				}
			}
			
			return true;
		}
		
		static void AppendParameterTypeNames(
			ParameterInfo[] parameters,
			StringBuilder output)
		{
			for (int i = 0, len = parameters.Length; i < len; ++i)
			{
				Type type = parameters[i].DereferencedParameterType;
				AppendNamespace(type.Namespace, string.Empty, output);
				output.Append(type.Name);
				if (i != len - 1)
				{
					output.Append('_');
				}
			}
		}
		
		static void AppendTypeNames(
			Type[] typeParams,
			StringBuilder output)
		{
			if (typeParams != null)
			{
				for (int i = 0, len = typeParams.Length; i < len; ++i)
				{
					Type curType = typeParams[i];
					AppendNamespace(
						curType.Namespace,
						string.Empty,
						output);
					output.Append(curType.Name);
					if (i != len - 1)
					{
						output.Append('_');
					}
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
				info.DereferencedParameterType = DereferenceParameterType(
					reflectionInfo);
				info.Kind = GetTypeKind(
					info.DereferencedParameterType);
				parameters[i] = info;
			}
			return parameters;
		}
		
		static Type DereferenceParameterType(
			System.Reflection.ParameterInfo info)
		{
			Type paramType = info.ParameterType;
			return info.IsOut
				? paramType.GetElementType()
				: paramType.IsByRef
					? paramType.GetElementType()
					: paramType;
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
				info.Kind = GetTypeKind(
					info.DereferencedParameterType);
				parameters[i] = info;
			}
			return parameters;
		}
		
		static bool IsStatic(Type type)
		{
			return type.IsAbstract && type.IsSealed;
		}
		
		static bool IsManagedValueType(Type type)
		{
			return type.IsValueType && !IsFullValueType(type);
		}
		
		static bool IsFullValueType(Type type)
		{
			if (!type.IsValueType)
			{
				return false;
			}
			if (type.IsPrimitive || type.IsEnum)
			{
				return true;
			}
			const BindingFlags bindingFlags =
				BindingFlags.Instance
				| BindingFlags.NonPublic
				| BindingFlags.Public;
			foreach (FieldInfo field in type.GetFields(bindingFlags))
			{
				if (!field.IsStatic
					&& !IsFullValueType(field.FieldType))
				{
					return false;
				}
			}
			return true;
		}
		
		static void AppendWithoutGenericTypeCountSuffix(
			string typeName,
			StringBuilder output)
		{
			// Names are like "List`1" or "Dictionary`2"
			// Remove the backtick (`) and everything after it
			int backtickIndex = typeName.IndexOf('`');
			if (backtickIndex < 0)
			{
				output.Append(typeName);
			}
			else
			{
				output.Append(typeName, 0, backtickIndex);
			}
		}
		
		static void AppendType(
			JsonType jsonType,
			Assembly[] assemblies,
			StringBuilders builders)
		{
			Type type = GetType(jsonType.Name, assemblies);
			if (type.IsEnum)
			{
				AppendEnum(
					type,
					assemblies,
					builders);
			}
			else
			{
				Type[] genericArgTypes = type.GetGenericArguments();
				if (jsonType.GenericParams != null)
				{
					// Template declaration for the type
					if (!IsStatic(type))
					{
						int indent = AppendNamespaceBeginning(
							type.Namespace,
							builders.CppTypeDeclarations);
						AppendIndent(
							indent,
							builders.CppTypeDeclarations);
						AppendCppTemplateTypenames(
							genericArgTypes.Length,
							builders.CppTypeDeclarations);
						builders.CppTypeDeclarations.Append("struct ");
						AppendWithoutGenericTypeCountSuffix(
							type.Name,
							builders.CppTypeDeclarations);
						builders.CppTypeDeclarations.Append(";");
						builders.CppTypeDeclarations.Append('\n');
						AppendNamespaceEnding(
							indent,
							builders.CppTypeDeclarations);
						builders.CppTypeDeclarations.Append('\n');
					}
					
					foreach (JsonGenericParams jsonGenericParams
						in jsonType.GenericParams)
					{
						Type[] typeParams = GetTypes(
							jsonGenericParams.Types,
							assemblies);
						type = type.MakeGenericType(typeParams);
						AppendType(
							jsonType,
							genericArgTypes,
							type,
							typeParams,
							assemblies,
							builders);
					}
				}
				else
				{
					AppendType(
						jsonType,
						genericArgTypes,
						type,
						null,
						assemblies,
						builders);
				}
			}
		}
		
		static void AppendType(
			JsonType jsonType,
			Type[] genericArgTypes,
			Type type,
			Type[] typeParams,
			Assembly[] assemblies,
			StringBuilders builders)
		{
			// Build type name starting with a lowercase letter
			builders.TempStrBuilder.Length = 0;
			AppendWithoutGenericTypeCountSuffix(
				type.Name,
				builders.TempStrBuilder);
			builders.TempStrBuilder[0] = char.ToLower(
				builders.TempStrBuilder[0]);
			string typeNameLower = builders.TempStrBuilder.ToString();
			
			bool isStatic = IsStatic(type);
			TypeKind typeKind = GetTypeKind(type);
			if (!isStatic && typeKind == TypeKind.ManagedStruct)
			{
				// C# StructStore Init call
				builders.CsharpStructStoreInitCalls.Append(
					"\t\t\tNativeScript.Bindings.StructStore<");
				AppendCsharpTypeName(
					type,
					builders.CsharpStructStoreInitCalls);
				builders.CsharpStructStoreInitCalls.Append(
					">.Init(");
				if (jsonType.MaxSimultaneous > 0)
				{
					builders.CsharpStructStoreInitCalls.Append(
						jsonType.MaxSimultaneous);
				}
				else
				{
					builders.CsharpStructStoreInitCalls.Append(
						"maxManagedObjects");
				}
				builders.CsharpStructStoreInitCalls.Append(
					");\n");
				
				// Build function name suffix
				builders.TempStrBuilder.Length = 0;
				AppendReleaseFunctionNameSuffix(
					type,
					typeParams,
					builders.TempStrBuilder);
				string funcNameSuffix = builders.TempStrBuilder.ToString();
				
				// Build function name
				builders.TempStrBuilder.Length = 0;
				builders.TempStrBuilder.Append("Release");
				AppendReleaseFunctionNameSuffix(
					type,
					typeParams,
					builders.TempStrBuilder);
				string funcName = builders.TempStrBuilder.ToString();
				
				// Build lowercase function name
				builders.TempStrBuilder[0] = char.ToLower(
					builders.TempStrBuilder[0]);
				string funcNameLower = builders.TempStrBuilder.ToString();
				
				// Ref counts array length name
				builders.TempStrBuilder.Length = 0;
				builders.TempStrBuilder.Append("RefCountsLen");
				builders.TempStrBuilder.Append(funcNameSuffix);
				string refCountsArrayLengthName = builders.TempStrBuilder.ToString();
				
				// Ref counts array length name (lowercase)
				builders.TempStrBuilder[0] = char.ToLower(
					builders.TempStrBuilder[0]);
				string refCountsArrayLengthNameLower = builders.TempStrBuilder.ToString();
				
				// Build ReleaseX parameters
				ParameterInfo paramInfo = new ParameterInfo();
				paramInfo.Name = "handle";
				paramInfo.ParameterType = typeof(int);
				paramInfo.IsOut = false;
				paramInfo.IsRef = false;
				paramInfo.DereferencedParameterType = typeof(int);
				paramInfo.Kind = TypeKind.Primitive;
				ParameterInfo[] parameters = new[] { paramInfo };
				
				// ReleaseX C# delegate type
				AppendCsharpDelegateType(
					funcName,
					true,
					type,
					typeKind,
					typeof(void),
					parameters,
					builders.CsharpDelegateTypes);
				
				// ReleaseX C# function
				AppendCsharpFunctionBeginning(
					type,
					funcName,
					true,
					typeKind,
					typeof(void),
					null,
					parameters,
					builders.CsharpFunctions);
				builders.CsharpFunctions.Append(
					"if (handle != 0)\n\t\t\t{\n");
				builders.CsharpFunctions.Append(
					"\t\t\t\tNativeScript.Bindings.StructStore<");
				AppendCsharpTypeName(
					type,
					builders.CsharpFunctions);
				builders.CsharpFunctions.Append(
					">.Remove(handle);\n\t\t\t}");
				AppendCsharpFunctionEnd(
					typeof(void),
					new Type[0],
					builders.CsharpFunctions);
				
				// C++ function pointer definition
				AppendCppFunctionPointerDefinition(
					funcName,
					true,
					null,
					TypeKind.None,
					parameters,
					typeof(void),
					builders.CppFunctionPointers);
				
				// C++ init param for ReleaseX
				AppendCppInitParam(
					funcNameLower,
					true,
					null,
					TypeKind.None,
					parameters,
					typeof(void),
					builders.CppInitParams);
				
				// C++ init body for ReleaseX
				AppendCppInitBody(
					funcName,
					funcNameLower,
					builders.CppInitBody);
				
				// C# init param for ReleaseX
				AppendCsharpInitParam(
					funcNameLower,
					builders.CsharpInitParams);
				
				// C# init call arg for ReleaseX
				AppendCsharpInitCallArg(
					funcName,
					builders.CsharpInitCall);
				
				// C++ init param for handle array length
				builders.CppInitParams.Append("\tint32_t ");
				builders.CppInitParams.Append(refCountsArrayLengthNameLower);
				builders.CppInitParams.Append(",\n");
				
				// C++ init body for handle array length
				AppendCppInitBody(
					refCountsArrayLengthName,
					refCountsArrayLengthNameLower,
					builders.CppInitBody);
				builders.CppInitBody.Append("\tPlugin::RefCounts");
				builders.CppInitBody.Append(funcNameSuffix);
				builders.CppInitBody.Append(" = new int32_t[");
				builders.CppInitBody.Append(refCountsArrayLengthNameLower);
				builders.CppInitBody.Append("]();\n");
				
				// C# init param for handle array length
				builders.CsharpInitParams.Append("\t\t\tint ");
				builders.CsharpInitParams.Append(funcName);
				builders.CsharpInitParams.Append(",\n");
				
				// C# init call arg for handle array length
				builders.CsharpInitCall.Append(
					"\t\t\t\t");
				if (jsonType.MaxSimultaneous > 0)
				{
					builders.CsharpInitCall.Append(
						jsonType.MaxSimultaneous);
				}
				else
				{
					builders.CsharpInitCall.Append(
						"maxManagedObjects");
				}
				builders.CsharpInitCall.Append(",\n");
				
				// C++ ref count state and functions
				builders.CppRefCountsStateAndFunctions.Append("\tint32_t RefCountsLen");
				builders.CppRefCountsStateAndFunctions.Append(funcNameSuffix);
				builders.CppRefCountsStateAndFunctions.Append(";\n\tint32_t* RefCounts");
				builders.CppRefCountsStateAndFunctions.Append(funcNameSuffix);
				builders.CppRefCountsStateAndFunctions.Append(";\n\t\n\tvoid ReferenceManaged");
				builders.CppRefCountsStateAndFunctions.Append(funcNameSuffix);
				builders.CppRefCountsStateAndFunctions.Append("(int32_t handle)\n");
				builders.CppRefCountsStateAndFunctions.Append("\t{\n");
				builders.CppRefCountsStateAndFunctions.Append("\t\tassert(handle >= 0 && handle < RefCountsLen");
				builders.CppRefCountsStateAndFunctions.Append(funcNameSuffix);
				builders.CppRefCountsStateAndFunctions.Append(");\n");
				builders.CppRefCountsStateAndFunctions.Append("\t\tif (handle != 0)\n");
				builders.CppRefCountsStateAndFunctions.Append("\t\t{\n");
				builders.CppRefCountsStateAndFunctions.Append("\t\t\tRefCounts");
				builders.CppRefCountsStateAndFunctions.Append(funcNameSuffix);
				builders.CppRefCountsStateAndFunctions.Append("[handle]++;\n");
				builders.CppRefCountsStateAndFunctions.Append("\t\t}\n");
				builders.CppRefCountsStateAndFunctions.Append("\t}\n");
				builders.CppRefCountsStateAndFunctions.Append("\t\n");
				builders.CppRefCountsStateAndFunctions.Append("\tvoid DereferenceManaged");
				builders.CppRefCountsStateAndFunctions.Append(funcNameSuffix);
				builders.CppRefCountsStateAndFunctions.Append("(int32_t handle)\n");
				builders.CppRefCountsStateAndFunctions.Append("\t{\n");
				builders.CppRefCountsStateAndFunctions.Append("\t\tassert(handle >= 0 && handle < RefCountsLen");
				builders.CppRefCountsStateAndFunctions.Append(funcNameSuffix);
				builders.CppRefCountsStateAndFunctions.Append(");\n");
				builders.CppRefCountsStateAndFunctions.Append("\t\tif (handle != 0)\n");
				builders.CppRefCountsStateAndFunctions.Append("\t\t{\n");
				builders.CppRefCountsStateAndFunctions.Append("\t\t\tint32_t numRemain = --RefCounts");
				builders.CppRefCountsStateAndFunctions.Append(funcNameSuffix);
				builders.CppRefCountsStateAndFunctions.Append("[handle];\n");
				builders.CppRefCountsStateAndFunctions.Append("\t\t\tif (numRemain == 0)\n");
				builders.CppRefCountsStateAndFunctions.Append("\t\t\t{\n");
				builders.CppRefCountsStateAndFunctions.Append("\t\t\t\tRelease");
				builders.CppRefCountsStateAndFunctions.Append(funcNameSuffix);
				builders.CppRefCountsStateAndFunctions.Append("(handle);\n");
				builders.CppRefCountsStateAndFunctions.Append("\t\t\t}\n");
				builders.CppRefCountsStateAndFunctions.Append("\t\t}\n");
				builders.CppRefCountsStateAndFunctions.Append("\t}\n\t\n");
			}
			
			// C++ type declaration
			int indent = AppendCppTypeDeclaration(
				type.Namespace,
				type.Name,
				isStatic,
				typeParams,
				builders.CppTypeDeclarations);
			
			// C++ type definition (beginning)
			AppendCppTypeDefinitionBegin(
				type,
				typeParams,
				type.BaseType,
				isStatic,
				indent,
				builders.CppTypeDefinitions);
			
			// C++ method definition
			int cppMethodDefinitionsIndent = AppendCppMethodDefinitionBegin(
				type,
				typeKind,
				typeParams,
				type.BaseType,
				isStatic,
				indent,
				builders.CppMethodDefinitions);
			
			// Constructors
			if (typeKind == TypeKind.FullStruct)
			{
				AppendFullValueTypeDefaultConstructor(
					type,
					indent,
					builders);
			}
			if (jsonType.Constructors != null)
			{
				foreach (JsonConstructor jsonCtor in jsonType.Constructors)
				{
					AppendConstructor(
						jsonCtor,
						type,
						isStatic,
						typeKind,
						assemblies,
						typeParams,
						genericArgTypes,
						typeNameLower,
						indent,
						builders);
				}
			}
			
			// Properties
			if (jsonType.Properties != null)
			{
				foreach (JsonProperty jsonProperty in jsonType.Properties)
				{
					AppendProperty(
						jsonProperty,
						type,
						isStatic,
						typeKind,
						typeParams,
						genericArgTypes,
						indent,
						assemblies,
						builders);
				}
			}
			
			// Fields
			if (typeKind == TypeKind.FullStruct)
			{
				AppendFullValueTypeFields(
					type,
					indent + 1,
					builders);
			}
			else
			{
				if (jsonType.Fields != null)
				{
					foreach (string jsonFieldName in jsonType.Fields)
					{
						AppendField(
							jsonFieldName,
							type,
							isStatic,
							typeKind,
							typeParams,
							genericArgTypes,
							indent,
							builders
						);
					}
				}
			}
			
			// Methods
			if (jsonType.Methods != null)
			{
				MethodInfo[] methods = type.GetMethods();
				foreach (JsonMethod jsonMethod in jsonType.Methods)
				{
					AppendMethod(
						jsonMethod,
						assemblies,
						type,
						isStatic,
						typeKind,
						methods,
						typeParams,
						typeNameLower,
						genericArgTypes,
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
		
		static void AppendReleaseFunctionNameSuffix(
			Type type,
			Type[] typeParams,
			StringBuilder output)
		{
			AppendNamespace(
				type.Namespace,
				string.Empty,
				output);
			AppendWithoutGenericTypeCountSuffix(
				type.Name,
				output);
			if (typeParams != null)
			{
				for (int i = 0, len = typeParams.Length; i < len; ++i)
				{
					Type typeParam = typeParams[i];
					AppendNamespace(
						typeParam.Namespace,
						string.Empty,
						output);
					AppendWithoutGenericTypeCountSuffix(
						typeParam.Name,
						output);
					if (i != len - 1)
					{
						output.Append('_');
					}
				}
			}
		}
		
		static void AppendEnum(
			Type type,
			Assembly[] assemblies,
			StringBuilders builders)
		{
			// C++ type declaration (actually definition)
			int indent = AppendNamespaceBeginning(
				type.Namespace,
				builders.CppTypeDeclarations);
			AppendIndent(
				indent,
				builders.CppTypeDeclarations);
			builders.CppTypeDeclarations.Append("enum struct ");
			builders.CppTypeDeclarations.Append(type.Name);
			builders.CppTypeDeclarations.Append(" : ");
			AppendCppTypeName(
				Enum.GetUnderlyingType(type),
				builders.CppTypeDeclarations);
			builders.CppTypeDeclarations.Append('\n');
			AppendIndent(
				indent,
				builders.CppTypeDeclarations);
			builders.CppTypeDeclarations.Append("{\n");
			FieldInfo[] fields = type.GetFields(
				BindingFlags.Static
				| BindingFlags.Public);
			for (int i = 0; i < fields.Length; ++i)
			{
				FieldInfo field = fields[i];
				AppendIndent(
					indent + 1,
					builders.CppTypeDeclarations);
				builders.CppTypeDeclarations.Append(field.Name);
				builders.CppTypeDeclarations.Append(" = ");
				builders.CppTypeDeclarations.Append(
					field.GetRawConstantValue());
				if (i != fields.Length - 1)
				{
					builders.CppTypeDeclarations.Append(',');
				}
				builders.CppTypeDeclarations.Append('\n');
			}
			AppendIndent(
				indent,
				builders.CppTypeDeclarations);
			builders.CppTypeDeclarations.Append("};\n");
			AppendNamespaceEnding(
				indent,
				builders.CppTypeDeclarations);
			builders.CppTypeDeclarations.Append('\n');
		}
		
		static void AppendHandleStoreTypeName(
			Type type,
			StringBuilder output)
		{
			output.Append("NativeScript.Bindings.");
			if (IsManagedValueType(type))
			{
				output.Append("StructStore<");
				AppendCsharpTypeName(type, output);
				output.Append('>');
			}
			else
			{
				output.Append("ObjectStore");
			}
		}
		
		static void AppendConstructor(
			JsonConstructor jsonCtor,
			Type enclosingType,
			bool enclosingTypeIsStatic,
			TypeKind enclosingTypeKind,
			Assembly[] assemblies,
			Type[] enclosingTypeParams,
			Type[] genericArgTypes,
			string typeNameLower,
			int indent,
			StringBuilders builders)
		{
			// Get the constructor's parameters
			ParameterInfo[] parameters;
			if (enclosingType.IsValueType
				&& !enclosingType.IsPrimitive
				&& !enclosingType.IsEnum
				&& jsonCtor.ParamTypes.Length == 0)
			{
				// Allow parameterless constructor for structs
				parameters = new ParameterInfo[0];
			}
			else
			{
				string[] constructorParamTypeNames;
				if (enclosingType.IsGenericType)
				{
					constructorParamTypeNames = OverrideGenericTypeNames(
						jsonCtor.ParamTypes,
						genericArgTypes,
						enclosingTypeParams);
				}
				else
				{
					constructorParamTypeNames = jsonCtor.ParamTypes;
				}
				parameters = GetConstructorParameters(
					enclosingType,
					constructorParamTypeNames);
			}
			
			Type[] exceptionTypes = GetTypes(
				jsonCtor.Exceptions,
				assemblies);
			
			// Build uppercase function name
			builders.TempStrBuilder.Length = 0;
			AppendNamespace(
				enclosingType.Namespace,
				string.Empty,
				builders.TempStrBuilder);
			AppendWithoutGenericTypeCountSuffix(
				enclosingType.Name,
				builders.TempStrBuilder);
			AppendTypeNames(
				enclosingTypeParams,
				builders.TempStrBuilder);
			builders.TempStrBuilder.Append("Constructor");
			AppendParameterTypeNames(
				parameters,
				builders.TempStrBuilder);
			string funcName = builders.TempStrBuilder.ToString();
			
			// Build lowercase function name
			builders.TempStrBuilder[0] = char.ToLower(
				builders.TempStrBuilder[0]);
			string funcNameLower = builders.TempStrBuilder.ToString();
			
			// C# init param declaration
			AppendCsharpInitParam(
				funcNameLower,
				builders.CsharpInitParams);

			// C# delegate type
			Type delegateReturnType;
			if (enclosingTypeKind == TypeKind.FullStruct)
			{
				delegateReturnType = enclosingType;
			}
			else
			{
				delegateReturnType = typeof(int);
			}
			AppendCsharpDelegateType(
				funcName,
				true,
				enclosingType,
				enclosingTypeKind,
				delegateReturnType,
				parameters,
				builders.CsharpDelegateTypes);

			// C# init call param
			AppendCsharpInitCallArg(funcName, builders.CsharpInitCall);

			// C# function
			if (enclosingTypeKind == TypeKind.FullStruct)
			{
				AppendCsharpFunctionBeginning(
					enclosingType,
					funcName,
					true,
					enclosingTypeKind,
					enclosingType,
					null,
					parameters,
					builders.CsharpFunctions);
				builders.CsharpFunctions.Append("new ");
				AppendCsharpTypeName(
					enclosingType,
					builders.CsharpFunctions);
				AppendCsharpFunctionCallParameters(
					true,
					parameters,
					builders.CsharpFunctions);
				builders.CsharpFunctions.Append(";");
				AppendCsharpFunctionReturn(
					parameters,
					enclosingType,
					exceptionTypes,
					builders.CsharpFunctions);
			}
			else
			{
				AppendCsharpFunctionBeginning(
					enclosingType,
					funcName,
					true,
					enclosingTypeKind,
					typeof(int),
					null,
					parameters,
					builders.CsharpFunctions);
				AppendHandleStoreTypeName(
					enclosingType,
					builders.CsharpFunctions);
				builders.CsharpFunctions.Append(
					".Store(new ");
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
					exceptionTypes,
					builders.CsharpFunctions);
			}
			
			// C++ function pointer
			AppendCppFunctionPointerDefinition(
				funcName,
				true,
				enclosingType,
				enclosingTypeKind,
				parameters,
				enclosingType,
				builders.CppFunctionPointers);
			
			// C++ type declaration
			AppendIndent(
				indent + 1,
				builders.CppTypeDefinitions);
			AppendCppMethodDeclaration(
				enclosingType.Name,
				enclosingTypeIsStatic,
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
				enclosingTypeParams,
				null,
				parameters,
				indent,
				builders.CppMethodDefinitions);
			if (enclosingTypeKind != TypeKind.FullStruct)
			{
				AppendIndent(
					indent + 1,
					builders.CppMethodDefinitions);
				builders.CppMethodDefinitions.Append(" : ");
				AppendCppTypeName(
					enclosingType.BaseType,
					builders.CppMethodDefinitions);
				builders.CppMethodDefinitions.Append("(0)\n");
			}
			AppendIndent(
				indent,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("{\n");
			AppendCppPluginFunctionCall(
				true,
				enclosingType,
				enclosingTypeKind,
				enclosingTypeParams,
				enclosingType,
				funcName,
				parameters,
				indent + 1,
				builders.CppMethodDefinitions);
			if (enclosingTypeKind == TypeKind.FullStruct)
			{
				AppendIndent(
					indent + 1,
					builders.CppMethodDefinitions);
				builders.CppMethodDefinitions.Append(
					"*this = returnValue;\n");
			}
			else
			{
				AppendIndent(
					indent + 1,
					builders.CppMethodDefinitions);
				builders.CppMethodDefinitions.Append(
					"Handle = returnValue;\n");
				AppendIndent(
					indent + 1,
					builders.CppMethodDefinitions);
				builders.CppMethodDefinitions.Append(
					"if (returnValue)\n");
				AppendIndent(
					indent + 1,
					builders.CppMethodDefinitions);
				builders.CppMethodDefinitions.Append(
					"{\n");
				AppendIndent(
					indent + 2,
					builders.CppMethodDefinitions);
				AppendReferenceManagedHandleFunctionCall(
					enclosingType,
					enclosingTypeKind,
					enclosingTypeParams,
					"returnValue",
					builders.CppMethodDefinitions);
				builders.CppMethodDefinitions.Append(";\n");
				AppendIndent(
					indent + 1,
					builders.CppMethodDefinitions);
				builders.CppMethodDefinitions.Append(
					"}\n");
			}
			AppendIndent(
				indent,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("}\n");
			AppendIndent(
				indent,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("\n");

			// C++ init params
			AppendCppInitParam(
				funcNameLower,
				true,
				enclosingType,
				enclosingTypeKind,
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
			JsonProperty jsonProperty,
			Type enclosingType,
			bool enclosingTypeIsStatic,
			TypeKind enclosingTypeKind,
			Type[] typeParams,
			Type[] typeGenericArgumentTypes,
			int indent,
			Assembly[] assemblies,
			StringBuilders builders)
		{
			PropertyInfo property = enclosingType.GetProperty(
				jsonProperty.Name);
			Type propertyType = OverrideGenericType(
				property.PropertyType,
				typeGenericArgumentTypes,
				typeParams);
			Type[] getExceptionTypes = GetTypes(
				jsonProperty.GetExceptions,
				assemblies);
			Type[] setExceptionTypes = GetTypes(
				jsonProperty.SetExceptions,
				assemblies);
			MethodInfo getMethod = property.GetGetMethod();
			if (getMethod != null && getMethod.IsPublic)
			{
				ParameterInfo[] parameters = ConvertParameters(
					getMethod.GetParameters());
				OverrideGenericParameterTypes(
					parameters,
					typeGenericArgumentTypes,
					typeParams);
				AppendGetter(
					property.Name,
					"Property",
					parameters,
					enclosingTypeIsStatic,
					enclosingTypeKind,
					getMethod.IsStatic,
					jsonProperty.GetIsReadOnly,
					enclosingType,
					typeParams,
					propertyType,
					indent,
					getExceptionTypes,
					builders);
			}
			MethodInfo setMethod = property.GetSetMethod();
			if (setMethod != null && setMethod.IsPublic)
			{
				ParameterInfo[] parameters = ConvertParameters(
					setMethod.GetParameters());
				OverrideGenericParameterTypes(
					parameters,
					typeGenericArgumentTypes,
					typeParams);
				AppendSetter(
					property.Name,
					"Property",
					parameters,
					enclosingTypeIsStatic,
					enclosingTypeKind,
					setMethod.IsStatic,
					jsonProperty.SetIsReadOnly,
					enclosingType,
					typeParams,
					propertyType,
					indent,
					setExceptionTypes,
					builders);
			}
		}
		
		static void AppendFullValueTypeDefaultConstructor(
			Type enclosingType,
			int indent,
			StringBuilders builders)
		{
			AppendIndent(
				indent + 1,
				builders.CppTypeDefinitions);
			AppendWithoutGenericTypeCountSuffix(
				enclosingType.Name,
				builders.CppTypeDefinitions);
			builders.CppTypeDefinitions.Append("();\n");
			
			AppendIndent(
				indent,
				builders.CppMethodDefinitions);
			AppendWithoutGenericTypeCountSuffix(
				enclosingType.Name,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("::");
			AppendWithoutGenericTypeCountSuffix(
				enclosingType.Name,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("()\n");
			AppendIndent(
				indent,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("{\n");
			AppendIndent(
				indent,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("}\n");
			AppendIndent(
				indent,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append('\n');
		}
		
		static void AppendFullValueTypeFields(
			Type enclosingType,
			int indent,
			StringBuilders builders)
		{
			FieldInfo[] fields = enclosingType.GetFields(
				BindingFlags.Instance
				| BindingFlags.Public
				| BindingFlags.NonPublic);
			Array.Sort(fields, DefaultFieldOrderComparer);
			foreach (FieldInfo field in fields)
			{
				AppendIndent(
					indent,
					builders.CppTypeDefinitions);
				AppendCppTypeName(
					field.FieldType,
					builders.CppTypeDefinitions);
				builders.CppTypeDefinitions.Append(' ');
				builders.CppTypeDefinitions.Append(field.Name);
				builders.CppTypeDefinitions.Append(";\n");
			}
		}
		
		static void AppendField(
			string jsonFieldName,
			Type enclosingType,
			bool enclosingTypeIsStatic,
			TypeKind enclosingTypeKind,
			Type[] typeTypeParams,
			Type[] typeGenericArgumentTypes,
			int indent,
			StringBuilders builders
		)
		{
			FieldInfo field = enclosingType.GetField(jsonFieldName);
			Type fieldType = OverrideGenericType(
				field.FieldType,
				typeGenericArgumentTypes,
				typeTypeParams);
			Type[] exceptionTypes = new Type[0];
			AppendGetter(
				field.Name,
				"Field",
				new ParameterInfo[0],
				enclosingTypeIsStatic,
				enclosingTypeKind,
				field.IsStatic,
				true,
				enclosingType,
				typeTypeParams,
				fieldType,
				indent,
				exceptionTypes,
				builders);
			ParameterInfo setParam = new ParameterInfo();
			setParam.Name = "value";
			setParam.ParameterType = fieldType;
			setParam.IsOut = false;
			setParam.IsRef = false;
			setParam.DereferencedParameterType = setParam.ParameterType;
			setParam.Kind = GetTypeKind(
				setParam.DereferencedParameterType);
			ParameterInfo[] parameters = new []{ setParam };
			AppendSetter(
				field.Name,
				"Field",
				parameters,
				enclosingTypeIsStatic,
				enclosingTypeKind,
				field.IsStatic,
				false,
				enclosingType,
				typeTypeParams,
				fieldType,
				indent,
				exceptionTypes,
				builders);
		}
		
		static void AppendMethod(
			JsonMethod jsonMethod,
			Assembly[] assemblies,
			Type enclosingType,
			bool enclosingTypeIsStatic,
			TypeKind enclosingTypeKind,
			MethodInfo[] methods,
			Type[] typeTypeParams,
			string typeNameLower,
			Type[] genericArgTypes,
			int indent,
			StringBuilders builders)
		{
			// Get the method
			MethodInfo method;
			if (enclosingType.IsGenericType)
			{
				string[] overriddenParamTypeNames = OverrideGenericTypeNames(
					jsonMethod.ParamTypes,
					genericArgTypes,
					typeTypeParams);
				method = GetMethod(
					enclosingType,
					methods,
					jsonMethod.Name,
					overriddenParamTypeNames);
			}
			else
			{
				method = GetMethod(
					enclosingType,
					methods,
					jsonMethod.Name,
					jsonMethod.ParamTypes);
			}
			
			Type[] exceptionTypes = GetTypes(
				jsonMethod.Exceptions,
				assemblies);
			
			if (jsonMethod.GenericParams != null)
			{
				// Generate for each set of generic types
				foreach (JsonGenericParams jsonGenericParams
					in jsonMethod.GenericParams)
				{
					Type[] methodTypeParams = GetTypes(
						jsonGenericParams.Types,
						assemblies);
					method = method.MakeGenericMethod(methodTypeParams);
					ParameterInfo[] parameters = ConvertParameters(
						method.GetParameters());
					AppendMethod(
						enclosingType,
						assemblies,
						typeNameLower,
						method.Name,
						enclosingTypeIsStatic,
						enclosingTypeKind,
						method.IsStatic,
						jsonMethod.IsReadOnly,
						method.ReturnType,
						typeTypeParams,
						methodTypeParams,
						parameters,
						indent,
						exceptionTypes,
						builders);
				}
			}
			else
			{
				ParameterInfo[] parameters = ConvertParameters(
					method.GetParameters());
				AppendMethod(
					enclosingType,
					assemblies,
					typeNameLower,
					method.Name,
					enclosingTypeIsStatic,
					enclosingTypeKind,
					method.IsStatic,
					jsonMethod.IsReadOnly,
					method.ReturnType,
					typeTypeParams,
					null,
					parameters,
					indent,
					exceptionTypes,
					builders);
			}
		}
		
		static Type OverrideGenericType(
			Type genericType,
			Type[] genericArgumentTypes,
			Type[] overrideTypes)
		{
			if (genericType.IsGenericParameter)
			{
				for (int i = 0, len = genericArgumentTypes.Length; i < len; ++i)
				{
					if (genericType.Equals(genericArgumentTypes[i]))
					{
						return overrideTypes[i];
					}
				}
			}
			return genericType;
		}
		
		static void OverrideGenericParameterTypes(
			ParameterInfo[] parameters,
			Type[] typeGenericArgumentTypes,
			Type[] typeParams)
		{
			for (int i = 0; i < parameters.Length; ++i)
			{
				ParameterInfo info = parameters[i];
				info.ParameterType = OverrideGenericType(
					info.ParameterType,
					typeGenericArgumentTypes,
					typeParams);
			}
		}
		
		static string[] OverrideGenericTypeNames(
			string[] typeNames,
			Type[] genericArgTypes,
			Type[] typeParams)
		{
			int numParams = typeNames.Length;
			string[] overriddenParamTypeNames = new string[numParams];
			for (int i = 0; i < numParams; ++i)
			{
				string typeName = typeNames[i];
				for (int j = 0; j < genericArgTypes.Length; ++j)
				{
					if (CheckTypeNameMatches(
						typeName,
						genericArgTypes[j]))
					{
						typeName = typeParams[i].FullName;
						break;
					}
				}
				overriddenParamTypeNames[i] = typeName;
			}
			return overriddenParamTypeNames;
		}
		
		static void AppendMethod(
			Type enclosingType,
			Assembly[] assemblies,
			string typeNameLower,
			string methodName,
			bool enclosingTypeIsStatic,
			TypeKind enclosingTypeKind,
			bool methodIsStatic,
			bool isReadOnly,
			Type returnType,
			Type[] enclosingTypeParams,
			Type[] methodTypeParams,
			ParameterInfo[] parameters,
			int indent,
			Type[] exceptionTypes,
			StringBuilders builders)
		{
			// Build uppercase function name
			builders.TempStrBuilder.Length = 0;
			AppendNamespace(
				enclosingType.Namespace,
				string.Empty,
				builders.TempStrBuilder);
			AppendWithoutGenericTypeCountSuffix(
				enclosingType.Name,
				builders.TempStrBuilder);
			AppendTypeNames(
				enclosingTypeParams,
				builders.TempStrBuilder);
			builders.TempStrBuilder.Append("Method");
			builders.TempStrBuilder.Append(methodName);
			AppendTypeNames(
				methodTypeParams,
				builders.TempStrBuilder);
			AppendParameterTypeNames(
				parameters,
				builders.TempStrBuilder);
			string funcName = builders.TempStrBuilder.ToString();
			
			// Build lowercase function name
			builders.TempStrBuilder[0] = char.ToLower(
				builders.TempStrBuilder[0]);
			string funcNameLower = builders.TempStrBuilder.ToString();
			
			// C# init param declaration
			AppendCsharpInitParam(
				funcNameLower,
				builders.CsharpInitParams);
			
			// C# delegate type
			AppendCsharpDelegateType(
				funcName,
				methodIsStatic,
				enclosingType,
				enclosingTypeKind,
				returnType,
				parameters,
				builders.CsharpDelegateTypes);
			
			// C# init call param
			AppendCsharpInitCallArg(
				funcName,
				builders.CsharpInitCall);
			
			// C# function
			AppendCsharpFunctionBeginning(
				enclosingType,
				funcName,
				methodIsStatic,
				enclosingTypeKind,
				returnType,
				methodTypeParams,
				parameters,
				builders.CsharpFunctions);
			AppendCsharpFunctionCallSubject(
				enclosingType,
				methodIsStatic,
				builders.CsharpFunctions);
			builders.CsharpFunctions.Append(methodName);
			AppendCSharpTypeParameters(
				methodTypeParams,
				builders.CsharpFunctions);
			AppendCsharpFunctionCallParameters(
				methodIsStatic,
				parameters,
				builders.CsharpFunctions);
			builders.CsharpFunctions.Append(';');
			if (!isReadOnly
				&& enclosingTypeKind == TypeKind.ManagedStruct)
			{
				AppendStructStoreReplace(
					enclosingType,
					"thisHandle",
					"thiz",
					builders.CsharpFunctions);
			}
			AppendCsharpFunctionReturn(
				parameters,
				returnType,
				exceptionTypes,
				builders.CsharpFunctions);
			
			// C++ function pointer
			AppendCppFunctionPointerDefinition(
				funcName,
				methodIsStatic,
				enclosingType,
				enclosingTypeKind,
				parameters,
				returnType,
				builders.CppFunctionPointers);
			
			// C++ method declaration
			AppendIndent(
				indent + 1,
				builders.CppTypeDefinitions);
			AppendCppMethodDeclaration(
				methodName,
				enclosingTypeIsStatic,
				methodIsStatic,
				returnType,
				methodTypeParams,
				parameters,
				builders.CppTypeDefinitions);
			
			// C++ method definition
			AppendCppMethodDefinition(
				enclosingType,
				returnType,
				methodName,
				enclosingTypeParams,
				methodTypeParams,
				parameters,
				indent,
				builders.CppMethodDefinitions);
			AppendIndent(
				indent,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("{\n");
			AppendCppPluginFunctionCall(
				methodIsStatic,
				enclosingType,
				enclosingTypeKind,
				enclosingTypeParams,
				returnType,
				funcName,
				parameters,
				indent + 1,
				builders.CppMethodDefinitions);
			AppendCppMethodReturn(
				returnType,
				indent + 1,
				builders.CppMethodDefinitions);
			AppendIndent(
				indent,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("}\n\t\n");
			
			// C++ init params
			AppendCppInitParam(
				funcNameLower,
				methodIsStatic,
				enclosingType,
				enclosingTypeKind,
				parameters,
				returnType,
				builders.CppInitParams);
			
			// C++ init body
			AppendCppInitBody(
				funcName,
				funcNameLower,
				builders.CppInitBody);
		}
		
		static void AppendCSharpTypeParameters(
			Type[] typeParams,
			StringBuilder output
		)
		{
			if (typeParams != null && typeParams.Length > 0)
			{
				output.Append('<');
				for (int i = 0; i < typeParams.Length; ++i)
				{
					Type typeParam = typeParams[i];
					AppendCsharpTypeName(typeParam, output);
					if (i != typeParams.Length - 1)
					{
						output.Append(", ");
					}
				}
				output.Append('>');
			}
		}
		
		static void AppendCppTypeParameters(
			Type[] typeParams,
			StringBuilder output)
		{
			if (typeParams != null && typeParams.Length > 0)
			{
				output.Append('<');
				for (int i = 0; i < typeParams.Length; ++i)
				{
					Type typeParam = typeParams[i];
					AppendCppTypeName(typeParam, output);
					if (i != typeParams.Length - 1)
					{
						output.Append(", ");
					}
				}
				output.Append('>');
			}
		}
		
		static void AppendMonoBehaviour(
			JsonMonoBehaviour jsonMonoBehaviour,
			Assembly[] assemblies,
			StringBuilders builders)
		{
			Type type = GetType(
				jsonMonoBehaviour.Name,
				assemblies);
				
			// C++ Type Declaration
			int cppIndent = AppendCppTypeDeclaration(
				type.Namespace,
				type.Name,
				false,
				null,
				builders.CppTypeDeclarations);
			
			// C++ Type Definition (begin)
			AppendCppTypeDefinitionBegin(
				type,
				null,
				typeof(MonoBehaviour),
				false,
				cppIndent,
				builders.CppTypeDefinitions
			);
			
			// C++ method definition
			int cppMethodDefinitionsIndent = AppendCppMethodDefinitionBegin(
				type,
				TypeKind.Class,
				null,
				typeof(MonoBehaviour),
				false,
				cppIndent,
				builders.CppMethodDefinitions);
			AppendCppMethodDefinitionEnd(
				cppMethodDefinitionsIndent,
				builders.CppMethodDefinitions);
			
			// C# Class extending MonoBehaviour
			int csharpIndent = AppendNamespaceBeginning(
				type.Namespace,
				builders.CsharpMonoBehaviours);
			AppendIndent(csharpIndent, builders.CsharpMonoBehaviours);
			builders.CsharpMonoBehaviours.Append("public class ");
			builders.CsharpMonoBehaviours.Append(type.Name);
			builders.CsharpMonoBehaviours.Append(" : UnityEngine.MonoBehaviour\n");
			AppendIndent(csharpIndent, builders.CsharpMonoBehaviours);
			builders.CsharpMonoBehaviours.Append("{\n");
			for (
				int messageIndex = 0;
				messageIndex < jsonMonoBehaviour.Messages.Length;
				++messageIndex)
			{
				// Find the MessageInfo
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
				
				// Build ParameterInfos
				ParameterInfo[] parameters = ConvertParameters(
					messageInfo.ParameterTypes);
				int numParams = parameters.Length;
				
				// C++ Method Declaration
				AppendIndent(
					cppIndent + 1,
					builders.CppTypeDefinitions);
				AppendCppMethodDeclaration(
					messageInfo.Name,
					false,
					false,
					typeof(void),
					null,
					parameters,
					builders.CppTypeDefinitions);
				
				// C# message function
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
					Type paramType = parameters[i].ParameterType;
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
					ParameterInfo param = parameters[i];
					if (param.Kind == TypeKind.Class
						|| param.Kind == TypeKind.ManagedStruct)
					{
						AppendIndent(
							csharpIndent + 2,
							builders.CsharpMonoBehaviours);
						builders.CsharpMonoBehaviours.Append("int param");
						builders.CsharpMonoBehaviours.Append(i);
						builders.CsharpMonoBehaviours.Append("Handle = ");
						AppendHandleStoreTypeName(
							param.DereferencedParameterType,
							builders.CsharpMonoBehaviours);
						builders.CsharpMonoBehaviours.Append('.');
						if (param.Kind == TypeKind.ManagedStruct)
						{
							builders.CsharpMonoBehaviours.Append("GetHandle");
						}
						else
						{
							builders.CsharpMonoBehaviours.Append("Store");
						}
						builders.CsharpMonoBehaviours.Append('(');
						builders.CsharpMonoBehaviours.Append("param");
						builders.CsharpMonoBehaviours.Append(i);
						builders.CsharpMonoBehaviours.Append(");\n");
					}
				}
				AppendIndent(
					csharpIndent + 2,
					builders.CsharpMonoBehaviours);
				builders.CsharpMonoBehaviours.Append(
					"int thisHandle = NativeScript.Bindings.ObjectStore.GetHandle(this);\n");
				AppendIndent(
					csharpIndent + 2,
					builders.CsharpMonoBehaviours);
				builders.CsharpMonoBehaviours.Append("NativeScript.Bindings.");
				builders.CsharpMonoBehaviours.Append(type.Name);
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
					ParameterInfo param = parameters[i];
					if (param.Kind == TypeKind.Class
						|| param.Kind == TypeKind.ManagedStruct)
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
					csharpIndent + 2,
					builders.CsharpMonoBehaviours);
				builders.CsharpMonoBehaviours.Append("if (NativeScript.Bindings.UnhandledCppException != null)\n");
				AppendIndent(
					csharpIndent + 2,
					builders.CsharpMonoBehaviours);
				builders.CsharpMonoBehaviours.Append("{\n");
				AppendIndent(
					csharpIndent + 3,
					builders.CsharpMonoBehaviours);
				builders.CsharpMonoBehaviours.Append("Exception ex = NativeScript.Bindings.UnhandledCppException;\n");
				AppendIndent(
					csharpIndent + 3,
					builders.CsharpMonoBehaviours);
				builders.CsharpMonoBehaviours.Append("NativeScript.Bindings.UnhandledCppException = null;\n");
				AppendIndent(
					csharpIndent + 3,
					builders.CsharpMonoBehaviours);
				builders.CsharpMonoBehaviours.Append("throw ex;\n");
				AppendIndent(
					csharpIndent + 2,
					builders.CsharpMonoBehaviours);
				builders.CsharpMonoBehaviours.Append("}\n");
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
				AppendCsharpDelegate(
					false,
					type.Name,
					messageInfo.Name,
					parameters,
					builders.CsharpMonoBehaviourDelegates);
				
				// C# Import
				AppendCsharpImport(
					type.Name,
					messageInfo.Name,
					parameters,
					builders.CsharpMonoBehaviourImports);
				
				// C# GetDelegate Call
				AppendCsharpGetDelegateCall(
					type.Name,
					messageInfo.Name,
					builders.CsharpMonoBehaviourGetDelegateCalls);
				
				// C++ Message
				builders.CppMonoBehaviourMessages.Append("DLLEXPORT void ");
				builders.CppMonoBehaviourMessages.Append(type.Name);
				builders.CppMonoBehaviourMessages.Append(messageInfo.Name);
				builders.CppMonoBehaviourMessages.Append("(int32_t thisHandle");
				if (numParams > 0)
				{
					builders.CppMonoBehaviourMessages.Append(", ");
				}
				for (int i = 0; i < numParams; ++i)
				{
					ParameterInfo param = parameters[i];
					switch (param.Kind)
					{
						case TypeKind.FullStruct:
						case TypeKind.Primitive:
						case TypeKind.Enum:
							AppendCppTypeName(
								param.ParameterType,
								builders.CppMonoBehaviourMessages);
							builders.CppMonoBehaviourMessages.Append(" param");
							builders.CppMonoBehaviourMessages.Append(i);
							break;
						default:
							builders.CppMonoBehaviourMessages.Append("int32_t param");
							builders.CppMonoBehaviourMessages.Append(i);
							builders.CppMonoBehaviourMessages.Append("Handle");
							break;
					}
					if (i != numParams-1)
					{
						builders.CppMonoBehaviourMessages.Append(", ");
					}
				}
				builders.CppMonoBehaviourMessages.Append(")\n{\n\t");
				AppendCppTypeName(
					type,
					builders.CppMonoBehaviourMessages);
				builders.CppMonoBehaviourMessages.Append(" thiz(thisHandle);\n");
				for (int i = 0; i < numParams; ++i)
				{
					ParameterInfo param = parameters[i];
					if (param.Kind == TypeKind.Class
						|| param.Kind == TypeKind.ManagedStruct)
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
				builders.CppMonoBehaviourMessages.Append("\ttry\n");
				builders.CppMonoBehaviourMessages.Append("\t{\n");
				builders.CppMonoBehaviourMessages.Append("\t\tthiz.");
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
				builders.CppMonoBehaviourMessages.Append(");\n");
				builders.CppMonoBehaviourMessages.Append("\t}\n");
				builders.CppMonoBehaviourMessages.Append("\tcatch (System::Exception ex)\n");
				builders.CppMonoBehaviourMessages.Append("\t{\n");
				builders.CppMonoBehaviourMessages.Append("\t\tPlugin::SetException(ex.Handle);\n");
				builders.CppMonoBehaviourMessages.Append("\t}\n");
				builders.CppMonoBehaviourMessages.Append("\tcatch (...)\n");
				builders.CppMonoBehaviourMessages.Append("\t{\n");
				builders.CppMonoBehaviourMessages.Append("\t\tSystem::Exception ex(System::String(\"Unhandled exception in ");
				AppendCppTypeName(
					type,
					builders.CppMonoBehaviourMessages);
				builders.CppMonoBehaviourMessages.Append("::");
				builders.CppMonoBehaviourMessages.Append(messageInfo.Name);
				builders.CppMonoBehaviourMessages.Append("\"));\n");
				builders.CppMonoBehaviourMessages.Append("\t\tPlugin::SetException(ex.Handle);\n");
				builders.CppMonoBehaviourMessages.Append("\t}\n");
				builders.CppMonoBehaviourMessages.Append("}\n\n\n");
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
		
		static void AppendCsharpDelegate(
			bool isStatic,
			string typeName,
			string funcName,
			ParameterInfo[] parameters,
			StringBuilder output)
		{
			output.Append("\t\tpublic delegate void ");
			output.Append(typeName);
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
			for (int i = 0; i < parameters.Length; ++i)
			{
				ParameterInfo param = parameters[i];
				if (param.Kind == TypeKind.FullStruct)
				{
					AppendCsharpTypeName(
						param.ParameterType,
						output);
					output.Append(" param");
					output.Append(i);
				}
				else
				{
					output.Append("int param");
					output.Append(i);
				}
				if (i != parameters.Length-1)
				{
					output.Append(", ");
				}
			}
			output.Append(");\n");
			output.Append("\t\tpublic static ");
			output.Append(typeName);
			output.Append(funcName);
			output.Append("Delegate ");
			output.Append(typeName);
			output.Append(funcName);
			output.Append(";\n\t\t\n");
		}
		
		static void AppendCsharpGetDelegateCall(
			string typeName,
			string funcName,
			StringBuilder output)
		{
			output.Append("\t\t\t");
			output.Append(typeName);
			output.Append(funcName);
			output.Append(" = GetDelegate<");
			output.Append(typeName);
			output.Append(funcName);
			output.Append("Delegate>(libraryHandle, \"");
			output.Append(typeName);
			output.Append(funcName);
			output.Append("\");\n");
		}
		
		static void AppendCsharpImport(
			string typeName,
			string funcName,
			ParameterInfo[] parameters,
			StringBuilder output
		)
		{
			output.Append("\t\t[DllImport(Constants.PluginName)]\n");
			output.Append("\t\tpublic static extern void ");
			output.Append(typeName);
			output.Append(funcName);
			output.Append("(int thisHandle");
			if (parameters.Length > 0)
			{
				output.Append(", ");
			}
			for (int i = 0; i < parameters.Length; ++i)
			{
				ParameterInfo param = parameters[i];
				if (param.Kind == TypeKind.FullStruct)
				{
					AppendCsharpTypeName(
						param.ParameterType,
						output);
					output.Append(" param");
					output.Append(i);
				}
				else
				{
					output.Append("int param");
					output.Append(i);
				}
				if (i != parameters.Length-1)
				{
					output.Append(", ");
				}
			}
			output.Append(");\n\t\t\n");
		}
		
		static void AppendExceptions(
			JsonDocument doc,
			Assembly[] assemblies,
			StringBuilders builders)
		{
			// Gather all specific types of exceptions
			Dictionary<string, Type> exceptionTypes = new Dictionary<string, Type>();
			if (doc.Types != null)
			{
				foreach (JsonType jsonType in doc.Types)
				{
					if (jsonType.Methods != null)
					{
						foreach (JsonMethod jsonMethod in jsonType.Methods)
						{
							if (jsonMethod.Exceptions != null)
							{
								AddUniqueTypes(
									jsonMethod.Exceptions,
									exceptionTypes,
									assemblies);
							}
						}
					}
					if (jsonType.Constructors != null)
					{
						foreach (JsonConstructor jsonCtor in jsonType.Constructors)
						{
							if (jsonCtor.Exceptions != null)
							{
								AddUniqueTypes(
									jsonCtor.Exceptions,
									exceptionTypes,
									assemblies);
							}
						}
					}
					if (jsonType.Properties != null)
					{
						foreach (JsonProperty jsonProperty in jsonType.Properties)
						{
							if (jsonProperty.GetExceptions != null)
							{
								AddUniqueTypes(
									jsonProperty.GetExceptions,
									exceptionTypes,
									assemblies);
							}
							if (jsonProperty.SetExceptions != null)
							{
								AddUniqueTypes(
									jsonProperty.SetExceptions,
									exceptionTypes,
									assemblies);
							}
						}
					}
				}
			}
			
			foreach (Type exceptionType in exceptionTypes.Values)
			{
				// Build function name
				builders.TempStrBuilder.Length = 0;
				AppendCsharpSetCsharpExceptionFunctionName(
					exceptionType,
					builders.TempStrBuilder);
				string funcName = builders.TempStrBuilder.ToString();
				
				// C++ thrower type
				int throwerIndent = AppendNamespaceBeginning(
					exceptionType.Namespace,
					builders.CppMethodDefinitions);
				AppendIndent(
					throwerIndent,
					builders.CppMethodDefinitions);
				builders.CppMethodDefinitions.Append("struct ");
				builders.CppMethodDefinitions.Append(exceptionType.Name);
				builders.CppMethodDefinitions.Append("Thrower : ");
				AppendCppTypeName(
					exceptionType,
					builders.CppMethodDefinitions);
				builders.CppMethodDefinitions.Append('\n');
				AppendIndent(
					throwerIndent,
					builders.CppMethodDefinitions);
				builders.CppMethodDefinitions.Append("{\n");
				AppendIndent(
					throwerIndent + 1,
					builders.CppMethodDefinitions);
				builders.CppMethodDefinitions.Append(exceptionType.Name);
				builders.CppMethodDefinitions.Append("Thrower(int32_t handle)\n");
				AppendIndent(
					throwerIndent + 2,
					builders.CppMethodDefinitions);
				builders.CppMethodDefinitions.Append(": ");
				AppendCppTypeName(
					exceptionType,
					builders.CppMethodDefinitions);
				builders.CppMethodDefinitions.Append("(handle)\n");
				AppendIndent(
					throwerIndent + 1,
					builders.CppMethodDefinitions);
				builders.CppMethodDefinitions.Append("{\n");
				AppendIndent(
					throwerIndent + 1,
					builders.CppMethodDefinitions);
				builders.CppMethodDefinitions.Append("}\n");
				AppendIndent(
					throwerIndent,
					builders.CppMethodDefinitions);
				builders.CppMethodDefinitions.Append("\n");
				AppendIndent(
					throwerIndent + 1,
					builders.CppMethodDefinitions);
				builders.CppMethodDefinitions.Append("virtual void ThrowReferenceToThis()\n");
				AppendIndent(
					throwerIndent + 1,
					builders.CppMethodDefinitions);
				builders.CppMethodDefinitions.Append("{\n");
				AppendIndent(
					throwerIndent + 2,
					builders.CppMethodDefinitions);
				builders.CppMethodDefinitions.Append("throw *this;\n");
				AppendIndent(
					throwerIndent + 1,
					builders.CppMethodDefinitions);
				builders.CppMethodDefinitions.Append("}\n");
				AppendIndent(
					throwerIndent,
					builders.CppMethodDefinitions);
				builders.CppMethodDefinitions.Append("};\n");
				AppendNamespaceEnding(
					throwerIndent,
					builders.CppMethodDefinitions);
				builders.CppMethodDefinitions.Append('\n');
				
				// C++ function
				builders.CppMethodDefinitions.Append("DLLEXPORT void ");
				builders.CppMethodDefinitions.Append(funcName);
				builders.CppMethodDefinitions.Append("(int32_t handle)\n");
				builders.CppMethodDefinitions.Append("{\n");
				builders.CppMethodDefinitions.Append("\tdelete Plugin::unhandledCsharpException;\n");
				builders.CppMethodDefinitions.Append("\tPlugin::unhandledCsharpException = new ");
				AppendCppTypeName(
					exceptionType,
					builders.CppMethodDefinitions);
				builders.CppMethodDefinitions.Append("Thrower(handle);\n");
				builders.CppMethodDefinitions.Append("}\n\n");
				
				// Build parameters
				ParameterInfo[] parameters = ConvertParameters(
					new Type[]{ typeof(int) });
				
				// C# imports
				AppendCsharpImport(
					string.Empty,
					funcName,
					parameters,
					builders.CsharpMonoBehaviourImports);
				
				// C# delegate
				AppendCsharpDelegate(
					true,
					string.Empty,
					funcName,
					parameters,
					builders.CsharpMonoBehaviourDelegates
				);
				
				// C# GetDelegate call
				AppendCsharpGetDelegateCall(
					string.Empty,
					funcName,
					builders.CsharpMonoBehaviourGetDelegateCalls);
			}
		}
		
		static void AddUniqueTypes(
			string[] typeNames,
			Dictionary<string, Type> types,
			Assembly[] assemblies)
		{
			foreach (string typeName in typeNames)
			{
				if (!types.ContainsKey(typeName))
				{
					Type type = GetType(
						typeName,
						assemblies);
					types.Add(
						typeName,
						type);
				}
			}
		}
		
		static void AppendGetter(
			string fieldName,
			string syntaxType,
			ParameterInfo[] parameters,
			bool enclosingTypeIsStatic,
			TypeKind enclosingTypeKind,
			bool methodIsStatic,
			bool isReadOnly,
			Type enclosingType,
			Type[] enclosingTypeParams,
			Type fieldType,
			int indent,
			Type[] exceptionTypes,
			StringBuilders builders)
		{
			// Build uppercase field name
			builders.TempStrBuilder.Length = 0;
			builders.TempStrBuilder.Append(char.ToUpper(fieldName[0]));
			builders.TempStrBuilder.Append(
				fieldName,
				1,
				fieldName.Length-1);
			string fieldNameUpper = builders.TempStrBuilder.ToString();
			
			// Build uppercase function name
			builders.TempStrBuilder.Length = 0;
			AppendNamespace(
				enclosingType.Namespace,
				string.Empty,
				builders.TempStrBuilder);
			AppendWithoutGenericTypeCountSuffix(
				enclosingType.Name,
				builders.TempStrBuilder);
			AppendTypeNames(
				enclosingTypeParams,
				builders.TempStrBuilder);
			builders.TempStrBuilder.Append(syntaxType);
			builders.TempStrBuilder.Append("Get");
			builders.TempStrBuilder.Append(fieldNameUpper);
			string funcName = builders.TempStrBuilder.ToString();
			
			// Build lowercase function name
			builders.TempStrBuilder[0] = char.ToLower(
				builders.TempStrBuilder[0]);
			string funcNameLower = builders.TempStrBuilder.ToString();
			
			// Build method name
			builders.TempStrBuilder.Length = 0;
			builders.TempStrBuilder.Append("Get");
			builders.TempStrBuilder.Append(fieldNameUpper);
			string methodName = builders.TempStrBuilder.ToString();
			
			// C# init param declaration
			AppendCsharpInitParam(
				funcNameLower,
				builders.CsharpInitParams);

			// C# delegate type
			AppendCsharpDelegateType(
				funcName,
				methodIsStatic,
				enclosingType,
				enclosingTypeKind,
				fieldType,
				parameters,
				builders.CsharpDelegateTypes);

			// C# init call param
			AppendCsharpInitCallArg(
				funcName,
				builders.CsharpInitCall);

			// C# function
			AppendCsharpFunctionBeginning(
				enclosingType,
				funcName,
				methodIsStatic,
				enclosingTypeKind,
				fieldType,
				enclosingTypeParams,
				parameters,
				builders.CsharpFunctions);
			AppendCsharpFunctionCallSubject(
				enclosingType,
				methodIsStatic,
				builders.CsharpFunctions);
			builders.CsharpFunctions.Append(fieldName);
			builders.CsharpFunctions.Append(';');
			if (!isReadOnly
				&& enclosingTypeKind == TypeKind.ManagedStruct)
			{
				AppendStructStoreReplace(
					enclosingType,
					"thisHandle",
					"thiz",
					builders.CsharpFunctions);
			}
			AppendCsharpFunctionReturn(
				parameters,
				fieldType,
				exceptionTypes,
				builders.CsharpFunctions);

			// C++ function pointer
			AppendCppFunctionPointerDefinition(
				funcName,
				methodIsStatic,
				enclosingType,
				enclosingTypeKind,
				parameters,
				fieldType,
				builders.CppFunctionPointers);

			// C++ method declaration
			AppendIndent(indent + 1, builders.CppTypeDefinitions);
			AppendCppMethodDeclaration(
				methodName,
				enclosingTypeIsStatic,
				methodIsStatic,
				fieldType,
				null,
				parameters,
				builders.CppTypeDefinitions);
			
			// C++ method definition
			AppendCppMethodDefinition(
				enclosingType,
				fieldType,
				methodName,
				enclosingTypeParams,
				null,
				parameters,
				indent,
				builders.CppMethodDefinitions);
			AppendIndent(indent, builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("{\n");
			AppendCppPluginFunctionCall(
				methodIsStatic,
				enclosingType,
				enclosingTypeKind,
				enclosingTypeParams,
				fieldType,
				funcName,
				parameters,
				indent + 1,
				builders.CppMethodDefinitions);
			AppendCppMethodReturn(
				fieldType,
				indent + 1,
				builders.CppMethodDefinitions);
			AppendIndent(indent, builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("}\n");
			AppendIndent(indent, builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append('\n');

			// C++ init params
			AppendCppInitParam(
				funcNameLower,
				methodIsStatic,
				enclosingType,
				enclosingTypeKind,
				parameters,
				fieldType,
				builders.CppInitParams);
			
			// C++ init body
			AppendCppInitBody(
				funcName,
				funcNameLower,
				builders.CppInitBody);
		}
		
		static void AppendSetter(
			string fieldName,
			string syntaxType,
			ParameterInfo[] parameters,
			bool enclosingTypeIsStatic,
			TypeKind enclosingTypeKind,
			bool methodIsStatic,
			bool isReadOnly,
			Type enclosingType,
			Type[] enclosingTypeParams,
			Type fieldType,
			int indent,
			Type[] exceptionTypes,
			StringBuilders builders)
		{
			// Build uppercased field name
			builders.TempStrBuilder.Length = 0;
			builders.TempStrBuilder.Append(char.ToUpper(fieldName[0]));
			builders.TempStrBuilder.Append(
				fieldName,
				1,
				fieldName.Length-1);
			string fieldNameUpper = builders.TempStrBuilder.ToString();
			
			// Build uppercase function name
			builders.TempStrBuilder.Length = 0;
			AppendNamespace(
				enclosingType.Namespace,
				string.Empty,
				builders.TempStrBuilder);
			AppendWithoutGenericTypeCountSuffix(
				enclosingType.Name,
				builders.TempStrBuilder);
			AppendTypeNames(
				enclosingTypeParams,
				builders.TempStrBuilder);
			builders.TempStrBuilder.Append(syntaxType);
			builders.TempStrBuilder.Append("Set");
			builders.TempStrBuilder.Append(fieldNameUpper);
			string funcName = builders.TempStrBuilder.ToString();
			
			// Build lowercase function name
			builders.TempStrBuilder[0] = char.ToLower(
				builders.TempStrBuilder[0]);
			string funcNameLower = builders.TempStrBuilder.ToString();
			
			// Build method name
			builders.TempStrBuilder.Length = 0;
			builders.TempStrBuilder.Append("Set");
			builders.TempStrBuilder.Append(fieldNameUpper);
			string methodName = builders.TempStrBuilder.ToString();
			
			// C# init param declaration
			AppendCsharpInitParam(
				funcNameLower,
				builders.CsharpInitParams);
			
			// C# delegate type
			AppendCsharpDelegateType(
				funcName,
				methodIsStatic,
				enclosingType,
				enclosingTypeKind,
				typeof(void),
				parameters,
				builders.CsharpDelegateTypes);
			
			// C# init call param
			AppendCsharpInitCallArg(
				funcName,
				builders.CsharpInitCall);
			
			// C# function
			AppendCsharpFunctionBeginning(
				enclosingType,
				funcName,
				methodIsStatic,
				enclosingTypeKind,
				typeof(void),
				enclosingTypeParams,
				parameters,
				builders.CsharpFunctions);
			AppendCsharpFunctionCallSubject(
				enclosingType,
				methodIsStatic,
				builders.CsharpFunctions);
			builders.CsharpFunctions.Append(fieldName);
			builders.CsharpFunctions.Append(" = ");
			builders.CsharpFunctions.Append("value;");
			if (!isReadOnly
				&& enclosingTypeKind == TypeKind.ManagedStruct)
			{
				AppendStructStoreReplace(
					enclosingType,
					"thisHandle",
					"thiz",
					builders.CsharpFunctions);
			}
			AppendCsharpFunctionReturn(
				parameters,
				typeof(void),
				exceptionTypes,
				builders.CsharpFunctions);
			
			// C++ function pointer
			AppendCppFunctionPointerDefinition(
				funcName,
				methodIsStatic,
				enclosingType,
				enclosingTypeKind,
				parameters,
				typeof(void),
				builders.CppFunctionPointers);
			
			// C++ method declaration
			AppendIndent(indent + 1, builders.CppTypeDefinitions);
			AppendCppMethodDeclaration(
				methodName,
				enclosingTypeIsStatic,
				methodIsStatic,
				typeof(void),
				null,
				parameters,
				builders.CppTypeDefinitions);
			
			// C++ method definition
			AppendCppMethodDefinition(
				enclosingType,
				typeof(void),
				methodName,
				enclosingTypeParams,
				null,
				parameters,
				indent,
				builders.CppMethodDefinitions);
			AppendIndent(indent, builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("{\n");
			AppendCppPluginFunctionCall(
				methodIsStatic,
				enclosingType,
				enclosingTypeKind,
				enclosingTypeParams,
				null,
				funcName,
				parameters,
				indent + 1,
				builders.CppMethodDefinitions);
			AppendIndent(indent, builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("}\n");
			AppendIndent(indent, builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append('\n');
			
			// C++ init params
			AppendCppInitParam(
				funcNameLower,
				methodIsStatic,
				enclosingType,
				enclosingTypeKind,
				parameters,
				typeof(void),
				builders.CppInitParams);
			
			// C++ init body
			AppendCppInitBody(
				funcName,
				funcNameLower,
				builders.CppInitBody);
		}
		
		static int AppendCppTypeDeclaration(
			string typeNamespace,
			string typeName,
			bool isStatic,
			Type[] typeParams,
			StringBuilder output)
		{
			int indent = AppendNamespaceBeginning(
				typeNamespace,
				output);
			AppendIndent(indent, output);
			if (isStatic)
			{
				output.Append("namespace ");
				AppendWithoutGenericTypeCountSuffix(
					typeName,
					output);
				output.Append('\n');
				AppendIndent(indent, output);
				output.Append("{\n");
				AppendIndent(indent, output);
				output.Append('}');
			}
			else
			{
				if (typeParams != null)
				{
					output.Append("template<> ");
				}
				output.Append("struct ");
				AppendWithoutGenericTypeCountSuffix(
					typeName,
					output);
				AppendCppTypeParameters(
					typeParams,
					output);
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
			Type type,
			Type[] typeParams,
			Type baseType,
			bool isStatic,
			int indent,
			StringBuilder output
		)
		{
			AppendNamespaceBeginning(
				type.Namespace,
				output);
			AppendIndent(
				indent,
				output);
			if (isStatic)
			{
				output.Append("namespace ");
				AppendWithoutGenericTypeCountSuffix(
					type.Name,
					output);
			}
			else
			{
				if (typeParams != null)
				{
					output.Append("template<> ");
				}
				output.Append("struct ");
				AppendWithoutGenericTypeCountSuffix(
					type.Name,
					output);
				AppendCppTypeParameters(typeParams, output);
				if (baseType != null
					&& !IsFullValueType(type))
				{
					output.Append(" : ");
					AppendCppTypeName(
						baseType,
						output);
				}
			}
			output.Append('\n');
			AppendIndent(
				indent,
				output);
			output.Append("{\n");
			if (!isStatic && !IsFullValueType(type))
			{
				// Constructor from nullptr_t
				AppendIndent(indent + 1, output);
				AppendWithoutGenericTypeCountSuffix(
					type.Name,
					output);
				AppendCppTypeParameters(
					typeParams,
					output);
				output.Append("(std::nullptr_t n);\n");
				
				// Constructor from handle
				AppendIndent(indent + 1, output);
				AppendWithoutGenericTypeCountSuffix(
					type.Name,
					output);
				AppendCppTypeParameters(
					typeParams,
					output);
				output.Append("(int32_t handle);\n");
				
				// Copy constructor
				AppendIndent(indent + 1, output);
				AppendWithoutGenericTypeCountSuffix(
					type.Name,
					output);
				AppendCppTypeParameters(
					typeParams,
					output);
				output.Append("(const ");
				AppendWithoutGenericTypeCountSuffix(
					type.Name,
					output);
				AppendCppTypeParameters(
					typeParams,
					output);
				output.Append("& other);\n");
				
				// Move constructor
				AppendIndent(indent + 1, output);
				AppendWithoutGenericTypeCountSuffix(
					type.Name,
					output);
				AppendCppTypeParameters(
					typeParams,
					output);
				output.Append('(');
				AppendWithoutGenericTypeCountSuffix(
					type.Name,
					output);
				AppendCppTypeParameters(
					typeParams,
					output);
				output.Append("&& other);\n");
				
				// Destructor
				AppendIndent(indent + 1, output);
				output.Append('~');
				AppendWithoutGenericTypeCountSuffix(
					type.Name,
					output);
				AppendCppTypeParameters(
					typeParams,
					output);
				output.Append("();\n");
				
				// Assignment operator to same type
				AppendIndent(indent + 1, output);
				AppendWithoutGenericTypeCountSuffix(
					type.Name,
					output);
				AppendCppTypeParameters(
					typeParams,
					output);
				output.Append("& operator=(const ");
				AppendWithoutGenericTypeCountSuffix(
					type.Name,
					output);
				AppendCppTypeParameters(
					typeParams,
					output);
				output.Append("& other);\n");
				
				// Assignment operator to nullptr_t
				AppendIndent(indent + 1, output);
				AppendWithoutGenericTypeCountSuffix(
					type.Name,
					output);
				AppendCppTypeParameters(
					typeParams,
					output);
				output.Append("& operator=(std::nullptr_t other);\n");
				
				// Move assignment operator to same type
				AppendIndent(indent + 1, output);
				AppendWithoutGenericTypeCountSuffix(
					type.Name,
					output);
				AppendCppTypeParameters(
					typeParams,
					output);
				output.Append("& operator=(");
				AppendWithoutGenericTypeCountSuffix(
					type.Name,
					output);
				AppendCppTypeParameters(
					typeParams,
					output);
				output.Append("&& other);\n");
				
				// Equality operator with same type
				AppendIndent(indent + 1, output);
				output.Append("bool operator==(const ");
				AppendWithoutGenericTypeCountSuffix(
					type.Name,
					output);
				AppendCppTypeParameters(
					typeParams,
					output);
				output.Append("& other) const;\n");
				
				// Inequality operator with same type
				AppendIndent(indent + 1, output);
				output.Append("bool operator!=(const ");
				AppendWithoutGenericTypeCountSuffix(
					type.Name,
					output);
				AppendCppTypeParameters(
					typeParams,
					output);
				output.Append("& other) const;\n");
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
			Type enclosingType,
			TypeKind enclosingTypeKind,
			Type[] enclosingTypeParams,
			Type baseType,
			bool isStatic,
			int indent,
			StringBuilder output)
		{
			int cppMethodDefinitionsIndent = AppendNamespaceBeginning(
				enclosingType.Namespace,
				output);
			if (!isStatic && (
				enclosingTypeKind == TypeKind.Class
				|| enclosingTypeKind == TypeKind.ManagedStruct))
			{
				if (baseType == null)
				{
					baseType = typeof(object);
				}
				
				// Construct with nullptr_t
				AppendIndent(indent, output);
				AppendWithoutGenericTypeCountSuffix(
					enclosingType.Name,
					output);
				AppendCppTypeParameters(
					enclosingTypeParams,
					output);
				output.Append("::");
				AppendWithoutGenericTypeCountSuffix(
					enclosingType.Name,
					output);
				output.Append("(std::nullptr_t n)\n");
				AppendIndent(indent, output);
				output.Append("\t: ");
				AppendCppTypeName(
					baseType,
					output);
				output.Append("(0)\n");
				AppendIndent(indent, output);
				output.Append("{\n");
				AppendIndent(indent, output);
				output.Append("}\n");
				AppendIndent(indent, output);
				output.Append("\n");
				
				// Construct with handle
				AppendIndent(indent, output);
				AppendWithoutGenericTypeCountSuffix(
					enclosingType.Name,
					output);
				AppendCppTypeParameters(
					enclosingTypeParams,
					output);
				output.Append("::");
				AppendWithoutGenericTypeCountSuffix(
					enclosingType.Name,
					output);
				output.Append("(int32_t handle)\n");
				AppendIndent(indent, output);
				output.Append("\t: ");
				AppendCppTypeName(
					baseType,
					output);
				output.Append("(handle)\n");
				AppendIndent(indent, output);
				output.Append("{\n");
				AppendIndent(indent + 1, output);
				output.Append("if (handle)\n");
				AppendIndent(indent + 1, output);
				output.Append("{\n");
				AppendIndent(indent + 2, output);
				AppendReferenceManagedHandleFunctionCall(
					enclosingType,
					enclosingTypeKind,
					enclosingTypeParams,
					"handle",
					output);
				output.Append(";\n");
				AppendIndent(indent + 1, output);
				output.Append("}\n");
				AppendIndent(indent, output);
				output.Append("}\n");
				AppendIndent(indent, output);
				output.Append("\n");
				
				// Copy constructor
				AppendIndent(indent, output);
				AppendWithoutGenericTypeCountSuffix(
					enclosingType.Name,
					output);
				AppendCppTypeParameters(
					enclosingTypeParams,
					output);
				output.Append("::");
				AppendWithoutGenericTypeCountSuffix(
					enclosingType.Name,
					output);
				output.Append("(const ");
				AppendWithoutGenericTypeCountSuffix(
					enclosingType.Name,
					output);
				AppendCppTypeParameters(
					enclosingTypeParams,
					output);
				output.Append("& other)\n");
				AppendIndent(indent, output);
				output.Append("\t: ");
				AppendCppTypeName(
					baseType,
					output);
				output.Append("(other.Handle)\n");
				AppendIndent(indent, output);
				output.Append("{\n");
				AppendIndent(indent + 1, output);
				output.Append("if (Handle)\n");
				AppendIndent(indent + 1, output);
				output.Append("{\n");
				AppendIndent(indent + 2, output);
				AppendReferenceManagedHandleFunctionCall(
					enclosingType,
					enclosingTypeKind,
					enclosingTypeParams,
					"Handle",
					output);
				output.Append(";\n");
				AppendIndent(indent + 1, output);
				output.Append("}\n");
				AppendIndent(indent, output);
				output.Append("}\n");
				AppendIndent(indent, output);
				output.Append("\n");
				
				// Move constructor
				AppendIndent(indent, output);
				AppendWithoutGenericTypeCountSuffix(
					enclosingType.Name,
					output);
				AppendCppTypeParameters(
					enclosingTypeParams,
					output);
				output.Append("::");
				AppendWithoutGenericTypeCountSuffix(
					enclosingType.Name,
					output);
				output.Append("(");
				AppendWithoutGenericTypeCountSuffix(
					enclosingType.Name,
					output);
				AppendCppTypeParameters(
					enclosingTypeParams,
					output);
				output.Append("&& other)\n");
				AppendIndent(indent, output);
				output.Append("\t: ");
				AppendCppTypeName(
					baseType,
					output);
				output.Append("(other.Handle)\n");
				AppendIndent(indent, output);
				output.Append("{\n");
				AppendIndent(indent + 1, output);
				output.Append("other.Handle = 0;\n");
				AppendIndent(indent, output);
				output.Append("}\n");
				AppendIndent(indent, output);
				output.Append("\n");
				
				// Destructor
				AppendIndent(indent, output);
				AppendWithoutGenericTypeCountSuffix(
					enclosingType.Name,
					output);
				AppendCppTypeParameters(
					enclosingTypeParams,
					output);
				output.Append("::~");
				AppendWithoutGenericTypeCountSuffix(
					enclosingType.Name,
					output);
				AppendCppTypeParameters(
					enclosingTypeParams,
					output);
				output.Append("()\n");
				AppendIndent(indent, output);
				output.Append("{\n");
				AppendIndent(indent + 1, output);
				output.Append("if (Handle)\n");
				AppendIndent(indent + 1, output);
				output.Append("{\n");
				AppendIndent(indent + 2, output);
				AppendDereferenceManagedHandleFunctionCall(
					enclosingType,
					enclosingTypeKind,
					enclosingTypeParams,
					"Handle",
					output);
				output.Append(";\n");
				AppendIndent(indent + 2, output);
				output.Append("Handle = 0;\n");
				AppendIndent(indent + 1, output);
				output.Append("}\n");
				AppendIndent(indent, output);
				output.Append("}\n");
				AppendIndent(indent, output);
				output.Append("\n");
				
				// Assignment operator to same type
				AppendIndent(indent, output);
				AppendWithoutGenericTypeCountSuffix(
					enclosingType.Name,
					output);
				AppendCppTypeParameters(
					enclosingTypeParams,
					output);
				output.Append("& ");
				AppendWithoutGenericTypeCountSuffix(
					enclosingType.Name,
					output);
				AppendCppTypeParameters(
					enclosingTypeParams,
					output);
				output.Append("::operator=(const ");
				AppendWithoutGenericTypeCountSuffix(
					enclosingType.Name,
					output);
				AppendCppTypeParameters(
					enclosingTypeParams,
					output);
				output.Append("& other)\n");
				AppendIndent(indent, output);
				output.Append("{\n");
				AppendSetHandle(
					enclosingType,
					enclosingTypeKind,
					enclosingTypeParams,
					indent + 1,
					"this",
					"other.Handle",
					output);
				AppendIndent(indent, output);
				output.Append("\treturn *this;\n");
				AppendIndent(indent, output);
				output.Append("}\n");
				AppendIndent(indent, output);
				output.Append("\n");
				
				// Assignment operator to nullptr_t
				AppendIndent(indent, output);
				AppendWithoutGenericTypeCountSuffix(
					enclosingType.Name,
					output);
				AppendCppTypeParameters(
					enclosingTypeParams,
					output);
				output.Append("& ");
				AppendWithoutGenericTypeCountSuffix(
					enclosingType.Name,
					output);
				AppendCppTypeParameters(
					enclosingTypeParams,
					output);
				output.Append("::operator=(std::nullptr_t other)\n");
				AppendIndent(indent, output);
				output.Append("{\n");
				AppendIndent(indent, output);
				output.Append("\tif (Handle)\n");
				AppendIndent(indent, output);
				output.Append("\t{\n");
				AppendIndent(indent, output);
				output.Append("\t\t");
				AppendDereferenceManagedHandleFunctionCall(
					enclosingType,
					enclosingTypeKind,
					enclosingTypeParams,
					"Handle",
					output);
				output.Append(";\n");
				AppendIndent(indent, output);
				output.Append("\t\tHandle = 0;\n");
				AppendIndent(indent, output);
				output.Append("\t}\n");
				AppendIndent(indent, output);
				output.Append("\treturn *this;\n");
				AppendIndent(indent, output);
				output.Append("}\n");
				AppendIndent(indent, output);
				output.Append("\n");
				
				// Move assignment operator to same type
				AppendIndent(indent, output);
				AppendWithoutGenericTypeCountSuffix(
					enclosingType.Name,
					output);
				AppendCppTypeParameters(
					enclosingTypeParams,
					output);
				output.Append("& ");
				AppendWithoutGenericTypeCountSuffix(
					enclosingType.Name,
					output);
				AppendCppTypeParameters(
					enclosingTypeParams,
					output);
				output.Append("::operator=(");
				AppendWithoutGenericTypeCountSuffix(
					enclosingType.Name,
					output);
				AppendCppTypeParameters(
					enclosingTypeParams,
					output);
				output.Append("&& other)\n");
				AppendIndent(indent, output);
				output.Append("{\n");
				AppendIndent(indent, output);
				output.Append("\tif (Handle)\n");
				AppendIndent(indent, output);
				output.Append("\t{\n");
				AppendIndent(indent, output);
				output.Append("\t\t");
				AppendDereferenceManagedHandleFunctionCall(
					enclosingType,
					enclosingTypeKind,
					enclosingTypeParams,
					"Handle",
					output);
				output.Append(";\n");
				AppendIndent(indent, output);
				output.Append("\t}\n");
				AppendIndent(indent, output);
				output.Append("\tHandle = other.Handle;\n");
				AppendIndent(indent, output);
				output.Append("\tother.Handle = 0;\n");
				AppendIndent(indent, output);
				output.Append("\treturn *this;\n");
				AppendIndent(indent, output);
				output.Append("}\n");
				AppendIndent(indent, output);
				output.Append('\n');
				
				// Equality operator with same type
				AppendIndent(indent, output);
				output.Append("bool ");
				AppendWithoutGenericTypeCountSuffix(
					enclosingType.Name,
					output);
				AppendCppTypeParameters(
					enclosingTypeParams,
					output);
				output.Append("::operator==(const ");
				AppendWithoutGenericTypeCountSuffix(
					enclosingType.Name,
					output);
				AppendCppTypeParameters(
					enclosingTypeParams,
					output);
				output.Append("& other) const\n");
				AppendIndent(indent, output);
				output.Append("{\n");
				AppendIndent(indent + 1, output);
				output.Append("return Handle == other.Handle;\n");
				AppendIndent(indent, output);
				output.Append("}\n");
				AppendIndent(indent, output);
				output.Append('\n');
				
				// Inequality operator with same type
				AppendIndent(indent, output);
				output.Append("bool ");
				AppendWithoutGenericTypeCountSuffix(
					enclosingType.Name,
					output);
				AppendCppTypeParameters(
					enclosingTypeParams,
					output);
				output.Append("::operator!=(const ");
				AppendWithoutGenericTypeCountSuffix(
					enclosingType.Name,
					output);
				AppendCppTypeParameters(
					enclosingTypeParams,
					output);
				output.Append("& other) const\n");
				AppendIndent(indent, output);
				output.Append("{\n");
				AppendIndent(indent + 1, output);
				output.Append("return Handle != other.Handle;\n");
				AppendIndent(indent, output);
				output.Append("}\n");
				AppendIndent(indent, output);
				output.Append('\n');
			}
			return cppMethodDefinitionsIndent;
		}
		
		static void AppendSetHandle(
			Type enclosingType,
			TypeKind enclosingTypeKind,
			Type[] enclosingTypeParams,
			int indent,
			string thisExpression,
			string otherHandleExpression,
			StringBuilder output)
		{
			string thisHandleExpression = thisExpression + "->Handle";
			AppendIndent(indent, output);
			output.Append("if (");
			output.Append(thisHandleExpression);
			output.Append(" != ");
			output.Append(otherHandleExpression);
			output.Append(")\n");
			AppendIndent(indent, output);
			output.Append("{\n");
			AppendIndent(indent + 1, output);
			output.Append("if (");
			output.Append(thisHandleExpression);
			output.Append(")\n");
			AppendIndent(indent + 1, output);
			output.Append("{\n");
			AppendIndent(indent + 2, output);
			AppendDereferenceManagedHandleFunctionCall(
				enclosingType,
				enclosingTypeKind,
				enclosingTypeParams,
				thisHandleExpression,
				output);
			output.Append(";\n");
			AppendIndent(indent + 1, output);
			output.Append("}\n");
			AppendIndent(indent + 1, output);
			output.Append(thisHandleExpression);
			output.Append(" = ");
			output.Append(otherHandleExpression);
			output.Append(";\n");
			AppendIndent(indent + 1, output);
			output.Append("if (");
			output.Append(thisHandleExpression);
			output.Append(")\n");
			AppendIndent(indent + 1, output);
			output.Append("{\n");
			AppendIndent(indent + 2, output);
			AppendReferenceManagedHandleFunctionCall(
				enclosingType,
				enclosingTypeKind,
				enclosingTypeParams,
				thisHandleExpression,
				output);
			output.Append(";\n");
			AppendIndent(indent + 1, output);
			output.Append("}\n");
			AppendIndent(indent, output);
			output.Append("}\n");
		}
		
		static void AppendReferenceManagedHandleFunctionCall(
			Type enclosingType,
			TypeKind enclosingTypeKind,
			Type[] enclosingTypeParams,
			string handleVariable,
			StringBuilder output)
		{
			if (enclosingTypeKind == TypeKind.ManagedStruct)
			{
				output.Append("Plugin::ReferenceManaged");
				AppendReleaseFunctionNameSuffix(
					enclosingType,
					enclosingTypeParams,
					output);
				output.Append("(Handle)");
			}
			else
			{
				output.Append("Plugin::ReferenceManagedClass(");
				output.Append(handleVariable);
				output.Append(")");
			}
		}
		
		static void AppendDereferenceManagedHandleFunctionCall(
			Type enclosingType,
			TypeKind enclosingTypeKind,
			Type[] enclosingTypeParams,
			string handleVariable,
			StringBuilder output)
		{
			if (enclosingTypeKind == TypeKind.ManagedStruct)
			{
				output.Append("Plugin::DereferenceManaged");
				AppendReleaseFunctionNameSuffix(
					enclosingType,
					enclosingTypeParams,
					output);
				output.Append("(Handle)");
			}
			else
			{
				output.Append("Plugin::DereferenceManagedClass(");
				output.Append(handleVariable);
				output.Append(")");
			}
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
			Type enclosingType,
			TypeKind enclosingTypeKind,
			Type returnType,
			ParameterInfo[] parameters,
			StringBuilder output)
		{
			output.Append("\t\tdelegate ");
			
			// Return type
			if (IsFullValueType(returnType))
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
				if (enclosingTypeKind == TypeKind.FullStruct)
				{
					output.Append("ref ");
					AppendCsharpTypeName(
						enclosingType,
						output);
					output.Append(" thiz");
				}
				else
				{
					output.Append("int thisHandle");
				}
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
			TypeKind enclosingTypeKind,
			Type returnType,
			Type[] typeParams,
			ParameterInfo[] parameters,
			StringBuilder output)
		{
			output.Append("\t\t[MonoPInvokeCallback(typeof(");
			output.Append(funcName);
			output.Append("Delegate))]\n\t\tstatic ");
			
			// Return type
			if (returnType != null)
			{
				if (IsFullValueType(returnType))
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
				if (enclosingTypeKind == TypeKind.FullStruct)
				{
					output.Append("ref ");
					AppendCsharpTypeName(
						enclosingType,
						output);
					output.Append(" thiz");
				}
				else
				{
					output.Append("int thisHandle");
				}
				if (parameters.Length > 0)
				{
					output.Append(", ");
				}
			}
			AppendCsharpParameterDeclaration(
				parameters,
				output);
			output.Append(")\n\t\t{\n\t\t\t");
			
			// Start try/catch block
			output.Append("try\n\t\t\t{\n\t\t\t\t");
			
			// Get "this"
			if (!isStatic
				&& enclosingTypeKind != TypeKind.FullStruct)
			{
				output.Append("var thiz = (");
				AppendCsharpTypeName(
					enclosingType,
					output);
				output.Append(')');
				AppendHandleStoreTypeName(
					enclosingType,
					output);
				output.Append(
					".Get(thisHandle);\n\t\t\t\t");
			}
			
			// Get managed type params from ObjectStore
			foreach (ParameterInfo param in parameters)
			{
				Type paramType = param.DereferencedParameterType;
				if (param.Kind == TypeKind.Class
					|| param.Kind == TypeKind.ManagedStruct)
				{
					output.Append("var ");
					output.Append(param.Name);
					output.Append(" = ");
					if (!paramType.Equals(typeof(object)))
					{
						output.Append('(');
						AppendCsharpTypeName(paramType, output);
						output.Append(')');
					}
					AppendHandleStoreTypeName(paramType, output);
					output.Append(".Get(");
					output.Append(param.Name);
					output.Append("Handle);\n\t\t\t\t");
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
		
		static void AppendStructStoreReplace(
			Type enclosingType,
			string handleVariable,
			string structVariable,
			StringBuilder output)
		{
			output.Append("\n\t\t\t\t");
			AppendHandleStoreTypeName(
				enclosingType,
				output);
			output.Append(".Replace(");
			output.Append(handleVariable);
			output.Append(", ref ");
			output.Append(structVariable);
			output.Append(");");
		}
		
		static void AppendCsharpFunctionReturn(
			ParameterInfo[] parameters,
			Type returnType,
			Type[] exceptionTypes,
			StringBuilder output)
		{
			// Store reference out and ref params and overwrite handles
			foreach (ParameterInfo param in parameters)
			{
				if ((param.Kind == TypeKind.Class
					|| param.Kind == TypeKind.ManagedStruct)
					&& (param.IsOut || param.IsRef))
				{
					output.Append("\n\t\t\t\tint ");
					output.Append(param.Name);
					output.Append("HandleNew = ");
					AppendHandleStoreTypeName(
						param.DereferencedParameterType,
						output);
					output.Append('.');
					if (param.Kind == TypeKind.ManagedStruct)
					{
						output.Append("Store");
					}
					else
					{
						output.Append("GetHandle");
					}
					output.Append('(');
					output.Append(param.Name);
					output.Append(");\n\t\t\t\t");
					output.Append(param.Name);
					output.Append("Handle = ");
					output.Append(param.Name);
					output.Append("HandleNew;");
				}
			}
			
			// Return
			if (!returnType.Equals(typeof(void)))
			{
				output.Append("\n\t\t\t\treturn ");
				if (IsFullValueType(returnType))
				{
					output.Append("returnValue");
				}
				else
				{
					AppendHandleStoreTypeName(
						returnType,
						output);
					output.Append(".GetHandle(returnValue)");
				}
				output.Append(';');
			}
			
			// Returning ends the function
			AppendCsharpFunctionEnd(
				returnType,
				exceptionTypes,
				output);
		}
		
		static void AppendCsharpFunctionEnd(
			Type returnType,
			Type[] exceptionTypes,
			StringBuilder output)
		{
			output.Append('\n');
			output.Append("\t\t\t}\n");
			if (Array.IndexOf(
				exceptionTypes,
				typeof(NullReferenceException)) < 0)
			{
				AppendCsharpCatchException(
					typeof(NullReferenceException),
					returnType,
					output);
			}
			foreach (Type exceptionType in exceptionTypes)
			{
				AppendCsharpCatchException(
					exceptionType,
					returnType,
					output);
			}
			AppendCsharpCatchException(
				typeof(Exception),
				returnType,
				output);
			output.Append("\t\t}\n");
			output.Append("\t\t\n");
		}
		
		static void AppendCsharpCatchException(
			Type exceptionType,
			Type returnType,
			StringBuilder output)
		{
			output.Append("\t\t\tcatch (");
			AppendCsharpTypeName(
				exceptionType,
				output);
			output.Append(" ex)\n");
			output.Append("\t\t\t{\n");
			output.Append("\t\t\t\tNativeScript.Bindings.");
			AppendCsharpSetCsharpExceptionFunctionName(
				exceptionType,
				output);
			output.Append("(NativeScript.Bindings.ObjectStore.Store(ex));\n");
			if (returnType != typeof(void))
			{
				output.Append("\t\t\t\treturn default(");
				if (IsFullValueType(returnType))
				{
					AppendCsharpTypeName(
						returnType,
						output);
				}
				else
				{
					output.Append("int");
				}
				output.Append(");\n");
			}
			output.Append("\t\t\t}\n");
		}
		
		static void AppendCsharpSetCsharpExceptionFunctionName(
			Type exceptionType,
			StringBuilder output
		)
		{
			output.Append("SetCsharpException");
			if (exceptionType != typeof(Exception))
			{
				AppendNamespace(
					exceptionType.Namespace,
					string.Empty,
					output);
				AppendWithoutGenericTypeCountSuffix(
					exceptionType.Name,
					output);
			}
		}
		
		static void AppendCsharpParameterDeclaration(
			ParameterInfo[] parameters,
			StringBuilder output)
		{
			for (int i = 0; i < parameters.Length; ++i)
			{
				ParameterInfo param = parameters[i];
				
				// out or ref qualifiers if necessary
				switch (param.Kind)
				{
					case TypeKind.FullStruct:
						if (param.IsOut)
						{
							output.Append("out ");
						}
						else
						{
							output.Append("ref ");
						}
						break;
					case TypeKind.ManagedStruct:
					case TypeKind.Primitive:
					case TypeKind.Enum:
					case TypeKind.Class:
						if (param.IsOut || param.IsRef)
						{
							output.Append("ref ");
						}
						break;
				}
				
				// Param type- int for handles
				switch (param.Kind)
				{
					case TypeKind.ManagedStruct:
					case TypeKind.Class:
						output.Append("int");
						break;
					default:
						AppendCsharpTypeName(
							param.DereferencedParameterType,
							output);
						break;
				}
				
				// Param name
				output.Append(' ');
				output.Append(param.Name);
				
				// Handle suffix if necessary
				if (param.Kind == TypeKind.Class
					|| param.Kind == TypeKind.ManagedStruct)
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
				
				// Pointer (*) or reference (&) suffix if necessary
				if (param.IsOut || param.IsRef)
				{
					output.Append('*');
				}
				else if (param.Kind == TypeKind.FullStruct)
				{
					output.Append('&');
				}
				
				// Param name
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
				if (parameter.Kind == TypeKind.Class
					|| parameter.Kind == TypeKind.ManagedStruct)
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
			string globalVariableName,
			string paramName,
			StringBuilder output)
		{
			output.Append("\tPlugin::");
			output.Append(globalVariableName);
			output.Append(" = ");
			output.Append(paramName);
			output.Append(";\n");
		}
		
		static void AppendCppMethodDefinition(
			Type enclosingType,
			Type returnType,
			string methodName,
			Type[] typeTypeParams,
			Type[] methodTypeParams,
			ParameterInfo[] parameters,
			int indent,
			StringBuilder output)
		{
			// Indent
			AppendIndent(
				indent,
				output);
			
			// Template
			if (methodTypeParams != null)
			{
				output.Append("template<> ");
			}
			
			// Return type
			if (returnType != null)
			{
				AppendCppTypeName(
					returnType,
					output);
				output.Append(' ');
			}
			
			// Type name
			AppendWithoutGenericTypeCountSuffix(
				enclosingType.Name,
				output);
			AppendCppTypeParameters(
				typeTypeParams,
				output);
			output.Append("::");
			
			// Method name
			AppendWithoutGenericTypeCountSuffix(
				methodName,
				output);
			
			// Template parameters
			AppendCppTypeParameters(
				methodTypeParams,
				output);
			
			// Parameters
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
			Type enclosingType,
			TypeKind enclosingTypeKind,
			Type[] enclosingTypeParams,
			Type returnType,
			string funcName,
			ParameterInfo[] parameters,
			int indent,
			StringBuilder output)
		{
			// Gather handles for out and ref parameters
			foreach (ParameterInfo param in parameters)
			{
				if ((param.Kind == TypeKind.Class
					|| param.Kind == TypeKind.ManagedStruct)
					&& (param.IsOut || param.IsRef))
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
				if (enclosingTypeKind == TypeKind.FullStruct)
				{
					output.Append("this");
				}
				else
				{
					output.Append("Handle");
				}
				if (parameters.Length > 0)
				{
					output.Append(", ");
				}
			}
			for (int i = 0; i < parameters.Length; ++i)
			{
				ParameterInfo param = parameters[i];
				switch (param.Kind)
				{
					case TypeKind.FullStruct:
					case TypeKind.Primitive:
					case TypeKind.Enum:
						output.Append(param.Name);
						break;
					default:
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
						break;
				}
				if (i != parameters.Length - 1)
				{
					output.Append(", ");
				}
			}
			output.Append(");\n");
			
			// Handle uncaught exceptions from the C# side
			AppendIndent(indent, output);
			output.Append("if (Plugin::unhandledCsharpException)\n");
			AppendIndent(indent, output);
			output.Append("{\n");
			AppendIndent(indent + 1, output);
			output.Append("System::Exception* ex = Plugin::unhandledCsharpException;\n");
			AppendIndent(indent + 1, output);
			output.Append("Plugin::unhandledCsharpException = nullptr;\n");
			AppendIndent(indent + 1, output);
			output.Append("ex->ThrowReferenceToThis();\n");
			AppendIndent(indent + 1, output);
			output.Append("delete ex;\n");
			AppendIndent(indent, output);
			output.Append("}\n");
			
			// Set out and ref parameters
			foreach (ParameterInfo param in parameters)
			{
				if ((param.Kind == TypeKind.Class
					|| param.Kind == TypeKind.ManagedStruct)
					&& (param.IsOut || param.IsRef))
				{
					AppendSetHandle(
						enclosingType,
						enclosingTypeKind,
						enclosingTypeParams,
						indent,
						param.Name,
						param.Name + "Handle",
						output);
				}
			}
		}
		
		static void AppendCppInitParam(
			string funcName,
			bool isStatic,
			Type enclosingType,
			TypeKind enclosingTypeKind,
			ParameterInfo[] parameters,
			Type returnType,
			StringBuilder output
		)
		{
			output.Append('\t');
			AppendCppFunctionPointer(
				funcName,
				isStatic,
				enclosingType,
				enclosingTypeKind,
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
			Type enclosingType,
			TypeKind enclosingTypeKind,
			ParameterInfo[] parameters,
			Type returnType,
			StringBuilder output
		)
		{
			output.Append('\t');
			AppendCppFunctionPointer(
				funcName,
				isStatic,
				enclosingType,
				enclosingTypeKind,
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
			Type enclosingType,
			TypeKind enclosingTypeKind,
			ParameterInfo[] parameters,
			Type returnType,
			char separator,
			StringBuilder output)
		{
			// Return type
			if (IsFullValueType(returnType))
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
				switch (enclosingTypeKind)
				{
					case TypeKind.FullStruct:
					case TypeKind.Primitive:
						AppendCppTypeName(
							enclosingType,
							output);
						output.Append("* thiz");
						break;
					default:
						output.Append("int32_t thisHandle");
						break;
				}
				if (parameters.Length > 0)
				{
					output.Append(", ");
				}
			}
			for (int i = 0; i < parameters.Length; ++i)
			{
				ParameterInfo param = parameters[i];
				switch (param.Kind)
				{
					case TypeKind.Primitive:
					case TypeKind.Enum:
						AppendCppTypeName(
							param.DereferencedParameterType,
							output);
						if (param.IsOut || param.IsRef)
						{
							output.Append('*');
						}
						break;
					case TypeKind.FullStruct:
						AppendCppTypeName(
							param.DereferencedParameterType,
							output);
						if (param.IsOut || param.IsRef)
						{
							output.Append('*');
						}
						else
						{
							output.Append('&');
						}
						break;
					case TypeKind.Class:
					case TypeKind.ManagedStruct:
						output.Append("int32_t");
						if (param.IsOut || param.IsRef)
						{
							output.Append('*');
						}
						break;
				}
				output.Append(' ');
				output.Append(param.Name);
				if (param.Kind == TypeKind.Class
					|| param.Kind == TypeKind.ManagedStruct)
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
		
		static void AppendCppTemplateTypenames(
			int numTypeParameters,
			StringBuilder output)
		{
			if (numTypeParameters > 0)
			{
				output.Append("template<");
				for (int i = 0; i < numTypeParameters; ++i)
				{
					output.Append("typename T");
					output.Append(i);
					if (i != numTypeParameters - 1)
					{
						output.Append(", ");
					}
				}
				output.Append("> ");
			}
		}
		
		static void AppendCppMethodDeclaration(
			string methodName,
			bool enclosingTypeIsStatic,
			bool methodIsStatic,
			Type returnType,
			Type[] typeParameters,
			ParameterInfo[] parameters,
			StringBuilder output)
		{
			AppendCppTemplateTypenames(
				typeParameters == null ? 0 : typeParameters.Length,
				output);
			
			if (!enclosingTypeIsStatic && methodIsStatic)
			{
				output.Append("static ");
			}
			
			// Return type
			if (returnType != null)
			{
				AppendCppTypeName(
					returnType,
					output);
				output.Append(' ');
			}
			
			// Method name might be a constructor/type name, so remove suffix
			// just in case
			AppendWithoutGenericTypeCountSuffix(
				methodName,
				output);
			
			// Parameters
			output.Append('(');
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
			else if (type.Equals(typeof(char)))
			{
				output.Append("char");
			}
			else if (type.Equals(typeof(float)))
			{
				output.Append("float");
			}
			else if (type.Equals(typeof(double)))
			{
				output.Append("double");
			}
			else if (type.Equals(typeof(string)))
			{
				output.Append("string");
			}
			else
			{
				output.Append(type.Namespace);
				output.Append('.');
				AppendWithoutGenericTypeCountSuffix(
					type.Name,
					output);
				Type[] genTypes = type.GetGenericArguments();
				AppendCSharpTypeParameters(
					genTypes,
					output);
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
			else if (type.Equals(typeof(char)))
			{
				output.Append("System::Char");
			}
			else if (type.Equals(typeof(float)))
			{
				output.Append("float");
			}
			else if (type.Equals(typeof(double)))
			{
				output.Append("double");
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
				Type[] genTypes = type.GetGenericArguments();
				AppendCppTypeParameters(
					genTypes,
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
			AppendWithoutGenericTypeCountSuffix(
				name,
				output);
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
				"C# StructStore Init calls",
				builders.CsharpStructStoreInitCalls);
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
			RemoveTrailingChars(builders.CsharpStructStoreInitCalls);
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
				"/*BEGIN STRUCTSTORE INIT CALLS*/\n",
				"\n\t\t\t/*END STRUCTSTORE INIT CALLS*/",
				builders.CsharpStructStoreInitCalls.ToString());
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
			cppSourceContents = InjectIntoString(
				cppSourceContents,
				"/*BEGIN REF COUNTS STATE AND FUNCTIONS*/\n",
				"\n\t/*END REF COUNTS STATE AND FUNCTIONS*/",
				builders.CppRefCountsStateAndFunctions.ToString());
			cppSourceContents = InjectIntoString(
				cppSourceContents,
				"/*BEGIN REF COUNTS INIT*/\n",
				"\n\t/*END REF COUNTS INIT*/",
				builders.CppRefCountsInit.ToString());
			
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
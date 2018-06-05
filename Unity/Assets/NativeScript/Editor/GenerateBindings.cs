﻿using System;
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
	/// bindings so the languages can call each other.
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
		#pragma warning disable 649
		
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
			public int MaxSimultaneous;
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
		class JsonPropertyGet
		{
			public bool IsReadOnly = true;
			public string[] ParamTypes;
			public string[] Exceptions;
		}
		
		[Serializable]
		class JsonPropertySet
		{
			public bool IsReadOnly;
			public string[] ParamTypes;
			public string[] Exceptions;
		}
		
		[Serializable]
		class JsonProperty
		{
			public string Name;
			public JsonPropertyGet Get;
			public JsonPropertySet Set;
		}
		
		[Serializable]
		class JsonEvent
		{
			public string Name;
		}
		
		[Serializable]
		class JsonType
		{
			public string Name;
			public JsonConstructor[] Constructors;
			public JsonMethod[] Methods;
			public JsonProperty[] Properties;
			public string[] Fields;
			public JsonEvent[] Events;
			public JsonGenericParams[] GenericParams;
			public int MaxSimultaneous;
			public JsonBaseType[] BaseTypes;
		}
		
		[Serializable]
		class JsonBaseType
		{
			public string BaseName;
			public string DerivedName;
			public string[] GenericTypes;
			public int MaxSimultaneous;
			public JsonConstructor[] Constructors;
			public JsonMethod[] OverrideMethods;
			public JsonProperty[] OverrideProperties;
			public JsonEvent[] OverrideEvents;
		}
		
		[Serializable]
		class JsonArray
		{
			public string Type;
			public int[] Ranks;
		}
		
		[Serializable]
		class JsonDelegate
		{
			public string Type;
			public JsonGenericParams[] GenericParams;
			public int MaxSimultaneous;
		}
		
		[Serializable]
		class JsonDocument
		{
			public int MaxSimultaneousObjects;
			public int DefaultMaxSimultaneous;
			public string[] Assemblies;
			public JsonType[] Types;
			public JsonArray[] Arrays;
			public JsonDelegate[] Delegates;
		}
		
		const int InitialStringBuilderCapacity = 1024 * 100;
		
		class StringBuilders
		{
			public readonly StringBuilder CsharpInitParams =
				new StringBuilder(InitialStringBuilderCapacity);
			public readonly StringBuilder CsharpDelegateTypes =
				new StringBuilder(InitialStringBuilderCapacity);
			public readonly StringBuilder CsharpStoreInitCalls =
				new StringBuilder(InitialStringBuilderCapacity);
			public readonly StringBuilder CsharpInitCall =
				new StringBuilder(InitialStringBuilderCapacity);
			public readonly StringBuilder CsharpBaseTypes =
				new StringBuilder(InitialStringBuilderCapacity);
			public readonly StringBuilder CsharpFunctions =
				new StringBuilder(InitialStringBuilderCapacity);
			public readonly StringBuilder CsharpDelegates =
				new StringBuilder(InitialStringBuilderCapacity);
			public readonly StringBuilder CsharpImports =
				new StringBuilder(InitialStringBuilderCapacity);
			public readonly StringBuilder CsharpGetDelegateCalls =
				new StringBuilder(InitialStringBuilderCapacity);
			public readonly StringBuilder CsharpDestroyFunctionEnumerators =
				new StringBuilder(InitialStringBuilderCapacity);
			public readonly StringBuilder CsharpDestroyQueueCases =
				new StringBuilder(InitialStringBuilderCapacity);
			public readonly StringBuilder CppFunctionPointers =
				new StringBuilder(InitialStringBuilderCapacity);
			public readonly StringBuilder CppTypeDeclarations =
				new StringBuilder(InitialStringBuilderCapacity);
			public readonly StringBuilder CppTemplateDeclarations =
				new StringBuilder(InitialStringBuilderCapacity);
			public readonly StringBuilder CppTemplateSpecializationDeclarations =
				new StringBuilder(InitialStringBuilderCapacity);
			public readonly StringBuilder CppTypeDefinitions =
				new StringBuilder(InitialStringBuilderCapacity);
			public readonly StringBuilder CppMethodDefinitions =
				new StringBuilder(InitialStringBuilderCapacity);
			public readonly StringBuilder CppInitParams =
				new StringBuilder(InitialStringBuilderCapacity);
			public readonly StringBuilder CppInitBody =
				new StringBuilder(InitialStringBuilderCapacity);
			public readonly StringBuilder CppInitBodyFirstBoot =
				new StringBuilder(InitialStringBuilderCapacity);
			public readonly StringBuilder CppGlobalStateAndFunctions =
				new StringBuilder(InitialStringBuilderCapacity);
			public readonly StringBuilder CppUnboxingMethodDeclarations =
				new StringBuilder(InitialStringBuilderCapacity);
			public readonly StringBuilder CppStringDefaultParams =
				new StringBuilder(InitialStringBuilderCapacity);
			public readonly StringBuilder CppMacros =
				new StringBuilder(InitialStringBuilderCapacity);
			public readonly StringBuilder TempStrBuilder =
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
			public bool IsVirtual;
			public bool HasDefault;
			public object DefaultValue;
			public bool IsVarArg;
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
			Pointer
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

		struct TypeName
		{
			public string Name;
			public string Namespace;
			public int NumTypeParams;
		}

		const int DEFAULT_MAX_SIMULTANEOUS = 1000;
		const int DEFAULT_MAX_SIMULTANEOUS_OBJECTS = 1000;

		static readonly Type[] PRIMITIVE_TYPES = {
			typeof(bool),
			typeof(sbyte),
			typeof(byte),
			typeof(short),
			typeof(ushort),
			typeof(int),
			typeof(uint),
			typeof(long),
			typeof(ulong),
			typeof(char),
			typeof(float),
			typeof(double),
		};
		
		const string PostCompileWorkPref = "NativeScriptGenerateBindingsPostCompileWork";
		
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
					Path.Combine(
						ProjectDirPath,
						"Assets"),
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
		#pragma warning restore 649
		
		public static void Generate()
		{
			EditorPrefs.DeleteKey(PostCompileWorkPref);
			JsonDocument doc = LoadJson();
			Assembly[] assemblies = GetAssemblies(doc.Assemblies);
			
			// Determine whether we need to generate stubs
			// We can skip this step if we've already generated all the
			// required base types
			bool needStubs = false;
			if (doc.Types != null)
			{
				foreach (JsonType jsonType in doc.Types)
				{
					if (jsonType.BaseTypes != null)
					{
						foreach (JsonBaseType jsonBaseType in jsonType.BaseTypes)
						{
							// Check if the type is already generated
							Type type = TryGetType(
								jsonBaseType.BaseName,
								assemblies);
							if (type == null)
							{
								needStubs = true;
								goto determinedNeedStubs;
							}
						}
					}
				}
			}
			determinedNeedStubs:;
			
			if (needStubs)
			{
				// We'll need to be able to get these via reflection later
				StringBuilders builders = new StringBuilders();
				string timestamp = DateTime.Now.ToLongTimeString();
				AppendStubs(
					doc.Types,
					assemblies,
					timestamp,
					builders);
				InjectBuilders(builders);
				
				// Compile and continue after scripts are refreshed
				Debug.Log("Waiting for compile...");
				EditorPrefs.SetBool(PostCompileWorkPref, true);
				AssetDatabase.Refresh();
			}
			else
			{
				DoPostCompileWork(true);
			}
		}
		
		static void AppendStubs(
			JsonType[] jsonTypes,
			Assembly[] assemblies,
			string timestamp,
			StringBuilders builders)
		{
			// Base types
			foreach (JsonType jsonType in jsonTypes)
			{
				if (jsonType.BaseTypes != null)
				{
					foreach (JsonBaseType jsonBaseType in jsonType.BaseTypes)
					{
						string typeFullName = jsonType.Name;
						TypeName typeName = SplitJsonTypeName(typeFullName);

						string baseTypeFullName = jsonBaseType.BaseName;
						TypeName baseTypeName = SplitJsonTypeName(baseTypeFullName);

						Type type = GetType(typeFullName, assemblies);

						Type[] typeParams = GetTypes(
							jsonBaseType.GenericTypes,
							assemblies);

						AppendStubBaseType(
							typeName,
							baseTypeName,
							typeParams,
							type,
							timestamp,
							builders.CsharpBaseTypes);
					}
				}
			}

			// Need at least one init param
			builders.CsharpInitParams.Append("object stub");
			builders.CsharpInitCall.Append("null // stub");
		}

		static void AppendStubBaseType(
			TypeName typeName,
			TypeName baseTypeName,
			Type[] typeParams,
			Type type,
			string timestamp,
			StringBuilder output)
		{
			int indent = AppendNamespaceBeginning(
				baseTypeName.Namespace,
				output);
			AppendIndent(indent, output);
			if (type.IsClass)
			{
				output.Append("abstract public class ");
			}
			else
			{
				output.Append("public interface ");
			}
			output.Append(baseTypeName.Name);
			output.Append(" : ");
			AppendCsharpTypeFullName(
				typeName,
				output);
			AppendCSharpTypeParameters(
				typeParams,
				output);
			output.Append('\n');
			AppendIndent(indent, output);
			output.Append("{\n");
			AppendIndent(indent + 1, output);
			output.Append("// Stub version. GenerateBindings is still in progress. ");
			output.Append(timestamp);
			output.Append('\n');
			if (type.IsClass)
			{
				output.Append('\n');
				ConstructorInfo[] constructors = type.GetConstructors();
				if (constructors != null && constructors.Length > 0)
				{
					foreach (ConstructorInfo ctor in constructors)
					{
						if (ctor.IsPublic
						    && ctor.GetCustomAttributes(typeof(ObsoleteAttribute), true).Length == 0)
						{
							ParameterInfo[] ctorParams = ConvertParameters(
								ctor.GetParameters());
							output.Append("\t\t");
							output.Append(baseTypeName.Name);
							output.Append('(');
							AppendCsharpParams(
								ctorParams,
								output);
							output.Append(")\n");
							output.Append("\t\t\t: base(");
							AppendCsharpFunctionCallParameters(
								ctorParams,
								output);
							output.Append(")\n");
							output.Append("\t\t{\n");
							output.Append("\t\t}\n");
							output.Append("\t\t\n");
							break;
						}
					}
				}
			}
			AppendIndent(indent, output);
			output.Append("}\n");
			AppendNamespaceEnding(indent, output);
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
			DateTime beforeTime = DateTime.Now;
			
			JsonDocument doc = LoadJson();
			Assembly[] assemblies = GetAssemblies(doc.Assemblies);
			StringBuilders builders = new StringBuilders();
			
			// Get the default number of maximum simultaneous objects in case
			// it's not specified for a specific type
			int defaultMaxSimultaneous = doc.DefaultMaxSimultaneous != 0
				? doc.DefaultMaxSimultaneous
				: DEFAULT_MAX_SIMULTANEOUS;
			
			// Init param for max managed Objects
			builders.CppInitParams.Append("\tint32_t maxManagedObjects,\n");
			builders.CsharpInitParams.Append("\t\t\tint maxManagedObjects,\n");
			builders.CsharpInitCall.Append("\t\t\t\t");
			builders.CsharpInitCall.Append(defaultMaxSimultaneous);
			builders.CsharpInitCall.Append(",\n");
			
			// C# ObjectStore Init call
			builders.CsharpStoreInitCalls.Append(
				"\t\t\tNativeScript.Bindings.ObjectStore.Init(");
			builders.CsharpStoreInitCalls.Append(defaultMaxSimultaneous);
			builders.CsharpStoreInitCalls.Append(");\n");
			
			// Generate types
			if (doc.Types != null)
			{
				foreach (JsonType jsonType in doc.Types)
				{
					Type type = GetType(jsonType.Name, assemblies);
					TypeKind typeKind = GetTypeKind(type);
					AppendType(
						jsonType,
						type,
						typeKind,
						assemblies,
						defaultMaxSimultaneous,
						builders);
					
					if (jsonType.BaseTypes != null)
					{
						Type[] genericArgTypes = type.GetGenericArguments();
						foreach (JsonBaseType jsonBaseType in jsonType.BaseTypes)
						{
							TypeName baseTypeTypeName = GetBaseTypeBaseNameAndNamespace(
								jsonBaseType,
								type,
								genericArgTypes,
								builders.TempStrBuilder);
							AppendBaseType(
								type,
								baseTypeTypeName,
								jsonBaseType,
								assemblies,
								defaultMaxSimultaneous,
								builders);
						}
					}
				}
			}
			
			// Generate boxing and unboxing for primitive types
			foreach (Type type in PRIMITIVE_TYPES)
			{
				string dummyString;
				ParameterInfo[] dummyParams;
				AppendBoxingBindings(
					type,
					TypeKind.Primitive,
					null,
					builders,
					out dummyString,
					out dummyParams);
				AppendUnboxing(
					type,
					TypeKind.Primitive,
					null,
					builders);
			}
			
			// Generate arrays
			if (doc.Arrays != null)
			{
				foreach (JsonArray array in doc.Arrays)
				{
					AppendArray(
						array,
						assemblies,
						builders);
				}
			}
			
			// Generate delegates
			if (doc.Delegates != null)
			{
				foreach (JsonDelegate del in doc.Delegates)
				{
					AppendDelegate(
						del,
						assemblies,
						defaultMaxSimultaneous,
						builders);
				}
			}
			
			// Generate exception setters
			AppendExceptions(
				doc,
				assemblies,
				builders);
			
			// Output source files
			RemoveTrailingChars(builders);
			InjectBuilders(builders);
			
			// Inform the user of the result
			if (canRefreshAssetDb)
			{
				AssetDatabase.Refresh();
				DateTime afterTime = DateTime.Now;
				TimeSpan duration = afterTime - beforeTime;
				Debug.LogFormat(
					"Done generating bindings in {0} seconds.",
					duration.TotalSeconds);
			}
			else
			{
				Debug.LogWarning(
					"Can't auto-refresh due to a bug in Unity. " +
					"Please manually refresh assets with " +
					"Assets -> Refresh to finish generating bindings");
			}
		}
		
		static JsonDocument LoadJson()
		{
			string jsonPath = Path.Combine(
				Application.dataPath,
				NativeScriptConstants.JSON_CONFIG_PATH);
			string json = File.ReadAllText(jsonPath);
			return JsonUtility.FromJson<JsonDocument>(json);
		}
		
		static Assembly[] GetAssemblies(string[] assemblyNames)
		{
			const int numDefaultAssemblies =
#if UNITY_2017_2_OR_NEWER
				43;
#else
				7;
#endif
			
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
			assemblies[3] = typeof(Vector3).Assembly; // UnityEngine (core module for 2017.2+)
			assemblies[4] = typeof(Bindings).Assembly; // Runtime scripts
			assemblies[5] = typeof(GenerateBindings).Assembly; // Editor scripts
			assemblies[6] = typeof(EditorPrefs).Assembly; // UnityEditor
#if UNITY_2017_2_OR_NEWER
			assemblies[7] = typeof(UnityEngine.Accessibility.VisionUtility).Assembly; // Unity accessibility module
			assemblies[8] = typeof(UnityEngine.AI.NavMesh).Assembly; // Unity AI module
			assemblies[9] = typeof(UnityEngine.Animations.AnimationClipPlayable).Assembly; // Unity animation module
			assemblies[10] = typeof(UnityEngine.XR.ARRenderMode).Assembly; // Unity AR module
			assemblies[11] = typeof(UnityEngine.AudioSettings).Assembly; // Unity audio module
			assemblies[12] = typeof(UnityEngine.Cloth).Assembly; // Unity cloth module
			assemblies[13] = typeof(UnityEngine.ClusterInput).Assembly; // Unity cluster input module
			assemblies[14] = typeof(UnityEngine.ClusterNetwork).Assembly; // Unity custer renderer module
			assemblies[15] = typeof(UnityEngine.CrashReportHandler.CrashReportHandler).Assembly; // Unity crash reporting module
			assemblies[16] = typeof(UnityEngine.Playables.PlayableDirector).Assembly; // Unity director module
			assemblies[17] = typeof(UnityEngine.SocialPlatforms.IAchievement).Assembly; // Unity game center module
			assemblies[18] = typeof(UnityEngine.ImageConversion).Assembly; // Unity image conversion module
			assemblies[19] = typeof(UnityEngine.GUI).Assembly; // Unity IMGUI module
			assemblies[20] = typeof(UnityEngine.JsonUtility).Assembly; // Unity JSON serialize module
			assemblies[21] = typeof(UnityEngine.ParticleSystem).Assembly; // Unity particle system module
			assemblies[22] = typeof(UnityEngine.Analytics.PerformanceReporting).Assembly; // Unity performance reporting module
			assemblies[23] = typeof(UnityEngine.Physics2D).Assembly; // Unity physics 2D module
			assemblies[24] = typeof(UnityEngine.Physics).Assembly; // Unity physics module
			assemblies[25] = typeof(UnityEngine.ScreenCapture).Assembly; // Unity screen capture module
			assemblies[26] = typeof(UnityEngine.Terrain).Assembly; // Unity terrain module
			assemblies[27] = typeof(UnityEngine.TerrainCollider).Assembly; // Unity terrain physics module
			assemblies[28] = typeof(UnityEngine.Font).Assembly; // Unity text rendering module
			assemblies[29] = typeof(UnityEngine.Tilemaps.Tile).Assembly; // Unity tilemap module
			assemblies[30] = typeof(UnityEngine.Experimental.UIElements.Button).Assembly; // Unity UI elements module
			assemblies[31] = typeof(UnityEngine.Canvas).Assembly; // Unity UI module
			assemblies[32] = typeof(UnityEngine.Networking.NetworkTransport).Assembly; // Unity cloth module
			assemblies[33] = typeof(UnityEngine.Analytics.Analytics).Assembly; // Unity analytics module
			assemblies[34] = typeof(UnityEngine.RemoteSettings).Assembly; // Unity Unity connect module
			assemblies[35] = typeof(UnityEngine.Networking.DownloadHandlerAudioClip).Assembly; // Unity web request audio module
			assemblies[36] = typeof(UnityEngine.WWWForm).Assembly; // Unity web request module
			assemblies[37] = typeof(UnityEngine.Networking.DownloadHandlerTexture).Assembly; // Unity web request texture module
			assemblies[38] = typeof(UnityEngine.WWW).Assembly; // Unity web request WWW module
			assemblies[39] = typeof(UnityEngine.WheelCollider).Assembly; // Unity vehicles module
			assemblies[40] = typeof(UnityEngine.Video.VideoClip).Assembly; // Unity video module
			assemblies[41] = typeof(UnityEngine.XR.InputTracking).Assembly; // Unity VR module
			assemblies[42] = typeof(UnityEngine.WindZone).Assembly; // Unity wind module
#endif
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
			if (type == typeof(void))
			{
				return TypeKind.None;
			}
			
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
			bool allowDefault,
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
			
			if (allowDefault)
			{
				return new ParameterInfo[0];
			}
			
			// Throw an exception so the user knows what to fix in the JSON
			StringBuilder errorBuilder = new StringBuilder(1024);
			errorBuilder.Append("Constructor \"");
			AppendCsharpTypeFullName(type, errorBuilder);
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
			string[] paramTypeNames,
			string[] genericTypeNames)
		{
			foreach (MethodInfo method in methods)
			{
				// Name must match
				if (method.Name != methodName)
				{
					continue;
				}
				
				// All parameters must match
				if (!CheckParametersMatch(
					paramTypeNames,
					method.GetParameters()))
				{
					continue;
				}
				
				// Generic arg count must match
				Type[] methodGenericArgs = method.GetGenericArguments();
				int numGenericTypeNames = genericTypeNames == null ? 0 : genericTypeNames.Length;
				if (methodGenericArgs.Length == numGenericTypeNames)
				{
					return method;
				}
			}
			
			// Throw an exception so the user knows what to fix in the JSON
			StringBuilder errorBuilder = new StringBuilder(1024);
			errorBuilder.Append("Method \"");
			AppendCsharpTypeFullName(type, errorBuilder);
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
		
		static Type[] GetDirectInterfaces(Type type)
		{
			Type[] allInterfaces = type.GetInterfaces();
			List<Type> minimalInterfaces = new List<Type>();
			foreach(Type iType in allInterfaces)
			{
				bool contains = false;
				foreach (Type t in allInterfaces)
				{
					if (Array.IndexOf(t.GetInterfaces(), iType) >= 0)
					{
						contains = true;
						break;
					}
				}
				if (!contains)
				{
					minimalInterfaces.Add(iType);
				}
			}
			minimalInterfaces.Sort((x, y) => x.Name.CompareTo(y.Name));
			return minimalInterfaces.ToArray();
		}
		
		static void AddCppCtorInitType(Type type, List<Type> types)
		{
			if (type.BaseType != null
				&& type.BaseType != typeof(object)
				&& type.BaseType != typeof(ValueType))
			{
				AddCppCtorInitType(type.BaseType, types);
			}
			foreach (Type interfaceType in GetDirectInterfaces(type))
			{
				AddCppCtorInitType(interfaceType, types);
			}
			if (!types.Contains(type))
			{
				types.Add(type);
			}
		}
		
		static Type[] GetCppCtorInitTypes(Type type, bool includeSelf)
		{
			List<Type> types = new List<Type>();
			AddCppCtorInitType(type, types);
			if (!includeSelf)
			{
				types.RemoveAll(t => t == type);
			}
			return types.ToArray();
		}

		static void AppendCppConstructorInitializerList(
			Type[] interfaceTypes,
			int indent,
			StringBuilder output,
			string newline = "\n")
		{
			string separator = ": ";
			for (int i = 0; i < interfaceTypes.Length; ++i)
			{
				Type interfaceType = interfaceTypes[i];
				AppendIndent(
					indent,
					output);
				output.Append(separator);
				AppendCppTypeFullName(
					interfaceType,
					output);
				output.Append("(nullptr)");
				output.Append(newline);
				separator = ", ";
			}
		}

		static void AppendUppercaseWithUnderscores(
			string str,
			StringBuilder output)
		{
			if (string.IsNullOrEmpty(str))
			{
				return;
			}
			char prev = str[0];
			output.Append(char.ToUpper(prev));
			for (int i = 1; i < str.Length; ++i)
			{
				char cur = str[i];
				if (char.IsUpper(cur) && char.IsLower(prev))
				{
					output.Append('_');
				}
				output.Append(char.ToUpper(cur));
				prev = cur;
			}
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
				AppendTypeNameWithoutSuffixes(
					type.Name,
					output);
				if (type.IsArray)
				{
					output.Append("Array");
					output.Append(type.GetArrayRank());
				}
				if (i != len - 1)
				{
					output.Append('_');
				}
			}
		}
		
		static void AppendTypeNames(
			Type[] typeNames,
			StringBuilder output)
		{
			if (typeNames != null)
			{
				for (int i = 0, len = typeNames.Length; i < len; ++i)
				{
					Type curType = typeNames[i];
					AppendNamespace(
						curType.Namespace,
						string.Empty,
						output);
					AppendTypeNameWithoutSuffixes(
						curType.Name,
						output);
					if (i != len - 1)
					{
						output.Append('_');
					}
				}
			}
		}

		static TypeName GetBaseTypeBaseNameAndNamespace(
			JsonBaseType jsonBaseType,
			Type type,
			Type[] typeParams,
			StringBuilder tempStringBuilder)
		{
			// Get specified (optional) base type name
			TypeName baseTypeTypeName = SplitJsonTypeName(jsonBaseType.BaseName);

			// If base type name isn't provided, make one
			if (string.IsNullOrEmpty(baseTypeTypeName.Name))
			{
				tempStringBuilder.Length = 0;
				AppendNamespace(
					type.Namespace,
					string.Empty,
					tempStringBuilder);
				tempStringBuilder.Append("Base");
				AppendTypeNameWithoutSuffixes(
					type.Name,
					tempStringBuilder);
				AppendTypeNames(
					typeParams,
					tempStringBuilder);
				baseTypeTypeName.Name = tempStringBuilder.ToString();
			}

			baseTypeTypeName.NumTypeParams = typeParams.Length;
			return baseTypeTypeName;
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

		static TypeName SplitJsonTypeName(string fullName)
		{
			string typeName;
			string typeNamespace;

			// No full name
			if (string.IsNullOrEmpty(fullName))
			{
				typeName = string.Empty;
				typeNamespace = string.Empty;
			}
			else
			{
				// Has a namespace
				int index = fullName.LastIndexOf('.');
				if (index >= 0)
				{
					typeNamespace = fullName.Substring(0, index);
					typeName = fullName.Substring(index + 1);
				}
				// No namespace. Just name.
				else
				{
					typeName = fullName;
					typeNamespace = string.Empty;
				}
			}

			return GetTypeName(typeName, typeNamespace);
		}
		
		static ParameterInfo[] ConvertParameters(
			System.Reflection.ParameterInfo[] reflectionParameters,
			int start = 0)
		{
			int num = reflectionParameters.Length - start;
			ParameterInfo[] parameters = new ParameterInfo[num];
			for (int i = start; i < num; ++i)
			{
				System.Reflection.ParameterInfo reflectionInfo =
					reflectionParameters[i];
				ParameterInfo info = new ParameterInfo();
				info.Name = reflectionInfo.Name;
				info.ParameterType = reflectionInfo.ParameterType;
				info.IsOut = reflectionInfo.IsOut;
				info.IsRef = !info.IsOut && info.ParameterType.IsByRef;
				info.DereferencedParameterType = DereferenceParameterType(
					reflectionInfo);
				info.Kind = GetTypeKind(
					info.DereferencedParameterType);
				info.HasDefault = (reflectionInfo.Attributes &
					ParameterAttributes.HasDefault) ==
					ParameterAttributes.HasDefault;
				info.DefaultValue = reflectionInfo.DefaultValue;
				info.IsVarArg = reflectionInfo.IsDefined(
					typeof(ParamArrayAttribute),
					false);
				parameters[i - start] = info;
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

		static TypeName GetTypeName(Type type)
		{
			TypeName typeName;
			typeName.Name = type.Name;
			typeName.Namespace = type.Namespace;
			typeName.NumTypeParams = type.GetGenericArguments().Length;
			return typeName;
		}

		static TypeName GetTypeName(
			string name,
			string namespaceName)
		{
			TypeName typeName;
			typeName.Name = name;
			typeName.Namespace = namespaceName;
			typeName.NumTypeParams = 0;
			return typeName;
		}

		static TypeName GetTypeName(
			string name,
			string namespaceName,
			int numTypeParams)
		{
			TypeName typeName;
			typeName.Name = name;
			typeName.Namespace = namespaceName;
			typeName.NumTypeParams = numTypeParams;
			return typeName;
		}
		
		static bool IsStatic(Type type)
		{
			return type.IsAbstract && type.IsSealed;
		}
		
		static bool IsDelegate(Type type)
		{
			return typeof(Delegate).IsAssignableFrom(type);
		}
		
		static bool IsNonDelegateClass(Type type)
		{
			return type.IsClass && !IsDelegate(type);
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
			if (type.IsPrimitive || type.IsEnum || type == typeof(void))
			{
				return true;
			}
			const BindingFlags bindingFlags =
				BindingFlags.Instance
				| BindingFlags.NonPublic
				| BindingFlags.Public;
			foreach (FieldInfo field in type.GetFields(bindingFlags))
			{
				if (!field.IsPublic
					|| (!field.IsStatic
						&& !IsFullValueType(field.FieldType)))
				{
					return false;
				}
			}
			return true;
		}
		
		static int ArrayIndexOf<T>(T[] array, T value)
		{
			return array != null ?
				Array.IndexOf(array, value) :
				-1;
		}
		
		static void AppendTypeNameWithoutGenericSuffix(
			string typeName,
			StringBuilder output)
		{
			// Names are like "List`1"
			// Remove the ` and everything after it
			int backtickIndex = typeName.IndexOf('`');
			if (backtickIndex < 0)
			{
				output.Append(typeName);
			}
			else
			{
				// Append up to (but not including) the `
				output.Append(
					typeName,
					0,
					backtickIndex);
				
				// Find the first non-number after the `
				int endIndex = backtickIndex + 1;
				while (
					endIndex < typeName.Length
					&& char.IsNumber(typeName[endIndex]))
				{
					endIndex++;
				}
				
				// Append everything after the numbers
				if (endIndex < typeName.Length)
				{
					output.Append(
						typeName,
						endIndex,
						typeName.Length - endIndex);
				}
			}
		}
		
		static void AppendTypeNameWithoutSuffixes(
			string typeName,
			StringBuilder output)
		{
			// Names are like "List`1" or "int[]" or "List`1[]"
			// Remove the first of ` or [ and everything after it
			int backtickIndex = typeName.IndexOf('`');
			if (backtickIndex < 0)
			{
				int bracketIndex = typeName.IndexOf('[');
				if (bracketIndex < 0)
				{
					output.Append(typeName);
				}
				else
				{
					output.Append(typeName, 0, bracketIndex);
				}
			}
			else
			{
				output.Append(typeName, 0, backtickIndex);
			}
		}
		
		static int AppendType(
			JsonType jsonType,
			Type type,
			TypeKind typeKind,
			Assembly[] assemblies,
			int defaultMaxSimultaneous,
			StringBuilders builders)
		{
			if (typeKind == TypeKind.Enum)
			{
				AppendEnum(
					type,
					builders);
				AppendUnboxing(
					type,
					typeKind,
					null,
					builders);
				return 0;
			}
			else
			{
				int totalMaxSimultaneous = 0;
				Type[] genericArgTypes = type.GetGenericArguments();
				if (jsonType.GenericParams != null)
				{
					if (!IsStatic(type))
					{
						AppendCppTemplateDeclaration(
							GetTypeName(type),
							builders.CppTemplateDeclarations);
					}
					
					foreach (JsonGenericParams jsonGenericParams
						in jsonType.GenericParams)
					{
						Type[] typeParams = GetTypes(
							jsonGenericParams.Types,
							assemblies);
						Type genericType = type.MakeGenericType(typeParams);
						int maxSimultaneous = jsonGenericParams.MaxSimultaneous != 0
							? jsonGenericParams.MaxSimultaneous
							: jsonType.MaxSimultaneous != 0
								? jsonType.MaxSimultaneous
								: defaultMaxSimultaneous;
						totalMaxSimultaneous += maxSimultaneous;
						AppendType(
							jsonType,
							genericArgTypes,
							genericType,
							typeKind,
							typeParams,
							maxSimultaneous,
							assemblies,
							builders);
						if (typeKind != TypeKind.Class)
						{
							AppendUnboxing(
								genericType,
								typeKind,
								typeParams,
								builders);
						}
					}
				}
				else
				{
					int maxSimultaneous = jsonType.MaxSimultaneous != 0
						? jsonType.MaxSimultaneous
						: defaultMaxSimultaneous;
					totalMaxSimultaneous += maxSimultaneous;
					AppendType(
						jsonType,
						genericArgTypes,
						type,
						typeKind,
						null,
						maxSimultaneous,
						assemblies,
						builders);
					if (typeKind != TypeKind.Class)
					{
						AppendUnboxing(
							type,
							typeKind,
							null,
							builders);
					}
				}
				return totalMaxSimultaneous;
			}
		}
		
		static void AppendType(
			JsonType jsonType,
			Type[] genericArgTypes,
			Type type,
			TypeKind typeKind,
			Type[] typeParams,
			int maxSimultaneous,
			Assembly[] assemblies,
			StringBuilders builders)
		{
			bool isStatic = IsStatic(type);
			if (!isStatic && typeKind == TypeKind.ManagedStruct)
			{
				// C# StructStore Init call
				builders.CsharpStoreInitCalls.Append(
					"\t\t\tNativeScript.Bindings.StructStore<");
				AppendCsharpTypeFullName(
					type,
					builders.CsharpStoreInitCalls);
				builders.CsharpStoreInitCalls.Append(">.Init(");
				builders.CsharpStoreInitCalls.Append(maxSimultaneous);
				builders.CsharpStoreInitCalls.Append(");\n");
				
				// Build function name suffix
				builders.TempStrBuilder.Length = 0;
				AppendReleaseFunctionNameSuffix(
					GetTypeName(type),
					typeParams,
					builders.TempStrBuilder);
				string funcNameSuffix = builders.TempStrBuilder.ToString();
				
				// Build function name
				builders.TempStrBuilder.Length = 0;
				builders.TempStrBuilder.Append("Release");
				AppendReleaseFunctionNameSuffix(
					GetTypeName(type),
					typeParams,
					builders.TempStrBuilder);
				string funcName = builders.TempStrBuilder.ToString();
				
				// Build lowercase function name
				builders.TempStrBuilder[0] = char.ToLower(
					builders.TempStrBuilder[0]);
				string funcNameLower = builders.TempStrBuilder.ToString();
				
				// Build ReleaseX parameters
				ParameterInfo paramInfo = new ParameterInfo();
				paramInfo.Name = "handle";
				paramInfo.ParameterType = typeof(int);
				paramInfo.IsOut = false;
				paramInfo.IsRef = false;
				paramInfo.DereferencedParameterType = typeof(int);
				paramInfo.Kind = TypeKind.Primitive;
				ParameterInfo[] parameters = { paramInfo };
				
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
					parameters,
					builders.CsharpFunctions);
				builders.CsharpFunctions.Append(
					"if (handle != 0)\n\t\t\t{\n");
				builders.CsharpFunctions.Append(
					"\t\t\t\tNativeScript.Bindings.StructStore<");
				AppendCsharpTypeFullName(
					type,
					builders.CsharpFunctions);
				builders.CsharpFunctions.Append(
					">.Remove(handle);\n\t\t\t}");
				AppendCsharpFunctionEnd(
					typeof(void),
					new Type[0],
					parameters,
					builders.CsharpFunctions);
				
				// C++ function pointer definition
				AppendCppFunctionPointerDefinition(
					funcName,
					true,
					default(TypeName),
					TypeKind.None,
					parameters,
					typeof(void),
					builders.CppFunctionPointers);
				
				// C++ init param for ReleaseX
				AppendCppInitParam(
					funcNameLower,
					true,
					default(TypeName),
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
				
				// C++ init body for handle array length
				builders.CppInitBody.Append("\tPlugin::RefCounts");
				builders.CppInitBody.Append(funcNameSuffix);
				builders.CppInitBody.Append(" = (int32_t*)curMemory;\n");
				builders.CppInitBody.Append("\tcurMemory += ");
				builders.CppInitBody.Append(maxSimultaneous);
				builders.CppInitBody.Append(" * sizeof(int32_t);\n");
				builders.CppInitBody.Append("\tPlugin::RefCountsLen");
				builders.CppInitBody.Append(funcNameSuffix);
				builders.CppInitBody.Append(" = ");
				builders.CppInitBody.Append(maxSimultaneous);
				builders.CppInitBody.Append(";\n");
				
				// C++ ref count state and functions
				builders.CppGlobalStateAndFunctions.Append("\tint32_t RefCountsLen");
				builders.CppGlobalStateAndFunctions.Append(funcNameSuffix);
				builders.CppGlobalStateAndFunctions.Append(";\n\tint32_t* RefCounts");
				builders.CppGlobalStateAndFunctions.Append(funcNameSuffix);
				builders.CppGlobalStateAndFunctions.Append(";\n\t\n\tvoid ReferenceManaged");
				builders.CppGlobalStateAndFunctions.Append(funcNameSuffix);
				builders.CppGlobalStateAndFunctions.Append("(int32_t handle)\n");
				builders.CppGlobalStateAndFunctions.Append("\t{\n");
				builders.CppGlobalStateAndFunctions.Append("\t\tassert(handle >= 0 && handle < RefCountsLen");
				builders.CppGlobalStateAndFunctions.Append(funcNameSuffix);
				builders.CppGlobalStateAndFunctions.Append(");\n");
				builders.CppGlobalStateAndFunctions.Append("\t\tif (handle != 0)\n");
				builders.CppGlobalStateAndFunctions.Append("\t\t{\n");
				builders.CppGlobalStateAndFunctions.Append("\t\t\tRefCounts");
				builders.CppGlobalStateAndFunctions.Append(funcNameSuffix);
				builders.CppGlobalStateAndFunctions.Append("[handle]++;\n");
				builders.CppGlobalStateAndFunctions.Append("\t\t}\n");
				builders.CppGlobalStateAndFunctions.Append("\t}\n");
				builders.CppGlobalStateAndFunctions.Append("\t\n");
				builders.CppGlobalStateAndFunctions.Append("\tvoid DereferenceManaged");
				builders.CppGlobalStateAndFunctions.Append(funcNameSuffix);
				builders.CppGlobalStateAndFunctions.Append("(int32_t handle)\n");
				builders.CppGlobalStateAndFunctions.Append("\t{\n");
				builders.CppGlobalStateAndFunctions.Append("\t\tassert(handle >= 0 && handle < RefCountsLen");
				builders.CppGlobalStateAndFunctions.Append(funcNameSuffix);
				builders.CppGlobalStateAndFunctions.Append(");\n");
				builders.CppGlobalStateAndFunctions.Append("\t\tif (handle != 0)\n");
				builders.CppGlobalStateAndFunctions.Append("\t\t{\n");
				builders.CppGlobalStateAndFunctions.Append("\t\t\tint32_t numRemain = --RefCounts");
				builders.CppGlobalStateAndFunctions.Append(funcNameSuffix);
				builders.CppGlobalStateAndFunctions.Append("[handle];\n");
				builders.CppGlobalStateAndFunctions.Append("\t\t\tif (numRemain == 0)\n");
				builders.CppGlobalStateAndFunctions.Append("\t\t\t{\n");
				builders.CppGlobalStateAndFunctions.Append("\t\t\t\tRelease");
				builders.CppGlobalStateAndFunctions.Append(funcNameSuffix);
				builders.CppGlobalStateAndFunctions.Append("(handle);\n");
				builders.CppGlobalStateAndFunctions.Append("\t\t\t}\n");
				builders.CppGlobalStateAndFunctions.Append("\t\t}\n");
				builders.CppGlobalStateAndFunctions.Append("\t}\n\t\n");
			}
			
			// C++ type declaration
			int indent = AppendCppTypeDeclaration(
				GetTypeName(type),
				isStatic,
				typeParams,
				typeParams != null ?
					builders.CppTemplateSpecializationDeclarations :
					builders.CppTypeDeclarations);
			
			// C++ type definition (beginning)
			Type[] interfaceTypes = GetDirectInterfaces(type);
			string baseTypeName;
			string baseTypeNamespace;
			Type[] baseTypeTypeParams;
			switch (typeKind)
			{
				case TypeKind.FullStruct:
					baseTypeName = null;
					baseTypeNamespace = null;
					baseTypeTypeParams = null;
					break;
				case TypeKind.ManagedStruct:
					if (interfaceTypes.Length == 0)
					{
						baseTypeName = "ManagedType";
						baseTypeNamespace = "Plugin";
						baseTypeTypeParams = null;
					}
					else
					{
						baseTypeName = null;
						baseTypeNamespace = null;
						baseTypeTypeParams = null;
					}
					break;
				default:
					Type baseType = type.BaseType ?? typeof(object);
					baseTypeName = baseType.Name;
					baseTypeNamespace = baseType.Namespace;
					baseTypeTypeParams = baseType.GetGenericArguments();
					break;
			}
			
			AppendCppTypeDefinitionBegin(
				GetTypeName(type),
				typeKind,
				typeParams,
				GetTypeName(baseTypeName, baseTypeNamespace),
				baseTypeTypeParams,
				interfaceTypes,
				isStatic,
				indent,
				builders.CppTypeDefinitions);
			
			// C++ method definition
			Type[] cppCtorInterfaceTypes = GetCppCtorInitTypes(
				type,
				false);
			int cppMethodDefinitionsIndent = AppendCppMethodDefinitionsBegin(
				GetTypeName(type),
				typeKind,
				typeParams,
				cppCtorInterfaceTypes,
				isStatic,
				(extraIndent, subject) => {},
				(extraIndent, subject) => {},
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
						jsonCtor.ParamTypes,
						jsonCtor.Exceptions,
						type,
						isStatic,
						typeKind,
						assemblies,
						typeParams,
						genericArgTypes,
						cppCtorInterfaceTypes,
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
			
			// Events
			if (jsonType.Events != null)
			{
				foreach (JsonEvent jsonEvent in jsonType.Events)
				{
					AppendEvent(
						jsonEvent,
						type,
						isStatic,
						typeKind,
						typeParams,
						indent,
						builders
					);
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
						genericArgTypes,
						indent,
						builders);
				}
			}
			
			// Boxing
			if (typeKind != TypeKind.Class)
			{
				AppendBoxing(
					type,
					typeKind,
					typeParams,
					indent,
					builders);
			}
			
			// C++ type definition (ending)
			AppendCppTypeDefinitionEnd(
				isStatic,
				indent,
				builders.CppTypeDefinitions);
			
			// C++ method definition (ending)
			AppendCppMethodDefinitionsEnd(
				cppMethodDefinitionsIndent,
				builders.CppMethodDefinitions);
			
			// Generate iterator if this type implements IEnumerable<T>
			Type[] allInterfaces = type.GetInterfaces();
			foreach (Type interfaceType in allInterfaces)
			{
				if (interfaceType.IsGenericType
					&& interfaceType.GetGenericTypeDefinition() == typeof(IEnumerable<>))
				{
					builders.TempStrBuilder.Length = 0;
					AppendNamespace(
						type.Namespace,
						string.Empty,
						builders.TempStrBuilder);
					AppendTypeNameWithoutGenericSuffix(
						type.Name,
						builders.TempStrBuilder);
					AppendTypeNames(
						typeParams,
						builders.TempStrBuilder);
					string bindingEnumerableTypeName = builders.TempStrBuilder.ToString();
					
					Type elementType = interfaceType.GetGenericArguments()[0];
					AppendGenericEnumerableIterator(
						type,
						typeof(IEnumerator<>).MakeGenericType(elementType),
						elementType,
						bindingEnumerableTypeName,
						builders.CppTypeDefinitions,
						builders.CppMethodDefinitions);
					break;
				}
			}
		}
		
		static void AppendBaseType(
			Type type,
			TypeName cppBaseTypeTypeName,
			JsonBaseType jsonBaseType,
			Assembly[] assemblies,
			int defaultMaxSimultaneous,
			StringBuilders builders)
		{
			int maxSimultaneous = jsonBaseType.MaxSimultaneous != 0
				? jsonBaseType.MaxSimultaneous
				: defaultMaxSimultaneous;
			if (jsonBaseType.GenericTypes != null)
			{
				Type[] typeParams = GetTypes(
					jsonBaseType.GenericTypes,
					assemblies);
				Type genericType = type.MakeGenericType(typeParams);
				AppendBaseType(
					genericType,
					jsonBaseType,
					cppBaseTypeTypeName,
					typeParams,
					maxSimultaneous,
					assemblies,
					builders);
			}
			else
			{
				AppendBaseType(
					type,
					jsonBaseType,
					cppBaseTypeTypeName,
					null,
					maxSimultaneous,
					assemblies,
					builders);
			}
		}
		
		static void AppendReleaseFunctionNameSuffix(
			TypeName typeTypeName,
			Type[] typeParams,
			StringBuilder output)
		{
			AppendNamespace(
				typeTypeName.Namespace,
				string.Empty,
				output);
			AppendTypeNameWithoutSuffixes(
				typeTypeName.Name,
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
					AppendTypeNameWithoutSuffixes(
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
			StringBuilders builders)
		{
			// C++ type declaration
			int indent = AppendCppTypeDeclaration(
				GetTypeName(type),
				false,
				null,
				builders.CppTypeDeclarations);
			
			// C++ type definition (begin)
			AppendCppTypeDefinitionBegin(
				GetTypeName(type),
				TypeKind.FullStruct,
				null,
				default(TypeName),
				null,
				null,
				false,
				indent,
				builders.CppTypeDefinitions);
			AppendIndent(
				indent + 1,
				builders.CppTypeDefinitions);
			
			// Primitive type field
			Type underlyingType = Enum.GetUnderlyingType(type);
			AppendCppPrimitiveTypeName(
				underlyingType,
				builders.CppTypeDefinitions);
			builders.CppTypeDefinitions.Append(" Value;\n");
			
			// Enumerator fields
			FieldInfo[] fields = type.GetFields(
				BindingFlags.Static
				| BindingFlags.Public);
			for (int i = 0; i < fields.Length; ++i)
			{
				FieldInfo field = fields[i];
				AppendIndent(
					indent + 1,
					builders.CppTypeDefinitions);
				builders.CppTypeDefinitions.Append("static const ");
				AppendCppTypeFullName(
					type,
					builders.CppTypeDefinitions);
				builders.CppTypeDefinitions.Append(' ');
				builders.CppTypeDefinitions.Append(field.Name);
				builders.CppTypeDefinitions.Append(";\n");
			}
			
			// Constructor from primitive type
			AppendIndent(
				indent + 1,
				builders.CppTypeDefinitions);
			builders.CppTypeDefinitions.Append("explicit ");
			builders.CppTypeDefinitions.Append(type.Name);
			builders.CppTypeDefinitions.Append('(');
			AppendCppPrimitiveTypeName(
				underlyingType,
				builders.CppTypeDefinitions);
			builders.CppTypeDefinitions.Append(" value);\n");
			
			// Conversion operator to primitive type
			AppendIndent(
				indent + 1,
				builders.CppTypeDefinitions);
			builders.CppTypeDefinitions.Append("explicit operator ");
			AppendCppPrimitiveTypeName(
				underlyingType,
				builders.CppTypeDefinitions);
			builders.CppTypeDefinitions.Append("() const;\n");
			
			// Equality operator
			AppendIndent(
				indent + 1,
				builders.CppTypeDefinitions);
			builders.CppTypeDefinitions.Append("bool operator==(");
			builders.CppTypeDefinitions.Append(type.Name);
			builders.CppTypeDefinitions.Append(" other);\n");
			
			// Inequality operator
			AppendIndent(
				indent + 1,
				builders.CppTypeDefinitions);
			builders.CppTypeDefinitions.Append("bool operator!=(");
			builders.CppTypeDefinitions.Append(type.Name);
			builders.CppTypeDefinitions.Append(" other);\n");
			
			AppendNamespaceBeginning(
				type.Namespace,
				builders.CppMethodDefinitions);
			
			// Constructor from primitive type
			AppendIndent(
				indent,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append(type.Name);
			builders.CppMethodDefinitions.Append("::");
			builders.CppMethodDefinitions.Append(type.Name);
			builders.CppMethodDefinitions.Append('(');
			AppendCppPrimitiveTypeName(
				underlyingType,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append(" value)\n");
			AppendIndent(
				indent + 1,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append(": Value(value)\n");
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
			
			// Conversion operator to primitive type
			AppendIndent(
				indent,
				builders.CppMethodDefinitions);
			AppendCppTypeFullName(
				type,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("::operator ");
			AppendCppPrimitiveTypeName(
				underlyingType,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("() const\n");
			AppendIndent(
				indent,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("{\n");
			AppendIndent(
				indent + 1,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("return Value;\n");
			AppendIndent(
				indent,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("}\n");
			AppendIndent(
				indent,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append('\n');
			
			// Equality operator
			AppendIndent(
				indent,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("bool ");
			AppendCppTypeFullName(
				type,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("::operator==(");
			builders.CppMethodDefinitions.Append(type.Name);
			builders.CppMethodDefinitions.Append(" other)\n");
			AppendIndent(
				indent,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("{\n");
			AppendIndent(
				indent + 1,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("return Value == other.Value;\n");
			AppendIndent(
				indent,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("}\n");
			AppendIndent(
				indent,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append('\n');
			
			// Inequality operator
			AppendIndent(
				indent,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("bool ");
			AppendCppTypeFullName(
				type,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("::operator!=(");
			builders.CppMethodDefinitions.Append(type.Name);
			builders.CppMethodDefinitions.Append(" other)\n");
			AppendIndent(
				indent,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("{\n");
			AppendIndent(
				indent + 1,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("return Value != other.Value;\n");
			AppendIndent(
				indent,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("}\n");
			AppendIndent(
				indent,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append('\n');
			
			AppendBoxing(
				type,
				TypeKind.Enum,
				null,
				indent,
				builders);
			
			AppendNamespaceEnding(
				indent,
				builders.CppMethodDefinitions);
			AppendIndent(
				indent,
				builders.CppTypeDefinitions);
			builders.CppTypeDefinitions.Append("};\n");
			AppendNamespaceEnding(
				indent,
				builders.CppTypeDefinitions);
			builders.CppTypeDefinitions.Append('\n');
			
			// Static initialization
			for (int i = 0; i < fields.Length; ++i)
			{
				FieldInfo field = fields[i];
				builders.CppMethodDefinitions.Append("const ");
				AppendCppTypeFullName(
					type,
					builders.CppMethodDefinitions);
				builders.CppMethodDefinitions.Append(' ');
				AppendCppTypeFullName(
					type,
					builders.CppMethodDefinitions);
				builders.CppMethodDefinitions.Append("::");
				builders.CppMethodDefinitions.Append(field.Name);
				builders.CppMethodDefinitions.Append('(');
				builders.CppMethodDefinitions.Append(
					field.GetRawConstantValue());
				builders.CppMethodDefinitions.Append(");\n");
			}
			builders.CppMethodDefinitions.Append('\n');
		}
		
		static void AppendBoxing(
			Type type,
			TypeKind typeKind,
			Type[] typeParams,
			int indent,
			StringBuilders builders)
		{
			string boxFuncName;
			ParameterInfo[] boxCppParams;
			AppendBoxingBindings(
				type,
				typeKind,
				typeParams,
				builders,
				out boxFuncName,
				out boxCppParams);
			
			for (Type baseType = type.BaseType;
				baseType != null;
				baseType = baseType.BaseType)
			{
				string boxMethodDefinitionName;
				string boxMethodDeclarationName;
				AppendCppBoxingMethodNames(
					baseType,
					builders.TempStrBuilder,
					out boxMethodDefinitionName,
					out boxMethodDeclarationName);
				AppendCppBoxingMethodDeclaration(
					boxMethodDeclarationName,
					boxCppParams,
					indent + 1,
					builders.CppTypeDefinitions);
				AppendCppBoxingMethodDefinition(
					type,
					typeParams,
					baseType,
					typeKind,
					boxMethodDefinitionName,
					boxFuncName,
					boxCppParams,
					indent,
					builders.CppMethodDefinitions);
			}
			foreach (Type interfaceType in type.GetInterfaces())
			{
				string boxMethodDefinitionName;
				string boxMethodDeclarationName;
				AppendCppBoxingMethodNames(
					interfaceType,
					builders.TempStrBuilder,
					out boxMethodDefinitionName,
					out boxMethodDeclarationName);
				AppendCppBoxingMethodDeclaration(
					boxMethodDeclarationName,
					boxCppParams,
					indent + 1,
					builders.CppTypeDefinitions);
				AppendCppBoxingMethodDefinition(
					type,
					typeParams,
					interfaceType,
					typeKind,
					boxMethodDefinitionName,
					boxFuncName,
					boxCppParams,
					indent,
					builders.CppMethodDefinitions);
			}
		}
		
		static void AppendBoxingBindings(
			Type type,
			TypeKind typeKind,
			Type[] typeParams,
			StringBuilders builders,
			out string boxFuncName,
			out ParameterInfo[] boxCppParams)
		{
			builders.TempStrBuilder.Length = 0;
			builders.TempStrBuilder.Append("Box");
			AppendTypeNameWithoutSuffixes(
				type.Name,
				builders.TempStrBuilder);
			AppendTypeNames(
				typeParams,
				builders.TempStrBuilder);
			boxFuncName = builders.TempStrBuilder.ToString();
			
			builders.TempStrBuilder[0] = char.ToLower(
				builders.TempStrBuilder[0]);
			string boxFuncNameLower = builders.TempStrBuilder.ToString();
			
			ParameterInfo[] boxParams = {
				new ParameterInfo
				{
					Name = "val",
					ParameterType = type,
					DereferencedParameterType = type,
					IsOut = false,
					IsRef = false,
					Kind = typeKind
				}
			};
			
			boxCppParams = new ParameterInfo[0];
			
			// C# init params
			AppendCsharpInitParam(
				boxFuncNameLower,
				builders.CsharpInitParams);
			
			// C# delegate types
			AppendCsharpDelegateType(
				boxFuncName,
				true,
				type,
				typeKind,
				typeof(object),
				boxParams,
				builders.CsharpDelegateTypes);
			
			// C# init call args
			AppendCsharpInitCallArg(
				boxFuncName,
				builders.CsharpInitCall);
			
			// C# box function
			AppendCsharpFunctionBeginning(
				typeof(object),
				boxFuncName,
				true,
				TypeKind.Class,
				typeof(object),
				boxParams,
				builders.CsharpFunctions);
			builders.CsharpFunctions.Append(
				"NativeScript.Bindings.ObjectStore.Store((object)val);");
			AppendCsharpFunctionReturn(
				boxParams,
				typeof(object),
				TypeKind.Class,
				null,
				true,
				builders.CsharpFunctions);
			
			// C++ function pointers
			AppendCppFunctionPointerDefinition(
				boxFuncName,
				true,
				GetTypeName(type),
				typeKind,
				boxParams,
				typeof(object),
				builders.CppFunctionPointers);
			
			// C++ init params
			AppendCppInitParam(
				boxFuncNameLower,
				true,
				GetTypeName(type),
				typeKind,
				boxParams,
				typeof(object),
				builders.CppInitParams);
			
			// C++ init body
			AppendCppInitBody(
				boxFuncName,
				boxFuncNameLower,
				builders.CppInitBody);
		}
		
		static void AppendUnboxing(
			Type type,
			TypeKind typeKind,
			Type[] typeParams,
			StringBuilders builders)
		{
			builders.TempStrBuilder.Length = 0;
			builders.TempStrBuilder.Append("Unbox");
			AppendTypeNameWithoutSuffixes(
				type.Name,
				builders.TempStrBuilder);
			AppendTypeNames(
				typeParams,
				builders.TempStrBuilder);
			string unboxFuncName = builders.TempStrBuilder.ToString();
			
			builders.TempStrBuilder[0] = char.ToLower(
				builders.TempStrBuilder[0]);
			string unboxFuncNameLower = builders.TempStrBuilder.ToString();
			
			builders.TempStrBuilder.Length = 0;
			builders.TempStrBuilder.Append("operator ");
			AppendCppTypeFullName(
				type,
				builders.TempStrBuilder);
			string unboxMethodDefinitionName = builders.TempStrBuilder.ToString();
			
			builders.TempStrBuilder.Length = 0;
			builders.TempStrBuilder.Append("explicit ");
			builders.TempStrBuilder.Append(unboxMethodDefinitionName);
			string unboxMethodDeclarationName = builders.TempStrBuilder.ToString();
			
			ParameterInfo[] unboxParams = {
				new ParameterInfo
				{
					Name = "val",
					ParameterType = typeof(object),
					DereferencedParameterType = typeof(object),
					IsOut = false,
					IsRef = false,
					Kind = TypeKind.Class
				}
			};
			
			ParameterInfo[] unboxCppParams = new ParameterInfo[0];
			
			// C# init params
			AppendCsharpInitParam(
				unboxFuncNameLower,
				builders.CsharpInitParams);
			
			// C# delegate types
			AppendCsharpDelegateType(
				unboxFuncName,
				true,
				type,
				typeKind,
				type,
				unboxParams,
				builders.CsharpDelegateTypes);
			
			// C# init call args
			AppendCsharpInitCallArg(
				unboxFuncName,
				builders.CsharpInitCall);
			
			// C# unbox function
			AppendCsharpFunctionBeginning(
				typeof(object),
				unboxFuncName,
				true,
				TypeKind.Class,
				type,
				unboxParams,
				builders.CsharpFunctions);
			switch (typeKind)
			{
				case TypeKind.Class:
				case TypeKind.ManagedStruct:
					AppendHandleStoreTypeName(
						type,
						builders.CsharpFunctions);
					builders.CsharpFunctions.Append(".Store((");
					AppendCsharpTypeFullName(
						type,
						builders.CsharpFunctions);
					builders.CsharpFunctions.Append(")val);");
					break;
				default:
					builders.CsharpFunctions.Append('(');
					AppendCsharpTypeFullName(
						type,
						builders.CsharpFunctions);
					builders.CsharpFunctions.Append(")val;");
					break;
			}
			AppendCsharpFunctionReturn(
				unboxParams,
				type,
				typeKind,
				null,
				true,
				builders.CsharpFunctions);
			
			// C++ function pointers
			AppendCppFunctionPointerDefinition(
				unboxFuncName,
				true,
				GetTypeName(type),
				typeKind,
				unboxParams,
				type,
				builders.CppFunctionPointers);
			
			// C++ unbox method declaration and definition
			AppendIndent(
				2,
				builders.CppUnboxingMethodDeclarations);
			AppendCppMethodDeclaration(
				unboxMethodDeclarationName,
				false,
				false,
				false,
				null,
				null,
				unboxCppParams,
				builders.CppUnboxingMethodDeclarations);
			int indent = AppendNamespaceBeginning(
				"System",
				builders.CppMethodDefinitions);
			AppendCppMethodDefinitionBegin(
				GetTypeName(typeof(object)),
				null,
				unboxMethodDefinitionName,
				null,
				null,
				unboxCppParams,
				indent,
				builders.CppMethodDefinitions);
			AppendIndent(
				indent,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("{\n");
			AppendIndent(
				indent + 1,
				builders.CppMethodDefinitions);
			AppendCppTypeFullName(
				type,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append(" returnVal(");
			if (typeKind == TypeKind.ManagedStruct)
			{
				builders.CppMethodDefinitions.Append("Plugin::InternalUse::Only, ");
			}
			builders.CppMethodDefinitions.Append("Plugin::");
			builders.CppMethodDefinitions.Append(unboxFuncName);
			builders.CppMethodDefinitions.Append("(Handle));\n");
			AppendCppUnhandledExceptionHandling(
				indent + 1,
				builders.CppMethodDefinitions);
			AppendIndent(
				indent + 1,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("return returnVal;\n");
			AppendIndent(
				indent,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("}\n\t\n");
			
			// C++ method definitions (end)
			AppendCppMethodDefinitionsEnd(
				indent,
				builders.CppMethodDefinitions);
			
			// C++ init params
			AppendCppInitParam(
				unboxFuncNameLower,
				true,
				GetTypeName(type),
				typeKind,
				unboxParams,
				type,
				builders.CppInitParams);
			
			// C++ init body
			AppendCppInitBody(
				unboxFuncName,
				unboxFuncNameLower,
				builders.CppInitBody);
		}
		
		static void AppendCppBoxingMethodNames(
			Type baseType,
			StringBuilder tempBuilder,
			out string boxMethodDefinitionName,
			out string boxMethodDeclarationName)
		{
			tempBuilder.Length = 0;
			tempBuilder.Append("operator ");
			AppendCppTypeFullName(
				baseType,
				tempBuilder);
			boxMethodDefinitionName = tempBuilder.ToString();
			
			tempBuilder.Length = 0;
			tempBuilder.Append("explicit ");
			tempBuilder.Append(boxMethodDefinitionName);
			boxMethodDeclarationName = tempBuilder.ToString();
		}
		
		static void AppendCppBoxingMethodDeclaration(
			string boxMethodDeclarationName,
			ParameterInfo[] boxCppParams,
			int indent,
			StringBuilder output)
		{
			AppendIndent(
				indent,
				output);
			AppendCppMethodDeclaration(
				boxMethodDeclarationName,
				false,
				false,
				false,
				null,
				null,
				boxCppParams,
				output);
		}
		
		static void AppendCppBoxingMethodDefinition(
			Type enclosingType,
			Type[] enclosingTypeTypeParams,
			Type boxedType,
			TypeKind typeKind,
			string boxMethodDefinitionName,
			string boxFuncName,
			ParameterInfo[] boxCppParams,
			int indent,
			StringBuilder output)
		{
			AppendCppMethodDefinitionBegin(
				GetTypeName(enclosingType),
				null,
				boxMethodDefinitionName,
				enclosingTypeTypeParams,
				null,
				boxCppParams,
				indent,
				output);
			AppendIndent(
				indent,
				output);
			output.Append("{\n");
			AppendIndent(
				indent + 1,
				output);
			output.Append("int32_t handle = Plugin::");
			output.Append(boxFuncName);
			output.Append('(');
			if (typeKind == TypeKind.ManagedStruct)
			{
				output.Append("Handle");
			}
			else
			{
				output.Append("*this");
			}
			output.Append(");\n");
			AppendCppUnhandledExceptionHandling(
				indent + 1,
				output);
			AppendIndent(
				indent + 1,
				output);
			output.Append(
				"if (handle)\n");
			AppendIndent(
				indent + 1,
				output);
			output.Append(
				"{\n");
			AppendIndent(
				indent + 2,
				output);
			AppendReferenceManagedHandleFunctionCall(
				GetTypeName(typeof(object)),
				TypeKind.Class,
				null,
				"handle",
				output);
			output.Append(";\n");
			AppendIndent(
				indent + 2,
				output);
			output.Append("return ");
			AppendCppTypeFullName(
				boxedType,
				output);
			output.Append("(Plugin::InternalUse::Only, handle);\n");
			AppendIndent(
				indent + 1,
				output);
			output.Append(
				"}\n");
			AppendIndent(
				indent + 1,
				output);
			output.Append("return nullptr;\n");
			AppendIndent(
				indent,
				output);
			output.Append("}\n");
			AppendIndent(
				indent,
				output);
			output.Append('\n');
		}
		
		static void AppendHandleStoreTypeName(
			Type type,
			StringBuilder output)
		{
			output.Append("NativeScript.Bindings.");
			if (IsManagedValueType(type))
			{
				output.Append("StructStore<");
				AppendCsharpTypeFullName(type, output);
				output.Append('>');
			}
			else
			{
				output.Append("ObjectStore");
			}
		}
		
		static void AppendConstructor(
			string[] paramTypeNames,
			string[] exceptionNames,
			Type enclosingType,
			bool enclosingTypeIsStatic,
			TypeKind enclosingTypeKind,
			Assembly[] assemblies,
			Type[] enclosingTypeParams,
			Type[] genericArgTypes,
			Type[] interfaceTypes,
			int indent,
			StringBuilders builders)
		{
			// Get the constructor's parameters
			ParameterInfo[] parameters;
			if (enclosingType.IsValueType
				&& !enclosingType.IsPrimitive
				&& !enclosingType.IsEnum
				&& paramTypeNames.Length == 0)
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
						paramTypeNames,
						genericArgTypes,
						enclosingTypeParams);
				}
				else
				{
					constructorParamTypeNames = paramTypeNames;
				}
				parameters = GetConstructorParameters(
					enclosingType,
					false,
					constructorParamTypeNames);
			}
			
			Type[] exceptionTypes = GetTypes(
				exceptionNames,
				assemblies);
			
			// Build uppercase function name
			builders.TempStrBuilder.Length = 0;
			AppendNamespace(
				enclosingType.Namespace,
				string.Empty,
				builders.TempStrBuilder);
			AppendTypeNameWithoutGenericSuffix(
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

			TypeName enclosingTypeTypeName = GetTypeName(enclosingType);

			// Build C++ constructor method name
			builders.TempStrBuilder.Length = 0;
			AppendCppTypeName(
				enclosingTypeTypeName,
				builders.TempStrBuilder);
			string cppMethodName = builders.TempStrBuilder.ToString();
			
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
					parameters,
					builders.CsharpFunctions);
				builders.CsharpFunctions.Append("new ");
				AppendCsharpTypeFullName(
					enclosingType,
					builders.CsharpFunctions);
				builders.CsharpFunctions.Append('(');
				AppendCsharpFunctionCallParameters(
					parameters,
					builders.CsharpFunctions);
				builders.CsharpFunctions.Append(");");
				AppendCsharpFunctionReturn(
					parameters,
					enclosingType,
					enclosingTypeKind,
					exceptionTypes,
					true,
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
					parameters,
					builders.CsharpFunctions);
				AppendHandleStoreTypeName(
					enclosingType,
					builders.CsharpFunctions);
				builders.CsharpFunctions.Append(
					".Store(new ");
				AppendCsharpTypeFullName(
					enclosingType,
					builders.CsharpFunctions);
				builders.CsharpFunctions.Append('(');
				AppendCsharpFunctionCallParameters(
					parameters,
					builders.CsharpFunctions);
				builders.CsharpFunctions.Append("));");
				AppendCsharpFunctionReturn(
					parameters,
					typeof(int),
					TypeKind.Primitive,
					exceptionTypes,
					true,
					builders.CsharpFunctions);
			}
			
			// C++ function pointer
			AppendCppFunctionPointerDefinition(
				funcName,
				true,
				enclosingTypeTypeName,
				enclosingTypeKind,
				parameters,
				enclosingType,
				builders.CppFunctionPointers);
			
			// C++ type declaration
			AppendIndent(
				indent + 1,
				builders.CppTypeDefinitions);
			AppendCppMethodDeclaration(
				cppMethodName,
				enclosingTypeIsStatic,
				false,
				false,
				null,
				null,
				parameters,
				builders.CppTypeDefinitions);
			
			// C++ method definition
			AppendCppMethodDefinitionBegin(
				GetTypeName(enclosingType),
				null,
				cppMethodName,
				enclosingTypeParams,
				null,
				parameters,
				indent,
				builders.CppMethodDefinitions);
			if (enclosingTypeKind == TypeKind.Class)
			{
				AppendCppConstructorInitializerList(
					interfaceTypes,
					indent + 1,
					builders.CppMethodDefinitions);
			}
			AppendIndent(
				indent,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("{\n");
			AppendCppPluginFunctionCall(
				true,
				GetTypeName(enclosingType),
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
					GetTypeName(enclosingType),
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
			builders.CppMethodDefinitions.Append('\n');

			// C++ init params
			AppendCppInitParam(
				funcNameLower,
				true,
				GetTypeName(enclosingType),
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
			JsonPropertyGet jsonPropertyGet = jsonProperty.Get;
			if (jsonPropertyGet != null)
			{
				PropertyInfo property = null;
				MethodInfo getMethod;
				if (jsonPropertyGet.ParamTypes != null)
				{
					PropertyInfo[] properties = enclosingType.GetProperties();
					foreach (PropertyInfo curProperty in properties)
					{
						// Name must match
						if (curProperty.Name != jsonProperty.Name)
						{
							continue;
						}
						
						// Must have a get method
						getMethod = curProperty.GetGetMethod();
						if (getMethod == null)
						{
							continue;
						}
						
						// All parameters must match
						if (CheckParametersMatch(
							jsonPropertyGet.ParamTypes,
							getMethod.GetParameters()))
						{
							property = curProperty;
							break;
						}
					}
				}
				else
				{
					property = enclosingType.GetProperty(jsonProperty.Name);
				}
				
				if (property == null)
				{
					builders.TempStrBuilder.Length = 0;
					builders.TempStrBuilder.Append("Property '");
					builders.TempStrBuilder.Append(jsonProperty.Name);
					builders.TempStrBuilder.Append("' not found on ");
					builders.TempStrBuilder.Append(enclosingType);
					throw new Exception(builders.TempStrBuilder.ToString());
				}
				
				getMethod = property.GetGetMethod();
				if (getMethod != null)
				{
					Type propertyType = property.PropertyType;
					TypeKind propertyTypeKind = GetTypeKind(propertyType);
					Type[] exceptionTypes = GetTypes(
						jsonPropertyGet.Exceptions,
						assemblies);
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
						jsonPropertyGet.IsReadOnly,
						enclosingType,
						typeParams,
						propertyType,
						propertyTypeKind,
						indent,
						exceptionTypes,
						builders);
				}
			}
			
			JsonPropertySet jsonPropertySet = jsonProperty.Set;
			if (jsonPropertySet != null)
			{
				PropertyInfo property = null;
				if (jsonPropertySet.ParamTypes != null)
				{
					PropertyInfo[] properties = enclosingType.GetProperties();
					foreach (PropertyInfo curProperty in properties)
					{
						// Name must match
						if (curProperty.Name != jsonProperty.Name)
						{
							continue;
						}
						
						// Must have a set method
						MethodInfo setMethod = curProperty.GetSetMethod();
						if (setMethod == null)
						{
							continue;
						}
						
						// All parameters must match
						if (CheckParametersMatch(
							jsonPropertySet.ParamTypes,
							setMethod.GetParameters()))
						{
							property = curProperty;
							break;
						}
					}
				}
				else
				{
					property = enclosingType.GetProperty(jsonProperty.Name);
				}
				
				if (property == null)
				{
					builders.TempStrBuilder.Length = 0;
					builders.TempStrBuilder.Append("Property '");
					builders.TempStrBuilder.Append(jsonProperty.Name);
					builders.TempStrBuilder.Append("' not found on ");
					builders.TempStrBuilder.Append(enclosingType);
					throw new Exception(builders.TempStrBuilder.ToString());
				}
				
				MethodInfo method = property.GetSetMethod();
				if (method != null)
				{
					Type[] exceptionTypes = GetTypes(
						jsonPropertySet.Exceptions,
						assemblies);
					ParameterInfo[] parameters = ConvertParameters(
						method.GetParameters());
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
						method.IsStatic,
						jsonPropertySet.IsReadOnly,
						enclosingType,
						typeParams,
						indent,
						exceptionTypes,
						builders);
				}
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
			AppendTypeNameWithoutGenericSuffix(
				enclosingType.Name,
				builders.CppTypeDefinitions);
			builders.CppTypeDefinitions.Append("();\n");
			
			AppendIndent(
				indent,
				builders.CppMethodDefinitions);
			AppendTypeNameWithoutGenericSuffix(
				enclosingType.Name,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("::");
			AppendTypeNameWithoutGenericSuffix(
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
				AppendCppTypeFullName(
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
			StringBuilders builders)
		{
			FieldInfo field = enclosingType.GetField(jsonFieldName);
			Type fieldType = OverrideGenericType(
				field.FieldType,
				typeGenericArgumentTypes,
				typeTypeParams);
			TypeKind fieldTypeKind = GetTypeKind(fieldType);
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
				fieldTypeKind,
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
			ParameterInfo[] parameters = { setParam };
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
				indent,
				exceptionTypes,
				builders);
		}
		
		static void AppendEvent(
			JsonEvent jsonEvent,
			Type enclosingType,
			bool enclosingTypeIsStatic,
			TypeKind enclosingTypeKind,
			Type[] typeTypeParams,
			int indent,
			StringBuilders builders)
		{
			EventInfo eventInfo = enclosingType.GetEvent(jsonEvent.Name);
			MethodInfo addMethod = eventInfo.GetAddMethod();
			MethodInfo removeMethod = eventInfo.GetRemoveMethod();
			Type eventType = eventInfo.EventHandlerType;
			string uppercaseEventName = char.ToUpper(jsonEvent.Name[0])
				+ jsonEvent.Name.Substring(1);
			
			ParameterInfo[] addRemoveParams = {
				new ParameterInfo {
					Name = "del",
					ParameterType = eventType,
					DereferencedParameterType = eventType,
					IsOut = false,
					IsRef = false,
					Kind = TypeKind.Class,
					IsVirtual = false
				}
			};
			
			AppendEventAddRemoveMethod(
				jsonEvent.Name,
				uppercaseEventName,
				"Add",
				addMethod.IsStatic,
				enclosingType,
				enclosingTypeKind,
				enclosingTypeIsStatic,
				typeTypeParams,
				addRemoveParams,
				indent,
				builders);
			AppendEventAddRemoveMethod(
				jsonEvent.Name,
				uppercaseEventName,
				"Remove",
				removeMethod.IsStatic,
				enclosingType,
				enclosingTypeKind,
				enclosingTypeIsStatic,
				typeTypeParams,
				addRemoveParams,
				indent,
				builders);
		}
		
		static void AppendEventAddRemoveMethod(
			string eventName,
			string uppercaseEventName,
			string operation,
			bool methodIsStatic,
			Type enclosingType,
			TypeKind enclosingTypeKind,
			bool enclosingTypeIsStatic,
			Type[] typeTypeParams,
			ParameterInfo[] methodParams,
			int indent,
			StringBuilders builders)
		{
			builders.TempStrBuilder.Length = 0;
			AppendNamespace(
				enclosingType.Namespace,
				string.Empty,
				builders.TempStrBuilder);
			AppendTypeNameWithoutGenericSuffix(
				enclosingType.Name,
				builders.TempStrBuilder);
			AppendTypeNames(
				typeTypeParams,
				builders.TempStrBuilder);
			builders.TempStrBuilder.Append(operation);
			builders.TempStrBuilder.Append("Event");
			builders.TempStrBuilder.Append(uppercaseEventName);
			string funcName = builders.TempStrBuilder.ToString();
			
			builders.TempStrBuilder[0] = char.ToLower(
				builders.TempStrBuilder[0]);
			string funcNameLower = builders.TempStrBuilder.ToString();
			
			builders.TempStrBuilder.Length = 0;
			builders.TempStrBuilder.Append(operation);
			builders.TempStrBuilder.Append(uppercaseEventName);
			string methodName = builders.TempStrBuilder.ToString();
			
			// C# init param
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
				methodParams,
				builders.CsharpDelegateTypes);
			
			// C# init call arg
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
				methodParams,
				builders.CsharpFunctions);
			AppendCsharpFunctionCallSubject(
				enclosingType,
				methodIsStatic,
				builders.CsharpFunctions);
			builders.CsharpFunctions.Append('.');
			builders.CsharpFunctions.Append(eventName);
			builders.CsharpFunctions.Append(" += del;");
			AppendCsharpFunctionEnd(
				typeof(void),
				null,
				methodParams,
				builders.CsharpFunctions);
			
			// C++ function pointer
			AppendCppFunctionPointerDefinition(
				funcName,
				methodIsStatic,
				GetTypeName(enclosingType),
				enclosingTypeKind,
				methodParams,
				typeof(void),
				builders.CppFunctionPointers);
			
			// C++ method declaration
			Type cppReturnType = typeof(void);
			string cppMethodName = methodName;
			bool cppMethodIsStatic = methodIsStatic;
			ParameterInfo[] cppParameters = methodParams;
			ParameterInfo[] cppCallParameters = methodParams;
			AppendIndent(
				indent + 1,
				builders.CppTypeDefinitions);
			AppendCppMethodDeclaration(
				cppMethodName,
				enclosingTypeIsStatic,
				false,
				cppMethodIsStatic,
				cppReturnType,
				null,
				cppParameters,
				builders.CppTypeDefinitions);
			
			// C++ method definition
			AppendCppMethodDefinitionBegin(
				GetTypeName(enclosingType),
				cppReturnType,
				cppMethodName,
				typeTypeParams,
				null,
				cppParameters,
				indent,
				builders.CppMethodDefinitions);
			AppendIndent(
				indent,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("{\n");
			AppendCppPluginFunctionCall(
				methodIsStatic,
				GetTypeName(enclosingType),
				enclosingTypeKind,
				typeTypeParams,
				typeof(void),
				funcName,
				cppCallParameters,
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
				GetTypeName(enclosingType),
				enclosingTypeKind,
				methodParams,
				typeof(void),
				builders.CppInitParams);
			
			// C++ init body
			AppendCppInitBody(
				funcName,
				funcNameLower,
				builders.CppInitBody);
		}
		
		static MethodInfo GetMethod(
			JsonMethod jsonMethod,
			Type enclosingType,
			Type[] typeTypeParams,
			Type[] genericArgTypes,
			MethodInfo[] methods,
			string[] methodGenericTypeNames)
		{
			// Map convenience method names to actual method names
			switch (jsonMethod.Name)
			{
				case "+x":
					jsonMethod.Name = "op_UnaryPlus";
					break;
				case "-x":
					jsonMethod.Name = "op_UnaryNegation";
					break;
				case "!x":
					jsonMethod.Name = "op_LogicalNot";
					break;
				case "~x":
					jsonMethod.Name = "op_OnesComplement";
					break;
				case "x++":
					jsonMethod.Name = "op_Increment";
					break;
				case "x--":
					jsonMethod.Name = "op_Decrement";
					break;
				case "(true)x":
					jsonMethod.Name = "op_True";
					break;
				case "(false)x":
					jsonMethod.Name = "op_False";
					break;
				case "implicit":
					jsonMethod.Name = "op_Implicit";
					break;
				case "explicit":
					jsonMethod.Name = "op_Explicit";
					break;
				case "x+y":
					jsonMethod.Name = "op_Addition";
					break;
				case "x-y":
					jsonMethod.Name = "op_Subtraction";
					break;
				case "x*y":
					jsonMethod.Name = "op_Multiply";
					break;
				case "x/y":
					jsonMethod.Name = "op_Division";
					break;
				case "x%y":
					jsonMethod.Name = "op_Modulus";
					break;
				case "x&y":
					jsonMethod.Name = "op_BitwiseAnd";
					break;
				case "x|y":
					jsonMethod.Name = "op_BitwiseOr";
					break;
				case "x^y":
					jsonMethod.Name = "op_ExclusiveOr";
					break;
				case "x<<y":
					jsonMethod.Name = "op_LeftShift";
					break;
				case "x>>y":
					jsonMethod.Name = "op_RightShift";
					break;
				case "x==y":
					jsonMethod.Name = "op_Equality";
					break;
				case "x!=y":
					jsonMethod.Name = "op_Inequality";
					break;
				case "x<y":
					jsonMethod.Name = "op_LessThan";
					break;
				case "x>y":
					jsonMethod.Name = "op_GreaterThan";
					break;
				case "x<=y":
					jsonMethod.Name = "op_LessThanOrEqual";
					break;
				case "x>=y":
					jsonMethod.Name = "op_GreaterThanOrEqual";
					break;
			}
			
			if (enclosingType.IsGenericType)
			{
				string[] overriddenParamTypeNames = OverrideGenericTypeNames(
					jsonMethod.ParamTypes,
					genericArgTypes,
					typeTypeParams);
				return GetMethod(
					enclosingType,
					methods,
					jsonMethod.Name,
					overriddenParamTypeNames,
					methodGenericTypeNames);
			}
			else
			{
				return GetMethod(
					enclosingType,
					methods,
					jsonMethod.Name,
					jsonMethod.ParamTypes,
					methodGenericTypeNames);
			}
		}
		
		static void AppendMethod(
			JsonMethod jsonMethod,
			Assembly[] assemblies,
			Type enclosingType,
			bool enclosingTypeIsStatic,
			TypeKind enclosingTypeKind,
			MethodInfo[] methods,
			Type[] typeTypeParams,
			Type[] genericArgTypes,
			int indent,
			StringBuilders builders)
		{
			Type[] exceptionTypes = GetTypes(
				jsonMethod.Exceptions,
				assemblies);
			
			if (jsonMethod.GenericParams != null)
			{
				// Generate for each set of generic types
				bool generateDeclaration = true;
				foreach (JsonGenericParams jsonGenericParams
					in jsonMethod.GenericParams)
				{
					MethodInfo method = GetMethod(
						jsonMethod,
						enclosingType,
						typeTypeParams,
						genericArgTypes,
						methods,
						jsonGenericParams.Types);
					Type[] methodTypeParams = GetTypes(
						jsonGenericParams.Types,
						assemblies);
					method = method.MakeGenericMethod(methodTypeParams);
					ParameterInfo[] parameters = ConvertParameters(
						method.GetParameters());
					Type returnType = method.ReturnType;
					TypeKind returnTypeKind = GetTypeKind(returnType);
					AppendMethod(
						enclosingType,
						method.Name,
						enclosingTypeIsStatic,
						enclosingTypeKind,
						method.IsStatic,
						jsonMethod.IsReadOnly,
						returnType,
						returnTypeKind,
						typeTypeParams,
						methodTypeParams,
						parameters,
						generateDeclaration,
						indent,
						exceptionTypes,
						builders);
					generateDeclaration = false;
				}
			}
			else
			{
				MethodInfo method = GetMethod(
					jsonMethod,
					enclosingType,
					typeTypeParams,
					genericArgTypes,
					methods,
					null);
				ParameterInfo[] parameters = ConvertParameters(
					method.GetParameters());
				Type returnType = method.ReturnType;
				TypeKind returnTypeKind = GetTypeKind(returnType);
				AppendMethod(
					enclosingType,
					method.Name,
					enclosingTypeIsStatic,
					enclosingTypeKind,
					method.IsStatic,
					jsonMethod.IsReadOnly,
					returnType,
					returnTypeKind,
					typeTypeParams,
					null,
					parameters,
					true,
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
					if (genericType == genericArgumentTypes[i])
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
			foreach (ParameterInfo info in parameters)
			{
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
				foreach (Type genericArgType in genericArgTypes)
				{
					if (CheckTypeNameMatches(
						typeName,
						genericArgType))
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
			string methodName,
			bool enclosingTypeIsStatic,
			TypeKind enclosingTypeKind,
			bool methodIsStatic,
			bool isReadOnly,
			Type returnType,
			TypeKind returnTypeKind,
			Type[] enclosingTypeParams,
			Type[] methodTypeParams,
			ParameterInfo[] parameters,
			bool generateDeclaration,
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
			AppendTypeNameWithoutGenericSuffix(
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
				parameters,
				builders.CsharpFunctions);
			if (methodName.StartsWith("op_"))
			{
				string op;
				switch (methodName)
				{
					case "op_UnaryPlus":
						op = "+";
						break;
					case "op_UnaryNegation":
						op = "-";
						break;
					case "op_LogicalNot":
						op = "!";
						break;
					case "op_OnesComplement":
						op = "~";
						break;
					case "op_Increment":
						op = "++";
						break;
					case "op_Decrement":
						op = "--";
						break;
					case "op_Implicit":
						op = string.Empty;
						break;
					case "op_Explicit":
						builders.TempStrBuilder.Length = 0;
						builders.TempStrBuilder.Append('(');
						AppendTypeNameWithoutGenericSuffix(
							returnType.Name,
							builders.TempStrBuilder);
						builders.TempStrBuilder.Append(')');
						op = builders.TempStrBuilder.ToString();
						break;
					case "op_True":
						op = "(true)";
						break;
					case "op_False":
						op = "(false)";
						break;
					case "op_Addition":
						op = "+";
						break;
					case "op_Subtraction":
						op = "-";
						break;
					case "op_Multiply":
						op = "*";
						break;
					case "op_Division":
						op = "/";
						break;
					case "op_Modulus":
						op = "%";
						break;
					case "op_BitwiseAnd":
						op = "&";
						break;
					case "op_BitwiseOr":
						op = "|";
						break;
					case "op_ExclusiveOr":
						op = "^";
						break;
					case "op_LeftShift":
						op = "<<";
						break;
					case "op_RightShift":
						op = ">>";
						break;
					case "op_Equality":
						op = "==";
						break;
					case "op_Inequality":
						op = "!=";
						break;
					case "op_LessThan":
						op = "<";
						break;
					case "op_GreaterThan":
						op = ">";
						break;
					case "op_LessThanOrEqual":
						op = "<=";
						break;
					case "op_GreaterThanOrEqual":
						op = ">=";
						break;
					default:
						throw new Exception(
							"Unsupported overloaded operator: " + methodName);
				}
				switch (parameters.Length)
				{
					case 1:
						builders.CsharpFunctions.Append(op);
						builders.CsharpFunctions.Append(parameters[0].Name);
						break;
					case 2:
						builders.CsharpFunctions.Append(parameters[0].Name);
						builders.CsharpFunctions.Append(' ');
						builders.CsharpFunctions.Append(op);
						builders.CsharpFunctions.Append(' ');
						builders.CsharpFunctions.Append(parameters[1].Name);
						break;
					default:
						throw new Exception(
							"Unsupported number of overloaded operator params: "
							+ parameters.Length);
				}
			}
			else
			{
				AppendCsharpFunctionCallSubject(
					enclosingType,
					methodIsStatic,
					builders.CsharpFunctions);
				builders.CsharpFunctions.Append('.');
				builders.CsharpFunctions.Append(methodName);
				AppendCSharpTypeParameters(
					methodTypeParams,
					builders.CsharpFunctions);
				builders.CsharpFunctions.Append('(');
				AppendCsharpFunctionCallParameters(
					parameters,
					builders.CsharpFunctions);
				builders.CsharpFunctions.Append(')');
			}
			builders.CsharpFunctions.Append(';');
			if (!isReadOnly
				&& !methodIsStatic
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
				returnTypeKind,
				exceptionTypes,
				false,
				builders.CsharpFunctions);
			
			// C++ function pointer
			AppendCppFunctionPointerDefinition(
				funcName,
				methodIsStatic,
				GetTypeName(enclosingType),
				enclosingTypeKind,
				parameters,
				returnType,
				builders.CppFunctionPointers);
			
			// C++ method declaration
			string cppMethodName;
			bool cppMethodIsStatic;
			ParameterInfo[] cppParameters;
			ParameterInfo[] cppCallParameters;
			Type cppReturnType = returnType;
			if (methodName.StartsWith("op_"))
			{
				switch (methodName)
				{
					case "op_UnaryPlus":
						cppMethodName = "operator+";
						break;
					case "op_UnaryNegation":
						cppMethodName = "operator-";
						break;
					case "op_LogicalNot":
						cppMethodName = "operator!";
						break;
					case "op_OnesComplement":
						cppMethodName = "operator~";
						break;
					case "op_Increment":
						cppMethodName = "operator++";
						break;
					case "op_Decrement":
						cppMethodName = "operator--";
						break;
					case "op_Implicit":
						builders.TempStrBuilder.Length = 0;
						builders.TempStrBuilder.Append("operator ");
						AppendCppTypeFullName(
							returnType,
							builders.TempStrBuilder);
						cppMethodName = builders.TempStrBuilder.ToString();
						cppReturnType = null;
						break;
					case "op_Explicit":
						builders.TempStrBuilder.Length = 0;
						builders.TempStrBuilder.Append("explicit operator ");
						AppendCppTypeFullName(
							returnType,
							builders.TempStrBuilder);
						cppMethodName = builders.TempStrBuilder.ToString();
						cppReturnType = null;
						break;
					case "op_True":
						cppMethodName = "TrueOperator";
						break;
					case "op_False":
						cppMethodName = "FalseOperator";
						break;
					case "op_Addition":
						cppMethodName = "operator+";
						break;
					case "op_Subtraction":
						cppMethodName = "operator-";
						break;
					case "op_Multiply":
						cppMethodName = "operator*";
						break;
					case "op_Division":
						cppMethodName = "operator/";
						break;
					case "op_Modulus":
						cppMethodName = "operator%";
						break;
					case "op_BitwiseAnd":
						cppMethodName = "operator&";
						break;
					case "op_BitwiseOr":
						cppMethodName = "operator|";
						break;
					case "op_ExclusiveOr":
						cppMethodName = "operator^";
						break;
					case "op_LeftShift":
						cppMethodName = "operator<<";
						break;
					case "op_RightShift":
						cppMethodName = "operator>>";
						break;
					case "op_Equality":
						cppMethodName = "operator==";
						break;
					case "op_Inequality":
						cppMethodName = "operator!=";
						break;
					case "op_LessThan":
						cppMethodName = "operator<";
						break;
					case "op_GreaterThan":
						cppMethodName = "operator>";
						break;
					case "op_LessThanOrEqual":
						cppMethodName = "operator<=";
						break;
					case "op_GreaterThanOrEqual":
						cppMethodName = "operator>=";
						break;
					default:
						throw new Exception(
							"Unsupported overloaded operator: " + methodName);
				}
				cppMethodIsStatic = false;
				ParameterInfo thisParam;
				switch (enclosingTypeKind)
				{
					case TypeKind.Class:
					case TypeKind.ManagedStruct:
						thisParam = new ParameterInfo{
							Name = "Handle",
							ParameterType = typeof(int),
							DereferencedParameterType = typeof(int),
							IsOut = false,
							IsRef = false,
							Kind = TypeKind.Primitive
						};
						break;
					default:
						thisParam = new ParameterInfo{
							Name = "*this",
							ParameterType = enclosingType,
							DereferencedParameterType = enclosingType,
							IsOut = false,
							IsRef = false,
							Kind = TypeKind.Primitive
						};
						break;
				}
				switch (parameters.Length)
				{
					case 1:
						cppParameters = new ParameterInfo[0];
						cppCallParameters = new [] {
							thisParam };
						break;
					case 2:
						cppParameters = new [] {
							parameters[0] };
						cppCallParameters = new [] {
							thisParam,
							parameters[0]
						};
						break;
					default:
						throw new Exception(
							"Unsupported number of overloaded operator parameters: "
							+ parameters.Length);
				}
			}
			else
			{
				cppMethodName = methodName;
				cppMethodIsStatic = methodIsStatic;
				cppParameters = parameters;
				cppCallParameters = parameters;
			}
			if (generateDeclaration)
			{
				AppendIndent(
					indent + 1,
					builders.CppTypeDefinitions);
				AppendCppMethodDeclaration(
					cppMethodName,
					enclosingTypeIsStatic,
					false,
					cppMethodIsStatic,
					cppReturnType,
					methodTypeParams,
					cppParameters,
					builders.CppTypeDefinitions);
			}
			
			// C++ method definition
			AppendCppMethodDefinitionBegin(
				GetTypeName(enclosingType),
				cppReturnType,
				cppMethodName,
				enclosingTypeParams,
				methodTypeParams,
				cppParameters,
				indent,
				builders.CppMethodDefinitions);
			AppendIndent(
				indent,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("{\n");
			AppendCppPluginFunctionCall(
				methodIsStatic,
				GetTypeName(enclosingType),
				enclosingTypeKind,
				enclosingTypeParams,
				returnType,
				funcName,
				cppCallParameters,
				indent + 1,
				builders.CppMethodDefinitions);
			AppendCppMethodReturn(
				returnType,
				returnTypeKind,
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
				GetTypeName(enclosingType),
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
					AppendCsharpTypeFullName(typeParam, output);
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
					AppendCppTypeFullName(typeParam, output);
					if (i != typeParams.Length - 1)
					{
						output.Append(", ");
					}
				}
				output.Append('>');
			}
		}

		static void AppendCppFunctionCall(
			string funcName,
			ParameterInfo[] parameters,
			Type returnType,
			bool enclosingTypeIsStatic,
			int indent,
			StringBuilder output)
		{
			foreach (ParameterInfo param in parameters)
			{
				if (param.Kind == TypeKind.Class
				    || param.Kind == TypeKind.ManagedStruct)
				{
					AppendIndent(
						indent,
						output);
					output.Append("int ");
					output.Append(param.Name);
					output.Append("Handle = ");
					AppendHandleStoreTypeName(
						param.DereferencedParameterType,
						output);
					output.Append('.');
					if (param.Kind == TypeKind.Class)
					{
						output.Append("GetHandle");
					}
					else
					{
						output.Append("Store");
					}
					output.Append('(');
					output.Append(param.Name);
					output.Append(");\n");
				}
			}
			if (!enclosingTypeIsStatic)
			{
				AppendIndent(
					indent,
					output);
				output.Append(
					"int thisHandle = NativeScript.Bindings.ObjectStore.GetHandle(this);\n");
			}
			AppendIndent(
				indent,
				output);
			if (returnType != typeof(void))
			{
				output.Append("var returnVal = ");
			}
			output.Append("NativeScript.Bindings.");
			output.Append(funcName);
			output.Append('(');
			if (!enclosingTypeIsStatic)
			{
				output.Append("thisHandle");
				if (parameters.Length > 0)
				{
					output.Append(", ");
				}
			}
			for (int i = 0; i < parameters.Length; ++i)
			{
				ParameterInfo param = parameters[i];
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
			output.Append(");\n");
			AppendIndent(
				indent,
				output);
			output.Append("if (NativeScript.Bindings.UnhandledCppException != null)\n");
			AppendIndent(
				indent,
				output);
			output.Append("{\n");
			AppendIndent(
				indent + 1,
				output);
			output.Append("Exception ex = NativeScript.Bindings.UnhandledCppException;\n");
			AppendIndent(
				indent + 1,
				output);
			output.Append("NativeScript.Bindings.UnhandledCppException = null;\n");
			AppendIndent(
				indent + 1,
				output);
			output.Append("throw ex;\n");
			AppendIndent(
				indent,
				output);
			output.Append("}\n");
		}
		
		static void AppendArray(
			JsonArray jsonArray,
			Assembly[] assemblies,
			StringBuilders builders)
		{
			// Get element type
			Type elementType = GetType(
				jsonArray.Type,
				assemblies);
			TypeKind elementTypeKind = GetTypeKind(elementType);
			
			// Default ranks to just 1
			int[] ranks;
			if (jsonArray.Ranks == null
				|| jsonArray.Ranks.Length == 0)
			{
				ranks = new[]{ 1 };
			}
			else
			{
				ranks = jsonArray.Ranks;
			}
			
			// C++ element proxy for [1-R] for all ranks R
			Type[] cppTypeParams = { elementType };
			foreach (int rank in ranks)
			{
				// Build array name
				builders.TempStrBuilder.Length = 0;
				AppendCppArrayTypeName(
					rank,
					builders.TempStrBuilder);
				string cppArrayTypeName = builders.TempStrBuilder.ToString();
				
				for (int i = 1; i <= rank; ++i)
				{
					AppendArrayElementProxy(
						elementType,
						elementTypeKind,
						i,
						rank,
						cppTypeParams,
						cppArrayTypeName,
						builders);
				}
			}
			
			foreach (int rank in ranks)
			{
				// Build array name
				builders.TempStrBuilder.Length = 0;
				AppendCppArrayTypeName(
					rank,
					builders.TempStrBuilder);
				string cppArrayTypeName = builders.TempStrBuilder.ToString();
				
				// Build array name with element type
				builders.TempStrBuilder.Append('<');
				AppendCppTypeFullName(
					elementType,
					builders.TempStrBuilder);
				builders.TempStrBuilder.Append('>');
				string cppGenericArrayTypeName = builders.TempStrBuilder.ToString();
				
				// Build element proxy name
				builders.TempStrBuilder.Length = 0;
				AppendCppArrayElementProxyName(
					1,
					rank,
					elementType,
					builders.TempStrBuilder);
				string cppElementProxyTypeName = builders.TempStrBuilder.ToString();
				
				// Build "TypeArray" name
				builders.TempStrBuilder.Length = 0;
				AppendNamespace(
					elementType.Namespace,
					string.Empty,
					builders.TempStrBuilder);
				AppendTypeNameWithoutGenericSuffix(
					elementType.Name,
					builders.TempStrBuilder);
				builders.TempStrBuilder.Append(cppArrayTypeName);
				string bindingArrayTypeName = builders.TempStrBuilder.ToString();
				
				// MakeArrayType() creates a Type for a "vector"
				// MakeArrayType(int) creates a Type for a multi-dimensional array
				// Use MakeArrayType() instead of MakeArrayType(1) to create a vector
				// instead of a multi-dimensional array with one dimension.
				// This avoids problems like the name being "float[*]", which is
				// invalid C# code.
				Type arrayType;
				if (rank == 1)
				{
					arrayType = elementType.MakeArrayType();
				}
				else
				{
					arrayType = elementType.MakeArrayType(rank);
				}
				
				// C++ type declaration
				int indent = AppendCppTypeDeclaration(
					GetTypeName(cppArrayTypeName, "System"),
					false,
					cppTypeParams,
					cppTypeParams != null ?
						builders.CppTemplateSpecializationDeclarations :
						builders.CppTypeDeclarations);
				
				// C++ type definition (beginning)
				Type[] interfaceTypes = GetDirectInterfaces(arrayType);
				AppendCppTypeDefinitionBegin(
					GetTypeName(cppArrayTypeName, "System"),
					TypeKind.Class,
					cppTypeParams,
					GetTypeName("Array", "System"),
					null,
					interfaceTypes,
					false,
					indent,
					builders.CppTypeDefinitions);
				
				// C++ method definitions (beginning)
				Type[] cppCtorInitTypes = GetCppCtorInitTypes(
					arrayType,
					false);
				int cppMethodDefinitionsIndent = AppendCppMethodDefinitionsBegin(
					GetTypeName(cppArrayTypeName, "System"),
					TypeKind.Class,
					cppTypeParams,
					cppCtorInitTypes,
					false,
					(extraIndent, subject) => {
						AppendIndent(
							extraIndent,
							builders.CppMethodDefinitions);
						builders.CppMethodDefinitions.Append(subject);
						builders.CppMethodDefinitions.Append(
							"InternalLength = 0;\n");
						if (rank > 1)
						{
							for (int i = 0; i < rank; ++i)
							{
								AppendIndent(
									extraIndent,
									builders.CppMethodDefinitions);
								builders.CppMethodDefinitions.Append(subject);
								builders.CppMethodDefinitions.Append(
									"InternalLengths[");
								builders.CppMethodDefinitions.Append(i);
								builders.CppMethodDefinitions.Append(
									"] = 0;\n");
							}
						}
					},
					(extraIndent, subject) => {
						AppendIndent(
							extraIndent,
							builders.CppMethodDefinitions);
						builders.CppMethodDefinitions.Append(
							"InternalLength = ");
						builders.CppMethodDefinitions.Append(subject);
						builders.CppMethodDefinitions.Append(
							"InternalLength;\n");
						if (rank > 1)
						{
							for (int i = 0; i < rank; ++i)
							{
								AppendIndent(
									extraIndent,
									builders.CppMethodDefinitions);
								builders.CppMethodDefinitions.Append(
									"InternalLengths[");
								builders.CppMethodDefinitions.Append(i);
								builders.CppMethodDefinitions.Append(
									"] = ");
								builders.CppMethodDefinitions.Append(subject);
								builders.CppMethodDefinitions.Append(
									"InternalLengths[");
								builders.CppMethodDefinitions.Append(i);
								builders.CppMethodDefinitions.Append(
									"];\n");
							}
						}
					},
					indent,
					builders.CppMethodDefinitions);
				
				// C++ fields
				AppendIndent(
					indent + 1,
					builders.CppTypeDefinitions);
				builders.CppTypeDefinitions.Append(
					"int32_t InternalLength;\n");
				if (rank > 1)
				{
					AppendIndent(
						indent + 1,
						builders.CppTypeDefinitions);
					builders.CppTypeDefinitions.Append(
						"int32_t InternalLengths[");
					builders.CppTypeDefinitions.Append(rank);
					builders.CppTypeDefinitions.Append("];\n");
				}
				
				AppendArrayConstructor(
					elementType,
					arrayType,
					cppArrayTypeName,
					rank,
					bindingArrayTypeName,
					cppCtorInitTypes,
					indent,
					builders);
				
				// Base GetLength
				AppendArrayCppGetLengthFunction(
					indent,
					cppArrayTypeName,
					cppTypeParams,
					builders);
				
				// GetLength for multi-dimensional arrays
				if (rank > 1)
				{
					AppendArrayMultidimensionalGetLength(
						elementType,
						arrayType,
						cppArrayTypeName,
						rank,
						bindingArrayTypeName,
						indent,
						builders);
				}
				
				AppendArrayCppGetRankFunction(
					indent,
					cppArrayTypeName,
					cppTypeParams,
					rank,
					builders);
				
				AppendArrayGetItem(
					elementType,
					elementTypeKind,
					arrayType,
					cppArrayTypeName,
					rank,
					builders);
				
				AppendArraySetItem(
					elementType,
					arrayType,
					cppArrayTypeName,
					rank,
					builders);
				
				// C++ operator[] method declaration
				AppendIndent(
					indent + 1,
					builders.CppTypeDefinitions);
				builders.CppTypeDefinitions.Append("Plugin::");
				AppendCppArrayElementProxyName(
					1,
					rank,
					elementType,
					builders.CppTypeDefinitions);
				builders.CppTypeDefinitions.Append(' ');
				AppendTypeNameWithoutGenericSuffix(
					"operator[]",
					builders.CppTypeDefinitions);
				builders.CppTypeDefinitions.Append("(int32_t index);\n");
				
				// C++ operator[] method definition
				AppendCppArrayIndexOperatorMethodDefinition(
					0,
					cppMethodDefinitionsIndent,
					GetTypeName(cppGenericArrayTypeName, "System"),
					cppElementProxyTypeName,
					builders.CppMethodDefinitions);
				
				// C++ type definition (end)
				AppendCppTypeDefinitionEnd(
					false,
					indent,
					builders.CppTypeDefinitions);
				
				// C++ method definitions (ending)
				AppendCppMethodDefinitionsEnd(
					cppMethodDefinitionsIndent,
					builders.CppMethodDefinitions);
				
				if (rank == 1)
				{
					AppendArrayIterator(
						elementType,
						cppGenericArrayTypeName,
						bindingArrayTypeName,
						builders.CppTypeDefinitions,
						builders.CppMethodDefinitions);
				}
			}
		}
		
		static void AppendArrayIterator(
			Type elementType,
			string cppGenericArrayTypeName,
			string bindingArrayTypeName,
			StringBuilder cppTypeDefinitions,
			StringBuilder cppMethodDefinitions)
		{
			// Iterator type definition
			cppTypeDefinitions.Append("namespace Plugin\n");
			cppTypeDefinitions.Append("{\n");
			cppTypeDefinitions.Append("\tstruct ");
			cppTypeDefinitions.Append(bindingArrayTypeName);
			cppTypeDefinitions.Append("Iterator\n");
			cppTypeDefinitions.Append("\t{\n");
			cppTypeDefinitions.Append("\t\tSystem::");
			cppTypeDefinitions.Append(cppGenericArrayTypeName);
			cppTypeDefinitions.Append("& array;\n");
			cppTypeDefinitions.Append("\t\tint index;\n");
			cppTypeDefinitions.Append("\t\t");
			cppTypeDefinitions.Append(bindingArrayTypeName);
			cppTypeDefinitions.Append("Iterator(System::");
			cppTypeDefinitions.Append(cppGenericArrayTypeName);
			cppTypeDefinitions.Append("& array, int32_t index);\n");
			cppTypeDefinitions.Append("\t\t");
			cppTypeDefinitions.Append(bindingArrayTypeName);
			cppTypeDefinitions.Append("Iterator& operator++();\n");
			cppTypeDefinitions.Append("\t\tbool operator!=(const ");
			cppTypeDefinitions.Append(bindingArrayTypeName);
			cppTypeDefinitions.Append("Iterator& other);\n");
			cppTypeDefinitions.Append("\t\t");
			AppendCppTypeFullName(
				elementType,
				cppTypeDefinitions);
			cppTypeDefinitions.Append(" operator*();\n");
			cppTypeDefinitions.Append("\t};\n");
			cppTypeDefinitions.Append("}\n");
			cppTypeDefinitions.Append('\n');
			
			// begin() and end() declarations
			cppTypeDefinitions.Append("namespace System\n");
			cppTypeDefinitions.Append("{\n");
			cppTypeDefinitions.Append("\tPlugin::");
			cppTypeDefinitions.Append(bindingArrayTypeName);
			cppTypeDefinitions.Append("Iterator begin(System::");
			cppTypeDefinitions.Append(cppGenericArrayTypeName);
			cppTypeDefinitions.Append("& array);\n");
			cppTypeDefinitions.Append("\tPlugin::");
			cppTypeDefinitions.Append(bindingArrayTypeName);
			cppTypeDefinitions.Append("Iterator end(System::");
			cppTypeDefinitions.Append(cppGenericArrayTypeName);
			cppTypeDefinitions.Append("& array);\n");
			cppTypeDefinitions.Append("}\n");
			cppTypeDefinitions.Append('\n');
			
			// Iterator method definitions
			cppMethodDefinitions.Append("namespace Plugin\n");
			cppMethodDefinitions.Append("{\n");
			cppMethodDefinitions.Append('\t');
			cppMethodDefinitions.Append(bindingArrayTypeName);
			cppMethodDefinitions.Append("Iterator::");
			cppMethodDefinitions.Append(bindingArrayTypeName);
			cppMethodDefinitions.Append("Iterator(System::");
			cppMethodDefinitions.Append(cppGenericArrayTypeName);
			cppMethodDefinitions.Append("& array, int32_t index)\n");
			cppMethodDefinitions.Append("\t\t: array(array)\n");
			cppMethodDefinitions.Append("\t\t, index(index)\n");
			cppMethodDefinitions.Append("\t{\n");
			cppMethodDefinitions.Append("\t}\n");
			cppMethodDefinitions.Append("\t\n");
			cppMethodDefinitions.Append('\t');
			cppMethodDefinitions.Append(bindingArrayTypeName);
			cppMethodDefinitions.Append("Iterator& ");
			cppMethodDefinitions.Append(bindingArrayTypeName);
			cppMethodDefinitions.Append("Iterator::");
			cppMethodDefinitions.Append("operator++()\n");
			cppMethodDefinitions.Append("\t{\n");
			cppMethodDefinitions.Append("\t\tindex++;\n");
			cppMethodDefinitions.Append("\t\treturn *this;\n");
			cppMethodDefinitions.Append("\t}\n");
			cppMethodDefinitions.Append("\t\n");
			cppMethodDefinitions.Append("\tbool ");
			cppMethodDefinitions.Append(bindingArrayTypeName);
			cppMethodDefinitions.Append("Iterator::");
			cppMethodDefinitions.Append("operator!=(const ");
			cppMethodDefinitions.Append(bindingArrayTypeName);
			cppMethodDefinitions.Append("Iterator& other)\n");
			cppMethodDefinitions.Append("\t{\n");
			cppMethodDefinitions.Append("\t\treturn index != other.index;\n");
			cppMethodDefinitions.Append("\t}\n");
			cppMethodDefinitions.Append("\t\n");
			cppMethodDefinitions.Append('\t');
			AppendCppTypeFullName(
				elementType,
				cppMethodDefinitions);
			cppMethodDefinitions.Append(' ');
			cppMethodDefinitions.Append(bindingArrayTypeName);
			cppMethodDefinitions.Append("Iterator::");
			cppMethodDefinitions.Append("operator*()\n");
			cppMethodDefinitions.Append("\t{\n");
			cppMethodDefinitions.Append("\t\treturn array[index];\n");
			cppMethodDefinitions.Append("\t}\n");
			cppMethodDefinitions.Append("}\n");
			cppMethodDefinitions.Append('\n');
			
			// begin() and end() definitions
			cppMethodDefinitions.Append("namespace System\n");
			cppMethodDefinitions.Append("{\n");
			cppMethodDefinitions.Append("\tPlugin::");
			cppMethodDefinitions.Append(bindingArrayTypeName);
			cppMethodDefinitions.Append("Iterator begin(System::");
			cppMethodDefinitions.Append(cppGenericArrayTypeName);
			cppMethodDefinitions.Append("& array)\n");
			cppMethodDefinitions.Append("\t{\n");
			cppMethodDefinitions.Append("\t\treturn Plugin::");
			cppMethodDefinitions.Append(bindingArrayTypeName);
			cppMethodDefinitions.Append("Iterator(array, 0);\n");
			cppMethodDefinitions.Append("\t}\n");
			cppMethodDefinitions.Append("\t\n");
			cppMethodDefinitions.Append("\tPlugin::");
			cppMethodDefinitions.Append(bindingArrayTypeName);
			cppMethodDefinitions.Append("Iterator end(System::");
			cppMethodDefinitions.Append(cppGenericArrayTypeName);
			cppMethodDefinitions.Append("& array)\n");
			cppMethodDefinitions.Append("\t{\n");
			cppMethodDefinitions.Append("\t\treturn Plugin::");
			cppMethodDefinitions.Append(bindingArrayTypeName);
			cppMethodDefinitions.Append("Iterator(array, array.GetLength() - 1);\n");
			cppMethodDefinitions.Append("\t}\n");
			cppMethodDefinitions.Append("}\n");
			cppMethodDefinitions.Append('\n');
		}
		
		static void AppendGenericEnumerableIterator(
			Type enumerableType,
			Type enumeratorType,
			Type elementType,
			string bindingEnumerableTypeName,
			StringBuilder cppTypeDefinitions,
			StringBuilder cppMethodDefinitions)
		{
			// Iterator type definition
			cppTypeDefinitions.Append("namespace Plugin\n");
			cppTypeDefinitions.Append("{\n");
			cppTypeDefinitions.Append("\tstruct ");
			cppTypeDefinitions.Append(bindingEnumerableTypeName);
			cppTypeDefinitions.Append("Iterator\n");
			cppTypeDefinitions.Append("\t{\n");
			cppTypeDefinitions.Append("\t\t");
			AppendCppTypeFullName(
				enumeratorType,
				cppTypeDefinitions);
			cppTypeDefinitions.Append(" enumerator;\n");
			cppTypeDefinitions.Append("\t\tbool hasMore;\n");
			cppTypeDefinitions.Append("\t\t");
			cppTypeDefinitions.Append(bindingEnumerableTypeName);
			cppTypeDefinitions.Append("Iterator(decltype(nullptr));\n");
			cppTypeDefinitions.Append("\t\t");
			cppTypeDefinitions.Append(bindingEnumerableTypeName);
			cppTypeDefinitions.Append("Iterator(");
			AppendCppTypeFullName(
				enumerableType,
				cppTypeDefinitions);
			cppTypeDefinitions.Append("& enumerable);\n");
			cppTypeDefinitions.Append("\t\t~");
			cppTypeDefinitions.Append(bindingEnumerableTypeName);
			cppTypeDefinitions.Append("Iterator();\n");
			cppTypeDefinitions.Append("\t\t");
			cppTypeDefinitions.Append(bindingEnumerableTypeName);
			cppTypeDefinitions.Append("Iterator& operator++();\n");
			cppTypeDefinitions.Append("\t\tbool operator!=(const ");
			cppTypeDefinitions.Append(bindingEnumerableTypeName);
			cppTypeDefinitions.Append("Iterator& other);\n");
			cppTypeDefinitions.Append("\t\t");
			AppendCppTypeFullName(
				elementType,
				cppTypeDefinitions);
			cppTypeDefinitions.Append(" operator*();\n");
			cppTypeDefinitions.Append("\t};\n");
			cppTypeDefinitions.Append("}\n");
			cppTypeDefinitions.Append('\n');
			
			// begin() and end() declarations
			int indent = AppendNamespaceBeginning(
				enumerableType.Namespace,
				cppTypeDefinitions);
			AppendIndent(
				indent,
				cppTypeDefinitions);
			cppTypeDefinitions.Append("Plugin::");
			cppTypeDefinitions.Append(bindingEnumerableTypeName);
			cppTypeDefinitions.Append("Iterator begin(");
			AppendCppTypeFullName(
				enumerableType,
				cppTypeDefinitions);
			cppTypeDefinitions.Append("& enumerable);\n");
			AppendIndent(
				indent,
				cppTypeDefinitions);
			cppTypeDefinitions.Append("Plugin::");
			cppTypeDefinitions.Append(bindingEnumerableTypeName);
			cppTypeDefinitions.Append("Iterator end(");
			AppendCppTypeFullName(
				enumerableType,
				cppTypeDefinitions);
			cppTypeDefinitions.Append("& enumerable);\n");
			AppendNamespaceEnding(
				indent,
				cppTypeDefinitions);
			cppTypeDefinitions.Append('\n');
			
			// Iterator method definitions
			cppMethodDefinitions.Append("namespace Plugin\n");
			cppMethodDefinitions.Append("{\n");
			cppMethodDefinitions.Append('\t');
			cppMethodDefinitions.Append(bindingEnumerableTypeName);
			cppMethodDefinitions.Append("Iterator::");
			cppMethodDefinitions.Append(bindingEnumerableTypeName);
			cppMethodDefinitions.Append("Iterator(decltype(nullptr))\n");
			cppMethodDefinitions.Append("\t\t: enumerator(nullptr)\n");
			cppMethodDefinitions.Append("\t\t, hasMore(false)\n");
			cppMethodDefinitions.Append("\t{\n");
			cppMethodDefinitions.Append("\t}\n");
			cppMethodDefinitions.Append("\t\n");
			cppMethodDefinitions.Append('\t');
			cppMethodDefinitions.Append(bindingEnumerableTypeName);
			cppMethodDefinitions.Append("Iterator::");
			cppMethodDefinitions.Append(bindingEnumerableTypeName);
			cppMethodDefinitions.Append("Iterator(");
			AppendCppTypeFullName(
				enumerableType,
				cppMethodDefinitions);
			cppMethodDefinitions.Append("& enumerable)\n");
			cppMethodDefinitions.Append("\t\t: enumerator(enumerable.GetEnumerator())\n");
			cppMethodDefinitions.Append("\t{\n");
			cppMethodDefinitions.Append("\t\thasMore = enumerator.MoveNext();\n");
			cppMethodDefinitions.Append("\t}\n");
			cppMethodDefinitions.Append("\t\n");
			cppMethodDefinitions.Append('\t');
			cppMethodDefinitions.Append(bindingEnumerableTypeName);
			cppMethodDefinitions.Append("Iterator::~");
			cppMethodDefinitions.Append(bindingEnumerableTypeName);
			cppMethodDefinitions.Append("Iterator()\n");
			cppMethodDefinitions.Append("\t{\n");
			cppMethodDefinitions.Append("\t\tif (enumerator != nullptr)\n");
			cppMethodDefinitions.Append("\t\t{\n");
			cppMethodDefinitions.Append("\t\t\tenumerator.Dispose();\n");
			cppMethodDefinitions.Append("\t\t}\n");
			cppMethodDefinitions.Append("\t}\n");
			cppMethodDefinitions.Append("\t\n");
			cppMethodDefinitions.Append('\t');
			cppMethodDefinitions.Append(bindingEnumerableTypeName);
			cppMethodDefinitions.Append("Iterator& ");
			cppMethodDefinitions.Append(bindingEnumerableTypeName);
			cppMethodDefinitions.Append("Iterator::");
			cppMethodDefinitions.Append("operator++()\n");
			cppMethodDefinitions.Append("\t{\n");
			cppMethodDefinitions.Append("\t\thasMore = enumerator.MoveNext();\n");
			cppMethodDefinitions.Append("\t\treturn *this;\n");
			cppMethodDefinitions.Append("\t}\n");
			cppMethodDefinitions.Append("\t\n");
			cppMethodDefinitions.Append("\tbool ");
			cppMethodDefinitions.Append(bindingEnumerableTypeName);
			cppMethodDefinitions.Append("Iterator::");
			cppMethodDefinitions.Append("operator!=(const ");
			cppMethodDefinitions.Append(bindingEnumerableTypeName);
			cppMethodDefinitions.Append("Iterator& other)\n");
			cppMethodDefinitions.Append("\t{\n");
			cppMethodDefinitions.Append("\t\treturn hasMore;\n");
			cppMethodDefinitions.Append("\t}\n");
			cppMethodDefinitions.Append("\t\n");
			cppMethodDefinitions.Append('\t');
			AppendCppTypeFullName(
				elementType,
				cppMethodDefinitions);
			cppMethodDefinitions.Append(' ');
			cppMethodDefinitions.Append(bindingEnumerableTypeName);
			cppMethodDefinitions.Append("Iterator::");
			cppMethodDefinitions.Append("operator*()\n");
			cppMethodDefinitions.Append("\t{\n");
			cppMethodDefinitions.Append("\t\treturn enumerator.GetCurrent();\n");
			cppMethodDefinitions.Append("\t}\n");
			cppMethodDefinitions.Append("}\n");
			cppMethodDefinitions.Append('\n');
			
			// begin() and end() definitions
			indent = AppendNamespaceBeginning(
				enumerableType.Namespace,
				cppMethodDefinitions);
			AppendIndent(
				indent,
				cppMethodDefinitions);
			cppMethodDefinitions.Append("Plugin::");
			cppMethodDefinitions.Append(bindingEnumerableTypeName);
			cppMethodDefinitions.Append("Iterator begin(");
			AppendCppTypeFullName(
				enumerableType,
				cppMethodDefinitions);
			cppMethodDefinitions.Append("& enumerable)\n");
			AppendIndent(
				indent,
				cppMethodDefinitions);
			cppMethodDefinitions.Append("{\n");
			AppendIndent(
				indent + 1,
				cppMethodDefinitions);
			cppMethodDefinitions.Append("return Plugin::");
			cppMethodDefinitions.Append(bindingEnumerableTypeName);
			cppMethodDefinitions.Append("Iterator(enumerable);\n");
			AppendIndent(
				indent,
				cppMethodDefinitions);
			cppMethodDefinitions.Append("}\n");
			AppendIndent(
				indent,
				cppMethodDefinitions);
			cppMethodDefinitions.Append('\n');
			AppendIndent(
				indent,
				cppMethodDefinitions);
			cppMethodDefinitions.Append("Plugin::");
			cppMethodDefinitions.Append(bindingEnumerableTypeName);
			cppMethodDefinitions.Append("Iterator end(");
			AppendCppTypeFullName(
				enumerableType,
				cppMethodDefinitions);
			cppMethodDefinitions.Append("& enumerable)\n");
			AppendIndent(
				indent,
				cppMethodDefinitions);
			cppMethodDefinitions.Append("{\n");
			AppendIndent(
				indent + 1,
				cppMethodDefinitions);
			cppMethodDefinitions.Append("return Plugin::");
			cppMethodDefinitions.Append(bindingEnumerableTypeName);
			cppMethodDefinitions.Append("Iterator(nullptr);\n");
			AppendIndent(
				indent,
				cppMethodDefinitions);
			cppMethodDefinitions.Append("}\n");
			AppendNamespaceEnding(
				indent,
				cppMethodDefinitions);
			cppMethodDefinitions.Append('\n');
		}
		
		static void AppendCppArrayIndexOperatorMethodDefinition(
			int rank,
			int indent,
			TypeName enclosingTypeTypeName,
			string nextCppElementProxyTypeName,
			StringBuilder output)
		{
			AppendIndent(
				indent,
				output);
			AppendCppTypeFullName(
				GetTypeName(nextCppElementProxyTypeName, "Plugin"),
				output);
			output.Append(' ');
			output.Append(enclosingTypeTypeName.Namespace);
			output.Append("::");
			AppendTypeNameWithoutGenericSuffix(
				enclosingTypeTypeName.Name,
				output);
			output.Append("::operator[](int32_t index)\n");
			AppendIndent(
				indent,
				output);
			output.Append("{\n");
			AppendIndent(
				indent + 1,
				output);
			output.Append("return Plugin::");
			output.Append(nextCppElementProxyTypeName);
			output.Append("(Plugin::InternalUse::Only, Handle, ");
			for (int i = 0; i < rank; ++i)
			{
				output.Append("Index");
				output.Append(i);
				output.Append(", ");
			}
			output.Append("index);\n");
			AppendIndent(
				indent,
				output);
			output.Append("}\n");
			AppendIndent(
				indent,
				output);
			output.Append('\n');
		}
		
		static void AppendCppArrayTypeName(
			int rank,
			StringBuilder output)
		{
			output.Append("Array");
			output.Append(rank);
		}
		
		static void AppendCppArrayElementProxyName(
			int rank,
			int maxRank,
			Type elementType,
			StringBuilder output)
		{
			output.Append("ArrayElementProxy");
			output.Append(rank);
			output.Append('_');
			output.Append(maxRank);
			output.Append('<');
			AppendCppTypeFullName(
				elementType,
				output);
			output.Append('>');
		}

		static ParameterInfo[] BuildArrayGetItemsParams(
			int rank,
			string indexName)
		{
			ParameterInfo[] parameters = new ParameterInfo[rank];
			for (int i = 0; i < rank; ++i)
			{
				ParameterInfo param = new ParameterInfo();
				param.Name = indexName + i;
				param.ParameterType = typeof(int);
				param.IsOut = false;
				param.IsRef = false;
				param.DereferencedParameterType = param.ParameterType;
				param.Kind = GetTypeKind(
					param.DereferencedParameterType);
				parameters[i] = param;
			}
			return parameters;
		}
		
		static ParameterInfo[] BuildArraySetItemsParams(
			int rank,
			string indexName,
			Type elementType)
		{
			ParameterInfo[] parameters = new ParameterInfo[rank+1];
			for (int i = 0; i < rank; ++i)
			{
				ParameterInfo param = new ParameterInfo();
				param.Name = indexName + i;
				param.ParameterType = typeof(int);
				param.IsOut = false;
				param.IsRef = false;
				param.DereferencedParameterType = param.ParameterType;
				param.Kind = GetTypeKind(
					param.DereferencedParameterType);
				parameters[i] = param;
			}
			
			ParameterInfo lastParamInfo = new ParameterInfo();
			lastParamInfo.Name = "item";
			lastParamInfo.ParameterType = elementType;
			lastParamInfo.IsOut = false;
			lastParamInfo.IsRef = false;
			lastParamInfo.DereferencedParameterType = lastParamInfo.ParameterType;
			lastParamInfo.Kind = GetTypeKind(
				lastParamInfo.DereferencedParameterType);
			parameters[rank] = lastParamInfo;
			
			return parameters;
		}
		
		static void AppendArrayGetItemFuncName(
			TypeName elementTypeTypeName,
			string bindingArrayTypeName,
			int rank,
			StringBuilder output)
		{
			AppendNamespace(
				elementTypeTypeName.Namespace,
				string.Empty,
				output);
			output.Append(elementTypeTypeName.Name);
			AppendTypeNameWithoutGenericSuffix(
				bindingArrayTypeName,
				output);
			output.Append("GetItem");
			output.Append(rank);
		}
		
		static void AppendArraySetItemFuncName(
			TypeName elementTypeTypeName,
			string bindingArrayTypeName,
			int rank,
			StringBuilder output)
		{
			AppendNamespace(
				elementTypeTypeName.Namespace,
				string.Empty,
				output);
			output.Append(elementTypeTypeName.Name);
			AppendTypeNameWithoutGenericSuffix(
				bindingArrayTypeName,
				output);
			output.Append("SetItem");
			output.Append(rank);
		}
		
		static void AppendArrayElementProxy(
			Type elementType,
			TypeKind elementTypeKind,
			int rank,
			int maxRank,
			Type[] cppTypeParams,
			string cppArrayTypeName,
			StringBuilders builders)
		{
			// Build element proxy name
			builders.TempStrBuilder.Length = 0;
			AppendCppArrayElementProxyName(
				rank,
				maxRank,
				elementType,
				builders.TempStrBuilder);
			string cppElementProxyTypeName = builders.TempStrBuilder.ToString();
			
			// Build next element proxy name
			builders.TempStrBuilder.Length = 0;
			AppendCppArrayElementProxyName(
				rank + 1,
				maxRank,
				elementType,
				builders.TempStrBuilder);
			string nextCppElementProxyTypeName = builders.TempStrBuilder.ToString();
			
			// GetItem name
			builders.TempStrBuilder.Length = 0;
			AppendArrayGetItemFuncName(
				GetTypeName(elementType),
				cppArrayTypeName,
				rank,
				builders.TempStrBuilder);
			string getItemFuncName = builders.TempStrBuilder.ToString();
			
			// SetItem name
			builders.TempStrBuilder.Length = 0;
			AppendArraySetItemFuncName(
				GetTypeName(elementType),
				cppArrayTypeName,
				rank,
				builders.TempStrBuilder);
			string setItemFuncName = builders.TempStrBuilder.ToString();
			
			// GetItem call params
			ParameterInfo[] getItemCallParams = BuildArrayGetItemsParams(
				rank,
				"Index");
			
			// SetItem params
			ParameterInfo[] setItemCallParams = BuildArraySetItemsParams(
				rank,
				"Index",
				elementType);
			
			// C++ element proxy type declaration
			int indent = AppendNamespaceBeginning(
				"Plugin",
				builders.CppTemplateSpecializationDeclarations);
			AppendIndent(indent, builders.CppTemplateSpecializationDeclarations);
			builders.CppTemplateSpecializationDeclarations.Append("template<> struct ");
			AppendTypeNameWithoutGenericSuffix(
				cppElementProxyTypeName,
				builders.CppTemplateSpecializationDeclarations);
			builders.CppTemplateSpecializationDeclarations.Append(";\n");
			AppendNamespaceEnding(
				indent,
				builders.CppTemplateSpecializationDeclarations);
			builders.CppTemplateSpecializationDeclarations.Append('\n');
			
			// C++ element proxy type definition
			AppendNamespaceBeginning(
				"Plugin",
				builders.CppTypeDefinitions);
			AppendIndent(
				indent,
				builders.CppTypeDefinitions);
			builders.CppTypeDefinitions.Append("template<> struct ");
			builders.CppTypeDefinitions.Append(cppElementProxyTypeName);
			builders.CppTypeDefinitions.Append('\n');
			AppendIndent(
				indent,
				builders.CppTypeDefinitions);
			builders.CppTypeDefinitions.Append("{\n");
			AppendIndent(
				indent + 1,
				builders.CppTypeDefinitions);
			builders.CppTypeDefinitions.Append("int32_t Handle;\n");
			for (int i = 0; i < rank; ++i)
			{
				AppendIndent(
					indent + 1,
					builders.CppTypeDefinitions);
				builders.CppTypeDefinitions.Append("int32_t Index");
				builders.CppTypeDefinitions.Append(i);
				builders.CppTypeDefinitions.Append(";\n");
			}
			AppendIndent(
				indent + 1,
				builders.CppTypeDefinitions);
			builders.CppTypeDefinitions.Append(cppElementProxyTypeName);
			builders.CppTypeDefinitions.Append(
				"(Plugin::InternalUse, int32_t handle, ");
			for (int i = 0; i < rank; ++i)
			{
				builders.CppTypeDefinitions.Append("int32_t index");
				builders.CppTypeDefinitions.Append(i);
				if (i != rank - 1)
				{
					builders.CppTypeDefinitions.Append(", ");
				}
			}
			builders.CppTypeDefinitions.Append(");\n");
			if (rank == maxRank)
			{
				AppendIndent(
					indent + 1,
					builders.CppTypeDefinitions);
				builders.CppTypeDefinitions.Append("void operator=(");
				AppendCppTypeFullName(
					elementType,
					builders.CppTypeDefinitions);
				builders.CppTypeDefinitions.Append(" item);\n");
				AppendIndent(
					indent + 1,
					builders.CppTypeDefinitions);
				builders.CppTypeDefinitions.Append("operator ");
				AppendCppTypeFullName(
					elementType,
					builders.CppTypeDefinitions);
				builders.CppTypeDefinitions.Append("();\n");
			}
			else
			{
				AppendIndent(
					indent + 1,
					builders.CppTypeDefinitions);
				builders.CppTypeDefinitions.Append("Plugin::");
				AppendCppArrayElementProxyName(
					rank + 1,
					maxRank,
					elementType,
					builders.CppTypeDefinitions);
				builders.CppTypeDefinitions.Append(" operator[](");
				builders.CppTypeDefinitions.Append("int32_t index);\n");
			}
			AppendIndent(
				indent,
				builders.CppTypeDefinitions);
			builders.CppTypeDefinitions.Append("};\n");
			builders.CppTypeDefinitions.Append("}\n");
			builders.CppTypeDefinitions.Append('\n');
			
			// C++ element proxy method definitions (beginning)
			int cppMethodDefinitionsIndent = AppendNamespaceBeginning(
				"Plugin",
				builders.CppMethodDefinitions);
			
			// C++ element proxy constructor definition
			AppendIndent(
				cppMethodDefinitionsIndent,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append(cppElementProxyTypeName);
			builders.CppMethodDefinitions.Append(
				"::ArrayElementProxy");
			builders.CppMethodDefinitions.Append(rank);
			builders.CppMethodDefinitions.Append('_');
			builders.CppMethodDefinitions.Append(maxRank);
			builders.CppMethodDefinitions.Append(
				"(Plugin::InternalUse, int32_t handle, ");
			for (int i = 0; i < rank; ++i)
			{
				builders.CppMethodDefinitions.Append("int32_t index");
				builders.CppMethodDefinitions.Append(i);
				if (i != rank - 1)
				{
					builders.CppMethodDefinitions.Append(", ");
				}
			}
			builders.CppMethodDefinitions.Append(")\n");
			AppendIndent(
				cppMethodDefinitionsIndent,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("{\n");
			AppendIndent(
				cppMethodDefinitionsIndent + 1,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("Handle = handle;\n");
			for (int i = 0; i < rank; ++i)
			{
				AppendIndent(
					cppMethodDefinitionsIndent + 1,
					builders.CppMethodDefinitions);
				builders.CppMethodDefinitions.Append("Index");
				builders.CppMethodDefinitions.Append(i);
				builders.CppMethodDefinitions.Append(" = index");
				builders.CppMethodDefinitions.Append(i);
				builders.CppMethodDefinitions.Append(";\n");
			}
			AppendIndent(
				cppMethodDefinitionsIndent,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("}\n");
			AppendIndent(
				cppMethodDefinitionsIndent,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append('\n');
			
			if (rank == maxRank)
			{
				// C++ element proxy operator= definition
				AppendIndent(
					cppMethodDefinitionsIndent,
					builders.CppMethodDefinitions);
				builders.CppMethodDefinitions.Append("void ");
				builders.CppMethodDefinitions.Append(cppElementProxyTypeName);
				builders.CppMethodDefinitions.Append("::");
				builders.CppMethodDefinitions.Append("operator=(");
				AppendCppTypeFullName(
					elementType,
					builders.CppMethodDefinitions);
				builders.CppMethodDefinitions.Append(" item)\n");
				AppendIndent(
					cppMethodDefinitionsIndent,
					builders.CppMethodDefinitions);
				builders.CppMethodDefinitions.Append("{\n");
				AppendCppPluginFunctionCall(
					false,
					GetTypeName(cppArrayTypeName, "System"),
					TypeKind.Class,
					cppTypeParams,
					typeof(void),
					setItemFuncName,
					setItemCallParams,
					cppMethodDefinitionsIndent + 1,
					builders.CppMethodDefinitions);
				AppendIndent(
					cppMethodDefinitionsIndent,
					builders.CppMethodDefinitions);
				builders.CppMethodDefinitions.Append("}\n");
				AppendIndent(
					cppMethodDefinitionsIndent,
					builders.CppMethodDefinitions);
				builders.CppMethodDefinitions.Append('\n');
				
				// C++ element proxy type conversion operator definition
				AppendIndent(
					cppMethodDefinitionsIndent,
					builders.CppMethodDefinitions);
				builders.CppMethodDefinitions.Append(cppElementProxyTypeName);
				builders.CppMethodDefinitions.Append("::");
				builders.CppMethodDefinitions.Append("operator ");
				AppendCppTypeFullName(
					elementType,
					builders.CppMethodDefinitions);
				builders.CppMethodDefinitions.Append("()\n");
				AppendIndent(
					cppMethodDefinitionsIndent,
					builders.CppMethodDefinitions);
				builders.CppMethodDefinitions.Append("{\n");
				AppendCppPluginFunctionCall(
					false,
					GetTypeName(cppArrayTypeName, "System"),
					TypeKind.Class,
					cppTypeParams,
					elementType,
					getItemFuncName,
					getItemCallParams,
					indent + 1,
					builders.CppMethodDefinitions);
				AppendCppMethodReturn(
					elementType,
					elementTypeKind,
					indent + 1,
					builders.CppMethodDefinitions);
				AppendIndent(
					cppMethodDefinitionsIndent,
					builders.CppMethodDefinitions);
				builders.CppMethodDefinitions.Append("}\n");
			}
			else
			{
				AppendCppArrayIndexOperatorMethodDefinition(
					rank,
					cppMethodDefinitionsIndent,
					GetTypeName(cppElementProxyTypeName, "Plugin"),
					nextCppElementProxyTypeName,
					builders.CppMethodDefinitions);
			}
			
			// C++ method definitions (ending)
			AppendCppMethodDefinitionsEnd(
				cppMethodDefinitionsIndent,
				builders.CppMethodDefinitions);
		}
		
		static void AppendArrayConstructor(
			Type elementType,
			Type arrayType,
			string cppArrayTypeName,
			int rank,
			string csharpTypeName,
			Type[] cppCtorInitTypes,
			int indent,
			StringBuilders builders)
		{
			builders.TempStrBuilder.Length = 0;
			AppendNamespace(
				elementType.Namespace,
				string.Empty,
				builders.TempStrBuilder);
			AppendTypeNameWithoutGenericSuffix(
				csharpTypeName,
				builders.TempStrBuilder);
			builders.TempStrBuilder.Append("Constructor");
			builders.TempStrBuilder.Append(rank);
			string funcName = builders.TempStrBuilder.ToString();
			
			builders.TempStrBuilder[0] = char.ToLower(
				builders.TempStrBuilder[0]);
			string funcNameLower = builders.TempStrBuilder.ToString();
			
			ParameterInfo[] parameters = new ParameterInfo[rank];
			for (int i = 0; i < rank; ++i)
			{
				ParameterInfo info = new ParameterInfo();
				info.Name = "length" + i;
				info.ParameterType = typeof(int);
				info.IsOut = false;
				info.IsRef = false;
				info.DereferencedParameterType = info.ParameterType;
				info.Kind = TypeKind.Primitive;
				parameters[i] = info;
			}

			TypeName cppArrayTypeTypeName = GetTypeName(
				cppArrayTypeName,
				"System");
			
			// C# Delegate Type
			AppendCsharpDelegateType(
				funcName,
				true,
				arrayType,
				TypeKind.Class,
				arrayType,
				parameters,
				builders.CsharpDelegateTypes);
			
			// C# Init Call
			AppendCsharpInitCallArg(
				funcName,
				builders.CsharpInitCall);
			
			// C# Init Param
			AppendCsharpInitParam(
				funcNameLower,
				builders.CsharpInitParams);
			
			// C# function
			AppendCsharpFunctionBeginning(
				arrayType,
				funcName,
				true,
				TypeKind.Class,
				arrayType,
				parameters,
				builders.CsharpFunctions);
			AppendHandleStoreTypeName(
				arrayType,
				builders.CsharpFunctions);
			builders.CsharpFunctions.Append(".Store(new ");
			AppendCsharpTypeFullName(
				elementType,
				builders.CsharpFunctions);
			builders.CsharpFunctions.Append('[');
			for (int i = 0; i < rank; ++i)
			{
				builders.CsharpFunctions.Append("length");
				builders.CsharpFunctions.Append(i);
				if (i != rank-1)
				{
					builders.CsharpFunctions.Append(", ");
				}
			}
			builders.CsharpFunctions.Append("]);");
			AppendCsharpFunctionReturn(
				parameters,
				arrayType,
				TypeKind.Class,
				null,
				true,
				builders.CsharpFunctions);
			
			// C++ function pointer definition
			AppendCppFunctionPointerDefinition(
				funcName,
				true,
				cppArrayTypeTypeName,
				TypeKind.Class,
				parameters,
				arrayType,
				builders.CppFunctionPointers);
			
			// C++ init param
			AppendCppInitParam(
				funcNameLower,
				true,
				cppArrayTypeTypeName,
				TypeKind.Class,
				parameters,
				arrayType,
				builders.CppInitParams);
			
			// C++ init body
			AppendCppInitBody(
				funcName,
				funcNameLower,
				builders.CppInitBody);
			
			// C++ method declaration
			AppendIndent(
				indent + 1,
				builders.CppTypeDefinitions);
			AppendCppMethodDeclaration(
				cppArrayTypeName,
				false,
				false,
				false,
				null,
				null,
				parameters,
				builders.CppTypeDefinitions);
			
			// C++ method definition
			Type[] cppTypeParams = { elementType };
			AppendCppMethodDefinitionBegin(
				GetTypeName(cppArrayTypeName, "System"),
				null,
				cppArrayTypeName,
				cppTypeParams,
				null,
				parameters,
				indent,
				builders.CppMethodDefinitions);
			string separator = ": ";
			foreach (Type interfaceType in cppCtorInitTypes)
			{
				AppendIndent(
					indent + 1,
					builders.CppMethodDefinitions);
				builders.CppMethodDefinitions.Append(separator);
				AppendCppTypeFullName(
					interfaceType,
					builders.CppMethodDefinitions);
				builders.CppMethodDefinitions.Append("(nullptr)\n");
				separator = ", ";
			}
			AppendIndent(
				indent,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("{\n");
			AppendCppPluginFunctionCall(
				true,
				cppArrayTypeTypeName,
				TypeKind.Class,
				cppTypeParams,
				arrayType,
				funcName,
				parameters,
				indent + 1,
				builders.CppMethodDefinitions);
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
				cppArrayTypeTypeName,
				TypeKind.Class,
				cppTypeParams,
				"returnValue",
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append(";\n");
			if (rank > 1)
			{
				AppendIndent(
					indent + 2,
					builders.CppMethodDefinitions);
				builders.CppMethodDefinitions.Append("InternalLength = ");
				for (int i = 0; i < rank; ++i)
				{
					builders.CppMethodDefinitions.Append("length");
					builders.CppMethodDefinitions.Append(i);
					if (i != rank - 1)
					{
						builders.CppMethodDefinitions.Append(" * ");
					}
				}
				builders.CppMethodDefinitions.Append(";\n");
				for (int i = 0; i < rank; ++i)
				{
					AppendIndent(
						indent + 2,
						builders.CppMethodDefinitions);
					builders.CppMethodDefinitions.Append("InternalLengths[");
					builders.CppMethodDefinitions.Append(i);
					builders.CppMethodDefinitions.Append("] = length");
					builders.CppMethodDefinitions.Append(i);
					builders.CppMethodDefinitions.Append(";\n");
				}
			}
			else
			{
				AppendIndent(
					indent + 2,
					builders.CppMethodDefinitions);
				builders.CppMethodDefinitions.Append(
					"InternalLength = length0;\n");
			}
			AppendIndent(
				indent + 1,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append(
				"}\n");
			AppendIndent(
				indent,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("}\n");
			AppendIndent(
				indent,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append('\n');
		}
		
		static void AppendArrayCppGetLengthFunction(
			int indent,
			string cppArrayTypeName,
			Type[] cppTypeParams,
			StringBuilders builders)
		{
			ParameterInfo[] parameters = new ParameterInfo[0];
			
			// C++ method declaration
			AppendIndent(
				indent + 1,
				builders.CppTypeDefinitions);
			AppendCppMethodDeclaration(
				"GetLength",
				false,
				false,
				false,
				typeof(int),
				null,
				parameters,
				builders.CppTypeDefinitions);
			
			// C++ method definition
			AppendCppMethodDefinitionBegin(
				GetTypeName(cppArrayTypeName, "System"),
				typeof(int),
				"GetLength",
				cppTypeParams,
				null,
				parameters,
				indent,
				builders.CppMethodDefinitions);
			AppendIndent(
				indent,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("{\n");
			AppendIndent(
				indent + 1,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append(
				"int32_t returnVal = InternalLength;\n");
			AppendIndent(
				indent + 1,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("if (returnVal == 0)\n");
			AppendIndent(
				indent + 1,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("{\n");
			AppendIndent(
				indent + 2,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append(
				"returnVal = Array::GetLength();\n");
			AppendIndent(
				indent + 2,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append(
				"InternalLength = returnVal;\n");
			AppendIndent(
				indent + 1,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("};\n");
			AppendIndent(
				indent + 1,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("return returnVal;\n");
			AppendIndent(
				indent,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("}\n");
			AppendIndent(
				indent,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append('\n');
		}
		
		static void AppendArrayCppGetRankFunction(
			int indent,
			string cppArrayTypeName,
			Type[] cppTypeParams,
			int rank,
			StringBuilders builders)
		{
			ParameterInfo[] parameters = new ParameterInfo[0];
			
			// C++ method declaration
			AppendIndent(
				indent + 1,
				builders.CppTypeDefinitions);
			AppendCppMethodDeclaration(
				"GetRank",
				false,
				false,
				false,
				typeof(int),
				null,
				parameters,
				builders.CppTypeDefinitions);
			
			// C++ method definition
			AppendCppMethodDefinitionBegin(
				GetTypeName(cppArrayTypeName, "System"),
				typeof(int),
				"GetRank",
				cppTypeParams,
				null,
				parameters,
				indent,
				builders.CppMethodDefinitions);
			AppendIndent(
				indent,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("{\n");
			AppendIndent(
				indent + 1,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("return ");
			builders.CppMethodDefinitions.Append(rank);
			builders.CppMethodDefinitions.Append(";\n");
			AppendIndent(
				indent,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("}\n");
			AppendIndent(
				indent,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append('\n');
		}
		
		static void AppendArrayMultidimensionalGetLength(
			Type elementType,
			Type arrayType,
			string cppArrayTypeName,
			int rank,
			string csharpTypeName,
			int indent,
			StringBuilders builders)
		{
			builders.TempStrBuilder.Length = 0;
			AppendNamespace(
				elementType.Namespace,
				string.Empty,
				builders.TempStrBuilder);
			AppendTypeNameWithoutGenericSuffix(
				csharpTypeName,
				builders.TempStrBuilder);
			builders.TempStrBuilder.Append("GetLength");
			builders.TempStrBuilder.Append(rank);
			string funcName = builders.TempStrBuilder.ToString();
			
			builders.TempStrBuilder[0] = char.ToLower(
				builders.TempStrBuilder[0]);
			string funcNameLower = builders.TempStrBuilder.ToString();
			
			ParameterInfo[] parameters = {
				new ParameterInfo {
					Name = "dimension",
					ParameterType = typeof(int),
					IsOut = false,
					IsRef = false,
					DereferencedParameterType = typeof(int),
					Kind = TypeKind.Primitive,
				}
			};

			TypeName cppArrayTypeTypeName = GetTypeName(
				cppArrayTypeName,
				"System");
			
			// C# Delegate Type
			AppendCsharpDelegateType(
				funcName,
				false,
				arrayType,
				TypeKind.Class,
				typeof(int),
				parameters,
				builders.CsharpDelegateTypes);
			
			// C# Init Call
			AppendCsharpInitCallArg(
				funcName,
				builders.CsharpInitCall);
			
			// C# Init Param
			AppendCsharpInitParam(
				funcNameLower,
				builders.CsharpInitParams);
			
			// C# function
			AppendCsharpFunctionBeginning(
				arrayType,
				funcName,
				false,
				TypeKind.Class,
				typeof(int),
				parameters,
				builders.CsharpFunctions);
			builders.CsharpFunctions.Append(
				"thiz.GetLength(dimension);");
			AppendCsharpFunctionReturn(
				parameters,
				typeof(int),
				TypeKind.Primitive,
				null,
				false,
				builders.CsharpFunctions);
			
			// C++ function pointer definition
			AppendCppFunctionPointerDefinition(
				funcName,
				false,
				cppArrayTypeTypeName,
				TypeKind.Class,
				parameters,
				arrayType,
				builders.CppFunctionPointers);
			
			// C++ init param
			AppendCppInitParam(
				funcNameLower,
				false,
				cppArrayTypeTypeName,
				TypeKind.Class,
				parameters,
				arrayType,
				builders.CppInitParams);
			
			// C++ init body
			AppendCppInitBody(
				funcName,
				funcNameLower,
				builders.CppInitBody);
			
			// C++ method declaration
			AppendIndent(
				indent + 1,
				builders.CppTypeDefinitions);
			AppendCppMethodDeclaration(
				"GetLength",
				false,
				false,
				false,
				typeof(int),
				null,
				parameters,
				builders.CppTypeDefinitions);
			
			// C++ method definition
			Type[] cppTypeParams = { elementType };
			AppendCppMethodDefinitionBegin(
				GetTypeName(cppArrayTypeName, "System"),
				typeof(int),
				"GetLength",
				cppTypeParams,
				null,
				parameters,
				indent,
				builders.CppMethodDefinitions);
			AppendIndent(
				indent,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("{\n");
			AppendIndent(
				indent + 1,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append(
				"assert(dimension >= 0 && dimension < ");
			builders.CppMethodDefinitions.Append(rank);
			builders.CppMethodDefinitions.Append(");\n");
			AppendIndent(
				indent + 1,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append(
				"int32_t length = InternalLengths[dimension];\n");
			AppendIndent(
				indent + 1,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("if (length)\n");
			AppendIndent(
				indent + 1,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("{\n");
			AppendIndent(
				indent + 2,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("return length;\n");
			AppendIndent(
				indent + 1,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("}\n");
			AppendCppPluginFunctionCall(
				false,
				GetTypeName(cppArrayTypeName, "System"),
				TypeKind.Class,
				cppTypeParams,
				typeof(int),
				funcName,
				parameters,
				indent + 1,
				builders.CppMethodDefinitions);
			AppendIndent(
				indent + 1,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append(
				"InternalLengths[dimension] = returnValue;\n");
			AppendCppMethodReturn(
				typeof(int),
				TypeKind.Primitive,
				indent + 1,
				builders.CppMethodDefinitions);
			AppendIndent(
				indent,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("}\n");
			AppendIndent(
				indent,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append('\n');
		}
		
		static void AppendArrayGetItem(
			Type elementType,
			TypeKind elementTypeKind,
			Type arrayType,
			string cppArrayTypeName,
			int rank,
			StringBuilders builders)
		{
			builders.TempStrBuilder.Length = 0;
			AppendArrayGetItemFuncName(
				GetTypeName(elementType),
				cppArrayTypeName,
				rank,
				builders.TempStrBuilder);
			string funcName = builders.TempStrBuilder.ToString();
			
			builders.TempStrBuilder[0] = char.ToLower(
				builders.TempStrBuilder[0]);
			string funcNameLower = builders.TempStrBuilder.ToString();
			
			ParameterInfo[] parameters = BuildArrayGetItemsParams(
				rank,
				"index");
			
			// C# Delegate Type
			AppendCsharpDelegateType(
				funcName,
				false,
				arrayType,
				TypeKind.Class,
				elementType,
				parameters,
				builders.CsharpDelegateTypes);
			
			// C# Init Call
			AppendCsharpInitCallArg(
				funcName,
				builders.CsharpInitCall);
			
			// C# Init Param
			AppendCsharpInitParam(
				funcNameLower,
				builders.CsharpInitParams);
			
			// C# function
			AppendCsharpFunctionBeginning(
				arrayType,
				funcName,
				false,
				TypeKind.Class,
				elementType,
				parameters,
				builders.CsharpFunctions);
			builders.CsharpFunctions.Append("thiz[");
			for (int i = 0; i < rank; ++i)
			{
				builders.CsharpFunctions.Append("index");
				builders.CsharpFunctions.Append(i);
				if (i != rank-1)
				{
					builders.CsharpFunctions.Append(", ");
				}
			}
			builders.CsharpFunctions.Append("];");
			AppendCsharpFunctionReturn(
				parameters,
				elementType,
				elementTypeKind,
				null,
				false,
				builders.CsharpFunctions);

			TypeName cppArrayTypeTypeName = GetTypeName(
				"System",
				cppArrayTypeName);

			// C++ function pointer definition
			AppendCppFunctionPointerDefinition(
				funcName,
				false,
				cppArrayTypeTypeName,
				TypeKind.Class,
				parameters,
				elementType,
				builders.CppFunctionPointers);
			
			// C++ init param
			AppendCppInitParam(
				funcNameLower,
				false,
				cppArrayTypeTypeName,
				TypeKind.Class,
				parameters,
				elementType,
				builders.CppInitParams);
			
			// C++ init body
			AppendCppInitBody(
				funcName,
				funcNameLower,
				builders.CppInitBody);
		}
		
		static void AppendArraySetItem(
			Type elementType,
			Type arrayType,
			string cppArrayTypeName,
			int rank,
			StringBuilders builders)
		{
			builders.TempStrBuilder.Length = 0;
			AppendArraySetItemFuncName(
				GetTypeName(elementType),
				cppArrayTypeName,
				rank,
				builders.TempStrBuilder);
			string funcName = builders.TempStrBuilder.ToString();
			
			builders.TempStrBuilder[0] = char.ToLower(
				builders.TempStrBuilder[0]);
			string funcNameLower = builders.TempStrBuilder.ToString();
			
			// Build parameters as indexes then element
			ParameterInfo[] parameters = BuildArraySetItemsParams(
				rank,
				"index",
				elementType);
			
			// C# Delegate Type
			AppendCsharpDelegateType(
				funcName,
				false,
				arrayType,
				TypeKind.Class,
				typeof(void),
				parameters,
				builders.CsharpDelegateTypes);
			
			// C# Init Call
			AppendCsharpInitCallArg(
				funcName,
				builders.CsharpInitCall);
			
			// C# Init Param
			AppendCsharpInitParam(
				funcNameLower,
				builders.CsharpInitParams);
			
			// C# function
			AppendCsharpFunctionBeginning(
				arrayType,
				funcName,
				false,
				TypeKind.Class,
				typeof(void),
				parameters,
				builders.CsharpFunctions);
			builders.CsharpFunctions.Append("thiz[");
			for (int i = 0; i < rank; ++i)
			{
				builders.CsharpFunctions.Append("index");
				builders.CsharpFunctions.Append(i);
				if (i != rank-1)
				{
					builders.CsharpFunctions.Append(", ");
				}
			}
			builders.CsharpFunctions.Append("] = item;");
			AppendCsharpFunctionReturn(
				parameters,
				typeof(void),
				TypeKind.None,
				null,
				false,
				builders.CsharpFunctions);

			TypeName cppArrayTypeTypeName = GetTypeName(
				"System",
				cppArrayTypeName);

			// C++ function pointer definition
			AppendCppFunctionPointerDefinition(
				funcName,
				false,
				cppArrayTypeTypeName,
				TypeKind.Class,
				parameters,
				arrayType,
				builders.CppFunctionPointers);
			
			// C++ init param
			AppendCppInitParam(
				funcNameLower,
				false,
				cppArrayTypeTypeName,
				TypeKind.Class,
				parameters,
				arrayType,
				builders.CppInitParams);
			
			// C++ init body
			AppendCppInitBody(
				funcName,
				funcNameLower,
				builders.CppInitBody);
		}
		
		static void AppendDelegate(
			JsonDelegate jsonDelegate,
			Assembly[] assemblies,
			int defaultMaxSimultaneous,
			StringBuilders builders)
		{
			Type type = GetType(
				jsonDelegate.Type,
				assemblies);
			if (jsonDelegate.GenericParams != null)
			{
				foreach (JsonGenericParams jsonGenericParams
					in jsonDelegate.GenericParams)
				{
					// C++ template declaration
					AppendCppTemplateDeclaration(
						GetTypeName(type),
						builders.CppTemplateDeclarations);
				}
				
				foreach (JsonGenericParams jsonGenericParams
					in jsonDelegate.GenericParams)
				{
					Type[] typeParams = GetTypes(
						jsonGenericParams.Types,
						assemblies);
					Type genericType = type.MakeGenericType(typeParams);
					
					// Build numbered C++ class name (e.g. Action_2)
					builders.TempStrBuilder.Length = 0;
					AppendTypeNameWithoutSuffixes(
						type.Name,
						builders.TempStrBuilder);
					builders.TempStrBuilder.Append('_');
					builders.TempStrBuilder.Append(
						jsonGenericParams.Types.Length);
					string cppTypeName = builders.TempStrBuilder.ToString();
					
					// Max simultaneous handles of this type
					int maxSimultaneous = jsonGenericParams.MaxSimultaneous != 0
						? jsonGenericParams.MaxSimultaneous
						: jsonDelegate.MaxSimultaneous != 0
							? jsonDelegate.MaxSimultaneous
							: defaultMaxSimultaneous;
					
					AppendDelegate(
						genericType,
						cppTypeName,
						typeParams,
						maxSimultaneous,
						builders);
				}
			}
			else
			{
				int maxSimultaneous = jsonDelegate.MaxSimultaneous != 0
					? jsonDelegate.MaxSimultaneous
					: defaultMaxSimultaneous;
				AppendDelegate(
					type,
					type.Name,
					null,
					maxSimultaneous,
					builders);
			}
		}

		static void AppendDelegate(
			Type type,
			string cppTypeName,
			Type[] typeParams,
			int maxSimultaneous,
			StringBuilders builders)
		{
			builders.TempStrBuilder.Length = 0;
			AppendNamespace(
				type.Namespace,
				string.Empty,
				builders.TempStrBuilder);
			AppendTypeNameWithoutSuffixes(
				type.Name,
				builders.TempStrBuilder);
			AppendTypeNames(
				typeParams,
				builders.TempStrBuilder);
			string bindingTypeName = builders.TempStrBuilder.ToString();
			
			builders.TempStrBuilder.Length = 0;
			builders.TempStrBuilder.Append("Release");
			builders.TempStrBuilder.Append(bindingTypeName);
			string releaseFuncName = builders.TempStrBuilder.ToString();
			
			builders.TempStrBuilder[0] = char.ToLower(
				builders.TempStrBuilder[0]);
			string releaseFuncNameLower = builders.TempStrBuilder.ToString();
			
			builders.TempStrBuilder.Length = 0;
			builders.TempStrBuilder.Append(bindingTypeName);
			builders.TempStrBuilder.Append("Constructor");
			string constructorFuncName = builders.TempStrBuilder.ToString();
			
			builders.TempStrBuilder[0] = char.ToLower(
				builders.TempStrBuilder[0]);
			string constructorFuncNameLower = builders.TempStrBuilder.ToString();
			
			builders.TempStrBuilder.Length = 0;
			builders.TempStrBuilder.Append(bindingTypeName);
			builders.TempStrBuilder.Append("Add");
			string addFuncName = builders.TempStrBuilder.ToString();
			
			builders.TempStrBuilder[0] = char.ToLower(
				builders.TempStrBuilder[0]);
			string addFuncNameLower = builders.TempStrBuilder.ToString();
			
			builders.TempStrBuilder.Length = 0;
			builders.TempStrBuilder.Append(bindingTypeName);
			builders.TempStrBuilder.Append("Remove");
			string removeFuncName = builders.TempStrBuilder.ToString();
			
			builders.TempStrBuilder[0] = char.ToLower(
				builders.TempStrBuilder[0]);
			string removeFuncNameLower = builders.TempStrBuilder.ToString();

			TypeName typeTypeName = GetTypeName(type);

			// C++ type declaration
			int indent = AppendCppTypeDeclaration(
				typeTypeName,
				false,
				typeParams,
				typeParams != null ?
					builders.CppTemplateSpecializationDeclarations :
					builders.CppTypeDeclarations);
			
			ParameterInfo[] addRemoveParams = {
				new ParameterInfo
				{
					Name = "del",
					ParameterType = type,
					DereferencedParameterType = type,
					IsOut = false,
					IsRef = false,
					Kind = TypeKind.Class,
					IsVirtual = true
				}};
			
			ParameterInfo[] releaseParams = {
				new ParameterInfo
				{
					Name = "handle",
					ParameterType = typeof(int),
					DereferencedParameterType = typeof(int),
					IsOut = false,
					IsRef = false,
					Kind = TypeKind.Primitive
				},
				new ParameterInfo
				{
					Name = "classHandle",
					ParameterType = typeof(int),
					DereferencedParameterType = typeof(int),
					IsOut = false,
					IsRef = false,
					Kind = TypeKind.Primitive
				}};
			
			ParameterInfo[] constructorParams = {
				new ParameterInfo
				{
					Name = "cppHandle",
					ParameterType = typeof(int),
					DereferencedParameterType = typeof(int),
					IsOut = false,
					IsRef = false,
					Kind = TypeKind.Primitive
				},
				new ParameterInfo
				{
					Name = "handle",
					ParameterType = typeof(int),
					DereferencedParameterType = typeof(int),
					IsOut = true,
					IsRef = false,
					Kind = TypeKind.Primitive
				},
				new ParameterInfo
				{
					Name = "classHandle",
					ParameterType = typeof(int),
					DereferencedParameterType = typeof(int),
					IsOut = true,
					IsRef = false,
					Kind = TypeKind.Primitive
				}};
			
			AppendCppPointerFreeListStateAndFunctions(
				GetTypeName(cppTypeName, type.Namespace),
				typeParams,
				bindingTypeName,
				builders.CppGlobalStateAndFunctions);

			AppendCppPointerFreeListInit(
				typeParams,
				GetTypeName(cppTypeName, type.Namespace),
				maxSimultaneous,
				bindingTypeName,
				builders.CppInitBody,
				builders.CppInitBodyFirstBoot);

			// C++ type definition (begin)
			AppendCppTypeDefinitionBegin(
				GetTypeName(cppTypeName, type.Namespace),
				TypeKind.Class,
				typeParams,
				GetTypeName(typeof(object)),
				null,
				null,
				false,
				indent,
				builders.CppTypeDefinitions);
			
			// C++ type fields
			AppendIndent(
				indent + 1,
				builders.CppTypeDefinitions);
			builders.CppTypeDefinitions.Append("int32_t CppHandle;\n");
			AppendIndent(
				indent + 1,
				builders.CppTypeDefinitions);
			builders.CppTypeDefinitions.Append("int32_t ClassHandle;\n");
			
			// C++ method declarations
			AppendIndent(
				indent + 1,
				builders.CppTypeDefinitions);
			AppendCppMethodDeclaration(
				cppTypeName,
				false,
				false,
				false,
				null,
				null,
				new ParameterInfo[0],
				builders.CppTypeDefinitions);
			AppendIndent(
				indent + 1,
				builders.CppTypeDefinitions);
			AppendCppMethodDeclaration(
				"operator+=",
				false,
				false,
				false,
				typeof(void),
				null,
				addRemoveParams,
				builders.CppTypeDefinitions);
			AppendIndent(
				indent + 1,
				builders.CppTypeDefinitions);
			AppendCppMethodDeclaration(
				"operator-=",
				false,
				false,
				false,
				typeof(void),
				null,
				addRemoveParams,
				builders.CppTypeDefinitions);
			
			// C++ function pointers
			AppendCppFunctionPointerDefinition(
				releaseFuncName,
				true,
				default(TypeName),
				TypeKind.None,
				releaseParams,
				typeof(void),
				builders.CppFunctionPointers);
			AppendCppFunctionPointerDefinition(
				constructorFuncName,
				true,
				default(TypeName),
				TypeKind.None,
				constructorParams,
				typeof(void),
				builders.CppFunctionPointers);
			AppendCppFunctionPointerDefinition(
				addFuncName,
				false,
				default(TypeName),
				TypeKind.None,
				addRemoveParams,
				typeof(void),
				builders.CppFunctionPointers);
			AppendCppFunctionPointerDefinition(
				removeFuncName,
				false,
				default(TypeName),
				TypeKind.None,
				addRemoveParams,
				typeof(void),
				builders.CppFunctionPointers);
			
			// C++ init params
			AppendCppInitParam(
				releaseFuncNameLower,
				true,
				default(TypeName),
				TypeKind.None,
				releaseParams,
				typeof(void),
				builders.CppInitParams);
			AppendCppInitParam(
				constructorFuncNameLower,
				true,
				default(TypeName),
				TypeKind.None,
				constructorParams,
				typeof(void),
				builders.CppInitParams);
			AppendCppInitParam(
				addFuncNameLower,
				false,
				default(TypeName),
				TypeKind.None,
				addRemoveParams,
				typeof(void),
				builders.CppInitParams);
			AppendCppInitParam(
				removeFuncNameLower,
				false,
				default(TypeName),
				TypeKind.None,
				addRemoveParams,
				typeof(void),
				builders.CppInitParams);
			
			// C++ and C# init params
			AppendCppInitBody(
				releaseFuncName,
				releaseFuncNameLower,
				builders.CppInitBody);
			AppendCppInitBody(
				constructorFuncName,
				constructorFuncNameLower,
				builders.CppInitBody);
			AppendCppInitBody(
				addFuncName,
				addFuncNameLower,
				builders.CppInitBody);
			AppendCppInitBody(
				removeFuncName,
				removeFuncNameLower,
				builders.CppInitBody);
			AppendCsharpInitParam(
				releaseFuncNameLower,
				builders.CsharpInitParams);
			AppendCsharpInitParam(
				constructorFuncNameLower,
				builders.CsharpInitParams);
			AppendCsharpInitParam(
				addFuncNameLower,
				builders.CsharpInitParams);
			AppendCsharpInitParam(
				removeFuncNameLower,
				builders.CsharpInitParams);
			AppendCsharpInitCallArg(
				releaseFuncName,
				builders.CsharpInitCall);
			AppendCsharpInitCallArg(
				constructorFuncName,
				builders.CsharpInitCall);
			AppendCsharpInitCallArg(
				addFuncName,
				builders.CsharpInitCall);
			AppendCsharpInitCallArg(
				removeFuncName,
				builders.CsharpInitCall);
			
			// C++ method definitions (end)
			int cppMethodDefinitionsIndent = AppendNamespaceBeginning(
				type.Namespace,
				builders.CppMethodDefinitions);
			
			AppendCppBaseTypeConstructor(
				bindingTypeName,
				typeTypeName,
				TypeKind.Class,
				cppTypeName,
				typeParams,
				new Type[0],
				new ParameterInfo[0],
				constructorParams,
				true,
				constructorFuncName,
				cppMethodDefinitionsIndent,
				builders.CppMethodDefinitions);
			
			AppendCppBaseTypeNullptrConstructor(
				bindingTypeName,
				typeTypeName,
				typeParams,
				new Type[0],
				true,
				cppMethodDefinitionsIndent,
				builders.CppMethodDefinitions);
			
			AppendCppBaseTypeCopyConstructor(
				bindingTypeName,
				typeTypeName,
				typeParams,
				new Type[0],
				true,
				cppMethodDefinitionsIndent,
				builders.CppMethodDefinitions);

			AppendCppBaseTypeMoveConstructor(
				typeTypeName,
				typeParams,
				new Type[0],
				true,
				cppMethodDefinitionsIndent,
				builders.CppMethodDefinitions);

			AppendCppBaseTypeHandleConstructor(
				bindingTypeName,
				typeTypeName,
				typeParams,
				new Type[0],
				true,
				cppMethodDefinitionsIndent,
				builders.CppMethodDefinitions);

			AppendCppBaseTypeDestructor(
				bindingTypeName,
				typeTypeName,
				typeParams,
				true,
				string.Empty,
				releaseFuncName,
				bindingTypeName,
				cppMethodDefinitionsIndent,
				builders.CppMethodDefinitions);

			AppendCppBaseTypeAssignmentOperatorSameType(
				typeTypeName,
				typeParams,
				true,
				cppMethodDefinitionsIndent,
				builders.CppMethodDefinitions);

			AppendCppBaseTypeAssignmentOperatorNullptr(
				typeTypeName,
				typeParams,
				true,
				releaseFuncName,
				cppMethodDefinitionsIndent,
				builders.CppMethodDefinitions);

			AppendCppBaseTypeMoveAssignmentOperator(
				bindingTypeName,
				typeTypeName,
				typeParams,
				true,
				releaseFuncName,
				cppMethodDefinitionsIndent,
				builders.CppMethodDefinitions);

			AppendCppBaseTypeEqualityOperator(
				typeTypeName,
				typeParams,
				cppMethodDefinitionsIndent,
				true,
				builders.CppMethodDefinitions);

			AppendCppBaseTypeInequalityOperator(
				typeTypeName,
				typeParams,
				cppMethodDefinitionsIndent,
				true,
				builders.CppMethodDefinitions);

			// C++ add
			AppendCppMethodDefinitionBegin(
				GetTypeName(type),
				typeof(void),
				"operator+=",
				typeParams,
				null,
				addRemoveParams,
				indent,
				builders.CppMethodDefinitions);
			AppendIndent(
				indent,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("{\n");
			AppendIndent(
				indent + 1,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("Plugin::");
			builders.CppMethodDefinitions.Append(addFuncName);
			builders.CppMethodDefinitions.Append("(Handle, del.Handle);\n");
			AppendCppUnhandledExceptionHandling(
				indent + 1,
				builders.CppMethodDefinitions);
			AppendIndent(
				indent,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("}\n");
			AppendIndent(
				indent,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append('\n');
			
			// C++ remove
			AppendCppMethodDefinitionBegin(
				GetTypeName(type),
				typeof(void),
				"operator-=",
				typeParams,
				null,
				addRemoveParams,
				indent,
				builders.CppMethodDefinitions);
			AppendIndent(
				indent,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("{\n");
			AppendIndent(
				indent + 1,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("Plugin::");
			builders.CppMethodDefinitions.Append(removeFuncName);
			builders.CppMethodDefinitions.Append("(Handle, del.Handle);\n");
			AppendCppUnhandledExceptionHandling(
				indent + 1,
				builders.CppMethodDefinitions);
			AppendIndent(
				indent,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("}\n");
			AppendIndent(
				indent,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append('\n');
			
			// C# GetDelegate call
			AppendCsharpGetDelegateCall(
				GetTypeName(type),
				typeParams,
				"NativeInvoke",
				builders.CsharpGetDelegateCalls);
			
			// C# class (beginning)
			builders.CsharpBaseTypes.Append("class ");
			builders.CsharpBaseTypes.Append(bindingTypeName);
			builders.CsharpBaseTypes.Append('\n');
			builders.CsharpBaseTypes.Append("{\n");
			
			// C# class fields
			builders.CsharpBaseTypes.Append("\tpublic int CppHandle;\n");
			builders.CsharpBaseTypes.Append("\tpublic ");
			AppendCsharpTypeFullName(
				type,
				builders.CsharpBaseTypes);
			builders.CsharpBaseTypes.Append(" Delegate;\n");
			builders.CsharpBaseTypes.Append("\t\n");
			
			// C# class constructor
			builders.CsharpBaseTypes.Append("\tpublic ");
			builders.CsharpBaseTypes.Append(bindingTypeName);
			builders.CsharpBaseTypes.Append("(int cppHandle)\n");
			builders.CsharpBaseTypes.Append("\t{\n");
			builders.CsharpBaseTypes.Append("\t\tCppHandle = cppHandle;\n");
			builders.CsharpBaseTypes.Append("\t\tDelegate = NativeInvoke;\n");
			builders.CsharpBaseTypes.Append("\t}\n");
			builders.CsharpBaseTypes.Append("\t\n");
			
			// Build the name of the C++ binding function that C# calls
			builders.TempStrBuilder.Length = 0;
			AppendNativeInvokeFuncName(
				type,
				typeParams,
				"NativeInvoke",
				builders.TempStrBuilder);
			string nativeInvokeFuncName = builders.TempStrBuilder.ToString();
			
			// operator() is how C# forwards the delegate invocation to C++
			MethodInfo invokeMethod = type.GetMethod("Invoke");
			AppendBaseTypeCppMethodCall(
				type,
				bindingTypeName,
				typeTypeName,
				typeParams,
				invokeMethod,
				"NativeInvoke",
				nativeInvokeFuncName,
				"operator()",
				false,
				true,
				indent,
				builders);
			
			// C# class (ending)
			builders.CsharpBaseTypes.Append("}\n");
			builders.CsharpBaseTypes.Append('\n');
			
			// Invoke() is how C++ invokes the delegate
			AppendBaseTypeMethodCallsCsharpMethod(
				type,
				bindingTypeName,
				typeParams,
				invokeMethod,
				"Invoke",
				null,
				indent,
				builders);
			
			// C# constructor delegate type
			AppendCsharpDelegateType(
				constructorFuncName,
				true,
				type,
				TypeKind.Class,
				typeof(void),
				constructorParams,
				builders.CsharpDelegateTypes);
			
			AppendCsharpBaseTypeConstructorFunction(
				type,
				GetTypeName(bindingTypeName, string.Empty),
				false,
				constructorFuncName,
				constructorParams,
				new ParameterInfo[0],
				builders.CsharpFunctions);

			// C# release delegate type
			AppendCsharpDelegateType(
				releaseFuncName,
				true,
				type,
				TypeKind.Class,
				typeof(void),
				releaseParams,
				builders.CsharpDelegateTypes);
			
			AppendCsharpBaseTypeReleaseFunction(
				type,
				GetTypeName(bindingTypeName, string.Empty),
				true,
				releaseFuncName,
				null,
				releaseParams,
				builders.CsharpFunctions);

			// C# add delegate type
			AppendCsharpDelegateType(
				addFuncName,
				false,
				type,
				TypeKind.Class,
				typeof(void),
				addRemoveParams,
				builders.CsharpDelegateTypes);
			
			// C# add function
			AppendCsharpFunctionBeginning(
				type,
				addFuncName,
				false,
				TypeKind.Class,
				typeof(void),
				addRemoveParams,
				builders.CsharpFunctions);
			builders.CsharpFunctions.Append("thiz += del;");
			AppendCsharpFunctionReturn(
				addRemoveParams,
				typeof(void),
				TypeKind.Class,
				null,
				false,
				builders.CsharpFunctions);
			
			// C# remove delegate type
			AppendCsharpDelegateType(
				removeFuncName,
				false,
				type,
				TypeKind.Class,
				typeof(void),
				addRemoveParams,
				builders.CsharpDelegateTypes);
			
			// C# remove function
			AppendCsharpFunctionBeginning(
				type,
				removeFuncName,
				false,
				TypeKind.Class,
				typeof(void),
				addRemoveParams,
				builders.CsharpFunctions);
			builders.CsharpFunctions.Append("thiz -= del;");
			AppendCsharpFunctionReturn(
				addRemoveParams,
				typeof(void),
				TypeKind.Class,
				null,
				false,
				builders.CsharpFunctions);
			
			// C++ method definitions (end)
			AppendCppMethodDefinitionsEnd(
				indent,
				builders.CppMethodDefinitions);
			
			// C++ type definition (end)
			AppendCppTypeDefinitionEnd(
				false,
				indent,
				builders.CppTypeDefinitions);
		}
		
		static void AppendBaseType(
			Type type,
			JsonBaseType jsonBaseType,
			TypeName baseTypeTypeName,
			Type[] typeParams,
			int maxSimultaneous,
			Assembly[] assemblies,
			StringBuilders builders)
		{
			// Get specified derived type name
			TypeName derivedTypeTypeName = SplitJsonTypeName(
				jsonBaseType.DerivedName);

			builders.TempStrBuilder.Length = 0;
			builders.TempStrBuilder.Append("Release");
			builders.TempStrBuilder.Append(baseTypeTypeName.Name);
			string releaseFuncName = builders.TempStrBuilder.ToString();
			
			builders.TempStrBuilder[0] = char.ToLower(
				builders.TempStrBuilder[0]);
			string releaseFuncNameLower = builders.TempStrBuilder.ToString();

			builders.TempStrBuilder.Length = 0;
			AppendCppTypeName(
				baseTypeTypeName,
				builders.TempStrBuilder);
			string cppBaseTypeName = builders.TempStrBuilder.ToString();

			bool hasDefaultConstructor = !type.IsClass ||
				(type.GetConstructor(new Type[0]) != null ||
					type.GetConstructors().Length == 0);

			// Either use specified constructors, the default constructor, or
			// nothing in the case of MonoBehaviour (where you can't call 'new')
			JsonConstructor[] jsonConstructors = jsonBaseType.Constructors;
			if (jsonConstructors == null)
			{
				// Base classes must have a default constructor or no
				// constructors at all
				if (!hasDefaultConstructor)
				{
					// Throw an exception so the user knows what to fix in the JSON
					StringBuilder errorBuilder = new StringBuilder(1024);
					errorBuilder.Append("Base type \"");
					AppendCsharpTypeFullName(
						type,
						errorBuilder);
					errorBuilder.Append(
						")\" doesn't have any specified constructors or a default constructor");
					throw new Exception(errorBuilder.ToString());
				}
				
				jsonConstructors = new JsonConstructor[]
				{
					new JsonConstructor
					{
						ParamTypes = new string[0]
					}
				};
			}
			
			// Build constructor function names and parameter lists
			int numConstructors = jsonConstructors.Length;
			string[] constructorFuncNames = new string[numConstructors];
			string[] constructorFuncNameLowers = new string[numConstructors];
			ParameterInfo[][] cppConstructorParams = new ParameterInfo[numConstructors][];
			ParameterInfo[][] constructorParams = new ParameterInfo[numConstructors][];
			for (int i = 0; i < numConstructors; ++i)
			{
				JsonConstructor jsonCtor = jsonConstructors[i];
				Type[] paramTypes = GetTypes(
					jsonCtor.ParamTypes,
					assemblies);
				
				builders.TempStrBuilder.Length = 0;
				builders.TempStrBuilder.Append(baseTypeTypeName.Name);
				builders.TempStrBuilder.Append("Constructor");
				AppendTypeNames(
					paramTypes,
					builders.TempStrBuilder);
				string constructorFuncName = builders.TempStrBuilder.ToString();
				
				builders.TempStrBuilder[0] = char.ToLower(
					builders.TempStrBuilder[0]);
				string constructorFuncNameLower = builders.TempStrBuilder.ToString();
				
				ParameterInfo[] parameters = GetConstructorParameters(
					type,
					true,
					jsonCtor.ParamTypes);
				int numParams = parameters.Length;
				ParameterInfo[] fullParams = new ParameterInfo[numParams + 2];
				fullParams[0] = new ParameterInfo
				{
					Name = "cppHandle",
					ParameterType = typeof(int),
					DereferencedParameterType = typeof(int),
					IsOut = false,
					IsRef = false,
					Kind = TypeKind.Primitive
				};
				fullParams[1] = new ParameterInfo
				{
					Name = "handle",
					ParameterType = typeof(int),
					DereferencedParameterType = typeof(int),
					IsOut = true,
					IsRef = false,
					Kind = TypeKind.Primitive
				};
				Array.Copy(
					parameters,
					0,
					fullParams,
					2,
					numParams);
				
				constructorFuncNames[i] = constructorFuncName;
				constructorFuncNameLowers[i] = constructorFuncNameLower;
				cppConstructorParams[i] = parameters;
				constructorParams[i] = fullParams;
			}
			
			// Determine what the C++ class should derive from
			Type cppBaseClass;
			Type[] cppBaseClassTypeParams;
			Type[] cppCtorInitTypes = GetCppCtorInitTypes(
				type,
				true);
			Type[] cppInterfaceTypes;
			if (type.IsInterface)
			{
				cppBaseClass = typeof(object);
				cppBaseClassTypeParams = null;
				cppInterfaceTypes = new Type[] { type };
			}
			else
			{
				cppBaseClass = type;
				cppBaseClassTypeParams = typeParams;
				cppInterfaceTypes = new Type[0];
			}

			AppendCppPointerFreeListStateAndFunctions(
				baseTypeTypeName,
				null,
				baseTypeTypeName.Name,
				builders.CppGlobalStateAndFunctions);

			AppendCppPointerFreeListInit(
				null,
				baseTypeTypeName,
				maxSimultaneous,
				baseTypeTypeName.Name,
				builders.CppInitBody,
				builders.CppInitBodyFirstBoot);
			
			// C++ type declaration
			int indent = AppendCppTypeDeclaration(
				baseTypeTypeName,
				false,
				null,
				builders.CppTypeDeclarations);
			
			ParameterInfo[] releaseParams = {
				new ParameterInfo
				{
					Name = "handle",
					ParameterType = typeof(int),
					DereferencedParameterType = typeof(int),
					IsOut = false,
					IsRef = false,
					Kind = TypeKind.Primitive
				}};
			
			// C++ type definition (begin)
			AppendCppTypeDefinitionBegin(
				baseTypeTypeName,
				TypeKind.Class,
				null,
				GetTypeName(cppBaseClass),
				cppBaseClassTypeParams,
				cppInterfaceTypes,
				false,
				indent,
				builders.CppTypeDefinitions);
			
			// C++ type fields
			AppendIndent(
				indent + 1,
				builders.CppTypeDefinitions);
			builders.CppTypeDefinitions.Append("int32_t CppHandle;\n");
			
			// C++ constructor declarations
			for (int i = 0; i < numConstructors; ++i)
			{
				AppendIndent(
					indent + 1,
					builders.CppTypeDefinitions);
				AppendCppMethodDeclaration(
					cppBaseTypeName,
					false,
					false,
					false,
					null,
					null,
					cppConstructorParams[i],
					builders.CppTypeDefinitions);
			}

			// C++ constructor declaration macro
			builders.CppMacros.Append("#define ");
			AppendUppercaseWithUnderscores(
				derivedTypeTypeName.Namespace,
				builders.CppMacros);
			builders.CppMacros.Append('_');
			AppendUppercaseWithUnderscores(
				derivedTypeTypeName.Name,
				builders.CppMacros);
			builders.CppMacros.Append("_DEFAULT_CONSTRUCTOR_DECLARATION \\\n");
			AppendIndent(indent, builders.CppMacros);
			builders.CppMacros.Append(derivedTypeTypeName.Name);
			builders.CppMacros.Append("(Plugin::InternalUse iu, int32_t handle);\n");
			builders.CppMacros.Append('\n');

			// C++ constructor definition macro
			builders.CppMacros.Append("#define ");
			AppendUppercaseWithUnderscores(
				derivedTypeTypeName.Namespace,
				builders.CppMacros);
			builders.CppMacros.Append('_');
			AppendUppercaseWithUnderscores(
				derivedTypeTypeName.Name,
				builders.CppMacros);
			builders.CppMacros.Append("_DEFAULT_CONSTRUCTOR_DEFINITION \\\n");
			AppendIndent(indent, builders.CppMacros);
			builders.CppMacros.Append(derivedTypeTypeName.Name);
			builders.CppMacros.Append("::");
			builders.CppMacros.Append(derivedTypeTypeName.Name);
			builders.CppMacros.Append("(Plugin::InternalUse iu, int32_t handle) \\\n");
			AppendCppConstructorInitializerList(
				cppCtorInitTypes,
				indent + 1,
				builders.CppMacros,
				" \\\n");
			AppendIndent(indent + 1, builders.CppMacros);
			builders.CppMacros.Append(", ");
			AppendCppTypeFullName(
				baseTypeTypeName,
				builders.CppMacros);
			builders.CppMacros.Append("(iu, handle)\n");

			// C++ constructor inline definition macro
			builders.CppMacros.Append("#define ");
			AppendUppercaseWithUnderscores(
				derivedTypeTypeName.Namespace,
				builders.CppMacros);
			builders.CppMacros.Append('_');
			AppendUppercaseWithUnderscores(
				derivedTypeTypeName.Name,
				builders.CppMacros);
			builders.CppMacros.Append("_DEFAULT_CONSTRUCTOR \\\n");
			AppendIndent(indent, builders.CppMacros);
			builders.CppMacros.Append(derivedTypeTypeName.Name);
			builders.CppMacros.Append("(Plugin::InternalUse iu, int32_t handle) \\\n");
			AppendCppConstructorInitializerList(
				cppCtorInitTypes,
				indent + 1,
				builders.CppMacros,
				" \\\n");
			AppendIndent(indent + 1, builders.CppMacros);
			builders.CppMacros.Append(", ");
			AppendCppTypeFullName(
				baseTypeTypeName,
				builders.CppMacros);
			builders.CppMacros.Append("(iu, handle) \\\n");
			AppendIndent(indent, builders.CppMacros);
			builders.CppMacros.Append("{ \\\n");
			AppendIndent(indent, builders.CppMacros);
			builders.CppMacros.Append("} \\\n");
			builders.CppMacros.Append('\n');

			// C++ function pointers
			AppendCppFunctionPointerDefinition(
				releaseFuncName,
				true,
				default(TypeName),
				TypeKind.None,
				releaseParams,
				typeof(void),
				builders.CppFunctionPointers);
			for (int i = 0; i < numConstructors; ++i)
			{
				AppendCppFunctionPointerDefinition(
					constructorFuncNames[i],
					true,
					default(TypeName),
					TypeKind.None,
					constructorParams[i],
					typeof(void),
					builders.CppFunctionPointers);
			}
			
			// C++ init params
			AppendCppInitParam(
				releaseFuncNameLower,
				true,
				default(TypeName),
				TypeKind.None,
				releaseParams,
				typeof(void),
				builders.CppInitParams);
			for (int i = 0; i < numConstructors; ++i)
			{
				AppendCppInitParam(
					constructorFuncNameLowers[i],
					true,
					default(TypeName),
					TypeKind.None,
					constructorParams[i],
					typeof(void),
					builders.CppInitParams);
			}
			
			// C++ and C# init params
			AppendCppInitBody(
				releaseFuncName,
				releaseFuncNameLower,
				builders.CppInitBody);
			AppendCsharpInitParam(
				releaseFuncNameLower,
				builders.CsharpInitParams);
			AppendCsharpInitCallArg(
				releaseFuncName,
				builders.CsharpInitCall);
			for (int i = 0; i < numConstructors; ++i)
			{
				string funcName = constructorFuncNames[i];
				string funcNameLower = constructorFuncNameLowers[i];
				AppendCppInitBody(
					funcName,
					funcNameLower,
					builders.CppInitBody);
				AppendCsharpInitParam(
					funcNameLower,
					builders.CsharpInitParams);
				AppendCsharpInitCallArg(
					funcName,
					builders.CsharpInitCall);
			}
			
			// C++ method definitions (end)
			int cppMethodDefinitionsIndent = AppendNamespaceBeginning(
				baseTypeTypeName.Namespace,
				builders.CppMethodDefinitions);
			
			for (int i = 0; i < numConstructors; ++i)
			{
				AppendCppBaseTypeConstructor(
					baseTypeTypeName.Name,
					baseTypeTypeName,
					TypeKind.Class,
					cppBaseTypeName,
					typeParams,
					cppCtorInitTypes,
					cppConstructorParams[i],
					constructorParams[i],
					false,
					constructorFuncNames[i],
					cppMethodDefinitionsIndent,
					builders.CppMethodDefinitions);
			}
			
			AppendCppBaseTypeNullptrConstructor(
				baseTypeTypeName.Name,
				baseTypeTypeName,
				typeParams,
				cppCtorInitTypes,
				false,
				cppMethodDefinitionsIndent,
				builders.CppMethodDefinitions);
			
			AppendCppBaseTypeCopyConstructor(
				baseTypeTypeName.Name,
				baseTypeTypeName,
				typeParams,
				cppCtorInitTypes,
				false,
				cppMethodDefinitionsIndent,
				builders.CppMethodDefinitions);

			AppendCppBaseTypeMoveConstructor(
				baseTypeTypeName,
				typeParams,
				cppCtorInitTypes,
				false,
				cppMethodDefinitionsIndent,
				builders.CppMethodDefinitions);

			AppendCppBaseTypeHandleConstructor(
				baseTypeTypeName.Name,
				baseTypeTypeName,
				typeParams,
				cppCtorInitTypes,
				false,
				cppMethodDefinitionsIndent,
				builders.CppMethodDefinitions);
			
			AppendCppBaseTypeDestructor(
				baseTypeTypeName.Name,
				baseTypeTypeName,
				typeParams,
				false,
				derivedTypeTypeName.Name,
				releaseFuncName,
				baseTypeTypeName.Name,
				cppMethodDefinitionsIndent,
				builders.CppMethodDefinitions);

			AppendCppBaseTypeAssignmentOperatorSameType(
				baseTypeTypeName,
				typeParams,
				false,
				cppMethodDefinitionsIndent,
				builders.CppMethodDefinitions);

			AppendCppBaseTypeAssignmentOperatorNullptr(
				baseTypeTypeName,
				typeParams,
				false,
				releaseFuncName,
				cppMethodDefinitionsIndent,
				builders.CppMethodDefinitions);

			AppendCppBaseTypeMoveAssignmentOperator(
				baseTypeTypeName.Name,
				baseTypeTypeName,
				typeParams,
				false,
				releaseFuncName,
				cppMethodDefinitionsIndent,
				builders.CppMethodDefinitions);

			AppendCppBaseTypeEqualityOperator(
				baseTypeTypeName,
				typeParams,
				cppMethodDefinitionsIndent,
				false,
				builders.CppMethodDefinitions);

			AppendCppBaseTypeInequalityOperator(
				baseTypeTypeName,
				typeParams,
				cppMethodDefinitionsIndent,
				false,
				builders.CppMethodDefinitions);

			if (!string.IsNullOrEmpty(derivedTypeTypeName.Name))
			{
				// C++ whole object free list
				AppendCppWholeObjectFreeListStateAndFunctions(
					null,
					baseTypeTypeName,
					baseTypeTypeName.Name,
					builders.CppGlobalStateAndFunctions);
				AppendCppWholeObjectFreeListInit(
					maxSimultaneous,
					baseTypeTypeName.Name,
					builders.CppInitBody,
					builders.CppInitBodyFirstBoot);
				
				// C++ binding function to create the base class
				builders.TempStrBuilder.Length = 0;
				builders.TempStrBuilder.Append("New");
				builders.TempStrBuilder.Append(baseTypeTypeName.Name);
				string cppDefaultConstructorBindingFunctionName = builders.TempStrBuilder.ToString();
				AppendIndent(
					indent,
					builders.CppMethodDefinitions);
				builders.CppMethodDefinitions.Append("DLLEXPORT int32_t ");
				builders.CppMethodDefinitions.Append(cppDefaultConstructorBindingFunctionName);
				builders.CppMethodDefinitions.Append("(int32_t handle)\n");
				AppendIndent(
					indent,
					builders.CppMethodDefinitions);
				builders.CppMethodDefinitions.Append("{\n");
				AppendIndent(
					indent + 1,
					builders.CppMethodDefinitions);
				AppendCppTypeFullName(
					baseTypeTypeName,
					builders.CppMethodDefinitions);
				builders.CppMethodDefinitions.Append("* memory = Plugin::StoreWhole");
				builders.CppMethodDefinitions.Append(baseTypeTypeName.Name);
				builders.CppMethodDefinitions.Append("();\n");
				AppendIndent(
					indent + 1,
					builders.CppMethodDefinitions);
				AppendCppTypeFullName(
					derivedTypeTypeName,
					builders.CppMethodDefinitions);
				builders.CppMethodDefinitions.Append("* thiz = new (memory) ");
				AppendCppTypeFullName(
					derivedTypeTypeName,
					builders.CppMethodDefinitions);
				builders.CppMethodDefinitions.Append("(Plugin::InternalUse::Only, handle);\n");
				AppendIndent(
					indent + 1,
					builders.CppMethodDefinitions);
				builders.CppMethodDefinitions.Append("return thiz->CppHandle;\n");
				AppendIndent(
					indent,
					builders.CppMethodDefinitions);
				builders.CppMethodDefinitions.Append("}\n\n");

				// C# usage of the C++ binding function to create from C# default constructor
				ParameterInfo[] cppDefaultConstructorBindingFunctionParams = ConvertParameters(
					new Type[] { typeof(int) });
				AppendCsharpDelegate(
					true,
					GetTypeName(string.Empty, string.Empty),
					null,
					cppDefaultConstructorBindingFunctionName,
					cppDefaultConstructorBindingFunctionParams,
					typeof(int),
					TypeKind.None,
					builders.CsharpDelegates);
				AppendCsharpImport(
					GetTypeName(string.Empty, string.Empty),
					null,
					cppDefaultConstructorBindingFunctionName,
					ConvertParameters(Type.EmptyTypes),
					typeof(int),
					builders.CsharpImports);
				AppendCsharpGetDelegateCall(
					GetTypeName(string.Empty, string.Empty),
					null,
					cppDefaultConstructorBindingFunctionName,
					builders.CsharpGetDelegateCalls);

				// C++ binding function to destroy the base class
				builders.TempStrBuilder.Length = 0;
				builders.TempStrBuilder.Append("Destroy");
				builders.TempStrBuilder.Append(baseTypeTypeName.Name);
				string cppDestroyBindingFunctionName = builders.TempStrBuilder.ToString();
				AppendIndent(
					indent,
					builders.CppMethodDefinitions);
				builders.CppMethodDefinitions.Append("DLLEXPORT void ");
				builders.CppMethodDefinitions.Append(cppDestroyBindingFunctionName);
				builders.CppMethodDefinitions.Append("(int32_t cppHandle)\n");
				AppendIndent(
					indent,
					builders.CppMethodDefinitions);
				builders.CppMethodDefinitions.Append("{\n");
				AppendIndent(
					indent + 1,
					builders.CppMethodDefinitions);
				AppendCppTypeFullName(
					baseTypeTypeName,
					builders.CppMethodDefinitions);
				builders.CppMethodDefinitions.Append("* instance = Plugin::Get");
				builders.CppMethodDefinitions.Append(baseTypeTypeName.Name);
				builders.CppMethodDefinitions.Append("(cppHandle);\n");
				AppendIndent(
					indent + 1,
					builders.CppMethodDefinitions);
				builders.CppMethodDefinitions.Append("instance->~");
				AppendCppTypeName(
					baseTypeTypeName,
					builders.CppMethodDefinitions);
				builders.CppMethodDefinitions.Append("();\n");
				AppendIndent(
					indent,
					builders.CppMethodDefinitions);
				builders.CppMethodDefinitions.Append("}\n\n");

				// C# usage of the C++ binding function to destroy from C# default constructor
				ParameterInfo[] cppDestroyBindingFunctionParams = ConvertParameters(
					new Type[] { typeof(int) });
				AppendCsharpDelegate(
					true,
					GetTypeName(string.Empty, string.Empty),
					null,
					cppDestroyBindingFunctionName,
					cppDestroyBindingFunctionParams,
					typeof(void),
					TypeKind.None,
					builders.CsharpDelegates);
				ParameterInfo[] cppDestroyImportFunctionParams = ConvertParameters(
					new Type[0]);
				AppendCsharpImport(
					GetTypeName(string.Empty, string.Empty),
					null,
					cppDestroyBindingFunctionName,
					cppDestroyImportFunctionParams,
					typeof(void),
					builders.CsharpImports);
				AppendCsharpGetDelegateCall(
					GetTypeName(string.Empty, string.Empty),
					null,
					cppDestroyBindingFunctionName,
					builders.CsharpGetDelegateCalls);
				
				// C# DestroyFunction enumerator
				builders.CsharpDestroyFunctionEnumerators.Append("\t\t\t");
				builders.CsharpDestroyFunctionEnumerators.Append(baseTypeTypeName.Name);
				builders.CsharpDestroyFunctionEnumerators.Append(",\n");

				// C# Destroy queue cases
				builders.CsharpDestroyQueueCases.Append("\t\t\t\t\t\tcase DestroyFunction.");
				builders.CsharpDestroyQueueCases.Append(baseTypeTypeName.Name);
				builders.CsharpDestroyQueueCases.Append(":\n");
				builders.CsharpDestroyQueueCases.Append("\t\t\t\t\t\t\t");
				builders.CsharpDestroyQueueCases.Append(cppDestroyBindingFunctionName);
				builders.CsharpDestroyQueueCases.Append("(entry.CppHandle);\n");
				builders.CsharpDestroyQueueCases.Append("\t\t\t\t\t\t\tbreak;\n");
			}

			// C# class (beginning)
			builders.CsharpBaseTypes.Append("namespace ");
			builders.CsharpBaseTypes.Append(baseTypeTypeName.Namespace);
			builders.CsharpBaseTypes.Append('\n');
			builders.CsharpBaseTypes.Append("{\n");
			builders.CsharpBaseTypes.Append("\tclass ");
			builders.CsharpBaseTypes.Append(baseTypeTypeName.Name);
			if (jsonBaseType != null)
			{
				builders.CsharpBaseTypes.Append(" : ");
				AppendCsharpTypeFullName(
					type,
					builders.CsharpBaseTypes);
			}
			builders.CsharpBaseTypes.Append('\n');
			builders.CsharpBaseTypes.Append("\t{\n");
			
			// C# class fields
			builders.CsharpBaseTypes.Append("\t\tpublic int CppHandle;\n");
			builders.CsharpBaseTypes.Append("\t\t\n");

			if (derivedTypeTypeName.Name != null)
			{
				// C# class default constructor if the base class has one
				if (hasDefaultConstructor)
				{
					builders.CsharpBaseTypes.Append("\t\tpublic ");
					builders.CsharpBaseTypes.Append(baseTypeTypeName.Name);
					builders.CsharpBaseTypes.Append("()\n");
					builders.CsharpBaseTypes.Append("\t\t{\n");
					builders.CsharpBaseTypes.Append(
						"\t\t\tint handle = NativeScript.Bindings.ObjectStore.Store(this);\n");
					builders.CsharpBaseTypes.Append(
						"\t\t\tCppHandle = NativeScript.Bindings.New");
					builders.CsharpBaseTypes.Append(baseTypeTypeName.Name);
					builders.CsharpBaseTypes.Append("(handle);\n");
					builders.CsharpBaseTypes.Append("\t\t}\n");
					builders.CsharpBaseTypes.Append("\t\t\n");
				}

				// C# finalizer/destructor
				builders.CsharpBaseTypes.Append("\t\t~");
				builders.CsharpBaseTypes.Append(baseTypeTypeName.Name);
				builders.CsharpBaseTypes.Append("()\n");
				builders.CsharpBaseTypes.Append("\t\t{\n");
				builders.CsharpBaseTypes.Append("\t\t\tif (CppHandle != 0)\n");
				builders.CsharpBaseTypes.Append("\t\t\t{\n");
				builders.CsharpBaseTypes.Append(
					"\t\t\t\tNativeScript.Bindings.QueueDestroy(NativeScript.Bindings.DestroyFunction.");
				builders.CsharpBaseTypes.Append(baseTypeTypeName.Name);
				builders.CsharpBaseTypes.Append(", CppHandle);\n");
				builders.CsharpBaseTypes.Append("\t\t\t\tCppHandle = 0;\n");
				builders.CsharpBaseTypes.Append("\t\t\t}\n");
				builders.CsharpBaseTypes.Append("\t\t}\n");
				builders.CsharpBaseTypes.Append("\t\t\n");
			}

			// C# class constructors
			for (int i = 0; i < numConstructors; ++i)
			{
				builders.CsharpBaseTypes.Append("\t\tpublic ");
				builders.CsharpBaseTypes.Append(baseTypeTypeName.Name);
				builders.CsharpBaseTypes.Append("(int cppHandle");
				ParameterInfo[] parameters = cppConstructorParams[i];
				if (parameters.Length > 0)
				{
					builders.CsharpBaseTypes.Append(", ");
					AppendCsharpParams(
						parameters,
						builders.CsharpBaseTypes);
				}
				builders.CsharpBaseTypes.Append(")\n");
				builders.CsharpBaseTypes.Append("\t\t\t: base(");
				AppendCsharpFunctionCallParameters(
					parameters,
					builders.CsharpBaseTypes);
				builders.CsharpBaseTypes.Append(")\n");
				builders.CsharpBaseTypes.Append("\t\t{\n");
				builders.CsharpBaseTypes.Append("\t\t\tCppHandle = cppHandle;\n");
				builders.CsharpBaseTypes.Append("\t\t}\n");
				builders.CsharpBaseTypes.Append("\t\t\n");
			}
			
			// C# constructor delegate type
			for (int i = 0; i < numConstructors; ++i)
			{
				AppendCsharpDelegateType(
					constructorFuncNames[i],
					true,
					type,
					TypeKind.Class,
					typeof(void),
					constructorParams[i],
					builders.CsharpDelegateTypes);
			}
			
			for (int i = 0; i < numConstructors; ++i)
			{
				AppendCsharpBaseTypeConstructorFunction(
					type,
					baseTypeTypeName,
					false,
					constructorFuncNames[i],
					constructorParams[i],
					cppConstructorParams[i],
					builders.CsharpFunctions);
			}

			// C# release delegate type
			AppendCsharpDelegateType(
				releaseFuncName,
				true,
				type,
				TypeKind.Class,
				typeof(void),
				releaseParams,
				builders.CsharpDelegateTypes);
			
			AppendCsharpBaseTypeReleaseFunction(
				type,
				baseTypeTypeName,
				false,
				releaseFuncName,
				jsonBaseType.DerivedName,
				releaseParams,
				builders.CsharpFunctions);
			
			// All abstract methods
			foreach (MethodInfo methodInfo in type.GetMethods())
			{
				// Property methods like "get_X" have a "special name"
				if (methodInfo.IsAbstract && !methodInfo.IsSpecialName)
				{
					AppendBaseTypeNativeMethod(
						type,
						baseTypeTypeName,
						typeParams,
						methodInfo,
						false,
						indent,
						builders);
				}
			}
			
			// All interface methods
			if (type.IsInterface)
			{
				foreach (Type interfaceType in type.GetInterfaces())
				{
					foreach (MethodInfo methodInfo in interfaceType.GetMethods())
					{
						// Property methods like "get_X" have a "special name"
						if (methodInfo.IsAbstract && !methodInfo.IsSpecialName)
						{
							AppendBaseTypeNativeMethod(
								type,
								baseTypeTypeName,
								typeParams,
								methodInfo,
								false,
								indent,
								builders);
						}
					}
				}
			}
			
			// Specified virtual methods
			if (jsonBaseType.OverrideMethods != null)
			{
				MethodInfo[] methods = type.GetMethods();
				Type[] genericArgTypes = type.GetGenericArguments();
				foreach (JsonMethod jsonMethod in jsonBaseType.OverrideMethods)
				{
					if (jsonMethod.GenericParams != null)
					{
						foreach (JsonGenericParams jsonGenericParams in
							jsonMethod.GenericParams)
						{
							MethodInfo methodInfo = GetMethod(
								jsonMethod,
								type,
								typeParams,
								genericArgTypes,
								methods,
								jsonGenericParams.Types);
							AppendBaseTypeNativeMethod(
								type,
								baseTypeTypeName,
								typeParams,
								methodInfo,
								false,
								indent,
								builders);
						}
					}
					else
					{
						MethodInfo methodInfo = GetMethod(
							jsonMethod,
							type,
							typeParams,
							genericArgTypes,
							methods,
							null);
						AppendBaseTypeNativeMethod(
							type,
							baseTypeTypeName,
							typeParams,
							methodInfo,
							false,
							indent,
							builders);
					}
				}
			}
			
			// All abstract properties
			foreach (PropertyInfo propertyInfo in type.GetProperties())
			{
				MethodInfo getMethodInfo = propertyInfo.GetGetMethod();
				MethodInfo setMethodInfo = propertyInfo.GetSetMethod();
				if ((getMethodInfo == null || !getMethodInfo.IsAbstract) &&
					(setMethodInfo == null || !setMethodInfo.IsAbstract))
				{
					continue;
				}
				AppendBaseTypeProperty(
					type,
					baseTypeTypeName.Name,
					baseTypeTypeName,
					typeParams,
					propertyInfo,
					getMethodInfo,
					setMethodInfo,
					indent,
					builders);
			}
			
			// All interface properties
			if (type.IsInterface)
			{
				foreach (Type interfaceType in type.GetInterfaces())
				{
					foreach (PropertyInfo propertyInfo in
						interfaceType.GetProperties())
					{
						MethodInfo getMethodInfo = propertyInfo.GetGetMethod();
						MethodInfo setMethodInfo = propertyInfo.GetSetMethod();
						if ((getMethodInfo == null || !getMethodInfo.IsAbstract) &&
							(setMethodInfo == null || !setMethodInfo.IsAbstract))
						{
							continue;
						}
						AppendBaseTypeProperty(
							type,
							baseTypeTypeName.Name,
							baseTypeTypeName,
							typeParams,
							propertyInfo,
							getMethodInfo,
							setMethodInfo,
							indent,
							builders);
					}
				}
			}
			
			// Specified virtual properties
			if (jsonBaseType.OverrideProperties != null)
			{
				PropertyInfo[] properties = type.GetProperties();
				foreach (JsonProperty jsonProperty in jsonBaseType.OverrideProperties)
				{
					PropertyInfo propertyInfo = null;
					foreach (PropertyInfo curPropertyInfo in properties)
					{
						if (curPropertyInfo.Name == jsonProperty.Name)
						{
							propertyInfo = curPropertyInfo;
							break;
						}
					}
					if (propertyInfo == null)
					{
						// Throw an exception so the user knows what to fix in the JSON
						StringBuilder errorBuilder = new StringBuilder(1024);
						errorBuilder.Append("Property \"");
						AppendCsharpTypeFullName(
							type,
							errorBuilder);
						errorBuilder.Append('.');
						errorBuilder.Append(jsonProperty.Name);
						errorBuilder.Append(")\" not found");
						throw new Exception(errorBuilder.ToString());
					}
					
					MethodInfo getMethodInfo = propertyInfo.GetGetMethod();
					MethodInfo setMethodInfo = propertyInfo.GetSetMethod();
					if ((getMethodInfo == null || !getMethodInfo.IsVirtual) &&
						(setMethodInfo == null || !setMethodInfo.IsVirtual))
					{
						// Throw an exception so the user knows what to fix in the JSON
						StringBuilder errorBuilder = new StringBuilder(1024);
						errorBuilder.Append("Property \"");
						AppendCsharpTypeFullName(
							type,
							errorBuilder);
						errorBuilder.Append('.');
						errorBuilder.Append(jsonProperty.Name);
						errorBuilder.Append(
							")\" doesn't have either a virtual 'get' or 'set' to override");
						throw new Exception(errorBuilder.ToString());
					}
					AppendBaseTypeProperty(
						type,
						baseTypeTypeName.Name,
						baseTypeTypeName,
						typeParams,
						propertyInfo,
						getMethodInfo,
						setMethodInfo,
						indent,
						builders);
				}
			}
			
			// All abstract events
			foreach (EventInfo eventInfo in type.GetEvents())
			{
				MethodInfo addMethodInfo = eventInfo.GetAddMethod();
				MethodInfo removeMethodInfo = eventInfo.GetRemoveMethod();
				if ((addMethodInfo == null || !addMethodInfo.IsAbstract) &&
					(removeMethodInfo == null || !removeMethodInfo.IsAbstract))
				{
					continue;
				}
				AppendBaseTypeEvent(
					type,
					baseTypeTypeName,
					typeParams,
					eventInfo,
					addMethodInfo,
					removeMethodInfo,
					indent,
					builders);
			}
			
			// All interface events
			if (type.IsInterface)
			{
				foreach (Type interfaceType in type.GetInterfaces())
				{
					foreach (EventInfo eventInfo in interfaceType.GetEvents())
					{
						MethodInfo addMethodInfo = eventInfo.GetAddMethod();
						MethodInfo removeMethodInfo = eventInfo.GetRemoveMethod();
						if ((addMethodInfo == null || !addMethodInfo.IsAbstract) &&
							(removeMethodInfo == null || !removeMethodInfo.IsAbstract))
						{
							continue;
						}
						AppendBaseTypeEvent(
							type,
							baseTypeTypeName,
							typeParams,
							eventInfo,
							addMethodInfo,
							removeMethodInfo,
							indent,
							builders);
					}
				}
			}
			
			// Specified virtual events
			if (jsonBaseType.OverrideEvents != null)
			{
				EventInfo[] events = type.GetEvents();
				foreach (JsonEvent jsonEvent in jsonBaseType.OverrideEvents)
				{
					EventInfo eventInfo = null;
					foreach (EventInfo curEventInfo in events)
					{
						if (curEventInfo.Name == jsonEvent.Name)
						{
							eventInfo = curEventInfo;
							break;
						}
					}
					if (eventInfo == null)
					{
						// Throw an exception so the user knows what to fix in the JSON
						StringBuilder errorBuilder = new StringBuilder(1024);
						errorBuilder.Append("Event \"");
						AppendCsharpTypeFullName(
							type,
							errorBuilder);
						errorBuilder.Append('.');
						errorBuilder.Append(jsonEvent.Name);
						errorBuilder.Append(")\" not found");
						throw new Exception(errorBuilder.ToString());
					}
					
					MethodInfo addMethodInfo = eventInfo.GetAddMethod();
					MethodInfo removeMethodInfo = eventInfo.GetRemoveMethod();
					if ((addMethodInfo == null || !addMethodInfo.IsVirtual) &&
						(removeMethodInfo == null || !removeMethodInfo.IsVirtual))
					{
						// Throw an exception so the user knows what to fix in the JSON
						StringBuilder errorBuilder = new StringBuilder(1024);
						errorBuilder.Append("Event \"");
						AppendCsharpTypeFullName(
							type,
							errorBuilder);
						errorBuilder.Append('.');
						errorBuilder.Append(jsonEvent.Name);
						errorBuilder.Append(
							")\" doesn't have either a virtual 'add' or 'remove' to override");
						throw new Exception(errorBuilder.ToString());
					}
					AppendBaseTypeEvent(
						type,
						baseTypeTypeName,
						typeParams,
						eventInfo,
						addMethodInfo,
						removeMethodInfo,
						indent,
						builders);
				}
			}
			
			// C# class (ending)
			builders.CsharpBaseTypes.Append("\t}\n");
			builders.CsharpBaseTypes.Append("}\n");
			builders.CsharpBaseTypes.Append("\n");
			
			// C++ method definitions (end)
			AppendCppMethodDefinitionsEnd(
				indent,
				builders.CppMethodDefinitions);

			// C++ type definition (end)
			AppendCppTypeDefinitionEnd(
				false,
				indent,
				builders.CppTypeDefinitions);
		}
		
		static void AppendBaseTypeNativeMethod(
			Type type,
			TypeName typeTypeName,
			Type[] typeParams,
			MethodInfo methodInfo,
			bool typeIsDelegate,
			int indent,
			StringBuilders builders)
		{
			AppendCsharpGetDelegateCall(
				GetTypeName(type),
				typeParams,
				methodInfo.Name,
				builders.CsharpGetDelegateCalls);
			
			// Build the name of the C++ binding function that C# calls
			builders.TempStrBuilder.Length = 0;
			AppendNativeInvokeFuncName(
				type,
				typeParams,
				methodInfo.Name,
				builders.TempStrBuilder);
			string nativeInvokeFuncName = builders.TempStrBuilder.ToString();
			
			AppendBaseTypeCppMethodCall(
				type,
				typeTypeName.Name,
				typeTypeName,
				typeParams,
				methodInfo,
				methodInfo.Name,
				nativeInvokeFuncName,
				methodInfo.Name,
				IsNonDelegateClass(type),
				typeIsDelegate,
				indent,
				builders);
		}
		
		static void AppendBaseTypeProperty(
			Type type,
			string typeName,
			TypeName typeTypeName,
			Type[] typeParams,
			PropertyInfo propertyInfo,
			MethodInfo getMethodInfo,
			MethodInfo setMethodInfo,
			int indent,
			StringBuilders builders)
		{
			bool isOverride = IsNonDelegateClass(type);
			
			ParameterInfo[] parameters;
			if (getMethodInfo != null && getMethodInfo.IsVirtual)
			{
				parameters = ConvertParameters(
					getMethodInfo.GetParameters());
			}
			else
			{
				System.Reflection.ParameterInfo[] setParams =
					setMethodInfo.GetParameters();
				parameters = ConvertParameters(setParams, 1);
			}
			
			builders.CsharpBaseTypes.Append("\t\tpublic ");
			if (isOverride)
			{
				builders.CsharpBaseTypes.Append("override ");
			}
			AppendCsharpTypeFullName(
				propertyInfo.PropertyType,
				builders.CsharpBaseTypes);
			builders.CsharpBaseTypes.Append(' ');
			if (parameters.Length == 0)
			{
				builders.CsharpBaseTypes.Append(propertyInfo.Name);
			}
			else
			{
				builders.CsharpBaseTypes.Append("this[");
				AppendCsharpParams(
					parameters,
					builders.CsharpBaseTypes);
				builders.CsharpBaseTypes.Append(']');
			}
			builders.CsharpBaseTypes.Append('\n');
			builders.CsharpBaseTypes.Append("\t\t{\n");
			
			TypeKind propertyTypeKind = GetTypeKind(
				propertyInfo.PropertyType);
			
			if (getMethodInfo != null && getMethodInfo.IsVirtual)
			{
				AppendBaseTypeNativePropertyOrEvent(
					type,
					typeName,
					typeParams,
					typeTypeName,
					propertyInfo.Name,
					propertyTypeKind,
					getMethodInfo,
					"Get",
					false,
					indent,
					builders);
			}
			
			if (setMethodInfo != null && setMethodInfo.IsVirtual)
			{
				AppendBaseTypeNativePropertyOrEvent(
					type,
					typeName,
					typeParams,
					typeTypeName,
					propertyInfo.Name,
					propertyTypeKind,
					setMethodInfo,
					"Set",
					false,
					indent,
					builders);
			}
			
			builders.CsharpBaseTypes.Append("\t\t}\n");
			builders.CsharpBaseTypes.Append("\t\t\n");
		}
		
		static void AppendBaseTypeEvent(
			Type type,
			TypeName typeTypeName,
			Type[] typeParams,
			EventInfo eventInfo,
			MethodInfo addMethodInfo,
			MethodInfo removeMethodInfo,
			int indent,
			StringBuilders builders)
		{
			bool isOverride = IsNonDelegateClass(type);
			
			builders.CsharpBaseTypes.Append("\t\tpublic ");
			if (isOverride)
			{
				builders.CsharpBaseTypes.Append("override ");
			}
			builders.CsharpBaseTypes.Append("event ");
			AppendCsharpTypeFullName(
				eventInfo.EventHandlerType,
				builders.CsharpBaseTypes);
			builders.CsharpBaseTypes.Append(' ');
			builders.CsharpBaseTypes.Append(eventInfo.Name);
			builders.CsharpBaseTypes.Append('\n');
			builders.CsharpBaseTypes.Append("\t\t{\n");
			
			TypeKind eventHandlerTypeKind = GetTypeKind(
				eventInfo.EventHandlerType);
			
			if (addMethodInfo != null && addMethodInfo.IsVirtual)
			{
				AppendBaseTypeNativePropertyOrEvent(
					type,
					typeTypeName.Name,
					typeParams,
					typeTypeName,
					eventInfo.Name,
					eventHandlerTypeKind,
					addMethodInfo,
					"Add",
					false,
					indent,
					builders);
			}
			
			if (removeMethodInfo != null && removeMethodInfo.IsVirtual)
			{
				AppendBaseTypeNativePropertyOrEvent(
					type,
					typeTypeName.Name,
					typeParams,
					typeTypeName,
					eventInfo.Name,
					eventHandlerTypeKind,
					removeMethodInfo,
					"Remove",
					false,
					indent,
					builders);
			}
			
			builders.CsharpBaseTypes.Append("\t\t\t}\n");
			builders.CsharpBaseTypes.Append("\t\t\t\n");
		}
		
		static void AppendBaseTypeNativePropertyOrEvent(
			Type type,
			string typeName,
			Type[] typeParams,
			TypeName typeTypeName,
			string propertyOrEventName,
			TypeKind propertyOrEventTypeKind,
			MethodInfo methodInfo,
			string operationType,
			bool typeIsDelegate,
			int indent,
			StringBuilders builders)
		{
			builders.TempStrBuilder.Length = 0;
			builders.TempStrBuilder.Append(operationType);
			builders.TempStrBuilder.Append(propertyOrEventName);
			string funcName = builders.TempStrBuilder.ToString();
			
			AppendCsharpGetDelegateCall(
				GetTypeName(type),
				typeParams,
				funcName,
				builders.CsharpGetDelegateCalls);
			
			// Build the name of the C++ binding function that C# calls
			builders.TempStrBuilder.Length = 0;
			AppendNativeInvokeFuncName(
				type,
				typeParams,
				funcName,
				builders.TempStrBuilder);
			string nativeInvokeFuncName = builders.TempStrBuilder.ToString();
			
			ParameterInfo[] invokeParams = AppendBaseTypeCppNativeInvokeCall(
				type,
				typeName,
				typeTypeName,
				typeParams,
				methodInfo,
				funcName,
				funcName,
				typeIsDelegate,
				indent,
				builders);
			
			// C# method that calls the C++ binding function
			ParameterInfo[] invokeParamsWithThis = PrependThisParameter(
				invokeParams);
			builders.CsharpBaseTypes.Append("\t\t\t");
			builders.CsharpBaseTypes.Append(char.ToLower(operationType[0]));
			builders.CsharpBaseTypes.Append(
				operationType,
				1,
				operationType.Length - 1);
			builders.CsharpBaseTypes.Append('\n');
			builders.CsharpBaseTypes.Append("\t\t\t{\n");
			AppendCsharpBaseTypeCppMethodCallMethodBody(
				methodInfo,
				nativeInvokeFuncName,
				invokeParamsWithThis,
				propertyOrEventTypeKind,
				4,
				builders.CsharpBaseTypes);
			builders.CsharpBaseTypes.Append("\t\t\t}\n");
		}
		
		static void AppendCsharpParams(
			ParameterInfo[] parameters,
			StringBuilder output)
		{
			for (int i = 0; i < parameters.Length; ++i)
			{
				ParameterInfo param = parameters[i];
				AppendCsharpTypeFullName(
					param.ParameterType,
					output);
				output.Append(' ');
				output.Append(param.Name);
				if (i != parameters.Length - 1)
				{
					output.Append(", ");
				}
			}
		}

		static void AppendBaseTypeMethodCallsCsharpMethod(
			Type type,
			string typeName,
			Type[] typeParams,
			MethodInfo methodInfo,
			string methodName,
			string csharpMethodName,
			int indent,
			StringBuilders builders)
		{
			builders.TempStrBuilder.Length = 0;
			builders.TempStrBuilder.Append(typeName);
			builders.TempStrBuilder.Append(methodName);
			string funcName = builders.TempStrBuilder.ToString();
			
			builders.TempStrBuilder[0] = char.ToLower(
				builders.TempStrBuilder[0]);
			string funcNameLower = builders.TempStrBuilder.ToString();
			
			// C++ method declaration for the method
			ParameterInfo[] invokeParams = ConvertParameters(
				methodInfo.GetParameters());
			AppendIndent(
				indent + 1,
				builders.CppTypeDefinitions);
			AppendCppMethodDeclaration(
				methodName,
				false,
				false,
				false,
				methodInfo.ReturnType,
				null,
				invokeParams,
				builders.CppTypeDefinitions);
			
			// C++ function pointer for the C# binding function
			AppendCppFunctionPointerDefinition(
				funcName,
				false,
				default(TypeName),
				TypeKind.None,
				invokeParams,
				methodInfo.ReturnType,
				builders.CppFunctionPointers);
			
			// C++ and C# Init parameter and body for the C# binding function
			AppendCppInitParam(
				funcNameLower,
				false,
				default(TypeName),
				TypeKind.None,
				invokeParams,
				methodInfo.ReturnType,
				builders.CppInitParams);
			AppendCppInitBody(
				funcName,
				funcNameLower,
				builders.CppInitBody);
			AppendCsharpInitParam(
				funcNameLower,
				builders.CsharpInitParams);
			AppendCsharpInitCallArg(
				funcName,
				builders.CsharpInitCall);
			
			// C++ method definition for the method
			TypeKind returnTypeKind = GetTypeKind(
				methodInfo.ReturnType);
			AppendCppMethodDefinitionBegin(
				GetTypeName(type),
				methodInfo.ReturnType,
				methodName,
				typeParams,
				null,
				invokeParams,
				indent,
				builders.CppMethodDefinitions);
			AppendIndent(
				indent,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("{\n");
			AppendCppPluginFunctionCall(
				false,
				GetTypeName(type),
				TypeKind.Class,
				typeParams,
				methodInfo.ReturnType,
				funcName,
				invokeParams,
				indent + 1,
				builders.CppMethodDefinitions);
			AppendCppMethodReturn(
				methodInfo.ReturnType,
				returnTypeKind,
				indent + 1,
				builders.CppMethodDefinitions);
			AppendIndent(
				indent,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("}\n");
			AppendIndent(
				indent,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append('\n');
			
			// C# delegate type for the binding function that C++ calls
			ParameterInfo[] invokeParamsWithThis = new ParameterInfo[
				invokeParams.Length + 1];
			for (int i = 0; i < invokeParams.Length; ++i)
			{
				invokeParamsWithThis[i+1] = invokeParams[i];
			}
			invokeParamsWithThis[0] = new ParameterInfo {
				Name = "thisHandle",
				ParameterType = typeof(int),
				DereferencedParameterType = typeof(int),
				IsOut = false,
				IsRef = false,
				Kind = TypeKind.Primitive
			};
			AppendCsharpDelegateType(
				funcName,
				true,
				type,
				TypeKind.Class,
				methodInfo.ReturnType,
				invokeParamsWithThis,
				builders.CsharpDelegateTypes);
			
			// C# binding function that C++ calls to invoke the method
			AppendCsharpFunctionBeginning(
				type,
				funcName,
				true,
				TypeKind.Class,
				methodInfo.ReturnType,
				invokeParamsWithThis,
				builders.CsharpFunctions);
			builders.CsharpFunctions.Append("((");
			AppendCsharpTypeFullName(
				type,
				builders.CsharpFunctions);
			builders.CsharpFunctions.Append(
				")NativeScript.Bindings.ObjectStore.Get(thisHandle))");
			if (csharpMethodName != null)
			{
				builders.CsharpFunctions.Append('.');
				builders.CsharpFunctions.Append(csharpMethodName);
			}
			builders.CsharpFunctions.Append('(');
			AppendCsharpFunctionCallParameters(
				invokeParams,
				builders.CsharpFunctions);
			builders.CsharpFunctions.Append(");");
			AppendCsharpFunctionReturn(
				invokeParams,
				methodInfo.ReturnType,
				returnTypeKind,
				null,
				false,
				builders.CsharpFunctions);
		}
		
		static void AppendNativeInvokeFuncName(
			Type type,
			Type[] typeParams,
			string funcName,
			StringBuilder output)
		{
			AppendNamespace(
				type.Namespace,
				string.Empty,
				output);
			AppendTypeNameWithoutSuffixes(
				type.Name,
				output);
			AppendTypeNames(
				typeParams,
				output);
			output.Append(funcName);
		}
		
		static void AppendBaseTypeCppMethodCall(
			Type type,
			string typeName,
			TypeName typeTypeName,
			Type[] typeParams,
			MethodInfo invokeMethod,
			string funcName,
			string nativeInvokeFuncName,
			string methodName,
			bool isOverride,
			bool typeIsDelegate,
			int indent,
			StringBuilders builders)
		{
			ParameterInfo[] invokeParams = AppendBaseTypeCppNativeInvokeCall(
				type,
				typeName,
				typeTypeName,
				typeParams,
				invokeMethod,
				funcName,
				methodName,
				typeIsDelegate,
				indent,
				builders);
			
			// C# method that calls the C++ binding function
			ParameterInfo[] invokeParamsWithThis = PrependThisParameter(
				invokeParams);
			TypeKind invokeReturnTypeKind = GetTypeKind(
				invokeMethod.ReturnType);
			AppendCsharpBaseTypeCppMethodCallMethod(
				isOverride,
				invokeMethod,
				funcName,
				invokeParams,
				nativeInvokeFuncName,
				invokeParamsWithThis,
				invokeReturnTypeKind,
				builders.CsharpBaseTypes);
		}
		
		static ParameterInfo[] AppendBaseTypeCppNativeInvokeCall(
			Type type,
			string typeName,
			TypeName typeTypeName,
			Type[] typeParams,
			MethodInfo invokeMethod,
			string funcName,
			string methodName,
			bool typeIsDelegate,
			int indent,
			StringBuilders builders)
		{
			// C++ method declaration
			ParameterInfo[] invokeParams = ConvertParameters(
				invokeMethod.GetParameters());
			AppendIndent(
				indent + 1,
				builders.CppTypeDefinitions);
			AppendCppMethodDeclaration(
				methodName,
				false,
				true,
				false,
				invokeMethod.ReturnType,
				null,
				invokeParams,
				builders.CppTypeDefinitions);
			
			// C++ method definition. This is a no-op that game code overrides.
			AppendCppMethodDefinitionBegin(
				typeTypeName,
				invokeMethod.ReturnType,
				methodName,
				typeIsDelegate ? typeParams : null,
				null,
				invokeParams,
				indent,
				builders.CppMethodDefinitions);
			AppendIndent(
				indent,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("{\n");
			if (invokeMethod.ReturnType != typeof(void))
			{
				TypeKind returnTypeKind = GetTypeKind(invokeMethod.ReturnType);
				AppendIndent(
					indent + 1,
					builders.CppMethodDefinitions);
				if (returnTypeKind == TypeKind.Class ||
					returnTypeKind == TypeKind.ManagedStruct)
				{
					builders.CppMethodDefinitions.Append("return nullptr;\n");
				}
				else
				{
					builders.CppMethodDefinitions.Append("return {};\n");
				}
			}
			AppendIndent(
				indent,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append("}\n");
			AppendIndent(
				indent,
				builders.CppMethodDefinitions);
			builders.CppMethodDefinitions.Append('\n');
			
			// C++ binding function that C# calls. Calls the C++ method.
			TypeKind invokeReturnTypeKind = GetTypeKind(
				invokeMethod.ReturnType);
			AppendCppBaseTypeMethodInvokeBindingFunction(
				funcName,
				type,
				typeParams,
				invokeMethod,
				methodName,
				invokeReturnTypeKind,
				invokeParams,
				indent,
				typeName,
				builders.CppMethodDefinitions);
			
			// C# delegate for the C++ binding function
			AppendCsharpDelegate(
				false,
				GetTypeName(type),
				typeParams,
				funcName,
				invokeParams,
				invokeMethod.ReturnType,
				invokeReturnTypeKind,
				builders.CsharpDelegates);
			
			// C# import for the C++ binding function
			AppendCsharpImport(
				GetTypeName(type),
				typeParams,
				funcName,
				invokeParams,
				typeof(void),
				builders.CsharpImports);
			
			return invokeParams;
		}
		
		static ParameterInfo[] PrependThisParameter(
			ParameterInfo[] invokeParams)
		{
			ParameterInfo[] invokeParamsWithThis = new ParameterInfo[
				invokeParams.Length + 1];
			for (int i = 0; i < invokeParams.Length; ++i)
			{
				invokeParamsWithThis[i+1] = invokeParams[i];
			}
			invokeParamsWithThis[0] = new ParameterInfo {
				Name = "thisHandle",
				ParameterType = typeof(int),
				DereferencedParameterType = typeof(int),
				IsOut = false,
				IsRef = false,
				Kind = TypeKind.Primitive
			};
			return invokeParamsWithThis;
		}

		static void AppendCsharpBaseTypeReleaseFunction(
			Type type,
			TypeName bindingTypeTypeName,
			bool typeIsDelegate,
			string releaseFuncName,
			string derivedName,
			ParameterInfo[] releaseParams,
			StringBuilder output)
		{
			AppendCsharpFunctionBeginning(
				type,
				releaseFuncName,
				true,
				TypeKind.Class,
				typeof(void),
				releaseParams,
				output);
			if (typeIsDelegate || derivedName != null)
			{
				AppendCsharpTypeFullName(
					bindingTypeTypeName,
					output);
				output.Append(" thiz;\n");
			}
			if (typeIsDelegate)
			{
				output.Append("\t\t\t\tif (classHandle != 0)\n");
				output.Append("\t\t\t\t{\n");
				output.Append("\t\t\t\t\tthiz = (");
				AppendCsharpTypeFullName(
					bindingTypeTypeName,
					output);
				output.Append(")ObjectStore.Remove(classHandle);\n");
				output.Append("\t\t\t\t\tthiz.CppHandle = 0;\n");
				output.Append("\t\t\t\t}\n");
				output.Append("\t\t\t\t\n");
			}
			if (derivedName != null)
			{
				output.Append("\t\t\t\tthiz = (");
				AppendCsharpTypeFullName(
					bindingTypeTypeName,
					output);
				output.Append(")ObjectStore.Get(handle);\n");
				output.Append("\t\t\t\tint cppHandle = thiz.CppHandle;\n");
				output.Append("\t\t\t\tthiz.CppHandle = 0;\n");
				output.Append("\t\t\t\tQueueDestroy(DestroyFunction.");
				output.Append(bindingTypeTypeName.Name);
				output.Append(", cppHandle);\n");
			}
			output.Append("\t\t\t\tObjectStore.Remove(handle);");
			AppendCsharpFunctionReturn(
				releaseParams,
				typeof(void),
				TypeKind.Class,
				null,
				true,
				output);
		}

		static void AppendCsharpBaseTypeCppMethodCallMethod(
			bool isOverride,
			MethodInfo invokeMethod,
			string funcName,
			ParameterInfo[] invokeParams,
			string nativeInvokeFuncName,
			ParameterInfo[] invokeParamsWithThis,
			TypeKind invokeReturnTypeKind,
			StringBuilder output)
		{
			output.Append("\t\tpublic ");
			if (isOverride)
			{
				output.Append("override ");
			}
			AppendCsharpTypeFullName(
				invokeMethod.ReturnType,
				output);
			output.Append(' ');
			output.Append(funcName);
			output.Append("(");
			AppendCsharpParams(
				invokeParams,
				output);
			output.Append(")\n");
			output.Append("\t\t{\n");
			AppendCsharpBaseTypeCppMethodCallMethodBody(
				invokeMethod,
				nativeInvokeFuncName,
				invokeParamsWithThis,
				invokeReturnTypeKind,
				3,
				output);
			output.Append("\t\t}\n");
			output.Append("\t\n");
		}
		
		static void AppendCsharpBaseTypeCppMethodCallMethodBody(
			MethodInfo invokeMethod,
			string nativeInvokeFuncName,
			ParameterInfo[] invokeParamsWithThis,
			TypeKind invokeReturnTypeKind,
			int indent,
			StringBuilder output)
		{
			AppendIndent(
				indent,
				output);
			output.Append("if (CppHandle != 0)\n");
			AppendIndent(
				indent,
				output);
			output.Append("{\n");
			AppendIndent(
				indent + 1,
				output);
			output.Append("int thisHandle = CppHandle;\n");
			AppendCppFunctionCall(
				nativeInvokeFuncName,
				invokeParamsWithThis,
				invokeMethod.ReturnType,
				true,
				indent + 1,
				output);
			if (invokeMethod.ReturnType != typeof(void))
			{
				AppendIndent(
					indent + 1,
					output);
				output.Append("return ");
				switch (invokeReturnTypeKind)
				{
					case TypeKind.Class:
					case TypeKind.ManagedStruct:
						if (invokeMethod.ReturnType != typeof(object))
						{
							output.Append('(');
							AppendCsharpTypeFullName(
								invokeMethod.ReturnType,
								output);
							output.Append(')');
						}
						AppendHandleStoreTypeName(
							invokeMethod.ReturnType,
							output);
						output.Append(".Get(returnVal);\n");
						break;
					default:
						output.Append("returnVal;\n");
						break;
				}
			}
			AppendIndent(
				indent,
				output);
			output.Append("}\n");
			if (invokeMethod.ReturnType != typeof(void))
			{
				AppendIndent(
					indent,
					output);
				output.Append("return default(");
				AppendCsharpTypeFullName(
					invokeMethod.ReturnType,
					output);
				output.Append(");\n");
			}
		}

		static void AppendCsharpBaseTypeConstructorFunction(
			Type type,
			TypeName typeTypeName,
			bool typeIsDelegate,
			string constructorFuncName,
			ParameterInfo[] constructorParams,
			ParameterInfo[] cppConstructorParams,
			StringBuilder output)
		{
			AppendCsharpFunctionBeginning(
				type,
				constructorFuncName,
				true,
				TypeKind.Class,
				typeof(void),
				constructorParams,
				output);
			output.Append("var thiz = new ");
			AppendCsharpTypeFullName(typeTypeName, output);
			output.Append("(cppHandle");
			if (cppConstructorParams.Length > 0)
			{
				output.Append(", ");
				AppendCsharpFunctionCallParameters(
					cppConstructorParams,
					output);
			}
			output.Append(");\n");
			if (typeIsDelegate)
			{
				output.Append(
					"\t\t\t\tclassHandle = NativeScript.Bindings.ObjectStore.Store(thiz);\n");
				output.Append(
					"\t\t\t\thandle = NativeScript.Bindings.ObjectStore.Store(thiz.Delegate);");
			}
			else
			{
				output.Append(
					"\t\t\t\thandle = NativeScript.Bindings.ObjectStore.Store(thiz);");
			}
			AppendCsharpFunctionReturn(
				constructorParams,
				typeof(void),
				TypeKind.Class,
				null,
				true,
				output);
		}

		static void AppendCppBaseTypeMethodInvokeBindingFunction(
			string funcName,
			Type type,
			Type[] typeParams,
			MethodInfo method,
			string methodName,
			TypeKind methodReturnTypeKind,
			ParameterInfo[] methodParams,
			int indent,
			string typeName,
			StringBuilder output)
		{
			AppendIndent(
				indent,
				output);
			output.Append("DLLEXPORT ");
			if (method.ReturnType == typeof(void))
			{
				output.Append("void");
			}
			else if (method.ReturnType == typeof(bool))
			{
				// C linkage requires us to use primitive types
				output.Append("int32_t");
			}
			else if (method.ReturnType == typeof(char))
			{
				// C linkage requires us to use primitive types
				output.Append("int16_t");
			}
			else
			{
				switch (methodReturnTypeKind)
				{
					case TypeKind.Class:
					case TypeKind.ManagedStruct:
						output.Append("int32_t");
						break;
					case TypeKind.Primitive:
						AppendCppPrimitiveTypeName(
							method.ReturnType,
							output);
						break;
					default:
						AppendCppTypeFullName(
							method.ReturnType,
							output);
						break;
				}
			}
			output.Append(' ');
			AppendCsharpDelegateName(
				GetTypeName(type),
				typeParams,
				funcName,
				output);
			output.Append("(int32_t cppHandle");
			if (methodParams.Length > 0)
			{
				output.Append(", ");
			}
			for (int i = 0; i < methodParams.Length; ++i)
			{
				ParameterInfo param = methodParams[i];
				switch (param.Kind)
				{
					case TypeKind.Class:
					case TypeKind.ManagedStruct:
						output.Append("int32_t ");
						output.Append(param.Name);
						output.Append("Handle");
						break;
					case TypeKind.Primitive:
						AppendCppPrimitiveTypeName(
							param.ParameterType,
							output);
						output.Append(' ');
						output.Append(param.Name);
						break;
					default:
						AppendCppTypeFullName(
							param.ParameterType,
							output);
						output.Append(' ');
						output.Append(param.Name);
						break;
				}
				if (i != methodParams.Length - 1)
				{
					output.Append(", ");
				}
			}
			output.Append(")\n");
			AppendIndent(
				indent,
				output);
			output.Append("{\n");
			AppendIndent(
				indent + 1,
				output);
			output.Append("try\n");
			AppendIndent(
				indent + 1,
				output);
			output.Append("{\n");
			for (int i = 0; i < methodParams.Length; ++i)
			{
				ParameterInfo parameter = methodParams[i];
				if (parameter.Kind == TypeKind.Class ||
				    parameter.Kind == TypeKind.ManagedStruct)
				{
					AppendIndent(
						indent + 2,
						output);
					output.Append("auto ");
					output.Append(parameter.Name);
					output.Append(" = ");
					AppendCppTypeFullName(
						parameter.ParameterType,
						output);
					output.Append("(Plugin::InternalUse::Only, ");
					output.Append(parameter.Name);
					output.Append("Handle);\n");
				}
			}
			AppendIndent(
				indent + 2,
				output);
			if (method.ReturnType != typeof(void))
			{
				output.Append("return ");
			}
			output.Append("Plugin::Get");
			output.Append(typeName);
			output.Append("(cppHandle)->");
			output.Append(methodName);
			output.Append("(");
			for (int i = 0; i < methodParams.Length; ++i)
			{
				ParameterInfo parameter = methodParams[i];
				if (parameter.Kind == TypeKind.Class ||
				    parameter.Kind == TypeKind.ManagedStruct)
				{
					output.Append(parameter.Name);
				}
				else
				{
					output.Append(parameter.Name);
				}
				if (i != methodParams.Length - 1)
				{
					output.Append(", ");
				}
			}
			output.Append(")");
			if (
				method.ReturnType != typeof(void) &&
				(methodReturnTypeKind == TypeKind.Class ||
				 methodReturnTypeKind == TypeKind.ManagedStruct))
			{
				output.Append(".Handle");
			}
			output.Append(";\n");
			AppendIndent(
				indent + 1,
				output);
			output.Append("}\n");
			AppendIndent(
				indent + 1,
				output);
			output.Append(
				"catch (System::Exception ex)\n");
			AppendIndent(
				indent + 1,
				output);
			output.Append("{\n");
			AppendIndent(
				indent + 2,
				output);
			output.Append(
				"Plugin::SetException(ex.Handle);\n");
			if (method.ReturnType != typeof(void))
			{
				AppendIndent(
					indent + 2,
					output);
				output.Append(
					"return {};\n");
			}
			AppendIndent(
				indent + 1,
				output);
			output.Append("}\n");
			AppendIndent(
				indent + 1,
				output);
			output.Append("catch (...)\n");
			AppendIndent(
				indent + 1,
				output);
			output.Append("{\n");
			AppendIndent(
				indent + 2,
				output);
			output.Append(
				"System::String msg = \"Unhandled exception invoking ");
			AppendCppTypeFullName(
				type,
				output);
			output.Append("\";\n");
			AppendIndent(
				indent + 2,
				output);
			output.Append(
				"System::Exception ex(msg);\n");
			AppendIndent(
				indent + 2,
				output);
			output.Append(
				"Plugin::SetException(ex.Handle);\n");
			if (method.ReturnType != typeof(void))
			{
				AppendIndent(
					indent + 2,
					output);
				output.Append(
					"return {};\n");
			}
			AppendIndent(
				indent + 1,
				output);
			output.Append("}\n");
			AppendIndent(
				indent,
				output);
			output.Append("}\n");
			AppendIndent(
				indent,
				output);
			output.Append('\n');
		}

		static void AppendCppBaseTypeInequalityOperator(
			TypeName typeTypeName,
			Type[] typeParams,
			int cppMethodDefinitionsIndent,
			bool typeIsDelegate,
			StringBuilder output)
		{
			AppendIndent(
				cppMethodDefinitionsIndent,
				output);
			output.Append("bool ");
			AppendCppTypeFullName(
				typeTypeName,
				output);
			AppendCppTypeParameters(
				typeIsDelegate ? typeParams : null,
				output);
			output.Append("::operator!=(const ");
			AppendCppTypeFullName(
				typeTypeName,
				output);
			AppendCppTypeParameters(
				typeIsDelegate ? typeParams : null,
				output);
			output.Append("& other) const\n");
			AppendIndent(
				cppMethodDefinitionsIndent,
				output);
			output.Append("{\n");
			AppendIndent(
				cppMethodDefinitionsIndent + 1,
				output);
			output.Append(
				"return Handle != other.Handle;\n");
			AppendIndent(
				cppMethodDefinitionsIndent,
				output);
			output.Append("}\n");
			AppendIndent(
				cppMethodDefinitionsIndent,
				output);
			output.Append('\n');
		}

		static void AppendCppBaseTypeEqualityOperator(
			TypeName typeTypeName,
			Type[] typeParams,
			int cppMethodDefinitionsIndent,
			bool typeIsDelegate,
			StringBuilder output)
		{
			AppendIndent(
				cppMethodDefinitionsIndent,
				output);
			output.Append("bool ");
			AppendCppTypeFullName(
				typeTypeName,
				output);
			AppendCppTypeParameters(
				typeIsDelegate ? typeParams : null,
				output);
			output.Append("::operator==(const ");
			AppendCppTypeFullName(
				typeTypeName,
				output);
			AppendCppTypeParameters(
				typeIsDelegate ? typeParams : null,
				output);
			output.Append("& other) const\n");
			AppendIndent(
				cppMethodDefinitionsIndent,
				output);
			output.Append("{\n");
			AppendIndent(
				cppMethodDefinitionsIndent + 1,
				output);
			output.Append(
				"return Handle == other.Handle;\n");
			AppendIndent(
				cppMethodDefinitionsIndent,
				output);
			output.Append("}\n");
			AppendIndent(
				cppMethodDefinitionsIndent,
				output);
			output.Append('\n');
		}

		static void AppendCppBaseTypeMoveAssignmentOperator(
			string bindingTypeName,
			TypeName typeTypeName,
			Type[] typeParams,
			bool typeIsDelegate,
			string releaseFuncName,
			int cppMethodDefinitionsIndent,
			StringBuilder output)
		{
			AppendIndent(
				cppMethodDefinitionsIndent,
				output);
			AppendCppTypeFullName(
				typeTypeName,
				output);
			AppendCppTypeParameters(
				typeIsDelegate ? typeParams : null,
				output);
			output.Append("& ");
			AppendCppTypeFullName(
				typeTypeName,
				output);
			AppendCppTypeParameters(
				typeIsDelegate ? typeParams : null,
				output);
			output.Append("::operator=(");
			AppendCppTypeFullName(
				typeTypeName,
				output);
			AppendCppTypeParameters(
				typeIsDelegate ? typeParams : null,
				output);
			output.Append("&& other)\n");
			AppendIndent(
				cppMethodDefinitionsIndent,
				output);
			output.Append("{\n");
			AppendIndent(
				cppMethodDefinitionsIndent + 1,
				output);
			output.Append("Plugin::Remove");
			output.Append(bindingTypeName);
			output.Append("(CppHandle);\n");
			AppendIndent(
				cppMethodDefinitionsIndent + 1,
				output);
			output.Append("CppHandle = 0;\n");
			AppendIndent(
				cppMethodDefinitionsIndent + 1,
				output);
			output.Append("if (Handle)\n");
			AppendIndent(
				cppMethodDefinitionsIndent + 1,
				output);
			output.Append("{\n");
			AppendIndent(
				cppMethodDefinitionsIndent + 2,
				output);
			output.Append("int32_t handle = Handle;\n");
			if (typeIsDelegate)
			{
				AppendIndent(
					cppMethodDefinitionsIndent + 2,
					output);
				output.Append("int32_t classHandle = ClassHandle;\n");
			}
			AppendIndent(
				cppMethodDefinitionsIndent + 2,
				output);
			output.Append("Handle = 0;\n");
			if (typeIsDelegate)
			{
				AppendIndent(
					cppMethodDefinitionsIndent + 2,
					output);
				output.Append("ClassHandle = 0;\n");
			}
			AppendIndent(
				cppMethodDefinitionsIndent + 2,
				output);
			output.Append(
				"if (Plugin::DereferenceManagedClassNoRelease(handle))\n");
			AppendIndent(
				cppMethodDefinitionsIndent + 2,
				output);
			output.Append("{\n");
			AppendIndent(
				cppMethodDefinitionsIndent + 3,
				output);
			output.Append("Plugin::");
			output.Append(releaseFuncName);
			output.Append("(handle");
			if (typeIsDelegate)
			{
				output.Append(", classHandle");
			}
			output.Append(");\n");
			AppendCppUnhandledExceptionHandling(
				cppMethodDefinitionsIndent + 3,
				output);
			AppendIndent(
				cppMethodDefinitionsIndent + 2,
				output);
			output.Append("}\n");
			AppendIndent(
				cppMethodDefinitionsIndent + 1,
				output);
			output.Append("}\n");
			if (typeIsDelegate)
			{
				AppendIndent(
					cppMethodDefinitionsIndent + 1,
					output);
				output.Append(
					"ClassHandle = other.ClassHandle;\n");
				AppendIndent(
					cppMethodDefinitionsIndent + 1,
					output);
				output.Append("other.ClassHandle = 0;\n");
			}
			AppendIndent(
				cppMethodDefinitionsIndent + 1,
				output);
			output.Append("Handle = other.Handle;\n");
			AppendIndent(
				cppMethodDefinitionsIndent + 1,
				output);
			output.Append("other.Handle = 0;\n");
			AppendIndent(
				cppMethodDefinitionsIndent + 1,
				output);
			output.Append("return *this;\n");
			AppendIndent(
				cppMethodDefinitionsIndent,
				output);
			output.Append("}\n");
			AppendIndent(
				cppMethodDefinitionsIndent,
				output);
			output.Append('\n');
		}

		static void AppendCppBaseTypeAssignmentOperatorNullptr(
			TypeName typeTypeName,
			Type[] typeParams,
			bool typeIsDelegate,
			string releaseFuncName,
			int cppMethodDefinitionsIndent,
			StringBuilder output)
		{
			AppendIndent(
				cppMethodDefinitionsIndent,
				output);
			AppendCppTypeFullName(
				typeTypeName,
				output);
			AppendCppTypeParameters(
				typeIsDelegate ? typeParams : null,
				output);
			output.Append("& ");
			AppendCppTypeFullName(
				typeTypeName,
				output);
			AppendCppTypeParameters(
				typeIsDelegate ? typeParams : null,
				output);
			output.Append(
				"::operator=(decltype(nullptr))\n");
			AppendIndent(
				cppMethodDefinitionsIndent,
				output);
			output.Append("{\n");
			AppendIndent(
				cppMethodDefinitionsIndent + 1,
				output);
			output.Append("if (Handle)\n");
			AppendIndent(
				cppMethodDefinitionsIndent + 1,
				output);
			output.Append("{\n");
			AppendIndent(
				cppMethodDefinitionsIndent + 2,
				output);
			output.Append("int32_t handle = Handle;\n");
			if (typeIsDelegate)
			{
				AppendIndent(
					cppMethodDefinitionsIndent + 2,
					output);
				output.Append("int32_t classHandle = ClassHandle;\n");
			}
			AppendIndent(
				cppMethodDefinitionsIndent + 2,
				output);
			output.Append("Handle = 0;\n");
			if (typeIsDelegate)
			{
				AppendIndent(
					cppMethodDefinitionsIndent + 2,
					output);
				output.Append("ClassHandle = 0;\n");
			}
			AppendIndent(
				cppMethodDefinitionsIndent + 2,
				output);
			output.Append(
				"if (Plugin::DereferenceManagedClassNoRelease(handle))\n");
			AppendIndent(
				cppMethodDefinitionsIndent + 2,
				output);
			output.Append("{\n");
			AppendIndent(
				cppMethodDefinitionsIndent + 3,
				output);
			output.Append("Plugin::");
			output.Append(releaseFuncName);
			output.Append("(handle");
			if (typeIsDelegate)
			{
				output.Append(", classHandle");
			}
			output.Append(");\n");
			AppendCppUnhandledExceptionHandling(
				cppMethodDefinitionsIndent + 3,
				output);
			AppendIndent(
				cppMethodDefinitionsIndent + 2,
				output);
			output.Append("}\n");
			AppendIndent(
				cppMethodDefinitionsIndent + 1,
				output);
			output.Append("}\n");
			if (typeIsDelegate)
			{
				AppendIndent(
					cppMethodDefinitionsIndent + 1,
					output);
				output.Append("ClassHandle = 0;\n");
			}
			AppendIndent(
				cppMethodDefinitionsIndent + 1,
				output);
			output.Append("Handle = 0;\n");
			AppendIndent(
				cppMethodDefinitionsIndent + 1,
				output);
			output.Append("return *this;\n");
			AppendIndent(
				cppMethodDefinitionsIndent,
				output);
			output.Append("}\n");
			AppendIndent(
				cppMethodDefinitionsIndent,
				output);
			output.Append('\n');
		}

		static void AppendCppBaseTypeAssignmentOperatorSameType(
			TypeName typeTypeName,
			Type[] typeParams,
			bool typeIsDelegate,
			int cppMethodDefinitionsIndent,
			StringBuilder output)
		{
			AppendIndent(
				cppMethodDefinitionsIndent,
				output);
			AppendCppTypeFullName(
				typeTypeName,
				output);
			AppendCppTypeParameters(
				typeIsDelegate ? typeParams : null,
				output);
			output.Append("& ");
			AppendCppTypeFullName(
				typeTypeName,
				output);
			AppendCppTypeParameters(
				typeIsDelegate ? typeParams : null,
				output);
			output.Append("::operator=(const ");
			AppendCppTypeFullName(
				typeTypeName,
				output);
			AppendCppTypeParameters(
				typeIsDelegate ? typeParams : null,
				output);
			output.Append("& other)\n");
			AppendIndent(
				cppMethodDefinitionsIndent,
				output);
			output.Append("{\n");
			AppendSetHandle(
				typeTypeName,
				TypeKind.Class,
				typeParams,
				cppMethodDefinitionsIndent + 1,
				"this",
				"other.Handle",
				output);
			if (typeIsDelegate)
			{
				AppendIndent(
					cppMethodDefinitionsIndent + 1,
					output);
				output.Append(
					"ClassHandle = other.ClassHandle;\n");
			}
			AppendIndent(
				cppMethodDefinitionsIndent + 1,
				output);
			output.Append("return *this;\n");
			AppendIndent(
				cppMethodDefinitionsIndent,
				output);
			output.Append("}\n");
			AppendIndent(
				cppMethodDefinitionsIndent,
				output);
			output.Append('\n');
		}

		static void AppendCppBaseTypeDestructor(
			string typeName,
			TypeName typeTypeName,
			Type[] typeParams,
			bool typeIsDelegate,
			string derivedTypeName,
			string releaseFuncName,
			string bindingTypeName,
			int cppMethodDefinitionsIndent,
			StringBuilder output)
		{
			AppendIndent(
				cppMethodDefinitionsIndent,
				output);
			AppendCppTypeFullName(
				typeTypeName,
				output);
			AppendCppTypeParameters(
				typeIsDelegate ? typeParams : null,
				output);
			output.Append("::~");
			AppendCppTypeName(
				typeTypeName,
				output);
			AppendCppTypeParameters(
				typeIsDelegate ? typeParams : null,
				output);
			output.Append("()\n");
			AppendIndent(
				cppMethodDefinitionsIndent,
				output);
			output.Append("{\n");
			if (!string.IsNullOrEmpty(derivedTypeName))
			{
				AppendIndent(
					cppMethodDefinitionsIndent + 1,
					output);
				output.Append("Plugin::RemoveWhole");
				output.Append(bindingTypeName);
				output.Append("(this);\n");
			}
			AppendIndent(
				cppMethodDefinitionsIndent + 1,
				output);
			output.Append("Plugin::Remove");
			output.Append(typeName);
			output.Append("(CppHandle);\n");
			AppendIndent(
				cppMethodDefinitionsIndent + 1,
				output);
			output.Append("CppHandle = 0;\n");
			AppendIndent(
				cppMethodDefinitionsIndent + 1,
				output);
			output.Append("if (Handle)\n");
			AppendIndent(
				cppMethodDefinitionsIndent + 1,
				output);
			output.Append("{\n");
			AppendIndent(
				cppMethodDefinitionsIndent + 2,
				output);
			output.Append("int32_t handle = Handle;\n");
			if (typeIsDelegate)
			{
				AppendIndent(
					cppMethodDefinitionsIndent + 2,
					output);
				output.Append("int32_t classHandle = ClassHandle;\n");
			}
			AppendIndent(
				cppMethodDefinitionsIndent + 2,
				output);
			output.Append("Handle = 0;\n");
			if (typeIsDelegate)
			{
				AppendIndent(
					cppMethodDefinitionsIndent + 2,
					output);
				output.Append("ClassHandle = 0;\n");
			}
			AppendIndent(
				cppMethodDefinitionsIndent + 2,
				output);
			output.Append(
				"if (Plugin::DereferenceManagedClassNoRelease(handle))\n");
			AppendIndent(
				cppMethodDefinitionsIndent + 2,
				output);
			output.Append("{\n");
			AppendIndent(
				cppMethodDefinitionsIndent + 3,
				output);
			output.Append("Plugin::");
			output.Append(releaseFuncName);
			output.Append("(handle");
			if (typeIsDelegate)
			{
				output.Append(", classHandle");
			}
			output.Append(");\n");
			AppendCppUnhandledExceptionHandling(
				cppMethodDefinitionsIndent + 3,
				output);
			AppendIndent(
				cppMethodDefinitionsIndent + 2,
				output);
			output.Append("}\n");
			AppendIndent(
				cppMethodDefinitionsIndent + 1,
				output);
			output.Append("}\n");
			AppendIndent(
				cppMethodDefinitionsIndent,
				output);
			output.Append("}\n");
			AppendIndent(
				cppMethodDefinitionsIndent,
				output);
			output.Append('\n');
		}

		static void AppendCppBaseTypeHandleConstructor(
			string bindingTypeName,
			TypeName typeTypeName,
			Type[] typeParams,
			Type[] interfaceTypes,
			bool typeIsDelegate,
			int cppMethodDefinitionsIndent,
			StringBuilder output)
		{
			AppendIndent(
				cppMethodDefinitionsIndent,
				output);
			AppendCppTypeFullName(
				typeTypeName,
				output);
			AppendCppTypeParameters(
				typeIsDelegate ? typeParams : null,
				output);
			output.Append("::");
			AppendCppTypeName(
				typeTypeName,
				output);
			output.Append(
				"(Plugin::InternalUse, int32_t handle)\n");
			AppendCppConstructorInitializerList(
				interfaceTypes,
				cppMethodDefinitionsIndent + 1,
				output);
			AppendIndent(
				cppMethodDefinitionsIndent,
				output);
			output.Append("{\n");
			AppendIndent(
				cppMethodDefinitionsIndent + 1,
				output);
			output.Append("Handle = handle;\n");
			AppendIndent(
				cppMethodDefinitionsIndent + 1,
				output);
			output.Append("CppHandle = Plugin::Store");
			output.Append(bindingTypeName);
			output.Append("(this);\n");
			AppendIndent(
				cppMethodDefinitionsIndent + 1,
				output);
			output.Append("if (Handle)\n");
			AppendIndent(
				cppMethodDefinitionsIndent + 1,
				output);
			output.Append("{\n");
			AppendIndent(
				cppMethodDefinitionsIndent + 2,
				output);
			output.Append(
				"Plugin::ReferenceManagedClass(Handle);\n");
			AppendIndent(
				cppMethodDefinitionsIndent + 1,
				output);
			output.Append("}\n");
			if (typeIsDelegate)
			{
				AppendIndent(
					cppMethodDefinitionsIndent + 1,
					output);
				output.Append(
					"ClassHandle = 0;\n");
			}
			AppendIndent(
				cppMethodDefinitionsIndent,
				output);
			output.Append("}\n");
			AppendIndent(
				cppMethodDefinitionsIndent,
				output);
			output.Append('\n');
		}
		
		static void AppendCppBaseTypeMoveConstructor(
			TypeName typeTypeName,
			Type[] typeParams,
			Type[] interfaceTypes,
			bool typeIsDelegate,
			int cppMethodDefinitionsIndent,
			StringBuilder output)
		{
			AppendIndent(
				cppMethodDefinitionsIndent,
				output);
			AppendCppTypeFullName(
				typeTypeName,
				output);
			AppendCppTypeParameters(
				typeIsDelegate ? typeParams : null,
				output);
			output.Append("::");
			AppendCppTypeName(
				typeTypeName,
				output);
			output.Append("(");
			AppendCppTypeFullName(
				typeTypeName,
				output);
			AppendCppTypeParameters(
				typeIsDelegate ? typeParams : null,
				output);
			output.Append("&& other)\n");
			AppendCppConstructorInitializerList(
				interfaceTypes,
				cppMethodDefinitionsIndent + 1,
				output);
			AppendIndent(
				cppMethodDefinitionsIndent,
				output);
			output.Append("{\n");
			AppendIndent(
				cppMethodDefinitionsIndent + 1,
				output);
			output.Append(
				"Handle = other.Handle;\n");
			AppendIndent(
				cppMethodDefinitionsIndent + 1,
				output);
			output.Append(
				"CppHandle = other.CppHandle;\n");
			if (typeIsDelegate)
			{
				AppendIndent(
					cppMethodDefinitionsIndent + 1,
					output);
				output.Append(
					"ClassHandle = other.ClassHandle;\n");
			}
			AppendIndent(
				cppMethodDefinitionsIndent + 1,
				output);
			output.Append("other.Handle = 0;\n");
			AppendIndent(
				cppMethodDefinitionsIndent + 1,
				output);
			output.Append("other.CppHandle = 0;\n");
			if (typeIsDelegate)
			{
				AppendIndent(
					cppMethodDefinitionsIndent + 1,
					output);
				output.Append("other.ClassHandle = 0;\n");
			}
			AppendIndent(
				cppMethodDefinitionsIndent,
				output);
			output.Append("}\n");
			AppendIndent(
				cppMethodDefinitionsIndent,
				output);
			output.Append('\n');
		}

		static void AppendCppBaseTypeCopyConstructor(
			string typeName,
			TypeName typeTypeName,
			Type[] typeParams,
			Type[] interfaceTypes,
			bool typeIsDelegate,
			int cppMethodDefinitionsIndent,
			StringBuilder output)
		{
			AppendIndent(
				cppMethodDefinitionsIndent,
				output);
			AppendCppTypeFullName(
				typeTypeName,
				output);
			AppendCppTypeParameters(
				typeIsDelegate ? typeParams : null,
				output);
			output.Append("::");
			AppendCppTypeName(
				typeTypeName,
				output);
			output.Append("(const ");
			AppendCppTypeFullName(
				typeTypeName,
				output);
			AppendCppTypeParameters(
				typeIsDelegate ? typeParams : null,
				output);
			output.Append("& other)\n");
			AppendCppConstructorInitializerList(
				interfaceTypes,
				cppMethodDefinitionsIndent + 1,
				output);
			AppendIndent(
				cppMethodDefinitionsIndent,
				output);
			output.Append("{\n");
			AppendIndent(
				cppMethodDefinitionsIndent + 1,
				output);
			output.Append(
				"Handle = other.Handle;\n");
			AppendIndent(
				cppMethodDefinitionsIndent + 1,
				output);
			output.Append("CppHandle = Plugin::Store");
			output.Append(typeName);
			output.Append("(this);\n");
			AppendIndent(
				cppMethodDefinitionsIndent + 1,
				output);
			output.Append("if (Handle)\n");
			AppendIndent(
				cppMethodDefinitionsIndent + 1,
				output);
			output.Append("{\n");
			AppendIndent(
				cppMethodDefinitionsIndent + 2,
				output);
			output.Append(
				"Plugin::ReferenceManagedClass(Handle);\n");
			AppendIndent(
				cppMethodDefinitionsIndent + 1,
				output);
			output.Append("}\n");
			if (typeIsDelegate)
			{
				AppendIndent(
					cppMethodDefinitionsIndent + 1,
					output);
				output.Append(
					"ClassHandle = other.ClassHandle;\n");
			}
			AppendIndent(
				cppMethodDefinitionsIndent,
				output);
			output.Append("}\n");
			AppendIndent(
				cppMethodDefinitionsIndent,
				output);
			output.Append('\n');
		}

		static void AppendCppBaseTypeNullptrConstructor(
			string typeName,
			TypeName cppTypeTypeName,
			Type[] typeParams,
			Type[] interfaceTypes,
			bool typeIsDelegate,
			int cppMethodDefinitionsIndent,
			StringBuilder output)
		{
			AppendIndent(
				cppMethodDefinitionsIndent,
				output);
			AppendCppTypeName(
				cppTypeTypeName,
				output);
			AppendCppTypeParameters(
				typeIsDelegate ? typeParams : null,
				output);
			output.Append("::");
			AppendCppTypeName(
				cppTypeTypeName,
				output);
			output.Append("(decltype(nullptr))\n");
			AppendCppConstructorInitializerList(
				interfaceTypes,
				cppMethodDefinitionsIndent + 1,
				output);
			AppendIndent(
				cppMethodDefinitionsIndent,
				output);
			output.Append("{\n");
			AppendIndent(
				cppMethodDefinitionsIndent + 1,
				output);
			output.Append("CppHandle = Plugin::Store");
			output.Append(typeName);
			output.Append("(this);\n");
			if (typeIsDelegate)
			{
				AppendIndent(
					cppMethodDefinitionsIndent + 1,
					output);
				output.Append("ClassHandle = 0;\n");
			}
			AppendIndent(
				cppMethodDefinitionsIndent,
				output);
			output.Append("}\n");
			AppendIndent(
				cppMethodDefinitionsIndent,
				output);
			output.Append('\n');
		}

		static void AppendCppBaseTypeConstructor(
			string bindingTypeName,
			TypeName typeTypeName,
			TypeKind typeKind,
			string cppTypeName,
			Type[] typeParams,
			Type[] interfaceTypes,
			ParameterInfo[] cppParameters,
			ParameterInfo[] parameters,
			bool typeIsDelegate,
			string constructorFuncName,
			int cppMethodDefinitionsIndent,
			StringBuilder output)
		{
			AppendCppMethodDefinitionBegin(
				typeTypeName,
				null,
				cppTypeName,
				typeIsDelegate ? typeParams : null,
				null,
				cppParameters,
				cppMethodDefinitionsIndent,
				output);
			AppendCppConstructorInitializerList(
				interfaceTypes,
				cppMethodDefinitionsIndent + 1,
				output);
			AppendIndent(
				cppMethodDefinitionsIndent,
				output);
			output.Append("{\n");
			AppendIndent(
				cppMethodDefinitionsIndent + 1,
				output);
			output.Append("CppHandle = Plugin::Store");
			output.Append(bindingTypeName);
			output.Append("(this);\n");
			AppendIndent(
				cppMethodDefinitionsIndent + 1,
				output);
			output.Append("System::Int32* handle = (System::Int32*)&Handle;\n");
			AppendIndent(
				cppMethodDefinitionsIndent + 1,
				output);
			output.Append("int32_t cppHandle = CppHandle;\n");
			if (typeIsDelegate)
			{
				AppendIndent(
					cppMethodDefinitionsIndent + 1,
					output);
				output.Append("System::Int32* classHandle = (System::Int32*)&ClassHandle;\n");
			}
			AppendCppPluginFunctionCall(
				true,
				GetTypeName(bindingTypeName, typeTypeName.Namespace),
				typeKind,
				typeParams,
				null,
				constructorFuncName,
				parameters,
				cppMethodDefinitionsIndent + 1,
				output);
			AppendIndent(
				cppMethodDefinitionsIndent + 1,
				output);
			output.Append("if (Handle)\n");
			AppendIndent(
				cppMethodDefinitionsIndent + 1,
				output);
			output.Append("{\n");
			AppendIndent(
				cppMethodDefinitionsIndent + 2,
				output);
			output.Append(
				"Plugin::ReferenceManagedClass(Handle);\n");
			AppendIndent(
				cppMethodDefinitionsIndent + 1,
				output);
			output.Append("}\n");
			AppendIndent(
				cppMethodDefinitionsIndent + 1,
				output);
			output.Append("else\n");
			AppendIndent(
				cppMethodDefinitionsIndent + 1,
				output);
			output.Append("{\n");
			AppendIndent(
				cppMethodDefinitionsIndent + 2,
				output);
			output.Append("Plugin::Remove");
			output.Append(bindingTypeName);
			output.Append("(CppHandle);\n");
			if (typeIsDelegate)
			{
				AppendIndent(
					cppMethodDefinitionsIndent + 2,
					output);
				output.Append("ClassHandle = 0;\n");
			}
			AppendIndent(
				cppMethodDefinitionsIndent + 2,
				output);
			output.Append("CppHandle = 0;\n");
			AppendIndent(
				cppMethodDefinitionsIndent + 1,
				output);
			output.Append("}\n");
			AppendCppUnhandledExceptionHandling(
				cppMethodDefinitionsIndent + 1,
				output);
			AppendIndent(
				cppMethodDefinitionsIndent,
				output);
			output.Append("}\n");
			AppendIndent(
				cppMethodDefinitionsIndent,
				output);
			output.Append('\n');
		}

		static void AppendCppPointerFreeListInit(
			Type[] typeParams,
			TypeName cppTypeTypeName,
			int maxSimultaneous,
			string typeName,
			StringBuilder output,
			StringBuilder outputFirstBoot)
		{
			output.Append("\tPlugin::");
			output.Append(typeName);
			output.Append("FreeListSize = ");
			output.Append(maxSimultaneous);
			output.Append(";\n");
			
			output.Append("\tPlugin::");
			output.Append(typeName);
			output.Append("FreeList = (");
			AppendCppTypeFullName(
				cppTypeTypeName,
				output);
			AppendCppTypeParameters(
				typeParams,
				output);
			output.Append("**)curMemory;\n");
			
			output.Append("\tcurMemory += ");
			output.Append(maxSimultaneous);
			output.Append(" * sizeof(");
			AppendCppTypeFullName(
				cppTypeTypeName,
				output);
			AppendCppTypeParameters(
				typeParams,
				output);
			output.Append("*);\n");
			
			output.Append("\t\n");
			
			outputFirstBoot.Append("\t\tfor (int32_t i = 0, end = Plugin::");
			outputFirstBoot.Append(typeName);
			outputFirstBoot.Append("FreeListSize - 1; i < end; ++i)\n");
			outputFirstBoot.Append("\t\t{\n");
			outputFirstBoot.Append("\t\t\tPlugin::");
			outputFirstBoot.Append(typeName);
			outputFirstBoot.Append("FreeList[i] = (");
			AppendCppTypeFullName(
				cppTypeTypeName,
				outputFirstBoot);
			AppendCppTypeParameters(
				typeParams,
				outputFirstBoot);
			outputFirstBoot.Append("*)(Plugin::");
			outputFirstBoot.Append(typeName);
			outputFirstBoot.Append("FreeList + i + 1);\n");
			outputFirstBoot.Append("\t\t}\n");
			
			outputFirstBoot.Append("\t\tPlugin::");
			outputFirstBoot.Append(typeName);
			outputFirstBoot.Append("FreeList[Plugin::");
			outputFirstBoot.Append(typeName);
			outputFirstBoot.Append("FreeListSize - 1] = nullptr;\n");
			
			outputFirstBoot.Append("\t\tPlugin::NextFree");
			outputFirstBoot.Append(typeName);
			outputFirstBoot.Append(" = Plugin::");
			outputFirstBoot.Append(typeName);
			outputFirstBoot.Append("FreeList + 1;\n");
			
			outputFirstBoot.Append("\t\t\n");
		}

		static void AppendCppPointerFreeListStateAndFunctions(
			TypeName cppTypeTypeName,
			Type[] typeParams,
			string bindingTypeName,
			StringBuilder output)
		{
			// Section comment
			output.Append("\t// Free list for ");
			AppendCppTypeFullName(
				cppTypeTypeName,
				output);
			AppendCppTypeParameters(
				typeParams,
				output);
			output.Append(" pointers\n");
			output.Append("\t\n");

			// Size variable
			output.Append("\tint32_t ");
			output.Append(bindingTypeName);
			output.Append("FreeListSize;\n");

			// Free list variable
			output.Append('\t');
			AppendCppTypeFullName(
				cppTypeTypeName,
				output);
			AppendCppTypeParameters(
				typeParams,
				output);
			output.Append("** ");
			output.Append(bindingTypeName);
			output.Append("FreeList;\n");

			// Next free variable
			output.Append('\t');
			AppendCppTypeFullName(
				cppTypeTypeName,
				output);
			AppendCppTypeParameters(
				typeParams,
				output);
			output.Append("** NextFree");
			output.Append(bindingTypeName);
			output.Append(";\n");
			output.Append("\t\n");

			// Store function
			output.Append("\tint32_t Store");
			output.Append(bindingTypeName);
			output.Append('(');
			AppendCppTypeFullName(
				cppTypeTypeName,
				output);
			AppendCppTypeParameters(
				typeParams,
				output);
			output.Append("* del)\n");
			output.Append("\t{\n");
			output.Append("\t\tassert(NextFree");
			output.Append(bindingTypeName);
			output.Append(" != nullptr);\n");
			output.Append("\t\t");
			AppendCppTypeFullName(
				cppTypeTypeName,
				output);
			AppendCppTypeParameters(
				typeParams,
				output);
			output.Append("** pNext = NextFree");
			output.Append(bindingTypeName);
			output.Append(";\n");
			output.Append("\t\tNextFree");
			output.Append(bindingTypeName);
			output.Append(" = (");
			AppendCppTypeFullName(
				cppTypeTypeName,
				output);
			AppendCppTypeParameters(
				typeParams,
				output);
			output.Append("**)*pNext;\n");
			output.Append("\t\t*pNext = del;\n");
			output.Append("\t\treturn (int32_t)(pNext - ");
			output.Append(bindingTypeName);
			output.Append("FreeList);\n");
			output.Append("\t}\n");
			output.Append("\t\n");

			// Get function
			output.Append('\t');
			AppendCppTypeFullName(
				cppTypeTypeName,
				output);
			AppendCppTypeParameters(
				typeParams,
				output);
			output.Append("* Get");
			output.Append(bindingTypeName);
			output.Append("(int32_t handle)\n");
			output.Append("\t{\n");
			output.Append(
				"\t\tassert(handle >= 0 && handle < ");
			output.Append(bindingTypeName);
			output.Append("FreeListSize);\n");
			output.Append("\t\treturn ");
			output.Append(bindingTypeName);
			output.Append("FreeList[handle];\n");
			output.Append("\t}\n");
			output.Append("\t\n");

			// Remove function
			output.Append("\tvoid Remove");
			output.Append(bindingTypeName);
			output.Append("(int32_t handle)\n");
			output.Append("\t{\n");
			output.Append("\t\t");
			AppendCppTypeFullName(
				cppTypeTypeName,
				output);
			AppendCppTypeParameters(
				typeParams,
				output);
			output.Append("** pRelease = ");
			output.Append(bindingTypeName);
			output.Append("FreeList + handle;\n");
			output.Append("\t\t*pRelease = (");
			AppendCppTypeFullName(
				cppTypeTypeName,
				output);
			AppendCppTypeParameters(
				typeParams,
				output);
			output.Append("*)NextFree");
			output.Append(bindingTypeName);
			output.Append(";\n");
			output.Append("\t\tNextFree");
			output.Append(bindingTypeName);
			output.Append(" = pRelease;\n");
			output.Append("\t}\n");
			output.Append("\t\n");
		}

		static void AppendCppWholeObjectFreeListInit(
			int maxSimultaneous,
			string bindingTypeName,
			StringBuilder output,
			StringBuilder outputFirstBoot)
		{
			output.Append("\tPlugin::");
			output.Append(bindingTypeName);
			output.Append("FreeWholeListSize = ");
			output.Append(maxSimultaneous);
			output.Append(";\n");

			output.Append("\tPlugin::");
			output.Append(bindingTypeName);
			output.Append("FreeWholeList = (Plugin::");
			output.Append(bindingTypeName);
			output.Append("FreeWholeListEntry*)curMemory;\n");

			output.Append("\tcurMemory += ");
			output.Append(maxSimultaneous);
			output.Append(" * sizeof(Plugin::");
			output.Append(bindingTypeName);
			output.Append("FreeWholeListEntry);\n");

			output.Append("\t\n");

			outputFirstBoot.Append("\t\tfor (int32_t i = 0, end = Plugin::");
			outputFirstBoot.Append(bindingTypeName);
			outputFirstBoot.Append("FreeWholeListSize - 1; i < end; ++i)\n");
			outputFirstBoot.Append("\t\t{\n");
			outputFirstBoot.Append("\t\t\tPlugin::");
			outputFirstBoot.Append(bindingTypeName);
			outputFirstBoot.Append("FreeWholeList[i].Next = Plugin::");
			outputFirstBoot.Append(bindingTypeName);
			outputFirstBoot.Append("FreeWholeList[i + 1].Next;\n");
			outputFirstBoot.Append("\t\t}\n");

			outputFirstBoot.Append("\t\tPlugin::");
			outputFirstBoot.Append(bindingTypeName);
			outputFirstBoot.Append("FreeWholeList[Plugin::");
			outputFirstBoot.Append(bindingTypeName);
			outputFirstBoot.Append("FreeWholeListSize - 1].Next = nullptr;\n");

			outputFirstBoot.Append("\t\tPlugin::NextFreeWhole");
			outputFirstBoot.Append(bindingTypeName);
			outputFirstBoot.Append(" = Plugin::");
			outputFirstBoot.Append(bindingTypeName);
			outputFirstBoot.Append("FreeWholeList + 1;\n");

			outputFirstBoot.Append("\t\t\n");
		}

		static void AppendCppWholeObjectFreeListStateAndFunctions(
			Type[] typeParams,
			TypeName cppTypeTypeName,
			string bindingTypeName,
			StringBuilder output)
		{
			// Section comment
			output.Append("\t// Free list for whole ");
			AppendCppTypeFullName(
				cppTypeTypeName,
				output);
			AppendCppTypeParameters(
				typeParams,
				output);
			output.Append(" objects\n");
			output.Append("\t\n");

			// Union with a pointer and a whole object
			output.Append("\tunion ");
			output.Append(bindingTypeName);
			output.Append("FreeWholeListEntry\n");
			output.Append("\t{\n");
			output.Append("\t\t");
			output.Append(bindingTypeName);
			output.Append("FreeWholeListEntry* Next;\n");
			output.Append("\t\t");
			AppendCppTypeFullName(
				cppTypeTypeName,
				output);
			AppendCppTypeParameters(
				typeParams,
				output);
			output.Append(" Value;\n");
			output.Append("\t};\n");

			// Size
			output.Append("\tint32_t ");
			output.Append(bindingTypeName);
			output.Append("FreeWholeListSize;\n");

			// Free list entries
			output.Append('\t');
			output.Append(bindingTypeName);
			output.Append("FreeWholeListEntry* ");
			output.Append(bindingTypeName);
			output.Append("FreeWholeList;\n");

			// Pointer to next free entry
			output.Append('\t');
			output.Append(bindingTypeName);
			output.Append("FreeWholeListEntry* NextFreeWhole");
			output.Append(bindingTypeName);
			output.Append(";\n");
			output.Append("\t\n");

			// Store function
			output.Append('\t');
			AppendCppTypeFullName(
				cppTypeTypeName,
				output);
			AppendCppTypeParameters(
				typeParams,
				output);
			output.Append("* StoreWhole");
			output.Append(bindingTypeName);
			output.Append("()\n");
			output.Append("\t{\n");
			output.Append("\t\tassert(NextFreeWhole");
			output.Append(bindingTypeName);
			output.Append(" != nullptr);\n");
			output.Append("\t\t");
			output.Append(bindingTypeName);
			output.Append("FreeWholeListEntry* pNext = NextFreeWhole");
			output.Append(bindingTypeName);
			output.Append(";\n");
			output.Append("\t\tNextFreeWhole");
			output.Append(bindingTypeName);
			output.Append(" = pNext->Next;\n");
			output.Append("\t\treturn &pNext->Value;\n");
			output.Append("\t}\n");
			output.Append("\t\n");

			// Remove function
			output.Append("\tvoid RemoveWhole");
			output.Append(bindingTypeName);
			output.Append('(');
			AppendCppTypeFullName(
				cppTypeTypeName,
				output);
			AppendCppTypeParameters(
				typeParams,
				output);
			output.Append("* instance)\n");
			output.Append("\t{\n");
			output.Append("\t\t");
			output.Append(bindingTypeName);
			output.Append("FreeWholeListEntry* pRelease = (");
			output.Append(bindingTypeName);
			output.Append("FreeWholeListEntry*)instance;\n");
			output.Append("\t\tif (pRelease >= ");
			output.Append(bindingTypeName);
			output.Append("FreeWholeList && pRelease < ");
			output.Append(bindingTypeName);
			output.Append("FreeWholeList + (");
			output.Append(bindingTypeName);
			output.Append("FreeWholeListSize - 1))\n");
			output.Append("\t\t{\n");
			output.Append("\t\t\tpRelease->Next = NextFreeWhole");
			output.Append(bindingTypeName);
			output.Append(";\n");
			output.Append("\t\t\tNextFreeWhole");
			output.Append(bindingTypeName);
			output.Append(" = pRelease->Next;\n");
			output.Append("\t\t}\n");
			output.Append("\t}\n");
			output.Append("\t\n");
		}

		static void AppendCsharpDelegate(
			bool isStatic,
			TypeName typeTypeName,
			Type[] typeParams,
			string funcName,
			ParameterInfo[] parameters,
			Type returnType,
			TypeKind returnTypeKind,
			StringBuilder output)
		{
			output.Append("\t\tpublic delegate ");
			if (returnType == typeof(void))
			{
				output.Append("void");
			}
			else
			{
				switch (returnTypeKind)
				{
					case TypeKind.Class:
					case TypeKind.ManagedStruct:
						output.Append("int");
						break;
					default:
						AppendCsharpTypeFullName(
							returnType,
							output);
						break;
				}
			}
			output.Append(' ');
			AppendCsharpDelegateName(
				typeTypeName,
				typeParams,
				funcName,
				output);
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
				switch (param.Kind)
				{
					case TypeKind.FullStruct:
					case TypeKind.Primitive:
					case TypeKind.Enum:
						AppendCsharpTypeFullName(
							param.ParameterType,
							output);
						output.Append(" param");
						output.Append(i);
						break;
					default:
						output.Append("int param");
						output.Append(i);
						break;
				}
				if (i != parameters.Length-1)
				{
					output.Append(", ");
				}
			}
			output.Append(");\n");
			output.Append("\t\tpublic static ");
			AppendCsharpDelegateName(
				typeTypeName,
				typeParams,
				funcName,
				output);
			output.Append("Delegate ");
			AppendCsharpDelegateName(
				typeTypeName,
				typeParams,
				funcName,
				output);
			output.Append(";\n\t\t\n");
		}
		
		static void AppendCsharpDelegateName(
			TypeName typeTypeName,
			Type[] typeParams,
			string funcName,
			StringBuilder output)
		{
			AppendNamespace(
				typeTypeName.Namespace,
				string.Empty,
				output);
			AppendTypeNameWithoutSuffixes(
				typeTypeName.Name,
				output);
			AppendTypeNames(
				typeParams,
				output);
			output.Append(funcName);
		}
		
		static void AppendCsharpGetDelegateCall(
			TypeName typeTypeName,
			Type[] typeParams,
			string funcName,
			StringBuilder output)
		{
			output.Append("\t\t\t");
			AppendCsharpDelegateName(
				typeTypeName,
				typeParams,
				funcName,
				output);
			output.Append(" = GetDelegate<");
			AppendCsharpDelegateName(
				typeTypeName,
				typeParams,
				funcName,
				output);
			output.Append("Delegate>(libraryHandle, \"");
			AppendCsharpDelegateName(
				typeTypeName,
				typeParams,
				funcName,
				output);
			output.Append("\");\n");
		}
		
		static void AppendCsharpImport(
			TypeName typeTypeName,
			Type[] typeParams,
			string funcName,
			ParameterInfo[] parameters,
			Type returnType,
			StringBuilder output
		)
		{
			output.Append("\t\t[DllImport(PLUGIN_NAME)]\n");
			output.Append("\t\tpublic static extern ");
			AppendCsharpTypeFullName(returnType, output);
			output.Append(' ');
			AppendCsharpDelegateName(
				typeTypeName,
				typeParams,
				funcName,
				output);
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
					AppendCsharpTypeFullName(
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
							JsonPropertyGet jsonPropertyGet = jsonProperty.Get;
							if (jsonPropertyGet != null
								&& jsonPropertyGet.Exceptions != null)
							{
								AddUniqueTypes(
									jsonPropertyGet.Exceptions,
									exceptionTypes,
									assemblies);
							}
							JsonPropertySet jsonPropertySet = jsonProperty.Set;
							if (jsonPropertySet != null
								&& jsonPropertySet.Exceptions != null)
							{
								AddUniqueTypes(
									jsonPropertySet.Exceptions,
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
				AppendCppTypeFullName(
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
				builders.CppMethodDefinitions.Append(": System::Runtime::InteropServices::_Exception(nullptr)\n");
				AppendIndent(
					throwerIndent + 2,
					builders.CppMethodDefinitions);
				builders.CppMethodDefinitions.Append(", System::Runtime::Serialization::ISerializable(nullptr)\n");
				AppendIndent(
					throwerIndent + 2,
					builders.CppMethodDefinitions);
				builders.CppMethodDefinitions.Append(", System::Exception(nullptr)\n");
				AppendIndent(
					throwerIndent + 2,
					builders.CppMethodDefinitions);
				builders.CppMethodDefinitions.Append(", System::SystemException(nullptr)\n");
				AppendIndent(
					throwerIndent + 2,
					builders.CppMethodDefinitions);
				builders.CppMethodDefinitions.Append(", ");
				AppendCppTypeFullName(
					exceptionType,
					builders.CppMethodDefinitions);
				builders.CppMethodDefinitions.Append("(Plugin::InternalUse::Only, handle)\n");
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
				builders.CppMethodDefinitions.Append('\n');
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
				AppendCppTypeFullName(
					exceptionType,
					builders.CppMethodDefinitions);
				builders.CppMethodDefinitions.Append("Thrower(handle);\n");
				builders.CppMethodDefinitions.Append("}\n\n");
				
				// Build parameters
				ParameterInfo[] parameters = ConvertParameters(
					new[]{ typeof(int) });
				
				// C# imports
				AppendCsharpImport(
					GetTypeName(string.Empty, string.Empty),
					null,
					funcName,
					ConvertParameters(Type.EmptyTypes),
					typeof(void),
					builders.CsharpImports);
				
				// C# delegate
				AppendCsharpDelegate(
					true,
					GetTypeName(string.Empty, string.Empty),
					null,
					funcName,
					parameters,
					typeof(void),
					TypeKind.None,
					builders.CsharpDelegates
				);
				
				// C# GetDelegate call
				AppendCsharpGetDelegateCall(
					GetTypeName(string.Empty, string.Empty),
					null,
					funcName,
					builders.CsharpGetDelegateCalls);
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
			TypeKind fieldTypeKind,
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
			AppendFieldPropertyFuncName(
				GetTypeName(enclosingType),
				enclosingTypeParams,
				syntaxType,
				"Get",
				fieldName,
				builders.TempStrBuilder);
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
				parameters,
				builders.CsharpFunctions);
			AppendCsharpFunctionCallSubject(
				enclosingType,
				methodIsStatic,
				builders.CsharpFunctions);
			if (parameters.Length > 0)
			{
				builders.CsharpFunctions.Append('[');
				for (int i = 0; i < parameters.Length; ++i)
				{
					builders.CsharpFunctions.Append(parameters[0].Name);
					if (i != parameters.Length-1)
					{
						builders.CsharpFunctions.Append(", ");
					}
				}
				builders.CsharpFunctions.Append("]");
			}
			else
			{
				builders.CsharpFunctions.Append('.');
				builders.CsharpFunctions.Append(fieldName);
			}
			builders.CsharpFunctions.Append(';');
			if (!isReadOnly
				&& !methodIsStatic
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
				fieldTypeKind,
				exceptionTypes,
				false,
				builders.CsharpFunctions);

			// C++ function pointer
			AppendCppFunctionPointerDefinition(
				funcName,
				methodIsStatic,
				GetTypeName(enclosingType),
				enclosingTypeKind,
				parameters,
				fieldType,
				builders.CppFunctionPointers);

			// C++ method declaration
			AppendIndent(indent + 1, builders.CppTypeDefinitions);
			AppendCppMethodDeclaration(
				methodName,
				enclosingTypeIsStatic,
				false,
				methodIsStatic,
				fieldType,
				null,
				parameters,
				builders.CppTypeDefinitions);
			
			// C++ method definition
			AppendCppMethodDefinitionBegin(
				GetTypeName(enclosingType),
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
				GetTypeName(enclosingType),
				enclosingTypeKind,
				enclosingTypeParams,
				fieldType,
				funcName,
				parameters,
				indent + 1,
				builders.CppMethodDefinitions);
			AppendCppMethodReturn(
				fieldType,
				fieldTypeKind,
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
				GetTypeName(enclosingType),
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
			int indent,
			Type[] exceptionTypes,
			StringBuilders builders)
		{
			TypeName enclosingTypeTypeName = GetTypeName(enclosingType);

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
			AppendFieldPropertyFuncName(
				enclosingTypeTypeName,
				enclosingTypeParams,
				syntaxType,
				"Set",
				fieldName,
				builders.TempStrBuilder);
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
				parameters,
				builders.CsharpFunctions);
			AppendCsharpFunctionCallSubject(
				enclosingType,
				methodIsStatic,
				builders.CsharpFunctions);
			if (parameters.Length > 1)
			{
				builders.CsharpFunctions.Append('[');
				for (int i = 0, end = parameters.Length-1; i < end; ++i)
				{
					builders.CsharpFunctions.Append(parameters[i].Name);
					if (i != end-1)
					{
						builders.CsharpFunctions.Append(", ");
					}
				}
				builders.CsharpFunctions.Append("] = ");
				builders.CsharpFunctions.Append(parameters[1].Name);
			}
			else
			{
				builders.CsharpFunctions.Append('.');
				builders.CsharpFunctions.Append(fieldName);
				builders.CsharpFunctions.Append(" = ");
				builders.CsharpFunctions.Append("value");
			}
			builders.CsharpFunctions.Append(';');
			if (!isReadOnly
				&& !methodIsStatic
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
				TypeKind.None,
				exceptionTypes,
				false,
				builders.CsharpFunctions);
			
			// C++ function pointer
			AppendCppFunctionPointerDefinition(
				funcName,
				methodIsStatic,
				enclosingTypeTypeName,
				enclosingTypeKind,
				parameters,
				typeof(void),
				builders.CppFunctionPointers);
			
			// C++ method declaration
			AppendIndent(indent + 1, builders.CppTypeDefinitions);
			AppendCppMethodDeclaration(
				methodName,
				enclosingTypeIsStatic,
				false,
				methodIsStatic,
				typeof(void),
				null,
				parameters,
				builders.CppTypeDefinitions);
			
			// C++ method definition
			AppendCppMethodDefinitionBegin(
				GetTypeName(enclosingType),
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
				enclosingTypeTypeName,
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
				enclosingTypeTypeName,
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
		
		static void AppendFieldPropertyFuncName(
			TypeName enclosingTypeTypeName,
			Type[] enclosingTypeParams,
			string syntaxType,
			string operationType,
			string fieldName,
			StringBuilder output)
		{
			AppendNamespace(
				enclosingTypeTypeName.Namespace,
				string.Empty,
				output);
			AppendTypeNameWithoutGenericSuffix(
				enclosingTypeTypeName.Name,
				output);
			AppendTypeNames(
				enclosingTypeParams,
				output);
			output.Append(syntaxType);
			output.Append(operationType);
			output.Append(char.ToUpper(fieldName[0]));
			output.Append(fieldName, 1, fieldName.Length-1);
		}
		
		static void AppendCppTemplateDeclaration(
			TypeName typeTypeName,
			StringBuilder output)
		{
			int indent = AppendNamespaceBeginning(
				typeTypeName.Namespace,
				output);
			AppendIndent(
				indent,
				output);
			AppendCppTemplateTypenames(
				typeTypeName.NumTypeParams,
				'T',
				output);
			output.Append("struct ");
			AppendCppTypeName(
				typeTypeName,
				output);
			output.Append(";");
			output.Append('\n');
			AppendNamespaceEnding(
				indent,
				output);
			output.Append('\n');
		}
		
		static int AppendCppTypeDeclaration(
			TypeName typeTypeName,
			bool isStatic,
			Type[] typeParams,
			StringBuilder output)
		{
			int indent = AppendNamespaceBeginning(
				typeTypeName.Namespace,
				output);
			AppendIndent(indent, output);
			if (isStatic)
			{
				output.Append("namespace ");
				AppendTypeNameWithoutGenericSuffix(
					typeTypeName.Name,
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
				AppendCppTypeName(
					typeTypeName,
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
			TypeName typeTypeName,
			TypeKind typeKind,
			Type[] typeParams,
			TypeName baseTypeTypeName,
			Type[] baseTypeTypeParams,
			Type[] interfaceTypes,
			bool isStatic,
			int indent,
			StringBuilder output)
		{
			AppendNamespaceBeginning(
				typeTypeName.Namespace,
				output);
			AppendIndent(
				indent,
				output);
			if (isStatic)
			{
				output.Append("namespace ");
				AppendTypeNameWithoutGenericSuffix(
					typeTypeName.Name,
					output);
			}
			else
			{
				if (typeParams != null)
				{
					output.Append("template<> ");
				}
				output.Append("struct ");
				AppendCppTypeName(
					typeTypeName,
					output);
				AppendCppTypeParameters(typeParams, output);
				switch (typeKind)
				{
					case TypeKind.Class:
						// Only add the base type if it's not System.Object or
						// there are no interfaces (since they always extend it)
						string separator = " : virtual ";
						if (
							(baseTypeTypeName.Name != null &&
								(baseTypeTypeName.Namespace != "System" ||
									baseTypeTypeName.Name != "Object")) ||
							(interfaceTypes == null ||
								interfaceTypes.Length == 0))
						{
							output.Append(separator);
							separator = ", virtual ";
							AppendCppTypeFullName(
								GetTypeName(
									baseTypeTypeName.Name ?? "Object",
									baseTypeTypeName.Namespace ?? "System",
									baseTypeTypeParams != null ? baseTypeTypeParams.Length : 0),
								output);
							AppendCppTypeParameters(
								baseTypeTypeParams,
								output);
						}
						if (interfaceTypes != null)
						{
							foreach (Type interfaceType in interfaceTypes)
							{
								output.Append(separator);
								separator = ", virtual ";
								AppendCppTypeFullName(
									GetTypeName(interfaceType),
									output);
								AppendCppTypeParameters(
									interfaceType.GetGenericArguments(),
									output);
							}
						}
						break;
					case TypeKind.ManagedStruct:
						output.Append(" : Plugin::ManagedType");
						break;
				}
			}
			output.Append('\n');
			AppendIndent(
				indent,
				output);
			output.Append("{\n");
			if (!isStatic)
			{
				switch (typeKind)
				{
					case TypeKind.Class:
					case TypeKind.ManagedStruct:
						// Constructor from nullptr
						AppendIndent(indent + 1, output);
						AppendCppTypeName(
							typeTypeName,
							output);
						AppendCppTypeParameters(
							typeParams,
							output);
						output.Append("(decltype(nullptr));\n");
						
						// Constructor from handle
						AppendIndent(indent + 1, output);
						AppendCppTypeName(
							typeTypeName,
							output);
						AppendCppTypeParameters(
							typeParams,
							output);
						output.Append(
							"(Plugin::InternalUse, int32_t handle);\n");
						
						// Copy constructor
						AppendIndent(indent + 1, output);
						AppendCppTypeName(
							typeTypeName,
							output);
						AppendCppTypeParameters(
							typeParams,
							output);
						output.Append("(const ");
						AppendCppTypeName(
							typeTypeName,
							output);
						AppendCppTypeParameters(
							typeParams,
							output);
						output.Append("& other);\n");
						
						// Move constructor
						AppendIndent(indent + 1, output);
						AppendCppTypeName(
							typeTypeName,
							output);
						AppendCppTypeParameters(
							typeParams,
							output);
						output.Append('(');
						AppendCppTypeName(
							typeTypeName,
							output);
						AppendCppTypeParameters(
							typeParams,
							output);
						output.Append("&& other);\n");
						
						// Destructor
						AppendIndent(indent + 1, output);
						output.Append("virtual ~");
						AppendCppTypeName(
							typeTypeName,
							output);
						AppendCppTypeParameters(
							typeParams,
							output);
						output.Append("();\n");
						
						// Assignment operator to same type
						AppendIndent(indent + 1, output);
						AppendCppTypeName(
							typeTypeName,
							output);
						AppendCppTypeParameters(
							typeParams,
							output);
						output.Append("& operator=(const ");
						AppendCppTypeName(
							typeTypeName,
							output);
						AppendCppTypeParameters(
							typeParams,
							output);
						output.Append("& other);\n");
						
						// Assignment operator to nullptr
						AppendIndent(indent + 1, output);
						AppendCppTypeName(
							typeTypeName,
							output);
						AppendCppTypeParameters(
							typeParams,
							output);
						output.Append("& operator=(decltype(nullptr));\n");
						
						// Move assignment operator to same type
						AppendIndent(indent + 1, output);
						AppendCppTypeName(
							typeTypeName,
							output);
						AppendCppTypeParameters(
							typeParams,
							output);
						output.Append("& operator=(");
						AppendCppTypeName(
							typeTypeName,
							output);
						AppendCppTypeParameters(
							typeParams,
							output);
						output.Append("&& other);\n");
						
						// Equality operator with same type
						AppendIndent(indent + 1, output);
						output.Append("bool operator==(const ");
						AppendCppTypeName(
							typeTypeName,
							output);
						AppendCppTypeParameters(
							typeParams,
							output);
						output.Append("& other) const;\n");
						
						// Inequality operator with same type
						AppendIndent(indent + 1, output);
						output.Append("bool operator!=(const ");
						AppendCppTypeName(
							typeTypeName,
							output);
						AppendCppTypeParameters(
							typeParams,
							output);
						output.Append("& other) const;\n");
						break;
				}
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
		
		static int AppendCppMethodDefinitionsBegin(
			TypeName enclosingTypeTypeName,
			TypeKind enclosingTypeKind,
			Type[] enclosingTypeParams,
			Type[] interfaceTypes,
			bool isStatic,
			Action<int, string> extraDefault,
			Action<int, string> extraCopy,
			int indent,
			StringBuilder output)
		{
			int cppMethodDefinitionsIndent = AppendNamespaceBeginning(
				enclosingTypeTypeName.Namespace,
				output);
			if (!isStatic && (
				enclosingTypeKind == TypeKind.Class
				|| enclosingTypeKind == TypeKind.ManagedStruct))
			{
				// Construct with nullptr
				AppendIndent(indent, output);
				AppendCppTypeName(
					enclosingTypeTypeName,
					output);
				AppendCppTypeParameters(
					enclosingTypeParams,
					output);
				output.Append("::");
				AppendCppTypeName(
					enclosingTypeTypeName,
					output);
				output.Append("(decltype(nullptr))\n");
				if (enclosingTypeKind == TypeKind.Class)
				{
					AppendCppConstructorInitializerList(
						interfaceTypes,
						indent + 1,
						output);
				}
				AppendIndent(indent, output);
				output.Append("{\n");
				extraDefault(indent + 1, "this->");
				AppendIndent(indent, output);
				output.Append("}\n");
				AppendIndent(indent, output);
				output.Append('\n');
				
				// Handle constructor
				AppendIndent(indent, output);
				AppendCppTypeName(
					enclosingTypeTypeName,
					output);
				AppendCppTypeParameters(
					enclosingTypeParams,
					output);
				output.Append("::");
				AppendCppTypeName(
					enclosingTypeTypeName,
					output);
				output.Append("(Plugin::InternalUse, int32_t handle)\n");
				if (enclosingTypeKind == TypeKind.Class)
				{
					AppendCppConstructorInitializerList(
						interfaceTypes,
						indent + 1,
						output);
				}
				AppendIndent(indent, output);
				output.Append("{\n");
				AppendIndent(indent + 1, output);
				output.Append("Handle = handle;\n");
				AppendIndent(indent + 1, output);
				output.Append("if (handle)\n");
				AppendIndent(indent + 1, output);
				output.Append("{\n");
				AppendIndent(indent + 2, output);
				AppendReferenceManagedHandleFunctionCall(
					enclosingTypeTypeName,
					enclosingTypeKind,
					enclosingTypeParams,
					"handle",
					output);
				output.Append(";\n");
				AppendIndent(indent + 1, output);
				output.Append("}\n");
				extraDefault(indent + 1, "this->");
				AppendIndent(indent, output);
				output.Append("}\n");
				AppendIndent(indent, output);
				output.Append('\n');
				
				// Copy constructor
				AppendIndent(indent, output);
				AppendCppTypeName(
					enclosingTypeTypeName,
					output);
				AppendCppTypeParameters(
					enclosingTypeParams,
					output);
				output.Append("::");
				AppendCppTypeName(
					enclosingTypeTypeName,
					output);
				output.Append("(const ");
				AppendCppTypeName(
					enclosingTypeTypeName,
					output);
				AppendCppTypeParameters(
					enclosingTypeParams,
					output);
				output.Append("& other)\n");
				AppendIndent(indent + 1, output);
				output.Append(": ");
				AppendCppTypeName(
					enclosingTypeTypeName,
					output);
				output.Append("(Plugin::InternalUse::Only, other.Handle)\n");
				AppendIndent(indent, output);
				output.Append("{\n");
				extraCopy(indent + 1, "other.");
				AppendIndent(indent, output);
				output.Append("}\n");
				AppendIndent(indent, output);
				output.Append('\n');
				
				// Move constructor
				AppendIndent(indent, output);
				AppendCppTypeName(
					enclosingTypeTypeName,
					output);
				AppendCppTypeParameters(
					enclosingTypeParams,
					output);
				output.Append("::");
				AppendCppTypeName(
					enclosingTypeTypeName,
					output);
				output.Append("(");
				AppendCppTypeName(
					enclosingTypeTypeName,
					output);
				AppendCppTypeParameters(
					enclosingTypeParams,
					output);
				output.Append("&& other)\n");
				AppendIndent(indent, output);
				output.Append("\t: ");
				AppendCppTypeName(
					enclosingTypeTypeName,
					output);
				output.Append("(Plugin::InternalUse::Only, other.Handle)\n");
				AppendIndent(indent, output);
				output.Append("{\n");
				AppendIndent(indent + 1, output);
				output.Append("other.Handle = 0;\n");
				extraCopy(indent + 1, "other.");
				extraDefault(indent + 1, "other.");
				AppendIndent(indent, output);
				output.Append("}\n");
				AppendIndent(indent, output);
				output.Append('\n');

				// Destructor
				AppendIndent(indent, output);
				AppendCppTypeName(
					enclosingTypeTypeName,
					output);
				AppendCppTypeParameters(
					enclosingTypeParams,
					output);
				output.Append("::~");
				AppendCppTypeName(
					enclosingTypeTypeName,
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
					enclosingTypeTypeName,
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
				output.Append('\n');
				
				// Assignment operator to same type
				AppendIndent(indent, output);
				AppendCppTypeName(
					enclosingTypeTypeName,
					output);
				AppendCppTypeParameters(
					enclosingTypeParams,
					output);
				output.Append("& ");
				AppendCppTypeName(
					enclosingTypeTypeName,
					output);
				AppendCppTypeParameters(
					enclosingTypeParams,
					output);
				output.Append("::operator=(const ");
				AppendCppTypeName(
					enclosingTypeTypeName,
					output);
				AppendCppTypeParameters(
					enclosingTypeParams,
					output);
				output.Append("& other)\n");
				AppendIndent(indent, output);
				output.Append("{\n");
				AppendSetHandle(
					enclosingTypeTypeName,
					enclosingTypeKind,
					enclosingTypeParams,
					indent + 1,
					"this",
					"other.Handle",
					output);
				extraCopy(indent + 1, "other.");
				AppendIndent(indent + 1, output);
				output.Append("return *this;\n");
				AppendIndent(indent, output);
				output.Append("}\n");
				AppendIndent(indent, output);
				output.Append('\n');
				
				// Assignment operator to nullptr
				AppendIndent(indent, output);
				AppendCppTypeName(
					enclosingTypeTypeName,
					output);
				AppendCppTypeParameters(
					enclosingTypeParams,
					output);
				output.Append("& ");
				AppendCppTypeName(
					enclosingTypeTypeName,
					output);
				AppendCppTypeParameters(
					enclosingTypeParams,
					output);
				output.Append("::operator=(decltype(nullptr))\n");
				AppendIndent(indent, output);
				output.Append("{\n");
				AppendIndent(indent + 1, output);
				output.Append("if (Handle)\n");
				AppendIndent(indent + 1, output);
				output.Append("{\n");
				AppendIndent(indent + 2, output);
				AppendDereferenceManagedHandleFunctionCall(
					enclosingTypeTypeName,
					enclosingTypeKind,
					enclosingTypeParams,
					"Handle",
					output);
				output.Append(";\n");
				AppendIndent(indent + 2, output);
				output.Append("Handle = 0;\n");
				AppendIndent(indent + 1, output);
				output.Append("}\n");
				AppendIndent(indent + 1, output);
				output.Append("return *this;\n");
				AppendIndent(indent, output);
				output.Append("}\n");
				AppendIndent(indent, output);
				output.Append('\n');
				
				// Move assignment operator to same type
				AppendIndent(indent, output);
				AppendCppTypeName(
					enclosingTypeTypeName,
					output);
				AppendCppTypeParameters(
					enclosingTypeParams,
					output);
				output.Append("& ");
				AppendCppTypeName(
					enclosingTypeTypeName,
					output);
				AppendCppTypeParameters(
					enclosingTypeParams,
					output);
				output.Append("::operator=(");
				AppendCppTypeName(
					enclosingTypeTypeName,
					output);
				AppendCppTypeParameters(
					enclosingTypeParams,
					output);
				output.Append("&& other)\n");
				AppendIndent(indent, output);
				output.Append("{\n");
				AppendIndent(indent + 1, output);
				output.Append("if (Handle)\n");
				AppendIndent(indent + 1, output);
				output.Append("{\n");
				AppendIndent(indent + 2, output);
				AppendDereferenceManagedHandleFunctionCall(
					enclosingTypeTypeName,
					enclosingTypeKind,
					enclosingTypeParams,
					"Handle",
					output);
				output.Append(";\n");
				AppendIndent(indent + 1, output);
				output.Append("}\n");
				AppendIndent(indent + 1, output);
				output.Append("Handle = other.Handle;\n");
				extraCopy(indent + 1, "other.");
				AppendIndent(indent + 1, output);
				output.Append("other.Handle = 0;\n");
				extraDefault(indent + 1, "other.");
				AppendIndent(indent + 1, output);
				output.Append("return *this;\n");
				AppendIndent(indent, output);
				output.Append("}\n");
				AppendIndent(indent, output);
				output.Append('\n');
				
				// Equality operator with same type
				AppendIndent(indent, output);
				output.Append("bool ");
				AppendCppTypeName(
					enclosingTypeTypeName,
					output);
				AppendCppTypeParameters(
					enclosingTypeParams,
					output);
				output.Append("::operator==(const ");
				AppendCppTypeName(
					enclosingTypeTypeName,
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
				AppendCppTypeName(
					enclosingTypeTypeName,
					output);
				AppendCppTypeParameters(
					enclosingTypeParams,
					output);
				output.Append("::operator!=(const ");
				AppendCppTypeName(
					enclosingTypeTypeName,
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
			TypeName enclosingTypeTypeName,
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
			output.Append(")\n");
			AppendIndent(indent, output);
			output.Append("{\n");
			AppendIndent(indent + 1, output);
			AppendDereferenceManagedHandleFunctionCall(
				enclosingTypeTypeName,
				enclosingTypeKind,
				enclosingTypeParams,
				thisHandleExpression,
				output);
			output.Append(";\n");
			AppendIndent(indent, output);
			output.Append("}\n");
			AppendIndent(indent, output);
			output.Append(thisHandleExpression);
			output.Append(" = ");
			output.Append(otherHandleExpression);
			output.Append(";\n");
			AppendIndent(indent, output);
			output.Append("if (");
			output.Append(thisHandleExpression);
			output.Append(")\n");
			AppendIndent(indent, output);
			output.Append("{\n");
			AppendIndent(indent + 1, output);
			AppendReferenceManagedHandleFunctionCall(
				enclosingTypeTypeName,
				enclosingTypeKind,
				enclosingTypeParams,
				thisHandleExpression,
				output);
			output.Append(";\n");
			AppendIndent(indent, output);
			output.Append("}\n");
		}
		
		static void AppendReferenceManagedHandleFunctionCall(
			TypeName enclosingTypeTypeName,
			TypeKind enclosingTypeKind,
			Type[] enclosingTypeParams,
			string handleVariable,
			StringBuilder output)
		{
			if (enclosingTypeKind == TypeKind.ManagedStruct)
			{
				output.Append("Plugin::ReferenceManaged");
				AppendReleaseFunctionNameSuffix(
					enclosingTypeTypeName,
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
			TypeName enclosingTypeTypeName,
			TypeKind enclosingTypeKind,
			Type[] enclosingTypeParams,
			string handleVariable,
			StringBuilder output)
		{
			if (enclosingTypeKind == TypeKind.ManagedStruct)
			{
				output.Append("Plugin::DereferenceManaged");
				AppendReleaseFunctionNameSuffix(
					enclosingTypeTypeName,
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
		
		static void AppendCppMethodDefinitionsEnd(
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
				AppendCsharpTypeFullName(
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
					AppendCsharpTypeFullName(
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
			AppendCsharpBindingParameterDeclaration(
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
					AppendCsharpTypeFullName(
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
					AppendCsharpTypeFullName(
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
			AppendCsharpBindingParameterDeclaration(
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
				AppendCsharpTypeFullName(
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
					if (paramType != typeof(object))
					{
						output.Append('(');
						AppendCsharpTypeFullName(paramType, output);
						output.Append(')');
					}
					AppendHandleStoreTypeName(paramType, output);
					output.Append(".Get(");
					output.Append(param.Name);
					output.Append("Handle);\n\t\t\t\t");
				}
			}
			
			// Save return value as local variable
			if (returnType != typeof(void))
			{
				output.Append("var returnValue = ");
			}
		}
		
		static void AppendCsharpFunctionCallSubject(
			Type enclosingType,
			bool isStatic,
			StringBuilder output)
		{
			if (isStatic)
			{
				AppendCsharpTypeFullName(
					enclosingType,
					output);
			}
			else
			{
				output.Append("thiz");
			}
		}
		
		static void AppendCsharpFunctionCallParameters(
			ParameterInfo[] parameters,
			StringBuilder output)
		{
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
			TypeKind returnTypeKind,
			Type[] exceptionTypes,
			bool forceReturnReturnValue,
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
			if (returnType != typeof(void))
			{
				output.Append("\n\t\t\t\treturn ");
				if (
					forceReturnReturnValue
					|| returnTypeKind == TypeKind.Enum
					|| returnTypeKind == TypeKind.FullStruct
					|| returnTypeKind == TypeKind.Primitive)
				{
					output.Append("returnValue");
				}
				else
				{
					AppendHandleStoreTypeName(
						returnType,
						output);
					output.Append('.');
					if (returnTypeKind == TypeKind.Class)
					{
						output.Append("GetHandle");
					}
					else
					{
						output.Append("Store");
					}
					output.Append("(returnValue)");
				}
				output.Append(';');
			}
			
			// Returning ends the function
			AppendCsharpFunctionEnd(
				returnType,
				exceptionTypes,
				parameters,
				output);
		}
		
		static void AppendCsharpFunctionEnd(
			Type returnType,
			Type[] exceptionTypes,
			ParameterInfo[] parameters,
			StringBuilder output)
		{
			output.Append('\n');
			output.Append("\t\t\t}\n");
			if (exceptionTypes == null
				|| Array.IndexOf(
				exceptionTypes,
				typeof(NullReferenceException)) < 0)
			{
				AppendCsharpCatchException(
					typeof(NullReferenceException),
					returnType,
					parameters,
					output);
			}
			if (exceptionTypes != null)
			{
				foreach (Type exceptionType in exceptionTypes)
				{
					AppendCsharpCatchException(
						exceptionType,
						returnType,
						parameters,
						output);
				}
			}
			AppendCsharpCatchException(
				typeof(Exception),
				returnType,
				parameters,
				output);
			output.Append("\t\t}\n");
			output.Append("\t\t\n");
		}
		
		static void AppendCsharpCatchException(
			Type exceptionType,
			Type returnType,
			ParameterInfo[] parameters,
			StringBuilder output)
		{
			output.Append("\t\t\tcatch (");
			AppendCsharpTypeFullName(
				exceptionType,
				output);
			output.Append(" ex)\n");
			output.Append("\t\t\t{\n");
			output.Append("\t\t\t\tUnityEngine.Debug.LogException(ex);\n");
			output.Append("\t\t\t\tNativeScript.Bindings.");
			AppendCsharpSetCsharpExceptionFunctionName(
				exceptionType,
				output);
			output.Append("(NativeScript.Bindings.ObjectStore.Store(ex));\n");
			foreach (ParameterInfo param in parameters)
			{
				if (param.IsOut)
				{
					output.Append("\t\t\t\t");
					output.Append(param.Name);
					if (param.Kind == TypeKind.Class
						|| param.Kind == TypeKind.ManagedStruct)
					{
						output.Append("Handle = default(int);\n");
					}
					else
					{
						output.Append(" = default(");
						AppendCsharpTypeFullName(
							param.DereferencedParameterType,
							output);
						output.Append(");\n");
					}
				}
			}
			if (returnType != typeof(void))
			{
				output.Append("\t\t\t\treturn default(");
				if (IsFullValueType(returnType))
				{
					AppendCsharpTypeFullName(
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
				AppendTypeNameWithoutGenericSuffix(
					exceptionType.Name,
					output);
			}
		}
		
		static void AppendCsharpBindingParameterDeclaration(
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
						AppendCsharpTypeFullName(
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
			Type[] methodTypeParameters,
			bool includeDefaults,
			StringBuilder output)
		{
			bool hasVarArgs = parameters.Length > 0 &&
				parameters[parameters.Length-1].IsVarArg;
			for (int i = 0; i < parameters.Length; ++i)
			{
				ParameterInfo param = parameters[i];

				if (!param.IsOut && !param.IsRef)
				{
					output.Append("const ");
				}

				Type paramType = param.DereferencedParameterType;
				
				int typeParamIndex = ArrayIndexOf(
					methodTypeParameters,
					paramType);
				if (typeParamIndex >= 0)
				{
					output.Append("MT");
					output.Append(typeParamIndex);
				}
				else
				{
					AppendCppTypeFullName(
						paramType,
						output);
				}

				// Pointer (*) or reference (&) suffix if necessary
				if (param.IsOut || param.IsRef)
				{
					output.Append('*');
				}
				else if (
					param.Kind == TypeKind.FullStruct ||
					param.Kind == TypeKind.ManagedStruct ||
					param.Kind == TypeKind.Class ||
					param.IsVirtual)
				{
					output.Append('&');
				}
				
				// Param name
				output.Append(' ');
				output.Append(param.Name);
				
				// Default if desired, present, and the method has no var args
				if (includeDefaults && param.HasDefault && !hasVarArgs)
				{
					output.Append(" = ");
					if (param.DereferencedParameterType == typeof(string))
					{
						if (param.DefaultValue != null)
						{
							throw new Exception(
								"Non-null string default parameters aren't supported");
						}
						output.Append("Plugin::NullString");
					}
					else if (param.DefaultValue is bool)
					{
						bool val = (bool)param.DefaultValue;
						output.Append(val ? "true" : "false");
					}
					else if (param.DefaultValue is char)
					{
						char val = (char)param.DefaultValue;
						output.Append('\'');
						output.Append(val);
						output.Append('\'');
					}
					else if ((param.DefaultValue is sbyte) ||
						(param.DefaultValue is byte) ||
						(param.DefaultValue is short) ||
						(param.DefaultValue is ushort) ||
						(param.DefaultValue is int) ||
						(param.DefaultValue is uint) ||
						(param.DefaultValue is long) ||
						(param.DefaultValue is ulong))
					{
						output.Append(param.DefaultValue);
					}
					else
					{
						Type type = param.DefaultValue.GetType();
						if (type.IsEnum)
						{
							AppendCppTypeFullName(
								type,
								output);
							output.Append("::");
							output.Append(param.DefaultValue);
						}
						else
						{
							StringBuilder error = new StringBuilder();
							error.Append("Default parameter type (");
							AppendCsharpTypeFullName(
								param.DefaultValue.GetType(),
								error);
							error.Append(") not supported");
							throw new Exception(error.ToString());
						}
					}
				}
				
				if (i != parameters.Length - 1)
				{
					output.Append(", ");
				}
			}
		}
		
		static void AppendDefaultStringParamName(
			string str,
			StringBuilder output)
		{
			foreach (char c in str)
			{
				if (char.IsLetterOrDigit(c))
				{
					output.Append(c);
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
		
		static void AppendCppMethodDefinitionBegin(
			TypeName enclosingTypeTypeName,
			Type returnType,
			string methodName,
			Type[] enclosingTypeTypeParams,
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
				AppendCppTypeFullName(
					returnType,
					output);
				output.Append(' ');
			}

			// Type name
			AppendCppTypeFullName(
				enclosingTypeTypeName,
				output);
			AppendCppTypeParameters(
				enclosingTypeTypeParams,
				output);
			output.Append("::");
			
			// Method name
			AppendTypeNameWithoutGenericSuffix(
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
				null, // don't substitute method type params
				false,
				output);
			output.Append(")\n");
		}
		
		static void AppendCppMethodReturn(
			Type returnType,
			TypeKind returnTypeKind,
			int indent,
			StringBuilder output)
		{
			if (returnType != null && returnType != typeof(void))
			{
				AppendIndent(indent, output);
				output.Append("return ");
				switch (returnTypeKind)
				{
					case TypeKind.Enum:
					case TypeKind.FullStruct:
					case TypeKind.Primitive:
						output.Append("returnValue");
						break;
					default:
						AppendCppTypeFullName(
							returnType,
							output);
						output.Append("(Plugin::InternalUse::Only, returnValue)");
						break;
				}
				output.Append(";\n");
			}
		}
		
		static void AppendCppPluginFunctionCall(
			bool isStatic,
			TypeName enclosingTypeTypeName,
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
					case TypeKind.Enum:
						output.Append(param.Name);
						break;
					case TypeKind.Primitive:
						if (param.IsOut || param.IsRef)
						{
							output.Append("&");
							output.Append(param.Name);
							output.Append("->Value");
						}
						else
						{
							output.Append(param.Name);
						}
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
			
			AppendCppUnhandledExceptionHandling(
				indent,
				output);
			
			// Set out and ref parameters
			foreach (ParameterInfo param in parameters)
			{
				if ((param.Kind == TypeKind.Class
					|| param.Kind == TypeKind.ManagedStruct)
					&& (param.IsOut || param.IsRef))
				{
					AppendSetHandle(
						enclosingTypeTypeName,
						enclosingTypeKind,
						enclosingTypeParams,
						indent,
						param.Name,
						param.Name + "Handle",
						output);
				}
			}
		}
		
		static void AppendCppUnhandledExceptionHandling(
			int indent,
			StringBuilder output)
		{
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
		}
		
		static void AppendCppInitParam(
			string funcName,
			bool isStatic,
			TypeName enclosingTypeTypeName,
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
				enclosingTypeTypeName,
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
			TypeName enclosingTypeTypeName,
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
				enclosingTypeTypeName,
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
			TypeName enclosingTypeTypeName,
			TypeKind enclosingTypeKind,
			ParameterInfo[] parameters,
			Type returnType,
			char separator,
			StringBuilder output)
		{
			// Return type
			if (returnType == typeof(bool))
			{
				// C linkage requires us to use primitive types
				output.Append("int32_t");
			}
			else if (returnType == typeof(char))
			{
				// C linkage requires us to use primitive types
				output.Append("int16_t");
			}
			else if (IsFullValueType(returnType))
			{
				AppendCppTypeFullName(returnType, output);
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
						AppendCppTypeFullName(
							enclosingTypeTypeName,
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

				if (!param.IsOut && !param.IsRef)
				{
					output.Append("const ");
				}

				switch (param.Kind)
				{
					case TypeKind.Primitive:
						AppendCppPrimitiveTypeName(
							param.DereferencedParameterType,
							output);
						if (param.IsOut || param.IsRef)
						{
							output.Append('*');
						}
						break;
					case TypeKind.Enum:
						AppendCppTypeFullName(
							param.DereferencedParameterType,
							output);
						if (param.IsOut || param.IsRef)
						{
							output.Append('*');
						}
						break;
					case TypeKind.FullStruct:
						AppendCppTypeFullName(
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
			char prefix,
			StringBuilder output)
		{
			if (numTypeParameters > 0)
			{
				output.Append("template<");
				for (int i = 0; i < numTypeParameters; ++i)
				{
					output.Append("typename ");
					output.Append(prefix);
					output.Append('T');
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
			bool methodIsVirtual,
			bool methodIsStatic,
			Type returnType,
			Type[] methodTypeParameters,
			ParameterInfo[] parameters,
			StringBuilder output)
		{
			AppendCppTemplateTypenames(
				methodTypeParameters == null ? 0 : methodTypeParameters.Length,
				'M',
				output);
			
			if (!enclosingTypeIsStatic && methodIsStatic)
			{
				output.Append("static ");
			}
			
			if (methodIsVirtual)
			{
				output.Append("virtual ");
			}
			
			// Return type
			if (returnType != null)
			{
				int typeParamIndex = ArrayIndexOf(
					methodTypeParameters,
					returnType);
				if (typeParamIndex >= 0)
				{
					output.Append("MT");
					output.Append(typeParamIndex);
				}
				else
				{
					AppendCppTypeFullName(
						returnType,
						output);
				}
				output.Append(' ');
			}
			
			// Method name might be a constructor/type name, so remove suffix
			// just in case
			AppendTypeNameWithoutGenericSuffix(
				methodName,
				output);
			
			// Parameters
			output.Append('(');
			AppendCppParameterDeclaration(
				parameters,
				methodTypeParameters,
				true,
				output);
			output.Append(')');
			
			output.Append(";\n");
		}
		
		static void AppendCsharpTypeFullName(
			Type type,
			StringBuilder output)
		{
			if (type == typeof(void))
			{
				output.Append("void");
			}
			else if (type == typeof(bool))
			{
				output.Append("bool");
			}
			else if (type == typeof(sbyte))
			{
				output.Append("sbyte");
			}
			else if (type == typeof(byte))
			{
				output.Append("byte");
			}
			else if (type == typeof(short))
			{
				output.Append("short");
			}
			else if (type == typeof(ushort))
			{
				output.Append("ushort");
			}
			else if (type == typeof(int))
			{
				output.Append("int");
			}
			else if (type == typeof(uint))
			{
				output.Append("uint");
			}
			else if (type == typeof(long))
			{
				output.Append("long");
			}
			else if (type == typeof(ulong))
			{
				output.Append("ulong");
			}
			else if (type == typeof(char))
			{
				output.Append("char");
			}
			else if (type == typeof(float))
			{
				output.Append("float");
			}
			else if (type == typeof(double))
			{
				output.Append("double");
			}
			else if (type == typeof(string))
			{
				output.Append("string");
			}
			else if (type == typeof(object))
			{
				output.Append("object");
			}
			else if (type.IsArray)
			{
				AppendCsharpTypeFullName(
					type.GetElementType(),
					output);
				output.Append('[');
				output.Append(',', type.GetArrayRank()-1);
				output.Append(']');
			}
			else
			{
				AppendCsharpTypeFullName(GetTypeName(type), output);
				Type[] genTypes = type.GetGenericArguments();
				AppendCSharpTypeParameters(
					genTypes,
					output);
			}
		}

		static void AppendCsharpTypeFullName(
			TypeName typeName,
			StringBuilder output)
		{
			if (!string.IsNullOrEmpty(typeName.Namespace))
			{
				output.Append(typeName.Namespace);
				output.Append('.');
			}
			AppendTypeNameWithoutGenericSuffix(typeName.Name, output);
		}
		
		static void AppendCppTypeFullName(
			Type type,
			StringBuilder output)
		{
			if (type == typeof(void))
			{
				output.Append("void");
			}
			else if (type == typeof(bool))
			{
				output.Append("System::Boolean");
			}
			else if (type == typeof(sbyte))
			{
				output.Append("System::SByte");
			}
			else if (type == typeof(byte))
			{
				output.Append("System::Byte");
			}
			else if (type == typeof(short))
			{
				output.Append("System::Int16");
			}
			else if (type == typeof(ushort))
			{
				output.Append("System::UInt16");
			}
			else if (type == typeof(int))
			{
				output.Append("System::Int32");
			}
			else if (type == typeof(uint))
			{
				output.Append("System::UInt32");
			}
			else if (type == typeof(long))
			{
				output.Append("System::Int64");
			}
			else if (type == typeof(ulong))
			{
				output.Append("System::UInt64");
			}
			else if (type == typeof(char))
			{
				output.Append("System::Char");
			}
			else if (type == typeof(float))
			{
				output.Append("System::Single");
			}
			else if (type == typeof(double))
			{
				output.Append("System::Double");
			}
			else if (type == typeof(string))
			{
				output.Append("System::String");
			}
			else if (type == typeof(IntPtr))
			{
				output.Append("void*");
			}
			else if (type.IsArray)
			{
				int rank = type.GetArrayRank();
				output.Append("System::Array");
				output.Append(rank);
				output.Append('<');
				Type elementType = type.GetElementType();
				for (int i = 0; i < rank; ++i)
				{
					AppendCppTypeFullName(
						elementType,
						output);
					if (i != rank -1)
					{
						output.Append(", ");
					}
				}
				output.Append('>');
			}
			else if (IsDelegate(type))
			{
				AppendCppTypeFullName(
					GetTypeName(type),
					output);
				Type[] genTypes = type.GetGenericArguments();
				AppendCppTypeParameters(
					genTypes,
					output);
			}
			else
			{
				TypeName typeName = GetTypeName(type);
				AppendCppTypeFullName(typeName, output);
				Type[] genTypes = type.GetGenericArguments();
				AppendCppTypeParameters(genTypes, output);
			}
		}
		
		static void AppendCppTypeFullName(
			TypeName typeName,
			StringBuilder output)
		{
			AppendNamespace(typeName.Namespace, "::", output);
			if (!string.IsNullOrEmpty(typeName.Namespace))
			{
				output.Append("::");
			}
			AppendCppTypeName(typeName, output);
		}

		static void AppendCppTypeName(
			TypeName typeName,
			StringBuilder output)
		{
			AppendTypeNameWithoutGenericSuffix(typeName.Name, output);
			if (typeName.NumTypeParams > 0)
			{
				output.Append('_');
				output.Append(typeName.NumTypeParams);
			}
		}
		
		static void AppendCppPrimitiveTypeName(
			Type type,
			StringBuilder output)
		{
			if (type == typeof(void))
			{
				output.Append("void");
			}
			else if (type == typeof(bool))
			{
				output.Append("uint32_t"); // C# bool is 4 bytes
			}
			else if (type == typeof(sbyte))
			{
				output.Append("int8_t");
			}
			else if (type == typeof(byte))
			{
				output.Append("uint8_t");
			}
			else if (type == typeof(short))
			{
				output.Append("int16_t");
			}
			else if (type == typeof(ushort))
			{
				output.Append("uint16_t");
			}
			else if (type == typeof(int))
			{
				output.Append("int32_t");
			}
			else if (type == typeof(uint))
			{
				output.Append("uint32_t");
			}
			else if (type == typeof(long))
			{
				output.Append("int64_t");
			}
			else if (type == typeof(ulong))
			{
				output.Append("uint64_t");
			}
			else if (type == typeof(char))
			{
				output.Append("uint16_t"); // C# char is 2 bytes
			}
			else if (type == typeof(float))
			{
				output.Append("float");
			}
			else if (type == typeof(double))
			{
				output.Append("double");
			}
			else if (type == typeof(IntPtr))
			{
				output.Append("void*");
			}
			else
			{
				throw new Exception(type + " is not a C++ primitive");
			}
		}
		
		static void RemoveTrailingChars(
			StringBuilders builders)
		{
			RemoveTrailingChars(builders.CsharpInitParams);
			RemoveTrailingChars(builders.CsharpDelegateTypes);
			RemoveTrailingChars(builders.CsharpStoreInitCalls);
			RemoveTrailingChars(builders.CsharpInitCall);
			RemoveTrailingChars(builders.CsharpBaseTypes);
			RemoveTrailingChars(builders.CsharpFunctions);
			RemoveTrailingChars(builders.CsharpDelegates);
			RemoveTrailingChars(builders.CsharpImports);
			RemoveTrailingChars(builders.CsharpGetDelegateCalls);
			RemoveTrailingChars(builders.CsharpDestroyFunctionEnumerators);
			RemoveTrailingChars(builders.CsharpDestroyQueueCases);
			RemoveTrailingChars(builders.CppFunctionPointers);
			RemoveTrailingChars(builders.CppTypeDeclarations);
			RemoveTrailingChars(builders.CppTemplateDeclarations);
			RemoveTrailingChars(builders.CppTemplateSpecializationDeclarations);
			RemoveTrailingChars(builders.CppTypeDefinitions);
			RemoveTrailingChars(builders.CppMethodDefinitions);
			RemoveTrailingChars(builders.CppInitParams);
			RemoveTrailingChars(builders.CppInitBody);
			RemoveTrailingChars(builders.CppInitBodyFirstBoot);
			RemoveTrailingChars(builders.CppGlobalStateAndFunctions);
			RemoveTrailingChars(builders.CppUnboxingMethodDeclarations);
			RemoveTrailingChars(builders.CppStringDefaultParams);
			RemoveTrailingChars(builders.CppMacros);
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
				"/*BEGIN STORE INIT CALLS*/\n",
				"\n\t\t\t/*END STORE INIT CALLS*/",
				builders.CsharpStoreInitCalls.ToString());
			csharpContents = InjectIntoString(
				csharpContents,
				"/*BEGIN INIT CALL*/\n",
				"\n\t\t\t\t/*END INIT CALL*/",
				builders.CsharpInitCall.ToString());
			csharpContents = InjectIntoString(
				csharpContents,
				"/*BEGIN BASE TYPES*/\n",
				"\n/*END BASE TYPES*/",
				builders.CsharpBaseTypes.ToString());
			csharpContents = InjectIntoString(
				csharpContents,
				"/*BEGIN FUNCTIONS*/\n",
				"\n\t\t/*END FUNCTIONS*/",
				builders.CsharpFunctions.ToString());
			csharpContents = InjectIntoString(
				csharpContents,
				"/*BEGIN DELEGATES*/\n",
				"\n\t\t/*END DELEGATES*/",
				builders.CsharpDelegates.ToString());
			csharpContents = InjectIntoString(
				csharpContents,
				"/*BEGIN IMPORTS*/\n",
				"\n\t\t/*END IMPORTS*/",
				builders.CsharpImports.ToString());
			csharpContents = InjectIntoString(
				csharpContents,
				"/*BEGIN GETDELEGATE CALLS*/\n",
				"\n\t\t\t/*END GETDELEGATE CALLS*/",
				builders.CsharpGetDelegateCalls.ToString());
			csharpContents = InjectIntoString(
				csharpContents,
				"/*BEGIN DESTROY FUNCTION ENUMERATORS*/\n",
				"\n\t\t\t/*END DESTROY FUNCTION ENUMERATORS*/",
				builders.CsharpDestroyFunctionEnumerators.ToString());
			csharpContents = InjectIntoString(
				csharpContents,
				"/*BEGIN DESTROY QUEUE CASES*/\n",
				"\n\t\t\t\t\t\t/*END DESTROY QUEUE CASES*/",
				builders.CsharpDestroyQueueCases.ToString());
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
				"/*BEGIN TEMPLATE DECLARATIONS*/\n",
				"\n/*END TEMPLATE DECLARATIONS*/",
				builders.CppTemplateDeclarations.ToString());
			cppHeaderContents = InjectIntoString(
				cppHeaderContents,
				"/*BEGIN TEMPLATE SPECIALIZATION DECLARATIONS*/\n",
				"\n/*END TEMPLATE SPECIALIZATION DECLARATIONS*/",
				builders.CppTemplateSpecializationDeclarations.ToString());
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
				"/*BEGIN INIT BODY FIRST BOOT*/\n",
				"\n\t\t/*END INIT BODY FIRST BOOT*/",
				builders.CppInitBodyFirstBoot.ToString());
			cppSourceContents = InjectIntoString(
				cppSourceContents,
				"/*BEGIN GLOBAL STATE AND FUNCTIONS*/\n",
				"\n\t/*END GLOBAL STATE AND FUNCTIONS*/",
				builders.CppGlobalStateAndFunctions.ToString());
			cppHeaderContents = InjectIntoString(
				cppHeaderContents,
				"/*BEGIN UNBOXING METHOD DECLARATIONS*/\n",
				"\n\t\t/*END UNBOXING METHOD DECLARATIONS*/",
				builders.CppUnboxingMethodDeclarations.ToString());
			cppHeaderContents = InjectIntoString(
				cppHeaderContents,
				"/*BEGIN STRING DEFAULT PARAMETERS*/\n",
				"\n\t/*END STRING DEFAULT PARAMETERS*/",
				builders.CppStringDefaultParams.ToString());
			cppHeaderContents = InjectIntoString(
				cppHeaderContents,
				"/*BEGIN MACROS*/\n",
				"\n/*END MACROS*/",
				builders.CppMacros.ToString());
			
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
			for (int startIndex = 0; ; )
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
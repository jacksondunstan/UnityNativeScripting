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
		private class JsonMethod
		{
			public string Name;
			public string[] Types;
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
		private class JsonDocument
		{
			public JsonAssembly[] Assemblies;
		}
		
		private class StringBuilders
		{
			public StringBuilder CsharpInitParams = new StringBuilder();
			public StringBuilder CsharpDelegateTypes = new StringBuilder();
			public StringBuilder CsharpInitCall = new StringBuilder();
			public StringBuilder CsharpFunctions = new StringBuilder();
			public StringBuilder CppFunctionPointers = new StringBuilder();
			public StringBuilder CppTypeDeclarations = new StringBuilder();
			public StringBuilder CppTypeDefinitions = new StringBuilder();
			public StringBuilder CppMethodDefinitions = new StringBuilder();
			public StringBuilder CppInitParams = new StringBuilder();
			public StringBuilder CppInitBody = new StringBuilder();
			public StringBuilder TempStrBuilder = new StringBuilder();
		}
		
		private class ParameterInfo
		{
			public string Name;
			public Type ParameterType;
		}
		
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
			// Clear console
	#if UNITY_2017
			Assembly
				.GetAssembly(typeof(SceneView))
				.GetType("UnityEditor.LogEntries")
				.GetMethod("Clear")
				.Invoke(new object(), null);
	#endif
			
			// Load JSON
			string jsonPath = Path.Combine(
				Application.dataPath,
				BindingConstants.BindingsJsonPath);
			string json = File.ReadAllText(jsonPath);
			JsonDocument doc = JsonUtility.FromJson<JsonDocument>(json);
			
			// Build binding strings
			StringBuilders builders = new StringBuilders();
			StringBuilder csharpInitParams = builders.CsharpInitParams;
			StringBuilder csharpDelegateTypes = builders.CsharpDelegateTypes;
			StringBuilder csharpInitCall = builders.CsharpInitCall;
			StringBuilder csharpFunctions = builders.CsharpFunctions;
			StringBuilder cppFunctionPointers = builders.CppFunctionPointers;
			StringBuilder cppTypeDeclarations = builders.CppTypeDeclarations;
			StringBuilder cppTypeDefinitions = builders.CppTypeDefinitions;
			StringBuilder cppMethodDefinitions = builders.CppMethodDefinitions;
			StringBuilder cppInitParams = builders.CppInitParams;
			StringBuilder cppInitBody = builders.CppInitBody;
			StringBuilder tempStrBuilder = builders.TempStrBuilder;
			foreach (JsonAssembly jsonAssembly in doc.Assemblies)
			{
				Assembly assembly = Assembly.LoadFrom(jsonAssembly.Path);
				foreach (JsonType jsonType in jsonAssembly.Types)
				{
					Type type = assembly.GetType(jsonType.Name);
					string typeNameLower = char.ToLower(type.Name[0])
						+ type.Name.Substring(1);
					bool hasMethodDefinitions = (jsonType.Constructors.Length
						+ jsonType.Properties.Length
						+ jsonType.Fields.Length
						+ jsonType.Methods.Length) > 0;
					bool isStatic = type.IsAbstract && type.IsSealed;
					
					// C++ type declaration
					int indent = AppendNamespaceBeginning(
						type.Namespace,
						cppTypeDeclarations);
					AppendIndent(indent, cppTypeDeclarations);
					if (isStatic)
					{
						cppTypeDeclarations.Append("namespace ");
						cppTypeDeclarations.Append(type.Name);
						cppTypeDeclarations.Append('\n');
						AppendIndent(indent, cppTypeDeclarations);
						cppTypeDeclarations.Append("{\n");
						AppendIndent(indent, cppTypeDeclarations);
						cppTypeDeclarations.Append('}');
					}
					else
					{
						cppTypeDeclarations.Append("struct ");
						cppTypeDeclarations.Append(type.Name);
						cppTypeDeclarations.Append(";");
					}
					cppTypeDeclarations.Append('\n');
					AppendNamespaceEnding(
						indent,
						cppTypeDeclarations);
					cppTypeDeclarations.Append('\n');
					
					// C++ type definition (beginning)
					AppendNamespaceBeginning(
						type.Namespace,
						cppTypeDefinitions);
					AppendIndent(
						indent,
						cppTypeDefinitions);
					if (isStatic)
					{
						cppTypeDefinitions.Append("namespace ");
						cppTypeDefinitions.Append(type.Name);
					}
					else
					{
						cppTypeDefinitions.Append("struct ");
						cppTypeDefinitions.Append(type.Name);
						cppTypeDefinitions.Append(" : ");
						cppTypeDefinitions.Append(type.BaseType.Namespace);
						cppTypeDefinitions.Append("::");
						cppTypeDefinitions.Append(type.BaseType.Name);
					}
					cppTypeDefinitions.Append('\n');
					AppendIndent(
						indent,
						cppTypeDefinitions);
					cppTypeDefinitions.Append("{\n");
					if (!isStatic)
					{
						AppendIndent(
							indent + 1,
							cppTypeDefinitions);
						AppendSystemObjectLifecycleCall(
							"SYSTEM_OBJECT_LIFECYCLE_DECLARATION",
							type,
							cppTypeDefinitions);
						cppTypeDefinitions.Append('\n');
					}
					
					// C++ method definition
					int cppMethodDefinitionsIndent;
					if (hasMethodDefinitions)
					{
						cppMethodDefinitionsIndent = AppendNamespaceBeginning(
							type.Namespace,
							cppMethodDefinitions);
						if (!isStatic)
						{
							AppendIndent(indent, cppMethodDefinitions);
							AppendSystemObjectLifecycleCall(
								"SYSTEM_OBJECT_LIFECYCLE_DEFINITION",
								type,
								cppMethodDefinitions);
							cppMethodDefinitions.Append('\n');
							AppendIndent(indent, cppMethodDefinitions);
							cppMethodDefinitions.Append('\n');
						}
					}
					else
					{
						cppMethodDefinitionsIndent = 0;
					}
					
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
							parameters,
							csharpFunctions);
						csharpFunctions.Append("ObjectStore.Store(");
						csharpFunctions.Append("new ");
						csharpFunctions.Append(type.Name);
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
							parameters,
							cppTypeDefinitions);
						
						// C++ method definition
						AppendCppMethodDefinition(
							type,
							null,
							type.Name,
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
						cppMethodDefinitions.Append(")\n\t{\n\t}\n\t\n");

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
							GenerateGetter(
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
							GenerateSetter(
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
						GenerateGetter(
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
						GenerateSetter(
							field.Name,
							"Field",
							typeNameLower,
							new []{ setParam },
							field.IsStatic,
							type,
							field.FieldType,
							indent,
							builders);
					}
					
					// Methods
					foreach (JsonMethod jsonMethod in jsonType.Methods)
					{
						Type[] paramTypes = GetTypes(
							jsonMethod.Types,
							assembly);
						MethodInfo method = type.GetMethod(
							jsonMethod.Name,
							paramTypes);
						ParameterInfo[] parameters = ConvertParameters(
							method.GetParameters());
						
						// Build uppercase function name
						tempStrBuilder.Length = 0;
						tempStrBuilder.Append(type.Name);
						tempStrBuilder.Append("Method");
						tempStrBuilder.Append(method.Name);
						AppendTypeNames(paramTypes, tempStrBuilder);
						string funcName = tempStrBuilder.ToString();
						
						// Build lowercase function name
						tempStrBuilder.Length = 0;
						tempStrBuilder.Append(typeNameLower);
						tempStrBuilder.Append("Method");
						tempStrBuilder.Append(method.Name);
						AppendTypeNames(paramTypes, tempStrBuilder);
						string funcNameLower = tempStrBuilder.ToString();
						
						// C# init param declaration
						AppendCsharpInitParam(funcNameLower, csharpInitParams);

						// C# delegate type
						AppendCsharpDelegateType(
							funcName,
							method.IsStatic,
							method.ReturnType,
							parameters,
							csharpDelegateTypes);

						// C# init call param
						AppendCsharpInitCallArg(funcName, csharpInitCall);

						// C# function
						AppendCsharpFunctionBeginning(
							type,
							funcName,
							method.IsStatic,
							method.ReturnType,
							parameters,
							csharpFunctions);
						AppendCsharpFunctionCallSubject(
							type,
							method.IsStatic,
							csharpFunctions);
						csharpFunctions.Append(method.Name);
						AppendCsharpFunctionCallParameters(
							method.IsStatic,
							parameters,
							csharpFunctions);
						csharpFunctions.Append(';');
						AppendCsharpFunctionReturn(
							method.ReturnType,
							csharpFunctions);
						
						// C++ function pointer
						AppendCppFunctionPointerDefinition(
							funcName,
							method.IsStatic,
							parameters,
							method.ReturnType,
							cppFunctionPointers);
						
						// C++ method declaration
						AppendIndent(
							indent + 1,
							cppTypeDefinitions);
						AppendCppMethodDeclaration(
							method.Name,
							method.IsStatic,
							method.ReturnType,
							parameters,
							cppTypeDefinitions);
						
						// C++ method definition
						AppendCppMethodDefinition(
							type,
							method.ReturnType,
							method.Name,
							parameters,
							indent,
							cppMethodDefinitions);
						AppendIndent(indent, cppMethodDefinitions);
						cppMethodDefinitions.Append("{\n");
						AppendIndent(indent + 1, cppMethodDefinitions);
						AppendCppMethodReturn(
							method.ReturnType,
							cppMethodDefinitions);
						AppendCppPluginFunctionCall(
							method.IsStatic,
							method.ReturnType,
							funcName,
							parameters,
							cppMethodDefinitions);
						cppMethodDefinitions.Append(";\n");
						AppendIndent(indent, cppMethodDefinitions);
						cppMethodDefinitions.Append("}\n\t\n");
						
						// C++ init params
						AppendCppInitParam(
							funcNameLower,
							method.IsStatic,
							parameters,
							method.ReturnType,
							cppInitParams);
						
						// C++ init body
						AppendCppInitBody(funcName, funcNameLower, cppInitBody);
					}
					
					// C++ type definition (ending)
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
					
					// C++ method definition (ending)
					if (hasMethodDefinitions)
					{
						RemoveTrailingChars(cppMethodDefinitions);
						cppMethodDefinitions.Append('\n');
						AppendNamespaceEnding(
							cppMethodDefinitionsIndent,
							cppMethodDefinitions);
						cppMethodDefinitions.Append('\n');
					}
				}
			}
			
			// Remove trailing chars (e.g. commas) for last elements
			RemoveTrailingChars(csharpInitParams);
			RemoveTrailingChars(csharpDelegateTypes);
			RemoveTrailingChars(csharpInitCall);
			RemoveTrailingChars(csharpFunctions);
			RemoveTrailingChars(cppFunctionPointers);
			RemoveTrailingChars(cppTypeDeclarations);
			RemoveTrailingChars(cppMethodDefinitions);
			RemoveTrailingChars(cppTypeDefinitions);
			RemoveTrailingChars(cppInitParams);
			RemoveTrailingChars(cppInitBody);
			
			if (dryRun)
			{
				LogStringBuilder("C# init params", csharpInitParams);
				LogStringBuilder("C# delegates", csharpDelegateTypes);
				LogStringBuilder("C# init call", csharpInitCall);
				LogStringBuilder("C# functions", csharpFunctions);
				LogStringBuilder("C++ function pointers", cppFunctionPointers);
				LogStringBuilder("C++ type declarations", cppTypeDeclarations);
				LogStringBuilder("C++ type definitions", cppTypeDefinitions);
				LogStringBuilder("C++ method definitions", cppMethodDefinitions);
				LogStringBuilder("C++ init params", cppInitParams);
				LogStringBuilder("C++ init body", cppInitBody);
			}
			else
			{
				// Inject into source files
				string csharpPath = Path.Combine(
					Application.dataPath,
					Path.Combine(
						"NativeScript",
						"Bindings.cs"));
				string cppHeaderPath = Path.Combine(
					Application.dataPath,
					Path.Combine(
						"NativeScript",
						"Bindings.h"));
				string cppSourcePath = Path.Combine(
					Application.dataPath,
					Path.Combine(
						"NativeScript",
						"Bindings.cpp"));
				string csharpContents = File.ReadAllText(csharpPath);
				string cppHeaderContents = File.ReadAllText(cppHeaderPath);
				string cppSourceContents = File.ReadAllText(cppSourcePath);
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
				File.WriteAllText(csharpPath, csharpContents);
				File.WriteAllText(cppHeaderPath, cppHeaderContents);
				File.WriteAllText(cppSourcePath, cppSourceContents);
				AssetDatabase.Refresh();
				Debug.Log("Done");
			}
		}
		
		static Type[] GetTypes(
			string[] typeNames,
			Assembly assembly)
		{
			Assembly systemAssembly = typeof(string).Assembly;
			Type[] types = new Type[typeNames.Length];
			for (int i = 0; i < typeNames.Length; ++i)
			{
				string typeName = typeNames[i];
				types[i] = assembly.GetType(typeName)
					?? systemAssembly.GetType(typeName);
			}
			return types;
		}
		
		static void AppendTypeNames(
			Type[] types,
			StringBuilder output)
		{
			for (int i = 0; i < types.Length; ++i)
			{
				Type type = types[i];
				output.Append(type.Namespace);
				output.Append(type.Name);
				if (i != types.Length - 1)
				{
					output.Append('_');
				}
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
		
		static void GenerateGetter(
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
				parameters,
				stringBuilders.CppTypeDefinitions);
			
			// C++ method definition
			AppendCppMethodDefinition(
				enclosingType,
				fieldType,
				methodName,
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
		
		static void GenerateSetter(
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
				parameters,
				stringBuilders.CppTypeDefinitions);
			
			// C++ method definition
			AppendCppMethodDefinition(
				enclosingType,
				typeof(void),
				methodName,
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
		
		static void AppendSystemObjectLifecycleCall(
			string macroName,
			Type enclosingType,
			StringBuilder output)
		{
			output.Append(macroName);
			output.Append('(');
			output.Append(enclosingType.Name);
			output.Append(", ");
			output.Append(enclosingType.BaseType.Namespace);
			output.Append("::");
			output.Append(enclosingType.BaseType.Name);
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
				AppendCsharpTypeName(returnType, output);
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
			output.Append(");\n\t\t\n");
		}
		
		static void AppendCsharpFunctionBeginning(
			Type enclosingType,
			string funcName,
			bool isStatic,
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
				if (returnType.IsValueType)
				{
					AppendCsharpTypeName(returnType, output);
				}
				else
				{
					output.Append("int");
				}
				output.Append(' ');
			}
			
			output.Append(funcName);
			
			// Paramters
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
				AppendCsharpTypeName(enclosingType, output);
				output.Append(" thiz = (");
				AppendCsharpTypeName(enclosingType, output);
				output.Append(
					")ObjectStore.Get(thisHandle);\n\t\t\t");
			}
			
			// Save return value as local variable
			if (!returnType.Equals(typeof(void)))
			{
				AppendCsharpTypeName(returnType, output);
				output.Append(" obj = ");
			};
		}
		
		static void AppendCsharpFunctionCallSubject(
			Type enclosingType,
			bool isStatic,
			StringBuilder output)
		{
			if (isStatic)
			{
				AppendCsharpTypeName(enclosingType, output);
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
			if (!isStatic)
			{
				output.Append("thisHandle");
				if (parameters.Length > 0)
				{
					output.Append(", ");
				}
			}
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
			ParameterInfo[] parameters,
			int indent,
			StringBuilder output)
		{
			AppendIndent(indent, output);
			if (returnType != null)
			{
				AppendCppTypeName(returnType, output);
				output.Append(' ');
			}
			output.Append(enclosingType.Name);
			output.Append("::");
			output.Append(methodName);
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
			output.Append("\n\t\n");
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
			ParameterInfo[] parameters,
			StringBuilder output)
		{
			if (isStatic)
			{
				output.Append("static ");
			}
			
			// Return type
			if (returnType != null)
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
				output.Append(type.Namespace);
				output.Append("::");
				output.Append(type.Name);
			}
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
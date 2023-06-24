// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class MantisX : ModuleRules
{
	public MantisX(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				Path.Combine(ModuleDirectory, "Public")
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
                Path.Combine(ModuleDirectory, "Private")
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Projects"
			}
			);

        if (Target.Platform == UnrealTargetPlatform.Android)
        {
			PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "Private/Android"));
			PrivateDependencyModuleNames.Add("Launch");
            var pluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
			AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(pluginPath, "MantisXAndroid_UPL.xml"));
        }

		// TODO: BT support for Windows..
        if (Target.Platform == UnrealTargetPlatform.Win64)
        {

        }
	}
}

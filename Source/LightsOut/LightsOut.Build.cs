// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class LightsOut : ModuleRules
{
	public LightsOut(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "ProceduralMeshComponent" });

        PublicIncludePaths.AddRange(
			new string[] {
				"./LightsOut/Generics",
				"./LightsOut/Items",
				"./LightsOut/Core/Calamities",
				"./LightsOut/Utility"
            }
			);

	}
}

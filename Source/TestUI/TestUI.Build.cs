// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TestUI : ModuleRules
{
	public TestUI(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "Slate", "SlateCore", "UMG", "CustomUIKit" });
	}
}

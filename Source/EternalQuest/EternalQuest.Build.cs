// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class EternalQuest : ModuleRules
{
	public EternalQuest(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(["EternalQuest"]);
	
		PublicDependencyModuleNames.AddRange(["Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG", "Niagara", "NavigationSystem", "AIModule"]);

		PrivateDependencyModuleNames.AddRange(["GameFeatures", "GameplayAbilities", "GameplayTasks", "GameplayTags"]);

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(["Slate", "SlateCore"]);

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}

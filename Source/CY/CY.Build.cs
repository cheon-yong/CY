// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CY : ModuleRules
{
	public CY(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(new string[] {
			"CY"
		});

		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject",
			"NetCore",
			"Engine", 
			"InputCore", 
			"EnhancedInput",
			"Niagara",
			"NavigationSystem",
			"AIModule",
			"ModularGameplay",
			"ModularGameplayActors",
			"GameFeatures",
			"ControlFlows",

			// GAS
			"GameplayAbilities",
			"GameplayTasks",
			"GameplayTags",

			// UI
			"UMG",
			"Slate",
			"SlateCore",

			// Online Subsystem
			"OnlineSubsystem",
			"OnlineSubsystemSteam",
			"OnlineSubsystemNull",
			"OnlineSubsystemUtils",

			// MessageSystem
			"GameplayMessageRuntime",

			// Setting
			"DeveloperSettings",

			// Common
			"CommonGame",
			"CommonUser",
			"CommonUI",
			"CommonInput",
			"CommonLoadingScreen",

			"AsyncMixin"
		});
	}
}

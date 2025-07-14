// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TealesOfMeadow : ModuleRules
{
	public TealesOfMeadow(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput",
			"HeadMountedDisplay", "AIModule", "GameplayCameras", "NavigationSystem",
            "PhysicsCore", "Niagara", "MotionWarping"
        });

        PrivateDependencyModuleNames.AddRange(new string[] {
            "Niagara"
        });

        PublicIncludePaths.AddRange(new string[] 
		{
            "TealesOfMeadow/Public/Player",
            "TealesOfMeadow/Public/Components",
            "TealesOfMeadow/Public/Components/Stamina",
            "TealesOfMeadow/Public/Components/Health",
            "TealesOfMeadow/Public/Components/DetectEnemy",
            "TealesOfMeadow/Public/Animation",
            "TealesOfMeadow/Public/Animation/Notifys",
            "TealesOfMeadow/Public/Weapons",
            "TealesOfMeadow/Public/Weapons/Components",
            "TealesOfMeadow/Public/UI",
            "TealesOfMeadow/Public/Enemy",
            "TealesOfMeadow/Public/Enemy/Tasks",
            "TealesOfMeadow/Public/Enemy/Components",
            "TealesOfMeadow/Public/Enemy/Services",
            "TealesOfMeadow/Public/Enemy/Decorators",
            "TealesOfMeadow/Public/Interfaces",
            "TealesOfMeadow/Public/GamePlay"
        });
	}
}

using UnrealBuildTool;

public class BEVYNetwork : ModuleRules
{
    public BEVYNetwork(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        
        PublicIncludePaths.AddRange(
            new string[] {
            }
        );
        
        PrivateIncludePaths.AddRange(
            new string[] {
            }
        );
        
        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "InputCore",
                "Json",
                "JsonUtilities",
                "WebRTC",
                "WebRTCAdapter"
            }
        );
        
        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Slate",
                "SlateCore",
                "SSL",
                "Networking",
                "Sockets",
                "WebRTCLibrary",
                "DeveloperSettings"
            }
        );
        
        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
            }
        );
    }
}
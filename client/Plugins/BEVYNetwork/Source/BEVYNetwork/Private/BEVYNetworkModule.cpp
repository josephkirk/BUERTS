#include "BEVYNetworkModule.h"
#include "Modules/ModuleManager.h"
#include "WebRTCIncludes.h"
#include "BEVYNetworkSettings.h"

#define LOCTEXT_NAMESPACE "FBEVYNetworkModule"

void FBEVYNetworkModule::StartupModule()
{
    // Initialize WebRTC
    if (!FModuleManager::Get().IsModuleLoaded("WebRTC"))
    {
        FModuleManager::Get().LoadModule("WebRTC");
    }

    // Load settings
    if (const UBEVYNetworkSettings* Settings = GetDefault<UBEVYNetworkSettings>())
    {
        // Initialize any module-level resources based on settings
    }

    UE_LOG(LogTemp, Log, TEXT("BEVY Network Module started"));
}

void FBEVYNetworkModule::ShutdownModule()
{
    UE_LOG(LogTemp, Log, TEXT("BEVY Network Module shutdown"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FBEVYNetworkModule, BEVYNetwork)
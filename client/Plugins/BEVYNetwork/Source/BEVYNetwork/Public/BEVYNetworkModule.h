#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FBEVYNetworkModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

    static inline FBEVYNetworkModule& Get()
    {
        return FModuleManager::LoadModuleChecked<FBEVYNetworkModule>("BEVYNetwork");
    }

    static inline bool IsAvailable()
    {
        return FModuleManager::Get().IsModuleLoaded("BEVYNetwork");
    }
};
#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "BEVYNetworkSettings.generated.h"

UCLASS(config = Game, defaultconfig, meta = (DisplayName = "BEVY Network Settings"))
class BEVYNETWORK_API UBEVYNetworkSettings : public UDeveloperSettings
{
    GENERATED_BODY()

public:
    UBEVYNetworkSettings();

    UPROPERTY(Config, EditAnywhere, Category = "WebRTC", meta = (DisplayName = "Signaling Server URL"))
    FString SignalingServerUrl;

    UPROPERTY(Config, EditAnywhere, Category = "WebRTC", meta = (DisplayName = "STUN Servers"))
    TArray<FString> StunServers;

    UPROPERTY(Config, EditAnywhere, Category = "WebRTC", meta = (DisplayName = "TURN Servers"))
    TArray<FString> TurnServers;

    UPROPERTY(Config, EditAnywhere, Category = "WebRTC", meta = (DisplayName = "TURN Username"))
    FString TurnUsername;

    UPROPERTY(Config, EditAnywhere, Category = "WebRTC", meta = (DisplayName = "TURN Credential"))
    FString TurnCredential;

    UPROPERTY(Config, EditAnywhere, Category = "Network", meta = (DisplayName = "Maximum Bandwidth (KB/s)"))
    int32 MaxBandwidthKBs;

    UPROPERTY(Config, EditAnywhere, Category = "Network", meta = (DisplayName = "Enable Unreliable Channel"))
    bool bEnableUnreliableChannel;

    virtual FName GetCategoryName() const override { return FName(TEXT("Game")); }
};
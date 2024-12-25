#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "BEVYWebRTCConnection.h"
#include "BEVYNetworkSubsystem.generated.h"

UCLASS()
class BEVYNETWORK_API UBEVYNetworkSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    // Begin USubsystem
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;
    // End USubsystem

    UFUNCTION(BlueprintCallable, Category = "BEVY|Network")
    UBEVYWebRTCConnection* GetConnection() const { return Connection; }

    UFUNCTION(BlueprintCallable, Category = "BEVY|Network")
    void ConnectToServer();

    UFUNCTION(BlueprintCallable, Category = "BEVY|Network")
    void DisconnectFromServer();

    UFUNCTION(BlueprintCallable, Category = "BEVY|Network")
    bool IsConnected() const;

    UFUNCTION(BlueprintCallable, Category = "BEVY|Network")
    void SendGameMessage(const FBEVYNetworkMessage& Message, bool bReliable = true);

private:
    UPROPERTY()
    UBEVYWebRTCConnection* Connection;

    void OnConnectionStateChanged(const FString& NewState);
    void OnMessageReceived(const FBEVYNetworkMessage& Message);
};
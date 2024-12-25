#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WebRTCProvider.h"
#include "WebRTCPeerConnection.h"
#include "BEVYNetworkMessage.h"
#include "HAL/Runnable.h"
#include "HAL/ThreadSafeBool.h"
#include "BEVYWebRTCConnection.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWebRTCConnected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWebRTCMessageReceived, const FBEVYNetworkMessage&, Message);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWebRTCError, const FString&, Error);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWebRTCStateChanged, const FString&, NewState);

USTRUCT(BlueprintType)
struct FWebRTCConnectionStats
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "BEVY|Network")
    float RoundTripTime;

    UPROPERTY(BlueprintReadOnly, Category = "BEVY|Network")
    float PacketLoss;

    UPROPERTY(BlueprintReadOnly, Category = "BEVY|Network")
    float Bandwidth;

    FWebRTCConnectionStats()
        : RoundTripTime(0.0f)
        , PacketLoss(0.0f)
        , Bandwidth(0.0f)
    {}
};

UCLASS(BlueprintType, Blueprintable)
class BEVYNETWORK_API UBEVYWebRTCConnection : public UObject, public FRunnable
{
    GENERATED_BODY()

public:
    UBEVYWebRTCConnection();
    virtual ~UBEVYWebRTCConnection();

    // Blueprint interface
    UFUNCTION(BlueprintCallable, Category = "BEVY|Network")
    void Connect(const FString& SignalingServerUrl);

    UFUNCTION(BlueprintCallable, Category = "BEVY|Network")
    void Disconnect();

    UFUNCTION(BlueprintCallable, Category = "BEVY|Network")
    void SendMessage(const FBEVYNetworkMessage& Message, bool bReliable = true);

    UFUNCTION(BlueprintCallable, Category = "BEVY|Network")
    bool IsConnected() const;

    UFUNCTION(BlueprintCallable, Category = "BEVY|Network")
    FWebRTCConnectionStats GetStats() const;

    // Event delegates
    UPROPERTY(BlueprintAssignable, Category = "BEVY|Network")
    FOnWebRTCConnected OnConnected;

    UPROPERTY(BlueprintAssignable, Category = "BEVY|Network")
    FOnWebRTCMessageReceived OnMessageReceived;

    UPROPERTY(BlueprintAssignable, Category = "BEVY|Network")
    FOnWebRTCError OnError;

    UPROPERTY(BlueprintAssignable, Category = "BEVY|Network")
    FOnWebRTCStateChanged OnStateChanged;

protected:
    // FRunnable interface
    virtual bool Init() override;
    virtual uint32 Run() override;
    virtual void Stop() override;
    virtual void Exit() override;

private:
    TSharedPtr<IWebRTCProvider> WebRTCProvider;
    TSharedPtr<IWebRTCPeerConnection> PeerConnection;
    TSharedPtr<IWebRTCDataChannel> ReliableChannel;
    TSharedPtr<IWebRTCDataChannel> UnreliableChannel;

    FThreadSafeBool bStopThread;
    FRunnableThread* NetworkThread;
    FCriticalSection StateLock;

    TQueue<FBEVYNetworkMessage, EQueueMode::Mpsc> MessageQueue;
    FWebRTCConnectionStats ConnectionStats;

    void SetupPeerConnection();
    void SetupDataChannels();
    void HandleSignalingMessage(const FString& Message);
    void ProcessMessageQueue();
    void UpdateConnectionStats();

    void OnConnectionStateChanged(EWebRTCConnectionState NewState);
    void OnDataChannelMessage(const TArray<uint8>& Data, bool bReliable);
    void OnICECandidate(const FString& SdpMid, int32 SdpMLineIndex, const FString& Sdp);
    
    void BroadcastError(const FString& Error);
    void BroadcastStateChange(const FString& NewState);
};
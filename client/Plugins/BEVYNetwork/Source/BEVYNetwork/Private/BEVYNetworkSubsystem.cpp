#include "BEVYNetworkSubsystem.h"
#include "BEVYNetworkSettings.h"

void UBEVYNetworkSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    Connection = NewObject<UBEVYWebRTCConnection>(this);
    if (Connection)
    {
        Connection->OnStateChanged.AddDynamic(this, &UBEVYNetworkSubsystem::OnConnectionStateChanged);
        Connection->OnMessageReceived.AddDynamic(this, &UBEVYNetworkSubsystem::OnMessageReceived);
    }
}

void UBEVYNetworkSubsystem::Deinitialize()
{
    if (Connection)
    {
        DisconnectFromServer();
        Connection->OnStateChanged.RemoveDynamic(this, &UBEVYNetworkSubsystem::OnConnectionStateChanged);
        Connection->OnMessageReceived.RemoveDynamic(this, &UBEVYNetworkSubsystem::OnMessageReceived);
        Connection = nullptr;
    }

    Super::Deinitialize();
}

void UBEVYNetworkSubsystem::ConnectToServer()
{
    if (Connection && !IsConnected())
    {
        const UBEVYNetworkSettings* Settings = GetDefault<UBEVYNetworkSettings>();
        if (Settings && !Settings->SignalingServerUrl.IsEmpty())
        {
            Connection->Connect(Settings->SignalingServerUrl);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("BEVYNetwork: Invalid signaling server configuration"));
        }
    }
}

void UBEVYNetworkSubsystem::DisconnectFromServer()
{
    if (Connection)
    {
        Connection->Disconnect();
    }
}

bool UBEVYNetworkSubsystem::IsConnected() const
{
    return Connection && Connection->IsConnected();
}

void UBEVYNetworkSubsystem::SendGameMessage(const FBEVYNetworkMessage& Message, bool bReliable)
{
    if (Connection && IsConnected())
    {
        Connection->SendMessage(Message, bReliable);
    }
}

void UBEVYNetworkSubsystem::OnConnectionStateChanged(const FString& NewState)
{
    UE_LOG(LogTemp, Log, TEXT("BEVYNetwork: Connection state changed to %s"), *NewState);
}

void UBEVYNetworkSubsystem::OnMessageReceived(const FBEVYNetworkMessage& Message)
{
    // Handle different message types
    switch (Message.Type)
    {
        case EBEVYMessageType::StateUpdate:
            // Process state update
            break;
            
        case EBEVYMessageType::Command:
            // Process command
            break;
            
        case EBEVYMessageType::Error:
            UE_LOG(LogTemp, Warning, TEXT("BEVYNetwork: Received error message: %s"), *Message.Payload);
            break;
            
        default:
            UE_LOG(LogTemp, Warning, TEXT("BEVYNetwork: Unknown message type received"));
            break;
    }
}
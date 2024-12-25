#include "BEVYWebRTCConnection.h"
#include "WebRTCIncludes.h"
#include "Json.h"
#include "JsonUtilities.h"

UBEVYWebRTCConnection::UBEVYWebRTCConnection()
{
    WebRTCProvider = IWebRTCProvider::Get();
}

void UBEVYWebRTCConnection::Connect(const FString& SignalingServerUrl)
{
    SetupPeerConnection();
}

void UBEVYWebRTCConnection::Disconnect()
{
    if (DataChannel.IsValid())
    {
        DataChannel->Close();
        DataChannel.Reset();
    }

    if (PeerConnection.IsValid())
    {
        PeerConnection->Close();
        PeerConnection.Reset();
    }
}

void UBEVYWebRTCConnection::SendMessage(const FString& Message)
{
    if (!DataChannel.IsValid() || !DataChannel->IsOpen())
    {
        OnError.Broadcast(TEXT("Data channel not ready"));
        return;
    }

    TArray<uint8> Data;
    FTCHARToUTF8 Converter(*Message);
    Data.Append((uint8*)Converter.Get(), Converter.Length());
    DataChannel->Send(Data);
}

void UBEVYWebRTCConnection::SetupPeerConnection()
{
    FWebRTCConfiguration Config;
    Config.IceServers.Add(FWebRTCIceServer(TEXT("stun:stun.l.google.com:19302")));
    
    PeerConnection = WebRTCProvider->CreatePeerConnection(Config);
    
    PeerConnection->OnConnectionStateChanged().AddLambda([this](EWebRTCConnectionState NewState) {
        OnConnectionStateChanged(NewState);
    });

    PeerConnection->OnIceCandidate().AddLambda([this](const FString& SdpMid, int32 SdpMLineIndex, const FString& Sdp) {
        OnICECandidate(SdpMid, SdpMLineIndex, Sdp);
    });

    SetupDataChannel();
}

void UBEVYWebRTCConnection::SetupDataChannel()
{
    FWebRTCDataChannelConfiguration DataChannelConfig;
    DataChannelConfig.Label = TEXT("BEVYGameData");
    DataChannelConfig.Ordered = true;
    
    DataChannel = PeerConnection->CreateDataChannel(DataChannelConfig);
    
    DataChannel->OnMessage().AddLambda([this](const TArray<uint8>& Data) {
        OnDataChannelMessage(Data);
    });
}

void UBEVYWebRTCConnection::HandleSignalingMessage(const FString& Message)
{
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Message);

    if (!FJsonSerializer::Deserialize(Reader, JsonObject))
    {
        OnError.Broadcast(TEXT("Invalid signaling message format"));
        return;
    }

    // Process SDP offer/answer and ICE candidates
    FString Type = JsonObject->GetStringField(TEXT("type"));
    if (Type == TEXT("offer"))
    {
        FString Sdp = JsonObject->GetStringField(TEXT("sdp"));
        PeerConnection->SetRemoteDescription(FWebRTCSessionDescription(EWebRTCSdpType::Offer, Sdp));
        // Create and send answer
    }
    else if (Type == TEXT("candidate"))
    {
        FString SdpMid = JsonObject->GetStringField(TEXT("sdpMid"));
        int32 SdpMLineIndex = JsonObject->GetIntegerField(TEXT("sdpMLineIndex"));
        FString Candidate = JsonObject->GetStringField(TEXT("candidate"));
        PeerConnection->AddIceCandidate(FWebRTCIceCandidate(SdpMid, SdpMLineIndex, Candidate));
    }
}

void UBEVYWebRTCConnection::OnConnectionStateChanged(EWebRTCConnectionState NewState)
{
    if (NewState == EWebRTCConnectionState::Connected)
    {
        OnConnected.Broadcast();
    }
}

void UBEVYWebRTCConnection::OnDataChannelMessage(const TArray<uint8>& Data)
{
    FString Message;
    FUTF8ToTCHAR Converter((const ANSICHAR*)Data.GetData(), Data.Num());
    Message = FString(Converter.Length(), Converter.Get());
    OnMessageReceived.Broadcast(Message);
}

void UBEVYWebRTCConnection::OnICECandidate(const FString& SdpMid, int32 SdpMLineIndex, const FString& Sdp)
{
    // Send ICE candidate to signaling server
    TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
    JsonObject->SetStringField(TEXT("type"), TEXT("candidate"));
    JsonObject->SetStringField(TEXT("sdpMid"), SdpMid);
    JsonObject->SetNumberField(TEXT("sdpMLineIndex"), SdpMLineIndex);
    JsonObject->SetStringField(TEXT("candidate"), Sdp);

    FString JsonString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

    // Send to signaling server
}
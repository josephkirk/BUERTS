#pragma once

#include "CoreMinimal.h"
#include "BEVYNetworkMessage.generated.h"

UENUM(BlueprintType)
enum class EBEVYMessageType : uint8
{
    Command,
    StateUpdate,
    Connect,
    Disconnect,
    Ping,
    Error
};

USTRUCT(BlueprintType)
struct BEVYNETWORK_API FBEVYNetworkMessage
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BEVY|Network")
    EBEVYMessageType Type;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BEVY|Network")
    FString Payload;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BEVY|Network")
    int32 SequenceNumber;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BEVY|Network")
    double Timestamp;

    FBEVYNetworkMessage()
        : Type(EBEVYMessageType::Command)
        , SequenceNumber(0)
        , Timestamp(0.0)
    {}

    bool Serialize(TArray<uint8>& OutData) const;
    bool Deserialize(const TArray<uint8>& InData);
};
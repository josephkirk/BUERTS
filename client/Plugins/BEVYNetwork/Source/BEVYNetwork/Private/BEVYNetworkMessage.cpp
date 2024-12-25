#include "BEVYNetworkMessage.h"
#include "JsonObjectConverter.h"

bool FBEVYNetworkMessage::Serialize(TArray<uint8>& OutData) const
{
    FString JsonString;
    if (!FJsonObjectConverter::UStructToJsonObjectString(*this, JsonString))
    {
        return false;
    }

    FTCHARToUTF8 Converter(*JsonString);
    OutData.SetNum(Converter.Length());
    FMemory::Memcpy(OutData.GetData(), Converter.Get(), Converter.Length());
    
    return true;
}

bool FBEVYNetworkMessage::Deserialize(const TArray<uint8>& InData)
{
    FUTF8ToTCHAR Converter((const ANSICHAR*)InData.GetData(), InData.Num());
    FString JsonString(Converter.Length(), Converter.Get());

    return FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, this);
}
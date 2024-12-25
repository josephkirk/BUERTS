#include "BEVYNetworkSettings.h"

UBEVYNetworkSettings::UBEVYNetworkSettings()
{
    // Default settings
    SignalingServerUrl = TEXT("ws://localhost:8080");
    StunServers.Add(TEXT("stun:stun.l.google.com:19302"));
    MaxBandwidthKBs = 1024; // 1 MB/s
    bEnableUnreliableChannel = true;

    // Categories
    CategoryName = TEXT("Plugins");
}
# BUERTS Class Diagrams

## System Overview

```mermaid
graph TB
    subgraph "Client (Unreal Engine)"
        UP[BEVYNetwork Plugin] --> UGS[Game Subsystem]
        UP --> UCM[Connection Manager]
        UP --> UNS[Network Subsystem]
        UGS --> GSM[Game State Manager]
        UCM --> WSC[WebRTC Connection]
        UNS --> MSH[Message Handler]
    end

    subgraph "Server (Bevy)"
        BEV[Bevy Engine] --> NP[Network Plugin]
        BEV --> GP[Game Plugin]
        BEV --> SP[Session Plugin]
        NP --> NC[Network Coordinator]
        GP --> GC[Game Coordinator]
        SP --> SC[Session Coordinator]
    end

    UCM <--> NC
```

## Client Plugin Structure

```mermaid
classDiagram
    class FBEVYNetworkModule {
        +StartupModule()
        +ShutdownModule()
        +Get()
        +IsAvailable()
    }
    
    class UBEVYNetworkSettings {
        +FString SignalingServerUrl
        +TArray<FString> StunServers
        +TArray<FString> TurnServers
        +FString TurnUsername
        +FString TurnCredential
        +int32 MaxBandwidthKBs
        +bool bEnableUnreliableChannel
    }
    
    class URTSWebRTCConnection {
        -TSharedPtr<IWebRTCProvider> WebRTCProvider
        -TSharedPtr<IWebRTCPeerConnection> PeerConnection
        -TSharedPtr<IWebRTCDataChannel> ReliableChannel
        -TSharedPtr<IWebRTCDataChannel> UnreliableChannel
        +Connect(FString SignalingServerUrl)
        +Disconnect()
        +SendMessage(FBEVYNetworkMessage Message)
        +bool IsConnected()
    }
    
    class FBEVYNetworkMessage {
        +EBEVYMessageType Type
        +FString Payload
        +int32 SequenceNumber
        +double Timestamp
        +bool Serialize(TArray<uint8>& OutData)
        +bool Deserialize(TArray<uint8>& InData)
    }

    FBEVYNetworkModule --> UBEVYNetworkSettings
    URTSWebRTCConnection --> FBEVYNetworkMessage
    FBEVYNetworkModule --> URTSWebRTCConnection
```

## Server Architecture

```mermaid
classDiagram
    class WebRTCSignalingPlugin {
        +build(App)
    }
    
    class GamePlugin {
        +build(App)
    }
    
    class SessionPlugin {
        +build(App)
    }
    
    class SignalingState {
        +HashMap<String, Sender> clients
    }
    
    class GameState {
        +HashMap<Uuid, Entity> entities
        +u64 tick
        +f64 last_update
    }
    
    class SessionManager {
        +HashMap<Uuid, GameSession> sessions
        +create_session()
        +add_player()
        +remove_player()
    }
    
    WebRTCSignalingPlugin --> SignalingState
    GamePlugin --> GameState
    SessionPlugin --> SessionManager
```

## Network Protocol Flow

```mermaid
sequenceDiagram
    participant Client
    participant SignalingServer
    participant GameServer
    
    Client->>SignalingServer: Connect
    SignalingServer-->>Client: Accept
    
    Client->>SignalingServer: Create Offer
    SignalingServer->>GameServer: Forward Offer
    GameServer->>SignalingServer: Create Answer
    SignalingServer-->>Client: Send Answer
    
    loop ICE Candidates
        Client->>SignalingServer: ICE Candidate
        SignalingServer->>GameServer: Forward Candidate
        GameServer->>SignalingServer: ICE Candidate
        SignalingServer-->>Client: Forward Candidate
    end
    
    Client<->GameServer: WebRTC Connection Established
    
    loop Game Loop
        GameServer->>Client: State Update
        Client->>GameServer: Commands
        GameServer->>Client: State Reconciliation
    end
```

## State Synchronization

```mermaid
graph TD
    subgraph "Client"
        CP[Player Input] --> CC[Command Creation]
        CC --> CQ[Command Queue]
        CQ --> CS[Command Sender]
        SR[State Receiver] --> SP[State Processor]
        SP --> CR[Client Reconciliation]
        CR --> R[Renderer]
    end
    
    subgraph "Server"
        CR[Command Receiver] --> CV[Command Validator]
        CV --> CP[Command Processor]
        CP --> SU[State Updater]
        SU --> SS[State Sender]
    end
    
    CS --> CR
    SS --> SR
```

## Component Relationships

```mermaid
classDiagram
    class Entity {
        +Uuid id
        +Vector3 position
        +Vector3 rotation
        +HashMap properties
    }
    
    class NetworkMessage {
        +MessageType type
        +String payload
        +i32 sequence_number
        +f64 timestamp
    }
    
    class Command {
        +CommandType type
        +Vec<String> target_ids
        +Vector3 position
    }
    
    class GameState {
        +Vec<Entity> entities
        +u64 tick
        +f64 timestamp
    }
    
    NetworkMessage --> Command
    NetworkMessage --> GameState
    GameState --> Entity
```

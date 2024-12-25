# BUERTS (Bevy-Unreal Engine RTS)

A real-time strategy game implementation using Bevy as the server and Unreal Engine as the client. This project demonstrates a hybrid game architecture combining the efficiency of Rust/Bevy for game logic and physics with the powerful rendering and client-side capabilities of Unreal Engine.

## Architecture Overview

### Server (Bevy)
- Written in Rust using the Bevy game engine
- Handles authoritative game state
- Manages physics and game logic
- Processes player inputs
- Provides real-time state synchronization
- WebSocket-based networking layer

### Client (Unreal Engine)
- Handles game rendering and visual effects
- Processes player input
- Implements client-side prediction
- Manages UI/UX
- Provides networked state reconciliation

## Prerequisites

### Server
- Rust (latest stable version)
- Cargo package manager
- Development libraries for Bevy

### Client
- Unreal Engine 5.2 or later
- Visual Studio 2019 or later
- WebSockets plugin for Unreal Engine

## Project Structure

```
BUERTS/
├── server/             # Bevy server implementation
│   ├── src/           # Server source code
│   └── Cargo.toml     # Rust dependencies
│
├── client/            # Unreal Engine client
│   └── Source/        # Client source code
│
└── docs/              # Documentation
```

## Getting Started

### Server Setup
1. Navigate to the server directory:
   ```bash
   cd server
   ```

2. Build and run the server:
   ```bash
   cargo run
   ```

### Client Setup
1. Open the Unreal Engine project in the client directory
2. Compile the project in Visual Studio
3. Add the NetworkManager actor to your level
4. Configure the WebSocket connection in the editor

## Development Workflow

1. Server Development
   - Use `cargo build` to compile
   - Use `cargo test` to run tests
   - Server runs on `ws://localhost:8080` by default

2. Client Development
   - Compile C++ code in Visual Studio
   - Use Unreal Editor for level design and testing
   - Connect to server using the NetworkManager

## Network Protocol

The server and client communicate using a WebSocket protocol with JSON messages:

### Client Messages
- Connect: Initial connection request
- Command: Player commands (movement, actions)
- Disconnect: Clean disconnection

### Server Messages
- Connected: Connection acknowledgment
- StateUpdate: Game state updates
- Error: Error notifications

## Contributing

1. Fork the repository
2. Create your feature branch
3. Commit your changes
4. Push to the branch
5. Create a new Pull Request

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

- Bevy Game Engine community
- Unreal Engine community
- Contributors and testers
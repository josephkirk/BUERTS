# BUERTS (Bevy-Unreal Engine Real-Time Strategy)

A real-time strategy game implementation using Bevy as the authoritative server and Unreal Engine as the client, connected via WebRTC for low-latency networking.

## Overview

BUERTS demonstrates a hybrid game architecture that combines:
- Bevy's efficient ECS (Entity Component System) for game logic and physics
- Unreal Engine's powerful rendering and client capabilities
- WebRTC for reliable, low-latency networking
- Modular design for extensibility

### Key Features

- WebRTC-based networking with both reliable and unreliable channels
- Authoritative server architecture
- Client-side prediction and reconciliation
- Efficient state synchronization
- Session management system
- Modular plugin design

## Project Structure

```
BUERTS/
├── client/                    # Unreal Engine client
│   └── Plugins/
│       └── BEVYNetwork/      # Network plugin for UE
├── server/                   # Bevy server
│   ├── src/                  # Server source code
│   │   ├── game/            # Game logic
│   │   ├── network/         # Networking
│   │   └── session/         # Session management
│   └── config.toml          # Server configuration
├── protocol/                 # Shared protocol definitions
└── docs/                    # Documentation
```

## Prerequisites

### Server (Rust/Bevy)
- Rust (latest stable version)
- Cargo package manager
- Required system libraries:
  ```bash
  # Ubuntu/Debian
  apt install build-essential pkg-config libssl-dev

  # Windows
  # Install Visual Studio Build Tools
  ```

### Client (Unreal Engine)
- Unreal Engine 5.2 or later
- Visual Studio 2019 or later
- WebRTC plugin for Unreal Engine
- CMake 3.20+

## Getting Started

### Building the Server

1. Navigate to server directory:
   ```bash
   cd server
   ```

2. Install dependencies and build:
   ```bash
   cargo build --release
   ```

3. Configure server:
   - Copy `config.example.toml` to `config.toml`
   - Modify settings as needed

4. Run the server:
   ```bash
   cargo run --release
   ```

### Setting Up the Client

1. Open Unreal Engine project in the client directory
2. Build the BEVYNetwork plugin:
   - Right-click the .uproject file
   - Select "Generate Visual Studio project files"
   - Open solution and build

3. Configure the plugin:
   - Open Project Settings
   - Navigate to BEVY Network Settings
   - Set signaling server URL and other parameters

## Development

### Server Development

- Use `cargo check` for quick syntax verification
- Run tests: `cargo test`
- Format code: `cargo fmt`
- Check lints: `cargo clippy`

### Client Development

- Follow Unreal Engine C++ coding standards
- Use Visual Studio for C++ development
- Enable Hot Reload for faster iteration
- Test with PIE (Play In Editor)

### Network Protocol

The client and server communicate using a WebRTC data channel with two types of messages:
1. Reliable channel for critical game data
2. Unreliable channel for frequent updates (positions, etc.)

Message types include:
- State updates
- Commands
- Session management
- Connection handling

## Building for Release

### Server
```bash
cd server
cargo build --release
```

### Client
1. Package the project in Unreal Engine
2. Include the BEVYNetwork plugin
3. Configure for desired platforms

## Contributing

1. Fork the repository
2. Create your feature branch
3. Write tests for new features
4. Format and lint your code
5. Submit a pull request

### Code Style

- Rust: Follow rustfmt and clippy guidelines
- C++: Follow Unreal Engine coding standards
- Document public APIs
- Write meaningful commit messages

## Testing

- Run server tests: `cargo test`
- Run client tests through Unreal Test framework
- Test network conditions using provided tools
- Verify against different platforms

## License

This project is licensed under the MIT License - see LICENSE file for details.

## Acknowledgments

- Bevy Game Engine community
- Unreal Engine community
- WebRTC contributors
- Project contributors

## Support

- Create an issue for bug reports
- Join discussions for feature requests
- Check documentation for common issues

## Roadmap

See [TODO.md](TODO.md) for planned features and improvements.
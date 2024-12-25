mod game;
mod network;
mod session;

use bevy::prelude::*;
use game::GamePlugin;
use network::WebRTCSignalingPlugin;
use session::SessionPlugin;
use std::fs;
use toml::Value;

fn main() {
    // Load configuration
    let config = fs::read_to_string("config.toml")
        .expect("Failed to read config file");
    let config: Value = toml::from_str(&config)
        .expect("Failed to parse config file");

    // Initialize logger
    env_logger::init();

    // Create Bevy app
    App::new()
        .add_plugins(MinimalPlugins)
        .add_plugin(WebRTCSignalingPlugin)
        .add_plugin(SessionPlugin)
        .add_plugin(GamePlugin)
        .insert_resource(config)
        .add_systems(Startup, setup)
        .run();
}

fn setup(
    mut commands: Commands,
    config: Res<Value>,
) {
    // Initialize server resources based on config
    if let Some(server_config) = config.get("server") {
        log::info!("Server starting with config: {:?}", server_config);
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_config_loading() {
        // Add configuration tests
    }

    #[test]
    fn test_game_state() {
        // Add game state tests
    }
}
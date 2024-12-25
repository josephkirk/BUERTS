use serde::{Deserialize, Serialize};
use std::collections::HashMap;

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct Vector3 {
    pub x: f32,
    pub y: f32,
    pub z: f32,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub enum MessageType {
    Command,
    StateUpdate,
    Connect,
    Disconnect,
    Ping,
    Error,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct NetworkMessage {
    pub message_type: MessageType,
    pub payload: String,
    pub sequence_number: i32,
    pub timestamp: f64,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct GameState {
    pub timestamp: f64,
    pub tick: i32,
    pub entities: Vec<Entity>,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct Entity {
    pub id: String,
    pub entity_type: String,
    pub position: Vector3,
    pub rotation: Vector3,
    pub properties: HashMap<String, serde_json::Value>,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub enum Command {
    Move {
        target_ids: Vec<String>,
        position: Vector3,
        formation: Option<String>,
    },
    Attack {
        attacker_ids: Vec<String>,
        target_id: String,
    },
    Build {
        builder_id: String,
        building_type: String,
        position: Vector3,
        rotation: f32,
    },
    Gather {
        gatherer_ids: Vec<String>,
        resource_id: String,
    },
}

impl NetworkMessage {
    pub fn new(message_type: MessageType, payload: String) -> Self {
        Self {
            message_type,
            payload,
            sequence_number: 0,
            timestamp: 0.0,
        }
    }

    pub fn command(command: Command) -> Result<Self, serde_json::Error> {
        Ok(Self::new(
            MessageType::Command,
            serde_json::to_string(&command)?,
        ))
    }

    pub fn state_update(state: GameState) -> Result<Self, serde_json::Error> {
        Ok(Self::new(
            MessageType::StateUpdate,
            serde_json::to_string(&state)?,
        ))
    }
}
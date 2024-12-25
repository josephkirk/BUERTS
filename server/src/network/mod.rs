use bevy::prelude::*;
use futures::{SinkExt, StreamExt};
use serde::{Deserialize, Serialize};
use std::collections::HashMap;
use tokio::sync::mpsc;
use tokio_tungstenite::tungstenite::Message;

// Message types
#[derive(Debug, Serialize, Deserialize)]
pub enum SignalingMessage {
    Offer {
        sdp: String,
        client_id: String,
    },
    Answer {
        sdp: String,
        client_id: String,
    },
    IceCandidate {
        candidate: String,
        sdp_mid: String,
        sdp_m_line_index: i32,
        client_id: String,
    },
    Connect {
        client_id: String,
    },
    Disconnect {
        client_id: String,
    },
}

#[derive(Debug)]
pub struct WebRTCSignalingPlugin;

impl Plugin for WebRTCSignalingPlugin {
    fn build(&self, app: &mut App) {
        app.insert_resource(SignalingState::default())
            .add_systems(Update, handle_signaling_messages);
    }
}

#[derive(Debug, Resource, Default)]
pub struct SignalingState {
    pub clients: HashMap<String, mpsc::UnboundedSender<Message>>,
}

fn handle_signaling_messages(
    mut signaling_state: ResMut<SignalingState>,
    // Add your message event reader here
) {
    // Handle incoming signaling messages
}
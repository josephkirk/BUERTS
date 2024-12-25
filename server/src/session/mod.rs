use bevy::prelude::*;
use std::collections::HashMap;
use uuid::Uuid;
use crate::network::WebRTCSignalingPlugin;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum SessionState {
    Lobby,
    Starting,
    InProgress,
    Finished,
}

#[derive(Debug)]
pub struct GameSession {
    pub id: Uuid,
    pub players: HashMap<String, Player>,
    pub state: SessionState,
    pub created_at: f64,
    pub started_at: Option<f64>,
}

#[derive(Debug)]
pub struct Player {
    pub id: String,
    pub name: String,
    pub ready: bool,
    pub connected: bool,
}

pub struct SessionPlugin;

impl Plugin for SessionPlugin {
    fn build(&self, app: &mut App) {
        app.init_resource::<SessionManager>()
            .add_systems(Update, (
                update_sessions,
                check_session_timeouts,
                handle_player_disconnects,
            ));
    }
}

#[derive(Resource, Default)]
pub struct SessionManager {
    pub sessions: HashMap<Uuid, GameSession>,
}

impl SessionManager {
    pub fn create_session(&mut self) -> Uuid {
        let session_id = Uuid::new_v4();
        let session = GameSession {
            id: session_id,
            players: HashMap::new(),
            state: SessionState::Lobby,
            created_at: bevy::utils::Duration::from_secs_f64(0.0).as_secs_f64(),
            started_at: None,
        };
        self.sessions.insert(session_id, session);
        session_id
    }

    pub fn add_player(&mut self, session_id: Uuid, player_id: String, name: String) -> bool {
        if let Some(session) = self.sessions.get_mut(&session_id) {
            if session.state == SessionState::Lobby {
                let player = Player {
                    id: player_id.clone(),
                    name,
                    ready: false,
                    connected: true,
                };
                session.players.insert(player_id, player);
                return true;
            }
        }
        false
    }

    pub fn remove_player(&mut self, session_id: Uuid, player_id: &str) {
        if let Some(session) = self.sessions.get_mut(&session_id) {
            session.players.remove(player_id);
        }
    }

    pub fn get_session(&self, session_id: Uuid) -> Option<&GameSession> {
        self.sessions.get(&session_id)
    }

    pub fn get_session_mut(&mut self, session_id: Uuid) -> Option<&mut GameSession> {
        self.sessions.get_mut(&session_id)
    }
}

fn update_sessions(
    mut session_manager: ResMut<SessionManager>,
    time: Res<Time>,
) {
    for (_id, session) in session_manager.sessions.iter_mut() {
        if session.state == SessionState::Starting {
            if let Some(started_at) = session.started_at {
                if time.elapsed_seconds_f64() - started_at >= 5.0 {  // 5 second countdown
                    session.state = SessionState::InProgress;
                }
            }
        }
    }
}

fn check_session_timeouts(
    mut session_manager: ResMut<SessionManager>,
    time: Res<Time>,
) {
    let current_time = time.elapsed_seconds_f64();
    let timeout = 600.0; // 10 minutes

    session_manager.sessions.retain(|_, session| {
        let session_time = session.started_at.unwrap_or(session.created_at);
        current_time - session_time < timeout
    });
}

fn handle_player_disconnects(
    mut session_manager: ResMut<SessionManager>,
) {
    // Handle cleanup of disconnected players
}
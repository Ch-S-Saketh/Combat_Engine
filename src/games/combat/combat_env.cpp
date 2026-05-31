#include "combat_env.h"
#include "renderer.h"
#include "debug_overlay.h"

CombatEnvironment::CombatEnvironment() {
    reset();
}

void CombatEnvironment::reset() {
    initialize_simulation(sim);
}

PlayerInput CombatEnvironment::decode_action(int action) const {
    PlayerInput input{};
    input.left = (action & 1) != 0;
    input.right = (action & 2) != 0;
    input.attack = (action & 4) != 0;
    return input;
}

void CombatEnvironment::step(int action_p1, int action_p2) {
    PlayerInput p1 = decode_action(action_p1);
    PlayerInput p2 = decode_action(action_p2);
    
    simulate_frame(sim, p1, p2);
}

std::vector<float> CombatEnvironment::get_observation(int player_id) const {
    std::vector<float> obs;
    // Basic flat feature vector of all active entities
    for (int i = 0; i < sim.num_entities; i++) {
        obs.push_back(sim.entities[i].position.x);
        obs.push_back(sim.entities[i].position.y);
        obs.push_back(sim.entities[i].velocity.x);
        obs.push_back(sim.entities[i].velocity.y);
        obs.push_back((float)sim.entities[i].health);
        obs.push_back((float)sim.entities[i].attackCooldown);
        obs.push_back(sim.entities[i].iframeTimer);
    }
    return obs;
}

float CombatEnvironment::get_reward(int player_id) const {
    // For a real ML setup, you'd calculate reward based on state delta.
    // For now, we can just return a dummy value or a heuristic.
    if (player_id < 0 || player_id >= sim.num_entities) return 0.0f;
    return (float)sim.entities[player_id].health; 
}

bool CombatEnvironment::is_done() const {
    for (int i = 0; i < sim.num_entities; i++) {
        if (sim.entities[i].health <= 0) return true;
    }
    return false;
}

void CombatEnvironment::render() const {
    render_simulation(sim);
}

void CombatEnvironment::render_debug(bool isPaused, bool isRecording, bool isPlaying) const {
    // We need to cast away constness because debug_overlay takes SimulationState& not const SimulationState&
    // Or we fix debug_overlay to take const SimulationState&
    debug_overlay(const_cast<SimulationState&>(sim), isPaused, isRecording, isPlaying);
}

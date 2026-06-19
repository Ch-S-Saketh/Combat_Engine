#include "dqn_agent.h"
#include <random>
#include <algorithm>

DQNAgent::DQNAgent(int state_size, int act_size) 
    : net({state_size, 24, 24, act_size}), action_size(act_size), batch_size(16), max_buffer_size(20000) {
}

int DQNAgent::get_action(const std::vector<float>& state, float epsilon) {
    float r = static_cast<float>(rand()) / RAND_MAX;
    if (r < epsilon) {
        return rand() % action_size;
    }
    
    std::vector<float> q_values = net.forward(state);
    int best_action = 0;
    for (int i = 1; i < action_size; ++i) {
        if (q_values[i] > q_values[best_action]) {
            best_action = i;
        }
    }
    return best_action;
}

void DQNAgent::update(const std::vector<float>& state, int action, float reward, const std::vector<float>& next_state, float alpha, float gamma, bool done) {
    if (replay_buffer.size() >= max_buffer_size) {
        replay_buffer.pop_front();
    }
    replay_buffer.push_back({state, action, reward, next_state, done});
    
    if (replay_buffer.size() < batch_size) return;
    
    // Sample a batch
    std::vector<Transition> batch;
    for (size_t i = 0; i < batch_size; ++i) {
        int idx = rand() % replay_buffer.size();
        batch.push_back(replay_buffer[idx]);
    }
    
    // Train on batch
    for (const auto& transition : batch) {
        float target_q = transition.reward;
        if (!transition.done) {
            std::vector<float> next_q = net.forward(transition.next_state);
            float max_next = next_q[0];
            for (int i = 1; i < action_size; ++i) {
                if (next_q[i] > max_next) max_next = next_q[i];
            }
            target_q += gamma * max_next;
        }
        
        std::vector<float> current_q = net.forward(transition.state);
        current_q[transition.action] = target_q;
        net.backward(current_q, alpha);
    }
}

void DQNAgent::save(const std::string& filename) const { net.save(filename); }
void DQNAgent::load(const std::string& filename) { net.load(filename); }

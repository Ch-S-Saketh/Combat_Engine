#pragma once
#include "neural_net.h"
#include <vector>
#include <deque>
#include <string>

struct Transition {
    std::vector<float> state;
    int action;
    float reward;
    std::vector<float> next_state;
    bool done;
};

class DQNAgent {
public:
    DQNAgent(int state_size, int action_size);
    
    int get_action(const std::vector<float>& state, float epsilon);
    void update(const std::vector<float>& state, int action, float reward, const std::vector<float>& next_state, float alpha, float gamma, bool done);
    
    void save(const std::string& filename) const;
    void load(const std::string& filename);

    // Expose for visualization
    const NeuralNet& get_network() const { return net; }

private:
    NeuralNet net;
    int action_size;
    
    std::deque<Transition> replay_buffer;
    size_t batch_size;
    size_t max_buffer_size;
};

#pragma once
#include <vector>

class Environment {
public:
    virtual ~Environment() = default;

    // ML-Agent Interface
    virtual void reset() = 0;
    virtual void step(int action_p1, int action_p2) = 0;
    virtual std::vector<float> get_observation(int player_id) const = 0;
    virtual float get_reward(int player_id) const = 0;
    virtual bool is_done() const = 0;

    // Engine Hooks
    virtual void render() const = 0;
    virtual void render_debug(bool isPaused, bool isRecording, bool isPlaying) const = 0;
};

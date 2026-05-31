#pragma once

#include "../../core/environment.h"
#include "simulation.h"

class CombatEnvironment : public Environment {
private:
    SimulationState sim;

    PlayerInput decode_action(int action) const;

public:
    CombatEnvironment();
    virtual ~CombatEnvironment() = default;

    void reset() override;
    void step(int action_p1, int action_p2) override;
    std::vector<float> get_observation(int player_id) const override;
    float get_reward(int player_id) const override;
    bool is_done() const override;

    void render() const override;
    void render_debug(bool isPaused, bool isRecording, bool isPlaying) const override;
};

#pragma once
#include "../../core/environment.h"
#include <vector>

struct Pipe {
    float x;
    float gap_y;
    bool passed;
};

class FlappyBirdEnv : public Environment {
public:
    FlappyBirdEnv();
    ~FlappyBirdEnv() override = default;

    void reset() override;
    void step(int action_p1, int action_p2) override;
    std::vector<float> get_observation(int player_id) const override;
    float get_reward(int player_id) const override;
    bool is_done() const override;

    void render() const override;
    void render_debug(bool isPaused, bool isRecording, bool isPlaying) const override;

private:
    float bird_y;
    float bird_vy;
    float reward_p1;
    bool crashed;
    std::vector<Pipe> pipes;

    int frame_counter;
};

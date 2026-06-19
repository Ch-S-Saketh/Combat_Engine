#include "flappy_bird_env.h"
#include <raylib.h>

const float GRAVITY = 15.0f;
const float JUMP_VELOCITY = -450.0f;
const float PIPE_SPEED = 200.0f;
const float PIPE_SPAWN_X = 1300.0f;
const float PIPE_WIDTH = 80.0f;
const float GAP_SIZE = 180.0f;
const float FIXED_DT = 1.0f / 144.0f; // Matching engine's fixed timestep

FlappyBirdEnv::FlappyBirdEnv() {
    reset();
}

void FlappyBirdEnv::reset() {
    bird_y = 360.0f;
    bird_vy = 0.0f;
    reward_p1 = 0.0f;
    crashed = false;
    pipes.clear();
    frame_counter = 0;
}

void FlappyBirdEnv::step(int action_p1, int action_p2) {
    if (crashed) return;

    reward_p1 = 0.0f; // Reset reward for this step

    // Action 1: Jump (checking if SPACE or A is pressed)
    // In main.cpp, SPACE maps to bit 2 (value 4) and A maps to bit 0 (value 1)
    if ((action_p1 & 4) != 0 || (action_p1 & 1) != 0) {
        bird_vy = JUMP_VELOCITY;
    }

    // Physics
    bird_vy += GRAVITY;
    bird_y += bird_vy * FIXED_DT;

    // Boundary checks (Screen height is 720)
    if (bird_y < 0.0f || bird_y > 720.0f) {
        crashed = true;
        reward_p1 = -100.0f;
        return;
    }

    // Pipe spawning (every ~120 frames)
    frame_counter++;
    if (frame_counter >= 120) {
        frame_counter = 0;
        // Deterministic pseudo-random generation would be better for true RL, 
        // but using simple Raylib GetRandomValue for this example.
        float gap_y = 100.0f + static_cast<float>(GetRandomValue(0, 340));
        pipes.push_back({PIPE_SPAWN_X, gap_y, false});
    }

    // Pipe logic
    for (auto& p : pipes) {
        p.x -= PIPE_SPEED * FIXED_DT;

        // Collision logic
        bool in_pipe_x = (100.0f + 20.0f > p.x) && (100.0f - 20.0f < p.x + PIPE_WIDTH);
        bool in_gap_y = (bird_y - 20.0f > p.gap_y) && (bird_y + 20.0f < p.gap_y + GAP_SIZE);

        if (in_pipe_x && !in_gap_y) {
            crashed = true;
            reward_p1 = -100.0f;
            return;
        }

        // Passing reward (+1 when bird completely passes a pipe)
        if (!p.passed && p.x + PIPE_WIDTH < 100.0f - 20.0f) {
            p.passed = true;
            reward_p1 = 1.0f;
        }
    }

    // Remove off-screen pipes
    if (!pipes.empty() && pipes.front().x < -PIPE_WIDTH) {
        pipes.erase(pipes.begin());
    }
}

// What the RL Agent sees
std::vector<float> FlappyBirdEnv::get_observation(int player_id) const {
    if (pipes.empty()) {
        return {bird_y, bird_vy, PIPE_SPAWN_X, 360.0f, 360.0f + GAP_SIZE};
    }
    const Pipe& p = pipes.front();
    return {bird_y, bird_vy, p.x, p.gap_y, p.gap_y + GAP_SIZE};
}

float FlappyBirdEnv::get_reward(int player_id) const {
    return reward_p1;
}

bool FlappyBirdEnv::is_done() const {
    return crashed;
}

void FlappyBirdEnv::render() const {
    // Draw Bird
    DrawCircle(100, static_cast<int>(bird_y), 20, YELLOW);

    // Draw Pipes
    for (const auto& p : pipes) {
        // Top pipe
        DrawRectangle(static_cast<int>(p.x), 0, static_cast<int>(PIPE_WIDTH), static_cast<int>(p.gap_y), GREEN);
        // Bottom pipe
        DrawRectangle(static_cast<int>(p.x), static_cast<int>(p.gap_y + GAP_SIZE), static_cast<int>(PIPE_WIDTH), 720 - static_cast<int>(p.gap_y + GAP_SIZE), GREEN);
    }
}

void FlappyBirdEnv::render_debug(bool isPaused, bool isRecording, bool isPlaying) const {
    DrawText("FLAPPY BIRD RL", 10, 10, 20, WHITE);
    if (crashed) DrawText("CRASHED - Press ENTER to restart", 10, 40, 20, RED);
    DrawText(TextFormat("Reward: %.1f", reward_p1), 10, 70, 20, GREEN);
}

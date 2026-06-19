#include <raylib.h>

#include "core/timestep.h"
#include "core/environment.h"
#include "game_entry.h"
#include "core/replay.h"
#include "core/dqn_agent.h"

enum class AppState {
    MainMenu,
    Gameplay,
    PauseScreen
};

#include <iostream>
#include <cstring>
#include <cstdlib>

int main(int argc, char* argv[]) {
    bool train_mode = false;
    bool play_mode = false;

    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "--train") {
            train_mode = true;
        } else if (std::string(argv[i]) == "--play") {
            play_mode = true;
        }
    }

    Environment* env = CreateGameEnvironment();

    if (train_mode) {
        std::cout << "🚀 Starting Deep Q-Learning Agent Training Mode..." << std::endl;
        DQNAgent agent(3, 2); 
        
        const int TOTAL_EPISODES = 2500;
        float epsilon = 1.0f;
        float alpha = 0.001f; // DQN learning rate
        float gamma = 0.95f;
        int total_steps = 0;

        for (int e = 1; e <= TOTAL_EPISODES; ++e) {
            env->reset();
            int steps = 0;
            float episode_reward = 0.0f;
            
            std::vector<float> state = env->get_observation(0);
            
            while (!env->is_done()) {
                int action_idx = agent.get_action(state, epsilon);
                int action_p1 = (action_idx == 1) ? 4 : 0; 
                
                env->step(action_p1, 0);
                steps++;
                
                std::vector<float> next_state = env->get_observation(0);
                float reward = env->get_reward(0);
                episode_reward += reward;
                
                bool done = env->is_done();
                agent.update(state, action_idx, reward, next_state, alpha, gamma, done);
                state = next_state;
                
                if (steps > 15000) break; // Safety timeout
            }
            total_steps += steps;

            if (epsilon > 0.01f) epsilon *= 0.994f;

            if (e % 100 == 0 || e == TOTAL_EPISODES) {
                std::cout << "Episode " << e << " | Steps: " << steps 
                          << " | Final Reward: " << episode_reward 
                          << " | Epsilon: " << epsilon << std::endl;
            }
        }
        agent.save("agent.bin");
        std::cout << "✅ Training Complete! Saved to agent.bin." << std::endl;
        delete env;
        return 0;
    }

    InitWindow(1280,720,"RLEngine");
    SetTargetFPS(144);

    DQNAgent agent(3, 2);
    if (play_mode) {
        agent.load("agent.bin");
        std::cout << "✅ Loaded trained agent from agent.bin" << std::endl;
    }

    ReplaySystem replay;
    replay.playbackMode = false;
    replay.playbackFrame = 0;
    bool isRecording = false;
    bool isPaused = false;

    env->reset();

    AppState currentState = AppState::MainMenu;

    float accumulator = 0.0f;

    while(!WindowShouldClose()){
        if (currentState == AppState::MainMenu) {
            if (IsKeyPressed(KEY_ENTER)) {
                currentState = AppState::Gameplay;
                env->reset();
            }
            
            BeginDrawing();
            ClearBackground(BLACK);
            DrawText("MAIN MENU", 500, 300, 40, WHITE);
            DrawText("Press ENTER to start", 500, 350, 20, LIGHTGRAY);
            EndDrawing();
        }
        else if (currentState == AppState::Gameplay) {
            if (IsKeyPressed(KEY_ESCAPE)) {
                currentState = AppState::PauseScreen;
            }

            float frameTime = GetFrameTime();
            accumulator += frameTime;
            if (IsKeyPressed(KEY_TAB)) isPaused = !isPaused;
            if (IsKeyPressed(KEY_R)) {
                isRecording = !isRecording;
                if (isRecording) {
                    replay.frames.clear();
                    env->reset();
                }
            }
            if (IsKeyPressed(KEY_P)) {
                replay.playbackMode = !replay.playbackMode;
                if (replay.playbackMode) {
                    replay.playbackFrame = 0;
                    env->reset();
                }
            }

            while(accumulator>=FIXED_DT){
                bool shouldStep = !isPaused || IsKeyPressed(KEY_F);

                if (shouldStep) {
                    int action_p1 = 0;
                    int action_p2 = 0;

                    if (play_mode) {
                        std::vector<float> obs = env->get_observation(0);
                        int action_idx = agent.get_action(obs, 0.0f); // Greedy policy
                        action_p1 = (action_idx == 1) ? 4 : 0;
                        
                        // Let user control player 2 manually (or just leave 0)
                        if (IsKeyDown(KEY_LEFT)) action_p2 |= 1;
                        if (IsKeyDown(KEY_RIGHT)) action_p2 |= 2;
                        if (IsKeyDown(KEY_ENTER)) action_p2 |= 4;
                    } else if (replay.playbackMode) {
                        ReplayFrame frame = get_replay_frame(replay, replay.playbackFrame++);
                        action_p1 = frame.action_p1;
                        action_p2 = frame.action_p2;
                        if (replay.playbackFrame >= replay.frames.size()) {
                            replay.playbackMode = false;
                        }
                    } else {
                        if (IsKeyDown(KEY_A)) action_p1 |= 1;
                        if (IsKeyDown(KEY_D)) action_p1 |= 2;
                        if (IsKeyDown(KEY_SPACE)) action_p1 |= 4;

                        if (IsKeyDown(KEY_LEFT)) action_p2 |= 1;
                        if (IsKeyDown(KEY_RIGHT)) action_p2 |= 2;
                        if (IsKeyDown(KEY_ENTER)) action_p2 |= 4;

                        if (isRecording) {
                            record_frame(replay, action_p1, action_p2);
                        }
                    }

                    env->step(action_p1, action_p2);
                }

                accumulator -= FIXED_DT;
                if (isPaused) accumulator = 0.0f;
            }
            BeginDrawing();
            ClearBackground(BLACK);
            env->render();
            env->render_debug(isPaused, isRecording, replay.playbackMode);
            
            if (play_mode) {
                DrawText("AI Playing (DQN)", 10, 10, 20, WHITE);
                
                const auto& net = agent.get_network();
                const auto& acts = net.get_activations();
                const auto& sizes = net.get_layer_sizes();
                
                if (acts.size() > 0) {
                    int start_x = 1140;
                    int start_y = 20;
                    int layer_spacing = 40;
                    
                    for (size_t l = 0; l < sizes.size(); ++l) {
                        int num_nodes = sizes[l];
                        int node_spacing = 150 / std::max(1, num_nodes);
                        
                        for (int n = 0; n < num_nodes; ++n) {
                            int x = start_x + l * layer_spacing;
                            int y = start_y + n * node_spacing;
                            
                            if (l < sizes.size() - 1) {
                                int next_nodes = sizes[l+1];
                                int next_spacing = 150 / std::max(1, next_nodes);
                                for (int nn = 0; nn < next_nodes; ++nn) {
                                    int nx = start_x + (l+1) * layer_spacing;
                                    int ny = start_y + nn * next_spacing;
                                    DrawLine(x, y, nx, ny, Fade(DARKGRAY, 0.05f));
                                }
                            }
                            
                            float act_val = acts[l][n];
                            Color c = (act_val > 0.05f) ? LIME : GRAY;
                            if (l == 0) c = SKYBLUE;
                            DrawCircle(x, y, 3, c);
                        }
                    }
                }
            } else {
                DrawText("Human Playing", 10, 10, 20, BLACK);
            }
            
            EndDrawing();
        }
        else if (currentState == AppState::PauseScreen) {
            if (IsKeyPressed(KEY_ESCAPE)) {
                currentState = AppState::Gameplay;
            }
            
            accumulator = 0.0f;
            
            BeginDrawing();
            ClearBackground(BLACK);
            env->render();

            if (play_mode) {
                DrawText("AI Playing (DQN)", 10, 10, 20, WHITE);
                
                const auto& net = agent.get_network();
                const auto& acts = net.get_activations();
                const auto& sizes = net.get_layer_sizes();
                
                if (acts.size() > 0) {
                    int start_x = 1140;
                    int start_y = 20;
                    int layer_spacing = 40;
                    
                    for (size_t l = 0; l < sizes.size(); ++l) {
                        int num_nodes = sizes[l];
                        int node_spacing = 150 / std::max(1, num_nodes);
                        
                        for (int n = 0; n < num_nodes; ++n) {
                            int x = start_x + l * layer_spacing;
                            int y = start_y + n * node_spacing;
                            
                            if (l < sizes.size() - 1) {
                                int next_nodes = sizes[l+1];
                                int next_spacing = 150 / std::max(1, next_nodes);
                                for (int nn = 0; nn < next_nodes; ++nn) {
                                    int nx = start_x + (l+1) * layer_spacing;
                                    int ny = start_y + nn * next_spacing;
                                    DrawLine(x, y, nx, ny, Fade(DARKGRAY, 0.05f));
                                }
                            }
                            
                            float act_val = acts[l][n];
                            Color c = (act_val > 0.05f) ? LIME : GRAY;
                            if (l == 0) c = SKYBLUE;
                            DrawCircle(x, y, 3, c);
                        }
                    }
                }
            } else {
                DrawText("Human Playing", 10, 10, 20, BLACK);
            }

            DrawRectangle(0, 0, 1280, 720, {0, 0, 0, 150});
            DrawText("PAUSED", 550, 300, 40, WHITE);
            DrawText("Press ESC to resume", 520, 350, 20, LIGHTGRAY);
            EndDrawing();
        }
    }
    CloseWindow();
    delete env;
    return 0;
}

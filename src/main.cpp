#include <raylib.h>

#include "core/timestep.h"
#include "game/simulation.h"
#include "game/replay.h"
#include "render/renderer.h"
#include "debug/debug_overlay.h"

int main(){
    InitWindow(1280,720,"Deterministic Combat Engine");
    SetTargetFPS(144);

    SimulationState sim;
    ReplaySystem replay;
    replay.playbackMode = false;
    replay.playbackFrame = 0;
    bool isRecording = false;
    bool isPaused = false;

    initialize_simulation(sim);

    float accumulator = 0.0f;

    while(!WindowShouldClose()){
        float frameTime = GetFrameTime();
        accumulator += frameTime;
        if (IsKeyPressed(KEY_TAB)) isPaused = !isPaused;
        if (IsKeyPressed(KEY_R)) {
            isRecording = !isRecording;
            if (isRecording) {
                replay.frames.clear();
                initialize_simulation(sim); // Reset state to start recording from frame 0
            }
        }
        if (IsKeyPressed(KEY_P)) {
            replay.playbackMode = !replay.playbackMode;
            if (replay.playbackMode) {
                replay.playbackFrame = 0;
                initialize_simulation(sim); // Reset state to start playback from frame 0
            }
        }

        while(accumulator>=FIXED_DT){
            bool shouldStep = !isPaused || IsKeyPressed(KEY_F);

            if (shouldStep) {
                PlayerInput p1{};
                PlayerInput p2{};

                if (replay.playbackMode) {
                    ReplayFrame frame = get_replay_frame(replay, replay.playbackFrame++);
                    p1 = frame.p1;
                    p2 = frame.p2;
                    if (replay.playbackFrame >= replay.frames.size()) {
                        replay.playbackMode = false; // End of playback
                    }
                } else {
                    p1.left = IsKeyDown(KEY_A);
                    p1.right = IsKeyDown(KEY_D);
                    p1.attack = IsKeyDown(KEY_SPACE);

                    p2.left = IsKeyDown(KEY_LEFT);
                    p2.right = IsKeyDown(KEY_RIGHT);
                    p2.attack = IsKeyDown(KEY_ENTER);

                    if (isRecording) {
                        record_frame(replay, p1, p2);
                    }
                }

                simulate_frame(sim,p1,p2);
            }

            accumulator -= FIXED_DT;
            if (isPaused) accumulator = 0.0f; // Don't build up accumulator while paused
        }
        BeginDrawing();
        ClearBackground(BLACK);
        render_simulation(sim);
        debug_overlay(sim, isPaused, isRecording, replay.playbackMode);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}

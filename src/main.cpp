#include <raylib.h>

#include "core/timestep.h"
#include "core/environment.h"
#include "game_entry.h"
#include "core/replay.h"

enum class AppState {
    MainMenu,
    Gameplay,
    PauseScreen
};

int main(){
    InitWindow(1280,720,"Deterministic Combat Engine");
    SetTargetFPS(144);

    Environment* env = CreateGameEnvironment();
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

                    if (replay.playbackMode) {
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

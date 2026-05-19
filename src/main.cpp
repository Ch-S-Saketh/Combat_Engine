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
    
    initialize_simulation(sim);

    float accumulator = 0.0f;

    while(!WindowShouldClose()){
        float frameTime = GetFrameTime();
        accumulator += frameTime;
        while(accumulator>=FIXED_DT){
            PlayerInput p1{};
            p1.left = IsKeyDown(KEY_A);
            p1.right = IsKeyDown(KEY_D);
            p1.attack = IsKeyDown(KEY_SPACE);

            PlayerInput p2{};
            p2.left = IsKeyDown(KEY_LEFT);
            p2.right = IsKeyDown(KEY_RIGHT);
            p2.attack = IsKeyDown(KEY_ENTER);

            simulate_frame(sim,p1,p2);

            accumulator -= FIXED_DT;
        }
        BeginDrawing();
        ClearBackground(BLACK);
        render_simulation(sim);
        debug_overlay(sim);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}

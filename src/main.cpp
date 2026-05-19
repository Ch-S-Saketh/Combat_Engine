#include <raylib.h>

#include "core/timestep.h"
#include "game/simulation.h"
// #include "game/replay.h"
// #include "render/rendered/h"

int main(){
    InitWindow(1280,720,"Deterministic Combat Engine");
    SetTargetFPS(144);

    SimulationState sim;
    // ReplaySystem replay;
    
    initialize_simulation(sim);

    float accumulator = 0.0f;

    while(!WindowShouldClose()){
        float frameTime = GetFrameTime();
        accumulator += frameTime;
        while(accumulator>=FIXED_DT){\
            PlayerInput p1{};
            PlayerInput p2{};

            simulate_frame(sim,p1,p2);

            accumulator -= FIXED_DT;
        }
        BeginDrawing();
        ClearBackground(BLACK);
        // render_simulation(sim);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}

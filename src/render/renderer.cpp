#include "renderer.h"

void render_simulation(const SimulationState &sim){
    for (int i = 0; i < sim.num_entities; i++) {
        Color color = (i == 0) ? RED : ((i == 1) ? BLUE : GREEN);
        DrawRectangle(sim.entities[i].position.x, sim.entities[i].position.y, 10, 10, color);
    }
}
#include "renderer.h"

void render_simulation(const SimulationState &sim){
    DrawRectangle(sim.players[0].position.x,sim.players[0].position.y,10,10,RED);
    DrawRectangle(sim.players[1].position.x,sim.players[1].position.y,10,10,BLUE);
}
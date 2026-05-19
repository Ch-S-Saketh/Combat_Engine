#include "simulation.h"

void initialize_simulation(SimulationState &sim){
    sim.frame = 0;

    sim.players[0].position = {200,300};
    sim.players[1].position = {800,300};

    sim.players[0].health = 100;
    sim.players[1].health = 100;
}

void simulate_frame(...){
    process_input(...);

    update_movement(...);

    update_combat(...);

    update_hitboxes(...);
    
    resolve_hits(...);

    sim.frame++;
}

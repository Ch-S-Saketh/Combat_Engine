#pragma once

#include "entity.h"
#include "input.h"

struct SimulationState{
    Entity players[2];

    int frame;
};

void initialize_simulation(SimulationState &sim);

void simulate_frame(
        SimulationState &sim,
        const PlayerInput &p1,
        const PlayerInput &p2
        );

unsigned int hash_state(const SimulationState &sim);

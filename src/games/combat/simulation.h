#pragma once

#include "entity.h"
#include "input.h"

constexpr int MAX_ENTITIES = 32;

struct SimulationState{
    Entity entities[MAX_ENTITIES];
    int num_entities;

    int frame;
};

void initialize_simulation(SimulationState &sim);

void simulate_frame(
        SimulationState &sim,
        const PlayerInput &p1,
        const PlayerInput &p2
        );

unsigned int hash_state(const SimulationState &sim);

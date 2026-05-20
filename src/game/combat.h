#pragma once

#include "simulation.h"

void process_attacks(SimulationState &sim, const PlayerInput &p1, const PlayerInput &p2);

Rectangle get_hurtbox(const Entity &entity);
Rectangle get_hitbox(const Entity &entity);

void resolve_hits(SimulationState &sim);

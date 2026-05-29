#include "simulation.h"
#include "combat.h"
#include <cstddef>

void initialize_simulation(SimulationState &sim){
    sim.frame = 0;
    sim.num_entities = 2;

    sim.entities[0].position = {200,300};
    sim.entities[1].position = {800,300};

    sim.entities[0].health = 100;
    sim.entities[1].health = 100;

    sim.entities[0].iframeTimer = 0.0f;
    sim.entities[1].iframeTimer = 0.0f;

    sim.entities[0].attackCooldown = 0;
    sim.entities[1].attackCooldown = 0;

    sim.entities[0].attacking = false;
    sim.entities[1].attacking = false;

    sim.entities[0].attackTimer = 0;
    sim.entities[1].attackTimer = 0;

    sim.entities[0].id = 0;
    sim.entities[1].id = 1;

    sim.entities[0].velocity = {0,0};
    sim.entities[1].velocity = {0,0};

}

void simulate_frame(SimulationState &sim, const PlayerInput &p1, const PlayerInput &p2){
    auto process_input = [](Entity &entity, const PlayerInput &input) {
        entity.velocity.x = 0;
        if(input.left) entity.velocity.x -= 5.0f;
        if(input.right) entity.velocity.x += 5.0f;
    };

    if (sim.num_entities > 0) process_input(sim.entities[0], p1);
    if (sim.num_entities > 1) process_input(sim.entities[1], p2);

    auto update_movement = [](Entity &entity) {
        if(!entity.attacking) {
            entity.position.x += entity.velocity.x;
            entity.position.y += entity.velocity.y;
        }
    };

    for (int i = 0; i < sim.num_entities; i++) {
        update_movement(sim.entities[i]);
    }

    process_attacks(sim, p1, p2);

    resolve_hits(sim);

    sim.frame++;
}

unsigned int hash_state(const SimulationState &sim) {
    unsigned int hash = 2166136261u;
    const unsigned char *data = reinterpret_cast<const unsigned char*>(&sim);
    for (size_t i = 0; i < sizeof(SimulationState); ++i) {
        hash ^= data[i];
        hash *= 16777619u;
    }
    return hash;
}

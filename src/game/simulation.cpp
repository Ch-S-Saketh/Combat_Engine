#include "simulation.h"
#include "combat.h"
#include <cstddef>

void initialize_simulation(SimulationState &sim){
    sim.frame = 0;

    sim.players[0].position = {200,300};
    sim.players[1].position = {800,300};

    sim.players[0].health = 100;
    sim.players[1].health = 100;

    sim.players[0].iframeTimer = 0.0f;
    sim.players[1].iframeTimer = 0.0f;

    sim.players[0].attackCooldown = 0;
    sim.players[1].attackCooldown = 0;

    sim.players[0].attacking = false;
    sim.players[1].attacking = false;

    sim.players[0].attackTimer = 0;
    sim.players[1].attackTimer = 0;

    sim.players[0].id = 0;
    sim.players[1].id = 1;

    sim.players[0].velocity = {0,0};
    sim.players[1].velocity = {0,0};

}

void simulate_frame(SimulationState &sim, const PlayerInput &p1, const PlayerInput &p2){
    auto process_input = [](Entity &entity, const PlayerInput &input) {
        entity.velocity.x = 0;
        if(input.left) entity.velocity.x -= 5.0f;
        if(input.right) entity.velocity.x += 5.0f;
    };

    process_input(sim.players[0], p1);
    process_input(sim.players[1], p2);

    auto update_movement = [](Entity &entity) {
        if(!entity.attacking) {
            entity.position.x += entity.velocity.x;
            entity.position.y += entity.velocity.y;
        }
    };

    update_movement(sim.players[0]);
    update_movement(sim.players[1]);

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

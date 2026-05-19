#include "simulation.h"
#include "combat.h"

void initialize_simulation(SimulationState &sim){
    sim.frame = 0;

    sim.players[0].position = {200,300};
    sim.players[1].position = {800,300};

    sim.players[0].health = 100;
    sim.players[1].health = 100;
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

    auto update_hitboxes = [](Entity &entity) {
        entity.hurtbox.x = entity.position.x - 25;
        entity.hurtbox.y = entity.position.y - 50;
        entity.hurtbox.width = 50;
        entity.hurtbox.height = 100;
        
        if(entity.attacking) {
            entity.hitbox.x = entity.position.x;
            entity.hitbox.y = entity.position.y - 25;
            entity.hitbox.width = 60;
            entity.hitbox.height = 50;
        } else {
            entity.hitbox = {0,0,0,0};
        }
    };

    update_hitboxes(sim.players[0]);
    update_hitboxes(sim.players[1]);
    
    resolve_hits(sim);

    sim.frame++;
}

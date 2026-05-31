#include "combat.h"

void process_attacks(SimulationState &sim, const PlayerInput &p1, const PlayerInput &p2){
    auto process_player_attack = [](Entity &entity, const PlayerInput &input) {
        if(input.attack && entity.attackCooldown == 0){
            entity.attacking = true;
            entity.attackTimer = 10;
            entity.attackCooldown = 30;
        }
        if(entity.attackCooldown > 0){
            entity.attackCooldown--;
        }
        if(entity.attackTimer > 0){
            entity.attackTimer--;
        } else {
            entity.attacking = false;
        }
    };

    if (sim.num_entities > 0) process_player_attack(sim.entities[0], p1);
    if (sim.num_entities > 1) process_player_attack(sim.entities[1], p2);
}

Rectangle get_hurtbox(const Entity &entity) {
    return { entity.position.x - 25, entity.position.y - 50, 50, 100 };
}

Rectangle get_hitbox(const Entity &entity) {
    if (entity.attacking) {
        return { entity.position.x, entity.position.y - 25, 60, 50 };
    }
    return { 0, 0, 0, 0 };
}

void resolve_hits(SimulationState &sim){
    for (int i = 0; i < sim.num_entities; i++) {
        Rectangle hitbox = get_hitbox(sim.entities[i]);
        if (hitbox.width == 0 && hitbox.height == 0) continue;

        for (int j = 0; j < sim.num_entities; j++) {
            if (i == j) continue;
            Rectangle hurtbox = get_hurtbox(sim.entities[j]);

            if(CheckCollisionRecs(hitbox, hurtbox) && sim.entities[j].iframeTimer == 0){
                sim.entities[j].health -=10;
                sim.entities[j].iframeTimer = 20;
            }
        }
    }

    for (int i = 0; i < sim.num_entities; i++) {
        if(sim.entities[i].iframeTimer>0){
            sim.entities[i].iframeTimer--;
        }
    }
}
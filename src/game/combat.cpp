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

    process_player_attack(sim.players[0], p1);
    process_player_attack(sim.players[1], p2);
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
    Rectangle p0_hitbox = get_hitbox(sim.players[0]);
    Rectangle p0_hurtbox = get_hurtbox(sim.players[0]);
    Rectangle p1_hitbox = get_hitbox(sim.players[1]);
    Rectangle p1_hurtbox = get_hurtbox(sim.players[1]);

    if(CheckCollisionRecs(p0_hitbox, p1_hurtbox) && sim.players[1].iframeTimer == 0){
        sim.players[1].health -=10;
        sim.players[1].iframeTimer = 20;
    }
    if(CheckCollisionRecs(p1_hitbox, p0_hurtbox) && sim.players[0].iframeTimer == 0){
        sim.players[0].health -=10;
        sim.players[0].iframeTimer = 20;
    }
    if(sim.players[0].iframeTimer>0){
        sim.players[0].iframeTimer--;
    }
    if(sim.players[1].iframeTimer>0){
        sim.players[1].iframeTimer--;
    }
}
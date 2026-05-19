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

void resolve_hits(SimulationState &sim){
    if(CheckCollisionRecs(sim.players[0].hitbox,sim.players[1].hurtbox)){
        sim.players[1].health -=10;
    }
    if(CheckCollisionRecs(sim.players[1].hitbox,sim.players[0].hurtbox)){
        sim.players[0].health -=10;
    }
}
#include "combat.h"

void main(){
    if(input.attack && entity.attackCooldown == 0){
        entity.attacking = true;

    }
    if(entity.attackCooldown > 0){
        entity.attackCooldown--;
    }
    if(CheckCollisionRecs(attacker.hitbox,defender.hurtbox){
        defender.health -=10;
    }
}
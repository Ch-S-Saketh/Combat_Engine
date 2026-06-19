#include "game_entry.h"

// 1. Include your game environment header here
#include "games/flappy_bird/flappy_bird_env.h"
// #include "games/combat/combat_env.h"

Environment* CreateGameEnvironment() {
    // 2. Return a new instance of your game
    return new FlappyBirdEnv();
    
    // return new CombatEnvironment();
}

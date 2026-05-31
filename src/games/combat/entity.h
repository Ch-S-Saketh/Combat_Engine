#pragma once

#include <raylib.h>

struct Entity{
    int id;
    Vector2 position;
    Vector2 velocity;
    int health;
    bool attacking;
    int attackTimer;
    int attackCooldown;
    float iframeTimer;

};

#pragma once
#include "core/environment.h"

// The engine calls this to instantiate the game.
// You only need to change this file to switch games.
Environment* CreateGameEnvironment();

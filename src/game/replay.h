#pragma once

#include <vector>
#include "input.h"

struct ReplayFrame{
    PlayerInput p1;
    PlayerInput p2;
};

struct ReplaySystem{
    std::vector<ReplayFrame> frames;

    bool playbackMode;

    int playbackFrame;
};
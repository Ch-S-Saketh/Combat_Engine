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

void record_frame(ReplaySystem &replay, const PlayerInput &p1, const PlayerInput &p2);
ReplayFrame get_replay_frame(const ReplaySystem &replay, int index);
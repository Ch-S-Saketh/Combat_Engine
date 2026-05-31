#pragma once

#include <vector>

struct ReplayFrame{
    int action_p1;
    int action_p2;
};

struct ReplaySystem{
    std::vector<ReplayFrame> frames;

    bool playbackMode;

    int playbackFrame;
};

void record_frame(ReplaySystem &replay, int action_p1, int action_p2);
ReplayFrame get_replay_frame(const ReplaySystem &replay, int index);
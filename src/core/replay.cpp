#include "replay.h"

void record_frame(ReplaySystem &replay, int action_p1, int action_p2){
    replay.frames.push_back({action_p1, action_p2});
}

ReplayFrame get_replay_frame(const ReplaySystem &replay, int index){
    if (index >= 0 && index < replay.frames.size()) {
        return replay.frames[index];
    }
    return {};
}
#include "replay.h"

void record_frame(ReplaySystem &replay, const PlayerInput &p1, const PlayerInput &p2){
    replay.frames.push_back({p1, p2});
}

ReplayFrame get_replay_frame(const ReplaySystem &replay, int index){
    if (index >= 0 && index < replay.frames.size()) {
        return replay.frames[index];
    }
    return {};
}
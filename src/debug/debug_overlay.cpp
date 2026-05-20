#include "debug_overlay.h"

void debug_overlay(SimulationState &sim, bool isPaused, bool isRecording, bool isPlaying){
    DrawText(TextFormat("Frame: %d",sim.frame),20,20,20,GREEN);

    DrawText(TextFormat("P1 Health: %d",sim.players[0].health),20,50,20,RED);
    DrawText(TextFormat("P2 Health: %d",sim.players[1].health),20,80,20,BLUE);

    DrawText(TextFormat("Hash: %08X", hash_state(sim)), 20, 110, 20, YELLOW);

    if (isPaused) DrawText("PAUSED (Press TAB to resume, F to step)", 20, 140, 20, PURPLE);
    if (isRecording) DrawText("RECORDING (Press R to stop)", 20, 170, 20, RED);
    if (isPlaying) DrawText("PLAYING BACK (Press P to stop)", 20, 200, 20, BLUE);
}
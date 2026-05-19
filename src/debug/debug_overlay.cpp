#include "debug_overlay.h"

void debug_overlay(SimulationState &sim){
    DrawText(TextFormat("Frame: %d",sim.frame),20,20,20,GREEN);

    DrawText(TextFormat("P1 Health: %d",sim.players[0].health),20,50,20,RED);
    DrawText(TextFormat("P2 Health: %d",sim.players[1].health),20,80,20,BLUE);

}
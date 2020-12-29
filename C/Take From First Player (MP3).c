// NAME: Take From First Player (MP3)
// GAMES: MP3_USA
// EXECUTION: Direct
// PARAM: Number|NUM_COINS

#include "ultra64.h"

extern void AdjustPlayerCoins(s32 player_index, s32 count);

void main() {
    int i, min;
    s32 placementsOfPlayers[4];

    // Calculate all placements.
    for (i = 0; i < 4; i++) {
        // func_800EE9C0 gets player i's placement.
        // (A number value from 0 to 3, 0 being first place.)
        // Use func_800EECF0 instead to factor in bonus stars.
        placementsOfPlayers[i] = func_800EE9C0(i);
    }

    // Set min to be the minimum of each placement value.
    // (May not be necessary; it would only be if a tie for
    // first place yields 1 for both first place players.)
    min = 1000;
    for (i = 0; i < 4; i++) {
        if (placementsOfPlayers[i] < min) {
            min = placementsOfPlayers[i];
        }
    }

    // Take away coins from the top placing player(s).
    for (i = 0; i < 4; i++) {
        if (placementsOfPlayers[i] == min) {
            AdjustPlayerCoins(i, -NUM_COINS);
        }
    }
}
// NAME: Path Split N-Way (MP3)
// GAMES: MP3_USA
// EXECUTION: Process
// PARAM: Space[]|DESTINATIONS

// Implementation of an "n-way path split" event.
// Intended for use as a passing event on an invisible space.
// When the player reaches the space with this event,
// they will be prompted to choose a direction with the
// classic "red arrows." This event is intended to demonstrate
// the APIs the game provides for showing these arrows.
// The functionality may someday be built into PP64.

// Note that when using this custom event, you shouldn't connect
// the spaces with lines in PP64. You should instead configure the
// event's space parameters in the UI.

// Note that each target space should also have its own custom
// event that pushes the player back to the split space if they are
// moving in reverse.

#include "ultra64.h"

// Data pertaining to the destination spaces.
s16 destination_spaces[] = { DESTINATIONS, -1 };
s16 destination_chains[] = { DESTINATIONS_chain_indices, -1 };
s16 destination_chain_space_indices[] = { DESTINATIONS_chain_space_indices, -1 };

// Internals used by arrow setup/teardown.
extern s16 D_800CDD58;
extern s16 D_800D51F8;

// Pointers used by arrow setup/teardown.
void *unk_arrow_1;
void *unk_arrow_2;
void *unk_arrow_3;
void *unk_arrow_4;
void *unk_arrow_5;

// player struct definition
struct player {
    s8 id;
    s8 cpu_difficulty;
    u8 controller;
    u8 character;
    /**
     * Miscellaneous flags.
     * 1: Is CPU player
     * 4: ?
     */
    u8 flags;
    s8 pad5[1];
    s16 bonus_coins; // 6, coins collected in a mini-game
    s16 coins_won; // 8, coins from mini-game win
    s16 coins; // 10 (0xA)
    s16 minigame_coins; // 12 (0xC)
    s8 stars; // 14 (0xE)

    u8 cur_chain_index; // 15 (0xF)
    u8 cur_space_index; // 16 (0x10)
    u8 next_chain_index; // 17 (0x11)
    u8 next_space_index; // 18 (0x12)
    u8 unk1_chain_index; // 19 (0x13)
    u8 unk1_space_index; // 20 (0x14)
    s8 reverse_chain_index; // 21 (0x15)
    s8 reverse_space_index; // 22 (0x16)

    u8 flags2; // 23 (0x17)
    s8 items[3]; // 24 (0x18)
    s8 bowser_suit_flag; // 27 (0x1B)
    u8 turn_color_status; // 28 (0x1C)
    s8 unk1D; // 29 (0x1D)

    s8 unks1E1F[2]; // 20 - 31

    void *process; // 32 (0x20) struct process *
    void *obj; // 36 (0x24) struct object *
    s16 minigame_star; // 40 (0x28)
    s16 max_coins; // 42 (0x2A)
    s8 happening_space_count; // 44 (0x2C)
    s8 red_space_count;
    s8 blue_space_count;
    s8 chance_space_count;
    s8 bowser_space_count; // 48 (0x30)
    s8 battle_space_count;
    s8 item_space_count;
    s8 bank_space_count;
    s8 game_guy_space_count; // 52 (0x34)

    // s8 pad2[3];
}; // sizeof == 56 | 0x38

extern s16 GetCurrentPlayerIndex();
extern s16 GetCurrentSpaceIndex();
extern struct player *GetPlayerStruct(s32 index);
extern u32 PlayerIsCPU(s16 index);

// This takes an absolute index and sets the out params
// to the chain and chain space indices.
// Returns 0 if succeeded (valid abs index) or -1 if not found.
extern s32 func_800EB310(s16 absSpaceIndex, s8 *chainIndex, s8 *chainSpaceIndex);

// Entrypoint to the custom event.
void main() {
    s16 current_player_index = GetCurrentPlayerIndex();
    struct player *player = GetPlayerStruct(current_player_index);

    // If the player is moving in reverse, for simplicity,
    // just force them back on the path they came.
    // A more elaborate implementation would let them pick a
    // direction still.
    if ((player->flags2 & 1) != 0) {
        s8 nextChainIndex = -1;
        s8 nextChainSpaceIndex = -1;
        s32 currentSpaceIndex = GetCurrentSpaceIndex();
        if (func_800EB310(currentSpaceIndex, &nextChainIndex, &nextChainSpaceIndex) != -1) {
            SetNextChainAndSpace(current_player_index, nextChainIndex, nextChainSpaceIndex);
        }
        return;
    }

    // Set idle animation.
    func_800F2304(-1, -1, 2);

    SleepVProcess();

    SetupArrows();

    // This establishes red arrows for each of the destination spaces.
    // It returns a pointer to a struct that maintains the arrow state.
    void *arrowstate = func_800D76A0(current_player_index, destination_spaces, 2);

    // This probably initiates prompting for the player's selection.
    func_800D742C(arrowstate, current_player_index, 0);

    if (PlayerIsCPU(current_player_index) != 0) {
        // For CPUs, pick direction randomly.
        s32 ai_choice = GetRandomByte() % DESTINATIONS_length;

        // I think this simulates pressing the joystick to pick an arrow,
        // and then pressing A.
        for (s32 i = 0; i < ai_choice; i++) {
            func_800D7250(arrowstate, -2);
        }
        func_800D7250(arrowstate, -4);
    }

    // Get the currently selected arrow and exit out of selection mode.
    s16 choice = func_800D7518(arrowstate);

    // Destroys the arrows.
    func_800D6CA0(arrowstate);
    TeardownArrows();

    // Apply the player's choice.
    SetNextChainAndSpace(current_player_index, destination_chains[choice], destination_chain_space_indices[choice]);

    // Since this is a process-based event, this is necessary to indicate completion.
    EndProcess(NULL);
}

void SetupArrows() {
    while (func_800E9AE0() != 0) {
        SleepVProcess();
    }
    SleepVProcess();
    unk_arrow_1 = func_800E210C(0, 0x92, 1);
    unk_arrow_2 = func_800E210C(1, 0xA0, 1);
    unk_arrow_5 = func_800E210C(0xD, 0xAE, 1);
    unk_arrow_3 = func_800E210C(3, 0xBC, 1);
    unk_arrow_4 = func_800E210C(0xB, 0xCA, 1);
    SleepProcess(3);
    D_800CDD58 = 1;
    D_800D51F8 = 1;
}

void TeardownArrows() {
    D_800CDD58 = 0;
    D_800D51F8 = 0;
    func_800E21F4(unk_arrow_1);
    func_800E21F4(unk_arrow_2);
    func_800E21F4(unk_arrow_5);
    func_800E21F4(unk_arrow_3);
    func_800E21F4(unk_arrow_4);
}

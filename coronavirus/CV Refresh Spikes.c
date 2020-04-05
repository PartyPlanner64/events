// NAME: CV Refresh Spikes
// GAMES: MP3_USA
// EXECUTION: Direct

#include "ultra64.h"

struct coords3d {
    f32 x;
    f32 y;
    f32 z;
};

struct spacedata {
    s8 pad[8];
    struct coords3d coords;
    s8 pad2[8];
    void *unk24;
};

struct object {
    /*0x00*/ struct object *prev;
    /*0x04*/ struct object *next;
    /*0x08*/ s8 pad1[2];
    /*0x0A*/ u16 unka;

    /*0x0C*/ struct coords3d coords;

    /*0x18*/ f32 rot1;
    f32 rot2;
    f32 rot3;

    /*0x24*/ f32 unk36;
    f32 unk40;
    f32 unk44;

    /*0x30*/ f32 unk48;
    f32 funk52;
    f32 funk56;

    void *unk60;
    void *unk64;
    s8 pad5[2];
    s16 unk70;
};

struct player {
    s8 unk0;
    s8 cpu_difficulty;
    s8 controller;
    u8 character;
    /**
     * Miscellaneous flags.
     * 1: Is CPU player
     */
    u8 flags;
    s8 pad0[5];
    /**
     * Current coin count.
     */
    s16 coins; // 10
    /**
     * Coins obtained during a Mini-Game.
     */
    s16 minigame_coins; // 12
    s8 stars; // 14

    u8 cur_chain_index; // 15
    u8 cur_space_index; // 16
    u8 next_chain_index; // 17
    u8 next_space_index; // 18
    u8 unk1_chain_index; // 19
    u8 unk1_space_index; // 20
    u8 reverse_chain_index; // 21
    u8 reverse_space_index; // 22

    u8 flags2; // 23
    u8 items[3]; // 24
    u8 bowser_suit_flag; // 27
    u8 turn_color_status; // 28

    s8 pad1[7]; // 29 - 35

    void *obj; // 36 // struct object *
    s16 minigame_star; // 40
    s16 coin_star; // 42
    s8 happening_space_count; // 44
    s8 red_space_count;
    s8 blue_space_count;
    s8 chance_space_count;
    s8 bowser_space_count; // 48
    s8 battle_space_count;
    s8 item_space_count;
    s8 bank_space_count;
    s8 game_guy_space_count; // 52

    // s8 pad2[3];
}; // sizeof == 56

extern struct player *GetPlayerStruct(s32 player_index);

/** Used to show any model (called by 800D90C8) */
extern struct object *func_800D912C(
    u32 file, u32 shadowFile, f32 unk1, f32 unk2, void *unk3
);

s16 spike_spaces[] = {
    12, 13, 14, 15, 16, 17, 18, 19, 20, 21
};

/**
 * Stores the owner of each spike, 3 bits per spike.
 * 0: no owner
 * 1-4: player index of owner.
 */
extern u32 D_800CD098;

#define BITS_PER_SPIKE 3
#define NUM_SPIKES 10

s32 get_spike_owner(s32 spike_index) {
    u32 shifted = D_800CD098 >> (spike_index * BITS_PER_SPIKE);
    return shifted & 0x7;
}

struct object *spike_models[NUM_SPIKES];

// To expose spike_models to other events, this fixed
// RAM location will be set to a pointer to it (gross).
extern struct object **D_807A5550;

u32 get_owner_model_file(s32 character) {
    // Use a player flag pole model from directory 60.
    return 0x003C0000 | (18 + character);
}

struct object *draw_spike_model(s32 model_file, s32 spike_index) {
    struct object *charmodel;
    charmodel = func_800D912C(model_file, 0x00000009, 0.9f, 0.8f, NULL);
    charmodel->unka |= 4; // ?

    struct spacedata *spike_space_data;
    spike_space_data = GetSpaceData(spike_spaces[spike_index]);

    // Move the model to the space's coordinates.
    func_80089A20(&charmodel->coords, &spike_space_data->coords);

    return charmodel;
}

void main() {
    D_807A5550 = &spike_models;

    for (s32 i = 0; i < NUM_SPIKES; i++) {
        u32 spike_owner = get_spike_owner(i);

        if (spike_models[i] != NULL) {
            if (spike_owner == 0) {
                func_800D9B54(spike_models[i]); // Free model.
                spike_models[i] = NULL;
            }
            continue;
        }

        if (spike_owner == 0) {
            continue;
        }

        struct player *player = GetPlayerStruct(spike_owner - 1);

        s32 model_file = get_owner_model_file(player->character);
        spike_models[i] = draw_spike_model(model_file, i);
    }
}
// NAME: CV Bowser Event
// GAMES: MP3_USA
// EXECUTION: Direct
// PARAM: Space|BOWSER_MODEL_SPACE

#include "ultra64.h"

#define BOWSER_PICTURE 4

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
    void *unk0;
    void *unk4;
    s8 pad1[2];
    u16 unka;
    struct coords3d coords;
    void *unk24;
    s8 pad3[8];
    s8 unk36;
    f32 unk40;
    s8 pad4[4];
    f32 unk48;
    f32 funk52;
    f32 funk56;
    void *unk60;
    s8 pad5[6];
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

    struct object *obj; // 36
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

char *bowser_intro_msg =
    "\x0B" // Start the message
    "\x1A\x1A\x1A\x1A" // Standard padding for picture
    "\x11" // Player Character's Name
    "\x82" // ,
    " why so surprised\xC3"
    "\x0A" // Newline
    "\x1A\x1A\x1A\x1A"
    "Can\x5Ct I help defeat"
    "\x0A" // Newline
    "\x1A\x1A\x1A\x1A"
    "the "
    "\x03" // Red font
    "virus"
    "\x08" // White Font
    " too"
    "\xC3" // "?"
    "\xFF"; // Prompt

char *bowser_take_spikes_msg =
    "\x0B" // Start the message
    "\x1A\x1A\x1A\x1A" // Standard padding for picture
    "\x85\x85\x85" // ...
    "Don\x5Ct look at me"
    "\x0A" // Newline
    "\x1A\x1A\x1A\x1A"
    "like that\x85 I\x5Cm serious\xC2"
    "\xFF"
    "\x0B" // Feed
    "\x1A\x1A\x1A\x1A"
    "I\x5Cll help take those "
    "\x03" // Red font
    "spikes"
    "\x08" // White Font
    "\x0A" // Newline
    "\x1A\x1A\x1A\x1A"
    "off of your hands\x85"
    "\xFF"; // Prompt

char *bowser_no_spikes_msg =
    "\x0B" // Start the message
    "\x1A\x1A\x1A\x1A" // Standard padding for picture
    "\x85\x85\x85" // ...
    "Don\x5Ct look at me"
    "\x0A" // Newline
    "\x1A\x1A\x1A\x1A"
    "like that\x85 I mean it\xC2"
    "\xFF"
    "\x0B" // Feed
    "\x1A\x1A\x1A\x1A"
    "Here\x5Cs "
    "\x06" // Blue font
    "15 coins"
    "\x08" // White Font
    " to help the effort\x85"
    "\xFF"; // Prompt

char *bowser_take_coins_msg =
    "\x0B" // Start the message
    "\x1A\x1A\x1A\x1A" // Standard padding for picture
    "\x5D" // (
    "Surely others won\x5Ct mind"
    "\x0A" // Newline
    "\x1A\x1A\x1A\x1A"
    "\x03" // Red font
    "pitching in"
    "\x08" // White Font
    "\xC3" // ?
    "\x5E" // )
    "\xFF"; // Prompt

/**
 * Stores the owner of each spike, 3 bits per spike.
 * 0: no owner
 * 1-4: player index of owner.
 * Examples: All claimed player 1: 0x9249249
 */
extern u32 D_800CD098;

extern struct object **D_807A5550;

#define BITS_PER_SPIKE 3
#define NUM_SPIKES 10

s32 get_spike_owner(s32 spike_index) {
    u32 shifted = D_800CD098 >> (spike_index * BITS_PER_SPIKE);
    return shifted & 0x7;
}

void set_spike_owner(s32 spike_index, s32 owner) {
    u32 shiftAmount = (spike_index * BITS_PER_SPIKE);
    
    // Clear any old value.
    u32 shiftedClearMask = 0x7 << shiftAmount;
    D_800CD098 &= ~shiftedClearMask;
    
    // Set new value.
    u32 shiftedNewOwner = owner << shiftAmount;
    D_800CD098 |= shiftedNewOwner;
}

u32 all_spikes_claimed() {
    for (s32 i = 0; i < NUM_SPIKES; i++) {
        if (get_spike_owner(i) == 0) {
            return FALSE;
        }
    }
    return TRUE;
}

void update_spike_model(struct player *player, s32 spike_index) {
    if (D_807A5550 != NULL) {
        if (D_807A5550[spike_index] != NULL) {
            // Free prior characters model.
            func_800D9B54(D_807A5550[spike_index]);
            D_807A5550[spike_index] = NULL;
        }
    }
}

u32 player_has_any_spike(s32 player_index) {
    for (s32 i = 0; i < NUM_SPIKES; i++) {
        if (get_spike_owner(i) - 1 == player_index) {
            return TRUE;
        }
    }
    return FALSE;
}

u32 clear_player_spike_ownership(s32 player_index) {
    for (s32 i = 0; i < NUM_SPIKES; i++) {
        if (get_spike_owner(i) - 1 == player_index) {
            set_spike_owner(i, 0);
            update_spike_model(NULL, i);
        }
    }
    return FALSE;
}

s32 any_other_player_has_coins(s32 besides_player) {
    for (s32 i = 0; i < 4; i++) {
        if (i == besides_player) {
            continue;
        }
        struct player *player = GetPlayerStruct(i);
        if (player->coins > 0) {
            return TRUE;
        }
    }
    return FALSE;
}

void take_coins_from_other_players(s32 besides_player) {
    for (s32 i = 0; i < 4; i++) {
        if (i == besides_player) {
            continue;
        }
        struct player *player = GetPlayerStruct(i);
        AdjustPlayerCoinsGradual(i, -5);
        ShowPlayerCoinChange(i, -5);
    }
}

void show_simple_message(s32 pic, char *msg) {
    ShowMessage(pic, msg, 0, 0, 0, 0, 0);
    func_800EC9DC();
    CloseMessage();
    func_800EC6EC();
}

void player_sad_animation() {
    func_800F2304(-1, 3, 0); // Sad animation, no loop
}

void player_idle_animation() {
    func_800F2304(-1, -1, 0);
}

// RotatePlayerModel
extern void func_800ED20C(s32 player_index, s32 frames, s32 space_index);

void main() {
    s32 player_index = GetCurrentPlayerIndex();
    struct player *player = GetPlayerStruct(player_index);
    s32 character_str = 0x1C00 + player->character;

    func_800ED20C(-1, 8, BOWSER_MODEL_SPACE); // Face Bowser
    player_idle_animation();
    
    ShowMessage(
        BOWSER_PICTURE,
        bowser_intro_msg,
        character_str, 0, 0, 0, 0
    );
    func_800EC9DC();
    CloseMessage();
    func_800EC6EC();
    
    SleepProcess(30);
    
    if (player_has_any_spike(player_index) && !all_spikes_claimed()) {
        show_simple_message(BOWSER_PICTURE, bowser_take_spikes_msg);
        clear_player_spike_ownership(player_index);
        player_sad_animation();
        SleepProcess(30);
    }
    else {
        show_simple_message(BOWSER_PICTURE, bowser_no_spikes_msg);
        AdjustPlayerCoinsGradual(player_index, 15);
        ShowPlayerCoinChange(player_index, 15);
        SleepProcess(30);
        if (any_other_player_has_coins(player_index)) {
            show_simple_message(BOWSER_PICTURE, bowser_take_coins_msg);
            take_coins_from_other_players(player_index);
            SleepProcess(30);
        }
    }
}
// NAME: CV Claim Spike
// GAMES: MP3_USA
// EXECUTION: Direct
// PARAM: Number|SPIKE_INDEX

/**
 * This event allows a player to claim a "spike" of the coronavirus.
 * They are asked if they want to pay 5 coins to do so.
 * A copy of the player's model is placed on the spike after purchase.
 * When all spikes are claimed, this event triggers conversion of
 * spikes into stars.
 */

#include "ultra64.h"

#define SPIKE_COST 5
#define TUMBLE_PICTURE 0x15
#define MSTAR_PICTURE 0x16

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

/** Used to show any model (called by 800D90C8) */
extern struct object *func_800D912C(
    u32 file, u32 shadowFile, f32 unk1, f32 unk2, void *unk3
);

char *not_enough_coins_msg =
    "\x1A\x1A\x1A\x1A" // Standard padding for picture
    "It takes "
    "\x06" // Blue font
    "5 Coins"
    "\x08" // White Font
    " to claim a"
    "\x0A" // New line
    "\x1A\x1A\x1A\x1A"
    "\x03" // Blue font
    "spike"
    "\x08" // White Font
    "\x85" // .
    "\xFF";

char *already_have_spike_msg =
    "\x1A\x1A\x1A\x1A" // Standard padding for picture
    "Great job\x82 "
    "\x11" // Player Character's Name
    "\xC2" // "!"
    "\x0A" // New line
    "\x1A\x1A\x1A\x1A"
    "You already claimed this "
    "\x0A" // New line
    "\x1A\x1A\x1A\x1A"
    "\x03" // Red font
    "spike"
    "\x08" // White Font
    "\x85" // .
    "\xFF";

char *prompt_msg =
    "\x0B" // Start the message
    "\x1A\x1A\x1A\x1A" // Standard padding for picture
    "\x11" // Player Character's Name
    "\x82" // ,
    " you made it"
    "\xC2" // "!"
    "\x0A" // Newline
    "\x1A\x1A\x1A\x1A"
    "Would you like to claim this"
    "\x0A" // Newline
    "\x1A\x1A\x1A\x1A"
    "\x03" // Red font
    "spike"
    "\x08" // White Font
    " for "
    "\x06" // Blue font
    "5 Coins"
    "\x08" // White Font
    "\xC3" // "?"
    "\x0A" // Newline
    "\x1A\x1A\x1A\x1A\x1A\x1A" // Little more for option indent
    "\x0C" // Start option
    "Claim"
    "\xC2" // !
    "\x0D" // End option
    "\x0A" // Newline
    "\x1A\x1A\x1A\x1A\x1A\x1A"
    "\x0C" // Start option
    "Don\x5Ct Claim"
    "\xC2" // !
    "\x0D" // End option
    "\x0A" // Newline
    "\x1A\x1A\x1A\x1A\x1A\x1A"
    "\x0C" // Start option
    "View Map"
    "\x0D"; // End option

char *begin_star_dist_msg =
    "\x1A\x1A\x1A\x1A" // Standard padding for picture
    "Wow\xC2 That\x5Cs the last"
    "\x0A" // New line
    "\x1A\x1A\x1A\x1A"
    "\x03" // Red font
    "spike"
    "\x08" // White Font
    "\x85" // .
    "\xFF";

char *begin_star_dist_msg =
    "\x1A\x1A\x1A\x1A" // Standard padding for picture
    "Wow\xC2 That\x5Cs the last"
    "\x0A" // New line
    "\x1A\x1A\x1A\x1A"
    "\x03" // Red font
    "spike"
    "\x08" // White Font
    "\x85" // .
    "\xFF";

char *virus_defeat_msg =
    "\x1A\x1A\x1A\x1A" // Standard padding for picture
    "The "
    "\x03" // Red font
    "virus"
    "\x08" // White Font
    " has been subdued\xC2"
    "\x0A" // New line
    "\x1A\x1A\x1A\x1A"
    "Per stay at home orders\x82"
    "\x0A" // New line
    "\x1A\x1A\x1A\x1A"
    "the game must end soon\x85"
    "\x0A" // New line
    "\x1A\x1A\x1A\x1A"
    "This will be the last turn\x85"
//    "Let\x5Cs see the results"
//    "\x85" // .
    "\xFF";

char *virus_tough_msg =
    "\x1A\x1A\x1A\x1A" // Standard padding for picture
    "This "
    "\x03" // Red font
    "virus"
    "\x08" // White Font
    " is tough\xC2"
    "\x0A" // New line
    "\x1A\x1A\x1A\x1A"
    "Let\x5Cs try another round"
    "\x85" // .
    "\xFF";

char *virus_already_defeated_msg =
    "\x1A\x1A\x1A\x1A" // Standard padding for picture
    "The "
    "\x03" // Red font
    "virus"
    "\x08" // White Font
    " has been defeated"
    "\x0A" // New line
    "\x1A\x1A\x1A\x1A"
    "already\x85 Here\x5Cs "
    "\x06" // Blue font
    "20 Coins"
    "\x08" // White Font
    " though"
    "\xC2" // !
    "\xFF";

extern u8 D_800CD05A; // total_turns
extern u8 D_800CD05B; // current_turns
extern u8 D_800CD067; // current_player_index

/**
 * Stores the owner of each spike, 3 bits per spike.
 * 0: no owner
 * 1-4: player index of owner.
 * Examples: All claimed player 1: 0x09249249
 */
extern u32 D_800CD098;

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

s32 get_times_virus_defeated() {
    return D_800CD098 >> 30;
}

void add_virus_defeat() {
    s32 prevTimes = get_times_virus_defeated();
    prevTimes++;
    
    D_800CD098 &= 0x3FFFFFFF; // Clear prior number.
    D_800CD098 |= prevTimes << 30;
}

s32 game_should_end() {
    s32 defeats = get_times_virus_defeated();
    s32 total_turns = D_800CD05A;
    
    switch (total_turns) {
        case 20: return defeats >= 1;
        case 35: return defeats >= 2;
        case 50: return defeats >= 3;
    }
    return FALSE;
}

extern struct object **D_807A5550;

s16 spike_spaces[] = {
    12, 13, 14, 15, 16, 17, 18, 19, 20, 21
};

u32 get_owner_model_file(s32 character) {
    // Use a player flag pole model from directory 60.
    return 0x003C0000 | (18 + character);
}

struct object *draw_spike_model(u32 model_file, s32 spike_index) {
    struct object *charmodel;
    charmodel = func_800D912C(model_file, 0x00000009, 0.9f, 0.8f, NULL);
    charmodel->unka |= 4; // ?

    struct spacedata *spike_space_data;
    spike_space_data = GetSpaceData(spike_spaces[spike_index]);

    // Move the model to the space's coordinates.
    func_80089A20(&charmodel->coords, &spike_space_data->coords);

    return charmodel;
}

void update_spike_model(struct player *player, s32 spike_index) {
    if (D_807A5550 != NULL) {
        if (D_807A5550[spike_index] != NULL) {
            // Free prior characters model.
            func_800D9B54(D_807A5550[spike_index]);
            D_807A5550[spike_index] = NULL;
        }

        if (player != NULL) {
            s32 model_file = get_owner_model_file(player->character);
            D_807A5550[spike_index] =
                draw_spike_model(model_file, spike_index);
        }
    }
}

void clear_spike_ownership() {
    for (s32 i = 0; i < NUM_SPIKES; i++) {
        set_spike_owner(i, 0);
        update_spike_model(NULL, i);
    }
}

enum FadeTypes {
    FADE_BAR = 0,
    FADE_CIRCLE = 1,
    FADE_STAR = 2,
    FADE_BOWSER = 3,
    FADE_QUESTION = 4,
    FADE_EXCLAMATION = 5,
    FADE_TOAD = 6,
    FADE_KOOPA = 7,
    FADE_GOOMBA = 8,
    FADE_GAME_GUY = 9,
    FADE_TUMBLE = 10,
    FADE_GENERIC = 11,
    FADE_BOO = 12
};

void player_happy_taunt(s32 player_index) {
    struct player *player = GetPlayerStruct(player_index);
    s32 taunt_sound = 0x2BE + player->character;
    PlaySound(taunt_sound);
}

void player_idle_animation() {
    func_800F2304(-1, -1, 0);
}

void show_simple_message(s32 pic, char *msg) {
    ShowMessage(pic, msg, 0, 0, 0, 0, 0);
    func_800EC9DC();
    CloseMessage();
    func_800EC6EC();
}

// Not using this, should I?
void reset_players_to_start() {
    for (s32 i = 0; i < 4; i++) {
        struct player *player = GetPlayerStruct(i);
        player->cur_chain_index = 0;
        player->cur_space_index = 0;
        player->next_chain_index = 0;
        player->next_space_index = 1;
        player->reverse_chain_index = 0;
        player->reverse_space_index = 0;
    }
}

// The intention is to have this take the player to the
// end game results scene, but it doesn't work yet.
void do_game_end() {
    show_simple_message(MSTAR_PICTURE, virus_defeat_msg);

    SleepProcess(10);
    
    // Set current turn to last turn, to end things.
    D_800CD05B = D_800CD05A;
    
    // This is also glitchy.
    // D_800CD067 = 3; // Set to last player to trigger Mini-Game.
    //for (s32 i = 0; i < 4; i++) {
    //    struct player *player = GetPlayerStruct(i);
    //    if (player->turn_color_status == 0) {
    //        player->turn_color_status = 4; // Happening status
    //    }
    //}

    //InitFadeOut(FADE_STAR, 30);
    //while (GetFadeStatus() != 0) {
    //    SleepVProcess();
    //}
    
    // Exit game, switches scene to final results abruptly.
    // FIXME
    //func_80048128(0x4F, 0, 0x4190);
    //func_800F8774();
}

void spike_star_distribution() {
    SleepProcess(60);
    
    show_simple_message(MSTAR_PICTURE, begin_star_dist_msg);
        
    for (s32 i = 0; i < NUM_SPIKES; i++) {
        SleepProcess(30);

        s32 owner_index = get_spike_owner(i);
        if (owner_index == 0) {
            continue; // Shouldn't happen, but check.
        }
        owner_index--; // From 1-4 to 0-3
        
        player_happy_taunt(owner_index);
        
        struct player *player = GetPlayerStruct(owner_index);
        player->stars++;
        PlaySound(0x11B);
    }
    
    SleepProcess(30);
    
    add_virus_defeat();
    if (game_should_end()) {
        do_game_end();
    }
    else {
        show_simple_message(MSTAR_PICTURE, virus_tough_msg);
        clear_spike_ownership();
    }
}

void main() {
    s32 player_index = GetCurrentPlayerIndex();
    struct player *player = GetPlayerStruct(player_index);
    s32 character_str = 0x1C00 + player->character;
    
    // Check if the virus is defeated already.
    if (all_spikes_claimed()) {
        show_simple_message(MSTAR_PICTURE, virus_already_defeated_msg);
        
        // Give 20 coins
        AdjustPlayerCoinsGradual(player_index, 20);
        ShowPlayerCoinChange(player_index, 20);
        SleepProcess(35);
        
        // Celebration
        player_happy_taunt(player_index);
        func_800F2304(-1, 6, 0); // Joy animation
        func_8004ACE0(610, player_index);
        SleepProcess(70);
        return;
    }
    
    // Check if player already owns the spike.
    if ((player_index + 1) == get_spike_owner(SPIKE_INDEX)) {
        ShowMessage(
            TUMBLE_PICTURE,
            already_have_spike_msg,
            character_str, 0, 0, 0, 0
        );
        func_800EC9DC();
        CloseMessage();
        func_800EC6EC();
        return;
    }

    // Check for sufficient coins.
    if (PlayerHasCoins(player_index, SPIKE_COST) == 0) {
        // PlaySound(0x29A); // Toad: Waoh...

        show_simple_message(TUMBLE_PICTURE, not_enough_coins_msg);
        return;
    }

    while (TRUE) {
        // Prompt the player to claim.
        ShowMessage(
            TUMBLE_PICTURE,
            prompt_msg,
            character_str, 0, 0, 0, 0
        );

        // Get the selection, either from the player or CPU.
        // If A0 is a pointer to AI data, AI logic is ran to pick for CPUs.
        // If A0 is 0 or 1, the 0th or 1st option is chosen by CPUs.
        // If A0 is 2, the value of A1 is the CPUs option index choice.
        // In this case, we have CPUs always pick Yes (0)
        s32 choice = GetBasicPromptSelection(2, 0);

        CloseMessage();
        func_800EC6EC();

        switch (choice) {
            case 0:
                set_spike_owner(SPIKE_INDEX, player_index + 1);

                // Take coins
                AdjustPlayerCoinsGradual(player_index, -SPIKE_COST);
                ShowPlayerCoinChange(player_index, -SPIKE_COST);
                SleepProcess(35);

                // Celebration
                player_happy_taunt(player_index);
                func_800F2304(-1, 6, 0); // Joy animation
                func_8004ACE0(610, player_index);

                // Update model
                update_spike_model(player, SPIKE_INDEX);

                SleepProcess(70);
                
                player_idle_animation();
                if (all_spikes_claimed()) {
                    spike_star_distribution();
                }
                return;

            case 1:
                // Just exit.
                return;

            case 2:
                // Let player view the map, then repeat the loop to pick again.
                ViewBoardMap();
                break;
        }
    }
}
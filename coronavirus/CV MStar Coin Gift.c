// NAME: CV MStar Coin Gift
// GAMES: MP3_USA
// EXECUTION: Direct
// PARAM: Space|MSTAR_SPACE

#include "ultra64.h"

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

extern u8 D_800CD05B; // current turn

// RotatePlayerModel
extern void func_800ED20C(s32 player_index, s32 frames, s32 space_index);

void show_simple_message(s32 pic, char *msg) {
    ShowMessage(pic, msg, 0, 0, 0, 0, 0);
    func_800EC9DC();
    CloseMessage();
    func_800EC6EC();
}

s32 has_educated = FALSE;

#define COIN_GIFT 5
#define MSTAR_PICTURE 0x16

char *explain_msg =
    "\x1A\x1A\x1A\x1A" // Standard padding for picture
    "Welcome to the "
    "\x03" // Red font
    "Coronavirus"
    "\x08" // White Font
    "\xC2" // !
    "\x0A" // New line
    "\x1A\x1A\x1A\x1A"
    "Here you\x5Cll work together"
    "\x0A"
    "\x1A\x1A\x1A\x1A"
    "to defeat this nasty foe"
    "\x85" // .
    "\xFF"
    "\x0B" // Feed text
    "\x1A\x1A\x1A\x1A"
    "The "
    "\x03" // Red font
    "virus"
    "\x08" // White Font
    " will be defeated when"
    "\x0A"
    "\x1A\x1A\x1A\x1A"
    "we disable all its "
    "\x03" // Red font
    "spikes"
    "\x08" // White Font
    "\x85" // .
    "\xFF";

// TODO: Why didn't this work as one contiguous string?
char *explain_msg_2 =
    "\x1A\x1A\x1A\x1A"
    "Each player should act"
    "\x0A"
    "\x1A\x1A\x1A\x1A"
    "quickly and try to claim"
    "\x0A"
    "\x1A\x1A\x1A\x1A"
    "as many "
    "\x03" // Red font
    "spikes"
    "\x08" // White Font
    " as they can\x85"
    "\xFF"
    "\x0B" // Feed text
    "\x1A\x1A\x1A\x1A"
    "A "
    "\x07" // Red font
    "star"
    "\x08" // White Font
    " will be rewarded"
    "\x0A"
    "\x1A\x1A\x1A\x1A"
    "for each claimed spike"
    "\x0A"
    "\x1A\x1A\x1A\x1A"
    "after all are claimed\x85"
    "\xFF";

char *explain_msg_3 =
    "\x1A\x1A\x1A\x1A"
    "Good luck\xC2 I\x5Cm"
    "\x0A"
    "\x1A\x1A\x1A\x1A"
    "rooting for you all\xC2"
    "\xFF";

void main() {
    func_800ED20C(-1, 8, MSTAR_SPACE);
    
    // Go on a long spiel about the board on the first pass.
    if (D_800CD05B == 1 && !has_educated) {
        show_simple_message(MSTAR_PICTURE, explain_msg);
        show_simple_message(MSTAR_PICTURE, explain_msg_2);
        show_simple_message(MSTAR_PICTURE, explain_msg_3);
        
        has_educated = TRUE;
    }

    s32 player_index = GetCurrentPlayerIndex();
    struct player *player = GetPlayerStruct(player_index);
    
    // Joy noise
    s32 taunt_sound = 0x2BE + player->character;
    PlaySound(taunt_sound);
    
    // Joy animation
    func_800F2304(player_index, 5, 0);
    
    AdjustPlayerCoinsGradual(player_index, COIN_GIFT);
    ShowPlayerCoinChange(player_index, COIN_GIFT);
    SleepProcess(35);
}
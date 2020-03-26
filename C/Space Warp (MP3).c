// NAME: Space Warp (MP3)
// GAMES: MP3_USA
// EXECUTION: Direct
// PARAM: Space|Destination
// PARAM: Space|PreviousSpace
// PARAM: Space|NextSpace

// This is a C port of Airsola's "Landing Warp" event from MPL.
// https://www.mariopartylegacy.com/forum/index.php?action=downloads;sa=view;down=291

// ====================================================================
//  IMPORTANT: This code is specifically designed to warp a player who
//             lands on a Space into a Space of the same type.
//             This is due to limitations on how warps execute.
//
//             Destination is where the player will warp to.
//             PreviousSpace is the space the player will step into if
//             they go in reverse after the warp.
//             NextSpace is the next space that the player will step
//             into if they roll normally.
// ====================================================================

#include "ultra64.h"

struct coords3d {
    f32 x;
    f32 z;
    f32 y;
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
extern struct spacedata *GetSpaceData(s32 space_index);

char *message = "You\x5Cve been warped to another space\xC2\xFF";

void PlaySadVoice() {
    s32 player_index = GetCurrentPlayerIndex();
    struct player *player = GetPlayerStruct(player_index);
    s32 sad_sound = 0x287 + player->character;
    PlaySound(sad_sound);
}

void main() {
    InitFadeOut(4, 16);
    SleepProcess(16);
    PlaySadVoice();
    SleepVProcess();

    s32 player_index = GetCurrentPlayerIndex();
    SetPlayerOntoChain(player_index, Destination_chain_index, Destination_chain_space_index);
    SetPrevChainAndSpace(player_index, PreviousSpace_chain_index, PreviousSpace_chain_space_index);
    SetNextChainAndSpace(player_index, NextSpace_chain_index, NextSpace_chain_space_index);

    struct spacedata *destspace = GetSpaceData(Destination);
    struct player *player = GetPlayerStruct(player_index);

    player->obj->coords.x = destspace->coords.x;
    player->obj->coords.y = destspace->coords.y + 10.0f;

    // See https://pastebin.com/H1jxNFqr
    PlaySound(0x107); // Warp Block Sound
    SleepProcess(15);
    PlaySound(0x107);
    SleepProcess(15);

    InitFadeIn(4, 16);
    SleepProcess(31);

    ShowMessage(-1, message, 0, 0, 0, 0, 0);
    CloseMessage();
}

// NAME: Give Item (MP3)
// GAMES: MP3_USA
// EXECUTION: Direct

typedef signed char            s8;
typedef unsigned char          u8;
typedef signed short int       s16;
typedef unsigned short int     u16;
typedef signed int             s32;
typedef unsigned int           u32;
typedef float  f32;
typedef double f64;

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

    void *obj; // struct object *
    s8 pad2[1]; // 40
    s16 minigame_star; // 41
    s16 coin_star; // 42
    s8 happening_space_count;
    s8 red_space_count;
    s8 blue_space_count;
    s8 chance_space_count; // 46
    s8 bowser_space_count;
    s8 battle_space_count;
    s8 item_space_count;
    s8 bank_space_count; // 50
    s8 game_guy_space_count; // 51

    s8 pad3[5];
}; // sizeof == 56

extern s16 GetCurrentPlayerIndex();
extern struct player *GetPlayerStruct(s32 player_index);
extern s32 PlayerHasEmptyItemSlot(s32 player_index);

void main() {
    s32 cur_player_index = GetCurrentPlayerIndex();
    s32 open_index = PlayerHasEmptyItemSlot(cur_player_index);
    if (open_index != -1) {
      struct player *player = GetPlayerStruct(cur_player_index);
      player->items[open_index] = 0x0A; // Golden Mushroom
    }
}

// 0x00 = Mushroom
// 0x01 = Skeleton Key
// 0x02 = Poison Mushroom
// 0x03 = Reverse Mushroom
// 0x04 = Cellular Shopper
// 0x05 = Warp Block
// 0x06 = Plunder Chest
// 0x07 = Bowser Phone
// 0x08 = Dueling Glove
// 0x09 = Lucky Lamp
// 0x0A = Golden Mushroom
// 0x0B = Boo Bell
// 0x0C = Boo Repellant
// 0x0D = Bowser Suit
// 0x0E = Magic Lamp
// 0x0F = Koopa Card
// 0x10 = Barter Box
// 0x11 = Lucky Coin
// 0x12 = Wacky Watch

// NAME: Buy Star (Custom) (MP3)
// GAMES: MP3_USA
// EXECUTION: Direct
// PARAM: Number|STAR_COST

// This is a "port" of Airsola's "Buy a Star" event to the C language.
// https://www.mariopartylegacy.com/forum/index.php?action=downloads;sa=view;down=300

#define TOAD_PICTURE 3
#define MAX_STARS 99

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

char price[16];

char *not_enough_coins_msg =
    "\x1A\x1A\x1A\x1A" // Standard padding for picture
    "Oh dear"
    "\x85\x85\x85" // ...
    "\x0A" // New line
    "\x1A\x1A\x1A\x1A"
    "It seems you don\x5Ct have enough coins"
    "\x85\x85\x85" // ...
    "\x0A"
    "\x1A\x1A\x1A\x1A"
    "Maybe next time"
    "\xC2" // !
    "\xFF";

char *too_many_stars_msg =
    "\x1A\x1A\x1A\x1A" // Standard padding for picture
    "Nice to see y"
    "\x3D" // -
    "\x0A" // New line
    "\x1A\x1A\x1A\x1A"
    "Whoa"
    "\xC2\xC2\xC2" // !!!
    " You can\x5Ct carry any more stars\xC2"
    "\x0A"
    "\x1A\x1A\x1A\x1A"
    "I guess I\x5Cll see you later"
    "\xC3" // ?
    "\xFF";

char *declined_msg =
    "\x1A\x1A\x1A\x1A" // Standard padding for picture
    "Wait"
    "\xC2" // !
    " Are you sure"
    "\xC3" // ?
    "\x0A" // New line
    "\x1A\x1A\x1A\x1A"
    "Nobody\x5Cs ever said no before"
    "\x85\x85\x85" // ...
    "\x0A" // New line
    "\x1A\x1A\x1A\x1A"
    "That\x5Cs new"
    "\x85\x85\x85" // ...
    " See ya"
    "\xC2" // !
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
    "Would you like to trade"
    "\x0A" // Newline
    "\x1A\x1A\x1A\x1A"
    "\x06" // Blue font
    "\x12" // Star Price
    " Coins "
    "\x08" // White Font
    "for a "
    "\x07" // Yellow Font
    "Star"
    "\x08" // White Font
    "\xC3" // "?"
    "\x0A" // Newline
    "\x1A\x1A\x1A\x1A\x1A\x1A" // Little more for option indent
    "\x0C" // Start option
    "Of course"
    "\xC2" // !
    "\x0D" // End option
    "\x0A" // Newline
    "\x1A\x1A\x1A\x1A\x1A\x1A"
    "\x0C" // Start option
    "Nope"
    "\xC2" // !
    "\x0D" // End option
    "\x0A" // Newline
    "\x1A\x1A\x1A\x1A\x1A\x1A"
    "\x0C" // Start option
    "View Map"
    "\x0D"; // End option

void main() {
    s32 player_index = GetCurrentPlayerIndex();

    // Check for sufficient coins.
    if (PlayerHasCoins(player_index, STAR_COST) == 0) {
        PlaySound(0x29A); // Toad: Waoh...

        ShowMessage(TOAD_PICTURE, not_enough_coins_msg, 0, 0, 0, 0, 0);
        func_800EC9DC();
        CloseMessage();
        func_800EC6EC();
        return;
    }

    // Edge case: check if the player has too many stars.
    struct player *player = GetPlayerStruct(player_index);
    if (player->stars >= MAX_STARS) {
        PlaySound(0x29A); // Toad: Waoh...

        ShowMessage(TOAD_PICTURE, too_many_stars_msg, 0, 0, 0, 0, 0);
        func_800EC9DC();
        CloseMessage();
        func_800EC6EC();
        return;
    }

    s32 character_str = 0x1C00 + player->character;
    sprintf(price, "%d", STAR_COST);

    while (1) {
        // Prompt the player to buy.
        ShowMessage(
            TOAD_PICTURE,
            prompt_msg,
            character_str, price, 0, 0, 0
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
                // Take coins
                AdjustPlayerCoinsGradual(player_index, -STAR_COST);
                ShowPlayerCoinChange(player_index, -STAR_COST);
                SleepProcess(35);

                // Celebration
                func_8004A520(111); // Play star jingle
                player->stars++;
                func_800F2304(-1, 6, 0);
                func_8004ACE0(610, player_index);
                SleepProcess(60);
                SleepProcess(50);

                // Restore board music
                func_8004A520(GetBoardAudioIndex());
                return;

            case 1:
                // Show decline message and exit
                ShowMessage(TOAD_PICTURE, declined_msg, 0, 0, 0, 0, 0);
                func_800EC9DC();
                CloseMessage();
                func_800EC6EC();
                return;

            case 2:
                // Let player view the map, then repeat the loop to pick again.
                ViewBoardMap();
                break;
        }
    }
}

// NAME: Thwomp Toll (MP3)
// GAMES: MP3_USA
// EXECUTION: Direct
// PARAM: +Number|INITIAL_TOLL
// PARAM: Space|DECLINE_DEST
// PARAM: Space|DECLINE_REVERSE_DEST

// This is a "port" of Airsola's "Buy a Star" event to the C language.
// https://www.mariopartylegacy.com/forum/index.php?action=downloads;sa=view;down=348

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

    u8 status_flags; // 23
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

// Structure declarations for the structs used by the AI logic.
union ai_next {
    struct ai_node *ptr;
    unsigned int decision;
};
struct ai_node {
    unsigned char type;
    unsigned int data;
    union ai_next next;
};

// Example AI logic that makes a random choice.
struct ai_node ai_random_choice[] = {
    { 0, 0x00000000, 0x064C9932 },
};

#define THWOMP_PICTURE 8

extern struct player *GetPlayerStruct(s32 player_index);

// This is the address where we will store the toll amount.
// This is one of a few available addresses that get copied into EEPROM.
extern u32 D_800CD098;

char toll_cost[16];

char *ThwompNotEnoughCoins_Message =
    "\x1A\x1A\x1A\x1A" // Standard padding for picture
    "Not enough coins"
    "\xC3" // "?"
    "\x0A" // New line
    "\x1A\x1A\x1A\x1A"
    "No passing for you then"
    "\xC2" // !
    "\x0A"
    "\x1A\x1A\x1A\x1A"
    "Next time bring "
    "\x05" // Green Font
    "\x11" // String 1
    " coins"
    "\x08" // White Font
    "\xC2" // !
    "\xFF";

char *Thwomp_Prompt =
    "\x0B" // Start the message
    "\x1A\x1A\x1A\x1A" // Standard padding for picture
    "If you pay at least "
    "\x05" // Green Font
    "\x11" // String #1 (ThwompToll)
    " coins"
    "\x08" // White Font
    "\x82" // Comma (,)
    "\x0A" // New line
    "\x1A\x1A\x1A\x1A"
    "I\x5Cll let you pass through"
    "\xC2" // !
    "\x0A" // New line
    "\x1A\x1A\x1A\x1A\x1A\x1A" // Picture + Option Indent
    "\x0C" // Start option
    "Pay"
    "\x0D" // End option
    "\x0A" // New line
    "\x1A\x1A\x1A\x1A\x1A\x1A"
    "\x0C" // Start option
    "Don\x5Ct pay"
    "\x0D" // End option
    "\x0A" // New line
    "\x1A\x1A\x1A\x1A\x1A\x1A"
    "\x0C" // Start option
    "View Map"
    "\x0D";

char *ThwompYes_Message =
    "\x1A\x1A\x1A\x1A" // Standard padding for picture
    "OK"
    "\x85" // .
    "\x0A" // New line
    "\x1A\x1A\x1A\x1A"
    "Go on"
    "\xC2" // !
    "\xFF";

char *ThwompNo_Message =
    "\x1A\x1A\x1A\x1A" // Standard padding for picture
    "No passing for you then"
    "\xC2" // !
    "\xFF";

char *ThwompToll_Message =
    "\x0B" // Start the message
    "\x1A\x1A\x1A\x1A" // Standard padding for picture
    "Choose the amount of"
    "\x0A" // New line
    "\x1A\x1A\x1A\x1A"
    "coins you\x5Cll pay"
    "\x85" // .
    "\x0A" // New line
    "\x0A" // New line
    "\x1A\x1A\x1A\x1A\x1A" // Picture + Option Indent
    "Pay"
    "\x10\x10" // Space 2x (  )
    "\x29" // Coin icon
    "\x20" // ???
    "\x3E" // x (multiply)
    "\x20" // ???
    "\x05" // Green Font
    "\x0F" // ???
    "\x11" // String #1 (Price)
    "\x19" // ???
    "\x0A" // New line
    "\x0A" // New line
    "\x1A\x1A\x1A\x1A\x1A\x1A"
    "\x28" // Control Stick Icon
    "\x85\x85\x85" // ...
    "Choose"
    "\x0A" // New line
    "\x1A\x1A\x1A\x1A\x1A\x1A"
    "\x21" // (A) Button Icon
    "\x85\x85\x85"
    "Confirm"
    "\x0A" // New line
    "\x1A\x1A\x1A\x1A\x1A\x1A"
    "\x22" // (B) Button Icon
    "\x85\x85\x85"
    "Back";

void main() {
    SetBoardPlayerAnimation(-1, -1, 2);

    s32 player_index = GetCurrentPlayerIndex();
    struct player *player = GetPlayerStruct(-1);

    PlaySound(0x1AE); // Thwomp's cry.

    if (D_800CD098 == 0) {
        D_800CD098 = INITIAL_TOLL;
    }
    s32 proposed_payment = D_800CD098;
    sprintf(toll_cost, "%d", proposed_payment);

    if (PlayerHasCoins(player_index, proposed_payment) == 0) {
        ShowMessage(THWOMP_PICTURE, ThwompNotEnoughCoins_Message, toll_cost, 0, 0, 0, 0);
        func_800EC9DC();
        CloseMessage();
        func_800EC6EC();

        SendPlayerBack(player);
        return;
    }

    while (1) {
        ShowMessage(THWOMP_PICTURE, Thwomp_Prompt, toll_cost, 0, 0, 0, 0);

        // Get the selection, either from the player or CPU.
        // If A0 is a pointer to AI data, AI logic is ran to pick for CPUs.
        s32 choice = GetBasicPromptSelection(ai_random_choice, 0);

        CloseMessage();
        func_800EC6EC();

        switch (choice) {
            case 0:
                // Alternative ShowMessage?
                func_800EC92C(THWOMP_PICTURE, ThwompToll_Message, toll_cost, 0, 0, 0, 0);

                s32 isCPU = PlayerIsCPU(-1);
                s32 aPressed = 0;
                s32 bPressed = 0;
                if (!isCPU) {
                    // Give real players opportunity to increase cost.
                    while (!aPressed && !bPressed) {
                        aPressed = ButtonsPressed(player, A_BUTTON);
                        bPressed = ButtonsPressed(player, B_BUTTON);

                        s32 yState = GetJoystickVerticalState(player);
                        if (yState > 0 && proposed_payment < player->coins) {
                            proposed_payment++;
                        }
                        else if (yState < 0 && proposed_payment > D_800CD098) {
                            proposed_payment--;
                        }
                        else {
                            SleepVProcess();
                            continue;
                        }

                        PlaySound(582); // Coin sound

                        // Refresh the amount
                        sprintf(toll_cost, "%d", proposed_payment);
                        func_8005B6BC(2, toll_cost, 0);
                        func_8005B43C(2, ThwompToll_Message, -1, -1);
                        SleepVProcess();
                        SleepProcess(2); // Reduce speed of loop further
                    }
                }

                CloseMessage();
                func_800EC6EC();

                if (isCPU || aPressed) {
                    PlaySound(583); // Cash register sound.
                    SleepProcess(10);

                    // Take coins
                    AdjustPlayerCoinsGradual(player_index, -proposed_payment);
                    ShowPlayerCoinChange(player_index, -proposed_payment);
                    SleepProcess(30);

                    PlaySound(0x1AE); // Thwomp's cry.

                    ShowMessage(THWOMP_PICTURE, ThwompYes_Message, 0, 0, 0, 0, 0);
                    func_800EC9DC();
                    CloseMessage();
                    func_800EC6EC();

                    // New toll minimum for next time
                    D_800CD098 = proposed_payment;
                    return;
                }
                break; // Go back.

            case 1:
                // Show decline message and exit
                ShowMessage(THWOMP_PICTURE, ThwompNo_Message, 0, 0, 0, 0, 0);
                func_800EC9DC();
                CloseMessage();
                func_800EC6EC();

                SendPlayerBack(player);
                return;

            case 2:
                // Let player view the map, then repeat the loop to pick again.
                ViewBoardMap();
                break;
        }
    }
}

// OSContPad is defined in ultra64.h.
// The array of structs in the game is sizeof 8, so we need
// to pad out a bit to access it like an array.
struct OSContPadPadded {
    OSContPad cont;
    u16 padding;
};

// This is the location of the ultra64 controller struct array.
extern struct OSContPadPadded D_800CC3F4[4];

// Returns 1 if the button mask matches the player's controller, 0 otherwise.
s32 ButtonsPressed(struct player *player, u32 buttons) {
    return (D_800CC3F4[player->controller].cont.button & buttons) == buttons;
}

// Returns 1 if Y axis of the joystick has been pushed up enough to be considered "up"
// Returns -1 if it is pushed "down"
// 0 if in a more or less neutral vertical state.
s32 GetJoystickVerticalState(struct player *player) {
    s8 stick_y = D_800CC3F4[player->controller].cont.stick_y;

    // According to docs, -80 <= stick_y <= 80
    if (stick_y < -50) {
        return -1;
    }
    if (stick_y > 60) {
        return 1;
    }
    return 0;
}

void SendPlayerBack(struct player *player) {
    // Send the player back
    if (player->status_flags & 0x80) {
        SetNextChainAndSpace(
            -1,
            DECLINE_REVERSE_DEST_chain_index,
            DECLINE_REVERSE_DEST_chain_space_index
        );
    }
    else {
        SetNextChainAndSpace(
            -1,
            DECLINE_DEST_chain_index,
            DECLINE_DEST_chain_space_index
        );
    }
}

// NAME: Model Viewer
// GAMES: MP1_USA
// EXECUTION: Direct
// PARAM: Space|MODEL_SPACE

// This code demonstrates some 3d model object APIs.

#include "ultra64.h"

struct coords3d {
    f32 x;
    f32 y;
    f32 z;
};

struct spacedata {
    s8 pad[4];
    struct coords3d coords;
    s8 pad2[8];
    void *unk24;
};

struct objecttype {
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

char msg[5];

void main() {
    struct objecttype *model_obj;
    struct spacedata *model_space_data;

    model_space_data = GetSpaceData(MODEL_SPACE);

    // Loop over each model, and show it on the space indicated
    // by the parameter. Shows a message box before showing
    // each model.
    // It might be possible to go past 110, didn't try.
    for (int model_index = 0; model_index <= 110; model_index++) {
        // Some entries crash
        switch (model_index) {
            case 40:
            case 110:
                continue;
        }

        int win_id = CreateTextWindow(30, 42, 19, 2);
        sprintf(msg, "%d\xFF", model_index);
        LoadStringIntoWindow(win_id, msg, -1, -1);
        SetTextCharsPerFrame(win_id, 0);
        ShowTextWindow(win_id);
        WaitForTextConfirmation(win_id);
        HideTextWindow(win_id);

        // Obtain reference to the model.
        model_obj = func_8003DBE0(model_index, NULL);

        model_obj->unka |= 4; // ?

        // This makes the model visible (maybe?)
        func_8004CDCC(model_obj);

        // Move the model to the space's coordinates.
        func_800A0D50(&model_obj->coords, &model_space_data->coords);

        SleepProcess(30); // 1 second pause

        // This cleans up the model.
        func_8003E694(model_obj);
    }
}

// 0: Mario (low-res)
// 1: Luigi
// 2: Peach
// 3: Yoshi
// 4: Wario
// 5: DK
// 6: Bowser
// 7: Toad
// 8: Koopa Troopa
// 9: Boo
// 10: Whomp
// 11: Goomba
// 12: Piranha Plant
// 13: Blue Thwomp
// 14: Black Bob-omb
// 15: Red Bob-omb
// 16: Mecha Fly Guy
// 17: Pisudon (Robot that switches gates in Luigi's Engine Room)
// 18: Cloud with face (Mario Rainbow Castle)
// 19: Lakitu
// 20: Top of castle from Mario Rainbow Castle
// 21: Black Star
// 22: Pipe
// 23: Mario (high-res)
// 24: Luigi
// 25: Peach
// 26: Yoshi
// 27: Wario
// 28: DK
// 29: Mario Pin (from cake)
// 30: Luigi pin
// 31: Peach pin
// 32: Yoshi pin
// 33: Wario pin
// 34: DK pin
// 35: Piranha Plant 2D
// 36: Red Direction Arrow
// 37: Star
// 38: Boulder
// 39: Golden Banana
// 40: (crash)
// 41: 20 coin gate from DK Jungle Adventure
// 42: Box holding Golden Bananas
// 43: Small Wood Crate
// 44: Gray thing?
// 45: Black/white thing?
// 46: Black Bob-omb Cannon
// 47: Bowser Cannon
// 48: Black Bob-omb flag
// 49: Red Bob-omb flag
// 50: Black/Red Bob-omb flag
// 51: Blue spiked gate (Luigi's Engine Room)
// 52: Engine part (Luigi's Engine Room)
// 53: Big skull thing from Bowser's Magma Mountain
// 54: Direction skull face from Bowser's Magma Mountain
// 55: Direction skull face from Bowser's Magma Mountain
// 56: Black ball
// 57: Koopa Troopa (low-res)
// 58: Toad (2D)
// 59: Bowser (low-res)
// 60: Machine that bowser uses in Luigi's Magma Mountain
// 61: Coin
// 62: Golden Banana
// 63: Box holding Golden Bananas
// 64: Star
// 65: Boulder (from sea?)
// 66: Sky boat
// 67: Basic cannon
// 68: Basic Bowser cannon
// 69: Mario Head
// 70: Luigi Head
// 71: Peach Head
// 72: Yoshi Head
// 73: Wario Head
// 74: DK Head
// 75: ?
// 76: Green Warp Effect
// 77: White shine Effect
// 78: White circle Effect
// 79: Magma Effect
// 80: Very low poly Mario
// 81: Very low poly Luigi
// 82: Very low poly Peach
// 83: Very low poly Yoshi
// 84: Very low poly Wario
// 85: Very low poly DK
// 86: Flames
// 87: White cloudy Effect
// 88: Mushroom
// 89: Coin bank (purple, two arrows)
// 90: Coin bank (golden)
// 91: Coin bank (green with star)
// 92: Big fish
// 93: Toad
// 94: Mario (stick model)
// 95: Luigi (stick model)
// 96: Peach (stick model)
// 97: Yoshi (stick model)
// 98: Wario (stick model)
// 99: DK (stick model)
// 100: Rainbow warp Effect
// 101: Pink Yoshi
// 102: Blue Yoshi
// 103: White cloud
// 104: Star?
// 105: Star?
// 106: Hovering Boo
// 107: White shine Effect
// 108: Bowser (high res)
// 109: Boo (high res)
// 110: (crash)

// NAME: CV Display Bowser
// GAMES: MP3_USA
// EXECUTION: Direct
// PARAM: Space|MODEL_SPACE

/**
 * Displays a bowser model and rotates it.
 */

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

struct object *model_obj;

void main() {
    if (model_obj != NULL) {
        return; // Was already loaded.
    }

    struct spacedata *model_space_data;
    model_space_data = GetSpaceData(MODEL_SPACE);

    // Obtain reference to the model.
    model_obj = func_800D90C8(8, NULL);
    
    model_obj->rot1 = 25.0f;
    model_obj->rot3 = 45.0f;

    // Move the model to the space's coordinates.
    func_80089A20(&model_obj->coords, &model_space_data->coords);
    
    model_obj->rot1 = 25.0f;
    model_obj->rot3 = 45.0f;

    model_obj->unka |= 4; // ?
}

// Values for MODEL_NUM:
// 0: Mario (low-res)
// 1: Luigi
// 2: Peach
// 3: Yoshi
// 4: Wario
// 5: DK
// 6: Waluigi
// 7: Daisy
// 8: Bowser
// 9: Toad
// 10: Boo
// 11: Whomp
// 12: Goomba
// 13: Blue Thwomp
// 14: Mario (high-res)
// 15: Luigi
// 16: Peach
// 17: Yoshi
// 18: Wario
// 19: DK
// 20: Waluigi
// 21: Daisy
// 22: Red arrow
// 23: Star (lays flat)
// 24: Toad (2D on board)
// 25: Coin (?)
// 26: Star
// 27: Toad
// 28: Rainbow effect around player
// 29: Koopa Kid (2D on board)
// 30: Koopa bank coin
// 31: Parachute (for goomba?)
// 32: Box with arrows opening
// 33: Chest with arrow opening
// 34: White shining effect
// 35: Mario in Bowser Suit
// 36: Golden Lamp
// 37: Genie of the lamp
// 38: Gate (Chilly Waters)
// 39: Gate (Deep Bloober Sea)
// 40: Gate
// 41: Gate
// 42: Gate
// 43: Gate
// 44: Mecha Fly Guy
// 45: Snowman head sleeping
// 46: Snowman head awake
// 47: Snowball
// 48: Toad's item shop
// 49: Toad's item shop
// 50: Bank building
// 51: Boo coffin
// 52: Green thing ?
// 53: Koopa Troopa
// 54: Ripple effect
// 55: Stars bursting away effect
// 56: Two white stars pulsing effect
// 57: Koopa Kid
// 58: Millenium Star
// 59: Tumble
// 60: Jeanie
// 61: The blue lamp with red icon on it
// 62: Game Guy
// 63: Poison Mushroom
// 64: Reverse Mushroom
// 65: Purple Bowser image
// 66: Green upside down Bowser image
// 67: Game guy coin
// 68: Koopa gold card
// 69: Stars and cloud expanding effect
// 70: Blue shining effect
// 71: Quicksand pit
// 72: Purple pit
// 73: ? (two red arrows?)
// 74: Green cactus
// 75: Green cactus (female)
// 76: Cart to ride on
// 77: Rock spike
// 78: Pink Koopa Kid ball
// 79: (crash)


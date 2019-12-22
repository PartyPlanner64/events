// NAME: Model Viewer (MP3)
// GAMES: MP3_USA
// EXECUTION: Direct
// PARAM: Space|MODEL_SPACE

// This code demonstrates some 3d model object APIs.

#define NULL (void *)0

typedef signed char            s8;
typedef unsigned char          u8;
typedef signed short int       s16;
typedef unsigned short int     u16;
typedef signed int             s32;
typedef unsigned int           u32;
typedef float  f32;
typedef double f64;

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
    // May go beyond 79, haven't checked.
    for (int model_index = 0; model_index <= 78; model_index++) {
        sprintf(msg, "%d\xFF", model_index);
        ShowMessage(-1, msg, 0, 0, 0, 0, 0);
		func_800EC9DC();
        CloseMessage();
        func_800EC6EC();
        
        // Obtain reference to the model.
        model_obj = func_800D90C8(model_index, NULL);
        
        model_obj->unka |= 4; // ?
    
        // This makes the model visible (maybe?)
        func_800ECC54(model_obj);
        
        // Move the model to the space's coordinates.
        func_80089A20(&model_obj->coords, &model_space_data->coords);

        SleepProcess(30); // 1 second pause
        
        // Clean up the model
        func_800D9B54(model_obj);
    }
}

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


// NAME: Model Viewer
// GAMES: MP1_USA
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
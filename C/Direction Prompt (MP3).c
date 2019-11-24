// NAME: Direction Prompt (MP3)
// GAMES: MP3_USA
// EXECUTION: Direct
// PARAM: Space|left_choice
// PARAM: Space|right_choice

// This demonstrates a prompt dialog.
// The player will be asked to choose left or right,
// and the choice will impact the direction they head.

// Note that in order to handle going in reverse,
// additional code would be needed.

// ┌───┐ Ho ho ho!
// │   │ Left or right?
// └───┘   Left
//         Right
//         View Map
char *promptMessage = "\x0B" // Start the message
    "\x1A\x1A\x1A\x1A" // Standard padding for picture
    "Ho ho ho"
    "\xC2" // !
    "\n" // Newline
    "\x1A\x1A\x1A\x1A"
    "Left or right"
    "\xC3" // ?
    "\n"
    "\x1A\x1A\x1A\x1A\x1A\x1A" // Little more for option indent
    "\x0C" // Start option
    "Left"
    "\x0D" // End option
    "\n"
    "\x1A\x1A\x1A\x1A\x1A\x1A"
    "\x0C"
    "Right"
    "\x0D"
    "\n"
    "\x1A\x1A\x1A\x1A\x1A\x1A"
    "\x0C"
    "View Map"
    "\x0D";

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


void main() {
	PlaySound(345); // "Ho ho ho!"
    
    int directionChosen = 0;
    
    while (!directionChosen) {
        ShowMessage(0x16, promptMessage, 0, 0, 0, 0, 0);
    
        // Get the selection, either from the player or CPU.
        // If A0 is a pointer to AI data, AI is ran to pick
        // for CPUs.
        // If A0 is 0 or 1, the 0th or 1st option is chosen
        // by CPUs.
        // If A0 is 2, then the value of A1 is the CPUs
        // choice (an index value).
        int dir = GetBasicPromptSelection(ai_random_choice, 0);
    
        // Obligatory message box closing/cleanup calls.
        func_800EC6C8();
        func_800EC6EC();

        switch (dir) {
            case 0: // Left
                directionChosen = 1;
                SetNextChainAndSpace(-1,
                    left_choice_chain_index,
                    left_choice_chain_space_index);
                break;

            case 1: // Right
				directionChosen = 1;
                SetNextChainAndSpace(-1,
                    right_choice_chain_index,
                    right_choice_chain_space_index);
                break;

            case 2: // View Map
                ViewBoardMap();
                break;
        }
    }
}

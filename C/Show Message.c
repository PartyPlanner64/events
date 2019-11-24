// NAME: Show Message
// GAMES: MP1_USA
// EXECUTION: Direct

char *message = "Hello World"
    "\xFF";

void main() {
    // Upper left X coordinate
    // Upper left Y coordinate
    // Characters on X axis
    // Characters on Y axis
    int win_id = CreateTextWindow(30, 42, 19, 4);

    LoadStringIntoWindow(win_id, message, -1, -1);

    // 0 frames between each character output.
    SetTextCharsPerFrame(win_id, 0);

    ShowTextWindow(win_id);
    WaitForTextConfirmation(win_id);
    HideTextWindow(win_id);
}

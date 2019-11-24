// NAME: Show Message (MP3)
// GAMES: MP3_USA
// EXECUTION: Direct

char *message = "Hello World" "\xFF";

void main() {
    // First argument is character image (-1 for none).
	ShowMessage(-1, message, 0, 0);
    
    // The rest here perform the "wait for confirmation"
	// and messagebox teardown.
    func_800EC9DC();
    CloseMessage();
    func_800EC6EC();
}

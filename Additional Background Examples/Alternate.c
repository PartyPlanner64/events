extern unsigned char current_turn;

// This uses the main background on odd turns
// and the additional background on even turns.
int PickBackground() {
    if (current_turn & 1 == 0) {
      return ADDITIONAL_BG_1;
    }
    else {
      return DEFAULT_BG;
    }
}

// NAME: Give Random Coins in Range
// GAMES: MP1_USA,MP2_USA,MP3_USA
// EXECUTION: Direct
// PARAM: +Number|MAX_COINS

// Gives the current player 
void main() {
    int numCoins = GetRandomByte() % MAX_COINS;
    if (numCoins == 0) {
        numCoins = 1; // Give at least one.
    }

    int currentPlayerIndex = GetCurrentPlayerIndex();

    AdjustPlayerCoinsGradual(currentPlayerIndex, numCoins);
    ShowPlayerCoinChange(currentPlayerIndex, numCoins);

    // Sleep for 30 frames to let the coin change take effect.
    SleepProcess(30);
}

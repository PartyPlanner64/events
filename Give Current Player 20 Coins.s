; NAME: Give Current Player 20 Coins
; GAMES: MP1_USA
; EXECUTION: Direct

ADDIU SP SP -4
SW RA 0(SP)

; Get the current player index
JAL GetCurrentPlayerIndex
NOP

; Give the player 20 coins
ADDIU A0 V0 0
ADDIU A1 A1 20
JAL AdjustPlayerCoins
NOP

LW RA 0(SP)
JR RA
ADDIU SP SP 4
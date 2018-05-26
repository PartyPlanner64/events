; NAME: Give Coins P1
; GAMES: MP1_USA
; EXECUTION: Direct

ADDIU SP SP -4
SW RA 0(SP)

; Give the first player 99 coins
ADDIU A0 A0 0
ADDIU A1 A1 99
JAL AdjustPlayerCoins
NOP

LW RA 0(SP)
JR RA
ADDIU SP SP 4
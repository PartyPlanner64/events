; NAME: Give Current Player Coins
; GAMES: MP3_USA,MP1_USA,MP2_USA
; EXECUTION: Direct
; PARAM: Number|coins

ADDIU SP SP -4
SW RA 0(SP)

; Give the player the coins
JAL GetCurrentPlayerIndex
NOP
ADDIU A0 V0 0
JAL AdjustPlayerCoinsGradual
ADDIU A1 R0 coins

; Display the coin change by the player stats
JAL GetCurrentPlayerIndex
NOP
ADDIU A0 V0 0
JAL ShowPlayerCoinChange
ADDIU A1 R0 coins

; Sleep for 30 frames to let coin change take effect
JAL SleepProcess 
ADDIU A0 R0 30

LW RA 0(SP)
JR RA
ADDIU SP SP 4

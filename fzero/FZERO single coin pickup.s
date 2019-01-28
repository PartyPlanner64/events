; NAME: FZERO single coin pickup
; GAMES: MP3_USA
; EXECUTION: Direct

ADDIU SP SP -8
SW RA 4(SP)
SW S0 0(SP)

JAL RNGPercentChance
ADDIU A0 R0 50 ; 50% chance
; True/false bit was returned in V0

BEQ V0 R0 exit
NOP

JAL GetCurrentPlayerIndex
NOP
ADDIU A0 V0 0
JAL AdjustPlayerCoins
ADDIU A1 R0 1 ; +1 coin

JAL PlaySound
ADDIU A0 R0 0x106 ; Coin add sound

exit:
LW RA 4(SP)
LW S0 0(SP)
JR RA
ADDIU SP SP 8

; NAME: FZERO single coin pickup
; GAMES: MP3_USA
; EXECUTION: Direct

ADDIU SP SP -8
SW RA 4(SP)
SW S0 0(SP)

JAL guRandom ; Generates random 32-bit int
NOP
; Random int was returned in V0

; 50% chance of either outcome
.definelabel halfOf32Bit,0x1FFFFFFF
LUI T0 hi(halfOf32Bit)
ADDIU T0 T0 lo(halfOf32Bit)
SLT S0 V0 T0 ; is V0 < 0x1FFFFFFF ?
NOP

BEQ S0 R0 exit
NOP

JAL GetCurrentPlayerIndex
NOP

ADDIU A0 V0 0
ADDIU A1 A1 1 ; +1 coin
JAL AdjustPlayerCoins
NOP

ADDIU A0 R0 0x106 ; Coin add sound
JAL PlaySound
NOP

exit:
LW RA 4(SP)
LW S0 0(SP)
JR RA
ADDIU SP SP 8

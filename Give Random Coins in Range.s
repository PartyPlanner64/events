; NAME: Give Random Coins in Range
; GAMES: MP1_USA,MP2_USA,MP3_USA
; EXECUTION: Direct

ADDIU SP SP -8
SW RA 4(SP)
SW S0 0(SP)

.definelabel MAX_COINS,20

JAL guRandom ; Generates random 32-bit int
NOP
; Random int was returned in V0

; Ensure it won't cause divide-by-zero
BNE V0 R0 divide
NOP
ADDIU V0 R0 1

divide:
ADDIU T0 R0 MAX_COINS
DIVU V0 T0 ; V0 / T0
MFHI S0 ; HI contains the remainder, pull that into S0

; Get the current player index
JAL GetCurrentPlayerIndex
NOP

; Give the player the coins
ADDIU A0 V0 0
ADDU A1 S0 R0
JAL AdjustPlayerCoins
NOP

LW RA 4(SP)
LW S0 0(SP)
JR RA
ADDIU SP SP 8
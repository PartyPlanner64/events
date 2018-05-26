; NAME: Draw Debug Text
; GAMES: MP1_USA
; EXECUTION: Direct

ADDIU SP SP -4
SW RA 0(SP)

ADDIU A0 R0 5
ADDIU A1 R0 5
LUI A2 hi(message)
ADDIU A2 A2 lo(message)

JAL DrawDebugText
NOP

LW RA 0x000(SP)
JR RA
ADDIU SP SP 4

.fill 4

message:
.asciiz "TEST test"

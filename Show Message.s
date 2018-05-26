; NAME: Show Message
; GAMES: MP1_USA
; EXECUTION: Direct

ADDIU SP SP -16 ; Including 4 byte pad
SW RA 8(SP)
SW S1 4(SP)
SW S0 0(SP)

ADDIU A0 R0 30 ; Upper left X coordinate
ADDIU A1 R0 42 ; Upper left Y coordinate
ADDIU A2 R0 19 ; Characters on X axis
JAL CreateTextWindow
ADDIU A3 R0 4 ; Characters on Y axis

ADDU S1 V0 R0 ; Save window handle (V0) into S0 and S1
SLL S0 S1 0x10
SRA S0 S0 0x10

ADDU A0 S0 R0 ; Window handle

LUI A1 hi(message)
ADDIU A1 A1 lo(message)
;ADDIU A1 R0 0x0247 ; String index
ADDIU A2 R0 -1
JAL LoadStringIntoWindow
ADDIU A3 R0 -1

ADDU A0 S0 R0
JAL SetTextCharsPerFrame ; 0 frames between characters
ADDU A1 R0 R0

JAL ShowTextWindow
ADDU A0 S1 R0

JAL WaitForTextConfirmation
ADDU A0 S1 R0

JAL HideTextWindow
ADDU A0 S1 R0

LW RA 8(SP)
LW S1 4(SP)
LW S0 0(SP)
JR RA
ADDIU SP SP 16

.align 16
message:
.ascii "Hello World"
.byte 0xFF,0 ; FF=Pause

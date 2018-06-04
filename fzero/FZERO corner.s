; NAME: FZERO corner
; GAMES: MP3_USA
; EXECUTION: Direct

ADDIU SP SP -40 ; Including 4 byte pad
SW RA 32(SP)
SW S0 28(SP)

JAL guRandom ; Generates random 32-bit int
NOP
; Random int was returned in V0

; 50% chance of either outcome
.definelabel halfOf32Bit,0x1FFFFFFF
LUI T0 hi(halfOf32Bit)
ADDIU T0 T0 lo(halfOf32Bit)
SLT S0 V0 T0 ; is V0 < 0x1FFFFFFF ?
NOP

BEQ S0 R0 setupsafemessage
NOP

setupslipmessage:
LUI A1 hi(slippedMessage)
ADDIU A1 A1 lo(slippedMessage)
J showmessage
NOP

setupsafemessage:
LUI A1 hi(safeMessage)
ADDIU A1 A1 lo(safeMessage)

showmessage:
SW R0 16(SP) ; A4
SW R0 20(SP) ; A5
SW R0 24(SP) ; A6
ADDI A0 R0 -1 ; Character image (-1 for none)
ADDU A2 R0 R0
JAL 0x800EC8EC ; ShowMessage
ADDU A3 R0 R0

; The rest here perform the "wait for confirmation"
; and probably messagebox teardown.
JAL 0x800EC9DC
NOP
JAL 0x800EC6C8
NOP
JAL 0x800EC6EC
NOP

BEQ S0 R0 exit ; They're safe, so just exit.

ADDIU A0 R0 0x109 ; Coin loss sound
JAL PlaySound
NOP

subtractcoins:
JAL GetCurrentPlayerIndex
NOP
ADDIU A0 V0 0
ADDI A1 R0 -5
JAL AdjustPlayerCoins
NOP

exit:
LW RA 32(SP)
LW S0 28(SP)
JR RA
ADDIU SP SP 40

.align 16
slippedMessage:
.ascii "You slipped on the corner and lost "
.byte 0x03 ; Red
.ascii "5 coins"
.byte 0x02 ; Back to default color
.byte 0xC2 ; !
.byte 0xFF,0 ; FF=Pause

.align 16
safeMessage:
.ascii "You slipped on the corner but recovered"
.byte 0x85 ; Period .
.byte 0xFF,0 ; FF=Pause

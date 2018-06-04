; NAME: FZERO explain coin slide
; GAMES: MP3_USA
; EXECUTION: Direct

ADDIU SP SP -32
SW RA 28(SP)

SW R0 16(SP) ; A4
SW R0 20(SP) ; A5
SW R0 24(SP) ; A6
ADDI A0 R0 -1 ; Character image (-1 for none)
LUI A1 hi(message)
ADDIU A1 A1 lo(message)
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

LW RA 28(SP)
JR RA
ADDIU SP SP 32

.align 16
message:
.byte 0x07 ; Yellow font
.ascii "Coins"
.byte 0x08 ; White font
.ascii " are often found on this path"
.byte 0x85 ; Period
.byte 0x0A ; \n
.ascii "This could be your lucky day"
.byte 0xC2 ; !
.byte 0xFF,0 ; FF=Pause
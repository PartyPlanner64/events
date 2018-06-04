; NAME: FZERO give mushroom
; GAMES: MP3_USA
; EXECUTION: Direct

ADDIU SP SP -48 ; Including 4 byte pad
SW RA 40(SP)
SW S2 36(SP)
SW S1 32(SP)
SW S0 28(SP)

JAL GetCurrentPlayerIndex
NOP

; Figure out which player item slots to fill
ADDU S0 R0 R0

LUI S1 hi(p1_item1)
ADDIU S1 S1 lo(p1_item1)

BEQ S0 V0 determineslot
NOP
ADDIU S0 S0 1

LUI S1 hi(p2_item1)
ADDIU S1 S1 lo(p2_item1)

BEQ S0 V0 determineslot
NOP
ADDIU S0 S0 1

LUI S1 hi(p3_item1)
ADDIU S1 S1 lo(p3_item1)

BEQ S0 V0 determineslot
NOP
ADDIU S0 S0 1

LUI S1 hi(p4_item1)
ADDIU S1 S1 lo(p4_item1)

; Figure out if the current player can even fit the item
determineslot:
ADDIU S2 R0 0xFF
LBU S0 0(S1)
BEQ S0 S2 givemushroom ; First slot empty (0xFF)
NOP
ADDIU S1 S1 1
LBU S0 0(S1)
BEQ S0 S2 givemushroom
NOP
ADDIU S1 S1 1
LBU S0 0(S1)
BEQ S0 S2 givemushroom
NOP

; If we reached here, the player has too many items.
SW R0 16(SP) ; A4
SW R0 20(SP) ; A5
SW R0 24(SP) ; A6
ADDI A0 R0 -1 ; Character image (-1 for none)
LUI A1 hi(cannotGiveItemMsg)
ADDIU A1 A1 lo(cannotGiveItemMsg)
ADDU A2 R0 R0
JAL 0x800EC8EC ; ShowMessage
ADDU A3 R0 R0

JAL 0x800EC9DC
NOP
JAL 0x800EC6C8
NOP
JAL 0x800EC6EC
NOP
J exit
NOP

givemushroom:
SW R0 16(SP) ; A4
SW R0 20(SP) ; A5
SW R0 24(SP) ; A6
ADDI A0 R0 -1 ; Character image (-1 for none)
LUI A1 hi(gotSuperMushroomMsg)
ADDIU A1 A1 lo(gotSuperMushroomMsg)
ADDU A2 R0 R0
JAL 0x800EC8EC ; ShowMessage
ADDU A3 R0 R0

JAL 0x800EC9DC
NOP
JAL 0x800EC6C8
NOP
JAL 0x800EC6EC
NOP

ADDIU S2 R0 0x0A ; Golden Mushroom value
SB S2 0(S1)

JAL PlaySound
ADDIU A0 R0 0x4FC ; intense chance time hit sound

exit:
LW RA 40(SP)
LW S2 36(SP)
LW S1 32(SP)
LW S0 28(SP)
JR RA
ADDIU SP SP 48


.align 16
cannotGiveItemMsg:
.ascii "You found an "
.byte 0x06 ; yellow
.ascii "interesting item"
.byte 0x08 ; white
.byte 0x82 ; comma
.byte 0x0A ; \n
.ascii "but you could not hold it"
.byte 0x85 ; .
.byte 0xFF,0 ; FF=Pause

.align 16
gotSuperMushroomMsg:
.ascii "You found a "
.byte 0x06 ; yellow
.ascii "Golden Mushroom"
.byte 0x08 ; white
.byte 0x85 ; .
.byte 0x0A ; \n
.ascii "Make it to the finish"
.byte 0xC2 ; !
.byte 0xFF,0 ; FF=Pause
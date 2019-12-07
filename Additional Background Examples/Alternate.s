; This uses the main background on odd turns
; and the additional background on even turns.
ADDIU SP SP -4
SW RA 0(SP)

; Load the current turn number into T0
LUI T0 hi(current_turn)
ADDIU T0 T0 lo(current_turn)
LBU T0 0(T0)

; T0 = T0 & 1
; In other words, T0 is now true if the current turn is odd.
ANDI T0 T0 1

BEQZ T0 usealternate
NOP

LI V0 DEFAULT_BG
J exit
NOP

usealternate:
LI V0 ADDITIONAL_BG_1

exit:
LW RA 0(SP)
JR RA
ADDIU SP SP 4
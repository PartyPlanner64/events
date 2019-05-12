; NAME: Direction Prompt
; GAMES: MP3_USA
; EXECUTION: Direct
; PARAM: Space|left_choice
; PARAM: Space|right_choice

; This demonstrates a prompt dialog.
; The player will be asked to choose left or right,
; and the choice will impact the direction they head.

; Note that in order to handle going in reverse,
; additional code would be needed.

ADDIU SP SP -40
SW RA 32(SP)
SW S0 28(SP)

JAL PlaySound
LI A0 345 ; "Ho ho ho!"

SW R0 16(SP) ; A4
SW R0 20(SP) ; A5
SW R0 24(SP) ; A6
LI A0 0x16 ; Character image (Millennium Star)
LUI A1 hi(promptMessage)
ADDIU A1 A1 lo(promptMessage)
ADDU A2 R0 R0
JAL 0x800EC8EC ; ShowMessage
ADDU A3 R0 R0

; Get the selection, either from the player or CPU.
; If A0 is a pointer to AI data, AI logic is ran to pick for CPUs.
; If A0 is 0 or 1, the 0th or 1st option is chosen by CPUs.
; If A0 is 2, then the value of A1 is the CPUs option index choice.
LUI A0 hi(ai_random_choice)
ADDIU A0 A0 lo(ai_random_choice)
JAL GetBasicPromptSelection
ADDU A1 R0 R0
MOVE S0 V0 ; S0 now has the chosen option index

; Obligatory message box closing/cleanup calls.
JAL 0x800EC6C8
NOP
JAL 0x800EC6EC
NOP

; Change the player's destination based on the choice.
LI A0 0
BEQ S0 A0 go_left
NOP
LI A0 1
BEQ S0 A0 go_right
NOP
; Should never reach right here. If so, fall into go_left.

go_left:
LI A0 -1
LI A1 left_choice_chain_index
LI A2 left_choice_chain_space_index
JAL SetNextChainAndSpace
NOP
J exit
NOP

go_right:
LI A0 -1
LI A1 right_choice_chain_index
LI A2 right_choice_chain_space_index
JAL SetNextChainAndSpace
NOP

exit:
LW RA 32(SP)
LW S0 28(SP)
JR RA
ADDIU SP SP 40

.align 16

; See the CPU Player AI wiki
ai_random_choice:
.word 0x00000000, 0x00000000, 0x064C9932
.align 16

; ┌───┐ Ho ho ho!
; │   │ Left or right?
; └───┘   Left
;         Right
promptMessage:
.byte 0x0B ; Start the message
.byte 0x1A,0x1A,0x1A,0x1A ; Standard padding for picture
.ascii "Ho ho ho"
.byte 0xC2 ; "!"
.byte 0x0A ; Newline
.byte 0x1A,0x1A,0x1A,0x1A
.ascii "Left or right"
.byte 0xC3 ; "?"
.byte 0x0A ; Newline
.byte 0x1A,0x1A,0x1A,0x1A,0x1A,0x1A ; Little more for option indent
.byte 0x0C ; Start option
.ascii "Left"
.byte 0x0D ; End option
.byte 0x0A ; Newline
.byte 0x1A,0x1A,0x1A,0x1A,0x1A,0x1A 
.byte 0x0C ; Start option
.ascii "Right"
.byte 0x0D ; End option
.byte 0
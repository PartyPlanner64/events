; NAME: Give Coins With Message
; GAMES: MP3_USA
; EXECUTION: Direct
; PARAM: Number|coins

; This is a variation on "Adjust Player Coins" that also
; demonstrates how to inject values into messages.
ADDIU SP SP -32
SW RA 28(SP)

; Get character used by current player.
; We're going to put their name in the message.
JAL GetCurrentPlayerIndex
NOP
SLL V1 V0 3
SUBU V1 V1 V0
SLL V0 V1 3
LUI A2 hi(p1_char)
ADDU A2 A2 V0
LBU A2 lo(p1_char)(A2)

; We're also going to put the coin count in the string.
; We need to convert it to a string.
LUI A0 hi(coin_string_loc)
ADDIU A0 A0 lo(coin_string_loc)
LUI A1 hi(percent_d)
ADDIU A1 A1 lo(percent_d)
JAL sprintf
ADDIU A2 R0 coins

; Show a message
SW R0 16(SP) ; A4
SW R0 20(SP) ; A5
SW R0 24(SP) ; A6
ADDI A0 R0 -1 ; Character image (-1 for none)
LUI A1 hi(message)
ADDIU A1 A1 lo(message)
ADDIU A2 A2 0x1c00
LUI A3 hi(coin_string_loc)
ADDIU A3 A3 lo(coin_string_loc)
JAL 0x800EC8EC ; ShowMessage
NOP

JAL 0x800EC9DC
NOP
JAL 0x800EC6C8
NOP
JAL 0x800EC6EC
NOP

; Give the player the coins
JAL GetCurrentPlayerIndex
NOP
ADDIU A0 V0 0
JAL AdjustPlayerCoinsGradual
ADDIU A1 R0 coins

; Display the coin change by the player stats
JAL GetCurrentPlayerIndex
NOP
ADDIU A0 V0 0
JAL ShowPlayerCoinChange
ADDIU A1 R0 coins

; Sleep for 30 frames to let coin change take effect
JAL SleepProcess
ADDIU A0 R0 30

LW RA 28(SP)
JR RA
ADDIU SP SP 32

percent_d:
.asciiz "%d" ; 0x25640000

coin_string_loc:
.fill 8

.align 16
message:
.ascii "Wow "
.byte 0x11 ; First inserted value marker.
.byte 0xC2 ; !
.ascii " You got "
.byte 0x12 ; Second inserted value marker.
.ascii "coins"
.byte 0xC2 ; !
.byte 0xFF,0 ; FF=Pause
; NAME: Play Sound
; GAMES: MP3_USA,MP1_USA
; EXECUTION: Direct

ADDIU SP SP -4
SW RA 0(SP)

; Pass a sound index.
; See the wiki for more details:
; https://github.com/PartyPlanner64/PartyPlanner64/wiki/Sounds
JAL PlaySound
ADDIU A0 R0 0x106

LW RA 0x000(SP)
JR RA
ADDIU SP SP 4
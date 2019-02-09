; NAME: Warp (Fade)
; GAMES: MP3_USA
; EXECUTION: Direct
; PARAM: Space|target_space

; Note: This warp will only work for spaces
; that are not the last space in their chain.
; There are many peculiarities that are hard
; to handle in a single "warp" event.

ADDIU SP SP -4
SW RA 0(SP)

; Fade types:
; I think it caches the first one you use, so repeats
; are probably wrong...
; 0: horizontal bars
; 1: circle
; 2: circle?
; 3: circle?
; 4: !
; 5: !
; 6: !
; 7: !
; 8: !
; 9: Game Guy
; A: Tumble

; Start a fade out
ADDIU A0 R0 1 ; fade type constant
JAL InitFadeOut
ADDIU A1 R0 20 ; duration (frames)

; Wait for the fade out to complete.
wait_fade_out:
JAL SleepVProcess
NOP
JAL GetFadeStatus ; 1=still fading, 0=complete
NOP
ADDIU A0 R0 1
BEQ V0 A0 wait_fade_out
NOP

; Set player at the new location
JAL GetCurrentPlayerIndex
NOP
ADD A0 R0 V0
ADDIU A1 R0 target_space_chain_index
JAL SetPlayerOntoChain
ADDIU A2 R0 target_space_chain_space_index

JAL PlaySound 
ADDIU A0 R0 0x107 ; Warp Block Sound Index

; Start a fade in
ADDIU A0 R0 1 ; fade type constant
JAL InitFadeIn
ADDIU A1 R0 20 ; duration (frames)

; Don't wait for the fade in to complete.
; Otherwise the player movement will be seen.

LW RA 0(SP)
JR RA
ADDIU SP SP 4
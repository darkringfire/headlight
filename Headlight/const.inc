; - Constants ---------------------
.equ    Freq		= 1 ; MHz
.equ    TimeOnMin	= 50 ; ms - time (Off -> Full On) if Delay = 1
.equ    LightStep	= 4
.equ    LightTimer	= Freq * 1000  * LightStep * TimeOnMin / 255
.equ	Timer1ms	= Freq * 1000
.equ    CtlSize		= 3 ; Size of PWM-control data
.equ    CtlNum		= 2 ; Number of PWMs
.if	Freq < 8
	.equ	PWM_Prescale	= 0b001 ; 001 - /1, 010 - /8, 011 - /64, 100 - 256
.else
	.equ	PWM_Prescale	= 0b010 ; 001 - /1, 010 - /8, 011 - /64, 100 - 256
.endif
;---
.equ	LightDelayOn	= 10 ; ms
;---
.equ	LightDelayOff	= 255 ; ms
.equ	DebounceDelay	= 10 ; ms
.equ	DenyDRLDelay	= 50 ; *10 ms
.equ	ActionsNum	= 5 ; stackable actions
.equ	BlinkIndDRLNum	= 5 * 2 ; Num * 2

; - Flags ----------------------
;	Input state
.equ	LowF	 = 0
.equ	FlashF	 = 1
.equ	HeadF	 = 2
.equ	ParkF	 = 3
.equ	ATF	 = 4
.equ	IgnF	 = 5
.equ	DenyDRLF = 6
;	Output state
;.equ	FogF	= 0
.equ	IndF	= 1
;	Action flags
.equ	LightStepF	= 0
.equ	T1msF		= 1
.equ	NeedLightF	= 2
.equ	AllowLightF	= 3

#ifdef _M328PDEF_INC_
	; - In ports ----------------------
	DefInPort Low,   D, 2
	DefInPort Flash, D, 3
	DefInPort Head,  C, 3
	DefInPort Park,  C, 2
	DefInPort AT,    C, 1
	DefInPort Ign,   C, 0

	; - Out ports ---------------------
	DefOCR Low, OCR0A
	DefOCR High, OCR0B

	DefOutPort Low,  D, 5
	DefOutPort High, D, 6

	DefOutPort Lamps, D, 7
;	DefOutPort Fog,   B, 0
	DefOutPort Ind,   B, 5 ; 5 - on board, 1 - release
#endif
#ifdef _TN2313DEF_INC_
	; - In ports ----------------------
	DefInPort Low,   D, 4
	DefInPort Flash, D, 3
	DefInPort Head,  A, 0
	DefInPort Park,  D, 2
	DefInPort AT,    A, 1
	DefInPort Ign,   D, 1

	; - Out ports ---------------------
	DefOCR Low, OCR0B
	DefOCR High, OCR0A

	DefOutPort Low,	  D, 5
	DefOutPort High,  B, 2

	DefOutPort Lamps, B, 0
;	DefOutPort Fog,   B, 0
	DefOutPort Ind,   B, 4
#endif

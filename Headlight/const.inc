; - Constants ---------------------
.equ    Freq		= 16 ; MHz
.equ    TimeOnMin	= 10 ; ms - time (Off -> Full On) if Delay = 1
.equ    LightStep	= 2
.equ    LightTimer	= Freq * 1000  * LightStep * TimeOnMin / 255
.equ	Timer1ms	= Freq * 1000
.equ    CtlSize		= 3 ; Size of PWM-control data
.equ    CtlNum		= 2 ; Number of PWMs
.equ	PWM_Prescale	= 0b010 ; 010 - /8, 011 - /64
.equ	LightDelayOn	= 255 ; ms
.equ	LightDelayOff	= 255 ; ms
.equ	DebounceDelay	= 10 ; ms
.equ	DenyDRLDelay	= 50 ; *10 ms
.equ	ActionsNum	= 4 ; stackable actions

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
.equ	FogF	= 0
.equ	IndF	= 1
;	Action flags
.equ	LightStepF	= 0
.equ	T1msF		= 1
.equ	NeedLightF	= 2
.equ	AllowLightF	= 3

; - In ports ----------------------
DefInPort Low,   D, 2
DefInPort Flash, D, 3
DefInPort Head,  C, 3
DefInPort Park,  C, 2
DefInPort AT,    C, 1
DefInPort Ign,   C, 0

; - Out ports ---------------------

.equ    PWM_DDR		= DDRD
.equ	PWM_Port	= PORTD

DefOCR Low, OCR0A
DefOCR High, OCR0B

DefOutPort Low,  D, 5
DefOutPort High, D, 6

DefOutPort Lamps, D, 7
DefOutPort Fog,   B, 0
DefOutPort Ind,   B, 5 ; 5 - on board, 1 - release

; = macro =========================================
.include "macro.inc"

; = CONSTANTS =====================================
.include "const.inc"

; = RAM ===========================================
.dseg
.include "dseg.inc"

; = EEPROM ========================================
.eseg
.include "eseg.inc"

; = FLASH =========================================
; - Code Section ----------------------------------
.cseg
	rjmp	reset
.include "vectors.inc"
.org	INT_VECTORS_SIZE

.include "interrupts.inc"

.include "sub.inc"
; ================= RESET ===========================
reset:
; Hardware reset
.include "hwinit.inc"
	/*
	; USART
	ldi	R16, High(8)
	uout	UBRR0H, R16
	ldi	R16, Low(8)
	uout	UBRR0L, R16
	ldi	R16, 1<<TXEN0
	uout	UCSR0B, R16
	ldi	R16, 1<<USBS0 | 0b11<<UCSZ00
*/
init:
; Software reset
        ; variables
	clr	R16
	SetState	LowState, R16, R16
	SetState	HighState, R16, R16
	sts		ActionFlags, R16

	ldi	R16, 0x80 ; imposible value - init
	sts	InState, R16
	sts	TempInState, R16
	sts	PrevInState, R16

	ldi	R16, LightDelayOn
	sts	LightDelayCounter, R16

	clr	R16
	sts	DenyDRLCounter, R16
	sts	BlinkIndDRLCounter, R16

	;--- Load Settings from EEPROM
	ldi	ZL, Low(EEP_DrlLevel)
	ldi	ZH, High(EEP_DrlLevel)
	rcall	EEPROM_read
	sts	DRLLevel, R16

	ldi	ZL, Low(EEP_DRLTime)
	ldi	ZH, High(EEP_DRLTime)
	rcall	EEPROM_read
	sts	DRLTime, R16

	ldi	ZL, Low(EEP_HeadTime)
	ldi	ZH, High(EEP_HeadTime)
	rcall	EEPROM_read
	sts	HeadTime, R16

	ldi	ZL, Low(EEP_FlashTime)
	ldi	ZH, High(EEP_FlashTime)
	rcall	EEPROM_read
	sts	FlashTime, R16

	; Action Counters
	PreInitActions
	InitAction	M_GetInState, GetInState, 1
	InitAction	M_DenyDRL, DenyDRL, 10
	InitAction	M_BlinkIndDRL, BlinkIndDRL, 250
	InitAction	M_LightState, LightState, 1
	InitAction	M_LightDelay, LightDelay, 1

        ; Enable interrupts
        sei

; ============ Main cycle =====================
start:
	rcall	THandler

	rjmp    start

; ========== Blink Ind DRL ================
BlinkIndDRL:
	lds	R16, BlinkIndDRLCounter
	tst	R16
	breq	Blink_end
	dec	R16
	sbrc	R16, 0
	sbp	Ind
	sbrs	R16, 0
	cbp	Ind
	sts	BlinkIndDRLCounter, R16
Blink_end:
	ret
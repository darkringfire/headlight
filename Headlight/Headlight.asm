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
	
	; USART
	ldi	R16, High(8)
	uout	UBRR0H, R16
	ldi	R16, Low(8)
	uout	UBRR0L, R16
	ldi	R16, 1<<TXEN0
	uout	UCSR0B, R16
	ldi	R16, 1<<USBS0 | 0b11<<UCSZ00

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

	;---
	.warning "TODO: read time from EEPROM"
	ldi	R16, 5
	sts	DRLLevel, R16
	ldi	R16, 200
	sts	DRLTime, R16
	ldi	R16, 100
	sts	HeadTime, R16
	ldi	R16, 5
	sts	FlashTime, R16
	;---

	; Action Counters
	PreInitActions
	InitAction	M_GetInState, GetInState, 1
	InitAction	M_DenyDRL, DenyDRL, 10
	InitAction	M_LightState, LightState, DebounceDelay
	InitAction	M_LightDelay, LightDelay, 1

        ; Enable interrupts
        sei

; ============ Main cycle =====================
start:
	rcall	THandler

	rjmp    start


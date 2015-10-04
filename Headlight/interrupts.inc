SlowChange:
; IN
;	Z - State address
;	R16 - CurrentValue
;	R17 - Flags
; OUT
;	R16 - New value
;	R17 - Flags
	
	push	R18
	push	R19
	push	R20
	push	R21
	ld	R18, Z+ ; ReqVal
	ld	R19, Z+ ; Delay
	ld	R20, Z+ ; Delay Counter
	ldi	R21, LightStep

	tst	R16
	brne	SetLightNeed
	tst	R18
	brne	SetLightNeed
	rjmp	SlowChange_end
SetLightNeed:
	sbr	R17, 1<<NeedLightF
	sbrs	R17, AllowLightF
	rjmp	Light_not_allowed

	tst	R20
	breq	SlowChange_update
	dec	R20
	rjmp	Store_DelayCounter
SlowChange_update:
	cp	R16, R18
	breq	SlowChange_end
	brcs	Slow_Inc
	; decrease
	sub	R16, R21
	brcs	Slow_Correct ; Overflow
	cp	R16, R18
	brlo	Slow_Correct ; R16 < R18
	rjmp	Slow_reset_counter
Slow_Inc:
	; increase
	add	R16, R21
	brcs	Slow_Correct ; Overflow
	cp	R18, R16 ; !!!
	brsh	Slow_reset_counter ; R18 >= R16
Slow_Correct:
	; correct
	mov	R16, R18
	rjmp	SlowChange_end
Light_not_allowed:
	clr	R16
Slow_reset_counter:
	mov	R20, R19
Store_DelayCounter:
	st	-Z, R20
SlowChange_end:
	pop	R21
	pop	R20
	pop	R19
	pop	R18
	ret

; = Timer 1 - A compare =============
OC1Ahandler:
        push    R16
        in      R16, SREG
        push    R16
	push	R17
        push    ZL
	push	ZH
	
	; set next interrupt
	uin	ZL, OCR1AL
	uin	ZH, OCR1AH
	subi	ZL, Low(-LightTimer)
	sbci	ZH, High(-LightTimer)
	uout	OCR1AH, ZH
	uout	OCR1AL, ZL

	; Load Flags
	lds	R17, ActionFlags
	cbr	R17, 1<<NeedLightF
	;SlowChange Low
	ldi	ZL, Low(LowState)
	ldi	ZH, High(LowState)
	rocr	R16, Low
	rcall	SlowChange
	wocr	Low, R16
	;SlowChange High
	ldi	ZL, Low(HighState)
	ldi	ZH, High(HighState)
	rocr	R16, High
	rcall	SlowChange
	wocr	High, R16

	sts	ActionFlags, R17

	pop	ZH
	pop	ZL
	pop	R17
	pop	R16
        out     SREG, R16
        pop     R16
	reti
; = Timer 1 - B compare =============
OC1Bhandler:
        push    R16
        in      R16, SREG
        push    R16
	push	R17
	
	; set next interrupt
	uin	R16, OCR1BL
	uin	R17, OCR1BH
	subi	R16, Low(-Timer1ms)
	sbci	R17, High(-Timer1ms)
	uout	OCR1BH, R17
	uout	OCR1BL, R16

	lds	R16, ActionFlags
	sbr	R16, 1<<T1msF
	sts	ActionFlags, R16

	pop	R17
        pop     R16
        out     SREG, R16
        pop     R16
	reti

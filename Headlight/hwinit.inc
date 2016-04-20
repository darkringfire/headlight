        ; Ports setup
	; - In ports
	SetInPort1 LowIn
	SetInPort1 FlashIn
	SetInPort1 HeadIn
	SetInPort1 ParkIn
	SetInPort1 ATIn
	SetInPort0 IgnIn

	; - Out ports
	SetOutPort LowOut
	SetOutPort HighOut
	SetOutPort LampsOut
;	SetOutPort FogOut
	SetOutPort IndOut

        ; Timer0 setup
        clr	R16
/*
        uout	LowOut_OCR, R16
        uout	HighOut_OCR, R16
*/
	wocr	Low, R16
	wocr	High, R16

        ldi     R16, 0b10<<COM0A0 | 0b10<<COM0B0 | 0b01<<WGM00 ; WGM: 001 - PWM Phase Correct
        uout    TCCR0A, R16
        ldi     R16, 0<<WGM02 | PWM_Prescale<<CS00
        uout    TCCR0B, R16

        ; Timer1 setup
        ldi     R16, high(LightTimer)
        uout    OCR1AH, R16
        ldi     R16, low(LightTimer)
        uout    OCR1AL, R16

        ldi     R16, high(Timer1ms)
        uout    OCR1BH, R16
        ldi     R16, low(Timer1ms)
        uout    OCR1BL, R16

        ldi     R16, 0b00<<WGM10		; WGM: 0100 - CTC, 0000 - Normal
        uout    TCCR1A, R16
        ldi     R16, 0b00<<WGM12 | 0b001<<CS10
        uout    TCCR1B, R16
#ifdef _M328PDEF_INC_
        ldi     R16, 0<<OCIE0B | 0<<OCIE0A | 0<<TOIE0
        uout    TIMSK0, R16
        ldi     R16, 1<<OCIE1B | 1<<OCIE1A | 0<<TOIE1
        uout    TIMSK1, R16
#endif
#ifdef _TN2313DEF_INC_
        ldi     R16, 0<<OCIE0B | 0<<OCIE0A | 0<<TOIE0 |  1<<OCIE1B | 1<<OCIE1A | 0<<TOIE1
        uout    TIMSK, R16
#endif

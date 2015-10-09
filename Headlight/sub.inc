; ============ EEPROM Read ===============
EEPROM_read:
	; Wait for completion of previous write
	sbic EECR,EEPE
	rjmp EEPROM_read
	; Set up address (r18:r17) in address register
	out EEARH, ZH
	out EEARL, ZL
	; Start eeprom read by writing EERE
	sbi EECR,EERE
	; Read data from Data Register
	in r16,EEDR
	ret

; =========== Pre Init Actions ================
PreInitActions_sub:
	ldi	R16, ActionsNum * 6
	clr	R17
	ldi	ZL, Low(Actions)
	ldi	ZH, High(Actions)
PreInitActions_loop:
	st	Z+, R17
	dec	R16
	brne	PreInitActions_loop
	ldi	R16, ActionsNum
	ret

; ============= Init Action ==================
InitAction_sub:
	tst	R16
	breq	Action_skip
	
	dec	R16
	st	-Z, YH
	st	-Z, YL
	st	-Z, YH
	st	-Z, YL
	st	-Z, XH
	st	-Z, XL
	ret
Action_skip:
	ret

; ============ T1msHandler ===================
THandler:
	cli
	lds	R16, ActionFlags
	sbrc	R16, T1msF
	rjmp	clear_1msF
	sei
	ret
clear_1msF:
	cbr	R16, 1<<T1msF
	sts	ActionFlags, R16
	sei

	ldi	ZL, Low(Actions)
	ldi	ZH, High(Actions)
	ldi	R16, ActionsNum
stack_action:
	ld	R17, Z+
	ld	R18, Z+
	ld	XL, Z+
	ld	XH, Z+
	ld	YL, Z+
	ld	YH, Z+

	sbiw	Y, 1
	brcs	reset_act_cnt
	breq	push_act
	rjmp	stack_action_end
push_act:
	push	R17
	push	R18
reset_act_cnt:
	movw	Y, X
stack_action_end:
	st	-Z, YH
	st	-Z, YL
	adiw	Z, 2
	dec	R16
	brne	stack_action

	ret

; =========== Light Delay ================
LightDelay:
	cli
	lds	R17, ActionFlags
	; Set Need-Allow flags and LightDelay
	sbrs	R17, NeedLightF
	rjmp	N0
	sbrs	R17, AllowLightF
	rjmp	TestLightDelay
	;(Delay = Off)
	ldi	R16, LightDelayOff
	rjmp	StoreLightDelay
N0:
	sbrc	R17, AllowLightF
	rjmp	TestLightDelay
	;(Delay = On)
	ldi	R16, LightDelayOn
	rjmp	StoreLightDelay
TestLightDelay:
	lds	R16, LightDelayCounter
	tst	R16
	brne	DecLightDelay
	;(Allow = Need)
	bst	R17, NeedLightF
	bld	R17, AllowLightF
	rjmp	StoreLightDelay
DecLightDelay:
	dec	R16

StoreLightDelay:
	sts	LightDelayCounter, R16
	sts	ActionFlags, R17
	sei

	;(LightRelayOn if Need or Allow)
	andi	R17, 1<<NeedLightF | 1<<AllowLightF
	breq	DelayLight_Off
	sbp	Lamps
	ret
DelayLight_Off:
	cbp	Lamps
	ret

; =========== Get input state ================
GetInState:
	lds	R17, TempInState
	lds	R18, InState
	andi	R18, 1<<DenyDRLF ; unchangeable flags

	clr	R16

	sbpc	Ign
	sbr	R16, 1<<IgnF

	sbpc	AT
	sbr	R16, 1<<ATF

	sbps	Park
	sbr	R16, 1<<ParkF

	sbps	Head
	sbr	R16, 1<<HeadF

	sbps	Flash
	sbr	R16, 1<<FlashF

	sbps	Low
	sbr	R16, 1<<LowF

	cp	R16, R17
	brne	state_cnt_reset
	lds	R17, InStateCounter
	tst	R17
	breq	get_state_end
	; cnt dec
	dec	R17
	sts	InStateCounter, R17
	brne	get_state_end
	; state update
	or	R16, R18 ; repair unchangeable flags
	sts	InState, R16
	rjmp	get_state_end
state_cnt_reset:
	ldi	R17, DebounceDelay
	sts	TempInState, R16
	sts	InStateCounter, R17
get_state_end:
	ret


; ============== Light state ================
LightState:
	; R16 - Current state
	; R17 - Previous state
	; R18 - Low Level
	; R19 - High Level
	; R20 - Swith speed
	; R21 - Blink Indicator Counter
	clr	R21
	lds	R16, InState
	lds	R17, PrevInState
	cp	R16, R17
	brne	State_select
	ret
State_select:
	sbrc	R16, FlashF
	rjmp	flash_on

	sbrc	R16, HeadF
	rjmp	head_on
	
	sbrc	R16, ParkF
	rjmp	light_off

	sbrs	R16, IgnF
	rjmp	light_off

	sbrs	R16, ATF
	rjmp	light_off
	
	; DRL
	clr	R18
	clr	R19

	sbrc	R16, DenyDRLF
	rjmp	DRL_skip
	ResetActionCounter M_BlinkIndDRL, Z, R19, R20
	lds	R19, DRLLevel
	lds	R20, DRLTime
	ldi	R21, BlinkIndDRLNum
DRL_skip:
	rjmp	prev_flash_check

head_on:
	sbrs	R16, LowF
	rjmp	high_on

	; Low
	ser	R18
	clr	R19
	lds	R20, HeadTime
	rjmp	prev_flash_check

high_on:
	clr	R18
	ser	R19
	lds	R20, HeadTime
	rjmp	prev_flash_check

light_off:
	clr	R18
	clr	R19
	lds	R20, HeadTime
	rjmp	prev_flash_check

prev_flash_check:
	sbrs	R17, FlashF ; prev flash
	rjmp	light_set
	sbrs	R16, LowF
	ret
	lds	R20, FlashTime
	rjmp	light_set

flash_on:
	clr	R18
	ser	R19
	lds	R20, FlashTime
	rjmp	light_set
	
light_set:
	sts	PrevInState, R16
	sts	BlinkIndDRLCounter, R21
	SetState	LowState, R18, R20
	SetState	HighState, R19, R20

	cpi	R19, 0xFF
	breq	HighInd_on
	sbrc	R21, 0
	rjmp	HighInd_on
	cbp	Ind
	rjmp	Check_fog
HighInd_on:
	sbp	Ind
Check_fog:
	sbrc	R16, ParkF
	sbp	Fog
	sbrs	R16, ParkF
	cbp	Fog

	ret

; ============ Deny DRL ===============
DenyDRL:
	lds	R16, DenyDRLCounter
	lds	R17, InState

	; check allow condition
	sbrc	R17, HeadF
	rjmp	DenyDRL_allow
	sbrc	R17, IgnF
	rjmp	DenyDRL_park
DenyDRL_allow:
	cli
	lds	R17, InState
	cbr	R17, 1<<DenyDRLF
	sts	InState, R17
	sei
	clr	R16
	rjmp	DenyDRL_store
DenyDRL_park:
	; check park state
	sbrs	R17, ParkF
	rjmp	DenyDRL_check_delay

	tst	R16
	brne	DenyDRL_dec
	ret	; overtime in Park-ON
DenyDRL_check_delay:
	tst	R16
	breq	DenyDRL_reset
	cpi	R16, DenyDRLDelay
	breq	DenyDRL_end
	cli
	lds	R17, InState
	sbr	R17, 1<<DenyDRLF
	sts	InState, R17
	sei
DenyDRL_reset:
	ldi	R16, DenyDRLDelay
	rjmp	DenyDRL_store
DenyDRL_dec:
	dec	R16
DenyDRL_store:
	sts	DenyDRLCounter, R16
DenyDRL_end:
	ret


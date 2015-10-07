; === Universal OUT ===
.macro    UOUT
  	.if	@0 < 0x40
      	OUT	@0,@1
	.else
      	STS	@0,@1
   	.endif
.endm

; === Universal IN ===
.macro    UIN
  	.if	@1 < 0x40
      	IN	@0,@1
	.else
      	LDS	@0,@1
   	.endif
.endm

; =========== Load immediate Word ===========
.macro	LDIW
	ldi	@0L, Low(@1)
	ldi	@0H, High(@1)
.endm

; ========== Skip Bit in Port Cleared =========
.macro	SBPC
	sbic	@0In_In, @0In_Pin
.endm

; ========== Skip Bit in Port Set =========
.macro	SBPS
	sbis	@0In_In, @0In_Pin
.endm

; ======= Clear Bit in Port =============
.macro	CBP
	cbi	@0Out_Port, @0Out_Pin
.endm

; ======= Clear Bit in Port =============
.macro	SBP
	sbi	@0Out_Port, @0Out_Pin
.endm

; ========= Write OCR ===============
.macro	WOCR
        uout	@0Out_OCR, @1
.endm

; ========= Read OCR ===============
.macro	ROCR
        uin	@0, @1Out_OCR
.endm

; ======== Define In Port ==============
.macro	DefInPort
	.equ    @0In_Port	= PORT@1
	.equ	@0In_In		= PIN@1
	.equ    @0In_DDR	= DDR@1
	.equ    @0In_Pin	= @2
.endm

; ======= Define OCR ===========
.macro	DefOCR
	.equ    @0Out_OCR	= @1
.endm

; ======== Set Out Port ==============
.macro	DefOutPort
	.equ    @0Out_Port	= PORT@1
	.equ	@0Out_In	= PIN@1
	.equ    @0Out_DDR	= DDR@1
	.equ    @0Out_Pin	= @2
.endm

; ====== Setup In Port (NOT pulled up) ======
.macro SetInPort0
        cbi     @0_DDR, @0_Pin
        cbi     @0_Port, @0_Pin
.endm

; ====== Setup In Port (pulled up) ======
.macro SetInPort1
        cbi     @0_DDR, @0_Pin
        sbi     @0_Port, @0_Pin
.endm

; ====== Setup Out Port ======
.macro SetOutPort
	cbi	@0_Port, @0_Pin
        sbi	@0_DDR, @0_Pin
.endm



; =========== Set PWM data via registers ==============
.macro	SetState ; Address, ValueR, DelayR
	ldi	ZH, High(@0)
	ldi	ZL, Low(@0)
	cli
	st	Z+, @1
	st	Z+, @2
	st	Z+, @2
	sei
.endm

;============ Pre Init actions ===================
.macro	PreInitActions
	.set	AddedActions = 0
	rcall	PreInitActions_sub
.endm

; ========== Init stackable action ==================
.macro	InitAction
	.set	AddedActions = AddedActions + 1
	.if	AddedActions > ActionsNum
		.error "Too many actions added!"
	.endif
	;.equ	Action_@0_Sub = @1
	.equ	Action_@0_Addr = Actions + (ActionsNum - AddedActions) * 6
	.equ	Action_@0_Delay = Action_@0_Addr + 2
	.equ	Action_@0_Counter = Action_@0_Delay + 2

	ldi	XL, Low(@1)
	ldi	XH, High(@1)
	ldi	YL, Low(@2)
	ldi	YH, High(@2)
	rcall	InitAction_sub
.endm

; ============ Reset Action Counter ===============
.macro	ResetActionCounter
	ldi	ZL, Low(Action_@0_Delay)
	ldi	ZH, High(Action_@0_Delay)
	ld	R16, Z+
	ld	R17, Z+
	st	Z+, R16
	st	Z+, R17
.endm

; =========== Force Action ==========
.macro	ForceAction
	ldi	ZL, Low(Action_@0_Counter)
	ldi	ZH, High(Action_@0_Counter)
	clr	R16
	st	Z+, R16
	st	Z+, R17
.endm


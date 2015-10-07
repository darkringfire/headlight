; controls: ReqValue:Delay:DelayCounter (3 bytes)
LowState:	.byte 3
HighState:	.byte 3

DRLLevel:	.byte 1
DRLTime:	.byte 1
HeadTime:	.byte 1
FlashTime:	.byte 1

InState:	.byte 1
TempInState:	.byte 1
PrevInState:	.byte 1
InStateCounter:	.byte 1

LightDelayCounter:	.byte 1
DenyDRLCounter:		.byte 1
BlinkIndDRLCounter:	.byte 1

ActionFlags:	.byte 1

Actions:	; Address:Delay:Counter (6 bytes)
		.byte 6 * ActionsNum
Actions_end:


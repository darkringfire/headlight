EEP_DrlLevel:	.db	127	; 0 - off; 255 - full
; Time to switch from 0 to 0xFF
EEP_DRLTime:	.db	1000 / TimeOnMin	; ms
EEP_HeadTime:	.db	300 / TimeOnMin		; ms
EEP_FlashTime:	.db	50 / TimeOnMin		; ms


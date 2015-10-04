.org    INT0addr
        reti
.org    INT1addr
        reti
.org    PCI0addr
        reti
.org    PCI1addr
        reti
.org    PCI2addr
        reti
.org    WDTaddr	
        reti
.org    OC2Aaddr
        reti
.org    OC2Baddr
        reti
.org    OVF2addr
        reti
.org    ICP1addr
        reti
.org    OC1Aaddr
	rjmp    OC1Ahandler
        ;reti
.org    OC1Baddr
	rjmp    OC1Bhandler
        ;reti
.org    OVF1addr
        reti
.org    OC0Aaddr
        reti
.org    OC0Baddr
        reti
.org    OVF0addr
        reti
.org    SPIaddr	
        reti
.org    URXCaddr
        reti
.org    UDREaddr
        reti
.org    UTXCaddr
        reti
.org    ADCCaddr
        reti
.org    ERDYaddr
        reti
.org    ACIaddr	
        reti
.org    TWIaddr	
        reti
.org    SPMRaddr
        reti

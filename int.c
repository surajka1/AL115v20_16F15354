

#int_TIMER2
void  TIMER2_isr(void) 
{
//output_high(LED_BATT);
	return;
}

#int_TIMER0
	void  TIMER0_isr(void) 
{
//output_high(LED_BATT);
return;
}

#int_OSC_FAIL
void  OSC_FAIL_isr(void) 
{
	#ASM
	RESET;
	#ENDASM
}





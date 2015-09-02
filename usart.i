#line 1 "usart.c"
#line 1 "usart.c"





void putsUSART(char* in)
{
	while (*in) 
	{
		while (TXIF == 0) continue;
		TXREG =*in++;
	}
}

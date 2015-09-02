/*****************************************************************

Most Basic USART (RS232 Serial) Communication Support File.
Simple reading and writing of data without using
Interrupts.

BAUD RATE:57600 Bits per Second
CRYSTAL Frequency: 20MHz

Target Chip: PIC18F4520
Target Compiler: HI-TECH C For PIC18 (http://www.htsoft.com/)
Project: MPLAP Project File

Copyright (c) 2008-2010
eXtreme Electronics, India
www.eXtremeElectronics.co.in

*****************************************************************/

#include <htc.h>

#include "usart.h"

void USARTInit()
{
	//Baud Rate = 57600 Bits per Second
	//*** Note: Valid On 20MHz Crystal ONLY ***
	//For other crystal freq calculate new values for SPBRG
	SPBRG=86;

	//TXSTA REG
	TXEN=1;
	BRGH=1;
	
	//RCSTA
	SPEN=1;
	CREN=1;	//Enable Receiver (RX)
	
	//BAUDCON
	BRG16=1;

}

void USARTWriteByte(char ch)
{
	//Wait for TXREG Buffer to become available
	while(!TXIF);

	//Write data
	TXREG=ch;
}

void USARTWriteString(const char *str)
{
	while((*str)!='\0')
	{
		//Wait for TXREG Buffer to become available
		while(!TXIF);

		//Write data
		TXREG=(*str);

		//Next goto char
		str++;
	}
}

/*

Writes a line of text to USART and goes to new line
The new line is Windows style CR/LF pair.

This will work on Hyper Terminal Only NOT on Linux

*/

void USARTWriteLine(const char *ln)
{
	USARTWriteString(ln);
	USARTWriteString("\r\n");
}

void USARTWriteInt(int val,unsigned char field_length)
{
	if(val<0) 
	{
		USARTWriteByte('-');	//Write '-' sign for negative numbers.
		val=(val*(-1));				//Make it positive.
	}

	//Convert Number To String and pump over Tx Channel.
	char str[5]={0,0,0,0,0};
	int i=4,j=0;
	while(val)
	{
		str[i]=val%10;
		val=val/10;
		i--;
	}
	if(field_length>5)
		while(str[j]==0) j++;
	else
		j=5-field_length;
	
	for(i=j;i<5;i++)
	{
		USARTWriteByte('0'+str[i]);
	}
}


unsigned char USARTReadByte()
{
	while(!RCIF);	//Wait for a byte
	
	return RCREG;
}

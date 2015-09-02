/*****************************************************************

Most Basic USART (RS232 Serial) Communication Demo.
Explains simple reading and writing of data without using
Interrupts.

BAUD RATE:57600 Bits per Second
CRYSTAL Frequency: 20MHz

Target Chip: PIC18F4520
Target Compiler: HI-TECH C For PIC18 (http://www.htsoft.com/)
Project: MPLAP Project File

Author: Avinash Gupta
Copyright (c) 2008-2009
eXtreme Electronics, India
www.eXtremeElectronics.co.in

  							NOTICE
						-------------
NO PART OF THIS WORK CAN BE COPIED, DISTRIBUTED OR PUBLISHED WITHOUT A
WRITTEN PERMISSION FROM EXTREME ELECTRONICS INDIA. THE LIBRARY, NOR ANY PART
OF IT CAN BE USED IN COMMERCIAL APPLICATIONS. IT IS INTENDED TO BE USED FOR
HOBBY, LEARNING AND EDUCATIONAL PURPOSE ONLY. IF YOU WANT TO USE THEM IN 
COMMERCIAL APPLICATION PLEASE WRITE TO THE AUTHOR.

*****************************************************************/

#include <htc.h>

#include "usart.h"

//Chip Settings
__CONFIG(1,0x0200);
__CONFIG(2,0X1E1F);
__CONFIG(3,0X8100);
__CONFIG(4,0X00C1);
__CONFIG(5,0XC00F);

void main()
{
	//Initialize the USART
	USARTInit();

	//Write Some line of TEXT
	USARTWriteLine("********************************");
	USARTWriteLine(" ");
	USARTWriteLine("        GOD IS GREAT !!!");
	USARTWriteLine(" ");
	USARTWriteLine("********************************");
	USARTWriteLine("                     -USART Demo");
	USARTWriteLine("  -By eXtreme Electronics, India");
	USARTWriteLine("                 -For PIC18F4520");
	USARTWriteLine(" ");
	USARTWriteLine("Integer Printing Test ...");
	USARTWriteString("A positive integer: ");
	USARTWriteInt(99,255);	//No fixed field lenght i.e. 255
	USARTWriteLine(" ");

	USARTWriteString("A negative integer: ");
	USARTWriteInt(-24,255);  //No fixed field lenght i.e. 255
	USARTWriteLine(" ");

	USARTWriteString("An Integer with fixed field width(5): ");
	USARTWriteInt(782,5);
	USARTWriteLine(" ");

	USARTWriteLine(" ");
	USARTWriteLine(" ");
	
	USARTWriteLine("Please type on PC Keyboard .....");
	USARTWriteLine("Any Character you type will be returned by MCU");
	USARTWriteLine("But enclosed inside < and >");
	USARTWriteLine("Eg. if you press a");
	USARTWriteLine("MCU will return <a>");
	USARTWriteLine("This tests that both Rx and Tx are working OK");

	//Now Read some input
	while(1)
	{
		char data;

		data=USARTReadByte();	//Wait until a byte is available

		//Now Send the same byte but surrounded by < and >
		//like if user type 'a' we will send <a>
		USARTWriteByte('<');
		USARTWriteByte(data);
		USARTWriteByte('>');

	}
}
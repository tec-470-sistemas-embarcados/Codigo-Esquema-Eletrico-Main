#include <p18f4550.h>
#include <adc.h>
#include <delays.h>
#include <timers.h>
#include <stdio.h> 
#include "stdlib.h"
#include "stdio.h"
#include "usart.h"
#include "my_xlcd.h"  

 
#pragma config FOSC = HS //Externalr, port function on RA6, EC used by USB 
#pragma config WDT = OFF //Disable watchdog timer
#pragma config MCLRE = ON  // Habilitando MCLR



#define LEDLUZ PORTBbits.RB5 
#define LEDTEMP PORTBbits.RB1 
#define ESTOURO PORTBbits.RB2
#define IN1 PORTBbits.RB3 
#define portasta PORTBbits.RB4
#define porta PORTAbits.RA2
#define cortina PORTAbits.RA3

//--------------DECLARAÇÕES---------------

unsigned char count = 0;
unsigned int flag = 0;
unsigned char UsartDado = 0;

unsigned int temp_externa=0;
unsigned float lumi_externa=0;
unsigned int DATA=0;
unsigned int canal=0;

unsigned char tempP=0;
unsigned char tempC=0;
unsigned char lumiperc[]=" ";
 
int resultado;

#pragma code receberMensagem = 0x08 // armazena o código abaixo no endereço 0x08
#pragma interrupt ADCInterrupt

void ADCInterrupt()
{


if(INTCONbits.TMR0IF)//timer
	{
	
	


		count ++;
		if(count == 200)
		{				
			ADCON0bits.CHS3=0b0;
			ADCON0bits.CHS2=0b0;
			ADCON0bits.CHS1=0b0;
			ADCON0bits.CHS0=0b0;//clear ADCON0 to select channel 0 (AN0) 
			ADCON0bits.GO_DONE = 1; //manda converter
			
			if(flag==0)flag = 1;
			else flag = 0;
			ESTOURO = flag;
			
			canal = 0;
			count = 0;
			
		}

		if(count == 100)
		{			
			ADCON0bits.CHS3=0b0;
			ADCON0bits.CHS2=0b0;
			ADCON0bits.CHS1=0b0;
			ADCON0bits.CHS0=0b1;	// CANAL 1
				canal=1;
			ADCON0bits.GO_DONE = 1; //manda converter
			
		}
		
		//porta		
		if(porta==0 && tempP==1)
		{
			portasta=~portasta;
		
		/*	//USART caso haja mudança, att tudo
			while(BusyUSART());
		//temperatura e luminosidade ------------------------------
			putsUSART('temp:');//envia string
			putsUSART(temp_externa);//envia string
			putsUSART(';');//envia string			
			putsUSART('lumi:');//envia string
			sprintf("f=%u.%02u",(int)lumi_externa,100*(int)(lumi_externa-(int)lumi_externa) ); //consertar isso (dado float não funfa pra putrs)
			putsUSART(';');//envia string
		//porta------------------------------------------------------
			if(portasta ==0)//porta fechada		
				putsUSART('porta:fech;');//envia string
			else
				putsUSART('porta:aber;');//envia string

		//cortina---------------------------------------------------
			if(IN1 ==0)//cortina funcionando		
				putsUSART('cortina:on;');//envia string
			else //cortina desligada
				putsUSART('cortina:off;');//envia string
		*/
		}
		tempP = porta;

		//cortina
		if(cortina==0 && tempC==1)
		{
			IN1=~IN1;

	/*		//USART caso haja mudança, att tudo
			while(BusyUSART()); 
		//temperatura e luminosidade ------------------------------
			putsUSART('temp:');//envia string
			putsUSART(temp_externa);//envia string
			putsUSART(';');//envia string			
			putsUSART('lumi:');//envia string
			sprintf("f=%u.%02u",(int)lumi_externa,100*(int)(lumi_externa-(int)lumi_externa) ); //consertar isso (dado float não funfa pra putrs)
			putsUSART(';');//envia string
		//porta------------------------------------------------------
			if(portasta ==0)//porta fechada		
				putsUSART('porta:fech;');//envia string
			else
				putsUSART('porta:aber;');//envia string

		//cortina---------------------------------------------------
			if(IN1 ==0)//cortina funcionando		
				putsUSART('cortina:on;');//envia string
			else //cortina desligada
				putsUSART('cortina:off;');//envia string
		*/
		}
		tempC = cortina;

			

		INTCONbits.TMR0IF = 0;
	}	

 if(PIR1bits.ADIF) //se terminou a conversão
 {
  	DATA= (ADRESH <<2) + (ADRESL >>6);

			
		
   PIR1bits.ADIF = 0;
 }
}
 
void main()
{
	

ADC_INT_ENABLE(); //Habilitación de la interrupción AD

	TRISBbits.TRISB0 = 0;  //LED//Set LED Pin data direction to OUTPUT
	TRISBbits.TRISB1 = 0;  //LED//Set LED Pin data direction to OUTPUT
	TRISBbits.TRISB2 = 0;  //LED//Set LED Pin data direction to OUTPUT
	TRISBbits.TRISB3 = 0;  //LED//Set LED Pin data direction to OUTPUT
	TRISBbits.TRISB4 = 0;  //LED//Set LED Pin data direction to OUTPUT
	TRISBbits.TRISB5 = 0;  //LED//Set LED Pin data direction to OUTPUT
	
	ADCON1 = 0b00001101;//VSS,VDD ref. AN0 e AN1 analog only	
	ADCON2 = 0b00001000;//ADCON2 setup: Left justified, Tacq=2Tad, Tad=2*Tosc (or Fosc/2)
	
	PIR1bits.ADIF = 0;
	PIE1bits.ADIE = 1; //habilitar a interrupção do conversor AD

	T0CONbits.TMR0ON = 1;
	T0CONbits.T08BIT = 1;   // Temporizador/contador de 8 BITS
 	INTCONbits.TMR0IF = 0; //flag da interrupção
	INTCONbits.TMR0IE = 1; //habilita a interrupção
	INTCONbits.PEIE = 1;
	INTCONbits.GIE = 1;
	//Set Timer0
    OpenTimer0( TIMER_INT_ON &
                /*T0_16BIT &*/
                T0_SOURCE_INT &
                T0_PS_1_1);

	ADCON0bits.ADON = 1;//Enable A/D module
	ADCON0bits.GO_DONE = 1;

//USART------------------------------------------
//IN1 = 0; //se for 1 cortina desigada
//Define TX como transmissor (saída)
	TRISCbits.TRISC6 = 0; 
	
	//Define RX como receptor (entrada)
	TRISCbits.TRISC7 = 1; 


	 // USART configurações de interrupção
    RCONbits.IPEN   = 1; // desabilita interrupções de alta prioridade 
    INTCONbits.GIE  = 1; // habilita interrupções
    INTCONbits.PEIE = 1; // habilita perifericos
    PIE1bits.RCIE   = 1; // habilita interrupção de recepção serial do USART
	
	//configurando USART-----------------------
	OpenUSART(USART_TX_INT_OFF & 
				USART_RX_INT_OFF & 
				USART_SYNCH_MODE & //modifiquei para sincrono e parece q funfou de buenas
				USART_EIGHT_BIT &
				USART_BRGH_LOW,10);
	
	baudUSART(BAUD_8_BIT_RATE &
	BAUD_AUTO_OFF &
	BAUD_WAKEUP_OFF);

 
 
 //Retardo de 50 Tcy
 Delay10TCYx(5);

OpenXLCD();
	SetDDRamAddr(0x00);   
	putrsXLCD("Inicializando...");


 while(1)
 {
		//display
		OpenXLCD();
		SetDDRamAddr(0x00);   
		putrsXLCD("Temperatura: ");
		putIntXLCD(temp_externa);
			Delay10KTCYx(250);
    		Delay10KTCYx(250);
			Delay10KTCYx(250);

		OpenXLCD();
		SetDDRamAddr(0x00);  
		putrsXLCD("Luminosidade");
		
		
//WriteUSART('a'); 
	
	if(canal==0){
		lumi_externa = (5 * DATA / 1023.0)*10;
		
		if(lumi_externa < 1.20) //nivel 1 de claridade
		{
			LEDLUZ = 1;
			putrsXLCD(">75%");
		}
		else if ( lumi_externa >= 1.20 && lumi_externa < 2.4)
		{
			LEDLUZ = 1;
			putrsXLCD("<75%");
		}
		else if ( lumi_externa >= 2.4 && lumi_externa < 4.8)
		{
			LEDLUZ =0;
			putrsXLCD("<50%");
		}
		else if ( lumi_externa >= 4.8)
		{
			LEDLUZ = 0;
			putrsXLCD("<25%");
		}
			Delay10KTCYx(250);
    		Delay10KTCYx(250);
			Delay10KTCYx(250);
			
	}
	else{

//LEDTEMP = 1;
		temp_externa =( 5 * DATA / 1023.0)*100;
		
		if(temp_externa > 20)
		{
			LEDTEMP = 1;	
		}else
		{
			LEDTEMP = 0;
		}
       
	}	
	
		OpenXLCD();
		SetDDRamAddr(0x00);   
		if(portasta==0)putrsXLCD("Porta Fechada");
		else putrsXLCD("Porta Aberta");		
			Delay10KTCYx(250);
    		Delay10KTCYx(250);
			Delay10KTCYx(250);
		
		OpenXLCD();
		SetDDRamAddr(0x00);   
		if(IN1==1)putrsXLCD("Cortina Off");
		else putrsXLCD("Cortina On");		
			Delay10KTCYx(250);
    		Delay10KTCYx(250);
			Delay10KTCYx(250);

		ESTOURO = flag;


	//-------------------------------------------
	//enviando o caractere pela porta TX
/*	while(BusyUSART()); 
		putsUSART('Temp');//envia string
		putsUSART(temp_externa);//envia string
		putsUSART('Lumi');//envia string
		sprintf("f=%u.%02u",(int)lumi_externa,100*(int)(lumi_externa-(int)lumi_externa) ); //consertar isso (dado float não funfa pra putrs)
*/
	//	USARTWriteString("Hello World !");

	//-------------------------------------------
	//Recebendo o dado--------------------------
	 while(!DataRdyUSART());
		getsUSART(UsartDado,100);
		
		if(UsartDado == 'attinfo') //dado recebido
		{
			while(BusyUSART()); 
		//temperatura e luminosidade ------------------------------
			putsUSART('temp:');//envia string
			putsUSART(temp_externa);//envia string
			putsUSART(';');//envia string			
			putsUSART('lumi:');//envia string
			sprintf("f=%u.%02u",(int)lumi_externa,100*(int)(lumi_externa-(int)lumi_externa) ); //consertar isso (dado float não funfa pra putrs)
			putsUSART(';');//envia string
		//porta------------------------------------------------------
			if(portasta ==0)//porta fechada		
				putsUSART('port:fech;');//envia string
			else
				putsUSART('port:aber;');//envia string

		//cortina---------------------------------------------------
			if(IN1 ==0)//cortina funcionando		
				putsUSART('cortina:on;');//envia string
			else //cortina desligada
				putsUSART('cortina:off;');//envia string
		}	
		else if (UsartDado == 'ligdescort')// liga ou desliga cortina de vento
		{
			IN1 =~IN1; //desliga/liga cortina de vento	
			
		}
		else{}

	//--------------------------------------------
 }

}

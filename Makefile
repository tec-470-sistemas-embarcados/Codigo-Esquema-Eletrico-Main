# MPLAB IDE generated this makefile for use with GNU make.
# Project: embarcados.mcp
# Date: Mon Aug 31 19:19:16 2015

AS = MPASMWIN.exe
CC = mcc18.exe
LD = mplink.exe
AR = mplib.exe
RM = rm

embarcados.cof : main.o
	$(LD) /p18F4550 /l"C:\Program Files (x86)\Microchip\mplabc18\v3.47\lib" "main.o" /u_CRUNTIME /z__MPLAB_BUILD=1 /o"embarcados.cof" /M"embarcados.map" /W

main.o : main.c ../../../../../../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/adc.h ../../../../../../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/delays.h ../../../../../../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/timers.h ../../../../../../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/stdlib.h ../../../../../../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/stdio.h ../../../../../../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/usart.h main.c ../../../../../../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/p18f4550.h ../../../../../../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/pconfig.h ../../../../../../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/p18cxxx.h ../../../../../../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/stdarg.h ../../../../../../../../Program\ Files\ (x86)/Microchip/mplabc18/v3.47/h/stddef.h
	$(CC) -p=18F4550 "main.c" -fo="main.o" -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa-

clean : 
	$(RM) "main.o" "embarcados.cof" "embarcados.hex"


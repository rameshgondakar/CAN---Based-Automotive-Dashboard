/*
 * File:   clcd.c
 * Author: Ramesh
 *
 * Created on 5 May, 2025, 3:56 PM
 */


#include <xc.h>

#include "clcd.h"
void clcd_write(unsigned char byte, unsigned char control_bit)
{
	CLCD_RS = control_bit; //rs
	CLCD_PORT = byte; //portd

	/* Should be atleast 200ns */
	CLCD_EN = HI; //rc2
	CLCD_EN = LO;

	PORT_DIR = INPUT; //trisd7
	CLCD_RW = HI; //1-read, 0-write
	CLCD_RS = INSTRUCTION_COMMAND;

	do
	{
		CLCD_EN = 1;
		CLCD_EN = 0;
	} while (CLCD_BUSY);

	CLCD_RW = LO;
	PORT_DIR = OUTPUT;
}
void init_clcd()
{
	/* Set PortD as output port for CLCD data */
	TRISD = 0x00;
	/* Set PortC as output port for CLCD control */
	TRISC = TRISC & 0xF8; //why output means by giving the control we see the output

	CLCD_RW = LO; // in built macro

	
     /* Startup Time for the CLCD controller */
    __delay_ms(30); // we cant send in float value inside this function
    
    /* The CLCD Startup Sequence */
    clcd_write(EIGHT_BIT_MODE, INSTRUCTION_COMMAND	);
    __delay_us(4100);
    clcd_write(EIGHT_BIT_MODE, INSTRUCTION_COMMAND	);
    __delay_us(100);
    clcd_write(EIGHT_BIT_MODE, INSTRUCTION_COMMAND	);
    __delay_us(1); 
    
    CURSOR_HOME; //predefined macro
   // __delay_us(100);
    TWO_LINE_5x8_MATRIX_8_BIT;
    __delay_us(100);
    CLEAR_DISP_SCREEN;
    __delay_us(500);
    DISP_ON_AND_CURSOR_OFF;
    __delay_us(100);
}

void clcd_print(const unsigned char *data, unsigned char addr)
{
	clcd_write(addr, INSTRUCTION_COMMAND); // adrress will be automtically increaded
	while (*data != '\0') // but data we have to increament 
	{
		clcd_write(*data++, DATA_COMMAND);
	}
}

void clcd_putch(const unsigned char data, unsigned char addr)
{
	clcd_write(addr, INSTRUCTION_COMMAND);
	clcd_write(data, DATA_COMMAND);
}
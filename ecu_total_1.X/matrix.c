/*
 * File:   newmain.c
 * Author: Ramesh
 *
 * Created on 5 May, 2025, 4:28 PM
 */


#include <xc.h>

#include <xc.h>
#include "clcd.h"

void init_matrix_keypad(void)
{
	/* Config PORTB as digital */
	ADCON1 = 0x0F;

	/* Set Rows (RB7 - RB5) as Outputs and Columns (RB4 - RB1) as Inputs */
	TRISB = 0x1E|0X02;//partial intialise means we need use bitwise
    
   
	/* Set PORTB input as pull up for columns */
	RBPU = 0;

	MATRIX_KEYPAD_PORT = MATRIX_KEYPAD_PORT | 0xE0;
}

unsigned char scan_key(void)
{
	ROW1 = LO;
	ROW2 = HI;
	ROW3 = HI;

	if (COL1 == LO)
	{
		return 1;
	}
	
	ROW1 = HI;
	ROW2 = LO;
	ROW3 = HI;

	if (COL1 == LO)
	{
		return 2;
	}


	ROW1 = HI;
	ROW2 = HI;
	ROW3 = LO;
	/* TODO: Why more than 2 times? */
	ROW3 = LO; // why assigning two times means it is hardware issue @@@@@@@@@@

	if (COL1 == LO)
	{
		return 3;
	}


	return 0xFF;
}

unsigned char read_switches(unsigned char detection_type)
{
	static unsigned char once = 1, key;

	if (detection_type == STATE_CHANGE)
	{
		key = scan_key();
		if(key != 0xFF && once  )
		{
			once = 0;
			return key;
		}
		else if(key == 0xFF)
		{
			once = 1;
		}
	}
	else if (detection_type == LEVEL_CHANGE)
	{
		return scan_key();
	}

	return 0xFF;
}

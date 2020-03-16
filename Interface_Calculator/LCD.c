/*
 * LCD.c
 *
 *  Created on: Aug 23, 2019
 *      Author: Newname
 */

#include "LCD.h"
#include "LCD_CFG.h"
#include "../MACL/DIO.h"
#include "util/delay.h"
#include <stdlib.h>
u8 DATA = 0;

/*
	write_operation 	0
	read_operation		1
 	 IR			0
	 DR			1
  */


// INITIALIZATION, always start an LCD with
void LCD_vidInitialize(void)
{
	// Set PORTD, PORTC as output by default
	DIO_vidSetPortxDir(LCD_PORT_DATA, 1);	// PORTC
	DIO_vidSetPortxDir(LCD_PORT_CONTROL, 1);	// PORTD

	// RS and RW = 0
	DIO_vidSetPortBitValue(LCD_PORT_CONTROL, RS_PIN, 0);
	DIO_vidSetPortBitValue(LCD_PORT_CONTROL, RW_PIN, 0);

	// Delay > 30ms
	_delay_ms(30);

	// Function Set, F = 0, DL = 1, N = 1
	LCD_vidInstruction(0X38, write_operation, IR);

	// Delay > 39us
	_delay_us(40);

	// Display ON/OFF Control D = 1, C = 1, B = 0
	LCD_vidInstruction(0X0c, write_operation, IR);

	// Delay > 39us
	_delay_us(40);

	// Display Clear
	LCD_vidInstruction(0x01, write_operation, IR);

	// Delay > 1.53ms
	_delay_ms(2);

	// Entry Mode Set
	LCD_vidInstruction(0x06, write_operation, IR);
	_delay_ms(2);
}


// Send instruction to execute
void LCD_vidInstruction(u8 command, u8 RW_value, u8 RS_value)
{
	// INDICATE WHICH REGISTER
	if(RS_value == IR)
	{
		// RS = 0, indication for Instruction Register
		DIO_vidSetPortBitValue(LCD_PORT_CONTROL, RS_PIN, 0);
	}
	else
	{
		// RS = 1, indication for DATA Register
		DIO_vidSetPortBitValue(LCD_PORT_CONTROL, RS_PIN, 1);
	}

	// INDICATE R/W OPERATION
	if(RW_value == write_operation)
	{
		// R/W = 0, write indication
		DIO_vidSetPortBitValue(LCD_PORT_CONTROL, RW_PIN, 0);
		DIO_vidSetPortValue(LCD_PORT_DATA, command);
	}
	else
	{
		// R/W = 1, read indication
		DIO_vidSetPortBitValue(LCD_PORT_CONTROL, RW_PIN, 1);
		// Change Direction of PORT to input
		DIO_vidSetPortxDir(LCD_PORT_DATA, 0);
	}

	// E = 1, H-to-L pulse
	DIO_vidSetPortBitValue(LCD_PORT_CONTROL, E_PIN, 1);

	// Wait to make enable wide
	_delay_ms(3);

	// READ/WRITE VALUES FROM/TO LCD
	if(RW_value == write_operation);
	else
	{
		// Store the whole PIN_REG value
		DATA = DIO_GetPinValue(LCD_PORT_DATA);
	}

	// EN = 0
	DIO_vidSetPortBitValue(LCD_PORT_CONTROL, E_PIN, 0);

	// Wait to make enable wide
	_delay_ms(3);

	// FORCE Direction of PORT to OUTPUT
	DIO_vidSetPortxDir(LCD_PORT_DATA, 1);
}

// Send Data to display
void LCD_vidData(s16 data)
{

	LCD_vidInstruction(data, write_operation, DR);

}

// Display string from user
u8 LCD_String(s8 *ch)
{
	s8 i;

	// Iterate over string characters
	for(i = 0; ch[i] != '\0'; i++)
	{
		LCD_vidData(ch[i]);
	}

	// Return length of string
	return i;
}


// Shift all Display
void LCD_vidShift(void)
{
	u8 final_location, tmp = 0;

	// READ_FROM IR and assign to GLOBAL VAR. DATA
	LCD_vidInstruction(0, read_operation, IR);

	// to lose the value of BF and get only AC value
	final_location = DATA & 0x0f;

	tmp = 0x0f - final_location + 1 ;

	// shift right
	while(tmp != 0)
	{
		LCD_vidInstruction(0x1f, write_operation, IR);
		_delay_ms(400);
		tmp--;
	}

	tmp = 0x0f - final_location + 1 ;

	// Shift left
	while(tmp != 0 )
	{
		LCD_vidInstruction(0x1b, write_operation, IR);
		_delay_ms(400);
		tmp--;
	}

	// Clear Display
	LCD_vidInstruction(0x01, write_operation, IR);
	_delay_ms(2);

}


// Define a location to write to
void LCD_vidGoTo(u8 x, u8 y)
{
	// Array with 2 possible lines
	u8 location[] = {0x00, 0x40};

	// Configuring the instruction for the location acquired
	u8 address = location[y-1] + x - 1;

	// Just complete the instruction from data_sheet
	address = address + (1<<7);

	// Execute instruction
	LCD_vidInstruction(address, write_operation, IR);
	_delay_ms(100);
}


void LCD_vidPatternCreation(void)
{
	typedef struct
	{
		u8 value;
		u8 location_removed;
		u8 location_present;
	}indication;

	indication array;

	u8 dict[8][8] = {0x40, 0x07, 0x04, 0x04, 0x04, 0x1f, 0x00, 0x00, //Ú
					0x00, 0x00, 0x01, 0x01, 0x01, 0x1f, 0x04, 0x00, //È
					0x00, 0x01, 0x01, 0x01, 0x01, 0x1f, 0x00, 0x00, //Ï
					0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x00, 0x00,	// Ç
					0x01, 0x01, 0x01, 0x01, 0x01, 0x1f, 0x00, 0x00,	// á
					0x06, 0x09, 0x09, 0x09, 0x09, 0x1f, 0x00, 0x00,	// ã
					0x04, 0x04, 0x04, 0x0a, 0x0a, 0x1f, 0x00, 0x00,	//Ø
					0x00, 0x11, 0x11, 0x11, 0x11, 0x1f, 0x04, 0x00	};// È


	u8 address[] = {0x40, 0x48, 0x50, 0x58, 0x60, 0x68, 0x70, 0x78};

	u8 length_buffer = 0, length_ptr = 0, flag_busy = 0;
	u8 index = 0;
	u8 *ptr = "012345647", *pttr, *buffer;
	pttr = ptr;
	u8 k = 0, buff = 0 ;
	buffer = malloc(9* sizeof(u8));

	length_ptr = LCD_String(ptr);
	// Display Clear
	LCD_vidInstruction(0x01, write_operation, IR);

	// Delay > 1.53ms
	_delay_ms(2);

	for(k = 0; k < length_ptr; k++)
	{
		for(u8 xx = k+1; xx < length_ptr; xx++)
		{

			if(ptr[k] == pttr[xx])
			{
				index = k;
				array.location_removed = k;
				array.location_present = xx-1;
				array.value = ptr[k];
				flag_busy = 1;
				index++;
			}
		}
		if(flag_busy)
		{
			flag_busy = 0;
		}
		else
		{

			buffer[buff] = ptr[k];
			buff++;
		}
	}

	buffer[buff] = '\0';
	length_buffer = LCD_String(buffer);
	// Display Clear
	LCD_vidInstruction(0x01, write_operation, IR);

	// Delay > 1.53ms
	_delay_ms(2);
	/////////////////////////////////////////////////////////////////////////////////////////////////
	s8 i = 0, j = 0, tmp = 0;
	for(i = 0; i < length_buffer; i++)
	{
		// Set CGRAM address Instruction
		LCD_vidInstruction(address[i], write_operation, IR);
		_delay_ms(5);

		tmp = buffer[i] % 48;

		for(j = 0; j < 8; j++)
		{
			LCD_vidInstruction(dict[tmp][j], write_operation, DR);
			_delay_ms(5);
		}
	}


	// Display Clear
	LCD_vidInstruction(0x01, write_operation, IR);
	// Delay > 1.53ms
	_delay_ms(4);

	// Set DDRAM address Instruction
	LCD_vidInstruction(0x80, write_operation, IR);
	_delay_ms(4);


	for(j = length_buffer-1 ; j >= 0; j--)
	{
			if((j) == array.location_removed-1 && (j) != 0)
			{
				LCD_vidInstruction(array.location_present, write_operation, DR);
				_delay_ms(10);
			}

		LCD_vidInstruction(j, write_operation, DR);
			_delay_ms(10);
	}

	free(buffer);

}






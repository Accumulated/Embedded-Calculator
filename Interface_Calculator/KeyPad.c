/*
 * KeyPad.c
 *
 *  Created on: Aug 28, 2019
 *      Author: Newname
 */


/*
 * KEY_PAD_2.C
 *
 *  Created on: Aug 27, 2019
 *      Author: Newname
 */
#include "../MACL/DIO.h"
#include "KEY_PAD.h"
#include "../MACL/LinkedList.h"
#include "util/delay.h"
#include "LCD.h"

// Flag1 for input1, flag2 for input2
u8 location = 0;

u8 result = 0, operation = 0;
u8 s_flag = 0, m_flag = 0, d_flag = 0, a_flag = 0;


u8 character[4][4] = { '0', '1', '2', '3',
						'4', '5', '6', '7',
						'8', '9', '+', '-',
						'*', '/', '=', '#'};




// Columns are input, Rows are output
void KeyPad_vidInitialize(void)
{
	// Set low nibble columns are input and pulled up
	DIO_vidSetPinxDir(KEY_PAD_PORT, COL1, INPUT);
	DIO_vidSetPinxDir(KEY_PAD_PORT, COL2, INPUT);
	DIO_vidSetPinxDir(KEY_PAD_PORT, COL3, INPUT);
	DIO_vidSetPinxDir(KEY_PAD_PORT, COL4, INPUT);

	// Set high nibble rows as output
	DIO_vidSetPinxDir(KEY_PAD_PORT, ROW1, OUTPUT);
	DIO_vidSetPinxDir(KEY_PAD_PORT, ROW2, OUTPUT);
	DIO_vidSetPinxDir(KEY_PAD_PORT, ROW3, OUTPUT);
	DIO_vidSetPinxDir(KEY_PAD_PORT, ROW4, OUTPUT);

	// Rows are all  zeros
	DIO_vidSetPortBitValue(KEY_PAD_PORT, ROW1, 0);
	DIO_vidSetPortBitValue(KEY_PAD_PORT, ROW2, 0);
	DIO_vidSetPortBitValue(KEY_PAD_PORT, ROW3, 0);
	DIO_vidSetPortBitValue(KEY_PAD_PORT, ROW4, 0);

	KeyPad_vidKey_Pressed();

}


u8 KeyPad_vidKey_Pressed(void)
{
	while(1)
	{
		// 1st stage Make sure key is released
		do
		{
			// Rows are all  zeros
			DIO_vidSetPortBitValue(KEY_PAD_PORT, ROW1, 0);
			DIO_vidSetPortBitValue(KEY_PAD_PORT, ROW2, 0);
			DIO_vidSetPortBitValue(KEY_PAD_PORT, ROW3, 0);
			DIO_vidSetPortBitValue(KEY_PAD_PORT, ROW4, 0);
		}
		while((DIO_GetPinValue(KEY_PAD_PORT) & 0x0f) != 0x0f);

		// 2nd stage
		do
		{
			do
			{
				// delay for  noise
				_delay_ms(20);
			}
			while((DIO_GetPinValue(KEY_PAD_PORT) & 0x0f) == 0x0f);

			// Delay for De_bounce
			_delay_ms(20);
		}
		while((DIO_GetPinValue(KEY_PAD_PORT) & 0x0f) == 0x0f);


		while(1)
		{
			// 3rd stage
			DIO_vidSetPortBitValue(KEY_PAD_PORT, ROW1, 0);
			DIO_vidSetPortBitValue(KEY_PAD_PORT, ROW2, 1);
			DIO_vidSetPortBitValue(KEY_PAD_PORT, ROW3, 1);
			DIO_vidSetPortBitValue(KEY_PAD_PORT, ROW4, 1);

			if((DIO_GetPinValue(KEY_PAD_PORT) & 0x0f) != 0x0f)
			{
				location = 0;
				break;
			}

			DIO_vidSetPortBitValue(KEY_PAD_PORT, ROW1, 1);
			DIO_vidSetPortBitValue(KEY_PAD_PORT, ROW2, 0);
			DIO_vidSetPortBitValue(KEY_PAD_PORT, ROW3, 1);
			DIO_vidSetPortBitValue(KEY_PAD_PORT, ROW4, 1);

			if((DIO_GetPinValue(KEY_PAD_PORT) & 0x0f) != 0x0f)
			{
				location = 1;
				break;
			}

			DIO_vidSetPortBitValue(KEY_PAD_PORT, ROW1, 1);
			DIO_vidSetPortBitValue(KEY_PAD_PORT, ROW2, 1);
			DIO_vidSetPortBitValue(KEY_PAD_PORT, ROW3, 0);
			DIO_vidSetPortBitValue(KEY_PAD_PORT, ROW4, 1);

			if((DIO_GetPinValue(KEY_PAD_PORT) & 0x0f) != 0x0f)
			{
				location = 2;
				break;
			}

			DIO_vidSetPortBitValue(KEY_PAD_PORT, ROW1, 1);
			DIO_vidSetPortBitValue(KEY_PAD_PORT, ROW2, 1);
			DIO_vidSetPortBitValue(KEY_PAD_PORT, ROW3, 1);
			DIO_vidSetPortBitValue(KEY_PAD_PORT, ROW4, 0);

			if((DIO_GetPinValue(KEY_PAD_PORT) & 0x0f) != 0x0f)
			{
				location = 3;
				break;
			}

		}


		// Check for column location
		if((DIO_GetPinValue(KEY_PAD_PORT) & 0x0f) == 0x0E)
			result = character[location][COL1];
		if((DIO_GetPinValue(KEY_PAD_PORT) & 0x0f) ==  0x0D)
			result = character[location][COL2];
		if((DIO_GetPinValue(KEY_PAD_PORT) & 0x0f) ==  0x0B)
			result = character[location][COL3];
		if((DIO_GetPinValue(KEY_PAD_PORT) & 0x0f) ==  0x07)
			result = character[location][COL4];

		// Clear display by a button
		if (result == character[3][3])
		{
			// Display Clear
			LCD_vidInstruction(0x01, write_operation, IR);
			// Delay > 1.53ms
			_delay_ms(2);
			LinkedList_u8DeleteEntire();
			LinkedList_vidInitialize();
		}


		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// INdication for "=" sign
		else if(result == character[3][2])
		{
				LinkedList_vidRestart();
				caluculation_stage();
				flag1 = 0;
				flag2 = 0;
				input1 = 0;
				input2 = 0;
				op_flag = 0;
				break;
		}
		else
		{
			LCD_vidData(result);
			LinkedList_vidInsertion(result);
		}
	}
	return result;
}






















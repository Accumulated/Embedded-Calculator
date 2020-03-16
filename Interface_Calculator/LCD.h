/*
 * LCD.h
 *
 *  Created on: Aug 23, 2019
 *      Author: Newname
 */

#ifndef HAL_LCD_H_
#define HAL_LCD_H_

#include "../types.h"


// useful controls over instruction function
#define write_operation 	0
#define read_operation		1
#define IR	0
#define DR	1


void LCD_vidInitialize(void);
void LCD_vidData(s16 data);
void LCD_vidInstruction(u8 command, u8 RW_value, u8 RS_value);
void LCD_vidShift(void);
void LCD_vidGoTo(u8 x, u8 y);
u8 LCD_String(s8 *ch);
void LCD_vidPatternCreation(void);


#endif /* HAL_LCD_H_ */

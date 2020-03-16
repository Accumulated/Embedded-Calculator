/*
 * LinkedList.c
 *
 *  Created on: Aug 29, 2019
 *      Author: Newname
 */



#include "../types.h"
#include "LinkedList.h"
#include "stdlib.h"
#include <util/delay.h>
#include "../HAL/LCD.h"
#define POSITIVE	1
#define NEGATIVE	-1

// linked list for numbers
typedef struct node
{
	s16 value;
	struct node *next;
}
node;

u8 arr[4] = {'+', '-', '*', '/'};


// Define head and a pointer to move with
node *root, *ptr;
u8 NoOfNodes = 0;

s16 sum = 0, times = 1;

s16 sign = POSITIVE;
u8 op_flag = 0;


u8 flag1 = 0, flag2 = 0 ;
// Variables from user
s16 input1 = 0 ;
s16 input2 = 0 ;

// Function indicator
u8 function = 0;

// Initialize linked list
void LinkedList_vidInitialize(void)
{
	// Initializing list
	root = malloc(sizeof(node));
	root -> value = 0;
	root -> next = NULL;
}


// Insertion for values from user
void LinkedList_vidInsertion(s16 value)			// if still, you can change to u8
{
	// Check for operation
	LinkedList_vidCheckOp(value);
	if(op_flag)
	{
	}
	else
	{
		// Insert a new number (node)
		value = ((value) % 48) * sign;
		// Inserting sequence initialized
		ptr = root;
		node *tmp = malloc(sizeof(node));
		NoOfNodes++;

		// Insert patient details
		tmp -> next = ptr -> next;

		tmp -> value = value;

		// Update list
		ptr -> next = tmp;

		// Indicate success
	}
}


u8 LinkedList_u8DeleteEntire(void)
{
	// Buffer to save the previous condition of ptr
	node *last_before_stack = NULL;

	// Checking on the next field
	if(ptr -> next != NULL)
	{
		last_before_stack = ptr;
		ptr = ptr -> next;
		LinkedList_u8DeleteEntire();
		ptr = last_before_stack;
	}

	// free current node and return
	free(ptr);
	NoOfNodes = 0;
	return 0;
}

// Get one input from user only and it can be more than one digit
s16 LinkedList_s16GetFinalInputValue(void)
{
	// Buffer to save the previous condition of ptr
	node *last_before_stack = NULL;

	// Buffer to save times value
	s16 tmp = times, t = 0;

	// Checking on the next field
	if(ptr -> next != NULL && ptr != root)
	{
		times = times *  10;
		last_before_stack = ptr;
		ptr = ptr -> next;
		LinkedList_s16GetFinalInputValue();
		ptr = last_before_stack;
		t = ptr->value * tmp;
	}
	else if(ptr -> next != NULL && ptr == root)
	{
		times = 1;
		tmp = times;
		last_before_stack = ptr;
		ptr = ptr -> next;
		LinkedList_s16GetFinalInputValue();
		ptr = last_before_stack;
	}
	else
	{
		t = ptr->value * tmp;
	}

	sum = sum + t;
	return (sum);
}

u8 LinkdeList_u8NumberOfNodes(void)
{
	return NoOfNodes;
}


void LinkedList_vidCheckOp(s16 value)
{
	// check on the operation
	if(value == arr[0])
	{
		LinkedList_vidRestart();
		op_flag = 1;
		sign = POSITIVE;

		// Guard for current function and to reset the function for default if not any
		if(function == 1);
		else
			function = 0;
	}
	else if(value == arr[1])
	{
		LinkedList_vidRestart();
		op_flag = 1;
		sign = NEGATIVE;

		// Guard for current function and to reset the function for default if not any
		if(function == 1);
		else
			function = 0;
	}
	else if(value == arr[2])
	{
		LinkedList_vidRestart();
		op_flag = 1;

		// Invalid Multiplication if no arguments indicated before sign
		if(flag1 == 0)
		{
			LCD_String("syntax error");
			_delay_ms(2000);
		}
		// Change the default addition function to a multiplication function
		else
			function = 1;
	}
/*	else if(value == arr[3])
	{
		LinkedList_vidRestart();
		op_flag = 1;
		if(flag1 == 0)
		{
			LCD_String("Syntax error");
			_delay_ms(2000);
		}
	}*/
	else
	{
		op_flag = 0;
	}
}



void LinkedList_vidRestart (void)
{

	// Write if the variable flag is zero
		if(!flag1 && LinkdeList_u8NumberOfNodes() != 0)	//LinkedList_s16GetFinalInputValue
		{
			ptr = root;
			input1 = LinkedList_s16GetFinalInputValue();
			flag1 = 1;
			sum = 0;
			times = 1;
		}
		else if(!flag2 && LinkdeList_u8NumberOfNodes() != 0)
		{
			ptr = root;
			input2 = LinkedList_s16GetFinalInputValue();
			//LCD_vidData(input2 + 0x30);
			flag2 = 1;
			sum = 0;
			times = 1;
		}
		else;


	sign = POSITIVE;
	LinkedList_u8DeleteEntire();
	LinkedList_vidInitialize();
}

void caluculation_stage(void)
{
	LCD_String("=");
	switch(function)
	{
	case 0:
		LinkedList_vidCompanion((input1+input2));
		break;
	case 1:
		LinkedList_vidCompanion((input1*input2));
		function = 0;
		break;
	default:
		break;

	}


}

//  for user screen output
void LinkedList_vidCompanion(s32 show)
{

	s16 *ptr = malloc(10 * sizeof(s16));
		s8 k = 0, i = 0 ;

		// Indicate POSITIVE OR NEGATIVE VALUES
		if(show >= 0)
			sign = POSITIVE;
		else
			sign = NEGATIVE;
		// get values in order
		do
		{
			if(show/10 == 0)
				ptr[i] = sign*(show%10) + 0x30;
			else
			ptr[i] = sign*(show%10) + 0x30;
			i++;
		show /= 10;
		}while(show);

		ptr[i] = '\0';

		// get string length
		for(k = 0; ptr[k] != '\0'; k++);

		LCD_vidGoTo(1,2);

		// reverse the string sent
		for(int kk = k-1; kk >= 0; kk-- )
		{
			if(sign == NEGATIVE && kk == k-1)
			{
				LCD_String("-");
			}
			LCD_vidData(ptr[kk]);
		}

	free(ptr);
	sign = POSITIVE;

}












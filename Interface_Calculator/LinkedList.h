/*
 * LinkedList.h
 *
 *  Created on: Aug 29, 2019
 *      Author: Newname
 */

#ifndef MACL_LINKEDLIST_H_
#define MACL_LINKEDLIST_H_

#include "stdlib.h"
#include "../types.h"


s16 LinkedList_s16GetFinalInputValue(void);
u8 LinkedList_u8DeleteEntire(void);
void LinkedList_vidInsertion(s16 value);
void LinkedList_vidInitialize(void);
u8 LinkdeList_u8NumberOfNodes(void);
void LinkedList_vidRestart (void);
void LinkedList_vidCheckOp(s16 value);
void caluculation_stage(void);
void LinkedList_vidCompanion(s32 show);



extern u8 flag1, flag2 ;
u8 op_flag;
extern s16 input1 ;
extern s16 input2 ;



#endif /* MACL_LINKEDLIST_H_ */

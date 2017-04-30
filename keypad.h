/** Chatroom control codes for Microprocessor Laboratory
	2017 Spring Semester. Lee Sang Su 2011314425 SKKU
	github.com/sethlee0111

	Based on the code of EMBEDDED SYSTEMS LAB of SKKU 2009

	@author sethlee
	@version 1.0
*/

#ifndef KEYPAD_H
#define KEYPAD_H

#include "projectglobals.h"
#include "chatroom.h"

#define KEYPAD_CHECK Pim.pth.byte

#define FIRST_ROW    0xe0   // 1110
#define SECOND_ROW   0x70	// 0111
#define THIRD_ROW    0xb0	// 1011
#define FOURTH_ROW   0xd0	// 1101

#define SW_TIME      5      

static char Key = 0;
static int change = 0;
static int count;
static char keyEnable = 1;

static int rowDatas[4] = {0,0,0,0} ;

void Init_Keypad(void);
void Init_Key(void);
void Keypad_Check(void);
void PortH_Interrupt_handler (void);
void Pressed_Key(void);

#endif
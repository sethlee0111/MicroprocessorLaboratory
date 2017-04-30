/** Chatroom control codes for Microprocessor Laboratory
	2017 Spring Semester. Lee Sang Su 2011314425 SKKU
	github.com/sethlee0111

	@author sethlee
	@version 0.1
*/
#ifndef CHATROOM_H
#define CHATROOM_H

#include "sethlcd.h"
#include "keypad.h"
#include "sci.h"

typedef struct Messages {
	char read[20];
} Message;
static Message msg[5] = {{0}, {0}, {0}, {0}, {0}};

static unsigned char displayStartIndex=0;
static unsigned char readIndex=0;
static unsigned char msgIndex=4;
static unsigned char initialMsgIndex = 0;

static char sendMsg[30] = {0};
static char sendMsgIndex = 0;

static char buttonPushedSemaphore;
static char isCursorOn = 0;
static char cursorLeftUpperX = 53;
static char cursorLeftUpperY = 4;
static char cursorLetter[2] = {0,0};
void Import_Chatroom(void);
void Message_Put_Char(char chr);
void Display_Messages(void);
unsigned char nextDisplayIndex(void);
void Init_Cursor_Timer(void);
void Timer4_Interrupt(void);
void Button_Pushed(char key);
//void nextMsgIndex(void);

#endif
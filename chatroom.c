/** Chatroom control codes for Microprocessor Laboratory
	2017 Spring Semester. Lee Sang Su 2011314425 SKKU
	github.com/sethlee0111

	@author sethlee
	@version 1.1
*/
#include "chatroom.h"

/**----------------------------------------------
*	Initialization for chatroom  //
*	
*	@author	sethlee
*	@todo	add more stuff
*----------------------------------------------*/
void Import_Chatroom(void) {
	/* Chatroom Background */
	Draw_Vline(0,0,63);
	Draw_Vline(1,0,63);
	Draw_Vline(126,0,63);
	Draw_Vline(127,0,63);
	Draw_Hline(0,0,127);
	Draw_Hline(1,0,127);
	Draw_Hline(62,0,127);
	Draw_Hline(63,0,127);
	Draw_Hline(49,0,127);
	Draw_Hline(50,0,127);

	String_GLCD(4, 4, "HELLO, Seth Lee");
	String_GLCD(13, 4, "Welcome to");
	String_GLCD(21, 4, "The Chatroom");
	String_GLCD(30, 4, "Microprocessor");
	String_GLCD(39, 4, "--------------------");

	Draw_Logo();

	//String_GLCD(52, 4, "Say Something!");
}

/**----------------------------------------------
*	Initialization Timer for a cursor
*	
*	@author	sethlee
*	@todo	add more stuff
*----------------------------------------------*/
void Init_Cursor_Timer(void) {
	Tim0.tios.byte |= 0x30;	// C4I, C5I timer output capture

	/* Using C4I */
	Tim0.tscr1.byte = 0x80;	//	timer ON 
	Tim0.tscr2.byte = 0x07;	// 0b0111 Prescale 128

	//Tim0.tctl1.byte = 0x03; // OL4, OM4 set
	Tim0.tie.byte |= 0x10; 	// timer 4 interrupt enable, timer 5 is disabled for now
	Tim0.tc[4].word = Tim0.tcnt.word + 5; // timer output compare
	Tim0.tflg1.byte = 0x30;		//Interrupt flag clear
}
/* Interrupt Handler for the timer */
void Timer4_Interrupt(void) {
	char i;
	if(Tim0.tflg1.byte & 0x10) {
		Pim.pieh.byte = 0x07;	// disable semaphore for keypadsi
		Tim0.tc[4].word += 62500 / 2;		//next in 0.5s
		if(isCursorOn) {
			Erase_GLCD(cursorLeftUpperX, cursorLeftUpperX + 6, cursorLeftUpperY, cursorLeftUpperY + 4);
			String_GLCD(cursorLeftUpperX, cursorLeftUpperY, cursorLetter);
			isCursorOn = 0;
		} else {
			for(i = 0; i < 5; i++) {
				Draw_Vline(cursorLeftUpperY + i, cursorLeftUpperX, cursorLeftUpperX + 6);
			}
			isCursorOn = 1;
		}
		Tim0.tflg1.byte = 0x10;		//Interrupt flag clear
	}
}
/*	timer 5: measuring elasped time after last key input
*	if more than 1s has passed, the cursor moves to the next column
*/
void Timer5_Interrupt(void) {
		if(Tim0.tflg1.byte & 0x20) { //user started writing to the line,
			Tim0.tie.byte &= ~(0x20);	//timer 5 is disabled!
			Tim0.tflg1.byte = 0x20;	//	Timer 5 interrupt flag clear
			//	first, we make sure to put character on current cursor instead of cursor box
			Erase_GLCD(cursorLeftUpperX, cursorLeftUpperX + 6, cursorLeftUpperY, cursorLeftUpperY + 4);
			String_GLCD(cursorLeftUpperX, cursorLeftUpperY, cursorLetter);
			//write to sci
			if(cursorLetter[0] == '\0')	{
				sendMsg[sendMsgIndex++] = '\n';
				sendMsg[sendMsgIndex] = '\r';
				write_sci0(sendMsg);
				cursorLeftUpperY = 4;	// initialize cursor
				Erase_GLCD(cursorLeftUpperX, cursorLeftUpperX + 6, 4, 125);
				sendMsgIndex = 0;
			} else {
				sendMsg[sendMsgIndex++] = cursorLetter[0];
				cursorLetter[0] = 0x20;
				cursorLeftUpperY += 6; // we move cursor to the right
				Init_Key();
			}
		}
	}
/*
void Init_Message(void) {
	msg[4].read = "@ Hello, Welcome to @";
	msg[3].read = "<   The Chatroom    >";
	msg[2].read = "*   Please Check    *";
	msg[1].read = "*  Your Connection  *";
	msg[0].read = "_____________________";
}
*/

/**----------------------------------------------
*	When a keypad is pushed, this is invoked
*	
*	@author	sethlee
*	@todo	add more stuff
*----------------------------------------------*/
void Button_Pushed(char key) {
	cursorLetter[0] = key;
	Erase_GLCD(cursorLeftUpperX, cursorLeftUpperX + 6, cursorLeftUpperY, cursorLeftUpperY + 4);
	String_GLCD(cursorLeftUpperX, cursorLeftUpperY, cursorLetter);
	Tim0.tflg1.byte = 0x20;	//	Timer 5 interrupt flag clear
	if((key >= 0x30 && key <= 0x39) || key == 0x20) {// if the key is number,
		Tim0.tc[5].word = Tim0.tcnt.word + 10000;	// immediate cursor change
		Tim0.tie.byte |= 0x20;
		Tim0.tflg1.byte = 0x20;	//	Timer 5 interrupt flag clear
	} else {
	Tim0.tc[5].word = Tim0.tcnt.word + 62500;	// we count 1s
	Tim0.tie.byte |= 0x20;	//timer 5 is enabled!
	}

}
/**----------------------------------------------
*	put character in a message
*	
*	@author	sethlee
*	@todo	add more stuff
*----------------------------------------------*/
void Message_Put_Char(char data) {
	if(data == 0x0D){
	  	msg[msgIndex].read[readIndex++] = '\0';
	  	displayStartIndex = msgIndex;
	  	initialMsgIndex++;
	  	if(msgIndex == 0)
	  		msgIndex = 4;
	  	else
	  		msgIndex--;
	  	Display_Messages();
	 	readIndex = 0;	// initialize index for individual char
	} else {
		msg[msgIndex].read[readIndex++] = data;
	}
}

// Erase_GLCD(39 - (i * 9), 39 - (i * 9) + 8 , 4 , 123);
// String_GLCD(39 - (i * 9), 4, msg[i].read);

/**----------------------------------------------
*	Display all 5 rows of Messages
*	
*	@author	sethlee
*	@todo	add more stuff
*----------------------------------------------*/
void Display_Messages(void) {
	char i;
	if(initialMsgIndex < 5) {
		for(i = 0 ; i < 5; i++) {
			Erase_GLCD(39 - (i * 9), 39 - (i * 9) + 8 , 4 , 123);
			String_GLCD(39 - (i * 9), 4, msg[i].read);
		}
	} else {
		for(i = 0 ; i < 5; i++) {
			Erase_GLCD(39 - (i * 9), 39 - (i * 9) + 8 , 4 , 123);
			String_GLCD(39 - (i * 9), 4, msg[nextDisplayIndex()].read);
		}
	}
}
/*returns current display Index, put next display index*/
unsigned char nextDisplayIndex(void) {
	unsigned char temp = displayStartIndex;
	if(displayStartIndex == 4) {
		displayStartIndex = 0;
		return temp;
	} else {
		displayStartIndex += 1;
		return temp;
	}
	
}


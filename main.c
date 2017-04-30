/**----------------------------------------------
*	Main Method for the chatroom
*	
*	@author	sethlee
*----------------------------------------------*/
#include "main.h"

unsigned char RX[11]; 
unsigned char sci0[70] = {"<WELCOME to The Chatroom! have fun! -cordially yours, Seth> \n\r"};	// message indicating successful connection
void main()
{   
	int_disable();			// interrupt disable
	init_sci0(19200,RX);    // SCI init
	Init_Cursor_Timer();  	// chatroom cursor init
	int_enable();			// interrupt enable
	write_sci0( sci0 );		// write the successful connection
	
	Import_GLCD();			// GLCD init
  	Init_Keypad();			// keypad init
	Import_Chatroom();		// chatroom init

	while(1) {
	 	Keypad_Check(); 	// periodical checking of keypad
	}
	
}

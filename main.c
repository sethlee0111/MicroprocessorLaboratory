#include "main.h"

unsigned char RX[11]; 
unsigned char sci[50] = {"SCI OK "};

void main()
{   
	int_disable();
	init_sci0(19200,RX);    
	Init_Cursor_Timer();  
	int_enable();
	write_sci0( sci );
	
	Import_GLCD();
  Init_Keypad();
	Import_Chatroom();

	while(1) {
	 	Keypad_Check(); 
	}
	
}

/** SCI control codes for Microprocessor Laboratory
	2017 Spring Semester. Lee Sang Su 2011314425 SKKU
	github.com/sethlee0111

	Based on the code of EMBEDDED SYSTEMS LAB of SKKU 2009

	@author sethlee
	@version 1.0
*/
#include "main.h"
#include "sethlcd.h"

unsigned char *txbuffer_sci0; // Buffer for storing text
int txoffset_sci0;            
unsigned char *rxbuffer_sci0; // Buffer for storing read

/*  */
/**----------------------------------------------
*	Initialize the first channel of serial
*----------------------------------------------*/
void init_sci0(int baud_rate, unsigned char *rxbuf) { 
	Sci0.scibd.word = 52;		// module clock / (16*baud rate) = 52
	Sci0.scicr1.byte = 0x00 ;  // Normal operation & 8 data bit format & Parity disabled
	Sci0.scicr2.byte = 0x2C;  	//activate transmit & receive & receive interrupt
	txoffset_sci0 = 0;          
	rxbuffer_sci0 = rxbuf;
}
/**----------------------------------------------
*	Write & transmit
*----------------------------------------------*/
void write_sci0(unsigned char *text) {
	txbuffer_sci0 = text;
	
	Sci0.scicr2.byte |= 0x88;    // activate TIE & TE     

}
/* */
/**----------------------------------------------
*	Serial transmit & read interrupt 
*----------------------------------------------*/
void sci0_handler(void) {
	int data;
	Regs.portb.byte = 0xFF;   
	Pim.ptm.byte = 0xC0;      

	if(Sci0.scisr1.bit.rdrf == 1)   // 1 if receiving
	{
			*rxbuffer_sci0 = Sci0.scidrl.byte;  // save a 1 bit data to receive buffer
			 data = (*rxbuffer_sci0);

			 Message_Put_Char(data);

	}
	else if (Sci0.scisr1.bit.tdre == 1)  // tdre is 1 if transmitting
	{
		Sci0.scidrl.byte = *txbuffer_sci0;    
		txbuffer_sci0++;

		if(*txbuffer_sci0 == '\0')	//@TODO have to change it if this is changed to board-to-board
		{
			Sci0.scicr2.byte &= 0x7F;           
		}
	}
}
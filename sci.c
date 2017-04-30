
#include "main.h"
#include "sethlcd.h"

unsigned char *txbuffer_sci0; // Buffer for storing text
int txoffset_sci0;            
unsigned char *rxbuffer_sci0; // Buffer for storing read

/* Initialize the first channel of serial */
void init_sci0(int baud_rate, unsigned char *rxbuf) 
{
	unsigned long oscclk, modclk;  
	unsigned short br;    

	oscclk = 16000000;	  
	modclk = oscclk / 2;	  
	br = (unsigned short)(modclk / (16*baud_rate));    

	Sci0.scibd.word = 52;  
	Sci0.scicr1.byte = 0x00 ;  
	Sci0.scicr2.byte = 0x2C;  
	txoffset_sci0 = 0;          
	rxbuffer_sci0 = rxbuf;
}
void write_sci0(unsigned char *text)
{

	txbuffer_sci0 = text;
	
	Sci0.scicr2.byte |= 0x88;         

}
/* Serial transmit & read interrupt */
void sci0_handler(void)
{
	int data;
	Regs.portb.byte = 0xFF;   
	Pim.ptm.byte = 0xC0;      

	if(Sci0.scisr1.bit.rdrf == 1)   
	{
			*rxbuffer_sci0 = Sci0.scidrl.byte;  
			 data = (*rxbuffer_sci0);

			 Message_Put_Char(data);

	}
	else if (Sci0.scisr1.bit.tdre == 1)  
	{
		Sci0.scidrl.byte = *txbuffer_sci0;    
		txbuffer_sci0++;

		if(*txbuffer_sci0 == '\0')	//@TODO have to change it if this is changed to board-to-board
		{
			Sci0.scicr2.byte &= 0x7F;           
		}
	}
}
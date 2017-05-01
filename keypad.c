/** Chatroom control codes for Microprocessor Laboratory
	2017 Spring Semester. Lee Sang Su 2011314425 SKKU
	github.com/sethlee0111

	Based on the code of EMBEDDED SYSTEMS LAB of SKKU 2009

	@author sethlee
	@version 1.0
*/

#include "keypad.h"

int rowDataSamples = 0;

/**----------------------------------------------
*	Initialization for Keypad //
*	
*	@author	sethlee
*----------------------------------------------*/
void Init_Keypad(void) {
  Pim.ddrh.byte = 0xf0; // PORTH AllOutput Setting
  Pim.pieh.byte = 0x07;		// Column만 Interrupt로 이용
  Pim.ppsh.byte = 0x00;
  
}
/**----------------------------------------------
*	Delay function for Keypad //
*	
*	@author	sethlee
*----------------------------------------------*/
void m_delay(char x) {
  char i;
  for(i=0;i<x;i++); 
}

/**----------------------------------------------
*	Checking Keypad : should be iterating periodically//
*	
*	EMBEDDED SYSTEMS LAB
*----------------------------------------------*/
void Keypad_Check(void) {

  KEYPAD_CHECK = FIRST_ROW;
  m_delay(SW_TIME);
  KEYPAD_CHECK = SECOND_ROW;
  m_delay(SW_TIME);
  KEYPAD_CHECK = THIRD_ROW;
  m_delay(SW_TIME);
  KEYPAD_CHECK = FOURTH_ROW;
  m_delay(SW_TIME);
  
}

void Init_Key(void) {
  Key = 0;
}

/**----------------------------------------------
*	Interrupt Handler when the key is pressed
*	Interrupt is for the column
*
*	@author	sethlee
*----------------------------------------------*/
void PortH_Interrupt_handler (void){
   unsigned char keySource = (~(KEYPAD_CHECK)) >> 4; // row numbers: 0b 2341
   /* Sampler for the button */
  rowDatas[0] += (keySource & 0b0001);
  rowDatas[1] += (keySource & 0b1000) >> 3; // row 2
  rowDatas[2] += (keySource & 0b0100) >> 2; // row 3
  rowDatas[3] += (keySource & 0b0010) >> 1; // row 4
  rowDataSamples++;

  if(rowDataSamples > 400 ) { //&& keyEnable == 1
	Pressed_Key();
	rowDataSamples = 0;
  }
	Pim.pifh.byte = 0x01 + 0x02 + 0x04;
}

/**----------------------------------------------
*	Determine which key is pressed
*
*	@author	sethlee
*----------------------------------------------*/
void Pressed_Key(void) {
	char i;
	char maxRowIndex = 0;
	Pim.pieh.byte = 0x00;   // semaphore - prevents multiple inputs. 
	//This semaphore is disabled after one cursor blink at chatroom.c

	/* row Data max & selecting */
	for(i=1; i<4; i++) {
	  if(rowDatas[i] > rowDatas[maxRowIndex])
		maxRowIndex = i;
	}
	// rowData initialize
	for(i=0;i<4;i++) {
	  rowDatas[i] = 0;
	}

  if(maxRowIndex == 0) {  //first row
	
	m_delay(SW_TIME);
///////////////////////////////////////////////////////    
	if(Pim.pifh.byte & 0x01) //2nd Column에 인터럽트 발생시 
	{
	//Typing Number
	if(change == 0) {
	Key = 50; //Ascii Code
	} 
	
	//Typing Character
	//변수 change가 1로 변하면 Character Out
	else if(change==1) {
	   count =0;
	   
	if(Key==65) {
	  Key++; 
	} 
	else if(Key==66){
	  Key++; 
	} 
	else{
	  Key = 65;
	} 
	
	}
	 m_delay(SW_TIME);  
	}
	
////////////////////////////////////////////////////
	else if(Pim.pifh.byte & 0x02) { //1st Column
	  if(change == 0) {
	Key = 49; //Ascii Code
	} 
	
	//Typing Character
	//변수 change가 1로 변하면 Character Out
	else if(change==1) {
	   Key = 0x20;
	 }
	   

	 m_delay(SW_TIME);  
	}
	
///////////////////////////////////////////////////////    
	if(Pim.pifh.byte & 0x04) //3rd Column에 인터럽트 발생시 
	{
	
	
	//Typing Number
	if(change == 0) {
	Key = 51;
	} 
	
	//Typing Character
	//변수 change가 1로 변하면 Character Out
	else if(change==1) {
	   count =0;
	if(Key==68) {
	  Key++; 
	} 
	else if(Key==69){
	  Key++; 
	} 
	else{
	  Key = 68;
	} 
	}
	 m_delay(SW_TIME);  
	}
////////////////////////////////////////////////////    
   }
//1st Row check finish


//2nd Row check

   if(maxRowIndex == 1) {
	
	m_delay(SW_TIME);
	

///////////////////////////////////////////////////////    
	if(Pim.pifh.byte & 0x01) //2nd Column에 인터럽트 발생시 
	{
	
	
	//Typing Number
	if(change == 0) {
	Key = 53; //Ascii Code
	} 
	
	//Typing Character
	//변수 change가 1로 변하면 Character Out
	else if(change==1) {
	   count =0;
	   
	if(Key==74) {
	  Key++; 
	} 
	else if(Key==75){
	  Key++; 
	} 
	else{
	  Key = 74;
	} 
	
	}
	 m_delay(SW_TIME);  
	}
	
////////////////////////////////////////////////////
	else if(Pim.pifh.byte & 0x02) //1st Column에 인터럽트 발생시 
	{
	
	
	//Typing Number
	if(change == 0) {
	Key = 52; //Ascii Code
	} 
	
	//Typing Character
	//변수 change가 1로 변하면 Character Out
	else if(change==1) {
	   count =0;
	   
	if(Key==71) {
	  Key++; 
	} 
	else if(Key==72){
	  Key++; 
	} 
	else{
	  Key = 71;
	} 
	
	}
	 m_delay(SW_TIME);  
	}
	
///////////////////////////////////////////////////////    
	if(Pim.pifh.byte & 0x04) //3rd Column에 인터럽트 발생시 
	{
	
	
	//Typing Number
	if(change == 0) {
	Key = 54;
	} 
	
	//Typing Character
	//변수 change가 1로 변하면 Character Out
	else if(change==1) {
	   count =0;
	   
	if(Key==77) {
	  Key++; 
	} 
	else if(Key==78){
	  Key++; 
	} 
	else{
	  Key = 77;
	} 
	}
	 m_delay(SW_TIME);  
	}
////////////////////////////////////////////////////    
   }
//2nd Row check finish

//3rd Row check

   if(maxRowIndex == 2) {
	
	m_delay(SW_TIME);
	

///////////////////////////////////////////////////////    
	if(Pim.pifh.byte & 0x01) //2nd Column에 인터럽트 발생시 
	{
	
	
	//Typing Number
	if(change == 0) {
	Key = 56; //Ascii Code
	} 
	
	//Typing Character
	//변수 change가 1로 변하면 Character Out
	else if(change==1) {
	   count =0;
	   
	if(Key==84) {
	  Key++; 
	} 
	else if(Key==85){
	  Key++; 
	} 
	else{
	  Key = 84;
	} 
	
	}
	 m_delay(SW_TIME);  
	}
	
////////////////////////////////////////////////////
	else if(Pim.pifh.byte & 0x02) //1st Column에 인터럽트 발생시 
	{
	
	
	//Typing Number
	if(change == 0) {
	Key = 55; //Ascii Code
	} 
	
	//Typing Character
	//변수 change가 1로 변하면 Character Out
	else if(change==1) {
	   count =0;
	   
	if(Key==80) {
	  Key=82; 
	} 
	else if(Key==82){
	  Key++; 
	} 
	else{
	  Key = 80;
	} 
	
	}
	 m_delay(SW_TIME);  
	}
	
///////////////////////////////////////////////////////    
	if(Pim.pifh.byte & 0x04) //3rd Column에 인터럽트 발생시 
	{
	
	
	//Typing Number
	if(change == 0) {
	Key = 57;
	} 
	
	//Typing Character
	//변수 change가 1로 변하면 Character Out
	else if(change==1) {
	   count =0;       
	if(Key==87) {
	  Key++; 
	} 
	else if(Key==88){
	  Key++; 
	} 
	else{
	  Key = 87;
	} 
	}
	 m_delay(SW_TIME);  
	}
////////////////////////////////////////////////////    
   }
//3rd Row check finish


//4th Row check

   if(maxRowIndex == 3) {
	
	m_delay(SW_TIME);
	

///////////////////////////////////////////////////////    
	if(Pim.pifh.byte & 0x01) //2nd Column에 인터럽트 발생시 
	{
	
	
	//Typing Number
	if(change == 0) {
	Key = 48; //Ascii Code
	} 
	
	//Typing Character
	//변수 change가 1로 변하면 Character Out
	else if(change==1) {
	   count =0;
	   
	if(Key==81) {
	  Key=90; 
	} 
	else{
	  Key = 81;
	} 
	
	}
	 m_delay(SW_TIME);  
	}
	
////////////////////////////////////////////////////
	else if(Pim.pifh.byte & 0x02) //1st Column에 인터럽트 발생시 
	{
	
	//Typing Number
	if(change == 0) {
	change =1;
	Key = 42; //Ascii Code *
	} 
	else if(change==1) {
	   change = 0 ;
	  Key = 42;
	}
	 m_delay(4000);  
	}
	
///////////////////////////////////////////////////////    
	if(Pim.pifh.byte & 0x04) //3rd Column에 인터럽트 발생시 
	{
	Key = '\0';
	  
	 m_delay(SW_TIME);  
	}
////////////////////////////////////////////////////    
   }
//4th Row check finish
Pim.pifh.byte = 0x01 + 0x02 + 0x04;

   if(Key != 42)  // do not do this when Key = *
	Button_Pushed(Key);
}
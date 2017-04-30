/** Driver for LCD ABG128064A23 for Microprocessor Laboratory
	2017 Spring Semester. Lee Sang Su 2011314425 SKKU
	github.com/sethlee0111

	@author sethlee
	@version 0.9
*/

/*------------------    IMPORTANT NOTE   --------------------
	Every function is divided into "Buffer Enabled" and 
	"Buffer Disabled" Functions. Buffer disabled functions
	do NOT change buffer value. They are used for Testing
------------------------------------------------------------*/

#include "sethlcd.h"

void Busy_Wait(void) {
	unsigned int i;

	for(i=0;i<5;i++);
}


/**----------------------------------------------
*	write data or command to GLCD
*	@param flag for data, 1 byte data
*	@author sethlee
*----------------------------------------------*/
void Write_GLCD(unsigned char isData, unsigned char data) {

	// all write clear
	//GLCD_E = 1;         // E pin High -> No! why not..??
	GLCD_DataDir = 0xFF; //	Set as Output
	GLCD_Data = '\0';
	GLCD_RS = 1- isData;
	GLCD_RW = 1;

	Busy_Wait();
	GLCD_E = 0;			// E pin Low

	//Busy_Wait();		// is this necessary?
	GLCD_RS = isData;   // RS(DI) set low for command or high for data
	GLCD_RW = 0;        // RW set low for write mode
	Busy_Wait();        // Twl > 450ns & Tasu > 140ns

	GLCD_E = 1;         // E pin High
	//Busy_Wait();		// is this necessary?
	GLCD_Data = data;   // Set dat in the data port
	Busy_Wait();        // Twh > 450ns and Tdsu > 520ns

	GLCD_E = 0;         // E pin Low
	Busy_Wait();        // Tdhw > 10ns  
	GLCD_RW = 1;
	GLCD_Data = '\0';
	GLCD_RS = 1	- isData;
	Busy_Wait(); 

	//GLCD_E = 1;         // E pin High
} 
/**----------------------------------------------
*	write a single "column" to GLCD
*	Buffer : Disabled
*	@param 1 byte data
*	@author sethlee
*----------------------------------------------*/
void Write_Column_GLCD(unsigned char data)
{	
	Write_GLCD(1, data);    // Write in data mode
}

/**----------------------------------------------
*	write a single character to GLCD
*	Buffer : Disabled
*	@param page for x, y position, a character
*	@author sethlee
*----------------------------------------------*/
void Write_Character_GLCD(unsigned char page, unsigned char yPos, char chr)
{
	unsigned char columnIndex=0;
	unsigned char characterIndex = chr - 0x20; // determine font number
	Move_Cursor(page, yPos);
	do{
		//Write_GLCD(1, 0xFF);
		Write_Column_GLCD(font[characterIndex][columnIndex]);
		columnIndex++;
		} while(columnIndex < 5); // ascii is 5 * 7 font
}
/**----------------------------------------------
*	write a string to GLCD with x Page
*	Buffer : Disabled
*	@param page for x, y position, *string with NULL
*	@author sethlee
*----------------------------------------------*/
void Write_String_GLCD(unsigned char page, unsigned char yPos, char * chrArr)
{
	char columnIndex = 0;
	char chrArrIndex = 0; // Index for characters in the String
	char characterIndex;
	//int y = (int) yPos;

	Move_Cursor(page, yPos);

	while(chrArr[chrArrIndex] != '\0') {
		//this loop iterates until it gets to NULL character or maximum length of a string
		characterIndex = chrArr[chrArrIndex] - 0x20; // determine font number
		do {
			Write_Column_GLCD(font[characterIndex][columnIndex]);
			columnIndex++;	//increase the index of columns
			
			if(++yPos == 127){ 	//if we have to change row
				yPos = 0;
				page++;
				Move_Cursor(page, yPos); 
			}
			if(yPos == 64) { //the point where yPos exceeds 63
				Move_Cursor(page, yPos);
			}

		} while	(columnIndex < 5); // ascii is 5 * 7 font
		Write_Column_GLCD(0x00); // blank space

		if(++yPos == 127){ 	//if we have to change row
			yPos = 0;
			page++;
			Move_Cursor(page, yPos); 
		} 
		if(yPos == 64) { //the point where yPos exceeds 63
			Move_Cursor(page, yPos);
		}

		columnIndex = 0; // Initialize the index for counting columns for each fonts
		chrArrIndex++; 
	}     
}
/**----------------------------------------------
*	write a string to the BUFFER
*	Does NOT affact GLCD
*	@param page x, y position, *string
*	@author sethlee
*	@TODO apply changing lines
*----------------------------------------------*/ 

void String_Buffer(unsigned char xPos, unsigned char yPos, char* strArr) {
	unsigned char page = xPos / 8;
	unsigned char insidePage = xPos % 8;
	unsigned char s, c;	
	unsigned char k = yPos;
	unsigned char l = 0;
	unsigned char strIndex = 0; // Index for individual characters in strArr
	unsigned char i = 0; // Index for individual columns in strArr
	unsigned char fontNum;
	unsigned char shiftedFontColumn;

	if(xPos > 63 || yPos > 128)
		return;

	//find out the length of strArr 
	while(strArr[l] != '\0')
		l++;	//now, l is the length of the strArr, without NULL

	// Write on upper Page 
	Move_Cursor(page, yPos);
	for(s = 0; s < l; s++) {		//	loop for a character
		fontNum = strArr[s] - 0x20;
		for(c = 0; c < 5; c++) {	//	loop for a single column in a character
			shiftedFontColumn = font[fontNum][c] << insidePage;	// shift to left(downward)
			buffer[page][k] |= shiftedFontColumn;
			k++;	// increase an Index for yPos
		}
		k++; 	// space for space
	}
	page++;	// Write on lower Page
	Move_Cursor(page, yPos); k = yPos;
	for(s = 0; s < l; s++) {		//	loop for a character
		fontNum = strArr[s] - 0x20;
		for(c = 0; c < 5; c++) {	//	loop for a single column in a character
			shiftedFontColumn = font[fontNum][c] >> (8 - insidePage);	// shift to right(upward)
			buffer[page][k] |= shiftedFontColumn;
			k++;	// increase an Index for yPos
		}
		k++; 	// space for space
	}
}
/**----------------------------------------------
*	Update Buffer in a designated area
*	@param page boundary, Y boundary
*	@author sethlee
*	
*----------------------------------------------*/ 
void Buffer_Update_GLCD(unsigned char lowerPage, unsigned char upperPage,
					 unsigned char lowerY, unsigned char upperY) {
	unsigned char i, k;

	for(i = lowerPage; i <= upperPage; i++){ 
		Move_Cursor(i, lowerY);	
	  
		for(k = lowerY; k <= upperY; k++) {
			if(k == 64)
				Move_Cursor(i, 64);		//switch CS Area
			Write_GLCD(1,buffer[i][k]);
		}
	}
}

/**----------------------------------------------
*	Clear Buffer in a designated area
*	@param page boundary, Y boundary
*	@author sethlee
*	
*----------------------------------------------*/ 
void Clear_Buffer(unsigned char lowerX, unsigned char upperX,
					 unsigned char lowerY, unsigned char upperY) {
	unsigned char i, k, page;

	for(i = lowerX; i <= upperX; i++){ 
		page = i / 8;
		for(k = lowerY; k <= upperY; k++) {
			buffer[page][k] &= ~(0x01 << (i % 8)); //BIC Designated Area
		}
	}
}
/**----------------------------------------------
*	Clear GLCD in a designated area
*	@param page boundary, Y boundary
*	@author sethlee
*	
*----------------------------------------------*/ 
void Erase_GLCD(unsigned char lowerX, unsigned char upperX,
					 unsigned char lowerY, unsigned char upperY) {
	Clear_Buffer(lowerX, upperX, lowerY, upperY);
	Buffer_Update_GLCD(lowerX / 8, upperX / 8, lowerY, upperY);
}

/**----------------------------------------------
*	write a string to the GLCD
*	@param x, y coordinates
*	@author sethlee
*	@TODO apply changing lines, set appropriate boundary for y
*	@TODO make this available for various string ranges
*----------------------------------------------*/ 
void String_GLCD(unsigned char xPos, unsigned char yPos, char* strArr) {
	unsigned char l = 0 ;
	unsigned char page = xPos / 8;
	while(strArr[l] != '\0' && l < MAX_STRING_LENGTH)
		l++;
	String_Buffer(xPos, yPos, strArr);
	Buffer_Update_GLCD(page, page+1, yPos, 127);
}

/**----------------------------------------------
*	write a string character to GLCD with X POS
*	@param x position, y position, a character
*	@author sethlee
*----------------------------------------------*/
/*
void String_GLCD(unsigned char xPos, unsigned char yPos, char* chrArr)
{
	unsigned char columnIndex=0;
	unsigned char characterIndex = chr - 0x20;
	unsigned char upperPage = xPos / 8;
	unsigned char upperPageIndex = xPos % 8;
	// Write on upper page
	Move_Cursor(page, yPos);
	do{
		//Write_GLCD(1, 0xFF);
		Write_Column_GLCD(font[characterIndex][columnIndex]);
		columnIndex++;
		} while(columnIndex < 5); // ascii is 5 * 7 font
} */
/*
void Write_Str_GLCD(unsigned char xPos, unsigned char yPos, unsigned char *str, unsigned char count)
{
	unsigned char c=0;

	Move_Cursor(xPos, yPos);
	do{
		Write_Column_GLCD(chr[c]);    // Write in data mode
		c++;
		str++;
		} while(c < count * 5); // ascii is 5 * 7 font
} */
/**----------------------------------------------
*	read data or status to GLCD
*	@param flag for data, 1 byte data
*	@author sethlee
*----------------------------------------------*/
unsigned char Read_GLCD (unsigned char isData)
{
	unsigned char data;
	GLCD_DataDir = 0x00; // Set as Input
	GLCD_E = 0;         // E pin Low  
	GLCD_RS = isData;    // RS(DI) set low for status or high for data
	GLCD_RW = 1;        // RW set low for read mode  
	Busy_Wait();        // Twl > 450ns & Tasu > 140ns
	GLCD_E = 1;         // E pin High
	Busy_Wait();        // Data appears after a delay of Td (>320ns)
						
	data = GLCD_Data;   // Set data in the data port
	GLCD_E = 0;         // E pin Low
	Busy_Wait();        // Tdhr > 20ns, before disappearing data in the line   

	return data;
}
/**----------------------------------------------
*	CS Setting
*	
*	@author	sethlee
*	@param number for CS. 1 or 2.
*----------------------------------------------*/
void Set_CS(char csNum) {
	if(csNum == 1) {
		GLCD_CS1 = 1; GLCD_CS2 = 0;
	} else if(csNum == 2) {
		GLCD_CS1 = 0; GLCD_CS2 = 1;
	} else if(csNum == 3) {
		GLCD_CS1 = 1; GLCD_CS2 = 1;
	}
}
/**----------------------------------------------
*	Clear GLCD, cursor and screen buffer for GLCD
*	
*	@author	sethlee
*	@todo	add GLCD clear
*----------------------------------------------*/
void Clear_GLCD(void) {
	unsigned char i,k;

	GLCD_Control = 0;

	GLCD_CS1 = 1; GLCD_CS2 = 2;
	Write_GLCD(0, 0x3F); // command mode, display ON(0b00111111)
	Set_Start_Line(0);

	for(i=0; i < 8; i++) {
		for(k=0; k < 128; k++) {
			Move_Cursor(i, k);
			Write_GLCD(1, 0x00);
			buffer[i][k] = 0;
		}
	}
}


/**----------------------------------------------
*	Initialization for GLCD
*	
*	@author	sethlee
*	@todo	add GLCD clear
*----------------------------------------------*/
void Import_GLCD(void) {

	/* Set Port Directions */
	Pim.ddrp.byte = 0xff; //Port P : Output
	GLCD_DataDir = 0xff; //Port T : Output
	Clear_GLCD();
	//Write_GLCD(0, 0x3E); // command mode, display OFF(0b00111110)
	Write_GLCD(0, 0x3F); // command mode, display ON(0b00111111)
	Set_Start_Line(0);

}

/**----------------------------------------------
*	Set the Starting line
*	
*	https://openlabpro.com/guide/ks0108-graphic-lcd-interfacing-pic18f4550-part-2/
*	@todo	can be used for animating the contents
*----------------------------------------------*/
void Set_Start_Line(unsigned char line)
{
	unsigned char data;
	GLCD_CS1 = 1;       // Select left controller
	GLCD_CS2 = 1;       // Select right controller
	data = (0xC0 | line);   // Set start line command
	Write_GLCD(0,data);
}

/**----------------------------------------------
*	Moves x, y coordinates to a given cursor
*	@author sethlee
* 	@param page of x(0~7), position of y(0~127)
*----------------------------------------------*/
void Move_Cursor(unsigned char page, unsigned char yPos) {
	if(page > 7 || yPos > 127)
		return;

	//Write_GLCD(0, (0xB8 | page) & 0xBF); // command mode, set X page

	if(yPos < 64) {
		Set_CS(1);
		Write_GLCD(0, (yPos | 0x40) & 0x7F); //set yPos with DB6 High
	} else {
		Set_CS(2);
		yPos -= 64; 
		Write_GLCD(0, (yPos | 0x40) & 0x7F); //set yPos with DB6 High
	}

	Write_GLCD(0, (0xB8 | page) & 0xBF); //@TODO why this should be here..?

	currentPage = page;
	currentY = yPos;
}
/**----------------------------------------------
*	Draws a vertical line at a given point
*	Buffer : Enabled
*	@author sethlee
*	
*----------------------------------------------*/
void Draw_Vline(unsigned char yPos, unsigned char lowerX, unsigned char upperX) {
	unsigned char j, page, innerIndex;
	unsigned char lowerPage = lowerX / 8;
	for(j=lowerX; j <= upperX; j++) {
		page = j / 8;
		innerIndex = j % 8;
		buffer[page][yPos] |= (0x01 << innerIndex);
	}        
	Buffer_Update_GLCD(lowerPage, page, yPos, yPos); //	last page value becomes upperPage
}
/**----------------------------------------------
*	Draws a vertical line at a given point
*	Buffer : Disabled
*	@author sethlee
*	@deprecated
*----------------------------------------------*/
void Draw_Direct_Vline(unsigned char yPos, unsigned char lowerX, unsigned char upperX) {
	unsigned char j, page, innerIndex;
	unsigned char lowerPage = lowerX / 8;
	innerIndex = lowerX % 8; // first page
	Move_Cursor(lowerPage, yPos);
	Write_Column_GLCD(0xFF << innerIndex);

	for(j= lowerPage + 1; j <= (upperX / 8) - 1 ; j++) {	
		Move_Cursor(j, yPos);
		Write_Column_GLCD(0xFF);
	}        
	if(upperX/8 > lowerPage){	// if the line is not consisted of one page
		innerIndex = upperX % 8;
		Move_Cursor(upperX/8, yPos);
		Write_Column_GLCD(0xFF >> innerIndex);
	}
}
/**----------------------------------------------
*	Draws a horizontal line at a given point
*	Buffer : Enabled
*	@author sethlee
*	
*----------------------------------------------*/
void Draw_Hline(unsigned char xPos, unsigned char lowerY, unsigned char upperY) {
	unsigned char i;
	unsigned char page = xPos / 8;
	for(i=lowerY; i <= upperY; i++) {
		buffer[page][i] |= (0x01 << (xPos % 8));
	}
	Buffer_Update_GLCD(page, page, lowerY, upperY);
}
/**----------------------------------------------
*	Draws a dot at a given point
*	Buffer : Enabled
*	@author sethlee
*	
*----------------------------------------------*/
void Draw_Dot(unsigned char xPos, unsigned char yPos) {
	unsigned char page, rowNum; //rowNum : position of x(0~7) inside a single column
	unsigned char column;
	currentY = yPos;

	if(xPos > 63 || yPos > 127)
		return;

	currentPage = xPos / 8;
	rowNum = xPos % 8;
	column = 0x01 << rowNum;
	/* Write to the Buffer */
	buffer[currentPage][currentY] |= column; // ORR a new column in the screen buffer

	Move_Cursor(currentPage, currentY); // move the cursor
	Write_GLCD(1, buffer[currentPage][currentY]); // 1: data mode

}




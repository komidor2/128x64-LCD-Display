

#include <iostream> 
#include <wiringPi.h>
//#include <string>
#include <cstring>
#include <random>
#include <fstream>
#include <time.h>
#include <iomanip>   // to format output (hex with 4letters)
#include "Display128x64.h"


/*
# Display 12864 ZW display (128x64 point) SERIAL:
# Display output (purpouse) -    connected to ...
# 1  (GND)                  - RasPi (GPIO GND   - pin 6)
# 2  (+ power supply)       - RasPi (GPIO +5V   - pin 1)
# 3  VO                     - 
# 4  (RS Data/Command)      - +5V (CHIP SELECT - In serial communication)
# 5  (R/W Read/Write)       - RasPi (Serial data) - GPIO7 (pin26) 
# 6  (E - strobe)           - RasPi (serial CLOCK) - GPIO8 (pin24)
# 7  (Data bit0)            - 
# 8  (Data bit1)            - 
# 9  (Data bit2)            - 
# 10 (Data bit3)            - 
# 11 (Data bit4)            - 
# 12 (Data bit5)            - 
# 13 (Data bit6)            - 
# 14 (Data bit7)            - 
# 15 (PSB)                  - GND - Set serial communication
# 16 (NC)                   -
# 17 (Reset)                - RasPi - GPIO25(pin22)
# 18 (Vout)                 - 
# 19 (Podsvet - A)          - +5V (Or any LED brightness regulator - about 60mA)
# 20 (Podsvet - K)          - RasPi (GPIO GND - pin 6)
*/

#define sData_Pin 27  // (GPIO27)   = DATA (R/W) 
#define sClk_Pin  17  // (GPIO17)   = SCLK (E) 
#define reset_Pin 22  // (GPIO22)   = RESET  (RST)
#define QUICK_SLEEP 1 // microseconds to strobe
#define LONGER_SLEEP 150 // delay microseconds to allow display to process command




//==============================================================

void display128x64::showMapA(void){
    //write mapa [vertical][horizontal][left/right byte]to screen 
	//top half
	for(int j = 0;j<32;j++){
		for(int k = 0;k<8;k++){
			for(int i = 0;i<2;i++){
				std::cout<< std::showbase << std::setw(4) <<std::hex<<mapa[k][j][i];
			}			
		}
		std::cout<<std::dec<<std::endl;		
	}
	//lower half
	for(int j = 0;j<32;j++){
		for(int k = 8;k<16;k++){
			for(int i = 0;i<2;i++){
				std::cout<< std::showbase << std::setw(4) <<std::hex<<mapa[k][j][i];
			}			
		}
		std::cout<<std::dec<<std::endl;		
	}
	
}
void display128x64::showTextMapA(void){
	std::cout<<"***** textmapa ********"<<std::endl;
	for (int i=0;i<8;i++){
			for(int j=0;j<16;j++){
				std::cout<<std::dec<<txtmapa[i][j]<<" ";
		}
		std::cout<<" \n";
	}
	
}
void display128x64::clearMapA(void){
    //write 0s to mapa[16][32][2]
	for(int i = 0;i<2;i++){
		for(int j = 0;j<32;j++){
			for(int k = 0;k<16;k++){
				mapa[k][j][i] = 0x00;
			}			
		}		
	}
}
//=============================================================
//Using delayMicroseconds lets the linux scheduler decide to jump to another process.  Using this function avoids letting the
//scheduler know we are pausing and provides much faster operation if you are needing to use lots of delays.
void display128x64::DelayNanosecondsNoSleep (int delay_nS)
{
	long int start_time;
	long int time_difference;
	struct timespec gettime_now;

	clock_gettime(CLOCK_REALTIME, &gettime_now);
	start_time = gettime_now.tv_nsec;		//Get nS value
	while (1)
	{
		clock_gettime(CLOCK_REALTIME, &gettime_now);
		time_difference = gettime_now.tv_nsec - start_time;
		if (time_difference < 0)
			time_difference += 1000000000;				//(Rolls over every 1 second)
		if (time_difference > (delay_nS ))		//Delay for # nS
			break;
	}
}
//=============================================================
// Function converts written HEX value from a string to an int
int display128x64::convertHexCharToInt(char *strHex){
	int len = strlen(strHex);
	int rev = len-1;
	int l = 0;
	int result = 0;
	const char strHexTable[16] = {'0','1','2','3','4','5','6','7',
	                              '8','9','A','B','C','D','E','F'};
	for(int i = 0;i<len;i++){
		for(l = 0;l<16;l++){
			if (strHex[i] == strHexTable[l]) {
				
				break;
			}		
		}
		result += (l << (rev*4)); //add value found to Result
		rev--;
				
	}
	return result;
}
//===============================================================
void display128x64::loadTextFont(std::string fileName){
    //Had to recreate this function and included the conversion 
    //of reading hex values represented in text from a string   
    
    std::string row;
	std::ifstream fontfile (fileName);
	if (fontfile.is_open())
	{
        int adresafontu = 0;
        std::string delimiter = ",";
        size_t pos = 0;
        std::string token;
		while(getline(fontfile, row)) { // reading a text file
		  //std::cout << row<< std::endl;
		  while ((pos = row.find(delimiter)) != std::string::npos) { //cut out last two letters before the comma
			token = row.substr(0, pos);
			//std::cout << token << std::endl;
			std::string h = token.substr( token.length()-2,2); //convert to c_strings
			char val[3];
			std::strcpy(val, h.c_str());
			font2[adresafontu] = convertHexCharToInt(val); // convert into numeric value (int) 
			adresafontu++;                                 //increase counter
			row.erase(0, pos + delimiter.length());        //eliminate the element from the string to get to the next
		  }
		//std::cout << row << std::endl;
		}
		
	}
	else std::cout << "Unable to open file: "<<fileName<<std::endl;  
  
    fontfile.close();

    
}
//===============================================================

//=============================================================
void display128x64::setupDisplay(void){
	
	pinMode(sData_Pin,OUTPUT); //
	pinMode(sClk_Pin,OUTPUT);  //
	pinMode(reset_Pin,OUTPUT); //
	
	digitalWrite(sData_Pin,LOW); //DATA to "0"
	digitalWrite(sClk_Pin,LOW);  //CLOCK to "0"
	digitalWrite(reset_Pin,LOW); //RESET to "0"
	delay(100);
	digitalWrite(reset_Pin,HIGH); //RESET to "1"
	
	loadTextFont("font2.txt");  // Retrieve an external font from the file
	
    graphic_mode_ON = false;
	text_mode_ON = false;
    clearMapA();
    clearDisplay();

	
}


//==============================================================

//==============================================================
// Set the data pin for serial communication to "0" or "1"
void display128x64::setDataPin(int bit){

    if (bit == 1){
        digitalWrite(sData_Pin, HIGH); //
    }else{
        digitalWrite(sData_Pin, LOW);
    }

}

//==============================================================
// strobe signals the display to read the data bit from data_pin. count = 1 by default
void display128x64::strobe(int count){
	for(int i = 0;i<count;i++){
		digitalWrite(sClk_Pin, HIGH);
		//delayMicroseconds(QUICK_SLEEP);
		DelayNanosecondsNoSleep(300);
		digitalWrite(sClk_Pin, LOW);
		//delayMicroseconds(QUICK_SLEEP);
		DelayNanosecondsNoSleep(300);
	}
}
//==============================================================
//Function to retrieve a single bit from a byte
int display128x64::getBit(unsigned int x, int n){
    return (x >> (n)) & 0x01;
}
//==============================================================
// Function to swap the values of two integer vars           
void display128x64::swapIntVal(int &x,int &y){
            x=x+y; y=x-y; x=x-y;
}
//==============================================================
// Function for sending two bytes to the DDRAM of the Display
// This is primarely used to send characters
void display128x64::send2Bytes(int rs, char byte1, char byte2){
        
    setDataPin(1);                // The beginning of the communication is done with the sychonization sequence of 5x HIGH
    strobe(5);
    
    setDataPin(0);                // Then the RW bit is sent (when set to "0")
    strobe();
    setDataPin(rs);               // Then the RS bit is sent (commands = "0"; data = "1")
    strobe();
    setDataPin(0);                // Followed by zero bit
    strobe();
 
    // Sending upper nibble first Byte
    for(int i = 7;i>3;i--){
        setDataPin(getBit(byte1,i));    // The original code used "bit = (byte1 & (2**i)) >> i"
        strobe();                       // I removed the "bit" var (putted the expression directly at the function)
     }                                                                   

    setDataPin(0);                      // sending 4x "0" to synchronize
    strobe(4);

    //Sending lower nibble first Byte 
    for(int i = 3;i>-1;i--){   
        setDataPin(getBit(byte1,i));
        strobe();
    }
    setDataPin(0);                      // Then the separation sequence is again 4x "0"
    strobe(4);

    // Byte types were sent immediately without "head" (without 5x "1" + RW bit + RS bit + "0")
    // Sending upper nibble second Byte
    for(int i = 7;i>3;i--){
        setDataPin(getBit(byte2,i));
        strobe();
    }
    setDataPin(0);                        // Separation sequence 4x "0"
    strobe(4);

    //Sending lower nibble second Byte 
    for(int i = 3; i>-1;i--){
        setDataPin(getBit(byte2,i));
        strobe();
    }
    setDataPin(0);                        // Last separating sequence 4x "0"
    strobe(4);


}
//==============================================================
// Function to send one byte to the display
// Often used for commands 
// Commands have different delay requirements as the ST7920 will not
// accept any command if the previous is not finished
void display128x64::sendByte(int rs, char byte){

    setDataPin(1);                // the beginning of the communication is done with the "synchro" sequence of 5 singles
    strobe(5);
    
    setDataPin(0);                    // Then the RW bit is sent (when set to "0")        
    strobe();                                                                                  
    setDataPin(rs);                    // Then send the RS bit (commands = "0"; data = "1")            
    strobe();                                                                                                                                           
    setDataPin(0);                    // Followed by zero bit                                                        
    strobe();
    
    //send upper nibble
    for(int i=7;i>3;i--){
        setDataPin(getBit(byte,i));
        strobe();
    }
    setDataPin(0);                     // Then the separation sequence is sent 4x "0"
    strobe(4);

    //send lower nibble
    for(int i=3;i>-1;i--){
        setDataPin(getBit(byte,i));
        strobe();
    }
    setDataPin(0);                      //To restart the separation sequence 4x "0"
    strobe(4);
    if (rs == 0) delayMicroseconds(LONGER_SLEEP); //commands require some processing time (avg 72 microseconds per datasheet)
}

//==============================================================
// Function to change a pixel in the graphical Table of the display
// If use_memPlot is true, the change becomes only visible after MemDump()
// 1-point display / deletion / inversion at posX coordinates (0 to 127) and posY (0 to 63)
void display128x64::plot(int posX, int posY, int style, bool use_memPlot){
    unsigned int leftByte;
    unsigned int rightByte;
    // Checking for the correct range of coordinates and optionally adjusting them to the extreme values
    // You can comment these, if you sure your program won't need them. It speeds the code a little
    if (posX > 127) posX = 127;
    if (posX < 0  ) posX = 0;
    if (posY > 63 ) posY = 63;
    if (posY < 0  ) posY = 0;

    //translation of x/y coordinates into display memory positions
    int x_Block = posX >> 4; // determines in what block of 16 (two bytes) the horizontal position is in the display memory
    int x_Pos = posX & 15;   // the position in the block
    if (posY >= 32){         // the vertical pixels are devided in two memory tables of 32 lines.  
        posY -= 32;          // the second table starts at 0 again.
        x_Block += 8;        // to move to second table, the vertical address is increased by 128 (8 x 16)
    }
    
    //getting current state from memory
    unsigned int originalLeftByte  = mapa[x_Block][posY][0];
    unsigned int originalRightByte = mapa[x_Block][posY][1];
    
    //adjust left byte 
    if (x_Pos < 8){
        
        if (style == 1){  // Draw a point
            leftByte = (0b10000000 >> x_Pos) | originalLeftByte;
		}
        if (style == 0){  // Delete point
            leftByte = ~(0b10000000 >> x_Pos) & originalLeftByte;
		}
        else{  // invert point
            leftByte = (0b10000000 >> x_Pos) ^ originalLeftByte;
		}
        mapa[x_Block][posY][0] = leftByte;
        rightByte = originalRightByte;
    }    
    else{ //adjust right byte
        
        if (style == 1){  // Draw a point
            rightByte = (0b10000000 >> (x_Pos-8)) | originalRightByte;
		}
        else if (style == 0){  // Delete point
            rightByte = ~(0b10000000 >> (x_Pos-8)) & originalRightByte;
		}
        else{  // Invert point
            rightByte = (0b10000000 >> (x_Pos-8)) ^ originalRightByte;
		}
        mapa[x_Block][posY][1] = rightByte;
        leftByte = originalLeftByte;
	}
	
	//to plot out directly
	if(!use_memPlot){
		send2Bytes (0, 0b10000000 + posY, 0b10000000 + x_Block);  // Setting the graphics address
		send2Bytes(1, leftByte, rightByte);
	}
    
    
}


//==============================================================
// One of the 4 defined 16 x 16 pixel icons at position [x, y]
// X is in the range 0 to 7 (translated to 0, 16, 32, 48, 64, 80, 96, 112)
// Y ranges from 0 to 3 (translated to 0, 16, 32, 48)
void display128x64::printIcon(int iconId, int x, int y){
	if(!text_mode_ON) initTextMode();
    int shift = x;
    if (y == 1) shift = shift + 16;
    if (y == 2) shift = shift + 8;
    if (y == 3) shift = shift + 24;
    sendByte(0, 0b10000000 + shift);  // Address Counter to the required position
    send2Bytes(1, 0, iconId * 2);
}
//==============================================================
// Draw a horizontal line point by point
void display128x64::drawHorizontalLine(int posY, int fromX , int toX, int style, bool use_memPlot){  
   
    //for posX in range(fromX, toX + 1):
    for(int posX = fromX;posX<toX+1;posX++){
        
			plot(posX, posY, style, use_memPlot);
		
	}
}

//==============================================================
// Draw horizontal line from the edge to the edge after the bytes
void display128x64::drawHorizontalLine2(int posY, int fromByte, int toByte, char pattern){  
    if(!graphic_mode_ON) initGraphicMode();
    int shift = fromByte;
    if (posY >= 32){
        posY = posY - 32;
        shift = shift + 8; 
	}   

    send2Bytes( 0, 0b10000000 + posY, 0b10000000 + shift );     
    //for r in range(toByte - fromByte + 1):
    for(int r = 0;r<(toByte - fromByte + 1);r++){
        send2Bytes(1, pattern, pattern) ;    
        mapa[shift + r][posY][0] = pattern; 
        mapa[shift + r][posY][1] = pattern; 
    }
}    
//==============================================================
// Draw a vertical line using bit masks
// NOTICE: if given a pattern != 255 and if style == 2, style = 1.
void display128x64::drawVerticalLine(int posX, int fromY, int toY, int style, char pattern, bool use_memPlot){

        
    int bitSelector = 0;                              // The bit position in the pattern
    int pixelStyle = style;                // Gives it the style value, as plot uses the pixelStyle var.
    //for posY in range(fromY , toY + 1):
    for(int posY = fromY;posY<toY+1;posY++){
        if (pattern != 255){
            int bitValue = pattern & (0b10000000 >> (bitSelector % 8));  // According to the given pattern, selects individual bits
                                                                     // Which will either be displayed or deleted on the display
            if (bitValue == 0){                      
                pixelStyle = 0;
			}
            else{
                pixelStyle = 1;
			}
                
            if (style == 0){  // inverts the pixelStyle if the style is == 0 (delete)
                pixelStyle = !pixelStyle;
			}
            bitSelector++;
        } 
		
			plot(posX, posY, pixelStyle, use_memPlot);
		  
	    
	}       
}

//==============================================================
// Print position setting for text mode (for characters 8x16 point)
// Column (0 to 15) and line (0 to 3)
void display128x64::setTextCursorPos(int column , int row){  
    int shift = column;
    if (row == 1) shift = column + 32;
    if (row == 2) shift = column + 16;
    if (row == 3) shift = column + 48;
    if (row>3) {std::cout<<"Error: Max Row = 3\n";return;}

    sendByte( 0, 0b10000000 + int(shift / 2));  // Address Counter na pozadovanou pozici

    // In the case of --lichen-- columns, the printCharGraphicMode must be filled in with the printCharGraphicMode on the display before the new printout
    if ((column % 2) != 0){                                                    //If column is odd
        char orignal_character = txtmapa[row][column - 1]; // "Predcharacter" is determined from the auxiliary text memory

        sendByte(1, orignal_character); 
	}
 }  
//==============================================================
// Displaying one character from the 8x8 point font
// char_PosX = x position (0 to 15, translated to multiples of 8 up to 128), posY = 0 to 63
void display128x64::printCharGraphicMode(char ascII_Char, int char_PosX, int posY, bool invert, bool use_memPlot){  
	// display requires graphic mode. so check if this was done
    if(!graphic_mode_ON) initGraphicMode();
    // Control of boundrys
    if (ascII_Char <  32) ascII_Char = 32;
    if (ascII_Char > 255) ascII_Char = 255;
    if (char_PosX <  0) char_PosX = 0;  
    if (char_PosX > 15) char_PosX = 15;  
    if (posY > 63) posY = 63;
    if (posY <  0) posY = 0;

    ascII_Char = ascII_Char - 32;     // In the font file, the first printable char starts with 0
    int posX = char_PosX * 8;     

    // Drawing the character
    unsigned int leftByte, rightByte, originalLeftByte, originalRightByte;
    int x_Block, x_Pos;
    		//translation of x/y coordinates into display memory positions
		x_Block = posX >> 4;     // determines in what block of 16 (two bytes) the horizontal position is in the display memory
		x_Pos = posX & 15;       // the position in the block
        
    // The character from the font will be gradually transferred byte by byte to the display in 8 steps (top down)
	for(int adr_font = ascII_Char * 8; adr_font<(ascII_Char * 8) + 8;adr_font++){  
		

		if (posY >= 32){         // the vertical pixels are devided in two memory tables of 32 lines.  
			posY -= 32;          // the second table starts at 0 again.
			x_Block += 8;        // to move to second table, the vertical address is increased by 128 (8 x 16)
		}
		
		// To find out the current status of the two-byte on the display
        originalLeftByte  = mapa[x_Block][posY][0];  
        originalRightByte = mapa[x_Block][posY][1];

        if(x_Pos < 8){         // When the x_Pos is <8, change only the left byte from the double byte
            if (!invert){  // Normal text 
                leftByte = font2[adr_font];
			}
            else {   // Inverse text 
                leftByte = ~font2[adr_font];
            }    
                
            rightByte = originalRightByte;   // right byte stays unchanged
        }
        else{  // When the x_Pos >= 8, change only the right byte 
            if (!invert ){  // Normal text 
                rightByte = font2[adr_font];
			}
            else{   // Inverse text 
                rightByte = ~font2[adr_font];
			}

            leftByte = originalLeftByte;     // The left byte will be unchanged
        }
        //store values in memory
        mapa[x_Block][posY][0] = leftByte;        
        mapa[x_Block][posY][1] = rightByte;
        
        //to plot directly
        if(!use_memPlot){
			send2Bytes(0, 0b10000000 + posY, 0b10000000 + x_Block);  // Setting the graphics address
			send2Bytes(1, leftByte, rightByte);       // Prepress the two bytes to enter the address in the display
		}
        
        posY = posY + 1;   // Change to the next line
	}
}
//==============================================================
// Displaying several characters of the 8x8 font
// bytePosX = the position of the first printCharGraphicMode in the text is in column 0 to 15; posY = 0 to 63 (upper margin of the printCharGraphicMode)
void display128x64::printStringGraphicMode(std::string str, int bytePosX, int posY, bool invert, bool use_memPlot){  

    int len = str.length();
    for(int i = 0;i < len;i++){
        if (bytePosX < 16){ // Screen overlay
	    	std::string singleChar = str.substr(i,1);
		char sc[2];
		std::strcpy (sc,singleChar.c_str());
            printCharGraphicMode(sc[0], bytePosX, posY, invert, use_memPlot); 			
            bytePosX = bytePosX + 1;
	}
    }
}
//==============================================================
// Center the text in a given width (default 16)
std::string display128x64::centered( std::string const& original, int targetSize )
{
    int len = original.length();
    if(targetSize < len) { return original; }

    int diff = targetSize - len;
    int pad1 = diff/2;
    int pad2 = diff - pad1;
    return std::string(pad1, ' ') + original + std::string(pad2, ' ');
}
    


//==============================================================
// Prints multiple characters in text mode: font (8x16 point)
// Font definition is a part of the ROM in the display
// column = Initial column where the string will begin to print [0 to 16]
// row is in the range [0 to 3]
void display128x64::printStringTextMode(std::string str, int column, int row){ 
    if(!text_mode_ON) initTextMode();
    if (str.length() + column > 16){    // If the line is longer than 16 characters,
        str = str.substr (0,16 - column);  // ... so the end is cut off
    }
    //std::cout<<"truncutted text: "<<str<<std::endl;
    setTextCursorPos(column, row);     // The start position of the text is sent to the display
    for(unsigned int i = 0;i<str.length();i++){
		std::string singleChar = str.substr(i,1);
		char sc[2];
		std::strcpy (sc,singleChar.c_str());
		//std::cout<<"sending byte: "<<sc<<std::endl;
		
        sendByte(1,sc[0]);            // send the character to the display 
        txtmapa[row][i] = sc[0];      // Memory for text mode
    }
}

//==============================================================
// Prints a single character in text mode
void display128x64::printCharTextMode(char cByte, int bytePosX, int row){ 
    if(!text_mode_ON) initTextMode();
    setTextCursorPos(bytePosX, row);   // The start position of the text is sent to the display
    sendByte(1, cByte);                // The printCharGraphicMode code is sent to the display
    txtmapa[row][bytePosX] = cByte;
}
//============================================================== 
// Draws a line from and to the specified coordinates.
// Based on Bresenham's line algorithm (https://en.wikipedia.org/wiki/Bresenham's_line_algorithm)       
void display128x64::drawGenericLine2(int x0, int y0, int x1, int y1, int style, bool use_memPlot){
    int dx,dy, yi, xi, D, x,y;
	bool plotLineHigh;
	if (abs(y1 - y0) < abs(x1 - x0)){
		plotLineHigh = false;
		if (x0 > x1){
            swapIntVal(x0,x1);
            swapIntVal(y0,y1);
		}
	}else{
		plotLineHigh = true;
		if (y0 > y1){			
            swapIntVal(x0,x1);
            swapIntVal(y0,y1);
		}	
	}
	
	if(plotLineHigh == false){ //plotLineLow		
	    dx = x1 - x0;
		dy = y1 - y0;
		yi = 1;
		if (dy < 0){
			yi = -1;
			dy = -dy;
		}
		D = (2 * dy) - dx;
		y = y0;

		for (x = x0; x<x1; x++){
				plot(x, y, style, use_memPlot);
			
			if (D > 0){
				y = y + yi;
				D = D + (2 * (dy - dx));
			}
			else{
				D = D + 2*dy;
			}	
		}
		
		
	}else{                  //plotLineHigh	
		dx = x1 - x0;
		dy = y1 - y0;
		xi = 1;
		if (dx < 0){
			xi = -1;
			dx = -dx;
		}
		D = (2 * dx) - dy;
	    x = x0;

		for (y = y0; y<y1; y++){
				plot(x, y, style, use_memPlot);
			
			if (D > 0){
				x = x + xi;
				D = D + (2 * (dx - dy));
			}
			else{
				D = D + 2*dx;
			}	
		}			
	}	
}
//==============================================================
// New function.
// Draws a line from and to the specified coordinates.
// Based on this code: http://itsaboutcs.blogspot.com.br/2015/04/bresenhams-line-drawing-algorithm.html
// NEW FUNCTION!
void display128x64::drawGenericLine(int fromX, int fromY, int toX, int toY, int style, bool use_memPlot){

        
    // Bresenham Line Drawing Algorithm For All Kind Of Slopes Of Line
    int slope = 0;
    int dx = abs(fromX - toX);
    int dy = abs(toY - fromY);
    bool inverted;
    if (float(dx) == 0) {
		slope = 10;
	}
    else{ 
		slope = dy/float(dx);
	}
    
    int x = fromX;
    int y = fromY;   

        plot(x, y, style, use_memPlot);
	

    
    
    // checking the slope if slope > 1 
    // then interchange the role of x and y
    if (slope > 1){
        inverted = true;
        swapIntVal(dx, dy);
        swapIntVal(x, y);
        swapIntVal(toX, toY);
	}
    else{ 
		inverted = false;
	}
    // initialization of the inital decision parameter
    int p = 2 * dy - dx;
    
    //for k in range(2, dx):
    for(int k = 2;k<dx;k++){
        if (p > 0){
			if(y < toY){
				y++;
            }
            else{
				 y--;
			 }
            p = p + 2*(dy - dx);
		}
        else{
            p = p + 2*dy;
		}
		if (x < toX){
			x++;
		}else{
			x--;
		} 
        
        
        if (inverted){
            swapIntVal(x,y);// swap values in x and y
           
        }    

		plot(x, y, style, use_memPlot);
		 
         
	}
}            
//==============================================================
// New function.
// Draws a rectangle. If fill, all the internal part will be painted.
// NEW FUNCTION!
void display128x64::drawRectangle(int fromX, int fromY, int toX, int toY, bool fill, int style, bool use_memPlot){    
    //if(!graphic_mode_ON) initGraphicMode();
    int xPos,yPos;
             
    if (fromX > toX) swapIntVal(fromX, toX);
    if (fromY > toY) swapIntVal(fromY, toY);
    
    //for xPos in range (fromX, toX + 1):
    for(xPos = fromX;xPos<=toX;xPos++){
		plot(xPos, fromY, style, use_memPlot);
		plot(xPos, toY, style, use_memPlot);
		
    }    
    //for yPos in range (fromY + 1, toY):
    for(yPos = fromY+1;yPos<toY;yPos++){
		plot(fromX, yPos, style, use_memPlot);
		plot(toX, yPos, style, use_memPlot);
		
    }    
    if (fill){
        //for yPos in range (fromY + 1, toY):
        for(yPos = fromY+1;yPos<toY;yPos++){
            //for xPos in range (fromX + 1, toX):
            for(xPos = fromX+1;xPos<toX;xPos++){
				plot(xPos, yPos, style, use_memPlot);
				
			}
		}
     }   
}    
//==============================================================
//==============================================================
// New function.
// The arguments are self-explaining.
// Increasing stepDegree increases the speed of drawing, but may result in missing pixels.
// NEW FUNCTION
void display128x64::drawCircle(int circleCenterX, int circleCenterY, int radius, int startDegree, int stopDegree, int stepDegree, int style, bool use_memPlot){

        
    //for degree in range (startDegree, stopDegree + 1, stepDegree):
    for(int degree = startDegree;degree<=stopDegree;degree+=stepDegree){
        //int posX = int(round(cos(radians(degree)) * radius + circleCenterX));
        int posX = int(round(cos((degree*M_PI)/180) * radius + circleCenterX));
        int posY = int(round(sin((degree*M_PI)/180) * (- radius) + circleCenterY));
        
		plot(posX, posY, style, use_memPlot);
		
        
	}
}  
    
//==============================================================
// New function.
// Draws a line like a clock hand, where you enter the initial coordinate, the angle
// in degrees and the radius (the size of the line)
// NEW FUNCTION!
void display128x64::drawRadiusLine(int fromX, int fromY, int degree, int radius, int style, bool use_memPlot){
	if(!graphic_mode_ON) initGraphicMode();
    int targetX = int(cos((degree*M_PI)/180) * radius + fromX);
    int targetY = int(sin((degree*M_PI)/180) * radius + fromY);
    drawGenericLine2(fromX, fromY, targetX, targetY, style, use_memPlot);
}


/*
//==============================================================
// Prints a string with the font 3x5 pixels.
//   rotation = 0: no change in the text.
//   rotation = 1: text is turned 90degrees counter-clockwise (and keeps writing up)
//   rotation = 2: text is turned 180degrees counter-clockwise (and keeps writing left)
//   rotation = 3: text is turned 270degrees counter-clockwise (and keeps writing down)
// leftX and topY are the "top-left" position of the first char that you want to print.
// This function can be improved/optimized a lot. As I need this for yesterday, will leave the improvements for later.
// As I usually prefer Performance over code readbility, I copied and changed the plotting section, to meet different rotations.
// NEW FUNCTION!
def printString3x5(string, leftX, topY, rotation = 0, use_memPlot = 0):
    if use_memPlot:
        customPlot = memPlot
    else:
        customPlot = plot
        
    actualX = leftX
    actualY = topY
    firstRun = True
    
    if not rotation:       
        for char in string:
            char = ord(char)
            
            if (32 > char or char > 126): char = 127
            
            char -= 32
                
            // Draw the separator (a blank column) between the chars
            if (not firstRun):
                for row in range (5):
                    customPlot(actualX, actualY + row, 0) 
                actualX += 1
            else:
                firstRun = False
                
            for column in range(len(char3x5[char])):
                for row in range(len(char3x5[char][column])):
                    customPlot(actualX, actualY + row, char3x5[char][column][row])
                actualX += 1
                

    elif rotation == 1:// if rotation == 3 or any other number not cased in previous conditionals
        for char in string:
            char = ord(char)

            if (32 > char or char > 126): char = 127
            
            char -= 32
                
            // Draw the separator (a blank column) between the chars
            if (not firstRun):
                for row in range (5):
                    customPlot(actualX + row, actualY, 0) 
                actualY -= 1
            else:
                firstRun = False
                
            for column in range(len(char3x5[char])):
                for row in range(len(char3x5[char][column])):
                    customPlot(actualX + row, actualY, char3x5[char][column][row])
                actualY -= 1
                
    elif rotation == 2:      
        for char in string:
            char = ord(char)
            
            if (32 > char or char > 126): char = 127
            
            char -= 32
                
            // Draw the separator (a blank column) between the chars
            if (not firstRun):
                for row in range (5):
                    customPlot(actualX, actualY - row, 0) 
                actualX -= 1
            else:
                firstRun = False
                
            for column in range(len(char3x5[char])):
                for row in range(len(char3x5[char][column])):
                    customPlot(actualX, actualY - row, char3x5[char][column][row])
                actualX -= 1
                
    else: // if rotation == 3 or any other number not cased in previous conditionals
        for char in string:
            char = ord(char)

            if (32 > char or char > 126): char = 127
            
            char -= 32
                
            // Draw the separator (a blank column) between the chars
            if (not firstRun):
                for row in range (5):
                    customPlot(actualX - row, actualY, 0) 
                actualY += 1
            else:
                firstRun = False
                
            for column in range(len(char3x5[char])):
                for row in range(len(char3x5[char][column])):
                    customPlot(actualX - row, actualY, char3x5[char][column][row])
                actualY += 1
            
*/




      
//==============================================================
// Uploading a two-color BMP image of a 128x64 point to a variable map []
// CAUTION: Without any test for a correct BMP file format!
void display128x64::loadBMP12864(std::string fileName){
    //fileBMP = open(imageRelativePath, "rb")  // Load an image into a data variable []
    if(!graphic_mode_ON) initGraphicMode();
    int posY, shift;
    char rightByte, leftByte;
    char data[3000];
    std::ifstream fileBMP;
    fileBMP.open(fileName,std::ios::binary);
    if(fileBMP.is_open()){
		for(int i = 0;i<3000;i++){
			fileBMP.get(data[i]);
		}
		
		
		//data = fileBMP.read()
	}else{
		throw std::runtime_error("Unable to open the input image file.");
	}  
    fileBMP.close();                          // File closure

    // The detailed BMP master file specification is here:
    // http://www.root.cz/clanky/graficky-format-bmp-pouzivany-a-pritom-neoblibeny
    // The start of the image data determines 4 bytes in a file in positions 10 to 13 (ten) from the beginning of the file
    //zacatekdat = ord(data[10]) + (ord(data[11]) * 256) + (ord(data[12]) * 65536) + (ord(data[13]) * 16777216)
    long byte = data[10] + (data[11]*256) + (data[12] * 65536) + (data[13] * 16777216);
    //byte = zacatekdat

    //for mikrorow in range (63, -1, -1):  // Read data variables [] byte after byte and store in memory (map [])
    for(int mikrorow = 63;mikrorow>-1;mikrorow--){
        posY = mikrorow;
        if (mikrorow > 31){
            posY = posY - 32;
            shift = 8;
		}
        else{
            shift = 0;
		}
        send2Bytes(0, 0b10000000 + posY, 0b10000000 + shift);  // Setting the graphics address
        //for column in range (8):
        for(int column = 0;column<8;column++){
            leftByte = data[byte];
            rightByte = data[byte+1];
            send2Bytes(1, leftByte, rightByte) ;    // 
            mapa[column+shift][posY][0] = leftByte;
            mapa[column+shift][posY][1] = rightByte;

            byte = byte + 2;          // Passes to another double-out of graphical data
		}
     }
}



//==============================================================
// Overwrite the graphical memory (variable map []) to the display
void display128x64::memDump(){
    if (!graphic_mode_ON) initGraphicMode();
    //for mikrorow in range(32):
    for(int mikrorow=0;mikrorow<32;mikrorow++){
        send2Bytes( 0, 0b10000000 + mikrorow , 0b10000000 );  // Setting the graphics address
        //for horizontal in range (16):
        for(int horizontal = 0;horizontal<16;horizontal++){
            char leftByte  = mapa[horizontal][mikrorow][0] ;
            char rightByte = mapa[horizontal][mikrorow][1] ;
            send2Bytes( 1, leftByte, rightByte); 
		}   
    }         
} 
//==============================================================
// Set the display to the graphic mode
void display128x64::initGraphicMode(){  
    sendByte(0, 0b00110010);  // function set (8 bit)
    sendByte(0, 0b00110110);  // function set (extend instruction set)
    sendByte(0, 0b00110110);  // function set (graphic ON)
    sendByte(0, 0b00000010);  // enable CGRAM after being reset to BASIC instruction set
    graphic_mode_ON = true;

}

//==============================================================
// Set the display to text mode
void display128x64::initTextMode(){  
    sendByte(0, 0b00110000);  // function set (8 bit)
    sendByte(0, 0b00110100);  // function set (extend instruction set)
    sendByte(0, 0b00110110);  // function set (graphic OFF)
    sendByte(0, 0b00000010);  // Enable CGRAM (after reset to BASIC instruction set)
    sendByte(0, 0b00110000);  // function set (basic instruction set)
    sendByte(0, 0b00001100);  // displ.=ON , cursor=OFF , blink=OFF
    sendByte(0, 0b10000000);  // Address Counter na left horni roh
    text_mode_ON = true;
    graphic_mode_ON = false;
}           

//==============================================================
// Delete graphical display time
void display128x64::clearGraphic(char pattern){ 
    initGraphicMode();
    sendByte(0, 0b00110110);  // function set (extend instruction set)
    sendByte(0, 0b00110100);  // function set (graphic OFF) 
    sendByte(0, 0b00001000);  // displ.=OFF , cursor=OFF , blink=OFF
    
    //for vertical in range(32):
    for(int vertical =0;vertical<32;vertical++){ 
        send2Bytes(0, 0b10000000 + vertical, 0b10000000 );  // Setting the address on the display at the beginning of the micro-bar
        //for horizontal in range (16):
        for(int horizontal = 0;horizontal<16;horizontal++){
            send2Bytes(1, pattern, pattern);     // Double-bits fill the entire display with the same code   
 
            // And this code still writes to individual positions in the cache
            mapa[horizontal][vertical][0] = pattern ;   
            mapa[horizontal][vertical][1] = pattern ;
		}
    }        
    sendByte( 0, 0b00110110);  //function set (graphic ON) - smazany displej se zobrazi okamzite
    graphic_mode_ON = false;
    text_mode_ON = false;
}
//==============================================================
// Delete the text part of the display
void display128x64::clearText(){ 
	if(!text_mode_ON) initTextMode();
    sendByte(0, 0b00110000);  // function set (8 bit)
    sendByte(0, 0b00110000);  // function set (basic instruction set)
    sendByte(0, 0b00001100);  // displ.=ON , cursor=OFF , blink=OFF
    sendByte(0, 0b00000001);  // clear 

    // Erase the text memo
    for(int i = 0;i<8;i++){
		for(int j = 0;j<16;j++){
			txtmapa[i][j] = ' ';
		}
	}
}


//==============================================================
// Delete the graphical text and the text part of the display.
// CAUTION: the display is set in the TEXT mode
void display128x64::clearDisplay(char pattern){
    clearGraphic(pattern);
    clearText();
}




//==============================================================
// Definition of a graphical shape of 4 icons with the size of 16x16 points
// Icon = Number Icons 0 to 3
// iconData = 16-byte double-byte value
void display128x64::defineIcon(int iconId,int * iconData){
    if (!text_mode_ON) initTextMode();
    sendByte(0, 64 + (iconId * 16) );  // Setting the graphics address
    //for dat in range(16):
    for(int dat = 0;dat<16;dat++){
        char leftByte  = iconData[dat] / 256;
        char rightByte = iconData[dat] % 256;
        send2Bytes(1, leftByte, rightByte);
	}
}
//==============================================================
// Blink cursor after the last printCharGraphicMode entered in text mode
// Is designed for Chinese characters, so a 16x16-dot large square
// (It's useless for European characters (8x16 points))
void display128x64::blinkLastChineseChar(bool state){
    if (!text_mode_ON) initTextMode();
    if (state){
        sendByte(0, 0b00001111);  // displ.=ON , cursor=ON , blink=ON
    }else{
        sendByte(0, 0b00001100);  // displ.=ON , cursor=OFF , blink=OFF
	}
}
//==============================================================
// Hide all active pixels on display, but doesn't remove them from memory.
// If you hide them (True or any value except 0), and then show them again (False or 0),
// all active pixels will show again, on the same place.
// I was trying to make a function to turn off the light from the display,
// but doesn't look possible.
void display128x64::hideShowDisplay(bool state){
    if (!text_mode_ON) initTextMode();
    if (state){
        sendByte(0, 0b00001100);  // displ.=ON , cursor=OFF , blink=OFF
    }else{
        sendByte(0, 0b00001000);  // displ.=OFF , cursor=OFF , blink=OFF
	}

}
//==============================================================
// A weird command. It's like the hideShowDisplay(), but idk right
// what it does.
void display128x64::standby(){
    sendByte(0, 0b00110000);  // function set (8 bit)
    sendByte(0, 0b00110100);  // function set (extend instruction set)
    sendByte(0, 0b00000001);  // Standby command
}    
void display128x64::cursorON(){
	if(!text_mode_ON) initTextMode();
	sendByte(0, 0b00001101);  // displ.=ON , cursor=ON , blink=ON
	sendByte(0, 0b00000011);  // shift Cursor
}
//==============================================================
// Set the position to the appropriate column (0 to 7) and the row (0 to 3)
// For big characters and Icons (16x16 points)
void display128x64::setIconPos(int column, int row){ 

    int shift = column;
    if (row == 1) shift = column + 16;
    if (row == 2) shift = column + 8;
    if (row == 3) shift = column + 24;

    sendByte(0, 0b10000000 + shift);  // Address Counter to the desired position
}


//==============================================================
// Function to scroll through the loaded Font2.
//int font2[2048]; 
void display128x64::scrollFont2(){
	
  clearDisplay();
  char a[17]; 
  char b[] ={")"};
  char ch[4], next;
  
  for(int i= 0;i<256;i+=6){
	  for(int j = 0;j<6;j++){
	  sprintf(ch,"%d",(i+j));
	  std::strcpy(a,"chr(");
	  std::strcat(a,ch);
	  std::strcat(a,b);
	  printStringGraphicMode(a,1,j*10+2);
	  printCharGraphicMode((i+j),10,j*10+2);	  
	 } 
		 std::cin>>next;		
	 }	
	 
 
 
 

	
}





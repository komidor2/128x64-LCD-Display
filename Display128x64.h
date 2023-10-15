#ifndef DISPLAY128X64_H
#define DISPLAY128X64_H

#include <string>


/*
# ==============================================================
# Functions Index
# ==============================================================
# Note1: these translations are not very good.
# Note2: the Y position is inverted (like most (all?) displays), so the coordinate (0,0) is the top left.
#
# init()
#   Basic GPIO port settings - just run it once at the beginning of the program
#
# initTextMode()
#   Switches the display to text mode.
#   NOTE: However, look like to use functions that use plot(), you need to initTextMode.
#
# initGraphicMode()
#   Switches display to graphic mode.
#
# clearText()
#   Deletes the content of the text part of the display (the graphics portion remains unchanged)
#
# clearGraphic(pattern)
#   Fills the entire contents of the graphical part of the display by the byte.
#   (When 0x00 is deleted, 0xFF will fill it with white dots, other values will
#   fill the display with different vertical lines). The text part of the display remains unchanged.
#
# clearDisplay()
#   Performs both previous deletions at the same time. After returning the display is switched to text mode.
#
# defineIcon(iconId, iconData)
#   Defines one of four custom icons
#   iconId = icon identifier number 0 to 3
#   iconData = a variable that contains an array: 16x double-byte value
#
# printIcon(iconId, x, y)
#   The [x, y] coordinates print one of the four custom icons.
#   iconId = Icon identifier number 0 to 3
#   x = column 0 to 7
#   y = line 0 to 3
#
# ==== Graphic font 8x8 pixels
#   printCharGraphicMode(code, x, supers, inversion)
#     Displays one printCharGraphicMode with the ASCII code "code" at the "x" (0 to 15) coordinates,
#     posY (0 to 63) - the y position to
#     When "inverse" = True, a dark printCharGraphicMode appears on a light background.
#
#   printStringGraphicMode(text, x, supers, inversion)
#     Displays text (multiple characters) at "x" and "supers" (parameters same as "printCharGraphicMode ()").
#
# ==== Inside display font
#   printCharTextMode(code, x, y) 	Displays one printCharGraphicMode in text mode at [x, y].
#     Code is in the range 1 to 127 (from 32 to 126 it is a classic ASCII)
#     X is from 0 to 15
#     Y is a line 0 to 3
#
#   printStringTextMode(string, x, y)
#     Use large characters to display text on the display. Parameters are the
#     same as for "big_set ()" and apply to the first printCharGraphicMode of the text.
#
#
# plot(posX, posY, style)
#   At "posX" (0 to 127) and "posY" (0 to 63), it drawns, deletes,
#   or inverts one pixel. If style = 0 , point deletion is performed, 
#   style = 1 is drawn, and style = 2 changes the point status on the display.
#
# memPlot(posX, posY, style)
#   The same function as the previous "plot()", but the points do not
#   appear directly on the display, only in the temporary storage space.
#   This feature allows faster drawing. After use, however, it is necessary to transfer
#   the contents of that temporary memory to the display using the "memDump()" function.
#
# memDump()
#   Transfer the memory contents to the display after using the memPlot() command.
#
# drawHorizontalLine(posY, fromX, toX, style, use_memPlot = 0)
#   Drawing a simple horizontal line at a "supery" distance from the top edge with the
#   ability to define the beginning and end of the line (variables "from" to "to"). 
#   The "style" parameter is the same as the " plot() " function.
# All the functions that have the use_memPlot argument, can use the memPlot() instead of plot() (read memPlot() explanation) 
#
# drawHorizontalLine2(posY, fromX, toX, pattern)
#   Faster drawing of the horizontal line. In this case, the parameters "fromX" and "toX"
#   are in the range 0 to 7 (they are sixteen pixels on the display). Therefore,
#   the line can begin and end only at the coordinates on which the icons are printed.
#   The minimum length of such a line is 16 points. The "pattern" parameter specifies
#   the style of the line. Depending on the individual bits of that parameter, you can
#   set the line full, dashed, dotted, dashed ...
#
# drawVerticalLine(posX, fromY, toY, pattern, use_memPlot = 0)
#   Vertical line at arbitrary coordinates.
#   posX = X line coordinates in the range 0 to 127
#   fromY, toY = y coordinates of the beginning and end of the line in the range 0 to 63
#   The "pattern" parameter is the same as in the previous case.
# All the functions that have the use_memPlot argument, can use the memPlot() instead of plot() (read memPlot() explanation) 
#
# loadBMP12864(fileName)
#   Load a two-color image from the file into the display.
#   Beware of the correct file format!
#
# sendByte(rs, byte) 	Sends 1 byte to the display.
#   The data (1) or the command (0) register is selected using the "rs" parameter.
#
# send2Bytes(rs, byte1, byte2)
#   It sends 2 bytes at a time.
#   The data (1) or the command (0) register is selected using the "rs" parameter.



# ==== NEW FUNCTIONS BELOW!! ====
# They are not present on the original code. They aren't so complicated, but are handy.

# drawGenericLine(fromX, fromY, toX, toY, style = 1, use_memPlot = 0)
#   Draws a line from and to the specified coordinates.
#   Based on this code: http://itsaboutcs.blogspot.com.br/2015/04/bresenhams-line-drawing-algorithm.html
# All the functions that have the use_memPlot argument, can use the memPlot() instead of plot() (read memPlot() explanation) 
#
# drawCircle(circleCenterX, circleCenterY, radius, startDegree = 0, stopDegree = 360, stepDegree = 1, style = 1, use_memPlot = 0):
#   The arguments are self-explaining.
#   Increasing stepDegree increases the speed of drawing, but may result in missing pixels.
# All the functions that have the use_memPlot argument, can use the memPlot() instead of plot() (read memPlot() explanation) 
#   
# drawRadiusLine(fromX, fromY, degree, radius, style = 1, use_memPlot = 0):
#    Draws a line like a clock hand, where you enter the initial coordinate, the angle
#    in degrees and the radius (the size of the line)
# All the functions that have the use_memPlot argument, can use the memPlot() instead of plot() (read memPlot() explanation) 
#
# hideShowDisplay(state)
#    Hide all active pixels on display, but doesn't remove them from memory.
#    If you hide them (True or any value except 0), and then show them again (False or 0),
#    all active pixels will show again, on the same place.

# printString3x5(string, leftX, topY, rotation = 0, use_memPlot = 0):
#   Prints a string with the font 3x5 pixels.
#   rotation = 0: no change in the text.
#   rotation = 1: text is turned 90degrees counter-clockwise (and keeps writing up)
#   rotation = 2: text is turned 180degrees counter-clockwise (and keeps writing left)
#   rotation = 3: text is turned 270degrees counter-clockwise (and keeps writing down)
#   leftX and topY are the "top-left" position of the first char that you want to print.
# All the functions that have the use_memPlot argument, can use the memPlot() instead of plot() (read memPlot() explanation) 
# ==============================================================


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
           
//==============================================================
class display128x64
{
private:
    int mapa[16][32][2];            // Memory to store the current status of the displayed pixels on the display
	int txtmapa[8][17];             // Memory to which the current text status on the display is saved
	int font2[2048];                // The memory in which the font is stored that is retrieved from the external file
	bool graphic_mode_ON;
	bool text_mode_ON;
	
	void DelayNanosecondsNoSleep (int delay_us);
	int convertHexCharToInt(char *strHex);
	void loadTextFont(std::string fileName);	
	void setDataPin(int bit);
	void strobe(int count = 1);
	int getBit(unsigned int x, int n);    
	void swapIntVal(int &x,int &y);	
	void sendByte(int rs, char byte);
	void initGraphicMode();
	void initTextMode();

	
public:	
    	

	display128x64(){clearMapA();}
	void setupDisplay(void);
	void memDump();
	void send2Bytes(int rs, char byte1, char byte2);	
	void showMapA(void);
	void showTextMapA(void);
	void clearMapA(void);
	void plot(int posX, int posY, int style = 1, bool use_memPlot = false);
	void printIcon(int iconId, int x, int y);
	void drawHorizontalLine(int posY, int fromX = 0 , int toX = 127, int style = 1 , bool use_memPlot = false);
	void drawHorizontalLine2(int posY = 0, int fromByte = 0, int toByte = 5, char pattern = 0b11111111);
	void drawVerticalLine(int posX, int fromY = 0, int toY = 63, int style = 1, char pattern = 255, bool use_memPlot = false);
	void setTextCursorPos(int column , int row);
	void printCharGraphicMode(char ascii8bitCode, int bytePosX, int posY , bool invert = false, bool use_memPlot = false);
	void printStringGraphicMode(std::string str, int bytePosX, int posY, bool invert = false, bool use_memPlot = false);
	std::string centered( std::string const& original, int targetSize = 16 );
	void printStringTextMode(std::string str, int column, int row);
	void printCharTextMode(char code, int bytePosX, int row);     
	void drawGenericLine2(int x0, int y0, int x1, int y1, int style = 1, bool use_memPlot = false);
	void drawGenericLine(int fromX, int fromY, int toX, int toY, int style = 1, bool use_memPlot = false);
	void drawRectangle(int fromX, int fromY, int toX, int toY, bool fill = false, int style = 1, bool use_memPlot = false);
	void drawCircle(int circleCenterX, int circleCenterY, int radius, int startDegree = 0, int stopDegree = 360, int stepDegree = 1, int style = 1, bool use_memPlot = 0);
	void drawRadiusLine(int fromX, int fromY, int degree, int radius, int style = 1, bool use_memPlot = 0);
	void loadBMP12864(std::string fileName);


	void clearGraphic(char pattern = 0x00);
	void clearText();
	void clearDisplay(char pattern=0x00);
	void defineIcon(int iconId,int * iconData);  //16x16 definable characters, iconData[16] must be defined in 16 lines of 16 bit values
	void blinkLastChineseChar(bool state);
	void hideShowDisplay(bool state);
	void standby();
	void cursorON();
	void setIconPos(int column, int row);
	void scrollCN();
	void scrollFont2();
};



#endif

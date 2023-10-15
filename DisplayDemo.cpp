#include "Display128x64.h"
#include <iostream> 
#include <wiringPi.h>
#include <string>
#include <cstring>
#include <random>
#include <fstream>
#include <time.h>


int returnRandom(int min,int max);
void explText(void);

display128x64 displayTest;


int main(void){

  //initialize wiringPi
  wiringPiSetupGpio();
  
  std::cout<<"example...\n";
 
  explText();
  
  return 0;
  
}




//- - - - - - - - - - creates random int in range - - - - - - - - - - - - - - - 
int returnRandom(int min,int max){
	std::random_device rd;     // Only used once to initialise (seed) engine
    std::mt19937 rng(rd());    // Random-number engine used (Mersenne-Twister in this case)
    std::uniform_int_distribution<int> uni(min,max); // Guaranteed unbiased
    auto random_integer = uni(rng);
    return random_integer;
}
//==============================================================
//              Starts the default examples
//==============================================================
void explText(void){
    //- - - - - - - - - Writing text to the display - - - - - - - - - - - - - - - - - - - -  
	
    displayTest.setupDisplay();	
	
    
    displayTest.printStringTextMode("Viewing pointer",1,0);   // Display the text in the text mode at specified coordinates
    displayTest.printStringTextMode("display",4,1);
    displayTest.printStringTextMode("in text",4,2);
    displayTest.printStringTextMode("mode",5,3);
    
    delay(2000);
       
    displayTest.printStringTextMode("chr(1)...chr(32)", 0 , 1);
    char charCode = 0;
    for(int r = 2;r<4;r++){
        for(int s=0;s<16;s++){
            charCode = charCode + 1;
            displayTest.printCharTextMode(charCode, s, r);   // printCharGraphicMode display in text mode according to its (ASCII) code
		}
	}
		
    delay(3000);
    
    displayTest.printStringTextMode("chr(33)..chr(64)", 0, 1);
    for(int r=2;r<4;r++){
        for(int s = 0;s<16;s++){
            charCode = charCode + 1;
            displayTest.printCharTextMode(charCode, s, r);
        } 
    }
    delay(3000);
    displayTest.clearText();               // Delete the text part of the display
    delay(1000);

//- - - - - - - - - Icons - - - - - - - - - - - - - - - - - - - -  
    displayTest.printStringTextMode(" 4 own Icons ", 0, 0);
    
int iconData[16]; 
 
    // Definition of a zig-zag form of 4 own icons:
    // First User-Defined Icon (Focus Crisis)
    iconData[0]  =  0b0011111111111100;
    iconData[1]  =  0b0111111111111110;
    iconData[2]  =  0b1110000110000111;
    iconData[3]  =  0b1100000110000011;
    iconData[4]  =  0b1100000110000011;
    iconData[5]  =  0b1100000000000011;
    iconData[6]  =  0b1100000000000011;
    iconData[7]  =  0b1111100110011111;
    iconData[8]  =  0b1111100110011111;
    iconData[9]  =  0b1100000000000011;
    iconData[10] =  0b1100000000000111;
    iconData[11] =  0b1100000110000011;
    iconData[12] =  0b1100000110000011;
    iconData[13] =  0b1110000110000111;
    iconData[14] =  0b0111111111111110;
    iconData[15] =  0b0011111111111100;
    displayTest.defineIcon(0, iconData);

    // Second user-defined icon (square with crash)
    iconData[0]  =  0b1111111111111111;
    iconData[1]  =  0b1111111111111111;
    iconData[2]  =  0b1110000000000111;
    iconData[3]  =  0b1101000000001011;
    iconData[4]  =  0b1100100000010011;
    iconData[5]  =  0b1100010000100011;
    iconData[6]  =  0b1100001001000011;
    iconData[7]  =  0b1100000110000011;
    iconData[8]  =  0b1100000110000011;
    iconData[9]  =  0b1100001001000011;
    iconData[10] =  0b1100010000100011;
    iconData[11] =  0b1100100000010011;
    iconData[12] =  0b1101000000001011;
    iconData[13] =  0b1110000000000111;
    iconData[14] =  0b1111111111111111;
    iconData[15] =  0b1111111111111111;
    displayTest.defineIcon(1, iconData);


    // Third user-defined icon (empty square)
    iconData[0]  =  0b1111111111111111;
    iconData[1]  =  0b1111111111111111;
    iconData[2]  =  0b1100000000000011;
    iconData[3]  =  0b1100000000000011;
    iconData[4]  =  0b1100000000000011;
    iconData[5]  =  0b1100000000000011;
    iconData[6]  =  0b1100000000000011;
    iconData[7]  =  0b1100000000000011;
    iconData[8]  =  0b1100000000000011;
    iconData[9]  =  0b1100000000000011;
    iconData[10] =  0b1100000000000011;
    iconData[11] =  0b1100000000000011;
    iconData[12] =  0b1100000000000011;
    iconData[13] =  0b1100000000000011;
    iconData[14] =  0b1111111111111111;
    iconData[15] =  0b1111111111111111;
    displayTest.defineIcon(2, iconData) ;
    
    // Fourth user-defined icon (crisis in a circle)
    iconData[0]  =  0b0000011111100000;
    iconData[1]  =  0b0000100110010000;
    iconData[2]  =  0b0011000110001100;
    iconData[3]  =  0b0010000110000100;
    iconData[4]  =  0b0100000110000010;
    iconData[5]  =  0b1000000110000001;
    iconData[6]  =  0b1000000110000001;
    iconData[7]  =  0b1111111111111111;
    iconData[8]  =  0b1111111111111111;
    iconData[9]  =  0b1000000110000001;
    iconData[10] =  0b1000000110000001;
    iconData[11] =  0b0100000110000010;
    iconData[12] =  0b0010000110000100;
    iconData[13] =  0b0011000110001100;
    iconData[14] =  0b0000100110010000;
    iconData[15] =  0b0000011111100000;
    displayTest.defineIcon(3, iconData) ;
    


    displayTest.printIcon(1, 0, 1);    // Icon c.2 at the top of the second row from the top
    for(int icon=0;icon<7;icon++){
        // When you print the icons for yourself, you do not have to set positions for each
        icon = returnRandom(0, 3) * 2; // The last parameter is double the number of Icons
        displayTest.send2Bytes(1, 0,icon);    
	}
    displayTest.printIcon(2, 0, 2);    // Icon c.3 at the top of the third row from above
     // The rest of the row is filled with random icons
    for(int icon = 0;icon<7;icon++){
        // When you print the icons for yourself, you do not have to set positions for each
        icon = returnRandom(0, 3) * 2; // The last parameter is the double-pin number Icons
        displayTest.send2Bytes(1, 0, icon);    
	}
    displayTest.printIcon(3, 0, 3);    // Icon c.4 at the bottom of the bottom row
     // The rest of the lines are filled with c.4
    for(int icon = 0;icon<7;icon++){
       // When you print the icons for yourself, you do not have to set positions for each
        displayTest.send2Bytes(1, 0, 6);    // (6 = icon c.4)    
	}


    delay(2000);
    displayTest.printStringTextMode("Change of    ", 0, 0);
    delay(2000);
    displayTest.printStringTextMode(" Fourth Icon ", 0 , 0);
    
    delay(2000);

    
    // When changing the Icons definition, OK to change the appearance of ALL Icons displayed
     // Fourth user-defined icon will now be moved to the shuffle
    iconData[0]  =  0b1111000011110000;
    iconData[1]  =  0b1111000011110000;
    iconData[2]  =  0b1111000011110000;
    iconData[3]  =  0b1111000011110000;
    iconData[4]  =  0b0000111100001111;
    iconData[5]  =  0b0000111100001111;
    iconData[6]  =  0b0000111100001111;
    iconData[7]  =  0b0000111100001111;
    iconData[8]  =  0b1111000011110000;
    iconData[9]  =  0b1111000011110000;
    iconData[10] =  0b1111000011110000;
    iconData[11] =  0b1111000011110000;
    iconData[12] =  0b0000111100001111;
    iconData[13] =  0b0000111100001111;
    iconData[14] =  0b0000111100001111;
    iconData[15] =  0b0000111100001111;
    displayTest.defineIcon(3, iconData) ;
 
    delay(3000);

    displayTest.clearText();
    
//- - - - - - - - -Cinske charactery - - - - - - - - - - - - - - - -  
    
    displayTest.printStringTextMode(" ST7920s 16x16  " , 0 , 0);
    displayTest.printStringTextMode("   Caracters    " , 0 , 1);
    displayTest.printStringTextMode("     Demo       " , 0 , 2);
    //displayTest.showTextMapA();
    delay(3000);
    displayTest.clearText();
    //displayTest.showTextMapA();
    // Print caracters of the built-in 16x16 point font (in text mode)
    // scroll through this mostly chinese character set with function scollCN()
    // printCharGraphicMode is selected with the help of the last two functions "send2Bytes()"
    // The first of these two numbers must be higher than 161 (0xA1)

    displayTest.setIconPos(3, 0);                       // Setting the first printCharGraphicMode position [0,0] to [7,3]
    displayTest.send2Bytes(1, 0xA1, 0x7B);   
    displayTest.send2Bytes(1, 0xA1, 0x7A);
    displayTest.send2Bytes(1, 0xA1, 0x70);
    displayTest.send2Bytes(1, 0xA1, 0x74);

    displayTest.setIconPos(4, 2);                       // Setting the first printCharGraphicMode position [0,0] to [7,3]
    displayTest.send2Bytes(1, 0xBB, 0xAE);   
    displayTest.send2Bytes(1, 0xA3, 0xC1);
    displayTest.send2Bytes(1, 0xA3, 0xC2);
    displayTest.send2Bytes(1, 0xA3, 0xC3);

    // Vertical four-letter writing
    displayTest.setIconPos(0, 0);                       // printCharGraphicMode position setting
    displayTest.send2Bytes(1, 0xA2, 0x73);
    displayTest.setIconPos(0, 1) ;                      // printCharGraphicMode position setting 
    displayTest.send2Bytes(1, 0xA1, 0x7C);
    displayTest.setIconPos(0, 2);                       // printCharGraphicMode position setting 
    displayTest.send2Bytes(1, 0xA1, 0x7D);
    displayTest.setIconPos(0, 3);                       // printCharGraphicMode position setting 
    displayTest.send2Bytes(1, 0xA2, 0xDB);


    delay(5000);
    displayTest.clearText();
    displayTest.clearGraphic();
    delay(10);

//- - - - - - - - -  Text in the graphics mode - - - - - - - - - - - - - - - - - - -  
// Print 8x8 dot font (in graphic mode)

    
    delay(10);
    displayTest.printStringGraphicMode("The display now", 0,  0, false);  // Normal graphic on the top row
    displayTest.printStringGraphicMode("operates",        3, 15, false);        
    displayTest.printStringGraphicMode("in graphic",      1, 30, false); 
    displayTest.printStringGraphicMode("mode",            5, 45, false);  

    delay(2000);
    displayTest.printStringGraphicMode("graphic", 4, 30, true);  // Inversely rewritten text  
    
    delay(3000);
    displayTest.clearGraphic();    // Delete the graphical parts of the display

    bool invert;
    // In the same mode, y print individual characters according to their code
    //for code in range(32, 128):
    for(int code = 32;code <128;code++){
        int column = (code - 32) % 16;
        int row = int((code - 32)/16) * 11;      // The spacing between the lines is 11 points
        if (row / 22.0 == int(row / 22)){  // Every other row is inverse
            invert = true;
        }else{
            invert = false;
        }   
        displayTest.printCharGraphicMode(code, column, row, invert); // printCharGraphicMode subroutine in graphics mode
    }
    delay(2000);
    displayTest.clearGraphic();

    
    for(int code = 128;code<256;code++){
        int column = (code - 128) % 16;
        int row = int((code - 128) / 16) * 8; 
        displayTest.printCharGraphicMode(code, column, row, false);
	}
    delay(2000);
    displayTest.clearGraphic();
//- - - - - - - Point printing and car - - - - - - - - - - - - - - - - - -  

    displayTest.printStringGraphicMode("In this mode",         0,  2,false);  // Normal writing 2 pixels from the top edge
    displayTest.printStringGraphicMode("you draw",         0, 11,false);        
    displayTest.printStringGraphicMode("points and lines",      0, 20, false); 

    // Oramovani --[Window?]-- display with full car
    displayTest.drawHorizontalLine (  0, 0, 127, 1);          // Horizontal line at any position
    displayTest.drawHorizontalLine2( 63, 0,   7, 0b11111111); // Speed mountains. Line in a 16-column raster with mask setting
    displayTest.drawVerticalLine (  0, 0,  63, 0b11111111);   // Vertical line in any position with mask setting
    displayTest.drawVerticalLine (127, 0,  63, 0b11111111);

    // Internal small rectangle with a carcass
    displayTest.drawHorizontalLine2(31, 1,  6, 0b11001100);
    displayTest.drawHorizontalLine2(56, 1,  6, 0b11001100);
    displayTest.drawVerticalLine ( 16, 31, 56, 0b11001100);
    displayTest.drawVerticalLine (111, 31, 56, 0b11001100);


    // Randomly popping a point into a small rectangle in inverse mode
    
    for(int i = 0;i<2000;i++){
        int x= returnRandom(17, 110); //int(random.randint(17, 110))
        int y= returnRandom(32, 55); //int(random.randint(32, 55))
        displayTest.plot(x, y, 2);   // Print an inverse point at the coordinates [x, y]
	}

 //- - - - - - - - - - - - Different styles of --horizontalnich-- cars - - - - - - - - - - - - - - - - -  
    displayTest.clearGraphic();

    displayTest.drawHorizontalLine2( 0, 2, 5, 0b11111111); // Speed mountains. Line in a 16-column raster with mask setting
    displayTest.drawHorizontalLine2(10, 3, 4, 0b11001100); // Speed mountains. Line in a 16-column raster with mask setting
    displayTest.drawHorizontalLine2(20, 2, 5, 0b11110000); // Speed mountains. Line in a 16-column raster with mask setting
    displayTest.drawHorizontalLine2(30, 1, 6, 0b10101010); // Speed mountains. Line in a 16-column raster with mask setting
    displayTest.drawHorizontalLine2(40, 0, 7, 0b11110101); // Speed mountains. Line in a 16-column raster with mask setting
    displayTest.drawHorizontalLine2(50, 1, 6, 0b01110101); // Speed mountains. Line in a 16-column raster with mask setting

    delay(3000);

    displayTest.clearGraphic(0xff);    // To fill the screen with white ink
    
 //- - - - - - - - - - drawing - - - - - - - - - - - - - - - - - - - -  
    // Draw a double circle using the first print point on the display (slowly)
    //for circle in range(0, 6283, 4):
    int x,y,circle;
    for(circle = 0;circle<6283;circle +=4){
        x = int(((std::sin(circle / 1000.0) * 30.0)) + 32);
        y = int(((std::cos(circle / 1000.0) * 30.0)) + 32);
        displayTest.plot(x, y, 0);
        x = int(((std::sin(circle / 1000.0) * 20.0)) + 32);
        y = int(((std::cos(circle / 1000.0) * 20.0)) + 32);
        displayTest.plot(x, y, 0);
	}
    // Draw five of the circles using the memo
    // And then swipe the memory to the display (fast)
    //for circle in range(0, 6283, 4):
    for(circle = 0;circle<6283;circle +=4){
        x = int(((sin(circle / 1000.0) * 30.0)) + 96);
        y = int(((cos(circle / 1000.0) * 30.0)) + 32);
        displayTest.plot(x, y, 0,true);
        x = int(((sin(circle / 1000.0) * 25.0)) + 96);
        y = int(((cos(circle / 1000.0) * 25.0)) + 32);
        displayTest.plot(x, y, 0,true);
        x = int(((sin(circle / 1000.0) * 20.0)) + 96);
        y = int(((cos(circle / 1000.0) * 20.0)) + 32);
        displayTest.plot(x, y, 0,true);
        x = int(((sin(circle / 1000.0) * 15.0)) + 96);
        y = int(((cos(circle / 1000.0) * 15.0)) + 32);
        displayTest.plot(x, y, 0,true);
        x = int(((sin(circle / 1000.0) * 10.0)) + 96);
        y = int(((cos(circle / 1000.0) * 10.0)) + 32);
        displayTest.plot(x, y, 0,true);
	}

    displayTest.memDump();    // Spraying data from memory to display



    delay(2000);
    
    
    displayTest.clearGraphic();


//- - - - - - - - - - Overwrite text and graphic mode - - - - - - - - - -  

    displayTest.printStringGraphicMode("     Graphic    ",  0,  0, false);
    displayTest.printStringGraphicMode("  And text mode ", 0, 10, false); 
    displayTest.printStringGraphicMode("    Y is used   ",     0, 20, false); 
    displayTest.printStringGraphicMode("  In particular ", 0, 30, false); 

    delay(2000);

    displayTest.clearGraphic();
 
    
    
    displayTest.printStringTextMode("Big writing", 2, 0);
    displayTest.printStringTextMode("  In text  ", 3, 1);
    displayTest.printStringTextMode("    mode   ", 5, 2);
    displayTest.printIcon(1, 0, 2);
    displayTest.printIcon(2, 7, 2);  


    delay(2000);

    
    displayTest.drawHorizontalLine2(53, 0, 7, 0b10011001); 
    displayTest.printStringGraphicMode("Graphic row", 1, 56, false);   


    
    // When combining text and graphic mode, the common area on the XOR displays
    // Example: 4 points thick sikma line over the entire screen

    
    for(int x = 0;x<128;x++){
        displayTest.plot(x ,x/2     , 1);
        displayTest.plot(x,(x/2) + 1, 1);
        displayTest.plot(x,(x/2) + 2, 1);
        displayTest.plot(x,(x/2) + 3, 1);
	}
    delay(2000);
    displayTest.printStringGraphicMode("Delete graphics", 0, 56, true);   

    delay(2000);
    displayTest.clearGraphic();   // The graphic is deleted separately, so the original text remains

    displayTest.printStringGraphicMode("The text remains", 0, 56, true);   
    delay(2000);
    displayTest.printStringGraphicMode("Make a line     ", 0, 56, true);   

    
    for(int x = 0;x<128;x++){
        displayTest.plot(127-x ,x/2     , 1);
        displayTest.plot(127-x,(x/2) + 1, 1);
        displayTest.plot(127-x,(x/2) + 2, 1);
        displayTest.plot(127-x,(x/2) + 3, 1);
	}
    delay(2000);
    displayTest.printStringGraphicMode(" Delete text ", 0, 56, true);   
    delay(2000);
    
    displayTest.clearText();                // The display will be deleted separately
    
   
    displayTest.memDump();                // graphic se ale v tom pripade musi obnovit z pameti
    displayTest.clearMapA();
   
    delay(1000);
    displayTest.printStringGraphicMode("Graphic remains", 0, 56, true);   
    delay(2000);
    displayTest.clearGraphic();

//- - - - - - - - - nahrani obrazku - - - - - - - - - - - - - - - - - -  

    displayTest.printStringGraphicMode("   display    " ,0,  0,false);   
    displayTest.printStringGraphicMode("     file     " ,0, 10,false);   
    displayTest.printStringGraphicMode(" with a image " ,0, 20,false);   
    delay(2000);
    displayTest.loadBMP12864("./pokladnik.bmp"),  // Upload bitmaps of 128x64 pixels on the display 

    delay(4000);

    //Fill the display with vertical lines (pattern = 0b10101010)
    displayTest.clearDisplay(0b10101010); 
      

    displayTest.printStringGraphicMode("END OF EXAMPLES" , 2 , 25 , true);   
 
    delay(5000);
    displayTest.clearDisplay();


  
   
}
//==============================================================
// Function to scroll through Chinese character set of the ST7920.
// There are also symbols, numbers and frame constructions included.
// they start at 161 (0xA1) 32 (0x20) and strech to 169/96
void scrollCN(){
	
  
  delay(200);
  displayTest.clearText();
  char a;
  for(int i= 161;i<247;i++){
	 for(int k = 0; k<160; k+=16){
		displayTest.printStringTextMode(std::to_string(i)+" / "+std::to_string(k)+"   ", 0, 0);
		displayTest.setIconPos(0, 1); 
		for(int j = 0;j<8;j++){                     
			displayTest.send2Bytes(1, i, k+j);   
		}
		displayTest.setIconPos(0, 2); 
		for(int j = 8;j<16;j++){                     
			displayTest.send2Bytes(1, i, k+j);   
		}
		std::cin>>a;		
	 }	
	 
  }
  delay(3000);
  displayTest.clearText();
	
}





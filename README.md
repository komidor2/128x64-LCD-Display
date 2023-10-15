# 128x64-LCD-Display


This is a C++ class for running a 128x64 LCD and a demo using the class.

It was inspired from a python library originated the czech author Milan Krúpa and the English translation and improvements by Henrique Bruno Fantauzzi de Almeida.


python code: http://www.astromik.org/raspi/42.htm


>(Wayback Machine: https://web.archive.org/web/20160323175419/http://www.astromik.org/raspi/42.htm)

![alt text](http://www.astromik.org/raspi/glcd12864-zw-a.jpg)
>(Image displayed on original website)


## HISTORY:

Transferring a project from Arduino to Raspbery PI, I was looking for a ready cpp library or class to run a 128x64 LCD Display and found absolutely none (At least none that would not require cross-compiling, or other methods above my level).
Finally I came across the python library mentioned above and started to convert it to c++. 

## THE CLASS:
Using wiringPI, the class has definitions for three pins to send values to the display. Use the same pins or adjust the pin# to your needs. You may also want to manage the pins in the constructor of the class.


## THE DEMO:

The Demo application is seperated from the class and utilizes the different commands the class offers, including showing a B&W BMP graphic that can be loaded from a file (see picture above).


## CREDITS:
Conversion to C++: Ludwig Kersten

Translation of the python code and improvements by Henrique Bruno Fantauzzi de Almeida - Federal University of Rio de Janeiro - Brazil
https://github.com/SrBrahma/RPi-12864-LCD-ST7920-lib

The credits for the original code goes to the author. Thanks, Milan Krúpa!

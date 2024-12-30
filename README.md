# Overview
Demo: https://www.youtube.com/watch?v=6DPkey17IP0

This project implements a BASIC interpreter in a ESP32-2432S028 development kit which contains a TFT display.
The user can create programs using a serial terminal connected to the board via USB port.

## Following some terminal comands:
DIR - List the files stored on ESP32 flash  
LOAD <file> - Loads a BASIC file e.g: load program1.bas  
SAVE <file> - Saves the current program to a file e.g: save program1.bas  
LIST <begin>-<end> - List the lines of program e.g: LIST 10 (list the single line 10), LIST 50- (list from 50), LIST 50-100 (list from 50 to 100)  
DELETE <file> - Delete a file e.g: delete program1.bas  
NEW - Clear the memory to start a new program.  
*You can edit the program just typing the line in the terminal e.g: 10 print "Start"  

## The basic functions/keywords: 
IF/WHILE/FOR - Conditionals and loop.  
GOTO/GOSUB/RETURN - Subroutines calls/returns  
PRINT/COLOR/LOCATE - Text printing and positioning  
CLS/END/SLEEP - Program/screen control  
CIRCLE/LINE - Graphic functions  

## Example
10 CLS  
12 LOCATE 0,0  
15 print "Start"  
20 x=0  
30 y=20  
40 print y + 1  
50 print "x:";x   
60 WHILE x <> 10 : GOSUB 200 WEND  
70 print "poswhile"   
80 sleep 2000  
90 goto 10  
200 print "x:" ; x   
210 x = x + 1   
212 z = x * 20  
213 LINE 0,0,320,z,x  
215 sleep 100   
230 return  






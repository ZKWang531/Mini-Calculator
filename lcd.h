#ifndef LCD_H
#define LCD_H
#include "PLL.h"

// LCD Function Prototypes
void Port_lcd_Init(void);
                                        
void SendDisplayNibble(unsigned long RS_SET, unsigned long nibble);
void SendDisplayByte(unsigned long RS_SET, unsigned long eightbyte);

void InitDisplayPort(void);  

void clearDisplayScreen(void);                                    
void InitLCD(void);
void printDisplay(char shows_str[]);
unsigned long find_location(unsigned long locate);
void draw();
void LoadCustomChar(unsigned long location, unsigned long *pattern) ;

void DisplayCustomChar(unsigned long location);
void DrawPattern();
void next_line(void);
#endif
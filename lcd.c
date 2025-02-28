
#include "LCD.h"

#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
// portB

#define GPIO_PORTB_DATA_R       (*((volatile unsigned long *)0x400053FC))
#define GPIO_PORTB_DIR_R        (*((volatile unsigned long *)0x40005400))
#define GPIO_PORTB_AFSEL_R      (*((volatile unsigned long *)0x40005420))
#define GPIO_PORTB_DEN_R        (*((volatile unsigned long *)0x4000551C))
#define GPIO_PORTB_LOCK_R       (*((volatile unsigned long *)0x40005520))
#define GPIO_PORTB_CR_R         (*((volatile unsigned long *)0x40005524))
#define GPIO_PORTB_AMSEL_R      (*((volatile unsigned long *)0x40005528))
#define GPIO_PORTB_PCTL_R       (*((volatile unsigned long *)0x4000552C))

// portA
#define RS                      (*((volatile unsigned long *)0x40004020)) // PA3
#define GPIO_PORTA_DATA_R       (*((volatile unsigned long *)0x400043FC))
#define GPIO_PORTA_DIR_R        (*((volatile unsigned long *)0x40004400))
#define GPIO_PORTA_AFSEL_R      (*((volatile unsigned long *)0x40004420))
#define GPIO_PORTA_DEN_R        (*((volatile unsigned long *)0x4000451C))
#define GPIO_PORTA_LOCK_R       (*((volatile unsigned long *)0x40004520))
#define GPIO_PORTA_CR_R         (*((volatile unsigned long *)0x40004524))
#define GPIO_PORTA_AMSEL_R      (*((volatile unsigned long *)0x40004528))
#define GPIO_PORTA_PCTL_R       (*((volatile unsigned long *)0x4000452C))

// Instructions related Defines

void PortB_Init(void) { 
    volatile unsigned long delay;
    SYSCTL_RCGC2_R |= 0x00000002;     
    delay = SYSCTL_RCGC2_R;           
    GPIO_PORTB_LOCK_R = 0x4C4F434B;   
    GPIO_PORTB_CR_R = 0x0F;           
    GPIO_PORTB_AMSEL_R = 0x00;        
    GPIO_PORTB_PCTL_R = 0x00000000;   
    GPIO_PORTB_DIR_R = 0x0F;          
    GPIO_PORTB_AFSEL_R = 0x00;        
    GPIO_PORTB_DEN_R = 0x0F;          
}

void PortA_Init(void) { 
    volatile unsigned long delay;
    SYSCTL_RCGC2_R |= 0x00000001;     
    delay = SYSCTL_RCGC2_R;           
    GPIO_PORTA_LOCK_R = 0x4C4F434B;   
    GPIO_PORTA_CR_R = 0x0C;           
    GPIO_PORTA_AMSEL_R = 0x00;        
    GPIO_PORTA_PCTL_R = 0x00000000;   
    GPIO_PORTA_DIR_R = 0x0C;          
    GPIO_PORTA_AFSEL_R = 0x00;        
    GPIO_PORTA_DEN_R = 0x0C;          
}


void Port_lcd_Init(void) { 
    PortB_Init();
    PortA_Init();    
}


void SendDisplayNibble(unsigned long RS_SET, unsigned long nibble) {
    RS = RS_SET;								//Set up the RS bit appropriately: 0 for instructions or 1 for data.
    SysTick_Wait(36); 					//wait for 450 ns
    GPIO_PORTB_DATA_R = nibble;	//Send the nibble to the bits of the port.
    GPIO_PORTA_DATA_R |= 0x04; 	//Set En to High
    SysTick_Wait(36); 					//wait for 450 ns
    GPIO_PORTA_DATA_R ^= 0x04;	//Set En to Low
    SysTick_Wait(36);						//wait for 450 ns
}


void SendDisplayByte(unsigned long RS_SET, unsigned long eightbyte) {
	
    unsigned long eight_hight = eightbyte & 0xF0;	//High 4 bit
		unsigned long eight_low = eightbyte & 0x0F;		//Low 4 bit
	
		SendDisplayNibble(RS_SET, (eight_hight >> 4));
    SendDisplayNibble(RS_SET, eight_low);
    us_unit(100); //I set the delay to 100µs to give the LCD sufficient time to respond.
}


void InitDisplayPort(void) {
		unsigned long RS_init = 0x00;
    us_unit(15001);
	
    SendDisplayNibble(RS_init, 0x03);
    us_unit(4100);
	
    SendDisplayNibble(RS_init, 0x03);
    us_unit(100);
	
    SendDisplayNibble(RS_init, 0x03);
    us_unit(50);
	
    SendDisplayNibble(RS_init, 0x02);
		us_unit(50);
	
    SendDisplayByte(RS_init, 0x00111000);	//Function set
    SendDisplayByte(RS_init, 0x08); 
    SendDisplayByte(RS_init, 0x01); 
    us_unit(100);
		SendDisplayByte(RS_init, 0x06);
    SendDisplayByte(RS_init, 0x0F); // Set the display, cursor and blink on
}


void printDisplay(char shows_str[]){
    int i = 0;
    while (i < 100 && shows_str[i] != '\0') 
			{ 
				
        if(shows_str[i] == '/'){
					SendDisplayByte(0x08, 0xB8);
				}
				else if(shows_str[i] == '*'){
					SendDisplayByte(0x08, 0xB7);
				}
				else{
					SendDisplayByte(0x08, shows_str[i]);//RS is PA3,so I need to set 0x08 here
				}
        i++;
			}
}



void clearDisplayScreen(void) {
    SendDisplayByte(0x00, 0x01);//0x01 can clear all the char in LCD
    SendDisplayByte(0x00, 0x02);//go back to home
		us_unit(10000);
}

unsigned long find_location(unsigned long locate){	
	unsigned long address = 0x00;
	switch (locate) {
        case 0:
            address = 0x40;//01000000
            break;
        case 1:
            address = 0x48;//01001000  
            break;
        case 2:
            address = 0x50;//01010000 
            break;
        case 3:
            address = 0x58;//01011000 
            break;
        case 4:
            address = 0x60; 
            break;
        case 5:
            address = 0x68;
            break;
        case 6:
            address = 0x70;
            break;
        case 7:
            address = 0x78;
            break;
        default:
            address = 0x40;
            break;
    }
	return address;
}

void set_locate_Char(unsigned long location, unsigned long *pattern) {    
    SendDisplayByte(0x00, find_location(location));
		int i = 0;
    for (i = 0; i < 8; i++) {
        SendDisplayByte(0x08, pattern[i]);
    }
}

void Display_Char(unsigned long location) {
    SendDisplayByte(0x08, location);
}

//1000 0000 80
//0000 0000	00
void First_line(void) {
    SendDisplayByte(0x00, 0x80);
}
//1100 0000	C0	
//0100 0000	40
void Second_line(void) {
    SendDisplayByte(0x00, 0xC0);
}
//1001 0100	94
//0001 0100 14
void Third_line(void) {
    SendDisplayByte(0x00, 0x94);
}
//1101 0100 D4
//0101 0100	54
void Fouth_line(void) {
    SendDisplayByte(0x00, 0xD4);
}



#include "PLL.h"
#include "keypad.h"
//PORTF
#define GPIO_PORTF_DATA_R       (*((volatile unsigned long *)0x400253FC))
#define GPIO_PORTF_DIR_R        (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_AFSEL_R      (*((volatile unsigned long *)0x40025420))
#define GPIO_PORTF_PUR_R        (*((volatile unsigned long *)0x40025510))
#define GPIO_PORTF_DEN_R        (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_LOCK_R       (*((volatile unsigned long *)0x40025520))
#define GPIO_PORTF_CR_R         (*((volatile unsigned long *)0x40025524))
#define GPIO_PORTF_AMSEL_R      (*((volatile unsigned long *)0x40025528))
#define GPIO_PORTF_PCTL_R       (*((volatile unsigned long *)0x4002552C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
	
//PORTE
#define GPIO_PORTE_DATA_R       (*((volatile unsigned long *)0x400243FC))
#define GPIO_PORTE_DIR_R        (*((volatile unsigned long *)0x40024400))
#define GPIO_PORTE_AFSEL_R      (*((volatile unsigned long *)0x40024420))
#define GPIO_PORTE_PDR_R        (*((volatile unsigned long *)0x40024514))
#define GPIO_PORTE_DEN_R        (*((volatile unsigned long *)0x4002451C))
#define GPIO_PORTE_LOCK_R       (*((volatile unsigned long *)0x40024520))
#define GPIO_PORTE_CR_R         (*((volatile unsigned long *)0x40024524))
#define GPIO_PORTE_AMSEL_R      (*((volatile unsigned long *)0x40024528))
#define GPIO_PORTE_PCTL_R       (*((volatile unsigned long *)0x4002452C))
	
//PORTD
#define GPIO_PORTD_DATA_R       (*((volatile unsigned long *)0x400073FC))
#define GPIO_PORTD_DIR_R        (*((volatile unsigned long *)0x40007400))
#define GPIO_PORTD_AFSEL_R      (*((volatile unsigned long *)0x40007420))
#define GPIO_PORTD_DEN_R        (*((volatile unsigned long *)0x4000751C))
#define GPIO_PORTD_LOCK_R       (*((volatile unsigned long *)0x40007520))
#define GPIO_PORTD_CR_R         (*((volatile unsigned long *)0x40007524))
#define GPIO_PORTD_AMSEL_R      (*((volatile unsigned long *)0x40007528))
#define GPIO_PORTD_PCTL_R       (*((volatile unsigned long *)0x4000752C))

#define GPIO_PORTF_DATA_R       (*((volatile unsigned long *)0x400253FC))
#define GPIO_PORTF_DIR_R        (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_AFSEL_R      (*((volatile unsigned long *)0x40025420))
#define GPIO_PORTF_PUR_R        (*((volatile unsigned long *)0x40025510))
#define GPIO_PORTF_DEN_R        (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_LOCK_R       (*((volatile unsigned long *)0x40025520))
#define GPIO_PORTF_CR_R         (*((volatile unsigned long *)0x40025524))
#define GPIO_PORTF_AMSEL_R      (*((volatile unsigned long *)0x40025528))
#define GPIO_PORTF_PCTL_R       (*((volatile unsigned long *)0x4002552C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
	
unsigned char keyChar[4][4] = {
    {'1', '2', '3', '+'},
    {'4', '5', '6', '-'},
    {'7', '8', '9', '*'},
    {'/', '0', 'b', 'M'}
};

unsigned char key_operator[4][4] = {
    {'E', '=', 'x', 'y'},
    {'B', 'H', 'D', 'O'},
    {'(', ')', '^', '.'},
    {'S', 'C', 's', 'c'}
};

unsigned long KSET = 0;
unsigned char kch = '!';
unsigned char kch_op = '!';
unsigned long KROW;
unsigned long row;
unsigned long col=0;
//Function Prototypes
void Port_Init(void);
void PortE_Init(void);
void PortD_Init(void);
void Delay(void);
col_fsm[4][2]={{0,1},{1,2},{2,3},{3,0}};

void rowread() {
    int InE = (GPIO_PORTE_DATA_R & 0x0F);
    switch (InE) {
        case 0x01:
            row = 0;
            break;
        case 0x02:
            row = 1;
            break;
        case 0x04:
            row = 2;
            break;
        case 0x08:
            row = 3;
            break;
        default:
            row = 5;
            break;
    }
}
void colset(unsigned long col)
{
		switch (col) {
        case 0:
            GPIO_PORTD_DATA_R = 0x01;  
            break;
        case 1:
            GPIO_PORTD_DATA_R = 0x02;  
            break;
        case 2:
            GPIO_PORTD_DATA_R = 0x04;  
            break;
        case 3:
            GPIO_PORTD_DATA_R = 0x08;  
            break;
        default:
            GPIO_PORTD_DATA_R = 0x00;  
            break;
    }
}

char keypadpress(){    
       
  Port_Init();  
	 
	PLL_Init();
  SysTick_Init();
		
  colset(col);  
	SysTick_Wait(1000);
  rowread();    
		
  if (row!=5)
	{  
						
		kch = keyChar[row][col]; //determine and return the Key according with current row and col       
    return kch;    
  }
	col = col_fsm[col][1];											//set the next col to high level    	
	return keypadpress();
}
unsigned long InLight;
char keypad_op(){    
       
  Port_Init();  
	PLL_Init();
  SysTick_Init();
	
	GPIO_PORTF_DATA_R = 0x06;
	
  colset(col);  
	SysTick_Wait(1000);
  rowread();    
		
  if (row!=5) //row is in range 1-5
	{  
						
		kch_op = key_operator[row][col]; //determine and return the Key according with current row and col  
		GPIO_PORTF_DATA_R = 0x00;
    return kch_op;    
  }
	col = col_fsm[col][1];											//set the next col to high level    	
	return keypad_op();
}

void Port_Init(void){ volatile unsigned long delay;	
  PortE_Init();
	PortD_Init();
	PortF_Init();
}

void PortF_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000020;     // 1) F clock
  delay = SYSCTL_RCGC2_R;           // delay   
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock PortF PF0  
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0       
  GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTF_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTF_DIR_R = 0x0E;          // 5) PF4,PF0 input, PF3,PF2,PF1 output   
  GPIO_PORTF_AFSEL_R = 0x00;        // 6) no alternate function
  GPIO_PORTF_PUR_R = 0x11;          // enable pullup resistors on PF4,PF0       
  GPIO_PORTF_DEN_R = 0x1F;          // 7) enable digital pins PF4-PF0        
}
void PortE_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000010;     
  delay = SYSCTL_RCGC2_R;   
	GPIO_PORTE_LOCK_R = 0x4C4F434B;        
  GPIO_PORTE_CR_R = 0x0F;               
  GPIO_PORTE_AMSEL_R = 0x00;       
	GPIO_PORTE_PCTL_R = 0x00000000;     
  GPIO_PORTE_DIR_R = 0x00;      
  GPIO_PORTE_AFSEL_R = 0x00;       
  GPIO_PORTE_PDR_R = 0x0F;             
  GPIO_PORTE_DEN_R = 0x0F;           
}

void PortD_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000008;     // 1) F clock 
  delay = SYSCTL_RCGC2_R;           // delay   
  GPIO_PORTD_LOCK_R = 0x4C4F434B;   // 2) unlock PortF PF0  
  GPIO_PORTD_CR_R = 0xFF;           // allow changes to PF4-0       
  GPIO_PORTD_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTD_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTD_DIR_R = 0xFF;          // 5) PD0-PD3 output   
  GPIO_PORTD_AFSEL_R = 0x00;        // 6) no alternate function
	GPIO_PORTD_DEN_R = 0xFF;          // 7) enable digital pins PF4-PF0        
}

void Keypad_Init(void){ volatile unsigned long delay;	
	SYSCTL_RCGC2_R |= 0x00000010;     
  delay = SYSCTL_RCGC2_R;   
	GPIO_PORTE_LOCK_R = 0x4C4F434B;        
  GPIO_PORTE_CR_R = 0x0F;               
  GPIO_PORTE_AMSEL_R = 0x00;       
	GPIO_PORTE_PCTL_R = 0x00000000;     
  GPIO_PORTE_DIR_R = 0x00;      
  GPIO_PORTE_AFSEL_R = 0x00;       
  GPIO_PORTE_PDR_R = 0x0F;             
  GPIO_PORTE_DEN_R = 0x0F; 
	
  delay = SYSCTL_RCGC2_R;           // delay   
  GPIO_PORTD_LOCK_R = 0x4C4F434B;   // 2) unlock PortF PF0  
  GPIO_PORTD_CR_R = 0xFF;           // allow changes to PF4-0       
  GPIO_PORTD_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTD_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTD_DIR_R = 0xFF;          // 5) PD0-PD3 output   
  GPIO_PORTD_AFSEL_R = 0x00;        // 6) no alternate function
	GPIO_PORTD_DEN_R = 0xFF;          // 7) enable digital pins PF4-PF0   
}

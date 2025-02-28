#include "PLL.h"
#include "keypad.h"
#include "lcd.h"
#include "calculator.h"
#include <stdlib.h>
#include <string.h>
		
char expression[200] = { 0 };
char equationin[200] = { "(x-2)*(x-8)*(x-12)" };
char equatinxh[208] = { 0 };
char equatinx[208] = { 0 };
char equation[208] = { 0 };
char equat[208] = { 0 };
char str_x[8] = { 0 };
char str_xh[8] = { 0 };
char choice;
char equationou[200] = { 0 };
char str_from_x[8] = { 0 };
char str_x2h[8] = { 0 };

char str_result[200] = { 0 };
char passwordin[200] = { 0 };
char pass[200] = { 0 };
int passwordck = 1111;

int i = 0;
void cv_to_str(float num, char* str_fromfloat);
void cv_to_strxh(float num, char* str_fromfloat);

void scan(char *str,char *str2);
void scan_password(char *str);
void menu_display();
void scanx();
void scan_mode();
void floatcalculate(void);
void base_convert(void);
char mode_sel[2] = { 0 };

float x_inequ;
void set_mode();
void x_plus_h(char *str_from_xh,char *str_from_x);

void cal_equ();
void cal_equ_dt();
void outputResult(float result);
void fx(char *equo,char *equi,char *x, float *Y);
void LOCK();
void Funtion_mode_select(void);
void change_password();
void wait_input();
char wait;
int Y_index = 0;
unsigned long base_address = 0xCA;
void printY(int *yin,int *xin);

void LOCK(){
	
	//passwordck = 1111;
	printDisplay("Password:");
	printDisplay(pass);
	Second_line();
	scan_password(passwordin);
	us_unit(100000);
	clearDisplayScreen();
	char choice = '!';
	int i = 0;
	int same = 1;
	
	int passint = atoi(passwordin);
		
		if(passint != passwordck){
			LOCK();
		}else{
		clearDisplayScreen();
		printDisplay("Change the password?");
		Second_line();
		printDisplay("1:yes");
		Third_line();
		printDisplay("2:no");
		Fouth_line();
		choice = keypadpress();
		us_unit(200000);
		
		if(choice == '1'){
			change_password();
			LOCK();
		}

		clearDisplayScreen();
		Funtion_mode_select();
	}
}

void Funtion_mode_select(void) {
		scan_mode();
		us_unit(100000);
		switch (mode_sel[0]) {
    case '1':
        floatcalculate();
        break;
    case '2':
        base_convert();
        break;
    case '3':
        cal_equ();
        break;
    default:
        Funtion_mode_select();
        break;
		}
}
void change_password(){
	clearDisplayScreen();
	printDisplay("New Password:");
	Second_line();
	scan_password(passwordin);
	us_unit(100000);
	clearDisplayScreen();
	
	int i = 0;
	
	int passint = atoi(passwordin);
	
	passwordck = passint;	
}


void menu_display(){
		printDisplay("1:normal calculate");
		Second_line();
		printDisplay("2:base convert");
		Third_line();
		printDisplay("3:equation calculate");
		Fouth_line();
		printDisplay("Whats you need?:  ");
}
char eqeq[208] = { 0 };
char x_g[8] = { 0 };
void graph(void) {
    SysTick_Init();  
    Port_lcd_Init(); 
    InitDisplayPort();	
		
		clearDisplayScreen();
		First_line();
		printDisplay("The graph");
		Second_line();
		printDisplay("of the");
		Third_line();
		printDisplay("function:");
		int xary_total[3][5] = {
        {1, 2, 3, 4, 5},
        {1, 2, 3, 4, 5},
				{1, 2, 3, 4, 5},
    };
    int yary_total[3][5] = {{0}}; 
    int x_base = 0;               
    int j=0;
		int i=0;
    
    for (j = 0; j < 3; j++) {
       
        if (j == 0) {
            x_base = 0; 
        } else if (j == 1) {
            x_base = 5; 
        } else if (j == 2) {
            x_base = 10; 
        }

        for (i = 0; i < 5; i++) {
  
            int x = xary_total[j][i] + x_base;


            cv_to_str(x, x_g);
            change_equation(eqeq, equationin, x_g); 
            yary_total[j][i] = cal_dou(eqeq); 
        }
    }
    int max[3] = {0, 0, 0};
    int the_max = 0;
    int min[3] = {0, 0, 0};
    int the_min = 0;
    

    
    for (j = 0; j < 3; j++) {
        for (i = 0; i < 5; i++) {
            if (yary_total[j][i] > max[j]) {
                max[j] = yary_total[j][i];
            }
            if (i == 0 || yary_total[j][i] < min[j]) {
                min[j] = yary_total[j][i];
            }
        }
    }

    for (j = 0; j < 3; j++) {
        if (max[j] > the_max) {
            the_max = max[j];
        }
        if (j == 0 || min[j] < the_min) {
            the_min = min[j];
        }
    }

    
    for (j = 0; j < 3; j++) {
        for (i = 0; i < 5; i++) {
            int temp = yary_total[j][i];
            yary_total[j][i] = -7 + 14 * (temp - the_min) / (the_max - the_min);
        }
    }

    
    for (j = 0; j < 3; j++) {
        printY(yary_total[j], xary_total[j]);
    }
		
		wait_input();
		clearDisplayScreen();
		Funtion_mode_select();
		
}
int main(void){		
    SysTick_Init();  
    Port_lcd_Init(); 
    InitDisplayPort();
		
		LOCK();
		//Funtion_mode_select();
		//graph();
}

void printY(int *yin,int *xin){
	int *p_xin = xin;
	int *p_yin = yin;
	
	unsigned long Y_TOTAL[6][8] = {
			{ 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000 }, 
			{ 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000 },
			{ 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000 },
			{ 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000 },
			{ 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000 },
			{ 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000 }};
	int i = 0;
	while(i<5)
	{	
		if((*p_yin<8 && *p_yin>-8)==0)
		{
			printDisplay("Wrong" );
			Funtion_mode_select();
		}
		int Y;
		if(*p_yin>0)
		{
			Y = 7 - *p_yin;
			Y_TOTAL[Y_index][Y] = Y_TOTAL[Y_index][Y] | 0b00001<<(5-*p_xin);
			set_locate_Char(Y_index, Y_TOTAL[Y_index]);		
			SendDisplayByte(0x00, base_address); 
			SendDisplayByte(0x08, Y_index); 
		}
		else{
			Y = 0 - *p_yin;
			Y_TOTAL[Y_index+1][Y] = Y_TOTAL[Y_index+1][Y] | 0b00001<<(5-*p_xin);
			set_locate_Char(Y_index + 1, Y_TOTAL[Y_index+1]);
			SendDisplayByte(0x00, base_address+0xD4); 
			SendDisplayByte(0x08, Y_index +1);
		}					
		p_xin++;
		p_yin++;
		i++;			
	}
	Y_index = Y_index + 2;
	base_address = base_address + 1;	
	
	//EDGE OF GRAPH
	unsigned long EDGE1[8] = { 0b11111, 0b00000, 0b00000,0b00000,0b00000,0b00000,0b00000,0b00000 };
	unsigned long EDGE2[8] = { 0b10000, 0b10000, 0b10000,0b10000,0b10000,0b10000,0b10000,0b10000 };
	unsigned long EDGE3[8] = { 0b00000, 0b00000, 0b00000,0b00000,0b00000,0b00000,0b00000,0b11111 };
	unsigned long EDGE4[8] = { 0b00001, 0b00001, 0b00001,0b00001,0b00001,0b00001,0b00001,0b00001 };
	set_locate_Char(6, EDGE1);
	set_locate_Char(7, EDGE2);
	SendDisplayByte(0x00, 0x8A);  
	SendDisplayByte(0x08, '_'); 
	SendDisplayByte(0x00, 0x8B);  
	SendDisplayByte(0x08, '_'); 
	SendDisplayByte(0x00, 0x8C);  
	SendDisplayByte(0x08, '_');
	SendDisplayByte(0x00, 0xCD+0xD4);  
	SendDisplayByte(0x08, 7); 
	SendDisplayByte(0x00, 0xCD);  
	SendDisplayByte(0x08, 7);
	SendDisplayByte(0x00, 0xDE);  
	SendDisplayByte(0x08, 6);  
	SendDisplayByte(0x00, 0xDF);  
	SendDisplayByte(0x08, 6);  
	SendDisplayByte(0x00, 0xE0);  
	SendDisplayByte(0x08, 6);
	SendDisplayByte(0x00, 0xC9);  
	SendDisplayByte(0x08, '|'); 
	SendDisplayByte(0x00, 0xC9+0xD4);  
	SendDisplayByte(0x08, '|');
	SendDisplayByte(0x00, 0xE2);
}

float get_fx(char *equo,char *equi,char *x){		
		char *p = equo;
    while (*p != '\0') {
        *p = '\0';
        p++;
    }
		change_equation(equo,equi, x);
		us_unit(100000);
		float Y = cal_dou(equo);	
		return Y;
}

void equationinput(){
		scan(equationin,"equation: ");
		us_unit(200000);
		clearDisplayScreen();
		printDisplay(equationin);
		printDisplay("  ");		
		scanx();
		
}

float dt(char *equationin,char *xxxx){
		
		strcpy(equatinxh,equationin);
		strcpy(equatinx,equationin);
		float dx = 0;
		float f1 = 0;//fxh
		float f2 = 0;//fx
		float e = 0.001005;
		strcpy(str_x,xxxx);
		float x1 = atof(xxxx);
		float xh = x1 + e; 
		cv_to_str(xh, str_xh);
		change_equation(equation,equatinxh,str_xh);
		f1 = cal_dou(equation);	
	
		cv_to_str(x1, str_x);
		change_equation(equat, equatinx, str_x);
		f2 = cal_dou(equat);	
		float dxdy = (f1-f2)/e;
		return dxdy;
}
/*char equatdttx[208] = { 0 };
char equatdttxh[208] = { 0 };
char dtt_x[8] = { 0 };
char dtt_xh[8] = { 0 };
float dtt(char *equationin,char *dttx){
		strcpy(equatdttx,equationin);
		strcpy(equatdttxh,equationin);
		float dx = 0;
		float f1 = 0;//fxh
		float f2 = 0;//fx
		float e = 0.0125;
		strcpy(dtt_x,dttx);
		float x1 = atof(dtt_x);
		float xh = (x1 + e); 
		cv_to_str(xh, dtt_xh);
		f1 = dt(equatdttxh,dtt_xh);
		
		cv_to_str(x1, dtt_x);
		f2 = dt(equatdttx,dtt_x);

		float dxdy = (f1-f2)/(e);
		return dxdy;
}*/
char enter = 'b';
void cal_equ(){
		float result = 0;	
	
		clearDisplayScreen();
		printDisplay("whats you want?");
		Second_line();
		printDisplay("1:derivative");
		Third_line();
		printDisplay("2:get value");
		Fouth_line();
		printDisplay("3:draw graph     ");
				
		choice = keypadpress();
		us_unit(200000);
		clearDisplayScreen();
	
		if(choice == '1'){
			printDisplay("calculate ");
			Second_line();
			printDisplay("the derivative");
			us_unit(800000);
			scan(equationin,"equation");
			us_unit(200000);
			scanx();
			us_unit(100000);
			result= dt(equationin,str_from_x);
		}
		else if(choice == '2'){
			printDisplay("calculate");
			Second_line();
			printDisplay("the value");
			us_unit(800000);
			scan(equationin,"equation");
			us_unit(200000);
			scanx();
			us_unit(100000);
			result= get_fx(equationou,equationin,str_from_x);
		}
		else if(choice == '3'){
			printDisplay("draw");
			Second_line();
			printDisplay("the function");
			us_unit(800000);
			scan(equationin,"function");
			us_unit(200000);
			graph();
		}
		else{
			printDisplay("WRONG");
			wait_input();
			Funtion_mode_select();
		}
		
		outputResult(result);
		Funtion_mode_select();
}


void outputResult(float result){			
		cv_to_str(result, str_result);
		Second_line();
		printDisplay("The result is:");
		Third_line();
    printDisplay(str_result);
    wait_input();
    clearDisplayScreen();
}




void base_convert(void) {
    scan(expression,"expression"); 
    float result = 1.00; 
		convert_to_binary(expression, str_result);
		wait_input();
		clearDisplayScreen();
		printDisplay("The result is:");
		Second_line();
    printDisplay(str_result);
    wait_input();
    clearDisplayScreen();
    Funtion_mode_select(); 
}

void floatcalculate(void) {
    scan(expression,"expression"); 
    float result = 1.00;   
		result = cal_dou(expression);   
		
		cv_to_str(result, str_result);
		clearDisplayScreen();
		printDisplay("The result is:");
		Second_line();
    printDisplay(str_result);
		wait_input();
    clearDisplayScreen();
    Funtion_mode_select();    
}

void wait_input(){	
	us_unit(100000);
	wait = keypadpress();
	us_unit(100000);
}
void scan(char *str,char *str2) {
    clearDisplayScreen();
		printDisplay("Input ");
		printDisplay(str2);
		printDisplay(":");
		Second_line();
		char *p_str = str;
    char *p_end = str + 200; 

    while (p_str < p_end) {
        *p_str++ = '\0';
    }

    p_str = str;

    while (1) {
        char input = keypadpress();
        char input_op;

        if (input == 'M') {
            s_unit(2);
            input = keypad_op();
        }

        us_unit(50000);

        if (input == 'E') {
            break;
        }

        us_unit(10000);
			
        if (input == 'b') {
            us_unit(50000);
            if (p_str > str) {
                p_str--;
                *p_str = '\0';
            }
        } else {
            if (p_str - str < 100) {
                *p_str++ = input;
            }
        }

        us_unit(10000);
        
				clearDisplayScreen();
				printDisplay("Input ");
				printDisplay(str2);
				printDisplay(":");
				Second_line();
        printDisplay(str); 
        us_unit(200000);

        if (p_str - str >= 100) {
            break;
        }
    }

    SendDisplayByte(0x00, 0xC0);
}


void scanx(){
		int i = 0;		
		for (i = 0; i< 200; i++) 
			{
				str_from_x[i] = '\0';
			}
		i = 0;	
		while (1) {
					
					clearDisplayScreen();
					First_line();
					printDisplay("Input your X:");
					Second_line();
					printDisplay(str_from_x);	
			
					char input;
					input = keypadpress();
					char input_op;
					
					us_unit(50000);
					if (input == 'M')
							break;
					
					
					us_unit(10000);
					
					if (input == 'b')
					{
							us_unit(50000);
							input = '\0';
							us_unit(50000);
							i=i-1;
							str_from_x[i] = input;
					}
					else
					{
						str_from_x[i++] = input;
					}	
					us_unit(10000);
									
					us_unit(200000);
					if (i >= 100) {
							break;
					}	
			}	
		SendDisplayByte(0x00, 0xC0);
}

void scan_mode() {
		int i = 0;		
		for (i = 0; i< 200; i++) 
			{
				mode_sel[i] = '\0';
			}
		i = 0;	
		menu_display();
		while (1) {
					
					char input;
					input = keypadpress();
					char input_op;

					us_unit(50000);
					if (input == '+') //enter the strings
							break;
					
					
					us_unit(10000);
					
					if (input == 'b') //backspace
					{
							us_unit(50000);
							input = '\0';
							us_unit(50000);
							i=i-1;
							mode_sel[i] = input;
					}
					else
					{
						mode_sel[i++] = input;
					}	
					us_unit(10000);
					clearDisplayScreen();
					menu_display();
					printDisplay(mode_sel);	
					us_unit(200000);
					
					
					if (i >= 2) {
							break;
					}
					
			}	
		clearDisplayScreen();
}










void cv_to_str(float num, char* str_fromfloat) {
		char* p_clear = str_fromfloat;
		while (*p_clear) {
        *p_clear++ = '\0';
    }
		
    int precision = 3;
		int j = 0;
    int is_negative = 0;

    if (num < 0) {
        is_negative = 1;
        num = -num;
    }

    
    int integer_part = (int)num;

    
    float fractional_part = num - (float)integer_part;

    
    do {
        str_fromfloat[j++] = (integer_part % 10) + '0';
        integer_part /= 10;
    } while (integer_part > 0);

   
    if (is_negative) {
        str_fromfloat[j++] = '-';
    }

   
    int start = 0, end = j - 1;
    while (start < end) {
        char temp = str_fromfloat[start];
        str_fromfloat[start] = str_fromfloat[end];
        str_fromfloat[end] = temp;
        start++;
        end--;
    }

    
    str_fromfloat[j++] = '.';
		int i = 0;
    
    for (i = 0; i < precision; i++) {
        fractional_part *= 10;
        int digit = (int)fractional_part;
        str_fromfloat[j++] = digit + '0';
        fractional_part -= digit;
    }

  
    str_fromfloat[j] = '\0';
}

void scan_password(char *password) {
    char *p_password = password; 
    char *p_end = password + 200; 

    
    while (p_password < p_end) {
        *p_password++ = '\0';
    }

    p_password = password; 

    while (1) {
        char input = keypadpress(); 
        char input_op;

        if (input == 'M') {
            s_unit(2);
            input = keypad_op();
        }

        us_unit(50000);

        if (input == 'E') { 
            break;
        }

        us_unit(10000);

        if (input == 'b') { 
            us_unit(50000);
            if (p_password > password) {
                p_password--;
                *p_password = '\0';
            }
        } else {
            if (p_password - password < 100) {
                *p_password++ = input;
            }
        }

        us_unit(10000);
        clearDisplayScreen();
				printDisplay("Password:");
				Second_line();
        printDisplay(password); 
        us_unit(200000);

        if (p_password - password >= 100) {
            break;
        }
    }

    SendDisplayByte(0x00, 0xC0);
}
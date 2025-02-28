#define _CRT_SECURE_NO_WARNINGS
#include "calculator.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "lcd.h"
char adjusted_str[200] = { 0 };

float power(float base, float exponent) {
    float result = 1.0;
		int i = 0;
    for (i = 0; i < exponent; i++) {
        result *= base;
    }
    return result;
}

float cal_dou(char* str) {
    int j = 0;
    int i = 0;

		

    for (i = 0; i < strlen(str); i++) {
        
        if (str[i] == '-' && (i == 0 || str[i - 1] == '(' || str[i - 1] == '+' || str[i - 1] == '-' || str[i - 1] == '*' || str[i - 1] == '/' || str[i - 1] == '^')) {
            adjusted_str[j++] = '(';
            adjusted_str[j++] = '0';
            adjusted_str[j++] = '-';

            i++;
            
            while ((str[i] >= '0' && str[i] <= '9') || str[i] == '.') {
                adjusted_str[j++] = str[i++];
            }
            adjusted_str[j++] = ')';
            i--;
        }
        else {
            adjusted_str[j++] = str[i];
        }
    }
    adjusted_str[j] = '\0';

    char* p_str = adjusted_str;

    float num[100] = { 0 };
    char operator[100] = { 0 };

    int index = 0;

    char op_stack[100] = { 0 }; 
    float num_stack[100] = { 0 }; 

    int op_stack_index = -1;

    float temp = 0; 
    int temp_flag = 0; 
    float decimal_place = 0.1; 

    
    while (1) {
        if ((*p_str >= '0' && *p_str <= '9') || *p_str == '.') {
						temp_flag = 1;
						if (*p_str == '.') {
								p_str++;
								decimal_place = 0.1; 
								while (*p_str >= '0' && *p_str <= '9') {
										temp += (*p_str - '0') * decimal_place;
										decimal_place *= 0.1;
										p_str++;
								}
								decimal_place = 0.1;
								p_str--; 
						} else {
								temp *= 10;
								temp += *p_str - '0';
						}
				}

        else {
            if (temp_flag == 1) {
                num[index] = temp;
                operator[index] = '!'; 
                index++;
                temp_flag = 0;
                temp = 0;
            }

            if (!*p_str) {
                while (op_stack_index >= 0) {
                    operator[index++] = op_stack[op_stack_index--];
                }
                break;
            }
            else {
                if (op_stack_index == -1 || *p_str == '(') {
                    op_stack[++op_stack_index] = *p_str;
                }
                else if (*p_str == ')') {
										while (op_stack_index >= 0 && op_stack[op_stack_index] != '(') {
												operator[index++] = op_stack[op_stack_index--];
										}
										if (op_stack_index >= 0 && op_stack[op_stack_index] == '(') {
												--op_stack_index;
										}
								}

                else if (*p_str == '^') { 
										while (op_stack_index >= 0 && op_stack[op_stack_index] == '^') {
												operator[index++] = op_stack[op_stack_index--];
										}
										op_stack[++op_stack_index] = *p_str;
								}
                else if (*p_str == '*' || *p_str == '/') { 
                    while (op_stack_index >= 0 &&
                        (op_stack[op_stack_index] == '*' || op_stack[op_stack_index] == '/' || op_stack[op_stack_index] == '^')) {
                        operator[index++] = op_stack[op_stack_index--];
                    }
                    op_stack[++op_stack_index] = *p_str;
                }
                else { 
                    while (op_stack_index >= 0 && op_stack[op_stack_index] != '(') {
                        operator[index++] = op_stack[op_stack_index--];
                    }
                    op_stack[++op_stack_index] = *p_str;
                }
            }
        }
        p_str++;
    }
		int k = 0;
    
    op_stack_index = -1;
    for (i = 0; i < index; i++) {
        if (operator[i] == '!') { 
            num_stack[++op_stack_index] = num[i];
        }
        else { 
            float temp1 = num_stack[op_stack_index--]*1.00;
            float temp2 = num_stack[op_stack_index--]*1.00;
            switch (operator[i]) {
            case '+': num_stack[++op_stack_index] = temp2 + temp1; break;
            case '-': num_stack[++op_stack_index] = temp2 - temp1; break;
            case '*': num_stack[++op_stack_index] = temp2 * temp1; break;
            case '/': num_stack[++op_stack_index] = temp2 / temp1; break;
            case '^': num_stack[++op_stack_index] = power(temp2,temp1); 
							break;
            }
        }
    }

    
    float result = num_stack[op_stack_index]*1.00;

    return result;
}



int char_to_digit(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10; 
    return 0; 
}

char digit_to_char(int digit) {
    if (digit >= 0 && digit <= 9) return '0' + digit;
    if (digit >= 10 && digit <= 15) return 'A' + (digit - 10); 
    return '!'; 
}

char thein[200] = { 0 };
char theout[200] = { 0 };
char res[200] = { 0 };
char beout[200] = { 0 };
void convert_to_binary(char* str, char* result) {
    int input_base, output_base;
    int num = 0; 
		int len = strlen(str);
    char* p_start = str + 1; 
    char* p_end = str + len - 1;
		int i = 1;
		clearDisplayScreen();
		First_line();
		while(i!=len-1){
			SendDisplayByte(0x08,str[i]);
			i++;
		}
		strcpy(res,result);
    switch (*str) {
        case 'B': 
					input_base = 2; 
					break;
        case 'O': 
					input_base = 8; 
					break;
        case 'D':
					input_base = 10; 
					break;
        case 'H':
					input_base = 16; 
					break;
        default:
          strcpy(result, "ERROR");
          return;
    }

    switch (*p_end) {
        case 'B': 
            output_base = 2; 
            break;
        case 'O': 
            output_base = 8; 
            break;
        case 'D':
            output_base = 10; 
            break;
        case 'H':
            output_base = 16; 
            break;
        default:
            strcpy(result, "ERROR");
            return;
    }
		
    switch (input_base) {
        case 2: 
					strcpy(thein," in Bin");
					break;
        case 8: 
					strcpy(thein," in Orth"); 
					break;
        case 10:
					strcpy(thein," in Dec"); 
					break;
        case 16:
					strcpy(thein," in Hex"); 
					break;
        default:
          strcpy(thein, "ERROR");
          return;
    }
    switch (output_base) {
        case 2: 
					strcpy(theout," in Bin");
					break;
        case 8: 
					strcpy(theout," in Orth"); 
					break;
        case 10:
					strcpy(theout," in Dec"); 
					break;
        case 16:
					strcpy(theout," in Hex"); 
					break;
        default:
          strcpy(theout, "ERROR");
          return;
    }
		
		
		while (p_start < p_end) {
        int digit = char_to_digit(*p_start);
        if (digit < 0 || digit >= input_base) {
            strcpy(result, "ERROR");
            return;
        }
        num = num * input_base + digit;
        p_start++;
    }	
    
    i = 0;
    char temp_result[100]; 
    do {
        int digit = num % output_base;
        temp_result[i++] = digit_to_char(digit);
        num /= output_base;
    } while (num > 0);

    
    int j = 0;
    while (i > 0) {
        result[j++] = temp_result[--i];
    }
    result[j] = '\0'; 
		
		
		
		printDisplay(thein);
		Second_line();
		printDisplay("is: ");
		Third_line();
		printDisplay(result);
		printDisplay(theout);
		
		us_unit(300000);
}

void change_equation(char *equation_out, char *equation_in, char *xx) {
    char *p_str = equation_in;      
    char *p_eq = equation_out; 
    char *p_xx = xx;        

    while (*p_str != '\0') {
        if (*p_str == 'x') {
            char *p_x_copy = xx; 
            while (*p_x_copy != '\0') {
                *p_eq = *p_x_copy;
                p_eq++;
                p_x_copy++;
                us_unit(100); 
            }
        } else {
            *p_eq = *p_str;
            p_eq++;
            us_unit(100); 
        }
        p_str++;
        us_unit(100); 
    }

    *p_eq = '\0'; 

}

void change_equatxh(char *equatxh_out, char *equatxh_in, char *xxh){
char *p_strxh = equatxh_in;     
    char *p_eqxh = equatxh_out;  
    char *p_xxh = xxh;        

    while (*p_strxh != '\0') {
        if (*p_strxh == 'x') {
            char *p_x_copy = xxh; 
            while (*p_x_copy != '\0') {
                *p_eqxh = *p_x_copy;
                p_eqxh++;
                p_x_copy++;
                us_unit(100); 
            }
        } else {
            *p_eqxh = *p_strxh;
            p_eqxh++;
            us_unit(100); 
        }
        p_strxh++;
        us_unit(100); 
    }

    *p_eqxh = '\0'; 
    us_unit(100000); 
}

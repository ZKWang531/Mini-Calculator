#ifndef CALCULATOR_H
#define CALCULATOR_H

int cal(char* str);
float cal_dou(char* str);
void convert_to_binary(char* str, char* result);
float power(float base, float exponent);
void to_str(float num, char* str_fromfloat);
float get_equation(char *str, float xx);
void change_equation(char *equation_out, char *equation_in, char *xx);
void change_equatxh(char *equatxh_out, char *equatxh_in, char *xxh);
float stringtofloat(char *x);
#endif

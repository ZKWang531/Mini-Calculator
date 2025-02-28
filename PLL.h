#ifndef PLL_H
#define PLL_H


void PLL_Init(void);
void SysTick_Init(void);
void SysTick_Wait(unsigned long delay);
void us_unit(unsigned long delay);

#endif // PLL_H

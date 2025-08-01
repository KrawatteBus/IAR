#ifndef ICLED_H
#define ICLED_H

#include "main.h"

void ICLED_Init(void);
void ICLED_setPixel(uint8_t green, uint8_t red, uint8_t blue);
void ICLED_showReset(void);

void ICLED_DWT_SelfTest(void);

#endif
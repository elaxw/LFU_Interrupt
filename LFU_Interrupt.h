#pragma once
#include "timer.c"



static void NVIC_init(char position, char priority);
void TIM2_IRQHandler(void);
static void TIM1_Config(void);
void welcomeText();
void initTime();
void timeFormat();

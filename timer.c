#pragma once
#include "LFU_Interrupt.h"

static int milliS;
static int seconds;
static int minutes;
static int hours;


static void NVIC_init(char position, char priority) 
{	
	NVIC->IP[position]=(priority<<4);	//Interrupt priority register: Setzen der Interrupt Priorit�t
	NVIC->ICPR[position >> 0x05] |= (0x01 << (position & 0x1F));//Interrupt Clear Pendig Register: Verhindert, dass der Interrupt ausl�st sobald er enabled wird 
	NVIC->ISER[position >> 0x05] |= (0x01 << (position & 0x1F));//Interrupt Set Enable Register: Enable interrupt
} 

void TIM2_IRQHandler(void)	//Timer 1, l�st alle 1000ms aus
{
	TIM2->SR &=~0x01;	//Interrupt pending bit l�schen (Verhinderung eines nochmaligen Interrupt-ausl�sens)
	milliS++;
}
/******************************************************************************/
/*                     Initialization Timer2 (General Purpose Timer)          */
/******************************************************************************/
static void TIM1_Config(void)
{	
	/*---------------------- Timer 2 konfigurieren ---------------------------*/
	RCC->APB1ENR |= 0x01;	//Timer 2 Clock enable
	TIM2->SMCR = 0x0000;	//Timer 2 Clock Selection: CK_INT wird verwendet
	TIM2->CR1  = 0x0000;	//Auswahl des Timer Modus: Upcounter --> Z�hlt: von 0 bis zum Wert des Autoreload-Registers

	/* T_INT = 126,26ns, Annahme: Presc = 15 ---> Auto Reload Wert = 52801 (=0xCE41) --> 1s Update Event*/
	//TIM2->PSC = 60;		//Wert des prescalers (Taktverminderung) orig: 150
	//TIM2->ARR = 0xCE41;	//Auto-Reload Wert = Maximaler Zaehlerstand des Upcounters

	//TIM2->PSC = 999;
	//TIM2->ARR = 320;

	TIM2->PSC = 244;	//Korrektzr lt. Messung und anschliessender Berechnung
	TIM2->ARR = 2606;	//Korrektzr lt. Messung und anschliessender Berechnung
	/*--------------------- Interrupt Timer 2 konfigurieren ------------------*/
	TIM2->DIER = 0x01;	   //enable Interrupt bei einem UEV (�berlauf / Unterlauf)
	NVIC_init(TIM2_IRQn,2);	   //Enable Timer 1 Update Interrupt, Priority 2

	/*-------------------------- Timer 2 Starten -----------------------------*/
    TIM2->CR1 |= 0x0001;   //Counter-Enable bit setzen
}

void timeFormat() {
	if(milliS > 99) {
		milliS = 0;
		seconds ++;
	}
	if(seconds > 59){
		minutes++;
		seconds = 0;
	}
	if(minutes > 59) {
		hours++;
		minutes = 0;
	}
}

void initTime() {
	milliS = 0;
	seconds = 0;
	minutes = 0;
	hours = 0;
}

void welcomeText() {
	lcd_clear();
	lcd_set_cursor(0,0);
	lcd_put_string("Hallo!");
	wait_ms(5000);
	lcd_clear();
	lcd_set_cursor(0,0);
	lcd_put_string("Bitte HW checken");
	wait_ms(10000);
	lcd_clear();
}

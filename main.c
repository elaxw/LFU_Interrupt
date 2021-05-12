#include <stdlib.h>
#include "armv10_std.h"
#include <string.h>
#include <stdlib.h>
#include "std_stm32_f103rb.h"
#include "LFU_Interrupt.h"
//Prozess A: Hauptprogramm, Initialisierung, LCD Ausgabe
//Prozess B: Uhrenfunktion
//Prozess C: erfassung vom LFU

int main() {
	char buffer[16];
	lcd_init();
	welcomeText();
	TIM1_Config();

	while(1) {
		timeFormat();
		sprintf(buffer, "%d:%d:%d.%d  ", hours, minutes, seconds, milliS);
		lcd_set_cursor(1,0);
		lcd_put_string(buffer);
	}
	
	return 0;
}

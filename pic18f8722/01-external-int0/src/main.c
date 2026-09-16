/*
 * 01 - Intrerupere externa INT0
 *
 * La activarea INT0, LED-ul de pe RD0 clipeste de cateva ori.
 */
#include <xc.h>
#define _XTAL_FREQ 4000000

void interrupt ext0(){
if (INT0IF){
for (int i=0;i<=19;i++){
RD0 = ~RD0;
for (int i=0;i<=99;i++){
__delay_ms(5);}
}
}
INT0IF = 0;
}

void main(void){
   LATD = 0x0;
   TRISD = 0x0;
	GIE = 1;
	INT0IE = 1;
	INT0IF = 0;
	INTEDG0 = 1;
   while (1)
    {} // Asteapta evenimentul extern.
 }



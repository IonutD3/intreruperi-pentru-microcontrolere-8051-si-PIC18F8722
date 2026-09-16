/*
 * 03 - Prioritate HIGH/LOW pentru intreruperi externe
 *
 * INT2 este HIGH, iar INT1 este LOW. Astfel demonstram cele doua
 * niveluri de prioritate ale PIC18F8722.
 */
#include <xc.h>
#define _XTAL_FREQ  4000000

void interrupt low_priority ext2(){
if(INT2IF){
for (int i=0;i<=5;i++){
RD1= ~RD1;
for (int i=1;i<=100;i++){
__delay_ms(5);}
}
INT2IF = 0; }
}

void interrupt high_priority ext1(){
if(INT1IF){
for (int i=0;i<=5;i++){
RD0 = ~RD0;
for (int i=1;i<=100;i++){
__delay_ms(5);}
}
INT1IF = 0;}
}

void main(){
LATD = 0x0;
TRISD = 0x0; 
GIEH = 1;
GIEL = 1;
INT1IP = 1;
INT2IP = 0; 
IPEN = 1;
INT2IE =1; 
INT1IE =1;
INT2IF =0; 
INT1IF =0;
INTEDG2 =1; 
INTEDG1 =1; 
while(1) {} // Prioritatea este gestionata de hardware.
}

/*
 * 02 - INT1 si INT2
 *
 * Fiecare intrerupere externa comuta o pereche de LED-uri.
 */
#include <xc.h>
#define _XTAL_FREQ  4000000

void main(){
LATD = 0x0;
TRISD = 0x0; 
GIE=1; 
INT2IE =1; 
INT1IE =1;
INT2IF =0; 
INT1IF =0;
INTEDG2 =1; 
INTEDG1 =1; 
while(1) {} // Nu este nevoie de polling in bucla principala.
}

void interrupt ext2(){
if(INT1IF){
for (int i=1;i<=2;i++){
RD0 = ~RD0;
for (int i=1;i<=100;i++){
__delay_ms(5);}
}
}

if(INT2IF){
for (int i=1;i<=2;i++){
RD1= ~RD1;
for (int i=1;i<=100;i++){
__delay_ms(5);}
}
}
INT1IF = 0; 
INT2IF = 0; 
}


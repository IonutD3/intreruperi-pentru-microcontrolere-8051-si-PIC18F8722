/* 01-timer-and-external-interrupts
 *
 */

#include <reg51.h>
#include <stdio.h>

sbit LED  = P1^0;
sbit LED1  = P1^1;
sbit LED2  = P1^2;

int counter = 0; 
int counter1 = 0;
int i;

void delay(const unsigned x){
unsigned int i, j;
for(i=0;i<x;i++){
for (j=0;j<=120;j++);}
}

void timer0_isr() interrupt 1{
TH0 = 0xFC;
TL0 =0x18;
counter++;
if (counter == 1000){
LED = ~LED;
counter =0;}
}

void timer1_isr() interrupt 3{
TH1 = 0xFC;
TL1 =0x18;
counter1++;
if (counter1 == 500){
LED1 =~ LED1;
counter1 =0;}
}

void ex0_isr() interrupt 0{
for(i=0;i<=19;i++){
LED2 = ~LED2;
delay(250);}
}

void main(void){ 
 TMOD = 0x11;      
    TR0 = 1;             
    TR1 = 1;     
    IT0 = 1;
    PT0 = 0;
    PT1 = 1;
    IE = 0x8b;    
    while(1)
    {} 
}

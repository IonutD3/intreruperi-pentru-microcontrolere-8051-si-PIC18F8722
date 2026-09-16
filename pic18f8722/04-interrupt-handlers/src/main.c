/*
 * 04 - Mai multe surse si handler-e separate
 *
 * INT0..INT3 sunt rutate catre functii separate. Structura este
 * pastrata intentionat pentru a demonstra organizarea unui ISR.
 */
#include <xc.h>
#define _XTAL_FREQ 4000000

void ext0();
void ext1();
void ext2();
void ext3();

void interrupt high_priority ex1(){
if (INT0IF){
ext0();}
if (INT1IF){
ext1();}
}

void interrupt low_priority ex2(){
if (INT2IF){
ext2();}
if (INT3IF){
ext3();}
}

void ext0(){
for (int i=0;i<=5;i++){
RD0 = ~RD0;
for (int i=0;i<=99;i++)
__delay_ms(5);}
INT0IF = 0;}

void ext1(){
for (int i=0;i<=5;i++){
RD1 = ~RD1;
for (int i=0;i<=99;i++)
__delay_ms(5);}
INT1IF = 0;}

void ext2(){
for (int i=0;i<=5;i++){
RD2 = ~RD2;
for (int i=0;i<=99;i++)
__delay_ms(5);}
INT2IF = 0;}

void ext3(){
for (int i=0;i<=5;i++){
RD3 = ~RD3;
for (int i=0;i<=99;i++)
__delay_ms(5);}
INT3IF = 0;}

void main(void){
   LATD = 0x0;
  TRISD = 0x0;
//Activare intreruperi externe 0, 1, 2, 3 (high si low)
  GIEH = 1;
  GIEL = 1;
  IPEN = 1;
  INT0IE = 1;
  INT0IF = 0;
  INT1IE = 1;
  INT1IF = 0;
  INT2IE = 1;
  INT2IF = 0;
  INT3IE = 1;
  INT3IF = 0;
  INTEDG0 = 1;
  INTEDG1 = 1;
  INTEDG2 = 1;
  INTEDG3 = 1;
  INT1IP = 1;  //Nivel high
  INT2IP = 0;  //Nivel low
  INT3IP = 0;  //Nivel low
   while (1) 
    {}// Dispatcher-ul trateaza evenimentele externe.  
 }

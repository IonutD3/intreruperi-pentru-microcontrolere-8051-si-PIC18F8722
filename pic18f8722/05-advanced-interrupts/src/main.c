/*
 * 05 - Arhitectura avansata de intreruperi
 *
 * Sunt folosite patru intrari externe si trei timere, impartite
 * intre nivelurile HIGH si LOW. Exemplul arata cum pot fi grupate
 * mai multe surse intr-un sistem de intreruperi.
 */
#include <xc.h>
#define _XTAL_FREQ 4000000

void ext0();
void ext1();
void ext2();
void ext3();
void ext4();
void ext5();
void ext6();

int counter = 0; 
int counter1 = 0; 
int counter2 = 0;

void interrupt high_priority ex1(){
if (INT0IF){
ext0();}
if (INT1IF){
ext1();}
if(TMR0IF){
ext4();}
if(TMR2IF){
ext6();}
}

void interrupt low_priority ex2(){
if (INT2IF){
ext2();}
if (INT3IF){
ext3();}
if(TMR1IF){
ext5();}
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

void ext4(){
counter++;
if(counter == 30){
RD4 = ~RD4;
counter = 0;}
TMR0IF = 0;}

void ext5(){
counter1++;
if(counter1 == 2){
RD5 = ~RD5;
counter1 = 0;}
TMR1IF = 0;}

void ext6(){
counter2++;
if(counter2 == 3921){
RD6 = ~RD6;
counter2 = 0;}
TMR2IF = 0;}

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

//configurare timer 0
  T08BIT = 1;
  T0CS = 0;
  PSA = 0;
  T0PS2 = 1;
  T0PS1 = 0;
  T0PS0 = 1;
  TMR0IE = 1;
  TMR0IF = 0;
  TMR0IP =1; // Nivel high
  TMR0L = 0; 
  TMR0ON =1;

//configurare timer 1
T1CONbits.RD16 = 0;
T1CKPS1 = 0;
T1CKPS0 = 1;
TMR1CS = 0;
TMR1IE =1;
TMR1IF = 0;
TMR1IP = 0; // Nivel low
TMR1H = 0;
TMR1L = 0;
TMR1ON = 1;

//configurare timer 2
T2CKPS1 = 0;
T2CKPS0 = 0;
TMR2IE = 1;
TMR2IF = 0;
TMR2IP = 1;//Nivel high
TMR2 = 0;
PR2 = 255;
TMR2ON = 1;
   while (1)
      {} // Sistemul este condus de evenimente.
 }

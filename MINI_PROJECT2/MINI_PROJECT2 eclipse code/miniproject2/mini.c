/*
 * mini.c
 *
 *  Created on: Sep 17, 2021
 *      Author: Heba
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int seconds=0; //lw heya local hatbda2 melzero kol mara
int minutes=0;
int hours=0;
unsigned char sec_unit=0;
unsigned char sec_tens=0;
unsigned char min_unit=0;
unsigned char min_tens=0;
unsigned char hours_unit=0;
unsigned char hours_tens=0;

void Timer1 (void){
	/*we put only equal because others are already seroes*/
	TCCR1A=(1<<FOC1A); //ALL BITS ARE ZEROES THAT'S WHY WE DON'T NEED TO INITIALIZE THE OTHERS WITH ZEROES
	TCCR1B=(1<<WGM12)|(1<<CS10)|(1<<CS12);//initialize compare mode to 1
	//register kber fa 2smo 3la etnen tccr1a we tccr1b
	TCNT1=0;
	OCR1A=1000;
	TIMSK|=(1<<OCIE1A);
}
ISR (TIMER1_COMPA_vect){
//Function for timer 1 if it got an interrupt
/*It's always better to make the isr function short so we can put a flag and take
 the flag in the main and assign the ifs in it*/
seconds++;
if(seconds==60){
	seconds=0;
	minutes++;
}
if (minutes==60){
	minutes=0;
	hours++;
}
}
ISR (INT0_vect){
	/*SREG|=(1<<7);*/
	seconds=0;
	minutes=0;
	hours=0;
}

void INT0_Init (void){
	//Function elreset
	DDRD&=~(1<<PD2); //input
	PORTD|=(1<<PD2); //initialization for internal pull up resis
	GICR|=(1<<INT0);
	MCUCR|=(1<<ISC01);
	MCUCR&=~(1<<ISC00);
}
ISR (INT1_vect){

	TCCR1B&=~(1<<CS10)&~(1<<CS12)&~(1<<CS11); //setting bits to 0
}
void INT1_Init(void){
	//Function elpause
	DDRD&=~(1<<PD3); //input
	GICR|=(1<<INT1); //setting this bit only to 1 so as not to change the other values
	MCUCR|=(1<<ISC10)|(1<<ISC11);
	/* EL DISABLE WELENABLE BETA3 EL I-BIT BYT3ML LW7DO FELFUNCTION MSH M7TAG 23MLO*/
}
ISR (INT2_vect){

	TCCR1B|=(1<<CS10)|(1<<CS12); //setting bits to 1
}
void INT2_Init(void){
	//Function elresume
	DDRB&=~(1<<PB2); //input pin
	PORTB|=(1<<PB2); //setting the value of the bit to 1
	//lama 5lena el port be 1 yeb2a keda sh3'lna el internal pull up resis
    GICR|=(1<<INT2); //enable of general control register
    MCUCSR&=~(1<<ISC2); //setting to zero
}

void display (void){
	//setting the bit makes the bit called value is 1 and the others are left
    //laken hena mushkela 3shan ana 3yez a7ot elba2y be zeroes welie fat7ha bas be 1
    //set first bit to 1 and other 5 bits to 0
    unsigned char sec_unit=seconds%10;
    unsigned char sec_tens= seconds/10;
    unsigned char min_unit=minutes%10;
    unsigned char min_tens=minutes/10;
    unsigned char hours_unit=hours%10;
    unsigned char hours_tens=hours/10;
    //PORT A MS2OL 3AN ELSEGMENT
    //PORT C MS2OL 3AN ELDISPLAY
   /* PORTA & 11000000  ..000000
      PORTA | 00000001  .......1
      **************************
      PORTC & 00000000  00000000
      PORTC | 00000001  .......1
    */
    PORTA=(PORTA&0xC0)|(0xC1);
    PORTC=(PORTC&0xF0)|(sec_unit & 0x0F);
    _delay_ms(4);
    PORTA=(PORTA&0xC0)|(0xC2);
    PORTC=(PORTC&0xF0)|(sec_tens & 0x0F);
    _delay_ms(4);
    PORTA=(PORTA&0xC0)|(0xC4);
    PORTC=(PORTC&0xF0)|(min_unit & 0x0F);
    _delay_ms(4);
    PORTA=(PORTA&0xC0)|(0xC8);
    PORTC=(PORTC&0xF0)|(min_tens & 0x0F);
    _delay_ms(4);
    PORTA=(PORTA&0xC0)|(0xD0);
    PORTC=(PORTC&0xF0)|(hours_unit & 0x0F);
    _delay_ms(4);
    PORTA=(PORTA&0xC0)|(0xF0);
    PORTC=(PORTC&0xF0)|(hours_tens & 0x0F);
    _delay_ms(4);
}

int main (void){
SREG|=(1<<7);	//Initialization of global i-bit
DDRC|=(1<<0)|(1<<1)|(1<<2)|(1<<3); //OUTPUT PIN
PORTC&=~0x0F;//INITIALIZE to 0 by and not with ones
DDRA|=(1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5);//Output pins yetl3 3al segment
PORTA&=~ 0x3F;// INITIALIZE to 0 by anding not with six ones

INT0_Init();
INT1_Init();
INT2_Init();
Timer1(); //calling the timer function

while (1){
display();
}
}

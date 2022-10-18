/*
PORTD
DHT11
PD0 -----	DATA
GND ------	GND
VCC	------	5V

PORTB
GND --- GND


PORTC
PC0 -----
PC1 -----
PC2 ----- MID (POTETIOMETER)
PC3 ----- (+) (BUZZER)
PC4 ----- SDA (I2C LCD)
PC5 ----- SCL (I2C LCD)
 
 DHT11		BUZZER		   P.BUTTON
 _____
|_____|		(+___)			{ O }
| | |		 |  |			 | |
G D V		 P	G			 P G
N A C		 C	N		     E N
D T C		 3	D			 3 D
A							 +
							 10K OHM (RESISITOR GOES TO SOURCE)
*/

#define F_CPU 16000000 // frequency of the ATMEGA328PB microprocessor used
#include <stdio.h> // standard library
#include <avr/io.h> // standard library
#include <util/delay.h> // standard library
#include <stdio.h> // standard library
#include <stdint.h> // standard library
#include <stdlib.h> // standard library
#include <string.h> // use for iota() function
#include "ADC.h" // header file to use Analog To Digital Converter 
#include "I2C.h" // header file to interface with I2C Protocol
#include "DHT11.h" // header file interface with DHT11 Temperature and Humidity sensor
#define Potentiometer 100.00 //the value of the potentiometer in resistance values higher can crash program

int main()
{
	lcd_init(); //initialize LCD
	char data[5];//define a char for itoa function
	double TempSetPref; //
	Init_ADC();
	while(1)
	{////first while loop start
		DDRE = ~(1<<3);
		int X; // integer holder to carry integers around program (This probably only makes since too myself Curtis Searle)
		TempSetPref=(double)Potentiometer/1024*ReadADC_0(0); //reading Y axis value in Ohms
		_delay_ms(250); //wait
		lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
		lcd_PrintString("TEMP = ");
		lcd_GoToXY(0,1); // position on LCD first column and first row (settings is backwards)
		lcd_PrintString("THEN PUSH BUTTON");
		itoa(TempSetPref,data,10); // itoa() functions converts integer to string
		lcd_GoToXY(8,2); // position on LCD first column and second row
		lcd_PrintString(data); // print value stores in char data		
		_delay_ms(250); // wait
		X = TempSetPref; //carry the value of TEMP (temperature set by user) in X
			if (PINE != (1<<3)) // if button press is push begin if statement
			{/// first if loop start
				//X = TempSetPref; //carry the value of TEMP (temperature set by user) in X
				_delay_ms(2000); // wait 
				lcd_WriteCommand(0x01); // clear LCD
				_delay_ms(500); // wait 
				lcd_GoToXY(0,2);
				lcd_PrintString("Temperature set too:"); // confirm preferences
				itoa(TempSetPref,data,10); // convert integer to string
				lcd_GoToXY(0,1); // position on LCD first column and second row
				lcd_PrintString(data); // char declaration to satisfy itoa function
				_delay_ms(1000);
				lcd_WriteCommand(0x01);
				_delay_ms(500);
		while(1) 
		{ // second while statement
		Request(); // too START DHT11 send start pulse in bits
		Response(); // receive response in form of bits
		I_RH=Receive_data(); // store first eight bit in I_RH
		D_RH=Receive_data(); // store next eight bit in D_RH
		I_Temp=Receive_data(); // store next eight bit in I_Temp
		D_Temp=Receive_data(); // store next eight bit in D_Temp
		CheckSum=Receive_data();// store next eight bit in CheckSum
			if ((I_RH + D_RH + I_Temp + D_Temp) != CheckSum)
			{ //third if statement
				lcd_GoToXY(0,0);
				lcd_PrintString("Error");
			} // third if statement end
			else // first else statement
			{
				char data[5];// use to satisfie iota() function
				I_Temp = (I_Temp);
				lcd_init(); // Initialize LCD
				lcd_GoToXY(1,1); // Enter column and row position
				lcd_PrintString("Temp = ");
				lcd_GoToXY(1,2);
				lcd_PrintString("Humidity = ");
				//I_Temp = (I_Temp);
				itoa(I_RH,data,10);
				lcd_GoToXY(12,2);
				lcd_PrintString(data);
				lcd_PrintString(".");
				// integer temperature display
				itoa(D_RH,data,10);
				lcd_PrintString(data);
				lcd_PrintString("%");
				// Decimal humidity display
				itoa(I_Temp,data,10);
				lcd_GoToXY(6,1);
				lcd_PrintString(data);
				lcd_PrintString(".");
				// Integer Temperature display
				itoa(D_Temp,data,10);
				lcd_PrintString(data);
				lcd_WriteData(0xDF);
				lcd_PrintString("F");
				// Check sum display
				itoa(CheckSum,data,10);
				lcd_GoToXY(13,1);
				lcd_PrintString(data);
				lcd_PrintString(" ");
				_delay_ms(2000);
				
				if (I_Temp <= X)
				{
					_delay_ms(1000);
					lcd_WriteCommand(0x01);
					_delay_ms(1000);
					lcd_GoToXY(0,2);
					lcd_PrintString("THIS IS VERY BAD");
					lcd_GoToXY(0,1);
					lcd_PrintString("THIS IS VERY BAD");
					//_delay_ms(1000);
					//lcd_WriteCommand(0x01);						
					//_delay_ms(5000);
					while(1)
					{
						lcd_GoToXY(0,2);
						DDRC = (1<<3);
						PORTC = (1<<3);     // Turn ON the Buzzer connected to PORTC
						_delay_us(100);      // Wait time delay will effect the frequency of buzz
						PORTC = ~(1<<3);       // Turn OFF the Buzzer connected to PORTC
						_delay_us(100);      // Wait time delay will effect the frequency of buzz
						continue;
					}

				}
			continue;
		}//close second if statement
		}//close second while statement
		continue;	
  } //close 1st if while
 } //close 1st while
} // close main

/*

PORTD
DHT11
PD0 -----  DATA
GND ------ GND
VCC ------ 5V
PORTB
GND --- GND
PORTC
PC0 ----- MID (POTETIOMETER)
PC1 -----
PC2 ----- 
PC3 ----- (+) (BUZZER)
PC4 ----- SDA (I2C LCD)
PC5 ----- SCL (I2C LCD)
 
 DHT11		BUZZER		P.BUTTON
 _____
|_____|		(+___)		{ O }
| | |		 |  |		 | |
G D V		 P  G		 P G
N A C		 C  N		 E N
D T C		 3  D		 3 D
A				 +
				 10K OHM (RESISITOR GOES TO SOURCE)
							 
potentiometer
 _____
(_____)
 | | |
 | | |
 G P V
 N C C
 D 0 C
   
*/

#define F_CPU 16000000 // frequency of the ATMEGA328PB microprocessor used
#include <stdio.h> // standard library
#include <avr/io.h> // standard library
#include <util/delay.h> // standard library
#include <stdio.h> // standard library
#include <stdint.h> // standard library
#include <stdlib.h> // standard library
#include <string.h> // use for iota() function
#include <avr/interrupt.h> // used for interrupt services 
#include "ADC.h" // header file to use Analog To Digital Converter 
#include "I2C.h" // header file to interface with I2C Protocol
#include "DHT11.h" // header file interface with DHT11 Temperature and Humidity sensor
#define Potentiometer 100.00 //the value of the potentiometer in resistance values higher can crash program


ISR(PCINT0_vect) // PCINT0 = PORTB
{
	PINE != (1<<3);
}

int main()
{
	lcd_init(); //initialize LCD
	char data[5];//define a char for itoa function
	double TempSetPref; //
	Init_ADC();
	
			while(1)
			{
				lcd_WriteCommand(0x01);
				_delay_ms(100);
				lcd_GoToXY(3,2); // position on LCD first column and first row (settings is backwards)
				lcd_PrintString("Welcome To");
				lcd_GoToXY(3,1); // position on LCD first column and first row (settings is backwards)
				lcd_PrintString("ECE388 2022");
				_delay_ms(4000);
								
				lcd_WriteCommand(0x01);
				lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
				lcd_PrintString("Please Select");
				lcd_GoToXY(0,1); // position on LCD first column and first row (settings is backwards)
				lcd_PrintString("High Temperature");
				_delay_ms(4000);
				lcd_WriteCommand(0x01);
				break;
			}
			
	while(1)
	{

	DDRE = ~(1<<3);
	int X0; // integer holder to carry integers around program (This probably only makes since too myself Curtis Searle)
	TempSetPref=(double)Potentiometer/1024*ReadADC_0(0); //reading Y axis value in Ohms
		lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
		lcd_PrintString("TEMP = ");
		lcd_GoToXY(9,2); // position on LCD first column and first row (settings is backwards)
		lcd_WriteData(0XDF);
		lcd_GoToXY(10,2); // position on LCD first column and first row (settings is backwards)
		lcd_PrintString("F");
		lcd_GoToXY(0,1); // position on LCD first column and first row (settings is backwards)
		lcd_PrintString("THEN PUSH BUTTON");
		itoa(TempSetPref,data,10); // itoa() functions converts integer to string
		lcd_GoToXY(7,2); // position on LCD first column and second row
		lcd_PrintString(data); // print value stores in char data		
	X0 = TempSetPref; //carry the value of TEMP (temperature set by user) in X0
		
if (PINE != (1<<3))
{
			while(1)
			{
				lcd_WriteCommand(0x01); // CLEAR lcd for new message
				lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
				lcd_PrintString("Please Select");
				lcd_GoToXY(0,1); // position on LCD first column and first row (settings is backwards)
				lcd_PrintString("Low Temperature");
				_delay_ms(4000);
				lcd_WriteCommand(0x01); // CLEAR lcd for new message
				break;
			}
	
	while(1)
	{
		DDRE = ~(1<<3);
		int X1; // integer holder to carry integers around program (This probably only makes since too myself Curtis Searle)
		TempSetPref=(double)Potentiometer/1024*ReadADC_0(0); //reading Y axis value in Ohms
			_delay_ms(250); //wait
			lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
			lcd_PrintString("TEMP = ");
			lcd_GoToXY(9,2); // position on LCD first column and first row (settings is backwards)
			lcd_WriteData(0XDF);
			lcd_GoToXY(10,2); // position on LCD first column and first row (settings is backwards)
			lcd_PrintString("F");
			lcd_GoToXY(0,1); // position on LCD first column and first row (settings is backwards)
			lcd_PrintString("THEN PUSH BUTTON");
			itoa(TempSetPref,data,10); // itoa() functions converts integer to string
			lcd_GoToXY(7,2); // position on LCD first column and second row
			lcd_PrintString(data); // print value stores in char data
		X1 = TempSetPref; //carry the value of TEMP (temperature set by user) in X

if (PINE != (1<<3))
{
				while(1)
				{
					lcd_WriteCommand(0x01);
					lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
					lcd_PrintString("Please Select");
					lcd_GoToXY(0,1); // position on LCD first column and first row (settings is backwards)
					lcd_PrintString("High Humidity");
					_delay_ms(3000);
					lcd_WriteCommand(0x01);
					break;
				}
	while(1)
	{
	DDRE = ~(1<<3);
	int Y0; // integer holder to carry integers around program (This probably only makes since too myself Curtis Searle)
	TempSetPref=(double)Potentiometer/1024*ReadADC_0(0); //reading Y axis value in Ohms
			_delay_ms(250); //wait
			lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
			lcd_PrintString("HUMID = ");
			lcd_GoToXY(10,2); // position on LCD first column and first row (settings is backwards)
			lcd_PrintString("%");
			lcd_GoToXY(0,1); // position on LCD first column and first row (settings is backwards)
			lcd_PrintString("THEN PUSH BUTTON");
			itoa(TempSetPref,data,10); // itoa() functions converts integer to string
			lcd_GoToXY(8,2); // position on LCD first column and second row
			lcd_PrintString(data); // print value stores in char data
	Y0 = TempSetPref; //carry the value of TEMP (temperature set by user) in X

	if (PINE != (1<<3))
	{
				while(1)
				{
					lcd_WriteCommand(0x01);
					lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
					lcd_PrintString("Please Select");
					lcd_GoToXY(0,1); // position on LCD first column and first row (settings is backwards)
					lcd_PrintString("Low Humidity");
					_delay_ms(3000);
					lcd_WriteCommand(0x01);
					break;
				}
	while(1)
	{
	DDRE = ~(1<<3);
	int Y1; // integer holder to carry integers around program (This probably only makes since too myself Curtis Searle)
	TempSetPref=(double)Potentiometer/1024*ReadADC_0(0); //reading Y axis value in Ohms
		lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
		lcd_PrintString("HUMID = ");
		lcd_GoToXY(10,2); // position on LCD first column and first row (settings is backwards)
		lcd_PrintString("%");
		lcd_GoToXY(0,1); // position on LCD first column and first row (settings is backwards)
		lcd_PrintString("THEN PUSH BUTTON");
		itoa(TempSetPref,data,10); // itoa() functions converts integer to string
		lcd_GoToXY(8,2); // position on LCD first column and second row
		lcd_PrintString(data); // print value stores in char data
	Y1 = TempSetPref; //carry the value of TEMP (temperature set by user) in X

			if (PINE != (1<<3)) // if button press is push begin if statement
			{/// first if loop start
				//X = TempSetPref; //carry the value of TEMP (temperature set by user) in X
				_delay_ms(550); // wait 
				lcd_WriteCommand(0x01); // clear LCD
				
				lcd_GoToXY(0,2);
				lcd_PrintString("Upper Limits"); // confirm preferences
				lcd_GoToXY(0,1);
				lcd_PrintString("are set too:"); // confirm preferences
				_delay_ms(2500); // wait 
	
				lcd_WriteCommand(0x01); // clear LCD
				lcd_GoToXY(0,2);
				lcd_PrintString("Temp ="); // confirm preferences
				itoa(X0,data,10); // convert integer to string
				lcd_GoToXY(7,2); // position on LCD first column and second row
				lcd_PrintString(data); // char declaration to satisfy itoa function
				lcd_GoToXY(14,2); // position on LCD first column and first row (settings is backwards)
				lcd_WriteData(0XDF);
				lcd_GoToXY(15,2);
				lcd_PrintString("F"); // confirm preferences		
				lcd_GoToXY(0,1);
				lcd_PrintString("Humid ="); // confirm preferences
				itoa(Y0,data,10); // convert integer to string
				lcd_GoToXY(8,1); // position on LCD first column and second row
				lcd_PrintString(data); // char declaration to satisfy itoa function
				lcd_GoToXY(113,2);
				lcd_PrintString("%"); // confirm preferences
				_delay_ms(2500); // wait 
				
				lcd_WriteCommand(0x01); // clear LCD
				lcd_GoToXY(0,2);
				lcd_PrintString("Lower Limits"); // confirm preferences
				lcd_GoToXY(0,1);
				lcd_PrintString("are set too:"); // confirm preferences
				_delay_ms(2500); // wait 
				
				lcd_WriteCommand(0x01); // clear LCD
				lcd_GoToXY(0,2);
				lcd_PrintString("Temperature ="); // confirm preferences
				itoa(X1,data,10); // convert integer to string
				lcd_GoToXY(14,2); // position on LCD first column and second row
				lcd_PrintString(data); // char declaration to satisfy itoa function
				lcd_GoToXY(0,1);
				lcd_PrintString("Humidity ="); // confirm preferences
				itoa(Y1,data,10); // convert integer to string
				lcd_GoToXY(11,1); // position on LCD first column and second row
				lcd_PrintString(data); // char declaration to satisfy itoa function
				
				_delay_ms(2000);
				lcd_WriteCommand(0x01);
				_delay_ms(500);
		while(1) 
		{ // second while statement
		while(1){
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
				I_Temp = (I_Temp*1.8)+32;
				lcd_init(); // Initialize LCD
				lcd_GoToXY(0,1); // Enter column and row position
				lcd_PrintString("Temp = ");
				lcd_GoToXY(0,2);
				lcd_PrintString("Humid = ");
				//I_Temp = (I_Temp);
				itoa(I_RH,data,10);
				lcd_GoToXY(8,2);
				lcd_PrintString(data);
				lcd_PrintString(".");
				// integer temperature display
				itoa(D_RH,data,10);
				lcd_PrintString(data);
				lcd_PrintString("%");
				// Decimal humidity display
				itoa(I_Temp,data,10);
				lcd_GoToXY(7,1);
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
				
				if (I_RH > Y0 && I_Temp < X0)
				{
					lcd_WriteCommand(0x01);
					_delay_ms(1000);
					lcd_GoToXY(0,2);
					lcd_PrintString("WARNING!");
					_delay_ms(1000);
					lcd_GoToXY(0,2);
					lcd_PrintString("Humid and Temp");
					lcd_GoToXY(0,1);
					lcd_PrintString("is too high!");
				}
				if (I_RH < Y1 && I_Temp < X1)
				{
					lcd_WriteCommand(0x01);
					_delay_ms(1000);
					lcd_GoToXY(0,2);
					lcd_PrintString("WARNING!");
					_delay_ms(1000);
					lcd_GoToXY(0,2);
					lcd_PrintString("Humid and Temp");
					lcd_GoToXY(0,1);
					lcd_PrintString("is too low!");
				}
				else if (I_Temp > X0)
				{
					lcd_WriteCommand(0x01);
					_delay_ms(1000);
					lcd_GoToXY(0,2);
					lcd_PrintString("WARNING!");
					_delay_ms(1000);
					lcd_GoToXY(0,2);
					lcd_PrintString("Temp is");
					lcd_GoToXY(0,1);
					lcd_PrintString("too high!");
				} 
				else if (I_Temp < X1)
				{
					lcd_WriteCommand(0x01);
					_delay_ms(1000);
					lcd_GoToXY(0,1);
					lcd_PrintString("WARNING!");
					lcd_GoToXY(0,2);
					lcd_PrintString("Temp is low!");
				}
				else if (I_RH > Y0)
				{
					lcd_WriteCommand(0x01);
					_delay_ms(1000);
					lcd_GoToXY(0,1);
					lcd_PrintString("WARNING!");
					lcd_GoToXY(0,2);
					lcd_PrintString("Humid is high!");
				}
				else if (I_RH < Y1)
				{
					lcd_WriteCommand(0x01);
					_delay_ms(1000);
					lcd_GoToXY(0,1);
					lcd_PrintString("WARNING!");
					lcd_GoToXY(0,2);
					lcd_PrintString("Humid is low!");
				}
				else
				{
					break;
				}
					while(1)
					{
								if (PINE != (1<<3)) // inactive state the button is reading high
								{
								PINC |= (1<<3); //////////////////////////STUPID!!!!!!!!!!!!!!!!!!!!
								PCMSK1 = (1<<5);
								PCICR = (1<<PCIE1);
								sei();
								}
						DDRC = (1<<3);
						PORTC = (1<<3);     // Turn ON the Buzzer connected to PORTC
						_delay_us(80);      // Wait time delay will effect the frequency of buzz
						PORTC = ~(1<<3);       // Turn OFF the Buzzer connected to PORTC
						_delay_us(80);      // Wait time delay will effect the frequency of buzz
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

}//move to bottom
}//move to bottom

   }
  }
 }
}

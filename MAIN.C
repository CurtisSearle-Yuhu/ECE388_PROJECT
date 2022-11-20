/*
Summary:
This code is used to interface with ECE388 Project Weather Station. This project monitors Humidity, Temperature, and heat index
to preserve the quality of musical instruments. The user will be prompted with a series of preferences to set upper and lower thresholds
for temperature and humidity. Monitoring temperature and humidity is accomplished with a DHT11 module. If the DHT11 reads a value that exceeds 
the threshold the project will send multiple warning notifications this includes visual, hearing, and reading warning notifications via
(blinking LED, Active buzzer, and LCD prompts).
Sensors:
1) DHT11
2) LED
3) Potentiometer
4) Active Buzzer
5) Push Button
(This only includes sensors interfaced with with code)

 DHT11		BUZZER		PUSH BUTTON						POTETIOMETER
 _____													 _____
|_____|		(+___)		{ O }							(_____)
 | | |		 |  |		 | |							 | | |
 G D V		 P  G		 P G							 | | |
 N A C		 C  N		 E N							 G P V	
 D T C		 3  D		 3 D							 N C C
 A				+									   	 D 0 C
				10K OHM (RESISITOR GOES TO SOURCE) 
ATMEGA328PB:
--PORTB-- (NO CODING TO THIS REGISTERS)
--PORTC--
PC0 ----- MID (POTETIOMETER)
PC3 ----- (+) (BUZZER)
PC4 ----- SDA (I2C, LCD)
PC5 ----- SCL (I2C, LCD)
-- PORTD --
PD0 -----  DATA (DHT11, DATA)
GND ------ GND(DHT11, GND)
VCC ------ 5V (DHT11, VCC)
PD1 -----  CATHODE (LED)
--PORTE--
PE3 ----- PUSH BUTTON 
*/

#define F_CPU 16000000 // frequency of the ATMEGA328PB microprocessor used
#include <stdio.h> // standard library
#include <avr/io.h> // standard library
#include <util/delay.h> // standard library
#include <stdio.h> // standard library
#include <stdint.h> // standard library
#include <stdlib.h> // standard library
#include <string.h> // use for iota() function libary for strings
#include <avr/interrupt.h> // used for interrupt services
#include <avr/eeprom.h>	// Reading/Writing to EEPROM AVR EEPROM header file 
#include "ADC.h" // header file to use Analog To Digital Converter
#include "I2C.h" // header file to interface with I2C Protocol
#include "DHT11.h" // header file interface with DHT11 Temperature and Humidity sensor
#define Potentiometer 100.00 //the value of the potentiometer in resistance values higher can crash program


ISR(PCINT0_vect) // PCINT0 = PORTB
{
	PINE != (1<<3);
}
/*------------------------------------------------------- MAIN (HELLO WORLD) ------------------------------------------------------------------------*/
int main() // main function
{
	char string[7];
	DDRC |= (1<<3); // Set the DDRC Pin 3 High to be an output
	PORTC &= ~(1<<3); // set PORTC Pin 3 Low
	DDRD |= (1<<1); // Set the DDRD Pin 1 High to be an output
	PORTD &= ~(1<<1); // set PORTD Pin 1 Low
	_delay_ms(100); // wait 1/10 of a second
	PORTD &= ~(1<<1); // set PORTD Pin 1 High
	lcd_init(); //initialize LCD see header file "I2C.h"
	char data[5];//define a char for itoa function
	double TempSetPref; // will be the value of the Potentiometer
	Init_ADC(); // initialize the Analog to Digital convector (Potentiometer)
	float Humiture; // define Heat Index
	
	while(1) // open 1st while loop
	{
		Hello: // start of program with Hello Message 
		lcd_WriteCommand(0x01); // clear display (LCD)
		_delay_ms(100); // delay a set amount
		lcd_GoToXY(2,2); // position on LCD first column and first row (settings is backwards)
		lcd_PrintString("Welcome To"); // welcome message
		lcd_GoToXY(2,1); // position on LCD first column and first row (settings is backwards)
		lcd_PrintString("ECE388 2022"); // personalize the display
		_delay_ms(2500); // delay a set amount 2.5 Seconds
/*------------------------------------------------------------- SELECT HIGH TEMPETURE  ----------------------------------------------------------*/	
		HIGH_T:	//This is a goto to set High Temperature
		lcd_WriteCommand(0x01); // clear the LCD
		lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
		lcd_PrintString("Please Select"); // instruct the user with a displayed message
		lcd_GoToXY(0,1); // position on LCD first column and first row (settings is backwards)
		lcd_PrintString("High Temperature"); // instruct the user with a displayed message
		_delay_ms(2500); // delay a set amount
		lcd_WriteCommand(0x01); // clear the LCD
		break; // terminate the 1st while loop
	} // close first while loop

	while(1) // open 2nd while loop
	{
		DDRE = ~(1<<3); // set the DDRE low to make it an input this will allow the press button to be read
		int X0; // integer holder to carry High Temperature value
		TempSetPref=(double)Potentiometer/1024*ReadADC_0(0); //reading Potentiometer value in Ohms
		lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
		lcd_PrintString("High TEMP = "); // read the Temperature value on the LCD
		lcd_GoToXY(14,2); // position on LCD first column and first row (settings is backwards)
		lcd_WriteData(0XDF); // ASCii value for degree symbol
		lcd_GoToXY(15,2); // position on LCD first column and first row (settings is backwards)
		lcd_PrintString("F"); // display a F for Fahrenheit
		lcd_GoToXY(0,1); // position on LCD first column and first row (settings is backwards)
		lcd_PrintString("THEN PUSH BUTTON"); // instruct the user with a displayed message
		itoa(TempSetPref,data,10); // itoa() functions converts integer to string
		lcd_GoToXY(12,2); // position on LCD first column and second row
		lcd_PrintString(data); // print value stores in char data
		X0 = TempSetPref; //carry the value of High TEMP (temperature set by user) in X0
		
		if (PINE != (1<<3)) // open 1st if loop, if push button is pressed then true
		{
			_delay_ms(1000); // wait a set time 1 second 
/*---------------------------------------- OPTION TOO RESET ----------------------------------------------------------------*/						
		if (PINE != (1<<3)) // if push button is held down
		{	
			lcd_WriteCommand(0x01); // clear display (LCD)
			lcd_GoToXY(2,2); // position on LCD first column and first row (settings is backwards)
			lcd_PrintString("Let go of Button"); // welcome message
			lcd_GoToXY(2,1); // position on LCD first column and first row (settings is backwards)
			lcd_PrintString("To go Back"); // personalize the display
			_delay_ms(3000);
/*---------------------------------------- DISPLAY EEPROM ----------------------------------------------------------------*/	
			if (PINE != (1<<3) && X0 > 90)
			{		
				lcd_WriteCommand(0x01); // CLEAR LCD for new message
				lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
				lcd_PrintString("Loading Memory"); // instruct the user with a displayed message
				lcd_GoToXY(0,1); // position on LCD first column and first row (settings is backwards)
				lcd_PrintString("Please Wait..."); // instruct the user with a displayed message
				_delay_ms(2500);
				lcd_WriteCommand(0x01);
				_delay_ms(200);
				eeprom_busy_wait();
				eeprom_read_block(string,0,strlen(string));
				lcd_GoToXY(0,2);
				lcd_PrintString("Last Saved Data:");
				while (1)
				{
				TempSetPref=(double)Potentiometer/1024*ReadADC_0(0); //reading Potentiometer value in Ohms
				eeprom_busy_wait();
					if (TempSetPref >=90 && TempSetPref<100)
					{
						eeprom_read_block(string,0,strlen(string));
						lcd_GoToXY(0,2);
						lcd_PrintString("Saved Data1:               ");
						lcd_GoToXY(0,1);
						lcd_PrintString(string);
					}
					if (TempSetPref >=80 && TempSetPref<90)
					{
						eeprom_read_block(string,9,strlen(string));
						lcd_GoToXY(0,2);
						lcd_PrintString("Saved Data2:             ");
						lcd_GoToXY(0,1);
						lcd_PrintString(string);
					}
					if (TempSetPref >=70 && TempSetPref<80)
					{
						eeprom_read_block(string,18,strlen(string));
						lcd_GoToXY(0,2);
						lcd_PrintString("Saved Data3:           ");
						lcd_GoToXY(0,1);
						lcd_PrintString(string);
					}
					if (TempSetPref >=60 && TempSetPref<70)
					{
						eeprom_read_block(string,27,strlen(string));
						lcd_GoToXY(0,2);
						lcd_PrintString("Saved Data4:          ");
						lcd_GoToXY(0,1);
						lcd_PrintString(string);
					}
					if (TempSetPref >=50 && TempSetPref<60)
					{
						eeprom_read_block(string,36,strlen(string));
						lcd_GoToXY(0,2);
						lcd_PrintString("Saved Data5:          ");
						lcd_GoToXY(0,1);
						lcd_PrintString(string);
					}
					if (TempSetPref >=90 && TempSetPref<100)
					{
						eeprom_read_block(string,0,strlen(string));
						lcd_GoToXY(0,2);
						lcd_PrintString("Saved Data1:               ");
						lcd_GoToXY(0,1);
						lcd_PrintString(string);
					}
					if (TempSetPref >=80 && TempSetPref<90)
					{
						eeprom_read_block(string,9,strlen(string));
						lcd_GoToXY(0,2);
						lcd_PrintString("Saved Data2:             ");
						lcd_GoToXY(0,1);
						lcd_PrintString(string);
					}
					if (TempSetPref >=70 && TempSetPref<80)
					{
						eeprom_read_block(string,18,strlen(string));
						lcd_GoToXY(0,2);
						lcd_PrintString("Saved Data3:           ");
						lcd_GoToXY(0,1);
						lcd_PrintString(string);
					}
					if (TempSetPref >=60 && TempSetPref<70)
					{
						eeprom_read_block(string,27,strlen(string));
						lcd_GoToXY(0,2);
						lcd_PrintString("Saved Data4:          ");
						lcd_GoToXY(0,1);
						lcd_PrintString(string);
					}
					if (TempSetPref >=50 && TempSetPref<60)
					{
						eeprom_read_block(string,36,strlen(string));
						lcd_GoToXY(0,2);
						lcd_PrintString("Saved Data5:          ");
						lcd_GoToXY(0,1);
						lcd_PrintString(string);
					}
					if (PINE != (1<<3))
					{
						goto Hello;
					}
				}
				_delay_ms(3000); // wait 3 seconds
				goto HIGH_T;
			}	
			if (PINE != (1<<3)) // inactive state the button is reading high
			{
				lcd_WriteCommand(0x01); // CLEAR LCD for new message
				lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
				lcd_PrintString("RESETTING"); // instruct the user with a displayed message
				lcd_GoToXY(0,1); // position on LCD first column and first row (settings is backwards)
				lcd_PrintString("PROGRAM..."); // instruct the user with a displayed message
				_delay_ms(3000); // wait 3 seconds 
				lcd_WriteCommand(0x01); // CLEAR LCD for new message
				lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
				lcd_PrintString("Please Don't"); // instruct the user with a displayed message
				lcd_GoToXY(0,1); // position on LCD first column and first row (settings is backwards)
				lcd_PrintString("Hold the Button"); // instruct the user with a displayed message
				_delay_ms(3000);// wait 3 seconds 
				lcd_WriteCommand(0x01); // CLEAR LCD for new message
				lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
				lcd_PrintString("Please wait"); // instruct the user with a displayed message
				_delay_ms(2500); // wait 2.5 seconds 
				lcd_WriteCommand(0x01); // CLEAR LCD for new message
				lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
				lcd_PrintString("Reset Complete"); // instruct the user with a displayed message
				lcd_GoToXY(0,1); // position on LCD first column and first row (settings is backwards)
				lcd_PrintString("Thank You !"); // instruct the user with a displayed message
				_delay_ms(2500); // wait 2.5 seconds 
				lcd_WriteCommand(0x01); // clear display and reset program
				PCMSK1 = (1<<5); //
				PCICR = (1<<PCIE1); //
				sei(); // set global interrupt high (i)
			}
		goto Hello; // return to the welcome message 
		}
/*----------------------------------------------------------- SELECT LOW TEMPETURE  --------------------------------------------------------------------*/
	while(1) // open 3rd while loop-
	{
		LOW_T: // Set low Temperature preferences 
		lcd_WriteCommand(0x01); // CLEAR LCD for new message
		lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
		lcd_PrintString("Please Select"); // instruct the user with a displayed message
		lcd_GoToXY(0,1); // position on LCD first column and first row (settings is backwards)
		lcd_PrintString("Low Temperature"); // instruct the user with a displayed message
		_delay_ms(2500); // delay a set time
		lcd_WriteCommand(0x01); // CLEAR LCD for new message
		break; // terminate the 3rd while loop
	} // close 3rd while loop
	while(1) // open 4th while loop
	{
		DDRE = ~(1<<3); // set the DDRE at Pin 3 low to be a, input, so the Push button can be read
		int X1; // integer holder to hold Low Temperature value
		TempSetPref=(double)Potentiometer/1024*ReadADC_0(0); //reading Y axis value in Ohms
		_delay_ms(250); //wait
		lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
		lcd_PrintString("Low TEMP = "); // read the Temperature value on the LCD
		lcd_GoToXY(13,2); // position on LCD first column and first row (settings is backwards)
		lcd_WriteData(0XDF); // Ascii value for degree symbol
		lcd_GoToXY(14,2); // position on LCD first column and first row (settings is backwards)
		lcd_PrintString("F"); // display a F for Fahrenheit
		lcd_GoToXY(0,1); // position on LCD first column and first row (settings is backwards)
		lcd_PrintString("THEN PUSH BUTTON"); // instruct the user with a displayed message
		itoa(TempSetPref,data,10); // itoa() functions converts integer to string
		lcd_GoToXY(11,2); // position on LCD first column and second row
		lcd_PrintString(data); // print value stores in char data
		X1 = TempSetPref; //carry the value of Low TEMP (temperature set by user) in X
				
		if (PINE != (1<<3)) // open 1st if loop, if push button is pressed then true
		{
		_delay_ms(1000); // wait a set time 1 second
/*---------------------------------------- OPTION TOO RESET ----------------------------------------------------------------*/
					if (PINE != (1<<3)) // if push button is pressed
					{
						lcd_WriteCommand(0x01); // clear display (LCD)
						lcd_GoToXY(2,2); // position on LCD first column and first row (settings is backwards)
						lcd_PrintString("Let go of Button"); // welcome message
						lcd_GoToXY(2,1); // position on LCD first column and first row (settings is backwards)
						lcd_PrintString("To go Back"); // personalize the display
						_delay_ms(4000); // wait a set time 4 seconds
/*---------------------------------------- DISPLAY EEPROM ----------------------------------------------------------------*/	
					if (PINE != (1<<3) && X0 > 90)
					{
						lcd_WriteCommand(0x01); // CLEAR LCD for new message
						lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
						lcd_PrintString("Loading Memory"); // instruct the user with a displayed message
						lcd_GoToXY(0,1); // position on LCD first column and first row (settings is backwards)
						lcd_PrintString("Please Wait..."); // instruct the user with a displayed message
						_delay_ms(2500);
						lcd_WriteCommand(0x01);
						_delay_ms(200);
						eeprom_busy_wait();
						eeprom_read_block(string,0,strlen(string));
						lcd_GoToXY(0,2);
						lcd_PrintString("Last Saved Data:");
while (1)
{
	TempSetPref=(double)Potentiometer/1024*ReadADC_0(0); //reading Potentiometer value in Ohms
	eeprom_busy_wait();
	if (TempSetPref >=90 && TempSetPref<100)
	{
		eeprom_read_block(string,0,strlen(string));
		lcd_GoToXY(0,2);
		lcd_PrintString("Saved Data1:               ");
		lcd_GoToXY(0,1);
		lcd_PrintString(string);
	}
	if (TempSetPref >=80 && TempSetPref<90)
	{
		eeprom_read_block(string,9,strlen(string));
		lcd_GoToXY(0,2);
		lcd_PrintString("Saved Data2:             ");
		lcd_GoToXY(0,1);
		lcd_PrintString(string);
	}
	if (TempSetPref >=70 && TempSetPref<80)
	{
		eeprom_read_block(string,18,strlen(string));
		lcd_GoToXY(0,2);
		lcd_PrintString("Saved Data3:           ");
		lcd_GoToXY(0,1);
		lcd_PrintString(string);
	}
	if (TempSetPref >=60 && TempSetPref<70)
	{
		eeprom_read_block(string,27,strlen(string));
		lcd_GoToXY(0,2);
		lcd_PrintString("Saved Data4:          ");
		lcd_GoToXY(0,1);
		lcd_PrintString(string);
	}
	if (TempSetPref >=50 && TempSetPref<60)
	{
		eeprom_read_block(string,36,strlen(string));
		lcd_GoToXY(0,2);
		lcd_PrintString("Saved Data5:          ");
		lcd_GoToXY(0,1);
		lcd_PrintString(string);
	}
	if (TempSetPref >=90 && TempSetPref<100)
	{
		eeprom_read_block(string,0,strlen(string));
		lcd_GoToXY(0,2);
		lcd_PrintString("Saved Data1:               ");
		lcd_GoToXY(0,1);
		lcd_PrintString(string);
	}
	if (TempSetPref >=80 && TempSetPref<90)
	{
		eeprom_read_block(string,9,strlen(string));
		lcd_GoToXY(0,2);
		lcd_PrintString("Saved Data2:             ");
		lcd_GoToXY(0,1);
		lcd_PrintString(string);
	}
	if (TempSetPref >=70 && TempSetPref<80)
	{
		eeprom_read_block(string,18,strlen(string));
		lcd_GoToXY(0,2);
		lcd_PrintString("Saved Data3:           ");
		lcd_GoToXY(0,1);
		lcd_PrintString(string);
	}
	if (TempSetPref >=60 && TempSetPref<70)
	{
		eeprom_read_block(string,27,strlen(string));
		lcd_GoToXY(0,2);
		lcd_PrintString("Saved Data4:          ");
		lcd_GoToXY(0,1);
		lcd_PrintString(string);
	}
	if (TempSetPref >=50 && TempSetPref<60)
	{
		eeprom_read_block(string,36,strlen(string));
		lcd_GoToXY(0,2);
		lcd_PrintString("Saved Data5:          ");
		lcd_GoToXY(0,1);
		lcd_PrintString(string);
	}
	if (PINE != (1<<3))
	{
		goto Hello;
	}
}
						_delay_ms(3000); // wait 3 seconds
						goto LOW_T;
					}

					if (PINE != (1<<3)) // inactive state the button is reading high
					{
						_delay_ms(2000); // wait a set time 2 seconds
						lcd_WriteCommand(0x01); // CLEAR LCD for new message
						lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
						lcd_PrintString("RESETTING"); // instruct the user with a displayed message
						lcd_GoToXY(0,1); // position on LCD first column and first row (settings is backwards)
						lcd_PrintString("PROGRAM..."); // instruct the user with a displayed message
						_delay_ms(3000); // wait a set time 3 seconds
						lcd_WriteCommand(0x01); // CLEAR LCD for new message
						lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
						lcd_PrintString("Please Don't"); // instruct the user with a displayed message
						lcd_GoToXY(0,1); // position on LCD first column and first row (settings is backwards)
						lcd_PrintString("Hold the Button"); // instruct the user with a displayed message
						_delay_ms(3000); // wait a set time 3 seconds
						lcd_WriteCommand(0x01); // CLEAR LCD for new message
						lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
						lcd_PrintString("Please wait"); // instruct the user with a displayed message
						_delay_ms(2500); // wait a set time 2.5 seconds
						lcd_WriteCommand(0x01); // CLEAR LCD for new message
						lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
						lcd_PrintString("Reset Complete"); // instruct the user with a displayed message
						lcd_GoToXY(0,1); // position on LCD first column and first row (settings is backwards)
						lcd_PrintString("Thank You !"); // instruct the user with a displayed message
						_delay_ms(2500); // wait a set time 2.5 seconds
						lcd_WriteCommand(0x01); // clear display
						PCMSK1 = (1<<5); //
						PCICR = (1<<PCIE1); //
						sei(); // set global interrupt high (i)
					}
					goto HIGH_T; // Allow user to go back to previous settings of High Temperature 
					}
/*----------------------------------------------------------------------TEMPETURE PREFERENCE CHECK------------------------------------------------------------*/
if (X0 < X1 || X1 > X0 )
{
	while(X0 < X1)
	{
		lcd_WriteCommand(0x01); // CLEAR LCD for new message
		lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
		lcd_PrintString("Please Enter"); // instruct the user with a displayed message
		lcd_GoToXY(0,1); // position on LCD first column and first row (settings is backwards)
		lcd_PrintString("Valid Entires!"); // instruct the user with a displayed message
		_delay_ms(3500);
		goto HIGH_T;
	}
	while(X1 > X0)
	{
		lcd_WriteCommand(0x01); // CLEAR LCD for new message
		lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
		lcd_PrintString("Please Enter"); // instruct the user with a displayed message
		lcd_GoToXY(0,1); // position on LCD first column and first row (settings is backwards)
		lcd_PrintString("Valid Entires!"); // instruct the user with a displayed message
		_delay_ms(3500);
		goto HIGH_T;
	}
}
/*---------------------------------------------------------- SELECT HIGH HUMIDITY  ----------------------------------------------------*/
	while(1) // open 5th while loop
	{
		HIGH_H: // Preferences for HIgh Humidity
		lcd_WriteCommand(0x01); // clear display
		lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
		lcd_PrintString("Please Select"); // User Display Message 
		lcd_GoToXY(0,1); // position on LCD first column and first row (settings is backwards)
		lcd_PrintString("High Humidity"); // User Display Message for High Humidity
		_delay_ms(2500); // wait a set time 2.5 seconds
		lcd_WriteCommand(0x01); // clear LCD display
		break; // terminate while 5th loop
	} // close 5th while loop
	while(1) // open 6th while loop
	{
		DDRE = ~(1<<3); // Set DDRE low to make Pin 3 an Input
		int Y0; // integer holder for High Humidity 
		TempSetPref=(double)Potentiometer/1024*ReadADC_0(0); //reading Y axis value in Ohms
		_delay_ms(250); // delay a set time
		lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
		lcd_PrintString("High HUMID = "); // read the Humidity value on the LCD
		lcd_GoToXY(15,2); // position on LCD first column and first row (settings is backwards)
		lcd_PrintString("%"); // Display a percentage symbol for Humidity value 
		lcd_GoToXY(0,1); // position on LCD first column and first row (settings is backwards)
		lcd_PrintString("THEN PUSH BUTTON"); // display message to instruct USER
		itoa(TempSetPref,data,10); // itoa() functions converts integer to string
		lcd_GoToXY(13,2); // position on LCD first column and second row
		lcd_PrintString(data); // print value stores in char data
		Y0 = TempSetPref; //carry the value of TEMP (temperature set by user) in X

		if (PINE != (1<<3)) // open 1st if loop, if push button is pressed then true
		{
		_delay_ms(1000); // delay set time 1 second
/*---------------------------------------- OPTION TOO RESET ----------------------------------------------------------------*/
					if (PINE != (1<<3)) // Enter if statement when Push Button is long pressed 
					{
						lcd_WriteCommand(0x01); // clear display (LCD)
						lcd_GoToXY(2,2); // position on LCD first column and first row (settings is backwards)
						lcd_PrintString("Let go of Button"); // Display instruction for User
						lcd_GoToXY(2,1); // position on LCD first column and first row (settings is backwards)
						lcd_PrintString("To go Back"); // display Instruction for User
						_delay_ms(4000); // wait set time 4 seconds
/*---------------------------------------- DISPLAY EEPROM ----------------------------------------------------------------*/	
						if (PINE != (1<<3) && X0 > 90)
						{
							lcd_WriteCommand(0x01); // CLEAR LCD for new message
							lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
							lcd_PrintString("Loading Memory"); // instruct the user with a displayed message
							lcd_GoToXY(0,1); // position on LCD first column and first row (settings is backwards)
							lcd_PrintString("Please Wait..."); // instruct the user with a displayed message
							_delay_ms(2500);
							lcd_WriteCommand(0x01);
							_delay_ms(200);
							eeprom_busy_wait();
							eeprom_read_block(string,0,strlen(string));
							lcd_GoToXY(0,2);
							lcd_PrintString("Last Saved Data:");
							while (1)
							{
								TempSetPref=(double)Potentiometer/1024*ReadADC_0(0); //reading Potentiometer value in Ohms
								eeprom_busy_wait();
								if (TempSetPref >=90 && TempSetPref<100)
								{
									eeprom_read_block(string,0,strlen(string));
									lcd_GoToXY(0,2);
									lcd_PrintString("Saved Data1:               ");
									lcd_GoToXY(0,1);
									lcd_PrintString(string);
								}
								if (TempSetPref >=80 && TempSetPref<90)
								{
									eeprom_read_block(string,9,strlen(string));
									lcd_GoToXY(0,2);
									lcd_PrintString("Saved Data2:             ");
									lcd_GoToXY(0,1);
									lcd_PrintString(string);
								}
								if (TempSetPref >=70 && TempSetPref<80)
								{
									eeprom_read_block(string,18,strlen(string));
									lcd_GoToXY(0,2);
									lcd_PrintString("Saved Data3:           ");
									lcd_GoToXY(0,1);
									lcd_PrintString(string);
								}
								if (TempSetPref >=60 && TempSetPref<70)
								{
									eeprom_read_block(string,27,strlen(string));
									lcd_GoToXY(0,2);
									lcd_PrintString("Saved Data4:          ");
									lcd_GoToXY(0,1);
									lcd_PrintString(string);
								}
								if (TempSetPref >=50 && TempSetPref<60)
								{
									eeprom_read_block(string,36,strlen(string));
									lcd_GoToXY(0,2);
									lcd_PrintString("Saved Data5:          ");
									lcd_GoToXY(0,1);
									lcd_PrintString(string);
								}
								if (TempSetPref >=90 && TempSetPref<100)
								{
									eeprom_read_block(string,0,strlen(string));
									lcd_GoToXY(0,2);
									lcd_PrintString("Saved Data1:               ");
									lcd_GoToXY(0,1);
									lcd_PrintString(string);
								}
								if (TempSetPref >=80 && TempSetPref<90)
								{
									eeprom_read_block(string,9,strlen(string));
									lcd_GoToXY(0,2);
									lcd_PrintString("Saved Data2:             ");
									lcd_GoToXY(0,1);
									lcd_PrintString(string);
								}
								if (TempSetPref >=70 && TempSetPref<80)
								{
									eeprom_read_block(string,18,strlen(string));
									lcd_GoToXY(0,2);
									lcd_PrintString("Saved Data3:           ");
									lcd_GoToXY(0,1);
									lcd_PrintString(string);
								}
								if (TempSetPref >=60 && TempSetPref<70)
								{
									eeprom_read_block(string,27,strlen(string));
									lcd_GoToXY(0,2);
									lcd_PrintString("Saved Data4:          ");
									lcd_GoToXY(0,1);
									lcd_PrintString(string);
								}
								if (TempSetPref >=50 && TempSetPref<60)
								{
									eeprom_read_block(string,36,strlen(string));
									lcd_GoToXY(0,2);
									lcd_PrintString("Saved Data5:          ");
									lcd_GoToXY(0,1);
									lcd_PrintString(string);
								}
								if (PINE != (1<<3))
								{
									goto HIGH_H;
								}
							}
							_delay_ms(3000); // wait 3 seconds
							goto HIGH_H;
						}

						if (PINE != (1<<3)) // inactive state the button is reading high
						{
							_delay_ms(2000); // wait set period of time 2 seconds
							lcd_WriteCommand(0x01); // CLEAR LCD for new message
							lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
							lcd_PrintString("RESETTING"); // instruct the user with a displayed message
							lcd_GoToXY(0,1); // position on LCD first column and first row (settings is backwards)
							lcd_PrintString("PROGRAM..."); // instruct the user with a displayed message
							_delay_ms(3000); // wait set period of time 3 seconds
							lcd_WriteCommand(0x01); // CLEAR LCD for new message
							lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
							lcd_PrintString("Please Don't"); // instruct the user with a displayed message
							lcd_GoToXY(0,1); // position on LCD first column and first row (settings is backwards)
							lcd_PrintString("Hold the Button"); // instruct the user with a displayed message
							_delay_ms(3000); // wait set period of time 3 seconds
							lcd_WriteCommand(0x01); // CLEAR LCD for new message
							lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
							lcd_PrintString("Please wait"); // instruct the user with a displayed message
							_delay_ms(2500); // wait set period of time 2.5 seconds
							lcd_WriteCommand(0x01); // CLEAR LCD for new message
							lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
							lcd_PrintString("Reset Complete"); // instruct the user with a displayed message
							lcd_GoToXY(0,1); // position on LCD first column and first row (settings is backwards)
							lcd_PrintString("Thank You !"); // instruct the user with a displayed message
							_delay_ms(2500); // wait set period of time 2.5 seconds
							lcd_WriteCommand(0x01); // clear display and reset program
							PCMSK1 = (1<<5); //
							PCICR = (1<<PCIE1); //
							sei(); // set global interrupt high (i)
						}
						goto LOW_T; // if button is held allow user to go back to low temperature preferences 
					}
/*----------------------------------------------------- SELECT LOW HUMIDITY  --------------------------------------------------*/
	while(1) // open 7th while loop
	{ 
	LOW_H: // Low humidity prefectures 
		lcd_WriteCommand(0x01); // clear display
		lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
		lcd_PrintString("Please Select"); // display instructions
		lcd_GoToXY(0,1); // position on LCD first column and first row (settings is backwards)
		lcd_PrintString("Low Humidity"); // display low humidity settings
		_delay_ms(2500); // wait set time 2.5 seconds
		lcd_WriteCommand(0x01); // clear display
		break; // terminate 7th while loop
	} // close 7th while loop
	while(1) // open 8th while loop
	{
		DDRE = ~(1<<3); // Set DDRE low to make Pin 3 an Input
		int Y1; // integer value that holds the low humidity value
		TempSetPref=(double)Potentiometer/1024*ReadADC_0(0); //store Low Humidity value in Ohms
		lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
		lcd_PrintString("Low HUMID = "); // read the Low Humidity value on the LCD
		lcd_GoToXY(14,2); // position on LCD first column and first row (settings is backwards)
		lcd_PrintString("%"); // display percentage symbol for low humidity value 
		lcd_GoToXY(0,1); // position on LCD first column and first row (settings is backwards)
		lcd_PrintString("THEN PUSH BUTTON"); // display instruction for user
		itoa(TempSetPref,data,10); // itoa() functions converts integer to string
		lcd_GoToXY(12,2); // position on LCD first column and second row
		lcd_PrintString(data); // print value stores in char data
		Y1 = TempSetPref; //carry the value of TEMP (temperature set by user) in X			
					
		if (PINE != (1<<3)) // open 1st if loop, if push button is pressed then true
		{
		_delay_ms(1000); // wait set time 1 second
/*---------------------------------------- OPTION TOO RESET ----------------------------------------------------------------*/
					if (PINE != (1<<3))// if push button is held for set time 1 second 
					{
							lcd_WriteCommand(0x01); // clear display (LCD)
							lcd_GoToXY(2,2); // position on LCD first column and first row (settings is backwards)
							lcd_PrintString("Let go of Button"); // welcome message
							lcd_GoToXY(2,1); // position on LCD first column and first row (settings is backwards)
							lcd_PrintString("To go Back"); // personalize the display
							_delay_ms(4000); // wait set time 4 seconds
/*---------------------------------------- DISPLAY EEPROM ----------------------------------------------------------------*/	
						if (PINE != (1<<3) && X0 > 90)
						{
							lcd_WriteCommand(0x01); // CLEAR LCD for new message
							lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
							lcd_PrintString("Loading Memory"); // instruct the user with a displayed message
							lcd_GoToXY(0,1); // position on LCD first column and first row (settings is backwards)
							lcd_PrintString("Please Wait..."); // instruct the user with a displayed message
							_delay_ms(2500);
							lcd_WriteCommand(0x01);
							_delay_ms(200);
							eeprom_busy_wait();
							eeprom_read_block(string,0,strlen(string));
							lcd_GoToXY(0,2);
							lcd_PrintString("Last Saved Data:");
							lcd_GoToXY(0,1);
							lcd_PrintString(string);
							_delay_ms(3000); // wait 3 seconds
							goto LOW_H;
						}

						if (PINE != (1<<3)) // inactive state the button is reading high
						{
							_delay_ms(2000); // wait set time 2 seconds
							lcd_WriteCommand(0x01); // CLEAR LCD for new message
							lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
							lcd_PrintString("RESETTING"); // instruct the user with a displayed message
							lcd_GoToXY(0,1); // position on LCD first column and first row (settings is backwards)
							lcd_PrintString("PROGRAM..."); // instruct the user with a displayed message
							_delay_ms(3000); // wait set time 3 seconds 
							lcd_WriteCommand(0x01); // CLEAR LCD for new message
							lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
							lcd_PrintString("Please Don't"); // instruct the user with a displayed message
							lcd_GoToXY(0,1); // position on LCD first column and first row (settings is backwards)
							lcd_PrintString("Hold the Button"); // instruct the user with a displayed message
							_delay_ms(3000); // wait set time 3 seconds
							lcd_WriteCommand(0x01); // CLEAR LCD for new message
							lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
							lcd_PrintString("Please wait"); // instruct the user with a displayed message
							_delay_ms(2500); // wait set time 2.5 seconds
							lcd_WriteCommand(0x01); // CLEAR LCD for new message
							lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
							lcd_PrintString("Reset Complete"); // instruct the user with a displayed message
							lcd_GoToXY(0,1); // position on LCD first column and first row (settings is backwards)
							lcd_PrintString("Thank You !"); // instruct the user with a displayed message
							_delay_ms(2500); // wait set time 2.5 seconds
							lcd_WriteCommand(0x01); // clear display and reset program
							PCMSK1 = (1<<5); //
							PCICR = (1<<PCIE1); //
							sei(); // set global interrupt high (i)
						}
						goto HIGH_H; // allow user to go back to change previous settings for High Humidity 
					}
					
/*----------------------------------------------------------------------HUMIDITY PREFERENCE CHECK------------------------------------------------------------*/
if (Y0 < Y1 || Y1 > Y0 )
{
	while(Y0 < Y1)
	{
		lcd_WriteCommand(0x01); // CLEAR LCD for new message
		lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
		lcd_PrintString("Please Enter"); // instruct the user with a displayed message
		lcd_GoToXY(0,1); // position on LCD first column and first row (settings is backwards)
		lcd_PrintString("Valid Entires!"); // instruct the user with a displayed message
		_delay_ms(3500);
		goto HIGH_H;
	}
	while(Y1 > Y0)
	{
		lcd_WriteCommand(0x01); // CLEAR LCD for new message
		lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
		lcd_PrintString("Please Enter"); // instruct the user with a displayed message
		lcd_GoToXY(0,1); // position on LCD first column and first row (settings is backwards)
		lcd_PrintString("Valid Entires!"); // instruct the user with a displayed message
		_delay_ms(3500);
		goto HIGH_H;
	}
}						
/*----------------------------------------------------------------------- DISPLAY PREFERENCES  --------------------------------------------------------------------------*/			
	_delay_ms(550); // wait half a second
	lcd_WriteCommand(0x01); // clear LCD								
	lcd_GoToXY(0,2); //position 0 column and row 1
	lcd_PrintString("Upper Limits"); // confirm preferences
	lcd_GoToXY(0,1); //position 0 column and row 2
	lcd_PrintString("are set too:"); // confirm preferences
	_delay_ms(2500); // wait 2.5 seconds 						
	lcd_WriteCommand(0x01); // clear LCD
	lcd_GoToXY(0,2); //position 0 column and row 1
	lcd_PrintString("Temp ="); // confirm preferences
	itoa(X0,data,10); // convert integer to string
	lcd_GoToXY(7,2); // position on LCD first column and second row
	lcd_PrintString(data); // display value store in data
	lcd_GoToXY(9,2); // position on LCD first column and first row (settings is backwards)
	lcd_WriteData(0XDF); //ASCii value for degree symbol
	lcd_GoToXY(10,2); // position Column 10, Row 1 of LCD
	lcd_PrintString("F"); // display F for Fahrenheit
	lcd_GoToXY(0,1); // position Column 0, Row 2 of LCD
	lcd_PrintString("Humid ="); // confirm preferences
	itoa(Y0,data,10); // convert integer to string
	lcd_GoToXY(8,1); // position on LCD first column and second row
	lcd_PrintString(data); // char declaration to satisfy itoa function
	lcd_GoToXY(10,1); // position Column 10, Row 2 of LCD
	lcd_PrintString("%"); // confirm preferences
	_delay_ms(3000); // wait 3 seconds									
	lcd_WriteCommand(0x01); // clear LCD
	lcd_GoToXY(0,2); // position Column 0, Row 1 of LCD
	lcd_PrintString("Lower Limits"); // confirm preferences
	lcd_GoToXY(0,1); // position Column 0, Row 1 of LCD
	lcd_PrintString("are set too:"); // confirm preferences
	_delay_ms(2500); // wait 2.5 seconds
	lcd_WriteCommand(0x01); // clear LCD
	lcd_GoToXY(0,2); // position Column 0, Row 1 of LCD
	lcd_PrintString("Temp ="); // confirm preferences
	itoa(X1,data,10); // convert integer to string
	lcd_GoToXY(7,2); // position on LCD first column and second row
	lcd_PrintString(data); // char declaration to satisfy itoa function
	lcd_GoToXY(9,2); // position on LCD first column and first row (settings is backwards)
	lcd_WriteData(0XDF); // ASCII value for degree symbol
	lcd_GoToXY(10,2); // position Column 10, Row 1 of LCD
	lcd_PrintString("F"); // display F for Fahrenheit 
	lcd_GoToXY(0,1); // position Column 0, Row 2 of LCD
	lcd_PrintString("Humid ="); // confirm preferences
	itoa(Y1,data,10); // convert integer to string
	lcd_GoToXY(8,1); // position on LCD first column and second row
	lcd_PrintString(data); // char declaration to satisfy itoa function
	lcd_GoToXY(10,1); // position Column 0, Row 2 of LCD
	lcd_PrintString("%"); // confirm preferences
	_delay_ms(3000); // wait 3 seconds			
	_delay_ms(2000); // wait 2 seconds
	lcd_WriteCommand(0x01); // clear LCD display
	_delay_ms(500); // wait half a second (.5 sec)
/*--------------------------------------------------------- DHT11 INITIALIZE ----------------------------------------------------*/
	while(1) // open 9th while loop
	{
		while(1) // open 10th while loop
		{
			Request(); // too START DHT11 send start pulse in bits
			Response(); // receive response in form of bits
			I_RH=Receive_data(); // store first eight bit in I_RH
			D_RH=Receive_data(); // store next eight bit in D_RH
			I_Temp=Receive_data(); // store next eight bit in I_Temp
			D_Temp=Receive_data(); // store next eight bit in D_Temp
			CheckSum=Receive_data();// store next eight bit in CheckSum
			if ((I_RH + D_RH + I_Temp + D_Temp) != CheckSum) // as instructed in data sheet last 8 bits will be value for checksum
			{
				lcd_GoToXY(0,2); // positioning column 0, row 1
				lcd_PrintString("Error"); // display error message in reading DHT11 signal
			}
			else
			{
				char data[5];// use to satisfie iota() function
				I_Temp = (I_Temp*1.8)+32;  // convert Temperature value from celcius to Fahrenheit  //////////////////////////////////////////////////
				Humiture = (I_Temp + I_RH)/2; // Heat index value
				lcd_init(); // Initialize LCD
				lcd_GoToXY(0,2); // position column 0, row 1
				lcd_PrintString("Humid = "); // display Humidity value
				itoa(I_RH,data,10); // convert Humidity value integer into String
				lcd_GoToXY(8,2); // position column 0, row 1
				lcd_PrintString(data); // display value converted from I_RH integer to data char to string
				lcd_PrintString("."); // Display . for decimal
				itoa(D_RH,data,10); // convert value from D_RH integer to data char to string
				lcd_PrintString(data); // display value in data
				lcd_PrintString("%"); // display percentage for humidity
				lcd_GoToXY(0,1); // Enter column and row position (column,row)
				lcd_PrintString("Temp = "); // display Temperature 
				itoa(I_Temp,data,10); // convert value  from I_Temp (Integer) integer to data char to string
				lcd_GoToXY(7,1); // position column 0, row 1
				lcd_PrintString(data); // display I_Temp (temperature) that got converted to char type Data
				lcd_PrintString("."); // display . for decimal format
				itoa(D_Temp,data,10); // convert value from D_Temp (decimal) integer to data char to string
				lcd_PrintString(data); // display value stored in data char from D_temp (decimal temperature)integer 
				lcd_WriteData(0xDF); // display degree symbol from ASCii value
				lcd_PrintString("F"); // Display F for Fahrenheit
				_delay_ms(2000); // wait 2.5 seconds
			
				lcd_GoToXY(0,2); // position column 0, row 1
				lcd_PrintString("Humiture = "); // display value for heat index
				itoa(Humiture,data,10); // convert integer value Humiture to char data base 10
				lcd_PrintString(data); // display converted Humiture integer stored as char data
			
				lcd_GoToXY(0,1); // Enter column and row position (column,row)
				lcd_PrintString("Temp = "); // display value stored as Temperature 
				itoa(I_Temp,data,10); // convert value  from I_Temp (Integer) integer to data char to string
				lcd_GoToXY(7,1); // position column 7, row 2
				lcd_PrintString(data); // display I_Temp (temperature) that got converted to char type Data
				lcd_PrintString("."); // display . for decimal format
				
				itoa(D_Temp,data,10); // Integer Temperature display
				lcd_PrintString(data); // display value stored as Temperature decimal
				lcd_WriteData(0xDF); // display degree symbol from ASCii value
				lcd_PrintString("F"); // Display F for Fahrenheit
				_delay_ms(2000); // wait set time 2 seconds
/*------------------------ OPTION TO RESET -------------------------------------------------*/										
				if (PINE != (1<<3)) // if push button is pressed
				{
					_delay_ms(2000); // if push button is held for over 2 seconds
				if (PINE != (1<<3)) // if push button is held down
				{
					goto Hello; // return to start of program
				}
					break; // terminate if loop, button was pushed but USER is not trying to reset program 
				}
		}
/*---------------------------------- WARNING DISPLAY -------------------------------------------------------------------------------------------*/

	/*we have 2 objects Temperature and Humidity both with three options; that is equal too (=), less than (<), or greater then (>). 
	Therefore, we can use permutation so that 3*3 = 9 there are 9 possible outcomes for any combination of humidity and Tempeture. */
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////	
		if ( I_Temp > X0 && I_RH > Y0) // if Temperature and Humidity exceeds the high boundary
		{
/*---------------------------------------- WRITE TO EEPROM ----------------------------------------------------------------*/	
			int P = 1;
			char string[11], str[] = "d";
			sprintf(string,"T=%d H=%d.",I_Temp,I_RH);
			sprintf(str,"d",P);
			lcd_WriteCommand(0x01); // clear display
			_delay_ms(1000); // wait 1 second
			lcd_GoToXY(0,2); // position 0 column, row 1
			lcd_PrintString("WARNING!"); // display Warning message on LCD
			_delay_ms(1000); // wait 1 second
			lcd_GoToXY(0,2); // position column 0, row 1
			lcd_PrintString("Humid and Temp"); // display message for what threshold was broken
			lcd_GoToXY(0,1); // position column 0, row 2
			lcd_PrintString("is too High!"); // display message for what threshold was broken
			eeprom_busy_wait();
			for(int j = 0; j < 255; j ++)
			{
				eeprom_read_block(str,j,strlen(str));
				if (strcmp(str,".") ==0)
				{
					eeprom_update_block(string,j,strlen(string));
					goto Warning;
				}
			}
		}	
		else if ( I_Temp > X0 && I_RH < Y1) // if Temperature is ok and Humidity exceeds the low boundary
		{
/*---------------------------------------- WRITE TO EEPROM ----------------------------------------------------------------*/
			int P = 1;
			char string[11], str[] = "d";
			sprintf(string,"T=%d H=%d.",I_Temp,I_RH);
			sprintf(str,"d",P);
			lcd_WriteCommand(0x01); // clear LCD
			_delay_ms(1000); // wait 1 second
			lcd_GoToXY(0,2); // position 0 column, row 1
			lcd_PrintString("WARNING!"); // display Warning message on LCD
			_delay_ms(1000); // wait 1 second
			lcd_WriteCommand(0x01); // clear LCD
			lcd_GoToXY(0,2); // position column 0, row 1
			lcd_PrintString("Temp is High !"); // display message for what threshold was broken
			lcd_GoToXY(0,1); // position column 0, row 2
			lcd_PrintString("Humid is Low!"); // display message for what threshold was broken
			eeprom_busy_wait();
			for(int j = 0; j < 255; j ++)
			{
				eeprom_read_block(str,j,strlen(str));
				if (strcmp(str,".") ==0)
				{
					eeprom_update_block(string,j,strlen(string));
					goto Warning;
				}
			}
		}
		else if ( I_Temp < X1 && I_RH > Y0) // if Temperature is low and Humidity exceeds the high boundary
		{
/*---------------------------------------- WRITE TO EEPROM ----------------------------------------------------------------*/
			int P = 1;
			char string[11], str[] = "d";
			sprintf(string,"T=%d H=%d.",I_Temp,I_RH);
			sprintf(str,"d",P);
			lcd_WriteCommand(0x01); // clear LCD
			_delay_ms(1000); // position column 0, row 2
			lcd_GoToXY(0,2); // position column 0, row 1
			lcd_PrintString("WARNING!"); // display Warning message on LCD
			_delay_ms(1000); // position column 0, row 2
			lcd_WriteCommand(0x01); // clear LCD
			lcd_GoToXY(0,2); // position column 0, row 1
			lcd_PrintString("Temp is Low !"); // display message for what threshold was broken
			lcd_GoToXY(0,1); // position column 0, row 2
			lcd_PrintString("Humid is High!"); // display message for what threshold was broken	
			eeprom_busy_wait();
			for(int j = 0; j < 255; j ++)
			{
				eeprom_read_block(str,j,strlen(str));
				if (strcmp(str,".") ==0)
				{
					eeprom_update_block(string,j,strlen(string));
					goto Warning;
				}
			}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////				
		}
		else if ( I_Temp < X1 && I_RH < Y1) // if Temperature and Humidity exceeds the low boundary
		{
/*---------------------------------------- WRITE TO EEPROM ----------------------------------------------------------------*/
		int P = 1;
			char string[11], str[] = "d";
			sprintf(string,"T=%d H=%d.",I_Temp,I_RH);
			sprintf(str,"d",P);
			lcd_WriteCommand(0x01); // clear LCD
			_delay_ms(1000); // wait 1 second
			lcd_GoToXY(0,2); // position 0 column, row 1
			lcd_PrintString("WARNING!"); // display Warning message on LCD
			_delay_ms(1000); // wait 1 second
			lcd_WriteCommand(0x01); // clear LCD
			lcd_GoToXY(0,2); // position 0 column, row 1
			lcd_PrintString("Temp and Humid !"); // display message for what threshold was broken
			lcd_GoToXY(0,1); // position 0 column, row 2
			lcd_PrintString("is too low!"); // display message for what threshold was broken
			eeprom_busy_wait();
			for(int j = 0; j < 255; j ++)
			{
				eeprom_read_block(str,j,strlen(str));
				if (strcmp(str,".") ==0)
				{
					eeprom_update_block(string,j,strlen(string));
					goto Warning;
				}
			}
		}
		else if (I_Temp > X0) // if Temperature exceeds the high boundary
		{
/*---------------------------------------- WRITE TO EEPROM ----------------------------------------------------------------*/
			int P = 1;
			char string[11], str[] = "d";
			sprintf(string,"T=%d H=%d.",I_Temp,I_RH);
			sprintf(str,"d",P);
			lcd_WriteCommand(0x01); // clear LCD
			_delay_ms(1000); // wait 1 second
			lcd_GoToXY(0,2); // position 0 column, row 1
			lcd_PrintString("WARNING!"); // display Warning message on LCD
			_delay_ms(1000); // wait 1 second
			lcd_WriteCommand(0x01); // clear LCD
			lcd_GoToXY(0,2); // position 0 column, row 1
			lcd_PrintString("Temp"); 
			lcd_GoToXY(0,1); // position 0 column, row 2
			lcd_PrintString("is too High!"); // display message for what threshold was broken
			eeprom_busy_wait();
			for(int j = 0; j < 255; j ++)
			{
				eeprom_read_block(str,j,strlen(str));
				if (strcmp(str,".") ==0)
				{
					eeprom_update_block(string,j,strlen(string));
					goto Warning;
				}
			}
		}
		else if (I_Temp < X1) // if Temperature exceeds the low boundary
		{
/*---------------------------------------- WRITE TO EEPROM ----------------------------------------------------------------*/
			int P = 1;
			char string[11], str[] = "d";
			sprintf(string,"T=%d H=%d.",I_Temp,I_RH);
			sprintf(str,"d",P);
			lcd_WriteCommand(0x01); // clear LCD
			_delay_ms(1000); // wait 1 second
			lcd_GoToXY(0,2); // position 0 column, row 1
			lcd_PrintString("WARNING!"); // display Warning message on LCD
			_delay_ms(1000); // wait 1 second
			lcd_WriteCommand(0x01); // clear LCD
			lcd_GoToXY(0,2); // position 0 column, row 1
			lcd_PrintString("Temp");
			lcd_GoToXY(0,1); // position 0 column, row 2
			lcd_PrintString("is too Low!"); // display message for what threshold was broken
		}
		else if (I_RH > Y0) // if Humidity exceeds the high boundary
		{
/*---------------------------------------- WRITE TO EEPROM ----------------------------------------------------------------*/
			int P = 1;
			char string[11], str[] = "d";
			sprintf(string,"T=%d H=%d.",I_Temp,I_RH);
			sprintf(str,"d",P);
			lcd_WriteCommand(0x01); // clear LCD
			_delay_ms(1000); // wait 1 second
			lcd_GoToXY(0,2); // position 0 column, row 1
			lcd_PrintString("WARNING!"); // display Warning message on LCD
			_delay_ms(1000); // wait 1 second
			lcd_WriteCommand(0x01); // clear LCD
			lcd_GoToXY(0,2); // position 0 column, row 1
			lcd_PrintString("Humid");
			lcd_GoToXY(0,1); // position 0 column, row 2
			lcd_PrintString("is too High!"); // display message for what threshold was broken
			eeprom_busy_wait();
			for(int j = 0; j < 255; j ++)
			{
				eeprom_read_block(str,j,strlen(str));
				if (strcmp(str,".") ==0)
				{
					eeprom_update_block(string,j,strlen(string));
					goto Warning;
				}
			}
		}
		else if (I_RH < Y1) // if Humidity exceeds the low boundary
		{
/*---------------------------------------- WRITE TO EEPROM ----------------------------------------------------------------*/
			int P = 1;
			char string[11], str[] = "d";
			sprintf(string,"T=%d H=%d.",I_Temp,I_RH);
			sprintf(str,"d",P);
			lcd_WriteCommand(0x01); // clear LCD
			_delay_ms(1000); // wait 1 second
			lcd_GoToXY(0,2); // position 0 column, row 1
			lcd_PrintString("WARNING!"); // display Warning message on LCD
			_delay_ms(1000); // wait 1 second
			lcd_WriteCommand(0x01); // clear LCD
			lcd_GoToXY(0,2); // position 0 column, row 1
			lcd_PrintString("Humid");
			lcd_GoToXY(0,1); // position 0 column, row 2
			lcd_PrintString("is too Low!"); // display message for what threshold was broken
			eeprom_busy_wait();
			for(int j = 0; j < 255; j ++)
			{
				eeprom_read_block(str,j,strlen(str));
				if (strcmp(str,".") ==0)
				{
					eeprom_update_block(string,j,strlen(string));
					goto Warning;
				}
			}
		}
		else
		{
			break;
		}
																							
			while(1) // open 11th while loop
			{
				Warning:
/*---------------------------------------- OPTION TOO RESET ----------------------------------------------------------------*/
			if (PINE != (1<<3)) // if push button is pressed
			{
				DDRC |= (1<<3); //  set the DDRC High to make pin 3 a input
				PORTC &= ~(1<<3); // set PORTC pin 3 low
				DDRD |= (1<<1); //  set the DDRE High to make pin 1 a input
				PORTD &= ~(1<<1); // set PORTC pin 1 low
				_delay_ms(100); //wait 1/10 of a second
				PORTD &= ~(1<<1); // set PORTC pin 1 low
				_delay_ms(2000); // wait 2 seconds
				if (PINE != (1<<3))	// if a button press is held for 2 seconds  enter if statment		
				{
					PORTC = 0x00; // make PORTC LOW t turn off LED
					PORTD = 0x00; // make PORTC LOW to turn off Active Buzzer
					_delay_ms(2000); // wait 2 seconds
					lcd_WriteCommand(0x01); // CLEAR LCD for new message
					lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
					lcd_PrintString("RESETTING"); // instruct the user with a displayed message
					lcd_GoToXY(0,1); // position on LCD first column and first row (settings is backwards)
					lcd_PrintString("PROGRAM..."); // instruct the user with a displayed message
					_delay_ms(3000); // wait 3 seconds
					lcd_WriteCommand(0x01); // CLEAR LCD for new message
					lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
					lcd_PrintString("Please Don't"); // instruct the user with a displayed message
					lcd_GoToXY(0,1); // position on LCD first column and first row (settings is backwards)
					lcd_PrintString("Hold the Button"); // instruct the user with a displayed message
					_delay_ms(3000); // wait 3 seconds
					lcd_WriteCommand(0x01); // CLEAR LCD for new message
					lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
					lcd_PrintString("Please wait"); // instruct the user with a displayed message
					_delay_ms(2500); // wait 2.5 seconds 
					lcd_WriteCommand(0x01); // CLEAR LCD for new message
					lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
					lcd_PrintString("Reset Complete"); // instruct the user with a displayed message
					lcd_GoToXY(0,1); // position on LCD first column and first row (settings is backwards)
					lcd_PrintString("Thank You !"); // instruct the user with a displayed message
					_delay_ms(2500); // wait 2.5 seconds 
					lcd_WriteCommand(0x01); // clear display and reset program
					PCMSK1 = (1<<5); //
					PCICR = (1<<PCIE1); //
					sei(); // set global interrupt high (i)
				} // close if statement
		/*-------------------------------------------------------------- BUZZER ------------------------------------------------------------------------------------*/
			} // close if statement			
				DDRC |= (1<<3); // SET DDRC pin 3 to an output to turn LED ON
				PORTC |= (1<<3); // set PORTC pin 3 HIGH to turn BUzzer On
				DDRD |= (1<<1); // turn DDRD to an output to turn LED on
				PORTD |= (1<<1); // Set PORTC High to turn Buzzer on
				_delay_ms(100); // wait 1/10 seconds
				PORTD &= ~(1<<1); // set PORTD LOW to turn LED off
				_delay_ms(100); // wait 1/10 of a second
			} // close 11th while loop
	      continue;
	      }//close 10th while loop
	     }//close 9th while statement
	    continue;
	    } //close 4th if statement
	   } //close 8th while loop
	  } // end 3rd if statement
	 }// end 6th while loop
	}// close 2nd if statement
   } // close 4th while loop
  } // close 1st if statement
 }// close 1st while loop
}//close main function

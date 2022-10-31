#define F_CPU 16000000 // frequency of the ATMEGA328PB microprocessor used
#include <stdio.h> // standard library
#include <avr/io.h> // standard library
#include <util/delay.h> // standard library
#include <stdio.h> // standard library
#include <stdint.h> // standard library
#include <stdlib.h> // standard library
#include <string.h> // use for iota() function
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
/*--------------------------------------------------------------------------- MAIN (HELLO WORLD) --------------------------------------------------------------------------------------------------*/
int main() // main function
{
	lcd_init(); //initialize LCD see header file "I2C.h"
	char data[5];//define a char for itoa function
	double TempSetPref; // will be the value of the Potentiometer
	Init_ADC(); // initialize the Analog to Digital convector (Potentiometer)
	
	while(1) // open 1st while loop
	{
Hello:
		lcd_WriteCommand(0x01); // clear display (LCD)
		_delay_ms(100); // delay a set amount
		lcd_GoToXY(2,2); // position on LCD first column and first row (settings is backwards)
		lcd_PrintString("Welcome To"); // welcome message
		lcd_GoToXY(2,1); // position on LCD first column and first row (settings is backwards)
		lcd_PrintString("ECE388 2022"); // personalize the display
		_delay_ms(2500); // delay a set amount
/*------------------------------------------------------------------------------------ SELECT HIGH TEMPETURE  ---------------------------------------------------------------------------------*/	
HIGH_T:		
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
		DDRE = ~(1<<3); // set the DDRE low to make it a input this will allow the press button to be read
		int X0; // integer holder to carry integers around program
		TempSetPref=(double)Potentiometer/1024*ReadADC_0(0); //reading Potentiometer value in Ohms
		lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
		lcd_PrintString("High TEMP = "); // read the Temperature value on the LCD
		lcd_GoToXY(14,2); // position on LCD first column and first row (settings is backwards)
		lcd_WriteData(0XDF); // Ascii value for degree symbol
		lcd_GoToXY(15,2); // position on LCD first column and first row (settings is backwards)
		lcd_PrintString("F"); // display a F for Fahrenheit
		lcd_GoToXY(0,1); // position on LCD first column and first row (settings is backwards)
		lcd_PrintString("THEN PUSH BUTTON"); // instruct the user with a displayed message
		itoa(TempSetPref,data,10); // itoa() functions converts integer to string
		lcd_GoToXY(12,2); // position on LCD first column and second row
		lcd_PrintString(data); // print value stores in char data
		X0 = TempSetPref; //carry the value of TEMP (temperature set by user) in X0
		
					if (PINE != (1<<3)) // open 1st if loop, if push button is pressed then true
					{
						_delay_ms(1000);
/*---------------------------------------- OPTION TOO RESET ----------------------------------------------------------------*/						
					if (PINE != (1<<3))
					{	
						lcd_WriteCommand(0x01); // clear display (LCD)
						lcd_GoToXY(2,2); // position on LCD first column and first row (settings is backwards)
						lcd_PrintString("Let go of Button"); // welcome message
						lcd_GoToXY(2,1); // position on LCD first column and first row (settings is backwards)
						lcd_PrintString("To go Back"); // personalize the display
						_delay_ms(4000);
						if (PINE != (1<<3)) // inactive state the button is reading high
					{
						lcd_WriteCommand(0x01); // CLEAR LCD for new message
						lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
						lcd_PrintString("RESETTING"); // instruct the user with a displayed message
						lcd_GoToXY(0,1); // position on LCD first column and first row (settings is backwards)
						lcd_PrintString("PROGRAM..."); // instruct the user with a displayed message
						_delay_ms(3000);
						lcd_WriteCommand(0x01); // CLEAR LCD for new message
						lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
						lcd_PrintString("Please Don't"); // instruct the user with a displayed message
						lcd_GoToXY(0,1); // position on LCD first column and first row (settings is backwards)
						lcd_PrintString("Hold the Button"); // instruct the user with a displayed message
						_delay_ms(3000);
						lcd_WriteCommand(0x01); // CLEAR LCD for new message
						lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
						lcd_PrintString("Please wait"); // instruct the user with a displayed message
						_delay_ms(2500);
						lcd_WriteCommand(0x01); // CLEAR LCD for new message
						lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
						lcd_PrintString("Reset Complete"); // instruct the user with a displayed message
						lcd_GoToXY(0,1); // position on LCD first column and first row (settings is backwards)
						lcd_PrintString("Thank You !"); // instruct the user with a displayed message
						_delay_ms(2500);
						lcd_WriteCommand(0x01); // clear display and reset program
						PCMSK1 = (1<<5); //
						PCICR = (1<<PCIE1); //
						sei(); // set global interrupt high (i)
					}
					goto Hello;
					}
/*-------------------------------------------------------------------------------------- SELECT LOW TEMPETURE  ----------------------------------------------------------------------------------*/
	while(1) // open 3rd while loop-
	{
LOW_T:
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
		DDRE = ~(1<<3);
		int X1; // integer holder to carry integers around program (This probably only makes since too myself Curtis Searle)
		TempSetPref=(double)Potentiometer/1024*ReadADC_0(0); //reading Y axis value in Ohms
		_delay_ms(250); //wait
		lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
		lcd_PrintString("Low TEMP = "); // read the Temperature value on the LCD
		lcd_GoToXY(13,2); // position on LCD first column and first row (settings is backwards)
		lcd_WriteData(0XDF); // Ascii value for degree symbol
		lcd_GoToXY(14,2); // position on LCD first column and first row (settings is backwards)
		lcd_PrintString("F");
		lcd_GoToXY(0,1); // position on LCD first column and first row (settings is backwards)
		lcd_PrintString("THEN PUSH BUTTON");
		itoa(TempSetPref,data,10); // itoa() functions converts integer to string
		lcd_GoToXY(11,2); // position on LCD first column and second row
		lcd_PrintString(data); // print value stores in char data
		X1 = TempSetPref; //carry the value of TEMP (temperature set by user) in X
				
					if (PINE != (1<<3)) // open 1st if loop, if push button is pressed then true
					{
					_delay_ms(1000);
/*---------------------------------------- OPTION TOO RESET ----------------------------------------------------------------*/
					if (PINE != (1<<3))
					{
						lcd_WriteCommand(0x01); // clear display (LCD)
						lcd_GoToXY(2,2); // position on LCD first column and first row (settings is backwards)
						lcd_PrintString("Let go of Button"); // welcome message
						lcd_GoToXY(2,1); // position on LCD first column and first row (settings is backwards)
						lcd_PrintString("To go Back"); // personalize the display
						_delay_ms(4000);
					if (PINE != (1<<3)) // inactive state the button is reading high
					{
						_delay_ms(2000);
						lcd_WriteCommand(0x01); // CLEAR LCD for new message
						lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
						lcd_PrintString("RESETTING"); // instruct the user with a displayed message
						lcd_GoToXY(0,1); // position on LCD first column and first row (settings is backwards)
						lcd_PrintString("PROGRAM..."); // instruct the user with a displayed message
						_delay_ms(3000);
						lcd_WriteCommand(0x01); // CLEAR LCD for new message
						lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
						lcd_PrintString("Please Don't"); // instruct the user with a displayed message
						lcd_GoToXY(0,1); // position on LCD first column and first row (settings is backwards)
						lcd_PrintString("Hold the Button"); // instruct the user with a displayed message
						_delay_ms(3000);
						lcd_WriteCommand(0x01); // CLEAR LCD for new message
						lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
						lcd_PrintString("Please wait"); // instruct the user with a displayed message
						_delay_ms(2500);
						lcd_WriteCommand(0x01); // CLEAR LCD for new message
						lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
						lcd_PrintString("Reset Complete"); // instruct the user with a displayed message
						lcd_GoToXY(0,1); // position on LCD first column and first row (settings is backwards)
						lcd_PrintString("Thank You !"); // instruct the user with a displayed message
						_delay_ms(2500);
						lcd_WriteCommand(0x01); // clear display and reset program
						PCMSK1 = (1<<5); //
						PCICR = (1<<PCIE1); //
						sei(); // set global interrupt high (i)
					}
					goto HIGH_T;
					}
/*---------------------------------------------------------------------- SELECT HIGH HUMIDITY  ---------------------------------------------------------------------------*/
	while(1)
	{ // open 5th while loop
HIGH_H:
		lcd_WriteCommand(0x01);
		lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
		lcd_PrintString("Please Select");
		lcd_GoToXY(0,1); // position on LCD first column and first row (settings is backwards)
		lcd_PrintString("High Humidity");
		_delay_ms(2500);
		lcd_WriteCommand(0x01);
		break;
	} // close 5th while loop
	while(1) // open 6th while loop
	{
		DDRE = ~(1<<3);
		int Y0; // integer holder to carry integers around program (This probably only makes since too myself Curtis Searle)
		TempSetPref=(double)Potentiometer/1024*ReadADC_0(0); //reading Y axis value in Ohms
		_delay_ms(250); // delay a set time
		lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
		lcd_PrintString("High HUMID = "); // read the Humidity value on the LCD
		lcd_GoToXY(15,2); // position on LCD first column and first row (settings is backwards)
		lcd_PrintString("%");
		lcd_GoToXY(0,1); // position on LCD first column and first row (settings is backwards)
		lcd_PrintString("THEN PUSH BUTTON");
		itoa(TempSetPref,data,10); // itoa() functions converts integer to string
		lcd_GoToXY(13,2); // position on LCD first column and second row
		lcd_PrintString(data); // print value stores in char data
		Y0 = TempSetPref; //carry the value of TEMP (temperature set by user) in X

					if (PINE != (1<<3)) // open 1st if loop, if push button is pressed then true
					{
					_delay_ms(1000);
/*---------------------------------------- OPTION TOO RESET ----------------------------------------------------------------*/
					if (PINE != (1<<3))
					{
					lcd_WriteCommand(0x01); // clear display (LCD)
					lcd_GoToXY(2,2); // position on LCD first column and first row (settings is backwards)
					lcd_PrintString("Let go of Button"); // welcome message
					lcd_GoToXY(2,1); // position on LCD first column and first row (settings is backwards)
					lcd_PrintString("To go Back"); // personalize the display
					_delay_ms(4000);
					if (PINE != (1<<3)) // inactive state the button is reading high
						if (PINE != (1<<3)) // inactive state the button is reading high
						{
							_delay_ms(2000);
							lcd_WriteCommand(0x01); // CLEAR LCD for new message
							lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
							lcd_PrintString("RESETTING"); // instruct the user with a displayed message
							lcd_GoToXY(0,1); // position on LCD first column and first row (settings is backwards)
							lcd_PrintString("PROGRAM..."); // instruct the user with a displayed message
							_delay_ms(3000);
							lcd_WriteCommand(0x01); // CLEAR LCD for new message
							lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
							lcd_PrintString("Please Don't"); // instruct the user with a displayed message
							lcd_GoToXY(0,1); // position on LCD first column and first row (settings is backwards)
							lcd_PrintString("Hold the Button"); // instruct the user with a displayed message
							_delay_ms(3000);
							lcd_WriteCommand(0x01); // CLEAR LCD for new message
							lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
							lcd_PrintString("Please wait"); // instruct the user with a displayed message
							_delay_ms(2500);
							lcd_WriteCommand(0x01); // CLEAR LCD for new message
							lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
							lcd_PrintString("Reset Complete"); // instruct the user with a displayed message
							lcd_GoToXY(0,1); // position on LCD first column and first row (settings is backwards)
							lcd_PrintString("Thank You !"); // instruct the user with a displayed message
							_delay_ms(2500);
							lcd_WriteCommand(0x01); // clear display and reset program
							PCMSK1 = (1<<5); //
							PCICR = (1<<PCIE1); //
							sei(); // set global interrupt high (i)
						}
						goto LOW_T;
						}
/*-------------------------------------------------------------------- SELECT LOW HUMIDITY  --------------------------------------------------------------------------*/
	while(1)
	{ // open 7th while loop
LOW_H:		
		lcd_WriteCommand(0x01);
		lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
		lcd_PrintString("Please Select");
		lcd_GoToXY(0,1); // position on LCD first column and first row (settings is backwards)
		lcd_PrintString("Low Humidity");
		_delay_ms(2500);
		lcd_WriteCommand(0x01);
		break;
	} // close 7th while loop
	while(1) // open 8th while loop
	{
		DDRE = ~(1<<3);
		int Y1; // integer holder to carry integers around program (This probably only makes since too myself Curtis Searle)
		TempSetPref=(double)Potentiometer/1024*ReadADC_0(0); //reading Y axis value in Ohms
		lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
		lcd_PrintString("Low HUMID = "); // read the Humidity value on the LCD
		lcd_GoToXY(14,2); // position on LCD first column and first row (settings is backwards)
		lcd_PrintString("%");
		lcd_GoToXY(0,1); // position on LCD first column and first row (settings is backwards)
		lcd_PrintString("THEN PUSH BUTTON");
		itoa(TempSetPref,data,10); // itoa() functions converts integer to string
		lcd_GoToXY(12,2); // position on LCD first column and second row
		lcd_PrintString(data); // print value stores in char data
		Y1 = TempSetPref; //carry the value of TEMP (temperature set by user) in X			
					
					if (PINE != (1<<3)) // open 1st if loop, if push button is pressed then true
					{
					_delay_ms(1000);
/*---------------------------------------- OPTION TOO RESET ----------------------------------------------------------------*/
					if (PINE != (1<<3))
					{
							lcd_WriteCommand(0x01); // clear display (LCD)
							lcd_GoToXY(2,2); // position on LCD first column and first row (settings is backwards)
							lcd_PrintString("Let go of Button"); // welcome message
							lcd_GoToXY(2,1); // position on LCD first column and first row (settings is backwards)
							lcd_PrintString("To go Back"); // personalize the display
							_delay_ms(4000);
						if (PINE != (1<<3)) // inactive state the button is reading high
						{
							_delay_ms(2000);
							lcd_WriteCommand(0x01); // CLEAR LCD for new message
							lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
							lcd_PrintString("RESETTING"); // instruct the user with a displayed message
							lcd_GoToXY(0,1); // position on LCD first column and first row (settings is backwards)
							lcd_PrintString("PROGRAM..."); // instruct the user with a displayed message
							_delay_ms(3000);
							lcd_WriteCommand(0x01); // CLEAR LCD for new message
							lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
							lcd_PrintString("Please Don't"); // instruct the user with a displayed message
							lcd_GoToXY(0,1); // position on LCD first column and first row (settings is backwards)
							lcd_PrintString("Hold the Button"); // instruct the user with a displayed message
							_delay_ms(3000);
							lcd_WriteCommand(0x01); // CLEAR LCD for new message
							lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
							lcd_PrintString("Please wait"); // instruct the user with a displayed message
							_delay_ms(2500);
							lcd_WriteCommand(0x01); // CLEAR LCD for new message
							lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
							lcd_PrintString("Reset Complete"); // instruct the user with a displayed message
							lcd_GoToXY(0,1); // position on LCD first column and first row (settings is backwards)
							lcd_PrintString("Thank You !"); // instruct the user with a displayed message
							_delay_ms(2500);
							lcd_WriteCommand(0x01); // clear display and reset program
							PCMSK1 = (1<<5); //
							PCICR = (1<<PCIE1); //
							sei(); // set global interrupt high (i)
						}
						goto HIGH_H;
						}							
/*----------------------------------------------------------------------- DISPLAY PREFERENCES  --------------------------------------------------------------------------*/			
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
	lcd_GoToXY(9,2); // position on LCD first column and first row (settings is backwards)
	lcd_WriteData(0XDF);
	lcd_GoToXY(10,2);
	lcd_PrintString("F"); // confirm preferences
	lcd_GoToXY(0,1);
	lcd_PrintString("Humid ="); // confirm preferences
	itoa(Y0,data,10); // convert integer to string
	lcd_GoToXY(8,1); // position on LCD first column and second row
	lcd_PrintString(data); // char declaration to satisfy itoa function
	lcd_GoToXY(11,1);
	lcd_PrintString("%"); // confirm preferences
	_delay_ms(3000); // wait
									
	lcd_WriteCommand(0x01); // clear LCD
	lcd_GoToXY(0,2);
	lcd_PrintString("Lower Limits"); // confirm preferences
	lcd_GoToXY(0,1);
	lcd_PrintString("are set too:"); // confirm preferences
	_delay_ms(2500); // wait
									
	lcd_WriteCommand(0x01); // clear LCD
	lcd_GoToXY(0,2);
	lcd_PrintString("Temp ="); // confirm preferences
	itoa(X1,data,10); // convert integer to string
	lcd_GoToXY(7,2); // position on LCD first column and second row
	lcd_PrintString(data); // char declaration to satisfy itoa function
	lcd_GoToXY(9,2); // position on LCD first column and first row (settings is backwards)
	lcd_WriteData(0XDF);
	lcd_GoToXY(10,2);
	lcd_PrintString("F"); // confirm preferences
	lcd_GoToXY(0,1);
	lcd_PrintString("Humid ="); // confirm preferences
	itoa(Y1,data,10); // convert integer to string
	lcd_GoToXY(8,1); // position on LCD first column and second row
	lcd_PrintString(data); // char declaration to satisfy itoa function
	lcd_GoToXY(11,1);
	lcd_PrintString("%"); // confirm preferences
	_delay_ms(3000); // wait
									
	_delay_ms(2000);
	lcd_WriteCommand(0x01);
	_delay_ms(500);
/*----------------------------------------------------------------------- DHT11 INITIALIZE ----------------------------------------------------------------------------------*/
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
		if ((I_RH + D_RH + I_Temp + D_Temp) != CheckSum)
		{
			lcd_GoToXY(0,0);
			lcd_PrintString("Error");
		}
		else
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
			_delay_ms(500);
/*------------------------ OPTION TO RESET -------------------------------------------------*/										
					if (PINE != (1<<3)) // inactive state the button is reading high
					{
						_delay_ms(2000);
					if (PINE != (1<<3))
					{
						goto Hello;
					}
						break;
					}	
/*---------------------------------- WARNING DISPLAY -------------------------------------------------------------------------------------------

	we have 2 objects Temperature and Humidity both with three options; that is equal too (=), less than (<), or greater then (>). 
	Therefore, we can use permutation so that 3*3 = 9 there are 9 possible outcomes for any combination of humidity and Tempeture.

	*/

	if ( I_Temp > X0 && I_RH > Y0)
	{
		lcd_WriteCommand(0x01);
		_delay_ms(1000);
		lcd_GoToXY(0,2);
		lcd_PrintString("WARNING!");
		_delay_ms(1000);
		lcd_GoToXY(0,2);
		lcd_PrintString("Humid and Temp");
		lcd_GoToXY(0,1);
		lcd_PrintString("is too High!");
	}
	else if ( I_Temp > X0 && I_RH < Y1)
	{
		lcd_WriteCommand(0x01);
		_delay_ms(1000);
		lcd_GoToXY(0,2);
		lcd_PrintString("WARNING!");
		_delay_ms(1000);
		lcd_WriteCommand(0x01);
		lcd_GoToXY(0,2);
		lcd_PrintString("Temp is High !");
		lcd_GoToXY(0,1);
		lcd_PrintString("Humid is Low!");
	}

	else if ( I_Temp < X1 && I_RH > Y0)
	{
		lcd_WriteCommand(0x01);
		_delay_ms(1000);
		lcd_GoToXY(0,2);
		lcd_PrintString("WARNING!");
		_delay_ms(1000);
		lcd_WriteCommand(0x01);
		lcd_GoToXY(0,2);
		lcd_PrintString("Temp is Low !");
		lcd_GoToXY(0,1);
		lcd_PrintString("Humid is High!");
	}
	else if ( I_Temp < X1 && I_RH < Y1)
	{
		lcd_WriteCommand(0x01);
		_delay_ms(1000);
		lcd_GoToXY(0,2);
		lcd_PrintString("WARNING!");
		_delay_ms(1000);
		lcd_WriteCommand(0x01);
		lcd_GoToXY(0,2);
		lcd_PrintString("Temp and Humid !");
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
		lcd_WriteCommand(0x01);
		lcd_GoToXY(0,2);
		lcd_PrintString("Temp");
		lcd_GoToXY(0,1);
		lcd_PrintString("is too High!");
		/*char str[10];
		sprintf(str, "%d", I_Temp);
		char EEPROM0[15],EEPROM1[15] = "F";
		lcd_init();
		memset(EEPROM0,0,15);
		eeprom_busy_wait(); //Initialize LCD
		eeprom_write_block(str,0,strlen(EEPROM1)); // Write W_arrayfrom EEPROM address 0
		eeprom_read_block(EEPROM0,0,strlen(EEPROM1)); //Read EEPROM from address 0
		//lcd_PrintString(EEPROM0); // Print Read_array on LCD*/
	}
	else if (I_Temp < X1)
	{
		lcd_WriteCommand(0x01);
		_delay_ms(1000);
		lcd_GoToXY(0,2);
		lcd_PrintString("WARNING!");
		_delay_ms(1000);
		lcd_WriteCommand(0x01);
		lcd_GoToXY(0,2);
		lcd_PrintString("Temp");
		lcd_GoToXY(0,1);
		lcd_PrintString("is too Low!");
		/*char str[10];
		sprintf(str, "%d", I_Temp);
		char EEPROM0[15],EEPROM1[15] = "F";
		memset(EEPROM0,0,15);
		eeprom_busy_wait(); //Initialize LCD
		eeprom_write_block(str,0,strlen(EEPROM1)); // Write W_arrayfrom EEPROM address 0
		eeprom_read_block(EEPROM0,0,strlen(EEPROM1)); //Read EEPROM from address 0
		//lcd_PrintString(EEPROM0); // Print Read_array on LCD*/
	}
	else if (I_RH > Y0)
	{
		lcd_WriteCommand(0x01);
		_delay_ms(1000);
		lcd_GoToXY(0,2);
		lcd_PrintString("WARNING!");
		_delay_ms(1000);
		lcd_WriteCommand(0x01);
		lcd_GoToXY(0,2);
		lcd_PrintString("Humid");
		lcd_GoToXY(0,1);
		lcd_PrintString("is too High!");
		/*char str[10];
		sprintf(str, "%d", I_RH);
		char EEPROM0[15],EEPROM1[15] = "God is True";
		memset(EEPROM0,0,15);
		eeprom_busy_wait(); //Initialize LCD
		eeprom_write_block(str,0,strlen(EEPROM1)); // Write W_arrayfrom EEPROM address 0
		eeprom_read_block(EEPROM0,0,strlen(EEPROM1)); //Read EEPROM from address 0
		//lcd_PrintString(EEPROM0); // Print Read_array on LCD*/
	}
	else if (I_RH < Y1)
	{
		lcd_WriteCommand(0x01);
		_delay_ms(1000);
		lcd_GoToXY(0,2);
		lcd_PrintString("WARNING!");
		_delay_ms(1000);
		lcd_WriteCommand(0x01);
		lcd_GoToXY(0,2);
		lcd_PrintString("Humid");
		lcd_GoToXY(0,1);
		lcd_PrintString("is too Low!");
		/*char str[10];
		sprintf(str, "%d", I_RH);
		char EEPROM0[15],EEPROM1[15] = "God is True";
		memset(EEPROM0,0,15);
		eeprom_busy_wait(); //Initialize LCD 
		eeprom_write_block(str,0,strlen(EEPROM1)); // Write W_arrayfrom EEPROM address 0 
		eeprom_read_block(EEPROM0,0,strlen(EEPROM1)); //Read EEPROM from address 0
		//lcd_PrintString(EEPROM0); // Print Read_array on LCD*/	
	}
	else
	{
		break;
	}
																							
	while(1) // open 11th while loop
	{
/*---------------------------------------- OPTION TOO RESET ----------------------------------------------------------------*/
					if (PINE != (1<<3)) // inactive state the button is reading high
					{
					_delay_ms(2000);
					if (PINE != (1<<3))				
					{
					DDRC |= (1<<2)|(1<<3); // set high to write to the pin DDRC = ouput
					//PORTC |= (1<<3); // set portc pin 3 HIGH
					PORTC &= ~(1<<3); // set portc pin 3 low
					PORTC &= ~(1<<2); // set pin 2 low
					_delay_ms(2000);
					lcd_WriteCommand(0x01); // CLEAR LCD for new message
					lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
					lcd_PrintString("RESETTING"); // instruct the user with a displayed message
					lcd_GoToXY(0,1); // position on LCD first column and first row (settings is backwards)
					lcd_PrintString("PROGRAM..."); // instruct the user with a displayed message
					_delay_ms(3000);
					lcd_WriteCommand(0x01); // CLEAR LCD for new message
					lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
					lcd_PrintString("Please Don't"); // instruct the user with a displayed message
					lcd_GoToXY(0,1); // position on LCD first column and first row (settings is backwards)
					lcd_PrintString("Hold the Button"); // instruct the user with a displayed message
					_delay_ms(3000);
					lcd_WriteCommand(0x01); // CLEAR LCD for new message
					lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
					lcd_PrintString("Please wait"); // instruct the user with a displayed message
					_delay_ms(2500);
					lcd_WriteCommand(0x01); // CLEAR LCD for new message
					lcd_GoToXY(0,2); // position on LCD first column and first row (settings is backwards)
					lcd_PrintString("Reset Complete"); // instruct the user with a displayed message
					lcd_GoToXY(0,1); // position on LCD first column and first row (settings is backwards)
					lcd_PrintString("Thank You !"); // instruct the user with a displayed message
					_delay_ms(2500);
					lcd_WriteCommand(0x01); // clear display and reset program
					PCMSK1 = (1<<5); //
					PCICR = (1<<PCIE1); //
					sei(); // set global interrupt high (i)
				}
/*-------------------------------------------------------------- BUZZER ------------------------------------------------------------------------------------*/
			}
	DDRC |= (1<<2);
	PORTC |= (1<<2);
	//_delay_ms(10);
	//PORTC &= ~(1<<2);
	//_delay_ms(10);
	//PORTC |= (1<<2);
	//_delay_ms(10);
	DDRC = (1<<3);
	PORTC = (1<<3);     // Turn ON the Buzzer connected to PORTC
	_delay_us(80);      // Wait time delay will effect the frequency of buzz
	PORTC = ~(1<<3);       // Turn OFF the Buzzer connected to PORTC
	_delay_us(80);      // Wait time delay will effect the frequency of buzz
						} // close 11th while loop
						}
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
 }// close 2nd while loop
}//close main fucntion


/*
#define F_CPU 16000000UL			// Define frequency here its 8MHz 
#include <avr/io.h>			// Include avr std header file 
#include <util/delay.h>			// Include delay header file 
#include <avr/eeprom.h>			// Include AVR EEPROM header file 
#include <string.h>			// Include string header file 
#include "I2C.h"		// Include LCD header file 

int main()
{
	int i = 55;
	char str[10];
	sprintf(str, "%d", i);
	char EEPROM0[15],EEPROM1[15] = "God is True";
	lcd_init();
	memset(EEPROM0,0,15);
	eeprom_busy_wait();		/Initialize LCD 
	eeprom_write_block(str,0,strlen(EEPROM1));	// Write W_arrayfrom EEPROM address 0 
	eeprom_read_block(EEPROM0,0,strlen(EEPROM1));	/Read EEPROM from address 0
	lcd_PrintString(EEPROM0);		// Print Read_array on LCD
	return(0);
}*/

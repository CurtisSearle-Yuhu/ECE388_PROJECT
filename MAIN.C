// MAIN FUNCTION

#define F_CPU 16000000 // frequency of the ATMEGA328PB microprocessor used
#include <stdio.h> // standard library
#include <avr/io.h> // standard library
#include <util/delay.h> // standard library
#include <stdio.h> // standard library
#include <stdint.h> // standard library
#include <stdlib.h> // standard library
#include <string.h> // use for iota() function this will turn a integer to a string to be printed on LCD
#include <stdbool.h> // not used its just a header file I need too learn
#include "ADC.h" // header file to use Analog To Digital Converter (potentiometer)
#include "I2C.h" // header file to interface with I2C Protocol
#include "DHT11.h" // header file interface with DHT11 Temperature and Humidity sensor
#define Potentiometer 100.00 //the value of the potentiometer in resistance values higher then 10k can crash program (feel free to test it) //update this was long ago i should test this again

int main()
{
	lcd_init(); //initialize LCD
	char data[5];//define a char for itoa function
	double TempSetPref; //
	Init_ADC(); //initialize ADC	
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
			if (PINE != (1<<3)) // if button press is push begin if statement
			{/// first if loop start
				X = TempSetPref; //carry the value of TEMP (temperature set by user) in X
				_delay_ms(2000); // wait 
				lcd_WriteCommand(0x01); // clear LCD
				_delay_ms(500); // wait 
				lcd_GoToXY(0,2);
				lcd_PrintString("Temperature set too:"); // confirm preferences
				itoa(TempSetPref,data,10); // convert integer to string
				lcd_GoToXY(0,1); // position on LCD first column and second row
				lcd_PrintString(data); // char declaration to satisfy itoa function
				_delay_ms(5000);
				lcd_WriteCommand(0x01);
				_delay_ms(500);	
		while(1) 
		{ // second while statement
		Request(); // too START DHT11 send start pulse in bits
		Response(); // receive response in form of bits
		Integer_Humidity=Receive_data(); // store first eight bit in I_RH
		Decimal_Humidity=Receive_data(); // store next eight bit in D_RH
		Integer_Temp=Receive_data(); // store next eight bit in I_Temp
		Decimal_Temp=Receive_data(); // store next eight bit in D_Temp
		CheckSum=Receive_data();// store next eight bit in CheckSum
			if ((Integer_Humidity + Decimal_Humidity + Integer_Temp + Decimal_Temp) != CheckSum) // stole this for statement from on line probably not needed
			{ //third if statement
				lcd_GoToXY(0,2);
				lcd_PrintString("Error");
			} // third if statement end
			else 
			{ // first else statement
				char DATA[5];// use to satisfie iota() function can call the charcter anything you like
				lcd_init(); // Initialize LCD
				lcd_GoToXY(1,1); // Enter column and row position
				lcd_PrintString("Temp = ");
				lcd_GoToXY(1,2); // (start of string display,line the string is displayed) values is backwards 
				lcd_PrintString("Humidity = ");
				Integer_Temp = (Integer_Temp*1.8)+(32);
				itoa(Integer_Humidity,DATA,10); // integer humidity display
				lcd_GoToXY(12,2);
				lcd_PrintString(DATA);
				lcd_PrintString(".");
				itoa(Decimal_Humidity,DATA,10); // Decimal humidity display
				lcd_PrintString(DATA);
				lcd_PrintString("%");
				itoa(Integer_Temp,DATA,10); // integer temperature display
				lcd_GoToXY(6,1);
				lcd_PrintString(DATA);
				lcd_PrintString(".");
				itoa(Decimal_Temp,DATA,10); // decimal Temperature display
				lcd_PrintString(DATA);
				lcd_WriteData(0xDF);
				lcd_PrintString("F");
				itoa(CheckSum,DATA,10); // Check sum display if deleted CheckSum for loop delete this too
				lcd_GoToXY(0,1); // if deleted CheckSum for loop delete this too
				lcd_PrintString(DATA); // if deleted CheckSum for loop delete this too
				lcd_PrintString(" "); // if deleted CheckSum for loop delete this too 
				_delay_ms(2000); // if deleted CheckSum for loop delete this too
				
				if (Integer_Temp <= X) // if temperature set by user is equal or falls below preference this if statment will occur
				{ // fourth if statment
					_delay_ms(1000);
					lcd_WriteCommand(0x01);
					_delay_ms(1000);
					lcd_GoToXY(0,2);
					lcd_PrintString("THIS IS VERY BAD"); // WARNING statement
					lcd_GoToXY(0,1);
					lcd_PrintString("THIS IS VERY BAD"); // repetitive not needed
					_delay_ms(1000);
					lcd_WriteCommand(0x01);						
					_delay_ms(5000); // doest need be such a long wait shorten the time
					while(1)
					{ // third while loop
						DDRC = (1<<3); // make physical PINC a input
						PORTC = (1<<3); // Turn ON the Buzzer connected to PORTC
						_delay_us(100); // Wait time delay will effect the frequency of buzz
						PORTC = ~(1<<3); // Turn OFF the Buzzer connected to PORTC
						_delay_us(100); // Wait time delay will effect the frequency of buzz
						continue; // continue buzzing until shut off
					} // close 3rd while loop
				}// close fourth if statement
			continue;
		 }//close second if statement
		}//close third while statement
		continue;	
  } //close second while
 } //close first while
} // end main fucntion

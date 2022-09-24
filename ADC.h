//analog to digital conversion header file used to interact with the potentiometer

void Init_ADC()
{
	ADMUX |= (1<<REFS0); // select VREF=AVCC
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADEN); // see 28.4 of the ATMEGA datasheet *128
}
uint16_t ReadADC_0(uint8_t ADC_Channel)
{
	ADMUX = (ADMUX & 0xF0) | (ADC_Channel & 0x0F); //select ADC channel with safety mask
	ADCSRA |= (1<<ADSC); //single conversion mode
	while( ADCSRA & (1<<ADSC) ); // 
	return ADC; //
}

//// following codes below are not used for this project it was extracted for other projects PLEASE DO NOT DELETE
/*uint16_t ReadADC_Y(uint8_t ADCchannel)
{
	ADMUX = (ADMUX & 0xF0) | (ADCchannel & 0x0F); //select ADC channel with safety mask
	ADCSRA |= (1<<ADSC); //single conversion mode
	while( ADCSRA & (1<<ADSC) ); // wait until ADC conversion is complete
	return ADC; //
}
uint16_t ReadADC_R3(uint8_t ADCchannel)
{
	ADMUX = (ADMUX & 0xF0) | (ADCchannel & 0x0F); //select ADC channel with safety mask
	ADCSRA |= (1<<ADSC); //single conversion mode
	while( ADCSRA & (1<<ADSC) ); // wait until ADC conversion is complete
	return ADC;
}*/

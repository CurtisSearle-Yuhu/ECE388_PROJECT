//analog to digital conversion header file used to interact with the potentiometer


void Init_ADC()
{
	ADMUX |= (1<<REFS0); // Select Vref=AVcc
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADEN); //set prescaller to 128 and enable ADC
}
uint16_t ReadADC_0(uint8_t ADCchannel)
{
	ADMUX = (ADMUX & 0xF0) | (ADCchannel & 0x0F); //select ADC channel with safety mask
	ADCSRA |= (1<<ADSC); //single conversion mode
	while( ADCSRA & (1<<ADSC) ); // wait until ADC conversion is complete
	return ADC; //
}
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

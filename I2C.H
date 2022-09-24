// HEADER FILE I2C FUNCTION TAKEN FROM ECE263 USED FOR I2C LCD

// I2C BASIC LOW LEVEL PROTOTYPES
void i2c_init();
void i2c_start();
void i2c_stop();
void i2c_write(uint8_t d);
uint8_t i2c_read(uint8_t ackVal);

// LCD PROTOTYPES AND DEFINES
#define LCDADDR (0x27<<1)
#define LCDWRITE 0
#define LCDREAD 1

// Pin Connections:
//  7  6  5  4  3  2  1  0
// D7 D6 D5 D4 BL  E RW RS
#define BLBIT 3
#define ENBIT 2
#define RWBIT 1
#define RSBIT 0

#define DATA 1
#define CMD 0

// Possibly defining macros based off these later. For now basically documentation
#define BLHI (1<<BLBIT)
#define BLLO (0<<BLBIT)
#define ENHI (1<<ENBIT)
#define ENLO (0<<ENBIT)
#define RWHI (1<<RWBIT)
#define RWLO (0<<RWBIT)
#define RSHI (1<<RSBIT)
#define RSLO (0<<RSBIT)

// LCD Function prototypes
void lcd_init();
void lcd_WriteCommand(uint8_t d);
void lcd_WriteData(uint8_t d);
void lcd_PrintString(char s[]);
void lcd_GoToXY(uint8_t x, uint8_t y);
// I2C and routines based on ECE 263 class textbook pgs 642 to 627
void i2c_write(uint8_t d)
{
	TWDR0 = d;
	TWCR0 = 1<<TWINT | 1<<TWEN;
	while (!(TWCR0 & (1<<TWINT)))
	; // (TWCR0 & (1<<TWINT)) == 0, wait
}

uint8_t i2c_read(uint8_t ackVal)
{
	TWCR0 = 1<<TWINT | 1<<TWEN | ackVal<<TWEA;
	while (!(TWCR0 & (1<<TWINT)))
	;
	return TWDR0;
	
}

void i2c_start()
{
	TWCR0 = 1<<TWINT | 1<<TWSTA | 1<<TWEN;
	while (!(TWCR0 & (1<<TWINT)))
	;
}

void i2c_stop()
{
	TWCR0 = 1<<TWINT | 1<<TWEN | 1<<TWSTO;
}

void i2c_init()
{
	TWSR0 = 0x00;
	TWBR0 = 153;
	TWCR0 = 1<<TWEN;
}


// DEFINING LCD FUNCTIONS
void lcd_init()
{
	i2c_init();
	_delay_ms(100); // wait for i2c initialization to finish up completely
	lcd_WriteCommand(0x20); // FUNCTION SET, 4 bit data mode, ls 4 bits=x
	lcd_WriteCommand(0x28); // FUNCTION SET, 4 bit data mode, 2 lines, 5x8 font
	lcd_WriteCommand(0x08); // Turn display off
	lcd_WriteCommand(0x01); _delay_ms(4); // Clear display and wait a bit
	lcd_WriteCommand(0x06); // ENTRY MODE SELECT (INCR CURSOR, NO SHIFT DISPLAY)
	lcd_WriteCommand(0x0F); // Display on, cursor on, cursor blinking
}

void lcd_PrintString(char s[])
{
	while(*s)
	{
		lcd_WriteData(*s++);
	}
}

void lcd_GoToXY(uint8_t x, uint8_t y)
{
	lcd_WriteCommand(0x80 | (64*y+x));
}

void lcd_WriteData(uint8_t d)
{
	uint8_t nibbleH = d & 0xF0;
	uint8_t nibbleL = d<<4;
	i2c_start();
	i2c_write(LCDADDR | LCDWRITE);
	i2c_write(BLHI | RSHI | ENHI | RWLO | nibbleH);
	i2c_write(BLHI | RSHI | ENLO | RWLO | nibbleH);
	i2c_write(BLHI | RSHI | ENHI | RWLO | nibbleL);
	i2c_write(BLHI | RSHI | ENLO | RWLO | nibbleL);
	i2c_stop();
}

void lcd_WriteCommand(uint8_t d)
{
	uint8_t nibbleH = d & 0xF0;
	uint8_t nibbleL = d<<4;
	i2c_start();
	i2c_write(LCDADDR | LCDWRITE);
	i2c_write(BLHI | RSLO | ENHI | RWLO | nibbleH);
	i2c_write(BLHI | RSLO | ENLO | RWLO | nibbleH);
	i2c_write(BLHI | RSLO | ENHI | RWLO | nibbleL);
	i2c_write(BLHI | RSLO | ENLO | RWLO | nibbleL);
	i2c_stop();
}

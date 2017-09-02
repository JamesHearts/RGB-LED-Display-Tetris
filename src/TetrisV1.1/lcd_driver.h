/*
 * LCD_RFID Driver
 *
 * Created: 3/31/2017 5:11:11 PM
 * Author : pbouyounes
 */ 

#define F_CPU 8000000

void lcd_command(char cmd);
void lcd_data(char data);
void lcd_init(void);
void lcd_string(char a[]);
void intr_init(void);

ISR(PCINT1_vect)
{
	if ((PINB & 0x30) == 0x10)
	{
		lcd_init();
		lcd_command(0x01);
		lcd_string("Paul Bouyounes");
		lcd_command(0xC0);
		lcd_string("Player 1");
		_delay_ms(25);
	}
	
	else if ((PINB & 0x30) == 0x20)
	{
		lcd_init();
		lcd_command(0x01);
		lcd_string("James Mak ");
		lcd_command(0xC0);
		lcd_string("Player 2");
		_delay_ms(25);
	}
	
	else
	{
		lcd_init();
		lcd_command(0x01);
		lcd_string("PB&J's LED");
		lcd_command(0xC0);
		lcd_string("Game Station");
		_delay_ms(25);
	}
}

void intr_init(void)
{
	sei();
	PCICR = 0x02;
	PCMSK1 = 0x30;
}

void lcd_init(void)
{
	DDRB |= 0xCF;      //Change DDRA --> DDRB and PORTA --> PORTB if LCD attached to port B and so forth
	lcd_command(0x33); //Initialize LCD Driver
	lcd_command(0x32); //Four bit mode
	lcd_command(0x2C); //2 Line Mode
	lcd_command(0x0C); //Display On, Cursor Off, Blink Off Change to 0x0F if cursor is desired
	lcd_command(0x01); //Clear Screen, Cursor Home
}

void lcd_command(char cmd)
{
	char temp = cmd;
	PORTB = 0;
	_delay_ms(1);
	cmd = ( (cmd & 0xF0) >> 4) | 0x80; //Write Upper Nibble (RS=0) E --> 1
	PORTB = cmd;
	_delay_ms(1);
	cmd ^= 0x80; //E --> 0
	PORTB = cmd;
	_delay_ms(1);
	cmd = temp;
	cmd = (cmd & 0x0F) | 0x80; //Write Lower Nibble (RS=0) E --> 1
	PORTB = cmd;
	_delay_ms(1);
	cmd ^= 0x80; //E --> 0
	PORTB = cmd;
	_delay_ms(1);
}

void lcd_data(char data)
{
	char temp = data;
	PORTB = 0x40;
	_delay_ms(1);
	data = ( (data & 0xF0) >> 4) | 0xC0; //Write Upper Nibble (RS=1) E --> 1
	PORTB = data;
	_delay_ms(1);
	data ^= 0x80; // E --> 0
	PORTB = data;
	_delay_ms(1);
	data = temp;
	data = (data & 0x0F) | 0xC0; //Write Lower Nibble (RS=1) E --> 1
	PORTB = data;
	_delay_ms(1);
	data ^= 0x80; //E --> 0
	PORTB = data;
	_delay_ms(1);
}

void lcd_string(char a[]) //Write string on LCD
{
	for(int i = 0; i < strlen(a); i++)
	{
		lcd_data(a[i]);
	}
}
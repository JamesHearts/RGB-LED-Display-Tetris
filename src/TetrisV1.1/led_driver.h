#define BOARD_HEIGHT 20
#define BOARD_WIDTH 10

uint8_t row_leds = 30;
uint8_t row_length = 10;
uint8_t column_length = 20;

uint8_t OFF[7] = {0b00010001,0b00110001,0b00010001,0b00110001,0b00010001,0b00110001,0b00010001};
uint8_t BLUE[7] = {0b00010001,0b00110001,0b00010001,0b00110001,0b10010001,0b10110001,0b00010001};
uint8_t GREEN[7] = {0b00010001,0b00110001,0b10010001,0b10110001,0b00010001,0b00110001,0b00010001};
uint8_t CYAN[7] = {0b00010001,0b00110001,0b10010001,0b10110001,0b10010001,0b10110001,0b00010001};
uint8_t RED[7] = {0b10010001,0b10110001,0b00010001,0b00110001,0b00010001,0b00110001,0b00010001};
uint8_t PURPLE[7] = {0b10010001,0b10110001,0b00010001,0b00110001,0b10010001,0b10110001,0b00010001};
uint8_t YELLOW[7] = {0b10010001,0b10110001,0b10010001,0b10110001,0b00010001,0b00110001,0b00010001};
uint8_t WHITE[7] = {0b10010001,0b10110001,0b10010001,0b10110001,0b10010001,0b10110001,0b10010001};

uint8_t row_color_vector[10] = {0,0,0,0,0,0,0,0,0,0};

uint8_t color_program[20][10];
uint8_t color_program_temp[20][10];

void colorBoardInit()
{
	for(int i = 0; i < BOARD_HEIGHT; i++)
	{
		for(int j = 0; j < BOARD_WIDTH; j++)
		{
			color_program[i][j] = 0;
		}
	}
}

void testColorBoardInit()
{
	for(int i = 0; i < BOARD_HEIGHT; i++)
	{
		for(int j = 0; j < BOARD_WIDTH; j++)
		{
			color_program_temp[i][j] = 0;
		}
		
	}
}

void colorBoardSave()
{
	for(int i = 0; i < BOARD_HEIGHT; i++)
	{
		for(int j = 0; j < BOARD_WIDTH; j++)
		{
			color_program_temp[i][j] = color_program[i][j];
		}
	}
}

void colorBoardRestore()
{
	for(int i = 0; i < BOARD_HEIGHT; i++)
	{
		for(int j = 0; j < BOARD_WIDTH; j++)
		{
			color_program[i][j] = color_program_temp[i][j];
		}
	}
}

void colorBoardTest_2()
{
	while(1)
	{
		for(int k = 0;k < 8;k++)
		{
			for(int i = 0; i < BOARD_HEIGHT; i++)
			{
				for(int j = 0; j < BOARD_WIDTH; j++)
				{
					color_program[i][j] = k;
				}
				
					for(int m = 0; m < 10; m++)
					{
						matrix_refresh();
					}
			}
			
		
		}
	}
}

void colorBoardTest()
{
	uint8_t colorCounter = 0;
	uint8_t row = 0;
	uint8_t col = 0;
	uint8_t colUpdate = 1;
	
	while(1)
	{
		for(int i = 0; i < BOARD_WIDTH; i++)
		{
			color_program[row][i] = colorCounter;
		}
		
		if(row == BOARD_HEIGHT)
		{
			row = 0;
			
			colorCounter++;
				
			if(colorCounter > 7)
			{
					colorCounter = 0;
			}
			
		} 		

		matrix_refresh();
		_delay_ms(50);
		row++;
		
		if(((PIND & 0x10) == 0x00))
		{
			break;
		}
	}
	
	
}

void led_init() //Initializes the LED board, resets all values.
{
	DDRA = 0b11111111;
	PORTA = 0b00000000;
	PORTA = 0b00010001;
	PORTA = 0b01010101;
	PORTA = 0b00010001;
	
	_delay_us(1000);
}

void led_color(int color, int data) //0-off, 1-blue, 2-green, 3- cyan, 4-purple, 5-red, 6-yellow, 7-white.
{
	switch (color) {
		
		case 0: PORTA = OFF[data];
		break;
		case 1: PORTA = BLUE[data];
		break;
		case 2: PORTA = GREEN[data];
		break;
		case 3: PORTA = CYAN[data];
		break;
		case 4: PORTA = PURPLE[data];
		break;
		case 5: PORTA = RED[data];
		break;
		case 6: PORTA = YELLOW[data];
		break;
		case 7: PORTA = WHITE[data];
		break;
		default: PORTA = OFF[data];
		break;
	}
}

void row_color_load(int row)//Loads the color values of the next row from the program array.
{
	int j = 9;
	
	for(int i = 0; i < 10; i++)
	{
		if(j < 0)
		break;
		row_color_vector[i] = color_program[row][j];
		j--;
	}
}

void matrix_refresh()
{
	uint8_t first = 1;
	
	for(int i = 0; i < column_length+1; i++)
	{
		PORTA = 0b00000001;//Reset all shift register values
		PORTA = 0b01010001;
		PORTA = 0b00010001;
		
		row_color_load(i); //Load the row_color_vector with new color values for the row
		
		if(first == 1)//load the col value to the shift register
		{
			//Load 1 to Column
			PORTA = 0b00011001;
			PORTA = 0b00011011;
			PORTA = 0b00010001;
			PORTA = 0b00010101;
			PORTA = 0b00010001;
			
			
			first = 0;
		}
		else
		{
			//Load 0 to Column
			PORTA = 0b00010001;
			PORTA = 0b00010011;
			PORTA = 0b00010001;
			PORTA = 0b00010101;
			PORTA = 0b00010001;
			
			//first++;
		}
		
		for(int j = 0; j < row_length; j++) //Load the row_color_vector to the shift registers
		{
			for(int k = 0; k < 7; k++)
			{
				led_color(row_color_vector[j],k);
			}
		}
		
		//Move row values to the storage register
		PORTA = 0b01010001;
		PORTA = 0b00010001;
		
		_delay_us(50);
		
	}
}

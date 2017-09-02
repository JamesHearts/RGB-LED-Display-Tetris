/*
 * LED_DRIVER.c
 *
 * Created: 2/8/2017 2:28:32 PM
 * Author : James
 * Inspired By: Javilops Tetris Guide.
 */ 

#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <avr/interrupt.h>
#include <math.h>
#include "game_pieces.h"
#include "led_driver.h"
#include "board.h"
#include "lcd_driver.h"
//#include "time.h"
#include <stdlib.h>
 
 bool gameOver = false;
 int mPosRow, mPosCol;           // Position of the piece that is falling down
 int mPiece, mRotation;          // Kind and rotation the piece that is falling down 
 int randomColor;
 int firstColor = true;
 uint8_t player = 0;
 bool first = true; //This variable controls the creation of the first game piece.
 char stringBuffer[];

 uint8_t GetRand(int pA, int pB);
 void InitGame();
 void drawBoard();
 void drawPiece();
 void gameInit();
 void pieceFall();
 void CreatePiece();
 
 uint8_t colorOrder[7] = {1,2,3,4,5,6,7}; //Holds a predetermined color pattern. Until rand() works.
 int pieceOrder[7] = {0,1,2,3,4,5,6}; //HOlds a predetermined piece pattern. Until rand() works.
 int rotationOrder[4] = {0,1,2,3}; //Holds a predetermined rotation order. Until rand() works.

uint8_t colorOrderCount = 1;
uint8_t pieceOrderCount = 0;
uint8_t rotationOrderCount = 0;
	  
// GetRand returns a random number bounded between pA and pB.
uint8_t GetRand (int pA, int pB)
{
	return rand() % (pB - pA + 1) + pA;
}

// InitGame starts the game, and generates a random first piece
void CreatePiece()
{
	// Initialize random numbers
	srand((unsigned int) time(NULL));
	
	// Create a piece
	mPiece          = pieceOrder[pieceOrderCount];
	mRotation       = rotationOrder[rotationOrderCount];
	randomColor     = colorOrder[colorOrderCount];
	
	pieceOrderCount++;
	rotationOrderCount++;
	colorOrderCount++;
	
	if(pieceOrderCount == 7)
	{
		pieceOrderCount = 0;
	}
	
	if(rotationOrderCount == 4)
	{
		rotationOrderCount = 0;
	}
	
	if(colorOrderCount == 6)
	{
		colorOrderCount = 0;
	}
	//mPiece          = GetRand(0, 6);
	//mRotation       = GetRand(0, 3);
	mPosRow         = GetRowInitialPosition(mPiece, mRotation);
	mPosCol         = (BOARD_WIDTH / 2) + GetColInitialPosition(mPiece, mRotation);	
}

void pieceMovement()
{
	//----------------------------------------------------------//
	// mKey: Bit 0 = UP, Bit 1 = LEFT, Bit 2 = RIGHT, Bit 3 = DOWN, BIT 4 = A, Bit 5 = B.
	// Low - True buttons.
	// For this particular game, Bit 7 = no function, Bit 6 = no function, Bit 0 = no function, B = ROTATE.
	uint8_t mKey = PIND; //Button signals come from PortB.

	switch (mKey)
	{
		case (0b00111011): //RIGHT
		{
			if(IsPossibleMovement(mPosRow, mPosCol+1, mPiece, mRotation))
			mPosCol++;
			drawPiece();
			
			
		}
		break;
		case (0b00111101): //LEFT
		{
			if(IsPossibleMovement(mPosRow, mPosCol-1, mPiece, mRotation))
			{
				mPosCol--;
				drawPiece();
			}
			
		}
		break;
		case (0b00110111): //DOWN
		{
			if(IsPossibleMovement(mPosRow+1, mPosCol, mPiece, mRotation))
			{
				mPosRow++;
				drawPiece();
			}
			
		}
		break;
		case (0b00011111): //ROTATE
		{
			if (IsPossibleMovement(mPosRow, mPosCol, mPiece, (mRotation + 1) % 4))
			{
				mRotation = (mRotation + 1) % 4;
				drawPiece();
			}
			
			
		}
		break;
		default : 
		{
			while(1){
				if(PIND == 0x3F)
				{
					break;
				}
			}
		}
	}
}

// pieceFall makes the piece move down a block and checks for the game to end.
// This function is the main loop. Returns game over flag.
void pieceFall()
{
		
	   pieceMovement();
	   _delay_ms(2);	
		
		if (IsPossibleMovement(mPosRow+1, mPosCol, mPiece, mRotation))
		{
			mPosRow++;
			
			drawPiece();
		}
		else
		{
			StorePiece(mPosRow, mPosCol, mPiece, mRotation);
			drawBoard();
			for(int z = 0; z < 15; z++)
			{
				matrix_refresh();
			}
			//drawBoard();
			DeletePossibleLines();
			
			drawBoard();
			for(int z = 0; z < 15; z++)
			{
				matrix_refresh();
			}
			
			if(((score != scoreTemp) || (score == 0)) && (firstScore == true))
			{
				firstScore = false;
				ultoa(score, stringBuffer, 16);
				if(player == 1)
				{
					lcd_command(0x01);
					lcd_string("Player One Score");
					lcd_command(0xC0);
					lcd_string(stringBuffer);
				}
				if(player == 2)
				{
					lcd_command(0x01);
					lcd_string("Player Two Score");
					lcd_command(0xC0);
					lcd_string(stringBuffer);
				}
			}
			    
			if(IsGameOver())
			{
				lcd_init();
				lcd_command(0x01);
				lcd_string("Push A : Quit");
				lcd_command(0xC0);
				lcd_string("Push B : Restart");
				_delay_ms(1000);
				
				while(1)
				{
					if((PIND & 0x10) == 0x00) //if A
					{
					    first = true;
						player = 0;
						mainLoop();
					}
					if((PIND & 0x20) == 0x00) //if B
					{
						led_init();
						colorBoardInit();
						matrix_refresh();
						InitBoard();
						first = true;
						startGame();
					}
						
				}
				
			}
			
			drawBoard();
			for(int z = 0; z < 15; z++)
			{
				matrix_refresh();
			}
			
			
			
			CreatePiece();
		}
		
}

void drawPiece()
{
	colorBoardSave();
	testBoardSave();
	
	StorePiece(mPosRow, mPosCol, mPiece, mRotation);
	drawBoard();
	for(int z = 0; z < 15; z++)
	{
		matrix_refresh();
	}
	colorBoardRestore();
	testBoardRestore();
	
}

// drawBoard takes the testBoard matrix and turns it into the game_program matrix (colors)
void drawBoard()
{
	for(int i = 0; i < BOARD_HEIGHT; i++)
	{
		for(int j = 0; j < BOARD_WIDTH; j++)
		{
			if(testBoard[i][j] == POS_FILLED)
			{
				if(color_program[i][j] == 0)
				{
					color_program[i][j] = randomColor;
				}
			}
			else
			{
				color_program[i][j] = 0;
			}
			
		}
	}
}

void startGame()
{
	while(1) //Main game Loop
	{
		if(first == true) CreatePiece(); first = false;
		pieceFall();
	}
}

void gameInit()
{
	led_init();
	colorBoardInit();
	matrix_refresh();
	InitBoard();
	lcd_init();
	
	_delay_ms(1000);
	
	DDRD = 0x00; //Set Port D to Input.
	DDRC = 0xFF; //Set Port C to Output.
	PORTC = 0x00; //Set Port C to OFF by default.
	
	lcd_init();
	lcd_command(0x01);
	lcd_string("Waiting For");
	lcd_command(0xC0);
	lcd_string("Controller...");
	
	while(1){ //Stay in this while loop until the controller is detected.
		if(PIND == 0x3F)
		{
			break;
		}
	}
	
	PORTC = 0x02; //Controller detect, start music.
	
	lcd_init();
	lcd_command(0x01);
	lcd_string("PB&J's Tetris");
	lcd_command(0xC0);
	lcd_string("RGB Game");
	_delay_ms(1000);
	//intr_init();
}

void mainLoop()
{
	
	led_init();
	colorBoardInit();
	matrix_refresh();
	InitBoard();
	
	while(1)
	{
		lcd_init();
		lcd_command(0x01);
		lcd_string("Waiting on ");
		lcd_command(0xC0);
		lcd_string("Player Card...");
		_delay_ms(1000);
		
		while(1)
		{
			if(((PINB & 0x30) == 0x10) || ((PINB & 0x30) == 0x20)) break;
		}
		
		if((PINB & 0x30) == 0x10)
		{
			lcd_init();
			lcd_command(0x01);
			lcd_string("Player One ");
			lcd_command(0xC0);
			lcd_string("Push A to Start ");
			player = 1;
			_delay_ms(1000);
		}
		else
		{
			lcd_init();
			lcd_command(0x01);
			lcd_string("Player Two ");
			lcd_command(0xC0);
			lcd_string("Push A to Start ");
			_delay_ms(1000);
			player = 2;
		}
			
		colorBoardTest();
		
		_delay_ms(2000);
		
	
			if(player == 1)
			{
				lcd_init();
				lcd_command(0x01);
				lcd_string("Player One Score");
				lcd_command(0xC0);
				lcd_string("0");
				_delay_ms(1000);
			}
			if(player == 2)
			{
				lcd_init();
				lcd_command(0x01);
				lcd_string("Player Two Score");
				lcd_command(0xC0);
				lcd_string("0");
				_delay_ms(1000);
			}
				
		led_init();
		colorBoardInit();
		matrix_refresh();
		InitBoard();
		
		startGame();
		
	}
	
}

int main(void)
{
	led_init();
	colorBoardInit();
	matrix_refresh();
	
	colorBoardTest_2();
	//gameInit();	
	//mainLoop();
	return 0;
}




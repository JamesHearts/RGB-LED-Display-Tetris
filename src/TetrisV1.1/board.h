#define BOARD_HEIGHT 20
#define BOARD_WIDTH 10
#define PIECE_BLOCKS 5

typedef int bool;
enum { false, true };
enum { POS_FREE, POS_FILLED }; // POS_FREE = free position of the board; POS_FILLED = filled position of the board

uint8_t testBoard [BOARD_HEIGHT][BOARD_WIDTH]; // Board that contains the pieces, testBoard will be copied to the displayBoard each refresh cycle.
uint8_t testBoardTemp [BOARD_HEIGHT][BOARD_WIDTH]; // Board that contains the pieces, testBoard will be copied to the displayBoard each refresh cycle.
int  score = 0;
int  scoreTemp = 1;
bool firstScore = true;

bool IsFreeBlock            (int pRow, int pCol);
bool IsPossibleMovement     (int pRow, int pCol, int pPiece, int pRotation);
void StorePiece             (int pRow, int pCol, int pPiece, int pRotation);
void testBoardSave          ();
void testBoardRestore       ();
void DeletePossibleLines    ();
bool IsGameOver             ();
void InitBoard();
void DeleteLine (int pRow);

// InitBoard makes all the blocks on the board available. POS_FREE
void InitBoard()
{
	for (int i = 0; i < BOARD_HEIGHT; i++)
	{
		for (int j = 0; j < BOARD_WIDTH; j++)
		{
				testBoard[i][j] = POS_FREE;
				testBoardTemp[i][j] = POS_FREE;
		}
	}
}

// InitBoard makes all the blocks on the board available. POS_FREE
void testBoardSave()
{
	for (int i = 0; i < BOARD_HEIGHT; i++)
	{
		for (int j = 0; j < BOARD_WIDTH; j++)
		{
			testBoardTemp[i][j] = testBoard[i][j];
		}
	}
}

void testBoardRestore()
{
	for (int i = 0; i < BOARD_HEIGHT; i++)
	{
		for (int j = 0; j < BOARD_WIDTH; j++)
		{
			testBoard[i][j] = testBoardTemp[i][j];
		}
	}
}

// IsGameOver checks to see if the top most horizontal line is filled, meaning the game is over.
bool IsGameOver()
{
	//If the first line has blocks, then, game over
	for (int i = 0; i < BOARD_WIDTH; i++)
	{
		if(testBoard[0][i] == POS_FILLED)
		{
			 score = 0;
			 scoreTemp = 1; 
			 firstScore = true;
			 return true;
		}
	}
	
	return false;
}

// DeleteLine an iterative loop that deletes a single horizontal line.
void DeleteLine(int pRow)
{
	// Moves all the upper lines one row down
	for(int i = pRow; i > 0; i--)
	{
		for(int j = 0; j < BOARD_WIDTH; j++)
		{
			testBoard[i][j] = testBoard[i-1][j];
		}
	}

	scoreTemp = score;
	score++;
	firstScore = true;
	
}

// DeletePossibleLines deletes any horizontal lines that have been completely filled by blocks.
void DeletePossibleLines()
{
	for(int i = 0; i < BOARD_HEIGHT; i++)
	{
		int j = 0;
		while(j < BOARD_WIDTH)
		{
			if(testBoard[i][j] != POS_FILLED) break;
			j++;
		}
		
		if(j == BOARD_WIDTH) DeleteLine (i);
	}
}

// IsFreeBlock checks to see if a block is free.
bool IsFreeBlock (int pRow, int pCol)
{
	if (testBoard [pRow][pCol] == POS_FREE) return true; else return false;
}

// StorePiece stores the piece onto the board
void StorePiece (int pRow, int pCol, int pPiece, int pRotation)
{
	// Store each block of the piece into the board
	for (int i1 = pRow, i2 = 0; i1 < pRow + PIECE_BLOCKS; i1++, i2++) //i1 and i2 are the X and Y for the piece blocks.
	{
		for (int j1 = pCol, j2 = 0; j1 < pCol + PIECE_BLOCKS; j1++, j2++)
		{
			// Store only the blocks of the piece that are not holes
			if (GetBlockType (pPiece, pRotation, i2, j2) != 0)
			testBoard[i1][j1] = POS_FILLED;
		}
	}
}

// IsPossibleMovement checks to see whether or not the game piece has a collision with the limits of the board or another piece.
bool IsPossibleMovement(int pRow, int pCol, int pPiece, int pRotation)
{
	// Checks collision with pieces already stored in the board or the board limits
	// This is just to check the 5x5 blocks of a piece with the appropriate area in the board
	for(int i1 = pRow, i2 = 0; i1 < pRow + PIECE_BLOCKS; i1++, i2++)
	{
		for(int j1 = pCol, j2 = 0; j1 < pCol + PIECE_BLOCKS; j1++, j2++)
		{
			// Check if the piece is outside the limits of the board
			if(j1 < 0           || j1 > BOARD_WIDTH  - 1    || i1 > BOARD_HEIGHT - 1)
			{
				if(GetBlockType(pPiece, pRotation, i2, j2) != 0)
				return false;
			}
			
			// Check if the piece have a collision with a block already stored on the board.
			if(i1 >= 0)
			{
				if((GetBlockType(pPiece, pRotation, i2, j2) != 0) && (!IsFreeBlock (i1, j1)))
				return false;
			}
		}
	}
	
	// No collision
	return true;
}


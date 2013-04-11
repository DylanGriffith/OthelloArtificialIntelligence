#include "stdafx.h"
#include "Board.h"

// Flips all the pieces when a token of type currentPlayer is inserted into
// square (x,y).
void makeFlips(int x, int y, int insertedColour, Board *board);

// RETURN: WHITE if player is BLACK and BLACK otherwise
inline int otherColour(int colour);

Board::Board(void)
{
	for(int i=0;i<GRIDSIZE;i++)
	{
		for(int j=0;j<GRIDSIZE;j++)
		{
			boardArray[i*GRIDSIZE+j] = EMPTY;
		}
	}
}

Board::Board(Board *board, int x, int y, int player)
{
	for(int i = 0; i < GRIDSIZE*GRIDSIZE; i++)
	{
		this->boardArray[i] = board->boardArray[i];
	}
	this->insertToken(x,y,player);
}

Board::~Board(void)
{
}

void Board::insertToken(int x, int y, int colour)
{
	boardArray[x + y*GRIDSIZE] = colour;
	makeFlips(x,y,colour,this);
}

void Board:: changeToken(int x, int y, int colour)
{
	boardArray[x + y*GRIDSIZE] = colour;
}

int Board::getSquare(int x, int y)
{
	return boardArray[x + y*GRIDSIZE];
}


void makeFlips(int x, int y, int insertedColour, Board *board)
{
	if(x+2<GRIDSIZE)
	{
		if(board->getSquare(x+2,y) == insertedColour && board->getSquare(x+1,y) == otherColour(insertedColour))
			board->changeToken(x+1,y,insertedColour);
		if(y+2<GRIDSIZE)
		{
			if(board->getSquare(x+2,y+2) == insertedColour && board->getSquare(x+1,y+1) == otherColour(insertedColour))
				board->changeToken(x+1,y+1,insertedColour);
		}
		if(y-2>=0)
		{
			if(board->getSquare(x+2,y-2) == insertedColour && board->getSquare(x+1,y-1) == otherColour(insertedColour))
				board->changeToken(x+1,y-1,insertedColour);
		}
	}
	if(x-2>=0)
	{
		if(board->getSquare(x-2,y) == insertedColour && board->getSquare(x-1,y) == otherColour(insertedColour))
			board->changeToken(x-1,y,insertedColour);
		if(y+2<GRIDSIZE)
		{
			if(board->getSquare(x-2,y+2) == insertedColour && board->getSquare(x-1,y+1) == otherColour(insertedColour))
				board->changeToken(x-1,y+1,insertedColour);
		}
		if(y-2>=0)
		{
			if(board->getSquare(x-2,y-2) == insertedColour && board->getSquare(x-1,y-1) == otherColour(insertedColour))
				board->changeToken(x-1,y-1,insertedColour);
		}
	}
	if(y+2<GRIDSIZE)
	{
		if(board->getSquare(x,y+2) == insertedColour && board->getSquare(x,y+1) == otherColour(insertedColour))
			board->changeToken(x,y+1,insertedColour);
	}
	if(y-2>=0)
	{
		if(board->getSquare(x,y-2) == insertedColour && board->getSquare(x,y-1) == otherColour(insertedColour))
			board->changeToken(x,y-1,insertedColour);
	}
}

inline int otherColour(int colour)
{
	return (colour == BLACK) ? WHITE : BLACK;
}
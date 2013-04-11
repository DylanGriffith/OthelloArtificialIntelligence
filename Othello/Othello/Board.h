#pragma once
#include "stdafx.h"

#define EMPTY 0
#define BLACK 1
#define WHITE -1
#define GRIDSIZE 8

class Board
{
private:
	int boardArray[GRIDSIZE*GRIDSIZE];
public:

	// Constructs a board with all cells initialised to EMPTY
	Board(void);

	// FUNCTION: Used to create a new board with one token added.
	//
	// RETURN: A new board with all squares the same as board
	// but square (x,y) set to player
	Board(Board *board, int x, int y, int player);

	~Board(void);

	// Gets the value at square (x,y) in the board
	int getSquare(int x, int y);

	// Inserts a token of colour 'colour' into the board at square (x,y)
	// and also makes the flips that occur in othello game after such
	// an insert
	void insertToken(int x, int y, int colour);

	// Same as insertToken except it makes no flips.
	void changeToken(int x, int y, int colour);

	void changeBoard(Board *board, int x, int y, int player);
};
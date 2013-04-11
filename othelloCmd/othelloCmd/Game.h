#pragma once
#include "Board.h"
#include "othelloAI.h"

#define AI_STRATEGY_A 777
#define AI_STRATEGY_B 778
#define AI_STRATEGY_C 779
#define AI_STRATEGY_D 780
#define AI_STRATEGY_E 781


class Game
{
	Board *board;
	int currentPlayer;
	OthelloAI *ai;
	bool canCurrentPlayerMove();

public:
	Game(void);
	~Game(void);

	//
	// FUNCTION: getBoard()
	//
	// PURPOSE: returns a copy of the Board for this game
	Board getBoard();

	// FUNCTION getCurrentPlayer()
	//
	// RETURN: the next player to make a move in the game. 
	// NOTE: currentPlayer is updated immediately if a player's
	// turn must be skipped. Thus this will not return the a
	// player that cannot move (except if a game is finished).
	int getCurrentPlayer();

	//
	// FUNCTION: isLegalMove(int x, int y)
	//
	// PURPOSE: Determines if a move in the (x,y) square is legal for the current player.
	bool isLegalMove(int x, int y);

	//
	// FUNCTION: isLegalMove(int x, int y)
	//
	// PURPOSE: Determines if a move in the (x,y) square is legal for the current player in Board someBoard.
	static bool isLegalMove(int x, int y, Board *someBoard, int playerToMove);

	//
	// FUNCTION: isLegalMove(int x, int y)
	//
	// PURPOSE: Determines if a move in the (x,y) square is legal 
	// for the current player in the current board of game.
	bool isLegalMove(int x, int y, int player);

	//
	// FUNCTION: makeMove(int x, int y)
	//
	// PURPOSE: makes a move in square (x,y) if it is legal and it is the users turn. This function does nothing otherwise.
	void makeMove(int x, int y);

	//
	// FUNCTION: makeMoveAI()
	//
	// PURPOSE: Causes the AI to make a move in the current game
	void makeMoveAI(int strategy);

	// FUNCTION: Used to determine if a game has ended
	// RETURN: true if the game is at a terminal stage and false otherwise
	bool isOverGame();

	// FUNCTION: isOverGame(Board *someBoard)
	// PURPOSE: Used to determine if a board 'someBoard' is terminal (ie.
	// nobody can move)
	// RETURN: true if someBoard is at a terminal stage and false otherwise
	static bool isTerminal(Board *someBoard);

	// FUNCTION: Used to determine the winner of a game at its terminal stage
	int whoWins();
};


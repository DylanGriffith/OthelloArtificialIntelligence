#pragma once
#include "stdafx.h"
#include "Board.h"


class StateMove
{
public:
	StateMove(void);

	// Constructs a StateMove by explicitly specifying all its field values
	StateMove(unsigned long long firstHalf, unsigned long long secondHalf, int playerToMove, int moveMade);

	// Constructs a StateMove with its properties defined by the parameters
	StateMove(Board *board, int movingPlayer, int moveTo);

	~StateMove(void);

	unsigned long long stateFirstHalf;
	unsigned long long stateSecondHalf;
	int player;
	// A move (in the format 'x+y*GRIDSIZE')
	int move;

	// RETURN: true iff this state and move is equal to other state and move
	bool isEqual(StateMove *other);

	// RETURN: true iff this state is equal to the state defined by
	// board and movingPlayer
	bool isEqualState(Board *board, int movingPlayer);
};
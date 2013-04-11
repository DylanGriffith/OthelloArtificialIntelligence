#include "StateMove.h"
#include "Board.h"

// RETURN: x to the power i
inline unsigned long long power(int x, int i)
{
	unsigned long long answer = 1;
	for(int counter=0;counter<i;counter++) answer *= x;
	return answer;
}

inline unsigned long long value(int square);

StateMove::StateMove(Board *board, int movingPlayer, int moveTo)
{
	stateFirstHalf = 0;
	stateSecondHalf = 0;
	for(unsigned long long i=0; i<32 ;i++)
	{
		stateFirstHalf += power(3,i)*value(board->getSquare(i%GRIDSIZE,i/GRIDSIZE));
	}
	for(unsigned long long i=32; i<64 ;i++)
	{
		stateSecondHalf += power(3,i)*value(board->getSquare(i%GRIDSIZE,i/GRIDSIZE));
	}
	player = movingPlayer;
	move = moveTo;
}

StateMove::StateMove(unsigned long long firstHalf, unsigned long long secondHalf, int playerToMove, int moveMade)
{
	stateFirstHalf = firstHalf;
	stateSecondHalf = secondHalf;
	player = playerToMove;
	move = moveMade;
}

StateMove::StateMove(void)
{

}


StateMove::~StateMove(void)
{
}


bool StateMove::isEqual(StateMove *other)
{
	return (this->stateFirstHalf == other->stateFirstHalf && this->stateSecondHalf == other->stateSecondHalf&& this->move == other->move);
}

bool StateMove::isEqualState(Board *board, int movingPlayer)
{
	unsigned long long newStateFirstHalf, newStateSecondHalf;

	newStateFirstHalf = 0;
	newStateSecondHalf = 0;

	for(unsigned long long i=0; i<32 ;i++)
	{
		newStateFirstHalf += power(3,i)*value(board->getSquare(i%GRIDSIZE,i/GRIDSIZE));
	}
	for(unsigned long long i=32; i<64 ;i++)
	{
		newStateSecondHalf += power(3,i)*value(board->getSquare(i%GRIDSIZE,i/GRIDSIZE));
	}
	return (newStateFirstHalf == stateFirstHalf && newStateSecondHalf == stateSecondHalf && movingPlayer == player);
}

inline unsigned long long value(int square)
{
	if(square == WHITE)
		return 2;
	if(square == BLACK)
		return 1;
	return 0;
}

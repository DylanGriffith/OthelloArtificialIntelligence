#include "stdafx.h"
#include "State.h"

// RETURN: x to the power i
inline unsigned long long power(int x, int i)
{
	unsigned long long answer = 1;
	for(int counter=0;counter<i;counter++) answer *= x;
	return answer;
}

// RETURN: Unique modulo 3 value of square
inline unsigned long long value(int square);

State::State(void)
{
}


State::~State(void)
{
}

bool State::isEqual(Board *board, int player)
{
	unsigned long long first = 0;
	unsigned long long second = 0;
	for(unsigned long long i=0; i<32 ;i++)
	{
		first += power(3,i)*value(board->getSquare(i%GRIDSIZE,i/GRIDSIZE));
	}
	for(unsigned long long i=32; i<64 ;i++)
	{
		second += power(3,i)*value(board->getSquare(i%GRIDSIZE,i/GRIDSIZE));
	}
	return (this->firstHalf == first && this->secondHalf == second && this->movingPlayer == player);
}

bool State::isEqual(State *other)
{
	return (this->firstHalf == other->firstHalf && this->secondHalf == other->secondHalf && this->movingPlayer == other->movingPlayer);
}

State::State(unsigned long long first, unsigned long long second, unsigned long long player)
{
	firstHalf = first;
	secondHalf = second;
	movingPlayer = player;
	hash = getHash();
}

State::State(Board *board, int player)
{
	firstHalf = 0;
	secondHalf = 0;
	for(unsigned long long i=0; i<32 ;i++)
	{
		firstHalf += power(3,i)*value(board->getSquare(i%GRIDSIZE,i/GRIDSIZE));
	}
	for(unsigned long long i=32; i<64 ;i++)
	{
		secondHalf += power(3,i)*value(board->getSquare(i%GRIDSIZE,i/GRIDSIZE));
	}
	movingPlayer = player;
	hash = getHash();
}

unsigned long long State::getHash() const
{
	return firstHalf*secondHalf + movingPlayer;
}

inline unsigned long long value(int square)
{
	if(square == WHITE)
		return 2;
	if(square == BLACK)
		return 1;
	return 0;
}

// Overwrite comparison operator 'less' for two states
bool operator<(State a, State b)
{
	return a.getHash() < b.getHash();
}

bool operator==(const State a, const State b)
{
   return (a.firstHalf == b.firstHalf && a.secondHalf == b.secondHalf && a.movingPlayer == b.movingPlayer);
}

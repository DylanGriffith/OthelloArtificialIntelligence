#pragma once
#include "Board.h"


class State
{
	friend bool operator<(State a, State b);
	friend bool operator==(State a, State b);
public:
	State(void);

	// RETURN: true if these states are equal and false otherwise
	bool isEqual(State *other);

	// RETURN: true if the state is the same as that defined by
	// the parameters
	bool isEqual(Board *board, int player);

	// Construct a state defined by the parameters
	State(Board *board, int player);

	// Construct a state explicitly from the field values
	State(unsigned long long first, unsigned long long second, unsigned long long player);

	~State(void);

	// RETURN: The hash code for the state
	unsigned long long getHash() const;

	unsigned long long hash;
	unsigned long long firstHalf;
	unsigned long long movingPlayer;
	unsigned long long secondHalf;
};

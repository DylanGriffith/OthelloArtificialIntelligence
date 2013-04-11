#pragma once
#include "stdafx.h"
#include "Board.h"
#include "State.h"
#include <vector>
#include <hash_set>
#include "StateMove.h"
#include <exception>
#include <map>
#include <queue>

using std::vector; using std::exception; using std::multimap; using std::queue;using namespace std;
class OthelloAI
{
private:
	int me;


	multimap<State, int> book;

	//hash_set<StateMove> book;

	// Returns the first legal move found
	int firstLegalMove(Board *board);

	// FUNCTION: Determines a minimax score
	//
	// RETURN: The score for the node board using minimax algorithm to a depth of depth
	int minimax(Board *board, int depth, int player);

	// FUNCTION: Uses minimax search method to find a move
	//
	// RETURN: The move decided by minimax search algorithm
	// to be the best (in the format (x+y*GRIDSIZE))
	int minimaxSearch(Board *board);

	// FUNCTION: Determines a negamax score
	//
	// RETURN: The score for the node board using negamax algorithm to a depth of depth
	int negamax(Board *board, int depth, int playerToMove);

	// FUNCTION: Uses negamax search method to find a move
	//
	// RETURN: The move decided by negamax search algorithm
	// to be the best (in the format (x+y*GRIDSIZE))
	int negamaxSearch(Board *board);

	// PURPOSE: Uses minimax search with alpha beta pruning
	// method and scouting optimization to find a best move
	//
	// RETURN: The move decided by minimax search algorithm
	// to be the best (in the format 'x+y*GRIDSIZE')
	int negascoutSearch(Board *board);

	// FUNCTION: score(Board *board, int depth, int alpha, int beta, int player)
	//
	// PURPOSE: Determines a minimax score using alpha beta pruning and scouting
	//
	// RETURN: The score for the node board using minimax algorithm to a depth of depth
	int negascout(Board *board, int depth, int alpha, int beta, int player);

	// PURPOSE: Uses minimax search with alpha beta pruning
	// method to find a move
	//
	// RETURN: The move decided by minimax search algorithm
	// to be the best (in the format 'x+y*GRIDSIZE')
	int alphabetaSearch(Board *board);

	// PURPOSE: ONLY USE FOR LEARNING. Uses minimax search 
	// with alpha beta pruning and scouting to find a move with a
	// very deep search tree.
	//
	// RETURN: The move decided by minimax search algorithm
	// to be the best (in the format 'x+y*GRIDSIZE')
	int deepSearch(Board *board);

	// FUNCTION: alphabeta(Board *board, int depth, int alpha, int beta, int player)
	//
	// PURPOSE: Determines a minimax score using alpha beta pruning
	//
	// RETURN: The score for the node board using minimax algorithm to a depth of depth
	int alphabeta(Board *board, int depth, int alpha, int beta, int player);



	// PURPOSE: Opens the move book and stores in 'book' variable
	// to be read in the findInMoveBook(Board *board, int playerToMove) function
	void openBook();

	// FUNCTION: findMoveInBook(Board *board, int playerToMove)
	// PURPOSE: Used to find the move to to make in the current state
	// defined by board and playerToMove. 
	// RETURN: The move to make as defined in the book
	// THROWS: stateNotInBookException
	int findMoveInBook(Board *board, int playerToMove);


public:

	// Sets the 'me' field of the OthelloAI to player
	void setPlayer(int player);

	// Default AI with me set to BLACK
	OthelloAI(void);
	// AI with me to player
	OthelloAI(int player);
	~OthelloAI(void);

	// Used to find the move chosen by the AI for the currentBoard
	// using the strategy 'strategy'
	//
	// RETURN: A move in the format (x+y*GRIDSIZE)
	int getMove(Board *currentBoard, int strategy);

};
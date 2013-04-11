#include "stdafx.h"
#include "OthelloAI.h"
#include "Board.h"
#include "Game.h"
#include "StateMove.h"
#include <time.h>
#include <queue>

#include <stdlib.h>

using namespace std;

#define INFINITY 999999
#define MINIMAX_DEPTH 4
#define ALPHABETA_DEPTH 5
#define DEEP_DEPTH 11
#define SCOUT_DEPTH 8

#define BOOK "book_moves.dat"

// RETURN: WHITE if player is BLACK and BLACK otherwise
inline int otherPlayer(int player);

// FUNCTION: scoreBoardAdvanced(Board *board, player)
// PURPOSE: An advanced heuristic function for scoring
// a Board board from the perspective of player.
int scoreBoardAdvanced(Board *board, int player);

// PURPOSE: Used as a simple heuristic scoring function
//
// RETURN: Sum over all tokens beloning to 'me' minus the sum over all tokens belonging to other player
int scoreBoardSimple(Board *board, int player);

// FUNCTION: randomLegalMove(Board *board)
// RETURN: a random legal move in the Board
// referenced by 'board' for player 'me'
int randomLegalMove(Board *board, int me);


// FUNCTION: appendToBook(StateMove sm)
// PURPOSE: Saves the state move sm in the book at the end
void appendToBook(StateMove sm);

// RETURN: number of possible moves for player 'me' in
// the Board 'board'
int possibleMoves(Board *board, int me);

class stateNotInBookException: public exception
{
	virtual const char* what() const throw()
	{
		return "State not in book";
	}
} StateNotInBookException;

// BoardOrder class is used only for 
// expanding children in good order in 
// negascout algorithm
class BoardOrder
{
	friend bool operator<(BoardOrder a, BoardOrder b);
public:
	~BoardOrder(void){}
	BoardOrder(void){}
	BoardOrder(int priority, Board board)
	{
		this->priority = priority;
		this->board = board;
	}
	int priority;
	Board board;
};

// Redefine less than operator for use in
// priority queue
bool operator <(BoardOrder a, BoardOrder b)
{
	return a.priority < b.priority;
}

OthelloAI::OthelloAI(void)
{
	openBook();
	me = BLACK;
}

OthelloAI::OthelloAI(int player)
{
	openBook();
	me = player;
}

void OthelloAI::setPlayer(int player)
{
	me = player;
}

OthelloAI::~OthelloAI(void)
{
	
}

int OthelloAI::getMove(Board *currentBoard, int strategy)
{
	int move = 0;
	switch(strategy)
	{
	case AI_STRATEGY_A:
		move = minimaxSearch(currentBoard);
	case AI_STRATEGY_B:
		move = alphabetaSearch(currentBoard);
		break;
	case AI_STRATEGY_C:
		try // IF: current state is in book
		{
			move = findMoveInBook(currentBoard,me);
		}catch (stateNotInBookException e) // ELSE:
		{
			move = negascoutSearch(currentBoard);
		}
		break;
	case AI_STRATEGY_D: // Random moving player
		move =  randomLegalMove(currentBoard,me);
		break;
	case AI_STRATEGY_E: // Learning phase
		try // IF: current state is in book
		{
			move = findMoveInBook(currentBoard,me);
		}catch (stateNotInBookException e) // ELSE:
		{
			// Make a move using minimax with deep alphabeta search
			move = deepSearch(currentBoard);
			// Record that move in book
			appendToBook(StateMove(currentBoard, me, move));
		}
		break;
	}
	return move;
}

int OthelloAI::negascoutSearch(Board *board)
{
	int bestMoveScore = -2*INFINITY;
	int bestMove;
	int score;

	for (int move=0; move < GRIDSIZE*GRIDSIZE; move++)
	{
		if(Game::isLegalMove(move%GRIDSIZE,move/GRIDSIZE,board,me))
		{
			Board moveBoard (board, move%GRIDSIZE, move/GRIDSIZE, me);
			score = -negascout(&moveBoard, SCOUT_DEPTH, -2*INFINITY, -bestMoveScore, otherPlayer(me));
			if(score > bestMoveScore)
			{
				bestMove = move;
				bestMoveScore = score;
			}
		}
	}
	return bestMove;
}

int OthelloAI::negascout(Board *board, int depth, int alpha, int beta, int player)
{
	if (depth <= 0 || Game::isTerminal(board))
	{
		int score = scoreBoardAdvanced(board,player);
		return score;
	}
	priority_queue<BoardOrder> children;
	bool cantMove = true;

	// Queue children of state
	for(int i=0;i<GRIDSIZE*GRIDSIZE;i++)
	{
		int x = i%GRIDSIZE;
		int y = i/GRIDSIZE;
		if(Game::isLegalMove(x,y,board,player))
		{
			cantMove = false;
			Board child (board,x,y,player);
			// Prioritise depending on whether it is min turn or
			// max turn
			int priority;
			if(player == me)
				priority = scoreBoardSimple(&child,me);
			else
				priority = -scoreBoardSimple(&child,me);
			children.push(BoardOrder (priority, child));
		}
	}
	if(cantMove)
	{
		// Child is same node
		children.push(BoardOrder (0, *board));
		depth++; // Increment depth for consistency with other algorithms
	}

	bool firstChild = true;
	int b = beta; // scouting variable
	while(children.size() > 0)
	{
		Board child = (children.top().board);
		children.pop();
		int score = -negascout(&child, depth-1, -b, -alpha, otherPlayer(player));
		if(alpha < score && score < beta && !firstChild) // Perform full alpha-beta search
			score = -negascout(&child, depth - 1, -beta, -alpha, otherPlayer(player));
		firstChild = false;
		alpha = max(alpha, score);
		if(beta <= alpha)
			return alpha;
		b = alpha + 1;
	}

	return alpha;
}

int OthelloAI::minimaxSearch(Board *board)
{
	int bestMoveScore = -2*INFINITY;
	int bestMove;
	int score;
	for (int move=0; move < GRIDSIZE*GRIDSIZE; move++)
	{
		if(Game::isLegalMove(move%GRIDSIZE,move/GRIDSIZE,board,me))
		{
			Board moveBoard (board, move%GRIDSIZE, move/GRIDSIZE, me);
			score = minimax(&moveBoard, MINIMAX_DEPTH, otherPlayer(me));
			if(score > bestMoveScore)
			{
				bestMove = move;
				bestMoveScore = score;
			}
		}
	}
	return bestMove;
}

int OthelloAI::minimax(Board *board, int depth, int player)
{
	int score;
	if (depth <= 0 || Game::isTerminal(board))
		return scoreBoardAdvanced(board,me);
	bool cantMove = true;
	if(player == me)
	{
		score = -2*INFINITY;
		for(int i=0;i<GRIDSIZE;i++)
		{
			for(int j=0;j<GRIDSIZE;j++)
			{
				if(Game::isLegalMove(i,j,board,player))
				{
					Board child (board, i, j, player);
					cantMove = false;
					score = max(score, minimax(&child, depth-1, otherPlayer(player)));
				}		
			}
		}
		if(cantMove) // Child node is same node
			score = minimax(board, depth, otherPlayer(player));
	}else
	{
		score = 2*INFINITY;
		for(int i=0;i<GRIDSIZE;i++)
		{
			for(int j=0;j<GRIDSIZE;j++)
			{
				if(Game::isLegalMove(i,j,board,player))
				{
					Board child (board, i, j, player);
					cantMove = false;
					score = min(score, minimax(&child, depth-1, otherPlayer(player)));
				}
			}
		}
		if(cantMove) // Child node is same node
			score = minimax(board, depth, otherPlayer(player));
	}
	return score;
}

int OthelloAI::alphabetaSearch(Board *board)
{
	int bestMoveScore = -2*INFINITY;
	int bestMove;
	int alphabetaScore;
	for (int move=0; move < GRIDSIZE*GRIDSIZE; move++)
	{
		if(Game::isLegalMove(move%GRIDSIZE,move/GRIDSIZE,board,me))
		{
			Board moveBoard (board, move%GRIDSIZE, move/GRIDSIZE, me);
			alphabetaScore = alphabeta(&moveBoard, ALPHABETA_DEPTH, bestMoveScore, +INFINITY, otherPlayer(me));
			if(alphabetaScore > bestMoveScore)
			{
				bestMove = move;
				bestMoveScore = alphabetaScore;
			}
		}
	}
	return bestMove;
}

int OthelloAI::alphabeta(Board *board, int depth, int alpha, int beta, int player)
{
	if (depth <= 0 || Game::isTerminal(board))
		return scoreBoardAdvanced(board,me);
	bool cantMove = true;	
	if(player == me)
	{
		// Queue children of state
		for(int i=0;i<GRIDSIZE*GRIDSIZE;i++)
		{
			int x = i%GRIDSIZE;
			int y = i/GRIDSIZE;
			if(Game::isLegalMove(x,y,board,player))
			{
				cantMove = false;
				alpha = max(alpha, alphabeta(&Board(board,x,y,player), depth-1, alpha, beta, otherPlayer(player)));
				if(beta <= alpha)
					return alpha;
			}
		}
		if(cantMove) // Child node is same node
			alpha = max(alpha, alphabeta(board, depth, alpha, beta, otherPlayer(player)));
		return alpha;
	}else
	{
		// Queue children of state
		for(int i=0;i<GRIDSIZE*GRIDSIZE;i++)
		{
			int x = i%GRIDSIZE;
			int y = i/GRIDSIZE;
			if(Game::isLegalMove(x,y,board,player))
			{
				cantMove = false;
				beta = min(beta, alphabeta(&Board(board,x,y,player), depth-1, alpha, beta, otherPlayer(player)));
				if(beta <= alpha)
					return beta;
			}
		}
		if(cantMove) // Child node is same node
			beta = min(beta, alphabeta(board, depth, alpha, beta, otherPlayer(player)));
		return beta;
	}
}

int OthelloAI::negamaxSearch(Board *board)
{
	int bestMoveScore = -2*INFINITY;
	int bestMove;
	int score;
	for (int move=0; move < GRIDSIZE*GRIDSIZE; move++)
	{
		if(Game::isLegalMove(move%GRIDSIZE,move/GRIDSIZE,board,me))
		{
			Board moveBoard (board, move%GRIDSIZE, move/GRIDSIZE, me);
			score = negamax(&moveBoard, MINIMAX_DEPTH, otherPlayer(me));
			if(score > bestMoveScore)
			{
				bestMove = move;
				bestMoveScore = score;
			}
		}
	}
	return bestMove;
}

int OthelloAI::negamax(Board *board, int depth, int playerToMove)
{
	Board *child;
	if(depth <= 0 || Game::isTerminal(board))
		return scoreBoardAdvanced(board,otherPlayer(playerToMove));
	int alpha = INFINITY;
	bool cantMove = true;
	for(int i=0; i<GRIDSIZE;i++)
	{
		for(int j=0; j<GRIDSIZE;j++)
		{
			if(Game::isLegalMove(i,j,board,playerToMove))
			{
				Board child (board, i, j, playerToMove);
				cantMove = false;
				alpha = min(alpha, -negamax(&child, depth-1, otherPlayer(playerToMove)));
			}
		}
	}
	if(cantMove) // child is the same as board if you skip a turn
		alpha = min(alpha, -negamax(board, depth, otherPlayer(playerToMove)));
	return alpha;
}

int randomLegalMove(Board *board, int me)
{
	srand(time(NULL));
	while(true)
	{
		int move = rand()%64;
		if (Game::isLegalMove(move%GRIDSIZE, move/GRIDSIZE, board, me))
			return move;
	}
}

int OthelloAI::firstLegalMove(Board *board)
{
	for(int i=0;i<GRIDSIZE;i++)
		for(int j=0;j<GRIDSIZE;j++)
			if(Game::isLegalMove(i,j,board,me))
				return (i+j*GRIDSIZE);
}

int scoreBoardSimple(Board *board, int player)
{
	int score = 0;
	for(int i=0;i<GRIDSIZE;i++)
	{
		for(int j=0;j<GRIDSIZE;j++)
		{
			if(board->getSquare(i,j) == player)
				score++;
			else if(board->getSquare(i,j) == otherPlayer(player))
				score--;
		}

	}
	return score;
}

int scoreBoardAdvanced(Board *board, int player)
{
	int scoreSimple = scoreBoardSimple(board,player);

	if(Game::isTerminal(board))
	{
		// player wins
		if(scoreSimple > 0)
			return INFINITY;
		// player loses
		if(scoreSimple < 0)
			return -INFINITY;
		// match was a draw (draw is more desirable
		// than a loss but still less desirable than
		// any other state)
		else
			return -INFINITY/2;
	}

	// Add 10 points for every square owned by player and
	// subtract 10 for every square owned by other player
	int score = 10*scoreSimple;

	// Add 5 points for every legal move and subtract
	// 5 for every legal move of opponent
	for(int i=0;i<GRIDSIZE;i++)
	{
		for(int j=0;j<GRIDSIZE;j++)
		{

			if(Game::isLegalMove(i,j,board,player))
			{
				score += 5;
			}
			if(Game::isLegalMove(i,j,board,otherPlayer(player)))
			{
				score -= 5;
			}
		}
	}

	// Add 50 points for every corner owned by player
	// and subtract 50 points for every corner owned
	// by other player
	int cornerBonus = 0;
	int corner = board->getSquare(0,0);
	if(corner == player)
		cornerBonus += 50;
	else if(corner == otherPlayer(player))
		cornerBonus -= 50;
	corner = board->getSquare(0,7);
	if(corner == player)
		cornerBonus += 50;
	else if(corner == otherPlayer(player))
		cornerBonus -= 50;
	corner = board->getSquare(7,0);
	if(corner == player)
		cornerBonus += 50;
	else if(corner == otherPlayer(player))
		cornerBonus -= 50;
	corner = board->getSquare(7,7);
	if(corner == player)
		cornerBonus += 50;
	else if(corner == otherPlayer(player))
		cornerBonus -= 50;
	return score + cornerBonus;
}

inline int otherPlayer(int player)
{

	if(player == BLACK)
		return WHITE;
	else
		return BLACK;
}


void appendToBook(StateMove sm)
{
	ofstream bookfile;
	bookfile.open(BOOK, ofstream::out | ofstream::app);
	bookfile << sm.stateFirstHalf << " " << sm.stateSecondHalf << " " << sm.player << " " << sm.move << endl;
	bookfile.close();
}

int OthelloAI::findMoveInBook(Board *board, int playerToMove)
{
	int move = 0;
	if(book.count(State (board, playerToMove)) > 0)
		move = book.lower_bound(State (board, playerToMove))->second;
	else
		throw StateNotInBookException;
	return move;
}

void OthelloAI::openBook()
{
	unsigned long long firstHalf, secondHalf;
	int player, move;
	ifstream bookfile (BOOK);
	if (bookfile.is_open())
	{
		while ( bookfile.good() )
		{
			bookfile >> firstHalf >> secondHalf >> player >> move;
			book.insert(pair<State, int> (State (firstHalf, secondHalf, player), move));
		}
		bookfile.close();
	}
}

int OthelloAI::deepSearch(Board *board)
{
	int bestMoveScore = -2*INFINITY;
	int bestMove;
	int score;
	for (int move=0; move < GRIDSIZE*GRIDSIZE; move++)
	{
		if(Game::isLegalMove(move%GRIDSIZE,move/GRIDSIZE,board,me))
		{
			Board moveBoard (board, move%GRIDSIZE, move/GRIDSIZE, me);
			score = -negascout(&moveBoard, DEEP_DEPTH, -2*INFINITY, -bestMoveScore, otherPlayer(me));
			if(score > bestMoveScore)
			{
				bestMove = move;
				bestMoveScore = score;
			}
		}
	}
	return bestMove;
}

int possibleMoves(Board *board, int me)
{
	int moves;
	for(int i=0;i<GRIDSIZE;i++)
	{
		for(int j=0;j<GRIDSIZE;j++)
		{
			if(Game::isLegalMove(i,j,board,me))
				moves++;
		}
	}
	return moves;
}
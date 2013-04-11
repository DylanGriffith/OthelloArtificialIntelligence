#include "Game.h"
#include "OthelloAI.h"
#include <vector>

using std::vector;

// RETURN: WHITE if player is BLACK and BLACK otherwise
inline int otherPlayer(int player);
// Initialises the board to the starting position for the Othello game
void initialiseOthello(Board *board);

Game::Game(void)
{
	currentPlayer = BLACK;
	board = new(Board);
	initialiseOthello(board);
	ai = new(OthelloAI);
}


Game::~Game(void)
{
	delete board;
	delete ai;
}

int Game::getCurrentPlayer()
{
	return currentPlayer;
}

bool Game::isLegalMove(int x, int y)
{
	if(board->getSquare(x,y) != EMPTY)
		return false;
	if(x+2<GRIDSIZE)
	{
		if(board->getSquare(x+2,y) == currentPlayer && board->getSquare(x+1,y) == otherPlayer(currentPlayer))
			return true;
		if(y+2<GRIDSIZE)
		{
			if(board->getSquare(x+2,y+2) == currentPlayer && board->getSquare(x+1,y+1) == otherPlayer(currentPlayer))
				return true;
		}
		if(y-2>=0)
		{
			if(board->getSquare(x+2,y-2) == currentPlayer && board->getSquare(x+1,y-1) == otherPlayer(currentPlayer))
				return true;
		}
	}
	if(x-2>=0)
	{
		if(board->getSquare(x-2,y) == currentPlayer && board->getSquare(x-1,y) == otherPlayer(currentPlayer))
			return true;
		if(y+2<GRIDSIZE)
		{
			if(board->getSquare(x-2,y+2) == currentPlayer && board->getSquare(x-1,y+1) == otherPlayer(currentPlayer))
				return true;
		}
		if(y-2>=0)
		{
			if(board->getSquare(x-2,y-2) == currentPlayer && board->getSquare(x-1,y-1) == otherPlayer(currentPlayer))
				return true;
		}
	}
	if(y+2<GRIDSIZE)
	{
		if(board->getSquare(x,y+2) == currentPlayer && board->getSquare(x,y+1) == otherPlayer(currentPlayer))
			return true;
	}
	if(y-2>=0)
	{
		if(board->getSquare(x,y-2) == currentPlayer && board->getSquare(x,y-1) == otherPlayer(currentPlayer))
			return true;
	}

	return false;
}


Board Game::getBoard()
{
	return *board;
}

void Game::makeMove(int x, int y)
{
	if(isLegalMove(x,y))
	{
		board->insertToken(x,y,currentPlayer);
		currentPlayer = otherPlayer(currentPlayer);
		if(isOverGame())
		{
			// Do nothing
		}
		if(!canCurrentPlayerMove())
		{
			currentPlayer = otherPlayer(currentPlayer);
		}
	}
}

void Game::makeMoveAI(int strategy)
{
	if(isOverGame())
		return;
	ai->setPlayer(currentPlayer);
	while(true)
	{
		int aiMove = ai->getMove(board, strategy);
		int aiMoveX = aiMove%GRIDSIZE;
		int aiMoveY = aiMove/GRIDSIZE;
		if(isLegalMove(aiMoveX,aiMoveY))
		{
			board->insertToken(aiMoveX,aiMoveY,currentPlayer);
			currentPlayer = otherPlayer(currentPlayer);
			break;
		}else
		{
		}
	}
	if(isOverGame())
	{
		// Do nothing
	}else if(!canCurrentPlayerMove())
	{
		currentPlayer = otherPlayer(currentPlayer);
	}
}



inline int otherPlayer(int player)
{
	if(player == BLACK)
		return WHITE;
	else
		return BLACK;
}

void initialiseOthello(Board *board)
{
	delete board;
	board = new(Board);
	board->changeToken(3,3,WHITE);
	board->changeToken(4,4,WHITE);
	board->changeToken(3,4,BLACK);
	board->changeToken(4,3,BLACK);
}

bool Game::isLegalMove(int x, int y, Board *someBoard, int playerToMove)
{
	if(someBoard->getSquare(x,y) != EMPTY)
		return false;
	if(x+2<GRIDSIZE)
	{
		if(someBoard->getSquare(x+2,y) == playerToMove && someBoard->getSquare(x+1,y) == otherPlayer(playerToMove))
			return true;
		if(y+2<GRIDSIZE)
		{
			if(someBoard->getSquare(x+2,y+2) == playerToMove && someBoard->getSquare(x+1,y+1) == otherPlayer(playerToMove))
				return true;
		}
		if(y-2>=0)
		{
			if(someBoard->getSquare(x+2,y-2) == playerToMove && someBoard->getSquare(x+1,y-1) == otherPlayer(playerToMove))
				return true;
		}
	}
	if(x-2>=0)
	{
		if(someBoard->getSquare(x-2,y) == playerToMove && someBoard->getSquare(x-1,y) == otherPlayer(playerToMove))
			return true;
		if(y+2<GRIDSIZE)
		{
			if(someBoard->getSquare(x-2,y+2) == playerToMove && someBoard->getSquare(x-1,y+1) == otherPlayer(playerToMove))
				return true;
		}
		if(y-2>=0)
		{
			if(someBoard->getSquare(x-2,y-2) == playerToMove && someBoard->getSquare(x-1,y-1) == otherPlayer(playerToMove))
				return true;
		}
	}
	if(y+2<GRIDSIZE)
	{
		if(someBoard->getSquare(x,y+2) == playerToMove && someBoard->getSquare(x,y+1) == otherPlayer(playerToMove))
			return true;
	}
	if(y-2>=0)
	{
		if(someBoard->getSquare(x,y-2) == playerToMove && someBoard->getSquare(x,y-1) == otherPlayer(playerToMove))
			return true;
	}
	return false;
}

bool Game::isLegalMove(int x, int y, int player)
{
	if(board->getSquare(x,y) != EMPTY)
		return false;
	if(x+2<GRIDSIZE)
	{
		if(board->getSquare(x+2,y) == player && board->getSquare(x+1,y) == otherPlayer(player))
			return true;
		if(y+2<GRIDSIZE)
		{
			if(board->getSquare(x+2,y+2) == player && board->getSquare(x+1,y+1) == otherPlayer(player))
				return true;
		}
		if(y-2>=0)
		{
			if(board->getSquare(x+2,y-2) == player && board->getSquare(x+1,y-1) == otherPlayer(player))
				return true;
		}
	}
	if(x-2>=0)
	{
		if(board->getSquare(x-2,y) == player && board->getSquare(x-1,y) == otherPlayer(player))
			return true;
		if(y+2<GRIDSIZE)
		{
			if(board->getSquare(x-2,y+2) == player && board->getSquare(x-1,y+1) == otherPlayer(player))
				return true;
		}
		if(y-2>=0)
		{
			if(board->getSquare(x-2,y-2) == player && board->getSquare(x-1,y-1) == otherPlayer(player))
				return true;
		}
	}
	if(y+2<GRIDSIZE)
	{
		if(board->getSquare(x,y+2) == player && board->getSquare(x,y+1) == otherPlayer(player))
			return true;
	}
	if(y-2>=0)
	{
		if(board->getSquare(x,y-2) == player && board->getSquare(x,y-1) == otherPlayer(player))
			return true;
	}
	return false;
}

bool Game::canCurrentPlayerMove()
{
	for(int i=0;i<GRIDSIZE;i++)
		for(int j=0;j<GRIDSIZE;j++)
			if(this->isLegalMove(i,j,currentPlayer))
				return true;
	return false;
}


bool Game::isOverGame()
{
	for(int i=0;i<GRIDSIZE;i++)
		for(int j=0;j<GRIDSIZE;j++)
			if(this->isLegalMove(i,j,WHITE) || this->isLegalMove(i,j,BLACK))
				return false;
	return true;
}

bool Game::isTerminal(Board *someBoard)
{
	for(int i=0;i<GRIDSIZE;i++)
		for(int j=0;j<GRIDSIZE;j++)
			if(isLegalMove(i,j,someBoard,WHITE) || isLegalMove(i,j,someBoard,BLACK))
				return false;
	return true;
}

int Game::whoWins()
{
	int white = 0;
	int black = 0;
	for(int i=0;i<GRIDSIZE;i++)
		for(int j=0;j<GRIDSIZE;j++)
			if(board->getSquare(i,j) == WHITE)
				white++;
			else if(board->getSquare(i,j) == BLACK)
				black++;
	if(white > black)
		return WHITE;
	if(black > white)
		return BLACK;
	return EMPTY;
}
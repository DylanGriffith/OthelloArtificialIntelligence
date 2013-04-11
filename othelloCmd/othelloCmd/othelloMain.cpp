#include<iostream>
#include "Game.h"
#include "Board.h"

using namespace std;

#define BOARD_THICKNESS 3

// Prints the board to stdout
void printBoard(Game *game);

int main(void)
{
	int aiStrategy;
	int humanPlayer;
	int aiPlayer;
	Game *game = new(Game);
	while(true)
	{
		cout << "Select strategy type by typing a number and pressing enter" << endl;
		cout << "1: Minimax" << endl;
		cout << "2: Minimax with Alpha-Beta pruning" << endl;
		cout << "3: Minimax with Alpha-Beta pruning and Book Moves" << endl;
		cout << "4: Random moves" << endl;
		int choice;
		cin >> choice;
		switch (choice)
		{
		case 1:
			aiStrategy = AI_STRATEGY_A;
			break;
		case 2:
			aiStrategy = AI_STRATEGY_B;
			break;
		case 3:
			aiStrategy = AI_STRATEGY_C;
			break;
		case 4:
			aiStrategy = AI_STRATEGY_D;
			break;
		default:
			cout << "Invalid selection. Terminating application.";
			return 0;
			break;
		}
		cout << "Select who plays first" << endl;
		cout << "1: You play first" << endl;
		cout << "2: Computer plays first" << endl;
		cin >> choice;
		switch (choice)
		{
		case 1:
			humanPlayer = BLACK;
			aiPlayer = WHITE;
			break;
		case 2:
			aiPlayer = BLACK;
			humanPlayer = WHITE;
			break;
		default:
			cout << "Invalid selection. Terminating application.";
			return 0;
			break;
		}
		while(!game->isOverGame())
		{
			printBoard(game);
			if(game->getCurrentPlayer() == humanPlayer) // Human turn to move
			{
				int x,y;
				char xChar, yChar;
				cout << "Enter a move (eg. A5) then press enter..." << endl;
				cin >> xChar >> yChar;
				x = xChar - 'A';
				y = yChar - '1';
				if(x>=0 && x<GRIDSIZE && y>=0 && y<GRIDSIZE)
				{
					if(game->isLegalMove(x,y))
						game->makeMove(x,y);
					else
						cout << "Illegal move attempted" << endl;
				}else
				{
					cout << "Invalid selection!" << endl;
				}
			}else // Computer turn to move
			{
				cout << "AI is moving..." << endl;
				game->makeMoveAI(aiStrategy);
			}
		}
		if(game->whoWins() == BLACK)
			cout << "Game over. The winner is black!" << endl;
		else if(game->whoWins() == WHITE)
			cout << "Game over. The winner is white!" << endl;
		else
			cout << "Game over. It was a draw!" << endl;
	}
	return 0;
}

void printBoard(Game *game)
{
	cout << "   ";
	for(int i=0; i<GRIDSIZE; i++)
	{
		char letter = 'A' + i;
		for(int j=0; j<BOARD_THICKNESS+1; j++){cout << letter;}
		cout << " ";
	}
	cout << endl;
	cout << "   ";
	for (int i=0; i<GRIDSIZE; i++)
	{
		for(int j=0; j<BOARD_THICKNESS+1; j++) {cout << "-";}
		cout << " ";
	}
	cout << endl;
	Board currentBoard = game->getBoard();
	for(int y=0; y<GRIDSIZE; y++)
	{
		for(int i=0; i<BOARD_THICKNESS; i++)
		{
			cout << (y+1) << " |";
			for(int x=0; x<GRIDSIZE; x++)
			{
				if(currentBoard.getSquare(x,y) == BLACK)
				{
					for(int j=0; j<BOARD_THICKNESS+1; j++){cout << "B";}
					cout << "|";
				}else if(currentBoard.getSquare(x,y) == WHITE)
				{
					for(int j=0; j<BOARD_THICKNESS+1; j++){cout << "W";}
					cout << "|";
				}
				else
				{
					for(int j=0; j<BOARD_THICKNESS+1; j++){cout << " ";}
					cout << "|";
				}
			}
			cout << endl;
		}
		cout << "   ";
		for (int i=0; i<GRIDSIZE; i++)
		{
			for(int j=0; j<BOARD_THICKNESS+1; j++) {cout << "-";}
			cout << " ";
		}
		cout << endl;
	}
	cout << endl;
}
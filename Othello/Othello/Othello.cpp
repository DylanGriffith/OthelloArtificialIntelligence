// Othello.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#include "Othello.h"
#include "Board.h"
#include "Game.h"


#include <stdlib.h>

using namespace std;

#define MAX_LOADSTRING 100
#define GRIDWIDTH 90
#define AI_AI_GAMES 1
// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
Game *game;										// the game currently being played

int strategy1; // The strategy used by AI (defined in Game.h)
bool noplayer; // True if there is no human player (ie. AI vs. AI)
int strategy2; // Used only if noplayer is true
int humanPlayer;
int aiPlayer1;
int aiPlayer2;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);


// Draws the board in the window hdc using the global variable game.
void drawBoard(HDC);
void drawToken(HDC hdc, int x, int y, int player);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
					   _In_opt_ HINSTANCE hPrevInstance,
					   _In_ LPTSTR    lpCmdLine,
					   _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_OTHELLO, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_OTHELLO));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_OTHELLO));
	wcex.hCursor		= LoadCursor(NULL, IDC_HAND);
	wcex.hbrBackground	= (HBRUSH) CreateSolidBrush(RGB(0, 62, 0));
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_OTHELLO);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;
	game = new(Game);
	strategy1 = AI_STRATEGY_B;
	strategy2 = AI_STRATEGY_A;
	humanPlayer = BLACK;
	aiPlayer1 = BLACK;
	aiPlayer2 = WHITE;
	noplayer = false;


	hInst = hInstance; // Store instance handle in our global variable

	hWnd = CreateWindow(szWindowClass, szTitle, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		10, 10, GRIDSIZE*GRIDWIDTH + 17, GRIDSIZE*GRIDWIDTH + 60, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDM_AIFIRST:
			delete game;
			game = new(Game);
			noplayer = false;
			game->makeMoveAI(strategy2);
			humanPlayer = WHITE;
			aiPlayer2 = BLACK;
			InvalidateRect(hWnd, 0, TRUE);
			break;
		case IDM_AISECOND:
			delete game;
			game = new(Game);
			noplayer = false;
			humanPlayer = BLACK;
			aiPlayer2 = WHITE;
			InvalidateRect(hWnd, 0, TRUE);
			break;
		case IDM_STRATA:
			strategy2 = AI_STRATEGY_A;
			InvalidateRect(hWnd, 0, TRUE);
			break;
		case IDM_STRATB:
			strategy2 = AI_STRATEGY_B;
			InvalidateRect(hWnd, 0, TRUE);
			break;
		case IDM_STRATC:
			strategy2 = AI_STRATEGY_C;
			InvalidateRect(hWnd, 0, TRUE);
			break;
		case IDM_STRATD:
			strategy2 = AI_STRATEGY_D;
			InvalidateRect(hWnd, 0, TRUE);
			break;
		case IDM_STRATE:
			strategy2 = AI_STRATEGY_E;
			InvalidateRect(hWnd, 0, TRUE);
			break;
		case IDM_NOPLAYER:
			delete game;
			game = new(Game);
			strategy1 = AI_STRATEGY_D;
			strategy2 = AI_STRATEGY_C;
			aiPlayer1 = BLACK;
			aiPlayer2 = WHITE;
			noplayer = true;
			InvalidateRect(hWnd, 0, TRUE);
			break;
		case IDM_LEARNING:
			delete game;
			game = new(Game);
			noplayer = true;
			aiPlayer1 = BLACK;
			aiPlayer2 = WHITE;
			strategy1 = AI_STRATEGY_D;
			strategy2 = AI_STRATEGY_E;
			InvalidateRect(hWnd, 0, TRUE);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_LBUTTONDOWN:
		{
			if(noplayer)
			{
				for(int counter = 0;counter < AI_AI_GAMES; counter++)
				{
					delete game;
					game = new(Game);
					while(!game->isOverGame())
					{
						if(game->getCurrentPlayer() == aiPlayer1)
						{
							game->makeMoveAI(strategy1);
						}else
						{
							game->makeMoveAI(strategy2);
						}
					}
					// Swap players
					int temp = aiPlayer1;
					aiPlayer1 = aiPlayer2;
					aiPlayer2 = temp;
				}
				int winner = game->whoWins();
				if(winner == WHITE)
					::MessageBox(hWnd, _T("The winner is white!"), _T("Game over"), MB_OK | MB_ICONEXCLAMATION);
				else if (winner == BLACK)
					::MessageBox(hWnd, _T("The winner is black!"), _T("Game over"), MB_OK | MB_ICONEXCLAMATION);
				else
					::MessageBox(hWnd, _T("It was a draw!"), _T("Game over"), MB_OK | MB_ICONEXCLAMATION);
				InvalidateRect(hWnd, 0, TRUE);
				break;
			}else
			{
				int x,y;
				x = LOWORD(lParam)/GRIDWIDTH;
				y = HIWORD(lParam)/GRIDWIDTH;
				if (game->isLegalMove(x,y)) {
					game->makeMove(x,y);
					while(!game->isOverGame())
					{
						if(game->getCurrentPlayer() == aiPlayer2)
							game->makeMoveAI(strategy2);
						else
							break;
					}
					if(game->isOverGame())
					{
						int winner = game->whoWins();
						if(winner == WHITE)
							::MessageBox(hWnd, _T("The winner is white!"), _T("Game over"), MB_OK | MB_ICONEXCLAMATION);
						else if (winner == BLACK)
							::MessageBox(hWnd, _T("The winner is black!"), _T("Game over"), MB_OK | MB_ICONEXCLAMATION);
						else
							::MessageBox(hWnd, _T("It was a draw!"), _T("Game over"), MB_OK | MB_ICONEXCLAMATION);
					}

				} else {
					// Invalid move
				}
				// Repaint the window after the update
				InvalidateRect(hWnd, 0, TRUE);
				break;
			}
		}
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// Draw Grid
		drawBoard(hdc);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

void drawBoard(HDC hdc)
{
	HPEN hPenOld;
	// Draw the board lines
	HPEN hLinePen;
	COLORREF qLineColor;
	qLineColor = RGB(0, 0, 0);
	hLinePen = CreatePen(PS_SOLID, 2, qLineColor);
	hPenOld = (HPEN)SelectObject(hdc, hLinePen);


	for (int i = GRIDWIDTH; i < GRIDSIZE*GRIDWIDTH; i += GRIDWIDTH) {
		MoveToEx(hdc, i, 0, NULL);
		LineTo(hdc, i, GRIDSIZE*GRIDWIDTH);
	}


	for (int i = GRIDWIDTH; i < GRIDSIZE*GRIDWIDTH; i += GRIDWIDTH) {
		MoveToEx(hdc, 0, i, NULL);
		LineTo(hdc, GRIDSIZE*GRIDWIDTH, i);
	}

	Board currentBoard = game->getBoard();
	for (int i=0; i<GRIDSIZE;i++)
	{
		for(int j=0; j<GRIDSIZE;j++)
		{
			int square = currentBoard.getSquare(i,j);
			if(square != EMPTY)
			{
				drawToken(hdc,i,j,square);
			}
		}
	}

	SelectObject(hdc, hPenOld);
	DeleteObject(hLinePen);
}

void drawToken(HDC hdc, int x, int y, int player) {
	HPEN hPenOld;
	HPEN hLinePen;
	HBRUSH hBrushOld;
	HBRUSH hBrush;
	COLORREF qLineColor;

	if(player == WHITE)
	{
		qLineColor = RGB(255, 255, 255);
	}else if(player == BLACK)
	{
		qLineColor = RGB(0, 0, 0);
	}else
	{
		return;
	}

	const int penThickness = 2;
	hLinePen = CreatePen(PS_SOLID, penThickness, RGB(0, 0, 0));
	hBrush = CreateSolidBrush(qLineColor);
	hPenOld = (HPEN)SelectObject(hdc, hLinePen);
	hBrushOld = (HBRUSH)SelectObject(hdc, hBrush);

	// Get bounds
	const int x0	= x*GRIDWIDTH + 2*penThickness;
	const int x1	= (x + 1)*GRIDWIDTH - 2*penThickness;
	const int y0	= y*GRIDWIDTH + 2*penThickness;
	const int y1	= (y + 1)*GRIDWIDTH - 2*penThickness;

	Ellipse(hdc, x0, y0, x1, y1);

	SelectObject(hdc, hPenOld);
	DeleteObject(hLinePen);
	SelectObject(hdc, hBrushOld);
	DeleteObject(hBrush);
}
#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\Othello\Board.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace OthelloTest
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			Board *board = new Board();
		}

	};
}
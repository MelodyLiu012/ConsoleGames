
#include <iostream>
#include <list>
#include <thread>
using namespace std;

#include "Windows.h"

const int nScreenWidth = 120;
const int nScreenHeight = 30;

struct sSnakeSegment
{
	int x;
	int y;
};

int main()
{
	// Screen Buffer
	wchar_t * screen = new wchar_t[nScreenWidth * nScreenHeight];
	for (int i = 0; i < nScreenWidth * nScreenHeight; i++)
	{
		screen[i] = L' ';
	}
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	
	//Game Loop
	while (true)
	{
		//Reset Game
		list<sSnakeSegment> snake = { {60, 15} , {61, 15} , {62, 15} , {63, 15} , {64, 15} };
		int nFruitX = 30;
		int nFruitY = 15;
		int nScore = 0;
		int nSnakeDirection = 3; //0 is up, 1 is right, 2 is down, 3 is left
		bool bDead = false;
		bool bKeyLeft = false, bKeyRight = false, bKeyUp = false, bKeyDown = false,
			bKeyLeftOld = false, bKeyRightOld = false, bKeyUpOld = false, bKeyDownOld = false;

		while (!bDead)
		{
			//Timing and Input ===========================================
			this_thread::sleep_for(100ms);

			//Frame Timing to Increase Responsiveness to User Input
			auto t1 = chrono::system_clock::now();
														//If going right or left, go faster to compensate for aspect ratio
			while ((chrono::system_clock::now() - t1) < ((nSnakeDirection % 2 == 1) ? 50ms : 130ms)) 
			{
				//Get Input
				bKeyRight = (0x8000 & GetAsyncKeyState((unsigned char)('\x27'))) != false;
				bKeyLeft = (0x8000 & GetAsyncKeyState((unsigned char)('\x25'))) != false;
				bKeyUp = (0x8000 & GetAsyncKeyState((unsigned char)('\x26'))) != false;
				bKeyDown = (0x8000 & GetAsyncKeyState((unsigned char)('\x28'))) != false;

				if (bKeyRight && !bKeyRightOld)
				{
					nSnakeDirection = 1;
				}

				if (bKeyLeft && !bKeyLeftOld)
				{
					nSnakeDirection = 3;
				}

				if (bKeyUp && !bKeyUpOld)
				{
					nSnakeDirection = 0;
				}

				if (bKeyDown && !bKeyDownOld)
				{
					nSnakeDirection = 2;
				}

				bKeyRightOld = bKeyRight;
				bKeyLeftOld = bKeyLeft;
				bKeyUpOld = bKeyUp;
				bKeyDownOld = bKeyDown;

				if ((0x8000 & GetAsyncKeyState((unsigned char)('\x58'))))
				{
					bDead = true;
				}
				
			}

			//Game Logic =============================================

			//Update Snake Position
			switch (nSnakeDirection)
			{
			case 0: //Up
				snake.push_front({ snake.front().x, snake.front().y - 1 });
				break;
			case 1: //Right
				snake.push_front({ snake.front().x + 1, snake.front().y });
				break;
			case 2: //Down
				snake.push_front({ snake.front().x, snake.front().y + 1 });
				break;
			case 3: //Left
				snake.push_front({ snake.front().x - 1, snake.front().y });
				break;
			default:
				break;
			}

			//Detect Collision With Edge of Screen
			if (snake.front().x < 0 || snake.front().x >= nScreenWidth ||
				snake.front().y < 3 || snake.front().y >= nScreenHeight)
			{
				bDead = true;
			}

			//Detect If Snake Eats Fruit
			if (snake.front().x == nFruitX && snake.front().y == nFruitY)
			{
				nScore++;
				while (screen[nFruitY * nScreenWidth + nFruitX] != ' ')
				{
					nFruitX = rand() % nScreenWidth;
					nFruitY = (rand() % (nScreenHeight - 3)) + 3;
				}
				//Adds five tail segments for one fruit eaten
				for (int i = 0; i < 5; i++)
				{ 
					snake.push_back({ snake.back().x, snake.back().y });
				}
			}

			//Detect Collision With Itself
			for (list<sSnakeSegment>::iterator i = snake.begin(); i != snake.end(); i++)
			{
				if (i != snake.begin() && i->x == snake.front().x && i->y == snake.front().y)
				{
					bDead = true;
				}
			}

			//Take out last segment of snake's tail
			snake.pop_back();


			//Display to Player =================================================

			//Clear Screen
			for (int i = 0; i < nScreenWidth * nScreenHeight; i++)
			{
				screen[i] = L' ';
			}

			//Draw Stats and Border
			for (int i = 0; i < nScreenWidth; i++)
			{
				screen[i] = L'=';
				screen[2 * nScreenWidth + i] = L'=';
			}
			wsprintf(&screen[nScreenWidth + 6], L"Press arrow keys to move, X to end game");
			wsprintf(&screen[nScreenWidth * 2 - 14], L"Score: %d", nScore);

			//Draw Snake Body
			for (auto s : snake)
			{
				if (bDead)
				{
					screen[s.y * nScreenWidth + s.x] = L'+';
				}
				else
				{
					screen[s.y * nScreenWidth + s.x] = L'o';
				}
			}

			//Draw Snake Head
			if (bDead)
			{
				screen[snake.front().y * nScreenWidth + snake.front().x] = L'X';
			}
			else
			{
				screen[snake.front().y * nScreenWidth + snake.front().x] = L'O';
			}

			//Draw Fruit
			screen[nFruitY * nScreenWidth + nFruitX] = L'F';

			//If Snake Is Dead
			if (bDead)
			{
				wsprintf(&screen[15 * nScreenWidth + 45], L"Press \"Space\" to play again.");
			}

			//Display Frame
			WriteConsoleOutputCharacter(hConsole, screen, nScreenHeight * nScreenWidth, { 0, 0 }, &dwBytesWritten);
		}

		//Wait for Space to be Pressed
		while ((0x8000 & GetAsyncKeyState((unsigned char)('\x20'))) == false);

	}
	return 0;
}
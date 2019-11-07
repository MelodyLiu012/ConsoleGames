#include <iostream>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <conio.h>
using namespace std;


bool gameOver;
const int boardWidth = 20;
const int boardHeight = 20;
int x, y, fruitX, fruitY, score;
enum eDirection {STOP = 0, LEFT, RIGHT, UP, DOWN};
eDirection currentDir;
int tailX[boardWidth * boardHeight], tailY[boardWidth * boardHeight];
int numTail;
char borderChar = '#';


void Setup();
void Draw();
void Input();
void Logic();


int main()
{
	srand(static_cast<unsigned>(time(nullptr)));

	Setup();

	//Game Loop
	
	while (!gameOver)
	{
		Draw();
		Input();
		Logic();
		//Sleep(100);
	}
	
}


void Setup()
{
	gameOver = false;
	currentDir = STOP;
	x = boardWidth / 2;
	y = boardHeight / 2;
	fruitX = rand() % boardWidth;
	fruitY = rand() % boardHeight;
	while (fruitX == x && fruitY == y)
	{
		fruitX = rand() % boardWidth;
		fruitY = rand() % boardHeight;
	}
	score = 0;

}

void Draw()
{
	system("cls");

	cout << "Press WASD to move snake and start game, X to end game." << endl;
	cout << "Score: " << score << endl;
	

	for (int i = 0; i < boardWidth + 2; i++)
	{
		cout << borderChar;
	}
	cout << endl;

	for (int i = 0; i < boardHeight; i++)
	{
		cout << borderChar;
		for (int j = 0; j < boardWidth; j++)
		{
			if (j == x && i == y)
			{
				cout << "O";
			}
			else if (j == fruitX && i == fruitY)
			{
				cout << "F";
			}
			else
			{
				bool printTail = false;
				for (int k = numTail - 1; k >= 0; k--)
				{
					if (tailX[k] == j && tailY[k] == i)
					{
						cout << "o";
						printTail = true;
					}
				}
				if (!printTail)
				{
					cout << " ";
				}
			}
		}
		cout << borderChar << endl;

	}

	for (int i = 0; i < boardWidth + 2; i++)
	{
		cout << borderChar;
	}
	cout << endl;

	cout << "<Debug>" << endl;
	cout << "Head Position: " << "(" << x << ", " << y << ")" << endl;
	cout << "Fruit Position: " << "(" << fruitX << ", " << fruitY << ")" << endl;
}

void Input()
{
	if (_kbhit())
	{
		switch (_getch())
		{
		case 'w':
			currentDir = UP;
			break;
		case 'a':
			currentDir = LEFT;
			break;
		case 's':
			currentDir = DOWN;
			break;
		case 'd':
			currentDir = RIGHT;
			break;
		case 'x':
			gameOver = true;
			break;
		}
	}
}

void Logic()
{
	//If head hits tail segment
	for (int i = numTail - 1; i >= 0; i--)
	{
		if (tailX[i] == x && tailY[i] == y)
		{
			gameOver = true;
		}
	}
	
	//Update tail segment positions
	for (int i = numTail - 1; i > 0; i--)
	{
		tailX[i] = tailX[i - 1];
		tailY[i] = tailY[i - 1];
	}
	tailX[0] = x;
	tailY[0] = y;
	

	//Movement based on user inputted direction
	switch (currentDir)
	{
	case UP:
		y--;
		break;
	case LEFT:
		x--;
		break;
	case DOWN:
		y++;
		break;
	case RIGHT:
		x++;
		break;
	default:
		break;
	}
	
	//If snake head hits board edge, game over
	if (x < 0 || x > boardWidth - 1 || y < 0 || y > boardHeight - 1)
	{
		gameOver = true;
	}

	//If fruit gets eaten
	if (x == fruitX && y == fruitY)
	{
		score++;
		fruitX = rand() % boardWidth;
		fruitY = rand() % boardHeight;
		numTail++;
	} 
}
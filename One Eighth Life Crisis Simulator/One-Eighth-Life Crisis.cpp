// One-Eighth-Life Crisis.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class OELCSimulator : public olc::PixelGameEngine
{
public:
	OELCSimulator()
	{
		sAppName = "One-Eighth-Life Crisis Simulator";
	}
	
private:

	int waitCounter = 0;

	//Title Screen
	bool gameStart = true;
	int blackScreenOpacity = 255;
	int titleOpacity = 0;
	bool titleEnd = false;
	void titleScreen();
	
	//Game
	bool gameRunning = false;
	int textOpacity = 0;
	olc::Pixel textPixel;
	int page = 1;
	bool dialogueStart = false;

public:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here

		
		return true;
	}
	bool OnUserUpdate(float fElapsedTime) override
	{
		// called once per frame

		if (gameStart)
		{
			titleScreen();
		}

		if (gameRunning)
		{
			//Make background white
			for (int y = 0; y < ScreenHeight(); y++)
				for (int x = 0; x < ScreenWidth(); x++)
					Draw(x, y, olc::Pixel(255, 255, 255));

			//Draw text box
			DrawRect(20, ScreenHeight() / 2 + 30, ScreenWidth() - 40, ScreenHeight() / 2 - 50, olc::BLACK);
			DrawRect(21, ScreenHeight() / 2 + 31, ScreenWidth() - 42, ScreenHeight() / 2 - 52, olc::BLACK);

			//==============================================================================

			/*olc::Sprite * letterA = new olc::Sprite("Letter A.png");
			DrawSprite(20, 20, letterA, 1);*/

			//Take user input
			if (GetKey(olc::Key::X).bPressed)
			{
				page++;
			}

			if (page == 1) //It needs to do the "..." thing
			{
				if (waitCounter != 54)
				{
					waitCounter++;
				}
				if (waitCounter == 50)
				{
					dialogueStart = true;
				}
			}

			if (dialogueStart)
			{
				switch (page)
				{
				case 1:
					if (waitCounter >= 50)
						DrawString(40, ScreenHeight() / 2 + 50, ".", olc::BLACK, 1);
					if (waitCounter >= 52)
						DrawString(47, ScreenHeight() / 2 + 50, ".", olc::BLACK, 1);
					if (waitCounter == 54)
						DrawString(54, ScreenHeight() / 2 + 50, ".", olc::BLACK, 1);
					break;
				case 2:
					DrawString(40, ScreenHeight() / 2 + 50, "Hi.", olc::BLACK, 1);
					break;
				}
			}
	
		}
		
		

		return true;
	}
};


//All private function definitions================================================

void OELCSimulator::titleScreen()
{
	//Make background white
	for (int y = 0; y < ScreenHeight(); y++)
		for (int x = 0; x < ScreenWidth(); x++)
			Draw(x, y, olc::Pixel(255, 255, 255));

	//Black screen fade out
	if (!titleEnd)
	{
		for (int y = 0; y < ScreenHeight(); y++)
		{
			for (int x = 0; x < ScreenWidth(); x++)
			{
				Draw(x, y, olc::Pixel(0, 0, 0, blackScreenOpacity));
			}
		}
		if (blackScreenOpacity != 0)
		{
			blackScreenOpacity -= 3;
		}
	}
	//Title fade in
	olc::Pixel titlePixel(0, 0, 0, titleOpacity);
	DrawString(60, ScreenHeight() / 2, "One-Eighth-Life Crisis Simulator", titlePixel, 1);
	if (titleOpacity != 255 && !titleEnd)
	{	
		titleOpacity += 3;
	}

	//Delay after title fully appears and start button appearing
	if (titleOpacity == 255 && waitCounter < 30)
	{
		waitCounter++;
	}
	if (waitCounter == 30)
	{
		DrawString(230, ScreenHeight() / 2 + 150, "Start Your Day(X)", titlePixel, 1);
		if (GetKey(olc::Key::X).bPressed)
		{
			titleEnd = true;
		}
		if (titleEnd && titleOpacity > 0) //Title fade out
		{
			titleOpacity -= 3;
		}
		if (titleOpacity == 0)
		{
			gameStart = false;
			gameRunning = true;
			waitCounter = 0;
		}
	}
}




//Main Function===============================================================

int main()
{
	OELCSimulator game;
	if (game.Construct(400, 400, 4, 4))
		game.Start();
	return 0;
}



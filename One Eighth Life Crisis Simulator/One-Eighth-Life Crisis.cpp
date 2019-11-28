/*
Note to self:
- Change timing for drawing text to be based on clock()
*/


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
	int nextOpacity = 0;
	void drawNextIndicator();
	void drawDialoguePage(std::string dialogueText);
	std::string currentText = "";
	int currentCharIndex = 0;
	int page = 1;
	bool dialogueStart = false;
	

	//Page 16
	int ringFrame = 1;

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

			//Take user input
			if (GetKey(olc::Key::X).bPressed)
			{
				if ((page == 1 && waitCounter >= 70) || (page != 1 && waitCounter >= 20)) //Prevents player from clicking too fast
				{
					page++;
					waitCounter = 0;
					nextOpacity = 0;
					currentText = "";
					currentCharIndex = 0;
				}
			}

			if (page == 1) //First page needs special wait counter times 
			{
				if (waitCounter != 70) //Delay for "Next(X)" indicator for first page is 20 
				{
					waitCounter++;
				}
				if (waitCounter == 50) //Delay between title screen and dialogue start
				{
					dialogueStart = true;
				}
			}
			else 
			{
				if (waitCounter != 800)
				{
					waitCounter++;
				}
			}

			if (dialogueStart)
			{
				switch (page)
				{
				case 1: // It needs to do the "..." thing
					if (waitCounter >= 50)
						DrawString(40, ScreenHeight() / 2 + 50, ".", olc::BLACK, 1);
					if (waitCounter >= 52)
						DrawString(48, ScreenHeight() / 2 + 50, ".", olc::BLACK, 1);
					if (waitCounter >= 54)
						DrawString(56, ScreenHeight() / 2 + 50, ".", olc::BLACK, 1);
					if (waitCounter == 70)
						drawNextIndicator();
					break;
				case 2:
					drawDialoguePage("Hello.");
					break;
				case 3:
					drawDialoguePage("Welcome, lost soul.");
					break;
				case 4:
					drawDialoguePage("Where did you come from?");
					break;
				case 5:
					drawDialoguePage("Where are you headed?");
					break;
				case 6:
					drawDialoguePage("What is your future?");
					break;
				case 7:
					drawDialoguePage("...");
					break;
				case 8:
					drawDialoguePage("Do you have a future?");
					break;
				case 9:
					drawDialoguePage("...");
					break;
				case 10:
					drawDialoguePage("Well, no matter who you are...");
					break;
				case 11:
					drawDialoguePage("Where you are in life right now...");
					break;
				case 12:
					drawDialoguePage("If you're looking to re-experience the \n\nfeelings of your high school years...");
					break;
				case 13:
					drawDialoguePage("Or to, if only for a brief moment, ease \n\nyour suffering by relating it to someone \n\nelse's...");
					break;
				case 14:
					drawDialoguePage("Then you've come to the right place.");
					break;
				case 15:
					drawDialoguePage("In this game, you will take the role of \n\na pitiful high school junior.");
					break;
				case 16:
					// Draw alarm ring animation
					if (waitCounter % 10 == 0) //change frame every 10 counts
					{
						if (ringFrame == 1)
						{
							ringFrame = 2;
						}
						else 
						{
							ringFrame = 1;
						}
					}
					if (ringFrame == 1)
					{
						DrawSprite(20, 20, &olc::Sprite("AlarmRing1.jpg"), 1);
					}
					if (ringFrame == 2)
					{
						DrawSprite(20, 20, &olc::Sprite("AlarmRing2.jpg"), 1);
					}

					if (waitCounter >= 40)
						drawDialoguePage("Oh dear, do you hear that?");
					if (waitCounter >= 800)
						drawDialoguePage("Oh dear, do you hear that? (The alarm \n\nwon't stop unless you move on. Don't run \n\naway from your problem and press next.)");
					break;
				case 17:
					// Draw alarm ring animation
					if (waitCounter % 10 == 0) //change frame every 10 counts
					{
						if (ringFrame == 1)
						{
							ringFrame = 2;
						}
						else
						{
							ringFrame = 1;
						}
					}
					if (ringFrame == 1)
					{
						DrawSprite(20, 20, &olc::Sprite("AlarmRing1.jpg"), 1);
					}
					if (ringFrame == 2)
					{
						DrawSprite(20, 20, &olc::Sprite("AlarmRing2.jpg"), 1);
					}
					drawDialoguePage("It's the sound of your alarm.");
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
		DrawString(290, ScreenHeight() / 2 + 150, "Start(X)", titlePixel, 1);
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

void OELCSimulator::drawNextIndicator()
{
	olc::Pixel nextPixel(90, 90, 90, nextOpacity);
	DrawString(310, 360, "Next(X)", nextPixel, 1);
	if (nextOpacity != 255)
	{
		nextOpacity += 5;
	}
}

void OELCSimulator::drawDialoguePage(std::string dialogueText)
{
	//Drawing dialogue
	if (waitCounter % 2 == 0 && currentCharIndex < dialogueText.size())
	{
		currentText += dialogueText[currentCharIndex];
		currentCharIndex++;
		
	}
	DrawString(40, ScreenHeight() / 2 + 50, currentText, olc::BLACK, 1);
	

	//Set delay time to draw "Next(X)" indicator based on number of words
	int numWords = 1; //initialized to 1 to account for last word before the \0
	for (int i = 0; dialogueText[i] != '\0'; i++)
	{
		if (dialogueText[i] == ' ')
		{
			numWords++;
		}
	}
	if (waitCounter >= 200 + numWords * 10) 
	{
		drawNextIndicator();
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



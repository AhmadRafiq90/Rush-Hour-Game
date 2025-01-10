
//============================================================================
// Name        : .cpp
// Author      : M.Ahmad Rafiq
// Version     :
// Copyright   : (c) Reserved
// Description : Basic 2D game of Rush Hour...
//============================================================================

#ifndef RushHour_CPP_
#define RushHour_CPP_
#include "util.h"
#include <iostream>
#include <string>
#include <cmath>
#include <fstream>
using namespace std;

/* Function sets canvas size (drawing area) in pixels...
 *  that is what dimensions (x and y) your game will have
 *  Note that the bottom-left coordinate has value (0,0) and top-right coordinate has value (width-1,height-1)
 * */
void SetCanvasSize(int, int);

void DrawNpcCar();

int xI = 10, yI = 740;
bool flag = true;
void moveCar(int, int);
void MoveVerticalNpcs(int &, int &, int &, int &);
// Array that will store co-ordinates of obstacles
int ObstacleArray[200][2], TreeArray[5][2], BoxArray[3][2], PassengerArray[5][2], DropLocation[5][2];
int LocationArray[5][2] = {{130, 585},
						   {915, 145},
						   {280, 90},
						   {985, 585},
						   {740, 285}};
int score = 0, MilliSecondsTimer = 0, Minutestimer = 0, Secondstimer = 0;
int ObstacleLimiter, TreeLimiter, BoxLimiter, PassengerLimiter, IndexComparison;
bool Horizontal = true;
bool DisplayPassenger[4] = {true, true, true, true};
bool NearPassenger[4] = {false, false, false, false};
int MenuCounter = 0, TaxiMenuCounter = 0;
bool AtLocation = false, SpaceKeyPressed = false;
bool MainGame = false, DisplayLeaderboard = false, DisplayMenu = true, DisplayTaxiMenu = false, TakePlayerName = false, GameOver = false;
bool YellowTaxi = true;
string UserName;
void DrawHorizontalObstacles(int, int, int, int &);
void DrawVerticalObstacles(int, int, int, int &);
void DrawTree(int, int, int &);
void DrawBox(int, int, int &);
void DrawHorizontalCar(int, int, float *);
void DrawVerticalCar(int, int, float *);
void VerticalNPCMovement(int &, int, int, int, bool &);
void HorizontalNPCMovement(int &, int, int, int, bool &);
void DrawPassenger(int, int);
void DrawPassenger(int, int, int &);
int XRandomizer();
int YRandomizer();
void Swap(int *, int *);
void Swap(string *, string *);
void Menu();
void TaxiSelection();
void Display1();
void Leaderboard();
void PlayerName();
void GameIsOver();
void GameDisplay();
void NonPrintableKeys(int, int, int);
void PrintableKeys(unsigned char, int, int);
void Timer(int);
void MousePressedAndMoved(int, int);
void MouseMoved(int, int);
void MouseClicked(int, int, int, int);
void DrawMainTaxi();

/*
 * our gateway main function
 * */
int main(int argc, char *argv[])
{

	int width = 1920, height = 1080;			  // i have set my window size to be 800 x 600
	InitRandomizer();							  // seed the random number generator...
	glutInit(&argc, argv);						  // initialize the graphics library...
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // we will be using color display mode
	glutInitWindowPosition(50, 50);				  // set the initial position of our window
	glutInitWindowSize(1920, 1080);				  // set the size of our window
	glutCreateWindow("Rush Hour by Ahmad Rafiq"); // set the title of our game window
	SetCanvasSize(1020, 840);					  // set the number of pixels...
	glutDisplayFunc(Display1);					  // tell library which function to call for drawing Canvas
	glutIdleFunc(Display1);
	glutSpecialFunc(NonPrintableKeys); // tell library which function to call for non-printable ASCII characters
	glutKeyboardFunc(PrintableKeys);   // tell library which function to call for printable ASCII characters
									   // Register your functions to the library,
									   // you are telling the library names of function to call for different tasks.
	// This function tells the library to call our Timer function after 1000.0/FPS milliseconds...
	glutTimerFunc(1000.0, Timer, 0);

	glutMouseFunc(MouseClicked);
	glutPassiveMotionFunc(MouseMoved);	  // Mouse
	glutMotionFunc(MousePressedAndMoved); // Mouse

	// now handle the control to library and it will call our registered functions when
	// it deems necessary...
	glutMainLoop();
	return 1;
}
// For swapping two integers.
void Swap(int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}
// For swapping two strings.
void Swap(string *a, string *b)
{
	string temp = *a;
	*a = *b;
	*b = temp;
}
void Display1()
{
	if (DisplayMenu)
	{
		//glutDisplayFunc(GameIsOver);
		glutDisplayFunc(Menu);
		//glutIdleFunc(Menu);
	}
	else if (DisplayTaxiMenu)
	{
		glutDisplayFunc(TaxiSelection);
		//glutIdleFunc(TaxiSelection);
	}
	else if (TakePlayerName)
	{
		glutDisplayFunc(PlayerName);
	}
	else if (DisplayLeaderboard)
	{
		glutDisplayFunc(Leaderboard);
	}
	else if (MainGame)
	{
		glutDisplayFunc(GameDisplay);
		//glutIdleFunc(GameDisplay);
	}
}
// Function for taking name of user.
void PlayerName()
{
	glClearColor(0.8 /*Red Component*/, 0.8, /*Green Component*/ 0.8 /*Blue Component*/, 0 /*Alpha component*/);
	glClear(GL_COLOR_BUFFER_BIT); //Update the colors
	DrawString(440, 750, "Please Enter Your Name", colors[BLUE]);
	DrawString(440, 550, UserName, colors[RED]);
	glutSwapBuffers();
}
void GameIsOver()
{
	glClearColor(0.8 /*Red Component*/, 0.8, /*Green Component*/ 0.8 /*Blue Component*/, 0 /*Alpha component*/);
	glClear(GL_COLOR_BUFFER_BIT); //Update the colors
	DrawString(440, 550, "Game Over", colors[DARK_BLUE]);
	DrawString(440, 450, "Your Score is ", colors[DARK_BLUE]);
	string displayscore = Num2Str(score);
	DrawString(540, 450, displayscore, colors[RED]);
	glutSwapBuffers();
}
void SetCanvasSize(int width, int height)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 1020, 0, 840, -1, 1); // set the screen size to given width and height.
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
// FUnction for drawing trees.
void DrawTree(int x, int y, int &counter)
{
	// The Green or upper portion of the tree.
	DrawRectangle(x, y, 40, 30, colors[GREEN]);
	// The trunk of the tree.
	DrawRectangle(x + 10, y - 30, 20, 35, colors[BLACK]);
	TreeArray[counter][0] = x;
	TreeArray[counter][1] = y;
	counter++;
}
// This function draws horizontal obstacles at a gap of 50 pixels.
void DrawHorizontalObstacles(int obsx, int obsy, int LimitingValue, int &counter)
{
	// These are the co-ordinates where obstacles are being placed and are being stored in a 2-D array. 1st column contains the x-coord whereas
	// 2nd column has the y-coord
	for (; obsx <= LimitingValue; obsx += 50, counter++)
	{
		DrawRectangle(obsx, obsy, 49, 50, colors[BLACK]);
		ObstacleArray[counter][0] = obsx;
		ObstacleArray[counter][1] = obsy;
	}
}
//This function draws vertical obstacles at a gap of 50 pixels.
void DrawVerticalObstacles(int obsx, int obsy, int LimitingValue, int &counter)
{
	// These are the co-ordinates where obstacles are being placed and are being stored in a 2-D array. 1st column contains the x-coord whereas
	// 2nd column has the y-coord
	for (; obsy >= LimitingValue; obsy -= 50, counter++)
	{
		DrawRectangle(obsx, obsy, 49, 50, colors[BLACK]);
		ObstacleArray[counter][0] = obsx;
		ObstacleArray[counter][1] = obsy;
	}
}
// This function takes co-ordinates x and y as parameters and then draws a horizontal car at that location.
void DrawHorizontalCar(int x, int y, float *color)
{ //Lower tyres
	DrawCircle(x + 40, y, 7.5, colors[BLACK]);
	DrawCircle(x + 40, y + 25, 8.5, colors[BLACK]);
	// Upper tyres
	DrawCircle(x + 7, y, 7.5, colors[BLACK]);
	DrawCircle(x + 9, y + 25, 9.5, colors[BLACK]);
	DrawRectangle(x, y, 45, 30, color);
}
// This function takes co-ordinates x and y as parameters and then draws a vertical car at that location.
void DrawVerticalCar(int x, int y, float *color)
{ //Lower tyres
	DrawCircle(x + 5, y, 7.5, colors[BLACK]);
	DrawCircle(x + 25, y, 7.5, colors[BLACK]);
	// Upper tyres
	DrawCircle(x + 5, y + 40, 8, colors[BLACK]);
	DrawCircle(x + 25, y + 40, 8, colors[BLACK]);
	DrawRectangle(x, y, 30, 45, color);
}
// This function takes co-ordinates x and y as parameters and then draws a box at that location.
void DrawBox(int x, int y, int &counter)
{
	DrawSquare(x, y, 40, colors[BROWN]);
	BoxArray[counter][0] = x;
	BoxArray[counter][1] = y;
	counter++;
}
// This function takes co-ordinates x and y as parameters and then draws a passenger at that location.
void DrawPassenger(int x, int y, int &counter)
{
	DrawCircle(x, y, 8, colors[BLACK]);
	DrawLine(x, y - 5, x, y - 30, 2, colors[BLACK]);
	DrawLine(x, y - 30, x - 10, y - 45, 3, colors[BLACK]);
	DrawLine(x, y - 30, x + 10, y - 45, 3, colors[BLACK]);
	PassengerArray[counter][0] = x;
	PassengerArray[counter][1] = y;
	counter++;
}
// Overloaded function for drawing a passenger at the drop-off location and no counter because no need to store the co-ordinates of that location.
void DrawPassenger(int x, int y)
{
	DrawCircle(x, y, 8, colors[BLACK]);
	DrawLine(x, y - 5, x, y - 30, 2, colors[BLACK]);
	DrawLine(x, y - 30, x - 10, y - 45, 3, colors[BLACK]);
	DrawLine(x, y - 30, x + 10, y - 45, 3, colors[BLACK]);
}
// Used to control vertically moving NPC Cars,
void VerticalNPCMovement(int &Y, int LowerLimit, int UpperLimit, int speed, bool &NPCFlag)
{
	if (Y >= LowerLimit && NPCFlag)
	{
		Y += speed;
		if (Y == UpperLimit)
			NPCFlag = false;
	}
	else if (Y <= UpperLimit && !NPCFlag)
	{
		Y -= speed;
		if (Y == LowerLimit)
			NPCFlag = true;
	}
}
// Used to control horizontally moving NPC Cars,
void HorizontalNPCMovement(int &X, int UpperLimit, int LowerLimit, int speed, bool &NPCFlag)
{
	if (X <= UpperLimit && NPCFlag)
	{
		if (X == UpperLimit)
			NPCFlag = false;
		X += speed;
	}
	else if (X >= LowerLimit && !NPCFlag)
	{
		if (X == LowerLimit)
			NPCFlag = true;
		X -= speed;
	}
}
// For drawing the main taxi. if the boolean yellowtaxi is true, it draws the yellow taxi. the second boolean controls the orientation of the car.
void DrawMainTaxi()
{
	if (YellowTaxi)
	{
		if (Horizontal == true)
		{
			DrawHorizontalCar(xI, yI, colors[YELLOW]);
		}
		else
		{
			DrawVerticalCar(xI, yI, colors[YELLOW]);
		}
	}
	else
	{
		if (Horizontal == true)
		{
			DrawHorizontalCar(xI, yI, colors[RED]);
		}
		else
		{
			DrawVerticalCar(xI, yI, colors[RED]);
		}
	}
}
void DrawNpcCar()
{
	static int Npc1X = 625, Npc1Y = 30, Npc2X = 20, Npc2Y = 30, Npc3X = 575, Npc3Y = 270, Npc4X = 620, Npc4Y = 670;
	// Drawing npc car one
	DrawVerticalCar(Npc1X, Npc1Y, colors[BLUE_VIOLET]);
	// Drawing npc car two
	DrawVerticalCar(Npc2X, Npc2Y, colors[RED]);
	// Drawing npc car three
	DrawVerticalCar(Npc3X, Npc3Y, colors[GRAY]);
	// Drawing npc car four. Will be in horizontal orientation
	DrawHorizontalCar(Npc4X, Npc4Y, colors[MAGENTA]);
	MoveVerticalNpcs(Npc1Y, Npc2Y, Npc3Y, Npc4X);
	glutPostRedisplay();
}
// This function calls the necessary functions for controlling the movement of NPC cars.
void MoveVerticalNpcs(int &Npc1Y, int &Npc2Y, int &Npc3Y, int &Npc4X)
{
	static bool NpcFlag1 = true, NpcFlag2 = true, NpcFlag3 = true, NpcFlag4 = true;
	VerticalNPCMovement(Npc1Y, 30, 310, 1, NpcFlag1);
	VerticalNPCMovement(Npc2Y, 30, 510, 1, NpcFlag2);
	VerticalNPCMovement(Npc3Y, 270, 610, 1, NpcFlag3);
	HorizontalNPCMovement(Npc4X, 960, 620, 1, NpcFlag4);
}
// This function draws the main menu of the game.
void Menu()
{
	glClearColor(0 /*Red Component*/, 0, /*Green Component*/ 0 /*Blue Component*/, 0 /*Alpha component*/);
	glClear(GL_COLOR_BUFFER_BIT); //Update the colors
	DrawString(440, 750, "Welcome to Rush Hour", colors[BLUE]);
	if (MenuCounter == 0)
		DrawRectangle(450, 475, 100, 30, colors[PINK]);
	else if (MenuCounter == 1)
		DrawRectangle(450, 435, 100, 30, colors[PINK]);
	DrawString(465, 480, "Main Menu", colors[RED]);
	DrawString(460, 440, "Leaderboards", colors[RED]);
	glutSwapBuffers();
}
// This function draws the taxi selection menu of the game.
void TaxiSelection()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clean the screen and the depth buffer
	glLoadIdentity();									// Reset The Projection Matrix
	glClearColor(0 /*Red Component*/, 0, /*Green Component*/ 0 /*Blue Component*/, 0 /*Alpha component*/);
	glClear(GL_COLOR_BUFFER_BIT); //Update the colors
	DrawString(440, 750, "Please select your taxi color", colors[BLUE]);
	if (TaxiMenuCounter == 0)
		DrawRectangle(450, 475, 100, 30, colors[PINK]);
	else if (TaxiMenuCounter == 1)
		DrawRectangle(455, 435, 100, 30, colors[PINK]);
	DrawString(460, 480, "Red Taxi", colors[RED]);
	DrawString(460, 440, "Yellow Taxi", colors[RED]);
	//glutPostRedisplay();
	glutSwapBuffers();
	glutPostRedisplay();
}
// This function handles the leaderboard and the file reading part.
void Leaderboard()
{
	int HighScores[10];
	string NamesHighScores[10];
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clean the screen and the depth buffer
	glLoadIdentity();									// Reset The Projection Matrix
	glClearColor(0.8, 0.8, 0.8 /*Blue Component*/, 0 /*Alpha component*/);
	glClear(GL_COLOR_BUFFER_BIT);
	DrawString(440, 750, "Welcome to Leaderboards", colors[BLUE]);
	fstream Records;
	Records.open("highscores.txt", ios::in);
	if (Records)
	{
		int index = 0;
		int num;
		string name;
		while (Records >> name >> num)
		{
			*(HighScores + index) = num;
			*(NamesHighScores + index) = name;
			index++;
		}
		// Check whether array is sorted.
		for (int j = 0; j < 10; j++)
		{
			for (int i = 0; i < 10; i++)
			{
				if (*(HighScores + j) > *(HighScores + i))
				{
					Swap(HighScores + i, HighScores + j);
					Swap(NamesHighScores + i, NamesHighScores + j);
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < 10; i++)
		{
			HighScores[i] = 0;
			NamesHighScores[i] = "N/A";
		}
	}
	DrawString(200, 700, "Numbers", colors[RED]);
	DrawString(460, 700, "Names", colors[RED]);
	DrawString(720, 700, "Points", colors[RED]);
	int YAXIS = 660;
	for (int i = 0; i < 10; i++, YAXIS -= 30)
	{
		string number = Num2Str(i);
		string number1 = Num2Str(HighScores[i]);
		DrawString(220, YAXIS, number, colors[RED]);
		DrawString(470, YAXIS, NamesHighScores[i], colors[RED]);
		DrawString(735, YAXIS, number1, colors[RED]);
	}
	DrawString(435, 310, "Press ESC key to go back to menu", colors[RED]);
	glutSwapBuffers();
	glutPostRedisplay();
}
// This function draws the main canvas of the game.
void GameDisplay() /**/
{
	// Note that r, g and b values must be in the range [0,1] where 0 means dim rid and 1 means pure red and so on.
	glClearColor(1 /*Red Component*/, 1, /*Green Component*/ 1 /*Blue Component*/, 0 /*Alpha component*/);
	glClear(GL_COLOR_BUFFER_BIT); //Update the colors
	//Display Score
	const string display = "Score = ";
	string points = Num2Str(score);
	string final = display + points;
	DrawString(450, 825, final, colors[BLUE_VIOLET]);
	string MIN = Num2Str(Minutestimer);
	string SEC = Num2Str(Secondstimer);
	const string colon = " : ";
	string TIMEDISPLAY = MIN + colon + SEC;
	DrawString(850, 825, TIMEDISPLAY, colors[BLUE_VIOLET]);
	//DrawLine(int x1, int y1, int x2, int y2, int lwidth, float *color)
	int Xaxis = 10, InitialYAxis = 820, EndYAxis = -840;
	for (int i = 1; i <= 21; i++)
	{
		DrawLine(Xaxis, InitialYAxis, Xaxis, EndYAxis, 1, colors[BLACK]);
		Xaxis += 50;
	}
	DrawLine(10, 820, 1010, 820, 1, colors[BLACK]);

	int i = 0;

	DrawHorizontalObstacles(60, 720, 160, i);
	DrawHorizontalObstacles(460, 720, 960, i);
	DrawHorizontalObstacles(210, 520, 260, i);
	DrawVerticalObstacles(110, 510, 370, i);
	DrawVerticalObstacles(310, 670, 510, i);
	DrawVerticalObstacles(610, 610, 360, i);
	DrawHorizontalObstacles(660, 510, 760, i);
	DrawVerticalObstacles(910, 610, 410, i);
	DrawVerticalObstacles(460, 360, 310, i);
	DrawHorizontalObstacles(360, 310, 410, i);
	DrawHorizontalObstacles(110, 260, 160, i);
	DrawHorizontalObstacles(110, 210, 160, i);
	DrawHorizontalObstacles(110, 160, 160, i);
	DrawVerticalObstacles(260, 310, 110, i);
	DrawHorizontalObstacles(360, 210, 560, i);
	DrawVerticalObstacles(710, 210, 110, i);
	DrawHorizontalObstacles(760, 210, 960, i);
	DrawVerticalObstacles(460, 110, 10, i);

	ObstacleLimiter = i;
	int TreeCounter = 0, BoxCounter = 0, PassengerCounter = 0;
	// Drawing trees.
	DrawTree(265, 600, TreeCounter);
	DrawTree(415, 390, TreeCounter);
	DrawTree(815, 290, TreeCounter);
	DrawTree(715, 590, TreeCounter);
	TreeLimiter = TreeCounter;
	// Drawing boxes.
	DrawBox(815, 90, BoxCounter);
	DrawBox(315, 470, BoxCounter);
	DrawBox(715, 465, BoxCounter);
	BoxLimiter = BoxCounter;
	// Drawing passengers.
	if (DisplayPassenger[0])
		DrawPassenger(780, 605, PassengerCounter);
	else
	{
		static int index = GetRandInRange(0, 4);
		IndexComparison = index;
		DrawCircle(LocationArray[index][0], LocationArray[index][1], 20, colors[GREEN]);
		if (AtLocation && SpaceKeyPressed)
		{
			cout << "If ran";
			DrawPassenger(LocationArray[index][0], LocationArray[index][1]);
			AtLocation = false;
			SpaceKeyPressed = false;
		}
	}
	DrawPassenger(850, 605, PassengerCounter);
	 if (DisplayPassenger[1])
		DrawPassenger(390, 400, PassengerCounter);
		else
		{
			static int index = GetRandInRange(0,4);
			DrawCircle(LocationArray[index][0],LocationArray[index][1],20,colors[GREEN]);
		}
	if (DisplayPassenger[2])
		DrawPassenger(165, 355, PassengerCounter);
		else
		{
			static int index = GetRandInRange(0,4);
			DrawCircle(LocationArray[index][0],LocationArray[index][1],20,colors[GREEN]);
		}
	if (DisplayPassenger[3])
		DrawPassenger(920, 305, PassengerCounter);
		else
		{
			static int index = GetRandInRange(0,4);
			DrawCircle(LocationArray[index][0],LocationArray[index][1],20,colors[GREEN]);
		} 
	PassengerLimiter = PassengerCounter;
	unsigned int X = 0, Y = 0;
	if (NearPassenger[0] || NearPassenger[1] || NearPassenger[2] || NearPassenger[3])
		DrawString(750, 825, "Press Space", colors[BLUE_VIOLET]);
	DrawCircle(915, 145, 20, colors[GREEN]);
	DrawNpcCar();
	DrawMainTaxi();
	glutSwapBuffers(); // do not modify this line..
}
void NonPrintableKeys(int key, int x, int y)
{
	bool Collisioncheck = false, TreeCollision = false, BoxCollision = false;
	if (key == GLUT_KEY_LEFT /*GLUT_KEY_LEFT is constant and contains ASCII for left arrow key*/)
	{
		for (int k = 0; k <= ObstacleLimiter; k++)
		{
			for (int Xlimit = ObstacleArray[k][0], Ylimit = ObstacleArray[k][1]; Ylimit <= ObstacleArray[k][1] + 50; Ylimit++)
			{
				if (xI == Xlimit + 40 && (yI == Ylimit || yI == Ylimit - 30))
				{
					Collisioncheck = true;
					xI += 20;
					break;
				}
			}
		}
		for (int m = 0; m <= BoxLimiter; m++)
		{
			for (int X = BoxArray[m][0], Y = BoxArray[m][1]; Y <= BoxArray[m][1] + 50; Y++)
			{
				if (xI == X + 40 && (yI == Y || yI == Y - 30))
				{
					BoxCollision = true;
					cout << "Box Collision detected by left key\n";
					xI += 20;
					if (YellowTaxi)
						score -= 4;
					else
						score -= 2;
					break;
				}
			}
		}
		for (int j = 0; j <= TreeLimiter; j++)
		{
			for (int X = TreeArray[j][0], Y = TreeArray[j][1] - 30; Y <= TreeArray[j][1] + 30; Y++)
			{
				if (xI == X + 40 && yI == Y)
				{
					TreeCollision = true;
					cout << "Tree Collision detected by left key\n";
					xI += 20;
					if (YellowTaxi)
						score -= 4;
					else
						score -= 2;
					break;
				}
			}
		}
		for (int m = 0; m <= PassengerLimiter; m++)
		{
			for (int X = PassengerArray[m][0], Y = PassengerArray[m][1] - 150; Y <= PassengerArray[m][1] + 160; Y++)
			{
				if (xI == X + 40 && yI == Y)
				{
					NearPassenger[m] = true; //Near Passenger Check
					break;
				}
			}
		}
		for (int n = 0; n <= PassengerLimiter; n++)
		{
			for (int X = PassengerArray[n][0], Y = PassengerArray[n][1] - 50; Y <= PassengerArray[n][1] + 10; Y++)
			{
				if (xI == X + 15 && yI == Y)
				{
					cout << "Passenger Collision detected by left key\n";
					xI += 20;
					score -= 5;
					break;
				}
			}
		}
		/* for (int X = 915, Y = 145 - 45; Y <= 145 + 50; Y++) */
		 for (int X = LocationArray[IndexComparison][0], Y = LocationArray[IndexComparison][1] - 45; Y <= LocationArray[IndexComparison][1] + 50; Y++)
		{
			if (xI == X + 15 && yI == Y)
			{
				AtLocation = true; // Check for when passenger reaches location
				break;
			}
		} 
		if (!Collisioncheck && !BoxCollision && !TreeCollision)
		{
			if (YellowTaxi)
				xI -= 5;
			else
				xI -= 1;
			Horizontal = true;
		}
	}
	else if (key == GLUT_KEY_RIGHT /*GLUT_KEY_RIGHT is constant and contains ASCII for right arrow key*/)
	{
		for (int k = 0; k <= ObstacleLimiter; k++)
		{
			for (int Xlimit = ObstacleArray[k][0], Ylimit = ObstacleArray[k][1]; Ylimit <= ObstacleArray[k][1] + 50; Ylimit++)
			{
				if (xI + 40 == Xlimit && (yI == Ylimit || yI + 30 == Ylimit))
				/* if (xI == Xlimit - 40 && (yI == Ylimit || yI == Ylimit - 30)) */
				{
					Collisioncheck = true;
					xI -= 20;
					break;
				}
			}
		}
		for (int m = 0; m <= BoxLimiter; m++)
		{
			for (int X = BoxArray[m][0], Y = BoxArray[m][1]; Y <= BoxArray[m][1] + 50; Y++)
			{
				if (xI + 40 == X && (yI == Y || yI + 30 == Y))
				{
					BoxCollision = true;
					cout << "Box Collision detected by right key\n";
					xI -= 20;
					if (YellowTaxi)
						score -= 4;
					else
						score -= 2;
					break;
				}
			}
		}
		for (int j = 0; j <= TreeLimiter; j++)
		{
			for (int X = TreeArray[j][0], Y = TreeArray[j][1] - 30; Y <= TreeArray[j][1] + 30; Y++)
			{
				if (xI + 40 == X && yI == Y)
				{
					TreeCollision = true;
					cout << "Tree Collision detected by right key\n";
					xI -= 20;
					if (YellowTaxi)
						score -= 4;
					else
						score -= 2;
					break;
				}
			}
		}
		for (int m = 0; m <= PassengerLimiter; m++)
		{
			for (int X = PassengerArray[m][0], Y = PassengerArray[m][1] - 120; Y <= PassengerArray[m][1] + 40; Y++)
			{
				if (xI + 100 == X && yI == Y)
				{
					NearPassenger[m] = true; // Near Passenger Check
				}
			}
		}
		for (int n = 0; n <= PassengerLimiter; n++)
		{
			for (int X = PassengerArray[n][0], Y = PassengerArray[n][1] - 70; Y <= PassengerArray[n][1] + 10; Y++)
			{
				if (xI + 45 == X && yI == Y)
				{
					cout << "Passenger Collision detected by right key\n";
					xI -= 20;
					score -= 5;
					break;
				}
			}
		}
		if (!Collisioncheck && !BoxCollision && !TreeCollision)
		{
			if (YellowTaxi)
				xI += 5;
			else
				xI += 1;
			Horizontal = true;
		}
	}
	else if (key == GLUT_KEY_UP /*GLUT_KEY_UP is constant and contains ASCII for up arrow key*/)
	{
		if (DisplayMenu && MenuCounter > 0)
		{
			MenuCounter--;
		}
		else if (DisplayTaxiMenu && TaxiMenuCounter > 0)
		{
			TaxiMenuCounter--;
		}
		for (int k = 0; k <= ObstacleLimiter; k++)
		{
			for (int Xlimit = ObstacleArray[k][0] - 30, Ylimit = ObstacleArray[k][1]; Xlimit <= ObstacleArray[k][0] + 40; Xlimit++)
			{
				if (xI == Xlimit && yI + 40 == Ylimit)
				{
					Collisioncheck = true;
					cout << " Obstacle Collision detected by up key\n";
					yI -= 20;
					break;
				}
			}
		}
		for (int m = 0; m <= BoxLimiter; m++)
		{
			for (int X = BoxArray[m][0] - 30, Y = BoxArray[m][1]; X <= BoxArray[m][0] + 40; X++)
			{
				if (xI == X && yI + 40 == Y)
				{
					BoxCollision = true;
					cout << "Box Collision detected by up key\n";
					yI -= 20;
					if (YellowTaxi)
						score -= 4;
					else
						score -= 2;
					break;
				}
			}
		}
		for (int j = 0; j <= TreeLimiter; j++)
		{
			for (int X = TreeArray[j][0] - 30, Y = TreeArray[j][1]; Y <= TreeArray[j][1] + 40; Y++)
			{
				if (xI + 40 == X && yI + 40 == Y)
				{
					TreeCollision = true;
					cout << "Tree Collision detected by up key\n";
					yI -= 20;
					if (YellowTaxi)
						score -= 4;
					else
						score -= 2;
					break;
				}
			}
		}
		for (int n = 0; n <= PassengerLimiter; n++)
		{
			for (int X = PassengerArray[n][0] - 35, Y = PassengerArray[n][1] - 100; X <= PassengerArray[n][0] + 10; X++, Y++)
			{
				if (xI == X && yI == Y)
				{
					cout << "Passenger Collision detected by up key\n"; // Near Passenger Check
					score -= 5;
					yI -= 20;
					break;
				}
			}
		}
		if (!Collisioncheck && !BoxCollision && !TreeCollision)
		{
			if (YellowTaxi)
				yI += 5;
			else
				yI += 1;
			Horizontal = false;
		}
	}

	else if (key == GLUT_KEY_DOWN /*GLUT_KEY_DOWN is constant and contains ASCII for down arrow key*/)
	{
		if (DisplayMenu && MenuCounter == 0)
		{
			MenuCounter++;
		}
		else if (DisplayTaxiMenu && TaxiMenuCounter == 0)
		{
			TaxiMenuCounter++;
		}
		for (int k = 0; k <= ObstacleLimiter; k++)
		{
			for (int Xlimit = ObstacleArray[k][0] - 30, Ylimit = ObstacleArray[k][1]; Xlimit <= ObstacleArray[k][0] + 45; Xlimit++)
			{
				if (xI == Xlimit && yI - 40 == Ylimit)
				/* if (xI == Xlimit && yI == Ylimit + 40) */ // Do later
				{
					Collisioncheck = true;
					cout << "Collision detected by down key\n";
					yI += 20;
					break;
				}
			}
		}
		for (int m = 0; m <= BoxLimiter; m++)
		{
			for (int X = BoxArray[m][0] - 30, Y = BoxArray[m][1]; X <= BoxArray[m][0] + 45; X++)
			{
				if (xI == X && yI - 40 == Y)
				{
					BoxCollision = true;
					cout << "Box Collision detected by down key\n";
					if (YellowTaxi)
						score -= 4;
					else
						score -= 2;
					yI += 20;
					break;
				}
			}
		}
		for (int j = 0; j <= TreeLimiter; j++)
		{
			for (int X = TreeArray[j][0] - 25, Y = TreeArray[j][1] + 25; X <= TreeArray[j][0] + 39; X++)
			{
				if (xI == X && yI == Y)
				{
					TreeCollision = true;
					cout << "Tree Collision detected by down key\n";
					if (YellowTaxi)
						score -= 4;
					else
						score -= 2;
					yI += 20;
					break;
				}
			}
		}
		for (int n = 0; n <= PassengerLimiter; n++)
		{
			for (int X = PassengerArray[n][0] - 35, Y = PassengerArray[n][1] + 25; X <= PassengerArray[n][0] + 10; X++)
			{
				if (xI == X && yI == Y)
				{
					cout << "Passenger Collision detected by down key\n";
					score -= 5;
					yI += 20;
					break;
				}
			}
		}
		for (int m = 0; m <= PassengerLimiter; m++)
		{
			for (int X = PassengerArray[m][0] - 100, Y = PassengerArray[m][1] + 45; X <= PassengerArray[m][0] + 60; X++)
			{
				if (xI == X && yI == Y)
				{
					NearPassenger[m] = true;
					break;
				}
			}
		}
		if (!Collisioncheck && !BoxCollision && !TreeCollision)
		{
			if (YellowTaxi)
				yI -= 5;
			else
				yI -= 1;
			Horizontal = false;
		}
	}
	glutPostRedisplay();
}

void PrintableKeys(unsigned char key, int x, int y)
{
	static int index = 0;
	/* static bool NearPassengerOne = false;
	 for (int i = 0; i <= PassengerLimiter; i++)
	{
		for (int X= PassengerArray[i][0] - 200, Y = PassengerArray[i][1] - 200; X <= PassengerArray[i][0] + 250; X++,Y++)
		{
			if (xI == X && yI  == Y)
			{
				NearPassengerOne = true;
				break;
			}
		}
	} */
	if (key == 27 /* Escape key ASCII*/)
	{
		if (DisplayLeaderboard)
		{
			DisplayLeaderboard = false;
			DisplayMenu = true;
		}
		// exit the program when escape key is pressed.
	}
	if (key == ' ') //Key for placing the bomb
	{
		if (AtLocation)
			SpaceKeyPressed = true;
		cout << "Space key pressed" << endl;
		for (int m = 0; m <= PassengerLimiter; m++)
		{
			if (NearPassenger[m])
			{
				if (m == 0 || m == 1 || m == 2 || m == 3 || m == 4)
				{
					DisplayPassenger[m] = false;
					NearPassenger[m] = false;
					break;
				}
			}
		}
	}
	if (key == 13)
	{
		cout << "Enter key pressed\n";
		if (DisplayMenu)
		{
			if (MenuCounter == 0)
			{
				DisplayMenu = false;
				DisplayTaxiMenu = true;
			}
			else if (MenuCounter == 1)
			{
				DisplayMenu = false;
				DisplayLeaderboard = true;
			}
		}
		else if (DisplayTaxiMenu)
		{
			if (TaxiMenuCounter == 0)
				YellowTaxi = false;
			else if (TaxiMenuCounter == 1)
				YellowTaxi = true;
			DisplayTaxiMenu = false;
			TakePlayerName = true;
		}
		else if (TakePlayerName)
		{
			TakePlayerName = false;
			Secondstimer = 0;
			MainGame = true;
		}
	}
	if ((key >= 65 && key <= 90) || (key >= 97 && key <= 122) && TakePlayerName)
	{
		UserName += key;
		index++;
	}
	if ((key == 8 || key == 127) && TakePlayerName)
	{
		UserName[index] -= UserName[index];
		index--;
	}
	glutPostRedisplay();
}
void Timer(int m)
{
	//cout << xI << '\n'<< yI << '\n';
	if (MilliSecondsTimer == 60)
	{
		Secondstimer++;
		MilliSecondsTimer = 0;
	}
	if (Secondstimer == 60)
	{
		Minutestimer++;
		Secondstimer = 0;
	}
	MilliSecondsTimer += 5;
	// once again we tell the library to call our Timer function after next 1000/FPS
	glutTimerFunc(100, Timer, 0);
}
void MousePressedAndMoved(int x, int y)
{
	//cout << x << " " << y << endl;
	glutPostRedisplay();
}
void MouseMoved(int x, int y)
{
	//cout << x << " " << y << endl;
	glutPostRedisplay();
}
void MouseClicked(int button, int state, int x, int y)
{

	if (button == GLUT_LEFT_BUTTON) // dealing only with left button
	{
		cout << GLUT_DOWN << " " << GLUT_UP << endl;
	}
	else if (button == GLUT_RIGHT_BUTTON) // dealing with right button
	{
		cout << "Right Button Pressed" << endl;
	}
	glutPostRedisplay();
}
#endif /* RushHour_CPP_ */

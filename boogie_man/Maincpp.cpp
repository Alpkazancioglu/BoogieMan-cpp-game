#define RCAMERA_IMPLEMENTATION
#include <iostream>
#include <raylib.h>
#include "rcamera.h"
#include <vector>
#include <chrono>
#include "GameHandler.h"
#include "FileHandler.h"

game* currentgame;

int main()
{
	currentgame = new game(512, 380);
	currentgame->initialize("BoogieMan", 60);
	while (!WindowShouldClose())
	{

		currentgame->update();

		currentgame->draw();
	}
	
	currentgame->Clean(currentgame);

	return 0;
}
#include <iostream>
#include <raylib.h>
#include <vector>
#include <chrono>
#include "GameHandler.h"
#include "../util/FileHandler.h"

game* currentgame;

int main()
{
	currentgame = new game(512, 380);
	currentgame->initialize("BoogieMan", 144);
	while (!WindowShouldClose())
	{

		currentgame->update();

		currentgame->draw();
	}
	
	currentgame->Clean(currentgame);

	return 0;
}
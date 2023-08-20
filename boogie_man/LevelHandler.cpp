#include "LevelHandler.h"
#include <fstream>
#include <iostream>

void GameLevel::ReadLevel()
{
	Creature cre;

	json j;
	
	std::ifstream inputFile("level.json");
	inputFile >> j;
	inputFile.close();

	cre.name = j["name"];
	cre.age = j["age"];

	std::cout << "Name: " << cre.name << " age: " << cre.age << std::endl;
}

void GameLevel::WriteLevel()
{

	Creature cre;

	json j;
	j["name"] = cre.name;
	j["age"] = cre.age;

	std::ofstream outputfile("level.json");
	outputfile << j;
	outputfile.close();

}

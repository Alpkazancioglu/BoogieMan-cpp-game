#pragma once
#include <chrono>
#include <iostream>
#include <raylib.h>
#include <vector>
#include <bitset>
#include <memory>
#include "core/game.h"
#include "ECS.h"
#include "FileHandler.h"
#include "StopWatch.h"

class somestaticclass
{
public:


	static void foo(){};

};

class game
{
public:

	game(int screenw, int screenh);

	void initialize(const char* windowname , int fps);
	void update();
	void updatescreenrec();
	void draw();
	void fullscreen_g(int screenw_a, int screenh_a);
	void Clean(game* currentgame);
	void Update_Camera(Vector2 target);
	float entity_object_pos_calculate_x(float position, Rectangle initialrec);
	float entity_object_pos_calculate_y(float position, Rectangle initialrec);
	

private:

	int screenw;
	int screenh;
	std::unique_ptr<Camera2D> MainCamera;
	Rectangle screenrec;

	ecs* newecs;
	std::unique_ptr<Alpino> AlpinoGame;
	std::shared_ptr<RenderTexture2D> target;
	float scale;
	stopwatch_t newwatch;
};







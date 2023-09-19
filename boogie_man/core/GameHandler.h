#pragma once
#include <chrono>
#include <iostream>
#include <raylib.h>
#include <vector>
#include <bitset>
#include <memory>
#include "game.h"
#include "../ECS.h"
#include "../util/FileHandler.h"
#include "../util/StopWatch.h"

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
	void Update_Camera(Vector2 target , Vector2 Offset , Vec2<float> Zoom);
	float entity_object_pos_calculate_x(float position, Rectangle initialrec);
	float entity_object_pos_calculate_y(float position, Rectangle initialrec);
	

private:

	int screenw;
	int screenh;
	std::unique_ptr<Camera2D> MainCamera;
	Rectangle screenrec;

	ecs* newecs;
	std::unique_ptr<BoogieMan> BoogieManGame;
	std::shared_ptr<RenderTexture2D> target;
	float scale;
	stopwatch_t newwatch;

	bool DrawShadowMapFBO = true;
};







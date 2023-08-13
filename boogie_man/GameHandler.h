#pragma once
#include <chrono>
#include <iostream>
#include <raylib.h>
#include <vector>
#include <bitset>

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
	Camera2D* MainCamera;
	Rectangle screenrec;
};

int GiveRandomNumf(int min, int max, int sizeofarray, bool exclude_on_off, int numtoexclude);





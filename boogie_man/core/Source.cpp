#include <raylib.h>
#include <iostream>
#include "far_background.h"
#include "killua.h"
#include "nebula.h"
#include "game.h"
#include "middle_Background.h"
#include "foreground.h"
#include <vector>
#define REAL  0
#define DUPLICATE  1

/*
int main()
{
	const int WindowWidth = 512;
	const int WindowHeight = 380;
	const int sizeofnebula = 8;
	int gravity = 1000;
	float posy = WindowHeight - 40;
	float posx = WindowWidth / 2;
	float far_bgx = 0;
	float dt = 0;
	bool allow = true;
	bool onAir = false;
	bool collusion = false;
	SetTargetFPS(60);
	InitWindow(WindowWidth, WindowHeight, "alpino");

	Texture2D nebula;
	Texture2D killua;
	Texture2D far_background;
	Texture2D middle_background;
	Texture2D t_foreground;

	LoadTexture2DfromHeader(&killua, KILLUA_FORMAT, KILLUA_HEIGHT, KILLUA_WIDTH, KILLUA_DATA, 1);
	LoadTexture2DfromHeader(&nebula, NEBULA_FORMAT, NEBULA_HEIGHT, NEBULA_WIDTH, NEBULA_DATA, 1);
	LoadTexture2DfromHeader(&far_background, FAR_BACKGROUND_FORMAT, FAR_BACKGROUND_HEIGHT, FAR_BACKGROUND_WIDTH, FAR_BACKGROUND_DATA, 1);
	LoadTexture2DfromHeader(&middle_background, MIDDLE_BACKGROUND_FORMAT, MIDDLE_BACKGROUND_HEIGHT, MIDDLE_BACKGROUND_WIDTH, MIDDLE_BACKGROUND_DATA, 1);
	LoadTexture2DfromHeader(&t_foreground, FOREGROUND_FORMAT, FOREGROUND_HEIGHT, FOREGROUND_WIDTH, FOREGROUND_DATA, 1);

	Animdata killuaData
	{
		{0,0,killua.width / 6,killua.height}, // rectangle
		{(WindowWidth / 2) - (killuaData.rec.width / 2),WindowHeight - killuaData.rec.height} , // pos
		0, // frame
		0, // running time
		1.0 / 12.0, // uptade time
		0 // speed
	};

	Animdata nebulaData[sizeofnebula]{};
	hitboxs nebulaHitboxs[sizeofnebula]{};
	for (int i = 0; i < sizeofnebula; i++)
	{
		nebulaHitboxs[i].radius = 67.0 / 2;
		nebulaData[i].rec.x = 0.0;
		nebulaData[i].rec.y = 0.0;
		nebulaData[i].rec.width = nebula.width / 8;
		nebulaData[i].rec.height = nebula.height / 8;
		if (i == 7)
		{
			nebulaData[i].pos.x = WindowWidth;
			nebulaHitboxs[i].pos.x = WindowWidth+(nebulaHitboxs[i].radius+18);
		}
		else
		{
			nebulaData[i].pos.x = WindowWidth + (300 * i);
			nebulaHitboxs[i].pos.x = nebulaData[i].pos.x + (nebulaHitboxs[i].radius+18);
		}
		nebulaData[i].pos.y = WindowHeight - nebula.height / 8;
		nebulaHitboxs[i].pos.y = nebulaData[i].pos.y + (nebulaHitboxs[i].radius+18);
		nebulaData[i].runningtime = 0;
		nebulaData[i].updatetime = 1.0 / 12.0;
		nebulaData[i].speed = 200;
		
	}
	
	
	std::vector<AnimBackground> farbackground;
	farbackground = initbackgrounds(far_background, 80, farbackground);
	std::vector<AnimBackground> middlebackground;
	middlebackground = initbackgrounds(middle_background, 110, middlebackground);
	std::vector<AnimBackground>foreground;
	foreground = initbackgrounds(t_foreground, 160, foreground);

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(RAYWHITE);
		dt = GetFrameTime();
		
		DrawandUptadebackgrounds(farbackground, dt);
		DrawandUptadebackgrounds(middlebackground, dt);
		DrawandUptadebackgrounds(foreground, dt);

		killuaData = updateAnimdata(killuaData, dt, 5, onAir);
		killuaData.pos.y += killuaData.speed * dt;
		nebulaData[7] = updateAnimdata(nebulaData[7], dt, sizeofnebula, false);

		for (int i = 0; i < sizeofnebula - 1; i++)
		{
			nebulaData[i] = updateAnimdata(nebulaData[i], dt, sizeofnebula, false);
			nebulaData[i].pos.x -= nebulaData[i].speed * dt;
			nebulaHitboxs[i].pos.x -= nebulaData[i].speed * dt;
			DrawTextureRec(nebula, nebulaData[i].rec, nebulaData[i].pos, WHITE);
			DrawCircleV(nebulaHitboxs[i].pos, nebulaHitboxs[i].radius, WHITE);
		}
		
		for (int i = 0; i < sizeofnebula - 1; i++)
		{
			if (isObjectOut(nebulaData[sizeofnebula - 2]))
			{
				nebulaData[i].pos.x = WindowWidth + (300 * i);
				nebulaHitboxs[i].pos.x = WindowWidth + (300 * i) + (nebulaHitboxs[i].radius + 18);
			}

		}
		
		if (isObjectOut(nebulaData[sizeofnebula - 3]) || !allow)
		{

			nebulaData[7].pos.x -= nebulaData[7].speed * dt;
			nebulaHitboxs[7].pos.x -= nebulaData[7].speed * dt;
			DrawTextureRec(nebula, nebulaData[7].rec, nebulaData[7].pos, WHITE);
			allow = false;
			if (isObjectOut(nebulaData[7]))
			{
				nebulaData[7].pos.x = WindowWidth;
				nebulaHitboxs[7].pos.x = WindowWidth + (nebulaHitboxs[7].radius + 18);
				allow = true;
			}
		}
		
		if (isOnGround(killuaData, WindowHeight))
		{
			killuaData.speed = 0;
			onAir = false;
		}
		
		else
		{
			killuaData.speed += gravity * dt;
			onAir = true;
		}
		
		if (IsKeyPressed(KEY_SPACE) && !onAir)
		{
			killuaData.speed = -600;
		}
		DrawTextureRec(killua, killuaData.rec, killuaData.pos, WHITE);
		
		for (int i = 0; i < sizeofnebula; i++)
		{
			if (CheckCollisionCircleRec(nebulaHitboxs[i].pos, nebulaHitboxs[i].radius, Rectangle{ killuaData.pos.x + 38,killuaData.pos.y,(float)(killua.width / 6) - 38,(float)killua.height - 20 }))
			{
				std::cout << "game is over" << std::endl;
				collusion = true;
			}
		}
		EndDrawing();
	}
	
	UnloadTexture(killua);
	UnloadTexture(nebula);
	UnloadTexture(far_background);
	UnloadTexture(middle_background);
	UnloadTexture(t_foreground);
	CloseWindow();
}
*/
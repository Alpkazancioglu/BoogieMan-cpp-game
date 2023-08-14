#include "game.h"
#include <iostream>
#include "far_background.h"
#include "killua.h"
#include "game.h"
#include "middle_Background.h"
#include "foreground.h"
#include <string>
Alpino::Alpino(Vec2<int> WindowSize)
{
	
	this->WindowWidth = WindowSize.x;
	this->WindowHeight = WindowSize.y;

	LoadTexture2DfromHeader(&killua, KILLUA_FORMAT, KILLUA_HEIGHT, KILLUA_WIDTH, KILLUA_DATA, 1);
	Alpino::LoadTexture2DfromHeader(&nebula, NEBULA_FORMAT, NEBULA_HEIGHT, NEBULA_WIDTH, NEBULA_DATA, 1);
	
	t_foreground = LoadTexture(GetRelativeTexturePath("forest.png").c_str());
	middle_background = LoadTexture(GetRelativeTexturePath("forestback.png").c_str());
	far_background = LoadTexture(GetRelativeTexturePath("forestfarback.png").c_str());
	t_Fronstones = LoadTexture(GetRelativeTexturePath("front_rocks.png").c_str());

	for (size_t i = 0; i < 8; i++)
	{
		nebulas[i] = std::make_unique<Nebula>(WindowHeight, WindowWidth , nebula);
	}

	killuaData = 
	{
		{0.0f,0.0f,(float)killua.width / 6,(float)killua.height}, // rectangle
		{(WindowWidth / 2) - (killuaData.rec.width / 2),(float)( - 200 + (t_foreground.height * 4) - (killua.height))} , // pos
		0, // frame
		0, // running time
		1.0 / 12.0, // uptade time
		0 // speed
	};

	farbackground = initbackgrounds(far_background, 80, 4.1,{-650,-150},0,2, farbackground);
	middlebackground = initbackgrounds(middle_background, 110, 4.1, { -650,-150 },0,2, middlebackground);
	foreground = initbackgrounds(t_foreground, 160, 4.1, { -650,-150 },0,2, foreground);
	fronstones = initbackgrounds(t_Fronstones, 200, 1, { -550,(float)(getWsize().y/2)+410},0, 5, fronstones);
	
}

Alpino::~Alpino()
{
	UnloadTexture(killua);
	UnloadTexture(nebula);
	UnloadTexture(far_background);
	UnloadTexture(middle_background);
	UnloadTexture(t_foreground);
	UnloadTexture(t_Fronstones);
}

void Alpino::update(RenderTexture2D *fbo)
{

	switch (this->GAMESTAGE)
	{
	case INITIALPAGE:


	case INGAME:

		dt = GetFrameTime();
		
		DrawandUptadebackgrounds(farbackground, dt);
		DrawandUptadebackgrounds(middlebackground, dt);
		DrawandUptadebackgrounds(foreground, dt);
		
		for (int i = 0; i < 5; i++)
		{
			fronstones[i].pos = move(fronstones, dt, 1, i);
			drawStones(fronstones, 5);
		}
		killuaData = updateAnimdata(killuaData, dt, 5, onAir);
		killuaData.pos.y += killuaData.speed * dt;
		for (int i = 0; i < sizeofnebula; i++)
		{
			nebulas[i]->Data = updateAnimdata(nebulas[i]->Data, dt, sizeofnebula, false);
			nebulas[i]->Data.pos.x -= nebulas[i]->Data.speed * dt;
			nebulas[i]->Hitbox.pos.x -= nebulas[i]->Data.speed * dt;
			//DrawTextureRec(nebula, nebulas[i]->Data.rec, nebulas[i]->Data.pos, WHITE);
			//DrawCircleV(nebulas[i]->Hitbox.pos, nebulas[i]->Hitbox.radius, WHITE);
		}

		for (int i = 0; i < sizeofnebula; i++)
		{	
			rotatenebula(nebulas[i]->Data, getWsize().x , i);		
		}
		
		if (isOnGround(killuaData, GetMonitorHeight(GetCurrentMonitor())-killua.height+3))
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
		
		//std::cout << nebulas[1]->Data.rec.width + 700 << std::endl;
		DrawTextureRec(killua, killuaData.rec, killuaData.pos, WHITE);

		for (int i = 0; i < sizeofnebula; i++)
		{
			if (CheckCollisionCircleRec(nebulas[i]->Hitbox.pos, nebulas[i]->Hitbox.radius, Rectangle{ killuaData.pos.x + 38,killuaData.pos.y,(float)(killua.width / 6) - 38,(float)killua.height - 20 }))
			{
				//std::cout << "game is over" << std::endl;
				//GAMESTAGE = ENDPAGE;
			}
		}

	case ENDPAGE:
		
	default:
		break;
	}

	
}

void Alpino::draw(RenderTexture2D* fbo)
{
	switch (this->GAMESTAGE)
	{
	case INITIALPAGE:


	case INGAME:

		
	case ENDPAGE:


	default:
		break;
	}
}




bool Alpino::isObjectOut(Animdata data)
{
	
	//return data.pos.x <= -((int)data.rec.width << 3);
	return (data.pos.x <= (-700));
}

std::string Alpino::GetRelativeTexturePath(std::string textureName)
{
	std::string WorkingDir(GetWorkingDirectory());

	for (size_t i = 0; i < WorkingDir.size(); i++)
	{
		if (WorkingDir.at(i) == '\\') {
			WorkingDir.at(i) = '/';
		}
	}

	std::cout << "WORKING DIR: " << (WorkingDir + "/textures/" + textureName).c_str() << std::endl;

	return (WorkingDir + "/textures/" + textureName);
}

void Alpino::rotatenebula(Animdata data, int windowwidth,int index)
{
	if (isObjectOut(data))
	{
		nebulas[index]->Data.pos.x = windowwidth - 200;
		nebulas[index]->Hitbox.pos.x = windowwidth - 200 + (nebulas[index]->Hitbox.radius+18);
	}
	
}
Animdata Alpino::updateAnimdata(Animdata data, float dt, int maxframe, bool onair)
{
	if (onair)
		return data;
	else
	{
		data.runningtime += dt;
		if (data.updatetime - data.runningtime <= 0)
		{
			data.runningtime = 0.0;
			data.rec.x = data.frame * data.rec.width;
			data.frame++;
			if (data.frame > maxframe)
				data.frame = 0;
		}
		return data;
	}
}
Vector2 Alpino::uptadebackgrounds(std::vector<AnimBackground>& data, float dt, bool duplicate)
{
	
	
	if (duplicate)
	{
		data[1].pos.x = data[0].pos.x + data[1].texture.width * data[1].scale;
		return data[1].pos;
	}
	else
	{
		data[0].pos.x -= data[0].speed * dt;
		if (data[0].pos.x - data[REAL].origin.x<= -data[0].texture.width * data[0].scale)
		{
			return data[REAL].origin;
		}
		else
		{
			return data[0].pos;
		}
	}

}


std::vector<AnimBackground> Alpino::initbackgrounds(Texture2D texture, float speed,float scale, Vector2 pos ,float distance,int arraysize, std::vector<AnimBackground> &data)
{
	data.resize(arraysize);
	for (int i = 0; i < data.size(); i++)
	{
		data[i].texture = texture;
		data[i].speed = speed;
		data[i].scale = scale;
		data[i].distance = distance;
		data[i].arraysize = arraysize;
		data[i].origin = pos;
		if (i == 0)
		{
			data[0].pos = { pos.x,pos.y };
			data[0].duplicate = false;
		}
		else
		{
			data[i].pos = { data[i-1].pos.x + (texture.width *data[i].scale) + distance ,pos.y};
			data[i].duplicate = true;
		}
	};
	return data;
}

bool Alpino::isOnGround(Animdata data, int windowHeight)
{
	return data.pos.y + data.rec.height >= windowHeight;
}
void Alpino::DrawandUptadebackgrounds(std::vector<AnimBackground>& data, float dt)
{
	for (int i = 0; i < data.size(); i++)
	{
		data[i].pos = uptadebackgrounds(data, dt, data[i].duplicate);
		DrawTextureEx(data[i].texture, data[i].pos, 0.0, data[i].scale, WHITE);
	}

}
Vec2<int> getWsize()
{
	Vec2<int> temp(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()));
	return temp;
}


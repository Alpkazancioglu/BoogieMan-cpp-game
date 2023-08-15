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
	LoadTexture2DfromHeader(&nebula, NEBULA_FORMAT, NEBULA_HEIGHT, NEBULA_WIDTH, NEBULA_DATA, 1);
	
	t_foreground = LoadTexture(GetRelativeTexturePath("forest.png").c_str());
	middle_background = LoadTexture(GetRelativeTexturePath("forestback.png").c_str());
	far_background = LoadTexture(GetRelativeTexturePath("forestfarback.png").c_str());
	t_Fronstones = LoadTexture(GetRelativeTexturePath("front_rocks.png").c_str());
	CastleTexture = LoadTexture(GetRelativeTexturePath("CastleBack_small.png").c_str());
	t_woodenlog = LoadTexture(GetRelativeTexturePath("wooden_log.png").c_str());
	Castle_.SetTexture(CastleTexture);
	wooden_log.SetAnimData({}, { getWsize().x / 2 , 100 }, 0, 0, 0, 10);

	
	for (size_t i = 0; i < 8; i++)
	{
		nebulas[i] = std::make_unique<Nebula>(WindowHeight, WindowWidth , nebula);
	}

	killuaData =
	{
		{0.0f,0.0f,(float)killua.width / 6,(float)killua.height}, // rectangle
		{(getWsize().x / 2) - (killua.width / 12),getWsize().y - killua.height-26}, // pos
		0, // frame
		0, // running time
		1.0 / 12.0, // uptade time
		0 // speed
	};
	max_high = killuaData.pos.y - 100;
	farbackground = initbackgrounds(far_background, 80, 4.1,{-10,-80},0,2, farbackground);
	middlebackground = initbackgrounds(middle_background, 110, 4.1, { -10,-80 },0,2, middlebackground);
	foreground = initbackgrounds(t_foreground, 160, 4.1, { -10,-80 },0,2, foreground);
	fronstones = initbackgrounds(t_Fronstones, 200, 1, { 0,getWsize().y - (float)t_Fronstones.height}, 0, 5, fronstones);
	
}

Alpino::~Alpino()
{
	UnloadTexture(killua);
	UnloadTexture(nebula);
	UnloadTexture(far_background);
	UnloadTexture(middle_background);
	UnloadTexture(t_foreground);
	UnloadTexture(t_Fronstones);
	UnloadTexture(t_woodenlog);
	
}

void Alpino::update(RenderTexture2D *fbo)
{

	switch (this->GAMESTAGE)
	{
	case INITIALPAGE:


	case INGAME:

		dt = GetFrameTime();

		DrawRectangleGradientV(0,0,getWsize().x ,getWsize().y, { 90  , 125 , 151 , 255 } , {195 , 251 , 255 , 255});

		if (Castle_.Data.pos.x <= -Castle_.Texture->width * 0.7f)
		{
			Castle_.Move({ getWsize().x   , 100});
		}

		this->Castle_.IncrementPosition({ -(float)Castle_.Data.speed * dt , 0 });
		DrawTextureEx(CastleTexture, Castle_.Data.pos, 0.0f, 0.7f, {200,200,200,210});

		DrawandUptadebackgrounds(farbackground, dt , { 255,255,255,245 });
		DrawandUptadebackgrounds(middlebackground, dt , WHITE);
		DrawandUptadebackgrounds(foreground, dt , WHITE);
	
		for (int i = 0; i < 5; i++)
		{
			fronstones[i].pos = moveStones(fronstones, dt, 1, i);
			drawStones(fronstones, 5);
		}
		killuaData = updateAnimdata(killuaData, dt, 5);
		
		for (int i = 0; i < sizeofnebula; i++)
		{
			//nebulas[i]->Data = updateAnimdata(nebulas[i]->Data, dt, sizeofnebula);
			nebulas[i]->Data.rec.width = t_woodenlog.width;
			nebulas[i]->Data.rec.height = t_woodenlog.height;
			nebulas[i]->Data.pos.x -= nebulas[i]->Data.speed * dt;
			nebulas[i]->Hitbox.Data.pos.x -= nebulas[i]->Data.speed * dt;
			DrawTextureRec(t_woodenlog, nebulas[i]->Data.rec, nebulas[i]->Data.pos, WHITE);
			//DrawCircleV(nebulas[i]->Hitbox.Data.pos, nebulas[i]->Hitbox.radius, WHITE);
		}
		
		for (int i = 0; i < sizeofnebula; i++)
		{	
			RotateNebula(nebulas[i]->Data, getWsize().x , i);
		}
		
		KilluaJump();
		DrawTextureRec(killua, killuaData.rec, killuaData.pos, WHITE);
		
		for (int i = 0; i < sizeofnebula; i++)
		{
			if (CheckCollisionCircleRec(nebulas[i]->Hitbox.Data.pos, nebulas[i]->Hitbox.radius, Rectangle{ killuaData.pos.x + 38,killuaData.pos.y,(float)(killua.width / 6) - 38,(float)killua.height - 20 }))
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
	return (data.pos.x <= -nebulas[0]->Data.rec.width);
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

void Alpino::RotateNebula(Animdata data, int windowwidth,int index)
{
	if (isObjectOut(data))
	{
		int temp_i;
		temp_i = index - 1;
		if (temp_i == -1)
			temp_i = sizeofnebula - 1;
		nebulas[index]->Data.pos.x = nebulas[temp_i]->Data.pos.x+300;
		nebulas[index]->Hitbox.Data.pos.x = nebulas[temp_i]->Hitbox.Data.pos.x + 300;
	}
	
}
Animdata Alpino::updateAnimdata(Animdata data, float dt, int maxframe)
{
	if (!isOnGround(killuaData, getWsize().y) && data == killuaData)
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

bool Alpino::isOnGround(Animdata data, float windowHeight)
{
	return data.pos.y + data.rec.height >= getWsize().y-26;
}
void Alpino::DrawandUptadebackgrounds(std::vector<AnimBackground>& data, float dt, Color tint)
{
	for (int i = 0; i < data.size(); i++)
	{
		data[i].pos = uptadebackgrounds(data, dt, data[i].duplicate);
		DrawTextureEx(data[i].texture, data[i].pos, 0.0, data[i].scale, tint);
	}

}
inline Vector2 Alpino::moveStones(std::vector<AnimBackground>& data, float dt, float scale, int index)
{
	data[index].pos.x -= data[index].speed * dt;
	if (data[index].pos.x <= -data[index].texture.width)
	{
		int temp;
		temp = index - 1;
		if (temp == -1)
			temp = data[0].arraysize - 1;
		return { data[temp].pos.x + data[0].texture.width + data[0].distance,data[index].pos.y };
	}
	else
	{
		return data[index].pos;
	}
}
inline void Alpino::drawStones(std::vector<AnimBackground> data, unsigned int size)
{
	for (int i = 0; i < size; i++)
	{
		DrawTextureEx(data[i].texture, data[i].pos, 0, data[i].scale, WHITE);
	}
}
inline void Alpino::KilluaJump()
{
	current_high = killuaData.pos.y;
	killuaData.pos.y += killuaData.speed * dt;
	
	if (isOnGround(killuaData,getWsize().y))
	{
		killuaData.speed = 0;
		onAir = false;
		isplayerjumped = false;
	}
	else
	{
		killuaData.speed += gravity * dt;
	}		
	if (max_high >= current_high)
		onAir = true;
	if (IsKeyDown(KEY_SPACE) && !onAir && !isplayerjumped)
	{	
			killuaData.speed = -300;
	}
	else if (IsKeyReleased(KEY_SPACE))
			isplayerjumped = true;

}
Vec2<float> getWsize()
{
	Vec2<float> temp(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()));
	return temp;
}


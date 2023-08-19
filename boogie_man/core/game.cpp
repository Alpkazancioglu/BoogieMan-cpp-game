#include "game.h"
#include <iostream>
#include "far_background.h"
#include "killua.h"
#include "game.h"
#include "middle_Background.h"
#include "foreground.h"
#include <string>

#define FORESTPOSY 0

Alpino::Alpino(Vec2<int> WindowSize)
{
	
	this->WindowWidth = WindowSize.x;
	this->WindowHeight = WindowSize.y;

	//LoadTexture2DfromHeader(&killua_t, KILLUA_FORMAT, KILLUA_HEIGHT, KILLUA_WIDTH, KILLUA_DATA, 1);
	LoadTexture2DfromHeader(&nebula, NEBULA_FORMAT, NEBULA_HEIGHT, NEBULA_WIDTH, NEBULA_DATA, 1);
	
	t_foreground = LoadTexture(GetRelativeTexturePath("forest.png").c_str());
	middle_background = LoadTexture(GetRelativeTexturePath("forestback.png").c_str());
	far_background = LoadTexture(GetRelativeTexturePath("forestfarback.png").c_str());
	FrontVegetation_t = LoadTexture(GetRelativeTexturePath("front_rocks.png").c_str());
	CastleTexture = LoadTexture(GetRelativeTexturePath("CastleBack_small.png").c_str());
	t_woodenlog = LoadTexture(GetRelativeTexturePath("wooden_log.png").c_str());
	t_fog_cloud = LoadTexture(GetRelativeTexturePath("mist_atlas.png").c_str());
	Clouds_t = LoadTexture(GetRelativeTexturePath("Clouds.png").c_str());
	killua_t = LoadTexture(GetRelativeTexturePath("scarfy.png").c_str());


	wooden_log.SetAnimData({}, { getWsize().x / 2 , 100 }, 0, 0, 0, 10);
	float ForestScale = 3.8f;
	
	FrontVegetation.SetBaseAttributes(FrontVegetation_t, 1.0f, { {0,0,(float)FrontVegetation_t.width,(float)FrontVegetation_t.height},
	{0,getWsize().y - FrontVegetation_t.height-10},0,0,0,200}, 0.0f);
	
	
	castle.SetBaseAttributes(CastleTexture, 0.7f, { { 0,0,(float)CastleTexture.width , (float)CastleTexture.height },
		{ getWsize().x / 2 , 0 },
		0,
		0,
		0,
		10 }, 
		0.0f);
	
	fog_cloud.SetBaseAttributes(t_fog_cloud, 1.0f,
		{ {0,0,(float)t_fog_cloud.width / 4 ,(float)t_fog_cloud.height},
		{0,450},
		0,
		0,
		0.5f,
		135
		},
		0.0f);



	foreground_o.SetBaseAttributes(t_foreground, ForestScale, { {}, { 0, FORESTPOSY }, 0, 0, 0, 160 }, 0.0f);
	middlebackground_o.SetBaseAttributes(middle_background, ForestScale, { {}, { 0, FORESTPOSY }, 0, 0, 0, 110 }, 0.0f);
	farbackground_o.SetBaseAttributes(far_background, ForestScale, { {}, { 0, FORESTPOSY }, 0, 0, 0, 80 }, 0.0f);
	Clouds.SetBaseAttributes(Clouds_t, 1.0f, ObjectData({}, { 0, -20 }, 0, 0, 0, 80), 0.0f);
	farbackground_o.SetBaseAttributes(far_background, ForestScale, { {}, { 0, FORESTPOSY }, 0, 0, 0, 80 }, 0.0f);


	
	for (size_t i = 0; i < 8; i++)
	{
		nebulas[i] = std::make_unique<Nebula>(WindowHeight, WindowWidth , nebula);
	}

	killua.SetTexture(killua_t);
	killua.SetAnimData(
		
			{0.0f,0.0f,(float)(killua_t.width / 12),(float)killua_t.height}, // rectangle
			{(getWsize().x / 2) - (killua_t.width / 12),getWsize().y - killua_t.height - 26}, // pos
			5, // frame number
			0, // running time
			1.0 / 12.0, // uptade time
			0 // speed
		);
	max_high = killua.Data.pos.y - 100;


	Sky = std::make_unique<cubemap>(GetRelativeTexturePath("StandardCubeMap.png").c_str());
}

Alpino::~Alpino()
{
	UnloadTexture(killua_t);
	UnloadTexture(nebula);
	UnloadTexture(far_background);
	UnloadTexture(middle_background);
	UnloadTexture(t_foreground);
	UnloadTexture(t_woodenlog);
	UnloadTexture(Clouds_t);
	UnloadTexture(FrontVegetation_t);

	Sky->clear();
}

void Alpino::update(RenderTexture2D *fbo)
{

	switch (this->GAMESTAGE)
	{
	case INITIALPAGE:


	case INGAME:

		dt = GetFrameTime();

		
		std::cout << killua.Data.frame << std::endl;
		//DrawRectangleGradientV(0,0,getWsize().x ,getWsize().y, { 43, 82, 122 , 255 } , {179 , 181 , 199 , 255});
		Sky->Draw();

		
		//animation uptade for textures
		{
			fog_cloud.Data = updateAnimdata(fog_cloud.Data, dt, 3);
			killua.updateCharacterTexture(dt, 5, MoveEverything);
		}
		
		

		if (MoveEverything == MOVING_FRONT)
		{
			Clouds.RenderDuplicateEx(1, 0, { 200,200,200,220 });
			castle.RenderDuplicateExLoop(1, 0, { 200,200,200,210 }, -(float)(castle.Texture->width * castle.scale), { getWsize().x,0 }, dt, false);
			farbackground_o.RenderDuplicateExLoop(3, 0, WHITE, -(float)(foreground_o.Texture->width * foreground_o.scale), { 0 , FORESTPOSY }, dt, false);
			middlebackground_o.RenderDuplicateExLoop(3, 0, WHITE, -(float)(foreground_o.Texture->width * foreground_o.scale), { 0 , FORESTPOSY }, dt, false);
			fog_cloud.RenderDuplicateRecLoop(3, 0, { 255,255,255,220 }, -(float)(fog_cloud.Data.rec.width * fog_cloud.scale), { 0 , 450 }, dt, false , 4);
			foreground_o.RenderDuplicateExLoop(3, 0, WHITE, -(float)(foreground_o.Texture->width * foreground_o.scale), { 0 , FORESTPOSY }, dt, false);
			FrontVegetation.RenderDuplicateExLoop(5, 0, { WHITE }, -(float)(FrontVegetation_t.width * FrontVegetation.scale), { 0,FrontVegetation.Data.pos.y }, dt, false);
		}
		else if (MoveEverything == IDLE)
		{
			Clouds.RenderDuplicateEx(1, 0, { 200,200,200,220 });
			castle.RenderDuplicateEx(1, 0, { 200,200,200,210 });
			farbackground_o.RenderDuplicateEx(3, 0, WHITE);
			middlebackground_o.RenderDuplicateEx(3, 0, WHITE);
			fog_cloud.RenderDuplicateRec(3, 0, {255,255,255,220}, 4, 1);
			foreground_o.RenderDuplicateEx(3, 0, WHITE);
			FrontVegetation.RenderDuplicateEx(5, 0, WHITE);
		}
		
		else if (MoveEverything == MOVING_BACK)
		{
			Clouds.RenderDuplicateEx(1, 0, { 200,200,200,220 });
			castle.RenderDuplicateExLoop(1, 0, { 200,200,200,210 }, getWsize().x, { -(float)(CastleTexture.width * castle.scale), 0 }, dt, true);
			farbackground_o.RenderDuplicateExLoop(3, 0, WHITE,  0, { -(float)(foreground_o.Texture->width * foreground_o.scale) , FORESTPOSY }, dt, true);
			middlebackground_o.RenderDuplicateExLoop(3, 0, WHITE, 0, { -(float)(foreground_o.Texture->width * foreground_o.scale) , FORESTPOSY }, dt, true);
			fog_cloud.RenderDuplicateRecLoop(3, 0, { 255,255,255,220 }, 0, { -(float)(fog_cloud.Data.rec.width * fog_cloud.scale) , 450 }, dt, true , 4);
			foreground_o.RenderDuplicateExLoop(3, 0, WHITE, 0, { -(float)(foreground_o.Texture->width * foreground_o.scale) , FORESTPOSY }, dt, true);
			FrontVegetation.RenderDuplicateExLoop(5, 0, WHITE, 0, { -(float)(FrontVegetation_t.width * FrontVegetation.scale),FrontVegetation.Data.pos.y }, dt, true);
		}
		
		
		for (int i = 0; i < sizeofnebula; i++)
		{
			
			nebulas[i]->Data.rec.width = t_woodenlog.width;
			nebulas[i]->Data.rec.height = t_woodenlog.height;
			nebulas[i]->Data.pos.x -= nebulas[i]->Data.speed * MoveEverything * dt;
			nebulas[i]->Hitbox.Data.pos.x -= nebulas[i]->Data.speed * dt;
			DrawTextureRec(t_woodenlog, nebulas[i]->Data.rec, nebulas[i]->Data.pos, WHITE);
		}
		
		for (int i = 0; i < sizeofnebula; i++)
		{	
			RotateNebula(nebulas[i]->Data, getWsize().x , i);
		}
		
		CharacterMovement();
		DrawTextureRec(killua_t, killua.Data.rec, killua.Data.pos, WHITE);
		
		for (int i = 0; i < sizeofnebula; i++)
		{
			if (CheckCollisionCircleRec(nebulas[i]->Hitbox.Data.pos, nebulas[i]->Hitbox.radius, Rectangle{ killua.Data.pos.x + 38,killua.Data.pos.y,(float)(killua_t.width / 6) - 38,(float)killua_t.height - 20 }))
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
		Sky->Draw();

		
	case ENDPAGE:


	default:
		break;
	}
}

bool Alpino::isObjectOut(ObjectData data)
{
	return (data.pos.x <= -nebulas[0]->Data.rec.width);
}

void Alpino::RotateNebula(ObjectData data, int windowwidth,int index)
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
ObjectData  Alpino::updateAnimdata(ObjectData data, float dt, int maxframe)
{

	if (!isOnGround(killua.Data) && data == killua.Data || data == killua.Data && MoveEverything == IDLE)
	{
		return data;
	}
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
			return data;

		}
	}
}

bool Alpino::isOnGround(ObjectData data)
{
	return data.pos.y + data.rec.height >= getWsize().y-26;
}

inline void Alpino::CharacterMovement()
{

	killua.updateMovingState(MoveEverything);
	current_high = killua.Data.pos.y;
	killua.Data.pos.y += killua.Data.speed * dt;
	
	
	if (isOnGround(killua.Data) && IsKeyDown(KEY_SPACE))
	{	
		if (!isOnGround(killua.Data))
			isPlayerJumped = true;
		killua.Data.speed = 0;
	}
	else if (isOnGround(killua.Data))
	{
		isMaxHeightReached = false;
		killua.Data.speed = 0;
		isPlayerJumped = false;
	}
	else
	{
		killua.Data.speed += gravity * dt;
	}
	if (max_high >= current_high)
		isMaxHeightReached = true;
	if (IsKeyDown(KEY_SPACE) && !isMaxHeightReached && !isPlayerJumped)
	{
		killua.Data.speed = -300;
	}
	else if (IsKeyReleased(KEY_SPACE))
	{
		isMaxHeightReached = true;
	}
}





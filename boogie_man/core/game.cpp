#include "game.h"
#include <iostream>
#include "far_background.h"
#include "killua.h"
#include "game.h"
#include "middle_Background.h"
#include "foreground.h"
#include <string>
#include "LevelHandler.h"

#define FORESTPOSY 0
#define GROUND getWsize().y-26

BoogieMan::BoogieMan(Vec2<int> WindowSize)
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
	Road_t = LoadTexture(GetRelativeTexturePath("Road.png").c_str());
	WoodFront_t = LoadTexture(GetRelativeTexturePath("woodenlogwithroots.png").c_str());

	wooden_log.SetAnimData({}, { getWsize().x / 2 , 100 }, 0, 0, 0, 10);
	float ForestScale = 3.8f;
	
	FrontVegetation.SetBaseAttributes(FrontVegetation_t, 1.0f, { {0,0,(float)FrontVegetation_t.width,(float)FrontVegetation_t.height},
	{0,getWsize().y * 0.965f},0,0,0,200 * MoveEverything }, 0.0f);
	
	
	castle.SetBaseAttributes(CastleTexture, 0.7f, { { 0,0,(float)CastleTexture.width , (float)CastleTexture.height },
		{ getWsize().x / 2 , 0 },
		0,
		0,
		0,
		10 * MoveEverything },
		0.0f);
	
	fog_cloud.SetBaseAttributes(t_fog_cloud, 1.0f,
		{ {0,0,(float)t_fog_cloud.width / 4 ,(float)t_fog_cloud.height},
		{0,450},
		0,
		0,
		0.5f,
		135 * MoveEverything
		},
		0.0f);

    
	foreground_o.SetBaseAttributes(t_foreground, ForestScale, { {}, { bgGL::Pixel2Percent(0), FORESTPOSY}, 0, 0, 0, 160*MoveEverything }, 0.0f);
	middlebackground_o.SetBaseAttributes(middle_background, ForestScale, { {}, { 0, FORESTPOSY }, 0, 0, 0, 110 * MoveEverything }, 0.0f);
	farbackground_o.SetBaseAttributes(far_background, ForestScale, { {}, { 0, FORESTPOSY }, 0, 0, 0, 80 * MoveEverything }, 0.0f);
	Clouds.SetBaseAttributes(Clouds_t, 1.0f, ObjectData({}, { 0, -20 }, 0, 0, 0, 80 * MoveEverything), 0.0f);
	//farbackground_o.SetBaseAttributes(far_background, ForestScale, { {}, { 0, FORESTPOSY }, 0, 0, 0, 80 }, 0.0f);

	Road.SetBaseAttributes(t_foreground, 0.8f, { {}, { 0,getWsize().y - 120}, 0, 0, 0, 180 * MoveEverything }, 0.0f);
	Road.SetInstancing(40, bgGL::MakeInstanceOffsetArray(40, { 0,0 }, 2.6f, 0.5f));


	std::cout<< "size" << getWsize().x / 2 << std::endl;

	for (size_t i = 0; i < 8; i++)
	{
		nebulas[i] = std::make_unique<Nebula>(WindowHeight, WindowWidth , nebula);
	}

	GameLevel::Level newLevel("level.json");

	killua.SetTexture(killua_t);
	killua.SetAnimData(
			{0.0f,0.0f,(float)(killua_t.width / 12),(float)killua_t.height}, // rectangle
			{(getWsize().x / 2) - (killua_t.width / 12),getWsize().y - killua_t.height - 26}, // pos
			5, // frame number
			0, // running time
			1.0 / 12.0, // uptade time
			500 // speed
		);
	max_high = killua.Data.pos.y - 100;
	killua.scale = 1.0f;

	Sky = std::make_unique<bgGL::cubemap>(GetRelativeTexturePath("sky/Two_sided_background2.hdr").c_str() , true , -0.11f , 512);
	
	WoodFront.SetTexture(WoodFront_t);
	WoodFront.SetInstancing(100, bgGL::MakeInstanceOffsetArray(100, { 0,0 }, []() -> float {return (1.0f / GiveRandomNumf(3, 6, 100, false, 11)) * 4; }, 1.2f));
	
	ForestFront.SetTexture(t_foreground);
	ForestFront.SetInstancing(40, bgGL::MakeInstanceOffsetArray(40, { 0,0 }, []() -> float {return GiveRandomNumf(2, 3, 100, false, 11); }, 0.4f));

	WoodenLogWithRoots.SetBaseAttributes(WoodFront_t, 2.0f, { {},{-700,900},0,0,0,0 }, 0.0f);

	woodcol(WoodenLogWithRoots.Data);
	woodcol.rec = { WoodenLogWithRoots.Data.pos.x + 18,WoodenLogWithRoots.Data.pos.y + 40,63 * WoodenLogWithRoots.scale,40 * WoodenLogWithRoots.scale };

}

BoogieMan::~BoogieMan()
{
	UnloadTexture(killua_t);
	UnloadTexture(nebula);
	UnloadTexture(far_background);
	UnloadTexture(middle_background);
	UnloadTexture(t_foreground);
	UnloadTexture(t_woodenlog);
	UnloadTexture(Clouds_t);
	UnloadTexture(FrontVegetation_t);
	UnloadTexture(Road_t);
	UnloadTexture(WoodFront_t);
	
	Sky->clear();
}

void BoogieMan::update(RenderTexture2D *fbo , Camera2D &MainCamera)
{

	switch (this->GAMESTAGE)
	{
	case INITIALPAGE:

		break;
	case INGAME:

		BEGIN_INTERNAL_CAMERA(MainCamera);

		dt = GetFrameTime();

		
		std::cout << MoveEverything << std::endl;
		//DrawRectangleGradientV(0,0,getWsize().x ,getWsize().y, { 43, 82, 122 , 255 } , {179 , 181 , 199 , 255});
		//Sky->Draw();

		////animation update for textures
		{
		   fog_cloud.Data = updateAnimdata(fog_cloud.Data, dt, 3);
		   killua.updateCharacterTexture(dt, 5, MoveEverything);
		}
		/*
		if (MoveEverything == MOVING_FRONT)
		{
			
			Clouds.RenderDuplicateEx(1, 0, { 200,200,200,220 });
			castle.RenderDuplicateExLoop(1, 0, { 200,200,200,210 }, -(float)(castle.Texture->width * castle.scale), { getWsize().x,0 }, dt, false);
			farbackground_o.RenderDuplicateExLoop(3, 0, WHITE, -(float)(foreground_o.Texture->width * foreground_o.scale), { 0 , FORESTPOSY }, dt, false);
			middlebackground_o.RenderDuplicateExLoop(3, 0, WHITE, -(float)(foreground_o.Texture->width * foreground_o.scale), { 0 , FORESTPOSY }, dt, false);
			fog_cloud.RenderDuplicateRecLoop(3, 0, { 255,255,255,220 }, -(float)(fog_cloud.Data.rec.width * fog_cloud.scale), { 0 , 450 }, dt, false , 4);
			foreground_o.RenderDuplicateExLoop(3, 0, WHITE, -(float)(foreground_o.Texture->width * foreground_o.scale), { 0 , FORESTPOSY }, dt, false);
			Road.RenderDuplicateExLoop(3, 0, { 231, 255, 207 , 255}, -(float)(Road.Texture->width * Road.scale), {0 , Road.Data.pos.y}, dt, false);
			FrontVegetation.RenderDuplicateExLoop(5, 0, { WHITE }, -(float)(FrontVegetation_t.width * FrontVegetation.scale), { 0,FrontVegetation.Data.pos.y }, dt, false);
		}
		else if (MoveEverything == MOVING_BACK)
		{
			Clouds.RenderDuplicateEx(1, 0, { 200,200,200,220 });
			castle.RenderDuplicateExLoop(1, 0, { 200,200,200,210 }, getWsize().x, { -(float)(CastleTexture.width * castle.scale), 0 }, dt, true);
			farbackground_o.RenderDuplicateExLoop(3, 0, WHITE,  0, { -(float)(foreground_o.Texture->width * foreground_o.scale) , FORESTPOSY }, dt, true);
			middlebackground_o.RenderDuplicateExLoop(3, 0, WHITE, 0, { -(float)(foreground_o.Texture->width * foreground_o.scale) , FORESTPOSY }, dt, true);
			fog_cloud.RenderDuplicateRecLoop(3, 0, { 255,255,255,220 }, 0, { -(float)(fog_cloud.Data.rec.width * fog_cloud.scale) , 450 }, dt, true , 4);
			foreground_o.RenderDuplicateExLoop(3, 0, WHITE, 0, { -(float)(foreground_o.Texture->width * foreground_o.scale) , FORESTPOSY }, dt, true);
			Road.RenderDuplicateExLoop(3, 0, WHITE, 0, { -(float)(Road.Texture->width * Road.scale) , Road.Data.pos.y }, dt, true);
			FrontVegetation.RenderDuplicateExLoop(5, 0, WHITE, 0, { -(float)(FrontVegetation_t.width * FrontVegetation.scale),FrontVegetation.Data.pos.y }, dt, true);
		}
		*/

		castle.RenderDuplicateEx(1, 0, { 200,200,200,210 });
		farbackground_o.RenderDuplicateEx(3, 0, WHITE);
		middlebackground_o.RenderDuplicateEx(3, 0, WHITE);
		foreground_o.RenderDuplicateEx(3, 0, WHITE);
		//Road.RenderDuplicateEx(3, 0, { 231, 255, 207 , 255 });
		FrontVegetation.RenderDuplicateEx(5, 0, WHITE);
		fog_cloud.RenderDuplicateRec(3, 0, { 255,255,255,220 }, 4, 1);

		END_INTERNAL_CAMERA;

		Road.InstancedTexture->draw(MainCamera, { 231, 255, 207 , 255 },*Sky->GetFBOtexture(), 1.8);



		std::cout << "KILLUA POS: " << killua.Data.pos << std::endl;

		
		BEGIN_INTERNAL_CAMERA(MainCamera);
		
		CharacterMovement();		
		DrawTextureRec(killua_t, killua.Data.rec, killua.Data.pos.toVector2(), WHITE);
		
		END_INTERNAL_CAMERA;

		WoodFront.InstancedTexture->draw(MainCamera, GRAY, *Sky->GetFBOtexture(), 2.5);
		ForestFront.InstancedTexture->draw(MainCamera, GRAY, *Sky->GetFBOtexture(), 2.6);


		for (int i = 0; i < sizeofnebula; i++)
		{
			if (CheckCollisionCircleRec(nebulas[i]->Hitbox.Data.pos.toVector2(), nebulas[i]->Hitbox.radius, Rectangle{killua.Data.pos.x + 38,killua.Data.pos.y,(float)(killua_t.width / 6) - 38,(float)killua_t.height - 20}))
			{
				//std::cout << "game is over" << std::endl;
				//GAMESTAGE = ENDPAGE;
			}
		}

		


		break;

	case ENDPAGE:
		

		break;
	default:
		break;
	}

	
}

void BoogieMan::drawOffCamera()
{
	switch (this->GAMESTAGE)
	{
	case INITIALPAGE:

		break;
	case INGAME:

		Sky->drawFBO();


		//std::cout << "AVERAGE SKY COLOR: (" << averageSkyColor.r << " , " << averageSkyColor.g << " , " << averageSkyColor.b << " , " << averageSkyColor.a << ")" << std::endl;

		Clouds.RenderDuplicateEx(1, 0, { 200,200,200,220 });

		if (MoveEverything == MOVING_FRONT)
		{
			castle.RenderDuplicateExLoop(1, 0, { 200,200,200,210 }, -(float)(castle.Texture->width * castle.scale), { getWsize().x,0 }, dt, false);
		}
		else if (MoveEverything == IDLE)
		{
			castle.RenderDuplicateEx(1, 0, { 200,200,200,210 });
		}
		else if (MoveEverything == MOVING_BACK)
		{
			castle.RenderDuplicateExLoop(1, 0, { 200,200,200,210 }, getWsize().x, { -(float)(CastleTexture.width * castle.scale), 0 }, dt, true);
		}

		break;
	case ENDPAGE:

		break;
	default:
		break;
	}

}

void BoogieMan::drawOffFBO()
{
	Sky->Draw();
}

void BoogieMan::draw(RenderTexture2D* fbo)
{
	switch (this->GAMESTAGE)
	{
	case INITIALPAGE:

		break;
	case INGAME:
	    Sky->Draw();

		break;
	case ENDPAGE:

		break;
	default:
		break;
	}
}

bool BoogieMan::isObjectOut(ObjectData data)
{
	return (data.pos.x <= -nebulas[0]->Data.rec.width);
}

void BoogieMan::RotateNebula(ObjectData data, int windowwidth,int index)
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
ObjectData  BoogieMan::updateAnimdata(ObjectData data, float dt, int maxframe)
{

	if (!isOnGround() && data == killua.Data || data == killua.Data && MoveEverything == IDLE)
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

	return data;
}

bool BoogieMan::isOnGround()
{

	int direction = GameObject::VectorDirection({ killua.Data.pos.x + (killua.Texture->width / 26) - woodcol.pos.x,woodcol.pos.y - killua.Data.pos.y });

	if (CheckCollisionRecs({ killua.Data.pos.x,killua.Data.pos.y,(float)killua.Texture->width / 13,(float)killua.Texture->height }, woodcol.rec) && direction == UP)
	{
		return true;
	}

	return killua.Data.pos.y + killua.Data.rec.height >= GROUND;
}

inline void BoogieMan::CharacterMovement()
{


	std::cout << " collison" << max_high << std::endl;
	DrawRectangleRec({ killua.Data.pos.x,killua.Data.pos.y,(float)killua.Texture->width / 13,(float)killua.Texture->height }, WHITE);
	DrawRectangleRec(woodcol.rec, WHITE);
	static float LocalSpeed = killua.Data.speed;
	killua.updateMovingState(MoveEverything, dt, woodcol);
	current_high = killua.Data.pos.y;
	killua.Data.pos.y += LocalSpeed * dt;


	if (isOnGround() && IsKeyDown(KEY_SPACE))
	{
		if (!isOnGround())
			isPlayerJumped = true;
		LocalSpeed = 0;
	}
	else if (isOnGround())
	{
		max_high = killua.Data.pos.y - 100;
		isMaxHeightReached = false;
		LocalSpeed = 0;
		isPlayerJumped = false;
	}
	else
	{
		LocalSpeed += gravity * dt;
	}
	if (max_high >= current_high)
		isMaxHeightReached = true;
	if (IsKeyDown(KEY_SPACE) && !isMaxHeightReached && !isPlayerJumped)
	{
		LocalSpeed = -300;
	}
	else if (IsKeyReleased(KEY_SPACE))
	{
		isMaxHeightReached = true;
	}
}





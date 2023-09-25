#include "game.h"
#include <iostream>
#include "far_background.h"
#include "killua.h"
#include "game.h"
#include "middle_Background.h"
#include "foreground.h"
#include <string>
#include "LevelHandler.h"
#include "../util/Log.h"

#define FORESTPOSY 0
#define GROUND getWsize().y-26

Shader BloomShader;
Shader PixelShader;


BoogieMan::BoogieMan(Vec2<int> WindowSize)
{
	
	this->WindowWidth = WindowSize.x;
	this->WindowHeight = WindowSize.y;

	//LoadTexture2DfromHeader(&killua_t, KILLUA_FORMAT, KILLUA_HEIGHT, KILLUA_WIDTH, KILLUA_DATA, 1);
	bgGL::LoadTexture2DfromHeader(&nebula, NEBULA_FORMAT, NEBULA_HEIGHT, NEBULA_WIDTH, NEBULA_DATA, 1);
	
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

	Road.SetBaseAttributes(t_foreground, 0.8f, { {0.0f,getWsize().y - 120.0f , (float)t_foreground.width , (float)t_foreground.height}, { 0,getWsize().y - 120}, 0, 0, 0, 180 * MoveEverything }, 0.0f);
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

	Sky = std::make_unique<bgGL::cubemap>(GetRelativeTexturePath("sky/Two_sided_background2.hdr").c_str() , true , -0.11f , 256);
	
	WoodFront.SetTexture(WoodFront_t);
	WoodFront.SetInstancing(100, bgGL::MakeInstanceOffsetArray(100, { 0,0 }, []() -> float {return (GiveRandomNumf(1, 2, 100, false, 11)) * 4; }, 1.0f));
	
	ForestFront.SetTexture(t_foreground);
	ForestFront.SetInstancing(40, bgGL::MakeInstanceOffsetArray(40, { 0,0 }, []() -> float {return GiveRandomNumf(5, 6, 100, false, 11); }, 0.2f));

	ForestMid.SetTexture(middle_background);
	ForestMid.SetInstancing(40, bgGL::MakeInstanceOffsetArray(40, { 0,0 }, []() -> float {return GiveRandomNumf(6, 8, 100, false, 11); }, 0.4f));

	WoodenLogWithRoots.SetBaseAttributes(WoodFront_t, 2.0f, { {},{500,900},0,0,0,0 }, 0.0f);


	WoodenLogWithRoots.SetBaseAttributes(WoodFront_t, 2.0f, { {},{-700,900},0,0,0,0 }, 0.0f);

	woodcol.Data(WoodenLogWithRoots.Data);
	woodcol.Data.rec = { WoodenLogWithRoots.Data.pos.x + 18,WoodenLogWithRoots.Data.pos.y + 40,63 * WoodenLogWithRoots.scale,40 * WoodenLogWithRoots.scale };
	woodcol.Data.pos({ woodcol.Data.rec.x, woodcol.Data.rec.y });
	WoodenLogWithRoots.Data.rec = woodcol.Data.rec;
	
	BloomShader = LoadShader(0, TextFormat(GetRelativeTexturePath("shaders/bloom.fs").c_str(), 330));
	PixelShader = LoadShader(0, TextFormat(GetRelativeTexturePath("shaders/pixelizer.fs").c_str(), 330));

	//ShadowMap = std::make_unique<bgGL::shadowmap>(1024, 1024);
	ShadowMapFBO = std::make_unique<RenderTexture2D>(LoadRenderTexture(2048,2048));

	camera3d = { 0 };
	camera3d.position = { 1.0f, 1.0f, 1.0f };
	camera3d.target = { 4.0f, 1.0f, 4.0f };
	camera3d.up = { 0.0f, 1.0f, 0.0f };
	camera3d.fovy = 45.0f;
	camera3d.projection = CAMERA_PERSPECTIVE;

	//objects.push_back(&Road);
	//objects.push_back(&ForestMid);
	//objects.push_back(&ForestFront);
	objects.push_back(&killua);
	objects.push_back(&WoodenLogWithRoots);


	QT::InitList(headnode);


	killua.EnableAbility(ABILITY_FLAG_JUMP);
	killua.EnableAbility(ABILITY_FLAG_MOVE);
	killua.EnableAbility(ABILITY_FLAG_SPRINT);
	killua.EnableAbility(ABILITY_FLAG_DOUBLE_JUMP);
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
	UnloadRenderTexture(*ShadowMapFBO);

	UnloadShader(BloomShader);
	UnloadShader(PixelShader);

	QT::FreeQuads(headnode);

	LOG_TRACEBACK_PRINT_FILE("LogTraceBack.txt");
	
	Sky->clear();
}

void BoogieMan::update(RenderTexture2D *fbo , Camera2D &MainCamera)
{

	switch (this->GAMESTAGE)
	{
	case INITIALPAGE:

		break;
	case INGAME:

		////animation update for textures
	    {
		//fog_cloud.Data = updateAnimdata(fog_cloud.Data, dt, 3);
		killua.updateCharacterTexture(dt, 5, woodcol.Data);
	    }
		
		killua.Move();
		killua.Jump();

		std::cout << "KILLUA POS: " << killua.Data.pos << std::endl;

		QT::ContructQuads(headnode, objects, { 300,300 }, MainCamera);

		for (int i = 0; i < sizeofnebula; i++)
		{
			if (CheckCollisionCircleRec(nebulas[i]->Hitbox.Data.pos.toVector2(), nebulas[i]->Hitbox.radius, Rectangle{ killua.Data.pos.x + 38,killua.Data.pos.y,(float)(killua_t.width / 6) - 38,(float)killua_t.height - 20 }))
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

void BoogieMan::drawOffFBO(Camera2D& MainCamera)
{
	Sky->Draw();

	//glBindFramebuffer(GL_FRAMEBUFFER, ShadowMap->GetShadowMapFBO());
	BeginTextureMode(*ShadowMapFBO);
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_NOTEQUAL | GL_LESS);

	bgGL::ClearColorBufferBit(WHITE);
	glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glViewport(0, 0, ShadowMapFBO->texture.width, ShadowMapFBO->texture.height);

	//glBindTexture(GL_TEXTURE_2D, ShadowMap->GetShadowMapImage());
    //UpdateCamera(&camera3d, CAMERA_FIRST_PERSON);
	
	//Vec2<float> mousePos({ GetMouseX()/getWsize().x , GetMouseY() / getWsize().y});
	//mousePos(mousePos * 2.0f - 1.0f);
	glm::vec3 LightPosition(0.4f, 0.1f, 5.0f);
	//glm::vec3 LightPosition(mousePos.x , mousePos.y, 0.1f);
    //glm::vec3 LightPosition(camera3d.target.x , camera3d.target.y, camera3d.target.z);
	LOG("LIGHT POSITION: " << LightPosition.x << " " << LightPosition.y << " " << LightPosition.z);
	//glm::vec3 LightPosition(camera3d.target.x, camera3d.target.y, camera3d.target.z);
	ForestMid.InstancedTexture->drawShadowMap(MainCamera,LightPosition, 1.7);
	Road.InstancedTexture->drawShadowMap(MainCamera, LightPosition,1.8);
	WoodFront.InstancedTexture->drawShadowMap(MainCamera, LightPosition, 2.6);
	ForestFront.InstancedTexture->drawShadowMap(MainCamera, LightPosition, 2.8);

	glViewport(0, 0, getWsize().x, getWsize().y);
	glCullFace(GL_BACK);
	glDepthFunc(GL_LESS);
	glDisable(GL_DEPTH_TEST);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	EndTextureMode();
}

void BoogieMan::draw(RenderTexture2D* fbo, Camera2D& MainCamera)
{
	switch (this->GAMESTAGE)
	{
	case INITIALPAGE:

		break;
	case INGAME:
	{
		dt = GetFrameTime();

		BEGIN_INTERNAL_CAMERA(MainCamera);
		ForestMid.InstancedTexture->draw(MainCamera, { 231, 255, 207 , 255 }, *Sky->GetFBOtexture(), ShadowMapFBO->texture.id, 1.7);
		END_INTERNAL_CAMERA;


		BEGIN_INTERNAL_CAMERA(MainCamera);

		FrontVegetation.RenderDuplicateEx(5, 0, WHITE);
		fog_cloud.RenderDuplicateRec(3, 0, { 255,255,255,220 }, 4, 1);

		END_INTERNAL_CAMERA;

		Road.InstancedTexture->draw(MainCamera, { 231, 255, 207 , 255 }, *Sky->GetFBOtexture(), ShadowMapFBO->texture.id, 1.8);

		BEGIN_INTERNAL_CAMERA(MainCamera);
		DrawRectangleRec(woodcol.Data.rec, RED);
		DrawTextureRec(killua_t, killua.Data.rec, killua.Data.pos.toVector2(), WHITE);
		END_INTERNAL_CAMERA;

		WoodFront.InstancedTexture->draw(MainCamera, GRAY, *Sky->GetFBOtexture(), ShadowMapFBO->texture.id, 2.6);
		ForestFront.InstancedTexture->draw(MainCamera, GRAY, *Sky->GetFBOtexture(), ShadowMapFBO->texture.id, 2.8);

		BEGIN_INTERNAL_CAMERA(MainCamera);
		QT::Quad* CurrentQuad = headnode;
		while (CurrentQuad != nullptr)
		{
			DrawRectangleLines(CurrentQuad->Attrib.x * CurrentQuad->Attrib.z, CurrentQuad->Attrib.y * CurrentQuad->Attrib.w, CurrentQuad->Attrib.z, CurrentQuad->Attrib.w, RED);
			CurrentQuad = CurrentQuad->next;
		}
		END_INTERNAL_CAMERA;
	}
		break;
	case ENDPAGE:

		break;
	default:

		break;
	}
}





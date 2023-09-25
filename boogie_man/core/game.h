#pragma once
#include "raylib.h"
#include <vector>
#include "nebula.h"
#include "../util/WindowUtil.h"
#include <memory>
#include <string>
#include "MainCharacter.h"
#include "../QuadTreeCollision.h"
#include "../util/Thread.h"
#define REAL  0
#define DUPLICATE  1

#define INITIALPAGE 0
#define INGAME 1
#define ENDPAGE 2


class Nebula : public GameObject
{
public:
	
	Nebula(int WindowHeight , int WindowWidth , Texture2D Texture_i)
	{
		static int NebulaCount = 0;

		this->Texture = &Texture_i;

		this->Data.Set({ 0,0 ,(float)Texture->width / 8 , (float)Texture->height / 8 },
			           { getWsize().x + (300 * NebulaCount) - 400 ,getWsize().y - Texture->height / 8 },
			           0, 0, 1.0 / 12.0, 180);

		Hitbox.radius = 67.0 / 2;
		Hitbox.Data.pos.x = Data.pos.x + (Hitbox.radius + 18);
		Hitbox.Data.pos.y = Data.pos.y + (Hitbox.radius + 18);

		NebulaCount++;
	};

};


class BoogieMan
{
public:

	uint GAMESTAGE = INGAME;

	int WindowWidth;
	int WindowHeight;
	const int sizeofnebula = 8;
	int gravity = 1000;
	float posy = WindowHeight - 40;
	float posx = WindowWidth / 2;
	float far_bgx = 0;
	float dt = 0;
	bool allow = true;
	bool isMaxHeightReached = false;
	bool collusion = false;
	float max_high;
	float current_high;
	bool isPlayerJumped;
	int MoveEverything = 0;
	
	Texture2D killua_t;
	Texture2D far_background;
	Texture2D middle_background;
	Texture2D t_foreground;
	Texture2D nebula;
	Texture2D FrontVegetation_t;
	Texture2D CastleTexture;
	Texture2D t_woodenlog;
	Texture2D t_fog_cloud;
	Texture2D Clouds_t;
	Texture2D Road_t;
	Texture2D WoodFront_t;

	InstancedGameObject WoodFront;
	InstancedGameObject ForestFront;
	InstancedGameObject Road;
	InstancedGameObject ForestMid;

	GameObject farbackground_o;
	GameObject middlebackground_o;
	GameObject foreground_o;
	GameObject fog_cloud;
	GameObject wooden_log;
	GameObject Clouds;
	GameObject FrontVegetation;
	GameObject castle;
	GameObject WoodenLogWithRoots;

	ObjectData woodcol;

	std::vector<GameObject*> objects;

	QT::Quad* headnode;

	BgCharacter::MainCharacter killua;

	std::unique_ptr<bgGL::cubemap> Sky;

	std::unique_ptr<Nebula> nebulas[8];

	//std::unique_ptr<bgGL::shadowmap> ShadowMap;
	std::unique_ptr<RenderTexture2D> ShadowMapFBO;
	Camera camera3d;

	std::unique_ptr<ThreadPool> Threadpool;

	BoogieMan(Vec2<int> WindowSize);
	~BoogieMan();
	

	void update(RenderTexture2D* fbo , Camera2D &MainCamera);
	void draw(RenderTexture2D* fbo);
	void drawOffCamera();
	void drawOffFBO(Camera2D &MainCamera);
	
private:

};



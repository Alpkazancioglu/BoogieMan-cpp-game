#pragma once
#include "raylib.h"
#include <vector>
#include "nebula.h"
#include "../VectorMath.h"
#include <memory>
#include <string>
#include "../GameObject.h"
#define REAL  0
#define DUPLICATE  1

#define INITIALPAGE 0
#define INGAME 1
#define ENDPAGE 2

#define IDLE 0
#define MOVING_FRONT 1
#define MOVING_BACK -1

Vec2<float> getWsize();

class FrontStone
{
	Texture2D* Texture;
	
	
	FrontStone(Texture2D Texture_i)
	{
		this->Texture = &Texture_i;
	}

};


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


class Character
{



};

class Castle : public GameObject
{
public:

	
	void Init()
	{
		//this->SetAnimData({ 0,0,(float)this->Texture->width , (float)this->Texture->height }, { getWsize().x / 2 , 0 }, 0, 0, 0, 10);
		this->SetAnimData({ 0,0,(float)this->Texture->width , (float)this->Texture->height }, { getWsize().x / 2 , 100 }, 0, 0, 0, 10);
		this->rotation = 0.0f;
		this->scale = 0.7f;
	}


};


class Alpino
{
public:

	uint GAMESTAGE = INGAME;

	int WindowWidth;
	int WindowHeight;
	const int sizeofnebula = 8;
	int gravity = 1000;
	int MoveEverything = 1;
	float posy = WindowHeight - 40;
	float posx = WindowWidth / 2;
	float far_bgx = 0;
	float dt = 0;
	bool allow = true;
	bool onAir = false;
	bool collusion = false;
	float max_high;
	float current_high;
	bool isplayerjumped;
	
	Texture2D killua;
	Texture2D far_background;
	Texture2D middle_background;
	Texture2D t_foreground;
	Texture2D nebula;
	Texture2D t_Fronstones;
	Texture2D CastleTexture;
	Texture2D t_woodenlog;
	Texture2D t_fog_cloud;
	Castle Castle_;

	std::vector<AnimBackground> farbackground;
	std::vector<AnimBackground> middlebackground;
	std::vector<AnimBackground> foreground;
	std::vector<AnimBackground> fronstones;

	GameObject farbackground_o;
	GameObject middlebackground_o;
	GameObject foreground_o;
	GameObject fog_cloud;
	GameObject wooden_log;

	std::unique_ptr<Nebula> nebulas[8];

	Animdata killuaData;

	Alpino(Vec2<int> WindowSize);
	~Alpino();

	void update(RenderTexture2D* fbo);
	void draw(RenderTexture2D* fbo);
	
	bool isObjectOut(Animdata data);
    std::string GetRelativeTexturePath(std::string textureName);
	bool isOnGround(Animdata data, float windowHeight);
	Animdata updateAnimdata(Animdata data, float dt, int maxframe);
	Vector2 uptadebackgrounds(std::vector<AnimBackground>& data, float dt, bool duplicate);
	void RotateNebula(Animdata data, int windowwidth,int index);
	

	std::vector<AnimBackground> initbackgrounds(Texture2D texture, float speed,float scale, Vector2 pos,float distance,int arraysize, std::vector <AnimBackground>& data);
	void DrawandUptadebackgrounds(std::vector<AnimBackground>& data, float dt , Color tint);
	void drawStones(std::vector <AnimBackground> data, unsigned int size);
	void KilluaJump();
	void movecharacter()
	{
		if (IsKeyDown(KEY_D))
			MoveEverything = MOVING_FRONT;

		else if (IsKeyDown(KEY_A))
			MoveEverything = MOVING_BACK;
		
		else if (IsKeyUp(KEY_A) && IsKeyUp(KEY_D))
			MoveEverything = IDLE;
	
	}
	
	Vector2 moveStones(std::vector<AnimBackground>& data, float dt, float scale, int index);
private:

};



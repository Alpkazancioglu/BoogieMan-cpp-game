#pragma once
#include "raylib.h"
#include <vector>
#include "../VectorMath.h"
#include "nebula.h"
#include <memory>
#include <string>
#define REAL  0
#define DUPLICATE  1
#define INITIALPAGE 0
#define INGAME 1
#define ENDPAGE 2

typedef unsigned int uint;
Vec2<int> getWsize();
class Animdata
{
public:
	Rectangle rec;
	Vector2 pos;
	int frame;
	float runningtime;
	float updatetime;
	int speed;

};

class CollisionBox
{
public:
	
	Rectangle rec;
	Vector2 pos;
	int frame;
	float runningtime;
	float updatetime;
	float speed;
	float radius;
	
};

struct AnimBackground
{
	Texture2D texture;
	Vector2 pos;
	Vector2 origin;
	int arraysize;
	float speed;
	float scale;
	float distance;
	bool duplicate;



};
class FrontStone
{
	Texture2D* Texture;
	
	
	FrontStone(Texture2D Texture_i)
	{
		this->Texture = &Texture_i;
	}

};


class Nebula
{
public:
	Texture2D *Texture;
	Animdata Data;
	CollisionBox Hitbox;
	
	Nebula(int WindowHeight , int WindowWidth , Texture2D Texture_i)
	{
		static int NebulaCount = 0;

		this->Texture = &Texture_i;

		Hitbox.radius = 67.0 / 2;
		Data.rec.x = 0.0;
		Data.rec.y = 0.0;
		Data.rec.width = Texture->width / 8;
		Data.rec.height = Texture->height / 8;
		Data.pos.x = getWsize().x + (300 * NebulaCount) - 400;
		Hitbox.pos.x = Data.pos.x + (Hitbox.radius + 18);
		Data.pos.y = getWsize().y - Texture->height / 8 - 100;
		Hitbox.pos.y = Data.pos.y + (Hitbox.radius + 18);
		Data.runningtime = 0;
		Data.updatetime = 1.0 / 12.0;
		Data.speed = 400;
		NebulaCount++;
	};

};


class Character
{



};


class Alpino
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
	bool onAir = false;
	bool collusion = false;

	Texture2D killua;
	Texture2D far_background;
	Texture2D middle_background;
	Texture2D t_foreground;
	Texture2D nebula;
	Texture2D t_Fronstones;
	std::vector<AnimBackground> farbackground;
	std::vector<AnimBackground> middlebackground;
	std::vector<AnimBackground> foreground;
	std::vector<AnimBackground> fronstones;
	
	std::unique_ptr<Nebula> nebulas[8];

	Animdata killuaData;

	Alpino(Vec2<int> WindowSize);
	~Alpino();

	void update(RenderTexture2D* fbo);
	void draw(RenderTexture2D* fbo);
	
	bool isObjectOut(Animdata data);
    std::string GetRelativeTexturePath(std::string textureName);
	bool isOnGround(Animdata data, int windowHeight);
	Animdata updateAnimdata(Animdata data, float dt, int maxframe, bool onair);
	Vector2 uptadebackgrounds(std::vector<AnimBackground>& data, float dt, bool duplicate);
	void RotateNebula(Animdata data, int windowwidth,int index);
	void static LoadTexture2DfromHeader(Texture2D* texture, unsigned int format, unsigned int height, unsigned int width, unsigned char* data, int mipmaps)
	{
		Image image = { 0 };
		image.format = format;
		image.height = height;
		image.width = width;
		image.data = data;
		image.mipmaps = mipmaps;
		*texture = LoadTextureFromImage(image);
	}	

	std::vector<AnimBackground> initbackgrounds(Texture2D texture, float speed,float scale, Vector2 pos,float distance,int arraysize, std::vector <AnimBackground>& data);
	void DrawandUptadebackgrounds(std::vector<AnimBackground>& data, float dt);
	void drawStones(std::vector <AnimBackground> data, unsigned int size);
	void KilluaJump();
	Vector2 moveStones(std::vector<AnimBackground>& data, float dt, float scale, int index);
private:

};



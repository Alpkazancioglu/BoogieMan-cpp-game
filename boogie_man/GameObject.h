#pragma once
#include <raylib.h>

void LoadTexture2DfromHeader(Texture2D* texture, unsigned int format, unsigned int height, unsigned int width, unsigned char* data, int mipmaps);

class Animdata
{
public:
	Rectangle rec;
	Vector2 pos;
	int frame;
	float runningtime;
	float updatetime;
	int speed;

	void Set(Rectangle rec_i, Vector2 pos_i, int frame_i, float runningtime_i, float updatetime_i, int speed_i) {
		rec = rec_i;
		pos = pos_i;
		frame = frame_i;
		runningtime = runningtime_i;
		updatetime = updatetime_i;
		speed = speed_i;
	};
	bool operator ==(Animdata& other)
	{
		bool result = rec.x == other.rec.x && rec.y == other.rec.y && rec.width == other.rec.width && rec.height == other.rec.height && pos.x == other.pos.x &&
			pos.y == other.pos.y && frame == other.frame && runningtime == other.runningtime && updatetime == other.updatetime && speed == other.speed;
		return result;
	}
};

class CollisionBox
{
public:

	Animdata Data;
	float radius;

	void Set(const Rectangle rec_i, const Vector2 pos_i, int frame_i, float runningtime_i, float updatetime_i, float speed_i, float radius_i)
	{
		Data.Set(rec_i, pos_i, frame_i, runningtime_i, updatetime_i, speed_i);
		this->radius = radius_i;
	}
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


class GameObject
{
public:
	GameObject();
	void InitializeTextureFromHeader(unsigned int format, unsigned int height, unsigned int width, unsigned char* data, int mipmaps);
	void InitializeTextureFromFile(const char* FilePath);
	void Move(Vector2 pos);
	void IncrementPosition(Vector2 IncrAmount);
	void Scale(float scale);
	void Rotate(float degree);
	void SetAnimData(Rectangle rec,Vector2 pos,int frame,float runningtime,float updatetime,int speed);
	void MoveVerticaly();
	void SetTexture(Texture2D &texture);

	Texture2D *Texture;
	Animdata Data;
	CollisionBox Hitbox;
	float scale;
	float rotation;
};
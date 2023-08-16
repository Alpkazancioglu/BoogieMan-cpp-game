#pragma once
#include <raylib.h>
#include <vector>

typedef short int int16;

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
	~GameObject();
	void InitializeTextureFromHeader(unsigned int format, unsigned int height, unsigned int width, unsigned char* data, int mipmaps);
	void InitializeTextureFromFile(const char* FilePath);
	void RenderDuplicateEx(int16 duplicateCount, float distance, Color tint);
	void RenderDuplicateExLoop(int16 duplicateCount, float distance, Color tint, float EndDes, Vector2 StartDes , float dt , bool ReverseSpeed);
	void RenderDuplicateRecLoop(int16 duplicateCount, float distance, Color tint, float EndDes, Vector2 StartDes, float dt, bool ReverseSpeed, int atlas_cut_x = 1, int atlas_cut_y = 1);
	void AllocateRandomDistance(unsigned int duplicateCount, int min, int max, int RandomArrayCount);
	void RenderDuplicateRandomDisEx(unsigned int duplicateCount, int DistanceArrayoffset, Color tint);
	void RenderDuplicateRec(int16 duplicateCount, float distance, Color tint, int atlas_cut_x = 1, int atlas_cut_y = 1);
	void Move(Vector2 pos);
	void IncrementPosition(Vector2 IncrAmount);
	void Scale(float scale);
	void Rotate(float degree);
	void SetAnimData(Rectangle rec,Vector2 pos,int frame,float runningtime,float updatetime,int speed);
	Texture2D* GetTexture();
	void ReferenceCopyTexture(GameObject& Object2CopyTo);
	void ReferenceCopyArrayTexture(std::vector<GameObject> &Object2CopyTo);
	void SetTexture(Texture2D &texture);
	Texture2D *Texture = nullptr;
	Animdata Data;
	CollisionBox Hitbox;
	float scale;
	float rotation;
	
	std::vector<int> RandomDistances;
};
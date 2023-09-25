#pragma once
#include <raylib.h>
#include <vector>
#include "../util/VectorMath.h"
#include "../util/WindowUtil.h"

#define IDLE 0
#define MOVING_FRONT 1
#define MOVING_BACK -1
#define GROUND getWsize().y-26


class BoogieMan;


void LoadTexture2DfromHeader(Texture2D* texture, unsigned int format, unsigned int height, unsigned int width, unsigned char* data, int mipmaps);
struct Collision
{
	int direction;
	bool IsOnTop;
	bool IsColliding;
};

class ObjectData
{
public:
	
	Rectangle rec;
	Collision collision;
	Vec2<float> pos;
	Vec2<float> CurrentSpeed;
	int MovingStatus;
	int frame;
	float runningtime;
	float updatetime;
	int MaxSpeed;
	
	ObjectData()
	{

	}

	ObjectData(Rectangle rec_i, Vec2<float> pos_i, int frame_i, float runningtime_i, float updatetime_i, int speed_i)
	{
		rec = rec_i;
		pos = pos_i;
		frame = frame_i;
		runningtime = runningtime_i;
		updatetime = updatetime_i;
		MaxSpeed = speed_i;
	}

	void Set(Rectangle rec_i, Vec2<float> pos_i, int frame_i, float runningtime_i, float updatetime_i, int speed_i) {
		rec = rec_i;
		pos = pos_i;
		frame = frame_i;
		runningtime = runningtime_i;
		updatetime = updatetime_i;
		MaxSpeed = speed_i;
	};
	
	
	
	
	bool operator ==(ObjectData& other)
	{
		bool result = rec.x == other.rec.x && rec.y == other.rec.y && rec.width == other.rec.width && rec.height == other.rec.height && pos.x == other.pos.x &&
			pos.y == other.pos.y && frame == other.frame && runningtime == other.runningtime && updatetime == other.updatetime && MaxSpeed == other.MaxSpeed;
		return result;
	}

	void operator ()(ObjectData& other)
	{
		rec = other.rec;
		pos = other.pos;
		frame = other.frame;
		runningtime = other.runningtime;
		updatetime = other.updatetime;
		MaxSpeed = other.MaxSpeed;
	}

	

};

class CollisionBox
{
public:

	ObjectData Data;
	float radius;

	void Set(const Rectangle rec_i, const Vec2<float> pos_i, int frame_i, float runningtime_i, float updatetime_i, float speed_i, float radius_i)
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

enum Direction {
	DOWN,
	RIGHT,
	UP,
	LEFT
};



class GameObject
{
public:
	
	GameObject();
	~GameObject();
	void InitializeTextureFromHeader(unsigned int format, unsigned int height, unsigned int width, unsigned char* data, int mipmaps);
	void InitializeTextureFromFile(const char* FilePath);
	void RenderDuplicateEx(int16 duplicateCount, float distance, Color tint);
	void RenderDuplicateExLoop(int16 duplicateCount, float distance, Color tint, float EndDes, Vec2<float> StartDes , float dt , bool ReverseSpeed);
	void RenderDuplicateRecLoop(int16 duplicateCount, float distance, Color tint, float EndDes, Vec2<float> StartDes, float dt, bool ReverseSpeed, int atlas_cut_x = 1, int atlas_cut_y = 1);
	void AllocateRandomDistance(unsigned int duplicateCount, int min, int max, int RandomArrayCount);
	void RenderDuplicateRandomDisEx(unsigned int duplicateCount, int DistanceArrayoffset, Color tint);
	void RenderDuplicateRec(int16 duplicateCount, float distance, Color tint, int atlas_cut_x = 1, int atlas_cut_y = 1);
	void Move(Vec2<float> pos);
	void IncrementPosition(Vector2 IncrAmount);
	void Scale(float scale);
	void SetBaseAttributes(Texture2D& Texture, float scale, ObjectData data, float rotation);
	void Rotate(float degree);
	void SetAnimData(Rectangle rec, Vec2<float> pos,int frame,float runningtime,float updatetime,int speed);
	Texture2D* GetTexture();
	void ReferenceCopyTexture(GameObject& Object2CopyTo);
	void ReferenceCopyArrayTexture(std::vector<GameObject> &Object2CopyTo);
	void SetTexture(Texture2D &texture);
	void SetCollisionInfo(GameObject &object);
	static Direction VectorDirection(glm::vec2 target, float HeightCoeff);
	bool alpCheckCollision(ObjectData obstacle);
	bool isOnGround(ObjectData object);
	Direction DirectionRelativeToObject(ObjectData obstacle);

	inline bool operator< (GameObject& other)
	{
		return this->RenderDepthIndex < other.RenderDepthIndex;
	}
	
	Texture2D *Texture = nullptr;
	ObjectData Data;
	CollisionBox Hitbox;
	
	
	float scale;
	float rotation;
	Color tint;
	uint RenderDepthIndex;
	std::string RenderMethod;
	uint DuplicateCount;
	float duplicateDistance;
	Vector2 atlasCut;
	bool reverseSpeed;
	std::vector<int> RandomDistances;

};

class InstancedGameObject : public GameObject
{
public:
	void SetInstancing(int instanceCount, std::vector<glm::vec3> positionoffsets, Util::Shader& instanceShader);
	void SetInstancing(int instanceCount, std::vector<glm::vec3> positionoffsets);

	~InstancedGameObject();

	std::unique_ptr<bgGL::InstancedTexture2D> InstancedTexture = nullptr;
private:

};

class Item
{

};




class Character : public GameObject
{
public:
	
	
	int MovingStatus;
	float acceleration;
	float MaxJumpHigh;
	float CurrentHigh;
	void updateCharacterTexture(float dt, int maxframe,ObjectData obstacle);
	
	void CharacterMove(float dt,GameObject obstacle,GameObject object);
	
	
};





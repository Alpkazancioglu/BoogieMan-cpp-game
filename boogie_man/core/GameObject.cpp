#include "GameObject.h"
#include <raymath.h>
#include <iostream>
#include "../util/WindowUtil.h"



GameObject::GameObject()
{

}

GameObject::~GameObject()
{
	
}


//Initialize the texture from a header file consists of a byte array
void GameObject::InitializeTextureFromHeader(unsigned int format, unsigned int height, unsigned int width, unsigned char* data, int mipmaps)
{
	LoadTexture2DfromHeader(this->Texture, format, height, width, data, mipmaps);
}

//Initialize the texture from an external file
void GameObject::InitializeTextureFromFile(const char* FilePath)
{
	*this->Texture = LoadTexture(FilePath);
}

//Draw the object with duplicates
void GameObject::RenderDuplicateEx(int16 duplicateCount, float distance , Color tint)
{
	if (this->Texture != nullptr)
	{
		for (size_t i = 0; i < duplicateCount; i++)
		{
			float internalDistance = distance;

			if (i == 0)
			{
				internalDistance = 0;
			}

			DrawTextureEx(*this->Texture, { this->Data.pos.x + (i * this->Texture->width * scale) + internalDistance , this->Data.pos.y }, this->rotation, this->scale, tint);
		}
	}
	else
	{
		std::cerr << "Cannot Render "<< typeid(this).name() << " :: Texture was nullptr" << std::endl;
	}
}

//Draw the object with duplicates and also update the position as a loop
void GameObject::RenderDuplicateExLoop(int16 duplicateCount, float distance, Color tint , float EndDes , Vec2<float> StartDes , float dt, bool ReverseSpeed)
{
	
	if (ReverseSpeed)
	{
		if (this->Data.pos.x >= EndDes)
		{
			this->Move(StartDes);
		}
		this->IncrementPosition({ (float)this->Data.MaxSpeed * dt , 0 });
	}
	else
	{
		if (this->Data.pos.x <= EndDes)
		{
			this->Move(StartDes);
		}
		this->IncrementPosition({ -(float)this->Data.MaxSpeed * dt , 0 });
	}

	if (this->Texture != nullptr)
	{
		for (size_t i = 0; i < duplicateCount; i++)
		{
			float internalDistance = distance;

			if (i == 0)
			{
				internalDistance = 0;
			}

			DrawTextureEx(*this->Texture, { this->Data.pos.x + (i * this->Texture->width * scale) + internalDistance , this->Data.pos.y }, this->rotation, this->scale, tint);
		}
	}
	else
	{
		std::cerr << "Cannot Render " << typeid(this).name() << " :: Texture was nullptr" << std::endl;
	}

}

void GameObject::RenderDuplicateRecLoop(int16 duplicateCount, float distance, Color tint, float EndDes, Vec2<float> StartDes, float dt, bool ReverseSpeed , int atlas_cut_x, int atlas_cut_y)
{
	if (ReverseSpeed)
	{
		if (this->Data.pos.x >= EndDes)
		{
			this->Move(StartDes);
		}
		this->IncrementPosition({ (float)this->Data.MaxSpeed * dt , 0 });
	}
	else
	{
		if (this->Data.pos.x <= EndDes)
		{
			this->Move(StartDes);
		}
		this->IncrementPosition({ -(float)this->Data.MaxSpeed * dt , 0 });
	}

	if (this->Texture != nullptr)
	{
		for (size_t i = 0; i < duplicateCount; i++)
		{
			float internalDistance = distance;

			if (i == 0)
			{
				internalDistance = 0;
			}

			Rectangle destRec = { this->Data.pos.x + ((i * (this->Texture->width / atlas_cut_x) * scale)) + internalDistance ,this->Data.pos.y , ((float)this->Texture->width / atlas_cut_x) * scale , ((float)this->Texture->height / atlas_cut_y) * scale };
			Vector2 CenterPoint = { 0 , 0 };

			DrawTexturePro(*this->Texture, this->Data.rec, destRec, CenterPoint, rotation, tint);
		}
	}
	else
	{
		std::cerr << "Cannot Render " << typeid(this).name() << " :: Texture was nullptr" << std::endl;
	}
}

//Fill the random distance vector with random integers
void GameObject::AllocateRandomDistance(unsigned int DistanceCount, int min, int max, int RandomArrayCount)
{
	for (size_t i = 0; i < DistanceCount; i++)
	{
		this->RandomDistances.push_back(GiveRandomNumf(min,max,RandomArrayCount,false,0));
	}
}

//Draw the object with its duplicates with random distances between them (Required to use AllocateRandomDistance() in init faze)
void GameObject::RenderDuplicateRandomDisEx(unsigned int duplicateCount , int DistanceArrayoffset, Color tint )
{
	if (this->Texture != nullptr)
	{

		for (size_t i = DistanceArrayoffset; i < duplicateCount; i++)
		{
			float internalDistance = RandomDistances.at(i);

			if (i == 0)
			{
				internalDistance = 0;
			}

			DrawTextureEx(*this->Texture, { this->Data.pos.x + (i * this->Texture->width * scale) + internalDistance , this->Data.pos.y}, this->rotation, this->scale, tint);
		}
	}
	else
	{
		std::cerr << "Cannot Render " << typeid(this).name() << " :: Texture was nullptr" << std::endl;
	}	
}

//Draw the object with duplicates
void GameObject::RenderDuplicateRec(int16 duplicateCount, float distance, Color tint, int atlas_cut_x  , int atlas_cut_y )
{
	if (this->Texture != nullptr)
	{
		for (size_t i = 0; i < duplicateCount; i++)
		{
			float internalDistance = distance;

			if (i == 0)
			{
				internalDistance = 0;
			}

			Rectangle destRec = { this->Data.pos.x + ((i * (this->Texture->width / atlas_cut_x) * scale)) + internalDistance ,this->Data.pos.y , ((float)this->Texture->width / atlas_cut_x) * scale , ((float)this->Texture->height / atlas_cut_y) * scale };
			Vector2 CenterPoint = { 0 , 0 };

			DrawTexturePro(*this->Texture,  this->Data.rec  ,destRec,CenterPoint,rotation, tint);
		}
	}
	else
	{
		std::cerr << "Cannot Render " << typeid(this).name() << " :: Texture was nullptr" << std::endl;
	}
}

//Move the object to the given position on the screen
void GameObject::Move(Vec2<float> pos)
{
	this->Data.pos = pos;
}

//Add the given amount of distance to the existing position
void GameObject::IncrementPosition(Vector2 IncrAmount)
{
	this->Data.pos = { this->Data.pos.x + IncrAmount.x , this->Data.pos.y + IncrAmount.y };
}

//Set the scale
void GameObject::Scale(float scale)
{
	this->scale = scale;
}

void GameObject::SetBaseAttributes(Texture2D& Texture, float scale, ObjectData data, float rotation)
{
	this->SetTexture(Texture);
	this->scale = scale;
	this->Data(data);
	this->rotation = rotation;
}

//Set the rotation in degrees
void GameObject::Rotate(float degree)
{
	this->rotation += degree;
}

//Set Data
void GameObject::SetAnimData(Rectangle rec, Vec2<float> pos, int frame, float runningtime, float updatetime, int speed)
{
	this->Data.Set(rec, pos, frame, runningtime, updatetime, speed);
}
//Get the Texture2D pointer of the texture existing in the object(By default it returns nullptr)
Texture2D* GameObject::GetTexture()
{
	return this->Texture;
}

//Copy the existing texture to another object
void GameObject::ReferenceCopyTexture(GameObject& Object2CopyTo)
{
	Object2CopyTo.Texture = this->Texture;
}

//Copy the existing texture to an array of objects
void GameObject::ReferenceCopyArrayTexture(std::vector<GameObject>& Object2CopyTo)
{
	for (GameObject object : Object2CopyTo)
	{
		object.Texture = this->Texture;
	}
}

//Reference copy another texture to this one
void GameObject::SetTexture(Texture2D &texture)
{
	this->Texture = &texture;
}

void GameObject::SetCollisionInfo(GameObject object)
{
	
	this->Data.collision.direction = object.DirectionRelativeToObject(this->Data);
	this->Data.collision.IsOnTop = object.isOnGround(this->Data);
	this->Data.collision.IsColliding = object.alpCheckCollision(this->Data);



}



void InstancedGameObject::SetInstancing(int instanceCount, std::vector<glm::vec3> positionoffsets, Util::Shader& instanceShader)
{
	InstancedTexture = std::make_unique<bgGL::InstancedTexture2D>(instanceCount, *this->Texture, positionoffsets, instanceShader);
}

void InstancedGameObject::SetInstancing(int instanceCount, std::vector<glm::vec3> positionoffsets)
{
	InstancedTexture = std::make_unique<bgGL::InstancedTexture2D>(instanceCount, *this->Texture, positionoffsets);
}

InstancedGameObject::~InstancedGameObject()
{
	if (InstancedTexture != nullptr)
	{
		this->InstancedTexture->clean();
	}
}



bool GameObject::alpCheckCollision(ObjectData obstacle)
{
	return CheckCollisionRecs({ this->Data.pos.x + this->Data.CurrentSpeed.x,this->Data.pos.y+this->Data.CurrentSpeed.y,this->Data.rec.width,this->Data.rec.height }, { obstacle.rec });
}

void Character::updateCharacterTexture(float dt, int maxframe, ObjectData obstacle)
{
	
		
		if (!isOnGround(obstacle))
		{	
			
			if (this->MovingStatus == 1)
			{
				this->Data.frame = 0;
				this->Data.rec.x = Data.frame * Data.rec.width;
			
			}
			else if (this->MovingStatus == -1)
			{
				this->Data.frame = 11;
				this->Data.rec.x = Data.frame * Data.rec.width;
			}	
		}
		
		
		else if (this->MovingStatus == 1)
		{
			this->Data.runningtime += dt;
			
			if (Data.frame > maxframe)
			{
				this->Data.frame = 0;
				this->Data.rec.x = Data.frame * Data.rec.width;
			}
			
			if (Data.updatetime - Data.runningtime <= 0)
			{
				this->Data.runningtime = 0.0;
				this->Data.rec.x = Data.frame * Data.rec.width;
				this->Data.frame++;
				if (Data.frame > maxframe)
					this->Data.frame = 0;
				
			}
		}
		else if (this->MovingStatus == -1)
		{

			this->Data.runningtime += dt;
			if (Data.frame <= maxframe)
			{
				this->Data.frame = maxframe + 1;
				this->Data.rec.x = Data.frame * Data.rec.width;
			}
			if (Data.updatetime - Data.runningtime <= 0)
			{
				this->Data.runningtime = 0.0;
				this->Data.rec.x = Data.frame * Data.rec.width;
				this->Data.frame++;
				if (Data.frame > (maxframe * 2) + 1)
					this->Data.frame = maxframe + 1;
			}
		}

}


bool GameObject::isOnGround(ObjectData object)
{
	
	if (this->alpCheckCollision(object) && this->Data.collision.direction == UP) return true;
	
	return this->Data.pos.y + this->Data.rec.height >= GROUND;
}


Direction GameObject::DirectionRelativeToObject (ObjectData obstacle)
{
	static int fix = 0;
	static bool allow = false;
	
	double angle = atan2((obstacle.rec.y+fix) - (this->Data.pos.y + this->Data.rec.height), (this->Data.pos.x + this->Data.rec.width / 2) - (obstacle.rec.x + obstacle.rec.width / 2)) * 180 / PI;
	angle = fmod(angle, 360);
	if (angle < 0) angle += 360;
	if (angle == 360) angle = 0;
	
	std::cout << " Angle :: " << fix << std::endl;

	if (this->alpCheckCollision(obstacle) && angle >= 0 && angle < 180)
		allow = true;
	
	if (allow && !this->alpCheckCollision(obstacle))
	{
		if(angle < 90)
		{
			allow = false;
			fix = 0;
			return RIGHT;
		}
		else
		{
			allow = false;
			fix = 0;
			return  LEFT;
		}
	
	}
	
	
	
	if (angle >= 0 && angle < 180)
	{
		fix = 20;
		
		return UP;
	}
	
	
	else if (angle > 270 && angle <= 360)
	{
		fix = 0;
		return RIGHT;
	}
	
	else if (angle >= 180 && angle <= 270)
	{
		fix = 0;
		return LEFT;
	}
}






void Character::CharacterMove(float dt, GameObject obstacle, GameObject object)
{
	
	//this->direction = this->DirectionRelativeToObject(obstacle);
	//std::cout << "DIRECTION = " << this->direction << std::endl;
	//bool IsOnGround = this->isOnGround(obstacle);
	//bool IsColliding = this->alpCheckCollision(obstacle);
	obstacle.SetCollisionInfo(object);
	

	static bool isMaxHeightReached = false;
	static bool isPlayerJumped = false;
	static bool Slowing = false;
	
	float current_high = this->Data.pos.y;
	static Vec2<float> LocalSpeed = {};
	static float max_high = this->Data.pos.y - 100;
	
	std::cout << "SPEED" << this->Data.CurrentSpeed << std::endl;
	std::cout << "isonground " << Data.collision.IsOnTop << std::endl;
	std::cout <<"iscolliding" << Data.collision.IsColliding << std::endl;
	static float maxSpeed = this->Data.MaxSpeed;
	const float gravity = 1000;
	const float acceleration = 16.0f;
	this->Data.CurrentSpeed = LocalSpeed*dt;
	this->Data.pos.y += LocalSpeed.y * dt;
	
	//Y axis Movement
	
	if (obstacle.Data.collision.IsOnTop && IsKeyDown(KEY_SPACE))
	{
		if (!obstacle.Data.collision.IsOnTop)
			isPlayerJumped = true;
		
		
		LocalSpeed.y = 0;
	}
	else if (obstacle.Data.collision.IsOnTop)
	{
		max_high = this->Data.pos.y - 100;
		isMaxHeightReached = false;
		LocalSpeed.y = 0;
		isPlayerJumped = false;
	}
	else
	{
		LocalSpeed.y += gravity * dt;
	}
	if (max_high >= current_high)
		isMaxHeightReached = true;
	
	if (IsKeyDown(KEY_SPACE) && !isMaxHeightReached && !isPlayerJumped)
	{
		LocalSpeed.y = -300;
	}
	else if (IsKeyReleased(KEY_SPACE))
	{
		isMaxHeightReached = true;
	}
	
	// X Axis Movement
	
	if (IsKeyDown(KEY_LEFT_SHIFT))
	{
		maxSpeed = this->Data.MaxSpeed * 2;
	}

	
	if (IsKeyReleased(KEY_A) || IsKeyReleased(KEY_D) || IsKeyReleased(KEY_LEFT_SHIFT))
	{

		Slowing = true;
		maxSpeed = this->Data.MaxSpeed;
	}

	if (IsKeyDown(KEY_D))
	{
		if (( obstacle.Data.collision.IsColliding && obstacle.Data.collision.direction == LEFT))
		{
			this->Data.pos.x = obstacle.Data.pos.x - this->Data.rec.width;
			LocalSpeed.x = 0;
			this->MovingStatus = IDLE;
		}
		
		else
		{
			if (LocalSpeed.x <= this->Data.MaxSpeed)
				Slowing = false;
			
			if (!Slowing)
			{

				this->MovingStatus = MOVING_FRONT;
				if (LocalSpeed.x < 0)
					LocalSpeed.x += acceleration * 6;

				if (LocalSpeed.x <= maxSpeed)
					LocalSpeed.x += acceleration;

				this->Data.pos.x += LocalSpeed.x * dt;
			}
		}
	}
	else if (IsKeyDown(KEY_A))
	{
		if ((obstacle.Data.collision.IsColliding && obstacle.Data.collision.direction == RIGHT))
		{
			this->Data.pos.x = obstacle.Data.pos.x + obstacle.Data.rec.width;
			LocalSpeed.x = 0;
			this->MovingStatus = IDLE;
		}
		
		else
		{

			if (LocalSpeed.x >= -this->Data.MaxSpeed)
				Slowing = false;
			if (!Slowing)
			{
				this->MovingStatus = MOVING_BACK;
				if (LocalSpeed.x > 0)
					LocalSpeed.x -= acceleration * 6;

				if (LocalSpeed.x >= -maxSpeed)
					LocalSpeed.x -= acceleration;

				this->Data.pos.x += LocalSpeed.x * dt;
			}
		}
	}

	if (Slowing)
	{

		if (LocalSpeed.x > 0)
		{

			if (LocalSpeed.x >= this->Data.MaxSpeed)
				LocalSpeed.x -= acceleration * 2;

			else
				LocalSpeed.x -= acceleration * 10;

			this->Data.pos.x += LocalSpeed.x * dt;

			if (LocalSpeed.x <= 0)
			{
				this->MovingStatus = IDLE;
				LocalSpeed.x = 0; 
				Slowing = false;
			}
		}

		else if (LocalSpeed.x < 0)
		{
			if (LocalSpeed.x <= -this->Data.MaxSpeed)
				LocalSpeed.x += acceleration * 2;

			else
				LocalSpeed.x += acceleration * 10;

			this->Data.pos.x += LocalSpeed.x * dt;
			if (LocalSpeed.x >= 0)
			{
				this->MovingStatus = IDLE;
				LocalSpeed.x = 0;
				Slowing = false;
			}
		}

	}





}

Direction GameObject::VectorDirection(glm::vec2 target , float HeightCoeff)
{
	glm::vec2 compass[] = {
		glm::vec2(0.0f, 1.0f),    // up
		glm::vec2(1.0f, 0.0f),    // right
		glm::vec2(0.0f, -1.0f),    // down
		glm::vec2(-1.0f, 0.0f)    // left
	};
	float max = 0.0f;
	unsigned int best_match = -1;
	for (unsigned int i = 0; i < 4; i++)
	{
		glm::vec2 TargetVector(target.x / getWsize().x, (target.y / getWsize().y) * HeightCoeff);
		//std::cout << "TargetVector: " << Vec2<float>(TargetVector.x , TargetVector.y) << std::endl;
		float dot_product = glm::dot(TargetVector, compass[i]);
		//std::cout << "dot_product: " << dot_product << std::endl;

		if (dot_product >= max)
		{
			max = dot_product;
			best_match = i;
		}
	}
	return (Direction)best_match;

}




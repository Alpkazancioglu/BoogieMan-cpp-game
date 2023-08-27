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
		this->IncrementPosition({ (float)this->Data.speed * dt , 0 });
	}
	else
	{
		if (this->Data.pos.x <= EndDes)
		{
			this->Move(StartDes);
		}
		this->IncrementPosition({ -(float)this->Data.speed * dt , 0 });
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
		this->IncrementPosition({ (float)this->Data.speed * dt , 0 });
	}
	else
	{
		if (this->Data.pos.x <= EndDes)
		{
			this->Move(StartDes);
		}
		this->IncrementPosition({ -(float)this->Data.speed * dt , 0 });
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




void Character::updateCharacterTexture(float dt, int maxframe,int &MoveEverything)
{
	
		if (!isCharacterGround())
		{	
			
			if (MoveEverything == 1)
			{
				this->Data.frame = 0;
				this->Data.rec.x = Data.frame * Data.rec.width;
			
			}
			else if (MoveEverything == -1)
			{
				this->Data.frame = 11;
				this->Data.rec.x = Data.frame * Data.rec.width;
			}	
		}
		
		
		else if (MoveEverything == 1)
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
		else if (MoveEverything == -1)
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
	


bool Character::isCharacterGround()
{
		return Data.pos.y + Data.rec.height >= GetMonitorHeight(GetCurrentMonitor()) - 26;
}

void Character::updateMovingState(int& MoveEverything)
{
	if (IsKeyDown(KEY_D))
		//MoveEverything = MOVING_FRONT;
		this->Data.pos.x += 30;
	else if (IsKeyDown(KEY_A))
		//MoveEverything = MOVING_BACK;
		this->Data.pos.x -= 30;
	else if (!(IsKeyUp(KEY_A) && IsKeyUp(KEY_D)))
		//MoveEverything = 0;
		this->Data.pos.x;
}


//Load a texture from a header file consists of an image byte array
void LoadTexture2DfromHeader(Texture2D* texture, unsigned int format, unsigned int height, unsigned int width, unsigned char* data, int mipmaps)
{
	Image image = { 0 };
	image.format = format;
	image.height = height;
	image.width = width;
	image.data = data;
	image.mipmaps = mipmaps;
	*texture = LoadTextureFromImage(image);
}
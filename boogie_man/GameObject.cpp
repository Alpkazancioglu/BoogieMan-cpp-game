#include "GameObject.h"
#include <iostream>
#include "VectorMath.h"



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
			DrawTextureEx(*this->Texture, { this->Data.pos.x + (i * this->Texture->width) + distance , this->Data.pos.y }, this->rotation, this->scale, tint);
		}
	}
	else
	{
		std::cerr << "Cannot Render "<< typeid(this).name() << " :: Texture was nullptr" << std::endl;
	}
}

//Draw the object with duplicates and also update the position as a loop
void GameObject::RenderDuplicateExLoop(int16 duplicateCount, float distance, Color tint , float EndDes , Vector2 StartDes , float dt)
{

	if (this->Data.pos.x <= EndDes)
	{
		this->Move(StartDes);
	}

	this->IncrementPosition({ -(float)this->Data.speed * 8 * dt , 0 });

	if (this->Texture != nullptr)
	{
		for (size_t i = 0; i < duplicateCount; i++)
		{
			DrawTextureEx(*this->Texture, { this->Data.pos.x + (i * this->Texture->width) + distance , this->Data.pos.y }, this->rotation, this->scale, tint);
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
void GameObject::RenderDuplicateRandomDisEx(unsigned int duplicateCount , int DistanceArrayoffset, Color tint)
{
	if (this->Texture != nullptr)
	{
		for (size_t i = DistanceArrayoffset; i < duplicateCount; i++)
		{
			DrawTextureEx(*this->Texture, { this->Data.pos.x + (i * this->Texture->width) + RandomDistances.at(i) , this->Data.pos.y}, this->rotation, this->scale, tint);
		}
	}
	else
	{
		std::cerr << "Cannot Render " << typeid(this).name() << " :: Texture was nullptr" << std::endl;
	}	
}

//Draw the object with duplicates
void GameObject::RenderDuplicateRec(int16 duplicateCount, float distance, Color tint)
{
	if (this->Texture != nullptr)
	{
		for (size_t i = 0; i < duplicateCount; i++)
		{
			DrawTextureRec(*this->Texture,this->Data.rec,{ this->Data.pos.x + (i * this->Texture->width) + distance , this->Data.pos.y }, tint);
		
		}
	}
	else
	{
		std::cerr << "Cannot Render " << typeid(this).name() << " :: Texture was nullptr" << std::endl;
	}
}

//Move the object to the given position on the screen
void GameObject::Move(Vector2 pos)
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

//Set the rotation in degrees
void GameObject::Rotate(float degree)
{
	this->rotation += degree;
}

//Set Data
void GameObject::SetAnimData(Rectangle rec, Vector2 pos, int frame, float runningtime, float updatetime, int speed)
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
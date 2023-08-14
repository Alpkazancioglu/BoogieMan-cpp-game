#include "GameObject.h"

GameObject::GameObject()
{
}

void GameObject::InitializeTextureFromHeader(unsigned int format, unsigned int height, unsigned int width, unsigned char* data, int mipmaps)
{
	LoadTexture2DfromHeader(this->Texture, format, height, width, data, mipmaps);
}

void GameObject::InitializeTextureFromFile(const char* FilePath)
{
	*this->Texture = LoadTexture(FilePath);
}

void GameObject::Move(Vector2 pos)
{
	this->Data.pos = pos;
}

void GameObject::IncrementPosition(Vector2 IncrAmount)
{
	this->Data.pos = { this->Data.pos.x + IncrAmount.x , this->Data.pos.y + IncrAmount.y };
}

void GameObject::Scale(float scale)
{
	this->scale = scale;
}

void GameObject::Rotate(float degree)
{
	this->rotation += degree;
}

void GameObject::SetAnimData(Rectangle rec, Vector2 pos, int frame, float runningtime, float updatetime, int speed)
{
	this->Data.Set(rec, pos, frame, runningtime, updatetime, speed);
}

void GameObject::MoveVerticaly()
{
	//this->IncrementPosition({0})



	/*if (duplicate)
	{
		Data[1].pos.x = data[0].pos.x + data[1].texture.width * data[1].scale;
		return data[1].pos;
	}
	else
	{
		data[0].pos.x -= data[0].speed * dt;
		if (data[0].pos.x - data[REAL].origin.x <= -data[0].texture.width * data[0].scale)
		{
			return data[REAL].origin;
		}
		else
		{
			return data[0].pos;
		}
	}*/
}

void GameObject::SetTexture(Texture2D &texture)
{
	this->Texture = &texture;
}




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
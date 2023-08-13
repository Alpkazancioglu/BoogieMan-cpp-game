#include "ECS.h"
#include <iostream>
#include <raylib.h>
#include <vector>
#include <unordered_map>
#include <bitset>

entity::entity()
{
	static unsigned int countofentity = 0;
	entity_id = countofentity;
	countofentity++;
}

entity::~entity()
{
	for (componentbase* v : componentlist)
	{
		delete v;
	}

	componentlist.clear();

	std::cout << "Component list size: " << componentlist.size() << std::endl;
}

ecs::~ecs()
{
	
	for (auto& it:entitylist)
	{
		delete entitylist[it.first];
	}

	auto it = entitylist.begin();
	entitylist.erase(it, entitylist.end());
	std::cout <<"Entity list size: " << entitylist.size() << std::endl;
}

unsigned int ecs::create_entity(const char* entity_name)
{
	entity* newentity = new entity;
	entitylist[entity_name] = newentity;
	return newentity->entity_id;
}





basic_abilities::~basic_abilities()
{
	UnloadTexture(texture);
}

void basic_abilities::init_basic_abilities(const char* image_src, int initial_x, int initial_y, float initial_rotation, float initial_scale)
{
	static unsigned int count_of_entities = 0;
	id = count_of_entities;
	texture = LoadTexture(image_src);
	position.x = initial_x;
	position.y = initial_y;
	rotation = initial_rotation;
	scale = initial_scale;
	count_of_entities++;
}


void basic_abilities::update(int xpos, int ypos, float mainrotation, float mainscale)
{
	position.x += xpos;
	position.y += ypos;
	rotation += mainrotation;
	scale += mainscale;
}

void basic_abilities::replaceindex(int xpos, int ypos, float mainrotation, float mainscale)
{
	position.x = xpos;
	position.y = ypos;
	rotation = mainrotation;
	scale = mainscale;
}

void basic_abilities::render(Color tint)
{
	DrawTextureEx(texture, position, rotation, scale, tint);
}

void basic_abilities::GravityPhysics(Vector2 object_to_collide, int magnitude_of_gravity)
{
	if (position.y <= object_to_collide.y)
	{
		update(0, magnitude_of_gravity, 0, 0);
	}
	if (position.y >= object_to_collide.y)
	{
		update(0, -magnitude_of_gravity, 0, 0);
	}
}

void basic_abilities::clean(basic_abilities* current_object)
{
	
}


void PlayerArrowControl::ArrowControl(int right, int left, int up, int down, Vector2 *position)
{

	if (IsKeyPressed(KEY_RIGHT) || IsKeyDown(KEY_RIGHT))
	{
		position->x += right;
		position->y += 0;
		
	}
	if (IsKeyPressed(KEY_LEFT) || IsKeyDown(KEY_LEFT))
	{
		position->x += left;
		position->y += 0;
		
	}
	if (IsKeyPressed(KEY_UP))
	{
		position->x += 0;
		Jump(position, 5.0f);
		
	}
	if (IsKeyPressed(KEY_DOWN) || IsKeyDown(KEY_DOWN))
	{
		position->x += 0;
		position->y += down;
		
	}

}

void PlayerArrowControl::Jump(Vector2* position , float amount)
{
	for (size_t i = 0; i < amount; i++)
	{
		for (size_t j = 0; j < 20; j++)
		{
			position->y -= i;
		}
	}
	
}

unsigned int PossessItem::possess_new_item()
{
	static unsigned int count = 0;
	itemlist[count] = new entity;
	count++;

	return count;
}

void PossessItem::dispossess(unsigned int item_id)
{
	

}

void PossessItem::BinditemCurrent(unsigned int item_id)
{
	current = itemlist[item_id];
}



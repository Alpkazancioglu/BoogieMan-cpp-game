#include "GameEntity.h"


Game_Entity::Game_Entity(const char* image_src, int initial_x, int initial_y ,float initial_rotation, float initial_scale)
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


void Game_Entity::update(int xpos, int ypos , float mainrotation, float mainscale)
{
	position.x += xpos;
	position.y += ypos;
	rotation += mainrotation;
	scale += mainscale;
}

void Game_Entity::replaceindex(int xpos, int ypos, float mainrotation, float mainscale)
{
	position.x = xpos;
	position.y = ypos;
	rotation = mainrotation;
	scale = mainscale;
}

void Game_Entity::render(Color tint)
{
	DrawTextureEx(texture, position, rotation, scale, tint);
}

void Game_Entity::GravityPhysics(Vector2 object_to_collide , int magnitude_of_gravity)
{
	if(position.y <= object_to_collide.y)
	{
		update(0, magnitude_of_gravity, 0, 0);
	}
	if (position.y >= object_to_collide.y)
	{
		update(0, -magnitude_of_gravity, 0, 0);
	}
	
}

//void Game_Entity::PushBackComponent(component_base component)
//{
	//components.push_back(component);
	
//}


/*template<typename T>
void Game_Entity::PushBackComponent(T* component)
{
	components.push_back(component);
}

void Game_Entity::clear_component_list()
{
	for (size_t i = 0; i < components.size(); i++)
	{
		delete components[i];
	}
}


void* Game_Entity::ReturnComponent(int component_id)
{
	return components.at(component_id);
}



void Game_Entity::clean(Game_Entity *current_object)
{
	delete current_object;
}

Vector2 Game_Entity::GetObjectPosition()
{
	return position;
}







component_base::component_base()
{
	static unsigned int ComponentCount = 0;
	id_num = ComponentCount;
	ComponentCount++;
}

unsigned int component_base::get_id_num()
{
	return id_num;
}




unsigned int component_base::get_numberofcomponents()
{
	return ComponentCount;
}


void EnemyAttack::Attack_component(Game_Entity* Entitytoattack)
{





}





void ecs::Push_back_entity(Game_Entity* entitytopushback)
{
	Entities.push_back(entitytopushback);
}

unsigned int ecs::CreateNewEntity(const char* image_src, int initial_x, int initial_y, float initial_rotation, float initial_scale)
{
	Game_Entity* ptr = new Game_Entity(image_src, initial_x, initial_y, initial_rotation, initial_scale);
	Entities.push_back(ptr);
	
	return Entities.size();
}

Game_Entity *ecs::Return_Entity(int entityid)
{
	return Entities.at(entityid);
}

void ecs::clear_entity_list()
{
	for (Game_Entity* v : Entities)
	{
		delete v;
		std::cout << "Entity Deleted" << std::endl;
	}
}

void ecs::DeleteEntity(int entity_id)
{
	Entities.erase(Entities.begin() + entity_id);
}




void PlayerArrowControl::ArrowControl(int right, int left, int up, int down , Game_Entity Entity_to_use)
{

	*temp_entity = Entity_to_use;

	if (IsKeyPressed(KEY_RIGHT) || IsKeyDown(KEY_RIGHT))
	{

		temp_entity->update(right, 0, 0, 0);

	}
	if (IsKeyPressed(KEY_LEFT) || IsKeyDown(KEY_LEFT))
	{
		temp_entity->update(left, 0, 0, 0);

	}
	if (IsKeyPressed(KEY_UP) || IsKeyDown(KEY_UP))
	{
		temp_entity->update(0, up, 0, 0);

	}
	if (IsKeyPressed(KEY_DOWN) || IsKeyDown(KEY_DOWN))
	{
		temp_entity->update(0, down, 0, 0);

	}

}*/

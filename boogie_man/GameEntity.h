#pragma once
#include <iostream>
#include <raylib.h>
#include <vector>
#include <unordered_map>


class Game_Entity
{
public:

	Game_Entity(const char* image_src, int initial_x, int initial_y, float initial_rotation, float initial_scale);
	//Inputu baslangic miktarina ekler(Scale haric)
	void update(int xpos, int ypos, float mainrotation, float mainscale);
	void replaceindex(int xpos, int ypos, float mainrotation, float mainscale);
	void render(Color tint);
	void GravityPhysics(Vector2 object_to_collide, int magnitude_of_gravity);
	void clean(Game_Entity* current_object);
	Vector2 GetObjectPosition();
	unsigned int return_id() { return id; }

private:

	Vector2 position;
	float rotation;
	float scale;
	Texture2D texture;
	unsigned int id;

};


//Components for extra abilities 
class component_base
{
private:

	unsigned int id_num;
	unsigned int related_entity_id;

public:

	component_base();
	virtual unsigned int get_id_num();
	virtual unsigned int get_numberofcomponents();

};



//Basic arrow key control with user input
class PlayerArrowControl :public component_base
{
private:

	Game_Entity* temp_entity;

public:

	virtual unsigned int get_id_num() override;
	virtual unsigned int get_numberofcomponents() override;
	static void ArrowControl(int right, int left, int up, int down, Game_Entity Entity_to_use);

};


class ecs
{
private:

	std::vector<Game_Entity*> Entities;
	std::unordered_map<unsigned int, component_base*> component_array;

public:

	void Push_back_entity(Game_Entity* entitytopushback);
	unsigned int CreateNewEntity(const char* image_src, int initial_x, int initial_y, float initial_rotation, float initial_scale);
	Game_Entity* Return_Entity(int entityid);
	void clear_entity_list();
	void DeleteEntity(int entity_id);

	template<typename T>
	void PushBackComponent(T* component ,unsigned int entity_id);

	void clear_component_list(unsigned int entity_id);
	void* ReturnComponent(int component_id , unsigned int entity_id);
};
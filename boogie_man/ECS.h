#pragma once
#include <iostream>
#include <vector>
#include <array>
#include <unordered_map>
#include <bitset>
#include <raylib.h>

class entity;

class componentbase
{
public:

	virtual void write()
	{
		std::cout << "baseone" << std::endl;

	}

private:


};


class somerandomcomponent :public componentbase
{
public:

	void write() override
	{
		std::cout << "childone" << std::endl;

	}

	void write_unique()
	{
		std::cout << "This is a new function" << std::endl;
	}

private:

};

class basic_abilities:public componentbase
{
public:

	~basic_abilities();
	void init_basic_abilities(const char* image_src, int initial_x, int initial_y, float initial_rotation, float initial_scale);
	//Inputu baslangic miktarina ekler(Scale haric)
	void update(int xpos, int ypos, float mainrotation, float mainscale);
	void replaceindex(int xpos, int ypos, float mainrotation, float mainscale);
	void render(Color tint);
	void GravityPhysics(Vector2 object_to_collide, int magnitude_of_gravity);
	void clean(basic_abilities* current_object);
	Vector2* GetObjectPosition() { return &position; };
	float returnscale() { return scale; };
	unsigned int return_id() { return id; }

private:

	Vector2 position;
	float rotation;
	float scale;
	Texture2D texture;
	unsigned int id;

};


class PlayerArrowControl :public componentbase
{
public:

	void ArrowControl(int right, int left, int up, int down, Vector2 *position);
	void Jump(Vector2* position, float amount);

};


class Weapon_Attack:public componentbase
{
public:

	virtual void attack();
	virtual void render();

};

class PossessItem :public componentbase
{
private:

	entity* current;
	std::array<entity*, 30> itemlist;

public:

	~PossessItem();
	unsigned int possess_new_item();
	void dispossess(unsigned int item_id);
	void BinditemCurrent(unsigned int item_id);

	template<typename T>
	inline T* DisplayCurrent(const char* name_of_the_entity)
	{
		
	}

};


class entity
{
public:

	entity();
	~entity();
	unsigned int entity_id;
	std::vector<componentbase*> componentlist;

};


class ecs
{
public:

	std::unordered_map<const char*, entity*> entitylist;

	~ecs();

	unsigned int create_entity(const char* entity_name);

	template<typename typeofcomponent2>
	inline unsigned int addnewcomponent(const char* entity_name)
	{
		static unsigned int keycount = 0;

		typeofcomponent2* newcomponent = new typeofcomponent2;

		componentbase* ptr = newcomponent;

		entitylist[entity_name]->componentlist.push_back(ptr);

		unsigned int temp = keycount;

		keycount++;

		return temp;
	}

	template<typename typeofcomponent3>
	inline typeofcomponent3* returncomponent(const char* entity_name, unsigned int component_id)
	{
		return (typeofcomponent3*)entitylist[entity_name]->componentlist.at(component_id);
	}


};





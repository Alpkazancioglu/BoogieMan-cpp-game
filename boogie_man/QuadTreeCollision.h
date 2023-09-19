#pragma once
#include <iostream>
#include "util/VectorMath.h"
#include "core/GameObject.h"
#include <vector>
#include <string>

namespace QT
{

	class Quad
	{
	public:

		Quad():next(nullptr),prev(nullptr)
		{};

		std::vector<GameObject*> objects;
		Vec4<float> Attrib;
		unsigned int Index;
		Quad* next;
		Quad* prev;

	private:
	};

	void ContructQuads(Quad* HeadQuad,std::vector<GameObject*>& Objects, Vec2<float> DesiredQuadSize);
	bool CheckCollision(GameObject& obj1, GameObject& obj2);
	std::vector<GameObject*> FetchOnScreen(std::vector<GameObject*>& Objects);
	void SortObjectsPosition(std::vector<GameObject*>& Objects);

	void FreeQuads(Quad* HeadQuad);
	void InsertNode(Quad* HeadQuad);
	void DeleteLastNode(Quad* HeadQuad);
	void InitList(Quad* HeadQuad);
}


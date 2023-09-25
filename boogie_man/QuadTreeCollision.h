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

		Quad():next(nullptr),prev(nullptr),Index(-1)
		{};

		std::vector<GameObject*> objects;
		Vec4<float> Attrib;
		int Index;
		Quad* next;
		Quad* prev;

	private:
	};

	void ContructQuads(Quad*& HeadQuad,std::vector<GameObject*>& Objects, Vec2<float> DesiredQuadSize, Camera2D& camera);
	void SubdivideQuad(Quad*& QuadToSubdivide, std::vector<GameObject*>& InSameQuad);
	bool CheckCollision(GameObject& obj1, GameObject& obj2);
	std::vector<GameObject*> FetchOnScreen(std::vector<GameObject*>& Objects, Camera2D& camera);
	void SortObjectsPosition(std::vector<GameObject*>& Objects);

	void FreeQuads(QT::Quad*& HeadQuad);
	void InsertNode(Quad*& HeadQuad);
	void DeleteLastNode(Quad*& HeadQuad);
	void InitList(QT::Quad*& HeadQuad);
	void DeleteNode(Quad*& nodeToDelete);
	Quad* GetNodeWithIndex(Quad*& headnode, int index);
}


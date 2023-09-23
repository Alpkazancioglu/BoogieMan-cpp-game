#include "QuadTreeCollision.h"
#include "util/Log.h"
#include <string>

static unsigned int NodeCount = 0;
static QT::Quad* LastNode = nullptr;
static Vec2<float> PrevDesiredQuadSize;

void QT::ContructQuads(Quad* HeadQuad, std::vector<GameObject*>& Objects,Vec2<float> DesiredQuadSize , Camera2D &camera)
{
	//HeadQuad = new Quad;
	//FreeQuads(HeadQuad);
	//InitList(HeadQuad);

	Vec2<unsigned int> GridCount = (getWsize() / DesiredQuadSize).Cast<unsigned int>();

	std::vector<GameObject*> ObjectsOnScreen = FetchOnScreen(Objects,camera);

	SortObjectsPosition(ObjectsOnScreen);

	std::vector<Vec2<int>> ObjectIndexes;

	for (auto* object : ObjectsOnScreen)
	{
		Vec2<int> QuadIndex = (object->Data.pos / DesiredQuadSize).Cast<int>();
		LOG("OBJECT LOCATION: " << object->Data.pos);
		ObjectIndexes.push_back(QuadIndex);
	}

	std::sort(ObjectIndexes.begin(), ObjectIndexes.end());

	for (size_t i = 0; i < ObjectIndexes.size(); i++)
	{
		LOG("Object " << i << " " << ObjectIndexes.at(i));
		DrawRectangleLines(ObjectIndexes.at(i).x * DesiredQuadSize.x, ObjectIndexes.at(i).y * DesiredQuadSize.y, DesiredQuadSize.x, DesiredQuadSize.y , RED);
	}

	/*for (size_t y = 0; y < ObjectIndexes.size(); y++)
	{
		for (size_t i = y; i < ObjectIndexes.size() - y; i++)
		{
			if (ObjectIndexes.at(y) == ObjectIndexes.at(i))
			{
				InsertNode(HeadQuad);
				LastNode->objects.push_back(ObjectsOnScreen.at(y));
				LastNode->objects.push_back(ObjectsOnScreen.at(i));
			}
		}
	}*/

	/*LOG("NODE COUNT: " << NodeCount);
	Quad* temp = HeadQuad->next;

	while (temp != nullptr)
	{
		for (auto* object : temp->objects)
		{
			LOG("Object: " << object->Data.pos);
		}
		temp = temp->next;
	}*/

}

bool QT::CheckCollision(GameObject& obj1, GameObject& obj2)
{
	return false;
}

std::vector<GameObject*> QT::FetchOnScreen(std::vector<GameObject*>& Objects, Camera2D& camera)
{
	std::vector<GameObject*> OnScreen;
	Vec4<float> ScreenRect(camera.target.x - camera.offset.x , camera.target.y - camera.offset.y, getWsize().x, getWsize().y);
	//ScreenRect.zw(ScreenRect.zw() * camera.zoom);
	//ScreenRect.xy(ScreenRect.xy() / camera.zoom);

	for (auto* object : Objects)
	{
		if (object->Data.pos < (ScreenRect.xy() + ScreenRect.zw()) && object->Data.pos >= ScreenRect.xy())
		{
			OnScreen.push_back(object);
		}
	}
	//+ ((getWsize().x / 2) - camera.target.x)
	LOG("ScreenRect: " << ScreenRect);
	//DrawRectangle(ScreenRect.x, ScreenRect.y, ScreenRect.z, ScreenRect.w, RED);
	//DrawRectangleLines(ScreenRect.x, ScreenRect.y, ScreenRect.z, ScreenRect.w, RED);
	return OnScreen;
}


void MergeSortedIntervals(std::vector<GameObject*>& v, int s, int m, int e) 
{
	
	std::vector<GameObject*> temp;

	int i, j;
	i = s;
	j = m + 1;

	while (i <= m && j <= e) 
	{

		if (v[i]->Data.pos <= v[j]->Data.pos) {
			temp.push_back(v[i]);
			++i;
		}
		else {
			temp.push_back(v[j]);
			++j;
		}

	}

	while (i <= m) 
	{
		temp.push_back(v[i]);
		++i;
	}

	while (j <= e) 
	{
		temp.push_back(v[j]);
		++j;
	}

	for (int i = s; i <= e; ++i)
	{
		v[i] = temp[i - s];
	}

}

void MergeSort(std::vector<GameObject*>& v, int s, int e)
{
	if (s < e) 
	{
		int m = (s + e) / 2;
		MergeSort(v, s, m);
		MergeSort(v, m + 1, e);
		MergeSortedIntervals(v, s, m, e);
	}
}

void QT::SortObjectsPosition(std::vector<GameObject*>& Objects)
{
	MergeSort(Objects, 0, Objects.size() - 1);
}

void QT::FreeQuads(Quad* HeadQuad)
{
	if (HeadQuad != nullptr)
	{
		Quad* temp = HeadQuad->next;
		delete HeadQuad;
		FreeQuads(temp);
	}
	else
	{
		HeadQuad = nullptr;
		LastNode = nullptr;
		NodeCount = 0;
		LOG_INF("Quads are deallocated!");
	}
}

void QT::InsertNode(Quad* HeadQuad)
{
	LastNode->next = new Quad;
	LastNode->next->prev = LastNode;
	LastNode = LastNode->next;
	NodeCount++;
}

void QT::DeleteLastNode(Quad* HeadQuad)
{
	if (LastNode->prev != nullptr)
	{
		Quad* temp = LastNode->prev;
		delete LastNode;
		LastNode = temp;
	}
	else
	{
		delete LastNode;
	}

	NodeCount--;
}

void QT::InitList(Quad* HeadQuad)
{
	LastNode = HeadQuad;
	NodeCount++;
	LOG_INF("Quad list has been initialized!");
}

QT::Quad* QT::GetNodeWithIndex(Quad* headnode,int index)
{
	if (index == 0)
	{
		return headnode;
	}

	int IndexInterval = 0;
	QT::Quad* temp = headnode;
	while (IndexInterval < index)
	{
		temp = temp->next;
		IndexInterval++;
	}
	return temp;
}


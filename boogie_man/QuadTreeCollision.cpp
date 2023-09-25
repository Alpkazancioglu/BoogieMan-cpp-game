#include "QuadTreeCollision.h"
#include "util/Log.h"
#include <string>

static unsigned int NodeCount = 0;
static QT::Quad* LastNode = nullptr;
static int Index = 0;

void QT::ContructQuads(Quad*& HeadQuad, std::vector<GameObject*>& Objects,Vec2<float> DesiredQuadSize , Camera2D &camera)
{
	FreeQuads(HeadQuad);
	InitList(HeadQuad);
	
	std::vector<GameObject*> ObjectsOnScreen = FetchOnScreen(Objects,camera);

	SortObjectsPosition(ObjectsOnScreen);

	for (size_t i = 0; i < ObjectsOnScreen.size(); i++)
	{
		std::vector<GameObject*> InSameQuad;
		Vec4<float> ObjectRec({ ObjectsOnScreen.at(i)->Data.pos.x , ObjectsOnScreen.at(i)->Data.pos.y ,
							 ObjectsOnScreen.at(i)->Data.rec.width , ObjectsOnScreen.at(i)->Data.rec.height });
		Vec2<float> objectRecCenter(bgGL::FindCenterAABB(ObjectRec));

		Vec2<int> QuadIndex = (objectRecCenter / DesiredQuadSize).Cast<int>();
 		for (size_t t = i ; t < ObjectsOnScreen.size() - i - 1 ; t++)
		{
			Vec2<int> QuadIndexNext = (objectRecCenter / DesiredQuadSize).Cast<int>();

			if (QuadIndexNext == QuadIndex)
			{
				InSameQuad.push_back(ObjectsOnScreen.at(t));
			}
		}

		InSameQuad.push_back(ObjectsOnScreen.at(i));

		if (i != 0 && InSameQuad.size() == 1)
		{
			InsertNode(HeadQuad);
		}

		LastNode->Attrib.xy(QuadIndex.Cast<float>());
		LastNode->Attrib.zw(DesiredQuadSize);
		LastNode->objects.assign(InSameQuad.begin(), InSameQuad.end());

		if (InSameQuad.size() > 2)
		{
			SubdivideQuad(LastNode , InSameQuad);
		}

		LOG("Object " << i << " " << QuadIndex);
	}

	Quad* CurrentQuad = HeadQuad;
	while (CurrentQuad != nullptr)
	{
		//LOG("QUAD " << CurrentQuad->Index << ": " << CurrentQuad->Attrib);

		//DrawRectangleLines(CurrentQuad->Attrib.x * CurrentQuad->Attrib.z, CurrentQuad->Attrib.y * CurrentQuad->Attrib.w, CurrentQuad->Attrib.z, CurrentQuad->Attrib.w, RED);
		CurrentQuad = CurrentQuad->next;
	}

	LOG("QUAD COUNT: " << NodeCount);
}

void QT::SubdivideQuad(Quad*& QuadToSubdivide , std::vector<GameObject*> &InSameQuad)
{
	Vec2<float> SubdividedQuadSize(QuadToSubdivide->Attrib.zw() / 2);

	for (size_t y = 0; y < 2; y++)
	{
		for (size_t x = 0; x < 2; x++)
		{
			InsertNode(QuadToSubdivide);
			Vec2<int> SubdividedPosition( (QuadToSubdivide->Attrib.x + (x * SubdividedQuadSize.x)) / SubdividedQuadSize.x,(QuadToSubdivide->Attrib.y + (y * SubdividedQuadSize.y)) / SubdividedQuadSize.y );
			LastNode->Attrib.xy(SubdividedPosition.Cast<float>());
			LastNode->Attrib.zw(SubdividedQuadSize);
			//LastNode->objects.push_back(InSameQuad.at(y * 2 + x));
		}
	}
	/*QuadToSubdivide->Attrib.SetValues( QuadToSubdivide->Attrib.x + (2 * SubdividedQuadSize.x),
									   QuadToSubdivide->Attrib.y + (2 * SubdividedQuadSize.y),
									   SubdividedQuadSize.x , SubdividedQuadSize.y );*/

}

bool QT::CheckCollision(GameObject& obj1, GameObject& obj2)
{




	return false;
}

std::vector<GameObject*> QT::FetchOnScreen(std::vector<GameObject*>& Objects, Camera2D& camera)
{
	std::vector<GameObject*> OnScreen;
	Vec4<float> ScreenRect(camera.target.x - camera.offset.x , camera.target.y - camera.offset.y, getWsize().x, getWsize().y);
	
	for (auto* object : Objects)
	{
		Vec2<float> objectRecCenter(bgGL::FindCenterAABB({ object->Data.pos.x , object->Data.pos.y , object->Data.rec.width , object->Data.rec.height }));
		if (objectRecCenter < (ScreenRect.xy() + ScreenRect.zw()) && objectRecCenter >= ScreenRect.xy())
		{
			OnScreen.push_back(object);
		}
	}
	LOG("ScreenRect: " << ScreenRect);
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

void QT::FreeQuads(QT::Quad*& HeadQuad)
{
	if (HeadQuad != nullptr)
	{
		QT::Quad* temp = HeadQuad->next;
		delete HeadQuad;
		HeadQuad = nullptr;
		FreeQuads(temp);
	}
	else
	{
		LastNode = nullptr;
		NodeCount = 0;
		Index = 0;
		LOG_INF("Quads are deallocated!");
	}
}

void QT::InsertNode(Quad*& HeadQuad)
{
	Quad* newQuad = new Quad;
	newQuad->Index = Index;
	LastNode->next = newQuad;
	newQuad->prev = LastNode;
	LastNode = newQuad;
	NodeCount++;
	Index++;
}

void QT::DeleteLastNode(Quad*& HeadQuad)
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
	Index--;
}

void QT::InitList(QT::Quad*& HeadQuad)
{
	HeadQuad = new QT::Quad;
	HeadQuad->Index = Index;
	LastNode = HeadQuad;
	NodeCount++;
	Index++;
	LOG_INF("Quad list has been initialized!");
}

void QT::DeleteNode(Quad*& nodeToDelete)
{
	if (nodeToDelete != nullptr)
	{
		Quad* next = nodeToDelete->next;
		Quad* prev = nodeToDelete->prev;

		if (next != nullptr)
		{
			next->prev = prev;
		}
		if (prev != nullptr)
		{
			prev->next = next;
		}
		delete nodeToDelete;
	}
}

QT::Quad* QT::GetNodeWithIndex(Quad*& headnode,int index)
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


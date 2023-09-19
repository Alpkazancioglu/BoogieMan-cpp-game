#include "QuadTreeCollision.h"

static unsigned int NodeCount = 0;
static QT::Quad* LastNode = nullptr;
static Vec2<float> PrevDesiredQuadSize;

void QT::ContructQuads(Quad* HeadQuad, std::vector<GameObject*>& Objects,Vec2<float> DesiredQuadSize)
{
	Vec2<unsigned int> GridCount = (getWsize() / DesiredQuadSize).Cast<unsigned int>();

	Quad* temp = HeadQuad;
	std::vector<GameObject*> ObjectsOnScreen = FetchOnScreen(Objects);

	SortObjectsPosition(ObjectsOnScreen);

	for (auto* object : Objects)
	{
		if (object->Data.pos < getWsize() && object->Data.pos > 0)
		{
			object->Data.pos / getWsize()
		}
	}

	while (temp != nullptr)
	{
	    

		temp = temp->next;
	}
}

bool QT::CheckCollision(GameObject& obj1, GameObject& obj2)
{
	return false;
}

std::vector<GameObject*> QT::FetchOnScreen(std::vector<GameObject*>& Objects)
{
	std::vector<GameObject*> OnScreen;
	for (auto* object : Objects)
	{
		if (object->Data.pos < getWsize() && object->Data.pos > 0)
		{
			OnScreen.push_back(object);
		}
	}
	return OnScreen;
}

// the interval from [s to m] and [m+1 to e] in v are sorted
// the function will merge both of these intervals
// such the interval from [s to e] in v becomes sorted
void MergeSortedIntervals(std::vector<GameObject*>& v, int s, int m, int e) 
{
	// temp is used to temporary store the vector obtained by merging
	// elements from [s to m] and [m+1 to e] in v
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
		LastNode = nullptr;
		NodeCount = 0;
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
}


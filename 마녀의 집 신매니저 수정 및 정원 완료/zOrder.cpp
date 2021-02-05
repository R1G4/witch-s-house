#include "stdafx.h"
#include "zOrder.h"

void zOrder::insert(float x,float y, ZTYPE type)
{
	zTile temp;
	temp.x = x;
	temp.y = y;
	temp.type = type;
	temp.img = NULL;
	temp.keyName = "";
	_vZorder.push_back(temp);
	insertSort();
}

void zOrder::quickSort(int a, int b)
{
	if (a >= b)return;
	int pivot = _vZorder[(a + b) / 2].y;
	int left = a;
	int right = b;
	while (left <= right)
	{
		while (_vZorder[left].y < pivot)++left;
		while (_vZorder[right].y > pivot)--right;
		if (left <= right)
		{
			swap(_vZorder[left], _vZorder[right]);
			++left;
			--right;
		}
	}
	quickSort(a, right);
	quickSort(left, b);
}

void zOrder::insert(float x, float y, Image * img, ZTYPE type)
{
	zTile temp;
	temp.x = x;
	temp.y = y;
	temp.type = type;
	temp.img = img;
	temp.keyName = "";
	_vZorder.push_back(temp);

	insertSort();
}

void zOrder::insert(float x, float y, string keyName, ZTYPE type)
{
	zTile temp;
	temp.x = x;
	temp.y = y;
	temp.type = type;
	temp.img = IMAGEMANAGER->FindImage(keyName);
	temp.keyName = keyName;
	_vZorder.push_back(temp);

	insertSort();
}

void zOrder::insertSort()//계속해서 새로운 값들을 넣어 정렬해야하니까 삽입 정렬이 제일 빠르다고 판단하여 삽입정렬 활용함
{
	for (int i = 0; i < _vZorder.size() - 1; ++i)
	{
		for (int j = i + 1; j > 0 && _vZorder[j].y < _vZorder[j - 1].y; --j)
		{
			std::swap(_vZorder[j], _vZorder[j - 1]);
		}
	}	// 삽입 정렬을 했음

}

void zOrder::release()
{
	_vZorder.clear();
}

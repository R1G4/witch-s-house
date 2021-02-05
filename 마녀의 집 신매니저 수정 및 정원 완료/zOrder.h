#pragma once
#include "singletonBase.h"
#include <vector>
using namespace std;
enum ZTYPE
{
	ZPLAYER,
	ZENEMY,
	ZOBJECT,
	ZFRAMEOBJ
};
struct zTile
{
	float x,y;
	ZTYPE type;
	string keyName;
	Image* img;
};
class zOrder :
	public singletonBase<zOrder>
{
private:
	typedef vector<zTile> vZorder;
	typedef vector<zTile>::iterator viZorder;
	vZorder _vZorder;
	viZorder _viZorder;
public:
	void insert(float x,float y,ZTYPE type);
	void insert(float x, float y, Image* img, ZTYPE type);
	void insert(float x, float y, string keyName, ZTYPE type);
	void quickSort(int a,int b);
	void insertSort();
	void release();
	vector<zTile> getZorder() { return _vZorder; }
	vector<zTile>::iterator getIZorder() { return _viZorder; }
};


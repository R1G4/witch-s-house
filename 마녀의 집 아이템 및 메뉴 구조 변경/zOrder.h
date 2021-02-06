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
	void insert(float x,float y,ZTYPE type);//x,y좌표와 타입만 넣어주면 될 때(플레이어, 에너미 등)
	void insert(float x, float y, Image* img, ZTYPE type);//이미지도 넣을 수 있는 건데 아마 안쓸 거 같음
	void insert(float x, float y, string keyName, ZTYPE type);//키값을 넣을 수 있는 함수. 키값 넣으면 바로 FindImage로 해당 이미지를 찾아서 넣어주게끔 하였음
	void quickSort(int a,int b);
	void insertSort();//삽입정렬
	void release();
	vector<zTile> getZorder() { return _vZorder; }
	vector<zTile>::iterator getIZorder() { return _viZorder; }
};


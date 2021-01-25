#pragma once
#include "gameNode.h"
#include "tile.h"
#include <vector>
#include <string>

#define TILEWIDTH 32
#define TILEHEIGHT 32

#define TILENUMX (WINSIZEX / TILEWIDTH)
#define TILENUMY (WINSIZEY / TILEHEIGHT)

class aStarTest : public gameNode
{
private:
	vector<tile*>			_vTotalList;
	vector<tile*>::iterator _viTotalList;

	vector<tile*>			_vOpenList;
	vector<tile*>::iterator _viOpenList;

	vector<tile*>			_vCloseList;
	vector<tile*>::iterator _viCloseList;

	tile* _startTile;		//시작타일
	tile* _endTile;			//도착타일
	tile* _currentTile;		//현재타일

	int _count;
	bool _start;
	bool _stop; // 못찾은거
	int _numCount; // 몇번째 타일인지
	int _follow_count;	// 추적시간
	int _px;
	int _py;
	int _ex;
	int _ey;
public:
	aStarTest();
	~aStarTest();

	HRESULT init();
	
	void setTiles(int px, int py, int ex, int ey);

	//갈수 있는 길을 찾아내서 담아줄 함수
	vector<tile*> addOpenList(tile* currentTile);

	void resetEverything();


	//빠른 경로 찾을 함수
	void pathFinder(tile* currentTile);

	void release();
	void update();
	void render();
};


#pragma once
#include"gameNode.h"
#include "tankMap.h"
#include "tank.h"
#include "tile.h"
#include <vector>
#include <string>

enum ENEMYTANKDIRECTION
{
	ENEMYTANKDIRECTION_LEFT,
	ENEMYTANKDIRECTION_UP,
	ENEMYTANKDIRECTION_RIGHT,
	ENEMYTANKDIRECTION_DOWN
};


class enemyTank : public gameNode
{
private:
	// astar위한 변수들
	vector<tile*>			_vTotalList;
	vector<tile*>::iterator _viTotalList;

	vector<tile*>			_vOpenList;
	vector<tile*>::iterator _viOpenList;

	vector<tile*>			_vCloseList;
	vector<tile*>::iterator _viCloseList;

	tile* _playerTile;		// 플레이어타일
	tile* _emTile;			// 에너미타일
	tile* _objTile[400];		// 오브젝트타일
	tile* _currentTile;		// 현재에너미타일

	bool _setTile;
private:
	tankMap* _tankMap;
	image* _image;
	float _x, _y;
	float _speed;
	RECT _rc;
	ENEMYTANKDIRECTION _direction;

	tank* _player;

	int _count;
	bool _start;
	bool _stop;			// 못찾은거
	int _numCount;		// 몇번째 타일인지
	int _follow_count;	// 추적시간
	int _px;
	int _py;
	int _ex;
	int _ey;

public:
	// astar위한 함수들
	void setTiles(int px, int py, int ex, int ey);

	//갈수 있는 길을 찾아내서 담아줄 함수
	vector<tile*> addOpenList(tile* currentTile);

	void resetEverything();

	//빠른 경로 찾을 함수
	void pathFinder(tile* currentTile);

public:
	enemyTank() {};
	~enemyTank() {};

	HRESULT init();
	void release();
	void update();
	void render();

	void setTankPosition();
	void tankDirection();

	void setTankMapMemoryAddressLink(tankMap* tm) { _tankMap = tm; }
	void setTankMemoryAddressLink(tank* t) { _player = t; }
};


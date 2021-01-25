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
	// astar���� ������
	vector<tile*>			_vTotalList;
	vector<tile*>::iterator _viTotalList;

	vector<tile*>			_vOpenList;
	vector<tile*>::iterator _viOpenList;

	vector<tile*>			_vCloseList;
	vector<tile*>::iterator _viCloseList;

	tile* _playerTile;		// �÷��̾�Ÿ��
	tile* _emTile;			// ���ʹ�Ÿ��
	tile* _objTile[400];		// ������ƮŸ��
	tile* _currentTile;		// ���翡�ʹ�Ÿ��

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
	bool _stop;			// ��ã����
	int _numCount;		// ���° Ÿ������
	int _follow_count;	// �����ð�
	int _px;
	int _py;
	int _ex;
	int _ey;

public:
	// astar���� �Լ���
	void setTiles(int px, int py, int ex, int ey);

	//���� �ִ� ���� ã�Ƴ��� ����� �Լ�
	vector<tile*> addOpenList(tile* currentTile);

	void resetEverything();

	//���� ��� ã�� �Լ�
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


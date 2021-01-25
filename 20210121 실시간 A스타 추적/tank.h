#pragma once
#include "gameNode.h"
#include "tankMap.h"

enum TANKDIRECTION
{
	TANKDIRECTION_LEFT,
	TANKDIRECTION_UP,
	TANKDIRECTION_RIGHT,
	TANKDIRECTION_DOWN
};

class tank : public gameNode
{
private:
	tankMap* _tankMap;
	image* _image;
	float _x, _y;
	float _speed;
	int _tileX, _tileY;	//실제 탱크가 어느 타일에 위치하는지 계산용(left, top 기준일때)
	RECT _rc;
	TANKDIRECTION _direction;

public:
	tank();
	~tank();

	HRESULT init();
	void release();
	void update();
	void render();

	void tankMove();
	void setTankPosition();

	int getCurrentTileX() { return _tileX; }
	int getCurrentTileY() { return _tileY; }

	void setTankMapMemoryAddressLink(tankMap* tm) { _tankMap = tm; }
};


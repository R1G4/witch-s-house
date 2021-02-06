#pragma once
#include "gameNode.h"
#include "Player.h"
#include "DeadManager.h"
class thirdOnewayDead : public gameNode
{
private:

	Image* _backGround;
	tagTile _tiles[TILEX*TILEY];
	Vector2 camera;
	Player* _player;
	DeadManager* _deadManager;
	int _count;

public:
	thirdOnewayDead();
	~thirdOnewayDead();

	HRESULT init();
	void release();
	void update();
	void render();
	void tileCollision();
	void load();
};


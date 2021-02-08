#pragma once
#include "gameNode.h"
#include "Player.h"
#include "DeadManager.h"
class thirdSnakeDead : public gameNode
{
private:

	Image* _backGround;
	tagTile _tiles[TILEX*TILEY];
	Vector2 camera;
	Player* _player;
	DeadManager* _deadManager;
	int _count;

public:
	thirdSnakeDead();
	~thirdSnakeDead();

	HRESULT init();
	void release();
	void update();
	void render();
	void trigger();
	void tileCollision();
	void Collision();
	void load();
};

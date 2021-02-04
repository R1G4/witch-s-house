#pragma once
#include "gameNode.h"
#include "Player.h"
class thirdFloorDead : public gameNode
{
private:

	Image* _backGround;
	tagTile _tiles[TILEX*TILEY];
	Vector2 camera;
	Player* _player;

public:
	thirdFloorDead();
	~thirdFloorDead();

	HRESULT init();
	void release();
	void update();
	void render();
	void trigger();
	void tileCollision();
	void Collision();
	void load();
};


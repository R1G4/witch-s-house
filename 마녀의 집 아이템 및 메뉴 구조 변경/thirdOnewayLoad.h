#pragma once
#include "gameNode.h"
#include "Player.h"

class thirdOnewayLoad : public gameNode
{
private:

	Image* _backGround;
	tagTile _tiles[TILEX*TILEY];
	Vector2 camera;
	Player* _player;

	int _count;
	int _frame;

public:
	thirdOnewayLoad();
	~thirdOnewayLoad();

	HRESULT init();
	void release();
	void update();
	void render();
	void tileCollision();
	void load();
};


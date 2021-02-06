#pragma once
#include "gameNode.h"
#include "Player.h"
class thirdMain : public gameNode
{
private:

	Image* _backGround;
	tagTile _tiles[TILEX*TILEY];
	Vector2 camera;
	Player* _player;

	int _count;
	int _frame;


public:
	thirdMain();
	~thirdMain();

	HRESULT init();
	void release();
	void update();
	void render();
	void changeScene();
	void tileCollision();
	void load();
};


#pragma once
#include "gameNode.h"
#include "Player.h"
class stairs_2F : public gameNode
{
private:

	Image* _backGround;
	tagTile _tiles[TILEX*TILEY];
	Vector2 camera;
	Player* _player;
	int _count;

public:
	stairs_2F();
	~stairs_2F();

	HRESULT init();
	void release();
	void update();
	void render();
	void changeScene();
	void tileCollision();
	void load();
};


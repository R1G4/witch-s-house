#pragma once
#include "gameNode.h"
#include "Player.h"
class thirdSnakeRoom : public gameNode
{
private:
	Image* _backGround;
	tagTile _tiles[TILEX*TILEY];
	Vector2 camera;
	Player* _player;
public:
	thirdSnakeRoom();
	~thirdSnakeRoom();

	HRESULT init();
	void release();
	void update();
	void render();
	void tileCollision();
	void load();
};


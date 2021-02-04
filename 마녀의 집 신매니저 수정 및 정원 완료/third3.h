#pragma once
#include "gameNode.h"
#include "Player.h"
class third3 : public gameNode
{
private:
	Image* _backGround;
	tagTile _tiles[TILEX*TILEY];
	Vector2 camera;
	Player* _player;
public:
	third3();
	~third3();

	HRESULT init();
	void release();
	void update();
	void render();
	void trigger();
	void tileCollision();
	void load();
};


#pragma once
#include "gameNode.h"
#include "Player.h"
class third2 : public gameNode
{
private:
	Image* _backGround;
	tagTile _tiles[TILEX*TILEY];
	Vector2 camera;
	Player* _player;
public:
	third2();
	~third2();

	HRESULT init();
	void release();
	void update();
	void render();
	void trigger();
	void tileCollision();
	void load();
};


#pragma once
#include "gameNode.h"
#include "Player.h"


class castlefront : public gameNode
{
private:
	Image* _backGround;
	tagTile _tiles[TILEX*TILEY];
	Vector2 camera;
	Player* _player;



public:
	castlefront();
	~castlefront();

	virtual HRESULT init();
	virtual void update();
	virtual void release();
	virtual void render();

	void load();
	void tileCollision();
};
#pragma once
#include "gameNode.h"
#include "Player.h"
#include"boss.h"
class bossStage :
	public gameNode
{
private:
	Image* _backGround;
	tagTile _tiles[TILEX*TILEY];
	Vector2 camera;
	Player* _player;
	boss* _boss;
	int bossLocX, bossLocY;
	int bossFrameX, bossFrameY;
public:
	HRESULT init();
	void release();
	void update();
	void render();
	void load();
	void tileCollision();
};


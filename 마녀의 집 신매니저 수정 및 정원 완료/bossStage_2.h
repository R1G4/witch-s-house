#pragma once
#include "gameNode.h"
#include"boss.h"

class Player;
class bossStage_2 :
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
	vector<tagTile> _vObjTile;
public:
	HRESULT init();
	void release();
	void update();
	void render();
	void load();
	void tileCollision();
	void addresslink(Player* pla) { _player = pla; }
};


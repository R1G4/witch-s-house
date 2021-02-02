#pragma once
#include "gameNode.h"
#include "Player.h"
#include "mapTool.h"


class garDenUnder :
	public gameNode
{
private:
	Image* _backGround;
	tagTile _tiles[TILEX*TILEY];
	Vector2 camera;
	Player* _player;


public:
	garDenUnder();
	~garDenUnder();
	HRESULT init();
	void release();
	void update();
	void render();
	void load();
	void tileCollision();
	//	void adresslink(Player* pla) { _player = pla; }
};


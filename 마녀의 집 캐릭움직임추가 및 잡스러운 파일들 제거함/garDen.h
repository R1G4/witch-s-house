#pragma once
#include "gameNode.h"
#include "Player.h"
#include "mapTool.h"


class garDen :
	public gameNode
{
private:
	Image* _backGround;
	tagTile _tiles[TILEX*TILEY];
	Vector2 camera;
	Player* _player;
	
	
	int count;
	int frame;
public:
	garDen();
	~garDen();
	HRESULT init();
	void release();
	void update();
	void render();
	void load();
	void tileCollision();
	//	void adresslink(Player* pla) { _player = pla; }
};


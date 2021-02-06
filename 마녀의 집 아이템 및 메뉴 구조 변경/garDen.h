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
	
	getRect mapChange[2];
	getRect uiTriger;

	int count;
	int frame;
public:
	garDen();
	~garDen();
	virtual HRESULT init();
	virtual HRESULT init(CHRDIRECTION _CHRDIRECTION);
	virtual void release();
	virtual void update();
	virtual void render();
	void load();
	void tileCollision();
	//	void adresslink(Player* pla) { _player = pla; }
};


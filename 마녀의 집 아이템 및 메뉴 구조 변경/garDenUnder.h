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
	

	getRect mapChange;
	getRect uiTriger;

public:
	garDenUnder();
	~garDenUnder();
	virtual HRESULT init(CHRDIRECTION _chrdirection);
	virtual void release();
	virtual void update();
	virtual void render();
	void load();
	void tileCollision();
	//	void adresslink(Player* pla) { _player = pla; }
};


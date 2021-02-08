#pragma once
#include "gameNode.h"
#include "Player.h"



class soundStageE1 : public gameNode
{
private:
	Image* _backGround;
	tagTile _tiles[TILEX*TILEY];
	Vector2 camera;
	Player* _player;

	getRect mapChange[2];
	getRect uiTrigger;


public:
	soundStageE1();
	~soundStageE1();
	virtual HRESULT init(CHRDIRECTION _CHRDIRECTION);
	virtual void release();
	virtual void update();
	virtual void render();

	void load();
	void tileCollision();
};


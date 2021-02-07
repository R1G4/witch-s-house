#pragma once
#include "gameNode.h"
#include "Player.h"





class soundStageE2 : public gameNode
{
private:
	Image* _backGround;
	tagTile _tiles[TILEX*TILEY];
	Vector2 camera;
	Player* _player;

	getRect mapChange[2];
	getRect uiTrigger;


public:
	soundStageE2();
	~soundStageE2();
	virtual HRESULT init();
	virtual HRESULT init(CHRDIRECTION _CHRDIRECTION);
	virtual void release();
	virtual void update();
	virtual void render();

	void load();
	void tileCollision();
};


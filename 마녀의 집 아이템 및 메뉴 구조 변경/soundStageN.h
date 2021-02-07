#pragma once
#include "gameNode.h"
#include "Player.h"



class soundStageN : public gameNode
{
private:
	Image* _backGround;
	tagTile _tiles[TILEX*TILEY];
	Vector2 camera;
	Player* _player;

	getRect mapChange[2];
	getRect uiTrigger;

	float Px, Py;	//ÇÇ¾Æ³ë ÁÂÇ¥

public:
	soundStageN();
	~soundStageN();
	virtual HRESULT init();
	virtual HRESULT init(CHRDIRECTION _CHRDIRECTION);
	virtual void release();
	virtual void update();
	virtual void render();

	void load();
	void tileCollision();
};


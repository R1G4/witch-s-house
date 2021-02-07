#pragma once
#include "gameNode.h"
#include "Player.h"



class soundStage : public gameNode
{
private:
	Image* _backGround;
	tagTile _tiles[TILEX*TILEY];
	Vector2 camera;
	Player* _player;

	getRect mapChange[3];
	getRect uiTrigger[2];

	int count;
	int frame;

public:
	soundStage();
	~soundStage();
	HRESULT init();
	HRESULT init(CHRDIRECTION _CHRDIRECTION);
	void release();
	void update();
	void render();

	void load();
	void tileCollision();
};


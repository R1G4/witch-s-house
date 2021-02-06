#pragma once
#include "gameNode.h"
#include "Player.h"
#include "mapTool.h"
#include "soundStage.h"


class soundStageW : public gameNode
{
private:
	Image* _backGround;
	tagTile _tiles[TILEX*TILEY];
	Vector2 camera;
	Player* _player;

	getRect mapChange;
	getRect uiTrigger;


public:
	soundStageW();
	~soundStageW();
	virtual HRESULT init();
	//virtual HRESULT init(CHRDIRECTION _CHRDIRECTION);
	virtual void release();
	virtual void update();
	virtual void render();

	void load();
	void tileCollision();
};


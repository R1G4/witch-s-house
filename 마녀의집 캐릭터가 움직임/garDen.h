#pragma once
#include "gameNode.h"
#include "mapTool.h"
#include "boss.h"
#include"Player.h"
class garDen : public gameNode
{
private:
	tagTile _tiles[TILEX*TILEY];

	boss* _boss;
	Image* _backGround;
	DWORD _attribute[TILEX*TILEY];
	Vector2 camera;
	Player* _player;
	int _pos[2];
public:
	garDen();
	~garDen();

	virtual HRESULT init();
	virtual void update();
	virtual void release();
	virtual void render();
	//virtual void cameraMove();//문제 발생 가능성 높음 문제 발생하면 해결하러가야함
	void load();
};


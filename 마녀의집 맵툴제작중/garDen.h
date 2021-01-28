#pragma once
#include "gameNode.h"

class garDen : public gameNode
{
private:
	//tagTile _tiles[TILEX*TILEY];

	//DWORD _attribute[TILEX*TILEY];

	tagTile _tiles[TILEX*TILEY];
	POINT camera;
	int _pos[2];
	int frame;
	int count;

public:
	garDen();
	~garDen();


	virtual HRESULT init();
	virtual void update();
	virtual void release();
	virtual void render();

	void load();
};


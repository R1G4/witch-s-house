#pragma once
#include "gameNode.h"
#include "mapTool.h"

class garDenUnder : public gameNode
{
private:
	tagTile _tiles[TILEX*TILEY];
	POINT camera;

public:
	garDenUnder();
	~garDenUnder();

	virtual HRESULT init();
	virtual void update();
	virtual void release();
	virtual void render();

	void load();
};


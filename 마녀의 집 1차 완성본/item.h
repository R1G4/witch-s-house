#pragma once
#include "gameNode.h"
class item : public gameNode
{
private:
	Image* _item;
	Image* _itemTop;
	float _x, _y;
	float _rcAlpha;
	float _rcAlphaChange;

public:
	item();
	~item();

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	void rcAlphaChange();
	virtual void render();
};


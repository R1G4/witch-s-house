#pragma once
#include "gameNode.h"
class player : public gameNode
{
private:
	Image* viola;
	int _currentFrameX, _currentFrameY;
	float _alpha;
	POINT pt;
	int _count;

public:
	player();
	~player();

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();
};


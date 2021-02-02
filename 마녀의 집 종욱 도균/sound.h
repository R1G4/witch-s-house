#pragma once
#include "gameNode.h"

class sound : public gameNode
{
private:
	FloatRect _rc;
	float _volume;
	float _bgv;

public:

	sound();
	~sound();

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

	void volumeUp();

	float getVolume() { return _volume; }
};


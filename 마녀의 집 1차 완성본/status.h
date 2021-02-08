#pragma once
#include "gameNode.h"

enum MENU
{
	ITEM, LOAD
};

class status : public gameNode
{
private:
	int _openMenu;
	Image* _status;
	Image* _menu;
	Image* _load;
	FloatRect _rc;
	FloatRect _rc1;
	FloatRect _rc2;
	FloatRect _rc3;
	float _x, _y;
	float _rcAlpha;
	float _rcAlphaChange;
	bool _isMenu;
	bool _isChoice;
	bool _isLoad;

public:
	status();
	~status();

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	void rcAlphaChange();
	virtual void render();

	bool getIsMenu() { return _isMenu; }
};


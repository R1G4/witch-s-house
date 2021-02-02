#pragma once
#include "gameNode.h"
#include "settings.h"
#include "load.h"
#include "item.h"

enum MENU
{
	MENU, ITEM, LOAD, SETTINGS
};

class playerMenu : public gameNode
{
private:
	settings* _settings;
	load* _load;
	item* _item;

	int _openMenu;
	Image* _status;
	Image* _menu;
	FloatRect _rc;
	FloatRect _rcMenu[3];
	
	float _x, _y;
	float _rcAlpha;
	float _rcAlphaChange;
	bool _isMenu;
	bool _isCheck;
	bool _isLoad;
	

public:
	playerMenu();
	~playerMenu();

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	void rcAlphaChange();
	virtual void render();

	void openMenu();

	bool getIsMenu() { return _isMenu; }
};


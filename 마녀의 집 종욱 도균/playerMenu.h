#pragma once
#include "gameNode.h"

enum MENU
{
	MENU, ITEM, LOAD, SETTINGS
};

class playerMenu : public gameNode
{
private:
	int _openMenu;
	Image* _status;
	Image* _menu;
	Image* _load;
	Image* _loadTop;
	Image* _item;
	Image* _itemTop;
	FloatRect _rc;
	FloatRect _rcMenu[3];
	FloatRect _rc1;
	FloatRect _rc2;
	FloatRect _rc3;
	FloatRect _rcLoad[5];
	FloatRect _rcLoadChoice;
	FloatRect _rcSettingChoice;
	FloatRect _rcSettings[6];
	char _fileName[256];
	TCHAR str[128];
	float _x, _y;
	float _x2, _y2;
	float _x3, _y3;
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


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
	FloatRect _rcLoad[5];
	FloatRect _rcLoadChoice;
	FloatRect _rcSettingChoice;
	FloatRect _rcSettings[7];
	char _fileName[256];
	float _x, _y;
	float _x2, _y2;
	float _x3, _y3;
	float _rcAlpha;
	float _rcAlphaChange;
	bool _isMenu;
	bool _isCheck;
	bool _isLoad;
	float _volume;
	float _bgv;
	char str[128];
	int _percent;	//string 값 바꿔줄 변수
	int _bgvPercent;

	string mv;
	wstring mv_w = L"";
	
	string bgv;
	wstring bgv_w = L"";
	

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


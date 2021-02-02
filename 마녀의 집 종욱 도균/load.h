#pragma once
#include "gameNode.h"
class load : public gameNode
{
private:
	Image* _load;
	Image* _loadTop;
	FloatRect _rcLoad[5];
	FloatRect _rcLoadChoice;
	FloatRect _temp;
	float _x, _y;
	float _rcAlpha;
	float _rcAlphaChange;
public:
	load();
	~load();

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	void rcAlphaChange();
	void loadSaveFile();
	virtual void render();

};


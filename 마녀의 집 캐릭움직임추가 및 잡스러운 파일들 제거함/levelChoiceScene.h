#pragma once
#include "gameNode.h"
class levelChoiceScene : public gameNode
{
public:
	FloatRect _rc;
	FloatRect _rc1;
	FloatRect _rc2;
	FloatRect _rc3;
	Image* _choiceRect;
	float _rcAlpha;				//렉트 투명도
	float _rcAlphaChange;		//렉트 조절용
	float _x, _y;
	float _sceneAlpha;
	bool _isChoice;

public:
	levelChoiceScene();
	~levelChoiceScene();

	virtual HRESULT init();
	virtual void release();
	virtual void rcAlphaChange();
	virtual void update();
	bool sceneAlphaChange();
	virtual void render();
};


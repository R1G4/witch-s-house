#pragma once
#include "gameNode.h"
class levelChoiceScene : public gameNode
{
public:
	FloatRect _rc;
	FloatRect _rc2;
	FloatRect _rc3;
	Image* _choiceRect;
	float _rcAlpha;				//��Ʈ ����
	float _rcAlphaChange;		//��Ʈ ������
	float _x, _y;
	bool _isChoice;

public:
	levelChoiceScene();
	~levelChoiceScene();

	virtual HRESULT init();
	virtual void release();
	virtual void rcAlphaChange();
	virtual void update();
	virtual void render();
};


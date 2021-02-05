#pragma once
#include "gameNode.h"
#include "sound.h"
class settings : public gameNode
{
private:
	sound* _sound;
	FloatRect _rcSettingChoice;
	FloatRect _rcSettings[7];
	FloatRect _temp;
	float _x, _y;
	float _rcAlpha;
	float _rcAlphaChange;
	float _volume;
	int _percent;	//string �� �ٲ��� ����
	int _bgvPercent;
	float _bgv;

	string mv;
	wstring mv_w = L"";

	string bgv;
	wstring bgv_w = L"";
public:
	settings();
	~settings();

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	void rcAlphaChange();
	virtual void render();


	//bool getIsMenu() { return _isMenu; }

};


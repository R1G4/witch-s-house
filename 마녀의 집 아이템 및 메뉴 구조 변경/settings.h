#pragma once
#include "gameNode.h"
#include "sound.h"
class settings : public gameNode
{
private:
	sound* _sound;
	FloatRect _rcSettingChoice;		//선택된 렉트 (설정 종류)
	FloatRect _rcSettings[7];		//전체 설정 종류
	float _x, _y;
	float _rcAlpha;
	float _rcAlphaChange;
	float _volume;
	int _percent;	//string 값 바꿔줄 변수
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
	MENUSTATE settingOpen();
	virtual void render();


	//bool getIsMenu() { return _isMenu; }

};


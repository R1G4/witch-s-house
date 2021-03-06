#pragma once
#include "gameNode.h"

enum BOSSDIRECTION
{
	BOSSDIRECTION_LEFT,
	BOSSDIRECTION_RIGHT,
	BOSSDIRECTION_UP,
	BOSSDIRECTION_DOWN
};

class boss : public gameNode
{
private:
	Image* _image;                     // 프레임 이미지 
	BOSSDIRECTION _direction;            // 상태
	FloatRect _rc;                     // 충돌렉트
	FloatRect _sub_rc;                  // 어색한 움직임 보정 렉트
	float _x, _y;                     // 초기위치를 위한 변수
	float _speed;                     // 추격속도
	float _currentFrameX, _currentFrameY;   // 현재 프레임 이미지

	
	float alpha;
	int count;
public:
	HRESULT init();
	HRESULT init(float x, float y);
	void update();
	void release();
	void render();

	FloatRect getRect() { return _rc; }
	void setRect(FloatRect rc) { _rc = rc; }

	void setalpha(float x) { alpha = x; }

	void setX(float x) { _x = x; }
	void setY(float y) { _y = y; }
};

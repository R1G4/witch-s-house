#pragma once
#include "gameNode.h"

class garDen;

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
	garDen* _gardenMap;

	Image* _image;						//프레임 이미지 
	BOSSDIRECTION _direction;			// 상태
	FloatRect _rc;							// 충돌렉트
	FloatRect _sub_rc;							// 충돌렉트
	float _x, _y;							// 초기위치를 위한 변수
	float _speed;						// 추격속도
	float _currentFrameX, _currentFrameY;	// 현재 프레임 이미지

	int count;

public:
	HRESULT init();
	HRESULT init(float x, float y);
	void update();
	void release();
	void render();

	void setMapMemoryAddressLink(garDen* gm) { _gardenMap = gm; }

	FloatRect getRect() { return _rc; }
	void setRect(FloatRect rc) { _rc = rc; }

	void setX(float x) { _x = x; }
	void setY(float y) { _y = y; }
};


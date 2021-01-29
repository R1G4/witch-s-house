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

	Image* _image;						//������ �̹��� 
	BOSSDIRECTION _direction;			// ����
	FloatRect _rc;							// �浹��Ʈ
	FloatRect _sub_rc;							// �浹��Ʈ
	float _x, _y;							// �ʱ���ġ�� ���� ����
	float _speed;						// �߰ݼӵ�
	float _currentFrameX, _currentFrameY;	// ���� ������ �̹���

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


#pragma once
#include "gameNode.h"

enum BEARDIRECTION
{
	BEARDIRECTION_LEFT,
	BEARDIRECTION_RIGHT,
	BEARDIRECTION_UP,
	BEARDIRECTION_DOWN
};

class bear : public gameNode
{
private:
	Image* _image;							// ������ �̹���
	BEARDIRECTION _direction;				// ����
	FloatRect _rc;							// �浹��Ʈ
	FloatRect _sub_rc;						// ����� ������ ���� ��Ʈ
	float _x, _y;							// �ʱ���ġ�� ���� ����
	float _speed;							// �߰ݼӵ�
	float _currentFrameX, _currentFrameY;	// ���� ������ �̹���

	int count;

public:
	HRESULT init();
	HRESULT init(float x, float y);
	void update();
	void release();
	void render();

	FloatRect getRect() { return _rc; }
	void setRect(FloatRect rc) { _rc = rc; }

	void setX(float x) { _x = x; }
	void setY(float y) { _y = y; }
};


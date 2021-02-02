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
	Image* _image;						//������ �̹��� 
	BOSSDIRECTION _direction;			// ����
	FloatRect _rc;							// �浹��Ʈ
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


};


#pragma once
#include "gameNode.h"

class firstFloorStage : public gameNode
{
protected:
	//������ �̹���(�÷��̾�, ���ʹ�, ������Ʈ) Ÿ��
	vector<tagFrameTile> _vFrameTile;
	//��Ÿ��
	tagTile _tiles[TILEX*TILEY];
	//�ɹ���
	int _frameInterval;
	bool isTrigger;
	Vector2 camera;
public:
	firstFloorStage();
	~firstFloorStage();

	HRESULT init();
	void release();
	void update();
	void render();
	void getFrameTile();
	bool addFrameTile(tagFrameTile _frameTile);
	void setFrameIndex();			// ��ġ�� ������ �̹����� �ε��� ���� 
};


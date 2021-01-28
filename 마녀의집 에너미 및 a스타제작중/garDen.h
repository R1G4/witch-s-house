#pragma once
#include "gameNode.h"
#include "mapTool.h"
#include "boss.h"

class garDen : public gameNode
{
private:
	tagTile _tiles[TILEX*TILEY];

	boss* _boss;

	DWORD _attribute[TILEX*TILEY];
	POINT camera;
	int _pos[2];
public:
	garDen();
	~garDen();

	virtual HRESULT init();
	virtual void update();
	virtual void release();
	virtual void render();
	virtual void cameraMove();//���� �߻� ���ɼ� ���� ���� �߻��ϸ� �ذ��Ϸ�������
	void load();
};


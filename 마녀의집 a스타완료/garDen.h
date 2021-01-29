#pragma once
#include "gameNode.h"
#include "mapTool.h"
#include "astarTile.h"

class boss;

class garDen : public gameNode
{
private:
	// astar���� ������
	vector<astarTile*>			_vTotalList;
	vector<astarTile*>::iterator _viTotalList;

	vector<astarTile*>			_vOpenList;
	vector<astarTile*>::iterator _viOpenList;

	vector<astarTile*>			_vCloseList;
	vector<astarTile*>::iterator _viCloseList;

	astarTile* _playerTile;					// �÷��̾� Ÿ��
	astarTile* _enemyTile;					// ���ʹ� Ÿ��
	astarTile* _objTile[TILEX*TILEY];		// ������Ʈ Ÿ��
	astarTile* _currentTile;				// ���� ���ʹ� Ÿ��

	bool _setTile;

	int _follow_count;	// �����ð�
	int MaxIndex;	// ����Ұ� ������Ʈ Ÿ���� ����
	int _numCount;	// ���° Ÿ������
	bool _start;
	bool _stop;			// ��ã����

private:
	tagTile _tiles[TILEX*TILEY];

	DWORD _attribute[TILEX*TILEY];
	Vector2 camera;
	int _pos[2];


	boss* _boss;
	int b_left;		// ���� Ÿ�� x
	int b_top;		// ���� Ÿ�� y
	int p_left;		// �ӽ� �÷��̾� Ÿ�� x
	int p_top;		// �ӽ� �÷��̾� Ÿ�� x

	float _p_x, _p_y;	// �ӽ� �÷��̾� ��ġ

public:
	garDen();
	~garDen();

	virtual HRESULT init();
	virtual void update();
	virtual void release();
	virtual void render();
	virtual void cameraMove();//���� �߻� ���ɼ� ���� ���� �߻��ϸ� �ذ��Ϸ�������

	void load();

	void playerLocation();
	void enemyLocation();
	void objectLocation();

public:
	// astar���� �Լ���
	void setAstarTile();									// ���� astarTile ����
	void resetEverything();									// ���� ����
	vector<astarTile*> addOpenList(astarTile* currentTile);	// ���� �ִ� ���� ã�Ƴ��� ����� �Լ�
	void pathFinder(astarTile* currentTile);				// ���� ��� ã�� �Լ�

	vector<astarTile*>			 getVTotalList()  { return _vTotalList; };
	vector<astarTile*>::iterator getViTotalList() { return _viTotalList; };

};


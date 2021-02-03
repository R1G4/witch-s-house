#pragma once
#include "gameNode.h"
#include"boss.h"
class Player;
class bossStage :
	public gameNode
{
private://astar�� ����
	//astar�� ����
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

private://���������� ����
	//
	Image* _backGround;
	tagTile _tiles[TILEX*TILEY];
	Vector2 camera;
	Player* _player;
	boss* _boss;
	int bossLocX, bossLocY;
	int bossFrameX, bossFrameY;
	bool _isBossAppeal;
	float alpha;
	bool _Stop;
public://�������� ���ÿ�
	HRESULT init();
	void release();
	void update();
	void render();
	void load();
	void tileCollision();
	void addresslink(Player* pla) { _player = pla; }
	void activeTrigger();
	void activeCorr();

public://a��Ÿ��

	void playerLocation();
	void enemyLocation();
	void objectLocation();

	void setAstarTile();									// ���� astarTile ����
	void resetEverything();									// ���� ����
	vector<astarTile*> addOpenList(astarTile* currentTile);	// ���� �ִ� ���� ã�Ƴ��� ����� �Լ�
	void pathFinder(astarTile* currentTile);				// ���� ��� ã�� �Լ�

	vector<astarTile*>			 getVTotalList() { return _vTotalList; };
	vector<astarTile*>::iterator getViTotalList() { return _viTotalList; };
};


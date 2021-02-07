#pragma once
#include "gameNode.h"
#include"boss.h"
#include "DeadManager.h"
class Player;
struct Trigger
{
	bool isTriggerOn;
	int idx, idy;
	FloatRect tile;
};
class bossStage :
	public gameNode
{
protected://astar�� ����
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

protected://���������� ����
	//��� �̹̤�
	Image* _backGround;
	//��Ÿ��
	tagTile _tiles[TILEX*TILEY];
	//������ Ÿ�Ͽ� ����
	vector<tagFrameTile> _vFrameTile;
	//ī�޶�
	Vector2 camera;
	Player* _player;
	boss* _boss;
	//����� ����
	DeadManager* dead;
	//���� ��ġ ����
	int bossLocX, bossLocY;
	//���� ������ ����
	int bossFrameX, bossFrameY;
	//���� ���� ����
	bool _isBossAppeal;
	//���İ�
	float alpha;
	//Ư�� ��Ȳ���� ������Ʈ�� ���߰� �ϱ� ���� ����
	bool _Stop;
	int save_x;
	int save_y;
	CHRDIRECTION save_direc;
	string save_s_x;
	string save_s_y;
	string save_s_direc;
public://�������� ���ÿ�
	bossStage();
	~bossStage();

	HRESULT init();
	virtual HRESULT init(int x, int y,CHRDIRECTION direc);
	void release();
	void update();
	void render();
	void tileCollision();
	void addresslink(Player* pla) { _player = pla; }
	void getFrameTile();
	void setFrameIndex();


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


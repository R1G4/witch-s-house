#pragma once
#include "fifthFloorStage.h"

class prison_5f : public fifthFloorStage
{
private:
	//astar ����
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

private:
	enum IRONDOORORDER
	{
		FIRST,
		SECOND,
		THIRD
	};
	enum TRIGGER
	{
		DOORTOGARDEN = 17 + 15 * TILEX,
		DOORTOPRISONWELL = 22 + 4 * TILEX,
		DOORTOPRISONSKUL = 26 + 6 * TILEX,
		CAGE = 40 + 15 * TILEX,
		R_FLOWER_1 = 44 + 14 * TILEX,
		R_FLOWER_2 = 45 + 15 * TILEX,
		R_FLOWER_3 = 44 + 16 * TILEX,
		BOOK = 25 + 14 * TILEX,
		DOOR_1 = 30 + 14 * TILEX,
		DOOR_2 = 37 + 14 * TILEX,
		PAPER = 35 + 14 * TILEX,
		SKUL = 23 + 13 * TILEX,
		SUMMONSKUL = 19 + 15 * TILEX
	};

	TRIGGER _trigger;
	IRONDOORORDER _ido;
	int _skulX, _skulY;
	bool _isSummon;
	bool _isDrop;
	bool _sound;
	bool _sound_item;
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

public:
	prison_5f() {};
	~prison_5f() {};

	HRESULT init(CHRDIRECTION _chrdirection = CHRDIREC_UP, LOCATION _location = LOCATION_DEFAULT);
	void release();
	void update();
	void render();
	void Collision();
	void load(LOCATION location = LOCATION_DEFAULT);

	void setTrigger();
	void getMemory();
};


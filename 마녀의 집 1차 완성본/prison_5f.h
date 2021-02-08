#pragma once
#include "fifthFloorStage.h"

class prison_5f : public fifthFloorStage
{
private:
	//astar 세팅
    //astar용 벡터
	vector<astarTile*>			_vTotalList;
	vector<astarTile*>::iterator _viTotalList;

	vector<astarTile*>			_vOpenList;
	vector<astarTile*>::iterator _viOpenList;

	vector<astarTile*>			_vCloseList;
	vector<astarTile*>::iterator _viCloseList;

	astarTile* _playerTile;					// 플레이어 타일
	astarTile* _enemyTile;					// 에너미 타일
	astarTile* _objTile[TILEX*TILEY];		// 오브젝트 타일
	astarTile* _currentTile;				// 현재 에너미 타일

	bool _setTile;

	int _follow_count;	// 추적시간
	int MaxIndex;	// 통과불가 오브젝트 타일의 갯수
	int _numCount;	// 몇번째 타일인지
	bool _start;
	bool _stop;			// 못찾은거

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
public://a스타용

	void playerLocation();
	void enemyLocation();
	void objectLocation();

	void setAstarTile();									// 시작 astarTile 세팅
	void resetEverything();									// 모든것 리셋
	vector<astarTile*> addOpenList(astarTile* currentTile);	// 갈수 있는 길을 찾아내서 담아줄 함수
	void pathFinder(astarTile* currentTile);				// 빠른 경로 찾을 함수

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


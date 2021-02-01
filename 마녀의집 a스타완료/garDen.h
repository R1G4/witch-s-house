#pragma once
#include "gameNode.h"
#include "mapTool.h"
#include "astarTile.h"
#include "skul.h"
#include "boss.h"
#include "bear.h"

class garDen : public gameNode
{
private:
	// astar위한 변수들
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
	tagTile _tiles[TILEX*TILEY];

	DWORD _attribute[TILEX*TILEY];
	Vector2 camera;
	int _pos[2];


	boss* _boss;
	skul* _skul;
	bear* _bear;
	int b_left;		// 보스 타일 x
	int b_top;		// 보스 타일 y
	int p_left;		// 임시 플레이어 타일 x
	int p_top;		// 임시 플레이어 타일 x

	float _p_x, _p_y;	// 임시 플레이어 위치

	bool _isTrigger;
	astarTile* _triggerTile;				// 트리거 타일
public:
	garDen();
	~garDen();

	virtual HRESULT init();
	virtual void update();
	virtual void release();
	virtual void render();
	virtual void cameraMove();//문제 발생 가능성 높음 문제 발생하면 해결하러가야함

	void load();

	void playerLocation();
	void enemyLocation();
	void objectLocation();
	void getTrigger();

public:
	// astar위한 함수들
	void setAstarTile();									// 시작 astarTile 세팅
	void resetEverything();									// 모든것 리셋
	vector<astarTile*> addOpenList(astarTile* currentTile);	// 갈수 있는 길을 찾아내서 담아줄 함수
	void pathFinder(astarTile* currentTile);				// 빠른 경로 찾을 함수

	vector<astarTile*>			 getVTotalList()  { return _vTotalList; };
	vector<astarTile*>::iterator getViTotalList() { return _viTotalList; };

};


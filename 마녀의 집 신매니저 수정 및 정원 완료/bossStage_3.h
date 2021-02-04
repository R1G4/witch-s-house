#pragma once
#include "Player.h"
#include "boss.h"
#include "gameNode.h"
struct Trigger
{
	bool isTriggerOn;
	int idx, idy;
	FloatRect tile;
};
class bossStage_3 :
	public gameNode
{
private:
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
	Image* _backGround;
	tagTile _tiles[TILEX*TILEY];
	Vector2 camera;
	Player* _player;
	boss* _boss;
	int bossLocX, bossLocY;
	int bossFrameX, bossFrameY;
	vector<tagTile> _vObjTile;
	vector<Trigger> _vTrigger;
	bool _isBossAppeal;
	float alpha;
	bool _isStop;
public:
	HRESULT init();
	void release();
	void update();
	void render();
	void load();
	void tileCollision();
	void addresslink(Player* pla) { _player = pla; }
	void TriggerOn();
public:
	void playerLocation();
	void enemyLocation();
	void objectLocation();

	void setAstarTile();									// 시작 astarTile 세팅
	void resetEverything();									// 모든것 리셋
	vector<astarTile*> addOpenList(astarTile* currentTile);	// 갈수 있는 길을 찾아내서 담아줄 함수
	void pathFinder(astarTile* currentTile);				// 빠른 경로 찾을 함수

	vector<astarTile*>			 getVTotalList() { return _vTotalList; };
	vector<astarTile*>::iterator getViTotalList() { return _viTotalList; };

};


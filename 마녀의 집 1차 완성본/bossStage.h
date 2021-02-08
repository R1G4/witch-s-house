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
protected://astar용 세팅
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

protected://스테이지용 세팅
	//배경 이미ㅣ
	Image* _backGround;
	//맵타일
	tagTile _tiles[TILEX*TILEY];
	//프레임 타일용 벡터
	vector<tagFrameTile> _vFrameTile;
	//카메라
	Vector2 camera;
	Player* _player;
	boss* _boss;
	//데드씬 관련
	DeadManager* dead;
	//보스 위치 설정
	int bossLocX, bossLocY;
	//보스 프레임 설정
	int bossFrameX, bossFrameY;
	//보스 등장 여부
	bool _isBossAppeal;
	//알파값
	float alpha;
	//특정 상황에서 업데이트를 멈추게 하기 위한 변수
	bool _Stop;
	int save_x;
	int save_y;
	CHRDIRECTION save_direc;
	string save_s_x;
	string save_s_y;
	string save_s_direc;
public://스테이지 세팅용
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
};


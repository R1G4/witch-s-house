#pragma once
#include "gameNode.h"
#include "bear.h"
#include "DeadManager.h"

class  Player;
class firstFloorStage : public gameNode
{
	//관련 스테이지 참조를 위해 접근자 protected로 설정
protected:
	//스테이지 관련 변수
	enum MISSION
	{
		FAIL,
		SUCCESS
	};
	//프레임 이미지(플레이어, 에너미, 오브젝트) 타일
	vector<tagFrameTile> _vFrameTile;

	//맵타일
	tagTile _tiles[TILEX*TILEY];

	//케뭬라
	Vector2 camera;

	vector<string> _vScript;
	Player* _player;
	bear* _bear;
	DeadManager* _dead;
	float _sceneAlpha;			//씬 투명도
	int _frameInterval;			//프레임 바뀌는 간격
	int _delay;					//딜레이(특정 트리거 밞을 시 멈칫 하는 ?)
	bool isTrigger;
	bool _isBlood;
	float _light;
	//A* 관련 변수
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
	int MaxIndex;		// 통과불가 오브젝트 타일의 갯수
	int _numCount;		// 몇번째 타일인지
	bool _start;		// 시작할 시
	bool _stop;			// 못찾은거		
	int bossLocX;
	int bossLocY;
public:
	firstFloorStage();
	~firstFloorStage();

	HRESULT init();
	void release();
	void update();
	void enemyUpdate();
	void cameraUpdate();
	void render();
	void getFrameTile();
	void setFrameIndex();			// 배치된 프레임 이미지의 인덱스 설정 
	void tileCollision(int i, int j);
	void sceneChange(string name, CHRDIRECTION _chrdirection = CHRDIREC_UP, LOCATION _location = LOCATION_DEFAULT);
	void setAlpha();
	void addresslink(Player* player) { _player = player; }

	//폼 관련 변수
protected:
	enum FROM_DIRECTION
	{
		LEFT,
		RIGHT
	};
	FROM_DIRECTION _fromSelected;

private:
	bool _isForm;
	wstring FormInfo[2];
	float _rcAlpha;			//렉트 알파값(투명도)
	float _rcAlphaChange;	//렉트 알파값(투명도) 조절용
	FloatRect _rcSelected;	//선택된 박스를 렉트로 보여줌

public:
	void rcAlphaChange();
	void playerLocation();
	void enemyLocation();
	void objectLocation();
	void setAstarTile();
	void resetEverything();
	vector<astarTile*> addOpenList(astarTile * currentTile);
	void pathFinder(astarTile * currentTile);
	void autoSound(string key);
	void FormRender();

	//텍스를 넣는 동시에 폼 실행
	bool SelectionForm(wstring leftText, wstring rightText);

	//텍스트 넣기
	void setForm(wstring leftText, wstring rightText)
	{
		FormInfo[LEFT] = leftText;
		FormInfo[RIGHT] = rightText;
	};
};


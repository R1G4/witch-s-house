#pragma once
#include "gameNode.h"
#include "skul.h"
#include "DeadManager.h"

class Player;

class fifthFloorStage : public gameNode
{
protected:
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

	bool _isChangeScene;
	bool _isDead;
	bool _isMove;

	Player* _player;
	skul* _skul;
	DeadManager* _dead;

	int _frameInterval;
	bool isTrigger;
	bool _isSkulAppeal;
	bool _flowerDead;

	float _sceneAlpha;			//씬 투명도

	float _rcAlpha;				//선택창 투명도
	float _rcAlphaChange;

	bool _isStopToRead;			// 다이어로그용
	vector<string> _vScript;

	LOCATION5F _real_location1;
public:
	fifthFloorStage() {};
	~fifthFloorStage() {};

	HRESULT init();
	void release();
	void update();
	void cameraUpdate();
	void render();
	void getFrameTile();
	void setFrameIndex();			// 배치된 프레임 이미지의 인덱스 설정 
	void tileCollision();
	void rcAlphaChange();
	void sceneChange(string name);
	void sceneChange(string name, CHRDIRECTION _chrdirection, LOCATION _location);
	void addresslink(Player* player) { _player = player; }

	wstring stringToWstring(string src)
	{
		USES_CONVERSION;
		return A2W(src.c_str());
	}
};


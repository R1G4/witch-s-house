#pragma once
#include "gameNode.h"

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

	Player* _player;
	int _frameInterval;
	bool isTrigger;

	float _sceneAlpha;			//씬 투명도
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
	void sceneChange(string name);
	void sceneChange(string name, CHRDIRECTION _chrdirection, LOCATION _location);
	void addresslink(Player* player) { _player = player; }

	wstring stringToWstring(string src)
	{
		USES_CONVERSION;
		return A2W(src.c_str());
	}
};


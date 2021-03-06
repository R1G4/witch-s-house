#pragma once
#include "gameNode.h"

class  Player;
class firstFloorStage : public gameNode
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

	Player* _player;
	int _frameInterval;
	bool isTrigger;

	float _sceneAlpha;			//씬 투명도
public:
	firstFloorStage();
	~firstFloorStage();

	HRESULT init();
	void release();
	void update();
	void cameraUpdate();
	void render();
	void getFrameTile();
	void setFrameIndex();			// 배치된 프레임 이미지의 인덱스 설정 
	void tileCollision(int i, int j);
	void sceneChange(string name);
	void addresslink(Player* player) { _player = player; }
};


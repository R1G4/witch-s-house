#pragma once
#include "gameNode.h"

class firstFloorStage : public gameNode
{
protected:
	//프레임 이미지(플레이어, 에너미, 오브젝트) 타일
	vector<tagFrameTile> _vFrameTile;
	//맵타일
	tagTile _tiles[TILEX*TILEY];
	//케뭬라
	int _frameInterval;
	bool isTrigger;
	Vector2 camera;
public:
	firstFloorStage();
	~firstFloorStage();

	HRESULT init();
	void release();
	void update();
	void render();
	void getFrameTile();
	bool addFrameTile(tagFrameTile _frameTile);
	void setFrameIndex();			// 배치된 프레임 이미지의 인덱스 설정 
};


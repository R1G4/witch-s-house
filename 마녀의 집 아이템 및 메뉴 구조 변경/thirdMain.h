#pragma once
#include "gameNode.h"
#include "Player.h"
class thirdMain : public gameNode
{
private:

	Image* _backGround;
	Image* _saveCat;
	tagTile _tiles[TILEX*TILEY];
	Vector2 camera;
	Player* _player;

	int _count;
	int _catFrameX;
	int _catFrameY;
	int _candleFrame;


public:
	thirdMain();
	~thirdMain();


	HRESULT init(CHRDIRECTION _CHRDIRECTION); //플레이어 현재 상태에 따라 초기 위치, 모션 병경해주려고
	void release();
	void update();
	void render();
	void changeScene();
	void tileCollision();
	void load();
};


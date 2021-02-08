#pragma once
#include "gameNode.h"
#include "Player.h"

class thirdTo4thLoad : public gameNode
{
private:

	Image* _backGround;
	tagTile _tiles[TILEX*TILEY];
	Vector2 camera;
	Player* _player;

	int _count;
	int _frame;
	int _eyesFrameX;
	int _eyesFrameY;
	int _candleFrame;
	bool _moveSword;
	bool _moveEyes;

public:
	thirdTo4thLoad();
	~thirdTo4thLoad();

	HRESULT init();
	void release();
	void update();
	void render();
	void changeScene();
	void trigger();
	void tileCollision();
	void load();
};


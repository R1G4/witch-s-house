#pragma once
#include "gameNode.h"
#include "Player.h"

class third : public gameNode
{
private:

	enum trigger
	{
		SWORD = 1222,

		DEAD
	};


	Image* _backGround;
	tagTile _tiles[TILEX*TILEY];
	Vector2 camera;
	Player* _player;
	trigger _trigger;

	int _count;
	int _frame;


public:
	third();
	~third();

	HRESULT init();
	void release();
	void update();
	void render();
	void trigger();
	void tileCollision();
	void Collision();
	void load();
};


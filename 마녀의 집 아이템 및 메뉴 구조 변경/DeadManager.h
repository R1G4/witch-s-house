#pragma once
#include "gameNode.h"
#include "Player.h"
enum DEADVARIABLE
{
	DEAD_BOSS,
	DEAD_SKULL,
	DEAD_BEAR,
	DEAD_WALL,
	DEAD_CLOCK,
	DEAD_SOLDIER,
	DEAD_SNAKE
};
class DeadManager :
	public gameNode
{
private:
	DEADVARIABLE _dead;
	Image* _deadImage;
	Image* _deadImage2;
	FloatRect _deadRc;
	FloatRect _deadRc2;
	int frameX, frameY;
	int _count,_endCount;
	bool _gameOver;
	bool _isDead;
	Player* _player;
	float locX, locY;

public:
	void update();
	void render();
	void setDead(DEADVARIABLE dead);
	void frameXPlay();
	void frameYPlay();
	void setPlayerAddress(Player* player) { _player = player; }
	bool getIsDead() { return _isDead; }
	void setLocXY(float x, float y) { locX = x , locY = y; }
};


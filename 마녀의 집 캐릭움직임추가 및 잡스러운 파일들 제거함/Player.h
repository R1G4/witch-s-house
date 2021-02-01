#pragma once
#include "gameNode.h"
class characterState;
enum CHRDIRECTION
{
	CHRDIREC_DOWN,
	CHRDIREC_LEFT,
	CHRDIREC_RIGHT,
	CHRDIREC_UP
};
enum STATE
{
	CHR_IDLE,
	CHR_WALK,
	CHR_DASH,
	CHR_DEAD
};
struct tagPlayer
{
	float x, y;
	Image* img;
	FloatRect rc;
	int frameX, frameY;
	STATE state;
	CHRDIRECTION direc;
	float speed;
	bool isDash;
};
class Player :
	public gameNode
{
private:
	tagPlayer _player;
	Vector2 _startPosition;
	characterState* chr_State;
	int _count;
public:
	HRESULT init();
	void release();
	void update();
	void render();
	void setStart(int x,int y) { _startPosition = Vector2(x,y); }
	void setState(STATE st);
	void setChrImg(Image* img) { _player.img = img; }
	void setDirec(CHRDIRECTION chr_direc) { _player.direc = chr_direc; };
	bool getIsDash() { return _player.isDash; }
	void setFrameX(int frame) { _player.frameX = frame; }
	void setSpeed(float speed) { _player.speed = speed; }
	void setIsDash(bool onOff) { _player.isDash = onOff; }
	void framePlay();
	void move();
	//Vector2 getPlayerLoc() { return Vector2(_player.x, _player.y); }
	float getPlayerLocX() { return _player.x; }
	float getPlayerLocY() { return _player.y; }
};


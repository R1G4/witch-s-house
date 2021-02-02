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
	FloatRect searchRc;
	float alpha;
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
	void setState(STATE st);//플레이어 상태전환을 위한 함수
	void setDirec(CHRDIRECTION chr_direc) { _player.direc = chr_direc; };//플레이어 방향 전환을 위한 함수
	bool getIsDash() { return _player.isDash; }
	void framePlay();
	void move();
	//Vector2 getPlayerLoc() { return Vector2(_player.x, _player.y); }
	//플레이어 관련 getter,setter
	float getPlayerLocX() { return _player.x; }
	float getPlayerLocY() { return _player.y; }
	void setFrameX(int frame) { _player.frameX = frame; }
	void setSpeed(float speed) { _player.speed = speed; }
	void setIsDash(bool onOff) { _player.isDash = onOff; }
	void setStart(int x, int y) { _startPosition = Vector2(x, y); }
	void setChrImg(Image* img) { _player.img = img; }
	void setPLocaX(float x) { _player.x = x; }
	void setPLocaY(float y) { _player.y = y; }
	CHRDIRECTION getPdirec() { return _player.direc; }
	FloatRect getPlayerFrc() { return _player.rc; }
	FloatRect getSearchRc() { return _player.searchRc; }
	void setAlpha(float x) { _player.alpha = x; }
};


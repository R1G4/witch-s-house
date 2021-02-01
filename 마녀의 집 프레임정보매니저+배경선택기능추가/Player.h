#pragma once
#include "gameNode.h"
enum CHRDIRECTION
{
	CHRDIREC_UP,
	CHRDIREC_DOWN,
	CHRDIREC_RIGHT,
	CHRDIREC_LEFt
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
	int frameX, frameY;
	float speed;
};
class Player :
	public gameNode
{
private:
	tagPlayer _player;
	Vector2 _startPosition;
public:
	HRESULT init();
	void release();
	void update();
	void render();
	void setStart(POINT pt) { _startPosition = Vector2(pt.x*TILESIZE,pt.y*TILESIZE); }
};


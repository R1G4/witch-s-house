#pragma once
#include "gameNode.h"
#include "Player.h"
class thirdFrogRoom : public gameNode
{
private:



	Image* _backGround;
	Image* _bar;
	Image* _frog;
	tagTile _tiles[TILEX*TILEY];
	Vector2 camera;
	Player* _player;
	FloatRect _rc;
	text _text;

	float _x, _y;
	float _x2, _y2;
	float _rcAlpha;
	float _rcAlphaChange;
	int _count;
	int _alpha;
	int _frameX;
	int _frameY;
	bool _isText;

public:
	thirdFrogRoom();
	~thirdFrogRoom();

	HRESULT init();
	void release();
	void update();
	void render();
	void rcAlphaChange();
	void changeScene();
	void tileCollision();
	void openText();
	void load();
};


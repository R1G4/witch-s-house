#pragma once
#include "gameNode.h"
#include "Player.h"



class soundStageN : public gameNode
{
private:
	Image* _backGround;
	tagTile _tiles[TILEX*TILEY];
	Vector2 camera;
	Player* _player;

	getRect mapChange[2];
	getRect uiTrigger;

	float Px, Py;	//ÇÇ¾Æ³ë ÁÂÇ¥

	vector<string> _vScript;
	bool _isStopToRead;
	bool _isClick;
	bool _left;
	bool _right;

	FloatRect _rc;
	FloatRect _correct_rc;
	float _rcAlpha;
	float _rcAlphaChange;

	enum Trigger
	{
		P1 = 19 + 12 * TILEX,
		P2 = 20 + 12 * TILEX,
		P3 = 21 + 12 * TILEX,
		DOOR = 25 + 7 * TILEX
	};

public:
	soundStageN();
	~soundStageN();
	virtual HRESULT init();
	virtual HRESULT init(CHRDIRECTION _CHRDIRECTION);
	virtual void release();
	virtual void update();
	virtual void render();

	void load();
	void tileCollision();
};


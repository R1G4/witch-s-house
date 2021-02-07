#pragma once
#include "gameNode.h"
#include "Player.h"


class soundStageE2 : public gameNode
{
private:
	Image* _backGround;
	tagTile _tiles[TILEX*TILEY];
	Vector2 camera;
	Player* _player;

	getRect mapChange[2];
	getRect uiTrigger;

	vector<string> _vScript;
	bool _isStopToRead;
	bool _isClick;
	bool _disCover;
	

	FloatRect _rc;
	FloatRect _correct_rc;
	float _rcAlpha;
	float _rcAlphaChange;


	enum Trigger
	{
		PAPER = 20 + 6 * TILEX,
		BOOKS = 18 + 7 * TILEX,
		DOLL = 22 + 7 * TILEX,
		CLOCK = 22 + 12 * TILEX,
		PUMPKIN = 18 + 12 * TILEX
	};

public:
	soundStageE2();
	~soundStageE2();
	virtual HRESULT init();
	virtual HRESULT init(CHRDIRECTION _CHRDIRECTION);
	virtual void release();
	virtual void update();
	virtual void render();

	void load();
	void tileCollision();
};


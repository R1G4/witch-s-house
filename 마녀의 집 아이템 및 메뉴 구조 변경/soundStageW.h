#pragma once
#include "gameNode.h"
#include "Player.h"



class soundStageW : public gameNode
{
private:
	Image* _backGround;
	tagTile _tiles[TILEX*TILEY];
	Vector2 camera;
	Player* _player;

	getRect mapChange;
	getRect uiTrigger;

	vector<string> _vScript;
	bool _isStopToRead;
	bool _isClick;
	
	bool _open;
	bool _choose;
	bool _left;
	bool _right;
	bool _up;

	FloatRect _rc;
	FloatRect _correct_rc;
	float _rcAlpha;
	float _rcAlphaChange;
	bool _disCover;

	enum Trigger
	{
		TRIGGER = 23 + 8 * TILEX
	};

public:
	soundStageW();
	~soundStageW();
	virtual HRESULT init();
	//virtual HRESULT init(CHRDIRECTION _CHRDIRECTION);
	virtual void release();
	virtual void update();
	virtual void render();

	void load();
	void tileCollision();
};


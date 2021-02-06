#pragma once
#include "gameNode.h"
#include "Player.h"
class thirdFrogOutRoom : public gameNode
{
private:

	Image* _backGround;
	Image* _bar;
	Image* _frogOut;
	text _text;
	tagTile _tiles[TILEX*TILEY];
	Vector2 camera;
	Player* _player;
	FloatRect _rc;

	float _x, _y;
	float _rcAlpha;
	float _rcAlphaChange;
	int _count;
	int _frame;
	bool _isText;		//첫번째 상호작용 텍스트창 관리
	bool _leftClick;	//두번째 상호작용 텍스트창 관리
	bool _rightClick;	//두번째 상호작용 텍스트창 관리
	bool _playerRender;	//플레이어 랜더 관리
	bool _dialogue;

	bool _isStopToRead;			//텍스트매니저용 변수
	vector<string> _vScript;	//
public:
	thirdFrogOutRoom();
	~thirdFrogOutRoom();

	HRESULT init();
	void release();
	void update();
	void render();
	void changeScene();
	void rcAlphaChange();
	void readBook();
	void trigger();
	void tileCollision();
	void load();
};


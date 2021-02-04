#pragma once
#include "gameNode.h"
#include "Player.h"
class third2 : public gameNode
{
private:

	/*enum text
	{
		LEFT, RIGHT, UP, DOWN, OPENTEXT,OPENLEFT, OPENRIGHT, CHANGEIMG
	};*/

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
	bool _playerRender;
public:
	third2();
	~third2();

	HRESULT init();
	void release();
	void update();
	void render();
	void rcAlphaChange();
	void trigger();
	void tileCollision();
	void load();
};


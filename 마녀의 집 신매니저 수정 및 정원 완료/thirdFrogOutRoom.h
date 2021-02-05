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
	bool _isText;		//ù��° ��ȣ�ۿ� �ؽ�Ʈâ ����
	bool _leftClick;	//�ι�° ��ȣ�ۿ� �ؽ�Ʈâ ����
	bool _rightClick;	//�ι�° ��ȣ�ۿ� �ؽ�Ʈâ ����
	bool _playerRender;	//�÷��̾� ���� ����
	bool _dialogue;

	bool _isStopToRead;			//�ؽ�Ʈ�Ŵ����� ����
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


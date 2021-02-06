#pragma once
#include "gameNode.h"
#include "Player.h"
class thirdLibrary : public gameNode
{
private:

	Image* _backGround;
	tagTile _tiles[TILEX*TILEY];
	Vector2 camera;
	Player* _player;

	int _count;
	int _frame;
	bool _dialogue;
	bool _isStopToRead;			//텍스트매니저용 변수
	vector<string> _vScript;	//

public:
	thirdLibrary();
	~thirdLibrary();

	HRESULT init();
	void release();
	void update();
	void render();
	void changeScene();
	void readBook();
	void tileCollision();
	void load();
};


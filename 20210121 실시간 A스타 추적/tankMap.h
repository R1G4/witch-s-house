#pragma once
#include "gameNode.h"
#include "tileNode.h"

class tankMap : public gameNode
{
private:
	tagTile _tiles[TILEX * TILEY];

	//여러분 속성을 enum으로 하면 상관없겠지만
	//쌤은 비트필드란걸 써서 할꺼이므로 더블워드로 할게여
	DWORD _attribute[TILEX * TILEY];

	int _pos[2];	//사실은 의미가 없다 ㅋㅋㅋㅋ

public:
	tankMap();
	~tankMap();

	HRESULT init();
	void release();
	void update();
	void render();

	void load();

	tagTile* getTile() { return _tiles; }

	//2. 해당 속성을 넘겨줄 getter를 쌤이 만들었어요
	//2번패스
	DWORD* getAttribute() { return _attribute; }

	int getPosFirst() { return _pos[0]; }
	int getPosSecond() { return _pos[1]; }
};


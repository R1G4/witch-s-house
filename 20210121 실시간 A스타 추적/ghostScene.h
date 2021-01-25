#pragma once
#include "gameNode.h"
#include "knight.h"

class ghostScene : public gameNode
{
private:
	knight* _knight;

public:
	ghostScene() {};
	~ghostScene() {};

	HRESULT init();
	void release();
	void update();
	void render();

};


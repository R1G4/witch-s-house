#pragma once
#include "Player.h"
class characterState
{
private:
	Player* _player;
public:
	~characterState();
	virtual void enterState()=0;
	virtual void updateState() = 0;
	virtual void exitState() = 0;
	void LinkPlayerAddress(Player* player)
	{
		_player = player;
	}
};


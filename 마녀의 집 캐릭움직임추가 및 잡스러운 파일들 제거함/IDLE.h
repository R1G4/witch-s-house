#pragma once
#include"characterState.h"
class Player;
class IDLE:public characterState
{
public:
	void enterState();
	void updateState();
	void exitState();
};


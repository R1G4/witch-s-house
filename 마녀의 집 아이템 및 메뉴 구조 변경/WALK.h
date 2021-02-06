#pragma once
#include "characterState.h"
class WALK :
	public characterState
{
public:
	void enterState();
	void updateState();
	void exitState();
};


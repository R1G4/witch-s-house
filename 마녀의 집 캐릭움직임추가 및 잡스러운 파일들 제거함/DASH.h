#pragma once
#include "characterState.h"
class DASH :
	public characterState
{
public:
	void enterState();
	void updateState();
	void exitState();
};


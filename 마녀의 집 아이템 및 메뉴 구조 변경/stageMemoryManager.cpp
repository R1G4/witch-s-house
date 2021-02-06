#include "stdafx.h"
#include "stageMemoryManager.h"

//생성자가 호출될 시 이니셜라이징으로 변수 초기화
stageMemoryManager::stageMemoryManager():
	isCandle(false),
	isVase(false),
	isBearPickUp(false),
	isBearPut(false),
	isBearComing(false),
	isBox(false),
	isPalmLeft(false),
	isPalmRight(false)
{
}

stageMemoryManager::~stageMemoryManager()
{
}

#include "stdafx.h"
#include "stageMemoryManager.h"

//생성자가 호출될 시 이니셜라이징으로 변수 초기화
stageMemoryManager::stageMemoryManager() :
	isCandle(false),
	isVase(false),
	isBearPickUp(false),
	isBearPut(false),
	isBearComing(false),
	isBox(false),
	isPalmLeft(false),
	isPalmRight(false),
	isPalm(false),
	isScissors(false),
	isBearComing2(false),

	// 5층
	isSkuls_1(false),
	isSkuls_2(false),
	isSkuls_3(false),
	isSkuls_4(false),
	isSkuls_5(false),
	isRedFlower_1(false),
	isRedFlower_2(false),
	isRedFlower_3(false),
	isGetSkul1(false),
	isGetSkul2(false),
	isGetSkul3(false),
	isGetSkul4(false),
	isFlowerDead(false),
	isPotion(false),
	isKey(false),
	isLever(false),
	isFlowerpot(false)
{
}

stageMemoryManager::~stageMemoryManager()
{
}

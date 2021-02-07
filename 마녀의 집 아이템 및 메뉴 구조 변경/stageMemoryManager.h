#pragma once
#include "singletonBase.h"

class stageMemoryManager : public singletonBase<stageMemoryManager>
{
private:
	bool isCandle;
	bool isVase;
	bool isBearPickUp;
	bool isBearPut;
	bool isBearComing;
	bool isBox;
	bool isPalmLeft;
	bool isPalmRight;
	bool isPalm;

	//5Ãþ
	bool isSkuls_1;
	bool isSkuls_2;
	bool isSkuls_3;
	bool isSkuls_4;
	bool isSkuls_5;
	bool isRedFlower_1;
	bool isRedFlower_2;
	bool isRedFlower_3;
	bool isGetSkul1;
	bool isGetSkul2;
	bool isGetSkul3;
	bool isGetSkul4;
	bool isFlowerDead;
	bool isPotion;
	bool isKey;
	bool isLever;
	bool isFlowerpot;
public:
	stageMemoryManager();
	~stageMemoryManager();

	bool getIsCandle() { return isCandle; }
	bool getIsVase() { return isVase; }
	bool getIsBearPickUp() { return isBearPickUp; }
	bool getIsBearPut() { return isBearPut; }
	bool getIsBearComing() { return isBearComing; }
	bool getIsBox() { return isBox; }
	bool getIsPalmLeft() { return isPalmLeft; }
	bool getIsPalmRight() { return isPalmRight; }
	bool getIsPalm() { return isPalm; }

	//5Ãþ	
	bool getIsSkul1() { return isSkuls_1; }
	bool getIsSkul2() { return isSkuls_2; }
	bool getIsSkul3() { return isSkuls_3; }
	bool getIsSkul4() { return isSkuls_4; }
	bool getIsSkul5() { return isSkuls_5; }
	bool getIsRedFlower1() { return isRedFlower_1; }
	bool getIsRedFlower2() { return isRedFlower_2; }
	bool getIsRedFlower3() { return isRedFlower_3; }
	bool getIsGetSkul1() { return isGetSkul1; }
	bool getIsGetSkul2() { return isGetSkul2; }
	bool getIsGetSkul3() { return isGetSkul3; }
	bool getIsGetSkul4() { return isGetSkul4; }
	bool getIsFlowerDead() { return isFlowerDead; }
	bool getIsPotion() { return isPotion; }
	bool getIsKey() { return isKey; }
	bool getIsLever() { return isLever; }
	bool getIsFlowerpot() { return isFlowerpot; }

	void setIsCandle(bool IsCandle) { isCandle = IsCandle; }
	void setIsVase(bool IsVase) { isVase = IsVase; }
	void setIsBearPickUp(bool IsBearPickUp) { isBearPickUp = IsBearPickUp; }
	void setIsBearPut(bool IsBearPut) { isBearPut = IsBearPut; }
	void setIsBearComing(bool IsBearComing) { isBearComing = IsBearComing; }
	void setIsBox(bool IsBox) { isBox = IsBox; }
	void setIsPalmLeft(bool IsPalmLeft) { isPalmLeft = IsPalmLeft; }
	void setIsPalmRight(bool IsPalmRight) { isPalmRight = IsPalmRight; }
	void setIsPalm(bool IsPalm) { isPalm = IsPalm; }

	//5Ãþ
	void setIsSkul1(bool _skul) { isSkuls_1 = _skul; }
	void setIsSkul2(bool _skul) { isSkuls_2 = _skul; }
	void setIsSkul3(bool _skul) { isSkuls_3 = _skul; }
	void setIsSkul4(bool _skul) { isSkuls_4 = _skul; }
	void setIsSkul5(bool _skul) { isSkuls_5 = _skul; }
	void setIsRedFlower1(bool _speak) { isRedFlower_1 = _speak; }
	void setIsRedFlower2(bool _speak) { isRedFlower_2 = _speak; }
	void setIsRedFlower3(bool _speak) { isRedFlower_3 = _speak; }
	void setIsGetSkul1(bool _skul) { isGetSkul1 = _skul; }
	void setIsGetSkul2(bool _skul) { isGetSkul2 = _skul; }
	void setIsGetSkul3(bool _skul) { isGetSkul3 = _skul; }
	void setIsGetSkul4(bool _skul) { isGetSkul4 = _skul; }
	void setIsFlowerDead(bool _isDead) { isFlowerDead = _isDead; }
	void setIsPotion(bool _potion) { isPotion = _potion; }
	void setIsKey(bool _key) { isKey = _key; }
	void setIsLever(bool _lever) { isLever = _lever; }
	void setIsFlowerpot(bool _flowerpot) { isFlowerpot = _flowerpot; }
};
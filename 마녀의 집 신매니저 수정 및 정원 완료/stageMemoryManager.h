#pragma once
class stageMemoryManager : public singletonBase<stageMemoryManager>
{
private:
	bool isCandle;
	bool isVase;
	bool isBearPickUp;
	bool isBearPut;
	bool isBearComing;
	bool isBox;
public:
	stageMemoryManager();
	~stageMemoryManager();

	bool getIsCandle() { return isCandle; }
	bool getIsVase() { return isVase; }
	bool getIsBearPickUp() { return isBearPickUp; }
	bool getIsBearPut() { return isBearPut; }
	bool getIsBearComing() { return isBearComing; }
	bool getIsBox() { return isBox; }

	void setIsCandle(bool IsCandle) { isCandle = IsCandle; }
	void setIsVase(bool IsVase) { isVase = IsVase; }
	void setIsBearPickUp(bool IsBearPickUp) { isBearPickUp = IsBearPickUp; }
	void setIsBearPut(bool IsBearPut) { isBearPut = IsBearPut; }
	void setIsBearComing(bool IsBearComing) { isBearComing = IsBearComing; }
	void setIsBox(bool IsBox) { isBox = IsBox; }
};
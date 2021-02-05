#pragma once
#include "fifthFloorStage.h"

class gardenToBoss_5f : public fifthFloorStage
{
private:
	enum TRIGGER
	{
		BOOK = 21 + 9 * TILEX,
		DOORTOBOSS = 21 + 6 * TILEX,
		DOORTOGARDEN = 21 + 13 * TILEX,
		NEKO = 20 + 11 * TILEX
	};

public:
	gardenToBoss_5f() {};
	~gardenToBoss_5f() {};

	HRESULT init();
	void release();
	void update();
	void render();
	void Collision();
	void load();

	void setTrigger();

	// 다이얼로그용
	bool _isStopToRead;
	int _string_count;
	vector<string> _vScript;
	void renderText();
	int _count_dia;
	int _i_word;
};

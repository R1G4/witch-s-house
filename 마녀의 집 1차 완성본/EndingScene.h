#pragma once
#include "gameNode.h"
#include "Player.h"
class EndingScene :
	public gameNode
{
private:
	vector<wstring> _vEndingScript;
	Image* _backGround;
	Player* _player;
	int _timeCount;
	float alpha;
	int i;
public:
	EndingScene();
	~EndingScene();
	HRESULT init();
	void release();
	void update();
	void render();
	void setScript();
	wstring stringToWstring(string src)
	{
		USES_CONVERSION;
		return A2W(src.c_str());
	}
};


#pragma once
#include "gameNode.h"

class continueScene : public gameNode
{
	enum SAVE
	{
		FIRST = 0,
		SECOND,
		THIRD,
		FOURTH,
		FIFTH
	};

	struct tagText
	{
		int count;
		int level;
		float x;
		float y;
		wstring location;
	};

	SAVE _selectedLoad;			//해당 컨텐츠
	vector<tagText> _vText;
	FloatRect _rcSelected;		//선택된 메뉴(컨텐츠)를 렉트로 보여줌
	float _rcAlpha;				//렉트 투명도
	float _rcAlphaChange;		//렉트 조절용
	bool _isClick;				//선택키(Z)를 눌렀는지?
public:
	continueScene();
	~continueScene();

	virtual HRESULT init();
	virtual void release();
	void loadInfo();
	virtual void update();
	virtual void rcAlphaChange();
	virtual void render();
};


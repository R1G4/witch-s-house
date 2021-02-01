#pragma once
#include "gameNode.h"

class menu : public gameNode
{
	enum CONTENTS
	{
		NEW = 0,
		CONTINUE,
		OPTION,
		END
	};

	struct tagText
	{
		wstring text;
		float x;
		float y;
		float size;
		bool isSelected;
	};

	map<CONTENTS, tagText> _mText;	//컨텐츠 정보 맵에 담아둠
	CONTENTS _contents;			//해당 컨텐츠
	FloatRect _rcSelected;		//선택된 메뉴(컨텐츠)를 렉트로 보여줌
	float _rcAlpha;				//렉트 투명도
	float _rcAlphaChange;		//렉트 조절용
	float _sceneAlpha;			//씬 투명도
	bool _isClick;				//선택키(Z)를 눌렀는지?
	float _volume;
public:

	menu();
	~menu();

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void rcAlphaChange();
	bool sceneAlphaChange();
	virtual void render();
};
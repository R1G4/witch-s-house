#pragma once
#include "gameNode.h"
#include "settings.h"

class inMenu : public gameNode
{

private:
	enum CONTENTS
	{
		ITEM = 0,
		LOAD, 
		SETTINGS
	};

	struct tagTextInfo
	{
		FloatRect rc;
		wstring text;
		Vector2 vec;
	};

	settings* _settings;
	CONTENTS _contents;			//해당 컨텐츠
	FloatRect _rcSelected;		//선택된 메뉴(컨텐츠)를 렉트로 보여줌
	float _rcAlpha;				//렉트 투명도
	float _rcAlphaChange;		//렉트 조절용
	float _sceneAlpha;			//씬 투명도
	bool _isConnecting;			//선택키(스페이스바)를 눌러서 다른 컨텐츠로 접속 했는지?
	bool _isOpen;

	map<CONTENTS, tagTextInfo> _mText;	//컨텐츠 정보 map에 담아둔다.
public:
	inMenu();
	~inMenu();

	virtual HRESULT init();
	virtual void release();
	virtual void render();
	void selectedContents();
	void connect();
	void rcAlphaChange();
	bool openInMenu();
};


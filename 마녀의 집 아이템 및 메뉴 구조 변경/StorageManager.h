#pragma once
#include "singletonBase.h"

class StorageManager : public singletonBase<StorageManager>
{
	enum STORAGE
	{
		FIRST = 0,
		SECOND,
		THIRD,
		FOURTH,
		FIFTH
	};

	enum VIEW
	{
		SAVE = 0,
		LOAD
	};

	struct tagText
	{
		int count;
		int level;
		float x;
		float y;
		wstring location;
	};

	STORAGE _selectedStorage;	//해당 컨텐츠
	VIEW _view;
	vector<tagText> _vText;
	FloatRect _rcSelected;		//선택된 메뉴(컨텐츠)를 렉트로 보여줌
	float _rcAlpha;				//렉트 투명도
	float _rcAlphaChange;		//렉트 조절용
	bool _isClick;				//선택키(Z)를 눌렀는지?

	protected:
		bool saveData();
		bool loadData();
		void storageOpen();
		void loadTitle();
		virtual void rcAlphaChange();
public:
	StorageManager();
	~StorageManager();

	HRESULT init();
	MENUSTATE saveView();
	MENUSTATE loadView();
	virtual void render();
};


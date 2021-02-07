#pragma once
#include "singletonBase.h"
enum  STAGE:int
{
	OPENING,
	FIRSTSTAGE,
	SECONDSTAGE,
	THIRDSTAGE,
	FOURTHSTAGE,
	FINALSTAGE
};
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
	STAGE _saveStage;//세이브할 스테이지
	const char* fileName;//파일이름 
	bool _isSave;
	bool _toggle;
	int save_enum;
	string save_s_enum;
	vector<string>_vFileSlot;
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
	void setStage(STAGE stage) { _saveStage = stage; }
	bool getToggle() { return _toggle; }
//	void setToggle(bool toggle) { _toggle = toggle; }
	wstring stringToWstring(string src)
	{
		USES_CONVERSION;
		return A2W(src.c_str());
	}
};


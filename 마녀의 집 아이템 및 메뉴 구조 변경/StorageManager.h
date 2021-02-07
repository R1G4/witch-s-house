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

	STORAGE _selectedStorage;	//�ش� ������
	VIEW _view;
	vector<tagText> _vText;
	FloatRect _rcSelected;		//���õ� �޴�(������)�� ��Ʈ�� ������
	float _rcAlpha;				//��Ʈ ����
	float _rcAlphaChange;		//��Ʈ ������
	bool _isClick;				//����Ű(Z)�� ��������?
	STAGE _saveStage;//���̺��� ��������
	const char* fileName;//�����̸� 
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


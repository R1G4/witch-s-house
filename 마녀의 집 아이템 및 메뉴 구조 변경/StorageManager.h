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

	STORAGE _selectedStorage;	//�ش� ������
	VIEW _view;
	vector<tagText> _vText;
	FloatRect _rcSelected;		//���õ� �޴�(������)�� ��Ʈ�� ������
	float _rcAlpha;				//��Ʈ ����
	float _rcAlphaChange;		//��Ʈ ������
	bool _isClick;				//����Ű(Z)�� ��������?

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


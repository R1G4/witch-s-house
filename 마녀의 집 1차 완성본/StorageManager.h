#pragma once
#include "singletonBase.h"
#include<string>

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
	bool _isOpen;
	int save_enum;
	string save_s_enum;
	vector<string>_vFileSlot;
	wstring _divisionText;
protected:

	bool saveData();
	bool loadData();
	void storageOpen();
	void loadTitle();
	virtual void rcAlphaChange();
		
	float _player_x;
	float _player_y;
	CHRDIRECTION _player_direction;

	int _save_x;
	int _save_y;

	string _load_file_name;
	string _save_s_x;
	string _save_s_y;

	string _load_x;
	string _load_y;
	string _load_string_stage;
	string _load_string_position;

	int _load_x_i;
	int _load_y_i;

	string _save_obj[100];
	int _load_obj[100];

	LOCATION5F _real_location;

	string _stage;//��ȯ�� �������� �� �̸�
	int _playerX, _playerY;//�÷��̾� ��ǥ
	CHRDIRECTION _playerDirection;//�÷��̾� ����
public:
	StorageManager();
	~StorageManager();

	HRESULT init();
	MENUSTATE saveView();
	MENUSTATE loadView();
	virtual void render();
	void setStage(STAGE stage) { _saveStage = stage; }
	bool getIsOpen() { return _isOpen; }
	wstring stringToWstring(string src)
	{
		USES_CONVERSION;
		return A2W(src.c_str());
	}

	void setPlayerX(float x) { _player_x = x; }
	void setPlayerY(float y) { _player_y = y; }
	void setPlayerDirection(CHRDIRECTION direction) { _player_direction = direction; }
	void setFifthFloor(LOCATION5F location) { _real_location = location; }

	int getPlayerX() { return _player_x; }
	int getPlayerY() { return _player_y; }
	string getPlayerDirection() { return _load_string_position; }
	LOCATION5F getFifthFloor() { return _real_location; }
	const char* getFileName() {return fileName;}
};


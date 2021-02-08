#pragma once
#include "gameNode.h"
#include "settings.h"

class menu : public gameNode
{
	enum CONTENTS
	{
		NEW = 0,
		CONTINUE,
		MAPTOOL,
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

	map<CONTENTS, tagText> _mText;	//������ ���� �ʿ� ��Ƶ�
	CONTENTS _contents;			//�ش� ������
	settings* _settings;		//settings Ŭ���� ����
	FloatRect _rcSelected;		//���õ� �޴�(������)�� ��Ʈ�� ������
	float _rcAlpha;				//��Ʈ ����
	float _rcAlphaChange;		//��Ʈ ������
	float _sceneAlpha;			//�� ����
	bool _isClick;				//����Ű(Z)�� ��������?
	float _volume;
public:

	menu();
	~menu();

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	void openMenu();
	void contentsCheck(string key);
	virtual void rcAlphaChange();
	bool sceneAlphaChange();
	virtual void render();
	bool getMenuisRunning() { return _isClick; }
};
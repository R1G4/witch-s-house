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
	CONTENTS _contents;			//�ش� ������
	FloatRect _rcSelected;		//���õ� �޴�(������)�� ��Ʈ�� ������
	float _rcAlpha;				//��Ʈ ����
	float _rcAlphaChange;		//��Ʈ ������
	float _sceneAlpha;			//�� ����
	bool _isConnecting;			//����Ű(�����̽���)�� ������ �ٸ� �������� ���� �ߴ���?
	bool _isOpen;

	map<CONTENTS, tagTextInfo> _mText;	//������ ���� map�� ��Ƶд�.
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


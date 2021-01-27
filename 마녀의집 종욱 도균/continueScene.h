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

	SAVE _selectedLoad;			//�ش� ������
	vector<tagText> _vText;
	FloatRect _rcSelected;		//���õ� �޴�(������)�� ��Ʈ�� ������
	float _rcAlpha;				//��Ʈ ����
	float _rcAlphaChange;		//��Ʈ ������
	bool _isClick;				//����Ű(Z)�� ��������?
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


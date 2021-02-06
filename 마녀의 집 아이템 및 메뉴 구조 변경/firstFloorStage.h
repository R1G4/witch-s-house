#pragma once
#include "gameNode.h"
#include "bear.h"

class  Player;
class firstFloorStage : public gameNode
{
protected:
	enum MISSION
	{
		FAIL,
		SUCCESS
	};
	//������ �̹���(�÷��̾�, ���ʹ�, ������Ʈ) Ÿ��
	vector<tagFrameTile> _vFrameTile;
	//��Ÿ��
	tagTile _tiles[TILEX*TILEY];
	//�ɹ���
	Vector2 camera;

	Player* _player;
	bear* _bear;
	float _sceneAlpha;			//�� ����
	int _frameInterval;			//������ �ٲ�� ����
	int _delay;					//������(Ư�� Ʈ���� ���� �� ��ĩ �ϴ� ?)
	bool isTrigger;
	bool _isBlood;

public:
	firstFloorStage();
	~firstFloorStage();

	HRESULT init();
	void release();
	void update();
	void cameraUpdate();
	void render();
	void getFrameTile();
	void setFrameIndex();			// ��ġ�� ������ �̹����� �ε��� ���� 
	void tileCollision(int i, int j);
	void sceneChange(string name, CHRDIRECTION _chrdirection, LOCATION _location);
	void setAlpha();
	void addresslink(Player* player) { _player = player; }

	//FORM

protected:
	enum FROM_DIRECTION
	{
		LEFT,
		RIGHT
	};
	FROM_DIRECTION _fromSelected;

private:
	bool _isForm;
	wstring FormInfo[2];
	float _rcAlpha;			//��Ʈ ���İ�(����)
	float _rcAlphaChange;	//��Ʈ ���İ�(����) ������
	FloatRect _rcSelected;	//���õ� �ڽ��� ��Ʈ�� ������

public:
	void rcAlphaChange();
	void FormRender();
	//bool SelectionForm();

	//�ؽ��� �ִ� ���ÿ� �� ����
	bool SelectionForm(wstring leftText, wstring rightText);

	//�ؽ�Ʈ �ֱ�
	void setForm(wstring leftText, wstring rightText)
	{
		FormInfo[LEFT] = leftText;
		FormInfo[RIGHT] = rightText;
	};

	////�� ����
	//void startForm()	{_isForm = true;};

	////�� ����
	//void stopForm()		{_isForm = false;};
};


#pragma once
#include "gameNode.h"
#include "bear.h"
#include "DeadManager.h"

class  Player;
class firstFloorStage : public gameNode
{
	//���� �������� ������ ���� ������ protected�� ����
protected:
	//�������� ���� ����
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

	vector<string> _vScript;
	Player* _player;
	bear* _bear;
	DeadManager* _dead;
	float _sceneAlpha;			//�� ����
	int _frameInterval;			//������ �ٲ�� ����
	int _delay;					//������(Ư�� Ʈ���� ���� �� ��ĩ �ϴ� ?)
	bool isTrigger;
	bool _isBlood;
	float _light;
	//A* ���� ����
	vector<astarTile*>			_vTotalList;
	vector<astarTile*>::iterator _viTotalList;

	vector<astarTile*>			_vOpenList;
	vector<astarTile*>::iterator _viOpenList;

	vector<astarTile*>			_vCloseList;
	vector<astarTile*>::iterator _viCloseList;

	astarTile* _playerTile;					// �÷��̾� Ÿ��
	astarTile* _enemyTile;					// ���ʹ� Ÿ��
	astarTile* _objTile[TILEX*TILEY];		// ������Ʈ Ÿ��
	astarTile* _currentTile;				// ���� ���ʹ� Ÿ��

	bool _setTile;
	int _follow_count;	// �����ð�
	int MaxIndex;		// ����Ұ� ������Ʈ Ÿ���� ����
	int _numCount;		// ���° Ÿ������
	bool _start;		// ������ ��
	bool _stop;			// ��ã����		
	int bossLocX;
	int bossLocY;
public:
	firstFloorStage();
	~firstFloorStage();

	HRESULT init();
	void release();
	void update();
	void enemyUpdate();
	void cameraUpdate();
	void render();
	void getFrameTile();
	void setFrameIndex();			// ��ġ�� ������ �̹����� �ε��� ���� 
	void tileCollision(int i, int j);
	void sceneChange(string name, CHRDIRECTION _chrdirection = CHRDIREC_UP, LOCATION _location = LOCATION_DEFAULT);
	void setAlpha();
	void addresslink(Player* player) { _player = player; }

	//�� ���� ����
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
	void playerLocation();
	void enemyLocation();
	void objectLocation();
	void setAstarTile();
	void resetEverything();
	vector<astarTile*> addOpenList(astarTile * currentTile);
	void pathFinder(astarTile * currentTile);
	void autoSound(string key);
	void FormRender();

	//�ؽ��� �ִ� ���ÿ� �� ����
	bool SelectionForm(wstring leftText, wstring rightText);

	//�ؽ�Ʈ �ֱ�
	void setForm(wstring leftText, wstring rightText)
	{
		FormInfo[LEFT] = leftText;
		FormInfo[RIGHT] = rightText;
	};
};


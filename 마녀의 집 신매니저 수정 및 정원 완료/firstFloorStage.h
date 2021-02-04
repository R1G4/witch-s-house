#pragma once
#include "gameNode.h"

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
	float _sceneAlpha;			//�� ����
	int _frameInterval;			//������ �ٲ�� ����
	int _delay;					//������(Ư�� Ʈ���� ���� �� ��ĩ �ϴ� ?)
	bool isTrigger;

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
	void addresslink(Player* player) { _player = player; }
};


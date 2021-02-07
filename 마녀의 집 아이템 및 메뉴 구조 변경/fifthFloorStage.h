#pragma once
#include "gameNode.h"
#include "skul.h"
#include "DeadManager.h"

class Player;

class fifthFloorStage : public gameNode
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

	bool _isChangeScene;
	bool _isDead;
	bool _isMove;

	Player* _player;
	skul* _skul;
	DeadManager* _dead;

	int _frameInterval;
	bool isTrigger;
	bool _isSkulAppeal;
	bool _flowerDead;

	float _sceneAlpha;			//�� ����

	float _rcAlpha;				//����â ����
	float _rcAlphaChange;

	bool _isStopToRead;			// ���̾�α׿�
	vector<string> _vScript;

	LOCATION5F _real_location1;
public:
	fifthFloorStage() {};
	~fifthFloorStage() {};

	HRESULT init();
	void release();
	void update();
	void cameraUpdate();
	void render();
	void getFrameTile();
	void setFrameIndex();			// ��ġ�� ������ �̹����� �ε��� ���� 
	void tileCollision();
	void rcAlphaChange();
	void sceneChange(string name);
	void sceneChange(string name, CHRDIRECTION _chrdirection, LOCATION _location);
	void addresslink(Player* player) { _player = player; }

	wstring stringToWstring(string src)
	{
		USES_CONVERSION;
		return A2W(src.c_str());
	}
};


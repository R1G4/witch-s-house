#pragma once
#include "gameNode.h"
class mapTool :
	public gameNode
{
private:
	bool tabOpen;
	Image* _backGround;
	vector<Image*>_vSampleMap;
	tagButton Save;
	tagButton Load;
	tagButton terrain;
	tagButton Object;
	tagButton Erase;
	tagButton Open;
	tagButton Close;
	tagButton Collider;
	tagButton Player;
	tagButton Enemy;
	tagButton FrameObj;
	tagButton setTri;
	tagButton setCor;
	tagButton prevArrow;//desc: ������ �̹��� ��ư �߰� date: 2021/1/29 by pju
	tagButton nextArrow;
	tagButton BackGround;

	//���� Ÿ��
	tagCurrentTile _currentTile;
	//����Ÿ��
	tagSampleTile _sampleTile[SAMPLETILEX*SAMPLETILEY];
	//������ �̹���(�÷��̾�, ���ʹ�, ������Ʈ) Ÿ��
	vector<tagFrameTile> _vFrameTile;
	
	bool _leftButtonDown;
	int _y_rect_num;		// y�� ��Ʈ������ ��ȯ��Ű�� ���� ����?		
	int _frameSelected;		// ���� ������ ������ �ε����� ������ ���� ��ɿ��� ���ȴ�.
	int _frameInterval;		// �������� �ε��� ������ ����
	int _objSelected;		// ���� ������ ������ �ε����� ������ ���� ��ɿ��� ���ȴ�.
	int backCount;
	int _crtSelect;
	string backName;		// ��׶��� �̹��� ã���� ���̴� ���ڿ�
	string onjName;			// ������Ʈ �̹��� ã���� ���̴� ���ڿ�
	FRAMEATTRIBUTE FrAtt;	// ������ �̹��� �Ӽ� 
	tagTile _tiles[TILEX*TILEY];	//��Ÿ��
	FloatRect sampleSelec;	//�������Ÿ�� ���������� �˷��ִ� ��Ʈ
	FloatRect tileSelec;	//Ÿ�ϸʿ��� ���õ� Ÿ���� �˷���
	FloatRect sampleBack;
	FloatRect MapRC;		//�� ���� ���� �κи� ��½�Ű�� ���� ��Ʈ
	Vector2 camera;
public:
	mapTool();
	~mapTool();

	HRESULT init();
	void release();
	void update();
	void render();

	void setButton();
	void setup();
	void setMap();
	void setCtrl();
	void mapMove();
	void tileSelect();
	void save();
	void load();
	void erase();
	void setObjTile();
	void setFrameTile();
	bool addFrameTile(tagFrameTile _frameTile);
	void getFrameTile();			// Load�� ���� ������ ������ ��ġ
	void setFrameIndex();			// ��ġ�� ������ �̹����� �ε��� ���� 
	void setCorrelation();
	void setTrigger();
	void setBack();
	TERRAIN terrainSelect(int frameX, int frameY);
	OBJECT objSelect(int frameX, int frameY);
};


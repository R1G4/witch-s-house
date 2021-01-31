#pragma once
#include "gameNode.h"

class mapTool :
	public gameNode
{
private:
	bool tabOpen;
	Image* _backGround;
	vector<Image*>_vSampleMap;
//	int TILEX;
//	int TILEY;
	tagButton Save;
	tagButton Load;
	tagButton Prev;
	tagButton Next;
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
	//���� ������Ʈ
	tagSampleObj _sampleObj[SAMPLEOBJECTX*SAMPLEOBJECTY];
	//���� ������ �̹��� ����
	vector<tagSampleFrameInfo> _sampleFrameImg;
	//������ �̹���(�÷��̾�, ���ʹ�, ������Ʈ) Ÿ��
	vector<tagFrameTile> _vFrameTile;

	FRAMEATTRIBUTE FrAtt;
	int _change_num;		// ūƲ�� �ѱ�� ���� ����
	int _realNum;			// �� ������Ʈ�� ������ȣ �ο��� ���� ����
	int _y_rect_num;		// y�� ��Ʈ������ ��ȯ��Ű�� ���� ����?		
	int _frameSelected;		// ���� ������ ������ �ε����� ������ ���� ��ɿ��� ���ȴ�.
	int _frameInterval;		// �������� �ε��� ������ ����
	bool _change_number;	// Ŭ���� Ʋ�� �ѹ� ���� �ѱ������ �Ұ�

	//��Ÿ��
	tagTile _tiles[TILEX*TILEY];
	int _crtSelect;
	FloatRect sampleSelec;//�������Ÿ�� ���������� �˷��ִ� ��Ʈ
	FloatRect tileSelec;//Ÿ�ϸʿ��� ���õ� Ÿ���� �˷���
	FloatRect sampleBack;
	bool _leftButtonDown;
	//�� ���� ���� �κи� ��½�Ű�� ���� ��Ʈ
	FloatRect MapRC;
	Vector2 camera;
	//bool isterrain;	by pju �̳����� ��ü�ؾ� �ҵ�...
	int backCount;
	string backName;
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
	void previous();
	void next();
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


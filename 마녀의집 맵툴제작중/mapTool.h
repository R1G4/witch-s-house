#pragma once
#include "gameNode.h"
#define SAMPLETILESIZE 48
#define TILESIZE 48
#define TILEX 30
#define TILEY 30
#define TILESIZEX TILEX*TILESIZE
#define TILESIZEY TILEY*TILESIZE
#define SAMPLETILEX 7
#define SAMPLETILEY 2
#define SAMPLEOBJECTX 2 // �׸���ȯ�� ��ȯ �ʿ�
#define SAMPLEOBJECTY 3 // �׸���ȯ�� ��ȯ �ʿ�


enum CTRL
{
	CTRL_SAVE,			//���̺� ��ư �ν�
	CTRL_LOAD,			//�ε� ��ư �ν�
	CTRL_TERRAINDRAW,	//���� ��ư
	CTRL_OBJDRAW,		//������Ʈ ��ư
	CTRL_ERASER,		//���찳 ��ư
	CTRL_PREV,
	CTRL_NEXT,
	CTRL_END,
	CTRL_OPEN,			//Ÿ�ϸ���Ʈ ����
	CTRL_CLOSE,			//Ÿ�ϸ���Ʈ �ݱ�
	CTRL_COLLIDER, //�浹 Ÿ�� �����ϱ�
	CTRL_SETFRAMETILE
};
enum FRAMEATTRIBUTE
{
	PLAYER,ENEMY,OBJ
};
enum TERRAIN
{
	TR_GRASS, TR_FLOOR, TR_STONE, TR_WATER
};
enum OBJECT
{
	OBJ_UNMOVE, OBJ_PUSH, OBJ_LOOK, OBJ_CORELATION, OBJ_NONE
};
struct tagTile
{
	TERRAIN terrain;		//����
	OBJECT obj;				//������Ʈ
	FloatRect rc;				//��Ʈ
	int terrainFrameX;		//Ÿ�� ��ȣ(�ִϸ��̼� ������ ��ȣ �����ϸ��)
	int terrainFrameY;
	int objFrameX;			//������Ʈ ��ȣ (���� ����)
	int objFrameY;
	bool isMapOn;
	bool isCollider;
};
struct tagSampleTile
{
	FloatRect rcTile;
	int terrainFrameX;
	int terrainFrameY;
};
struct tagSampleObj
{
	FloatRect rc;
	int objFrameX;
	int objFrameY;
	int realNum;	// �� Ÿ���� ���� �ο���ȣ
};
struct FrameTile
{
	Image* img;
	FloatRect rc;
	int indexX;
	int indexY;
};
struct tagCurrentTile
{
	int x, y;
};
struct tagButton
{
	Image* img;
	FloatRect frc;
};
struct currentImgTile
{
	Image* img;
};
class mapTool :
	public gameNode
{
private:
	bool tabOpen;
	D2DRenderer* d2d;
	vector<Image*>_vSampleMap;

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
	
	//���� Ÿ��
	tagCurrentTile _currentTile;
	//����Ÿ��
	tagSampleTile _sampleTile[SAMPLETILEX*SAMPLETILEY];
	//���� ������Ʈ
	tagSampleObj _sampleObj[SAMPLEOBJECTX*SAMPLEOBJECTY];
	//�÷��̾� Ÿ��
	FrameTile _player;
	//���ʹ� Ÿ��
	FrameTile _enemy;
	//������Ʈ Ÿ��
	vector<FrameTile> _vFrameObj;

	FRAMEATTRIBUTE FrAtt;
	int _change_num;		// ūƲ�� �ѱ�� ���� ����
	int _realNum;			// �� ������Ʈ�� ������ȣ �ο��� ���� ����
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
	POINT camera;//��������� �ʿ����
	bool isterrain;
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
	TERRAIN terrainSelect(int frameX, int frameY);
	OBJECT objSelect(int frameX, int frameY);
};


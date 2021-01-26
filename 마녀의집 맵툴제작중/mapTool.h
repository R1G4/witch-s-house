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
enum CTRL
{
	CTRL_SAVE,			//���̺� ��ư �ν�
	CTRL_LOAD,			//�ε� ��ư �ν�
	CTRL_TERRAINDRAW,	//���� ��ư
	CTRL_OBJDRAW,		//������Ʈ ��ư
	CTRL_ERASER,		//���찳 ��ư
	CTRL_PREV,
	CTRL_NEXT,
	CTRL_END
};
enum TERRAIN
{
	TR_GRASS,TR_FLOOR,TR_STONE,TR_WATER
};
enum OBJECT
{
	OBJ_UNMOVE,OBJ_PUSH,OBJ_LOOK,OBJ_CORELATION,OBJ_NONE
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

};
struct tagSampleTile
{
	FloatRect rcTile;
	int terrainFrameX;
	int terrainFrameY;
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
class mapTool :
	public gameNode
{
private:
	D2DRenderer* d2d;
	vector<Image*>_vSampleMap;
	tagButton Save;
	tagButton Load;
	tagButton Prev;
	tagButton Next;
	tagButton terrain;
	tagButton Object;
	tagButton Erase;
	tagCurrentTile _currentTile;
	tagSampleTile _sampleTile[SAMPLETILEX*SAMPLETILEY];
	tagTile _tiles[TILEX*TILEY];
	int _crtSelect;
	FloatRect sampleSelec;
	bool _leftButtonDown;
	FloatRect MapRC;
	POINT camera;
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
	void save();
	void load();
	void erase();
	TERRAIN terrainSelect(int frameX,int frameY);
	OBJECT objSelect(int frameX, int frameY);
};


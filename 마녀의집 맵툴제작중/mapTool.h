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
	CTRL_SAVE,			//세이브 버튼 인식
	CTRL_LOAD,			//로드 버튼 인식
	CTRL_TERRAINDRAW,	//지형 버튼
	CTRL_OBJDRAW,		//오브젝트 버튼
	CTRL_ERASER,		//지우개 버튼
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
	TERRAIN terrain;		//지형
	OBJECT obj;				//오브젝트
	FloatRect rc;				//렉트
	int terrainFrameX;		//타일 번호(애니메이션 프레임 번호 생각하면됨)
	int terrainFrameY;
	int objFrameX;			//오브젝트 번호 (위와 같다)
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


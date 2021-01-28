#pragma once
#include "gameNode.h"
#define SAMPLETILESIZE 48

#define SAMPLEOBJTILESIZEX 300
#define SAMPLEOBJTILESIZEY 200

#define TILESIZE 48
#define TILEX 40
#define TILEY 40
#define TILESIZEX TILEX*TILESIZE
#define TILESIZEY TILEY*TILESIZE
#define SAMPLETILEX 7
#define SAMPLETILEY 2
#define SAMPLEOBJECTX 2 // 그림변환시 변환 필요
#define SAMPLEOBJECTY 3 // 그림변환시 변환 필요


enum CTRL
{
	CTRL_SAVE,			//세이브 버튼 인식
	CTRL_LOAD,			//로드 버튼 인식
	CTRL_TERRAINDRAW,	//지형 버튼
	CTRL_OBJDRAW,		//오브젝트 버튼
	CTRL_ERASER,		//지우개 버튼
	CTRL_PREV,
	CTRL_NEXT,
	CTRL_END,
	CTRL_OPEN,			//타일리스트 열기
	CTRL_CLOSE,			//타일리스트 닫기
	CTRL_COLLIDER, //충돌 타일 설정하기
	CTRL_SETFRAMETILE,
	CTRL_SETTRIGGER,
	CTRL_SETCORRELATION
};
enum FRAMEATTRIBUTE
{
	PLAYER,ENEMY,OBJ
};
enum TERRAIN
{
	TR_GRASS, TR_FLOOR, TR_STONE, TR_TRIGGER
};
enum OBJECT
{
	OBJ_PUSH, OBJ_LOOK, OBJ_CORELATION, OBJ_NONE
};
struct tagTile
{
	TERRAIN terrain;		//지형
	OBJECT obj;				//오브젝트
	FloatRect rc;			//렉트
	string frameKeyName;	//타일에 배치된 이미지가 프레임 이미지일 경우 키값을 저장한다.
	int terrainFrameX;		//타일 번호(애니메이션 프레임 번호 생각하면됨)
	int terrainFrameY;
	int objFrameX;			//오브젝트 번호 (위와 같다)
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
	int realNum;	// 각 타일의 실제 부여번호
};
struct tagSampleFrameInfo
{
	string keyName;			//키값(이미지)
	FloatRect rc;			//렉트
	FRAMEATTRIBUTE kinds;	//프레임 이미지 종류
};
struct tagFrameTile
{
	Image* img;				//키값에서 받은 이미지
	FloatRect rc;			//렉트
	FRAMEATTRIBUTE kinds;	//프레임 이미지 종류
	int frameX;				//프레임 x축 인덱스
	int frameY;				//프레임 y축 인덱스
	int indexX;				//배치된 타일의 x축 인덱스
	int indexY;				//배치된 타일의 y축 인덱스
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
	
	//현재 타일
	tagCurrentTile _currentTile;
	//샘플타일
	tagSampleTile _sampleTile[SAMPLETILEX*SAMPLETILEY];
	//샘플 오브젝트
	tagSampleObj _sampleObj[SAMPLEOBJECTX*SAMPLEOBJECTY];
	//샘플 프레임 이미지 정보
	vector<tagSampleFrameInfo> _sampleFrameImg;
	//프레임 이미지(플레이어, 에너미, 오브젝트) 타일
	vector<tagFrameTile> _vFrameTile;

	FRAMEATTRIBUTE FrAtt;
	int _change_num;		// 큰틀을 넘기기 위한 변수
	int _realNum;			// 각 오브젝트의 실제번호 부여를 위한 변수
	int _y_rect_num;		// y축 렉트갯수를 변환시키기 위한 변수?		
	int _frameSelected;		// 샘플 프레임 백터의 인덱스며 이전과 다음 기능에서 사용된다.
	int _frameInterval;		// 프레임의 인덱스 설정의 간격
	bool _change_number;	// 클릭시 틀을 한번 씩만 넘기기위한 불값

	//맵타일
	tagTile _tiles[TILEX*TILEY];
	int _crtSelect;
	FloatRect sampleSelec;//현재샘플타일 셀렉했음을 알려주는 렉트
	FloatRect tileSelec;//타일맵에서 선택될 타일을 알려줌
	FloatRect sampleBack;
	bool _leftButtonDown;
	//맵 툴의 일정 부분만 출력시키기 위한 렉트
	FloatRect MapRC;
	POINT camera;//만들었지만 필요없음
	//bool isterrain;	by pju 이넘으로 대체해야 할듯...
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
	void setSampleFrame();			//샘플 프레임 이미지 정보 초기화
	void setFrameTile();		
	bool addFrameTile(tagFrameTile _frameTile);
	void getFrameTile();			// Load후 실제 프레임 실제로 배치
	void setFrameIndex();			// 배치된 프레임 이미지의 인덱스 설정 
	void setCorrelation();
	void setTrigger();
	TERRAIN terrainSelect(int frameX, int frameY);
	OBJECT objSelect(int frameX, int frameY);
};


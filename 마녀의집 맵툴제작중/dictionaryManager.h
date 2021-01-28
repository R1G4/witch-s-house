#pragma once
#include "singletonBase.h"

//이미지 필요하므로 전방선언
class Image;

//타일 관련 정보를 전역변수로!
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
	PLAYER, ENEMY, OBJ
};
enum TERRAIN
{
	TR_CORELATION, TR_GRASS, TR_FLOOR, TR_STONE, TR_TRIGGER
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
	POINT camera;
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

//토탈 프레임 이미지는 자주 사용하므로 싱글톤화 한다.
class dictionaryManager : public singletonBase<dictionaryManager>
{
	vector<tagSampleFrameInfo> _sampleFrameImg;	

	public:
		dictionaryManager();
		~dictionaryManager();

	//전체 백터 가져오기
	vector<tagSampleFrameInfo> getTotalFrameImg() { return _sampleFrameImg; }

	//add방식 2가지(사실상 Setter)
	void addTotalFrameImg(string keyName, FRAMEATTRIBUTE kinds) 
	{ 
		tagSampleFrameInfo temp;
		temp.keyName = keyName;
		temp.kinds = kinds;
		_sampleFrameImg.push_back(temp);
	}
	void addTotalFrameImg(tagSampleFrameInfo Info) { _sampleFrameImg.push_back(Info); }
};


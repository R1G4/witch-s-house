#pragma once
#include "stdafx.h"
//타일 관련 정보를 전역변수로!
#define SAMPLETILESIZE 48

#define SAMPLEOBJTILESIZEX 300
#define SAMPLEOBJTILESIZEY 200

#define TILESIZE 48
#define TILEX 60
#define TILEY 60
#define TILESIZEX TILEX*TILESIZE
#define TILESIZEY TILEY*TILESIZE
#define SAMPLETILEX 7
#define SAMPLETILEY 2
#define SAMPLEOBJECTX 2 // 그림변환시 변환 필요
#define SAMPLEOBJECTY 3 // 그림변환시 변환 필요
#define OBJSIZE 56 // 오브젝트 추가 시 변환 필요



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
	CTRL_SETCORRELATION,
	CTRL_BACKGROUND
};

enum FRAMEATTRIBUTE
{
	NONE,PLAYER, ENEMY, OBJ
};
enum TERRAIN
{
	TR_CORELATION, TR_GRASS, TR_FLOOR, TR_STONE, TR_TRIGGER
};
enum OBJECT
{
	OBJ_PUSH, OBJ_LOOK, OBJ_CORELATION, OBJ_NONE
};
struct getRect
{
	FloatRect rc;
	int x, y;
};
enum CHRDIRECTION
{
	CHRDIREC_DOWN,
	CHRDIREC_LEFT,
	CHRDIREC_RIGHT,
	CHRDIREC_UP
};
struct tagTile
{
	TERRAIN terrain;		//지형
	OBJECT obj;				//오브젝트
	FloatRect rc;			//렉트
	string keyName;	//타일에 배치된 이미지가 프레임 이미지일 경우 키값을 저장한다.
	int terrainFrameX;		//타일 번호(애니메이션 프레임 번호 생각하면됨)
	int terrainFrameY;
	bool isMapOn;
	bool isCollider;
	Vector2 camera;
	FRAMEATTRIBUTE attribute;
	string backGroundName;
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
	FRAMEATTRIBUTE kinds;			//프레임 이미지 종류
	int triggerIndex = 0;			//기본 상태 (0 부터 triggerIndex 까지), 트리거 발동 상태 (triggerIndex 부터 프레임MAX 까지)
	char frameSortDirection;		//프레임 정렬 방향
	bool isTrigger = false;			//트리거가 존재하는 프레임인지?
	bool isTriggerUse = false;		//트리거가 발동상태인지?
	bool isLoop = false;			//트리거가 끝난 후 프레임 0 부터 triggerIndex 까지 루프 할지?
	int interval = 8;				//프레임의 최대 간격
};
struct tagFrameTile
{
	Image* img;				//키값에서 받은 이미지
	FloatRect rc;			//렉트
	string keyName;			//키값(이미지)
	FRAMEATTRIBUTE kinds;	//프레임 이미지 속성
	int frameX;				//프레임 x축 인덱스
	int frameY;				//프레임 y축 인덱스
	int indexX;				//배치된 타일의 x축 인덱스
	int indexY;				//배치된 타일의 y축 인덱스
	bool isTrigger = false;	//트리거 발동 유무(기본 false)
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
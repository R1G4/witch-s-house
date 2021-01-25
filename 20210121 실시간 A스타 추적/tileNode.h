#pragma once
//한 타일의 사이즈 32다 32 X 32
#define TILESIZE 32

//타일 갯수 가로 20개 세로 20개
#define TILEX 20
#define TILEY 20

//640 X 640
#define TILESIZEX TILESIZE * TILEX
#define TILESIZEY TILESIZE * TILEY

//타일셋(샘플타일) 범위
#define SAMPLETILEX 20
#define SAMPLETILEY 9

//타일에 속성을 매겨보세
//enum으로 하면 편하다.. 수업때매 비트필드란걸 보여주는거지

//요런 형태가 비트필드인데 예전엔 메모리 아껴아껴
//극한으로 아껴 쓴다고 많이 했으나.. 요샌 뭐 넘치는게 멤..
#define ATTR_UNMOVE 0x00000001
#define ATTR_FROZEN 0x00000002
#define ATTR_SWAMP  0x00000004
#define ATTR_POISON 0x00000008


enum TERRAIN
{
	TR_CEMENT, TR_DESERT, TR_GRASS, TR_WATER, TR_END
};

enum OBJECT
{
	OBJ_BLOCK1, OBJ_BLOCK3, OBJ_BLOCKS,
	OBJ_FLAG1, OBJ_FLAG2,
	OBJ_TANK1, OBJ_TANK2,
	OBJ_NONE
};

//위치(리스폰, 진영 위치 등등)
enum POS
{
	POS_FLAG1, POS_FLAG2,
	POS_TANK1, POS_TANK2
};

struct tagTile
{
	TERRAIN terrain;		//지형
	OBJECT obj;				//오브젝트
	RECT rc;				//렉트
	int terrainFrameX;		//타일 번호(애니메이션 프레임 번호 생각하면됨)
	int terrainFrameY;
	int objFrameX;			//오브젝트 번호 (위와 같다)
	int objFrameY;
};

//샘플 타일 구조체(팔레트 타일)
struct tagSampleTile
{
	RECT rcTile;
	int terrainFrameX;
	int terrainFrameY;
};


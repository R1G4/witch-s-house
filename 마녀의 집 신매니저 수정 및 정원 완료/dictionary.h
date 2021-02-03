#pragma once
#include "stdafx.h"
//Ÿ�� ���� ������ ����������!
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
#define SAMPLEOBJECTX 2 // �׸���ȯ�� ��ȯ �ʿ�
#define SAMPLEOBJECTY 3 // �׸���ȯ�� ��ȯ �ʿ�
#define OBJSIZE 56 // ������Ʈ �߰� �� ��ȯ �ʿ�



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
	TERRAIN terrain;		//����
	OBJECT obj;				//������Ʈ
	FloatRect rc;			//��Ʈ
	string keyName;	//Ÿ�Ͽ� ��ġ�� �̹����� ������ �̹����� ��� Ű���� �����Ѵ�.
	int terrainFrameX;		//Ÿ�� ��ȣ(�ִϸ��̼� ������ ��ȣ �����ϸ��)
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
	int realNum;	// �� Ÿ���� ���� �ο���ȣ
};
struct tagSampleFrameInfo
{
	FRAMEATTRIBUTE kinds;			//������ �̹��� ����
	int triggerIndex = 0;			//�⺻ ���� (0 ���� triggerIndex ����), Ʈ���� �ߵ� ���� (triggerIndex ���� ������MAX ����)
	char frameSortDirection;		//������ ���� ����
	bool isTrigger = false;			//Ʈ���Ű� �����ϴ� ����������?
	bool isTriggerUse = false;		//Ʈ���Ű� �ߵ���������?
	bool isLoop = false;			//Ʈ���Ű� ���� �� ������ 0 ���� triggerIndex ���� ���� ����?
	int interval = 8;				//�������� �ִ� ����
};
struct tagFrameTile
{
	Image* img;				//Ű������ ���� �̹���
	FloatRect rc;			//��Ʈ
	string keyName;			//Ű��(�̹���)
	FRAMEATTRIBUTE kinds;	//������ �̹��� �Ӽ�
	int frameX;				//������ x�� �ε���
	int frameY;				//������ y�� �ε���
	int indexX;				//��ġ�� Ÿ���� x�� �ε���
	int indexY;				//��ġ�� Ÿ���� y�� �ε���
	bool isTrigger = false;	//Ʈ���� �ߵ� ����(�⺻ false)
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
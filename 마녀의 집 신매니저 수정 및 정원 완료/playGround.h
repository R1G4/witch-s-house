#pragma once
#include "gameNode.h"
#include "mapTool.h"
#include "garDen.h"
#include "garDenUnder.h"
#include "menu.h"
#include "levelChoiceScene.h"
#include "bossStage.h"
#include "bossStage_2.h"
#include "Player.h"
#include "castlefront.h"
#include "entranceFake.h"
#include "entranceTrap.h"
#include "entrance.h"
#include "hallway.h"
#include "bearRoom.h"
#include "boxRoom.h"
#include "scissorsRoom.h"
#include "third.h"
#include "third2.h"
#include "third3.h"
#include "thirdFloorDead.h"
#include "thirdFrogRoom.h"


class playGround : public gameNode
{
private:
	float alpha;
	int _currentFrameX;
	int _currentFrameY;
	int count;
	POINT pt;
	Image* viola;
	POINT camera;
	Player* _player;
	bossStage* _bossStage;
	bossStage_2* _bossStage_2;
	firstFloorStage* _entranceFakeStage;
	firstFloorStage* _entranceTrapStage;
	firstFloorStage* _entranceStage;
	firstFloorStage* _hallwayStage;
	firstFloorStage* _bearRoomStage;
	firstFloorStage* _boxRoomStage;
	firstFloorStage* _scissorsRoomStage;
public:
	playGround();
	~playGround();

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();
	void addAutoImage();
	void addScene();
	void addFrameImg();
	void addressLink();

};


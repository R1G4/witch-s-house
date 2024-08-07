#pragma once
#include "gameNode.h"
#include "mapTool.h"
#include "garDen.h"
#include "garDenUnder.h"
#include "menu.h"
#include "levelChoiceScene.h"
#include "bossStage_1.h"
#include "bossStage_2.h"
#include "bossStage_3.h"
#include "bossStage_4.h"
#include "bossStage_5.h"
#include "Player.h"
#include "castlefront.h"
#include "entranceFake.h"
#include "entranceTrap.h"
#include "entrance.h"
#include "hallway.h"
#include "bearRoom.h"
#include "boxRoom.h"
#include "scissorsRoom.h"
#include "stairs_2F.h"
#include "thirdMain.h"
#include "thirdLibrary.h"
#include "thirdOnewayLoad.h"
#include "thirdFrogOutRoom.h"
#include "thirdSnakeRoom.h"
#include "thirdOnewayDead.h"
#include "thirdSnakeDead.h"
#include "thirdFrogRoom.h"
#include "garden_5f.h"
#include "gardenToBoss_5f.h"
#include "diningRoom_5f.h"
#include "prison_5f.h"
#include "prison_5f_well.h"
#include "prison_5f_skul.h"
#include "playerMenu.h"
#include "inMenu.h"
#include "soundStage.h"
#include "soundStageW.h"
#include "soundStageE1.h"
#include "soundStageE2.h"
#include "soundStageN.h"
#include "soundStageD.h"
#include "EndingScene.h"


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
	bossStage* _bossStage_1;
	bossStage* _bossStage_2;
	bossStage* _bossStage_3;
	bossStage* _bossStage_4;
	bossStage* _bossStage_5;
	firstFloorStage* _entranceFakeStage;
	firstFloorStage* _entranceTrapStage;
	firstFloorStage* _entranceStage;
	firstFloorStage* _hallwayStage;
	firstFloorStage* _bearRoomStage;
	firstFloorStage* _boxRoomStage;
	firstFloorStage* _scissorsRoomStage;

	fifthFloorStage* _garden_5f_Stage;
	fifthFloorStage* _gardenToBoss_5f_Stage;
	fifthFloorStage* _dining_5f_Stage;
	fifthFloorStage* _prison_5f_Stage;
	fifthFloorStage* _prison_well_5f_Stage;
	fifthFloorStage* _prison_skul_5f_Stage;

	//playerMenu* _playerMenu;
	inMenu* _inMenu;
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
	void addSound();
	void addressLink();

};


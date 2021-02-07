#include "stdafx.h"
#include "playGround.h"

playGround::playGround()
{
}


playGround::~playGround()
{
}

//ÃÊ±âÈ­ ÇÔ¼ö
HRESULT playGround::init()
{
	gameNode::init(true);

	//Å¬·¡½º »ý¼º ¹× ÂüÁ¶
	addressLink();
	//¾À Ãß°¡
	addScene();

	addAutoImage();
	addFrameImg();
	_inMenu = new inMenu;
	_inMenu->init();
	//_playerMenu = new playerMenu;
	//_playerMenu->init();
	SCENEMANAGER->addScene("MapToolScene", new mapTool);
		
	//SCENEMANAGER->changeScene("¼º¾Õ");
	//SCENEMANAGER->changeScene("MapToolScene");
	//SCENEMANAGER->changeScene("4ÃþÈ¦");

	SCENEMANAGER->changeScene("stairs_2F");
	//SCENEMANAGER->changeScene("prison_5f", CHRDIREC_RIGHT, LOCATION_DEFAULT);
	//SCENEMANAGER->changeScene("thirdMain");
	//SOUNDMANAGER->play("main");

	

	return S_OK;
}

//¸Þ¸ð¸® ÇØÁ¦
void playGround::release()
{

}

//¿¬»ê
void playGround::update()
{
	gameNode::update();

	//ÇÃ·¹ÀÌ¾î ¸Þ´º°¡ ¿­·ÁÀÖÀ»¶§´Â ¾÷µ¥ÀÌÆ®¸¦ Àá½Ã ¸ØÃá´Ù.
	if (!_inMenu->openInMenu())
	{
		SCENEMANAGER->update();
		FRAMEINFOMANAGER->update();
	}
}

//±×¸®±â Àü¿ë
void playGround::render()
{
	//¹é¹öÆÛ ÃÊ±âÈ­
	D2DRenderer::GetInstance()->BeginRender(D2D1::ColorF::Black);
	{

		SCENEMANAGER->render();
		_inMenu->render();
		//_playerMenu->render();

	}
	//¹é¹öÆÛ¿¡ ±×¸° ³»¿ëµéÀ» È­¸é¿¡ »Ñ·Á¶ó~
	D2DRenderer::GetInstance()->EndRender();

	
}

void playGround::addAutoImage()
{
	wstring backName_w = L"";
	string backName;
	for (int i = 1; i < 94; i++)
	{
		backName = "Image/mapTool/stageMap/0" + to_string(i) + ".png";
		backName_w = L"";
		backName_w.assign(backName.begin(), backName.end());
		IMAGEMANAGER->AddImage("¹è°æ" + to_string(i), backName_w);
	}
	for (int i = 1; i <= OBJSIZE; i++)
	{
		backName = "Image/obj/0" + to_string(i) + ".png";
		backName_w = L"";
		backName_w.assign(backName.begin(), backName.end());
		IMAGEMANAGER->AddImage("obj" + to_string(i), backName_w);
	}
}

void playGround::addScene()
{
	SCENEMANAGER->addScene("½ÃÀÛÈ­¸é", new menu);
	SCENEMANAGER->addScene("·¹º§¼±ÅÃ", new levelChoiceScene);
	SCENEMANAGER->addScene("MapToolScene", new mapTool);
	SCENEMANAGER->addScene("Á¤¿ø", new garDen);
	SCENEMANAGER->addScene("Á¤¿ø¾Æ·¡", new garDenUnder);
	SCENEMANAGER->addScene("¼º¾Õ", new castlefront);

	//º¸½º¾À °ü·Ã
	SCENEMANAGER->addScene("BossStage1", _bossStage_1);
	SCENEMANAGER->addScene("BossStage2", _bossStage_2);
	SCENEMANAGER->addScene("BossStage3", _bossStage_3);
	SCENEMANAGER->addScene("BossStage4", _bossStage_4);
	SCENEMANAGER->addScene("BossStage5", _bossStage_5);
	//1Ãþ
	SCENEMANAGER->addScene("entranceFake", _entranceFakeStage);
	SCENEMANAGER->addScene("entranceTrap", _entranceTrapStage);
	SCENEMANAGER->addScene("entrance", _entranceStage);
	SCENEMANAGER->addScene("hallway", _hallwayStage);
	SCENEMANAGER->addScene("bearRoom", _bearRoomStage);
	SCENEMANAGER->addScene("boxRoom", _boxRoomStage);
	SCENEMANAGER->addScene("scissorsRoom", _scissorsRoomStage);

	//3Ãþ
	SCENEMANAGER->addScene("stairs_2F", new stairs_2F);
	SCENEMANAGER->addScene("thirdMain", new thirdMain);
	SCENEMANAGER->addScene("thirdFrogRoom", new thirdFrogRoom);
	SCENEMANAGER->addScene("thirdLibrary", new thirdLibrary);
	SCENEMANAGER->addScene("thirdOnewayLoad", new thirdOnewayLoad);
	SCENEMANAGER->addScene("thirdFrogOutRoom", new thirdFrogOutRoom);
	SCENEMANAGER->addScene("thirdSnakeRoom", new thirdSnakeRoom);
	SCENEMANAGER->addScene("thirdOnewayDead", new thirdOnewayDead);
	SCENEMANAGER->addScene("thirdSnakeDead", new thirdSnakeDead);

	//4Ãþ
	SCENEMANAGER->addScene("4ÃþÈ¦", new soundStage);
	SCENEMANAGER->addScene("4Ãþ¿ÞÂÊ¹æ", new soundStageW);
	SCENEMANAGER->addScene("4Ãþ¿À¸¥ÂÊ¹æ1", new soundStageE1);
	SCENEMANAGER->addScene("4Ãþ¿À¸¥ÂÊ¹æ2", new soundStageE2);
	SCENEMANAGER->addScene("4ÃþÀ­¹æ", new soundStageN);
	SCENEMANAGER->addScene("4ÃþÇÔÁ¤", new soundStageD);

	//5Ãþ
	SCENEMANAGER->addScene("garden_5f", _garden_5f_Stage);
	SCENEMANAGER->addScene("gardenToBoss_5f", _gardenToBoss_5f_Stage);
	SCENEMANAGER->addScene("diningRoom_5f", _dining_5f_Stage);
	SCENEMANAGER->addScene("prison_5f", _prison_5f_Stage);
	SCENEMANAGER->addScene("prison_well_5f", _prison_well_5f_Stage);
	SCENEMANAGER->addScene("prison_skul_5f", _prison_skul_5f_Stage);

	//¿£µù
	SCENEMANAGER->addScene("Ending", new EndingScene);
}

void playGround::addFrameImg()
{	
	//key, ÀÌ¹ÌÁö °æ·Î(~.png), ÇÁ·¹ÀÓ xÃà, ÇÁ·¹ÀÓ yÃà, ¼Ó¼º(Á¾·ù), ÇÁ·¹ÀÓÀÌ ¹Ù²î´Â °£°Ý?, Æ®¸®°Å À¯¹«, Æ®¸®°Å ½ÃÀÛµÇ´Â ÇÁ·¹ÀÓ ÀÎµ¦½º, ¹Ýº¹ À¯¹« 
	FRAMEINFOMANAGER->AddFrameInfo("ÇÃ·¹ÀÌ¾î", L"Image/tempFrameImg/player.png", 16, 4, PLAYER);
	FRAMEINFOMANAGER->AddFrameInfo("°õ", L"Image/tempFrameImg/°õ.png", 1, 4, ENEMY);
	FRAMEINFOMANAGER->AddFrameInfo("´«±ò", L"Image/tempFrameImg/´«±ò.png", 3, 4, ENEMY);
	FRAMEINFOMANAGER->AddFrameInfo("ÇØ°ñ", L"Image/tempFrameImg/ÇØ°ñ.png", 3, 4, ENEMY);
	FRAMEINFOMANAGER->AddFrameInfo("¾×ÀÚ", L"Image/tempFrameImg/¾×ÀÚ1.png", 1, 4, OBJ, 8, true, 1, false);
	FRAMEINFOMANAGER->AddFrameInfo("²Éº´ÇÁ·¹ÀÓ", L"Image/tempFrameImg/²Éº´ÇÁ·¹ÀÓ.png", 1, 4, OBJ, 10, true, 1, false);
	FRAMEINFOMANAGER->AddFrameInfo("ÄÑÁøÃÊ", L"Image/tempFrameImg/ÄÑÁøÃÊ.png", 3, 1, OBJ);
	FRAMEINFOMANAGER->AddFrameInfo("²¨ÁøÃÊ", L"Image/tempFrameImg/²¨ÁøÃÊ.png", 3, 1, OBJ);
	FRAMEINFOMANAGER->AddFrameInfo("ÄÑÁøÃÊ²¨ÁøÃÊ", L"Image/tempFrameImg/ÄÑÁøÃÊ²¨ÁøÃÊ.png", 6, 1, OBJ, 10, true, 3, false);
	FRAMEINFOMANAGER->AddFrameInfo("¹®1", L"Image/tempFrameImg/¹®1.png", 1, 4, OBJ, 11, true, 1, false);
	FRAMEINFOMANAGER->AddFrameInfo("¹®2", L"Image/tempFrameImg/¹®2.png", 1, 4, OBJ, 11, true, 1, false);
	FRAMEINFOMANAGER->AddFrameInfo("¹®3", L"Image/tempFrameImg/¹®3.png", 1, 4, OBJ, 11, true, 1, false);
	FRAMEINFOMANAGER->AddFrameInfo("»¡°£²É1", L"Image/tempFrameImg/»¡°£²É1.png", 1, 3, OBJ, 15);
	FRAMEINFOMANAGER->AddFrameInfo("»¡°£²É2", L"Image/tempFrameImg/»¡°£²É2.png", 1, 4, OBJ, 15);
	FRAMEINFOMANAGER->AddFrameInfo("»¡°£²É3", L"Image/tempFrameImg/»¡°£²É3.png", 1, 3, OBJ, 15);
	FRAMEINFOMANAGER->AddFrameInfo("»õÀå", L"Image/tempFrameImg/»õÀå.png", 1, 4, OBJ, 5, true, 1, false);
	FRAMEINFOMANAGER->AddFrameInfo("2ÁßÃ¶Ã¢", L"Image/tempFrameImg/2ÁßÃ¶Ã¢.png", 1, 2, OBJ, 10, true, 1, false);
	FRAMEINFOMANAGER->AddFrameInfo("ÇÔÁ¤1", L"Image/tempFrameImg/ÇÔÁ¤1.png", 1, 5, OBJ, 6, true, 1, false);
	FRAMEINFOMANAGER->AddFrameInfo("ÇÏ¾á²É", L"Image/tempFrameImg/ÇÏ¾á²É.png", 1, 4, OBJ, 15);
	FRAMEINFOMANAGER->AddFrameInfo("ÇÏ¾á²ÉÁ×À½", L"Image/tempFrameImg/ÇÏ¾á²ÉÁ×À½.png", 9, 1, OBJ, 15, true, 1, false);
	FRAMEINFOMANAGER->AddFrameInfo("Çª¸¥¹°", L"Image/tempFrameImg/Çª¸¥¹°.png", 6, 1, OBJ);
	FRAMEINFOMANAGER->AddFrameInfo("ºÓÀº¹°", L"Image/tempFrameImg/ºÓÀº¹°.png", 6, 1, OBJ);
	FRAMEINFOMANAGER->AddFrameInfo("Ä®µç¾ÆÀú¾¾", L"Image/tempFrameImg/Ä®µç¾ÆÀú¾¾.png", 6, 1, OBJ, 8, true, 3, false);
	FRAMEINFOMANAGER->AddFrameInfo("¹Ù±¸´Ï°õ", L"Image/tempFrameImg/¹Ù±¸´Ï°õ.png", 4, 1, OBJ, 7, true, 1, false);
	FRAMEINFOMANAGER->AddFrameInfo("°¡À§", L"Image/tempFrameImg/°¡À§.png", 2, 1, OBJ, 5, true, 1, false);
	FRAMEINFOMANAGER->AddFrameInfo("»óÀÚ¾²·¯Áü", L"Image/tempFrameImg/»óÀÚ¾²·¯Áü.png", 1, 4, OBJ, 8, true, 1, false);	
	FRAMEINFOMANAGER->AddFrameInfo("°í¾çÀÌ1", L"Image/tempFrameImg/°í¾çÀÌ1.png", 8, 1, OBJ, 7);
	FRAMEINFOMANAGER->AddFrameInfo("¼Õ¹Ù´Ú", L"Image/tempFrameImg/¼Õ¹Ù´Ú.png", 1, 8, OBJ, 6, true, 1, false);

	//Ä³¸¯ÅÍ ÀÌ¹ÌÁö Ãß°¡
	FRAMEINFOMANAGER->AddFrameInfo("violaIdle", L"Image/violaFrameImg/violaIdle.png", 16, 4, PLAYER);
	FRAMEINFOMANAGER->AddFrameInfo("violaWalk", L"Image/violaFrameImg/violaWalk.png", 16, 4, PLAYER);
	FRAMEINFOMANAGER->AddFrameInfo("violaDash", L"Image/violaFrameImg/violaDash.png", 16, 4, PLAYER);
	//¿¤·» ÀÌ¹ÌÁö Ãß°¡
	FRAMEINFOMANAGER->AddFrameInfo("Elen", L"Image/ElenFrameImg/Elen.png", 8, 4, ENEMY);


	//»ç¸Á ÀÌ¹ÌÁö Ãß°¡
	IMAGEMANAGER->AddImage("gameOver", L"Image/Death/dead.png");
	IMAGEMANAGER->AddFrameImage("bossDead", L"Image/Death/death_boss.png", 3, 1);
	IMAGEMANAGER->AddFrameImage("skullDeath", L"Image/Death/skullDeath.png", 8, 4);
	IMAGEMANAGER->AddFrameImage("clockDeath", L"Image/Death/clockDeath.png", 1, 4);
	IMAGEMANAGER->AddFrameImage("flowerDeath", L"Image/Death/flowerDeath.png", 8, 4);
	IMAGEMANAGER->AddFrameImage("snakeDeath", L"Image/Death/snakeDeath.png", 3, 4);
	IMAGEMANAGER->AddImage("LWall", L"Image/Death/leftWall.png");
	IMAGEMANAGER->AddImage("RWall", L"Image/Death/rightWall.png");
	IMAGEMANAGER->AddFrameImage("bearDeath", L"Image/Death/bearDeath.png", 10, 1);
	IMAGEMANAGER->AddFrameImage("thornDeath", L"Image/Death/thornDeath.png", 5, 1);
	//±×¿Ü UI
	IMAGEMANAGER->AddImage("È­»ìÇ¥", L"Image/mapTool/È­»ìÇ¥.png");
	IMAGEMANAGER->AddImage("È­»ìÇ¥¿ÞÂÊ", L"Image/mapTool/È­»ìÇ¥¿ÞÂÊ.png");
	IMAGEMANAGER->AddImage("È­»ìÇ¥¿À¸¥ÂÊ", L"Image/mapTool/È­»ìÇ¥¿À¸¥ÂÊ.png");
	IMAGEMANAGER->AddImage("1Ãþ½ºÅ¸Æ®", L"Image/mapTool/stageMap/08.png");
	IMAGEMANAGER->AddImage("messe", L"Image/UI/messe.png");
	IMAGEMANAGER->AddImage("status", L"image/UI/status2.png");
	IMAGEMANAGER->AddImage("playerMenu", L"image/UI/bar7.png");
	IMAGEMANAGER->AddImage("menu", L"image/menu.png");
	IMAGEMANAGER->AddImage("bar", L"Image/obj/bar2.png");

	//°¡¸²¸·
	IMAGEMANAGER->AddImage("Back2", L"Image/back2.png");
}

void playGround::addressLink()
{
	_player = new Player;
	_bossStage_1 = new bossStage_1;
	_bossStage_2 = new bossStage_2;
	_bossStage_1->addresslink(_player);
	_bossStage_2->addresslink(_player);
	_bossStage_3 = new bossStage_3;
	_bossStage_3->addresslink(_player);
	_bossStage_4 = new bossStage_4;
	_bossStage_4->addresslink(_player);
	_bossStage_5 = new bossStage_5;
	_bossStage_5->addresslink(_player);

	_entranceFakeStage = new entranceFake;
	_entranceTrapStage = new entranceTrap;
	_entranceStage = new entrance;
	_hallwayStage = new hallway;
	_bearRoomStage = new bearRoom;
	_boxRoomStage = new boxRoom;
	_scissorsRoomStage = new scissorsRoom;
	_entranceFakeStage->addresslink(_player);
	_entranceTrapStage->addresslink(_player);
	_entranceStage->addresslink(_player);
	_hallwayStage->addresslink(_player);
	_bearRoomStage->addresslink(_player);
	_boxRoomStage->addresslink(_player);
	_scissorsRoomStage->addresslink(_player);

	// 5Ãþ ÇÃ·¹ÀÌ¾î ¸µÅ©
	_garden_5f_Stage = new garden_5f;
	_gardenToBoss_5f_Stage = new gardenToBoss_5f;
	_dining_5f_Stage = new diningRoom_5f;
	_prison_5f_Stage = new prison_5f;
	_prison_well_5f_Stage = new prison_5f_well;
	_prison_skul_5f_Stage = new prison_5f_skul;
	_garden_5f_Stage->addresslink(_player);
	_dining_5f_Stage->addresslink(_player);
	_prison_5f_Stage->addresslink(_player);
	_prison_well_5f_Stage->addresslink(_player);
	_prison_skul_5f_Stage->addresslink(_player);
	_gardenToBoss_5f_Stage->addresslink(_player);
}
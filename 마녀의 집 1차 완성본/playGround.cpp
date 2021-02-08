#include "stdafx.h"
#include "playGround.h"

playGround::playGround()
{
}


playGround::~playGround()
{
}

//초기화 함수
HRESULT playGround::init()
{
	gameNode::init(true);

	//클래스 생성 및 참조
	addressLink();
	//씬 추가
	addScene();

	addAutoImage();
	addFrameImg();
	//사운드 추가
	addSound();

	_inMenu = new inMenu;
	_inMenu->init();
	//_playerMenu = new playerMenu;
	//_playerMenu->init();
	SCENEMANAGER->addScene("MapToolScene", new mapTool);
		
	//SCENEMANAGER->changeScene("성앞");
	//SCENEMANAGER->changeScene("MapToolScene");
	//SCENEMANAGER->changeScene("4층홀");
	STORAGEMANAGER->init();

	SCENEMANAGER->changeScene("MapToolScene");
	//SCENEMANAGER->changeScene("stairs_2F");
	//SCENEMANAGER->changeScene("thirdMain",CHRDIREC_DOWN);
	//SOUNDMANAGER->play("main");

	return S_OK;
}

//메모리 해제
void playGround::release()
{

}

//연산
void playGround::update()
{
	gameNode::update();

	//플레이어 메뉴가 열려있을때는 업데이트를 잠시 멈춘다.
	if (!_inMenu->openInMenu())
	{
		SCENEMANAGER->update();
		FRAMEINFOMANAGER->update();
	}
}

//그리기 전용
void playGround::render()
{
	//백버퍼 초기화
	D2DRenderer::GetInstance()->BeginRender(D2D1::ColorF::Black);
	{

		SCENEMANAGER->render();
		_inMenu->render();
		//_playerMenu->render();

	}
	//백버퍼에 그린 내용들을 화면에 뿌려라~
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
		IMAGEMANAGER->AddImage("배경" + to_string(i), backName_w);
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
	SCENEMANAGER->addScene("시작화면", new menu);
	SCENEMANAGER->addScene("레벨선택", new levelChoiceScene);
	SCENEMANAGER->addScene("MapToolScene", new mapTool);
	SCENEMANAGER->addScene("정원", new garDen);
	SCENEMANAGER->addScene("정원아래", new garDenUnder);
	SCENEMANAGER->addScene("성앞", new castlefront);

	//보스씬 관련
	SCENEMANAGER->addScene("BossStage1", _bossStage_1);
	SCENEMANAGER->addScene("BossStage2", _bossStage_2);
	SCENEMANAGER->addScene("BossStage3", _bossStage_3);
	SCENEMANAGER->addScene("BossStage4", _bossStage_4);
	SCENEMANAGER->addScene("BossStage5", _bossStage_5);
	//1층
	SCENEMANAGER->addScene("entranceFake", _entranceFakeStage);
	SCENEMANAGER->addScene("entranceTrap", _entranceTrapStage);
	SCENEMANAGER->addScene("entrance", _entranceStage);
	SCENEMANAGER->addScene("hallway", _hallwayStage);
	SCENEMANAGER->addScene("bearRoom", _bearRoomStage);
	SCENEMANAGER->addScene("boxRoom", _boxRoomStage);
	SCENEMANAGER->addScene("scissorsRoom", _scissorsRoomStage);

	//3층
	SCENEMANAGER->addScene("stairs_2F", new stairs_2F);
	SCENEMANAGER->addScene("thirdMain", new thirdMain);
	SCENEMANAGER->addScene("thirdFrogRoom", new thirdFrogRoom);
	SCENEMANAGER->addScene("thirdLibrary", new thirdLibrary);
	SCENEMANAGER->addScene("thirdOnewayLoad", new thirdOnewayLoad);
	SCENEMANAGER->addScene("thirdFrogOutRoom", new thirdFrogOutRoom);
	SCENEMANAGER->addScene("thirdSnakeRoom", new thirdSnakeRoom);
	SCENEMANAGER->addScene("thirdOnewayDead", new thirdOnewayDead);
	SCENEMANAGER->addScene("thirdSnakeDead", new thirdSnakeDead);

	//4층
	SCENEMANAGER->addScene("4층홀", new soundStage);
	SCENEMANAGER->addScene("4층왼쪽방", new soundStageW);
	SCENEMANAGER->addScene("4층오른쪽방1", new soundStageE1);
	SCENEMANAGER->addScene("4층오른쪽방2", new soundStageE2);
	SCENEMANAGER->addScene("4층윗방", new soundStageN);
	SCENEMANAGER->addScene("4층함정", new soundStageD);

	//5층
	SCENEMANAGER->addScene("garden_5f", _garden_5f_Stage);
	SCENEMANAGER->addScene("gardenToBoss_5f", _gardenToBoss_5f_Stage);
	SCENEMANAGER->addScene("diningRoom_5f", _dining_5f_Stage);
	SCENEMANAGER->addScene("prison_5f", _prison_5f_Stage);
	SCENEMANAGER->addScene("prison_well_5f", _prison_well_5f_Stage);
	SCENEMANAGER->addScene("prison_skul_5f", _prison_skul_5f_Stage);

	//엔딩
	SCENEMANAGER->addScene("Ending", new EndingScene);
}

void playGround::addFrameImg()
{	
	//key, 이미지 경로(~.png), 프레임 x축, 프레임 y축, 속성(종류), 프레임이 바뀌는 간격?, 트리거 유무, 트리거 시작되는 프레임 인덱스, 반복 유무 
	FRAMEINFOMANAGER->AddFrameInfo("플레이어", L"Image/tempFrameImg/player.png", 16, 4, PLAYER);
	FRAMEINFOMANAGER->AddFrameInfo("곰", L"Image/tempFrameImg/곰.png", 1, 4, ENEMY);
	FRAMEINFOMANAGER->AddFrameInfo("눈깔", L"Image/tempFrameImg/눈깔.png", 3, 4, ENEMY);
	FRAMEINFOMANAGER->AddFrameInfo("해골", L"Image/tempFrameImg/해골.png", 3, 4, ENEMY);
	FRAMEINFOMANAGER->AddFrameInfo("액자", L"Image/tempFrameImg/액자1.png", 1, 4, OBJ, 8, true, 1, false);
	FRAMEINFOMANAGER->AddFrameInfo("꽃병프레임", L"Image/tempFrameImg/꽃병프레임.png", 1, 4, OBJ, 10, true, 1, false);
	FRAMEINFOMANAGER->AddFrameInfo("켜진초", L"Image/tempFrameImg/켜진초.png", 3, 1, OBJ);
	FRAMEINFOMANAGER->AddFrameInfo("꺼진초", L"Image/tempFrameImg/꺼진초.png", 3, 1, OBJ);
	FRAMEINFOMANAGER->AddFrameInfo("켜진초꺼진초", L"Image/tempFrameImg/켜진초꺼진초.png", 6, 1, OBJ, 10, true, 3, false);
	FRAMEINFOMANAGER->AddFrameInfo("문1", L"Image/tempFrameImg/문1.png", 1, 4, OBJ, 11, true, 1, false);
	FRAMEINFOMANAGER->AddFrameInfo("문2", L"Image/tempFrameImg/문2.png", 1, 4, OBJ, 11, true, 1, false);
	FRAMEINFOMANAGER->AddFrameInfo("문3", L"Image/tempFrameImg/문3.png", 1, 4, OBJ, 11, true, 1, false);
	FRAMEINFOMANAGER->AddFrameInfo("빨간꽃1", L"Image/tempFrameImg/빨간꽃1.png", 1, 3, OBJ, 15);
	FRAMEINFOMANAGER->AddFrameInfo("빨간꽃2", L"Image/tempFrameImg/빨간꽃2.png", 1, 4, OBJ, 15);
	FRAMEINFOMANAGER->AddFrameInfo("빨간꽃3", L"Image/tempFrameImg/빨간꽃3.png", 1, 3, OBJ, 15);
	FRAMEINFOMANAGER->AddFrameInfo("새장", L"Image/tempFrameImg/새장.png", 1, 4, OBJ, 5, true, 1, false);
	FRAMEINFOMANAGER->AddFrameInfo("2중철창", L"Image/tempFrameImg/2중철창.png", 1, 2, OBJ, 10, true, 1, false);
	FRAMEINFOMANAGER->AddFrameInfo("함정1", L"Image/tempFrameImg/함정1.png", 1, 5, OBJ, 6, true, 1, false);
	FRAMEINFOMANAGER->AddFrameInfo("하얀꽃", L"Image/tempFrameImg/하얀꽃.png", 1, 4, OBJ, 15);
	FRAMEINFOMANAGER->AddFrameInfo("하얀꽃죽음", L"Image/tempFrameImg/하얀꽃죽음.png", 9, 1, OBJ, 15, true, 1, false);
	FRAMEINFOMANAGER->AddFrameInfo("푸른물", L"Image/tempFrameImg/푸른물.png", 6, 1, OBJ);
	FRAMEINFOMANAGER->AddFrameInfo("붉은물", L"Image/tempFrameImg/붉은물.png", 6, 1, OBJ);
	FRAMEINFOMANAGER->AddFrameInfo("칼든아저씨", L"Image/tempFrameImg/칼든아저씨.png", 6, 1, OBJ, 8, true, 3, false);
	FRAMEINFOMANAGER->AddFrameInfo("바구니곰", L"Image/tempFrameImg/바구니곰.png", 4, 1, OBJ, 7, true, 1, false);
	FRAMEINFOMANAGER->AddFrameInfo("가위", L"Image/tempFrameImg/가위.png", 2, 1, OBJ, 5, true, 1, false);
	FRAMEINFOMANAGER->AddFrameInfo("상자쓰러짐", L"Image/tempFrameImg/상자쓰러짐.png", 1, 4, OBJ, 8, true, 1, false);	
	FRAMEINFOMANAGER->AddFrameInfo("고양이1", L"Image/tempFrameImg/고양이1.png", 8, 1, OBJ, 7);
	FRAMEINFOMANAGER->AddFrameInfo("손바닥", L"Image/tempFrameImg/손바닥.png", 1, 8, OBJ, 6, true, 1, false);

	//캐릭터 이미지 추가
	FRAMEINFOMANAGER->AddFrameInfo("violaIdle", L"Image/violaFrameImg/violaIdle.png", 16, 4, PLAYER);
	FRAMEINFOMANAGER->AddFrameInfo("violaWalk", L"Image/violaFrameImg/violaWalk.png", 16, 4, PLAYER);
	FRAMEINFOMANAGER->AddFrameInfo("violaDash", L"Image/violaFrameImg/violaDash.png", 16, 4, PLAYER);
	//엘렌 이미지 추가
	FRAMEINFOMANAGER->AddFrameInfo("Elen", L"Image/ElenFrameImg/Elen.png", 8, 4, ENEMY);


	//사망 이미지 추가
	IMAGEMANAGER->AddImage("gameOver", L"Image/Death/dead.png");
	IMAGEMANAGER->AddFrameImage("bossDead", L"Image/Death/death_boss.png", 3, 1);
	IMAGEMANAGER->AddFrameImage("skullDeath", L"Image/Death/skullDeath.png", 8, 4);
	IMAGEMANAGER->AddFrameImage("clockDeath", L"Image/Death/clockDeath.png", 1, 8);
	IMAGEMANAGER->AddFrameImage("flowerDeath", L"Image/Death/flowerDeath.png", 8, 4);
	IMAGEMANAGER->AddFrameImage("snakeDeath", L"Image/Death/snakeDeath.png", 3, 4);
	IMAGEMANAGER->AddImage("LWall", L"Image/Death/leftWall.png");
	IMAGEMANAGER->AddImage("RWall", L"Image/Death/rightWall.png");
	IMAGEMANAGER->AddFrameImage("bearDeath", L"Image/Death/bearDeath.png", 10, 1);
	IMAGEMANAGER->AddFrameImage("thornDeath", L"Image/Death/thornDeath.png", 5, 1);
	//그외 UI
	IMAGEMANAGER->AddImage("화살표", L"Image/mapTool/화살표.png");
	IMAGEMANAGER->AddImage("화살표왼쪽", L"Image/mapTool/화살표왼쪽.png");
	IMAGEMANAGER->AddImage("화살표오른쪽", L"Image/mapTool/화살표오른쪽.png");
	IMAGEMANAGER->AddImage("1층스타트", L"Image/mapTool/stageMap/08.png");
	IMAGEMANAGER->AddImage("messe", L"Image/UI/messe.png");
	IMAGEMANAGER->AddImage("status", L"image/UI/status2.png");
	IMAGEMANAGER->AddImage("playerMenu", L"image/UI/bar7.png");
	IMAGEMANAGER->AddImage("menu", L"image/menu.png");
	IMAGEMANAGER->AddImage("bar", L"Image/obj/bar2.png");
	IMAGEMANAGER->AddImage("곰들", L"Image/obj/곰들.png");

	//검은색판
	IMAGEMANAGER->AddImage("Back", L"Image/back.png");
	//가림막
	IMAGEMANAGER->AddImage("Back2", L"Image/back2.png");
}

void playGround::addSound()
{
	//가능하다면 중복되되지 않게해주세요^^
	SOUNDMANAGER->addSound("할로윈", "sound/bgm/할로윈 공포 브금.mp3", false, false);
	SOUNDMANAGER->addSound("main", "sound/bgm/main.OGG", false, false);
	SOUNDMANAGER->addSound("firstMap", "sound/bgm/firstMap.OGG", false, true);
	SOUNDMANAGER->addSound("다운받은거1", "sound/bgm/다운받은거1.mp3", false, false);
	SOUNDMANAGER->addSound("다운받은거2", "sound/bgm/다운받은거2.mp3", false, false);
	SOUNDMANAGER->addSound("wind", "sound/bgm/wind.OGG", false, true);
	SOUNDMANAGER->addSound("cursor", "sound/effect/cursor.OGG", false, false);
	SOUNDMANAGER->addSound("click", "sound/effect/click.OGG", false, false);
	SOUNDMANAGER->addSound("openDoarLong", "sound/effect/openDoarLong.OGG", false, false);
	SOUNDMANAGER->addSound("openDoarShort", "sound/effect/openDoarShort.OGG", false, false);
	SOUNDMANAGER->addSound("rockDoar", "sound/effect/rockDoar.OGG", false, false);
	SOUNDMANAGER->addSound("getItem", "sound/effect/getItem.OGG", false, false);
	SOUNDMANAGER->addSound("sword", "sound/effect/sword.OGG", false, false);
	SOUNDMANAGER->addSound("곰짜를때", "sound/effect/곰짜를때.OGG", false, false);
	SOUNDMANAGER->addSound("cat", "sound/effect/cat.OGG", false, false);
	SOUNDMANAGER->addSound("nextPage", "sound/effect/nextPage.OGG", false, false);
	SOUNDMANAGER->addSound("openBook", "sound/effect/openBook.OGG", false, false);
	SOUNDMANAGER->addSound("1층BGM", "sound/bgm/종욱이 맵.ogg", false, true);
	SOUNDMANAGER->addSound("노크", "sound/effect/똑똑똑똑.ogg", false, false);
	SOUNDMANAGER->addSound("사망", "sound/effect/dead.ogg", false, false);
	SOUNDMANAGER->addSound("박스", "sound/effect/downBox.ogg", false, false);
	SOUNDMANAGER->addSound("호로1", "sound/귀신효과음 쓰는사람 없으면 폴더 삭제하기/s038horror.ogg", false, false);
	SOUNDMANAGER->addSound("깨짐1", "sound/effect/mv_Break.ogg", false, false);
	SOUNDMANAGER->addSound("여자", "sound/effect/여자애들목소리.ogg", false, false);
	SOUNDMANAGER->addSound("호로2", "sound/귀신효과음 쓰는사람 없으면 폴더 삭제하기/t019ghost.ogg.ogg", false, false);
	SOUNDMANAGER->addSound("곰등장", "sound/effect/bear.ogg", false, false);
	SOUNDMANAGER->addSound("피1", "sound/effect/피1.ogg", false, false);
	SOUNDMANAGER->addSound("피2", "sound/effect/피2.ogg", false, false);
	SOUNDMANAGER->addSound("여자비웃음", "sound/effect/여자비웃음.ogg", false, false);
	SOUNDMANAGER->addSound("호러3", "sound/귀신효과음 쓰는사람 없으면 폴더 삭제하기/t024horror_a.ogg", false, false);
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

	// 5층 플레이어 링크
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
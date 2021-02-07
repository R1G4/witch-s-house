#include "stdafx.h"
#include "playGround.h"

playGround::playGround()
{
}


playGround::~playGround()
{
}

//�ʱ�ȭ �Լ�
HRESULT playGround::init()
{
	gameNode::init(true);

	//Ŭ���� ���� �� ����
	addressLink();
	//�� �߰�
	addScene();

	addAutoImage();
	addFrameImg();
	_inMenu = new inMenu;
	_inMenu->init();
	//_playerMenu = new playerMenu;
	//_playerMenu->init();
	SCENEMANAGER->addScene("MapToolScene", new mapTool);
		
	//SCENEMANAGER->changeScene("����");
	//SCENEMANAGER->changeScene("MapToolScene");
	//SCENEMANAGER->changeScene("4��Ȧ");

	SCENEMANAGER->changeScene("stairs_2F");
	//SCENEMANAGER->changeScene("prison_5f", CHRDIREC_RIGHT, LOCATION_DEFAULT);
	//SCENEMANAGER->changeScene("thirdMain");
	//SOUNDMANAGER->play("main");

	

	return S_OK;
}

//�޸� ����
void playGround::release()
{

}

//����
void playGround::update()
{
	gameNode::update();

	//�÷��̾� �޴��� ������������ ������Ʈ�� ��� �����.
	if (!_inMenu->openInMenu())
	{
		SCENEMANAGER->update();
		FRAMEINFOMANAGER->update();
	}
}

//�׸��� ����
void playGround::render()
{
	//����� �ʱ�ȭ
	D2DRenderer::GetInstance()->BeginRender(D2D1::ColorF::Black);
	{

		SCENEMANAGER->render();
		_inMenu->render();
		//_playerMenu->render();

	}
	//����ۿ� �׸� ������� ȭ�鿡 �ѷ���~
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
		IMAGEMANAGER->AddImage("���" + to_string(i), backName_w);
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
	SCENEMANAGER->addScene("����ȭ��", new menu);
	SCENEMANAGER->addScene("��������", new levelChoiceScene);
	SCENEMANAGER->addScene("MapToolScene", new mapTool);
	SCENEMANAGER->addScene("����", new garDen);
	SCENEMANAGER->addScene("�����Ʒ�", new garDenUnder);
	SCENEMANAGER->addScene("����", new castlefront);

	//������ ����
	SCENEMANAGER->addScene("BossStage1", _bossStage_1);
	SCENEMANAGER->addScene("BossStage2", _bossStage_2);
	SCENEMANAGER->addScene("BossStage3", _bossStage_3);
	SCENEMANAGER->addScene("BossStage4", _bossStage_4);
	SCENEMANAGER->addScene("BossStage5", _bossStage_5);
	//1��
	SCENEMANAGER->addScene("entranceFake", _entranceFakeStage);
	SCENEMANAGER->addScene("entranceTrap", _entranceTrapStage);
	SCENEMANAGER->addScene("entrance", _entranceStage);
	SCENEMANAGER->addScene("hallway", _hallwayStage);
	SCENEMANAGER->addScene("bearRoom", _bearRoomStage);
	SCENEMANAGER->addScene("boxRoom", _boxRoomStage);
	SCENEMANAGER->addScene("scissorsRoom", _scissorsRoomStage);

	//3��
	SCENEMANAGER->addScene("stairs_2F", new stairs_2F);
	SCENEMANAGER->addScene("thirdMain", new thirdMain);
	SCENEMANAGER->addScene("thirdFrogRoom", new thirdFrogRoom);
	SCENEMANAGER->addScene("thirdLibrary", new thirdLibrary);
	SCENEMANAGER->addScene("thirdOnewayLoad", new thirdOnewayLoad);
	SCENEMANAGER->addScene("thirdFrogOutRoom", new thirdFrogOutRoom);
	SCENEMANAGER->addScene("thirdSnakeRoom", new thirdSnakeRoom);
	SCENEMANAGER->addScene("thirdOnewayDead", new thirdOnewayDead);
	SCENEMANAGER->addScene("thirdSnakeDead", new thirdSnakeDead);

	//4��
	SCENEMANAGER->addScene("4��Ȧ", new soundStage);
	SCENEMANAGER->addScene("4�����ʹ�", new soundStageW);
	SCENEMANAGER->addScene("4�������ʹ�1", new soundStageE1);
	SCENEMANAGER->addScene("4�������ʹ�2", new soundStageE2);
	SCENEMANAGER->addScene("4������", new soundStageN);
	SCENEMANAGER->addScene("4������", new soundStageD);

	//5��
	SCENEMANAGER->addScene("garden_5f", _garden_5f_Stage);
	SCENEMANAGER->addScene("gardenToBoss_5f", _gardenToBoss_5f_Stage);
	SCENEMANAGER->addScene("diningRoom_5f", _dining_5f_Stage);
	SCENEMANAGER->addScene("prison_5f", _prison_5f_Stage);
	SCENEMANAGER->addScene("prison_well_5f", _prison_well_5f_Stage);
	SCENEMANAGER->addScene("prison_skul_5f", _prison_skul_5f_Stage);

	//����
	SCENEMANAGER->addScene("Ending", new EndingScene);
}

void playGround::addFrameImg()
{	
	//key, �̹��� ���(~.png), ������ x��, ������ y��, �Ӽ�(����), �������� �ٲ�� ����?, Ʈ���� ����, Ʈ���� ���۵Ǵ� ������ �ε���, �ݺ� ���� 
	FRAMEINFOMANAGER->AddFrameInfo("�÷��̾�", L"Image/tempFrameImg/player.png", 16, 4, PLAYER);
	FRAMEINFOMANAGER->AddFrameInfo("��", L"Image/tempFrameImg/��.png", 1, 4, ENEMY);
	FRAMEINFOMANAGER->AddFrameInfo("����", L"Image/tempFrameImg/����.png", 3, 4, ENEMY);
	FRAMEINFOMANAGER->AddFrameInfo("�ذ�", L"Image/tempFrameImg/�ذ�.png", 3, 4, ENEMY);
	FRAMEINFOMANAGER->AddFrameInfo("����", L"Image/tempFrameImg/����1.png", 1, 4, OBJ, 8, true, 1, false);
	FRAMEINFOMANAGER->AddFrameInfo("�ɺ�������", L"Image/tempFrameImg/�ɺ�������.png", 1, 4, OBJ, 10, true, 1, false);
	FRAMEINFOMANAGER->AddFrameInfo("������", L"Image/tempFrameImg/������.png", 3, 1, OBJ);
	FRAMEINFOMANAGER->AddFrameInfo("������", L"Image/tempFrameImg/������.png", 3, 1, OBJ);
	FRAMEINFOMANAGER->AddFrameInfo("�����ʲ�����", L"Image/tempFrameImg/�����ʲ�����.png", 6, 1, OBJ, 10, true, 3, false);
	FRAMEINFOMANAGER->AddFrameInfo("��1", L"Image/tempFrameImg/��1.png", 1, 4, OBJ, 11, true, 1, false);
	FRAMEINFOMANAGER->AddFrameInfo("��2", L"Image/tempFrameImg/��2.png", 1, 4, OBJ, 11, true, 1, false);
	FRAMEINFOMANAGER->AddFrameInfo("��3", L"Image/tempFrameImg/��3.png", 1, 4, OBJ, 11, true, 1, false);
	FRAMEINFOMANAGER->AddFrameInfo("������1", L"Image/tempFrameImg/������1.png", 1, 3, OBJ, 15);
	FRAMEINFOMANAGER->AddFrameInfo("������2", L"Image/tempFrameImg/������2.png", 1, 4, OBJ, 15);
	FRAMEINFOMANAGER->AddFrameInfo("������3", L"Image/tempFrameImg/������3.png", 1, 3, OBJ, 15);
	FRAMEINFOMANAGER->AddFrameInfo("����", L"Image/tempFrameImg/����.png", 1, 4, OBJ, 5, true, 1, false);
	FRAMEINFOMANAGER->AddFrameInfo("2��öâ", L"Image/tempFrameImg/2��öâ.png", 1, 2, OBJ, 10, true, 1, false);
	FRAMEINFOMANAGER->AddFrameInfo("����1", L"Image/tempFrameImg/����1.png", 1, 5, OBJ, 6, true, 1, false);
	FRAMEINFOMANAGER->AddFrameInfo("�Ͼ��", L"Image/tempFrameImg/�Ͼ��.png", 1, 4, OBJ, 15);
	FRAMEINFOMANAGER->AddFrameInfo("�Ͼ������", L"Image/tempFrameImg/�Ͼ������.png", 9, 1, OBJ, 15, true, 1, false);
	FRAMEINFOMANAGER->AddFrameInfo("Ǫ����", L"Image/tempFrameImg/Ǫ����.png", 6, 1, OBJ);
	FRAMEINFOMANAGER->AddFrameInfo("������", L"Image/tempFrameImg/������.png", 6, 1, OBJ);
	FRAMEINFOMANAGER->AddFrameInfo("Į�������", L"Image/tempFrameImg/Į�������.png", 6, 1, OBJ, 8, true, 3, false);
	FRAMEINFOMANAGER->AddFrameInfo("�ٱ��ϰ�", L"Image/tempFrameImg/�ٱ��ϰ�.png", 4, 1, OBJ, 7, true, 1, false);
	FRAMEINFOMANAGER->AddFrameInfo("����", L"Image/tempFrameImg/����.png", 2, 1, OBJ, 5, true, 1, false);
	FRAMEINFOMANAGER->AddFrameInfo("���ھ�����", L"Image/tempFrameImg/���ھ�����.png", 1, 4, OBJ, 8, true, 1, false);	
	FRAMEINFOMANAGER->AddFrameInfo("�����1", L"Image/tempFrameImg/�����1.png", 8, 1, OBJ, 7);
	FRAMEINFOMANAGER->AddFrameInfo("�չٴ�", L"Image/tempFrameImg/�չٴ�.png", 1, 8, OBJ, 6, true, 1, false);

	//ĳ���� �̹��� �߰�
	FRAMEINFOMANAGER->AddFrameInfo("violaIdle", L"Image/violaFrameImg/violaIdle.png", 16, 4, PLAYER);
	FRAMEINFOMANAGER->AddFrameInfo("violaWalk", L"Image/violaFrameImg/violaWalk.png", 16, 4, PLAYER);
	FRAMEINFOMANAGER->AddFrameInfo("violaDash", L"Image/violaFrameImg/violaDash.png", 16, 4, PLAYER);
	//���� �̹��� �߰�
	FRAMEINFOMANAGER->AddFrameInfo("Elen", L"Image/ElenFrameImg/Elen.png", 8, 4, ENEMY);


	//��� �̹��� �߰�
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
	//�׿� UI
	IMAGEMANAGER->AddImage("ȭ��ǥ", L"Image/mapTool/ȭ��ǥ.png");
	IMAGEMANAGER->AddImage("ȭ��ǥ����", L"Image/mapTool/ȭ��ǥ����.png");
	IMAGEMANAGER->AddImage("ȭ��ǥ������", L"Image/mapTool/ȭ��ǥ������.png");
	IMAGEMANAGER->AddImage("1����ŸƮ", L"Image/mapTool/stageMap/08.png");
	IMAGEMANAGER->AddImage("messe", L"Image/UI/messe.png");
	IMAGEMANAGER->AddImage("status", L"image/UI/status2.png");
	IMAGEMANAGER->AddImage("playerMenu", L"image/UI/bar7.png");
	IMAGEMANAGER->AddImage("menu", L"image/menu.png");
	IMAGEMANAGER->AddImage("bar", L"Image/obj/bar2.png");

	//������
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

	// 5�� �÷��̾� ��ũ
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
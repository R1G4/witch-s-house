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

#pragma region desc: ���߿� �ε����� �ű�� date: 2021/1/29 by pju
	IMAGEMANAGER->AddImage("ȭ��ǥ", L"Image/mapTool/ȭ��ǥ.png");
	IMAGEMANAGER->AddImage("ȭ��ǥ����", L"Image/mapTool/ȭ��ǥ����.png");
	IMAGEMANAGER->AddImage("ȭ��ǥ������", L"Image/mapTool/ȭ��ǥ������.png");
	IMAGEMANAGER->AddImage("1����ŸƮ", L"Image/mapTool/stageMap/08.png");

	//�Ʒ� �Ŵ��������� ������ �̹����� �߰� �����ϴ�.
	//key, �̹��� ���(~.png), ������ x��, ������ y��, �Ӽ�(����), �������� �ٲ�� ����?, Ʈ���� ����, Ʈ���� ���۵Ǵ� ������ �ε���, �ݺ� ���� 
#pragma endregion

	//ImageManager::GetInstance()->AddImage("TestObject", L"TrapObject.png");
	//ImageManager::GetInstance()->AddFrameImage("violaIdle", L"violaIdle.png",16,4);
	//ImageManager::GetInstance()->AddFrameImage("TestFrameObject",
	//	L"Bomb.png", 3, 1);
	//ImageManager::GetInstance()->AddImage("background", L"background.png");
	//viola = ImageManager::GetInstance()->FindImage("viloaIdle");
	//_currentFrameX = 0;
	//_currentFrameY = 0;
	//pt.x = WINSIZEX / 2;
	//pt.y = WINSIZEY / 2;
	//camera = PointMake(0, 0);
	addScene();
	addAutoImage();
	addFrameImg();
	SCENEMANAGER->addScene("MapToolScene", new mapTool);
	

	firstFloorStage* _firstFloorStage;
	_firstFloorStage = new stage1_1;
	SCENEMANAGER->addScene("stage1_1", _firstFloorStage);

#pragma region �������� ���� ���� �а� ����ʽÿ�
	//�������� ���� �� �ٽø����� �Ҽ���..
	//�ϴ� �����ȳ��� �����κ� �����Ͽ���
	//����ü Ÿ�Ͽ� �ε��� ������ �����ϰ� Ű��(image)���� ��ü�Ͽ���
	//������ ���İ� ����ϰ� �Ͻø� �� �Ͱ���
#pragma endregion

	SCENEMANAGER->addScene("����", new garDen);
	SCENEMANAGER->addScene("����2", new garDenUnder);
	SCENEMANAGER->changeScene("BossStage");
	//SCENEMANAGER->changeScene("stage1_1");
	
	/////////////////UI �ϴ� �ּ�ó��/////////////
	//addScene();
	//SCENEMANAGER->changeScene("����ȭ��");
	/////////////////////////////////////////////

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
	SCENEMANAGER->update();
	FRAMEINFOMANAGER->update();
}

//�׸��� ����
void playGround::render()
{
	//����� �ʱ�ȭ
	D2DRenderer::GetInstance()->BeginRender(D2D1::ColorF::Black);
	{
		SCENEMANAGER->render();
		/*for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				D2DRenderer::GetInstance()->DrawRectangle(RectMakePivot(
					Vector2(750 + j * TILESIZE, 100 + i * TILESIZE), Vector2(TILESIZE, TILESIZE), Pivot::Center));
			}
		}*/
		//�츮�� �� Ÿ�ϸ� ����
		//for (int i = 0; i < TILEY; ++i)
		//{
		//	for (int j = 0; j < TILEX; ++j)
		//	{
		//		D2DRenderer::GetInstance()->DrawRectangle(RectMakePivot(Vector2(j*TILESIZE, i*TILESIZE), Vector2(j*TILESIZE + TILESIZE, i*TILESIZE + TILESIZE), Pivot::LeftTop),D2DRenderer::DefaultBrush::White);
		//	}
		//}
		/*D2DRenderer::GetInstance()->DrawRectangle(RectMakePivot(Vector2(800, 550), Vector2(144, 48), Pivot::Center),D2DRenderer::DefaultBrush::White);
		D2DRenderer::GetInstance()->DrawRectangle(RectMakePivot(Vector2(800 + 144 + 10, 550), Vector2(144, 48), Pivot::Center));*/
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
	SCENEMANAGER->addScene("BossStage", new bossStage);
}

void playGround::addFrameImg()
{
	FRAMEINFOMANAGER->AddFrameInfo("�÷��̾�", L"Image/tempFrameImg/player.png", 16, 4, PLAYER);
	FRAMEINFOMANAGER->AddFrameInfo("��", L"Image/tempFrameImg/��.png", 1, 4, ENEMY);
	FRAMEINFOMANAGER->AddFrameInfo("����", L"Image/tempFrameImg/����.png", 3, 4, ENEMY);
	FRAMEINFOMANAGER->AddFrameInfo("�ذ�", L"Image/tempFrameImg/�ذ�.png", 3, 4, ENEMY);
	FRAMEINFOMANAGER->AddFrameInfo("����", L"Image/tempFrameImg/����1.png", 1, 4, OBJ, 8, true, 1, false);
	FRAMEINFOMANAGER->AddFrameInfo("�ɺ�������", L"Image/tempFrameImg/�ɺ�������.png", 1, 4, OBJ, 10, true, 1, true);
	FRAMEINFOMANAGER->AddFrameInfo("������", L"Image/tempFrameImg/������.png", 3, 1, OBJ);
	FRAMEINFOMANAGER->AddFrameInfo("������", L"Image/tempFrameImg/������.png", 3, 1, OBJ);
	FRAMEINFOMANAGER->AddFrameInfo("�����ʲ�����", L"Image/tempFrameImg/�����ʲ�����.png", 6, 1, OBJ, 10, true, 3, false);

	//ĳ���� �̹��� �߰�
	FRAMEINFOMANAGER->AddFrameInfo("violaIdle", L"Image/violaFrameImg/violaIdle.png", 16, 4, PLAYER);
	FRAMEINFOMANAGER->AddFrameInfo("violaWalk", L"Image/violaFrameImg/violaWalk.png", 16, 4, PLAYER);
	FRAMEINFOMANAGER->AddFrameInfo("violaDash", L"Image/violaFrameImg/violaDash.png", 16, 4, PLAYER);
}

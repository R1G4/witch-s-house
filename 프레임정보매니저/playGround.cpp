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
	IMAGEMANAGER->AddImage("1����ŸƮ", L"Image/mapTool/stageMap/008.png");

	//�Ʒ� �Ŵ��������� ������ �̹����� �߰� �����ϴ�.
	//key, path, Frame x, Frame y, Frame �Ӽ�(plyer,enemy,obj), Ʈ���� Frame�� �ε���(0�� �ƴ϶�� Ʈ���� �����Ѵٰ� ����)
	FRAMEINFOMANAGER->AddFrameInfo("�÷��̾�", L"Image/tempFrameImg/player.png", 16, 4, PLAYER);
	FRAMEINFOMANAGER->AddFrameInfo("��", L"Image/tempFrameImg/��.png", 1, 4, ENEMY);
	FRAMEINFOMANAGER->AddFrameInfo("����", L"Image/tempFrameImg/����.png", 3, 4, ENEMY);
	FRAMEINFOMANAGER->AddFrameInfo("�ذ�", L"Image/tempFrameImg/�ذ�.png", 3, 4, ENEMY);
	FRAMEINFOMANAGER->AddFrameInfo("����", L"Image/tempFrameImg/����1.png", 1, 4, OBJ, 8, true, 1, false);
	FRAMEINFOMANAGER->AddFrameInfo("�ɺ�������", L"Image/tempFrameImg/�ɺ�������.png", 1, 4, OBJ, 10, true, 1, true);
	FRAMEINFOMANAGER->AddFrameInfo("������", L"Image/tempFrameImg/������.png", 3, 1, OBJ);
	FRAMEINFOMANAGER->AddFrameInfo("������", L"Image/tempFrameImg/������.png", 3, 1, OBJ);
	FRAMEINFOMANAGER->AddFrameInfo("�����ʲ�����", L"Image/tempFrameImg/�����ʲ�����.png", 6, 1, OBJ, 10, true, 3, false);
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



	SCENEMANAGER->addScene("MapToolScene", new mapTool);
	//SCENEMANAGER->changeScene("MapToolScene");

	firstFloorStage* _firstFloorStage;
	_firstFloorStage = new stage1_1;
	SCENEMANAGER->addScene("stage1_1", _firstFloorStage);

	SCENEMANAGER->addScene("����", new garDen);
	SCENEMANAGER->changeScene("stage1_1");
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

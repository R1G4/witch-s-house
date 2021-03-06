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
	_sound = new sound;
	_sound->init();
#pragma region desc: 나중에 로딩으로 옮기기 date: 2021/1/29 by pju
	IMAGEMANAGER->AddImage("화살표", L"Image/mapTool/화살표.png");
	IMAGEMANAGER->AddImage("화살표왼쪽", L"Image/mapTool/화살표왼쪽.png");
	IMAGEMANAGER->AddImage("화살표오른쪽", L"Image/mapTool/화살표오른쪽.png");
	IMAGEMANAGER->AddImage("1층스타트", L"Image/mapTool/stageMap/08.png");

	//아래 매니저에서도 프레임 이미지를 추가 가능하다.
	//key, 이미지 경로(~.png), 프레임 x축, 프레임 y축, 속성(종류), 프레임이 바뀌는 간격?, 트리거 유무, 트리거 시작되는 프레임 인덱스, 반복 유무 
	FRAMEINFOMANAGER->AddFrameInfo("플레이어", L"Image/tempFrameImg/player.png", 16, 4, PLAYER);
	FRAMEINFOMANAGER->AddFrameInfo("곰", L"Image/tempFrameImg/곰.png", 1, 4, ENEMY);
	FRAMEINFOMANAGER->AddFrameInfo("눈깔", L"Image/tempFrameImg/눈깔.png", 3, 4, ENEMY);
	FRAMEINFOMANAGER->AddFrameInfo("해골", L"Image/tempFrameImg/해골.png", 3, 4, ENEMY);
	FRAMEINFOMANAGER->AddFrameInfo("액자", L"Image/tempFrameImg/액자1.png", 1, 4, OBJ, 8, true, 1, false);
	FRAMEINFOMANAGER->AddFrameInfo("꽃병프레임", L"Image/tempFrameImg/꽃병프레임.png", 1, 4, OBJ, 10, true, 1, true);
	FRAMEINFOMANAGER->AddFrameInfo("켜진초", L"Image/tempFrameImg/켜진초.png", 3, 1, OBJ);
	FRAMEINFOMANAGER->AddFrameInfo("꺼진초", L"Image/tempFrameImg/꺼진초.png", 3, 1, OBJ);
	FRAMEINFOMANAGER->AddFrameInfo("켜진초꺼진초", L"Image/tempFrameImg/켜진초꺼진초.png", 6, 1, OBJ, 10, true, 3, false);
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

	addBackGroundImage();

	SCENEMANAGER->addScene("MapToolScene", new mapTool);
	SCENEMANAGER->changeScene("MapToolScene");

	firstFloorStage* _firstFloorStage;
	_firstFloorStage = new stage1_1;
	SCENEMANAGER->addScene("stage1_1", _firstFloorStage);

	SCENEMANAGER->addScene("실험", new garDen);
	//SCENEMANAGER->changeScene("stage1_1");
	
	/////////////////UI 일단 주석처리/////////////
	addScene();
	SCENEMANAGER->changeScene("시작화면");
	SOUNDMANAGER->play("main");
	/////////////////////////////////////////////

	

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
	_sound->update();
	SCENEMANAGER->update();
	FRAMEINFOMANAGER->update();
}

//그리기 전용
void playGround::render()
{
	//백버퍼 초기화
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
		//우리가 쓸 타일맵 제작
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
	//백버퍼에 그린 내용들을 화면에 뿌려라~
	D2DRenderer::GetInstance()->EndRender();
}

void playGround::addBackGroundImage()
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
}

void playGround::addScene()
{
	SCENEMANAGER->addScene("시작화면", new menu);
	SCENEMANAGER->addScene("레벨선택", new levelChoiceScene);
	SCENEMANAGER->addScene("MapToolScene", new mapTool);
}
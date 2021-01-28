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


#pragma region  삭제해야하나? 혹시몰라서 냄겨둠



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
#pragma endregion

#pragma region 로딩씬 추가하면 거기에 옮기기 
	IMAGEMANAGER->AddFrameImage("플레이어", L"Image/tempFrameImg/player.png", 16, 4);
	IMAGEMANAGER->AddFrameImage("곰", L"Image/tempFrameImg/곰.png", 1, 4);
	IMAGEMANAGER->AddFrameImage("눈깔", L"Image/tempFrameImg/눈깔.png", 3, 4);
	IMAGEMANAGER->AddFrameImage("해골", L"Image/tempFrameImg/해골.png", 3, 4);
	IMAGEMANAGER->AddFrameImage("액자", L"Image/tempFrameImg/액자1.png", 1, 4);
	IMAGEMANAGER->AddImage("화살표", L"Image/mapTool/화살표.png");

	//프레임 이미지 샘플로 사용할려면 아래 매니저에 추가할 것
	DICTIONARYMANAGER->addTotalFrameImg("플레이어", PLAYER);
	DICTIONARYMANAGER->addTotalFrameImg("곰", ENEMY);
	DICTIONARYMANAGER->addTotalFrameImg("눈깔", ENEMY);
	DICTIONARYMANAGER->addTotalFrameImg("해골", ENEMY);
	DICTIONARYMANAGER->addTotalFrameImg("액자", OBJ);
#pragma endregion


	SCENEMANAGER->addScene("MapToolScene", new mapTool);
	//SCENEMANAGER->changeScene("MapToolScene");

	SCENEMANAGER->addScene("garDenScene", new garDen);
	SCENEMANAGER->addScene("garDenUnderScene", new garDenUnder);
	SCENEMANAGER->changeScene("garDenUnderScene");
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
	SCENEMANAGER->update();
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

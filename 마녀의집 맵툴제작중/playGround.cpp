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
	SCENEMANAGER->changeScene("MapToolScene");
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

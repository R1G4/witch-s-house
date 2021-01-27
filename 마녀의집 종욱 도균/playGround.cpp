#include "stdafx.h"
#include "playGround.h"

playGround::playGround()
{
}


playGround::~playGround()
{
}

HRESULT playGround::init()
{
	gameNode::init(true);

	addImg();
	addScene();

	SCENEMANAGER->changeScene("메뉴");
	return S_OK;
}

void playGround::release()
{

}

void playGround::update()
{
	gameNode::update();
	SCENEMANAGER->update();
}

void playGround::addScene()
{
	SCENEMANAGER->addScene("메뉴", new menu);
	SCENEMANAGER->addScene("계속하기", new continueScene);
	SCENEMANAGER->addScene("레벨선택", new levelChoiceScene);
	SCENEMANAGER->addScene("MapToolScene", new mapTool);
}

void playGround::addImg()
{
	ImageManager::GetInstance()->AddImage("menu", L"image/menu.png");
	ImageManager::GetInstance()->AddImage("bar", L"image/bar.png");
	ImageManager::GetInstance()->AddImage("bar2", L"image/mapTool/bar2.png");
	ImageManager::GetInstance()->AddImage("messe", L"image/messe.png");
}

void playGround::render()
{
	//백버퍼 초기화
	D2DRenderer::GetInstance()->BeginRender(D2D1::ColorF::Black);
	{
		SCENEMANAGER->render();
	}
	D2DRenderer::GetInstance()->EndRender();
}


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

	addScene();
	SCENEMANAGER->changeScene("시작화면");
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
	SCENEMANAGER->addScene("시작화면", new menu);
	SCENEMANAGER->addScene("레벨선택", new levelChoiceScene);
}

//그리기 전용
void playGround::render()
{	
	//백버퍼 초기화
	D2DRenderer::GetInstance()->BeginRender(D2D1::ColorF::Black);
	{
		SCENEMANAGER->render();
	}
	D2DRenderer::GetInstance()->EndRender();
}

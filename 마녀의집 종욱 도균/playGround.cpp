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
	SCENEMANAGER->changeScene("����ȭ��");
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
	SCENEMANAGER->addScene("����ȭ��", new menu);
	SCENEMANAGER->addScene("��������", new levelChoiceScene);
}

//�׸��� ����
void playGround::render()
{	
	//����� �ʱ�ȭ
	D2DRenderer::GetInstance()->BeginRender(D2D1::ColorF::Black);
	{
		SCENEMANAGER->render();
	}
	D2DRenderer::GetInstance()->EndRender();
}

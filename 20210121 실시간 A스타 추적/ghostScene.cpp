#include "stdafx.h"
#include "ghostScene.h"

HRESULT ghostScene::init()
{
	_knight = new knight;
	_knight->init();

	return S_OK;
}

void ghostScene::release()
{

}

void ghostScene::update()
{
	_knight->update();

}

void ghostScene::render()
{
	_knight->render();
}

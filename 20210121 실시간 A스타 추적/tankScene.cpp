#include "stdafx.h"
#include "tankScene.h"


tankScene::tankScene()
{
}


tankScene::~tankScene()
{
}

HRESULT tankScene::init()
{
	_tankMap = new tankMap;
	_tankMap->init();

	_tank = new tank;
	_tank->init();
	_tank->setTankMapMemoryAddressLink(_tankMap);

	_emTank = new enemyTank;
	_emTank->init();
	_emTank->setTankMapMemoryAddressLink(_tankMap);
	_emTank->setTankMemoryAddressLink(_tank);

	return S_OK;
}

void tankScene::release()
{
}

void tankScene::update()
{
	_tank->update();
	_emTank->update();
}

void tankScene::render()
{
	_tankMap->render();
	_tank->render();
	_emTank->render();
}

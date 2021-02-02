#include "stdafx.h"
#include "stage1_1.h"


stage1_1::stage1_1()
{
}


stage1_1::~stage1_1()
{
}

HRESULT stage1_1::init()
{
	camera = Vector2(0, 0);
	CAMERAMANAGER->setConfig(0, 0, TILESIZEX, TILESIZEY, 0, 0, TILESIZEX, TILESIZEY);

	load();
	firstFloorStage::init();

	return S_OK;
}

void stage1_1::release()
{
	firstFloorStage::release();
}

void stage1_1::update()
{
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			if (Vector2InRect(&_tiles[i*TILEX + j].rc, &CAMERAMANAGER->getWorldMouse()) && _tiles[i*TILEX + j].isCollider)
			{
				cout << "¤·¤·¤·¤·¤·" << endl;
			}
		}
	}

	if (KEYMANAGER->isStayKeyDown(VK_UP))
	{
		camera.y -= 5;
	}
	if (KEYMANAGER->isStayKeyDown(VK_DOWN))
	{
		camera.y += 5;
	}
	if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
	{
		camera.x += 5;

	}
	if (KEYMANAGER->isStayKeyDown(VK_LEFT))
	{
		camera.x -= 5;
	}

	CAMERAMANAGER->setCamera(camera);
	CAMERAMANAGER->setWorldMouse(_ptMouse);
	firstFloorStage::update();
}

void stage1_1::render()
{

	CAMERAMANAGER->render(IMAGEMANAGER->FindImage("1Ãþ½ºÅ¸Æ®"),
		Vector2(IMAGEMANAGER->FindImage("1Ãþ½ºÅ¸Æ®")->GetSize().x / 2,
		IMAGEMANAGER->FindImage("1Ãþ½ºÅ¸Æ®")->GetSize().y / 2));
	/*IMAGEMANAGER->FindImage("1Ãþ½ºÅ¸Æ®")->Render(
		Vector2(IMAGEMANAGER->FindImage("1Ãþ½ºÅ¸Æ®")->GetSize().x / 2,
			IMAGEMANAGER->FindImage("1Ãþ½ºÅ¸Æ®")->GetSize().y / 2));*/

	firstFloorStage::render();
}

void stage1_1::load()
{
	HANDLE file;
	DWORD read;
	file = CreateFile("Stage/stage1_1.map", GENERIC_READ, NULL, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);
	camera = _tiles->camera;
	CAMERAMANAGER->setCamera(camera);
	CloseHandle(file);
}


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
	firstFloorStage::update();
}

void stage1_1::render()
{
	IMAGEMANAGER->FindImage("1Ãþ½ºÅ¸Æ®")->Render(
		Vector2(IMAGEMANAGER->FindImage("1Ãþ½ºÅ¸Æ®")->GetSize().x / 2 - camera.x,
			IMAGEMANAGER->FindImage("1Ãþ½ºÅ¸Æ®")->GetSize().y / 2 - camera.y));

	firstFloorStage::render();
}

void stage1_1::load()
{
	HANDLE file;
	DWORD read;
	file = CreateFile("Stage/stage1_0.map", GENERIC_READ, NULL, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);
	camera = _tiles->camera;
	CloseHandle(file);
}


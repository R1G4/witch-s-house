#include "stdafx.h"
#include "garDenUnder.h"

garDenUnder::garDenUnder()
{
}

garDenUnder::~garDenUnder()
{
}

HRESULT garDenUnder::init()
{
	IMAGEMANAGER->AddFrameImage("TerrainSample", L"Image/mapTool/타일.png", 7, 2);
	IMAGEMANAGER->AddFrameImage("ObjectSample", L"Image/mapTool/objSample.png", 2, 3);
	IMAGEMANAGER->AddImage("나무", L"Image/mapTool/tileset/003_tr.png");
	camera = Vector2(0, 0);
	CAMERAMANAGER->setConfig(0, 0, TILESIZEX, TILESIZEY, 0, 0, TILESIZEX, TILESIZEY);
	load();
	return S_OK;
}

void garDenUnder::update()
{
	CAMERAMANAGER->setCamera(camera);
	CAMERAMANAGER->setWorldMouse(_ptMouse);



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
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			if (Vector2InRect(&_tiles[i*TILEX + j].rc, &CAMERAMANAGER->getWorldMouse()) && _tiles[i*TILEX + j].isCollider)
			{
				cout << "ㅇㅇㅇㅇㅇ" << endl;
			}
		}
	}
}

void garDenUnder::release()
{
}

void garDenUnder::render()
{
	CAMERAMANAGER->render(_backGround, Vector2(720 + 432, 648 - 96));

	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			if (KEYMANAGER->isToggleKey(VK_TAB))
			{
				CAMERAMANAGER->renderRc(_tiles[i*TILEX + j].rc, D2D1::ColorF::White, 1, 1);
				if (_tiles[i*TILEX + j].isCollider)
				{
					CAMERAMANAGER->renderFillRc(_tiles[i*TILEX + j].rc, D2D1::ColorF::Red, 0.4);
				}
			}
			if (_tiles[i*TILEX + j].terrain == TR_TRIGGER)CAMERAMANAGER->renderFillRc(_tiles[i*TILEX + j].rc, D2D1::ColorF::Aqua, 0.5);
		}
	}
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			if (_tiles[i*TILEX + j].obj == OBJ_NONE)continue;

			//중간에 배치하고 싶다면 이걸쓰세요. 디폴트 센타
			CAMERAMANAGER->render(IMAGEMANAGER->FindImage(_tiles[i*TILEX + j].keyName),
				Vector2(_tiles[i*TILEX + j].rc.left + TILESIZE / 2,
					_tiles[i*TILEX + j].rc.bottom - IMAGEMANAGER->FindImage(_tiles[i*TILEX + j].keyName)->GetSize().y / 2));

			//오른쪽으로 붙고자 하면 이걸쓰고
			/*CAMERAMANAGER->render(IMAGEMANAGER->FindImage(_tiles[i*TILEX + j].keyName),
				Vector2(_tiles[i*TILEX + j].rc.right,
					_tiles[i*TILEX + j].rc.bottom - IMAGEMANAGER->FindImage(_tiles[i*TILEX + j].keyName)->GetSize().y / 2));*/
		}
	}
	CAMERAMANAGER->render(IMAGEMANAGER->FindImage("나무"), Vector2(720 + 432, 648 - 96));
}

void garDenUnder::load()
{
	HANDLE file;
	DWORD read;
	file = CreateFile("Stage/gardenstage2.map", GENERIC_READ, NULL, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);
	camera = _tiles->camera;
	_backGround = IMAGEMANAGER->FindImage(_tiles->backGroundName);
	cout << _tiles->camera.x;
	CAMERAMANAGER->setCamera(camera);
	CloseHandle(file);
}

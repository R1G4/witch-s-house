#include "stdafx.h"
#include "garDen.h"

garDen::garDen()
{
}

garDen::~garDen()
{
}

HRESULT garDen::init()
{
	IMAGEMANAGER->AddFrameImage("TerrainSample", L"Image/mapTool/타일.png", 7, 2);
	IMAGEMANAGER->AddFrameImage("ObjectSample", L"Image/mapTool/objSample.png", 2, 3);
	IMAGEMANAGER->AddImage("배경", L"Image/mapTool/001.png");
	camera = Vector2(0, 0);
	CAMERAMANAGER->setConfig(0, 0, TILESIZEX, TILESIZEY, 0, 0, TILESIZEX, TILESIZEY);

	_boss = new boss;
	_boss->init(15, 13);

	
	load();
	return S_OK;
}

void garDen::update()
{
	CAMERAMANAGER->setCamera(camera);
	cameraMove();
	_boss->update();
	CAMERAMANAGER->setWorldMouse(_ptMouse);

	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			if (Vector2InRect(&_tiles[i*TILEX + j].rc,&CAMERAMANAGER->getWorldMouse())&&_tiles[i*TILEX+j].isCollider)
			{
				cout<<"ㅇㅇㅇㅇㅇ"<<endl;
			}
		}
	}
}

void garDen::release()
{
}

void garDen::render()
{
	//IMAGEMANAGER->FindImage("배경")->Render(Vector2(720 - camera.x, 648 - camera.y));
	CAMERAMANAGER->render(IMAGEMANAGER->FindImage("배경"), Vector2(720, 648));
	//IMAGEMANAGER->FindImage("배경")->Render(Vector2(720 - CAMERAMANAGER->getCameraX(), 648 - CAMERAMANAGER->getCameraY()));
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			if (KEYMANAGER->isToggleKey(VK_TAB))
			{
				CAMERAMANAGER->renderRc(_tiles[i*TILEX + j].rc,D2D1::ColorF::White,1,1);
				if (_tiles[i*TILEX + j].isCollider)
				{
					CAMERAMANAGER->renderFillRc(_tiles[i*TILEX + j].rc, D2D1::ColorF::Red, 0.4);
				}
//				CAMERAMANAGER->renderRc(_tiles[i*TILEX + j].rc, 1, 1);
			}
			//if (_tiles[i*TILEX + j].isCollider)_D2DRenderer->FillRectangle(_tiles[i*TILEX + j].rc, D2D1::ColorF::Red, 0.4);
		
			//if (_tiles[i*TILEX + j].terrain == TR_TRIGGER)_D2DRenderer->FillRectangle(_tiles[i*TILEX + j].rc, D2D1::ColorF::Aqua, 0.5);
			if (_tiles[i*TILEX + j].terrain == TR_TRIGGER)CAMERAMANAGER->renderFillRc(_tiles[i*TILEX + j].rc, D2D1::ColorF::Aqua, 0.5);
		}
	}
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			if (_tiles[i*TILEX + j].obj == OBJ_NONE)continue;
			/*IMAGEMANAGER->FindImage("ObjectSample")->FrameRender(
				Vector2(_tiles[i*TILEX + j].rc.left + TILESIZE / 2, _tiles[i*TILEX + j].rc.top + TILESIZE / 2),
				_tiles[i*TILEX + j].objFrameX, _tiles[i*TILEX + j].objFrameY);*/
			CAMERAMANAGER->FrameRender(IMAGEMANAGER->FindImage("ObjectSample"),
				Vector2(_tiles[i*TILEX + j].rc.left + TILESIZE / 2, _tiles[i*TILEX + j].rc.top + TILESIZE / 2),
				_tiles[i*TILEX + j].objFrameX, _tiles[i*TILEX + j].objFrameY);
		}
	}

	_boss->render();
}

void garDen::cameraMove()
{
	//카메라를 움직였을 때 타일도 함께 움직이도록 세팅함.
	//카메라 움직이는 방향과 타일 움직이는 방향은 반대여야함(카메라가 +면 타일은-값을, 카메라가 -값이면 타일은 +값을 줘야함)
	if (KEYMANAGER->isStayKeyDown(VK_UP))
	{

		camera.y -= 5;
		//for (int i = 0; i < TILEY; i++)
		//{
		//	for (int j = 0; j < TILEX; j++)
		//	{
		//		_tiles[i*TILEX + j].rc.Move(Vector2(0, 5));
		//	}
		//}
		CAMERAMANAGER->setRelativeY(-5.0);
	}
	if (KEYMANAGER->isStayKeyDown(VK_DOWN))
	{
		camera.y += 5;
		/*for (int i = 0; i < TILEY; i++)
		{
			for (int j = 0; j < TILEX; j++)
			{
				_tiles[i*TILEX + j].rc.Move(Vector2(0, -5));

			}
		}*/
		CAMERAMANAGER->setRelativeY(5.0);
	}
	if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
	{
		CAMERAMANAGER->setRelativeX(5.0);
		camera.x += 5;
		/*for (int i = 0; i < TILEY; i++)
		{
			for (int j = 0; j < TILEX; j++)
			{
				_tiles[i*TILEX + j].rc.Move(Vector2(-5, 0));
			}
		}*/
	}
	if (KEYMANAGER->isStayKeyDown(VK_LEFT))
	{
		camera.x -= 5;
		/*for (int i = 0; i < TILEY; i++)
		{
			for (int j = 0; j < TILEX; j++)
			{
				_tiles[i*TILEX + j].rc.Move(Vector2(5, 0));

			}
		}*/
		CAMERAMANAGER->setRelativeX(-5.0);
	}
}

void garDen::load()
{
	HANDLE file;
	DWORD read;
	file = CreateFile("save/save2.map", GENERIC_READ, NULL, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);
	camera = _tiles->camera;
	cout << _tiles->camera.x;
	CAMERAMANAGER->setCamera(camera);
	CloseHandle(file);
}

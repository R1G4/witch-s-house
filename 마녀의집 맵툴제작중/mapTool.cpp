#include "stdafx.h"
#include "mapTool.h"
/////////////////////////////테스트
#include<iostream>
#pragma comment (linker, "/entry:WinMainCRTStartup /subsystem:console")
using namespace std;
mapTool::mapTool()
{
}

mapTool::~mapTool()
{
}

HRESULT mapTool::init()
{
	IMAGEMANAGER->AddFrameImage("TerrainSample", L"Image/mapTool/타일.png", 7, 2);
	//IMAGEMANAGER->AddImage("temp", L"Image/mapTool/타일.png");
	setButton();
	setup();
	_crtSelect = CTRL_TERRAINDRAW;
	MapRC = RectMakePivot(Vector2(0, 0), Vector2(710, 710), Pivot::LeftTop);
	camera = PointMake(0, 0);
	tabOpen = true;
	return S_OK;
}

void mapTool::release()
{
}

void mapTool::update()
{
	setCtrl();
	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))_leftButtonDown = true;
	if (KEYMANAGER->isOnceKeyUp(VK_LBUTTON))_leftButtonDown = false;
	if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
	{
		camera.x += 48;
		for (int i = 0; i < TILEY; i++)
		{
			for (int j = 0; j < TILEY; j++)
			{
				_tiles[i*TILEX + j].rc.Move(Vector2(48, 0));
			}
		}
	}
	if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
	{
		camera.x -= 48;
		for (int i = 0; i < TILEY; i++)
		{
			for (int j = 0; j < TILEY; j++)
			{
				_tiles[i*TILEX + j].rc.Move(Vector2(-48, 0));
			}
		}
	}
	if (KEYMANAGER->isOnceKeyDown(VK_DOWN))
	{
		camera.y -= 48;
		for (int i = 0; i < TILEY; i++)
		{
			for (int j = 0; j < TILEY; j++)
			{
				_tiles[i*TILEX + j].rc.Move(Vector2(0, 48));
			}
		}
	}
	if (KEYMANAGER->isOnceKeyDown(VK_UP))
	{
		camera.y += 48;
		for (int i = 0; i < TILEY; i++)
		{
			for (int j = 0; j < TILEY; j++)
			{
				_tiles[i*TILEX + j].rc.Move(Vector2(0, -48));
			}
		}
	}
	MapRC = RectMakePivot(Vector2(0, 0), Vector2(710, 710), Pivot::LeftTop);
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			if (IntersectRectToRect(&_tiles[i*TILEX + j].rc, &MapRC))
			{
				_tiles[i*TILEX + j].isMapOn = true;
			}
			else _tiles[i*TILEX + j].isMapOn = false;
		}
	}
	switch (_crtSelect)
	{
	case CTRL_SAVE:
		break;
	case CTRL_LOAD:
		break;
	case CTRL_TERRAINDRAW:
		setMap();
		break;
	case CTRL_OBJDRAW:
		setMap();
		break;
	case CTRL_ERASER:
		setMap();
		break;
	case CTRL_PREV:
		break;
	case CTRL_NEXT:
		break;
	case CTRL_END:
		break;
	case CTRL_OPEN:
		break;
	case CTRL_CLOSE:
		break;
	}


	if (KEYMANAGER->isOnceKeyDown('M'))
	{
		if (tabOpen == false) tabOpen = true;

		else tabOpen = false;
	}
}

void mapTool::render()
{
	if (tabOpen == true)
	{
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < SAMPLETILEX; j++)
			{
				IMAGEMANAGER->FindImage("TerrainSample")->FrameRender(Vector2(750 + j * SAMPLETILESIZE, 100 + i * SAMPLETILESIZE), j, i);
				if (KEYMANAGER->isToggleKey(VK_TAB))
				{
					_D2DRenderer->DrawRectangle(_sampleTile[i*SAMPLETILEX + j].rcTile, D2DRenderer::DefaultBrush::White);
				}
			}
		}
	}
	else {}

	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			if (!_tiles[i*TILEX + j].isMapOn)continue;
			IMAGEMANAGER->FindImage("TerrainSample")->FrameRender(
				Vector2(_tiles[i*TILEX + j].rc.left + TILESIZE / 2, _tiles[i*TILEX + j].rc.top + TILESIZE / 2),
				_tiles[i*TILEX + j].terrainFrameX, _tiles[i*TILEX + j].terrainFrameY);
			if (KEYMANAGER->isToggleKey(VK_TAB))
			{
				_D2DRenderer->DrawRectangle(_tiles[i*TILEX + j].rc, D2DRenderer::DefaultBrush::White);
			}
		}
	}

	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			if (_tiles[i*TILEX + j].obj == OBJ_NONE)continue;
			IMAGEMANAGER->FindImage("TerrainSample")->FrameRender(
				Vector2(_tiles[i*TILEX + j].rc.left + TILESIZE / 2, _tiles[i*TILEX + j].rc.top + TILESIZE / 2),
				_tiles[i*TILEX + j].objFrameX, _tiles[i*TILEX + j].objFrameY);
		}
	}
	//IMAGEMANAGER->FindImage("TerrainSample")->Render(Vector2(800, 100));
	Save.img->Render(Vector2(Save.frc.left + 72, Save.frc.top + 24));
	Load.img->Render(Vector2(Load.frc.left + 72, Load.frc.top + 24));
	Erase.img->Render(Vector2(Erase.frc.left + 72, Erase.frc.top + 24));
	Prev.img->Render(Vector2(Prev.frc.left + 72, Prev.frc.top + 24));
	Next.img->Render(Vector2(Next.frc.left + 72, Next.frc.top + 24));
	terrain.img->Render(Vector2(terrain.frc.left + 72, terrain.frc.top + 24));
	Object.img->Render(Vector2(Object.frc.left + 72, Object.frc.top + 24));
	if (tabOpen == true)
		Close.img->Render(Vector2(Close.frc.left + 72, Close.frc.top + 24));
	else
		Open.img->Render(Vector2(Open.frc.left + 72, Open.frc.top + 24));


	_D2DRenderer->DrawRectangle(sampleSelec, D2DRenderer::DefaultBrush::White);
	_D2DRenderer->DrawRectangle(MapRC, D2DRenderer::DefaultBrush::White);
}

void mapTool::setButton()
{
	IMAGEMANAGER->AddImage("Save", L"Image/mapTool/Save.png");
	IMAGEMANAGER->AddImage("Load", L"Image/mapTool/Load.png");
	IMAGEMANAGER->AddImage("Erase", L"Image/mapTool/Erase.png");
	IMAGEMANAGER->AddImage("Prev", L"Image/mapTool/Prev.png");
	IMAGEMANAGER->AddImage("Next", L"Image/mapTool/Next.png");
	IMAGEMANAGER->AddImage("Terrain", L"Image/mapTool/Terrain.png");
	IMAGEMANAGER->AddImage("Object", L"Image/mapTool/Object.png"); \
		IMAGEMANAGER->AddImage("Close", L"Image/mapTool/Close.png");
	IMAGEMANAGER->AddImage("Open", L"Image/mapTool/Open.png");

	Save.img = IMAGEMANAGER->FindImage("Save");
	Load.img = IMAGEMANAGER->FindImage("Load");
	Prev.img = IMAGEMANAGER->FindImage("Prev");
	Next.img = IMAGEMANAGER->FindImage("Next");
	terrain.img = IMAGEMANAGER->FindImage("Terrain");
	Object.img = IMAGEMANAGER->FindImage("Object");
	Erase.img = IMAGEMANAGER->FindImage("Erase");
	Close.img = IMAGEMANAGER->FindImage("Close");
	Open.img = IMAGEMANAGER->FindImage("Open");


	Save.frc = RectMakePivot(Vector2(800, 550), Vector2(144, 48), Pivot::Center);
	Load.frc = RectMakePivot(Vector2(800 + 144 + 10, 550), Vector2(144, 48), Pivot::Center);
	Erase.frc = RectMakePivot(Vector2(800 + 288 + 10, 550), Vector2(144, 48), Pivot::Center);
	terrain.frc = RectMakePivot(Vector2(800, 550 + 58), Vector2(144, 48), Pivot::Center);
	Object.frc = RectMakePivot(Vector2(800 + 144 + 10, 550 + 58), Vector2(144, 48), Pivot::Center);
	Prev.frc = RectMakePivot(Vector2(800, 550 + 116), Vector2(144, 48), Pivot::Center);
	Next.frc = RectMakePivot(Vector2(800 + 154, 550 + 116), Vector2(144, 48), Pivot::Center);
	Close.frc = RectMakePivot(Vector2(800 + 288 + 10, 550 + 58), Vector2(144, 48), Pivot::Center);
	Open.frc = RectMakePivot(Vector2(800 + 288 + 10, 550 + 58), Vector2(144, 48), Pivot::Center);

}

void mapTool::setup()
{
	for (int i = 0; i < SAMPLETILEY; ++i)//y
	{
		for (int j = 0; j < SAMPLETILEX; ++j)//x
		{
			_sampleTile[i * SAMPLETILEX + j].terrainFrameX = j;//0~19->1번 줄 20~39->2번줄
			_sampleTile[i * SAMPLETILEX + j].terrainFrameY = i;

			//RectMake, RectMakeCenter
			_sampleTile[i*SAMPLETILEX + j].rcTile = RectMakePivot(
				Vector2(750 + j * SAMPLETILESIZE, 100 + i * SAMPLETILESIZE), Vector2(SAMPLETILESIZE, SAMPLETILESIZE), Pivot::Center);
		}
	}
	//우리가 쓸 타일맵 제작
	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			//_tiles[i*TILEX + j].rc = RectMakePivot(Vector2(j*TILESIZE, i*TILESIZE), Vector2(j*TILESIZE, i*TILESIZE),Pivot::Center);
			_tiles[i*TILEX + j].rc = RectMakePivot(Vector2(j*TILESIZE + TILESIZE / 2 - camera.x, i*TILESIZE + TILESIZE / 2 - camera.y), Vector2(TILESIZE, TILESIZE), Pivot::Center);
		}
	}
	cout << _tiles[0].rc.left << endl << _tiles[0].rc.top << endl;
	//잔디로 초기 타일맵 세팅
	for (int i = 0; i < TILEX * TILEY; ++i)
	{
		_tiles[i].terrainFrameX = 5;
		_tiles[i].terrainFrameY = 0;
		_tiles[i].objFrameX = 0;
		_tiles[i].objFrameY = 0;
		_tiles[i].terrain = terrainSelect(_tiles[i].terrainFrameX, _tiles[i].terrainFrameY);
		_tiles[i].obj = OBJ_NONE;
	}
}

void mapTool::setMap()
{
	if (_leftButtonDown)
	{
		for (int i = 0; i < SAMPLETILEY; i++)
		{
			for (int j = 0; j < SAMPLETILEX; j++)
			{
				/*if (IntersectRectToRect(&_tiles[i*TILEX + j].rc, &MapRC))
				{
					_tiles[i*TILEX+j].isMapOn = true;
				}*/
				if (Vector2InRect(&_sampleTile[i*SAMPLETILEX + j].rcTile, &Vector2(_ptMouse.x, _ptMouse.y)))
				{
					_currentTile.x = _sampleTile[i*SAMPLETILEX + j].terrainFrameX;
					_currentTile.y = _sampleTile[i*SAMPLETILEX + j].terrainFrameY;
					sampleSelec = RectMakePivot(Vector2(_sampleTile[i*SAMPLETILEX + j].rcTile.left, _sampleTile[i*SAMPLETILEX + j].rcTile.top), Vector2(48, 48), Pivot::LeftTop);

				}
			}
		}
	}
	if (tabOpen == false)
	{
		if (_leftButtonDown/*&&KEYMANAGER->isOnceKeyDown(VK_LBUTTON)*/)
		{
			for (int i = 0; i < TILEY; ++i)
			{
				for (int j = 0; j < TILEX; ++j)
				{
					if (Vector2InRect(&_tiles[i*TILEX + j].rc, &Vector2(_ptMouse.x, _ptMouse.y)) && _tiles[i*TILEX + j].isMapOn)
					{
						if (_crtSelect == CTRL_TERRAINDRAW)
						{
							_tiles[i*TILEX + j].terrainFrameX = _currentTile.x;
							_tiles[i*TILEX + j].terrainFrameY = _currentTile.y;

							_tiles[i*TILEX + j].terrain = terrainSelect(_currentTile.x, _currentTile.y);
						}
						else if (_crtSelect == CTRL_OBJDRAW)
						{
							_tiles[i*TILEX + j].objFrameX = _currentTile.x;
							_tiles[i*TILEX + j].objFrameY = _currentTile.y;

							_tiles[i*TILEX + j].obj = objSelect(_currentTile.x, _currentTile.y);
						}
						else if (_crtSelect == CTRL_ERASER)
						{
							_tiles[i*TILEX + j].objFrameX = NULL;
							_tiles[i*TILEX + j].objFrameY = NULL;
							_tiles[i*TILEX + j].obj = OBJ_NONE;
						}

						InvalidateRect(_hWnd, NULL, false);
						break;
					}
				}
			}
		}
	}


}

void mapTool::setCtrl()
{
	if (KEYMANAGER->isOnceKeyDown(VK_F2))
	{
		if (Vector2InRect(&Save.frc, &Vector2(_ptMouse)))_crtSelect = CTRL_SAVE;
		if (Vector2InRect(&Load.frc, &Vector2(_ptMouse)))_crtSelect = CTRL_LOAD;
		if (Vector2InRect(&Erase.frc, &Vector2(_ptMouse)))_crtSelect = CTRL_ERASER;
		if (Vector2InRect(&Prev.frc, &Vector2(_ptMouse)))_crtSelect = CTRL_PREV;
		if (Vector2InRect(&Next.frc, &Vector2(_ptMouse)))_crtSelect = CTRL_NEXT;
		if (Vector2InRect(&terrain.frc, &Vector2(_ptMouse)))_crtSelect = CTRL_TERRAINDRAW;
		if (Vector2InRect(&Object.frc, &Vector2(_ptMouse)))_crtSelect = CTRL_OBJDRAW;
	}

}

void mapTool::save()
{
}

void mapTool::load()
{
}

void mapTool::erase()
{
}

TERRAIN mapTool::terrainSelect(int frameX, int frameY)
{
	if ((frameX <= 2 && frameX >= 0) && (frameY <= 1)) return TR_FLOOR;
	if ((frameX <= 3 && frameX >= 2) && (frameY <= 1)) return TR_STONE;
	if ((frameX <= 6 && frameX >= 5) && (frameY <= 1)) return TR_GRASS;
	//if ((frameX <= 7 && frameX >= 0) && (frameY <= 6)) return TR_WATER;

	//기본은.. 잔디로다가
	return TR_GRASS;
}

OBJECT mapTool::objSelect(int frameX, int frameY)
{
	if ((frameX <= 2 && frameX >= 0) && (frameY <= 1)) return OBJ_LOOK;
	if ((frameX <= 3 && frameX >= 2) && (frameY <= 1)) return OBJ_LOOK;
	if ((frameX <= 6 && frameX >= 5) && (frameY <= 1)) return OBJ_LOOK;
	return OBJ_NONE;
}

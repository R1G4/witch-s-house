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
	IMAGEMANAGER->AddImage("배경", L"Image/mapTool/stageMap/003.png");
	IMAGEMANAGER->AddImage("나무", L"Image/mapTool/003_tr.png");
	load();
	return S_OK;
}

void garDenUnder::update()
{
}

void garDenUnder::release()
{
}

void garDenUnder::render()
{
	IMAGEMANAGER->FindImage("배경")->Render(Vector2(672 - camera.x, 552 - camera.y));

	cout << _tiles[0].objFrameX << endl;
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			//if (!_tiles[i*TILEX + j].isMapOn)continue;
			/*IMAGEMANAGER->FindImage("TerrainSample")->FrameRender(
				Vector2(_tiles[i*TILEX + j].rc.left + TILESIZE / 2, _tiles[i*TILEX + j].rc.top + TILESIZE / 2),
				_tiles[i*TILEX + j].terrainFrameX, _tiles[i*TILEX + j].terrainFrameY);*/
			if (KEYMANAGER->isToggleKey(VK_TAB))
			{
				_D2DRenderer->DrawRectangle(_tiles[i*TILEX + j].rc, D2DRenderer::DefaultBrush::White);
				if (_tiles[i*TILEX + j].isCollider)_D2DRenderer->FillRectangle(_tiles[i*TILEX + j].rc, D2D1::ColorF::Red, 0.4);
				if (_tiles[i*TILEX + j].terrain == TR_TRIGGER)_D2DRenderer->FillRectangle(_tiles[i*TILEX + j].rc, D2D1::ColorF::Aqua, 0.5);
			}
		}
	}
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			if (_tiles[i*TILEX + j].obj == OBJ_NONE)continue;
			IMAGEMANAGER->FindImage("ObjectSample")->FrameRender(
				Vector2(_tiles[i*TILEX + j].rc.left + TILESIZE / 2, _tiles[i*TILEX + j].rc.top + TILESIZE / 2),
				_tiles[i*TILEX + j].objFrameX, _tiles[i*TILEX + j].objFrameY);

		}
	}
	IMAGEMANAGER->FindImage("나무")->Render(Vector2(672 - camera.x, 552 - camera.y));
}

void garDenUnder::load()
{
	HANDLE file;
	DWORD read;
	file = CreateFile("Stage/gardenUnder.map", GENERIC_READ, NULL, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);
	camera = _tiles->camera;
	CloseHandle(file);
}

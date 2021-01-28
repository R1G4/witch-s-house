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
	IMAGEMANAGER->AddFrameImage("TerrainSample", L"Image/mapTool/Ÿ��.png", 7, 2);
	IMAGEMANAGER->AddFrameImage("ObjectSample", L"Image/mapTool/objSample.png", 2, 3);
	IMAGEMANAGER->AddImage("���", L"Image/mapTool/001.png");

	load();
	return S_OK;
}

void garDen::update()
{
	cameraMove();

}

void garDen::release()
{
}

void garDen::render()
{
	IMAGEMANAGER->FindImage("���")->Render(Vector2(720 - camera.x, 648 - camera.y));
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			if (KEYMANAGER->isToggleKey(VK_TAB))
			{
				_D2DRenderer->DrawRectangle(_tiles[i*TILEX + j].rc, D2DRenderer::DefaultBrush::White);
			}
			if (_tiles[i*TILEX + j].isCollider)_D2DRenderer->FillRectangle(_tiles[i*TILEX + j].rc, D2D1::ColorF::Red, 0.4);
			if (_tiles[i*TILEX + j].terrain == TR_TRIGGER)_D2DRenderer->FillRectangle(_tiles[i*TILEX + j].rc, D2D1::ColorF::Aqua, 0.5);

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
}

void garDen::cameraMove()
{
	//ī�޶� �������� �� Ÿ�ϵ� �Բ� �����̵��� ������.
	//ī�޶� �����̴� ����� Ÿ�� �����̴� ������ �ݴ뿩����(ī�޶� +�� Ÿ����-����, ī�޶� -���̸� Ÿ���� +���� �����)
	if (KEYMANAGER->isStayKeyDown(VK_UP))
	{

		camera.y -= 5;
		for (int i = 0; i < TILEY; i++)
		{
			for (int j = 0; j < TILEX; j++)
			{
				_tiles[i*TILEX + j].rc.Move(Vector2(0, 5));
			}
		}
	}
	if (KEYMANAGER->isStayKeyDown(VK_DOWN))
	{
		camera.y += 5;
		for (int i = 0; i < TILEY; i++)
		{
			for (int j = 0; j < TILEX; j++)
			{
				_tiles[i*TILEX + j].rc.Move(Vector2(0, -5));

			}
		}
	}
	if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
	{

		camera.x += 5;
		for (int i = 0; i < TILEY; i++)
		{
			for (int j = 0; j < TILEX; j++)
			{
				_tiles[i*TILEX + j].rc.Move(Vector2(-5, 0));
			}
		}
	}
	if (KEYMANAGER->isStayKeyDown(VK_LEFT))
	{
		camera.x -= 5;
		for (int i = 0; i < TILEY; i++)
		{
			for (int j = 0; j < TILEX; j++)
			{
				_tiles[i*TILEX + j].rc.Move(Vector2(5, 0));

			}
		}
	}

}

void garDen::load()
{
	HANDLE file;
	DWORD read;
	file = CreateFile("save/save1.map", GENERIC_READ, NULL, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	
	ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);
	camera = _tiles->camera;
	CloseHandle(file);
}

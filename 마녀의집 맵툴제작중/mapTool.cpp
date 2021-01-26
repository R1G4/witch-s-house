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
	IMAGEMANAGER->AddFrameImage("ObjectSample", L"Image/mapTool/objSample.png", 2, 3);	// 그림 변환시 변환 필요
	//IMAGEMANAGER->AddFrameImage("ObjectSample", L"Image/mapTool/오브젝트타일.png", 8, 3);
	//IMAGEMANAGER->AddImage("temp", L"Image/mapTool/타일.png");
	setButton();
	setup();
	//load();
	_crtSelect = CTRL_TERRAINDRAW;
	MapRC = RectMakePivot(Vector2(0, 0), Vector2(1270, 710), Pivot::LeftTop);
	camera = PointMake(0, 0);
	tabOpen = true;
	sampleBack = RectMakePivot(Vector2(600, 0), Vector2(1280-600, 300), Pivot::LeftTop);
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
	if (KEYMANAGER->isOnceKeyUp('P')|| KEYMANAGER->isOnceKeyUp('N')) _change_number = false;	// (prev next) f2키에서 떼면 순서 변환완료
	mapMove();
	tileSelect();
	switch (_crtSelect)
	{
	case CTRL_SAVE:
		save();
		break;
	case CTRL_LOAD:
		load();
		break;
	case CTRL_TERRAINDRAW:
		isterrain = true;
		setMap();
		break;
	case CTRL_OBJDRAW:
		isterrain = false;
		setMap();
		break;
	case CTRL_ERASER:
		setMap();
		break;
	case CTRL_PREV:
		previous();
		setMap();
		break;
	case CTRL_NEXT:
		next();
		setMap();
		break;
	case CTRL_END:
		break;
	case CTRL_COLLIDER:
		setMap();
		break;
	case CTRL_SETFRAMETILE:
		setFrameTile();
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
			if (_tiles[i*TILEX + j].isCollider)_D2DRenderer->FillRectangle(_tiles[i*TILEX + j].rc, D2D1::ColorF::Red, 0.4);
		}
	}
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			if (_tiles[i*TILEX + j].obj == OBJ_NONE)continue;
			IMAGEMANAGER->FindImage("ObjectSample")->FrameRender(
				Vector2(_tiles[i*TILEX + j].rc.left+TILESIZE/2, _tiles[i*TILEX + j].rc.top ),
				_tiles[i*TILEX + j].objFrameX, _tiles[i*TILEX + j].objFrameY);
		}//보정값 필요할지도 모름
	}
	if(tabOpen)_D2DRenderer->FillRectangle(sampleBack, D2D1::ColorF::Aquamarine, 0.7);
	if (tabOpen)
	{
		if (isterrain)
		{
			for (int i = 0; i < SAMPLETILEY; i++)
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
		if (!isterrain) {
			/*for (int i = 0; i < SAMPLEOBJECTY; i++)
			{
				for (int j = 0; j < SAMPLEOBJECTX; j++)
				{

					IMAGEMANAGER->FindImage("ObjectSample")->FrameRender(Vector2(750 + j * SAMPLETILESIZE, 100 + i * SAMPLETILESIZE), j, i);
					if (KEYMANAGER->isToggleKey(VK_TAB))
					{
						_D2DRenderer->DrawRectangle(_sampleObj[i*SAMPLEOBJECTX + j].rc, D2DRenderer::DefaultBrush::White);
					}
				}
			}*/
			int resetNum = _change_num;	// 같은 줄에 렉트랑 이미지를 고정시키기 위해 필요한 변수
			// 변형시켜야하는 랜더
			for (int i = _change_num; i < _change_num + 2; i++)
			{
				for (int j = 0; j < SAMPLEOBJECTX; j++)
				{
					IMAGEMANAGER->FindImage("ObjectSample")->FrameRender(Vector2(750 + j * SAMPLETILESIZE, 100 + (i - resetNum) * SAMPLETILESIZE), j, i);

					if (KEYMANAGER->isToggleKey(VK_TAB))
					{
						_D2DRenderer->DrawRectangle(_sampleObj[(i - resetNum)*SAMPLEOBJECTX + j].rc, D2DRenderer::DefaultBrush::White);
					}
				}
			}
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
	Collider.img->Render(Vector2(Collider.frc.left + 72, Collider.frc.top + 24));
	Player.img->SetSize(Vector2(144, 48));
	Player.img->Render(Vector2(Player.frc.left + 72, Player.frc.top + 24));
	_D2DRenderer->RenderTextField(Player.frc.left , Player.frc.top-5 , L"Player", 30, 144, 48, D2DRenderer::DefaultBrush::White, DWRITE_TEXT_ALIGNMENT_CENTER);
	Enemy.img->SetSize(Vector2(144, 48));
	Enemy.img->Render(Vector2(Enemy.frc.left + 72, Enemy.frc.top + 24));
	_D2DRenderer->RenderTextField(Enemy.frc.left , Enemy.frc.top -5, L"Enemy", 30, 144, 48, D2DRenderer::DefaultBrush::White, DWRITE_TEXT_ALIGNMENT_CENTER);
	FrameObj.img->SetSize(Vector2(144, 48));
	FrameObj.img->Render(Vector2(FrameObj.frc.left + 72, FrameObj.frc.top + 24));
	_D2DRenderer->RenderTextField(FrameObj.frc.left , FrameObj.frc.top-5, L"FrameObj", 30, 144, 48, D2DRenderer::DefaultBrush::White, DWRITE_TEXT_ALIGNMENT_CENTER);
	if (tabOpen == true)
		Close.img->Render(Vector2(Close.frc.left + 72, Close.frc.top + 24));
	else
		Open.img->Render(Vector2(Open.frc.left + 72, Open.frc.top + 24));
	_D2DRenderer->FillRectangle(_player.rc, D2D1::ColorF::Blue, 0.7);
	_D2DRenderer->FillRectangle(_enemy.rc, D2D1::ColorF::Black, 0.7);

	if(tabOpen)_D2DRenderer->DrawRectangle(sampleSelec, D2DRenderer::DefaultBrush::White);
	_D2DRenderer->DrawRectangle(MapRC, D2DRenderer::DefaultBrush::White);
	if(!tabOpen)_D2DRenderer->FillRectangle(tileSelec, D2D1::ColorF::Enum::LightYellow, 0.5);
	
}

void mapTool::setButton()
{
	IMAGEMANAGER->AddImage("Save", L"Image/mapTool/Save.png");
	IMAGEMANAGER->AddImage("Load", L"Image/mapTool/Load.png");
	IMAGEMANAGER->AddImage("Erase", L"Image/mapTool/Erase.png");
	IMAGEMANAGER->AddImage("Prev", L"Image/mapTool/Prev.png");
	IMAGEMANAGER->AddImage("Next", L"Image/mapTool/Next.png");
	IMAGEMANAGER->AddImage("Terrain", L"Image/mapTool/Terrain.png");
	IMAGEMANAGER->AddImage("Object", L"Image/mapTool/Object.png"); 
	IMAGEMANAGER->AddImage("Collider", L"Image/mapTool/Collider.png");
	IMAGEMANAGER->AddImage("Close", L"Image/mapTool/Close.png");
	IMAGEMANAGER->AddImage("Open", L"Image/mapTool/Open.png");
	IMAGEMANAGER->AddImage("Bar", L"Image/mapTool/bar2.png");
	Save.img = IMAGEMANAGER->FindImage("Save");
	Load.img = IMAGEMANAGER->FindImage("Load");
	Prev.img = IMAGEMANAGER->FindImage("Prev");
	Next.img = IMAGEMANAGER->FindImage("Next");
	terrain.img = IMAGEMANAGER->FindImage("Terrain");
	Object.img = IMAGEMANAGER->FindImage("Object");
	Erase.img = IMAGEMANAGER->FindImage("Erase");
	Close.img = IMAGEMANAGER->FindImage("Close");
	Open.img = IMAGEMANAGER->FindImage("Open");
	Collider.img = IMAGEMANAGER->FindImage("Collider");
	Player.img = IMAGEMANAGER->FindImage("Bar");
	Enemy.img = IMAGEMANAGER->FindImage("Bar");
	FrameObj.img = IMAGEMANAGER->FindImage("Bar");

	Save.frc = RectMakePivot(Vector2(0+72+48, 660), Vector2(144, 48), Pivot::Center);
	Load.frc = RectMakePivot(Vector2(144 + 10+72+48, 660), Vector2(144, 48), Pivot::Center);
	Erase.frc = RectMakePivot(Vector2(288 + 10+72+48, 660), Vector2(144, 48), Pivot::Center);
	terrain.frc = RectMakePivot(Vector2(432+10+72+48, 660), Vector2(144, 48), Pivot::Center);
	Object.frc = RectMakePivot(Vector2(576+10+72+48, 660), Vector2(144, 48), Pivot::Center);
	Prev.frc = RectMakePivot(Vector2(720+10+72+48,660), Vector2(144, 48), Pivot::Center);
	Next.frc = RectMakePivot(Vector2(864+10+72 +48, 660), Vector2(144, 48), Pivot::Center);
	Close.frc = RectMakePivot(Vector2(1008 + 10+72+48, 660), Vector2(144, 48), Pivot::Center);
	Open.frc = RectMakePivot(Vector2(1008 + 10 + 72+48, 660), Vector2(144, 48), Pivot::Center);
	Collider.frc = RectMakePivot(Vector2(0 + 72 + 48, 600), Vector2(144, 48), Pivot::Center);
	Player.frc = RectMakePivot(Vector2(144 + 10 + 72 + 48, 600), Vector2(144, 48), Pivot::Center);
	Enemy.frc = RectMakePivot(Vector2(288 + 10 + 72 + 48, 600), Vector2(144, 48), Pivot::Center);
	FrameObj.frc = RectMakePivot(Vector2(432 + 10 + 72 + 48, 600), Vector2(144, 48), Pivot::Center);
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
	/*for (int i = 0; i < SAMPLEOBJECTY; ++i)//y
	{
		for (int j = 0; j < SAMPLEOBJECTX; ++j)//x
		{
			_sampleObj[i * SAMPLEOBJECTX + j].objFrameX = j;//0~19->1번 줄 20~39->2번줄
			_sampleObj[i * SAMPLEOBJECTX + j].objFrameY = i;

			//RectMake, RectMakeCenter
			_sampleObj[i * SAMPLEOBJECTX + j].rc = RectMakePivot(
				Vector2(750 + j * SAMPLETILESIZE, 100 + i * SAMPLETILESIZE), Vector2(SAMPLETILESIZE, SAMPLETILESIZE), Pivot::Center);
		}
	}*/
	// 변형시켜야할 오브젝트 이닛
	for (int i = 0; i < SAMPLEOBJECTY; ++i)//y
	{
		for (int j = 0; j < SAMPLEOBJECTX; ++j)//x
		{
			_sampleObj[i * SAMPLEOBJECTX + j].objFrameX = j;//0~5->1번 줄 6~11->2번줄 12~17->3번줄
			_sampleObj[i * SAMPLEOBJECTX + j].objFrameY = i;

			//RectMake, RectMakeCenter
			_sampleObj[i * SAMPLEOBJECTX + j].rc = RectMakePivot(
				Vector2(750 + j * SAMPLETILESIZE, 100 + i * SAMPLETILESIZE), Vector2(SAMPLETILESIZE, SAMPLETILESIZE), Pivot::Center);

			// 리얼 넘버
			_sampleObj[i * SAMPLEOBJECTX + j].realNum = _realNum;
			_realNum++;
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
	if (_leftButtonDown && tabOpen)
	{
		for (int i = 0; i < SAMPLETILEY; i++)
		{
			for (int j = 0; j < SAMPLETILEX; j++)
			{
				/*if (IntersectRectToRect(&_tiles[i*TILEX + j].rc, &MapRC))
				{
					_tiles[i*TILEX+j].isMapOn = true;
				}*/
				if (Vector2InRect(&_sampleTile[i*SAMPLETILEX + j].rcTile, &Vector2(_ptMouse.x, _ptMouse.y)) && isterrain)
				{
					_currentTile.x = _sampleTile[i*SAMPLETILEX + j].terrainFrameX;
					_currentTile.y = _sampleTile[i*SAMPLETILEX + j].terrainFrameY;
					sampleSelec = RectMakePivot(Vector2(_sampleTile[i*SAMPLETILEX + j].rcTile.left, _sampleTile[i*SAMPLETILEX + j].rcTile.top), Vector2(48, 48), Pivot::LeftTop);

				}
			}
		}
		/*for (int i = 0; i < SAMPLEOBJECTY; i++)
		{
			for (int j = 0; j < SAMPLEOBJECTX; j++)
			{
				if (Vector2InRect(&_sampleObj[i * SAMPLEOBJECTX + j].rc, &Vector2(_ptMouse.x, _ptMouse.y)) && !isterrain)
				{
					_currentTile.x = _sampleObj[i * SAMPLEOBJECTX + j].objFrameX;
					_currentTile.y = _sampleObj[i * SAMPLEOBJECTX + j].objFrameY;
					sampleSelec = RectMakePivot(Vector2(_sampleObj[i * SAMPLEOBJECTX + j].rc.left, _sampleObj[i * SAMPLEOBJECTX + j].rc.top), Vector2(48, 48), Pivot::LeftTop);
					//cout << _currentTile.x << endl << _currentTile.y << endl;
				}
			}
		}*/
		// 변형시켜야할 오브젝트 업데이트
		int resetNum = _change_num;	// 같은 줄에 렉트랑 이미지를 고정시키기 위해 필요한 변수
		for (int i = _change_num; i < _change_num + 2; i++)	// 이전이나 이후 버튼으로 가져오는 2줄
		{
			for (int j = 0; j < SAMPLEOBJECTX; j++)	// 가로줄이니 안바꿔도됨
			{
				if (Vector2InRect(&_sampleObj[(i - resetNum) * SAMPLEOBJECTX + j].rc, &Vector2(_ptMouse.x, _ptMouse.y)) && !isterrain)
				{
					_currentTile.x = _sampleObj[i * SAMPLEOBJECTX + j].objFrameX;
					_currentTile.y = _sampleObj[i * SAMPLEOBJECTX + j].objFrameY;
					sampleSelec = RectMakePivot(Vector2(_sampleObj[i * SAMPLEOBJECTX + j].rc.left, _sampleObj[i * SAMPLEOBJECTX + j].rc.top), Vector2(48, 48), Pivot::LeftTop);
					//cout << _currentTile.x << endl << _currentTile.y << endl;

					_sampleObj[i * SAMPLEOBJECTX + j].rc = RectMakePivot(
						Vector2(750 + j * SAMPLETILESIZE, 100 + (i - resetNum) * SAMPLETILESIZE), Vector2(SAMPLETILESIZE, SAMPLETILESIZE), Pivot::Center);

					cout << _sampleObj[i * SAMPLEOBJECTX + j].realNum << endl;
					_crtSelect = CTRL_OBJDRAW;
				}
			}
		}
	}
	if (tabOpen == false)
	{
		if (_leftButtonDown)
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
						else if (_crtSelect == CTRL_COLLIDER)
						{
							if (!_tiles[i*TILEX + j].isCollider)
							{
								_tiles[i*TILEX + j].isCollider = true;
								_leftButtonDown = false;
							}
							else 
							{ 
								_tiles[i*TILEX + j].isCollider = false; 
								_leftButtonDown=false;
							}
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
	if (KEYMANAGER->isStayKeyDown(VK_CONTROL))
	{
		if (KEYMANAGER->isOnceKeyDown('S'))_crtSelect = CTRL_SAVE;
		if (KEYMANAGER->isOnceKeyDown('L'))_crtSelect = CTRL_LOAD;
		if (KEYMANAGER->isOnceKeyDown('E'))_crtSelect = CTRL_ERASER;
		if (KEYMANAGER->isOnceKeyDown('P'))_crtSelect = CTRL_PREV;
		if (KEYMANAGER->isOnceKeyDown('N'))_crtSelect = CTRL_NEXT;
		if (KEYMANAGER->isOnceKeyDown('T'))_crtSelect = CTRL_TERRAINDRAW;
		if (KEYMANAGER->isOnceKeyDown('O'))_crtSelect = CTRL_OBJDRAW;
		if (KEYMANAGER->isOnceKeyDown('C'))_crtSelect = CTRL_COLLIDER;
	}
	if (KEYMANAGER->isOnceKeyDown(VK_F1))
	{
		_crtSelect = CTRL_SETFRAMETILE;
		FrAtt = PLAYER;
	}
	if (KEYMANAGER->isOnceKeyDown(VK_F2))
	{
		_crtSelect = CTRL_SETFRAMETILE;
		FrAtt = ENEMY;
	}
	if (KEYMANAGER->isOnceKeyDown(VK_F3))
	{
		_crtSelect = CTRL_SETFRAMETILE;
		FrAtt = OBJ;
	}
	//if (KEYMANAGER->isOnceKeyDown('P'))
	//{
	//	_crtSelect = CTRL_PREV;
	//	cout << _change_num;
	//}
	//if (KEYMANAGER->isOnceKeyDown('N'))
	//{ 
	//	_crtSelect = CTRL_NEXT;
	//	cout << _change_num;
	//}
}

void mapTool::mapMove()
{
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
	MapRC = RectMakePivot(Vector2(0, 0), Vector2(1270, 710), Pivot::LeftTop);
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
}

void mapTool::tileSelect()
{
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			if (Vector2InRect(&_tiles[i*TILEX + j].rc, &Vector2(_ptMouse)))
			{
				tileSelec = RectMakePivot(Vector2(_tiles[i*TILEX + j].rc.left, _tiles[i*TILEX + j].rc.top), Vector2(TILESIZE, TILESIZE), Pivot::LeftTop);
			}
		}
	}
}

void mapTool::save()
{
	HANDLE file;
	DWORD write;
	file = CreateFile("saveMap1.map", GENERIC_WRITE, NULL, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	WriteFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &write, NULL);

	CloseHandle(file);
}

void mapTool::load()
{
	HANDLE file;
	DWORD read;
	file = CreateFile("saveMap1.map", GENERIC_READ, NULL, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);

	CloseHandle(file);
}

void mapTool::erase()
{
}

void mapTool::previous()
{
	// 변형시켜야할 이전 세트
	if (!_change_number)
	{
		_change_number = true;
		if (_change_num == 0)
			_change_num = SAMPLEOBJECTY - 2;
		else
			_change_num = _change_num - 2;
	}
}

void mapTool::next()
{
	// 변형시켜야할 이전 세트
	if (!_change_number)
	{
		_change_number = true;
		if (_change_num == SAMPLEOBJECTY - 2)
			_change_num = 0;
		else
			_change_num = _change_num + 2;
	}
}

void mapTool::setFrameTile()
{
	
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			switch (FrAtt)
			{
			case PLAYER:
				if (Vector2InRect(&_tiles[i*TILEX + j].rc, &Vector2(_ptMouse)) && !tabOpen&&_leftButtonDown)
				{
					_player.rc = RectMakePivot(Vector2(_tiles[i*TILEX + j].rc.left+TILESIZE/2, _tiles[i*TILEX + j].rc.top+TILESIZE/2), Vector2(TILESIZE, TILESIZE), Pivot::Center);
				}
				break;
			case ENEMY:
				if (Vector2InRect(&_tiles[i*TILEX + j].rc, &Vector2(_ptMouse)) && !tabOpen&&_leftButtonDown)
				{
					_enemy.rc = RectMakePivot(Vector2(_tiles[i*TILEX + j].rc.left+TILESIZE/2, _tiles[i*TILEX + j].rc.top+TILESIZE/2), Vector2(TILESIZE, TILESIZE), Pivot::Center);
				}
				break;
			case OBJ:

				break;
			}
		}
	}
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
	/*if ((frameX <= 2 && frameX >= 0) && (frameY <= 1)) return OBJ_LOOK;
	if ((frameX <= 3 && frameX >= 2) && (frameY <= 1)) return OBJ_LOOK;
	if ((frameX <= 6 && frameX >= 5) && (frameY <= 1)) return OBJ_LOOK;*/
	if (frameX <= SAMPLEOBJECTX && frameY <= SAMPLEOBJECTY)return OBJ_LOOK;		// 오브젝트 선택범위 확장을 위해 변환 필요
	return OBJ_NONE;
}

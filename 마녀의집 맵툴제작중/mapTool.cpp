#include "stdafx.h"
#include "mapTool.h"
/////////////////////////////�׽�Ʈ
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
	IMAGEMANAGER->AddFrameImage("TerrainSample", L"Image/mapTool/Ÿ��.png", 7, 2);
	IMAGEMANAGER->AddFrameImage("ObjectSample", L"Image/mapTool/objSample.png", 2, 3);	// �׸� ��ȯ�� ��ȯ �ʿ�
	IMAGEMANAGER->AddImage("���", L"Image/mapTool/001.png");

	//tilex = IMAGEMANAGER->FindImage("���")->GetWidth() / 48;
	setButton();
	_frameInterval = 0;	//������ �ε��� ���� �ʱ�ȭ
	_realNum = _change_num = 0;
	_y_rect_num = 1;	// png���� ��ȯ�� y���� ���������
//	TILEX = IMAGEMANAGER->FindImage("���")->GetWidth() / TILESIZE;
//	TILEY = IMAGEMANAGER->FindImage("���")->GetHeight() / TILESIZE;
	setup();
	//load();
	_crtSelect = CTRL_TERRAINDRAW;
	//MapRC = RectMakePivot(Vector2(0, 0), Vector2(1270, 710), Pivot::LeftTop);
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
	//if (KEYMANAGER->isOnceKeyUp('P')|| KEYMANAGER->isOnceKeyUp('N')) _change_number = false;	// (prev next) f2Ű���� ���� ���� ��ȯ�Ϸ�
	mapMove();
	tileSelect();
	switch (_crtSelect)
	{
	case CTRL_SAVE:
		save();
		break;
	case CTRL_LOAD:
		load(); //�ҷ�����
		getFrameTile();	//�ҷ��� �������� ���� ������ Ÿ�� ��ġ
		_crtSelect = CTRL_TERRAINDRAW;	//Ű�� �Է� ���� �ʴ� �̻� ��� �ε��ϱ⿡ ������ ���� �ʱ�ȭ
		break;
	case CTRL_TERRAINDRAW:
		//isterrain = true; by pju �̳����� ��ü�ϱ� ����
		setMap();
		break;
	case CTRL_OBJDRAW:
		//isterrain = false; by pju �̳����� ��ü�ϱ� ����
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
		if (KEYMANAGER->isOnceKeyDown('P'))
			_frameSelected = _frameSelected <= 0 ? DICTIONARYMANAGER->getTotalFrameImg().size() - 1 : _frameSelected -= 1;
		if (KEYMANAGER->isOnceKeyDown('N'))
			_frameSelected = _frameSelected >= DICTIONARYMANAGER->getTotalFrameImg().size() - 1 ? 0 : _frameSelected += 1;
		break;
	case CTRL_SETCORRELATION:
		setMap();
		break;
	case CTRL_SETTRIGGER:
		setMap();
		break;
	}


	if (KEYMANAGER->isOnceKeyDown('M'))
	{
		if (tabOpen == false) tabOpen = true;

		else tabOpen = false;
	}

	// ��ġ�� ������ �̹����� �ε��� ���� 
	setFrameIndex();
}

void mapTool::render()
{
	//for (int i = 0; i < TILEY; i++)
	//{
	//	for (int j = 0; j < TILEX; j++)
	//	{
	//		if (!_tiles[i*TILEX + j].isMapOn)continue;
	//		IMAGEMANAGER->FindImage("TerrainSample")->FrameRender(
	//			Vector2(_tiles[i*TILEX + j].rc.left + TILESIZE / 2, _tiles[i*TILEX + j].rc.top + TILESIZE / 2),
	//			_tiles[i*TILEX + j].terrainFrameX, _tiles[i*TILEX + j].terrainFrameY);
	//		if (KEYMANAGER->isToggleKey(VK_TAB))
	//		{
	//			//_D2DRenderer->DrawRectangle(_tiles[i*TILEX + j].rc, D2DRenderer::DefaultBrush::White);
	//		}
	//		if (_tiles[i*TILEX + j].isCollider)_D2DRenderer->FillRectangle(_tiles[i*TILEX + j].rc, D2D1::ColorF::Red, 0.4);
	//		if (_tiles[i*TILEX + j].terrain == TR_TRIGGER)_D2DRenderer->FillRectangle(_tiles[i*TILEX + j].rc, D2D1::ColorF::Aqua, 0.5);
	//	}
	//}

	IMAGEMANAGER->FindImage("���")->Render(Vector2(720-camera.x,648-camera.y));
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			//if (!_tiles[i*TILEX + j].isMapOn)continue;
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
				Vector2(_tiles[i*TILEX + j].rc.left + TILESIZE, _tiles[i*TILEX + j].rc.top ),	// ������ �ٲ�
				_tiles[i*TILEX + j].objFrameX, _tiles[i*TILEX + j].objFrameY);
			cout << "ddd";
			if (_tiles[i*TILEX + j].obj == OBJ_CORELATION)_D2DRenderer->FillRectangle(_tiles[i*TILEX + j].rc, D2D1::ColorF::Aquamarine, 0.5);
		}//������ �ʿ������� ��
	}
	
	if(tabOpen)_D2DRenderer->FillRectangle(sampleBack, D2D1::ColorF::Aquamarine, 0.7);
	if (tabOpen)
	{
		if (_crtSelect == CTRL_TERRAINDRAW) 
		//if (isterrain)by pju �ϴ� isterrain���Ÿ� ���� �ּ�
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
		if(_crtSelect == CTRL_OBJDRAW)
		{
		//if (!isterrain) { by pju �ϴ� �̳����� ��ü�ϱ� ����
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
			int resetNum = _change_num;	// ���� �ٿ� ��Ʈ�� �̹����� ������Ű�� ���� �ʿ��� ����
			// �������Ѿ��ϴ� ����
			for (int i = _change_num; i < _change_num + _y_rect_num; i++)
			{
				for (int j = 0; j < SAMPLEOBJECTX; j++)
				{
					IMAGEMANAGER->FindImage("ObjectSample")->FrameRender(Vector2(750 + j * SAMPLEOBJTILESIZEX, 100 + (i - resetNum) * SAMPLEOBJTILESIZEY), j, i);

					if (KEYMANAGER->isToggleKey(VK_TAB))
					{
						_D2DRenderer->DrawRectangle(_sampleObj[(i - resetNum)*SAMPLEOBJECTX + j].rc, D2DRenderer::DefaultBrush::White);
					}
				}
			}
		}
		//���� ������ �̹��� �����ֱ�
		//���� ������ �÷��̸���Ʈ �����ϰ� �;���
		if (_crtSelect == CTRL_SETFRAMETILE)
		{
			int previous2 = _frameSelected + 2;
			int	next2 = _frameSelected - 2;
			int previous = _frameSelected + 1;
			int	next = _frameSelected - 1;

			if (next < 0)
				next = DICTIONARYMANAGER->getTotalFrameImg().size() - 1;
			if (previous > DICTIONARYMANAGER->getTotalFrameImg().size() - 1)
				previous = 0;
			if (next2 < 0)
				next2 = DICTIONARYMANAGER->getTotalFrameImg().size() - 2;
			if (previous2 > DICTIONARYMANAGER->getTotalFrameImg().size() - 2)
				previous2 = 0;

			_D2DRenderer->FillRectangle(Vector2(720, 150), Vector2(140, 140), Pivot::Center, D2D1::ColorF::Enum::White, 1.0f);
			IMAGEMANAGER->FindImage(DICTIONARYMANAGER->getTotalFrameImg()[next2].keyName)->SetScale(0.61);
			IMAGEMANAGER->FindImage(DICTIONARYMANAGER->getTotalFrameImg()[next2].keyName)->FrameRender(Vector2((720), 150), 0, 0);
			_D2DRenderer->DrawRectangle(Vector2(720, 150), Vector2(140, 140), Pivot::Center, D2D1::ColorF::Enum::DarkGray, 1.0f, 5);

			_D2DRenderer->FillRectangle(Vector2(820, 150), Vector2(180, 180), Pivot::Center, D2D1::ColorF::Enum::White, 1.0f);
			IMAGEMANAGER->FindImage(DICTIONARYMANAGER->getTotalFrameImg()[next].keyName)->SetScale(0.82);
			IMAGEMANAGER->FindImage(DICTIONARYMANAGER->getTotalFrameImg()[next].keyName)->FrameRender(Vector2((820), 150), 0, 0);
			_D2DRenderer->DrawRectangle(Vector2(820, 150), Vector2(180, 180), Pivot::Center, D2D1::ColorF::Enum::DarkGray, 1.0f, 5);

			_D2DRenderer->FillRectangle(Vector2(1180, 150), Vector2(140, 140), Pivot::Center, D2D1::ColorF::Enum::White, 1.0f);
			IMAGEMANAGER->FindImage(DICTIONARYMANAGER->getTotalFrameImg()[previous2].keyName)->SetScale(0.61);
			IMAGEMANAGER->FindImage(DICTIONARYMANAGER->getTotalFrameImg()[previous2].keyName)->FrameRender(Vector2((1180), 150), 0, 0);
			_D2DRenderer->DrawRectangle(Vector2(1180, 150), Vector2(140, 140), Pivot::Center, D2D1::ColorF::Enum::DarkGray, 1.0f, 5);

			_D2DRenderer->FillRectangle(Vector2(1100, 150), Vector2(180, 180), Pivot::Center, D2D1::ColorF::Enum::White, 1.0f);
			IMAGEMANAGER->FindImage(DICTIONARYMANAGER->getTotalFrameImg()[previous].keyName)->SetScale(0.82);
			IMAGEMANAGER->FindImage(DICTIONARYMANAGER->getTotalFrameImg()[previous].keyName)->FrameRender(Vector2((1100), 150), 0, 0);
			_D2DRenderer->DrawRectangle(Vector2(1100, 150), Vector2(180, 180), Pivot::Center, D2D1::ColorF::Enum::DarkGray, 1.0f, 5);

			_D2DRenderer->FillRectangle(Vector2(950, 150), Vector2(220, 220), Pivot::Center, D2D1::ColorF::Enum::White, 1.0f);
			IMAGEMANAGER->FindImage(DICTIONARYMANAGER->getTotalFrameImg()[_frameSelected].keyName)->FrameRender(Vector2((950), 150), 0, 0);
			_D2DRenderer->DrawRectangle(Vector2(950, 150), Vector2(220, 220), Pivot::Center, D2D1::ColorF::Enum::DarkGray, 1.0f, 5);

			IMAGEMANAGER->FindImage("ȭ��ǥ")->SetScale(0.65f);
			IMAGEMANAGER->FindImage("ȭ��ǥ")->Render(Vector2(950, 260));
		}
	}
	//IMAGEMANAGER->FindImage("TerrainSample")->Render(Vector2(800, 100));
	if (tabOpen)
	{
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
		_D2DRenderer->RenderTextField(Player.frc.left, Player.frc.top - 5, L"Player", 30, 144, 48, D2DRenderer::DefaultBrush::White, DWRITE_TEXT_ALIGNMENT_CENTER);
		Enemy.img->SetSize(Vector2(144, 48));
		Enemy.img->Render(Vector2(Enemy.frc.left + 72, Enemy.frc.top + 24));
		_D2DRenderer->RenderTextField(Enemy.frc.left, Enemy.frc.top - 5, L"Enemy", 30, 144, 48, D2DRenderer::DefaultBrush::White, DWRITE_TEXT_ALIGNMENT_CENTER);
		FrameObj.img->SetSize(Vector2(144, 48));
		FrameObj.img->Render(Vector2(FrameObj.frc.left + 72, FrameObj.frc.top + 24));
		_D2DRenderer->RenderTextField(FrameObj.frc.left, FrameObj.frc.top - 5, L"FrameObj", 30, 144, 48, D2DRenderer::DefaultBrush::White, DWRITE_TEXT_ALIGNMENT_CENTER);
		setCor.img->SetSize(Vector2(144, 48));
		setCor.img->Render(Vector2(setCor.frc.left + 72, setCor.frc.top + 24));
		_D2DRenderer->RenderTextField(setCor.frc.left, setCor.frc.top - 5, L"SetCorrelation", 20, 144, 48, D2DRenderer::DefaultBrush::White, DWRITE_TEXT_ALIGNMENT_CENTER);
		setTri.img->SetSize(Vector2(144, 48));
		setTri.img->Render(Vector2(setTri.frc.left + 72, setTri.frc.top + 24));
		_D2DRenderer->RenderTextField(setTri.frc.left, setTri.frc.top - 5, L"SetTrigger", 30, 144, 48, D2DRenderer::DefaultBrush::White, DWRITE_TEXT_ALIGNMENT_CENTER);

	}
	if (tabOpen == true)
		Close.img->Render(Vector2(Close.frc.left + 72, Close.frc.top + 24));
	else
		Open.img->Render(Vector2(Open.frc.left + 72, Open.frc.top + 24));

	//Ÿ�Ͽ� ������ �̹��� ��ġ ����
	for (int i = 0; i < _vFrameTile.size(); i++)
	{
		if (_vFrameTile[i].kinds == PLAYER)  _D2DRenderer->FillRectangle(_vFrameTile[i].rc, D2D1::ColorF::Blue, 0.7);
		if (_vFrameTile[i].kinds == ENEMY)  _D2DRenderer->FillRectangle(_vFrameTile[i].rc, D2D1::ColorF::Black, 0.7);
		_vFrameTile[i].img->FrameRender(Vector2((_vFrameTile[i].rc.left + _vFrameTile[i].rc.right) / 2,  _vFrameTile[i].rc.bottom  - _vFrameTile[i].img->GetSize().y / 2),
			_vFrameTile[i].frameX, _vFrameTile[i].frameY);
	}
	if(tabOpen)_D2DRenderer->DrawRectangle(sampleSelec, D2DRenderer::DefaultBrush::White);
	//_D2DRenderer->DrawRectangle(MapRC, D2DRenderer::DefaultBrush::White);
	if(!tabOpen)_D2DRenderer->FillRectangle(tileSelec, D2D1::ColorF::Enum::LightYellow, 0.5);
	
}

void mapTool::setFrameIndex()
{
	//����) Ÿ�Ͽ� ��ġ�� �������� ���� ���
	if (_vFrameTile.size() <= 0)
		return;

	//����) ������ ���� ���� ���� ���� ��
	_frameInterval++;
	if (_frameInterval % 9)
		return;

	_frameInterval = 0;
	for (int i = 0; i < _vFrameTile.size(); i++)
	{
		//������ ���� ���� �� �߰�
		//���� ����, ���η� ���ٷ� �����ϴ� �̹����� ���⿡ �ڵ����� x�� Ȥ�� y�� ������
		//�������� �ִ� �������� 1���϶�� ������ y���� ������Ŵ
		if (_vFrameTile[i].img->GetMaxFrameX() <= 1)
		{
			_vFrameTile[i].frameY = _vFrameTile[i].frameY + 1;
			if (_vFrameTile[i].frameY > _vFrameTile[i].img->GetMaxFrameY() - 1)
				_vFrameTile[i].frameY = 0;

		}
		//�������� �ִ� �������� 1���϶�� ������ x���� ������Ŵ
		else if (_vFrameTile[i].img->GetMaxFrameY() <= 1)
		{
			_vFrameTile[i].frameX = _vFrameTile[i].frameX + 1;
			if (_vFrameTile[i].frameX > _vFrameTile[i].img->GetMaxFrameX() - 1)
				_vFrameTile[i].frameX = 0;
		}
		//�׿��� ���
		else
		{
			//�Ʒ� ������ ��ȭ�� ���߿� ���� �� �� ����
			switch (_vFrameTile[i].kinds)
			{
			case PLAYER:
				//��κ� ���� �����̹Ƿ� ������ X����
				_vFrameTile[i].frameX = _vFrameTile[i].frameX + 1;
				if (_vFrameTile[i].frameX > _vFrameTile[i].img->GetMaxFrameX() - 1)
					_vFrameTile[i].frameX = 0;
				break;
			case ENEMY:
			case OBJ:
				//��κ� ���� �����̹Ƿ� ������ Y����
				_vFrameTile[i].frameY++;
				if (_vFrameTile[i].frameY > _vFrameTile[i].img->GetMaxFrameY() - 1)
					_vFrameTile[i].frameY = 0;
				break;
			}
		}
	}
}

void mapTool::setCorrelation()
{
}
void mapTool::setTrigger()
{
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
	setCor.img = IMAGEMANAGER->FindImage("Bar");
	setTri.img = IMAGEMANAGER->FindImage("Bar");

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
	setCor.frc = RectMakePivot(Vector2(576 + 10 + 72 + 48, 600), Vector2(144, 48), Pivot::Center);
	setTri.frc = RectMakePivot(Vector2(720 + 10 + 72 + 48, 600), Vector2(144, 48), Pivot::Center);

}

void mapTool::setup()
{
	for (int i = 0; i < SAMPLETILEY; ++i)//y
	{
		for (int j = 0; j < SAMPLETILEX; ++j)//x
		{
			_sampleTile[i * SAMPLETILEX + j].terrainFrameX = j;//0~19->1�� �� 20~39->2����
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
			_sampleObj[i * SAMPLEOBJECTX + j].objFrameX = j;//0~19->1�� �� 20~39->2����
			_sampleObj[i * SAMPLEOBJECTX + j].objFrameY = i;

			//RectMake, RectMakeCenter
			_sampleObj[i * SAMPLEOBJECTX + j].rc = RectMakePivot(
				Vector2(750 + j * SAMPLETILESIZE, 100 + i * SAMPLETILESIZE), Vector2(SAMPLETILESIZE, SAMPLETILESIZE), Pivot::Center);
		}
	}*/
	// �������Ѿ��� ������Ʈ �̴�
	for (int i = 0; i < SAMPLEOBJECTY; ++i)//y
	{
		for (int j = 0; j < SAMPLEOBJECTX; ++j)//x
		{
			_sampleObj[i * SAMPLEOBJECTX + j].objFrameX = j;//0~5->1�� �� 6~11->2���� 12~17->3����
			_sampleObj[i * SAMPLEOBJECTX + j].objFrameY = i;

			//RectMake, RectMakeCenter
			_sampleObj[i * SAMPLEOBJECTX + j].rc = RectMakePivot(
				Vector2(750 + j * SAMPLEOBJTILESIZEX, 100 + i * SAMPLEOBJTILESIZEY), Vector2(SAMPLEOBJTILESIZEX, SAMPLEOBJTILESIZEY), Pivot::Center);

			// ���� �ѹ�
			_sampleObj[i * SAMPLEOBJECTX + j].realNum = _realNum;
			_realNum++;
		}
	}

	//�츮�� �� Ÿ�ϸ� ����
	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			//_tiles[i*TILEX + j].rc = RectMakePivot(Vector2(j*TILESIZE, i*TILESIZE), Vector2(j*TILESIZE, i*TILESIZE),Pivot::Center);
			_tiles[i*TILEX + j].rc = RectMakePivot(Vector2(j*TILESIZE + TILESIZE / 2 - camera.x, i*TILESIZE + TILESIZE / 2 - camera.y), Vector2(TILESIZE, TILESIZE), Pivot::Center);
		}
	}
	cout << _tiles[0].rc.left << endl << _tiles[0].rc.top << endl;
	//�ܵ�� �ʱ� Ÿ�ϸ� ����
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
				if (Vector2InRect(&_sampleTile[i*SAMPLETILEX + j].rcTile, &Vector2(_ptMouse.x, _ptMouse.y)) && _crtSelect == CTRL_TERRAINDRAW) //&& isterrain) by pju �̳����� ��ü�ϱ� ����
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
		// �������Ѿ��� ������Ʈ ������Ʈ
		int resetNum = _change_num;	// ���� �ٿ� ��Ʈ�� �̹����� ������Ű�� ���� �ʿ��� ����
		for (int i = _change_num; i < _change_num + _y_rect_num; i++)	// �����̳� ���� ��ư���� �������� 2��
		{
			for (int j = 0; j < SAMPLEOBJECTX; j++)	// �������̴� �ȹٲ㵵��
			{
				if (Vector2InRect(&_sampleObj[(i - resetNum) * SAMPLEOBJECTX + j].rc, &Vector2(_ptMouse.x, _ptMouse.y)) && _crtSelect == CTRL_OBJDRAW)// !isterrain) by pju �̳����� ��ü�ϱ� ����
				{
					_currentTile.x = _sampleObj[i * SAMPLEOBJECTX + j].objFrameX;
					_currentTile.y = _sampleObj[i * SAMPLEOBJECTX + j].objFrameY;
					sampleSelec = RectMakePivot(Vector2(_sampleObj[i * SAMPLEOBJECTX + j].rc.left, _sampleObj[i * SAMPLEOBJECTX + j].rc.top), Vector2(48, 48), Pivot::LeftTop);
					//cout << _currentTile.x << endl << _currentTile.y << endl;

					_sampleObj[i * SAMPLEOBJECTX + j].rc = RectMakePivot(
						Vector2(750 + j * SAMPLEOBJTILESIZEX, 100 + (i - resetNum) * SAMPLEOBJTILESIZEY), Vector2(SAMPLEOBJTILESIZEX, SAMPLEOBJTILESIZEY), Pivot::Center);

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
					if (Vector2InRect(&_tiles[i*TILEX + j].rc, &Vector2(_ptMouse.x, _ptMouse.y)))
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
						else if (_crtSelect == CTRL_SETCORRELATION)
						{
							if (_tiles[i*TILEX + j].obj == OBJ_NONE)
							{
								_tiles[i*TILEX + j].terrain = TR_CORELATION;
								_tiles[i*TILEX + j].obj = OBJ_NONE;
							}
							if(_tiles[i*TILEX+j].obj!=OBJ_CORELATION)_tiles[i*TILEX + j].obj = OBJ_CORELATION;
							else if (_tiles[i*TILEX + j].obj == OBJ_CORELATION)_tiles[i*TILEX + j].obj = OBJ_LOOK;
							
							_leftButtonDown = false;
						}
						else if (_crtSelect == CTRL_SETTRIGGER)
						{
							_tiles[i*TILEX + j].terrain = TR_TRIGGER;
							_leftButtonDown = false;
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
	if (KEYMANAGER->isOnceKeyDown(VK_RBUTTON))
	{
		if (Vector2InRect(&Save.frc,&Vector2(_ptMouse)))_crtSelect = CTRL_SAVE;
		if (Vector2InRect(&Load.frc,&Vector2(_ptMouse)))_crtSelect = CTRL_LOAD;
		if (Vector2InRect(&Erase.frc,&Vector2(_ptMouse)))_crtSelect = CTRL_ERASER;
		if (Vector2InRect(&Prev.frc,&Vector2(_ptMouse))) { _crtSelect = CTRL_PREV; _change_number = false; }
		if (Vector2InRect(&Next.frc,&Vector2(_ptMouse))) { _crtSelect = CTRL_NEXT; _change_number = false; }
		if (Vector2InRect(&terrain.frc,&Vector2(_ptMouse)))_crtSelect = CTRL_TERRAINDRAW;
		if (Vector2InRect(&Object.frc,&Vector2(_ptMouse)))_crtSelect = CTRL_OBJDRAW;
		if (Vector2InRect(&Collider.frc,&Vector2(_ptMouse)))_crtSelect = CTRL_COLLIDER;
		if (Vector2InRect(&FrameObj.frc,&Vector2(_ptMouse)))_crtSelect = CTRL_SETFRAMETILE;
		if (Vector2InRect(&setCor.frc,&Vector2(_ptMouse)))_crtSelect = CTRL_SETCORRELATION;
		if (Vector2InRect(&setTri.frc,&Vector2(_ptMouse)))_crtSelect = CTRL_SETTRIGGER;

	}
	if (KEYMANAGER->isStayKeyDown(VK_SHIFT))
	{
		
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
		camera.x-= 48;
		for (int i = 0; i < TILEY; i++)
		{
			for (int j = 0; j < TILEY; j++)
			{
				_tiles[i*TILEX + j].rc.Move(Vector2(48, 0));
			}
		}
		for (int i = 0; i < _vFrameTile.size(); i++)
		{
			_vFrameTile[i].rc.Move(Vector2(48, 0));
		}
		
	}
	if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
	{
		camera.x += 48;
		for (int i = 0; i < TILEY; i++)
		{
			for (int j = 0; j < TILEY; j++)
			{
				_tiles[i*TILEX + j].rc.Move(Vector2(-48, 0));
			}
		}
		for (int i = 0; i < _vFrameTile.size(); i++)
		{
			_vFrameTile[i].rc.Move(Vector2(-48, 0));
		}
	}
	if (KEYMANAGER->isOnceKeyDown(VK_UP))
	{
		camera.y -= 48;
		for (int i = 0; i < TILEY; i++)
		{
			for (int j = 0; j < TILEY; j++)
			{
				_tiles[i*TILEX + j].rc.Move(Vector2(0, 48));
			}
		}	
		for (int i = 0; i < _vFrameTile.size(); i++)
		{
			_vFrameTile[i].rc.Move(Vector2(0, 48));
		}
	}
	if (KEYMANAGER->isOnceKeyDown(VK_DOWN))
	{
		camera.y += 48;
		for (int i = 0; i < TILEY; i++)
		{
			for (int j = 0; j < TILEY; j++)
			{
				_tiles[i*TILEX + j].rc.Move(Vector2(0, -48));
			}
		}
		for (int i = 0; i < _vFrameTile.size(); i++)
		{
			_vFrameTile[i].rc.Move(Vector2(0, -48));
		}
	}
	/*MapRC = RectMakePivot(Vector2(0, 0), Vector2(1270, 710), Pivot::LeftTop);
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
	}*/
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

	OPENFILENAME openFileName;
	static char strFileTitle[MAX_PATH], strFileExtension[10], strFilePath[100];
	TCHAR curDirectoryPath[256];

	GetCurrentDirectory(256, curDirectoryPath);

	ZeroMemory(&openFileName, sizeof(OPENFILENAME));    // ����ü�� 0���� �¾�
	openFileName.lStructSize = sizeof(OPENFILENAME);
	openFileName.hwndOwner = _hWnd;
	openFileName.lpstrTitle = "����";
	openFileName.lpstrFileTitle = strFileTitle; // strFileTitle
	openFileName.lpstrFilter = "Every File(*.*)\0*.*\0Text File\0*.txt;*.doc\0";
	openFileName.lpstrFile = strFilePath; // strFilePath
	openFileName.nMaxFile = MAX_PATH;
	openFileName.nMaxFileTitle = MAX_PATH;

	if (GetSaveFileName(&openFileName) != 0)    // �ε����� 1���� �����ϱ� ������ ����
	{
		switch (openFileName.nFilterIndex)
		{
		case 1:
			_tiles->camera = camera;
			file = CreateFile(strFilePath, GENERIC_WRITE, NULL, NULL,
				CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			WriteFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &write, NULL);
			CloseHandle(file);
			break;
		default:
			break;
		}
	}
	SetCurrentDirectory(curDirectoryPath);  // ����� ��θ� ���� ��η� ����

	_crtSelect = CTRL_TERRAINDRAW;
}

void mapTool::load()
{
	HANDLE file;
	DWORD read;

	OPENFILENAME openFileName;
	static char strFileTitle[MAX_PATH], strFileExtension[10], strFilePath[100];
	TCHAR curDirectoryPath[256];

	GetCurrentDirectory(256, curDirectoryPath);         // GetOpenFileName ȣ���ϸ� �⺻ ��θ��� �ٲ�� ������ �⺻ ��θ� �̸� ����

	ZeroMemory(&openFileName, sizeof(openFileName));    // ����ü�� 0���� �¾�
	openFileName.lStructSize = sizeof(openFileName);
	openFileName.hwndOwner = _hWnd;
	openFileName.lpstrTitle = "�ε�";
	openFileName.lpstrFileTitle = strFileTitle;
	openFileName.lpstrFile = strFilePath;
	openFileName.lpstrFilter = "Every File(*.*)\0*.*\0Text File\0*.txt;*.doc\0";
	openFileName.nMaxFile = MAX_PATH;
	openFileName.nMaxFileTitle = MAX_PATH;

	if (GetOpenFileName(&openFileName) != 0)    // �ε����� 1���� �����ϱ� ������ ����
	{
		switch (openFileName.nFilterIndex)
		{
		case 1:
			file = CreateFile(strFilePath, GENERIC_READ, NULL, NULL,
				OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

			ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);
			camera = _tiles->camera;
			CloseHandle(file);
			break;
		}
	}
	SetCurrentDirectory(curDirectoryPath);  // ����� ��θ� ���� ��η� ����
	_crtSelect = CTRL_TERRAINDRAW;
}

void mapTool::getFrameTile()
{
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			for (int k = 0; k < DICTIONARYMANAGER->getTotalFrameImg().size(); k++)
			{
				//�ҷ��� Ÿ���� Ű���� �����������̹��� ���Ϳ� �����ϴ� �̹����� ���
				if (DICTIONARYMANAGER->getTotalFrameImg()[k].keyName == _tiles[i*TILEX + j].frameKeyName)
				{
					//��Ʈ ����
					FloatRect rc;
					rc = RectMakePivot(Vector2(_tiles[i*TILEX + j].rc.left + TILESIZE / 2, _tiles[i*TILEX + j].rc.top + TILESIZE / 2), Vector2(TILESIZE, TILESIZE), Pivot::Center);

					tagFrameTile temp;
					temp.rc = rc;
					temp.kinds = DICTIONARYMANAGER->getTotalFrameImg()[k].kinds;
					temp.indexX = i;
					temp.indexY = j;
					temp.frameX = 0;
					temp.frameY = 0;
					temp.img = IMAGEMANAGER->FindImage(DICTIONARYMANAGER->getTotalFrameImg()[k].keyName);	//�̹����� ã�Ƽ� �ִ´�
					addFrameTile(temp);	//Ÿ�Ͽ� ������ �̹��� ��ġ
				}
			}
		}
	}
}

void mapTool::erase()
{
}

void mapTool::previous()
{
	// �������Ѿ��� ���� ��Ʈ
	if (!_change_number)
	{
		_change_number = true;

		_change_num = _change_num - _y_rect_num;
		if (_change_num < 0)
			_change_num = SAMPLEOBJECTY - _y_rect_num;
	}
}

void mapTool::next()
{
	// �������Ѿ��� ���� ��Ʈ
	if (!_change_number)
	{
		_change_number = true;

		_change_num = _change_num + _y_rect_num;
		if (_change_num > SAMPLEOBJECTY - _y_rect_num)
			_change_num = 0;
	}
}

void mapTool::setFrameTile()
{
	//���� ���� �����ְų� Ȥ�� ���콺 ���ʹ�ư Ŭ�� ���°� �ƴ� ��
	if (tabOpen || !_leftButtonDown)
		return;

	//���ʹ�ư �ѹ� �������� ������ �Ǿ�� �ϱ⿡ �ʱ�ȭ ���ش�.
	_leftButtonDown = false;

	//���� ����
	bool isDel = false;		
	FloatRect rc;
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			//�ش� Ÿ���� �ε����� ���콺�� �浹�� ���(Ÿ�Ͽ� ��ġ �õ��� ���)
			if (Vector2InRect(&_tiles[i*TILEX + j].rc, &Vector2(_ptMouse)))
			{
				//��Ʈ ����
				rc = RectMakePivot(Vector2(_tiles[i*TILEX + j].rc.left + TILESIZE / 2, _tiles[i*TILEX + j].rc.top + TILESIZE / 2), Vector2(TILESIZE, TILESIZE), Pivot::Center);

				tagFrameTile temp;
				temp.rc = rc;
				temp.kinds = DICTIONARYMANAGER->getTotalFrameImg()[_frameSelected].kinds;
				temp.indexX = i;
				temp.indexY = j;
				temp.frameX = 0;
				temp.frameY = 0;
				temp.img = IMAGEMANAGER->FindImage(DICTIONARYMANAGER->getTotalFrameImg()[_frameSelected].keyName);	//������ �̹��� ���Ϳ��� Ű���� �־ �̹����� �ִ´�

				//�̹� �����ϴ� Ÿ���� ������ �̹����� ��ġ �õ��� ��� addFrameTile�Լ����� true�� ��ȯ
				isDel = addFrameTile(temp);

				//���ŵǾ�� �ϴ� ����
				if (isDel)	//�ش� Ÿ���� ������ Ű���� �ʱ�ȭ ���ش�.
					_tiles[i*TILEX + j].frameKeyName = "";
				else	   //�ش� Ÿ���� ������ Ű���� �־��ش�.
					_tiles[i*TILEX + j].frameKeyName = DICTIONARYMANAGER->getTotalFrameImg()[_frameSelected].keyName;
			}
			//�浹���� ���� ���
			else
			{
				//�÷��̾�� �ߺ� ��ġ�� �Ұ���
				if (DICTIONARYMANAGER->getTotalFrameImg()[_frameSelected].kinds == PLAYER)
				{
					for (int k = 0; k < DICTIONARYMANAGER->getTotalFrameImg().size(); k++)
					{
						//�̹� Ÿ�Ͽ��� �÷��̾� Ÿ���� �����ϴ� ��� ���� Ÿ�Ͽ� ������ �̹��� Ű���� �����Ѵ�.
						if (DICTIONARYMANAGER->getTotalFrameImg()[k].kinds == PLAYER && DICTIONARYMANAGER->getTotalFrameImg()[k].keyName == _tiles[i*TILEX + j].frameKeyName)
							_tiles[i*TILEX + j].frameKeyName = "";
					}
				}
			}
		}
	}
}

bool mapTool::addFrameTile(tagFrameTile _frameTile)
{
	switch (_frameTile.kinds)
	{
		case PLAYER:
			for (int i = 0; i < _vFrameTile.size(); i++)
			{
				//�̹� �����ϴ� Ÿ���� �����ϴ� ���
				if (_vFrameTile[i].kinds == _frameTile.kinds &&
					_vFrameTile[i].indexX == _frameTile.indexX &&
					_vFrameTile[i].indexY == _frameTile.indexY)
				{
					//��ġ�� Ÿ���� ������ �̹����� �����Ѵ�.
					_vFrameTile.erase(_vFrameTile.begin() + i);
					//true�� ��ȯ�ϸ� �� �� ���� �� �ε�� Ÿ�Ͽ����� �����Ѵ�.
					return true;
				}
				//�׷��� ���� ��� 2�� �̻��� �÷��̾�� �������� ���ϹǷ�
				//������ ��ġ�� Ÿ���� ������ �̹����� �����Ѵ�.
				else if (_vFrameTile[i].kinds == _frameTile.kinds)
					_vFrameTile.erase(_vFrameTile.begin() + i);
			}
			break;
		case ENEMY:
			for (int i = 0; i < _vFrameTile.size(); i++)
			{
				if (_vFrameTile[i].kinds == _frameTile.kinds &&
					_vFrameTile[i].indexX == _frameTile.indexX &&
					_vFrameTile[i].indexY == _frameTile.indexY)
				{
					//��ġ�� Ÿ���� ������ �̹����� �����Ѵ�.
					_vFrameTile.erase(_vFrameTile.begin() + i);
					//true�� ��ȯ�ϸ� �� �� ���� �� �ε�� Ÿ�Ͽ����� �����Ѵ�.
					return true;
				}
			}
			break;
		case OBJ:
			for (int i = 0; i < _vFrameTile.size(); i++)
			{
				if (_vFrameTile[i].kinds == _frameTile.kinds &&
					_vFrameTile[i].indexX == _frameTile.indexX &&
					_vFrameTile[i].indexY == _frameTile.indexY)
				{
					//��ġ�� Ÿ���� ������ �̹����� �����Ѵ�.
					_vFrameTile.erase(_vFrameTile.begin() + i);
					//true�� ��ȯ�ϸ� �� �� ���� �� �ε�� Ÿ�Ͽ����� �����Ѵ�.
					return true;
				}
			}
			break;
	}

	//������ �̹����� �߰��Ѵ�.
	//�̹����� ��ġ �� �����ϴ� ����
	_vFrameTile.push_back(_frameTile);
	//false�� ��ȯ �� ��� ���� �� �ε�� Ÿ�Ͽ����� ���ŵ��� �ʴ´�.
	return false;
}

TERRAIN mapTool::terrainSelect(int frameX, int frameY)
{
	if ((frameX <= 2 && frameX >= 0) && (frameY <= 1)) return TR_FLOOR;
	if ((frameX <= 3 && frameX >= 2) && (frameY <= 1)) return TR_STONE;
	if ((frameX <= 6 && frameX >= 5) && (frameY <= 1)) return TR_GRASS;
	//if ((frameX <= 7 && frameX >= 0) && (frameY <= 6)) return TR_WATER;

	//�⺻��.. �ܵ�δٰ�
	return TR_GRASS;
}

OBJECT mapTool::objSelect(int frameX, int frameY)
{
	/*if ((frameX <= 2 && frameX >= 0) && (frameY <= 1)) return OBJ_LOOK;
	if ((frameX <= 3 && frameX >= 2) && (frameY <= 1)) return OBJ_LOOK;
	if ((frameX <= 6 && frameX >= 5) && (frameY <= 1)) return OBJ_LOOK;*/
	if (frameX <= SAMPLEOBJECTX && frameY <= SAMPLEOBJECTY)return OBJ_LOOK;		// ������Ʈ ���ù��� Ȯ���� ���� ��ȯ �ʿ�
	return OBJ_NONE;
}

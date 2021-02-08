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
	//IMAGEMANAGER->AddImage("���", L"Image/mapTool/stageMap/008.png");

	setButton();
	// png���� ��ȯ�� y���� ���������
	_y_rect_num = 1;	

	//������ �̹��� ���� �ʱ�ȭ
	_frameInterval = 0;	 

	//���õ� �̹���(���� �� Ÿ�� ���Կ�)
	_frameSelected = 0;	 
	_objSelected = 1;

	setup();
	_crtSelect = CTRL_TERRAINDRAW;
	camera = Vector2(0,0);
	
	CAMERAMANAGER->setConfig(0, 0, TILESIZEX, TILESIZEY, 0, 0, TILESIZEX, TILESIZEY );
	tabOpen = true;
	sampleBack = RectMakePivot(Vector2(600, 0), Vector2(1280-600, 300), Pivot::LeftTop);
	backCount = 1;
	backName = "���";
	onjName = "obj";

	return S_OK;
}

void mapTool::release()
{
}

void mapTool::update()
{
	mapMove();
	setCtrl();
	CAMERAMANAGER->setWorldMouse(_ptMouse);//Vector2InRect�Ἥ ���� �����ؾ��ϸ� ����������.(ptInrect����Ѵٸ� �ݵ��)
	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))_leftButtonDown = true;
	if (KEYMANAGER->isOnceKeyUp(VK_LBUTTON))_leftButtonDown = false;
	//if (KEYMANAGER->isOnceKeyUp('P')|| KEYMANAGER->isOnceKeyUp('N')) _change_number = false;	// (prev next) f2Ű���� ���� ���� ��ȯ�Ϸ�

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
		setMap();
		break;
	case CTRL_OBJDRAW:	
		setObjTile();
		//����Ű�� ������Ʈ�� ������ �ٲܼ��ִ�.
		if (KEYMANAGER->isOnceKeyDown('P'))
			_objSelected = _objSelected <= 0 ? OBJSIZE : _objSelected -= 1;
		if (KEYMANAGER->isOnceKeyDown('N'))
			_objSelected = _objSelected >= OBJSIZE ? 0 : _objSelected += 1;
		break;
	case CTRL_ERASER:
		setMap();
		break;
	case CTRL_PREV:
		setMap();
		break;
	case CTRL_NEXT:
		setMap();
		break;
	case CTRL_END:
		break;
	case CTRL_COLLIDER:
		setMap();
		break;
	case CTRL_SETFRAMETILE:
		//�������̹��� ��ġ
		setFrameTile();
		//����Ű�� �������̹����� ������ �ٲܼ��ִ�.
		if (KEYMANAGER->isOnceKeyDown('P'))
			_frameSelected = _frameSelected <= 0 ? FRAMEINFOMANAGER->GetSize() - 1 : _frameSelected -= 1;
		if (KEYMANAGER->isOnceKeyDown('N'))
			_frameSelected = _frameSelected >= FRAMEINFOMANAGER->GetSize() - 1 ? 0 : _frameSelected += 1;
		break;
	case CTRL_SETCORRELATION:
		setMap();
		break;
	case CTRL_SETTRIGGER:
		setMap();
		break;
	case CTRL_BACKGROUND:
		setBack();
		break;
	}
	if (KEYMANAGER->isOnceKeyDown('M'))
	{
		if (tabOpen == false) tabOpen = true;

		else tabOpen = false;
	}
	CAMERAMANAGER->setCamera(camera);//������--���߿� �÷��̾� �߽� ��ǥ ������ �� ī�޶���.

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

	//IMAGEMANAGER->FindImage("���")->Render(Vector2(720-camera.x,648-camera.y));
	//CAMERAMANAGER->render(IMAGEMANAGER->FindImage("���1"), Vector2(IMAGEMANAGER->FindImage("���1")->GetSize().x/2, IMAGEMANAGER->FindImage("���1")->GetSize().y/2));
	if (_backGround != NULL)CAMERAMANAGER->render(_backGround, Vector2(_backGround->GetSize().x / 2+480, _backGround->GetSize().y / 2));
	else{
		for (int i = 0; i < TILEY; i++)
		{
			for (int j = 0; j < TILEX; j++)
			{
				CAMERAMANAGER->FrameRender(IMAGEMANAGER->FindImage("TerrainSample"),
					Vector2(_tiles[i*TILEX + j].rc.left + TILESIZE / 2, _tiles[i*TILEX + j].rc.top + TILESIZE / 2),
					_tiles[i*TILEX + j].terrainFrameX, _tiles[i*TILEX + j].terrainFrameY);
				//if (!_tiles[i*TILEX + j].isMapOn)continue;
				if (KEYMANAGER->isToggleKey(VK_TAB))
				{
					_D2DRenderer->DrawRectangle(_tiles[i*TILEX + j].rc, D2DRenderer::DefaultBrush::White);
				}
				if (_tiles[i*TILEX + j].isCollider)_D2DRenderer->FillRectangle(_tiles[i*TILEX + j].rc, D2D1::ColorF::Red, 0.4);
				if (_tiles[i*TILEX + j].terrain == TR_TRIGGER)_D2DRenderer->FillRectangle(_tiles[i*TILEX + j].rc, D2D1::ColorF::Aqua, 0.5);
			}
		}
	}
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			if (KEYMANAGER->isToggleKey(VK_TAB))
			{
				//_D2DRenderer->DrawRectangle(_tiles[i*TILEX + j].rc, D2DRenderer::DefaultBrush::White);
				CAMERAMANAGER->renderRc(_tiles[i*TILEX + j].rc, D2D1::ColorF::White, 1.0f, 1.0f);
			}
			//if (_tiles[i*TILEX + j].isCollider)_D2DRenderer->FillRectangle(_tiles[i*TILEX + j].rc, D2D1::ColorF::Red, 0.4);
			if (_tiles[i*TILEX + j].isCollider)CAMERAMANAGER->renderFillRc(_tiles[i*TILEX + j].rc,  D2D1::ColorF::Red,0.4);
			//if (_tiles[i*TILEX + j].terrain == TR_TRIGGER)_D2DRenderer->FillRectangle(_tiles[i*TILEX + j].rc, D2D1::ColorF::Aqua, 0.5);
			if (_tiles[i*TILEX + j].terrain == TR_TRIGGER)CAMERAMANAGER->renderFillRc(_tiles[i*TILEX + j].rc, D2D1::ColorF::Aqua, 0.5);
		}
	}
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			if (_tiles[i*TILEX + j].obj == OBJ_NONE)continue;
			//�߰��� ��ġ�ϰ� �ʹٸ� �̰ɾ�����. ����Ʈ ��Ÿ
			CAMERAMANAGER->render(IMAGEMANAGER->FindImage(_tiles[i*TILEX + j].keyName),
				Vector2(_tiles[i*TILEX + j].rc.left + TILESIZE / 2,
					_tiles[i*TILEX + j].rc.bottom - IMAGEMANAGER->FindImage(_tiles[i*TILEX + j].keyName)->GetSize().y / 2));

			//���������� �ٰ��� �ϸ� �̰ɾ���
			/*CAMERAMANAGER->render(IMAGEMANAGER->FindImage(_tiles[i*TILEX + j].keyName),
				Vector2(_tiles[i*TILEX + j].rc.right,
					_tiles[i*TILEX + j].rc.bottom - IMAGEMANAGER->FindImage(_tiles[i*TILEX + j].keyName)->GetSize().y / 2));*/

			if (_tiles[i*TILEX + j].obj == OBJ_CORELATION)CAMERAMANAGER->renderFillRc(_tiles[i*TILEY + j].rc, D2D1::ColorF::Yellow, 0.5);
		}
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
		//desc: �������� �������� �߰� date 2021/2/1 by pju
		if (_crtSelect == CTRL_OBJDRAW)
		{
			int previous = _objSelected + 1;
			int	next = _objSelected - 1;

			if (next < 1)
				next = OBJSIZE;
			if (previous > OBJSIZE)
				previous = 1;

			_D2DRenderer->FillRectangle(Vector2(790, 150), Vector2(245, 245), Pivot::Center, D2D1::ColorF::Enum::White, 1.0f);
			IMAGEMANAGER->FindImage(onjName + to_string(next))->SetScale(0.82);
			IMAGEMANAGER->FindImage(onjName + to_string(next))->Render(Vector2(790, 150));
			_D2DRenderer->DrawRectangle(Vector2(790, 150), Vector2(245, 245), Pivot::Center, D2D1::ColorF::Enum::DarkGray, 1.0f, 5);

			_D2DRenderer->FillRectangle(Vector2(1130, 150), Vector2(245, 245), Pivot::Center, D2D1::ColorF::Enum::White, 1.0f);
			IMAGEMANAGER->FindImage(onjName + to_string(previous))->SetScale(0.82);
			IMAGEMANAGER->FindImage(onjName + to_string(previous))->Render(Vector2(1130, 150));
			_D2DRenderer->DrawRectangle(Vector2(1130, 150), Vector2(245, 245), Pivot::Center, D2D1::ColorF::Enum::DarkGray, 1.0f, 5);

			_D2DRenderer->FillRectangle(Vector2(950, 150), Vector2(275, 280), Pivot::Center, D2D1::ColorF::Enum::White, 1.0f);
			IMAGEMANAGER->FindImage(onjName + to_string(_objSelected))->Render(Vector2(950, 150));
			_D2DRenderer->DrawRectangle(Vector2(950, 150), Vector2(275, 280), Pivot::Center, D2D1::ColorF::Enum::DarkGray, 1.0f, 5);


			IMAGEMANAGER->FindImage("ȭ��ǥ����")->SetScale(0.65f);
			IMAGEMANAGER->FindImage("ȭ��ǥ����")->Render(Vector2(634, 150));

			IMAGEMANAGER->FindImage("ȭ��ǥ������")->SetScale(0.65f);
			IMAGEMANAGER->FindImage("ȭ��ǥ������")->Render(Vector2(1265, 150));

			IMAGEMANAGER->FindImage("ȭ��ǥ")->SetScale(0.65f);
			IMAGEMANAGER->FindImage("ȭ��ǥ")->Render(Vector2(950, 280));
		}

		//desc: �������� �������� �߰� date 2021/1/29 by pju
		//���� ������ �̹��� �����ֱ�
		//���� ������ �÷��̸���Ʈ �����ϰ� �;���
		if (_crtSelect == CTRL_SETFRAMETILE)
		{
			int previous2 = _frameSelected + 2;
			int	next2 = _frameSelected - 2;
			int previous = _frameSelected + 1;
			int	next = _frameSelected - 1;

			if (next < 0)
				next = FRAMEINFOMANAGER->GetSize() - 1;
			if (previous > FRAMEINFOMANAGER->GetSize() - 1)
				previous = 0;
			if (next2 < 0)
				next2 = FRAMEINFOMANAGER->GetSize() - 2;
			if (previous2 > FRAMEINFOMANAGER->GetSize() - 2)
				previous2 = 0;

			_D2DRenderer->FillRectangle(Vector2(720, 150), Vector2(140, 140), Pivot::Center, D2D1::ColorF::Enum::White, 1.0f);
			FRAMEINFOMANAGER->FindImage(next2)->SetScale(0.61);
			FRAMEINFOMANAGER->FindImage(next2)->FrameRender(Vector2((720), 150), 0, 0);
			_D2DRenderer->DrawRectangle(Vector2(720, 150), Vector2(140, 140), Pivot::Center, D2D1::ColorF::Enum::DarkGray, 1.0f, 5);

			_D2DRenderer->FillRectangle(Vector2(820, 150), Vector2(180, 180), Pivot::Center, D2D1::ColorF::Enum::White, 1.0f);
			FRAMEINFOMANAGER->FindImage(next)->SetScale(0.82);
			FRAMEINFOMANAGER->FindImage(next)->FrameRender(Vector2((820), 150), 0, 0);
			_D2DRenderer->DrawRectangle(Vector2(820, 150), Vector2(180, 180), Pivot::Center, D2D1::ColorF::Enum::DarkGray, 1.0f, 5);

			_D2DRenderer->FillRectangle(Vector2(1180, 150), Vector2(140, 140), Pivot::Center, D2D1::ColorF::Enum::White, 1.0f);
			FRAMEINFOMANAGER->FindImage(previous2)->SetScale(0.61);
			FRAMEINFOMANAGER->FindImage(previous2)->FrameRender(Vector2((1180), 150), 0, 0);
			_D2DRenderer->DrawRectangle(Vector2(1180, 150), Vector2(140, 140), Pivot::Center, D2D1::ColorF::Enum::DarkGray, 1.0f, 5);

			_D2DRenderer->FillRectangle(Vector2(1100, 150), Vector2(180, 180), Pivot::Center, D2D1::ColorF::Enum::White, 1.0f);
			FRAMEINFOMANAGER->FindImage(previous)->SetScale(0.82);
			FRAMEINFOMANAGER->FindImage(previous)->FrameRender(Vector2((1100), 150), 0, 0);
			_D2DRenderer->DrawRectangle(Vector2(1100, 150), Vector2(180, 180), Pivot::Center, D2D1::ColorF::Enum::DarkGray, 1.0f, 5);

			_D2DRenderer->FillRectangle(Vector2(950, 150), Vector2(220, 220), Pivot::Center, D2D1::ColorF::Enum::White, 1.0f);
			FRAMEINFOMANAGER->FindImage(_frameSelected)->FrameRender(Vector2((950), 150), 0, 0);
			_D2DRenderer->DrawRectangle(Vector2(950, 150), Vector2(220, 220), Pivot::Center, D2D1::ColorF::Enum::DarkGray, 1.0f, 5);

			_D2DRenderer->FillRectangle(prevArrow.frc, D2D1::ColorF::Enum::LightGreen, 0.95f);
			_D2DRenderer->FillRectangle(nextArrow.frc, D2D1::ColorF::Enum::LightGreen, 0.95f);

			IMAGEMANAGER->FindImage("ȭ��ǥ����")->SetScale(0.65f);
			IMAGEMANAGER->FindImage("ȭ��ǥ����")->Render(Vector2(634, 150));

			IMAGEMANAGER->FindImage("ȭ��ǥ������")->SetScale(0.65f);
			IMAGEMANAGER->FindImage("ȭ��ǥ������")->Render(Vector2(1265, 150));

			IMAGEMANAGER->FindImage("ȭ��ǥ")->SetScale(0.65f);
			IMAGEMANAGER->FindImage("ȭ��ǥ")->Render(Vector2(950, 260));

			D2DINS->RenderText
			(
				950 - 30,
				268,
				FRAMEINFOMANAGER->GetInfo(_frameSelected).kinds == PLAYER ? L"Player" :
				FRAMEINFOMANAGER->GetInfo(_frameSelected).kinds == ENEMY ? L"Enemy" : L"Object",
				22
				//��Ʈ ���� �𸣰ٽ�..
			);
		}
	}
	//��� ���� �����Ϸ��� �κ�
	string temp;
	temp = backName + to_string(backCount);
	//��� ���� �����Ϸ��� �κ�
	if (_crtSelect == CTRL_BACKGROUND && tabOpen)
	{
		IMAGEMANAGER->FindImage(temp)->SetSize(Vector2(600, 270));
		IMAGEMANAGER->FindImage(temp)->Render(Vector2(950, 150));
		IMAGEMANAGER->FindImage("ȭ��ǥ����")->SetScale(0.65f);
		IMAGEMANAGER->FindImage("ȭ��ǥ����")->Render(Vector2(634, 150));

		IMAGEMANAGER->FindImage("ȭ��ǥ������")->SetScale(0.65f);
		IMAGEMANAGER->FindImage("ȭ��ǥ������")->Render(Vector2(1265, 150));
	}
	//IMAGEMANAGER->FindImage("TerrainSample")->Render(Vector2(800, 100));
	//�� �� �����ϴ� �κ�
	if (tabOpen)
	{
		Save.img->Render(Vector2(Save.frc.left + 72, Save.frc.top + 24));
		Load.img->Render(Vector2(Load.frc.left + 72, Load.frc.top + 24));
		Erase.img->Render(Vector2(Erase.frc.left + 72, Erase.frc.top + 24));
		//Prev.img->Render(Vector2(Prev.frc.left + 72, Prev.frc.top + 24));
		//Next.img->Render(Vector2(Next.frc.left + 72, Next.frc.top + 24));
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
		BackGround.img->SetSize(Vector2(144, 48));
		BackGround.img->Render(Vector2(BackGround.frc.left + 72, BackGround.frc.top + 24));
		_D2DRenderer->RenderTextField(BackGround.frc.left, BackGround.frc.top - 5, L"BackGround", 24, 144, 48, D2DRenderer::DefaultBrush::White, DWRITE_TEXT_ALIGNMENT_CENTER);

	}
	if (tabOpen == true)
		Close.img->Render(Vector2(Close.frc.left + 72, Close.frc.top + 24));
	else
		Open.img->Render(Vector2(Open.frc.left + 72, Open.frc.top + 24));

	//Ÿ�Ͽ� ������ �̹��� ��ġ ����
	for (int i = 0; i < _vFrameTile.size(); i++)
	{
		//if (_vFrameTile[i].kinds == PLAYER)  CAMERAMANAGER->renderFillRc(_vFrameTile[i].rc, D2D1::ColorF::Blue, 0.7);
		//else if (_vFrameTile[i].kinds == ENEMY)  CAMERAMANAGER->renderFillRc(_vFrameTile[i].rc, D2D1::ColorF::Black, 0.7);
		//else CAMERAMANAGER->renderFillRc(_vFrameTile[i].rc, D2D1::ColorF::White, 0.7);

		//�Ʒ��ڵ�� ��������� ���ܵ�
		//CAMERAMANAGER->FrameRender
		//(
		//	_vFrameTile[i].img,
		//	//������ �̹��� �������
		//	//1. y���� Ÿ�� ����뿡 ��ġ�ϰ� ������
		//	Vector2((_vFrameTile[i].rc.left + _vFrameTile[i].rc.right) / 2, _vFrameTile[i].rc.bottom - _vFrameTile[i].img->GetSize().y / 2),
		//	//2. y���� ������ �������� ��� ������
		//	//Vector2((_vFrameTile[i].rc.left + _vFrameTile[i].rc.right) / 2, _vFrameTile[i].rc.top),
		//	_vFrameTile[i].frameX, _vFrameTile[i].frameY
		//);

		//���� ����ϴ� ����
		CAMERAMANAGER->FrameRender
		(
			_vFrameTile[i].img,
			Vector2((_vFrameTile[i].rc.left + _vFrameTile[i].rc.right) / 2, _vFrameTile[i].rc.bottom - _vFrameTile[i].img->GetSize().y / 2),
			//������ ���� ����밡 �ƴ� �ϴܿ� ����� �ϱ⿡..
	/*		_vFrameTile[i].keyName.find("��") != string::npos ? (_vFrameTile[i].keyName.size() <= 3 ?
				Vector2((_vFrameTile[i].rc.left + _vFrameTile[i].rc.right) / 2, _vFrameTile[i].rc.top) :
				Vector2((_vFrameTile[i].rc.left + _vFrameTile[i].rc.right) / 2, _vFrameTile[i].rc.bottom - _vFrameTile[i].img->GetSize().y / 2)) :
			Vector2((_vFrameTile[i].rc.left + _vFrameTile[i].rc.right) / 2, _vFrameTile[i].rc.bottom - _vFrameTile[i].img->GetSize().y / 2),*/
			_vFrameTile[i].frameX, _vFrameTile[i].frameY
		);
	}
	if(tabOpen)_D2DRenderer->DrawRectangle(sampleSelec, D2DRenderer::DefaultBrush::White);
	//_D2DRenderer->DrawRectangle(MapRC, D2DRenderer::DefaultBrush::White);
	if(!tabOpen)_D2DRenderer->FillRectangle(tileSelec, D2D1::ColorF::Enum::LightYellow, 0.5);
}

//desc: ������ �ε����� �Ŵ����� �̿��Ͽ� ���� date: 2021/1/29 by pju
void mapTool::setFrameIndex()
{
	Vector2 temp;
	if (_vFrameTile.size() <= 0)
		return;

	for (int i = 0; i < _vFrameTile.size(); i++)
	{
		//������ Ű, ������ �ε���, Ʈ���� �������, Ʈ���� ���� (��ǻ� ������ �����ϱ� ����..)
		temp = FRAMEINFOMANAGER->FrameOperation(_vFrameTile[i].keyName, Vector2(_vFrameTile[i].frameX, _vFrameTile[i].frameY), false, true);
		_vFrameTile[i].frameX = temp.x;
		_vFrameTile[i].frameY = temp.y;
	}
}

void mapTool::setCorrelation()
{
}
void mapTool::setTrigger()
{
}
void mapTool::setBack()
{
	if (KEYMANAGER->isOnceKeyDown('P'))
	{
		backCount--;
		if (backCount <1)backCount = 93;
	}
	if (KEYMANAGER->isOnceKeyDown('N'))
	{
		backCount++;
		if (backCount > 93)backCount = 1;
	}
	if (KEYMANAGER->isOnceKeyDown(VK_RETURN))
	{
		_backGround = IMAGEMANAGER->FindImage(backName + to_string(backCount));
	}
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
	//Prev.img = IMAGEMANAGER->FindImage("Prev");
	//Next.img = IMAGEMANAGER->FindImage("Next");
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
	setCor.img = IMAGEMANAGER->FindImage("ȭ��ǥ����");	//desc: ��ư �߰� date 2021/1/29 by pju
	setTri.img = IMAGEMANAGER->FindImage("ȭ��ǥ������");
	BackGround.img = IMAGEMANAGER->FindImage("Bar");

	Save.frc = RectMakePivot(Vector2(0+72+48, 660), Vector2(144, 48), Pivot::Center);
	Load.frc = RectMakePivot(Vector2(144 + 10+72+48, 660), Vector2(144, 48), Pivot::Center);
	Erase.frc = RectMakePivot(Vector2(288 + 10+72+48, 660), Vector2(144, 48), Pivot::Center);
	terrain.frc = RectMakePivot(Vector2(432+10+72+48, 660), Vector2(144, 48), Pivot::Center);
	Object.frc = RectMakePivot(Vector2(576+10+72+48, 660), Vector2(144, 48), Pivot::Center);
	//Prev.frc = RectMakePivot(Vector2(720+10+72+48,660), Vector2(144, 48), Pivot::Center);
	//Next.frc = RectMakePivot(Vector2(864+10+72 +48, 660), Vector2(144, 48), Pivot::Center);
	Close.frc = RectMakePivot(Vector2(1008 + 10+72+48, 660), Vector2(144, 48), Pivot::Center);
	Open.frc = RectMakePivot(Vector2(1008 + 10 + 72+48, 660), Vector2(144, 48), Pivot::Center);
	Collider.frc = RectMakePivot(Vector2(0 + 72 + 48, 600), Vector2(144, 48), Pivot::Center);
	Player.frc = RectMakePivot(Vector2(144 + 10 + 72 + 48, 600), Vector2(144, 48), Pivot::Center);
	Enemy.frc = RectMakePivot(Vector2(288 + 10 + 72 + 48, 600), Vector2(144, 48), Pivot::Center);
	FrameObj.frc = RectMakePivot(Vector2(432 + 10 + 72 + 48, 600), Vector2(144, 48), Pivot::Center);
	setCor.frc = RectMakePivot(Vector2(576 + 10 + 72 + 48, 600), Vector2(144, 48), Pivot::Center);
	setTri.frc = RectMakePivot(Vector2(720 + 10 + 72 + 48, 600), Vector2(144, 48), Pivot::Center);
	prevArrow.frc = RectMakePivot(Vector2(637, 150), Vector2(33, 33), Pivot::Center);
	nextArrow.frc = RectMakePivot(Vector2(1262, 150), Vector2(33, 33), Pivot::Center);
	BackGround.frc= RectMakePivot(Vector2(864 + 10 + 72 + 48, 600), Vector2(144, 48), Pivot::Center);
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

	//�츮�� �� Ÿ�ϸ� ����
	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			//_tiles[i*TILEX + j].rc = RectMakePivot(Vector2(j*TILESIZE, i*TILESIZE), Vector2(j*TILESIZE, i*TILESIZE),Pivot::Center);
			//_tiles[i*TILEX + j].rc = RectMakePivot(Vector2(j*TILESIZE + TILESIZE / 2 - camera.x, i*TILESIZE + TILESIZE / 2 - camera.y), Vector2(TILESIZE, TILESIZE), Pivot::Center);
			_tiles[i*TILEX + j].rc = RectMakePivot(Vector2(j*TILESIZE + TILESIZE / 2 , i*TILESIZE + TILESIZE / 2 ), Vector2(TILESIZE, TILESIZE), Pivot::Center);
		}
	}
	cout << _tiles[0].rc.left << endl << _tiles[0].rc.top << endl;
	//�ܵ�� �ʱ� Ÿ�ϸ� ����
	for (int i = 0; i < TILEX * TILEY; ++i)
	{
		_tiles[i].terrainFrameX = 5;
		_tiles[i].terrainFrameY = 0;
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
				if (Vector2InRect(&_sampleTile[i*SAMPLETILEX + j].rcTile, &Vector2(_ptMouse.x, _ptMouse.y)) && _crtSelect == CTRL_TERRAINDRAW) //&& isterrain) by pju �̳����� ��ü�ϱ� ����
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
		if (_leftButtonDown)
		{
			for (int i = 0; i < TILEY; ++i)
			{
				for (int j = 0; j < TILEX; ++j)
				{
					if (Vector2InRect(&_tiles[i*TILEX + j].rc, &CAMERAMANAGER->getWorldMouse()))
					{
						if (_crtSelect == CTRL_TERRAINDRAW)
						{
							_tiles[i*TILEX + j].terrainFrameX = _currentTile.x;
							_tiles[i*TILEX + j].terrainFrameY = _currentTile.y;

							_tiles[i*TILEX + j].terrain = terrainSelect(_currentTile.x, _currentTile.y);
						}
						else if (_crtSelect == CTRL_ERASER)
						{
							_tiles[i*TILEX + j].keyName = "";
							_tiles[i*TILEX + j].obj = OBJ_NONE;
						}
						else if (_crtSelect == CTRL_COLLIDER)
						{
							if (!_tiles[i*TILEX + j].isCollider)
							{						
								_tiles[i*TILEX + j].isCollider = true;
								_leftButtonDown = false;
								cout << _tiles[i*TILEX + j].isCollider<<endl;
							}
							
							else 
							{ 
								_tiles[i*TILEX + j].isCollider = false; 
								_leftButtonDown=false;
							}
						}
						else if (_crtSelect == CTRL_SETCORRELATION)
						{
							if (_tiles[i*TILEX + j].obj == OBJ_LOOK)_tiles[i*TILEX + j].obj = OBJ_CORELATION;
							else if (_tiles[i*TILEX + j].obj == OBJ_CORELATION)_tiles[i*TILEX + j].obj = OBJ_LOOK;
							_leftButtonDown = false;
						}
						else if (_crtSelect == CTRL_SETTRIGGER)
						{
							if (_tiles[i*TILEX + j].terrain == TR_TRIGGER)_tiles[i*TILEX + j].terrain = TR_GRASS;
							else _tiles[i*TILEX + j].terrain = TR_TRIGGER; 
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
		//if (Vector2InRect(&Prev.frc,&Vector2(_ptMouse)))  _crtSelect = CTRL_PREV;
		//if (Vector2InRect(&Next.frc,&Vector2(_ptMouse)))  _crtSelect = CTRL_NEXT; 
		if (Vector2InRect(&terrain.frc,&Vector2(_ptMouse)))_crtSelect = CTRL_TERRAINDRAW;
		if (Vector2InRect(&Object.frc,&Vector2(_ptMouse)))_crtSelect = CTRL_OBJDRAW;
		if (Vector2InRect(&Collider.frc,&Vector2(_ptMouse)))_crtSelect = CTRL_COLLIDER;
		if (Vector2InRect(&FrameObj.frc,&Vector2(_ptMouse)))_crtSelect = CTRL_SETFRAMETILE;
		if (Vector2InRect(&setCor.frc,&Vector2(_ptMouse)))_crtSelect = CTRL_SETCORRELATION;
		if (Vector2InRect(&setTri.frc,&Vector2(_ptMouse)))_crtSelect = CTRL_SETTRIGGER;

		//desc : ���� ���� �ε��� �� date 2021/2/1 by pju
		if (Vector2InRect(&prevArrow.frc, &Vector2(_ptMouse)) && tabOpen)
		{
			//��ư Ŭ�� �� ���� �̹�����
			switch (_crtSelect)
			{
			case CTRL_OBJDRAW:
				_objSelected = _objSelected <= 1 ? OBJSIZE  : _objSelected -= 1;
				break;
			case CTRL_SETFRAMETILE:
				_frameSelected = _frameSelected <= 0 ? FRAMEINFOMANAGER->GetSize() - 1 : _frameSelected -= 1;
				break;
			case CTRL_BACKGROUND:
				backCount--;
				if (backCount < 1)backCount = 93;
				break;
			}
		}
		if (Vector2InRect(&nextArrow.frc, &Vector2(_ptMouse)) && tabOpen)
		{
			//��ư Ŭ�� �� ���� �̹�����
			switch (_crtSelect)
			{
			case CTRL_OBJDRAW:
				_objSelected = _objSelected >= OBJSIZE ? 1 : _objSelected += 1;
				break;
			case CTRL_SETFRAMETILE:
				_frameSelected = _frameSelected >= FRAMEINFOMANAGER->GetSize() - 1 ? 0 : _frameSelected += 1;
				break;
			case CTRL_BACKGROUND:
				backCount++;
				if (backCount > 93)backCount = 1;
				break;
			}
		}
		if(Vector2InRect(&BackGround.frc,&Vector2(_ptMouse)))_crtSelect=CTRL_BACKGROUND;
	}
}

void mapTool::mapMove()
{
	if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
	{
		camera.x+= 48;
		cout << camera.x << endl;

	}
	if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
	{
		camera.x -= 48;
		cout << camera.x << endl;

	}
	if (KEYMANAGER->isOnceKeyDown(VK_UP))
	{
		camera.y -= 48;
		cout << camera.y<<endl;
	}
	if (KEYMANAGER->isOnceKeyDown(VK_DOWN))
	{
		camera.y += 48;
		cout << camera.y << endl;

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

	string temp;
	if (GetSaveFileName(&openFileName) != 0)    // �ε����� 1���� �����ϱ� ������ ����
	{
		switch (openFileName.nFilterIndex)
		{
		case 1:
			_tiles->camera = camera;
			temp = backName + to_string(backCount);
			_tiles->backGroundName = temp;
			cout << _tiles->camera.x;
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

	string temp;
	if (GetOpenFileName(&openFileName) != 0)    // �ε����� 1���� �����ϱ� ������ ����
	{
		switch (openFileName.nFilterIndex)
		{
		case 1:
			file = CreateFile(strFilePath, GENERIC_READ, NULL, NULL,
				OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

			ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);
			camera = _tiles->camera;
			_backGround = IMAGEMANAGER->FindImage(_tiles->backGroundName);

			//desc: ������ ���ڿ� �ʿ��� ���� ���� date: 2021/2/2 by pju
			//string ��׶����� ����� ���Ѵ�.
			switch (sizeof(_tiles->backGroundName))
			{
			case 3:
				//������ 3�� ��� 3��° �ڸ����� �ڸ���
				temp = _tiles->backGroundName.substr(3);
				//int������ ��ȯ
				backCount = atoi(temp.c_str());
				break;
			case 4:
				//������ 4�� ��� 4��° �ڸ����� �ڸ���
				temp = _tiles->backGroundName.substr(4);
				//int������ ��ȯ
				backCount = atoi(temp.c_str());
				break;
			case 5:
				//������ 5�� ��� 5��° �ڸ����� �ڸ���
				temp = _tiles->backGroundName.substr(5);
				//int������ ��ȯ
				backCount = atoi(temp.c_str());
				break;
			default:
				//�⺻ ù��° ���
				backCount = 1;
				break;
			}

			CAMERAMANAGER->setCamera(camera);
			CloseHandle(file);
			break;
		}
	}
	SetCurrentDirectory(curDirectoryPath);  // ����� ��θ� ���� ��η� ����
	_crtSelect = CTRL_TERRAINDRAW;
}

//desc: ���Ϳ��� �Ŵ����� map���� ��ȸ date: 2020/1/29 by pju
void mapTool::getFrameTile()
{
	_vFrameTile.clear();
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			//�̹��� ������ �������� �ʴ´ٸ� ���ʿ��� �ݺ����� �ǳʶڴ�.
			if (!FRAMEINFOMANAGER->GetSize())
				return;

			if (!FRAMEINFOMANAGER->KeyCheck(_tiles[i*TILEX + j].keyName))
				continue;

			//��Ʈ ����
			FloatRect rc;
			rc = RectMakePivot(Vector2(_tiles[i*TILEX + j].rc.left + TILESIZE / 2, _tiles[i*TILEX + j].rc.top + TILESIZE / 2), Vector2(TILESIZE, TILESIZE), Pivot::Center);

			FRAMEATTRIBUTE tempKinds = FRAMEINFOMANAGER->GetAttribute(_tiles[i*TILEX + j].keyName);

			tagFrameTile temp;
			temp.rc = rc;
			temp.kinds = tempKinds;
			temp.keyName = _tiles[i*TILEX + j].keyName;
			temp.indexX = i;
			temp.indexY = j;
			temp.frameX = 0;
			temp.frameY = 0;
			temp.img = FRAMEINFOMANAGER->FindImage(_tiles[i*TILEX + j].keyName);
			addFrameTile(temp);
		}
	}
}

void mapTool::erase()
{
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
			if (Vector2InRect(&_tiles[i*TILEX + j].rc, &CAMERAMANAGER->getWorldMouse()))
			{
				//��Ʈ ����
				rc = RectMakePivot(Vector2(_tiles[i*TILEX + j].rc.left + TILESIZE / 2, _tiles[i*TILEX + j].rc.top + TILESIZE / 2), Vector2(TILESIZE, TILESIZE), Pivot::Center);

				FRAMEATTRIBUTE tempKinds = FRAMEINFOMANAGER->GetAttribute(_frameSelected);
				//_frameSelected
				tagFrameTile temp;
				temp.rc = rc;
				temp.kinds = tempKinds;
				temp.keyName = FRAMEINFOMANAGER->FindKey(_frameSelected);
				temp.indexX = i;
				temp.indexY = j;
				temp.frameX = 0;
				temp.frameY = 0;
				temp.img = FRAMEINFOMANAGER->FindImage(_frameSelected);	//������ �̹��� ���Ϳ��� Ű���� �־ �̹����� �ִ´�

				//�̹� �����ϴ� Ÿ���� ������ �̹����� ��ġ �õ��� ��� addFrameTile�Լ����� true�� ��ȯ
				isDel = addFrameTile(temp);

				//���ŵǾ�� �ϴ� ����
				if (isDel)	//�ش� Ÿ���� ������ Ű���� �ʱ�ȭ ���ش�.
				{
					_tiles[i*TILEX + j].keyName = "";
					_tiles[i*TILEX + j].attribute = NONE;
				}
				else	   //�ش� Ÿ���� ������ Ű���� �־��ش�.
				{
					_tiles[i*TILEX + j].keyName = temp.keyName;
					//_tiles[i*TILEX + j].keyName = FRAMEINFOMANAGER->FindKey(_frameSelected);
					_tiles[i*TILEX + j].attribute = FRAMEINFOMANAGER->GetAttribute(_frameSelected);
				}

				//�Ʒ� ���ʿ��� ������ �����Ѵ�.
				if (tempKinds != PLAYER)
					break;
			}
			//�浹���� ���� ��� �÷��̾�� �ߺ� ��ġ�� �Ұ���
			else if (FRAMEINFOMANAGER->KeyCheck(_tiles[i*TILEX + j].keyName))
			{
				FRAMEATTRIBUTE tempKinds = FRAMEINFOMANAGER->GetAttribute(_frameSelected);

				if (tempKinds != PLAYER)
					continue;

				//�̹� Ÿ�Ͽ��� �÷��̾� Ÿ���� �����ϴ� ��� ���� Ÿ�Ͽ� ������ �̹��� Ű���� �����Ѵ�.
				_tiles[i*TILEX + j].keyName = "";
				_tiles[i*TILEX + j].attribute = NONE;
			}
		}
	}
}

void mapTool::setObjTile()
{
	//���� ���� �����ְų� Ȥ�� ���콺 ���ʹ�ư Ŭ�� ���°� �ƴ� ��
	if (tabOpen || !_leftButtonDown)
		return;

	//���ʹ�ư �ѹ� �������� ������ �Ǿ�� �ϱ⿡ �ʱ�ȭ ���ش�.
	_leftButtonDown = false;

	//���� ����
	bool isDel = false;

	string temp;
	//FloatRect rc;
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			//�ش� Ÿ���� �ε����� ���콺�� �浹�� ���(Ÿ�Ͽ� ��ġ �õ��� ���)
			if (Vector2InRect(&_tiles[i*TILEX + j].rc, &CAMERAMANAGER->getWorldMouse()))
			{
				//��Ʈ ����
				//rc = RectMakePivot(Vector2(_tiles[i*TILEX + j].rc.left + TILESIZE / 2, _tiles[i*TILEX + j].rc.top + TILESIZE / 2), Vector2(TILESIZE, TILESIZE), Pivot::Center);

				//���ŵǾ�� �ϴ� ����
				if (isDel)	//�ش� Ÿ���� ������ Ű���� �ʱ�ȭ ���ش�.
					_tiles[i*TILEX + j].obj = OBJ_NONE;
				else	   //�ش� Ÿ���� ������ Ű���� �־��ش�.
				{
					temp = onjName + to_string(_objSelected);
					_tiles[i*TILEX + j].keyName = temp;
					_tiles[i*TILEX + j].obj = OBJ_LOOK;
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

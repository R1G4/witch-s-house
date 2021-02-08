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
	//IMAGEMANAGER->AddImage("배경", L"Image/mapTool/stageMap/008.png");

	setButton();
	// png파일 변환시 y축의 약수여야함
	_y_rect_num = 1;	

	//프레임 이미지 간격 초기화
	_frameInterval = 0;	 

	//선택된 이미지(샘플 및 타일 삽입용)
	_frameSelected = 0;	 
	_objSelected = 1;

	setup();
	_crtSelect = CTRL_TERRAINDRAW;
	camera = Vector2(0,0);
	
	CAMERAMANAGER->setConfig(0, 0, TILESIZEX, TILESIZEY, 0, 0, TILESIZEX, TILESIZEY );
	tabOpen = true;
	sampleBack = RectMakePivot(Vector2(600, 0), Vector2(1280-600, 300), Pivot::LeftTop);
	backCount = 1;
	backName = "배경";
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
	CAMERAMANAGER->setWorldMouse(_ptMouse);//Vector2InRect써서 뭔가 검증해야하면 만들어줘야함.(ptInrect써야한다면 반드시)
	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))_leftButtonDown = true;
	if (KEYMANAGER->isOnceKeyUp(VK_LBUTTON))_leftButtonDown = false;
	//if (KEYMANAGER->isOnceKeyUp('P')|| KEYMANAGER->isOnceKeyUp('N')) _change_number = false;	// (prev next) f2키에서 떼면 순서 변환완료

	tileSelect();
	switch (_crtSelect)
	{
	case CTRL_SAVE:
		save();
		break;
	case CTRL_LOAD:
		load(); //불러오기
		getFrameTile();	//불러온 데이터의 토대로 프레임 타일 배치
		_crtSelect = CTRL_TERRAINDRAW;	//키를 입력 받지 않는 이상 계속 로드하기에 강제로 상태 초기화
		break;
	case CTRL_TERRAINDRAW:
		setMap();
		break;
	case CTRL_OBJDRAW:	
		setObjTile();
		//단축키로 오브젝트의 순서를 바꿀수있다.
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
		//프레임이미지 배치
		setFrameTile();
		//단축키로 프레임이미지의 순서를 바꿀수있다.
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
	CAMERAMANAGER->setCamera(camera);//무조건--나중에 플레이어 중심 좌표 넣으면 됨 카메라대신.

	// 배치된 프레임 이미지의 인덱스 설정 
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

	//IMAGEMANAGER->FindImage("배경")->Render(Vector2(720-camera.x,648-camera.y));
	//CAMERAMANAGER->render(IMAGEMANAGER->FindImage("배경1"), Vector2(IMAGEMANAGER->FindImage("배경1")->GetSize().x/2, IMAGEMANAGER->FindImage("배경1")->GetSize().y/2));
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
			//중간에 배치하고 싶다면 이걸쓰세요. 디폴트 센타
			CAMERAMANAGER->render(IMAGEMANAGER->FindImage(_tiles[i*TILEX + j].keyName),
				Vector2(_tiles[i*TILEX + j].rc.left + TILESIZE / 2,
					_tiles[i*TILEX + j].rc.bottom - IMAGEMANAGER->FindImage(_tiles[i*TILEX + j].keyName)->GetSize().y / 2));

			//오른쪽으로 붙고자 하면 이걸쓰고
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
		//if (isterrain)by pju 일단 isterrain제거를 위해 주석
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
		//desc: 이전으로 다음으로 추가 date 2021/2/1 by pju
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


			IMAGEMANAGER->FindImage("화살표왼쪽")->SetScale(0.65f);
			IMAGEMANAGER->FindImage("화살표왼쪽")->Render(Vector2(634, 150));

			IMAGEMANAGER->FindImage("화살표오른쪽")->SetScale(0.65f);
			IMAGEMANAGER->FindImage("화살표오른쪽")->Render(Vector2(1265, 150));

			IMAGEMANAGER->FindImage("화살표")->SetScale(0.65f);
			IMAGEMANAGER->FindImage("화살표")->Render(Vector2(950, 280));
		}

		//desc: 이전으로 다음으로 추가 date 2021/1/29 by pju
		//샘플 프레임 이미지 보여주기
		//옛날 아이팟 플레이리스트 구현하고 싶었음
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

			IMAGEMANAGER->FindImage("화살표왼쪽")->SetScale(0.65f);
			IMAGEMANAGER->FindImage("화살표왼쪽")->Render(Vector2(634, 150));

			IMAGEMANAGER->FindImage("화살표오른쪽")->SetScale(0.65f);
			IMAGEMANAGER->FindImage("화살표오른쪽")->Render(Vector2(1265, 150));

			IMAGEMANAGER->FindImage("화살표")->SetScale(0.65f);
			IMAGEMANAGER->FindImage("화살표")->Render(Vector2(950, 260));

			D2DINS->RenderText
			(
				950 - 30,
				268,
				FRAMEINFOMANAGER->GetInfo(_frameSelected).kinds == PLAYER ? L"Player" :
				FRAMEINFOMANAGER->GetInfo(_frameSelected).kinds == ENEMY ? L"Enemy" : L"Object",
				22
				//폰트 아직 모르겟숴..
			);
		}
	}
	//배경 샘플 랜딩하려는 부분
	string temp;
	temp = backName + to_string(backCount);
	//배경 샘플 랜딩하려는 부분
	if (_crtSelect == CTRL_BACKGROUND && tabOpen)
	{
		IMAGEMANAGER->FindImage(temp)->SetSize(Vector2(600, 270));
		IMAGEMANAGER->FindImage(temp)->Render(Vector2(950, 150));
		IMAGEMANAGER->FindImage("화살표왼쪽")->SetScale(0.65f);
		IMAGEMANAGER->FindImage("화살표왼쪽")->Render(Vector2(634, 150));

		IMAGEMANAGER->FindImage("화살표오른쪽")->SetScale(0.65f);
		IMAGEMANAGER->FindImage("화살표오른쪽")->Render(Vector2(1265, 150));
	}
	//IMAGEMANAGER->FindImage("TerrainSample")->Render(Vector2(800, 100));
	//각 탭 랜딩하는 부분
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

	//타일에 프레임 이미지 배치 랜더
	for (int i = 0; i < _vFrameTile.size(); i++)
	{
		//if (_vFrameTile[i].kinds == PLAYER)  CAMERAMANAGER->renderFillRc(_vFrameTile[i].rc, D2D1::ColorF::Blue, 0.7);
		//else if (_vFrameTile[i].kinds == ENEMY)  CAMERAMANAGER->renderFillRc(_vFrameTile[i].rc, D2D1::ColorF::Black, 0.7);
		//else CAMERAMANAGER->renderFillRc(_vFrameTile[i].rc, D2D1::ColorF::White, 0.7);

		//아래코드는 참고용으로 냄겨둠
		//CAMERAMANAGER->FrameRender
		//(
		//	_vFrameTile[i].img,
		//	//프레임 이미지 랜딩방식
		//	//1. y축을 타일 가운대에 위치하고 싶을때
		//	Vector2((_vFrameTile[i].rc.left + _vFrameTile[i].rc.right) / 2, _vFrameTile[i].rc.bottom - _vFrameTile[i].img->GetSize().y / 2),
		//	//2. y축을 바텀의 기준으로 잡고 싶을때
		//	//Vector2((_vFrameTile[i].rc.left + _vFrameTile[i].rc.right) / 2, _vFrameTile[i].rc.top),
		//	_vFrameTile[i].frameX, _vFrameTile[i].frameY
		//);

		//실제 사용하는 랜더
		CAMERAMANAGER->FrameRender
		(
			_vFrameTile[i].img,
			Vector2((_vFrameTile[i].rc.left + _vFrameTile[i].rc.right) / 2, _vFrameTile[i].rc.bottom - _vFrameTile[i].img->GetSize().y / 2),
			//문같은 경우는 가운대가 아닌 하단에 맞춰야 하기에..
	/*		_vFrameTile[i].keyName.find("문") != string::npos ? (_vFrameTile[i].keyName.size() <= 3 ?
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

//desc: 프레임 인덱스를 매니저를 이용하여 수정 date: 2021/1/29 by pju
void mapTool::setFrameIndex()
{
	Vector2 temp;
	if (_vFrameTile.size() <= 0)
		return;

	for (int i = 0; i < _vFrameTile.size(); i++)
	{
		//프레임 키, 프레임 인덱스, 트리거 사용유무, 트리거 무시 (사실상 맵툴용 구분하기 위해..)
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
	setCor.img = IMAGEMANAGER->FindImage("화살표왼쪽");	//desc: 버튼 추가 date 2021/1/29 by pju
	setTri.img = IMAGEMANAGER->FindImage("화살표오른쪽");
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
			//_tiles[i*TILEX + j].rc = RectMakePivot(Vector2(j*TILESIZE + TILESIZE / 2 - camera.x, i*TILESIZE + TILESIZE / 2 - camera.y), Vector2(TILESIZE, TILESIZE), Pivot::Center);
			_tiles[i*TILEX + j].rc = RectMakePivot(Vector2(j*TILESIZE + TILESIZE / 2 , i*TILESIZE + TILESIZE / 2 ), Vector2(TILESIZE, TILESIZE), Pivot::Center);
		}
	}
	cout << _tiles[0].rc.left << endl << _tiles[0].rc.top << endl;
	//잔디로 초기 타일맵 세팅
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
				if (Vector2InRect(&_sampleTile[i*SAMPLETILEX + j].rcTile, &Vector2(_ptMouse.x, _ptMouse.y)) && _crtSelect == CTRL_TERRAINDRAW) //&& isterrain) by pju 이넘으로 대체하기 위해
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

		//desc : 이전 이후 인덱스 비교 date 2021/2/1 by pju
		if (Vector2InRect(&prevArrow.frc, &Vector2(_ptMouse)) && tabOpen)
		{
			//버튼 클릭 시 이전 이미지로
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
			//버튼 클릭 시 이후 이미지로
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

	ZeroMemory(&openFileName, sizeof(OPENFILENAME));    // 구조체를 0으로 셋업
	openFileName.lStructSize = sizeof(OPENFILENAME);
	openFileName.hwndOwner = _hWnd;
	openFileName.lpstrTitle = "저장";
	openFileName.lpstrFileTitle = strFileTitle; // strFileTitle
	openFileName.lpstrFilter = "Every File(*.*)\0*.*\0Text File\0*.txt;*.doc\0";
	openFileName.lpstrFile = strFilePath; // strFilePath
	openFileName.nMaxFile = MAX_PATH;
	openFileName.nMaxFileTitle = MAX_PATH;

	string temp;
	if (GetSaveFileName(&openFileName) != 0)    // 인덱스가 1부터 시작하기 때문에 지정
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
	SetCurrentDirectory(curDirectoryPath);  // 변경된 경로를 원래 경로로 설정

	_crtSelect = CTRL_TERRAINDRAW;
}

void mapTool::load()
{
	HANDLE file;
	DWORD read;

	OPENFILENAME openFileName;
	static char strFileTitle[MAX_PATH], strFileExtension[10], strFilePath[100];
	TCHAR curDirectoryPath[256];

	GetCurrentDirectory(256, curDirectoryPath);         // GetOpenFileName 호출하면 기본 경로명이 바뀌기 때문에 기본 경로명 미리 저장

	ZeroMemory(&openFileName, sizeof(openFileName));    // 구조체를 0으로 셋업
	openFileName.lStructSize = sizeof(openFileName);
	openFileName.hwndOwner = _hWnd;
	openFileName.lpstrTitle = "로드";
	openFileName.lpstrFileTitle = strFileTitle;
	openFileName.lpstrFile = strFilePath;
	openFileName.lpstrFilter = "Every File(*.*)\0*.*\0Text File\0*.txt;*.doc\0";
	openFileName.nMaxFile = MAX_PATH;
	openFileName.nMaxFileTitle = MAX_PATH;

	string temp;
	if (GetOpenFileName(&openFileName) != 0)    // 인덱스가 1부터 시작하기 때문에 지정
	{
		switch (openFileName.nFilterIndex)
		{
		case 1:
			file = CreateFile(strFilePath, GENERIC_READ, NULL, NULL,
				OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

			ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);
			camera = _tiles->camera;
			_backGround = IMAGEMANAGER->FindImage(_tiles->backGroundName);

			//desc: 파일의 문자에 필요한 정보 추출 date: 2021/2/2 by pju
			//string 백그라운드의 사이즈를 구한다.
			switch (sizeof(_tiles->backGroundName))
			{
			case 3:
				//사이즈 3일 경우 3번째 자리부터 자르기
				temp = _tiles->backGroundName.substr(3);
				//int형으로 변환
				backCount = atoi(temp.c_str());
				break;
			case 4:
				//사이즈 4일 경우 4번째 자리부터 자르기
				temp = _tiles->backGroundName.substr(4);
				//int형으로 변환
				backCount = atoi(temp.c_str());
				break;
			case 5:
				//사이즈 5일 경우 5번째 자리부터 자르기
				temp = _tiles->backGroundName.substr(5);
				//int형으로 변환
				backCount = atoi(temp.c_str());
				break;
			default:
				//기본 첫번째 배경
				backCount = 1;
				break;
			}

			CAMERAMANAGER->setCamera(camera);
			CloseHandle(file);
			break;
		}
	}
	SetCurrentDirectory(curDirectoryPath);  // 변경된 경로를 원래 경로로 설정
	_crtSelect = CTRL_TERRAINDRAW;
}

//desc: 백터에서 매니저의 map에서 조회 date: 2020/1/29 by pju
void mapTool::getFrameTile()
{
	_vFrameTile.clear();
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			//이미지 정보가 존재하지 않는다면 불필요한 반복문은 건너뛴다.
			if (!FRAMEINFOMANAGER->GetSize())
				return;

			if (!FRAMEINFOMANAGER->KeyCheck(_tiles[i*TILEX + j].keyName))
				continue;

			//렉트 생성
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
	//예외 탭이 켜져있거나 혹은 마우스 왼쪽버튼 클릭 상태가 아닐 시
	if (tabOpen || !_leftButtonDown)
		return;

	//왼쪽버튼 한번 눌렀을대 설정이 되어야 하기에 초기화 해준다.
	_leftButtonDown = false;

	//삭제 여부
	bool isDel = false;
	FloatRect rc;
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			//해당 타일의 인덱스가 마우스와 충돌한 경우(타일에 배치 시도한 경우)
			if (Vector2InRect(&_tiles[i*TILEX + j].rc, &CAMERAMANAGER->getWorldMouse()))
			{
				//렉트 생성
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
				temp.img = FRAMEINFOMANAGER->FindImage(_frameSelected);	//프레임 이미지 백터에서 키값을 넣어서 이미지를 넣는당

				//이미 존재하는 타입의 프레임 이미지를 배치 시도한 경우 addFrameTile함수에서 true를 반환
				isDel = addFrameTile(temp);

				//제거되어야 하는 상태
				if (isDel)	//해당 타일의 프레임 키값을 초기화 해준다.
				{
					_tiles[i*TILEX + j].keyName = "";
					_tiles[i*TILEX + j].attribute = NONE;
				}
				else	   //해당 타일의 프레임 키값을 넣어준다.
				{
					_tiles[i*TILEX + j].keyName = temp.keyName;
					//_tiles[i*TILEX + j].keyName = FRAMEINFOMANAGER->FindKey(_frameSelected);
					_tiles[i*TILEX + j].attribute = FRAMEINFOMANAGER->GetAttribute(_frameSelected);
				}

				//아래 불필요한 연산은 제외한다.
				if (tempKinds != PLAYER)
					break;
			}
			//충돌하지 않은 경우 플레이어는 중복 배치가 불가능
			else if (FRAMEINFOMANAGER->KeyCheck(_tiles[i*TILEX + j].keyName))
			{
				FRAMEATTRIBUTE tempKinds = FRAMEINFOMANAGER->GetAttribute(_frameSelected);

				if (tempKinds != PLAYER)
					continue;

				//이미 타일에는 플레이어 타입이 존재하는 경우 기존 타일에 프레임 이미지 키값을 제거한다.
				_tiles[i*TILEX + j].keyName = "";
				_tiles[i*TILEX + j].attribute = NONE;
			}
		}
	}
}

void mapTool::setObjTile()
{
	//예외 탭이 켜져있거나 혹은 마우스 왼쪽버튼 클릭 상태가 아닐 시
	if (tabOpen || !_leftButtonDown)
		return;

	//왼쪽버튼 한번 눌렀을대 설정이 되어야 하기에 초기화 해준다.
	_leftButtonDown = false;

	//삭제 여부
	bool isDel = false;

	string temp;
	//FloatRect rc;
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			//해당 타일의 인덱스가 마우스와 충돌한 경우(타일에 배치 시도한 경우)
			if (Vector2InRect(&_tiles[i*TILEX + j].rc, &CAMERAMANAGER->getWorldMouse()))
			{
				//렉트 생성
				//rc = RectMakePivot(Vector2(_tiles[i*TILEX + j].rc.left + TILESIZE / 2, _tiles[i*TILEX + j].rc.top + TILESIZE / 2), Vector2(TILESIZE, TILESIZE), Pivot::Center);

				//제거되어야 하는 상태
				if (isDel)	//해당 타일의 프레임 키값을 초기화 해준다.
					_tiles[i*TILEX + j].obj = OBJ_NONE;
				else	   //해당 타일의 프레임 키값을 넣어준다.
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
			//이미 존재하는 타일이 존재하는 경우
			if (_vFrameTile[i].kinds == _frameTile.kinds &&
				_vFrameTile[i].indexX == _frameTile.indexX &&
				_vFrameTile[i].indexY == _frameTile.indexY)
			{
				//배치된 타일의 프레임 이미지를 제거한다.
				_vFrameTile.erase(_vFrameTile.begin() + i);
				//true를 반환하면 그 후 저장 및 로드용 타일에서도 제거한다.
				return true;
			}
			//그렇지 않을 경우 2개 이상의 플레이어는 존재하지 못하므로
			//기존의 배치된 타일의 프레임 이미지를 제거한다.
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
				//배치된 타일의 프레임 이미지를 제거한다.
				_vFrameTile.erase(_vFrameTile.begin() + i);
				//true를 반환하면 그 후 저장 및 로드용 타일에서도 제거한다.
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
				//배치된 타일의 프레임 이미지를 제거한다.
				_vFrameTile.erase(_vFrameTile.begin() + i);
				//true를 반환하면 그 후 저장 및 로드용 타일에서도 제거한다.
				return true;
			}
		}
		break;
	}

	//프레임 이미지를 추가한다.
	//이미지를 배치 및 랜더하는 백터
	_vFrameTile.push_back(_frameTile);
	//false를 반환 할 경우 저장 및 로드용 타일에서는 제거되지 않는다.
	return false;
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

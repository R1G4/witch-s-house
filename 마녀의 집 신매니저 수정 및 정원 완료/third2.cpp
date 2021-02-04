#include "stdafx.h"
#include "third2.h"

third2::third2()
{
}

third2::~third2()
{
}

HRESULT third2::init()
{
	IMAGEMANAGER->AddImage("3f_doar", L"Image/obj/3f_2_doar.png");
	_frogOut = IMAGEMANAGER->AddFrameImage("frogOut", L"Image/tempFrameImg/frogOut2.png", 10, 1);
	_bar = IMAGEMANAGER->AddImage("bar", L"Image/obj/bar2.png");

	CAMERAMANAGER->setConfig(0, 0, TILESIZEX, TILESIZEY, 0, 0, TILESIZEX, TILESIZEY);

	_player = new Player;
	load();

	_player->init();
	_player->setState(CHR_IDLE);
	_player->setDirec(CHRDIREC_DOWN);

	camera.x = _player->getPlayerLocX();
	camera.y = _player->getPlayerLocY();
	CAMERAMANAGER->setCamera(camera);

	_rcAlpha = 1.0f;
	_rcAlphaChange = 0.03f;
	_x = WINSIZEX / 2;
	_y = WINSIZEY / 2;
	_rc = RectMakePivot(Vector2(_x, _y), Vector2(270, 75), Pivot::Center);

	_count = 0;
	_frame = 0;
	_isText = false;
	_leftClick = false;
	_rightClick = false;
	_playerRender = true;
	return S_OK;
}

void third2::release()
{
}

void third2::update()
{
	//텍스트칸 렉트 알파값 조절
	rcAlphaChange();
	//카메라, 플레이어 좌표 연동
	camera.x = _player->getPlayerLocX();
	camera.y = _player->getPlayerLocY();
	CAMERAMANAGER->setCamera(Vector2(camera.x - WINSIZEX / 2, camera.y - WINSIZEY / 2));

	//타일충돌
	tileCollision();

	//_count++;
	//if(_count %10 == 0) cout << _player->getPlayerFrc().left / TILESIZE << endl;

	//프레임 이미지 
	//플레이어 기본 랜더가 아닐때
	//if (!_playerRender)
	//{
	//	//카운트 플플
	//	_count++;
	//	//카운트 50일떄
	//	if (_count % 50 == 0)
	//	{
	//		//프레임 플플
	//		_frame++;
	//		//프레임이 10보다 커지면
	//		if (_frame >= 10 )		
	//		{
	//			_frame = 9;
	//			_count = 0;
	//			cout << "rrrr" << endl;
	//			_playerRender = true;	//기본랜더 켜줘
	//			_isText = false;		//초기 백그라운드 화면 보여줘
	//		}
	//	}
	//}

	switch (_text)
	{
	case LEFT:

		break;
	case RIGHT:

		break;
	case UP:

		break;
	case DOWN:

		break;
	case OPENTEXT:

		break;
	case OPENLEFT:
		break;
	case OPENRIGHT:

		break;
	default:
		break;
	}

	//
	if (!_isText && _text != OPENTEXT)
	{
		_player->update();
		//_rc = RectMakePivot(Vector2(camera.x - 360, camera.y - 145), Vector2(270, 75), Pivot::Center);
		_rc = RectMakePivot(Vector2(camera.x - 360, camera.y - 145), Vector2(270, 75), Pivot::Center);
	}


	//문앞에 섰을때 선택지 뜨게
	if (_player->getPlayerFrc().left / TILESIZE <= 18 && KEYMANAGER->isOnceKeyDown(VK_SPACE) && _text != OPENTEXT)
	{
		_isText = true;

	}

	if (_isText)
	{
		if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
		{
			_rc = RectMakePivot(Vector2(_x + 290, _y - 10), Vector2(270, 75), Pivot::Center);
			_text = RIGHT;
		}
		if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
		{
			//_rc = RectMakePivot(Vector2(_x - 160, _y - 8), Vector2(270, 75), Pivot::Center);
			_rc = RectMakePivot(Vector2(camera.x - 360, camera.y - 145), Vector2(270, 75), Pivot::Center);
			_text = LEFT;
		}
		if (KEYMANAGER->isOnceKeyDown(VK_UP))
		{
			_rc = RectMakePivot(Vector2(_x + 70, _y - 150), Vector2(270, 75), Pivot::Center);
			_text = UP;
		}
		if (KEYMANAGER->isOnceKeyDown(VK_DOWN))
		{
			_rc = RectMakePivot(Vector2(_x + 70, _y + 120), Vector2(270, 75), Pivot::Center);
			_text = DOWN;
		}

		//대화창 활성화 시에 상호작용 스페이스바 작동안되서 A로 해둠 ㅜ

		//우측 텍스트 눌렀을때 플레이어 업데이트, 첫번째 텍스트창 닫음
		if (_text == RIGHT && KEYMANAGER->isOnceKeyDown('A'))
		{
			_player->update();
			_isText = false;
		}
		//좌측 텍스트 눌렀을때
		if (_text == LEFT && KEYMANAGER->isOnceKeyDown('A'))
		{
			_isText = false;
			_text = OPENTEXT;
		}

	}

	//첫번째 텍스트창에서 좌측 눌렀을때 두번째 텍스트 창 오픈
	if (_text == OPENTEXT)
	{
		//두번째 텍스트창에서 좌측 눌렀을때
		if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
		{
			_rc = RectMakePivot(Vector2(camera.x - 360, camera.y - 145), Vector2(270, 75), Pivot::Center);
			_leftClick = true;
		}
		//두번째 텍스트창에서 우측 눌렀을때
		if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
		{
			_rc = RectMakePivot(Vector2(_x + 290, _y - 10), Vector2(270, 75), Pivot::Center);
			_rightClick = true;
		}
	}

	if (_leftClick && KEYMANAGER->isOnceKeyDown('A'))
	{
		_text = OPENLEFT;
	}


	if (_rightClick && KEYMANAGER->isOnceKeyDown('A'))
	{
		_isText = true;	//첫번째 텍스트창으로 이동. 다만들면 변수명 _isText로 바꾸기
		_text = OPENRIGHT;	//일단 창 닫으려고 임시로 넣음. 2번째 텍스트창 오픈할때를 건드려야할듯

	}

}

void third2::render()
{
	//첫번째 텍스트창 off일때
	if (!_isText)
	{
		CAMERAMANAGER->render(_backGround, Vector2(_backGround->GetSize().x / 2 + 480, _backGround->GetSize().y / 2));
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
					if (_tiles[i*TILEX + j].terrain == TR_TRIGGER)CAMERAMANAGER->renderFillRc(_tiles[i*TILEX + j].rc, D2D1::ColorF::Aqua, 0.5);
				}

			}
		}
		_player->render();

		for (int i = 0; i < TILEY; i++)
		{
			for (int j = 0; j < TILEX; j++)
			{
				if (_tiles[i*TILEX + j].obj == OBJ_NONE)continue;
				//IMAGEMANAGER->FindImage(_tiles[i*TILEX + j].keyName)->SetAlpha(0.5);
				CAMERAMANAGER->render(IMAGEMANAGER->FindImage(_tiles[i*TILEX + j].keyName),
					Vector2(_tiles[i*TILEX + j].rc.left + TILESIZE / 2, _tiles[i*TILEX + j].rc.top));
			}
		}

		CAMERAMANAGER->render(IMAGEMANAGER->FindImage("3f_doar"), Vector2(WINSIZEX / 2 + 180, WINSIZEY / 2 + 60));
	}


	//첫번째 택스트창 on일때
	if (_isText)
	{
		CAMERAMANAGER->render(_backGround, Vector2(_backGround->GetSize().x / 2 + 480, _backGround->GetSize().y / 2));
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
					if (_tiles[i*TILEX + j].terrain == TR_TRIGGER)CAMERAMANAGER->renderFillRc(_tiles[i*TILEX + j].rc, D2D1::ColorF::Aqua, 0.5);
				}

			}
		}
		_player->render();


		for (int i = 0; i < TILEY; i++)
		{
			for (int j = 0; j < TILEX; j++)
			{
				if (_tiles[i*TILEX + j].obj == OBJ_NONE)continue;
				//IMAGEMANAGER->FindImage(_tiles[i*TILEX + j].keyName)->SetAlpha(0.5);
				CAMERAMANAGER->render(IMAGEMANAGER->FindImage(_tiles[i*TILEX + j].keyName),
					Vector2(_tiles[i*TILEX + j].rc.left + TILESIZE / 2, _tiles[i*TILEX + j].rc.top));
			}
		}

		CAMERAMANAGER->render(IMAGEMANAGER->FindImage("3f_doar"), Vector2(WINSIZEX / 2 + 180, WINSIZEY / 2 + 60));


		CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(camera.x - 180, camera.y - 10));	//왼쪽
		//CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(WINSIZEX / 2 + 20, WINSIZEY / 2 + 120));	//왼쪽
		CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(WINSIZEX / 2 + 470, WINSIZEY / 2 + 120));	//오른쪽
		CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(WINSIZEX / 2 + 250, WINSIZEY / 2 - 20));	//위
		CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(WINSIZEX / 2 + 250, WINSIZEY / 2 + 250));	//아래

		//D2DINS->RenderText(350, 330, L"개구리를 넣는다", RGB(100, 100, 100), 0.85f, 30);
		D2DINS->GetInstance()->RenderText(330, 330, L"엿보기 창으로 엿본다", RGB(255, 255, 255), 0.85f, 27);
		D2DINS->GetInstance()->RenderText(810, 330, L"아무것도 하지 않는다", RGB(255, 255, 255), 0.85f, 27);
		D2DINS->GetInstance()->RenderText(650, 190, L"귀를 댄다", RGB(255, 255, 255), 0.85f, 27);
		D2DINS->GetInstance()->RenderText(650, 460, L"문을 연다", RGB(255, 255, 255), 0.85f, 27);

		D2DINS->FillRectangle(_rc, D2D1::ColorF::Enum::WhiteSmoke, _rcAlpha / 5.5);
		D2DINS->GetInstance()->DrawRectangle(_rc, D2D1::ColorF::White, _rcAlpha, 1.0f);

	}

	//텍스트 열린 상태에서 한 문장 선택했을때
	if (_text == OPENTEXT  /*&& !_leftClick && !_rightClick*/)
	{
		CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(camera.x - 180, camera.y - 10));	//왼쪽
		CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(WINSIZEX / 2 + 470, WINSIZEY / 2 + 120)); // 오른쪽


		D2DINS->GetInstance()->RenderTextField(330, 320, L"\t개구리를\n \t창문으로 넣는다", RGB(255, 255, 255), 27, 270, 50, 0.85f, DWRITE_TEXT_ALIGNMENT_CENTER);
		D2DINS->GetInstance()->RenderText(800, 330, L"아무것도 하지 않는다", RGB(255, 255, 255), 0.85f, 27);

		D2DINS->FillRectangle(_rc, D2D1::ColorF::Enum::WhiteSmoke, _rcAlpha / 5.5);
		D2DINS->GetInstance()->DrawRectangle(_rc, D2D1::ColorF::White, _rcAlpha, 1.0f);
	}

	//두번째 텍스트창에서 좌측 눌렀을때 플레이어 렌더 false해주고
	if (_text == OPENLEFT)
	{
		_player->setFrameX(0);
		_player->setFrameSpeed(20);
		_playerRender = false;
		_text = CHANGEIMG;	//체인지 이미지 말고 다른걸로 바꾸기
	}
	//플레이어 기본상태 잠시 지워주고 개구리 던지는 이미지 랜더
	if (!_playerRender)
	{
		_player->setChrImg(_frogOut);
		_player->setDirec(CHRDIREC_DOWN);
		if (_player->getFrameX() >= IMAGEMANAGER->FindImage("frogOut")->GetMaxFrameX() - 1)
		{
			_playerRender = true;
			_player->setState(CHR_IDLE);
			_player->setDirec(CHRDIREC_LEFT);
		}
	}

}

void third2::rcAlphaChange()
{
	if (_rcAlpha >= 0.9f)
		_rcAlphaChange = -0.03f;

	if (_rcAlpha <= 0.35f)
		_rcAlphaChange = 0.03f;

	_rcAlpha += _rcAlphaChange;
}

void third2::tileCollision()
{
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			if (IntersectRectToRect(&_player->getPlayerFrc(), &_tiles[i*TILEX + j].rc) && _tiles[i*TILEX + j].isCollider)
			{
				switch (_player->getPdirec())
				{
				case CHRDIREC_DOWN:
					_player->setPLocaY(_tiles[i*TILEX + j].rc.top - TILESIZE / 4 * 3);
					break;
				case CHRDIREC_LEFT:
					_player->setPLocaX(_tiles[i*TILEX + j].rc.right + 4);
					break;
				case CHRDIREC_RIGHT:
					_player->setPLocaX(_tiles[i*TILEX + j].rc.left - TILESIZE / 4 * 3);
					break;
				case CHRDIREC_UP:
					_player->setPLocaY(_tiles[i*TILEX + j].rc.bottom + 4);
					break;
				}
			}
		}
	}
}

void third2::load()
{
	HANDLE file;
	DWORD read;
	file = CreateFile("Stage/3f_second.map", GENERIC_READ, NULL, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);
	camera = _tiles->camera;
	_backGround = IMAGEMANAGER->FindImage(_tiles->backGroundName);
	cout << _tiles->camera.x;
	for (int i = 0; i < TILEX*TILEY; i++)
	{
		if (_tiles[i].attribute == PLAYER)
		{
			_player->setStart(i%TILEX, i / TILEX);
			break;
		}
		if (_tiles[i].attribute == OBJ)
		{

		}
	}
	CloseHandle(file);
}
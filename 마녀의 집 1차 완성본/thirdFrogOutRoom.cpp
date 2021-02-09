#include "stdafx.h"
#include "thirdFrogOutRoom.h"

thirdFrogOutRoom::thirdFrogOutRoom()
{
}

thirdFrogOutRoom::~thirdFrogOutRoom()
{
}

HRESULT thirdFrogOutRoom::init()
{
	IMAGEMANAGER->AddImage("3f_doar", L"Image/obj/3f_2_doar.png");
	_frogOut = IMAGEMANAGER->AddFrameImage("frogOut", L"Image/tempFrameImg/frogOut2.png", 10, 1);
	_bar = IMAGEMANAGER->AddImage("bar", L"Image/obj/bar2.png");
	IMAGEMANAGER->AddImage("dialogueBar", L"Image/UI/dialogueBar.png");

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
	_rc = RectMakePivot(Vector2(0, 0), Vector2(0, 0), Pivot::Center);

	_count = 0;
	_frame = 0;
	_isText = false;
	_leftClick = false;
	_rightClick = false;
	_dialogue = false;
	_playerRender = true;
	_isStopToRead = false;
	_leftText2 = false;
	_leftText3 = false;

	return S_OK;
}

void thirdFrogOutRoom::release()
{
}

void thirdFrogOutRoom::update()
{
	//텍스트칸 렉트 알파값 조절
	rcAlphaChange();
	//카메라, 플레이어 좌표 연동
	camera.x = _player->getPlayerLocX();
	camera.y = _player->getPlayerLocY();
	CAMERAMANAGER->setCamera(Vector2(camera.x - WINSIZEX / 2, camera.y - WINSIZEY / 2));

	//타일충돌
	tileCollision();
	//바닥에 떨어진 일지 읽기
	readBook();

	switch (_text)
	{
	case TEXTLEFT:

		break;
	case TEXTRIGHT:

		break;
	case TEXTUP:

		break;
	case TEXTDOWN:

		break;
	case OPENTEXT:

		break;
	case OPENLEFT:
		break;
	case OPENRIGHT:

		break;
	case OPENUP:

		break;
	case OPENDOWN:

		break;
	case DEAD:

		break;
	default:
		break;
	}

	//텍스트창 or 다이어로그 열려있으면 플레이어 안움직이게
	if (!_isText && !_dialogue && !_leftText3 && _text != OPENLEFT && _text != OPENDOWN)
	{
		_player->update();
	}


	//문앞에 섰을때 상호작용 키 누르면 텍스트 on
	if (_player->getPlayerFrc().left / TILESIZE <= 18 && _text != OPENLEFT && !_dialogue)
	{
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
			_isText = true;

	}

	//텍스트가 열린 상태일때 방향키누르면 선택렉트 생성
	if (_isText)
	{
		if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
		{
			SOUNDMANAGER->play("cursor", 0.5f);
			_rc = RectMakePivot(Vector2(_x + 290, _y - 5), Vector2(270, 75), Pivot::Center);
			_text = TEXTRIGHT;
		}
		if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
		{
			SOUNDMANAGER->play("cursor", 0.5f);
			//_rc = RectMakePivot(Vector2(_x - 160, _y - 8), Vector2(270, 75), Pivot::Center);
			_rc = RectMakePivot(Vector2(camera.x - 360, camera.y - 135), Vector2(270, 75), Pivot::Center);
			_text = TEXTLEFT;
		}
		if (KEYMANAGER->isOnceKeyDown(VK_UP))
		{
			SOUNDMANAGER->play("cursor", 0.5f);
			_rc = RectMakePivot(Vector2(_x + 70, _y - 145), Vector2(270, 75), Pivot::Center);
			_text = TEXTUP;
		}
		if (KEYMANAGER->isOnceKeyDown(VK_DOWN))
		{
			SOUNDMANAGER->play("cursor", 0.5f);
			_rc = RectMakePivot(Vector2(_x + 70, _y + 125), Vector2(270, 75), Pivot::Center);
			_text = TEXTDOWN;
		}

		//대화창 활성화 시에 상호작용 스페이스바 작동안되서 A로 해둠 ㅜ

		//우측 텍스트 눌렀을때(아무것도 하지 않는다) 플레이어 업데이트on, 첫번째 텍스트창 닫음
		if (_text == TEXTRIGHT && KEYMANAGER->isStayKeyDown('A'))
		{
			SOUNDMANAGER->play("click", 0.3f);
			//상호작용 키 눌렀을때 렉트 사라지게하고 텍스트창 꺼주기
			_rc = RectMakePivot(Vector2(0, 0), Vector2(0, 0), Pivot::Center);
			_isText = false;
		}
		//좌측 텍스트 눌렀을때(엿보기 구멍으로 본다) 다이어로그 on
		if (_text == TEXTLEFT && KEYMANAGER->isOnceKeyDown('A'))
		{
			SOUNDMANAGER->play("click", 0.3f);
			_rc = RectMakePivot(Vector2(0, 0), Vector2(0, 0), Pivot::Center);
			_isText = false;
			_text = OPENLEFT;		//개구리 던질때로 바까야함
			_dialogue = true;
			_isStopToRead = TEXTMANAGER->setNextScript(true);
			_vScript = TEXTMANAGER->loadFile("dialog/3f/3f_eyes.txt");
			_isStopToRead = true;
		}
		//위에 텍스트 눌렀을때(귀를 댄다) 다이어로그 on
		if (_text == TEXTUP && KEYMANAGER->isOnceKeyDown('A'))
		{
			SOUNDMANAGER->play("click", 0.3f);
			_rc = RectMakePivot(Vector2(0, 0), Vector2(0, 0), Pivot::Center);
			_isText = false;
			_dialogue = true;
			_isStopToRead = TEXTMANAGER->setNextScript(true);
			_vScript = TEXTMANAGER->loadFile("dialog/3f/3f_ears.txt");
			_isStopToRead = true;

		}
		//아래 텍스트 눌렀을때(문을 연다)
		if (_text == TEXTDOWN && KEYMANAGER->isOnceKeyDown('A'))
		{
			SOUNDMANAGER->play("click", 0.3f);
			_rc = RectMakePivot(Vector2(0, 0), Vector2(0, 0), Pivot::Center);
			_isText = false;
			_text = OPENDOWN;
		}
	}

	//첫번째 텍스트에서 왼쪽(엿보기 창으로 엿본다) 눌렀을때 다이어로그 off
	if (_text == OPENLEFT && _dialogue)
	{
		if (KEYMANAGER->isStayKeyDown('B'))
		{
			_dialogue = false;
			_leftText2 = true;
		}
	}

	//첫번째 텍스트창에서 좌측(엿보기 구멍으로 본다) 눌렀을때 다이어로그 나온 후 두번째 텍스트 창 오픈
	if (_text == OPENLEFT)
	{
		//두번째 텍스트창에서 좌측 눌렀을때
		if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
		{
			SOUNDMANAGER->play("cursor", 0.3f);
			_rc = RectMakePivot(Vector2(camera.x - 360, camera.y - 135), Vector2(270, 75), Pivot::Center);
			_leftClick = true;
			_rightClick = false;
		}
		//두번째 텍스트창에서 우측 눌렀을때
		if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
		{
			SOUNDMANAGER->play("cursor", 0.3f);
			_rc = RectMakePivot(Vector2(_x + 290, _y - 5), Vector2(270, 75), Pivot::Center);
			_rightClick = true;
			_leftClick = false;
		}

		//두번째 텍스트창에서 좌측 텍스트 클릭하면 3번째 텍스트 오픈
		if (_leftClick && KEYMANAGER->isOnceKeyDown('A'))
		{
			SOUNDMANAGER->play("click", 0.3f);
			_leftClick = false;
			_isText = false;
			_leftText2 = false;
			_leftText3 = true;		//두번째창에서 왼쪽텍스트 누르면 3번째 텍스트 오픈

		}

		if (_rightClick && KEYMANAGER->isOnceKeyDown('A'))
		{
			SOUNDMANAGER->play("click", 0.3f);
			_rightClick = false;
			_leftText2 = false;
			_isText = true;
		}
	}

	if (_leftText3)
	{
		//세번째 텍스트창에서 좌측 눌렀을때
		if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
		{
			SOUNDMANAGER->play("cursor", 0.3f);
			_rc = RectMakePivot(Vector2(camera.x - 360, camera.y - 145), Vector2(270, 75), Pivot::Center);
			_leftClick = true;
			_rightClick = false;
		}
		//세번째 텍스트창에서 우측 눌렀을때
		if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
		{
			SOUNDMANAGER->play("cursor", 0.3f);
			_rc = RectMakePivot(Vector2(_x + 290, _y - 5), Vector2(270, 75), Pivot::Center);
			_rightClick = true;
			_leftClick = false;
		}

		//세번째 텍스트창에서 왼쪽 텍스트 누르면 개구리 넣기
		if (_leftClick && (KEYMANAGER->isOnceKeyDown('A') || KEYMANAGER->isOnceKeyDown('B')))
		{
			SOUNDMANAGER->play("getItem", 0.5f);
			_text = CHANGEIMG;
			_rightClick = false;
			_leftText3 = false;
			_leftClick = false;

		}

		if (_rightClick && (KEYMANAGER->isOnceKeyDown('A') || KEYMANAGER->isOnceKeyDown('B')))
		{
			SOUNDMANAGER->play("click", 0.3f);
			_rightClick = false;
			_leftText3 = false;
			_isText = true;

		}

		/*if (!_leftText3)
		{
			_isText = true;
		}*/
	}


	//첫번째 텍스트에서 아래 텍스트(문을 연다) 눌렀을때 선택렉트 보여주고 좌우키 움직이게
	if (_text == OPENDOWN && !_isText)
	{
		if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
		{
			SOUNDMANAGER->play("cursor", 0.3f);
			_rc = RectMakePivot(Vector2(camera.x - 360, camera.y - 135), Vector2(270, 75), Pivot::Center);
			_leftClick = true;
			_rightClick = false;
		}
		if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
		{
			SOUNDMANAGER->play("cursor", 0.3f);
			_rc = RectMakePivot(Vector2(_x + 290, _y - 15), Vector2(270, 75), Pivot::Center);
			_rightClick = true;
			_leftClick = false;
		}

		//아이템(개구리) 없이 문을열고 들어가면 바로 데드신으로 
		if (_leftClick && KEYMANAGER->isOnceKeyDown('A') /*&& 개구리 예외처리 해주기*/)
		{
			SOUNDMANAGER->play("click", 0.3f);
			changeScene();
			_leftClick = false;
		}
		//우측키 눌렀을때(일단 다른대에 되어있어서 주석처리함. 안되면 이거부터 풀기)////////////////////////////////////////////
		if (_rightClick && KEYMANAGER->isOnceKeyDown('A'))
		{
			SOUNDMANAGER->play("click", 0.3f);
			_rc = RectMakePivot(Vector2(0, 0), Vector2(0, 0), Pivot::Center);
			_isText = true;
			_rightClick = false;
		}
	}

}

void thirdFrogOutRoom::render()
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
		//Z-order 벡터에 데이터를 넣어주는 과정
		ZORDER->insert(_player->getPlayerFrc().left, _player->getPlayerFrc().top, ZPLAYER);

		//오브젝트를 넣어주는 과정

		for (int i = 0; i < TILEX*TILEY; i++)
		{
			if (_tiles[i].obj != OBJ_NONE)
			{
				//if (_tiles[i].keyName == "obj5" || _tiles[i].keyName == "obj9")
				ZORDER->insert(_tiles[i].rc.left, _tiles[i].rc.top, IMAGEMANAGER->FindImage(_tiles[i].keyName), ZOBJECT);
			}
		}

		//정렬된 순서대로 랜딩
		//각 인덱스의 타입을 확인하여 타입에 따라 이미지를 랜딩하도록 설계함
		for (int i = 0; i < ZORDER->getZorder().size(); i++)
		{
			if (ZORDER->getZorder()[i].type == ZPLAYER)_player->render();

			if (ZORDER->getZorder()[i].type == ZOBJECT)
			{
				CAMERAMANAGER->render(ZORDER->getZorder()[i].img,
					Vector2(ZORDER->getZorder()[i].x + TILESIZE / 2, ZORDER->getZorder()[i].y - ZORDER->getZorder()[i].img->GetSize().y / 2));
			}

		}
		CAMERAMANAGER->render(IMAGEMANAGER->FindImage("3f_doar"), Vector2(WINSIZEX / 2 + 180, WINSIZEY / 2 + 60));

		//zorder 벡터를 초기화해줌 안하면 느려짐
		ZORDER->release();

	}


	//첫번째 택스트창 on일때
	if (_isText)
	{
		//배경이미지 랜더
		CAMERAMANAGER->render(_backGround, Vector2(_backGround->GetSize().x / 2 + 480, _backGround->GetSize().y / 2));
		//탭키 눌렀을때 타일 랜더
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
		//Z-order 벡터에 데이터를 넣어주는 과정
		ZORDER->insert(_player->getPlayerFrc().left, _player->getPlayerFrc().top, ZPLAYER);

		//오브젝트를 넣어주는 과정

		for (int i = 0; i < TILEX*TILEY; i++)
		{
			if (_tiles[i].obj != OBJ_NONE)
			{
				//if (_tiles[i].keyName == "obj5" || _tiles[i].keyName == "obj9")
				ZORDER->insert(_tiles[i].rc.left, _tiles[i].rc.top, IMAGEMANAGER->FindImage(_tiles[i].keyName), ZOBJECT);
			}
		}

		//정렬된 순서대로 랜딩
		//각 인덱스의 타입을 확인하여 타입에 따라 이미지를 랜딩하도록 설계함
		for (int i = 0; i < ZORDER->getZorder().size(); i++)
		{
			if (ZORDER->getZorder()[i].type == ZPLAYER)_player->render();

			if (ZORDER->getZorder()[i].type == ZOBJECT)
			{
				CAMERAMANAGER->render(ZORDER->getZorder()[i].img,
					Vector2(ZORDER->getZorder()[i].x + TILESIZE / 2, ZORDER->getZorder()[i].y - ZORDER->getZorder()[i].img->GetSize().y / 2));
			}

		}
		CAMERAMANAGER->render(IMAGEMANAGER->FindImage("3f_doar"), Vector2(WINSIZEX / 2 + 180, WINSIZEY / 2 + 60));

		//zorder 벡터를 초기화해줌 안하면 느려짐
		ZORDER->release();

		CAMERAMANAGER->render(IMAGEMANAGER->FindImage("3f_doar"), Vector2(WINSIZEX / 2 + 180, WINSIZEY / 2 + 60));


		CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(camera.x - 180, camera.y - 10));	//왼쪽
		//CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(WINSIZEX / 2 + 20, WINSIZEY / 2 + 120));	//왼쪽
		CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(WINSIZEX / 2 + 470, WINSIZEY / 2 + 120));	//오른쪽
		CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(WINSIZEX / 2 + 250, WINSIZEY / 2 - 20));	//위
		CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(WINSIZEX / 2 + 250, WINSIZEY / 2 + 250));	//아래

		//D2DINS->RenderText(350, 330, L"개구리를 넣는다", RGB(100, 100, 100), 0.85f, 30);
		D2DINS->GetInstance()->RenderText(330, 335, L"엿보기 창으로 엿본다", RGB(255, 255, 255), 0.85f, 27);
		D2DINS->GetInstance()->RenderText(805, 340, L"아무것도 하지 않는다", RGB(255, 255, 255), 0.85f, 27);
		D2DINS->GetInstance()->RenderText(650, 200, L"귀를 댄다", RGB(255, 255, 255), 0.85f, 27);
		D2DINS->GetInstance()->RenderText(650, 470, L"문을 연다", RGB(255, 255, 255), 0.85f, 27);

		D2DINS->FillRectangle(_rc, D2D1::ColorF::Enum::WhiteSmoke, _rcAlpha / 5.5);
		D2DINS->GetInstance()->DrawRectangle(_rc, D2D1::ColorF::White, _rcAlpha, 1.0f);

	}

	//다이어로그 on일때
	//첫번째 텍스트에서 위에(귀를 댄다) 눌렀을때
	if (_dialogue)
	{
		if (_isStopToRead) TEXTMANAGER->renderText();

		if (_text == TEXTUP)
		{
			if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
			{
				_isText = true;
				_dialogue = false;
			}
		}
	}

	//다이어로그 꺼지면 이미지 일단 안보이는곳으로 치워뒀음
	//if (!_dialogue) CAMERAMANAGER->render(IMAGEMANAGER->FindImage("dialogueBar"), Vector2(0, 0));

	//첫번째 텍스트에서 아래(문을 연다) 눌렀을때
	if (_text == OPENDOWN && !_isText)
	{
		CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(camera.x - 180, camera.y - 10));	//왼쪽
		CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(WINSIZEX / 2 + 470, WINSIZEY / 2 + 110));	//오른쪽
		D2DINS->GetInstance()->RenderText(430, 330, L"연다", RGB(255, 255, 255), 0.85f, 27);
		D2DINS->GetInstance()->RenderText(840, 330, L"역시 그만둔다", RGB(255, 255, 255), 0.85f, 27);

		D2DINS->FillRectangle(_rc, D2D1::ColorF::Enum::WhiteSmoke, _rcAlpha / 5.5);
		D2DINS->GetInstance()->DrawRectangle(_rc, D2D1::ColorF::White, _rcAlpha, 1.0f);

	}

	//왼쪽텍스트 눌렀을때 두번째 텍스트
	if (_leftText2)
	{
		CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(camera.x - 180, camera.y - 10));	//왼쪽
		CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(WINSIZEX / 2 + 470, WINSIZEY / 2 + 120)); // 오른쪽


		D2DINS->GetInstance()->RenderTextField(330, 325, L"\t개구리를\n \t창문으로 넣는다", RGB(255, 255, 255), 27, 270, 50, 0.85f, DWRITE_TEXT_ALIGNMENT_CENTER);
		D2DINS->GetInstance()->RenderText(820, 340, L"역시 하지 않는다", RGB(255, 255, 255), 0.85f, 27);

		D2DINS->FillRectangle(_rc, D2D1::ColorF::Enum::WhiteSmoke, _rcAlpha / 5.5);
		D2DINS->GetInstance()->DrawRectangle(_rc, D2D1::ColorF::White, _rcAlpha, 1.0f);
	}
	//왼쪽텍스트 눌렀을때 세번째 텍스트
	if (_leftText3 && !_leftText2 && !_isText && !_dialogue)
	{
		CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(camera.x - 180, camera.y - 10));	//왼쪽
		CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(WINSIZEX / 2 + 470, WINSIZEY / 2 + 120)); // 오른쪽


		D2DINS->GetInstance()->RenderTextField(330, 320, L"정말로?", RGB(255, 255, 255), 27, 270, 50, 0.85f, DWRITE_TEXT_ALIGNMENT_CENTER);
		D2DINS->GetInstance()->RenderText(860, 340, L"... 그만 둔다", RGB(255, 255, 255), 0.85f, 27);

		D2DINS->FillRectangle(_rc, D2D1::ColorF::Enum::WhiteSmoke, _rcAlpha / 5.5);
		D2DINS->GetInstance()->DrawRectangle(_rc, D2D1::ColorF::White, _rcAlpha, 1.0f);
	}




	//텍스트 열린 상태에서 한 문장 선택했을때
	//이거 개구리 눌렀을때로 그대로 넣어주면 됌. 왼쪽은 다이로그로 바꿈
	//if (_text == OPENLEFT  /*&& !_leftClick && !_rightClick*/)
	//{
	//	CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(camera.x - 180, camera.y - 10));	//왼쪽
	//	CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(WINSIZEX / 2 + 470, WINSIZEY / 2 + 120)); // 오른쪽
	//	
	//	
	//	D2DINS->GetInstance()->RenderTextField(330, 320, L"\t개구리를\n \t창문으로 넣는다", RGB(255, 255, 255), 27, 270, 50, 0.85f, DWRITE_TEXT_ALIGNMENT_CENTER);
	//	D2DINS->GetInstance()->RenderText(800, 330, L"아무것도 하지 않는다", RGB(255, 255, 255), 0.85f, 27);
	//	
	//	D2DINS->FillRectangle(_rc, D2D1::ColorF::Enum::WhiteSmoke, _rcAlpha / 5.5);
	//	D2DINS->GetInstance()->DrawRectangle(_rc, D2D1::ColorF::White, _rcAlpha, 1.0f);
	//}

	//두번째 텍스트창에서 좌측 눌렀을때 플레이어 렌더 false해주고
	if (_text == CHANGEIMG)
	{
		_player->setFrameX(0);
		_player->setFrameSpeed(20);
		_playerRender = false;
		_text = DEFAULT;	//체인지 이미지 말고 다른걸로 바꾸기
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
			ITEMMANAGER->useItem("frog");
		}
	}

}

void thirdFrogOutRoom::changeScene()
{
	//아이템창에 개구리가 있다면
	if (ITEMMANAGER->KeyCheck("frog"))
	{
		SOUNDMANAGER->play("blood");
		SCENEMANAGER->changeScene("thirdSnakeDead");
	}
	//개구리가 없다면
	else
	{
		SOUNDMANAGER->play("openDoarLong");
		SCENEMANAGER->changeScene("thirdSnakeRoom");
	}


	////아이템창에 개구리가 없다면
	//if (ITEMMANAGER->getItemKinds("frog"))
	//{
	//	//다음신으로
	//	SOUNDMANAGER->play("blood");
	//	SCENEMANAGER->changeScene("thirdSnakeDead");
	//}
	////아이템창에 개구리가 있다면
	//else
	//{
	//	//데드신으로
	//	SCENEMANAGER->changeScene("thirdSnakeDead");
	//}

}

void thirdFrogOutRoom::rcAlphaChange()
{
	if (_rcAlpha >= 0.9f)
		_rcAlphaChange = -0.03f;

	if (_rcAlpha <= 0.35f)
		_rcAlphaChange = 0.03f;

	_rcAlpha += _rcAlphaChange;
}

void thirdFrogOutRoom::readBook()
{
	//책 읽기
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			if (IntersectRectToRect(&_player->getSearchRc(), &_tiles[i*TILEX + j].rc)
				&& _tiles[i*TILEX + j].terrain == TR_TRIGGER && _player->getPlayerFrc().bottom / TILESIZE <= 10)
			{
				if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
				{
					SOUNDMANAGER->play("nextPage");
					_dialogue = true;
					_isStopToRead = TEXTMANAGER->setNextScript(true);
					_vScript = TEXTMANAGER->loadFile("dialog/3f/3f_book.txt");
					_isStopToRead = true;
				}
			}
			//다이어로그 켜져있을때 스페이스바 누르면 원래대로 돌아가게. 모든 다이어로그 다 포함
			if (_dialogue)
			{
				if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
				{
					_dialogue = false;
				}
			}
		}
	}
}

void thirdFrogOutRoom::tileCollision()
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

void thirdFrogOutRoom::load()
{
	HANDLE file;
	DWORD read;
	file = CreateFile("Stage/3f_frogOutRoom2.map", GENERIC_READ, NULL, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);
	camera = _tiles->camera;
	_backGround = IMAGEMANAGER->FindImage(_tiles->backGroundName);
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
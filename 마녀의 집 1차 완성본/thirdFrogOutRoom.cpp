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
	//�ؽ�Ʈĭ ��Ʈ ���İ� ����
	rcAlphaChange();
	//ī�޶�, �÷��̾� ��ǥ ����
	camera.x = _player->getPlayerLocX();
	camera.y = _player->getPlayerLocY();
	CAMERAMANAGER->setCamera(Vector2(camera.x - WINSIZEX / 2, camera.y - WINSIZEY / 2));

	//Ÿ���浹
	tileCollision();
	//�ٴڿ� ������ ���� �б�
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

	//�ؽ�Ʈâ or ���̾�α� ���������� �÷��̾� �ȿ����̰�
	if (!_isText && !_dialogue && !_leftText3 && _text != OPENLEFT && _text != OPENDOWN)
	{
		_player->update();
	}


	//���տ� ������ ��ȣ�ۿ� Ű ������ �ؽ�Ʈ on
	if (_player->getPlayerFrc().left / TILESIZE <= 18 && _text != OPENLEFT && !_dialogue)
	{
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
			_isText = true;

	}

	//�ؽ�Ʈ�� ���� �����϶� ����Ű������ ���÷�Ʈ ����
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

		//��ȭâ Ȱ��ȭ �ÿ� ��ȣ�ۿ� �����̽��� �۵��ȵǼ� A�� �ص� ��

		//���� �ؽ�Ʈ ��������(�ƹ��͵� ���� �ʴ´�) �÷��̾� ������Ʈon, ù��° �ؽ�Ʈâ ����
		if (_text == TEXTRIGHT && KEYMANAGER->isStayKeyDown('A'))
		{
			SOUNDMANAGER->play("click", 0.3f);
			//��ȣ�ۿ� Ű �������� ��Ʈ ��������ϰ� �ؽ�Ʈâ ���ֱ�
			_rc = RectMakePivot(Vector2(0, 0), Vector2(0, 0), Pivot::Center);
			_isText = false;
		}
		//���� �ؽ�Ʈ ��������(������ �������� ����) ���̾�α� on
		if (_text == TEXTLEFT && KEYMANAGER->isOnceKeyDown('A'))
		{
			SOUNDMANAGER->play("click", 0.3f);
			_rc = RectMakePivot(Vector2(0, 0), Vector2(0, 0), Pivot::Center);
			_isText = false;
			_text = OPENLEFT;		//������ �������� �ٱ����
			_dialogue = true;
			_isStopToRead = TEXTMANAGER->setNextScript(true);
			_vScript = TEXTMANAGER->loadFile("dialog/3f/3f_eyes.txt");
			_isStopToRead = true;
		}
		//���� �ؽ�Ʈ ��������(�͸� ���) ���̾�α� on
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
		//�Ʒ� �ؽ�Ʈ ��������(���� ����)
		if (_text == TEXTDOWN && KEYMANAGER->isOnceKeyDown('A'))
		{
			SOUNDMANAGER->play("click", 0.3f);
			_rc = RectMakePivot(Vector2(0, 0), Vector2(0, 0), Pivot::Center);
			_isText = false;
			_text = OPENDOWN;
		}
	}

	//ù��° �ؽ�Ʈ���� ����(������ â���� ������) �������� ���̾�α� off
	if (_text == OPENLEFT && _dialogue)
	{
		if (KEYMANAGER->isStayKeyDown('B'))
		{
			_dialogue = false;
			_leftText2 = true;
		}
	}

	//ù��° �ؽ�Ʈâ���� ����(������ �������� ����) �������� ���̾�α� ���� �� �ι�° �ؽ�Ʈ â ����
	if (_text == OPENLEFT)
	{
		//�ι�° �ؽ�Ʈâ���� ���� ��������
		if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
		{
			SOUNDMANAGER->play("cursor", 0.3f);
			_rc = RectMakePivot(Vector2(camera.x - 360, camera.y - 135), Vector2(270, 75), Pivot::Center);
			_leftClick = true;
			_rightClick = false;
		}
		//�ι�° �ؽ�Ʈâ���� ���� ��������
		if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
		{
			SOUNDMANAGER->play("cursor", 0.3f);
			_rc = RectMakePivot(Vector2(_x + 290, _y - 5), Vector2(270, 75), Pivot::Center);
			_rightClick = true;
			_leftClick = false;
		}

		//�ι�° �ؽ�Ʈâ���� ���� �ؽ�Ʈ Ŭ���ϸ� 3��° �ؽ�Ʈ ����
		if (_leftClick && KEYMANAGER->isOnceKeyDown('A'))
		{
			SOUNDMANAGER->play("click", 0.3f);
			_leftClick = false;
			_isText = false;
			_leftText2 = false;
			_leftText3 = true;		//�ι�°â���� �����ؽ�Ʈ ������ 3��° �ؽ�Ʈ ����

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
		//����° �ؽ�Ʈâ���� ���� ��������
		if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
		{
			SOUNDMANAGER->play("cursor", 0.3f);
			_rc = RectMakePivot(Vector2(camera.x - 360, camera.y - 145), Vector2(270, 75), Pivot::Center);
			_leftClick = true;
			_rightClick = false;
		}
		//����° �ؽ�Ʈâ���� ���� ��������
		if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
		{
			SOUNDMANAGER->play("cursor", 0.3f);
			_rc = RectMakePivot(Vector2(_x + 290, _y - 5), Vector2(270, 75), Pivot::Center);
			_rightClick = true;
			_leftClick = false;
		}

		//����° �ؽ�Ʈâ���� ���� �ؽ�Ʈ ������ ������ �ֱ�
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


	//ù��° �ؽ�Ʈ���� �Ʒ� �ؽ�Ʈ(���� ����) �������� ���÷�Ʈ �����ְ� �¿�Ű �����̰�
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

		//������(������) ���� �������� ���� �ٷ� ��������� 
		if (_leftClick && KEYMANAGER->isOnceKeyDown('A') /*&& ������ ����ó�� ���ֱ�*/)
		{
			SOUNDMANAGER->play("click", 0.3f);
			changeScene();
			_leftClick = false;
		}
		//����Ű ��������(�ϴ� �ٸ��뿡 �Ǿ��־ �ּ�ó����. �ȵǸ� �̰ź��� Ǯ��)////////////////////////////////////////////
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
	//ù��° �ؽ�Ʈâ off�϶�
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
		//Z-order ���Ϳ� �����͸� �־��ִ� ����
		ZORDER->insert(_player->getPlayerFrc().left, _player->getPlayerFrc().top, ZPLAYER);

		//������Ʈ�� �־��ִ� ����

		for (int i = 0; i < TILEX*TILEY; i++)
		{
			if (_tiles[i].obj != OBJ_NONE)
			{
				//if (_tiles[i].keyName == "obj5" || _tiles[i].keyName == "obj9")
				ZORDER->insert(_tiles[i].rc.left, _tiles[i].rc.top, IMAGEMANAGER->FindImage(_tiles[i].keyName), ZOBJECT);
			}
		}

		//���ĵ� ������� ����
		//�� �ε����� Ÿ���� Ȯ���Ͽ� Ÿ�Կ� ���� �̹����� �����ϵ��� ������
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

		//zorder ���͸� �ʱ�ȭ���� ���ϸ� ������
		ZORDER->release();

	}


	//ù��° �ý�Ʈâ on�϶�
	if (_isText)
	{
		//����̹��� ����
		CAMERAMANAGER->render(_backGround, Vector2(_backGround->GetSize().x / 2 + 480, _backGround->GetSize().y / 2));
		//��Ű �������� Ÿ�� ����
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
		//Z-order ���Ϳ� �����͸� �־��ִ� ����
		ZORDER->insert(_player->getPlayerFrc().left, _player->getPlayerFrc().top, ZPLAYER);

		//������Ʈ�� �־��ִ� ����

		for (int i = 0; i < TILEX*TILEY; i++)
		{
			if (_tiles[i].obj != OBJ_NONE)
			{
				//if (_tiles[i].keyName == "obj5" || _tiles[i].keyName == "obj9")
				ZORDER->insert(_tiles[i].rc.left, _tiles[i].rc.top, IMAGEMANAGER->FindImage(_tiles[i].keyName), ZOBJECT);
			}
		}

		//���ĵ� ������� ����
		//�� �ε����� Ÿ���� Ȯ���Ͽ� Ÿ�Կ� ���� �̹����� �����ϵ��� ������
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

		//zorder ���͸� �ʱ�ȭ���� ���ϸ� ������
		ZORDER->release();

		CAMERAMANAGER->render(IMAGEMANAGER->FindImage("3f_doar"), Vector2(WINSIZEX / 2 + 180, WINSIZEY / 2 + 60));


		CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(camera.x - 180, camera.y - 10));	//����
		//CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(WINSIZEX / 2 + 20, WINSIZEY / 2 + 120));	//����
		CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(WINSIZEX / 2 + 470, WINSIZEY / 2 + 120));	//������
		CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(WINSIZEX / 2 + 250, WINSIZEY / 2 - 20));	//��
		CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(WINSIZEX / 2 + 250, WINSIZEY / 2 + 250));	//�Ʒ�

		//D2DINS->RenderText(350, 330, L"�������� �ִ´�", RGB(100, 100, 100), 0.85f, 30);
		D2DINS->GetInstance()->RenderText(330, 335, L"������ â���� ������", RGB(255, 255, 255), 0.85f, 27);
		D2DINS->GetInstance()->RenderText(805, 340, L"�ƹ��͵� ���� �ʴ´�", RGB(255, 255, 255), 0.85f, 27);
		D2DINS->GetInstance()->RenderText(650, 200, L"�͸� ���", RGB(255, 255, 255), 0.85f, 27);
		D2DINS->GetInstance()->RenderText(650, 470, L"���� ����", RGB(255, 255, 255), 0.85f, 27);

		D2DINS->FillRectangle(_rc, D2D1::ColorF::Enum::WhiteSmoke, _rcAlpha / 5.5);
		D2DINS->GetInstance()->DrawRectangle(_rc, D2D1::ColorF::White, _rcAlpha, 1.0f);

	}

	//���̾�α� on�϶�
	//ù��° �ؽ�Ʈ���� ����(�͸� ���) ��������
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

	//���̾�α� ������ �̹��� �ϴ� �Ⱥ��̴°����� ġ������
	//if (!_dialogue) CAMERAMANAGER->render(IMAGEMANAGER->FindImage("dialogueBar"), Vector2(0, 0));

	//ù��° �ؽ�Ʈ���� �Ʒ�(���� ����) ��������
	if (_text == OPENDOWN && !_isText)
	{
		CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(camera.x - 180, camera.y - 10));	//����
		CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(WINSIZEX / 2 + 470, WINSIZEY / 2 + 110));	//������
		D2DINS->GetInstance()->RenderText(430, 330, L"����", RGB(255, 255, 255), 0.85f, 27);
		D2DINS->GetInstance()->RenderText(840, 330, L"���� �׸��д�", RGB(255, 255, 255), 0.85f, 27);

		D2DINS->FillRectangle(_rc, D2D1::ColorF::Enum::WhiteSmoke, _rcAlpha / 5.5);
		D2DINS->GetInstance()->DrawRectangle(_rc, D2D1::ColorF::White, _rcAlpha, 1.0f);

	}

	//�����ؽ�Ʈ �������� �ι�° �ؽ�Ʈ
	if (_leftText2)
	{
		CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(camera.x - 180, camera.y - 10));	//����
		CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(WINSIZEX / 2 + 470, WINSIZEY / 2 + 120)); // ������


		D2DINS->GetInstance()->RenderTextField(330, 325, L"\t��������\n \tâ������ �ִ´�", RGB(255, 255, 255), 27, 270, 50, 0.85f, DWRITE_TEXT_ALIGNMENT_CENTER);
		D2DINS->GetInstance()->RenderText(820, 340, L"���� ���� �ʴ´�", RGB(255, 255, 255), 0.85f, 27);

		D2DINS->FillRectangle(_rc, D2D1::ColorF::Enum::WhiteSmoke, _rcAlpha / 5.5);
		D2DINS->GetInstance()->DrawRectangle(_rc, D2D1::ColorF::White, _rcAlpha, 1.0f);
	}
	//�����ؽ�Ʈ �������� ����° �ؽ�Ʈ
	if (_leftText3 && !_leftText2 && !_isText && !_dialogue)
	{
		CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(camera.x - 180, camera.y - 10));	//����
		CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(WINSIZEX / 2 + 470, WINSIZEY / 2 + 120)); // ������


		D2DINS->GetInstance()->RenderTextField(330, 320, L"������?", RGB(255, 255, 255), 27, 270, 50, 0.85f, DWRITE_TEXT_ALIGNMENT_CENTER);
		D2DINS->GetInstance()->RenderText(860, 340, L"... �׸� �д�", RGB(255, 255, 255), 0.85f, 27);

		D2DINS->FillRectangle(_rc, D2D1::ColorF::Enum::WhiteSmoke, _rcAlpha / 5.5);
		D2DINS->GetInstance()->DrawRectangle(_rc, D2D1::ColorF::White, _rcAlpha, 1.0f);
	}




	//�ؽ�Ʈ ���� ���¿��� �� ���� ����������
	//�̰� ������ ���������� �״�� �־��ָ� ��. ������ ���̷α׷� �ٲ�
	//if (_text == OPENLEFT  /*&& !_leftClick && !_rightClick*/)
	//{
	//	CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(camera.x - 180, camera.y - 10));	//����
	//	CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(WINSIZEX / 2 + 470, WINSIZEY / 2 + 120)); // ������
	//	
	//	
	//	D2DINS->GetInstance()->RenderTextField(330, 320, L"\t��������\n \tâ������ �ִ´�", RGB(255, 255, 255), 27, 270, 50, 0.85f, DWRITE_TEXT_ALIGNMENT_CENTER);
	//	D2DINS->GetInstance()->RenderText(800, 330, L"�ƹ��͵� ���� �ʴ´�", RGB(255, 255, 255), 0.85f, 27);
	//	
	//	D2DINS->FillRectangle(_rc, D2D1::ColorF::Enum::WhiteSmoke, _rcAlpha / 5.5);
	//	D2DINS->GetInstance()->DrawRectangle(_rc, D2D1::ColorF::White, _rcAlpha, 1.0f);
	//}

	//�ι�° �ؽ�Ʈâ���� ���� �������� �÷��̾� ���� false���ְ�
	if (_text == CHANGEIMG)
	{
		_player->setFrameX(0);
		_player->setFrameSpeed(20);
		_playerRender = false;
		_text = DEFAULT;	//ü���� �̹��� ���� �ٸ��ɷ� �ٲٱ�
	}
	//�÷��̾� �⺻���� ��� �����ְ� ������ ������ �̹��� ����
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
	//������â�� �������� �ִٸ�
	if (ITEMMANAGER->KeyCheck("frog"))
	{
		SOUNDMANAGER->play("blood");
		SCENEMANAGER->changeScene("thirdSnakeDead");
	}
	//�������� ���ٸ�
	else
	{
		SOUNDMANAGER->play("openDoarLong");
		SCENEMANAGER->changeScene("thirdSnakeRoom");
	}


	////������â�� �������� ���ٸ�
	//if (ITEMMANAGER->getItemKinds("frog"))
	//{
	//	//����������
	//	SOUNDMANAGER->play("blood");
	//	SCENEMANAGER->changeScene("thirdSnakeDead");
	//}
	////������â�� �������� �ִٸ�
	//else
	//{
	//	//���������
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
	//å �б�
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
			//���̾�α� ���������� �����̽��� ������ ������� ���ư���. ��� ���̾�α� �� ����
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
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
	//_count++;
	//if(_count %10 == 0) cout << _player->getPlayerFrc().left / TILESIZE << endl;

	//������ �̹��� 
	//�÷��̾� �⺻ ������ �ƴҶ�
	//if (!_playerRender)
	//{
	//	//ī��Ʈ ����
	//	_count++;
	//	//ī��Ʈ 50�ϋ�
	//	if (_count % 50 == 0)
	//	{
	//		//������ ����
	//		_frame++;
	//		//�������� 10���� Ŀ����
	//		if (_frame >= 10 )		
	//		{
	//			_frame = 9;
	//			_count = 0;
	//			cout << "rrrr" << endl;
	//			_playerRender = true;	//�⺻���� ����
	//			_isText = false;		//�ʱ� ��׶��� ȭ�� ������
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
	case OPENUP:

		break;
	case OPENDOWN:

		break;
	case DEAD:

		break;
	default:
		break;
	}

	//�ؽ�Ʈâ�� ���������� �÷��̾� �ȿ����̰�
	if (!_isText && _text != OPENLEFT && _text != OPENDOWN)
	{
		_player->update();
	}


	//���տ� ������ ������ �߰�
	if (_player->getPlayerFrc().left / TILESIZE <= 18 && _text != OPENLEFT)
	{
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
			_isText = true;

	}

	//�ؽ�Ʈ�� ���� �����϶� ����Ű������ ���÷�Ʈ ����
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

		//��ȭâ Ȱ��ȭ �ÿ� ��ȣ�ۿ� �����̽��� �۵��ȵǼ� A�� �ص� ��

		//���� �ؽ�Ʈ ��������(�ƹ��͵� ���� �ʴ´�) �÷��̾� ������Ʈon, ù��° �ؽ�Ʈâ ����
		if (_text == RIGHT && KEYMANAGER->isOnceKeyDown('A'))
		{
			cout << "ttt" << endl;
			_player->update();
			_isText = false;
		}
		//���� �ؽ�Ʈ ��������(������ �������� ����) ���̾�α� on
		if (_text == LEFT && KEYMANAGER->isOnceKeyDown('A'))
		{
			_isText = false;
			_text = OPENLEFT;		//������ �������� �ٱ����
			_dialogue = true;
			_isStopToRead = TEXTMANAGER->setNextScript(true);
			_vScript = TEXTMANAGER->loadFile("dialog/3f/3f_eyes.txt");
			_isStopToRead = true;
		}
		//���� �ؽ�Ʈ ��������(�͸� ���) ���̾�α� on
		if (_text == UP && KEYMANAGER->isOnceKeyDown('A'))
		{
			_isText = false;
			_dialogue = true;
			_isStopToRead = TEXTMANAGER->setNextScript(true);
			_vScript = TEXTMANAGER->loadFile("dialog/3f/3f_ears.txt");
			_isStopToRead = true;

		}
		//�Ʒ� �ؽ�Ʈ ��������(���� ����)
		if (_text == DOWN && KEYMANAGER->isOnceKeyDown('A'))
		{
			_isText = false;
			_text = OPENDOWN;
		}
	}

	//ù��° �ؽ�Ʈâ���� ����(������ �������� ����) �������� �ι�° �ؽ�Ʈ â ����
	if (_text == OPENLEFT)
	{
		//�ι�° �ؽ�Ʈâ���� ���� ��������
		if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
		{
			_rc = RectMakePivot(Vector2(camera.x - 360, camera.y - 145), Vector2(270, 75), Pivot::Center);
			_leftClick = true;
		}
		//�ι�° �ؽ�Ʈâ���� ���� ��������
		if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
		{
			_rc = RectMakePivot(Vector2(_x + 290, _y - 10), Vector2(270, 75), Pivot::Center);
			_rightClick = true;
		}

		//ù��° �ؽ�Ʈâ���� ���� �ؽ�Ʈ ���� �ٸ��� ������ ������ ������ ������ ��� �ȵǰ�
		if (_leftClick && KEYMANAGER->isOnceKeyDown('A'))
		{
			_text = CHANGEIMG;
			_leftClick = false;
		}
	}

	//ù��° �ؽ�Ʈâ���� ���� �ؽ�Ʈ ���� �ٸ��� ������ ������ ������ ������ ��� �ȵǰ�
	//if (_text != OPENDOWN && _leftClick && KEYMANAGER->isOnceKeyDown('A'))
	//{
	//	_text = CHANGEIMG;
	//	_leftClick = false;
	//}


	if (_rightClick && KEYMANAGER->isOnceKeyDown('A'))
	{
		_isText = true;
		_text = OPENRIGHT;	//�ϴ� â �������� �ӽ÷� ����. 2��° �ؽ�Ʈâ �����Ҷ��� �ǵ�����ҵ�
		_rightClick = false;

	}

	//ù��° �ؽ�Ʈ���� �Ʒ� �ؽ�Ʈ(���� ����) �������� ���÷�Ʈ �����ְ� �¿�Ű �����̰�
	if (_text == OPENDOWN && !_isText)
	{
		if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
		{
			cout << "rr" << endl;
			_rc = RectMakePivot(Vector2(camera.x - 360, camera.y - 145), Vector2(270, 75), Pivot::Center);
			_leftClick = true;
			_rightClick = false;
		}
		if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
		{
			_rc = RectMakePivot(Vector2(_x + 290, _y - 10), Vector2(270, 75), Pivot::Center);
			_rightClick = true;
			_leftClick = false;
		}

		//������(������) ���� �������� ���� �ٷ� ��������� 
		if (_leftClick && KEYMANAGER->isOnceKeyDown('A') /*&& ������ ����ó�� ���ֱ�*/)
		{
			cout << "changeScene" << endl;
			changeScene();
			_leftClick = false;
		}
		//����Ű ��������(�ϴ� �ٸ��뿡 �Ǿ��־ �ּ�ó����. �ȵǸ� �̰ź��� Ǯ��)////////////////////////////////////////////
		if (_rightClick && KEYMANAGER->isOnceKeyDown('A'))
		{
			//_player->update();
			//_isText = false;
			//_rightClick = false;
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


	//ù��° �ý�Ʈâ on�϶�
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


		CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(camera.x - 180, camera.y - 10));	//����
		//CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(WINSIZEX / 2 + 20, WINSIZEY / 2 + 120));	//����
		CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(WINSIZEX / 2 + 470, WINSIZEY / 2 + 120));	//������
		CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(WINSIZEX / 2 + 250, WINSIZEY / 2 - 20));	//��
		CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(WINSIZEX / 2 + 250, WINSIZEY / 2 + 250));	//�Ʒ�

		//D2DINS->RenderText(350, 330, L"�������� �ִ´�", RGB(100, 100, 100), 0.85f, 30);
		D2DINS->GetInstance()->RenderText(330, 330, L"������ â���� ������", RGB(255, 255, 255), 0.85f, 27);
		D2DINS->GetInstance()->RenderText(810, 330, L"�ƹ��͵� ���� �ʴ´�", RGB(255, 255, 255), 0.85f, 27);
		D2DINS->GetInstance()->RenderText(650, 190, L"�͸� ���", RGB(255, 255, 255), 0.85f, 27);
		D2DINS->GetInstance()->RenderText(650, 460, L"���� ����", RGB(255, 255, 255), 0.85f, 27);

		D2DINS->FillRectangle(_rc, D2D1::ColorF::Enum::WhiteSmoke, _rcAlpha / 5.5);
		D2DINS->GetInstance()->DrawRectangle(_rc, D2D1::ColorF::White, _rcAlpha, 1.0f);

	}

	//ù��° �ؽ�Ʈ���� ����(�͸� ���) ��������
	if (_dialogue)
	{
		cout << "yyy" << endl;
		//D2DINS->GetInstance()->RenderText(430, 330, L"����", RGB(255, 255, 255), 0.85f, 27);

		//CAMERAMANAGER->render(IMAGEMANAGER->FindImage("dialogueBar"), Vector2(WINSIZEX / 2 + 250, WINSIZEY / 2 + 250));

		////////////////////////////////////////////////////////////
		//cout << "reee" << endl;
		if (_isStopToRead)
			TEXTMANAGER->renderText();

		//////////////////////////////////////////////////////////

		if (KEYMANAGER->isOnceKeyDown('A')) _dialogue = false;
	}

	//���̾�α� ������ �̹��� �ϴ� �Ⱥ��̴°����� ġ������
	//if (!_dialogue) CAMERAMANAGER->render(IMAGEMANAGER->FindImage("dialogueBar"), Vector2(0, 0));

	//ù��° �ؽ�Ʈ���� �Ʒ�(���� ����) ��������
	if (_text == OPENDOWN)
	{
		CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(camera.x - 180, camera.y - 10));	//����
		CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(WINSIZEX / 2 + 470, WINSIZEY / 2 + 120));	//������
		D2DINS->GetInstance()->RenderText(430, 330, L"����", RGB(255, 255, 255), 0.85f, 27);
		D2DINS->GetInstance()->RenderText(850, 330, L"���� �׸��д�", RGB(255, 255, 255), 0.85f, 27);

		D2DINS->FillRectangle(_rc, D2D1::ColorF::Enum::WhiteSmoke, _rcAlpha / 5.5);
		D2DINS->GetInstance()->DrawRectangle(_rc, D2D1::ColorF::White, _rcAlpha, 1.0f);

	}


	//�ؽ�Ʈ ���� ���¿��� �� ���� ����������
	//�̰� ������ ���������� �״�� �־��ָ� ��. ������ ���̷α׷� �ٲ�
	if (_text == OPENLEFT  /*&& !_leftClick && !_rightClick*/)
	{
		CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(camera.x - 180, camera.y - 10));	//����
		CAMERAMANAGER->render(IMAGEMANAGER->FindImage("bar"), Vector2(WINSIZEX / 2 + 470, WINSIZEY / 2 + 120)); // ������


		D2DINS->GetInstance()->RenderTextField(330, 320, L"\t��������\n \tâ������ �ִ´�", RGB(255, 255, 255), 27, 270, 50, 0.85f, DWRITE_TEXT_ALIGNMENT_CENTER);
		D2DINS->GetInstance()->RenderText(800, 330, L"�ƹ��͵� ���� �ʴ´�", RGB(255, 255, 255), 0.85f, 27);

		D2DINS->FillRectangle(_rc, D2D1::ColorF::Enum::WhiteSmoke, _rcAlpha / 5.5);
		D2DINS->GetInstance()->DrawRectangle(_rc, D2D1::ColorF::White, _rcAlpha, 1.0f);
	}

	//�ι�° �ؽ�Ʈâ���� ���� �������� �÷��̾� ���� false���ְ�
	if (_text == CHANGEIMG)
	{
		cout << "Rrr" << endl;
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
		}
	}

}

void thirdFrogOutRoom::changeScene()
{
	SCENEMANAGER->changeScene("thirdSnakeDead");
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
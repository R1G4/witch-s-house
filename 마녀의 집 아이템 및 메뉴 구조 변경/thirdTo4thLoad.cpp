#include "stdafx.h"
#include "thirdTo4thLoad.h"

thirdTo4thLoad::thirdTo4thLoad()
{
}

thirdTo4thLoad::~thirdTo4thLoad()
{
}

HRESULT thirdTo4thLoad::init()
{
	IMAGEMANAGER->AddFrameImage("SavePoint", L"Image/mapTool/saveCat.png", 16, 4);
	CAMERAMANAGER->setConfig(0, 0, TILESIZEX, TILESIZEY, 0, 0, TILESIZEX, TILESIZEY);

	_player = new Player;
	load();

	_player->init();
	_player->setState(CHR_IDLE);
	_player->setDirec(CHRDIREC_DOWN);
	camera.x = _player->getPlayerLocX();
	camera.y = _player->getPlayerLocY();
	CAMERAMANAGER->setCamera(camera);

	_count = 0;
	_frame = 0;
	_candleFrame = 0;
	_eyesFrameX = 0;
	_eyesFrameY = 0;
	_moveSword = false;

	return S_OK;
}

void thirdTo4thLoad::release()
{
}

void thirdTo4thLoad::update()
{
	_count++;
	if (_count % 8 == 0)
	{
		_frame++;
		_candleFrame++;
		_eyesFrameX++;
		_eyesFrameY++;
		if (_frame > 15) _frame = 0;
		if (_candleFrame >= 3) _candleFrame = 0;

		if (_eyesFrameX > 2)
		{
			_eyesFrameX = 0;
		}
		if (_eyesFrameY > 3)
		{
			_eyesFrameY = 0;
		}
	}

	camera.x = _player->getPlayerLocX();
	camera.y = _player->getPlayerLocY();

	CAMERAMANAGER->setCamera(Vector2(camera.x - WINSIZEX / 2, camera.y - WINSIZEY / 2));
	_player->update();
	tileCollision();
	trigger();
	changeScene();

	//if (_count % 10 == 0) cout << _player->getPlayerFrc().right / TILESIZE << endl;

	

}

void thirdTo4thLoad::render()
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
		//�÷��̾�� ���ʹ��� ��� �� ��ü�� �����Լ��� ȣ���� ���̹Ƿ� z-order�񱳸� ���� y���� Ÿ�԰��� �־��ָ� ���

	ZORDER->insert(_player->getPlayerFrc().left, _player->getPlayerFrc().top, ZPLAYER);

	//������Ʈ�� �־��ִ� ����
	//������Ʈ�� ��� ������ ������ϹǷ� �̹����� �־��ְų� Ű���� �־��ִ°� ����
	//Ư�� Ʈ���ŷ� ����ó���� �ؾ��Ѵٸ�, �̹����� �ٷ� �ֱ⺸�ٴ� Ű���� �־��ִ� ����� ����ϴ°� ���ƺ���
	//������Ʈ�� ���� Ÿ�ϱ��� �ְԵǸ� ���α׷��� �������� �����ϱ����

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
	//�� �κп��� FrameInfoManager�� �� ���ư��� ������ �𸣰��� �����ʿ�
	for (int i = 0; i < ZORDER->getZorder().size(); i++)
	{
		if (ZORDER->getZorder()[i].type == ZPLAYER)_player->render();

		if (ZORDER->getZorder()[i].type == ZOBJECT)
		{	
			CAMERAMANAGER->render(ZORDER->getZorder()[i].img,
				Vector2(ZORDER->getZorder()[i].x + TILESIZE / 2, ZORDER->getZorder()[i].y + 48 - ZORDER->getZorder()[i].img->GetSize().y / 2));
		

			//19,32
			/*if (_player->getPlayerFrc().right / TILESIZE >= 19)
			{

				IMAGEMANAGER->FindImage(ZORDER->getZorder()[i].keyName)->SetAlpha(1);
				ZORDER->getZorder()[i].keyName = "061";
			}
			else
			{
				IMAGEMANAGER->FindImage(ZORDER->getZorder()[i].keyName)->SetAlpha(0);
			}*/
		}

		if (ZORDER->getZorder()[i].type == ZFRAMEOBJ)
		{
			CAMERAMANAGER->render(ZORDER->getZorder()[i].img,
				Vector2(ZORDER->getZorder()[i].x + TILESIZE / 2, ZORDER->getZorder()[i].y + 48 - ZORDER->getZorder()[i].img->GetSize().y / 2));
		}


	}

	//zorder ���͸� �ʱ�ȭ���� ���ϸ� ������
	ZORDER->release();


	//_player->render();

	//CAMERAMANAGER->FrameRender(IMAGEMANAGER->FindImage("SavePoint"), Vector2(920, 1680), _frame, 2);

	//������Ʈ ����
	//for (int i = 0; i < TILEY; i++)
	//{
	//	for (int j = 0; j < TILEX; j++)
	//	{
	//		if (_tiles[i*TILEX + j].obj == OBJ_NONE)continue;
	//		//IMAGEMANAGER->FindImage(_tiles[i*TILEX + j].keyName)->SetAlpha(0.5);
	//		CAMERAMANAGER->render(IMAGEMANAGER->FindImage(_tiles[i*TILEX + j].keyName),
	//			Vector2(_tiles[i*TILEX + j].rc.left + TILESIZE / 2, _tiles[i*TILEX + j].rc.top));
	//	}
	//}
	////�������̹��� ����
	//for (int i = 0; i < TILEY; i++)
	//{
	//	for (int j = 0; j < TILEX; j++)
	//	{
	//		if (_tiles[i*TILEX + j].attribute == NONE)continue;
	//		if (_tiles[i*TILEX + j].attribute == PLAYER)continue;


	//		if (_tiles[i*TILEX + j].attribute == OBJ)
	//		{
	//			CAMERAMANAGER->FrameRender(IMAGEMANAGER->FindImage(_tiles[i*TILEX + j].keyName),
	//				Vector2(_tiles[i*TILEX + j].rc.left + TILESIZE / 2, _tiles[i*TILEX + j].rc.top), _candleFrame, 0);
	//		}

	//		if (_tiles[i*TILEX + j].attribute == ENEMY)
	//		{
	//			CAMERAMANAGER->FrameRender(IMAGEMANAGER->FindImage(_tiles[i*TILEX + j].keyName),
	//				Vector2(_tiles[i*TILEX + j].rc.left + TILESIZE / 2, _tiles[i*TILEX + j].rc.top), _eyesFrameX, _eyesFrameY);

	//			//���� ���ʹ� �̹����� Ʈ���� �ߵ������� �Ⱥ��̴ٰ� �ߵ��ϸ� �̹��� ���̰�
	//			if (_player->getPlayerFrc().bottom / TILESIZE >= 22)
	//			{

	//				IMAGEMANAGER->FindImage(_tiles[i*TILEX + j].keyName)->SetAlpha(1);
	//			}
	//			else
	//			{
	//				IMAGEMANAGER->FindImage(_tiles[i*TILEX + j].keyName)->SetAlpha(0);

	//			}
	//		}

	//	}
	//}

}

void thirdTo4thLoad::changeScene()
{
	//4��Ȧ�� �̵�
	if (_player->getPlayerFrc().top / TILESIZE <= 6.2f
		&&(_player->getPlayerFrc().right / TILESIZE >= 36 && _player->getPlayerFrc().right / TILESIZE <= 37))
	{
		SCENEMANAGER->changeScene("4��Ȧ");
	}

}

void thirdTo4thLoad::trigger()
{
	for (int i = 0; i < (TILEX * TILEY); ++i)
	{
		//�÷��̾��� Ÿ�� �ε����� ������ 10���� Ŀ���� sword Ʈ���� on
		if (_player->getPlayerFrc().bottom / TILESIZE >= 10)
		{
			_moveSword = true;
		}
		//�÷��̾��� Ÿ�� �ε����� ������ 22���� Ŀ���� eyes Ʈ���� on
		if (_player->getPlayerFrc().bottom / TILESIZE >= 22)
		{
			_moveEyes = true;
		}


		if (_moveSword)
		{
			if (_tiles[i].terrain == TR_TRIGGER && _tiles[i].obj == OBJ_LOOK)
			{
				_tiles[i].rc.bottom -= 10;
				_tiles[i].rc.top -= 10;
			}
		}

		if (_moveEyes)
		{
			if (_tiles[i].terrain == TR_TRIGGER && _tiles[i].attribute == ENEMY)
			{
				_tiles[i].rc.bottom += 13;
				_tiles[i].rc.top += 13;
			}
		}


	}



}


void thirdTo4thLoad::tileCollision()
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

void thirdTo4thLoad::load()
{
	HANDLE file;
	DWORD read;
	file = CreateFile("Stage/3f_to_4f.map", GENERIC_READ, NULL, NULL,
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
		/*if (_tiles[i].attribute == ENEMY)
		{
			bossLocX = i % TILEX;
			bossLocY = i / TILEX;
		}*/
		if (_tiles[i].attribute == OBJ)
		{

		}
	}

	CloseHandle(file);
}
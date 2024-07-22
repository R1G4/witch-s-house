#include "stdafx.h"
#include "fifthFloorStage.h"
#include "Player.h"

HRESULT fifthFloorStage::init()
{
	_skul = new skul;
	_dead = new DeadManager;
	_player->init();
	_player->setAlpha(1.f);

	IMAGEMANAGER->AddImage("bar", L"Image/obj/bar2.png");
	//카메라 아직 잘 모르겠다 다 초기화 하자..
	CAMERAMANAGER->setConfig(0, 0, TILESIZEX, TILESIZEY, 0, 0, TILESIZEX, TILESIZEY);
	CAMERAMANAGER->setCamera(camera);

	getFrameTile();
	_sceneAlpha = 1.0f;
	_rcAlpha = 1.0f;
	_rcAlphaChange = 0.03f;
	_isSkulAppeal = _isDead = false;
	_dead->setPlayerAddress(_player);
	return S_OK;
}

HRESULT fifthFloorStage::init(int x, int y, CHRDIRECTION _direction)
{
	return S_OK;
}

void fifthFloorStage::release()
{
}

void fifthFloorStage::update()
{
	if(!_isChangeScene && !STORAGEMANAGER->getIsOpen())
		_player->update();
	_skul->update();
	tileCollision();
	if (_isDead)
		_dead->update();
}

void fifthFloorStage::cameraUpdate()
{
	CAMERAMANAGER->setWorldMouse(_ptMouse);
	camera.x = _player->getPlayerLocX();
	camera.y = _player->getPlayerLocY();
	CAMERAMANAGER->setCamera(Vector2(camera.x - WINSIZEX / 2, camera.y - WINSIZEY / 2));
}

void fifthFloorStage::render()
{
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			//if (!_tiles[i*TILEX + j].isMapOn)continue;
			if (KEYMANAGER->isToggleKey(VK_TAB))
			{
				CAMERAMANAGER->renderRc(_tiles[i*TILEX + j].rc, D2D1::ColorF::White, 1, 1);
				if (_tiles[i*TILEX + j].terrain == TR_TRIGGER)CAMERAMANAGER->renderFillRc(_tiles[i*TILEX + j].rc, D2D1::ColorF::Aqua, 0.5);
				if (_tiles[i*TILEX + j].isCollider == true)CAMERAMANAGER->renderFillRc(_tiles[i*TILEX + j].rc, D2D1::ColorF::Red, 0.5);
			}
		}
	}

	ZORDER->insert(_player->getPlayerFrc().left, _player->getPlayerFrc().top, ZPLAYER);
	if (_isSkulAppeal) ZORDER->insert(_skul->getRect().left, _skul->getRect().top, ZENEMY);

	for (int i = 0; i < TILEX*TILEY; i++)
	{
		if (_tiles[i].obj != OBJ_NONE)
		{
			if (_tiles[i].keyName == "obj5" || _tiles[i].keyName == "obj9")continue;	// 검은색 배경
			ZORDER->insert(_tiles[i].rc.left, _tiles[i].rc.bottom, IMAGEMANAGER->FindImage(_tiles[i].keyName), ZOBJECT);
		}
	}
	for (int i = 0; i < _vFrameTile.size(); i++)
	{
		if (_vFrameTile[i].keyName == "하얀꽃" && _flowerDead) continue;
		ZORDER->insert(_vFrameTile[i].rc.left, _vFrameTile[i].rc.top, _vFrameTile[i].keyName, ZFRAMEOBJ);
	}
	for (int i = 0; i < ZORDER->getZorder().size(); i++)
	{
		if (ZORDER->getZorder()[i].type == ZPLAYER)_player->render();
		if (ZORDER->getZorder()[i].type == ZENEMY) _skul->render();
		if (ZORDER->getZorder()[i].type == ZOBJECT)
		{
			CAMERAMANAGER->render(ZORDER->getZorder()[i].img,
				Vector2(ZORDER->getZorder()[i].x + TILESIZE / 2, ZORDER->getZorder()[i].y - ZORDER->getZorder()[i].img->GetSize().y / 2));
		}
		if (ZORDER->getZorder()[i].type == ZFRAMEOBJ)
		{
			for (int j = 0; j < _vFrameTile.size(); j++)
			{
				if (_vFrameTile[j].kinds == PLAYER) continue;
				if (_vFrameTile[j].kinds == ENEMY)continue;
				if (ZORDER->getZorder()[i].keyName == _vFrameTile[j].keyName)
					CAMERAMANAGER->FrameRender
					(
						_vFrameTile[j].img,
						Vector2((_vFrameTile[j].rc.left + _vFrameTile[j].rc.right) / 2, _vFrameTile[j].rc.bottom - _vFrameTile[j].img->GetSize().y / 2),
						_vFrameTile[j].frameX, _vFrameTile[j].frameY
					);
			}
		}
	}
	//zorder 벡터를 초기화해줌 안하면 느려짐
	ZORDER->release();

	//검은색 타일 오브젝트이미지까지 넣게되면 프로그램이 무거워져서 따로 뺌
	for (int i = 0; i < TILEX*TILEY; i++)
	{
		if (_tiles[i].keyName == "obj5" || _tiles[i].keyName == "obj9")
			CAMERAMANAGER->render(IMAGEMANAGER->FindImage(_tiles[i].keyName),
				Vector2(_tiles[i].rc.left + TILESIZE / 2, _tiles[i].rc.bottom - IMAGEMANAGER->FindImage(_tiles[i].keyName)->GetSize().y / 2));
	}
	IMAGEMANAGER->FindImage("Back2")->SetAlpha(1.0f);
	IMAGEMANAGER->FindImage("Back2")->SetSize(Vector2(1920, 1280));
	CAMERAMANAGER->render(IMAGEMANAGER->FindImage("Back2"), Vector2(_player->getPlayerLocX(), _player->getPlayerLocY()));

	STORAGEMANAGER->render();
	_dead->render();
}

void fifthFloorStage::getFrameTile()
{
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

			_vFrameTile.push_back(temp);
		}
	}
}

void fifthFloorStage::setFrameIndex()
{
	//예외) 타일에 배치된 프레임이 없을 경우
	if (_vFrameTile.size() <= 0)
		return;

	for (int i = 0; i < _vFrameTile.size(); i++)
	{
		if (_vFrameTile[i].isMaxframe)
		{
			_vFrameTile[i].frameX = FRAMEINFOMANAGER->FindImage(_vFrameTile[i].keyName)->GetMaxFrameX() - 1;
			_vFrameTile[i].frameY = FRAMEINFOMANAGER->FindImage(_vFrameTile[i].keyName)->GetMaxFrameY() - 1;
			continue;
		}

		Vector2 temp;
		temp = FRAMEINFOMANAGER->FrameOperation(_vFrameTile[i].keyName, Vector2(_vFrameTile[i].frameX, _vFrameTile[i].frameY), _vFrameTile[i].isTrigger);
		_vFrameTile[i].frameX = temp.x;
		_vFrameTile[i].frameY = temp.y;
	}
}

void fifthFloorStage::tileCollision()
{
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			FloatRect pFrc = _player->getPlayerFrc();
			if (IntersectRectToRect(&pFrc, &_tiles[i*TILEX + j].rc) && _tiles[i*TILEX + j].isCollider)
			{
				_player->setState(CHR_IDLE);

				if ((_tiles[i*TILEX + j].rc.left + _tiles[i*TILEX + j].rc.right) / 2 > (_player->getPlayerFrc().left+ _player->getPlayerFrc().right) / 2)
				{
					//박힌 타일이 플레이어보다 왼쪽이면
					_player->setPLocaX(_player->getPlayerLocX() - 1);
				}
				if ((_tiles[i*TILEX + j].rc.left + _tiles[i*TILEX + j].rc.right) / 2 < (_player->getPlayerFrc().left + _player->getPlayerFrc().right) / 2)
				{
					//박힌 타일이 플레이어보다 오른쪽이면
					_player->setPLocaX(_player->getPlayerLocX() + 1);
				}
				if ((_tiles[i*TILEX + j].rc.top + _tiles[i*TILEX + j].rc.bottom) / 2 < (_player->getPlayerFrc().top + _player->getPlayerFrc().bottom) / 2)
				{
					//박힌 타일이 플레이어보다 위쪽이면
					_player->setPLocaY(_player->getPlayerLocY() + 1);
				}
				if ((_tiles[i*TILEX + j].rc.top + _tiles[i*TILEX + j].rc.bottom) / 2 > (_player->getPlayerFrc().top + _player->getPlayerFrc().bottom) / 2)
				{
					//박힌 타일이 플레이어보다 아래쪽이면
					_player->setPLocaY(_player->getPlayerLocY() - 1);
				}
			}
		}
	}
}

void fifthFloorStage::rcAlphaChange()
{
	if (_rcAlpha >= 0.9f)
		_rcAlphaChange = -0.03f;

	if (_rcAlpha <= 0.35f)
		_rcAlphaChange = 0.03f;

	_rcAlpha += _rcAlphaChange;
}

void fifthFloorStage::sceneChange(string name)
{
	_sceneAlpha -= 0.012f;
	_player->setAlpha(_sceneAlpha);
	if (_sceneAlpha <= 0.f)
	{
		_isChangeScene = false;
		_player->release();
		_vFrameTile.clear();
		SCENEMANAGER->changeScene(name);
	}
}

void fifthFloorStage::sceneChange(string name, CHRDIRECTION _chrdirection, LOCATION _location)
{
	_sceneAlpha -= 0.012f;
	_player->setAlpha(_sceneAlpha);
	if (_sceneAlpha <= 0.f)
	{
		_isChangeScene = false;
		_player->release();
		_vFrameTile.clear();
		SCENEMANAGER->changeScene(name, _chrdirection, _location);
	}
}

void fifthFloorStage::setSave()
{
	STORAGEMANAGER->setStage(FOURTHSTAGE);
	_player_x = _player->getPlayerLocX();
	_player_y = _player->getPlayerLocY();
	_player_direction = _player->getPdirec();
	_save_s_x = to_string(_player_x);
	_save_s_y = to_string(_player_y);
	_save_string_position = to_string(_player_direction);

	INIDATA->addData("비올라", "x좌표", _save_s_x.c_str());
	INIDATA->addData("비올라", "y좌표", _save_s_y.c_str());
	INIDATA->addData("비올라", "방향", _save_string_position.c_str());
	STORAGEMANAGER->saveView();
}

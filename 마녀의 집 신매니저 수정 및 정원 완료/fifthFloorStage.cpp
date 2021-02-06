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

void fifthFloorStage::release()
{
}

void fifthFloorStage::update()
{
	if(!_isChangeScene)
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

	//타일보다는 상위에 오브젝트보다는 하위에 위치해야함
	_player->render();
	if (_isSkulAppeal) _skul->render();

	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			if (_tiles[i*TILEX + j].obj == OBJ_NONE)continue;

			IMAGEMANAGER->FindImage(_tiles[i*TILEX + j].keyName)->SetAlpha(_sceneAlpha);

			//중간에 배치하고 싶다면 이걸쓰세요. 디폴트 센타
			CAMERAMANAGER->render(IMAGEMANAGER->FindImage(_tiles[i*TILEX + j].keyName),
				Vector2(_tiles[i*TILEX + j].rc.left + TILESIZE / 2,
					_tiles[i*TILEX + j].rc.bottom - IMAGEMANAGER->FindImage(_tiles[i*TILEX + j].keyName)->GetSize().y / 2));
		}
	}

	//타일에 프레임 이미지 배치 랜더
	for (int i = 0; i < _vFrameTile.size(); i++)
	{
		if (KEYMANAGER->isToggleKey(VK_TAB))
		{
			IMAGEMANAGER->FindImage(_vFrameTile[i].keyName)->SetAlpha(_sceneAlpha);
			if (_vFrameTile[i].kinds == PLAYER)  CAMERAMANAGER->renderFillRc(_vFrameTile[i].rc, D2D1::ColorF::Blue, 0.7);
			else if (_vFrameTile[i].kinds == ENEMY) CAMERAMANAGER->renderFillRc(_vFrameTile[i].rc, D2D1::ColorF::Black, 0.7);
			else CAMERAMANAGER->renderFillRc(_vFrameTile[i].rc, D2D1::ColorF::White, 0.7);
		}

		if (_vFrameTile[i].kinds == PLAYER || _vFrameTile[i].kinds == ENEMY) continue;

		_vFrameTile[i].img->SetAlpha(_sceneAlpha);
		CAMERAMANAGER->FrameRender
		(
			_vFrameTile[i].img,
			Vector2((_vFrameTile[i].rc.left + _vFrameTile[i].rc.right) / 2, _vFrameTile[i].rc.bottom - _vFrameTile[i].img->GetSize().y / 2),
			_vFrameTile[i].frameX, _vFrameTile[i].frameY
		);
	}
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
		Vector2 temp;
		temp = FRAMEINFOMANAGER->FrameOperation(_vFrameTile[i].keyName, Vector2(_vFrameTile[i].frameX, _vFrameTile[i].frameY), _vFrameTile[i].isTrigger);
		_vFrameTile[i].frameX = temp.x;
		_vFrameTile[i].frameY = temp.y;
	}
}

void fifthFloorStage::tileCollision()
{
	/*
	int frontTile = (int)(_player->getSearchRc().left / TILESIZE) + (int)(_player->getSearchRc().top / TILESIZE) * TILEX;
	if(_tiles[frontTile].isCollider)
		_player->setState(CHR_IDLE);
	int frontX = (_player->getSearchRc().left + _player->getSearchRc().right) / 2;
	int frontY = (_player->getSearchRc().top + _player->getSearchRc().bottom) / 2;
	*/
	/*
	int frontTile[3]; 
	//cout << "x : " << (int)(_player->getPlayerLocX() / TILESIZE) << " y : " << (int)(_player->getPlayerLocY() / TILESIZE) << endl;
	//cout << frontTile << endl;
	//cout << _tiles[frontTile].isCollider << endl;
	switch (_player->getPdirec())
	{
	case CHRDIREC_DOWN:
		//frontTile = (int)(_player->getPlayerLocX() / TILESIZE) + (int)(_player->getPlayerLocY() / TILESIZE) * TILEX + TILEX;
		frontTile[0] = (int)(_player->getPlayerFrc().left / TILESIZE) + (int)(_player->getPlayerFrc().top / TILESIZE) * TILEX + TILEX;
		frontTile[1] = frontTile[0] - 1;
		frontTile[2] = frontTile[0] + 1;
		break;
	case CHRDIREC_LEFT:
		//frontTile = (int)(_player->getPlayerLocX() / TILESIZE) + (int)(_player->getPlayerLocY() / TILESIZE) * TILEX - 1;
		frontTile[0] = (int)(_player->getPlayerFrc().left / TILESIZE) + (int)(_player->getPlayerFrc().top / TILESIZE) * TILEX - 1;
		frontTile[1] = frontTile[0] - TILEX;
		frontTile[2] = frontTile[0] + TILEX;
		break;
	case CHRDIREC_RIGHT:
		//frontTile = (int)(_player->getPlayerLocX() / TILESIZE) + (int)(_player->getPlayerLocY() / TILESIZE) * TILEX + 1;
		frontTile[0] = (int)(_player->getPlayerFrc().left / TILESIZE) + (int)(_player->getPlayerFrc().top / TILESIZE) * TILEX + 1;
		frontTile[1] = frontTile[0] - TILEX;
		frontTile[2] = frontTile[0] + TILEX;
		break;
	case CHRDIREC_UP:
		//frontTile = (int)(_player->getPlayerLocX() / TILESIZE) + (int)(_player->getPlayerLocY() / TILESIZE) * TILEX - TILEX;
		frontTile[0] = (int)(_player->getPlayerFrc().left / TILESIZE) + (int)(_player->getPlayerFrc().top / TILESIZE) * TILEX - TILEX;
		frontTile[1] = frontTile[0] - 1;
		frontTile[2] = frontTile[0] + 1;
		break;
	}
	if (_tiles[frontTile[0]].isCollider)
		_player->setState(CHR_IDLE);
	if (_tiles[frontTile[0]].isCollider && _tiles[frontTile[1]].isCollider)
		_player->setState(CHR_IDLE);
	if (_tiles[frontTile[0]].isCollider && _tiles[frontTile[2]].isCollider)
		_player->setState(CHR_IDLE);
	*/
	
	int frontTile;
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			if (IntersectRectToRect(&_player->getPlayerFrc(), &_tiles[i*TILEX + j].rc) && _tiles[i*TILEX + j].isCollider)
			{
				switch (_player->getPdirec())
				{
				case CHRDIREC_DOWN:
					//frontTile = (int)(_player->getPlayerFrc().left / TILESIZE) + (int)(_player->getPlayerFrc().top / TILESIZE) * TILEX + TILEX;
					//if (_tiles[frontTile].isCollider)
						_player->setState(CHR_IDLE);
					//_player->setPLocaY(_tiles[i*TILEX + j].rc.top - TILESIZE / 4 * 3);
						_player->setPLocaY(_player->getPlayerLocY() - 1);
					break;
				case CHRDIREC_LEFT:
					//frontTile = (int)(_player->getPlayerFrc().left / TILESIZE) + (int)(_player->getPlayerFrc().top / TILESIZE) * TILEX - 1;
					//if (_tiles[frontTile].isCollider)
						_player->setState(CHR_IDLE);
					//_player->setPLocaX(_tiles[i*TILEX + j].rc.right + 4);
						_player->setPLocaX(_player->getPlayerLocX() + 1);

					break;
				case CHRDIREC_RIGHT:
					//frontTile = (int)(_player->getPlayerFrc().left / TILESIZE) + (int)(_player->getPlayerFrc().top / TILESIZE) * TILEX + 1;
					//if (_tiles[frontTile].isCollider)
						_player->setState(CHR_IDLE);
					//_player->setPLocaX(_tiles[i*TILEX + j].rc.left - TILESIZE / 4 * 3);
						_player->setPLocaX(_player->getPlayerLocX() - 1);
					break;
				case CHRDIREC_UP:
					//frontTile = (int)(_player->getPlayerFrc().left / TILESIZE) + (int)(_player->getPlayerFrc().top / TILESIZE) * TILEX - TILEX;
					//if (_tiles[frontTile].isCollider)
						_player->setState(CHR_IDLE);
					//_player->setPLocaY(_tiles[i*TILEX + j].rc.bottom + 4);
						_player->setPLocaY(_player->getPlayerLocY() + 1);
					break;
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

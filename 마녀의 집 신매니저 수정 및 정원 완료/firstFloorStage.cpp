#include "stdafx.h"
#include "firstFloorStage.h"
#include "Player.h"

firstFloorStage::firstFloorStage()
{
}

firstFloorStage::~firstFloorStage()
{
}

HRESULT firstFloorStage::init()
{
	_player->init();
	_player->setAlpha(1.f);

	//ī�޶� ���� �� �𸣰ڴ� �� �ʱ�ȭ ����..
	CAMERAMANAGER->setConfig(0, 0, TILESIZEX, TILESIZEY, 0, 0, TILESIZEX, TILESIZEY);
	CAMERAMANAGER->setCamera(camera);

	getFrameTile();
	_sceneAlpha = 1.f;

	_delay = 0;
	return S_OK;
}

void firstFloorStage::release()
{
	_player->release();
	_vFrameTile.clear();
}

void firstFloorStage::update()
{
	_player->update();
}

void firstFloorStage::cameraUpdate()
{
	CAMERAMANAGER->setWorldMouse(_ptMouse);
	camera.x = _player->getPlayerLocX();
	camera.y = _player->getPlayerLocY();
	CAMERAMANAGER->setCamera(Vector2(camera.x - WINSIZEX / 2, camera.y - WINSIZEY / 2));
}

void firstFloorStage::render()
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
				if (_tiles[i*TILEX + j].isCollider)CAMERAMANAGER->renderFillRc(_tiles[i*TILEX + j].rc, D2D1::ColorF::Red, 0.5);
			}
		}
	}

	//Ÿ�Ϻ��ٴ� ������ ������Ʈ���ٴ� ������ ��ġ�ؾ���
	_player->render();

	//Ÿ�Ͽ� ������ �̹��� ��ġ ����
	for (int i = 0; i < _vFrameTile.size(); i++)
	{
		if (KEYMANAGER->isToggleKey(VK_TAB))
		{
			IMAGEMANAGER->FindImage(_vFrameTile[i].keyName)->SetAlpha(_sceneAlpha);
			if (_vFrameTile[i].kinds == PLAYER)  CAMERAMANAGER->renderFillRc(_vFrameTile[i].rc, D2D1::ColorF::Blue, 0.7);
			else if (_vFrameTile[i].kinds == ENEMY)  CAMERAMANAGER->renderFillRc(_vFrameTile[i].rc, D2D1::ColorF::Black, 0.7);
			else CAMERAMANAGER->renderFillRc(_vFrameTile[i].rc, D2D1::ColorF::White, 0.7);
		}

		if (_vFrameTile[i].kinds == PLAYER) continue;

		_vFrameTile[i].img->SetAlpha(_sceneAlpha);
		CAMERAMANAGER->FrameRender
		(
			_vFrameTile[i].img,
			Vector2((_vFrameTile[i].rc.left + _vFrameTile[i].rc.right) / 2, _vFrameTile[i].rc.bottom - _vFrameTile[i].img->GetSize().y / 2),
			_vFrameTile[i].frameX, _vFrameTile[i].frameY
		);
	}

	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			if (_tiles[i*TILEX + j].obj == OBJ_NONE || _tiles[i*TILEX + j].keyName =="")continue;
			string temp = _tiles[i*TILEX + j].keyName;
			IMAGEMANAGER->FindImage(_tiles[i*TILEX + j].keyName)->SetAlpha(_sceneAlpha);

			//�߰��� ��ġ�ϰ� �ʹٸ� �̰ɾ�����. ����Ʈ ��Ÿ
			CAMERAMANAGER->render(IMAGEMANAGER->FindImage(_tiles[i*TILEX + j].keyName),
				Vector2(_tiles[i*TILEX + j].rc.left + TILESIZE / 2,
					_tiles[i*TILEX + j].rc.bottom - IMAGEMANAGER->FindImage(_tiles[i*TILEX + j].keyName)->GetSize().y / 2));
		}
	}
}

void firstFloorStage::getFrameTile()
{
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

			_vFrameTile.push_back(temp);
		}
	}
}

void firstFloorStage::setFrameIndex()
{
	//����) Ÿ�Ͽ� ��ġ�� �������� ���� ���
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

void firstFloorStage::tileCollision(int i, int j)
{
	if (!_tiles[i*TILEX + j].isCollider) return;

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

void firstFloorStage::sceneChange(string name, CHRDIRECTION _chrdirection, LOCATION _location)
{
	_sceneAlpha -= 0.008f;
	if (_sceneAlpha <= 0.62f)
		_sceneAlpha -= 0.02f;

	_player->setAlpha(_sceneAlpha);

	if (_sceneAlpha <= 0.f)	SCENEMANAGER->changeScene(name, _chrdirection, _location);
}
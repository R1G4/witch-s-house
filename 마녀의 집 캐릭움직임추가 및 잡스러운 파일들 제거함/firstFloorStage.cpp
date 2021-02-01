#include "stdafx.h"
#include "firstFloorStage.h"
#include "dictionary.h"


firstFloorStage::firstFloorStage()
{
}


firstFloorStage::~firstFloorStage()
{
}

HRESULT firstFloorStage::init()
{
	getFrameTile();
	return S_OK;
}

void firstFloorStage::release()
{
}

void firstFloorStage::update()
{
	//�Ʒ� �ڵ�� �׽�Ʈ��
	if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
	{
		for (int i = 0; i < FRAMEINFOMANAGER->GetSize(); i++)
		{
			if (FRAMEINFOMANAGER->GetAttribute(i) == OBJ)
			{
				isTrigger = true;
			}
		}
	}

	//������ �ε��� ����
	setFrameIndex();
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
			}
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

			//if (_tiles[i*TILEX + j].obj == OBJ_CORELATION)_D2DRenderer->FillRectangle(_tiles[i*TILEX + j].rc, D2D1::ColorF::Aquamarine, 0.5);
		}
	}

	//Ÿ�Ͽ� ������ �̹��� ��ġ ����
	for (int i = 0; i < _vFrameTile.size(); i++)
	{
		if (_vFrameTile[i].kinds == PLAYER)  CAMERAMANAGER->renderFillRc(_vFrameTile[i].rc, D2D1::ColorF::Blue, 0.7);
		else if (_vFrameTile[i].kinds == ENEMY)  CAMERAMANAGER->renderFillRc(_vFrameTile[i].rc, D2D1::ColorF::Black, 0.7);
		else CAMERAMANAGER->renderFillRc(_vFrameTile[i].rc, D2D1::ColorF::White, 0.7);
		CAMERAMANAGER->FrameRender
		(
			_vFrameTile[i].img,
			Vector2((_vFrameTile[i].rc.left + _vFrameTile[i].rc.right) / 2, _vFrameTile[i].rc.bottom - _vFrameTile[i].img->GetSize().y / 2),
			_vFrameTile[i].frameX, _vFrameTile[i].frameY
		);
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
			addFrameTile(temp);
		}
	}
}

bool firstFloorStage::addFrameTile(tagFrameTile _frameTile)
{
	//������ �̹����� �߰��Ѵ�.
	//�̹����� ��ġ �� �����ϴ� ����
	_vFrameTile.push_back(_frameTile);
	//false�� ��ȯ �� ��� ���� �� �ε�� Ÿ�Ͽ����� ���ŵ��� �ʴ´�.
	return false;
}

void firstFloorStage::setFrameIndex()
{
	//����) Ÿ�Ͽ� ��ġ�� �������� ���� ���
	if (_vFrameTile.size() <= 0)
		return;

	for (int i = 0; i < _vFrameTile.size(); i++)
	{
		Vector2 temp;
		temp = FRAMEINFOMANAGER->FrameOperation(_vFrameTile[i].keyName, Vector2(_vFrameTile[i].frameX, _vFrameTile[i].frameY), isTrigger);
		_vFrameTile[i].frameX = temp.x;
		_vFrameTile[i].frameY = temp.y;
	}
}

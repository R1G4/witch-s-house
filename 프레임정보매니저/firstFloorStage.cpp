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
	//아래 코드는 테스트용
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

	//프레임 인덱스 셋팅
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
				_D2DRenderer->DrawRectangle(_tiles[i*TILEX + j].rc, D2DRenderer::DefaultBrush::White);
			}
			if (_tiles[i*TILEX + j].isCollider)_D2DRenderer->FillRectangle(_tiles[i*TILEX + j].rc, D2D1::ColorF::Red, 0.4);
			if (_tiles[i*TILEX + j].terrain == TR_TRIGGER)_D2DRenderer->FillRectangle(_tiles[i*TILEX + j].rc, D2D1::ColorF::Aqua, 0.5);
		}
	}
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			if (_tiles[i*TILEX + j].obj == OBJ_NONE)continue;
			IMAGEMANAGER->FindImage("ObjectSample")->FrameRender(
				Vector2(_tiles[i*TILEX + j].rc.left + TILESIZE, _tiles[i*TILEX + j].rc.top),	// 보정값 바꿈
				_tiles[i*TILEX + j].objFrameX, _tiles[i*TILEX + j].objFrameY);
			camera = _tiles->camera;
			if (_tiles[i*TILEX + j].obj == OBJ_CORELATION)_D2DRenderer->FillRectangle(_tiles[i*TILEX + j].rc, D2D1::ColorF::Aquamarine, 0.5);
		}//보정값 필요할지도 모름
	}

	//타일에 프레임 이미지 배치 랜더
	for (int i = 0; i < _vFrameTile.size(); i++)
	{

  		if (_vFrameTile[i].kinds == PLAYER)  _D2DRenderer->FillRectangle(_vFrameTile[i].rc, D2D1::ColorF::Blue, 0.7);
		else if (_vFrameTile[i].kinds == ENEMY)  _D2DRenderer->FillRectangle(_vFrameTile[i].rc, D2D1::ColorF::Black, 0.7);
		else  _D2DRenderer->FillRectangle(_vFrameTile[i].rc, D2D1::ColorF::White, 0.7);

		_vFrameTile[i].img->FrameRender(Vector2((_vFrameTile[i].rc.left + _vFrameTile[i].rc.right) / 2, _vFrameTile[i].rc.bottom - _vFrameTile[i].img->GetSize().y / 2),
			_vFrameTile[i].frameX, _vFrameTile[i].frameY);
	}
}

void firstFloorStage::getFrameTile()
{
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			//이미지 정보가 존재하지 않는다면 불필요한 반복문은 건너뛴다.
			if (!FRAMEINFOMANAGER->GetSize())
				return;

			if (!FRAMEINFOMANAGER->KeyCheck(_tiles[i*TILEX + j].frameKeyName))
				continue;

			//렉트 생성
			FloatRect rc;
			rc = RectMakePivot(Vector2(_tiles[i*TILEX + j].rc.left + TILESIZE / 2, _tiles[i*TILEX + j].rc.top + TILESIZE / 2), Vector2(TILESIZE, TILESIZE), Pivot::Center);

			FRAMEATTRIBUTE tempKinds = FRAMEINFOMANAGER->GetAttribute(_tiles[i*TILEX + j].frameKeyName);

			tagFrameTile temp;
			temp.rc = rc;
			temp.kinds = tempKinds;
			temp.keyName = _tiles[i*TILEX + j].frameKeyName;
			temp.indexX = i;
			temp.indexY = j;
			temp.frameX = 0;
			temp.frameY = 0;
			temp.img = FRAMEINFOMANAGER->FindImage(_tiles[i*TILEX + j].frameKeyName);
			addFrameTile(temp);
		}
	}
}

bool firstFloorStage::addFrameTile(tagFrameTile _frameTile)
{
	//프레임 이미지를 추가한다.
	//이미지를 배치 및 랜더하는 백터
	_vFrameTile.push_back(_frameTile);
	//false를 반환 할 경우 저장 및 로드용 타일에서는 제거되지 않는다.
	return false;
}

void firstFloorStage::setFrameIndex()
{
	//예외) 타일에 배치된 프레임이 없을 경우
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

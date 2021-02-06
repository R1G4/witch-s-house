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

	//카메라 아직 잘 모르겠다 다 초기화 하자..
	CAMERAMANAGER->setConfig(0, 0, TILESIZEX, TILESIZEY, 0, 0, TILESIZEX, TILESIZEY);
	CAMERAMANAGER->setCamera(camera);

	getFrameTile();
	_sceneAlpha = 0.05;

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
	if(!_isForm)	_player->update();
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

	//타일보다는 상위에 오브젝트보다는 하위에 위치해야함
	_player->render();

	//타일에 프레임 이미지 배치 랜더
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
		if (_vFrameTile[i].kinds == ENEMY) continue;
		if (_vFrameTile[i].keyName == "손바닥" && !_isBlood) continue;
		
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
			if (ITEMMANAGER->KeyCheck(_tiles[i*TILEX + j].keyName) &&
				(STAGEMEMORYMANAGER->getIsBearPickUp()))	continue;

			string temp = _tiles[i*TILEX + j].keyName;
			IMAGEMANAGER->FindImage(_tiles[i*TILEX + j].keyName)->SetAlpha(_sceneAlpha);

			//중간에 배치하고 싶다면 이걸쓰세요. 디폴트 센타
			CAMERAMANAGER->render(IMAGEMANAGER->FindImage(_tiles[i*TILEX + j].keyName),
				Vector2(_tiles[i*TILEX + j].rc.left + TILESIZE / 2,
					_tiles[i*TILEX + j].rc.bottom - IMAGEMANAGER->FindImage(_tiles[i*TILEX + j].keyName)->GetSize().y / 2));
		}
	}

	//폼이 실행되고 있다면
	if (_isForm)	FormRender();
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

			if (tempKinds == ENEMY)
			{
				_bear = new bear;
				_bear->init(i%TILEX, i / TILEX);
			}
			_vFrameTile.push_back(temp);
		}
	}
}

void firstFloorStage::setFrameIndex()
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
	if (_sceneAlpha <= 0.65f)
		_sceneAlpha -= 0.021f;

	_player->setAlpha(_sceneAlpha);

	if (_sceneAlpha <= 0.f)	SCENEMANAGER->changeScene(name, _chrdirection, _location);
}

void firstFloorStage::setAlpha()
{
	if (_sceneAlpha < 1.f)
	{
		if (_sceneAlpha <= 0.65f)
			_sceneAlpha += 0.019f;
		else
			_sceneAlpha += 0.008f;
		if (_sceneAlpha >= 0.9f)
			_sceneAlpha = 1;

		_player->setAlpha(_sceneAlpha);
	}
}

bool firstFloorStage::SelectionForm(wstring leftText, wstring rightText)
{
	if (!_isForm && KEYMANAGER->isOnceKeyDown(VK_SPACE))
		_isForm = true;

	if (!_isForm) return false;

	FormInfo[LEFT] = leftText;
	FormInfo[RIGHT] = rightText;

	if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
		_fromSelected = (FROM_DIRECTION)(_fromSelected - 1 > -1 ? _fromSelected - 1 : _fromSelected);
	if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
		_fromSelected = (FROM_DIRECTION)(_fromSelected + 1 < 2 ? _fromSelected + 1 : _fromSelected);

	//선택되는 효과를 보여준다.(렉트)
	switch (_fromSelected)
	{
	case firstFloorStage::LEFT:
		_rcSelected = RectMakePivot(Vector2(WINSIZEX / 2 - 250, WINSIZEY / 2 - 50), Vector2(330, 88), Pivot::Center);
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE) * 0x20)
		{
			_isForm = false;
			return true;
		}
		break;
	case firstFloorStage::RIGHT:
		_rcSelected = RectMakePivot(Vector2(WINSIZEX / 2 + 250, WINSIZEY / 2 - 50), Vector2(330, 88), Pivot::Center);
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE) * 0x20)
		{
			_isForm = false;
			return true;
		}
		break;
	}

	rcAlphaChange();

	return false;
}

void firstFloorStage::FormRender()
{
	//if (!_isForm) return;



	IMAGEMANAGER->FindImage("bar")->SetScale(1.1f);
	IMAGEMANAGER->FindImage("bar")->Render(Vector2(WINSIZEX / 2 - 250, WINSIZEY / 2 - 50));	//왼쪽
	IMAGEMANAGER->FindImage("bar")->SetScale(1.1f);
	IMAGEMANAGER->FindImage("bar")->Render(Vector2(WINSIZEX / 2 + 250, WINSIZEY / 2 - 50)); // 오른쪽

	D2DINS->GetInstance()->RenderText(275, WINSIZEY / 2 - 68, FormInfo[LEFT], RGB(255, 255, 255), 0.85f, 27, DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_LEADING);
	D2DINS->GetInstance()->RenderText(760, WINSIZEY / 2 - 68, FormInfo[RIGHT], RGB(255, 255, 255), 0.85f, 27, DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_LEADING);


	//백그라운드 컬러 렉트
	D2DINS->FillRectangle
	(
		_rcSelected,
		D2D1::ColorF::Enum::WhiteSmoke,
		_rcAlpha / 5.5f
	);
	//테두리 컬러 렉트
	D2DINS->DrawRectangle
	(
		_rcSelected,
		D2D1::ColorF::Enum::White,
		_rcAlpha,
		1.0f
	);
}

void firstFloorStage::rcAlphaChange()
{
	if (_rcAlpha >= 0.9f)
		_rcAlphaChange = -0.03f;

	if (_rcAlpha <= 0.35f)
		_rcAlphaChange = 0.03f;

	_rcAlpha += _rcAlphaChange;
}
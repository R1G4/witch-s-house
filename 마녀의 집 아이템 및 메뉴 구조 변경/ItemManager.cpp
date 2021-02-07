#include "stdafx.h"
#include "ItemManager.h"

//이니셜라이징 으로 초기화 한다~
ItemManager::ItemManager():_rcAlpha(1.0f), _rcAlphaChange(0.03f), _index(FIRST)
{
}

ItemManager::~ItemManager()
{
}

bool ItemManager::addItem(string itemKey)
{
	//아이템 관련 변수 초기화
	Item _item;
	_item.itemKinds = (ITEMKINDS)conversion(itemKey);
	_item.isTry = false;

	for (int i = 0; i < _vItem.size(); i++)
	{
		//벡터에 존재하는지 체크한다.
		if (_vItem[i].itemKinds != _item.itemKinds) continue;

		//이미 존재하는 아이템이라면 false를 반환한다.
		return false;
	}

	//아이템에 따라 특성이 변한다.
	switch (_item.itemKinds)
	{
	case ITEM_TEDDYBEAR:
		_item.name = L"곰인형";
		_item.contents = L"왠지 모르게 외로워 보이는 곰인형";
		break;
	case ITEM_TEDDYBEARBODY:
		_item.name = L"곰인형의 몸통";
		_item.contents = L"머리랑 몸통뿐인 곰인형. 잘린 단면이 붉게 물들어 있다";
		break;
	case ITEM_TEDDYBEARHAND:
		_item.name = L"곰인형의 손발";
		_item.contents = L"가위로 잘라낸 것. 안은 고깃덩이로 채워져 있다.";
		break;
	case ITEM_FROH:
		_item.name = L"개구리";
		_item.contents = L"개굴~ 개굴~ 나중에 넣어주셈";
		break;
	case ITEM_TAEYUP:
		_item.name = L"태엽";
		_item.contents = L"시계에서 꺼낸 녹슨태엽";
	}
	_vItem.push_back(_item);

	//성공적으로 냠냠했으면 true를 반환한다.
	return true;
}

int ItemManager::conversion(string itemKey)
{
	return itemKey == "obj58" ? ITEM_TEDDYBEAR :
		itemKey == "obj59" ? ITEM_TEDDYBEARBODY :
		itemKey == "obj11" ? ITEM_TEDDYBEARHAND : ITEM_FROH;
		//itemKey == "frog" ? ITEM_FROH : ITEM_FROH;
}

bool ItemManager::useItem(int itemNum)
{
	for (int i = 0; i < _vItem.size(); i++)
	{
		//벡터에 존재하는지 체크한다.
		if (_vItem[i].itemKinds != (ITEMKINDS)itemNum && _vItem[i].isTry) continue;

		//해당 아이템 제거
		_vItem.erase(_vItem.begin() + i);

		//성공적으로 아이템 가능상태를 변경하였다면 true를 반환한다.
		return true;
	}
	//아이템이 존재하지 않을 경우 false를 반환한다.
	return false;
}

bool ItemManager::useItem(string itemKey)
{
	for (int i = 0; i < _vItem.size(); i++)
	{
		//벡터에 존재하는지 체크한다.
		if (_vItem[i].itemKinds != (ITEMKINDS)conversion(itemKey) && _vItem[i].isTry) continue;

		//해당 아이템 제거
		_vItem.erase(_vItem.begin() + i);

		//성공적으로 아이템 가능상태를 변경하였다면 true를 반환한다.
		return true;
	}
	//아이템이 존재하지 않을 경우 false를 반환한다.
	return false;
}

void ItemManager::render()
{
	if (_isOpenCheck != MENU_PROGRESS) return;
	IMAGEMANAGER->FindImage("messe")->SetAlpha(0.72f);
	IMAGEMANAGER->FindImage("messe")->SetSize(Vector2(1000, 77));
	IMAGEMANAGER->FindImage("messe")->Render(Vector2(WINSIZEX / 2, WINSIZEY / 2 + 68));

	IMAGEMANAGER->FindImage("messe")->SetSize(Vector2(1000, 200));
	IMAGEMANAGER->FindImage("messe")->Render(Vector2(WINSIZEX / 2, WINSIZEY / 2 + 212));

	//백그라운드 컬러 렉트
	D2DINS->FillRectangle
	(
		_rcSelected,
		D2D1::ColorF::Enum::WhiteSmoke,
		_rcAlpha / 3.5
	);

	for (int i = 0; i < _vItem.size(); i++)
	{
		D2DINS->RenderText
		(
			!(i % 2) ? WINSIZEX / 2 - 430 : WINSIZEX / 2 + 45,
			i < 2 ? WINSIZEY / 2 + 150 : WINSIZEY / 2 + 239,
			_vItem[i].name,
			RGB(255, 255, 255),
			1.f,
			34,
			DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_LEADING
			//폰트 아직 모르겟숴..
		);

		if (_vItem[i].itemKinds != _itemSelected.itemKinds) continue;

		D2DINS->RenderText
		(
			WINSIZEX / 2 - 455,
			WINSIZEY / 2 + 55,
			_vItem[i].contents,
			RGB(255, 255, 255),
			1.f,
			26,
			DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_LEADING
			//폰트 아직 모르겟숴..
		);
	}

	//테두리 컬러 렉트
	D2DINS->DrawRectangle
	(
		_rcSelected,
		D2D1::ColorF::Enum::White,
		_rcAlpha,
		3.f
	);
}


MENUSTATE ItemManager::itemOpen()
{
	//Z키를 누르지 않았을 경우 창이 열리지 않는다. 
	//임시로 추가한 keyinput 메뉴랑 합치면 추후 제거할것
	bool _isClick = false;

	if (!_isOpenCheck) _isOpenCheck = MENU_PROGRESS;

	if (KEYMANAGER->isOnceKeyDown(VK_SPACE))	_isClick = true;	

	if (KEYMANAGER->isOnceKeyDown('X') && _isOpenCheck)
	{
		_isOpenCheck = MENU_BACK;
		_isClick = false;
	}

	//백터의 사이즈가 0 이하라면(아이템이 존재 하지 않다면) 아래 코드는 무시한다.
	if (_vItem.size() <= 0)
	{
		if (_isClick)
		{
			_isClick = false;
			_isOpenCheck = MENU_BACK;
		}
		else _isOpenCheck = MENU_PROGRESS;
		return _isOpenCheck;
	}

	//종료
	if(!_isOpenCheck)
		return MENU_BACK;



	for (int i = 0; i < _vItem.size(); i++)
	{
		//해당 아이템 사용 시도를 초기화 해준다.
		_vItem[i].isTry = false;
	}

	//방향키 조작
	if (KEYMANAGER->isOnceKeyDown(VK_LEFT))		_index = (INDEX)(_index - 1 > -1 ? _index - 1 : _index);
	if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))	_index = (INDEX)(_index + 1 < 4 ? _index + 1 : _index);
	if (KEYMANAGER->isOnceKeyDown(VK_DOWN))		_index = (INDEX)(_index + 2 < 4 ? _index + 2 : _index);
	if (KEYMANAGER->isOnceKeyDown(VK_UP))		_index = (INDEX)(_index - 2 > -1 ? _index - 2 : _index);

	//백터 사이즈보다 인덱스가 클 경우 인덱스는 백터의 max 인덱스를 넣는당
	_index = (INDEX)(_index >= _vItem.size() ? _vItem.size() - 1 : _index);

	//인덱스에 따라 선택되는 효과를 보여준다.(렉트)
	switch (_index)
	{
		case ItemManager::FIRST:
			_rcSelected = RectMakePivot(Vector2(WINSIZEX / 2 - 240, WINSIZEY / 2 + 168), Vector2(450, 72), Pivot::Center);
			break;
		case ItemManager::SECOND:
			_rcSelected = RectMakePivot(Vector2(WINSIZEX / 2 + 240, WINSIZEY / 2 + 168), Vector2(450, 72), Pivot::Center);
			break;
		case ItemManager::THIRD:
			_rcSelected = RectMakePivot(Vector2(WINSIZEX / 2 - 240, WINSIZEY / 2 + 257), Vector2(450, 72), Pivot::Center);
			break;
		case ItemManager::FOURTH:
			_rcSelected = RectMakePivot(Vector2(WINSIZEX / 2 + 240, WINSIZEY / 2 + 257), Vector2(450, 72), Pivot::Center);
			break;
	}

	//선택된 아이템을 가져온다.
	_itemSelected = _vItem[_index];

	//뒤로가기
	if (_isClick)
	{
		_vItem[_index].isTry = true;
		_isOpenCheck = MENU_END;
		return MENU_END;
	}

	//렉트 알파값을 깜빡X2 하게 만든다.
	rcAlphaChange();

	//진행
	_isOpenCheck = MENU_PROGRESS;
	return _isOpenCheck;
}

void ItemManager::rcAlphaChange()
{
	if (_rcAlpha >= 0.9f)
		_rcAlphaChange = -0.03f;

	if (_rcAlpha <= 0.35f)
		_rcAlphaChange = 0.03f;

	_rcAlpha += _rcAlphaChange;
}

bool ItemManager::removeItem(string itemKey)
{
	for (int i = 0; i < _vItem.size(); i++)
	{
		//벡터에 존재하며 사용 가능한 상태인지?
		if (_vItem[i].itemKinds != (ITEMKINDS)conversion(itemKey)) continue;

		//해당 아이템 제거
		_vItem.erase(_vItem.begin() + i);

		return true;
	}
	return false;
}
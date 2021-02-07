#include "stdafx.h"
#include "ItemManager.h"

//�̴ϼȶ���¡ ���� �ʱ�ȭ �Ѵ�~
ItemManager::ItemManager():_rcAlpha(1.0f), _rcAlphaChange(0.03f), _index(FIRST)
{
}

ItemManager::~ItemManager()
{
}

bool ItemManager::addItem(string itemKey)
{
	//������ ���� ���� �ʱ�ȭ
	Item _item;
	_item.itemKinds = (ITEMKINDS)conversion(itemKey);
	_item.isTry = false;

	for (int i = 0; i < _vItem.size(); i++)
	{
		//���Ϳ� �����ϴ��� üũ�Ѵ�.
		if (_vItem[i].itemKinds != _item.itemKinds) continue;

		//�̹� �����ϴ� �������̶�� false�� ��ȯ�Ѵ�.
		return false;
	}

	//�����ۿ� ���� Ư���� ���Ѵ�.
	switch (_item.itemKinds)
	{
	case ITEM_TEDDYBEAR:
		_item.name = L"������";
		_item.contents = L"���� �𸣰� �ܷο� ���̴� ������";
		break;
	case ITEM_TEDDYBEARBODY:
		_item.name = L"�������� ����";
		_item.contents = L"�Ӹ��� ������� ������. �߸� �ܸ��� �Ӱ� ����� �ִ�";
		break;
	case ITEM_TEDDYBEARHAND:
		_item.name = L"�������� �չ�";
		_item.contents = L"������ �߶� ��. ���� ��굢�̷� ä���� �ִ�.";
		break;
	case ITEM_FROH:
		_item.name = L"������";
		_item.contents = L"����~ ����~ ���߿� �־��ּ�";
		break;
	case ITEM_TAEYUP:
		_item.name = L"�¿�";
		_item.contents = L"�ð迡�� ���� �콼�¿�";
	}
	_vItem.push_back(_item);

	//���������� �ȳ������� true�� ��ȯ�Ѵ�.
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
		//���Ϳ� �����ϴ��� üũ�Ѵ�.
		if (_vItem[i].itemKinds != (ITEMKINDS)itemNum && _vItem[i].isTry) continue;

		//�ش� ������ ����
		_vItem.erase(_vItem.begin() + i);

		//���������� ������ ���ɻ��¸� �����Ͽ��ٸ� true�� ��ȯ�Ѵ�.
		return true;
	}
	//�������� �������� ���� ��� false�� ��ȯ�Ѵ�.
	return false;
}

bool ItemManager::useItem(string itemKey)
{
	for (int i = 0; i < _vItem.size(); i++)
	{
		//���Ϳ� �����ϴ��� üũ�Ѵ�.
		if (_vItem[i].itemKinds != (ITEMKINDS)conversion(itemKey) && _vItem[i].isTry) continue;

		//�ش� ������ ����
		_vItem.erase(_vItem.begin() + i);

		//���������� ������ ���ɻ��¸� �����Ͽ��ٸ� true�� ��ȯ�Ѵ�.
		return true;
	}
	//�������� �������� ���� ��� false�� ��ȯ�Ѵ�.
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

	//��׶��� �÷� ��Ʈ
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
			//��Ʈ ���� �𸣰ٽ�..
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
			//��Ʈ ���� �𸣰ٽ�..
		);
	}

	//�׵θ� �÷� ��Ʈ
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
	//ZŰ�� ������ �ʾ��� ��� â�� ������ �ʴ´�. 
	//�ӽ÷� �߰��� keyinput �޴��� ��ġ�� ���� �����Ұ�
	bool _isClick = false;

	if (!_isOpenCheck) _isOpenCheck = MENU_PROGRESS;

	if (KEYMANAGER->isOnceKeyDown(VK_SPACE))	_isClick = true;	

	if (KEYMANAGER->isOnceKeyDown('X') && _isOpenCheck)
	{
		_isOpenCheck = MENU_BACK;
		_isClick = false;
	}

	//������ ����� 0 ���϶��(�������� ���� ���� �ʴٸ�) �Ʒ� �ڵ�� �����Ѵ�.
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

	//����
	if(!_isOpenCheck)
		return MENU_BACK;



	for (int i = 0; i < _vItem.size(); i++)
	{
		//�ش� ������ ��� �õ��� �ʱ�ȭ ���ش�.
		_vItem[i].isTry = false;
	}

	//����Ű ����
	if (KEYMANAGER->isOnceKeyDown(VK_LEFT))		_index = (INDEX)(_index - 1 > -1 ? _index - 1 : _index);
	if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))	_index = (INDEX)(_index + 1 < 4 ? _index + 1 : _index);
	if (KEYMANAGER->isOnceKeyDown(VK_DOWN))		_index = (INDEX)(_index + 2 < 4 ? _index + 2 : _index);
	if (KEYMANAGER->isOnceKeyDown(VK_UP))		_index = (INDEX)(_index - 2 > -1 ? _index - 2 : _index);

	//���� ������� �ε����� Ŭ ��� �ε����� ������ max �ε����� �ִ´�
	_index = (INDEX)(_index >= _vItem.size() ? _vItem.size() - 1 : _index);

	//�ε����� ���� ���õǴ� ȿ���� �����ش�.(��Ʈ)
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

	//���õ� �������� �����´�.
	_itemSelected = _vItem[_index];

	//�ڷΰ���
	if (_isClick)
	{
		_vItem[_index].isTry = true;
		_isOpenCheck = MENU_END;
		return MENU_END;
	}

	//��Ʈ ���İ��� ����X2 �ϰ� �����.
	rcAlphaChange();

	//����
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
		//���Ϳ� �����ϸ� ��� ������ ��������?
		if (_vItem[i].itemKinds != (ITEMKINDS)conversion(itemKey)) continue;

		//�ش� ������ ����
		_vItem.erase(_vItem.begin() + i);

		return true;
	}
	return false;
}
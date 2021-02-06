#include "stdafx.h"
#include "StorageManager.h"

StorageManager::StorageManager()
{
}

StorageManager::~StorageManager()
{
}

HRESULT StorageManager::init()
{
	_selectedStorage = FIRST;

	//���ϵ��� ����? �ҷ�����
	loadTitle();

	return S_OK;
}

void StorageManager::loadTitle()
{
	//�ش� ���� �ȿ� �����ϴ�(�ִ�5��)
	//���ϵ��� ���� �������
}

MENUSTATE StorageManager::saveView()
{
	//�ڷΰ��� ���� Ű
	//Ű�� �ȸ����� �ѹ��� üũ�ϴ� �뵵? 'X' int ���� ������
	if (KEYMANAGER->isOnceKeyDown('X') * (int) 'X')
		return MENU_BACK;

	//����� ����
	storageOpen();

	if (_isClick)
	{
		//if(saveData()) //���� ���� ����
		//������ �����Ͽ��ٸ�
		//loadTitle(); Ÿ��Ʋ�� �ٽ� �����ش�.(����� ������)
		//_isClick=fales;
		//return MENU_END(�޴����� ���� �Ϸ�� �Ǵ��ϸ� ��)
	}

	return MENU_PROGRESS;
}

MENUSTATE StorageManager::loadView()
{
	//�ڷΰ��� ���� Ű
	//Ű�� �ȸ����� �ѹ��� üũ�ϴ� �뵵? 'X' int ���� ������
	if (KEYMANAGER->isOnceKeyDown('X') * (int) 'X')
		return MENU_BACK;

	//����� ����
	storageOpen();

	if (_isClick)
	{
		//if(loadData()) //����� ������ �ҷ����� �Լ�
		//������ �ҷ��Դٸ�
		//_isClick=fales;
		// return MENU_END(�޴����� �ҷ������ �Ǵ��ϸ��)
	}

	return MENU_PROGRESS;
}

bool StorageManager::saveData()
{
	//���� ����
	//���� �Ϸ��Ѵٸ� true�� ��ȯ
	return false;
}

bool StorageManager::loadData()
{
	//���� �ε�
	//Ÿ��Ʋ�� ������� �������� �����ȿ� ���ϸ�� ��ġ�ϴ� ������ �ҷ���
	//�ε� �Ϸ��Ѵٸ� true�� ��ȯ
	return false;
}

//MENU_BACK = 0, MENU_PROGRESS, MENU_END
void StorageManager::storageOpen()
{
	//����Ű ����
	if (!_isClick && KEYMANAGER->isOnceKeyDown(VK_DOWN))
		_selectedStorage = (STORAGE)(_selectedStorage == FIFTH ? FIRST : (int)_selectedStorage + 1);

	if (!_isClick && KEYMANAGER->isOnceKeyDown(VK_UP))
		_selectedStorage = (STORAGE)(_selectedStorage == FIRST ? FIFTH : (int)_selectedStorage - 1);

	switch (_selectedStorage)
	{
	case StorageManager::FIRST:
		_rcSelected = RectMakePivot(Vector2(WINSIZEX / 2 - 10, 230), Vector2(1100, 100), Pivot::Center);
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
			_isClick = true;
		break;
	case StorageManager::SECOND:
		_rcSelected = RectMakePivot(Vector2(WINSIZEX / 2, 330), Vector2(1100, 100), Pivot::Center);
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
			_isClick = true;
		break;
	case StorageManager::THIRD:
		_rcSelected = RectMakePivot(Vector2(WINSIZEX / 2, 430), Vector2(1100, 100), Pivot::Center);
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
			_isClick = true;
		break;
	case StorageManager::FOURTH:
		_rcSelected = RectMakePivot(Vector2(WINSIZEX / 2, 530), Vector2(1100, 100), Pivot::Center);
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
			_isClick = true;
		break;
	case StorageManager::FIFTH:
		_rcSelected = RectMakePivot(Vector2(WINSIZEX / 2, 630), Vector2(1100, 100), Pivot::Center);
		break;
	}

	rcAlphaChange();
}

void StorageManager::rcAlphaChange()
{
	if (_rcAlpha >= 0.9f)
		_rcAlphaChange = -0.03f;

	if (_rcAlpha <= 0.35f)
		_rcAlphaChange = 0.03f;

	_rcAlpha += _rcAlphaChange;
}

void StorageManager::render()
{
	//��׶��� ���� �� ����~
	ImageManager::GetInstance()->FindImage("menu")->SetSize(Vector2(1280, 720));
	ImageManager::GetInstance()->FindImage("menu")->Render(Vector2(WINSIZEX / 2, WINSIZEY / 2));

	ImageManager::GetInstance()->FindImage("messe")->SetSize(Vector2(1240, 120));
	ImageManager::GetInstance()->FindImage("messe")->Render(Vector2(WINSIZEX / 2, 75));

	ImageManager::GetInstance()->FindImage("messe")->SetSize(Vector2(1240, 600));
	ImageManager::GetInstance()->FindImage("messe")->Render(Vector2(WINSIZEX / 2, WINSIZEY / 2 + 75));

	//��׶��� �÷� ��Ʈ
	D2DINS->FillRectangle
	(
		_rcSelected,
		D2D1::ColorF::Enum::WhiteSmoke,
		_rcAlpha / 3.5
	);

	//for (int i = 0; i < _vText.size(); i++)
	//{
	//	SAVE sel = (SAVE)i;
	//	D2DINS->RenderText
	//	(
	//		_vText[I]
	//		_mText.find(sel)->second.x - _mText.find(sel)->second.size * 2,
	//		_mText.find(sel)->second.y + i * 70,
	//		_mText.find(sel)->second.text,
	//		RGB(255, 255, 255),
	//		_sceneAlpha,
	//		_mText.find(sel)->second.size,
	//		DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_TRAILING
	//		//��Ʈ ���� �𸣰ٽ�..
	//	);
	//}

	//�׵θ� �÷� ��Ʈ
	D2DINS->DrawRectangle
	(
		_rcSelected,
		D2D1::ColorF::Enum::White,
		_rcAlpha,
		3.f
	);
}

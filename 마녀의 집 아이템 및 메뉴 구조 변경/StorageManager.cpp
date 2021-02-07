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
	for (int i = 1; i <= 5; i++)
	{
		_vFileSlot.push_back("");
	}
	//���ϵ��� ����? �ҷ�����
	loadTitle();

	return S_OK;
}

void StorageManager::loadTitle()
{
	//�ش� ���� �ȿ� �����ϴ�(�ִ�5��)
	//���ϵ��� ���� �������
	//_vFileSlot[0]=INIDATA->loadDataString("Save1", "��������", "�������� �̸�");
	//_vFileSlot[1] = INIDATA->loadDataString("Save2", "��������", "�������� �̸�");
	//_vFileSlot[2] = INIDATA->loadDataString("Save3", "��������", "�������� �̸�");
	//_vFileSlot[3] = INIDATA->loadDataString("Save4", "��������", "�������� �̸�");
	//_vFileSlot[4] = INIDATA->loadDataString("Save5", "��������", "�������� �̸�");
}

MENUSTATE StorageManager::saveView()
{
	if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
		_toggle = !_toggle;
	//�ڷΰ��� ���� Ű
	//Ű�� �ȸ����� �ѹ��� üũ�ϴ� �뵵? 'X' int ���� ������
	if (KEYMANAGER->isOnceKeyDown('X') * (int) 'X')
		return _toggle == true ? MENU_PROGRESS : MENU_BACK;
	
	/*_vFileSlot[0] = INIDATA->loadDataString("Save1", "��������", "�������� �̸�");
	_vFileSlot[1] = INIDATA->loadDataString("Save2", "��������", "�������� �̸�");
	_vFileSlot[2] = INIDATA->loadDataString("Save3", "��������", "�������� �̸�");
	_vFileSlot[3] = INIDATA->loadDataString("Save4", "��������", "�������� �̸�");
	_vFileSlot[4] = INIDATA->loadDataString("Save5", "��������", "�������� �̸�");*/

	//����� ����
	storageOpen();

	if (_isClick)
	{
		//if(saveData()) //���� ���� ����
		//������ �����Ͽ��ٸ�
		//loadTitle(); Ÿ��Ʋ�� �ٽ� �����ش�.(����� ������)
		//_isClick=fales;
		//return MENU_END(�޴����� ���� �Ϸ�� �Ǵ��ϸ� ��)
		saveData();
	}

	return _toggle == true ? MENU_PROGRESS : MENU_BACK;
}

MENUSTATE StorageManager::loadView()
{
	//�ڷΰ��� ���� Ű
	//Ű�� �ȸ����� �ѹ��� üũ�ϴ� �뵵? 'X' int ���� ������
	if (KEYMANAGER->isOnceKeyDown('X') * (int) 'X')
		return MENU_BACK;
	if (KEYMANAGER->isOnceKeyDown(VK_SPACE)*0x20)
		_toggle = !_toggle;
		//����� ����
	storageOpen();

	if (_isClick)
	{
		//if(loadData()) //����� ������ �ҷ����� �Լ�
		//������ �ҷ��Դٸ�
		//_isClick=fales;
		// return MENU_END(�޴����� �ҷ������ �Ǵ��ϸ��)


	}

	if (_toggle)
		return MENU_PROGRESS;
	return MENU_BACK;
}

bool StorageManager::saveData()
{
	//���� ����
	//���� �Ϸ��Ѵٸ� true�� ��ȯ
	save_enum = (int)_saveStage;
	save_s_enum = to_string(save_enum);
	/*switch (_saveStage)
	{
	case OPENING:
		INIDATA->addData("��������", "�������� �̸�", "������ ����");
		INIDATA->addData("��������", "�������� ��ȣ(OPENING:0,FINAL:5)", save_s_enum.c_str());
		INIDATA->iniSave(fileName);
		break;
	case FIRSTSTAGE:
		INIDATA->addData("��������", "�������� �̸�", "������ �� 1��");
		INIDATA->addData("��������", "�������� ��ȣ(OPENING:0,FINAL:5)", save_s_enum.c_str());
		INIDATA->iniSave(fileName);
		break;
	case SECONDSTAGE:
		INIDATA->addData("��������", "�������� �̸�", "������ �� 2��");
		INIDATA->addData("��������", "�������� ��ȣ(OPENING:0,FINAL:5)", save_s_enum.c_str());
		INIDATA->iniSave(fileName);
		break;
	case THIRDSTAGE:
		INIDATA->addData("��������", "�������� �̸�", "������ �� 3��");
		INIDATA->addData("��������", "�������� ��ȣ(OPENING:0,FINAL:5)", save_s_enum.c_str());
		INIDATA->iniSave(fileName);
		break;
	case FOURTHSTAGE:
		INIDATA->addData("��������", "�������� �̸�", "������ �� 4��");
		INIDATA->addData("��������", "�������� ��ȣ(OPENING:0,FINAL:5)", save_s_enum.c_str());
		INIDATA->iniSave(fileName);
		break;
	case FINALSTAGE:
		INIDATA->addData("��������", "�������� �̸�", "������ ��");
		INIDATA->addData("��������","�������� ��ȣ(OPENING:0,FINAL:5)",save_s_enum.c_str());
		INIDATA->iniSave(fileName);
		break;
	}*/
	return false;
}

bool StorageManager::loadData()
{
	//���� �ε�
	//Ÿ��Ʋ�� ������� �������� �����ȿ� ���ϸ�� ��ġ�ϴ� ������ �ҷ���
	//�ε� �Ϸ��Ѵٸ� true�� ��ȯ
	switch (_saveStage)
	{
	case OPENING:
		break;
	case FIRSTSTAGE:
		break;
	case SECONDSTAGE:
		break;
	case THIRDSTAGE:
		break;
	case FOURTHSTAGE:
		break;
	case FINALSTAGE:
		break;
	}
	return false;
}

//MENU_BACK = 0, MENU_PROGRESS, MENU_END
void StorageManager::storageOpen()
{
	//����Ű ����
	if (!_isClick && KEYMANAGER->isOnceKeyDown(VK_DOWN)*0x28)
		_selectedStorage = (STORAGE)(_selectedStorage == FIFTH ? FIRST : (int)_selectedStorage + 1);

	if (!_isClick && KEYMANAGER->isOnceKeyDown(VK_UP)*0x26)
		_selectedStorage = (STORAGE)(_selectedStorage == FIRST ? FIFTH : (int)_selectedStorage - 1);

	switch (_selectedStorage)
	{
	case StorageManager::FIRST:
		_rcSelected = RectMakePivot(Vector2(WINSIZEX / 2 - 10, 230), Vector2(1100, 100), Pivot::Center);
		if (KEYMANAGER->isOnceKeyDown(/*VK_SPACE*/'Z'))
		{
			_isClick = true;
			fileName = "Save1";
		}
		if (KEYMANAGER->isOnceKeyUp('Z'))_isClick = false;
		break;
	case StorageManager::SECOND:
		_rcSelected = RectMakePivot(Vector2(WINSIZEX / 2, 330), Vector2(1100, 100), Pivot::Center);
		if (KEYMANAGER->isOnceKeyDown(/*VK_SPACE*/'Z'))
		{
			_isClick = true;
			fileName = "Save2";
		}		
		if (KEYMANAGER->isOnceKeyUp('Z'))_isClick = false;
		break;
	case StorageManager::THIRD:
		_rcSelected = RectMakePivot(Vector2(WINSIZEX / 2, 430), Vector2(1100, 100), Pivot::Center);
		if (KEYMANAGER->isOnceKeyDown(/*VK_SPACE*/'Z'))
		{
			_isClick = true;
			fileName = "Save3";
		}
		if (KEYMANAGER->isOnceKeyUp('Z'))_isClick = false;
		break;
	case StorageManager::FOURTH:
		_rcSelected = RectMakePivot(Vector2(WINSIZEX / 2, 530), Vector2(1100, 100), Pivot::Center);
		if (KEYMANAGER->isOnceKeyDown(/*VK_SPACE*/'Z'))
		{
			_isClick = true;
			fileName = "Save4";
		}
		if (KEYMANAGER->isOnceKeyUp('Z'))_isClick = false;
		break;
	case StorageManager::FIFTH:
		_rcSelected = RectMakePivot(Vector2(WINSIZEX / 2, 630), Vector2(1100, 100), Pivot::Center);
		if (KEYMANAGER->isOnceKeyDown(/*VK_SPACE*/'Z'))
		{
			_isClick = true;
			fileName = "Save5";
		}
		if (KEYMANAGER->isOnceKeyUp('Z'))_isClick = false;
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
	//ImageManager::GetInstance()->FindImage("menu")->SetSize(Vector2(1280, 720));
	//ImageManager::GetInstance()->FindImage("menu")->Render(Vector2(WINSIZEX / 2, WINSIZEY / 2));
	if (!_toggle)return;
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
	for (int i = 0; i < _vFileSlot.size(); i++)
	{
		D2DINS->RenderTextField(100, 220 + i * 100, stringToWstring(_vFileSlot[i]), 40, 1100, 10,D2DRenderer::DefaultBrush::White,DWRITE_TEXT_ALIGNMENT_CENTER);
	}
}

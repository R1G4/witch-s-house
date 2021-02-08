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
	_vFileSlot[0] = INIDATA->loadDataString("Save1", "��������", "�������� �̸�");
	_vFileSlot[1] = INIDATA->loadDataString("Save2", "��������", "�������� �̸�");
	_vFileSlot[2] = INIDATA->loadDataString("Save3", "��������", "�������� �̸�");
	_vFileSlot[3] = INIDATA->loadDataString("Save4", "��������", "�������� �̸�");
	_vFileSlot[4] = INIDATA->loadDataString("Save5", "��������", "�������� �̸�");
}

MENUSTATE StorageManager::saveView()
{
	//xŰ�� �����ٸ� �ڷΰ��⸦ �Ѵ�.
	//xŰ �ߺ� �̽��� �߻��� �� ������ �ƽ�Ű �ڵ带 �ѹ��� �����ش�.
	if (_isOpen && KEYMANAGER->isOnceKeyDown('X') * (int) 'X')
	{
		//����� �������¸� false�� �θ� �����Ѵ�.
		_isOpen = false;
		return MENU_BACK;
	}

	//�޴��� ������ �ʾҰ� ����Ű�� ������ ��� �޴��� true�� �ϸ� �����ش�.
	if (!_isOpen && KEYMANAGER->isOnceKeyDown(VK_SPACE))
	{
		_divisionText = L"� ������ �����Ͻðڽ��ϱ�?";
		_isOpen = true;
	}

	//������ ����.
	_vFileSlot[0] = INIDATA->loadDataString("Save1", "��������", "�������� �̸�");
	_vFileSlot[1] = INIDATA->loadDataString("Save2", "��������", "�������� �̸�");
	_vFileSlot[2] = INIDATA->loadDataString("Save3", "��������", "�������� �̸�");
	_vFileSlot[3] = INIDATA->loadDataString("Save4", "��������", "�������� �̸�");
	_vFileSlot[4] = INIDATA->loadDataString("Save5", "��������", "�������� �̸�");

	//����� ���� (�����ϴ� �������� ��Ʈ�� �Ѵ�.)
	storageOpen();

	//Ŭ���� �� �����̰�
	if (_isClick)
	{
		//�ҷ����⸦ ���� ������
		if (loadData())
		{
			//������ �Ϸ� �� ���.
			if (saveData())
			{
				//����� â�� �����Ѵ�.
				_isOpen = false;

				//���� ���¸� �ʱ�ȭ �Ѵ�.
				_isClick = false;
				
				//�ڷΰ��⸦ ��ȯ�Ѵ�. ( SAVE�� �޴�â�� ���� �ʱ⿡ �ٷ� �ڷΰ��⸦ �Ѵ�.)
				return MENU_BACK;
			}
		}
	}

	//�׿� ���¿� ���� �������¸� ��ȯ�Ѵ�.
	return _isOpen == true ? MENU_PROGRESS : MENU_BACK;
}

MENUSTATE StorageManager::loadView()
{
	//�ڷΰ��� ���� Ű
	//Ű�� �ȸ����� �ѹ��� üũ�ϴ� �뵵? 'X' int ���� ������
	if (_isOpen && KEYMANAGER->isOnceKeyDown('X') * (int) 'X')
	{
		_isOpen = false;
		return MENU_BACK;
	}

	if (!_isOpen && KEYMANAGER->isOnceKeyDown(VK_SPACE))
	{
		_divisionText = L"� ������ �ҷ����ðڽ��ϱ�?";
		_isOpen = true;
	}

	//����� ����
	storageOpen();

	if (_isClick)
	{
		//�ҷ����⸦ �����ߴٸ� â�� �����Ѵ�.
		if (loadData())
		{
			_isOpen = false;
			_isClick = false;
			return MENU_END;
		}
	}

	if (_isOpen)
		return MENU_PROGRESS;

	return MENU_BACK;
}

bool StorageManager::saveData()
{
	//���� ����
	//������ ���������� �Ǿ����� üũ�Ѵ� 
	//���� ���� �� ��� true�� ��ȯ�ϸ� �ɼ� â�� ���û��¸� �ʱ�ȭ �Ѵ�.
	save_enum = (int)_saveStage;
	save_s_enum = to_string(save_enum);
	switch (_saveStage)
	{
	case OPENING:
		INIDATA->addData("��������", "�������� �̸�", "������ ����");
		INIDATA->addData("��������", "�������� ��", "����");
		INIDATA->addData("��������", "�������� ��ȣ(OPENING:0,FINAL:5)", save_s_enum.c_str());
		INIDATA->iniSave(fileName);
		return true;
	case FIRSTSTAGE:
		INIDATA->addData("��������", "�������� �̸�", "������ �� 1��");
		INIDATA->addData("��������", "�������� ��", "entrance");
		INIDATA->addData("��������", "�������� ��ȣ(OPENING:0,FINAL:5)", save_s_enum.c_str());
		INIDATA->iniSave(fileName);
		return true;
	case SECONDSTAGE:
		INIDATA->addData("��������", "�������� �̸�", "������ �� 2��");
		INIDATA->addData("��������", "�������� ��", "thirdMain");
		INIDATA->addData("��������", "�������� ��ȣ(OPENING:0,FINAL:5)", save_s_enum.c_str());
		INIDATA->iniSave(fileName);
		return true;
	case THIRDSTAGE:
		INIDATA->addData("��������", "�������� �̸�", "������ �� 3��");
		INIDATA->addData("��������", "�������� ��", "4�� Ȧ");
		INIDATA->addData("��������", "�������� ��ȣ(OPENING:0,FINAL:5)", save_s_enum.c_str());
		INIDATA->iniSave(fileName);
		return true;
	case FOURTHSTAGE:
		INIDATA->addData("��������", "�������� �̸�", "������ �� 4��");
		INIDATA->addData("��������", "�������� ��ȣ(OPENING:0,FINAL:5)", save_s_enum.c_str());

		_save_x = (int)(_player_x);
		_save_y = (int)(_player_y);
		_save_s_x = to_string(_save_x);
		_save_s_y = to_string(_save_y);

		_save_obj[0] = to_string(STAGEMEMORYMANAGER->getIsSkul1());
		_save_obj[1] = to_string(STAGEMEMORYMANAGER->getIsSkul2());
		_save_obj[2] = to_string(STAGEMEMORYMANAGER->getIsSkul3());
		_save_obj[3] = to_string(STAGEMEMORYMANAGER->getIsSkul4());
		_save_obj[4] = to_string(STAGEMEMORYMANAGER->getIsSkul5());
		_save_obj[5] = to_string(STAGEMEMORYMANAGER->getIsRedFlower1());
		_save_obj[6] = to_string(STAGEMEMORYMANAGER->getIsRedFlower2());
		_save_obj[7] = to_string(STAGEMEMORYMANAGER->getIsRedFlower3());
		_save_obj[8] = to_string(STAGEMEMORYMANAGER->getIsGetSkul1());
		_save_obj[9] = to_string(STAGEMEMORYMANAGER->getIsGetSkul2());
		_save_obj[10] = to_string(STAGEMEMORYMANAGER->getIsGetSkul3());
		_save_obj[11] = to_string(STAGEMEMORYMANAGER->getIsGetSkul4());
		_save_obj[12] = to_string(STAGEMEMORYMANAGER->getIsFlowerDead());
		_save_obj[13] = to_string(STAGEMEMORYMANAGER->getIsPotion());
		_save_obj[14] = to_string(STAGEMEMORYMANAGER->getIsKey());
		_save_obj[15] = to_string(STAGEMEMORYMANAGER->getIsLever());
		_save_obj[16] = to_string(STAGEMEMORYMANAGER->getIsFlowerpot());

		if (_real_location == GARDEN)
			INIDATA->addData("��������", "�������� ��", "garden_5f");
		else if (_real_location == GARDEN_TO_BOSS)
			INIDATA->addData("��������", "�������� ��", "gardenToBoss_5f");
		else if (_real_location == DININGROOM)
			INIDATA->addData("��������", "�������� ��", "diningRoom_5f");
		else if (_real_location == PRISON)
			INIDATA->addData("��������", "�������� ��", "prison_5f");
		else if (_real_location == PRISON_WELL)
			INIDATA->addData("��������", "�������� ��", "prison_well_5f");
		else if (_real_location == PRISON_SKUL)
			INIDATA->addData("��������", "�������� ��", "prison_skul_5f");

		INIDATA->addData("������Ʈ", "�ذ�1", _save_obj[0].c_str());
		INIDATA->addData("������Ʈ", "�ذ�2", _save_obj[1].c_str());
		INIDATA->addData("������Ʈ", "�ذ�3", _save_obj[2].c_str());
		INIDATA->addData("������Ʈ", "�ذ�4", _save_obj[3].c_str());
		INIDATA->addData("������Ʈ", "�ذ�5", _save_obj[4].c_str());
		INIDATA->addData("������Ʈ", "������1", _save_obj[5].c_str());
		INIDATA->addData("������Ʈ", "������2", _save_obj[6].c_str());
		INIDATA->addData("������Ʈ", "������3", _save_obj[7].c_str());
		INIDATA->addData("������Ʈ", "�ذ��1", _save_obj[8].c_str());
		INIDATA->addData("������Ʈ", "�ذ��2", _save_obj[9].c_str());
		INIDATA->addData("������Ʈ", "�ذ��3", _save_obj[10].c_str());
		INIDATA->addData("������Ʈ", "�ذ��4", _save_obj[11].c_str());
		INIDATA->addData("������Ʈ", "�Ͼ������", _save_obj[12].c_str());
		INIDATA->addData("������Ʈ", "����", _save_obj[13].c_str());
		INIDATA->addData("������Ʈ", "����", _save_obj[14].c_str());
		INIDATA->addData("������Ʈ", "����", _save_obj[15].c_str());
		INIDATA->addData("������Ʈ", "�ɺ�", _save_obj[16].c_str());
		INIDATA->iniSave(fileName);
		return true;
		break;
	case FINALSTAGE:
		INIDATA->addData("��������", "�������� �̸�", "������ ��");
		INIDATA->addData("��������", "�������� ��", "BossStage1");
		INIDATA->addData("��������", "�������� ��ȣ(OPENING:0,FINAL:5)", save_s_enum.c_str());
		INIDATA->iniSave(fileName); 
		return true;
	}
	return false;
}

bool StorageManager::loadData()
{
	//���� �ε�
	//�ε� ���������� �Ǿ����� üũ�Ѵ� 
	//�ε� ���� �� ��� true�� ��ȯ�ϸ� �ɼ� â�� ���û��¸� �ʱ�ȭ �Ѵ�.
	_saveStage = (STAGE)INIDATA->loadDataInterger(fileName, "��������", "�������� ��ȣ(OPENING:0,FINAL:5)");
	switch (_saveStage)
	{
	case OPENING:
		return true;
	case FIRSTSTAGE:
		return true;
	case SECONDSTAGE:
		return true;
	case THIRDSTAGE:
		return true;
	case FOURTHSTAGE:
		STAGEMEMORYMANAGER->setIsSkul1(INIDATA->loadDataInterger(fileName, "������Ʈ", "�ذ�1"));
		STAGEMEMORYMANAGER->setIsSkul2(INIDATA->loadDataInterger(fileName, "������Ʈ", "�ذ�2"));
		STAGEMEMORYMANAGER->setIsSkul3(INIDATA->loadDataInterger(fileName, "������Ʈ", "�ذ�3"));
		STAGEMEMORYMANAGER->setIsSkul4(INIDATA->loadDataInterger(fileName, "������Ʈ", "�ذ�4"));
		STAGEMEMORYMANAGER->setIsSkul5(INIDATA->loadDataInterger(fileName, "������Ʈ", "�ذ�5"));
		STAGEMEMORYMANAGER->setIsRedFlower1(INIDATA->loadDataInterger(fileName, "������Ʈ", "������1"));
		STAGEMEMORYMANAGER->setIsRedFlower2(INIDATA->loadDataInterger(fileName, "������Ʈ", "������2"));
		STAGEMEMORYMANAGER->setIsRedFlower3(INIDATA->loadDataInterger(fileName, "������Ʈ", "������3"));
		STAGEMEMORYMANAGER->setIsGetSkul1(INIDATA->loadDataInterger(fileName, "������Ʈ", "�ذ��1"));
		STAGEMEMORYMANAGER->setIsGetSkul2(INIDATA->loadDataInterger(fileName, "������Ʈ", "�ذ��2"));
		STAGEMEMORYMANAGER->setIsGetSkul3(INIDATA->loadDataInterger(fileName, "������Ʈ", "�ذ��3"));
		STAGEMEMORYMANAGER->setIsGetSkul4(INIDATA->loadDataInterger(fileName, "������Ʈ", "�ذ��4"));
		STAGEMEMORYMANAGER->setIsFlowerDead(INIDATA->loadDataInterger(fileName, "������Ʈ", "�Ͼ������"));
		STAGEMEMORYMANAGER->setIsPotion(INIDATA->loadDataInterger(fileName, "������Ʈ", "����"));
		STAGEMEMORYMANAGER->setIsKey(INIDATA->loadDataInterger(fileName, "������Ʈ", "����"));
		STAGEMEMORYMANAGER->setIsLever(INIDATA->loadDataInterger(fileName, "������Ʈ", "����"));
		STAGEMEMORYMANAGER->setIsFlowerpot(INIDATA->loadDataInterger(fileName, "������Ʈ", "�ɺ�"));

		_stage = INIDATA->loadDataString(fileName, "��������", "�������� ��");
		_playerDirection = (CHRDIRECTION)INIDATA->loadDataInterger(fileName, "��ö�", "����");
		_playerX = INIDATA->loadDataInterger(fileName, "��ö�", "x��ǥ");
		_playerY = INIDATA->loadDataInterger(fileName, "��ö�", "y��ǥ");
		SCENEMANAGER->changeScene(_stage, _playerX, _playerY, _playerDirection);
		return true;
	case FINALSTAGE:
		_stage = INIDATA->loadDataString(fileName, "��������", "�������� ��");
		_playerDirection = (CHRDIRECTION)INIDATA->loadDataInterger(fileName, "��ö�", "����");
		_playerX = INIDATA->loadDataInterger(fileName, "��ö�", "x��ǥ");
		_playerY = INIDATA->loadDataInterger(fileName, "��ö�", "y��ǥ");
		SCENEMANAGER->changeScene(_stage, _playerX, _playerY, _playerDirection);
		return true;
	}
	return false;
}

void StorageManager::storageOpen()
{
	//����� â�� ������ �ʾ��� ��� ������ ���� �ʴ´�.
	if (!_isOpen) return;

	//����Ű ����
	if (!_isClick && KEYMANAGER->isOnceKeyDown(VK_DOWN)*0x28)
		_selectedStorage = (STORAGE)(_selectedStorage == FIFTH ? FIRST : (int)_selectedStorage + 1);

	if (!_isClick && KEYMANAGER->isOnceKeyDown(VK_UP)*0x26)
		_selectedStorage = (STORAGE)(_selectedStorage == FIRST ? FIFTH : (int)_selectedStorage - 1);

	switch (_selectedStorage)
	{
	case StorageManager::FIRST:
		_rcSelected = RectMakePivot(Vector2(WINSIZEX / 2 - 10, 230), Vector2(1100, 100), Pivot::Center);
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE) * 0x20)
		{
			_isClick = true;
			fileName = "Save1";
		}
		if (KEYMANAGER->isOnceKeyUp(VK_SPACE))_isClick = false;
		break;
	case StorageManager::SECOND:
		_rcSelected = RectMakePivot(Vector2(WINSIZEX / 2, 330), Vector2(1100, 100), Pivot::Center);
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE) * 0x20)
		{
			_isClick = true;
			fileName = "Save2";
		}		
		if (KEYMANAGER->isOnceKeyUp(VK_SPACE))_isClick = false;
		break;
	case StorageManager::THIRD:
		_rcSelected = RectMakePivot(Vector2(WINSIZEX / 2, 430), Vector2(1100, 100), Pivot::Center);
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE) * 0x20)
		{
			_isClick = true;
			fileName = "Save3";
		}
		if (KEYMANAGER->isOnceKeyUp(VK_SPACE))_isClick = false;
		break;
	case StorageManager::FOURTH:
		_rcSelected = RectMakePivot(Vector2(WINSIZEX / 2, 530), Vector2(1100, 100), Pivot::Center);
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE) * 0x20)
		{
			_isClick = true;
			fileName = "Save4";
		}
		if (KEYMANAGER->isOnceKeyUp(VK_SPACE))_isClick = false;
		break;
	case StorageManager::FIFTH:
		_rcSelected = RectMakePivot(Vector2(WINSIZEX / 2, 630), Vector2(1100, 100), Pivot::Center);
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE) * 0x20)
		{
			_isClick = true;
			fileName = "Save5";
		}
		if (KEYMANAGER->isOnceKeyUp(VK_SPACE))_isClick = false;
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
	//����� â�� ������ �ʾҴٸ� ������ ���� �ʴ´�.
	if (!_isOpen)return;

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
	/*(const int x, const int y, const wstring& text, const int size,
		const DefaultBrush& defaultBrush, const DWRITE_TEXT_ALIGNMENT& align, const wstring& font)
*/
	D2DINS->RenderText
	(
		2,
		35,
		_divisionText,
		48,
		D2DRenderer::DefaultBrush::White,
		DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_TRAILING
	);

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

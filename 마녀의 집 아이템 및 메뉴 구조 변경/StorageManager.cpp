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
	if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
		_toggle = !_toggle;
	//�ڷΰ��� ���� Ű
	//Ű�� �ȸ����� �ѹ��� üũ�ϴ� �뵵? 'X' int ���� ������
	if (KEYMANAGER->isOnceKeyDown('X') * (int) 'X')
		return _toggle == true ? MENU_PROGRESS : MENU_BACK;
	
	_vFileSlot[0] = INIDATA->loadDataString("Save1", "��������", "�������� �̸�");
	_vFileSlot[1] = INIDATA->loadDataString("Save2", "��������", "�������� �̸�");
	_vFileSlot[2] = INIDATA->loadDataString("Save3", "��������", "�������� �̸�");
	_vFileSlot[3] = INIDATA->loadDataString("Save4", "��������", "�������� �̸�");
	_vFileSlot[4] = INIDATA->loadDataString("Save5", "��������", "�������� �̸�");

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

		loadData();
		return MENU_END;
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
	switch (_saveStage)
	{
	case OPENING:
		INIDATA->addData("��������", "�������� �̸�", "������ ����");
		INIDATA->addData("��������", "�������� ��", "����");
		INIDATA->addData("��������", "�������� ��ȣ(OPENING:0,FINAL:5)", save_s_enum.c_str());
		INIDATA->iniSave(fileName);
		break;
	case FIRSTSTAGE:
		INIDATA->addData("��������", "�������� �̸�", "������ �� 1��");
		INIDATA->addData("��������", "�������� ��", "entrance");
		INIDATA->addData("��������", "�������� ��ȣ(OPENING:0,FINAL:5)", save_s_enum.c_str());
		INIDATA->iniSave(fileName);
		break;
	case SECONDSTAGE:
		INIDATA->addData("��������", "�������� �̸�", "������ �� 2��");
		INIDATA->addData("��������", "�������� ��", "thirdMain");
		INIDATA->addData("��������", "�������� ��ȣ(OPENING:0,FINAL:5)", save_s_enum.c_str());
		INIDATA->iniSave(fileName);
		break;
	case THIRDSTAGE:
		INIDATA->addData("��������", "�������� �̸�", "������ �� 3��");
		INIDATA->addData("��������", "�������� ��", "4�� Ȧ");
		INIDATA->addData("��������", "�������� ��ȣ(OPENING:0,FINAL:5)", save_s_enum.c_str());
		INIDATA->iniSave(fileName);
		break;
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
			INIDATA->addData("��������5", "5�����̸�", "garden_5f");
		else if (_real_location == GARDEN_TO_BOSS)
			INIDATA->addData("��������5", "5�����̸�", "gardenToBoss_5f");
		else if (_real_location == DININGROOM)
			INIDATA->addData("��������5", "5�����̸�", "diningRoom_5f");
		else if (_real_location == PRISON)
			INIDATA->addData("��������5", "5�����̸�", "prison_5f");
		else if (_real_location == PRISON_WELL)
			INIDATA->addData("��������5", "5�����̸�", "prison_well_5f");
		else if (_real_location == PRISON_SKUL)
			INIDATA->addData("��������5", "5�����̸�", "prison_skul_5f");

		INIDATA->addData("��ö�", "x��ǥ", _save_s_x.c_str());
		INIDATA->addData("��ö�", "y��ǥ", _save_s_y.c_str());
		if (_player_direction == CHRDIREC_LEFT)
			INIDATA->addData("��ö�", "������", "CHRDIREC_LEFT");
		else if (_player_direction == CHRDIREC_RIGHT)
			INIDATA->addData("��ö�", "������", "CHRDIREC_RIGHT");
		else if (_player_direction == CHRDIREC_UP)
			INIDATA->addData("��ö�", "������", "CHRDIREC_UP");
		else if (_player_direction == CHRDIREC_DOWN)
			INIDATA->addData("��ö�", "������", "CHRDIREC_DOWN");

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
		break;
	case FINALSTAGE:
		INIDATA->addData("��������", "�������� �̸�", "������ ��");
		INIDATA->addData("��������","�������� ��ȣ(OPENING:0,FINAL:5)",save_s_enum.c_str());
		INIDATA->iniSave(fileName);
		break;
	}
	return false;
}

bool StorageManager::loadData()
{
	//���� �ε�
	//Ÿ��Ʋ�� ������� �������� �����ȿ� ���ϸ�� ��ġ�ϴ� ������ �ҷ���
	//�ε� �Ϸ��Ѵٸ� true�� ��ȯ
	_toggle = false;
	_saveStage = (STAGE)INIDATA->loadDataInterger(fileName, "��������", "�������� ��ȣ(OPENING:0,FINAL:5)");
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
		if (_selectedStorage == FIRST)
			_load_file_name = "Save1";
		else if (_selectedStorage == SECOND)
			_load_file_name = "Save2";
		else if (_selectedStorage == THIRD)
			_load_file_name = "Save3";
		else if (_selectedStorage == FOURTH)
			_load_file_name = "Save4";
		else if (_selectedStorage == FIFTH)
			_load_file_name = "Save5";
		_load_string_stage = INIDATA->loadDataString(_load_file_name.c_str(), "��������5", "5�����̸�");

		//_load_x_i = INIDATA->loadDataInterger(_load_file_name.c_str(), "��ö�", "x��ǥ");
		//_load_y_i = INIDATA->loadDataInterger(_load_file_name.c_str(), "��ö�", "y��ǥ");
		//_load_string_position = INIDATA->loadDataString(_load_file_name.c_str(), "��ö�", "������");
		_player_x = INIDATA->loadDataInterger(_load_file_name.c_str(), "��ö�", "x��ǥ");
		_player_y = INIDATA->loadDataInterger(_load_file_name.c_str(), "��ö�", "y��ǥ");
		_load_string_position = INIDATA->loadDataString(_load_file_name.c_str(), "��ö�", "������");

		STAGEMEMORYMANAGER->setIsSkul1(INIDATA->loadDataInterger(_load_file_name.c_str(), "������Ʈ", "�ذ�1"));
		STAGEMEMORYMANAGER->setIsSkul2(INIDATA->loadDataInterger(_load_file_name.c_str(), "������Ʈ", "�ذ�2"));
		STAGEMEMORYMANAGER->setIsSkul3(INIDATA->loadDataInterger(_load_file_name.c_str(), "������Ʈ", "�ذ�3"));
		STAGEMEMORYMANAGER->setIsSkul4(INIDATA->loadDataInterger(_load_file_name.c_str(), "������Ʈ", "�ذ�4"));
		STAGEMEMORYMANAGER->setIsSkul5(INIDATA->loadDataInterger(_load_file_name.c_str(), "������Ʈ", "�ذ�5"));
		STAGEMEMORYMANAGER->setIsRedFlower1(INIDATA->loadDataInterger(_load_file_name.c_str(), "������Ʈ", "������1"));
		STAGEMEMORYMANAGER->setIsRedFlower2(INIDATA->loadDataInterger(_load_file_name.c_str(), "������Ʈ", "������2"));
		STAGEMEMORYMANAGER->setIsRedFlower3(INIDATA->loadDataInterger(_load_file_name.c_str(), "������Ʈ", "������3"));
		STAGEMEMORYMANAGER->setIsGetSkul1(INIDATA->loadDataInterger(_load_file_name.c_str(), "������Ʈ", "�ذ��1"));
		STAGEMEMORYMANAGER->setIsGetSkul2(INIDATA->loadDataInterger(_load_file_name.c_str(), "������Ʈ", "�ذ��2"));
		STAGEMEMORYMANAGER->setIsGetSkul3(INIDATA->loadDataInterger(_load_file_name.c_str(), "������Ʈ", "�ذ��3"));
		STAGEMEMORYMANAGER->setIsGetSkul4(INIDATA->loadDataInterger(_load_file_name.c_str(), "������Ʈ", "�ذ��4"));
		STAGEMEMORYMANAGER->setIsFlowerDead(INIDATA->loadDataInterger(_load_file_name.c_str(), "������Ʈ", "�Ͼ������"));
		STAGEMEMORYMANAGER->setIsPotion(INIDATA->loadDataInterger(_load_file_name.c_str(), "������Ʈ", "����"));
		STAGEMEMORYMANAGER->setIsKey(INIDATA->loadDataInterger(_load_file_name.c_str(), "������Ʈ", "����"));
		STAGEMEMORYMANAGER->setIsLever(INIDATA->loadDataInterger(_load_file_name.c_str(), "������Ʈ", "����"));
		STAGEMEMORYMANAGER->setIsFlowerpot(INIDATA->loadDataInterger(_load_file_name.c_str(), "������Ʈ", "�ɺ�"));

		/*
		if (_load_string_position == "CHRDIREC_LEFT")
			_player->setDirec(CHRDIREC_LEFT);
		else if (_load_string_position == "CHRDIREC_RIGHT")
			_player->setDirec(CHRDIREC_RIGHT);
		else if (_load_string_position == "CHRDIREC_UP")
			_player->setDirec(CHRDIREC_UP);
		else if (_load_string_position == "CHRDIREC_DOWN")
			_player->setDirec(CHRDIREC_DOWN);
		*/
		SCENEMANAGER->changeScene(_load_string_stage/*, _player->getPdirec(), LOCATION_DEFAULT*/);

		//_player->setPLocaX(_load_x_i);
		//_player->setPLocaY(_load_y_i);
		break;
	case FINALSTAGE:
		_stage = INIDATA->loadDataString(fileName, "��������", "�������� ��");
		_playerDirection = (CHRDIRECTION)INIDATA->loadDataInterger(fileName, "��ö�", "����");
		_playerX = INIDATA->loadDataInterger(fileName, "��ö�", "x��ǥ");
		_playerY = INIDATA->loadDataInterger(fileName, "��ö�", "y��ǥ");
		SCENEMANAGER->changeScene(_stage, _playerX, _playerY, _playerDirection);
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

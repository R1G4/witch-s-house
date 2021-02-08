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
	//파일들의 제목? 불러오기
	loadTitle();

	return S_OK;
}

void StorageManager::loadTitle()
{
	//해당 폴더 안에 존재하는(최대5개)
	//파일들의 제목만 가지고옴
	_vFileSlot[0] = INIDATA->loadDataString("Save1", "스테이지", "스테이지 이름");
	_vFileSlot[1] = INIDATA->loadDataString("Save2", "스테이지", "스테이지 이름");
	_vFileSlot[2] = INIDATA->loadDataString("Save3", "스테이지", "스테이지 이름");
	_vFileSlot[3] = INIDATA->loadDataString("Save4", "스테이지", "스테이지 이름");
	_vFileSlot[4] = INIDATA->loadDataString("Save5", "스테이지", "스테이지 이름");
}

MENUSTATE StorageManager::saveView()
{
	//x키를 누른다면 뒤로가기를 한다.
	//x키 중복 이슈가 발생할 수 있으니 아스키 코드를 한번더 곱해준다.
	if (_isOpen && KEYMANAGER->isOnceKeyDown('X') * (int) 'X')
	{
		//저장소 열린상태를 false로 두며 무시한다.
		_isOpen = false;
		return MENU_BACK;
	}

	//메뉴가 열리지 않았고 선택키를 눌렀을 경우 메뉴를 true로 하며 보여준다.
	if (!_isOpen && KEYMANAGER->isOnceKeyDown(VK_SPACE))
	{
		_divisionText = L"어떤 파일을 저장하시겠습니까?";
		_isOpen = true;
	}

	//데이터 셋팅.
	_vFileSlot[0] = INIDATA->loadDataString("Save1", "스테이지", "스테이지 이름");
	_vFileSlot[1] = INIDATA->loadDataString("Save2", "스테이지", "스테이지 이름");
	_vFileSlot[2] = INIDATA->loadDataString("Save3", "스테이지", "스테이지 이름");
	_vFileSlot[3] = INIDATA->loadDataString("Save4", "스테이지", "스테이지 이름");
	_vFileSlot[4] = INIDATA->loadDataString("Save5", "스테이지", "스테이지 이름");

	//저장소 오픈 (선택하는 컨텐츠를 컨트롤 한다.)
	storageOpen();

	//클릭이 된 상태이고
	if (_isClick)
	{
		//불러오기를 성공 했으며
		if (loadData())
		{
			//저장이 완료 된 경우.
			if (saveData())
			{
				//저장소 창을 종료한다.
				_isOpen = false;

				//선택 상태를 초기화 한다.
				_isClick = false;
				
				//뒤로가기를 반환한다. ( SAVE는 메뉴창을 열지 않기에 바로 뒤로가기를 한다.)
				return MENU_BACK;
			}
		}
	}

	//그외 상태에 따른 열린상태를 반환한다.
	return _isOpen == true ? MENU_PROGRESS : MENU_BACK;
}

MENUSTATE StorageManager::loadView()
{
	//뒤로가기 조작 키
	//키가 안먹혀서 한번더 체크하는 용도? 'X' int 값을 곱해줌
	if (_isOpen && KEYMANAGER->isOnceKeyDown('X') * (int) 'X')
	{
		_isOpen = false;
		return MENU_BACK;
	}

	if (!_isOpen && KEYMANAGER->isOnceKeyDown(VK_SPACE))
	{
		_divisionText = L"어떤 파일을 불러오시겠습니까?";
		_isOpen = true;
	}

	//저장소 오픈
	storageOpen();

	if (_isClick)
	{
		//불러오기를 성공했다면 창을 종료한다.
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
	//파일 저장
	//저장이 성공적으로 되었는지 체크한다 
	//저장 성공 할 경우 true를 반환하며 옵션 창과 선택상태를 초기화 한다.
	save_enum = (int)_saveStage;
	save_s_enum = to_string(save_enum);
	switch (_saveStage)
	{
	case OPENING:
		INIDATA->addData("스테이지", "스테이지 이름", "시작의 정원");
		INIDATA->addData("스테이지", "스테이지 씬", "정원");
		INIDATA->addData("스테이지", "스테이지 번호(OPENING:0,FINAL:5)", save_s_enum.c_str());
		INIDATA->iniSave(fileName);
		return true;
	case FIRSTSTAGE:
		INIDATA->addData("스테이지", "스테이지 이름", "마녀의 집 1층");
		INIDATA->addData("스테이지", "스테이지 씬", "entrance");
		INIDATA->addData("스테이지", "스테이지 번호(OPENING:0,FINAL:5)", save_s_enum.c_str());
		INIDATA->iniSave(fileName);
		return true;
	case SECONDSTAGE:
		INIDATA->addData("스테이지", "스테이지 이름", "마녀의 집 2층");
		INIDATA->addData("스테이지", "스테이지 씬", "thirdMain");
		INIDATA->addData("스테이지", "스테이지 번호(OPENING:0,FINAL:5)", save_s_enum.c_str());
		INIDATA->iniSave(fileName);
		return true;
	case THIRDSTAGE:
		INIDATA->addData("스테이지", "스테이지 이름", "마녀의 집 3층");
		INIDATA->addData("스테이지", "스테이지 씬", "4층 홀");
		INIDATA->addData("스테이지", "스테이지 번호(OPENING:0,FINAL:5)", save_s_enum.c_str());
		INIDATA->iniSave(fileName);
		return true;
	case FOURTHSTAGE:
		INIDATA->addData("스테이지", "스테이지 이름", "마녀의 집 4층");
		INIDATA->addData("스테이지", "스테이지 번호(OPENING:0,FINAL:5)", save_s_enum.c_str());

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
			INIDATA->addData("스테이지", "스테이지 씬", "garden_5f");
		else if (_real_location == GARDEN_TO_BOSS)
			INIDATA->addData("스테이지", "스테이지 씬", "gardenToBoss_5f");
		else if (_real_location == DININGROOM)
			INIDATA->addData("스테이지", "스테이지 씬", "diningRoom_5f");
		else if (_real_location == PRISON)
			INIDATA->addData("스테이지", "스테이지 씬", "prison_5f");
		else if (_real_location == PRISON_WELL)
			INIDATA->addData("스테이지", "스테이지 씬", "prison_well_5f");
		else if (_real_location == PRISON_SKUL)
			INIDATA->addData("스테이지", "스테이지 씬", "prison_skul_5f");

		INIDATA->addData("오브젝트", "해골1", _save_obj[0].c_str());
		INIDATA->addData("오브젝트", "해골2", _save_obj[1].c_str());
		INIDATA->addData("오브젝트", "해골3", _save_obj[2].c_str());
		INIDATA->addData("오브젝트", "해골4", _save_obj[3].c_str());
		INIDATA->addData("오브젝트", "해골5", _save_obj[4].c_str());
		INIDATA->addData("오브젝트", "빨간꽃1", _save_obj[5].c_str());
		INIDATA->addData("오브젝트", "빨간꽃2", _save_obj[6].c_str());
		INIDATA->addData("오브젝트", "빨간꽃3", _save_obj[7].c_str());
		INIDATA->addData("오브젝트", "해골득1", _save_obj[8].c_str());
		INIDATA->addData("오브젝트", "해골득2", _save_obj[9].c_str());
		INIDATA->addData("오브젝트", "해골득3", _save_obj[10].c_str());
		INIDATA->addData("오브젝트", "해골득4", _save_obj[11].c_str());
		INIDATA->addData("오브젝트", "하얀꽃죽음", _save_obj[12].c_str());
		INIDATA->addData("오브젝트", "물약", _save_obj[13].c_str());
		INIDATA->addData("오브젝트", "열쇠", _save_obj[14].c_str());
		INIDATA->addData("오브젝트", "레버", _save_obj[15].c_str());
		INIDATA->addData("오브젝트", "꽃병", _save_obj[16].c_str());
		INIDATA->iniSave(fileName);
		return true;
		break;
	case FINALSTAGE:
		INIDATA->addData("스테이지", "스테이지 이름", "엘렌의 방");
		INIDATA->addData("스테이지", "스테이지 씬", "BossStage1");
		INIDATA->addData("스테이지", "스테이지 번호(OPENING:0,FINAL:5)", save_s_enum.c_str());
		INIDATA->iniSave(fileName); 
		return true;
	}
	return false;
}

bool StorageManager::loadData()
{
	//파일 로드
	//로드 성공적으로 되었는지 체크한다 
	//로드 성공 할 경우 true를 반환하며 옵션 창과 선택상태를 초기화 한다.
	_saveStage = (STAGE)INIDATA->loadDataInterger(fileName, "스테이지", "스테이지 번호(OPENING:0,FINAL:5)");
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
		STAGEMEMORYMANAGER->setIsSkul1(INIDATA->loadDataInterger(fileName, "오브젝트", "해골1"));
		STAGEMEMORYMANAGER->setIsSkul2(INIDATA->loadDataInterger(fileName, "오브젝트", "해골2"));
		STAGEMEMORYMANAGER->setIsSkul3(INIDATA->loadDataInterger(fileName, "오브젝트", "해골3"));
		STAGEMEMORYMANAGER->setIsSkul4(INIDATA->loadDataInterger(fileName, "오브젝트", "해골4"));
		STAGEMEMORYMANAGER->setIsSkul5(INIDATA->loadDataInterger(fileName, "오브젝트", "해골5"));
		STAGEMEMORYMANAGER->setIsRedFlower1(INIDATA->loadDataInterger(fileName, "오브젝트", "빨간꽃1"));
		STAGEMEMORYMANAGER->setIsRedFlower2(INIDATA->loadDataInterger(fileName, "오브젝트", "빨간꽃2"));
		STAGEMEMORYMANAGER->setIsRedFlower3(INIDATA->loadDataInterger(fileName, "오브젝트", "빨간꽃3"));
		STAGEMEMORYMANAGER->setIsGetSkul1(INIDATA->loadDataInterger(fileName, "오브젝트", "해골득1"));
		STAGEMEMORYMANAGER->setIsGetSkul2(INIDATA->loadDataInterger(fileName, "오브젝트", "해골득2"));
		STAGEMEMORYMANAGER->setIsGetSkul3(INIDATA->loadDataInterger(fileName, "오브젝트", "해골득3"));
		STAGEMEMORYMANAGER->setIsGetSkul4(INIDATA->loadDataInterger(fileName, "오브젝트", "해골득4"));
		STAGEMEMORYMANAGER->setIsFlowerDead(INIDATA->loadDataInterger(fileName, "오브젝트", "하얀꽃죽음"));
		STAGEMEMORYMANAGER->setIsPotion(INIDATA->loadDataInterger(fileName, "오브젝트", "물약"));
		STAGEMEMORYMANAGER->setIsKey(INIDATA->loadDataInterger(fileName, "오브젝트", "열쇠"));
		STAGEMEMORYMANAGER->setIsLever(INIDATA->loadDataInterger(fileName, "오브젝트", "레버"));
		STAGEMEMORYMANAGER->setIsFlowerpot(INIDATA->loadDataInterger(fileName, "오브젝트", "꽃병"));

		_stage = INIDATA->loadDataString(fileName, "스테이지", "스테이지 씬");
		_playerDirection = (CHRDIRECTION)INIDATA->loadDataInterger(fileName, "비올라", "방향");
		_playerX = INIDATA->loadDataInterger(fileName, "비올라", "x좌표");
		_playerY = INIDATA->loadDataInterger(fileName, "비올라", "y좌표");
		SCENEMANAGER->changeScene(_stage, _playerX, _playerY, _playerDirection);
		return true;
	case FINALSTAGE:
		_stage = INIDATA->loadDataString(fileName, "스테이지", "스테이지 씬");
		_playerDirection = (CHRDIRECTION)INIDATA->loadDataInterger(fileName, "비올라", "방향");
		_playerX = INIDATA->loadDataInterger(fileName, "비올라", "x좌표");
		_playerY = INIDATA->loadDataInterger(fileName, "비올라", "y좌표");
		SCENEMANAGER->changeScene(_stage, _playerX, _playerY, _playerDirection);
		return true;
	}
	return false;
}

void StorageManager::storageOpen()
{
	//저장소 창이 열리지 않았을 경우 랜더를 하지 않는다.
	if (!_isOpen) return;

	//방향키 조작
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
	//저장소 창이 열리지 않았다면 랜더를 하지 않는다.
	if (!_isOpen)return;

	ImageManager::GetInstance()->FindImage("messe")->SetSize(Vector2(1240, 120));
	ImageManager::GetInstance()->FindImage("messe")->Render(Vector2(WINSIZEX / 2, 75));

	ImageManager::GetInstance()->FindImage("messe")->SetSize(Vector2(1240, 600));
	ImageManager::GetInstance()->FindImage("messe")->Render(Vector2(WINSIZEX / 2, WINSIZEY / 2 + 75));

	//백그라운드 컬러 렉트
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

	//테두리 컬러 렉트
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

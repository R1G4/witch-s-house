#include "stdafx.h"
#include "continueScene.h"


continueScene::continueScene()
{
}


continueScene::~continueScene()
{
}

HRESULT continueScene::init()
{
	_selectedLoad = FIRST;

	//�� �Լ����� ���� ���� ���
	//���� ��� ���� �� ��� �߰� ����
	loadInfo();
	return S_OK;
}

void continueScene::release()
{
}

void continueScene::loadInfo()
{
}

void continueScene::update()
{
	//����Ű ����
	if (!_isClick && KEYMANAGER->isOnceKeyDown(VK_DOWN))
		_selectedLoad = (SAVE)(_selectedLoad == FIFTH ? FIRST : (int)_selectedLoad + 1);

	if (!_isClick && KEYMANAGER->isOnceKeyDown(VK_UP))
		_selectedLoad = (SAVE)(_selectedLoad == FIRST ? FIFTH : (int)_selectedLoad - 1);

	//�ڷΰ��� ���� Ű
	//���� ���� ����Ű���� �ٲ�� ����
	if (KEYMANAGER->isOnceKeyDown('X'))
		SCENEMANAGER->changeScene("�޴�");

	switch (_selectedLoad)
	{
	case continueScene::FIRST:
		_rcSelected = RectMakePivot(Vector2(WINSIZEX / 2 - 10, 230), Vector2(1100, 100), Pivot::Center);
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
			_isClick = true;
		break;
	case continueScene::SECOND:
		_rcSelected = RectMakePivot(Vector2(WINSIZEX / 2, 330), Vector2(1100, 100), Pivot::Center);
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
			_isClick = true;
		break;
	case continueScene::THIRD:
		_rcSelected = RectMakePivot(Vector2(WINSIZEX / 2, 430), Vector2(1100, 100), Pivot::Center);
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
			_isClick = true;
		break;
	case continueScene::FOURTH:
		_rcSelected = RectMakePivot(Vector2(WINSIZEX / 2, 530), Vector2(1100, 100), Pivot::Center);
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
			_isClick = true;
		break;
	case continueScene::FIFTH:
		_rcSelected = RectMakePivot(Vector2(WINSIZEX / 2, 630), Vector2(1100, 100), Pivot::Center);
		break;
	}

	if (_isClick)
	{
		//���⼭ �ҷ�����
	}

	rcAlphaChange();
}

void continueScene::rcAlphaChange()
{
	if (_rcAlpha >= 0.9f)
		_rcAlphaChange = -0.03f;

	if (_rcAlpha <= 0.35f)
		_rcAlphaChange = 0.03f;

	_rcAlpha += _rcAlphaChange;
}

void continueScene::render()
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

#include "stdafx.h"
#include "menu.h"

menu::menu()
{
}

menu::~menu()
{
}

HRESULT menu::init()
{
	//�ش� �������� ó������
	_contents = NEW;
	_isClick = false;

	//���� ��Ʈ �ʱ�ȭ
	_rcSelected = RectMakePivot(Vector2(WINSIZEX / 2 - 10, WINSIZEY / 2 + 65), Vector2(220, 60), Pivot::Center);

	//���� �ʱ�ȭ
	_sceneAlpha = 1.f;
	_rcAlpha = 1.f;
	_rcAlphaChange = 0.03f;

	//�� �޴��� ������ ���� �ʱ�ȭ
	setText();

	return S_OK;
}

void menu::setText()
{	
	//�޴� �ؽ�Ʈ ���� �ʱ�ȭ
	tagText textInfo;
	textInfo.x = WINSIZEX / 2 - 10;
	textInfo.y = WINSIZEY / 2 + 35;
	textInfo.size = 40;
	textInfo.text = L"ó������";
	_mText.insert(pair<CONTENTS, tagText>(NEW, textInfo));

	textInfo.text = L"����ؼ�";
	_mText.insert(pair<CONTENTS, tagText>(CONTINUE, textInfo));

	textInfo.text = L"����";
	_mText.insert(pair<CONTENTS, tagText>(OPTION, textInfo));

	textInfo.text = L"������";
	_mText.insert(pair<CONTENTS, tagText>(END, textInfo));
}

void menu::release()
{
}

void menu::update()
{
	//����Ű ����
	if (!_isClick && KEYMANAGER->isOnceKeyDown(VK_DOWN))
		_contents = (CONTENTS)(_contents == END ? 0 : (int)_contents + 1);

	if (!_isClick && KEYMANAGER->isOnceKeyDown(VK_UP))
		_contents = (CONTENTS)(_contents == NEW ? 3 : (int)_contents - 1);

	//���� Ű�� ������ �ʾҴٸ� ��Ʈ ��¦ ȿ��
	if (!_isClick)	rcAlphaChange();

	//���õ� �޴�(�׸�)�� ��Ʈ ��ġ ����
	switch (_contents)
	{
		case menu::NEW:
			_rcSelected = RectMakePivot(Vector2(WINSIZEX / 2 - 10, WINSIZEY / 2 + 55), Vector2(220, 60), Pivot::Center);
			if (_isClick)
			{
				if (sceneAlphaChange())
					SCENEMANAGER->changeScene("��������");
			}
			if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
				_isClick = true;
			break;

		case menu::CONTINUE:
			_rcSelected = RectMakePivot(Vector2(WINSIZEX / 2 - 10, WINSIZEY / 2 + 125), Vector2(220, 60), Pivot::Center);
			if (_isClick)
				SCENEMANAGER->changeScene("����ϱ�");

			if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
				_isClick = true;
			break;

		case menu::OPTION:
			_rcSelected = RectMakePivot(Vector2(WINSIZEX / 2 - 10, WINSIZEY / 2 + 195), Vector2(220, 60), Pivot::Center);
			break;

		case menu::END:
			_rcSelected = RectMakePivot(Vector2(WINSIZEX / 2 - 10, WINSIZEY / 2 + 265), Vector2(220, 60), Pivot::Center);
			if (_isClick)
			{
				if (sceneAlphaChange())
					PostQuitMessage(0);
			}
			if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
				_isClick = true;
			break;
	}
}

void menu::rcAlphaChange()
{
	if (_rcAlpha >= 0.9f)
		_rcAlphaChange = -0.03f;

	if (_rcAlpha <= 0.35f)
		_rcAlphaChange = 0.03f;

	_rcAlpha += _rcAlphaChange;
}

bool menu::sceneAlphaChange()
{
	_sceneAlpha -= 0.01f;
	_rcAlpha -= 0.05f;

	if (_sceneAlpha <= 0.f)
		return true;
	else
		return false;
}

void menu::render()
{
	//��׶��� ���� �� ����~
	ImageManager::GetInstance()->FindImage("menu")->SetSize(Vector2(1280, 720));
	ImageManager::GetInstance()->FindImage("menu")->SetAlpha(_sceneAlpha);
	ImageManager::GetInstance()->FindImage("menu")->Render(Vector2(WINSIZEX / 2, WINSIZEY / 2));

	//��׶��� �÷� ��Ʈ
	D2DINS->FillRectangle
	(
		_rcSelected,
		D2D1::ColorF::Enum::WhiteSmoke,
		_rcAlpha / 3.5
	);

	for (int i = 0; i < _mText.size(); i++)
	{
		CONTENTS sel = (CONTENTS)i;
		D2DINS->RenderText
		(
			_mText.find(sel)->second.x - _mText.find(sel)->second.size * 2,
			_mText.find(sel)->second.y + i * 70,
			_mText.find(sel)->second.text,
			RGB(255, 255, 255),
			_sceneAlpha,
			_mText.find(sel)->second.size,
			DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_TRAILING
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
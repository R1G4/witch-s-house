#include "stdafx.h"
#include "levelChoiceScene.h"

levelChoiceScene::levelChoiceScene()
{
}

levelChoiceScene::~levelChoiceScene()
{
}

HRESULT levelChoiceScene::init()
{
	_x = WINSIZEX / 2;
	_y = WINSIZEY / 2;

	_rc = RectMakePivot(Vector2(_x, _y - 120), Vector2(120, 60), Pivot::Center);
	_rc1 = RectMakePivot(Vector2(_x, _y - 120), Vector2(120, 60), Pivot::Center);
	_rc2 = RectMakePivot(Vector2(_x, _y + 60), Vector2(120, 60), Pivot::Center);
	_rc3 = RectMakePivot(Vector2(_x, _y + 240), Vector2(120, 60), Pivot::Center);
	_isChoice = false;
	_sceneAlpha = 1.0f;
	_rcAlpha = 1.0f;
	_rcAlphaChange = 0.03f;

	return S_OK;
}

void levelChoiceScene::release()
{

}

void levelChoiceScene::rcAlphaChange()
{
	if (_rcAlpha >= 0.9f)
		_rcAlphaChange = -0.03f;

	if (_rcAlpha <= 0.35f)
		_rcAlphaChange = 0.03f;

	_rcAlpha += _rcAlphaChange;
}


void levelChoiceScene::update()
{
	rcAlphaChange();

	FloatRect _temp;


	if (!_isChoice)
	{
		_rc = RectMakePivot(Vector2(_x, _y - 120), Vector2(120, 60), Pivot::Center);
		if (KEYMANAGER->isOnceKeyDown(VK_DOWN) && _rc.bottom <= WINSIZEY / 2 + 210)
			_y += 180;
		if (KEYMANAGER->isOnceKeyDown(VK_UP) && _rc.top >= WINSIZEY / 2 - 120)
			_y -= 180;

		if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
		{
			_isChoice = true;
			_x = WINSIZEX / 2;
			_y = WINSIZEY / 2;
		}



	}

	if (_isChoice)
	{
		_rc = RectMakePivot(Vector2(_x, _y + 50), Vector2(220, 80), Pivot::Center);
		if (KEYMANAGER->isOnceKeyDown(VK_DOWN) && _rc.bottom <= WINSIZEY / 2)
			_y += 180;
		if (KEYMANAGER->isOnceKeyDown(VK_UP) && _rc.top >= WINSIZEY / 2 - 120)
			_y -= 180;

		if (IntersectRectToRect(&_temp, &_rc, &_rc2))
		{
			if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
				_isChoice = false;
		}
	}

	//선택 키를 눌렀다면 백그라운드 및 이미지 투명도 조절
	if (_isChoice && IntersectRectToRect(&_temp, &_rc, &_rc1))
	{
		//게임화면으로 가기 전에 애니메이션이든 뭐 넣어야 할듯
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
			SCENEMANAGER->changeScene("MapToolScene");
	}

}

//배경화면 이미지로 넣으면 setalpha로 가져오기
//bool levelChoiceScene::sceneAlphaChange()
//{
//	_sceneAlpha -= 0.01f;
//	_rcAlpha -= 0.05f;
//
//	if (_sceneAlpha <= 0.f)
//		return true;
//	else
//		return false;
//}

void levelChoiceScene::render()
{
	//백그라운드 컬러 렉트

	D2DINS->FillRectangle
	(
		_rc,
		D2D1::ColorF::Enum::WhiteSmoke,
		_rcAlpha / 5.5
	);

	if (!_isChoice)
	{
		D2DINS->GetInstance()->RenderTextField(WINSIZEX / 2 - 30, WINSIZEY / 2 - 155, L"Easy", 30, 120, 60, D2DRenderer::DefaultBrush::Blue);
		D2DINS->GetInstance()->RenderTextField(WINSIZEX / 2 - 45, WINSIZEY / 2 + 25, L"Nomal", 30, 120, 60, D2DRenderer::DefaultBrush::Green);
		D2DINS->GetInstance()->RenderTextField(WINSIZEX / 2 - 20, WINSIZEY / 2 + 210, L"???", 30, 120, 60, D2DRenderer::DefaultBrush::Red);
	}
	if (_isChoice)
	{
		D2DINS->GetInstance()->RenderTextField(WINSIZEX / 2 - 30, WINSIZEY / 2 - 155, L"결정", 30, 120, 60, D2DRenderer::DefaultBrush::White);
		D2DINS->GetInstance()->RenderText(WINSIZEX / 2 - 90, WINSIZEY / 2 + 25, L"역시 그만둔다", 30, D2DRenderer::DefaultBrush::White);
	}

	D2DINS->GetInstance()->DrawRectangle
	(
		_rc,
		D2D1::ColorF::White,
		_rcAlpha,
		1.0f
	);
}

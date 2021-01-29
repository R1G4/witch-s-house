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
	_choiceRect = ImageManager::GetInstance()->AddImage("choiceRect", L"image/choiceRect.png");

	_rc = RectMakePivot(Vector2(_x, _y - 120), Vector2(120, 60), Pivot::Center);
	_rc2 = RectMakePivot(Vector2(_x, _y + 60), Vector2(120, 60), Pivot::Center);
	_rc3 = RectMakePivot(Vector2(_x, _y + 120), Vector2(120, 60), Pivot::Center);
	_isChoice = false;

	_alpha = 1.0f;

	return S_OK;
}

void levelChoiceScene::release()
{

}

void levelChoiceScene::update()
{
	FloatRect _temp;
	_alpha -= 0.03f;
	if (_alpha <= 0.01) _alpha = 1.0f;

	_rc = RectMakePivot(Vector2(_x, _y - 120), Vector2(120, 60), Pivot::Center);

	if (!_isChoice)
	{
		if (KEYMANAGER->isOnceKeyDown(VK_DOWN) /*&& !_isChoice  */ && _rc.bottom <= WINSIZEY / 2 + 210)
		{
			_y += 180;
		}
		if (KEYMANAGER->isOnceKeyDown(VK_UP) /*&& !_isChoice */ && _rc.top >= WINSIZEY / 2 - 120)
		{
			_y -= 180;
		}

		if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
		{
			_isChoice = true;

		}
	}


	if (_isChoice)
	{
		_rc = RectMakePivot(Vector2(_x, _y + 50), Vector2(220, 80), Pivot::Center);
		if (KEYMANAGER->isOnceKeyDown(VK_DOWN) && _rc.bottom <= WINSIZEY / 2)
		{
			_y += 180;
		}
		if (KEYMANAGER->isOnceKeyDown(VK_UP) && _rc.top >= WINSIZEY / 2 - 120)
		{
			_y -= 180;
		}

		if (KEYMANAGER->isOnceKeyDown(VK_SPACE) && (_rc.top >= WINSIZEY / 2 - 120))
		{
			_isChoice = false;
		}

	}



}

void levelChoiceScene::render()
{
	//D2DRenderer::FillRectangle(_rc, D2D1::ColorF::White, _alpha);

	_D2DRenderer->DrawRectangle(_rc, D2D1::ColorF::White, _alpha, 1.0f);


	if (!_isChoice)
	{
		_D2DRenderer->RenderTextField(WINSIZEX / 2 - 30, WINSIZEY / 2 - 155, L"Easy", 30, 120, 60, D2DRenderer::DefaultBrush::Blue);
		_D2DRenderer->RenderTextField(WINSIZEX / 2 - 45, WINSIZEY / 2 + 25, L"Nomal", 30, 120, 60, D2DRenderer::DefaultBrush::Green);
		_D2DRenderer->RenderTextField(WINSIZEX / 2 - 20, WINSIZEY / 2 + 210, L"???", 30, 120, 60, D2DRenderer::DefaultBrush::Red);
	}
	if (_isChoice)
	{
		_D2DRenderer->RenderTextField(WINSIZEX / 2 - 30, WINSIZEY / 2 - 155, L"결정", 30, 120, 60, D2DRenderer::DefaultBrush::White);
		_D2DRenderer->RenderText(WINSIZEX / 2 - 90, WINSIZEY / 2 + 25, L"역시 그만둔다", 30, D2DRenderer::DefaultBrush::White);
	}


}

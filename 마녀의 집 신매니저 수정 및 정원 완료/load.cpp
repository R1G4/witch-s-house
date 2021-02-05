#include "stdafx.h"
#include "load.h"

load::load()
{
}

load::~load()
{
}

HRESULT load::init()
{
	_load = IMAGEMANAGER->AddImage("load", L"image/UI/load.png");
	_loadTop = IMAGEMANAGER->AddImage("loadTop", L"image/UI/loadTop.png");

	_x = 650;
	_y = 230;
	_rcAlpha = 1.0f;
	_rcAlphaChange = 0.03f;
	_rcLoadChoice = RectMakePivot(Vector2(_x, _y), Vector2(600, 80), Pivot::Center);

	for (int i = 0; i < 5; i++)
	{
		_rcLoad[i] = RectMakePivot(Vector2(650, 230 + i * 90), Vector2(600, 80), Pivot::Center);
	}

	return S_OK;
}

void load::release()
{
}

void load::update()
{
	rcAlphaChange();
	_rcLoadChoice = RectMakePivot(Vector2(_x, _y), Vector2(600, 80), Pivot::Center);

	if (KEYMANAGER->isOnceKeyDown('S') && _rcLoadChoice.bottom <= _rcLoad[4].top)
	{
		SOUNDMANAGER->play("cursor", 0.5f);
		_y += 90;
	}
	if (KEYMANAGER->isOnceKeyDown('W') && _rcLoadChoice.top >= _rcLoad[0].bottom)
	{
		SOUNDMANAGER->play("cursor", 0.5f);
		_y -= 90;
	}



}

void load::render()
{
	_load = IMAGEMANAGER->FindImage("load");
	_load->Render(Vector2(650, 400));
	_loadTop = IMAGEMANAGER->FindImage("loadTop");
	_loadTop->Render(Vector2(650, 120));

	D2DINS->GetInstance()->RenderText(300, 95, L"어떤 파일에 저장하시겠습니까?", 30, D2DRenderer::DefaultBrush::White);

	D2DINS->GetInstance()->DrawRectangle(_rcLoadChoice, D2D1::ColorF::White, _rcAlpha, 1.0f);
	D2DINS->FillRectangle(_rcLoadChoice, D2D1::ColorF::Enum::WhiteSmoke, _rcAlpha / 5.5);
	for (int i = 0; i < 5; i++)
	{
		//D2DINS->GetInstance()->DrawRectangle(_rcLoad[i], D2D1::ColorF::White, _rcAlpha, 1.0f);
		D2DINS->GetInstance()->RenderText(380, 190 + i * 90, L"파일", 30, D2DRenderer::DefaultBrush::White);
	}
}

void load::rcAlphaChange()
{
	if (_rcAlpha >= 0.9f)
		_rcAlphaChange = -0.03f;

	if (_rcAlpha <= 0.35f)
		_rcAlphaChange = 0.03f;

	_rcAlpha += _rcAlphaChange;
}

void load::loadSaveFile()
{

}
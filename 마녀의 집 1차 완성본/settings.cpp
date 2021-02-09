#include "stdafx.h"
#include "settings.h"

settings::settings()
{
}

settings::~settings()
{
}

HRESULT settings::init()
{

	//_sound = new sound;
	//_sound->init();
	_x = 640;
	_y = WINSIZEY / 2 + 20;
	_rcAlpha = 1.0f;
	_rcAlphaChange = 0.03f;
	_volume = 0.4f;
	_bgv = 0.5f;
	_percent = 50;
	_bgvPercent = 50;

	for (int i = 0; i < 7; i++)
	{
		_rcSettings[i] = RectMakePivot(Vector2(630, WINSIZEY / 2 + 20 + i * 50), Vector2(400, 30), Pivot::Center);
	}

	_rcSettingChoice = RectMakePivot(Vector2(_x, _y), Vector2(400, 50), Pivot::Center);
	return S_OK;
}

void settings::release()
{
}

void settings::update()
{
	//SOUNDMANAGER->setVolume(_volume);
	//rcAlphaChange();

	//if (KEYMANAGER->isOnceKeyDown('S') && _rcSettingChoice.bottom <= _rcSettings[6].top)
	//{
	//	SOUNDMANAGER->play("cursor", 0.5f);
	//	_y += 50;
	//}
	//if (KEYMANAGER->isOnceKeyDown('W') && _rcSettingChoice.top >= _rcSettings[0].bottom)
	//{
	//	SOUNDMANAGER->play("cursor", 0.5f);
	//	_y -= 50;
	//}

	////볼륨 문자열 조절하려고
	//mv = "Master Volume              " + to_string(_percent) + "%";
	//mv_w = L"";
	//mv_w.assign(mv.begin(), mv.end());

	//FloatRect _temp;
	////마스터 볼륨 조절
	//if (IntersectRectToRect(&_temp, &_rcSettingChoice, &_rcSettings[2]))
	//{
	//	if (KEYMANAGER->isOnceKeyDown('V') && _percent <= 100)
	//	{
	//		_percent += 5;
	//		_volume += 0.025;
	//		
	//	}
	//	if (_percent >= 105)
	//	{
	//		_volume = 0;
	//		_percent = 0;
	//	}
	//}

	////기타 다른볼륨 조절은 할지말지
	//bgv = "backGround Volume     " + to_string(_bgvPercent) + "%";
	//bgv_w = L"";
	//bgv_w.assign(bgv.begin(), bgv.end());
	//if (IntersectRectToRect(&_temp, &_rcSettingChoice, &_rcSettings[3]))
	//{
	//	if (KEYMANAGER->isOnceKeyDown('V') && _bgvPercent <= 100)
	//	{
	//		_bgvPercent += 5;
	//		_bgv += 0.05;
	//		SOUNDMANAGER->setVolume(_bgv);
	//	}
	//	if (_bgvPercent >= 105)
	//	{
	//		_volume = 0;
	//		_bgvPercent = 0;
	//		SOUNDMANAGER->setVolume(_bgv);
	//	}
	//}
}

//해당 컨텐츠 상태 반환 추가
//뒤로가기, 진행, 타이틀화면으로
//플레이어 메뉴 구조 변경으로 함수 추가하였음 (수정할때 알려줘)by pju
MENUSTATE settings::settingOpen()
{
	SOUNDMANAGER->setVolume(_volume);

	_rcSettingChoice = RectMakePivot(Vector2(_x, _y), Vector2(400, 50), Pivot::Center);

	if (KEYMANAGER->isOnceKeyDown('X'))
		return MENU_BACK;

	if (KEYMANAGER->isOnceKeyDown(VK_DOWN) && _rcSettingChoice.bottom <= _rcSettings[6].top)
	{
		SOUNDMANAGER->play("cursor", 0.5f);
		_y += 50;
	}
	if (KEYMANAGER->isOnceKeyDown(VK_UP) && _rcSettingChoice.top >= _rcSettings[0].bottom)
	{
		SOUNDMANAGER->play("cursor", 0.5f);
		_y -= 50;
	}

	//볼륨 문자열 조절하려고
	mv = "Master Volume              " + to_string(_percent) + "%";
	mv_w = L"";
	mv_w.assign(mv.begin(), mv.end());

	FloatRect _temp;
	//마스터 볼륨 조절
	if (IntersectRectToRect(&_temp, &_rcSettingChoice, &_rcSettings[2]))
	{
		if (KEYMANAGER->isOnceKeyDown(VK_RIGHT) * 0x27 && _percent <= 100)
		{
			_percent += 5;
			_volume += 0.05;
		}
		if (_volume >= 1)
		{
			_volume = 0;
		}

		if (_percent >= 105)
		{
			_volume = 0;
			_percent = 0;

		}
	}

	//기타 다른볼륨 조절은 할지말지
	bgv = "backGround Volume     " + to_string(_bgvPercent) + "%";
	bgv_w = L"";
	bgv_w.assign(bgv.begin(), bgv.end());

	if (IntersectRectToRect(&_temp, &_rcSettingChoice, &_rcSettings[3]))
	{
		if (KEYMANAGER->isOnceKeyDown(VK_RIGHT) * 0x27 && _bgvPercent <= 100)
		{
			_bgvPercent += 5;
			_bgv += 0.05;
			SOUNDMANAGER->setVolume(_bgv);
		}
		else if (_bgvPercent >= 105)
		{
			_volume = 0;
			_bgvPercent = 0;
			SOUNDMANAGER->setVolume(_bgv);
		}
	}

	if (IntersectRectToRect(&_temp, &_rcSettingChoice, &_rcSettings[6]))
	{
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE))	return MENU_END;
	}
	rcAlphaChange();

	return MENU_PROGRESS;
}

void settings::render()
{
	D2DINS->GetInstance()->DrawRectangle(_rcSettingChoice, D2D1::ColorF::White, _rcAlpha, 1.0f);
	D2DINS->GetInstance()->RenderText(450, WINSIZEY / 2, L"항시 달리기                  OFF", 30, D2DRenderer::DefaultBrush::White);
	D2DINS->GetInstance()->RenderText(450, WINSIZEY / 2 + 50, L"전체 화면으로 기동      OFF", 30, D2DRenderer::DefaultBrush::White);
	D2DINS->GetInstance()->RenderText(450, WINSIZEY / 2 + 100, mv_w, 30, D2DRenderer::DefaultBrush::White);
	D2DINS->GetInstance()->RenderText(450, WINSIZEY / 2 + 150, bgv_w, 30, D2DRenderer::DefaultBrush::White);
	D2DINS->GetInstance()->RenderText(450, WINSIZEY / 2 + 200, L"Environmental sound     50%", 30, D2DRenderer::DefaultBrush::White);
	D2DINS->GetInstance()->RenderText(450, WINSIZEY / 2 + 250, L"Effect sound                   50%", 30, D2DRenderer::DefaultBrush::White);
	D2DINS->GetInstance()->RenderText(450, WINSIZEY / 2 + 300, L"타이틀 화면으로 돌아가기", 30, D2DRenderer::DefaultBrush::White);
	D2DINS->FillRectangle(_rcSettingChoice, D2D1::ColorF::Enum::WhiteSmoke, _rcAlpha / 5.5);
}

void settings::rcAlphaChange()
{
	if (_rcAlpha >= 0.9f)
		_rcAlphaChange = -0.03f;

	if (_rcAlpha <= 0.35f)
		_rcAlphaChange = 0.03f;

	_rcAlpha += _rcAlphaChange;
}
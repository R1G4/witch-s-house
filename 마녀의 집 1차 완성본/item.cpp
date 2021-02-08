#include "stdafx.h"
#include "item.h"

item::item()
{
}

item::~item()
{
}

HRESULT item::init()
{
	_item = IMAGEMANAGER->AddImage("item", L"image/UI/item2.png");
	_itemTop = IMAGEMANAGER->AddImage("itemTop", L"image/UI/itemTop.png");

	_rcAlpha = 1.0f;
	_rcAlphaChange = 0.03f;

	return S_OK;
}

void item::release()
{
}

void item::update()
{
}

void item::render()
{
	_item = IMAGEMANAGER->FindImage("item");
	_item->Render(Vector2(650, 500));
	_itemTop = IMAGEMANAGER->FindImage("itemTop");
	_itemTop->Render(Vector2(650, 370));
}

void item::rcAlphaChange()
{
	if (_rcAlpha >= 0.9f)
		_rcAlphaChange = -0.03f;

	if (_rcAlpha <= 0.35f)
		_rcAlphaChange = 0.03f;

	_rcAlpha += _rcAlphaChange;
}
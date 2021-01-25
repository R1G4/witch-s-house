#include "stdafx.h"
#include "progressBar.h"

HRESULT progressBar::init(int x, int y, int width, int height)
{
	_x = x;
	_y = y;

	_rcProgress = RectMake(x, y, width, height);

	_progressBarFront = IMAGEMANAGER->addImage("frontBar", "hpBarTop.bmp", width, height, true, RGB(255, 0, 255));
	_progressBarBack = IMAGEMANAGER->addImage("backBar", "hpBarBottom.bmp", width, height, true, RGB(255, 0, 255));

	//게이지의 가로크기는 이미지의 가로크기만큼.
	_width = _progressBarFront->getWidth();

	return S_OK;
}

void progressBar::release()
{

}

void progressBar::update()
{
	_rcProgress = RectMakeCenter(_x, _y, _progressBarFront->getWidth(),
		_progressBarFront->getHeight());
}

void progressBar::render()
{
	IMAGEMANAGER->render("backBar", getMemDC(),
		_rcProgress.left + _progressBarBack->getWidth() / 2,
		_y + _progressBarBack->getHeight() / 2, 0, 0,
		_progressBarBack->getWidth(), _progressBarBack->getHeight());

	IMAGEMANAGER->render("frontBar", getMemDC(),
		_rcProgress.left + _progressBarFront->getWidth() / 2,
		_y + _progressBarFront->getHeight() / 2, 0, 0,
		_width, _progressBarFront->getHeight());

}

void progressBar::setGauge(float currentGauge, float maxGauge)
{
	_width = (currentGauge / maxGauge) * _progressBarBack->getWidth();
}

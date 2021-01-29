#include "stdafx.h"
#include "CameraManager.h"

void CameraManager::setConfig(float offsetL, float offsetT, float width, float height, float minL, float minT, float maxL, float maxT)
{
	startX = offsetL;
	startY = offsetT;
	cameraX = minL;
	cameraY = minT;
	_width = width;
	_height = height;
	_originMinX = minX = minL;
	_originMinY = minY = minT;
	_originMaxX = maxX = maxL;
	_originMaxY = maxY = maxT;

	centerX = cameraX + (_width / 2);
	centerY = cameraY + (_height / 2);
	_originMinCX = minCX = minL + (_width / 2);
	_originMinCY = minCY = minT + (_height / 2);
	_originMaxCX =maxCX = maxL + (_width / 2);
	_originMaxCY =maxCY = maxT + (_height / 2);
}

void CameraManager::setCameraX(float x)
{
	x = min(maxX, x);
	x = max(minX, x);
	cameraX = floor(x);
	centerX =cameraX + (_width / 2);
}

void CameraManager::setCameraY(float y)
{
	y = min(maxY, y);
	y = max(minY, y);
	cameraY = floor(y);
	centerY = cameraY + (_height / 2);
}

void CameraManager::setCamera(Vector2 vec)
{
	setCameraX(vec.x);
	setCameraY(vec.y);
}

void CameraManager::setCenterX(float x)
{
	x = min(maxX, x);
	x = max(minX, x);
	centerX = floor(x);
	cameraX =centerX - (_width / 2);
}

void CameraManager::setCenterY(float y)
{
	y = min(maxY, y);
	y = max(minY,y);
	centerY = floor(y);
	cameraX = centerY - (_height / 2);
}

void CameraManager::setCenter(Vector2 vec)
{
	setCenterX(vec.x);
	setCenterY(vec.y);
	cout << cameraX<<endl;
	cout << centerX<<endl;
}

void CameraManager::movePivot(float x, float y)
{
	setCameraX(cameraX + x);
	setCameraY(cameraY + y);

}

void CameraManager::movePivot(Vector2 xy)
{
	setCameraX(cameraX + xy.x);
	setCameraY(cameraY + xy.y);
}

float CameraManager::getRelativeX(float left)
{
	float newL = startX + left - cameraX;
	return newL;
}

float CameraManager::getRelativeY(float top)
{
	float newL = startY + top - cameraY;
	return newL;
}

Vector2 CameraManager::getRelativeV2(Vector2 vec)
{
	return Vector2(getRelativeX(vec.x), getRelativeY(vec.y));
}

FloatRect CameraManager::getRectangle(FloatRect rc)
{
	float left = getRelativeX(rc.left);
	float top = getRelativeY(rc.top);
	float right = getRelativeX(rc.right);
	float bottom = getRelativeY(rc.bottom);

	return FloatRect(left, top,right, bottom);
}

void CameraManager::setWorldMouse(POINT ptmouse)
{
	worldMouse.x = startX+ptmouse.x + cameraX;
	worldMouse.y = startY + ptmouse.y + cameraY;
}


void CameraManager::renderRc(FloatRect rc, D2D1::ColorF::Enum color, float alpha, float strokeWidth)
{
	FloatRect relativeRc = getRectangle(rc);
	_D2DRenderer->DrawRectangle(relativeRc, color, alpha, strokeWidth);
}

void CameraManager::renderFillRc(FloatRect rc, D2D1::ColorF::Enum color, float alpha)
{
	FloatRect relativeRc = getRectangle(rc);
	_D2DRenderer->FillRectangle(relativeRc, color, alpha);
}

float CameraManager::setRelativeX(float x)
{
	return	cameraX =  cameraX-x;
}

float CameraManager::setRelativeY(float y)
{
	return 	cameraY = cameraY-y;
}

void CameraManager::render(Image * img, Vector2 center)
{
	Vector2 drawPos = getRelativeV2(center);
	img->Render(drawPos);
}

void CameraManager::FrameRender(Image * img, Vector2 center, int frameX, int frameY)
{
	Vector2 drawPos = getRelativeV2(center);
	img->FrameRender(drawPos, frameX, frameY);
}



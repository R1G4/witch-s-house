#pragma once
#include "singletonBase.h"
#include "image.h"
#include "D2DRenderer.h"
class CameraManager :
	public singletonBase<CameraManager>
{
private:
	float startX, startY;//기준이 될 점
	float cameraX, cameraY;//카메라 left,top
	float _width, _height;
	float _originMinCX,_originMinCY,minCX, minCY;
	float _originMaxCX, _originMaxCY, maxCY, maxCX;
	float _originMinX, minX,_originMinY,minY;
	float _originMaxX, maxX, _originMaxY, maxY;
	float centerX, centerY;
	float winWidth, winHeight;
	Vector2 worldMouse;
	float relativeX, relativeY;
	float relativeL, relativeT, relativeR, relativeB;
public:
	//총괄 카메라 설정
	void setConfig(float offsetL, float offsetT, float width, float height, float minL, float minT, float maxL, float maxT);
	void setCameraX(float x);//카메라 좌표 갱신같다
	void setCameraY(float y);//
	void setCamera(Vector2 vec);//여기까지

	void setCenterX(float x);
	void setCenterY(float y);
	void setCenter(Vector2 vec);

	void movePivot(float x, float y);//카메라 이동
	void movePivot(Vector2 xy);

	float getRelativeX(float left);
	float getRelativeY(float top);
	Vector2 getRelativeV2(Vector2 vec);

	FloatRect getRectangle(FloatRect rc);

	void setWorldMouse(POINT ptmouse);
	Vector2 getWorldMouse() { return worldMouse; }

	void renderRc(FloatRect rc, D2D1::ColorF::Enum color, float alpha, float strokeWidth);

	void renderFillRc(FloatRect rc, D2D1::ColorF::Enum color, float alpha);
	float setRelativeX(float x);
	float setRelativeY(float y);
	void render(Image* img,Vector2 center);
	void FrameRender(Image* img, Vector2 center, int frameX, int frameY);
	void txtRender(string str, Vector2 vec, int size, const D2DRenderer::DefaultBrush& defaultbrush=D2DRenderer::DefaultBrush::White
		, const DWRITE_TEXT_ALIGNMENT& align= DWRITE_TEXT_ALIGNMENT_LEADING, const wstring& font=L"맑은 고딕");

	float getCameraX() { return cameraX; }
	float getCameraY() { return cameraY; }
};


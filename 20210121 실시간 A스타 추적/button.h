#pragma once
#include "gameNode.h"

typedef void(*CALLBACK_FUNCTION)(void);

//Lambda expression <-- 조사 해보시구요 당연히 포폴에 사용하면 이쁨받습니다

//<functional> 라이브러리를 사용하면 그안에 move, bind, function등의 
//함수가 있는데 이것을 사용하면 콜백함수 파라미터로 일반 변수를 사용가능하게 됨
//해보려면 해보고 안해도 됨

//저 콜백함수에 람다식 이어서 사용하는것도 가능함
//역시 해보려면 하고 안할려면 안해도 됩니다 여러분 자유

enum BUTTONDIRECTION
{
	BUTTONDIRECTION_NULL,
	BUTTONDIRECTION_UP,
	BUTTONDIRECTION_DOWN
};

class button : public gameNode
{
private:
	BUTTONDIRECTION _direction;

	const char* _imageName;
	image* _image;
	RECT _rc;

	float _x, _y;
	POINT _btnDownFramePoint;
	POINT _btnUpFramePoint;

	CALLBACK_FUNCTION _callbackFunction;		//콜백함수 선언

public:
	button() {};
	~button() {};

	HRESULT init(const char* imageName, float x, float y,
		POINT btnDownFramePoint, POINT btnUpFramePoint,
		CALLBACK_FUNCTION cbFunction);

	void release();
	void update();
	void render();

};


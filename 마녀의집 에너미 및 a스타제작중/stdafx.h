#pragma once
#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <iostream>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <assert.h>

#include <vector>
#include <map>

#include <commdlg.h>

// 이미지 타일 관련 디파인문
#define TILESIZE 48
#define TILEX 40
#define TILEY 40
#define TILESIZEX TILEX*TILESIZE
#define TILESIZEY TILEY*TILESIZE

using namespace std;


enum class Pivot : int
{
	LeftTop,Center,Bottom
};

namespace Direction
{
	enum Enum : int
	{
		Left = 1,
		Top = 2,
		Right = 4,
		Bottom = 8,
		LeftTop = Left | Top,
		LeftBottom = Left | Bottom,
		RightTop = Right | Top,
		RightBottom = Right | Bottom,
		End = 0
	};

}


#include "LibraryHeader.h"//d2d라이브러리 헤더

#include "Vector2.h"
#include "Math.h"
#include "FloatRect.h"//실수형 렉트 만들기
#include "Figure.h"

using namespace Figure;

#include "commonMacroFunction.h"
#include "randomFunction.h"
#include "keyManager.h"
#include "D2DRenderer.h"
#include "imageManager.h"
#include "timeManager.h"
#include "soundManager.h"
#include "sceneManager.h"
#include "txtData.h"
#include "CameraManager.h"

//MS VS버전이 올라가면서 기존의 문자열 함수의 안정성을 올리기위해
//_s를 붙인 함수를 사용하게끔 경고하는 메시지를 무시해준다


// 여기서 프로그램에 필요한 추가 헤더를 참조합니다.
#define WINNAME (LPCTSTR)(TEXT("R1G4 마녀의집"))
#define WINSTARTX	100
#define WINSTARTY	100
#define WINSIZEX	1280		
#define WINSIZEY	720
#define WINSTYLE	WS_CAPTION | WS_SYSMENU

#define RND randomFunction::getSingleton()
#define KEYMANAGER keyManager::getSingleton()
#define IMAGEMANAGER ImageManager::GetInstance()
#define TIMEMANAGER timeManager::getSingleton()
#define EFFECTMANAGER effectManager::getSingleton()
#define SOUNDMANAGER soundManager::getSingleton()
#define SCENEMANAGER sceneManager::getSingleton()
#define TXTDATA txtData::getSingleton()
#define CAMERAMANAGER CameraManager::getSingleton()
#define D2DINS D2DRenderer::GetInstance()
#define OBJECTMANAGER ObjectManager::getSingleton()
#define SAFE_DELETE(p)		{if(p) {delete(p); (p) = NULL;}}
#define SAFE_RELEASE(p)		{if(p) {(p)->release(); (p) = NULL;}}
#define SAFE_DELETE_ARRAY(p) {if(p) {delete[](p); (p) = NULL;}}
#define NEW_SAFE_RELEASE(p) {if(p){p->Release(); (p) = NULL;}}

#define Synthesize(ValueType,ValueName,FuncName) \
protected: ValueType ValueName;\
public: inline ValueType Get##FuncName(void) const{return ValueName;}\
public: inline void Set##FuncName(ValueType value){ValueName = value;}

//====================================
// ## 20.05.29 ## Extern ##
//====================================

extern HINSTANCE	_hInstance;
extern HWND			_hWnd;
extern POINT		_ptMouse;
extern HDC			_hdc;
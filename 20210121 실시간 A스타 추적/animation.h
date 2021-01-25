#pragma once
#include <vector>

//콜백함수
typedef void(*CALLBACK_FUNCTION)(void);

//매개변수가 있는 콜백함수
typedef void(*CALLBACK_FUNCTION_PARAMETER)(void*);

//void* 보이드 포인터 -> 뭐든지 다 담을 수 있다. 다만 명시적_casting을 해줘야한다

class animation
{
public:
	typedef vector<POINT> vFrameList;	//프레임 리스트 담을 벡터
	typedef vector<int> vPlayList;		//플레이 리스트 담을 벡터

private:
	int			_frameNum;		//프레임 넘버
		
	vFrameList	_frameList;		//애니메이션 프레임 리스트
	vPlayList	_playList;		//애니메이션 플레이 인덱스 리스트

	int _frameWidth;
	int _frameHeight;

	BOOL _loop;

	float _frameUpdateSec;
	float _elapsedSec;

	DWORD _nowPlayIndex;
	BOOL _play;

	void*						_obj;
	CALLBACK_FUNCTION			_callbackFunction;
	CALLBACK_FUNCTION_PARAMETER	_callbackFunctionParameter;

public:
	animation();
	~animation();

	HRESULT init(int totalW, int totalH, int frameW, int frameH);
	void release();

	//디폴트 애니메이션 재생
	void setDefPlayFrame(BOOL reverse = FALSE, BOOL loop = FALSE);
	void setDefPlayFrame(BOOL reverse, BOOL loop, CALLBACK_FUNCTION cbFunction);
	void setDefPlayFrame(BOOL reverse, BOOL loop, CALLBACK_FUNCTION_PARAMETER cbFunction, void* obj);

	//배열로 애니메이션 재생
	void setPlayFrame(int* playArr, int arrLen, BOOL loop = FALSE);
	void setPlayFrame(int* playArr, int arrLen, BOOL loop, CALLBACK_FUNCTION cbFunction);
	void setPlayFrame(int* playArr, int arrLen, BOOL loop, CALLBACK_FUNCTION_PARAMETER cbFunction, void* obj);

	//시작프레임부터 끝프레임까지 구간만 애니메이션 재생
	void setPlayFrame(int start, int end, BOOL reverse = FALSE, BOOL loop = FALSE);
	void setPlayFrame(int start, int end, BOOL reverse, BOOL loop, CALLBACK_FUNCTION cbFunction);
	void setPlayFrame(int start, int end, BOOL reverse, BOOL loop, CALLBACK_FUNCTION_PARAMETER cbFunction, void* obj);

	
	//프레임 셋팅
	void setFPS(int framePerSec);
	//프레임 재생 속도 셋팅
	void frameUpdate(float elapsedTime);

	void start();	//애니메이션 재생
	void stop();	//애니메이션 스탑
	void pause();	//애니메이션 일시중지
	void resume();	//애니메이션 다시재생

	inline BOOL isPlay() { return _play; }
	inline POINT getFramePos() { return _frameList[_playList[_nowPlayIndex]]; }
	inline int getFrameWidth() { return _frameWidth; }
	inline int getFrameHeight() { return _frameHeight; }

};


#pragma once
#include "dictionary.h"

using namespace std;

//desc: 프레임 이미지 매니저 추가 date 2021/1/29 by pju
class frameInfoManager : public singletonBase<frameInfoManager>
{
private:
	struct tagSeq
	{
		string key;	//key
		int index;	//인덱스
	};

	//프레임 이미지 key, 프레임 이미지 정보
	typedef map<string, tagSampleFrameInfo> mTotalFrameInfo;
	typedef map<string, tagSampleFrameInfo>::iterator miTotalFrameInfo;

	//프레임 이미지 key와 인덱스의 벡터
	typedef vector<tagSeq> vFrameSequence;
	typedef vector<tagSeq>::iterator viFrameSequence;

	
private:
	mTotalFrameInfo _mTotalFrameInfo;
	miTotalFrameInfo _miTotalFrameInfo;
	vFrameSequence _vFrameSequence;
	viFrameSequence _viFrameSequence;
	int count;
public:
	frameInfoManager();
	~frameInfoManager();

	void update();

	//이미지매니저에 할당되지 않을 경우 이 함수를 통해 할당 가능하며 해당 매니저 정보를 추가한다.
		//key, 이미지 경로(~.png), 프레임 x축, 프레임 y축, 속성(종류), 프레임이 바뀌는 간격?, 트리거 유무, 트리거 시작되는 프레임 인덱스, 반복 유무 
	void AddFrameInfo(const string& key, const wstring& path, const int maxFrameX, const int maxFrameY, FRAMEATTRIBUTE kinds, int frameInterval = 8, bool isTrigger = false, int triggerIndex = 0, bool isLoop = true);

	//이미 할당되었다는 가정하에 해당 매니저에 정보를 추가한다.
	//key, 속성(종류), 프레임이 바뀌는 간격?, 트리거 유무, 트리거 시작되는 프레임 인덱스, 반복 유무 
	void AddFrameInfo(const string& key, FRAMEATTRIBUTE kinds, int frameInterval, int triggerIndex, bool isTrigger = false, bool isLoop = true);
	
	//index를 기준으로 이미지를 반환한다.(부정확 할 수 있음)
	class Image * FindImage(int index);
	//key를 기준으로 이미지를 반환한다.
	class Image * FindImage(string key);

	//index를 기준으로 프레임 정보를 담은 구조체를 한꺼번에 반환한다.
	tagSampleFrameInfo GetInfo(int index);

	//map의 사이즈(갯수)를 반환한다.
	int GetSize() { return _mTotalFrameInfo.size(); };

	//key를 기준으로 map에 존재하는지 bool값을 반환한다.
	bool KeyCheck(string key) { return _mTotalFrameInfo.find(key) == _mTotalFrameInfo.end() ? false : true;}

	//inex를 기준으로 map에 존재하는 key를 반환한다.
	string FindKey(int key);

	//key를 기준으로 map에서 속성을 조회한다.
	FRAMEATTRIBUTE GetAttribute(string key) { return _mTotalFrameInfo.find(key)->second.kinds; }
	//index를 기준으로 map에서 속성을 조회한다.
	FRAMEATTRIBUTE GetAttribute(int index);

	//트리거 관련 겟터
	int getTriggerIndex(string key) { return _mTotalFrameInfo.find(key)->second.triggerIndex; }

	//트리거 관련 셋터
	void setTriggerIndex(string key, bool triggerIndex) 
	{
		_mTotalFrameInfo.find(key)->second.triggerIndex = triggerIndex;
	}
	void setTriggerUse(string key, bool isTriggerUse) { _mTotalFrameInfo.find(key)->second.isTriggerUse = isTriggerUse; }
	void setTriggerUse(int index, bool isTriggerUse);
	void setLoop(string key, bool isLoop) { _mTotalFrameInfo.find(key)->second.isLoop = isLoop; }

	char FrameSortDirection(FRAMEATTRIBUTE Kinds, int maxFrameX, int maxFrameY);

	//다음 프레임 인덱스 계산
	//key에 대한 index, 현재 프레임 인덱스, 트리거 무시하여 재생(맵툴에서만 true)
	Vector2 FrameOperation(int index, Vector2 frameIndex, bool trigger = false, bool isAllFrame = false);
	Vector2 FrameOperation(string key, Vector2 frameIndex, bool trigger = false, bool isAllFrame = false);
	//key에 대한 index, 현재 프레임 인덱스, 트리거 무시하여 재생(맵툴에서만 true), 직접 프레임 재생 간격 설정
	Vector2 FrameOperation(int index, Vector2 frameIndex, int interval, bool trigger = false, bool isAllFrame = false);
	Vector2 FrameOperation(string key, Vector2 frameIndex, int interval, bool trigger = false, bool isAllFrame = false);
};


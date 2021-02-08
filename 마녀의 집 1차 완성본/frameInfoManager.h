#pragma once
#include "dictionary.h"

using namespace std;

//desc: ������ �̹��� �Ŵ��� �߰� date 2021/1/29 by pju
class frameInfoManager : public singletonBase<frameInfoManager>
{
private:
	struct tagSeq
	{
		string key;	//key
		int index;	//�ε���
	};

	//������ �̹��� key, ������ �̹��� ����
	typedef map<string, tagSampleFrameInfo> mTotalFrameInfo;
	typedef map<string, tagSampleFrameInfo>::iterator miTotalFrameInfo;

	//������ �̹��� key�� �ε����� ����
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

	//�̹����Ŵ����� �Ҵ���� ���� ��� �� �Լ��� ���� �Ҵ� �����ϸ� �ش� �Ŵ��� ������ �߰��Ѵ�.
		//key, �̹��� ���(~.png), ������ x��, ������ y��, �Ӽ�(����), �������� �ٲ�� ����?, Ʈ���� ����, Ʈ���� ���۵Ǵ� ������ �ε���, �ݺ� ���� 
	void AddFrameInfo(const string& key, const wstring& path, const int maxFrameX, const int maxFrameY, FRAMEATTRIBUTE kinds, int frameInterval = 8, bool isTrigger = false, int triggerIndex = 0, bool isLoop = true);

	//�̹� �Ҵ�Ǿ��ٴ� �����Ͽ� �ش� �Ŵ����� ������ �߰��Ѵ�.
	//key, �Ӽ�(����), �������� �ٲ�� ����?, Ʈ���� ����, Ʈ���� ���۵Ǵ� ������ �ε���, �ݺ� ���� 
	void AddFrameInfo(const string& key, FRAMEATTRIBUTE kinds, int frameInterval, int triggerIndex, bool isTrigger = false, bool isLoop = true);
	
	//index�� �������� �̹����� ��ȯ�Ѵ�.(����Ȯ �� �� ����)
	class Image * FindImage(int index);
	//key�� �������� �̹����� ��ȯ�Ѵ�.
	class Image * FindImage(string key);

	//index�� �������� ������ ������ ���� ����ü�� �Ѳ����� ��ȯ�Ѵ�.
	tagSampleFrameInfo GetInfo(int index);

	//map�� ������(����)�� ��ȯ�Ѵ�.
	int GetSize() { return _mTotalFrameInfo.size(); };

	//key�� �������� map�� �����ϴ��� bool���� ��ȯ�Ѵ�.
	bool KeyCheck(string key) { return _mTotalFrameInfo.find(key) == _mTotalFrameInfo.end() ? false : true;}

	//inex�� �������� map�� �����ϴ� key�� ��ȯ�Ѵ�.
	string FindKey(int key);

	//key�� �������� map���� �Ӽ��� ��ȸ�Ѵ�.
	FRAMEATTRIBUTE GetAttribute(string key) { return _mTotalFrameInfo.find(key)->second.kinds; }
	//index�� �������� map���� �Ӽ��� ��ȸ�Ѵ�.
	FRAMEATTRIBUTE GetAttribute(int index);

	//Ʈ���� ���� ����
	int getTriggerIndex(string key) { return _mTotalFrameInfo.find(key)->second.triggerIndex; }

	//Ʈ���� ���� ����
	void setTriggerIndex(string key, bool triggerIndex) 
	{
		_mTotalFrameInfo.find(key)->second.triggerIndex = triggerIndex;
	}
	void setTriggerUse(string key, bool isTriggerUse) { _mTotalFrameInfo.find(key)->second.isTriggerUse = isTriggerUse; }
	void setTriggerUse(int index, bool isTriggerUse);
	void setLoop(string key, bool isLoop) { _mTotalFrameInfo.find(key)->second.isLoop = isLoop; }

	char FrameSortDirection(FRAMEATTRIBUTE Kinds, int maxFrameX, int maxFrameY);

	//���� ������ �ε��� ���
	//key�� ���� index, ���� ������ �ε���, Ʈ���� �����Ͽ� ���(���������� true)
	Vector2 FrameOperation(int index, Vector2 frameIndex, bool trigger = false, bool isAllFrame = false);
	Vector2 FrameOperation(string key, Vector2 frameIndex, bool trigger = false, bool isAllFrame = false);
	//key�� ���� index, ���� ������ �ε���, Ʈ���� �����Ͽ� ���(���������� true), ���� ������ ��� ���� ����
	Vector2 FrameOperation(int index, Vector2 frameIndex, int interval, bool trigger = false, bool isAllFrame = false);
	Vector2 FrameOperation(string key, Vector2 frameIndex, int interval, bool trigger = false, bool isAllFrame = false);
};


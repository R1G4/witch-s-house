#include "stdafx.h"
#include "frameInfoManager.h"
#include "Image.h"

frameInfoManager::frameInfoManager()
{
}
frameInfoManager::~frameInfoManager()
{
}

void frameInfoManager::update()
{
	count++;
	if (count >= 8000)
		count = 0;
}

void frameInfoManager::AddFrameInfo(const string& key, const wstring& path, const int maxFrameX, const int maxFrameY, FRAMEATTRIBUTE kinds, int frameInterval,bool  isTrigger, int triggerIndex, bool isLoop)
{
	char sortDirection;
	Image* tempImg;
	tempImg = IMAGEMANAGER->FindImage(key);

	//해당 이미지가 추가되어 있지 않다면 추가해준다.
	if (!tempImg)
		tempImg = IMAGEMANAGER->AddFrameImage(key, path, maxFrameX, maxFrameY);

	//이미지 프레임 진행방향을 알기 위해 정렬 확인
	sortDirection = FrameSortDirection(kinds, tempImg->GetMaxFrameX(), tempImg->GetMaxFrameY());

	//0 일 경우 false이기에...
	//트리거는 오브젝트만 적용함
	if (triggerIndex == 0 && kinds == OBJ)
	{
		triggerIndex = sortDirection == 'N' ? tempImg->GetMaxFrameX() :
						sortDirection == 'X' ? tempImg->GetMaxFrameX() :
						tempImg->GetMaxFrameY();

		//그럼에도 false라면 반환
		if (triggerIndex == 0)
			return;
	}

	tagSampleFrameInfo tempInfo;
	tempInfo.kinds = kinds;
	tempInfo.triggerIndex = triggerIndex;
	tempInfo.isTrigger = isTrigger;
	tempInfo.isLoop = isLoop; 
	tempInfo.frameSortDirection = sortDirection;
	tempInfo.interval = frameInterval == 0 ? 8 : frameInterval;
	_mTotalFrameInfo.insert(pair< string, tagSampleFrameInfo>(key, tempInfo));

	tagSeq tempSeq;
	tempSeq.key = key;
	tempSeq.index = _vFrameSequence.size();

	_vFrameSequence.push_back(tempSeq);
}

void frameInfoManager::AddFrameInfo(const string& key, FRAMEATTRIBUTE kinds, int frameInterval, int triggerIndex, bool isTrigger, bool isLoop )
{
	char sortDirection;
	//해당 이미지가 추가되어 있지 않다면 
	Image* tempImg;
	tempImg = IMAGEMANAGER->FindImage(key);

	//에러나 먹어라..
	tempImg->GetSize();

	//이미지 프레임 진행방향을 알기 위해 정렬 확인
	sortDirection = FrameSortDirection(kinds, tempImg->GetMaxFrameX(), tempImg->GetMaxFrameY());

	//0 일 경우 false이기에...
	//트리거는 오브젝트만 적용함
	if (triggerIndex == 0 && kinds == OBJ)
	{
		triggerIndex = sortDirection == 'N' ? tempImg->GetMaxFrameX() :
			sortDirection == 'X' ? tempImg->GetMaxFrameX() :
			tempImg->GetMaxFrameY();

		//그럼에도 false라면 반환
		if (triggerIndex == 0)
			return;
	}

	tagSampleFrameInfo tempInfo;
	tempInfo.kinds = kinds;
	tempInfo.triggerIndex = triggerIndex;
	tempInfo.isTrigger = isTrigger;
	tempInfo.isLoop = isLoop;
	tempInfo.interval = frameInterval == 0 ? 8 : frameInterval;
	_mTotalFrameInfo.insert(pair< string, tagSampleFrameInfo>(key, tempInfo));

	tagSeq tempSeq;
	tempSeq.key = key;
	tempSeq.index = _vFrameSequence.size();

	_vFrameSequence.push_back(tempSeq);
}

class Image * frameInfoManager::FindImage(int index)
{
	string key;
	vector<tagSeq>::iterator iter;

	for (int i = 0; i < _vFrameSequence.size(); i++)
	{
		if (_vFrameSequence[i].index == index)
		{
			key = _vFrameSequence[index].key;
			break;
		}
	}

	//기본적으로 아래같은 사항이 발생하면 문제 생길거임
	//맵에 존재하지 백터 인덱스의 키값이 존재하지 않는다면
	//if (_mTotalFrameInfo.find(key) == _mTotalFrameInfo.end())
	//{
	//	//백터 인덱스 제거
	//	_vFrameSequence.erase(_vFrameSequence.begin() + index);
	//	--index;	//혹시몰라 전 인덱스로 변경
	//	key = _vFrameSequence[index].key;	//재할당
	//	
	//}

	return IMAGEMANAGER->FindImage(key);
}

class Image * frameInfoManager::FindImage(string key)
{
	Image* tempImg;
	tempImg = IMAGEMANAGER->FindImage(key);

	//해당 이미지가 존재하지 않는다면 에러발생할거임 ㅎㅎ.
	if (!tempImg)	return nullptr;

	return tempImg;
}

tagSampleFrameInfo frameInfoManager::GetInfo(int index)
{
	string key;

	for (int i = 0; i < _vFrameSequence.size(); i++)
	{
		if (_vFrameSequence[i].index == index)
		{
			key = _vFrameSequence[index].key;
			break;
		}
	}

	//기본적으로 아래같은 사항이 발생하면 문제 생길거임
	//맵에 존재하지 백터 인덱스의 키값이 존재하지 않는다면
	//if (_mTotalFrameInfo.find(key) == _mTotalFrameInfo.end())
	//{
	//	//백터 인덱스 제거
	//	_vFrameSequence.erase(_vFrameSequence.begin() + index);
	//	--index;	//혹시몰라 전 인덱스로 변경
	//	key = _vFrameSequence[index].key;	//재할당

	//}

	return _mTotalFrameInfo.find(key)->second;
}

string frameInfoManager::FindKey(int index)
{
	string key;
	vector<tagSeq>::iterator iter;

	for (int i = 0; i < _vFrameSequence.size(); i++)
	{
		if (_vFrameSequence[i].index == index)
		{
			key = _vFrameSequence[index].key;
			break;
		}
	}
	return key;
}


FRAMEATTRIBUTE frameInfoManager::GetAttribute(int index)
{
	string key;

	for (int i = 0; i < _vFrameSequence.size(); i++)
	{
		if (_vFrameSequence[i].index == index)
		{
			key = _vFrameSequence[index].key;
			break;
		}
	}
	return _mTotalFrameInfo.find(key)->second.kinds;
}

void frameInfoManager::setTrigger(int index, bool isTrigger)
{
	string key;

	for (int i = 0; i < _vFrameSequence.size(); i++)
	{
		if (_vFrameSequence[i].index == index)
		{
			key = _vFrameSequence[index].key;
			break;
		}
	}
	_mTotalFrameInfo[key].isTrigger = isTrigger;
	//_mTotalFrameInfo.find(key)->second.isTrigger == isTrigger;
}

//프레임 이미지의 정렬 기준
char frameInfoManager::FrameSortDirection(FRAMEATTRIBUTE Kinds, int maxFrameX, int maxFrameY)
{
	char sortDirection = 'N';

	if(maxFrameX > 1 && maxFrameY > 1)
		sortDirection = 'N';
	else if (maxFrameX <= 1)
		sortDirection = 'Y';
	else if (maxFrameY <= 1)
		sortDirection = 'X';

	return sortDirection;
}

Vector2 frameInfoManager::FrameOperation(int index, Vector2 frameIndex, bool trigger, bool isAllFrame)
{
	string key;
	bool isTrigger;
	bool isLoop;
	Image* img;
	for (int i = 0; i < _vFrameSequence.size(); i++)
	{
		if (_vFrameSequence[i].index == index)
		{
			key = _vFrameSequence[index].key;
			break;
		}
	}
	
	//인덱스에 해당하는 키값을 찾는다.
	tagSampleFrameInfo Info = _mTotalFrameInfo.find(key)->second;

	//프레임 바꾸는 간격
	if (!count % Info.interval)
		return frameIndex;

	img = IMAGEMANAGER->FindImage(key);

	//맵툴에서 보여주는 용으로 사용됨 isAllFrame
	//전체 프레임을 보여줄지? 아니면 프레임 여부에 따라 보여줄지?
	isTrigger = isAllFrame == true ? false : Info.isTrigger;
	//반복 재생을 할 것인지?
	isLoop = isAllFrame == true ? true : Info.isLoop;

	//프레임 진행 방향이 X축이라면
	if (Info.frameSortDirection == 'Y')
	{
		frameIndex.y++;
		if (!isTrigger)
		{
			if (!trigger)
			{
				frameIndex.y =
					frameIndex.y > img->GetMaxFrameY() - 1 ? (!isLoop ? img->GetMaxFrameY() - 1 : 0) : frameIndex.y;
			}
		}
		else
		{
			//트리거가 발동되지 않았을때(평상시)
			if (!trigger)
			{
				frameIndex.y = frameIndex.y > Info.triggerIndex - 1 ? 0 : frameIndex.y;
			}
			//트리거가 발동될시
			else
			{
				frameIndex.y =
					frameIndex.y > img->GetMaxFrameY() - 1 ? (!isLoop ? img->GetMaxFrameY() - 1 : Info.triggerIndex) : frameIndex.y;
			}
		}
	}
	else if (Info.frameSortDirection == 'X')
	{
		frameIndex.x++;
		if (!isTrigger)
		{
			if (!trigger)
			{
				frameIndex.x =
					frameIndex.x > img->GetMaxFrameX() - 1 ? (!isLoop ? img->GetMaxFrameX() - 1 : 0) : frameIndex.x;
			}
		}
		else
		{
			//트리거가 발동되지 않았을때(평상시)
			if (!trigger)
			{
				frameIndex.x = frameIndex.x > Info.triggerIndex - 1 ? 0 : frameIndex.x;
			}
			//트리거가 발동될시
			else
			{
				frameIndex.x = 
					frameIndex.x > img->GetMaxFrameX() - 1 ? (!isLoop ? img->GetMaxFrameX() - 1 : Info.triggerIndex) : frameIndex.x;
			}
		}
	}
	else
	{
		switch (Info.kinds)
		{
		case PLAYER:
			frameIndex.x++;
			frameIndex.x =
				frameIndex.x > img->GetMaxFrameX() - 1 ? (!isLoop ? img->GetMaxFrameX() - 1 : 0) : frameIndex.x;
			break;
		case OBJ:
		case ENEMY:
			frameIndex.y++;
			frameIndex.y =
				frameIndex.y > img->GetMaxFrameY() - 1 ? (!isLoop ? img->GetMaxFrameY() - 1 : 0) : frameIndex.y;
			break;
		}
	}

	return frameIndex;
}

Vector2 frameInfoManager::FrameOperation(string key, Vector2 frameIndex,bool trigger, bool isAllFrame)
{
	bool isTrigger;
	bool isLoop;
	Image* img;

	//인덱스에 해당하는 키값을 찾는다.
	tagSampleFrameInfo Info = _mTotalFrameInfo.find(key)->second;

	//프레임 바꾸는 간격
	if (count % Info.interval)
		return frameIndex;

	img = IMAGEMANAGER->FindImage(key);

	//맵툴에서 보여주는 용으로 사용됨 isAllFrame
	//전체 프레임을 보여줄지? 아니면 프레임 여부에 따라 보여줄지?
	isTrigger = isAllFrame == true ? false : Info.isTrigger;
	//반복 재생을 할 것인지?
	isLoop = isAllFrame == true ? true : Info.isLoop;

	//프레임 진행 방향이 X축이라면
	if (Info.frameSortDirection == 'Y')
	{
		frameIndex.y++;
		if (!isTrigger)
		{
			//if (!trigger)
			{
				frameIndex.y =
					frameIndex.y > img->GetMaxFrameY() - 1 ? (!isLoop ? img->GetMaxFrameY() - 1  : 0) : frameIndex.y;
			}
		}
		else
		{
			//트리거가 발동되지 않았을때(평상시)
			if (!trigger)
			{
				frameIndex.y = frameIndex.y > Info.triggerIndex - 1 ? 0 : frameIndex.y;
			}
			//트리거가 발동될시
			else
			{
				frameIndex.y =
					frameIndex.y > img->GetMaxFrameY() - 1 ? (!isLoop ? img->GetMaxFrameY() - 1 : Info.triggerIndex) : frameIndex.y;
			}
		}
	}
	else if (Info.frameSortDirection == 'X')
	{
		frameIndex.x++;
		if (!isTrigger)
		{
			//if (!trigger)
			{
				frameIndex.x =
					frameIndex.x > img->GetMaxFrameX() - 1 ? (!isLoop ? img->GetMaxFrameX() - 1 : 0) : frameIndex.x;
			}
		}
		else
		{
			//트리거가 발동되지 않았을때(평상시)
			if (!trigger)
			{
				frameIndex.x = frameIndex.x > Info.triggerIndex - 1 ? 0 : frameIndex.x;
			}
			//트리거가 발동될시
			else
			{
				frameIndex.x =
					frameIndex.x > img->GetMaxFrameX() - 1 ? (!isLoop ? img->GetMaxFrameX() - 1 : Info.triggerIndex) : frameIndex.x;
			}
		}
	}
	else
	{
		switch (Info.kinds)
		{
		case PLAYER:
			frameIndex.x++;
			frameIndex.x =
				frameIndex.x > img->GetMaxFrameX() - 1 ? (!isLoop ? img->GetMaxFrameX() - 1 : 0) : frameIndex.x;
			break;
		case OBJ:
		case ENEMY:
			frameIndex.y++;
			frameIndex.y = frameIndex.y > img->GetMaxFrameY() - 1 ? (!isLoop ? img->GetMaxFrameY() - 1 : 0) : frameIndex.y;
			break;
		}
	}

	return frameIndex;
}

Vector2 frameInfoManager::FrameOperation(int index, Vector2 frameIndex, int interval, bool trigger, bool isAllFrame)
{
	if (!interval)
		return frameIndex;

	//프레임 바꾸는 간격
	if (count % interval)
		return frameIndex;

	string key;
	bool isTrigger;
	bool isLoop;
	Image* img;
	for (int i = 0; i < _vFrameSequence.size(); i++)
	{
		if (_vFrameSequence[i].index == index)
		{
			key = _vFrameSequence[index].key;
			break;
		}
	}

	//인덱스에 해당하는 키값을 찾는다.
	tagSampleFrameInfo Info = _mTotalFrameInfo.find(key)->second;

	img = IMAGEMANAGER->FindImage(key);

	//맵툴에서 보여주는 용으로 사용됨 isAllFrame
	//전체 프레임을 보여줄지? 아니면 프레임 여부에 따라 보여줄지?
	isTrigger = isAllFrame == true ? false : Info.isTrigger;
	//반복 재생을 할 것인지?
	isLoop = isAllFrame == true ? true : Info.isLoop;

	//프레임 진행 방향이 X축이라면
	if (Info.frameSortDirection == 'Y')
	{
		frameIndex.y++;
		if (!isTrigger)
		{
			if (!trigger)
			{
				frameIndex.y =
					frameIndex.y > img->GetMaxFrameY() - 1 ? (!isLoop ? img->GetMaxFrameY() - 1 : 0) : frameIndex.y;
			}
		}
		else
		{
			//트리거가 발동되지 않았을때(평상시)
			if (!trigger)
			{
				frameIndex.y = frameIndex.y > Info.triggerIndex - 1 ? 0 : frameIndex.y;
			}
			//트리거가 발동될시
			else
			{
				frameIndex.y =
					frameIndex.y > img->GetMaxFrameY() - 1 ? (!isLoop ? img->GetMaxFrameY() - 1 : Info.triggerIndex) : frameIndex.y;
			}
		}
	}
	else if (Info.frameSortDirection == 'X')
	{
		frameIndex.x++;
		if (!isTrigger)
		{
			if (!trigger)
			{
				frameIndex.x =
					frameIndex.x > img->GetMaxFrameX() - 1 ? (!isLoop ? img->GetMaxFrameX() - 1 : 0) : frameIndex.x;
			}
		}
		else
		{
			//트리거가 발동되지 않았을때(평상시)
			if (!trigger)
			{
				frameIndex.x = frameIndex.x > Info.triggerIndex - 1 ? 0 : frameIndex.x;
			}
			//트리거가 발동될시
			else
			{
				frameIndex.x =
					frameIndex.x > img->GetMaxFrameX() - 1 ? (!isLoop ? img->GetMaxFrameX() - 1 : Info.triggerIndex) : frameIndex.x;
			}
		}
	}
	else
	{
		switch (Info.kinds)
		{
		case PLAYER:
			frameIndex.x++;
			frameIndex.x =
				frameIndex.x > img->GetMaxFrameX() - 1 ? (!isLoop ? img->GetMaxFrameX() - 1 : 0) : frameIndex.x;
			break;
		case OBJ:
		case ENEMY:
			frameIndex.y++;
			frameIndex.y =
				frameIndex.y > img->GetMaxFrameY() - 1 ? (!isLoop ? img->GetMaxFrameY() - 1 : 0) : frameIndex.y;
			break;
		}
	}

	return frameIndex;
}

Vector2 frameInfoManager::FrameOperation(string key, Vector2 frameIndex, int interval, bool trigger, bool isAllFrame)
{
	if (!interval)
		return frameIndex;

	//프레임 바꾸는 간격
	if (!count % interval)
		return frameIndex;

	bool isTrigger;
	bool isLoop;
	Image* img;

	//인덱스에 해당하는 키값을 찾는다.
	tagSampleFrameInfo Info = _mTotalFrameInfo.find(key)->second;

	img = IMAGEMANAGER->FindImage(key);

	//맵툴에서 보여주는 용으로 사용됨 isAllFrame
	//전체 프레임을 보여줄지? 아니면 프레임 여부에 따라 보여줄지?
	isTrigger = isAllFrame == true ? false : Info.isTrigger;
	//반복 재생을 할 것인지?
	isLoop = isAllFrame == true ? true : Info.isLoop;

	//프레임 진행 방향이 X축이라면
	if (Info.frameSortDirection == 'Y')
	{
		frameIndex.y++;
		if (!isTrigger)
		{
			if (!trigger)
			{
				frameIndex.y =
					frameIndex.y > img->GetMaxFrameY() - 1 ? (!isLoop ? img->GetMaxFrameY() - 1 : 0) : frameIndex.y;
			}
		}
		else
		{
			//트리거가 발동되지 않았을때(평상시)
			if (!trigger)
			{
				frameIndex.y = frameIndex.y > Info.triggerIndex - 1 ? 0 : frameIndex.y;
			}
			//트리거가 발동될시
			else
			{
				frameIndex.y =
					frameIndex.y > img->GetMaxFrameY() - 1 ? (!isLoop ? img->GetMaxFrameY() - 1 : Info.triggerIndex) : frameIndex.y;
			}
		}
	}
	else if (Info.frameSortDirection == 'X')
	{
		frameIndex.x++;
		if (!isTrigger)
		{
			if (!trigger)
			{
				frameIndex.x =
					frameIndex.x > img->GetMaxFrameX() - 1 ? (!isLoop ? img->GetMaxFrameX() - 1 : 0) : frameIndex.x;
			}
		}
		else
		{
			//트리거가 발동되지 않았을때(평상시)
			if (!trigger)
			{
				frameIndex.x = frameIndex.x > Info.triggerIndex - 1 ? 0 : frameIndex.x;
			}
			//트리거가 발동될시
			else
			{
				frameIndex.x =
					frameIndex.x > img->GetMaxFrameX() - 1 ? (!isLoop ? img->GetMaxFrameX() - 1 : Info.triggerIndex) : frameIndex.x;
			}
		}
	}
	else
	{
		//그외 사항은 그냥 줘버림
		switch (Info.kinds)
		{
		case PLAYER:
			frameIndex.x++;
			frameIndex.x =
				frameIndex.x > img->GetMaxFrameX() - 1 ? (!isLoop ? img->GetMaxFrameX() - 1 : 0) : frameIndex.x;
			break;
		case OBJ:
		case ENEMY:
			frameIndex.y++;
			frameIndex.y =
				frameIndex.y > img->GetMaxFrameY() - 1 ? (!isLoop ? img->GetMaxFrameY() - 1 : 0) : frameIndex.y;
			break;
		}
	}
	return frameIndex;
}
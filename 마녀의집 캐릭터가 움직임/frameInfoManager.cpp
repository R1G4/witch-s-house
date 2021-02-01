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

	//�ش� �̹����� �߰��Ǿ� ���� �ʴٸ� �߰����ش�.
	if (!tempImg)
		tempImg = IMAGEMANAGER->AddFrameImage(key, path, maxFrameX, maxFrameY);

	//�̹��� ������ ��������� �˱� ���� ���� Ȯ��
	sortDirection = FrameSortDirection(kinds, tempImg->GetMaxFrameX(), tempImg->GetMaxFrameY());

	//0 �� ��� false�̱⿡...
	//Ʈ���Ŵ� ������Ʈ�� ������
	if (triggerIndex == 0 && kinds == OBJ)
	{
		triggerIndex = sortDirection == 'N' ? tempImg->GetMaxFrameX() :
						sortDirection == 'X' ? tempImg->GetMaxFrameX() :
						tempImg->GetMaxFrameY();

		//�׷����� false��� ��ȯ
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
	//�ش� �̹����� �߰��Ǿ� ���� �ʴٸ� 
	Image* tempImg;
	tempImg = IMAGEMANAGER->FindImage(key);

	//������ �Ծ��..
	tempImg->GetSize();

	//�̹��� ������ ��������� �˱� ���� ���� Ȯ��
	sortDirection = FrameSortDirection(kinds, tempImg->GetMaxFrameX(), tempImg->GetMaxFrameY());

	//0 �� ��� false�̱⿡...
	//Ʈ���Ŵ� ������Ʈ�� ������
	if (triggerIndex == 0 && kinds == OBJ)
	{
		triggerIndex = sortDirection == 'N' ? tempImg->GetMaxFrameX() :
			sortDirection == 'X' ? tempImg->GetMaxFrameX() :
			tempImg->GetMaxFrameY();

		//�׷����� false��� ��ȯ
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

	//�⺻������ �Ʒ����� ������ �߻��ϸ� ���� �������
	//�ʿ� �������� ���� �ε����� Ű���� �������� �ʴ´ٸ�
	//if (_mTotalFrameInfo.find(key) == _mTotalFrameInfo.end())
	//{
	//	//���� �ε��� ����
	//	_vFrameSequence.erase(_vFrameSequence.begin() + index);
	//	--index;	//Ȥ�ø��� �� �ε����� ����
	//	key = _vFrameSequence[index].key;	//���Ҵ�
	//	
	//}

	return IMAGEMANAGER->FindImage(key);
}

class Image * frameInfoManager::FindImage(string key)
{
	Image* tempImg;
	tempImg = IMAGEMANAGER->FindImage(key);

	//�ش� �̹����� �������� �ʴ´ٸ� �����߻��Ұ��� ����.
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

	//�⺻������ �Ʒ����� ������ �߻��ϸ� ���� �������
	//�ʿ� �������� ���� �ε����� Ű���� �������� �ʴ´ٸ�
	//if (_mTotalFrameInfo.find(key) == _mTotalFrameInfo.end())
	//{
	//	//���� �ε��� ����
	//	_vFrameSequence.erase(_vFrameSequence.begin() + index);
	//	--index;	//Ȥ�ø��� �� �ε����� ����
	//	key = _vFrameSequence[index].key;	//���Ҵ�

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

//������ �̹����� ���� ����
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
	
	//�ε����� �ش��ϴ� Ű���� ã�´�.
	tagSampleFrameInfo Info = _mTotalFrameInfo.find(key)->second;

	//������ �ٲٴ� ����
	if (!count % Info.interval)
		return frameIndex;

	img = IMAGEMANAGER->FindImage(key);

	//�������� �����ִ� ������ ���� isAllFrame
	//��ü �������� ��������? �ƴϸ� ������ ���ο� ���� ��������?
	isTrigger = isAllFrame == true ? false : Info.isTrigger;
	//�ݺ� ����� �� ������?
	isLoop = isAllFrame == true ? true : Info.isLoop;

	//������ ���� ������ X���̶��
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
			//Ʈ���Ű� �ߵ����� �ʾ�����(����)
			if (!trigger)
			{
				frameIndex.y = frameIndex.y > Info.triggerIndex - 1 ? 0 : frameIndex.y;
			}
			//Ʈ���Ű� �ߵ��ɽ�
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
			//Ʈ���Ű� �ߵ����� �ʾ�����(����)
			if (!trigger)
			{
				frameIndex.x = frameIndex.x > Info.triggerIndex - 1 ? 0 : frameIndex.x;
			}
			//Ʈ���Ű� �ߵ��ɽ�
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

	//�ε����� �ش��ϴ� Ű���� ã�´�.
	tagSampleFrameInfo Info = _mTotalFrameInfo.find(key)->second;

	//������ �ٲٴ� ����
	if (count % Info.interval)
		return frameIndex;

	img = IMAGEMANAGER->FindImage(key);

	//�������� �����ִ� ������ ���� isAllFrame
	//��ü �������� ��������? �ƴϸ� ������ ���ο� ���� ��������?
	isTrigger = isAllFrame == true ? false : Info.isTrigger;
	//�ݺ� ����� �� ������?
	isLoop = isAllFrame == true ? true : Info.isLoop;

	//������ ���� ������ X���̶��
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
			//Ʈ���Ű� �ߵ����� �ʾ�����(����)
			if (!trigger)
			{
				frameIndex.y = frameIndex.y > Info.triggerIndex - 1 ? 0 : frameIndex.y;
			}
			//Ʈ���Ű� �ߵ��ɽ�
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
			//Ʈ���Ű� �ߵ����� �ʾ�����(����)
			if (!trigger)
			{
				frameIndex.x = frameIndex.x > Info.triggerIndex - 1 ? 0 : frameIndex.x;
			}
			//Ʈ���Ű� �ߵ��ɽ�
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

	//������ �ٲٴ� ����
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

	//�ε����� �ش��ϴ� Ű���� ã�´�.
	tagSampleFrameInfo Info = _mTotalFrameInfo.find(key)->second;

	img = IMAGEMANAGER->FindImage(key);

	//�������� �����ִ� ������ ���� isAllFrame
	//��ü �������� ��������? �ƴϸ� ������ ���ο� ���� ��������?
	isTrigger = isAllFrame == true ? false : Info.isTrigger;
	//�ݺ� ����� �� ������?
	isLoop = isAllFrame == true ? true : Info.isLoop;

	//������ ���� ������ X���̶��
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
			//Ʈ���Ű� �ߵ����� �ʾ�����(����)
			if (!trigger)
			{
				frameIndex.y = frameIndex.y > Info.triggerIndex - 1 ? 0 : frameIndex.y;
			}
			//Ʈ���Ű� �ߵ��ɽ�
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
			//Ʈ���Ű� �ߵ����� �ʾ�����(����)
			if (!trigger)
			{
				frameIndex.x = frameIndex.x > Info.triggerIndex - 1 ? 0 : frameIndex.x;
			}
			//Ʈ���Ű� �ߵ��ɽ�
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

	//������ �ٲٴ� ����
	if (!count % interval)
		return frameIndex;

	bool isTrigger;
	bool isLoop;
	Image* img;

	//�ε����� �ش��ϴ� Ű���� ã�´�.
	tagSampleFrameInfo Info = _mTotalFrameInfo.find(key)->second;

	img = IMAGEMANAGER->FindImage(key);

	//�������� �����ִ� ������ ���� isAllFrame
	//��ü �������� ��������? �ƴϸ� ������ ���ο� ���� ��������?
	isTrigger = isAllFrame == true ? false : Info.isTrigger;
	//�ݺ� ����� �� ������?
	isLoop = isAllFrame == true ? true : Info.isLoop;

	//������ ���� ������ X���̶��
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
			//Ʈ���Ű� �ߵ����� �ʾ�����(����)
			if (!trigger)
			{
				frameIndex.y = frameIndex.y > Info.triggerIndex - 1 ? 0 : frameIndex.y;
			}
			//Ʈ���Ű� �ߵ��ɽ�
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
			//Ʈ���Ű� �ߵ����� �ʾ�����(����)
			if (!trigger)
			{
				frameIndex.x = frameIndex.x > Info.triggerIndex - 1 ? 0 : frameIndex.x;
			}
			//Ʈ���Ű� �ߵ��ɽ�
			else
			{
				frameIndex.x =
					frameIndex.x > img->GetMaxFrameX() - 1 ? (!isLoop ? img->GetMaxFrameX() - 1 : Info.triggerIndex) : frameIndex.x;
			}
		}
	}
	else
	{
		//�׿� ������ �׳� �����
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
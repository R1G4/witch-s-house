#pragma once
#include "singletonBase.h"
#include "dictionary.h"

class gameNode;

class sceneManager : public singletonBase<sceneManager>
{
public:
	typedef map<string, gameNode*>			 mapSceneList;
	typedef map<string, gameNode*>::iterator mapSceneIter;

private:
	static gameNode* _currentScene;
	mapSceneList _mSceneList;

public:
	sceneManager();
	~sceneManager();

	HRESULT init();
	void release();
	void update();
	void render();

	gameNode* addScene(string sceneName, gameNode* scene);

	virtual HRESULT changeScene(string sceneName);
	//�������� key, �÷��̾ �ٶ󺸴� ����
	virtual HRESULT changeScene(string sceneName, CHRDIRECTION _chrdirection);
	//�������� key, �÷��̾ �ٶ󺸴� ����, �������� ���� �ű涧 �ʱ� �÷��̾� ��ġ(����)
	virtual HRESULT changeScene(string sceneName, CHRDIRECTION _chrdirection, LOCATION _location);
	bool getKeyCheck(string key) 
	{
		//key(sceneName)�� �� �ʿ� �����ϴ��� Ȯ���Ѵ�.
		if (!(_mSceneList.find(key) == _mSceneList.end() ? false : true))
			return false;	

		//���� �Ѵٸ� key�� �ش��ϴ� ���� ã�Ƴ��� ���� ���� ������ Ȯ�� �� ����� ��ȯ�Ѵ�,
		return _currentScene == _mSceneList.find(key)->second ? true : false;
	}
};


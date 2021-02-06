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
	//스테이지 key, 플레이어가 바라보는 방향
	virtual HRESULT changeScene(string sceneName, CHRDIRECTION _chrdirection);
	//스테이지 key, 플레이어가 바라보는 방향, 스테이지 씬을 옮길때 초기 플레이어 위치(구분)
	virtual HRESULT changeScene(string sceneName, CHRDIRECTION _chrdirection, LOCATION _location);
	bool getKeyCheck(string key) 
	{
		//key(sceneName)이 이 맵에 존재하는지 확인한다.
		if (!(_mSceneList.find(key) == _mSceneList.end() ? false : true))
			return false;	

		//존재 한다면 key에 해당하는 값을 찾아내어 현재 씬과 같은지 확인 후 결과를 반환한다,
		return _currentScene == _mSceneList.find(key)->second ? true : false;
	}
};


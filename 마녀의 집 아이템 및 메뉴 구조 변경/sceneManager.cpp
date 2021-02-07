#include "stdafx.h"
#include "sceneManager.h"
#include "gameNode.h"

sceneManager::sceneManager()
{
}


sceneManager::~sceneManager()
{
}

gameNode* sceneManager::_currentScene = NULL;

HRESULT sceneManager::init()
{
	_currentScene = NULL;

	return S_OK;
}

void sceneManager::release()
{
	mapSceneIter miSceneList = _mSceneList.begin();

	for (; miSceneList != _mSceneList.end();)
	{
		if (miSceneList->second != NULL)
		{
			if (miSceneList->second == _currentScene) miSceneList->second->release();
			SAFE_DELETE(miSceneList->second);
			miSceneList = _mSceneList.erase(miSceneList);
		}
		else ++miSceneList;
	}

	_mSceneList.clear();
}

void sceneManager::update()
{
	if (_currentScene) _currentScene->update();
}

void sceneManager::render()
{
	if (_currentScene) _currentScene->render();
}

gameNode * sceneManager::addScene(string sceneName, gameNode * scene)
{
	if (!scene) return nullptr;

	_mSceneList.insert(make_pair(sceneName, scene));

	return scene;
}

HRESULT sceneManager::changeScene(string sceneName)
{
	mapSceneIter find = _mSceneList.find(sceneName);

	if (find == _mSceneList.end()) return E_FAIL;
	if (find->second == _currentScene) return S_OK;

	if (SUCCEEDED(find->second->init()))
	{
		//� ���� ������ ����ֱ� ������ ������ ���� ���ְ�
		if (_currentScene) _currentScene->release();

		//���� ���� �ٲٷ��� ���� ��´�
		_currentScene = find->second;

		return S_OK;
	}

	return E_FAIL;
}

HRESULT sceneManager::changeScene(string sceneName, CHRDIRECTION _chrdirection)
{
	mapSceneIter find = _mSceneList.find(sceneName);

	if (find == _mSceneList.end()) return E_FAIL;
	if (find->second == _currentScene) return S_OK;

	if (SUCCEEDED(find->second->init(_chrdirection)))
	{
		//� ���� ������ ����ֱ� ������ ������ ���� ���ְ�
		if (_currentScene) _currentScene->release();

		//���� ���� �ٲٷ��� ���� ��´�
		_currentScene = find->second;
		
		return S_OK;
	}
	return E_FAIL;
}

HRESULT sceneManager::changeScene(string sceneName, CHRDIRECTION _chrdirection, LOCATION _location)
{
	mapSceneIter find = _mSceneList.find(sceneName);

	if (find == _mSceneList.end()) return E_FAIL;
	if (find->second == _currentScene) return S_OK;

	if (SUCCEEDED(find->second->init(_chrdirection, _location)))
	{
		//� ���� ������ ����ֱ� ������ ������ ���� ���ְ�
		if (_currentScene) _currentScene->release();

		//���� ���� �ٲٷ��� ���� ��´�
		_currentScene = find->second;

		return S_OK;
	}
	return E_FAIL;
}

HRESULT sceneManager::changeScene(string sceneName, int x, int y, CHRDIRECTION _chrdirection)
{
	mapSceneIter find = _mSceneList.find(sceneName);

	if (find == _mSceneList.end()) return E_FAIL;
	if (find->second == _currentScene) return S_OK;

	if (SUCCEEDED(find->second->init(x, y,_chrdirection)))
	{
		//� ���� ������ ����ֱ� ������ ������ ���� ���ְ�
		if (_currentScene) _currentScene->release();

		//���� ���� �ٲٷ��� ���� ��´�
		_currentScene = find->second;

		return S_OK;
	}
	return E_NOTIMPL;
}

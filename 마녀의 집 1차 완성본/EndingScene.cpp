#include "stdafx.h"
#include "EndingScene.h"

EndingScene::EndingScene()
{
}

EndingScene::~EndingScene()
{
}

HRESULT EndingScene::init()
{
	CAMERAMANAGER->setConfig(0, 0, TILESIZEX, TILESIZEY, 0, 0, TILESIZEX, TILESIZEY);
	_backGround = IMAGEMANAGER->FindImage("¹è°æ93");
	setScript();
	alpha = 1.0f;
	_timeCount = 0;
	_player = new Player;
	_player->setStart(0, 0);
	_player->init();
	_player->setPLocaX(WINSIZEX / 2+300);
	_player->setPLocaY(50);
	_player->setDirec(CHRDIREC_DOWN);
	_player->setState(CHR_WALK);
	i = 0;
	return S_OK;
}

void EndingScene::release()
{
}

void EndingScene::update()
{
	CAMERAMANAGER->setCameraX(_player->getPlayerLocX()-WINSIZEX/2-300);
	CAMERAMANAGER->setCameraY(_player->getPlayerLocY()-500);
	//if (_player->getPlayerLocY()>1300)CAMERAMANAGER->setCameraY(1500 - WINSIZEY / 2);
	_player->setSpeed(0.8);
	_player->setFrameSpeed(20);
	_player->update();
}

void EndingScene::render()
{
	_backGround->SetSize(Vector2(1280, 1800));
	CAMERAMANAGER->render(_backGround, Vector2(WINSIZEX / 2, WINSIZEY / 2));
	_timeCount++;
	D2DINS->RenderTextField(0, WINSIZEY / 2, _vEndingScript[i],RGB(255,255,255), 50, 500, 50,alpha, DWRITE_TEXT_ALIGNMENT_CENTER);
	if (_timeCount > 150)alpha-=0.1;
	if (alpha < 0)
	{
		_timeCount = 0;
		i++;
		alpha = 1;
		if (i > _vEndingScript.size()-1)SCENEMANAGER->changeScene("½ÃÀÛÈ­¸é");
	}
	_player->render();

}

void EndingScene::setScript()
{
	string temp = "Team : R1G4";
	//string temp = "ÆÀ : R1G4";
	wstring temp1 = L"";
	//wstring temp1 = stringToWstring(temp);
	temp1.assign(temp.begin(), temp.end());
	_vEndingScript.push_back(temp1);
	 temp = "ÆÀÀå : ±èµ¿Çö";
	wstring temp2 = stringToWstring(temp);
	//temp2.assign(temp.begin(), temp.end());
	_vEndingScript.push_back(temp2);
	 temp = "ÆÀ¿ø: ¼ÛÃ¢ÈÆ , ±èµµ±Õ";
	 wstring temp3 = stringToWstring(temp);
	//temp3.assign(temp.begin(), temp.end());
	_vEndingScript.push_back(temp3);
	 temp = "¹ÚÁ¾¿í , ¹Ú¼ºÈÆ";
	wstring temp4 = stringToWstring(temp);
	//temp4.assign(temp.begin(), temp.end());
	_vEndingScript.push_back(temp4);
	 temp = "Witch's House";
	wstring temp5 = L"";
	temp5.assign(temp.begin(), temp.end());
	_vEndingScript.push_back(temp5);
	 temp = "Á¦ÀÛ±â°£ : 2ÁÖ";
	 wstring temp6 = stringToWstring(temp);
	//wstring temp6 = L"";
	//temp6.assign(temp.begin(), temp.end());
	_vEndingScript.push_back(temp6);
	temp = "The End";
	wstring temp7 = L"";
	temp7.assign(temp.begin(), temp.end());
	_vEndingScript.push_back(temp7);
}

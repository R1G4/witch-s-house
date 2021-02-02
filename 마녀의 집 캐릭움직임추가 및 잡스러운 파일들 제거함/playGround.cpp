#include "stdafx.h"
#include "playGround.h"

playGround::playGround()
{
}


playGround::~playGround()
{
}

//초기화 함수
HRESULT playGround::init()
{
	gameNode::init(true);

#pragma region desc: 나중에 로딩으로 옮기기 date: 2021/1/29 by pju
	IMAGEMANAGER->AddImage("화살표", L"Image/mapTool/화살표.png");
	IMAGEMANAGER->AddImage("화살표왼쪽", L"Image/mapTool/화살표왼쪽.png");
	IMAGEMANAGER->AddImage("화살표오른쪽", L"Image/mapTool/화살표오른쪽.png");
	IMAGEMANAGER->AddImage("1층스타트", L"Image/mapTool/stageMap/08.png");

	//아래 매니저에서도 프레임 이미지를 추가 가능하다.
	//key, 이미지 경로(~.png), 프레임 x축, 프레임 y축, 속성(종류), 프레임이 바뀌는 간격?, 트리거 유무, 트리거 시작되는 프레임 인덱스, 반복 유무 
#pragma endregion


	addressLink();

	addScene();
	addAutoImage();
	addFrameImg();
	SCENEMANAGER->addScene("MapToolScene", new mapTool);
	

	firstFloorStage* _firstFloorStage;
	_firstFloorStage = new stage1_1;
	SCENEMANAGER->addScene("stage1_1", _firstFloorStage);

#pragma region 동현이형 파일 수정 읽고 지우십시오
	//동현이형 가든 맵 다시만들어야 할수도..
	//일단 에러안나게 랜더부분 수정하였음
	//구조체 타일에 인덱스 변수를 삭제하고 키값(image)으로 대체하였음
	//수정한 형식과 비슷하게 하시면 될 것같음
#pragma endregion

	
	//SCENEMANAGER->changeScene("성앞");
	SCENEMANAGER->changeScene("BossStage");

	//SCENEMANAGER->changeScene("stage1_1");
	//SCENEMANAGER->changeScene("MapToolScene");
	/////////////////UI 일단 주석처리/////////////
	//addScene();
	//SCENEMANAGER->changeScene("시작화면");
	/////////////////////////////////////////////

	return S_OK;
}

//메모리 해제
void playGround::release()
{

}

//연산
void playGround::update()
{
	gameNode::update();
	SCENEMANAGER->update();
	FRAMEINFOMANAGER->update();
}

//그리기 전용
void playGround::render()
{
	//백버퍼 초기화
	D2DRenderer::GetInstance()->BeginRender(D2D1::ColorF::Black);
	{
		SCENEMANAGER->render();
	}
	//백버퍼에 그린 내용들을 화면에 뿌려라~
	D2DRenderer::GetInstance()->EndRender();
}

void playGround::addAutoImage()
{
	wstring backName_w = L"";
	string backName;
	for (int i = 1; i < 94; i++)
	{
		backName = "Image/mapTool/stageMap/0" + to_string(i) + ".png";
		backName_w = L"";
		backName_w.assign(backName.begin(), backName.end());
		IMAGEMANAGER->AddImage("배경" + to_string(i), backName_w);
	}
	for (int i = 1; i <= OBJSIZE; i++)
	{
		backName = "Image/obj/0" + to_string(i) + ".png";
		backName_w = L"";
		backName_w.assign(backName.begin(), backName.end());
		IMAGEMANAGER->AddImage("obj" + to_string(i), backName_w);
	}
}

void playGround::addScene()
{
	SCENEMANAGER->addScene("시작화면", new menu);
	SCENEMANAGER->addScene("레벨선택", new levelChoiceScene);
	SCENEMANAGER->addScene("MapToolScene", new mapTool);
	SCENEMANAGER->addScene("BossStage", new bossStage);
	SCENEMANAGER->addScene("정원", new garDen);
	SCENEMANAGER->addScene("정원아래", new garDenUnder);
	SCENEMANAGER->addScene("성앞", new castlefront);

	//보스씬 관련
	SCENEMANAGER->addScene("BossStage", _bossStage);
	SCENEMANAGER->addScene("Boss2", _bossStage_2);
}

void playGround::addFrameImg()
{
	FRAMEINFOMANAGER->AddFrameInfo("플레이어", L"Image/tempFrameImg/player.png", 16, 4, PLAYER);
	FRAMEINFOMANAGER->AddFrameInfo("곰", L"Image/tempFrameImg/곰.png", 1, 4, ENEMY);
	FRAMEINFOMANAGER->AddFrameInfo("눈깔", L"Image/tempFrameImg/눈깔.png", 3, 4, ENEMY);
	FRAMEINFOMANAGER->AddFrameInfo("해골", L"Image/tempFrameImg/해골.png", 3, 4, ENEMY);
	FRAMEINFOMANAGER->AddFrameInfo("액자", L"Image/tempFrameImg/액자1.png", 1, 4, OBJ, 8, true, 1, false);
	FRAMEINFOMANAGER->AddFrameInfo("꽃병프레임", L"Image/tempFrameImg/꽃병프레임.png", 1, 4, OBJ, 10, true, 1, true);
	FRAMEINFOMANAGER->AddFrameInfo("켜진초", L"Image/tempFrameImg/켜진초.png", 3, 1, OBJ);
	FRAMEINFOMANAGER->AddFrameInfo("꺼진초", L"Image/tempFrameImg/꺼진초.png", 3, 1, OBJ);
	FRAMEINFOMANAGER->AddFrameInfo("켜진초꺼진초", L"Image/tempFrameImg/켜진초꺼진초.png", 6, 1, OBJ, 10, true, 3, false);
	FRAMEINFOMANAGER->AddFrameInfo("문1", L"Image/tempFrameImg/문1.png", 1, 4, OBJ, 11, true, 1, false);
	FRAMEINFOMANAGER->AddFrameInfo("빨간꽃1", L"Image/tempFrameImg/빨간꽃1.png", 1, 3, OBJ, 15);
	FRAMEINFOMANAGER->AddFrameInfo("빨간꽃2", L"Image/tempFrameImg/빨간꽃2.png", 1, 4, OBJ, 15);
	FRAMEINFOMANAGER->AddFrameInfo("빨간꽃3", L"Image/tempFrameImg/빨간꽃3.png", 1, 3, OBJ, 15);
	FRAMEINFOMANAGER->AddFrameInfo("새장", L"Image/tempFrameImg/새장.png", 1, 4, OBJ, 5, true, 1, false);
	FRAMEINFOMANAGER->AddFrameInfo("2중철창", L"Image/tempFrameImg/2중철창.png", 1, 2, OBJ, 10, true, 1, false);

	//캐릭터 이미지 추가
	FRAMEINFOMANAGER->AddFrameInfo("violaIdle", L"Image/violaFrameImg/violaIdle.png", 16, 4, PLAYER);
	FRAMEINFOMANAGER->AddFrameInfo("violaWalk", L"Image/violaFrameImg/violaWalk.png", 16, 4, PLAYER);
	FRAMEINFOMANAGER->AddFrameInfo("violaDash", L"Image/violaFrameImg/violaDash.png", 16, 4, PLAYER);
}

void playGround::addressLink()
{
	_player = new Player;
	_bossStage = new bossStage;
	_bossStage_2 = new bossStage_2;
	_bossStage->addresslink(_player);
	_bossStage_2->addresslink(_player);

}

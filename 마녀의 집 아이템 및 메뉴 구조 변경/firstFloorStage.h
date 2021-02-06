#pragma once
#include "gameNode.h"
#include "bear.h"

class  Player;
class firstFloorStage : public gameNode
{
protected:
	enum MISSION
	{
		FAIL,
		SUCCESS
	};
	//프레임 이미지(플레이어, 에너미, 오브젝트) 타일
	vector<tagFrameTile> _vFrameTile;
	//맵타일
	tagTile _tiles[TILEX*TILEY];
	//케뭬라
	Vector2 camera;

	Player* _player;
	bear* _bear;
	float _sceneAlpha;			//씬 투명도
	int _frameInterval;			//프레임 바뀌는 간격
	int _delay;					//딜레이(특정 트리거 밞을 시 멈칫 하는 ?)
	bool isTrigger;
	bool _isBlood;

public:
	firstFloorStage();
	~firstFloorStage();

	HRESULT init();
	void release();
	void update();
	void cameraUpdate();
	void render();
	void getFrameTile();
	void setFrameIndex();			// 배치된 프레임 이미지의 인덱스 설정 
	void tileCollision(int i, int j);
	void sceneChange(string name, CHRDIRECTION _chrdirection, LOCATION _location);
	void setAlpha();
	void addresslink(Player* player) { _player = player; }

	//FORM

protected:
	enum FROM_DIRECTION
	{
		LEFT,
		RIGHT
	};
	FROM_DIRECTION _fromSelected;

private:
	bool _isForm;
	wstring FormInfo[2];
	float _rcAlpha;			//렉트 알파값(투명도)
	float _rcAlphaChange;	//렉트 알파값(투명도) 조절용
	FloatRect _rcSelected;	//선택된 박스를 렉트로 보여줌

public:
	void rcAlphaChange();
	void FormRender();
	//bool SelectionForm();

	//텍스를 넣는 동시에 폼 실행
	bool SelectionForm(wstring leftText, wstring rightText);

	//텍스트 넣기
	void setForm(wstring leftText, wstring rightText)
	{
		FormInfo[LEFT] = leftText;
		FormInfo[RIGHT] = rightText;
	};

	////폼 실행
	//void startForm()	{_isForm = true;};

	////폼 종료
	//void stopForm()		{_isForm = false;};
};


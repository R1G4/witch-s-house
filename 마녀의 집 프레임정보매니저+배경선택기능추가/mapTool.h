#pragma once
#include "gameNode.h"

class mapTool :
	public gameNode
{
private:
	bool tabOpen;
	Image* _backGround;
	vector<Image*>_vSampleMap;
//	int TILEX;
//	int TILEY;
	tagButton Save;
	tagButton Load;
	tagButton Prev;
	tagButton Next;
	tagButton terrain;
	tagButton Object;
	tagButton Erase;
	tagButton Open;
	tagButton Close;
	tagButton Collider;
	tagButton Player;
	tagButton Enemy;
	tagButton FrameObj;
	tagButton setTri;
	tagButton setCor;
	tagButton prevArrow;//desc: 프레임 이미지 버튼 추가 date: 2021/1/29 by pju
	tagButton nextArrow;
	tagButton BackGround;

	//현재 타일
	tagCurrentTile _currentTile;
	//샘플타일
	tagSampleTile _sampleTile[SAMPLETILEX*SAMPLETILEY];
	//샘플 오브젝트
	tagSampleObj _sampleObj[SAMPLEOBJECTX*SAMPLEOBJECTY];
	//샘플 프레임 이미지 정보
	vector<tagSampleFrameInfo> _sampleFrameImg;
	//프레임 이미지(플레이어, 에너미, 오브젝트) 타일
	vector<tagFrameTile> _vFrameTile;

	FRAMEATTRIBUTE FrAtt;
	int _change_num;		// 큰틀을 넘기기 위한 변수
	int _realNum;			// 각 오브젝트의 실제번호 부여를 위한 변수
	int _y_rect_num;		// y축 렉트갯수를 변환시키기 위한 변수?		
	int _frameSelected;		// 샘플 프레임 백터의 인덱스며 이전과 다음 기능에서 사용된다.
	int _frameInterval;		// 프레임의 인덱스 설정의 간격
	bool _change_number;	// 클릭시 틀을 한번 씩만 넘기기위한 불값

	//맵타일
	tagTile _tiles[TILEX*TILEY];
	int _crtSelect;
	FloatRect sampleSelec;//현재샘플타일 셀렉했음을 알려주는 렉트
	FloatRect tileSelec;//타일맵에서 선택될 타일을 알려줌
	FloatRect sampleBack;
	bool _leftButtonDown;
	//맵 툴의 일정 부분만 출력시키기 위한 렉트
	FloatRect MapRC;
	Vector2 camera;
	//bool isterrain;	by pju 이넘으로 대체해야 할듯...
	int backCount;
	string backName;
public:
	mapTool();
	~mapTool();

	HRESULT init();
	void release();
	void update();
	void render();

	void setButton();
	void setup();
	void setMap();
	void setCtrl();
	void mapMove();
	void tileSelect();
	void save();
	void load();
	void erase();
	void previous();
	void next();
	void setFrameTile();		
	bool addFrameTile(tagFrameTile _frameTile);
	void getFrameTile();			// Load후 실제 프레임 실제로 배치
	void setFrameIndex();			// 배치된 프레임 이미지의 인덱스 설정 
	void setCorrelation();
	void setTrigger();
	void setBack();
	TERRAIN terrainSelect(int frameX, int frameY);
	OBJECT objSelect(int frameX, int frameY);
};


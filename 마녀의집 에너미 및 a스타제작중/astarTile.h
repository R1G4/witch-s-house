#pragma once
#include "gameNode.h"
class astarTile : public gameNode
{
private:
	int _idX;		//타일의 인덱스 X
	int _idY;		//타일의 인덱스 Y
	int _number;

	POINT _center;	//타일의 중점 값
	RECT _rc;

	//F = G + H
	//F == TotalCost (총 비용)
	//G == 시작점으로부터 현재 노드까지의 비용
	//H == 현재노드로부터 도착점까지의 비용

	float _totalCost;
	float _costFromStart;
	float _costToGoal;

	bool _isOpen;	//갈수있는 타일이니?

	astarTile* _parentNode;	//제일 상위 노드(현재타일)

	D2D1::ColorF::Enum _color;

	string _attribute;	//타일 속성

public:
	astarTile();
	~astarTile();

	HRESULT init(int idX, int idY);
	void release();
	void update();
	void render();

	int getIdx() { return _idX; }
	int getIdy() { return _idY; }

	void setColor(D2D1::ColorF::Enum cr) { _color = cr; }

	RECT getRect() { return _rc; }


	//================ 접근자, 설정자 들 ================
	void setCenter(POINT center) { _center = center; }
	POINT getCenter() { return _center; }

	void setAttribute(string str) { _attribute = str; }
	string getAttribute() { return _attribute; }

	void setTotalCost(float totalCost) { _totalCost = totalCost; }
	float getTotalCost() { return _totalCost; }

	void setCostFromStart(float costFromStart) { _costFromStart = costFromStart; }
	float getCostFromStart() { return _costFromStart; }

	void setCostToGoal(float costToGoal) { _costToGoal = costToGoal; }
	float getCostToGoal() { return _costToGoal; }

	void setParentNode(astarTile* t) { _parentNode = t; }
	astarTile* getParentNode() { return _parentNode; }

	void setIsOpen(bool isOpen) { _isOpen = isOpen; }
	bool getIsOpen() { return _isOpen; }

	void setNumber(int number) { _number = number; }
};


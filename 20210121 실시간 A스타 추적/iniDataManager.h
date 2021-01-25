#pragma once
#include "singletonBase.h"
#include <vector>

using namespace std;

struct tagIniData
{
	const char* subject;	//항목   여포
	const char* title;		//제목   무력
	const char* body;		//값     100
};

class iniDataManager : public singletonBase<iniDataManager>
{
private:
	typedef vector<tagIniData>				arrIniData;
	typedef vector<tagIniData>::iterator	arrIniDataIter;

	typedef vector<arrIniData>				arrIniDatas;
	typedef vector<arrIniData>::iterator	arrIniDatasIter;

private:
	arrIniDatas _vIniData;


public:
	iniDataManager();
	~iniDataManager();

	HRESULT init();
	
	//데이터 추가 함수
	void addData(const char* subject, const char* title, const char* body);
	//ini파일 만들어주는 함수
	void iniSave(const char* fileName);

	//문자열 데이터 불러오는 함수
	string loadDataString(const char* fileName, const char* subject, const char* title);

	//정수 데이터 불러오는 함수
	int loadDataInterger(const char* fileName, const char* subject, const char* title);

};


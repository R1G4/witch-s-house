#pragma once
#include "singletonBase.h"
#include <vector>

class txtData : public singletonBase<txtData>
{
public:
	txtData() {};
	~txtData() {};

	HRESULT init();
	void release();

	void txtSave(const char* saveFileName, vector<string> vStr);
	char* vectorArrayCombine(vector<string> vArray);

	vector<string> txtLoad(const char* loadFileName);
	vector<string> txtLoad(const char * loadFileName, const char * ch);
	vector<string> charArraySeparation(char charArray[]);
	vector<string> charArraySeparation(char charArray[], const char * ch);
	bool canLoadFile(const char * loadFileName);
	bool canLoadFile(const char * loadFileName, char ch);
};


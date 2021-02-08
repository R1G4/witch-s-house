#pragma once
#include "gameNode.h"
#include "singletonBase.h"

class textMananger : public singletonBase<textMananger>
{	
private:
	// 다이얼로그용
	bool _isStopToRead;
	int _string_count;
	vector<string> _vScript;
	int _count_dia;
	int _i_word;
public:
	textMananger() :_isStopToRead(0) {}
	~textMananger() {}

	void clearScript();
	vector<string> loadFile(const char* loadFileName);
	//void setScript(vector<string> script) { _vScript = script; }
	BOOL setNextScript(bool read);
	void renderText();
	BOOL getStop() { return _isStopToRead; }

	wstring stringToWstring(string src)
	{
		USES_CONVERSION;
		return A2W(src.c_str());
	}
};


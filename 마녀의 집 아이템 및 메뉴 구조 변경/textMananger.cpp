#include "stdafx.h"
#include "textMananger.h"

void textMananger::clearScript()
{
	_vScript.clear();
}

vector<string> textMananger::loadFile(const char * loadFileName)
{
	if (_vScript.empty() == false) return _vScript;

	_vScript = TXTDATA->txtLoad(loadFileName, "\n");
	return _vScript;
}

BOOL textMananger::setNextScript(bool read)
{
	if (_vScript.empty() == true) return false;
	if (read)
	{
		if (_string_count < _vScript.size())
		{
			_string_count++;
			_i_word = 0;
			if (_string_count >= _vScript.size())
			{
				_vScript.clear();
				_string_count = 0;
				return false;
			}
		}
		read = false;
	}
	return true;
}

void textMananger::renderText()
{
	D2DINS->FillRectangle(RectMakePivot(Vector2(0, WINSIZEY / 2 - 100), Vector2(WINSIZEX, 200), Pivot::LeftTop), D2D1::ColorF::Black, 0.5);

	char str[256];
	wstring wstr;

	strcpy_s(str, _vScript[_string_count].c_str());

	_count_dia++;
	if (_count_dia % 5 == 0 && _i_word <= strlen(str))	// 한줄 출력시 한글자씩 출력하게하는 함수
	{
		_i_word = _i_word + 2;
		strncpy_s(str, str, _i_word);
		wstr = stringToWstring(str);
		_count_dia = 0;
	}
	else if (_i_word >= strlen(str))
	{
		strncpy_s(str, str, _i_word);
		wstr = stringToWstring(str);
		_count_dia = 0;
	}
	else
	{
		strncpy_s(str, str, _i_word);
		wstr = stringToWstring(str);
	}

	D2DINS->RenderTextField(WINSIZEX / 2 - 200, WINSIZEY / 2 - 100, wstr, RGB(255, 255, 255), 20, 400, 200, 1.0);
}

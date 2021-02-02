#include "stdafx.h"
#include "sound.h"

sound::sound()
{
}

sound::~sound()
{
}

HRESULT sound::init()
{
	SOUNDMANAGER->addSound("main", "sound/main.OGG", false,false);
	SOUNDMANAGER->addSound("firstMap", "sound/firstMap.OGG", false,true);
	SOUNDMANAGER->addSound("wind", "sound/wind.OGG", false,true);
	SOUNDMANAGER->addSound("cursor", "sound/cursor.OGG", false, false);
	SOUNDMANAGER->addSound("click", "sound/click.OGG", false, false);

	if (SCENEMANAGER->changeScene("레벨선택"))
	{
		SOUNDMANAGER->stop("main");
	}
	_volume = 0.5f;
	_bgv = 0.5f;
	return S_OK;
}

void sound::release()
{
}

void sound::update()
{
}

void sound::render()
{
	
}

void sound::volumeUp()
{
	_volume += 0.025;
	SOUNDMANAGER->setVolume(_volume);
	if (_volume == 1.0f)
	{
		_volume = 0;
	}
}

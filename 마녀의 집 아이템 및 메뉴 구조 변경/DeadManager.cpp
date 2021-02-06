#include "stdafx.h"
#include "DeadManager.h"

void DeadManager::update()
{
	//DeadManager사용
	//1. 사망을 넣을 각 스테이지에서 참조를 DeadManager를 참조한 뒤 객체를 생성 (ex.DeadManager* dead)
	//2. 사망을 불러와야할 부분에서 DeadManager 객체->setDead(사망 원인)으로 사망 설정(ex. 보스의경우 dead->setDead(DEAD_BOSS))
	//3. 위치 지정이 필요한 사망 씬은 DEAD_SNAKE와 DEAD_CLOCK으로 이 둘은 setLocXY(x,y)를 통해 위치를 조정
	//4. DEAD_SKULL과 DEAD_BEAR의 경우 DEAD클래스 실행 동안 에너미 이미지를 꺼주는 편이 좋음(플레이어 이미지는 자동 변환될것)
	switch (_dead)
	{
	case DEAD_BOSS:
		//사망-by 보스
		_deadImage = IMAGEMANAGER->FindImage("bossDead");
		_isDead = true;
		if (!_gameOver)frameXPlay();
		_endCount++;
		if (_gameOver&&_endCount > 100)SCENEMANAGER->changeScene("시작화면");
		break;
	case DEAD_SKULL:
		//사망 by 해골
		//캐릭터이미지를 해골로 변환시키기에 별도의 랜더 등이 필요없음
		//
		_player->setChrImg(IMAGEMANAGER->FindImage("skullDeath"));
		if(!_isDead)_player->setFrameX(0);
		_player->setFrameSpeed(12);
		_isDead = true;
		if (_player->getFrameX() >= IMAGEMANAGER->FindImage("skullDeath")->GetMaxFrameX() - 1)
		{
			_player->setFrameX(IMAGEMANAGER->FindImage("skullDeath")->GetMaxFrameX() - 1);
			_gameOver = true;
		}
		if(_gameOver)_endCount++;
		if (_endCount > 100)SCENEMANAGER->changeScene("시작화면");
		break;
	case DEAD_BEAR:
		_player->setChrImg(IMAGEMANAGER->FindImage("bearDeath"));
		if (!_isDead)
		{
			_player->setFrameX(0);
			_player->setDirec(CHRDIREC_DOWN);
		}
		_player->setFrameY(0);
		_player->setFrameSpeed(10);
		_isDead = true;
		if (_player->getFrameX() >= IMAGEMANAGER->FindImage("bearDeath")->GetMaxFrameX() - 1)
		{
			_player->setFrameX(IMAGEMANAGER->FindImage("bearDeath")->GetMaxFrameX() - 1);
			_gameOver = true;
		}
		if (_gameOver)_endCount++;
		if (_endCount > 100)SCENEMANAGER->changeScene("시작화면");
		break;
	case DEAD_WALL:
		//사망씬-by 벽
		//이거는 setDead(DEAD_WALL)호출만하면 알아서 됨
		if (!_isDead)
		{
			_deadImage = IMAGEMANAGER->FindImage("LWall");
			_deadImage->SetSize(Vector2(IMAGEMANAGER->FindImage("LWall")->GetWidth(), 720));
			_deadImage2 = IMAGEMANAGER->FindImage("RWall");
			_deadImage2->SetSize(Vector2(IMAGEMANAGER->FindImage("RWall")->GetWidth(), 720));
			_deadRc = FloatRect(Vector2(0, 0), Vector2(IMAGEMANAGER->FindImage("LWall")->GetWidth(), 720), Pivot::LeftTop);
			_deadRc2 = FloatRect(Vector2(WINSIZEX - IMAGEMANAGER->FindImage("RWall")->GetWidth(), 0),
				Vector2(IMAGEMANAGER->FindImage("RWall")->GetWidth(), 720), Pivot::LeftTop);
			cout<<_deadRc2.bottom;
			cout << (float)IMAGEMANAGER->FindImage("RWall")->GetHeight();
		}
		_isDead = true;
		if (!_gameOver)
		{
			_deadRc.Move(Vector2(10, 0));
			_deadRc2.Move(Vector2(-10, 0));
			if (IntersectRectToRect(&_deadRc, &_deadRc2))_gameOver = true;
		}
		if (_gameOver)_endCount++;
		if (_endCount > 100)SCENEMANAGER->changeScene("시작화면");

		break;
	case DEAD_CLOCK:
		//사망 by 시계
		//setLocXY함수를 통해 프레임이미지 재생할 장소 지정
		//미세조정 필요할 것으로 판단됨
		_deadImage=IMAGEMANAGER->FindImage("clockDeath");
		_isDead = true;
		cout << locX << " " << endl;
		if (!_gameOver)frameYPlay();
		if(_gameOver)_endCount++;
		if (_endCount > 100)SCENEMANAGER->changeScene("시작화면");
		break;
	case DEAD_SOLDIER:
		break;
	case DEAD_SNAKE:
		//사망 by 뱀
		//시계와 마찬가지로 setLocXY함수로 이미지 재생할 장소를 지정
		_deadImage = IMAGEMANAGER->FindImage("snakeDeath");
		_isDead = true;
		if (!_gameOver)frameYPlay();
		if (_gameOver)_endCount++;
		if (_endCount > 100)SCENEMANAGER->changeScene("시작화면");
		break;
	}
}

void DeadManager::render()
{
	if (_isDead)
	{
		if (!_gameOver)
		{
			if (_dead == DEAD_BOSS || _dead == DEAD_SOLDIER)_deadImage->FrameRender(Vector2(WINSIZEX / 2, WINSIZEY / 2), frameX, frameY);
			if (_dead == DEAD_CLOCK || _dead == DEAD_SNAKE)CAMERAMANAGER->FrameRender(_deadImage, Vector2(locX + _deadImage->GetFrameSize().x / 2, locY),
				frameX, frameY);
			if (_dead == DEAD_WALL)
			{
				cout << _deadRc.top<<endl;
				_deadImage->Render(Vector2(_deadRc.left + _deadImage->GetWidth()/2, _deadRc.top+WINSIZEY/2));
				_deadImage2->Render(Vector2(_deadRc2.left + _deadImage2->GetWidth() / 2, _deadRc2.top +WINSIZEY/2));
			}
		}
		if (_gameOver)
		{
			IMAGEMANAGER->FindImage("gameOver")->SetSize(Vector2(1280, 720));
			IMAGEMANAGER->FindImage("gameOver")->Render(Vector2(WINSIZEX / 2, WINSIZEY / 2));
		}
	}
}

void DeadManager::setDead(DEADVARIABLE dead)
{
	_dead = dead;
}

void DeadManager::frameXPlay()
{

	_count++;
	if (_count % 8 == 0)
	{
		frameX++;
		if (frameX > _deadImage->GetMaxFrameX() - 1)
		{
			_gameOver = true;
			frameX = 0;
		}
	}
}
void DeadManager::frameYPlay()
{
	cout << frameY<<" "<<frameX<<endl;
	_count++;
	if (_count % 8 == 0)
	{
		frameY++;
		if (frameY > _deadImage->GetMaxFrameY() - 1)
		{
			if (_deadImage->GetMaxFrameX() - 1 > 0)
			{
				frameX++;
				frameY = 0;
			}
			else {
				_gameOver = true;
				frameY = 0;
			}	
		}
		if (frameX > _deadImage->GetMaxFrameX() - 2 && frameY >= _deadImage->GetMaxFrameY() - 1)
		{
			_gameOver = true;
			frameX = frameY = 0;
		}
	}
}

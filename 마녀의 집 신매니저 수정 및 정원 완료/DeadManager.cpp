#include "stdafx.h"
#include "DeadManager.h"

void DeadManager::update()
{
	//DeadManager���
	//1. ����� ���� �� ������������ ������ DeadManager�� ������ �� ��ü�� ���� (ex.DeadManager* dead)
	//2. ����� �ҷ��;��� �κп��� DeadManager ��ü->setDead(��� ����)���� ��� ����(ex. �����ǰ�� dead->setDead(DEAD_BOSS))
	//3. ��ġ ������ �ʿ��� ��� ���� DEAD_SNAKE�� DEAD_CLOCK���� �� ���� setLocXY(x,y)�� ���� ��ġ�� ����
	//4. DEAD_SKULL�� DEAD_BEAR�� ��� DEADŬ���� ���� ���� ���ʹ� �̹����� ���ִ� ���� ����(�÷��̾� �̹����� �ڵ� ��ȯ�ɰ�)
	switch (_dead)
	{
	case DEAD_BOSS:
		//���-by ����
		_deadImage = IMAGEMANAGER->FindImage("bossDead");
		_isDead = true;
		if (!_gameOver)frameXPlay();
		_endCount++;
		if (_gameOver&&_endCount > 100)SCENEMANAGER->changeScene("����ȭ��");
		break;
	case DEAD_SKULL:
		//��� by �ذ�
		//ĳ�����̹����� �ذ�� ��ȯ��Ű�⿡ ������ ���� ���� �ʿ����
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
		if (_endCount > 100)SCENEMANAGER->changeScene("����ȭ��");
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
		if (_endCount > 100)SCENEMANAGER->changeScene("����ȭ��");
		break;
	case DEAD_WALL:
		//�����-by ��
		//�̰Ŵ� setDead(DEAD_WALL)ȣ�⸸�ϸ� �˾Ƽ� ��
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
		if (_endCount > 100)SCENEMANAGER->changeScene("����ȭ��");

		break;
	case DEAD_CLOCK:
		//��� by �ð�
		//setLocXY�Լ��� ���� �������̹��� ����� ��� ����
		//�̼����� �ʿ��� ������ �Ǵܵ�
		_deadImage=IMAGEMANAGER->FindImage("clockDeath");
		_isDead = true;
		cout << locX << " " << endl;
		if (!_gameOver)frameYPlay();
		if(_gameOver)_endCount++;
		if (_endCount > 100)SCENEMANAGER->changeScene("����ȭ��");
		break;
	case DEAD_SOLDIER:
		break;
	case DEAD_SNAKE:
		//��� by ��
		//�ð�� ���������� setLocXY�Լ��� �̹��� ����� ��Ҹ� ����
		_deadImage = IMAGEMANAGER->FindImage("snakeDeath");
		_isDead = true;
		if (!_gameOver)frameYPlay();
		if (_gameOver)_endCount++;
		if (_endCount > 100)SCENEMANAGER->changeScene("����ȭ��");
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

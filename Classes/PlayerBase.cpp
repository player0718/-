#include "cocos2d.h"
#include "PlayerBase.h"
#include "GameScene.h"

USING_NS_CC;

bool PlayerBase::init()
{
	return true;
}

void PlayerBase::hpRaise(int num)
{
	hp += num * 10;
	if (hp > hpLimit)
	{
		hp = hpLimit;
	}
}

bool PlayerBase::expRaise(int num)
{
	exp += num * 10;
	if (exp >= expNeed)
	{
		expOverflow = exp - expNeed;
		level++;
		expNeed += 50;
		restartTime += 1;
		exp = expOverflow;
		return true;
	}
	else return false;
}

void PlayerBase::die()
{
	attack = false;
	move = false;
	collect = false;
}

void PlayerBase::restart(Vec2 spawnpoint)
{
	hp = hpLimit;
	attack = true;
	move = true;
	collect = true;
	this->setPosition(spawnpoint);
}

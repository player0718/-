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
		expNeed *= level;
		exp = expOverflow;
		return true;
	}
	else return false;
}

bool PlayerBase::die()
{
	if (hp <= 0)
	{
		attack = false;
		return true;
	}
	else return false;
}

/*void PlayerBase::restart(Vec2& spawnpoint)
{
	hp = hpLimit;
	attack = true;
	player->setPosition(spawnpoint);
}*/
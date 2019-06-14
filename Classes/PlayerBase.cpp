#include "cocos2d.h"
#include "PlayerBase.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"

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

/*bool PlayerBase::onTouchBegan(Touch* touch, Event* event)
{
	if (this->attack) {
		auto visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();
		auto arrow = Sprite::create("arrow.png");
		arrow->setPosition(Vec2(visibleSize.width / 4 + origin.x, visibleSize.height / 2 + origin.y));
		auto winsize = Director::getInstance()->getWinSize();
		this->addChild(arrow);
		Point po = touch->getLocation();
		auto playersize = arrow->getContentSize();
		arrow->removeFromParent();
		this->addChild(arrow);
		arrow->setPosition(Vec2(this->getPosition().x, this->getPosition().y - 20));
		Point shootVector = touch->getLocation() - arrow->getPosition();
		Point normalizedVector = ccpNormalize(shootVector);
		float radians = atan2(normalizedVector.y, -normalizedVector.x);
		float degree = CC_RADIANS_TO_DEGREES(radians);
		RotateTo* rot = RotateTo::create(0.01f, degree + 180);
		//arrow->runAction(MoveTo::create(1.0f,Vec2(po.x - winsize.width / 2+60, po.y - playersize.height*1.1-10)));
		float a;
		a = sqrt((po.x - this->getPosition().x)*(po.x - this->getPosition().x) + (po.y - this->getPosition().y)*(po.y - this->getPosition().y));
		MoveBy* move = MoveBy::create(1.0f, Vec2((po.x - arrow->getPosition().x) * 300 / a, (po.y - arrow->getPosition().y) * 300 / a));
		Sequence* seq = Sequence::create(rot, move, FadeOut::create(1.0f), NULL);
		arrow->runAction(seq);
	}

	return true;
}*/
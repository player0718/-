#ifndef __Player_H__
#define __Player_H__
#include "cocos2d.h"

#include "PlayerBase.h"
using namespace cocos2d;
USING_NS_CC;
#include <stdio.h>

class player : public cocos2d::Layer
{
private:
	bool running = false;
	bool shooting = false;
	cocos2d::Vector<Sprite*>arrowVector;
	cocos2d::Vector<Sprite*>groundVector;


	PlayerBase *playerbody;
	PlayerBase *monster;
	Sprite *ground[11];
	Sprite *bar1;
	Sprite *blood1;
	Sprite *arrow;
	std::map<cocos2d::EventKeyboard::KeyCode, bool> keys;
public:
	float speed = 0;
	static cocos2d::Scene* createScene();
	virtual bool init();
	bool isKeyPressed(EventKeyboard::KeyCode keyCode);
	bool ifpick(Sprite spr1, Sprite spr2);
	//void shootarrow();
	bool onTouchBegan(Touch* touch, Event* event);
	bool shootskill(Touch* touch, Event* event);
	//void TouchesEnded(Set *pTouches, Event *pEvent);
	void update(float delta) ;
	void keyPressedDuration(EventKeyboard::KeyCode code);
	void menuCloseCallback(Ref * pSender);
	void deleteStart();
	void ifhurt(float delta);
	void animationcreate(int direct);
	CREATE_FUNC(player);
};

#endif
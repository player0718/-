#ifndef __Player_H__
#define __Player_H__
#include "cocos2d.h"
using namespace cocos2d;
USING_NS_CC;
#include <stdio.h>

class player : public cocos2d::Layer
{
private:
	Sprite *playerbody;
	Sprite *arrow;
	ProgressTimer* progress1;//定义进度条指针
	LabelTTF* numsTTF;//定义标签指针
	std::map<cocos2d::EventKeyboard::KeyCode, bool> keys;
public:
	float speed = 0.6;
	static cocos2d::Scene* createScene();
	virtual bool init();
	bool isKeyPressed(EventKeyboard::KeyCode keyCode);
	bool ifpick(Sprite spr1, Sprite spr2);
	//void shootarrow();
	//bool canmove(Touch* touch, Event* event);
	bool onTouchBegan(Touch* touch, Event* event);
	//void TouchesEnded(Set *pTouches, Event *pEvent);
	void update(float delta) override;
	void keyPressedDuration(EventKeyboard::KeyCode code);
	void menuCloseCallback(Ref * pSender);
	void Update(float dt);
	CREATE_FUNC(player);
};

#endif
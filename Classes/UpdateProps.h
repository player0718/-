#pragma once
#include "cocos2d.h"	
#include "Map.h"

USING_NS_CC;

class UpdateProps:public cocos2d::Scene
{
public:
	GameMap* map;

	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(UpdateProps);

};


#include "UpdateProps.h"
#include "cocos2d.h"

USING_NS_CC;

Scene* UpdateProps::createScene()
{
	return UpdateProps::create();
}

bool UpdateProps::init()
{
	if (!Scene::init())
	{
		return false;
	}
	
}

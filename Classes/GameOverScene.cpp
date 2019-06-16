#include "GameOverScene.h"

USING_NS_CC;
using namespace ui;

Scene* GameOverScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameOverScene::create();
	layer->setName("GameOverScene");
	scene->addChild(layer);
	return scene;
}

bool GameOverScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto gameover = Text::create("YOU LOSE", "fonts/ConcertOne-Regular.ttf", 80);
	auto leave = Text::create("click the left to leave", "fonts/ConcertOne-Regular.ttf", 35);

	this->addChild(gameover,1);
	gameover->setColor(Color3B::WHITE);
	gameover->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2+100));
	this->addChild(leave,1);
	leave->setColor(Color3B::WHITE);
	leave->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 3+100));

	auto _listener = EventListenerTouchOneByOne::create();
	_listener->onTouchBegan = [this](Touch *touch, Event *event)
	{
		Director::getInstance()->end();
		return true;
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_listener, 100);
	return true;
}
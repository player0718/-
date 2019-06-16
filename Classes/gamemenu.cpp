#include "gamemenu.h"
#include "SimpleAudioEngine.h"
#include "Map.h"
#include "SelectLayer.h"
#include "GameScene.h"
USING_NS_CC;

Scene* gamemenu::createScene()
{
	//return   gamemenu::create();
	auto scene = Scene::create();

	auto layer = gamemenu::create();

	scene->addChild(layer);

	return scene;

}

static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}


bool gamemenu::init() {
	if (!Layer::init())
	{
		return false;
	}


	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto label = Label::createWithTTF("ARROW", "fonts/Marker Felt.ttf", 100);
	if (label == nullptr)
	{
		problemLoading("'fonts/Marker Felt.ttf'");
	}
	else
	{
		// position the label on the center of the screen
		label->setPosition(Vec2(origin.x + visibleSize.width / 2,
			origin.y + visibleSize.height - 1.5*label->getContentSize().height));

		// add the label as a child to this layer
		this->addChild(label, 1);
	}


	auto spirit_back = Sprite::create("bg.png");
	if (spirit_back == nullptr)
	{
		problemLoading("'bg.png'");
	}
	else
	{
		spirit_back->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
		spirit_back->setAnchorPoint(Point(0.5, 0.5));
		Size size_back = spirit_back->getContentSize();
		float scaleX = (float)visibleSize.width / (float)size_back.width;
		float scaleY = (float)visibleSize.height / (float)size_back.height;
		spirit_back->setScale(scaleX, scaleY);

		this->addChild(spirit_back);
	}

	auto closeItem = MenuItemImage::create(
		"vg_close.png",
		"CloseSelected.png",
		CC_CALLBACK_1(gamemenu::menuCloseCallback, this));

	if (closeItem == nullptr ||
		closeItem->getContentSize().width <= 0 ||
		closeItem->getContentSize().height <= 0)
	{
		problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
	}
	else
	{
		float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
		float y = origin.y + closeItem->getContentSize().height / 2;
		closeItem->setPosition(Vec2(x, y));
	}


	auto item1 = MenuItemImage::create(
		"djmode1.png",
		"djmode2.png",
		CC_CALLBACK_1(gamemenu::menubegingame_danji, this));

	if (item1 == nullptr ||
		item1->getContentSize().width <= 0 ||
		item1->getContentSize().height <= 0)
	{
		problemLoading("djmode1.png' and 'djmode2.png'");
	}
	else
	{
		item1->setPosition(Vec2(visibleSize.width / 2 + origin.x, 400));
	}

	auto item2 = MenuItemImage::create(
		"ljmode1.png",
		"ljmode2.png",
		CC_CALLBACK_1(gamemenu::menubegingame_lianji, this));

	if (item2 == nullptr ||
		item2->getContentSize().width <= 0 ||
		item2->getContentSize().height <= 0)
	{
		problemLoading("ljmode1.png' and 'ljmode2.png'");
	}
	else
	{
		item2->setPosition(Vec2(visibleSize.width / 2 + origin.x, 200));
	}

	// create menu, it's an autorelease object
	auto menu = Menu::create(item1,item2,closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	return true;
}

void gamemenu::menuCloseCallback(Ref* pSender)
{
	
	Director::getInstance()->end();



}

void gamemenu::menubegingame_danji(Ref* pSender)
{

	auto director = Director::getInstance();
	auto scene = GameScene::createScene();
	director->replaceScene(scene);



}

void gamemenu::menubegingame_lianji(Ref* pSender)
{

	auto director = Director::getInstance();
	auto scene = SelectLayer::createScene();
	director->replaceScene(scene);



}
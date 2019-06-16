#include "SelectLayer.h"
#include "ServerLayer.h"
#include "ClientLayer.h"

Scene* SelectLayer::createScene()
{
	auto scene = Scene::createWithPhysics();	
	auto layer = SelectLayer::create();

	scene->addChild(layer);
	return scene;
}

static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}


bool SelectLayer::init()
{
	if ( !Layer::init() )
	{
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
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

	this->initMenu();

	return true;
}

void SelectLayer::initMenu()
{
	auto winSize = Director::getInstance()->getVisibleSize();

	auto lblServer = Label::createWithSystemFont("Server", "Arial", 24.f);
	auto menuServer = MenuItemLabel::create(lblServer, CC_CALLBACK_1(SelectLayer::menuServerClick, this));
	menuServer->setPosition(Vec2(winSize.width * 0.5f, winSize.height * 0.6f));

	auto lblClient = Label::createWithSystemFont("Client", "Arial", 24.f);
	auto menuClient = MenuItemLabel::create(lblClient, CC_CALLBACK_1(SelectLayer::menuClientClick, this));
	menuClient->setPosition(Vec2(winSize.width * 0.5f, winSize.height * 0.5f));

	auto menu = Menu::create(menuServer, menuClient, nullptr);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);

}

void SelectLayer::menuClientClick(Ref* sender)
{
	auto scene = ClientLayer::createScene();
	Director::getInstance()->replaceScene(scene);
}

void SelectLayer::menuServerClick(Ref* sender)
{
	auto scene = ServerLayer::createScene();
	Director::getInstance()->replaceScene(scene);
}
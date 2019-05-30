/****************************************************************************
Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "menu1.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* HelloWorld1::createScene()
{
	return   HelloWorld1::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld1::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object
	auto closeItem = MenuItemImage::create(
		"vg_close.png",
		"CloseSelected.png",
		CC_CALLBACK_1(HelloWorld1::menuCloseCallback, this));

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

	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	/////////////////////////////
	// 3. add your codes below...

	// add a label shows "Hello World"
	// create and initialize a label

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

	auto menu_name = Sprite::create("menu_name.png");
	if (menu_name == nullptr)
	{
		problemLoading("'menu_name.png");
	}
	else
	{
		// position the sprite on the center of the screen
		menu_name->setPosition(250, 500);
		menu_name->setAnchorPoint(Point(0.5, 0.5));
		/*		Size size_back = menu_name->getContentSize();
		float scaleX = (float)visibleSize.width / (float)size_back.width;
		float scaleY = (float)visibleSize.height / (float)size_back.height;*/
		menu_name->setScale(2, 2);

		// add the sprite as a child to this layer*/
		this->addChild(menu_name, 1);
	}
	auto menu_begin1 = Sprite::create("danji.png");


	// add "HelloWorld" splash screen"

	if (menu_begin1 == nullptr)
	{
		problemLoading("'danji.png'");
	}
	else
	{

		// position the sprite on the center of the screen
		menu_begin1->setPosition(Vec2(visibleSize.width / 2 + origin.x, 400));
		menu_begin1->setAnchorPoint(Point(0.5, 0.5));
		/*	Size size_back = spirit_back->getContentSize();
		float scaleX = (float)visibleSize.width / (float)size_back.width;
		float scaleY = (float)visibleSize.height / (float)size_back.height;
		spirit_back->setScale(scaleX, scaleY);*/

		// add the sprite as a child to this layer
		this->addChild(menu_begin1, 1);
	}
	auto menu_begin2 = Sprite::create("lianji.png");


	if (menu_begin2 == nullptr)
	{
		problemLoading("'lianji.png'");
	}
	else
	{

		menu_begin2->setPosition(Vec2(visibleSize.width / 2 + origin.x, 250));
		menu_begin2->setAnchorPoint(Point(0.5, 0.5));
		this->addChild(menu_begin2, 1);
	}


	auto spirit_back = Sprite::create("bg.png");
	if (spirit_back == nullptr)
	{
		problemLoading("'bg.png'");
	}
	else
	{
		// position the sprite on the center of the screen
		spirit_back->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
		spirit_back->setAnchorPoint(Point(0.5, 0.5));
		Size size_back = spirit_back->getContentSize();
		float scaleX = (float)visibleSize.width / (float)size_back.width;
		float scaleY = (float)visibleSize.height / (float)size_back.height;
		spirit_back->setScale(scaleX, scaleY);

		// add the sprite as a child to this layer
		this->addChild(spirit_back, 0);
	}
	return true;
}




void HelloWorld1::menuCloseCallback(Ref* pSender)
{
	//Close the cocos2d-x game scene and quit the application
	Director::getInstance()->end();

	/*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

	//EventCustom customEndEvent("game_scene_close_event");
	//_eventDispatcher->dispatchEvent(&customEndEvent);


}

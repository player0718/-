#include "player.h"
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "PlayerBase.h"
#include<math.h>
USING_NS_CC;
Scene* player::createScene()
{
	auto scene = Scene::create();
	//    CCScene * scene = CCScene::create();// 创建场景
	//创建层
	player *layer = player::create();
	scene->addChild(layer);
	return scene;
}
static void problemLoading(const char* filename)

{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}
bool player::init() {

	if (!Layer::init())
	{
		return false;
	}
	

	setTouchEnabled(true);
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	arrow=Sprite::create("arrow.png");
	arrow->setPosition(Vec2(visibleSize.width / 4 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(arrow);
	playerbody = Sprite::create("playerbody.png");
	setTouchMode(kCCTouchesOneByOne);
	if (playerbody == nullptr)
	{
		problemLoading("'playerbody.png'");
	}
	else
	{
		// position the sprite on the center of the screen
		playerbody->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

		// add the sprite as a child to this layer
		this->addChild(playerbody, 1);
	}
	auto bar = Sprite::create("bar.png");
	//bar->setScale(0.3f);
	bar->setPosition(Vec2(playerbody->getContentSize().width / 2 , playerbody->getContentSize().height / 1.1 +30));
	playerbody->addChild(bar);
	//血条
	auto blood = Sprite::create("blood.png");
	//blood->setScale(0.3f);
	ProgressTimer* pBloodprogress = ProgressTimer::create(blood);
	pBloodprogress->setType(kCCProgressTimerTypeBar);
	pBloodprogress->setBarChangeRate(ccp(1, 0));
	pBloodprogress->setMidpoint(ccp(0, 0));
	pBloodprogress->setPosition(Vec2(playerbody->getContentSize().width / 2+23, playerbody->getContentSize().height / 1.1-203 ));
	pBloodprogress->setPercentage(50.0f);
	bar->addChild(pBloodprogress, 1, 1);
	
	EventListenerKeyboard* keyboard = EventListenerKeyboard::create();
	keyboard->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event)
	{
		keys[keyCode] = true;
	};
	keyboard->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event) {
		keys[keyCode] = false;
		//playerbody->pauseSchedulerAndActions();

	};
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboard, this);
	auto progressbgSprite = Sprite::create("bar.png");
	progressbgSprite->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height-10));
	this->addChild(progressbgSprite, 1);
	auto progressSprite = Sprite::create("blood.png");
	progress1 = ProgressTimer::create(progressSprite);
	progress1->setType(kCCProgressTimerTypeBar);
	progress1->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height -10));
	//进度动画运动方向，可以多试几个值，看看效果  
	progress1->setMidpoint(Vec2(0, 0));
	//进度条宽高变化  
	progress1->setBarChangeRate(Vec2(1, 0));
	progress1->setPercentage(0); //设置进度条百分比为0%     
	this->addChild(progress1, 1);
	numsTTF = CCLabelTTF::create("0", "Thonburi", 18);
	numsTTF->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height -10);
	this->addChild(numsTTF, 2);
	scheduleUpdate();  //默认调度器，调用update()方法
	this->scheduleUpdate();
	return true;
}





void player::Update(float dt)
{
	float cu = progress1->getPercentage();  //获取百分比     
	cu = cu + 1.0f;  //进度条进度 每次加1%      
	progress1->setPercentage(cu);

	if (cu <= 100)    //如果进度小于等于100%  
	{
		auto str1 = String::createWithFormat("%.2f%%", cu);
		numsTTF->setString(str1->getCString());  //设置标签显示内容
	}
	//如果进度条达到100%，则停止
	else
	{
		unscheduleUpdate();//停止默认调度器
	}
}

/*bool player::canmove(Touch* touch, Event* event)
{
	Point po = touch->getLocation();
	if (sqrt((po.x - playerbody->getPosition().x)*(po.x - playerbody->getPosition().x) + (po.y - playerbody->getPosition().y)*(po.y - playerbody->getPosition().y)) > 300)
	{
		return false;
	}
	else
	{
		return true;
	}
}*/

bool player::onTouchBegan(Touch* touch, Event* event)
{
	auto winsize = Director::getInstance()->getWinSize();
	arrow = Sprite::create("arrow.png");
	playerbody->addChild(arrow);
	Point po = touch->getLocation();
	auto playersize = arrow->getContentSize();
	arrow->removeFromParent();
	this->addChild(arrow);
	arrow->setPosition(Vec2(playerbody->getPosition().x , playerbody->getPosition().y-20 ));
	Point shootVector = touch->getLocation() - arrow->getPosition();
	Point normalizedVector = ccpNormalize(shootVector);
	float radians = atan2(normalizedVector.y ,- normalizedVector.x);
	float degree = CC_RADIANS_TO_DEGREES(radians);
	RotateTo* rot = RotateTo::create(0.01f, degree+180);
	//arrow->runAction(MoveTo::create(1.0f,Vec2(po.x - winsize.width / 2+60, po.y - playersize.height*1.1-10)));
	float a;
	a = sqrt((po.x - playerbody->getPosition().x)*(po.x - playerbody->getPosition().x) + (po.y - playerbody->getPosition().y)*(po.y - playerbody->getPosition().y));
	MoveBy* move = MoveBy::create(1.0f, Vec2((po.x - arrow->getPosition().x) * 300 / a, (po.y - arrow->getPosition().y) * 300 / a));
	Sequence* seq = Sequence::create(rot,move, FadeOut::create(1.0f), NULL);
	arrow->runAction(seq);
	
	return true;
}



bool player::isKeyPressed(EventKeyboard::KeyCode keyCode) {
	if (keys[keyCode]) {
		return true;
	}
	else {
		return false;
	}
}


void player::update(float delta) {
	//Node::update(delta);
	auto leftArrow = EventKeyboard::KeyCode::KEY_LEFT_ARROW, rightArrow = EventKeyboard::KeyCode::KEY_RIGHT_ARROW;
	auto upArrow = EventKeyboard::KeyCode::KEY_UP_ARROW, downArrow = EventKeyboard::KeyCode::KEY_DOWN_ARROW;
	if (isKeyPressed(leftArrow)) {
		keyPressedDuration(leftArrow);
	}
	else if (isKeyPressed(rightArrow)) {
		keyPressedDuration(rightArrow);
	}
	else if (isKeyPressed(upArrow)) {
		keyPressedDuration(upArrow);
	}
	else if (isKeyPressed(downArrow)) {
		keyPressedDuration(downArrow);
	}
}

void player::keyPressedDuration(EventKeyboard::KeyCode code) {
	int offsetX = 0, offsetY = 0;
	switch (code) {
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		offsetX = -5;
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		offsetX = 5;
		break;
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		offsetY = 5;
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		offsetY = -5;
		break;
	default:
		offsetY = offsetX = 0;
		break;
	}
	// 0.3s代表着动作从开始到结束所用的时间，从而显得不会那么机械。
	auto moveTo = MoveTo::create(speed, Vec2(playerbody->getPositionX() + offsetX, playerbody->getPositionY() + offsetY));
	playerbody->runAction(moveTo);
}

void player::menuCloseCallback(Ref* pSender)

{
	Director::getInstance()->end();
}

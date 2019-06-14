#include "player.h"
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
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


	
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	SpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("right.plist");
	Animation* animation = Animation::create();
	char name[100];
	for (int i = 11; i < 15; i++)
	{
		memset(name, 0, sizeof(name));
		sprintf(name, "player1_%d.png", i);
		animation->addSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name));
	}
	animation->setDelayPerUnit(0.5f);
	animation->setLoops(kRepeatForever);
	Animate* animate = Animate::create(animation);
	auto ani = Sprite::create();
	ani->setPosition(ccp(240, 160));
	ani->runAction(animate);
	addChild(ani);

	


	arrow = Sprite::create("arrow.png");
	arrow->setPosition(Vec2(visibleSize.width / 4 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(arrow);
	playerbody = PlayerBase::create("player1.png");
	if (playerbody == nullptr)
	{
		problemLoading("'player1.png'");
	}
	else
	{
		// position the sprite on the center of the screen
		playerbody->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

		// add the sprite as a child to this layer
		this->addChild(playerbody, 1);
	}
	auto bar = Sprite::create("bar.png");
	bar->setScale(0.3f);
	bar->setPosition(Vec2(playerbody->getContentSize().width / 2, playerbody->getContentSize().height / 1.1 +15));
	playerbody->addChild(bar);
	//血条
	auto blood = Sprite::create("blood.png");
	blood->setScale(0.3f);
	ProgressTimer* pBloodprogress = ProgressTimer::create(blood);
	pBloodprogress->setType(kCCProgressTimerTypeBar);
	pBloodprogress->setBarChangeRate(ccp(1, 0));
	pBloodprogress->setMidpoint(ccp(0, 0));
	pBloodprogress->setPosition(Vec2(playerbody->getContentSize().width / 2 +64, playerbody->getContentSize().height / 1.1 -17));
	float per = playerbody->hp * 100 / playerbody->hpLimit;
	pBloodprogress->setPercentage(per);
	bar->addChild(pBloodprogress, 1, 1);

	monster = PlayerBase::create("player1.png");
	if (monster == nullptr)
	{
		problemLoading("'monster.png'");
	}
	else
	{
		// position the sprite on the center of the screen
		monster->setPosition(Vec2(visibleSize.width * 3 / 4 + origin.x, visibleSize.height / 2 + origin.y));

		// add the sprite as a child to this layer
		this->addChild(monster, 1);
	}
	bar1 = Sprite::create("bar.png");
	bar1->setScale(0.3f);
	bar1->setPosition(Vec2(monster->getContentSize().width / 2, monster->getContentSize().height / 1.1 + 15));
	monster->addChild(bar1);
	//血条
	blood1 = Sprite::create("blood.png");
	blood1->setScale(0.3f);
	blood1->setPosition(Vec2(monster->getContentSize().width / 2, monster->getContentSize().height / 1.1 + 15));
	monster->addChild(blood1);
	/*ProgressTimer* pBloodprogress1 = ProgressTimer::create(blood1);
	pBloodprogress1->setType(kCCProgressTimerTypeBar);
	pBloodprogress1->setBarChangeRate(ccp(1, 0));
	pBloodprogress1->setMidpoint(ccp(0, 0));
	pBloodprogress1->setPosition(Vec2(monster->getContentSize().width / 2 + 64, monster->getContentSize().height / 1.1 - 17));
	float per1 = monster->hp * 100 / monster->hpLimit;
	pBloodprogress1->setPercentage(per1);
	bar1->addChild(pBloodprogress1, 1, 1);*/

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
	scheduleUpdate();  //默认调度器，调用update()方法
	

	auto touchListener1 = EventListenerTouchOneByOne::create();//创建单点触摸事件监听器
	touchListener1->onTouchBegan = CC_CALLBACK_2(player::onTouchBegan, this);//触摸开始
	touchListener1->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener1, this);
	setTouchEnabled(true);
	setTouchMode(kCCTouchesOneByOne);

	return true;
}

void player::animationcreate(int direct)
{
	if (!running) {
		running = true;
		SpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("animate.plist");
		Animation* animation = Animation::create();
		char name[100];
		for (int i = direct; i < direct+5; i++)
		{
			memset(name, 0, sizeof(name));
			sprintf(name, "player1_%d.png", i);
			animation->addSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name));
		}
		animation->setDelayPerUnit(0.05f);
		Animate* animate = Animate::create(animation);
		playerbody->runAction(animate);
		auto sequence = Sequence::create(DelayTime::create(0.25f), CallFunc::create([=]() {
			running = false;
		}), NULL);
		runAction(sequence);
	}
}


void player::ifhurt(float delta)
{
	for (int i = 0; i < arrowVector.size(); i++) {
		Sprite* firstarrow = arrowVector.at(i);
		if (firstarrow->getBoundingBox().intersectsRect(monster->getBoundingBox()))
		{
			monster->hpDecrease(0.01);
			blood1->setScaleX(monster->hp / monster->hpLimit);
			removeChild(firstarrow);
		}
	}
	for (int i = 0; i < groundVector.size(); i++) {
		Sprite* ground = groundVector.at(i);
		if (ground->getBoundingBox().intersectsRect(monster->getBoundingBox()))
		{
			monster->hpDecrease(1);
			blood1->setScaleX(monster->hp / monster->hpLimit);
		}
	}
}

bool player::shootskill(Touch * touch, Event * event)
{
	arrow = Sprite::create("arrow.png");
	arrowVector.pushBack(arrow);
	Point po = touch->getLocation();
	auto playersize = arrow->getContentSize();
	this->addChild(arrow);
	arrow->setPosition(Vec2(playerbody->getPosition().x, playerbody->getPosition().y - 10));
	Point shootVector = touch->getLocation() - arrow->getPosition();
	Point normalizedVector = ccpNormalize(shootVector);
	float radians = atan2(normalizedVector.y, -normalizedVector.x);
	float degree = CC_RADIANS_TO_DEGREES(radians);
	RotateTo* rot = RotateTo::create(0, degree + 180);
	MoveBy* move1 = MoveBy::create(0, Vec2(5 * sin(degree - 90), 5 * cos(degree - 90)));
	float a;
	a = sqrt((po.x - playerbody->getPosition().x)*(po.x - playerbody->getPosition().x) + (po.y - playerbody->getPosition().y)*(po.y - playerbody->getPosition().y));
	MoveBy* move = MoveBy::create(1.0f, Vec2((po.x - arrow->getPosition().x) * 300 / a, (po.y - arrow->getPosition().y) * 300 / a));
	Sequence* seq = Sequence::create(rot, move1, move, CallFunc::create([=]() {
		arrowVector.eraseObject(arrow);
		removeChild(arrow);
	}), NULL);
	arrow->runAction(seq);
	if (lianggen)
	{
		auto arrow1 = Sprite::create("arrow.png");
		arrowVector.pushBack(arrow1);
		this->addChild(arrow1);
		MoveBy* move1 = MoveBy::create(0, Vec2(-5 * sin(degree - 90), -5 * cos(degree - 90)));
		arrow1->setPosition(Vec2(playerbody->getPosition().x, playerbody->getPosition().y - 15));
		Sequence* seq1 = Sequence::create(rot->clone(),move1, move->clone(), CallFunc::create([=]() {
			removeChild(arrow1);
		}), NULL);
		arrow1->runAction(seq1);
	}
	if (sangen)
	{
		auto arrow3 = Sprite::create("arrow.png");
		arrowVector.pushBack(arrow3);
		this->addChild(arrow3);
		MoveBy* move3 = MoveBy::create(0, Vec2(-5 * sin(degree - 90),- 5 * cos(degree - 90)));
		arrow3->setPosition(Vec2(playerbody->getPosition().x, playerbody->getPosition().y - 5));
		Sequence* seq3 = Sequence::create(rot->clone(),move3 move->clone(), CallFunc::create([=]() {
			removeChild(arrow3);
		}), NULL);
		arrow3->runAction(seq3);
		auto arrow2 = Sprite::create("arrow.png");
		arrowVector.pushBack(arrow2);
		this->addChild(arrow2);
		arrow2->setPosition(Vec2(playerbody->getPosition().x, playerbody->getPosition().y - 15));
		Sequence* seq2 = Sequence::create(rot->clone(), move->clone(), CallFunc::create([=]() {
			removeChild(arrow2);
		}), NULL);
		arrow2->runAction(seq2);
	}
	if (houshe)
	{
		auto arrow4 = Sprite::create("arrow.png");
		arrowVector.pushBack(arrow4);
		this->addChild(arrow4);
		arrow4->setPosition(Vec2(playerbody->getPosition().x, playerbody->getPosition().y - 10));
		RotateTo* rot4 = RotateTo::create(0, degree);
		MoveBy* move4 = MoveBy::create(1.0f, Vec2(-(po.x - arrow->getPosition().x) * 300 / a, -(po.y - arrow->getPosition().y) * 300 / a));
		Sequence* seq4 = Sequence::create(rot4, move4, CallFunc::create([=]() {
			removeChild(arrow4);
		}), NULL);
		arrow4->runAction(seq4);
	}
	if (houer)
	{
		auto arrow5 = Sprite::create("arrow.png");
		arrowVector.pushBack(arrow5);
		this->addChild(arrow5);
		arrow5->setPosition(Vec2(playerbody->getPosition().x, playerbody->getPosition().y - 10));
		Sequence* seq5 = Sequence::create(rot->clone(), DelayTime::create(0.2f), move->clone(), CallFunc::create([=]() {
			removeChild(arrow5);
		}), NULL);
		arrow5->runAction(seq5);
	}
	if (sanxiang)
	{
		auto arrow6 = Sprite::create("arrow.png");
		arrowVector.pushBack(arrow6);
		this->addChild(arrow6);
		arrow6->setPosition(Vec2(playerbody->getPosition().x, playerbody->getPosition().y - 10));
		RotateTo* rot6 = RotateTo::create(0, degree + 90);
		MoveBy* move6 = MoveBy::create(1.0f, Vec2(-(po.y - arrow->getPosition().y) * 300 / a, (po.x - arrow->getPosition().x) * 300 / a));
		Sequence* seq6 = Sequence::create(rot6, move6, CallFunc::create([=]() {
			removeChild(arrow6);
		}), NULL);
		arrow6->runAction(seq6);
		auto arrow7 = Sprite::create("arrow.png");
		arrowVector.pushBack(arrow7);
		this->addChild(arrow7);
		arrow7->setPosition(Vec2(playerbody->getPosition().x, playerbody->getPosition().y - 10));
		RotateTo* rot7 = RotateTo::create(0, degree + 270);
		MoveBy* move7 = MoveBy::create(1.0f, Vec2((po.y - arrow->getPosition().y) * 300 / a, -(po.x - arrow->getPosition().x) * 300 / a));
		Sequence* seq7 = Sequence::create(rot7, move7, CallFunc::create([=]() {
			removeChild(arrow7);
		}), NULL);
		arrow7->runAction(seq7);
	}
	if (lianggen + houshe)
	{
		auto arrow8 = Sprite::create("arrow.png");
		arrowVector.pushBack(arrow8);
		this->addChild(arrow8);
		MoveBy* move8 = MoveBy::create(0, Vec2(-5 * sin(degree - 90), -5 * cos(degree - 90)));
		arrow8->setPosition(Vec2(playerbody->getPosition().x, playerbody->getPosition().y - 15));
		Sequence* seq8 = Sequence::create(rot->clone(),move8, move->clone(), CallFunc::create([=]() {
			removeChild(arrow8);
		}), NULL);
		arrow8->runAction(seq8);
		auto arrow9 = Sprite::create("arrow.png");
		arrowVector.pushBack(arrow9);
		this->addChild(arrow9);
		arrow9->setPosition(Vec2(playerbody->getPosition().x, playerbody->getPosition().y - 10));
		RotateTo* rot9 = RotateTo::create(0, degree);
		MoveBy* move9 = MoveBy::create(1.0f, Vec2(-(po.x - arrow->getPosition().x) * 300 / a, -(po.y - arrow->getPosition().y) * 300 / a));
		Sequence* seq9 = Sequence::create(rot9, move9, CallFunc::create([=]() {
			removeChild(arrow9);
		}), NULL);
		arrow9->runAction(seq9);
	}
	if (sangen + houshe)
	{
		auto arrow10 = Sprite::create("arrow.png");
		arrowVector.pushBack(arrow10);
		this->addChild(arrow10);
		MoveBy* move10 = MoveBy::create(0, Vec2(-5 * sin(degree - 90), -5 * cos(degree - 90)));
		arrow10->setPosition(Vec2(playerbody->getPosition().x, playerbody->getPosition().y - 5));
		Sequence* seq10 = Sequence::create(rot->clone(),move10, move->clone(), CallFunc::create([=]() {
			removeChild(arrow10);
		}), NULL);
		arrow10->runAction(seq10);
		auto arrow11 = Sprite::create("arrow.png");
		arrowVector.pushBack(arrow11);
		this->addChild(arrow11);
		arrow11->setPosition(Vec2(playerbody->getPosition().x, playerbody->getPosition().y - 15));
		Sequence* seq11 = Sequence::create(rot->clone(), move->clone(), CallFunc::create([=]() {
			removeChild(arrow11);
		}), NULL);
		arrow11->runAction(seq11);
		auto arrow12 = Sprite::create("arrow.png");
		arrowVector.pushBack(arrow12);
		this->addChild(arrow12);
		arrow12->setPosition(Vec2(playerbody->getPosition().x, playerbody->getPosition().y - 10));
		RotateTo* rot12 = RotateTo::create(0, degree);
		MoveBy* move12 = MoveBy::create(1.0f, Vec2(-(po.x - arrow->getPosition().x) * 300 / a, -(po.y - arrow->getPosition().y) * 300 / a));
		Sequence* seq12 = Sequence::create(rot12, move12, CallFunc::create([=]() {
			removeChild(arrow12);
		}), NULL);
		arrow12->runAction(seq12);
	}
	if (lianggen + sanxiang)
	{
		auto arrow13 = Sprite::create("arrow.png");
		arrowVector.pushBack(arrow13);
		this->addChild(arrow13);
		MoveBy* move13 = MoveBy::create(0, Vec2(-5 * sin(degree - 90), -5 * cos(degree - 90)));
		arrow13->setPosition(Vec2(playerbody->getPosition().x, playerbody->getPosition().y - 15));
		Sequence* seq13 = Sequence::create(rot->clone(),move13, move->clone(), CallFunc::create([=]() {
			removeChild(arrow13);
		}), NULL);
		arrow13->runAction(seq13);
		auto arrow14 = Sprite::create("arrow.png");
		arrowVector.pushBack(arrow14);
		this->addChild(arrow14);
		arrow14->setPosition(Vec2(playerbody->getPosition().x, playerbody->getPosition().y - 10));
		RotateTo* rot14 = RotateTo::create(0, degree + 90);
		MoveBy* move14 = MoveBy::create(1.0f, Vec2(-(po.y - arrow->getPosition().y) * 300 / a, (po.x - arrow->getPosition().x) * 300 / a));
		Sequence* seq14 = Sequence::create(rot14, move14, CallFunc::create([=]() {
			removeChild(arrow14);
		}), NULL);
		arrow14->runAction(seq14);
		auto arrow15 = Sprite::create("arrow.png");
		arrowVector.pushBack(arrow15);
		this->addChild(arrow15);
		arrow15->setPosition(Vec2(playerbody->getPosition().x, playerbody->getPosition().y - 10));
		RotateTo* rot15 = RotateTo::create(0, degree + 270);
		MoveBy* move15 = MoveBy::create(1.0f, Vec2((po.y - arrow->getPosition().y) * 300 / a, -(po.x - arrow->getPosition().x) * 300 / a));
		Sequence* seq15 = Sequence::create(rot15, move15, CallFunc::create([=]() {
			removeChild(arrow15);
		}), NULL);
		arrow15->runAction(seq15);
	}
	if (sangen + sanxiang)
	{
		auto arrow16 = Sprite::create("arrow.png");
		arrowVector.pushBack(arrow16);
		this->addChild(arrow16);
		MoveBy* move16 = MoveBy::create(0, Vec2(-5 * sin(degree - 90), -5 * cos(degree - 90)));
		arrow16->setPosition(Vec2(playerbody->getPosition().x, playerbody->getPosition().y - 5));
		Sequence* seq16 = Sequence::create(rot->clone(),move16, move->clone(), CallFunc::create([=]() {
			removeChild(arrow16);
		}), NULL);
		arrow16->runAction(seq16);
		auto arrow17 = Sprite::create("arrow.png");
		arrowVector.pushBack(arrow17);
		this->addChild(arrow17);
		arrow17->setPosition(Vec2(playerbody->getPosition().x, playerbody->getPosition().y - 15));
		Sequence* seq17 = Sequence::create(rot->clone(), move->clone(), CallFunc::create([=]() {
			removeChild(arrow17);
		}), NULL);
		arrow17->runAction(seq17);
		auto arrow18 = Sprite::create("arrow.png");
		arrowVector.pushBack(arrow18);
		this->addChild(arrow18);
		arrow18->setPosition(Vec2(playerbody->getPosition().x, playerbody->getPosition().y - 10));
		RotateTo* rot18 = RotateTo::create(0, degree + 90);
		MoveBy* move18 = MoveBy::create(1.0f, Vec2(-(po.y - arrow->getPosition().y) * 300 / a, (po.x - arrow->getPosition().x) * 300 / a));
		Sequence* seq18 = Sequence::create(rot18, move18, CallFunc::create([=]() {
			removeChild(arrow18);
		}), NULL);
		arrow18->runAction(seq18);
		auto arrow19 = Sprite::create("arrow.png");
		arrowVector.pushBack(arrow19);
		this->addChild(arrow19);
		arrow19->setPosition(Vec2(playerbody->getPosition().x, playerbody->getPosition().y - 10));
		RotateTo* rot19 = RotateTo::create(0, degree + 270);
		MoveBy* move19 = MoveBy::create(1.0f, Vec2((po.y - arrow->getPosition().y) * 300 / a, -(po.x - arrow->getPosition().x) * 300 / a));
		Sequence* seq19 = Sequence::create(rot19, move19, CallFunc::create([=]() {
			removeChild(arrow19);
		}), NULL);
		arrow19->runAction(seq19);
	}
	if (lianggen + sanxiang + houshe)
	{
		auto arrow20 = Sprite::create("arrow.png");
		arrowVector.pushBack(arrow20);
		this->addChild(arrow20);
		MoveBy* move20 = MoveBy::create(0, Vec2(-5 * sin(degree - 90), -5 * cos(degree - 90)));
		arrow20->setPosition(Vec2(playerbody->getPosition().x, playerbody->getPosition().y - 15));
		Sequence* seq20 = Sequence::create(rot->clone(),move20, move->clone(), CallFunc::create([=]() {
			removeChild(arrow20);
		}), NULL);
		arrow20->runAction(seq20);
		auto arrow21 = Sprite::create("arrow.png");
		arrowVector.pushBack(arrow21);
		this->addChild(arrow21);
		arrow21->setPosition(Vec2(playerbody->getPosition().x, playerbody->getPosition().y - 10));
		RotateTo* rot21 = RotateTo::create(0, degree + 90);
		MoveBy* move21 = MoveBy::create(1.0f, Vec2(-(po.y - arrow->getPosition().y) * 300 / a, (po.x - arrow->getPosition().x) * 300 / a));
		Sequence* seq21 = Sequence::create(rot21, move21, CallFunc::create([=]() {
			removeChild(arrow21);
		}), NULL);
		arrow21->runAction(seq21);
		auto arrow22 = Sprite::create("arrow.png");
		arrowVector.pushBack(arrow22);
		this->addChild(arrow22);
		arrow22->setPosition(Vec2(playerbody->getPosition().x, playerbody->getPosition().y - 10));
		RotateTo* rot22 = RotateTo::create(0, degree + 270);
		MoveBy* move22 = MoveBy::create(1.0f, Vec2((po.y - arrow->getPosition().y) * 300 / a, -(po.x - arrow->getPosition().x) * 300 / a));
		Sequence* seq22 = Sequence::create(rot22, move22, CallFunc::create([=]() {
			removeChild(arrow22);
		}), NULL);
		arrow22->runAction(seq22);
		auto arrow23 = Sprite::create("arrow.png");
		arrowVector.pushBack(arrow23);
		this->addChild(arrow23);
		arrow23->setPosition(Vec2(playerbody->getPosition().x, playerbody->getPosition().y - 10));
		RotateTo* rot23 = RotateTo::create(0, degree);
		MoveBy* move23 = MoveBy::create(1.0f, Vec2(-(po.x - arrow->getPosition().x) * 300 / a, -(po.y - arrow->getPosition().y) * 300 / a));
		Sequence* seq23 = Sequence::create(rot23, move23, CallFunc::create([=]() {
			removeChild(arrow23);
		}), NULL);
		arrow23->runAction(seq23);
	}
	if (sangen + sanxiang + houshe)
	{
		auto arrow24 = Sprite::create("arrow.png");
		arrowVector.pushBack(arrow24);
		this->addChild(arrow24);
		MoveBy* move24 = MoveBy::create(0, Vec2(-5 * sin(degree - 90), -5 * cos(degree - 90)));
		arrow24->setPosition(Vec2(playerbody->getPosition().x, playerbody->getPosition().y - 5));
		Sequence* seq24 = Sequence::create(rot->clone(),move24, move->clone(), CallFunc::create([=]() {
			removeChild(arrow24);
		}), NULL);
		arrow24->runAction(seq24);
		auto arrow25 = Sprite::create("arrow.png");
		arrowVector.pushBack(arrow25);
		this->addChild(arrow25);
		arrow25->setPosition(Vec2(playerbody->getPosition().x, playerbody->getPosition().y - 15));
		Sequence* seq25 = Sequence::create(rot->clone(), move->clone(), CallFunc::create([=]() {
			removeChild(arrow25);
		}), NULL);
		arrow25->runAction(seq25);
		auto arrow26 = Sprite::create("arrow.png");
		arrowVector.pushBack(arrow26);
		this->addChild(arrow26);
		arrow26->setPosition(Vec2(playerbody->getPosition().x, playerbody->getPosition().y - 10));
		RotateTo* rot26 = RotateTo::create(0, degree + 90);
		MoveBy* move26 = MoveBy::create(1.0f, Vec2(-(po.y - arrow->getPosition().y) * 300 / a, (po.x - arrow->getPosition().x) * 300 / a));
		Sequence* seq26 = Sequence::create(rot26, move26, CallFunc::create([=]() {
			removeChild(arrow26);
		}), NULL);
		arrow26->runAction(seq26);
		auto arrow27 = Sprite::create("arrow.png");
		arrowVector.pushBack(arrow27);
		this->addChild(arrow27);
		arrow27->setPosition(Vec2(playerbody->getPosition().x, playerbody->getPosition().y - 10));
		RotateTo* rot27 = RotateTo::create(0, degree + 270);
		MoveBy* move27 = MoveBy::create(1.0f, Vec2((po.y - arrow->getPosition().y) * 300 / a, -(po.x - arrow->getPosition().x) * 300 / a));
		Sequence* seq27 = Sequence::create(rot27, move27, CallFunc::create([=]() {
			removeChild(arrow27);
		}), NULL);
		arrow27->runAction(seq27);
		auto arrow28 = Sprite::create("arrow.png");
		arrowVector.pushBack(arrow28);
		this->addChild(arrow28);
		arrow28->setPosition(Vec2(playerbody->getPosition().x, playerbody->getPosition().y - 10));
		RotateTo* rot28 = RotateTo::create(0, degree);
		MoveBy* move28 = MoveBy::create(1.0f, Vec2(-(po.x - arrow->getPosition().x) * 300 / a, -(po.y - arrow->getPosition().y) * 300 / a));
		Sequence* seq28 = Sequence::create(rot28, move28, CallFunc::create([=]() {
			removeChild(arrow28);
		}), NULL);
		arrow28->runAction(seq28);
	}
	if (groundopen)
	{
		auto ground0 = Sprite::create("ground.png");
		groundVector.pushBack(ground0);
		this->addChild(ground0);
		ground0->setPosition(Vec2(playerbody->getPosition().x, playerbody->getPosition().y - 10));
		MoveBy* gmove0 = MoveBy::create(0, Vec2((po.x - arrow->getPosition().x) * 300 / a, (po.y - arrow->getPosition().y) * 300 / a));
		Sequence* gseq0 = Sequence::create(gmove0, DelayTime::create(1.0f), CallFunc::create([=]() {
			removeChild(ground0);
		}), NULL);
		ground0->runAction(gseq0);
		auto ground1 = Sprite::create("ground.png");
		groundVector.pushBack(ground1);
		this->addChild(ground1);
		ground1->setPosition(Vec2(playerbody->getPosition().x, playerbody->getPosition().y - 10));
		MoveBy* gmove1 = MoveBy::create(0, Vec2((po.x - arrow->getPosition().x) * 270 / a, (po.y - arrow->getPosition().y) * 270 / a));
		Sequence* gseq1 = Sequence::create(gmove1, DelayTime::create(1.0f), CallFunc::create([=]() {
			removeChild(ground1);
		}), NULL);
		ground1->runAction(gseq1);
		auto ground2 = Sprite::create("ground.png");
		groundVector.pushBack(ground2);
		this->addChild(ground2);
		ground2->setPosition(Vec2(playerbody->getPosition().x, playerbody->getPosition().y - 10));
		MoveBy* gmove2 = MoveBy::create(0, Vec2((po.x - arrow->getPosition().x) * 240 / a, (po.y - arrow->getPosition().y) * 240 / a));
		Sequence* gseq2 = Sequence::create(gmove2, DelayTime::create(1.0f), CallFunc::create([=]() {
			removeChild(ground2);
		}), NULL);
		ground2->runAction(gseq2);
		auto ground3 = Sprite::create("ground.png");
		groundVector.pushBack(ground3);
		this->addChild(ground3);
		ground3->setPosition(Vec2(playerbody->getPosition().x, playerbody->getPosition().y - 10));
		MoveBy* gmove3 = MoveBy::create(0, Vec2((po.x - arrow->getPosition().x) * 210 / a, (po.y - arrow->getPosition().y) * 210 / a));
		Sequence* gseq3 = Sequence::create(gmove3, DelayTime::create(1.0f), CallFunc::create([=]() {
			removeChild(ground3);
		}), NULL);
		ground3->runAction(gseq3);
		auto ground4 = Sprite::create("ground.png");
		groundVector.pushBack(ground4);
		this->addChild(ground4);
		ground4->setPosition(Vec2(playerbody->getPosition().x, playerbody->getPosition().y - 10));
		MoveBy* gmove4 = MoveBy::create(0, Vec2((po.x - arrow->getPosition().x) * 180 / a, (po.y - arrow->getPosition().y) * 180 / a));
		Sequence* gseq4 = Sequence::create(gmove4, DelayTime::create(1.0f), CallFunc::create([=]() {
			removeChild(ground4);
		}), NULL);
		ground4->runAction(gseq4);
		auto ground5 = Sprite::create("ground.png");
		groundVector.pushBack(ground5);
		this->addChild(ground5);
		ground5->setPosition(Vec2(playerbody->getPosition().x, playerbody->getPosition().y - 10));
		MoveBy* gmove5 = MoveBy::create(0, Vec2((po.x - arrow->getPosition().x) * 150 / a, (po.y - arrow->getPosition().y) * 150 / a));
		Sequence* gseq5 = Sequence::create(gmove5, DelayTime::create(1.0f), CallFunc::create([=]() {
			removeChild(ground5);
		}), NULL);
		ground5->runAction(gseq5);
		auto ground6 = Sprite::create("ground.png");
		groundVector.pushBack(ground6);
		this->addChild(ground6);
		ground6->setPosition(Vec2(playerbody->getPosition().x, playerbody->getPosition().y - 10));
		MoveBy* gmove6 = MoveBy::create(0, Vec2((po.x - arrow->getPosition().x) * 150 / a, (po.y - arrow->getPosition().y) * 150 / a));
		Sequence* gseq6 = Sequence::create(gmove6, DelayTime::create(1.0f), CallFunc::create([=]() {
			removeChild(ground6);
		}), NULL);
		ground6->runAction(gseq6);
		auto ground7 = Sprite::create("ground.png");
		groundVector.pushBack(ground7);
		this->addChild(ground7);
		ground7->setPosition(Vec2(playerbody->getPosition().x, playerbody->getPosition().y - 10));
		MoveBy* gmove7 = MoveBy::create(0, Vec2((po.x - arrow->getPosition().x) * 120 / a, (po.y - arrow->getPosition().y) * 120 / a));
		Sequence* gseq7 = Sequence::create(gmove7, DelayTime::create(1.0f), CallFunc::create([=]() {
			removeChild(ground7);
		}), NULL);
		ground7->runAction(gseq7);
		auto ground8 = Sprite::create("ground.png");
		groundVector.pushBack(ground8);
		this->addChild(ground8);
		ground8->setPosition(Vec2(playerbody->getPosition().x, playerbody->getPosition().y - 10));
		MoveBy* gmove8 = MoveBy::create(0, Vec2((po.x - arrow->getPosition().x) * 90 / a, (po.y - arrow->getPosition().y) * 90 / a));
		Sequence* gseq8 = Sequence::create(gmove8, DelayTime::create(1.0f), CallFunc::create([=]() {
			removeChild(ground8);
		}), NULL);
		ground8->runAction(gseq8);
		auto ground9 = Sprite::create("ground.png");
		groundVector.pushBack(ground9);
		this->addChild(ground9);
		ground9->setPosition(Vec2(playerbody->getPosition().x, playerbody->getPosition().y - 10));
		MoveBy* gmove9 = MoveBy::create(0, Vec2((po.x - arrow->getPosition().x) * 60 / a, (po.y - arrow->getPosition().y) * 60 / a));
		Sequence* gseq9 = Sequence::create(gmove9, DelayTime::create(1.0f), CallFunc::create([=]() {
			removeChild(ground9);
		}), NULL);
		ground9->runAction(gseq9);
		auto ground10 = Sprite::create("ground.png");
		groundVector.pushBack(ground10);
		this->addChild(ground10);
		ground10->setPosition(Vec2(playerbody->getPosition().x, playerbody->getPosition().y - 10));
		MoveBy* gmove10 = MoveBy::create(0, Vec2((po.x - arrow->getPosition().x) * 30 / a, (po.y - arrow->getPosition().y) * 30 / a));
		Sequence* gseq10 = Sequence::create(gmove10, DelayTime::create(1.0f), CallFunc::create([=]() {
			removeChild(ground10);
		}), NULL);
		ground10->runAction(gseq10);
	}
	return true;

}


bool player::onTouchBegan(Touch* touch, Event* event)
{
	if (!shooting) {
		shootskill(touch,  event);
		shooting = true;
		auto winsize = Director::getInstance()->getWinSize();
		
		auto sequence = Sequence::create(DelayTime::create(1.0f), CallFunc::create([=]() {
			shooting = false;
		}), NULL);
		runAction(sequence);
		return true;
	}
}



void player::deleteStart()
{
	dynamic_cast<Sprite *>(this->getChildByTag(1001))
		->removeFromParentAndCleanup(true);
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
	ifhurt(delta);
	//Node::update(delta);
	auto leftArrow = EventKeyboard::KeyCode::KEY_LEFT_ARROW, rightArrow = EventKeyboard::KeyCode::KEY_RIGHT_ARROW;
	auto upArrow = EventKeyboard::KeyCode::KEY_UP_ARROW, downArrow = EventKeyboard::KeyCode::KEY_DOWN_ARROW;
	if (isKeyPressed(leftArrow)) {
		keyPressedDuration(leftArrow);
		animationcreate(6);
	}
	else if (isKeyPressed(rightArrow)) {
		keyPressedDuration(rightArrow);
		animationcreate(11);
	}
	else if (isKeyPressed(upArrow)) {
		keyPressedDuration(upArrow);
		animationcreate(16);
	}
	else if (isKeyPressed(downArrow)) {
		keyPressedDuration(downArrow);
		animationcreate(1);
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


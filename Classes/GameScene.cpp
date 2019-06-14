#include "GameScene.h"
#include "cocos2d.h"
#include "ui/UIButton.h"
#define SKILL_SUM 14

USING_NS_CC;
using namespace ui;
Vector<Sprite*>GameScene::hpVector;
Vector<Sprite*>GameScene::expVector;

Scene* GameScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameScene::create();
	layer->setName("GameScene");
	scene->addChild(layer);
	return scene;
}

bool GameScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	

	srand((unsigned)time(NULL));
	setMap(randomMap());
	setSprite();
	setViewPointCenter(_player1->getPosition());
	setTouchController();

	/*Button* button = Button::create("bar.png");
	_tilemap->addChild(button, 3);
	button->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type) {
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			_player1->expRaise(_player1->EXP_RAISE);
			_tilemap->removeChild(button);
			break;
		case ui::Widget::TouchEventType::ENDED:
			break;
		default:
			break;
		}
	});*/



	
	EventListenerKeyboard* keyboard = EventListenerKeyboard::create();
	keyboard->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event)
	{
		keys[keyCode] = true;
	};
	keyboard->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event) {
		keys[keyCode] = false;
		//_player1->pauseSchedulerAndActions();

	};

	getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboard, this);

	scheduleUpdate();
	return true;
}


std::string GameScene::randomMap()
{
    int mapSum = 2;
	std::vector<std::string> mapList;
	mapList.resize(mapSum);
	mapList = { "map1","map2" };
	int randomMapNum = rand() % mapSum;
	while (randomMapNum == mapSum)
		randomMapNum = rand() % mapSum;
	mapName = mapList[randomMapNum];
	return mapList[randomMapNum];
}

void GameScene::setViewPointCenter(Vec2 position)
{
	int x = MAX(position.x, _screenWidth / 2);
	int y = MAX(position.y, _screenHeight / 2);
	x = MIN(x, (getMapSize().width *getTileSize().width - _screenWidth / 2));
	y = MIN(y, (getMapSize().height *getTileSize().height - _screenHeight / 2));
	Point actualPosition = Point(position.x, position.y);
	Point centerOfView = Point(_screenWidth / 2, _screenHeight / 2);
	Point viewPoint = centerOfView - actualPosition;
	this->setPosition(viewPoint);
}

Vec2 GameScene::tileCoordForPosition(Vec2 position)
{
	float x = (int)(position.x / (getTileSize().width / CC_CONTENT_SCALE_FACTOR()));
	float pointHeight = getTileSize().height / CC_CONTENT_SCALE_FACTOR();
	float y = (int)((getMapSize().height*pointHeight - position.y) / pointHeight);
	return Vec2(x, y);
}

Vec2 GameScene::positionForTileCoord(Vec2 coord)
{
	int x = coord.x * _tilemap->getTileSize().width + _tilemap->getTileSize().width / 2;
	int y = (_tilemap->getMapSize().height - coord.y) * _tilemap->getTileSize().height - _tilemap->getTileSize().height / 2;
	return Vec2(x, y);
}

void GameScene::setMap(const std::string& mapName)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	_screenWidth = visibleSize.width;
	_screenHeight = visibleSize.height;
	std::string _mapName = mapName + ".tmx";
	_tilemap = TMXTiledMap::create(_mapName);
	_tilemap->setAnchorPoint(Vec2::ZERO);
	_tilemap->setPosition(Vec2::ZERO);
	//_tilemap->setScale(0.6);
	this->addChild(_tilemap, -1);

	emptyTile = getMapSize().width*getMapSize().height;

	_collidable = _tilemap->getLayer("wall");
	/*_spawnpoint = _tilemap->getLayer("spawnpoint");
	_spawnpoint->setVisible(false);*/

	mapInfo.resize(getMapSize().width);
	propInfo.resize(getMapSize().width);
	spawnPoint.resize(getMapSize().width);

	for (int i = 0; i < getMapSize().width; i++)
	{
		mapInfo.at(i).resize(getMapSize().height);
		propInfo.at(i).resize(getMapSize().height);
		spawnPoint.at(i).resize(getMapSize().height);
	}

	for (int i = 0; i < getMapSize().width; i++)
	{
		for (int j = 0; j < getMapSize().height; j++)
		{
			int tileGid1 = _collidable->getTileGIDAt(Vec2(i, j));
			//int tileGid2 = _spawnpoint->getTileGIDAt(Vec2(i, j));
			if (tileGid1)
			{
				Value properties = _tilemap->getPropertiesForGID(tileGid1);
				ValueMap map = properties.asValueMap();
				std::string value = map.at("collidable").asString();
				if (value.compare("true") == 0)
				{
					mapInfo.at(i).at(j) = 1;  //记录地图上不可通过的瓦片
					emptyTile--;
				}
			}
			/*if (tileGid2)
			{
				Value properties = _tilemap->getPropertiesForGID(tileGid2);
				ValueMap map = properties.asValueMap();
				std::string value = map.at("spawnpoint").asString();
				if (value.compare("true") == 0)
				{
					spawnPoint.at(i).at(j) = 1;  //记录地图上的复活点
				}
			}*/
		}
	}
}

void GameScene::setSprite()
{
	int pos_x, pos_y;
	_player1 = PlayerBase::create("player1.png");
	emptyHpBar = Sprite::create("bar.png");
	_tilemap->addChild(_player1, 2);
	_player1->addChild(emptyHpBar, 4);

	_player1->setPosition(spawn());
	emptyHpBar->setPosition(Vec2(_player1->getContentSize().width / 2, _player1->getContentSize().height + 30));

	hpBar = Sprite::create("blood.png");
	emptyHpBar->addChild(hpBar, 4);
	hpBar->setAnchorPoint(Vec2(0, 0));
	hpBar->setPosition(Vec2(0, 1));
	hpBar->setScaleX(0);

	_newposition = _player1->getPosition();

	emptyExpBar = Sprite::create("bar.png");
	_player1->addChild(emptyExpBar, 3);
	emptyExpBar->setPosition(Vec2(_player1->getContentSize().width / 2,_screenHeight/2-20));
	emptyExpBar->setScaleX(4);
	emptyExpBar->setScaleY(2);
	emptyExpBar->setOpacity(150);

	expBar = Sprite::create("exp.png");
	emptyExpBar->addChild(expBar, 3);
	expBar->setAnchorPoint(Vec2(0, 0));
	expBar->setPosition(Vec2(0, 1));
	expBar->setScaleX(0);

	auto content = "Lv" + std::to_string(_player1->level);
	level_text = Text::create(content, "fonts/ConcertOne-Regular.ttf", 50);
	_player1->addChild(level_text, 3);
	level_text->setColor(Color3B::YELLOW);
	level_text->setPosition(Vec2(-emptyExpBar->getContentSize().width-200, _screenHeight / 2 - 10));

	std::string context = "restart after" + std::to_string(_downtime) + "s";
	Text* restartInfo = Text::create(context, "fonts/ConcertOne-Regular.ttf", 36);
	addChild(restartInfo, 6);
	restartInfo->setColor(Color3B::WHITE);
	restartInfo->setPosition(Vec2(0, -_screenHeight / 2));
}

void GameScene::setTouchController()
{
	_layer1 = Layer::create();
	addChild(_layer1, 0);
	_layer2 = Layer::create();
	addChild(_layer2, 1);

	auto _listener1 = EventListenerTouchOneByOne::create();
	_listener1->onTouchBegan = [this](Touch *touch, Event *event) 
	{
		if (!_player1->shooting) {
			_player1->shooting = true;
			auto winsize = Director::getInstance()->getWinSize();
			arrow = Sprite::create("arrow.png");
			arrowVector.pushBack(arrow);
			Point po = touch->getLocation();
			auto playersize = arrow->getContentSize();
			_tilemap->addChild(arrow, 2);
			arrow->setPosition(Vec2(_player1->getPosition().x, _player1->getPosition().y - 10));
			Point shootVector = touch->getLocation() - Vec2(_screenWidth / 2, _screenHeight / 2);
			Point normalizedVector = ccpNormalize(shootVector);
			float radians = atan2(normalizedVector.y, -normalizedVector.x);
			float degree = CC_RADIANS_TO_DEGREES(radians);
			RotateTo* rot = RotateTo::create(0, degree + 180);
			MoveBy* move1 = MoveBy::create(0, Vec2(5 * sin(degree - 90), 5 * cos(degree - 90)));
			//arrow->runAction(MoveTo::create(1.0f,Vec2(po.x - winsize.width / 2+60, po.y - playersize.height*1.1-10)));
			float a;
			a = sqrt((po.x - _screenWidth / 2)*(po.x - _screenWidth / 2) + (po.y - _screenHeight / 2)*(po.y - _screenHeight / 2));
			MoveBy* move = MoveBy::create(0.3f, Vec2((po.x - _screenWidth / 2) * _player1->RANGE / a, (po.y - _screenHeight / 2) * _player1->RANGE / a));
			Sequence* seq = Sequence::create(rot, move1, move, CallFunc::create([=]() {
				arrowVector.eraseObject(arrow);
				_tilemap->removeChild(arrow);
			}), NULL);
			arrow->runAction(seq);
			if (_player1->arrow_ahead==2)
			{
				auto arrow1 = Sprite::create("arrow.png");
				arrowVector.pushBack(arrow1);
				this->addChild(arrow1);
				MoveBy* move1 = MoveBy::create(0, Vec2(-5 * sin(degree - 90), -5 * cos(degree - 90)));
				arrow1->setPosition(Vec2(_player1->getPosition().x, _player1->getPosition().y - 15));
				Sequence* seq1 = Sequence::create(rot->clone(), move1, move->clone(), CallFunc::create([=]() {
					removeChild(arrow1);
				}), NULL);
				arrow1->runAction(seq1);
			}
			if (_player1->arrow_ahead==3)
			{
				auto arrow3 = Sprite::create("arrow.png");
				Point po1 = Vec2(po.x + 100 * (po.y - _screenHeight/2) / a, po.y - 100 * (po.x - _screenWidth/2) / a);
				float a1;
				a1 = sqrt((po1.x - _screenWidth/2)*(po1.x - _screenWidth/2) + (po1.y - _screenHeight/2)*(po1.y - _screenHeight/2));
				arrowVector.pushBack(arrow3);
				this->addChild(arrow3);
				RotateTo* rot1 = RotateTo::create(0, degree + 210);
				arrow3->setPosition(Vec2(_player1->getPosition().x, _player1->getPosition().y - 15));
				MoveBy* move10 = MoveBy::create(0.3f, Vec2((po1.x - _screenWidth/2) * _player1->RANGE / a1, (po1.y - _screenHeight/2) * _player1->RANGE / a1));
				Sequence* seq3 = Sequence::create(rot1, move10, CallFunc::create([=]() {
					removeChild(arrow3);
				}), NULL);
				arrow3->runAction(seq3);
				auto arrow2 = Sprite::create("arrow.png");
				Point po2 = Vec2(po.x - 100 * (po.y - _screenHeight/2) / a, po.y + 100 * (po.x - _screenWidth/2) / a);
				float a2;
				a2 = sqrt((po2.x - _screenWidth/2)*(po2.x - _screenWidth/2) + (po2.y - _screenHeight/2)*(po2.y - _screenHeight/2));
				arrowVector.pushBack(arrow2);
				this->addChild(arrow2);
				RotateTo* rot2 = RotateTo::create(0, degree + 150);
				arrow2->setPosition(Vec2(_player1->getPosition().x, _player1->getPosition().y - 15));
				MoveBy* move11 = MoveBy::create(0.3f, Vec2((po2.x -_screenWidth/2) * _player1->RANGE / a2, (po2.y - _screenHeight/2) * _player1->RANGE / a2));
				Sequence* seq2 = Sequence::create(rot2, move11, CallFunc::create([=]() {
					removeChild(arrow2);
				}), NULL);
				arrow2->runAction(seq2);

			}
			if (_player1->arrow_back)
			{
				auto arrow4 = Sprite::create("arrow.png");
				arrowVector.pushBack(arrow4);
				this->addChild(arrow4);
				arrow4->setPosition(Vec2(_player1->getPosition().x, _player1->getPosition().y - 10));
				RotateTo* rot4 = RotateTo::create(0, degree);
				MoveBy* move4 = MoveBy::create(0.3f, Vec2(-(po.x -_screenWidth/2) * _player1->RANGE / a, -(po.y - _screenHeight/2) * _player1->RANGE / a));
				Sequence* seq4 = Sequence::create(rot4, move4, CallFunc::create([=]() {
					removeChild(arrow4);
				}), NULL);
				arrow4->runAction(seq4);
			}
			/*if (houer)
			{
				auto arrow5 = Sprite::create("arrow.png");
				arrowVector.pushBack(arrow5);
				this->addChild(arrow5);
				arrow5->setPosition(Vec2(_player1->getPosition().x, _player1->getPosition().y - 10));
				Sequence* seq5 = Sequence::create(rot->clone(), DelayTime::create(0.2f), move->clone(), CallFunc::create([=]() {
					removeChild(arrow5);
				}), NULL);
				arrow5->runAction(seq5);
			}*/
			if (_player1->arrow_left_right)
			{
				auto arrow6 = Sprite::create("arrow.png");
				arrowVector.pushBack(arrow6);
				this->addChild(arrow6);
				arrow6->setPosition(Vec2(_player1->getPosition().x, _player1->getPosition().y - 10));
				RotateTo* rot6 = RotateTo::create(0, degree + 90);
				MoveBy* move6 = MoveBy::create(0.3f, Vec2(-(po.y - _screenHeight/2) * _player1->RANGE / a, (po.x - _screenWidth/2) * _player1->RANGE / a));
				Sequence* seq6 = Sequence::create(rot6, move6, CallFunc::create([=]() {
					removeChild(arrow6);
				}), NULL);
				arrow6->runAction(seq6);
				auto arrow7 = Sprite::create("arrow.png");
				arrowVector.pushBack(arrow7);
				this->addChild(arrow7);
				arrow7->setPosition(Vec2(_player1->getPosition().x, _player1->getPosition().y - 10));
				RotateTo* rot7 = RotateTo::create(0, degree + 270);
				MoveBy* move7 = MoveBy::create(0.3f, Vec2((po.y - _screenHeight/2) * _player1->RANGE / a, -(po.x - _screenWidth/2) * _player1->RANGE / a));
				Sequence* seq7 = Sequence::create(rot7, move7, CallFunc::create([=]() {
					removeChild(arrow7);
				}), NULL);
				arrow7->runAction(seq7);
			}
			/*if (_player1->arrow)
			{
				auto arrow8 = Sprite::create("arrow.png");
				arrowVector.pushBack(arrow8);
				this->addChild(arrow8);
				MoveBy* move8 = MoveBy::create(0, Vec2(-5 * sin(degree - 90), -5 * cos(degree - 90)));
				arrow8->setPosition(Vec2(_player1->getPosition().x, _player1->getPosition().y - 15));
				Sequence* seq8 = Sequence::create(rot->clone(), move8, move->clone(), CallFunc::create([=]() {
					removeChild(arrow8);
				}), NULL);
				arrow8->runAction(seq8);
				auto arrow9 = Sprite::create("arrow.png");
				arrowVector.pushBack(arrow9);
				this->addChild(arrow9);
				arrow9->setPosition(Vec2(_player1->getPosition().x, _player1->getPosition().y - 10));
				RotateTo* rot9 = RotateTo::create(0, degree);
				MoveBy* move9 = MoveBy::create(1.0f, Vec2(-(po.x - arrow->getPosition().x) * 300 / a, -(po.y - arrow->getPosition().y) * 300 / a));
				Sequence* seq9 = Sequence::create(rot9, move9, CallFunc::create([=]() {
					removeChild(arrow9);
				}), NULL);
				arrow9->runAction(seq9);
			}*/
			/*if (sangen + houshe)
			{
				auto arrow10 = Sprite::create("arrow.png");
				arrowVector.pushBack(arrow10);
				this->addChild(arrow10);
				MoveBy* move10 = MoveBy::create(0, Vec2(-5 * sin(degree - 90), -5 * cos(degree - 90)));
				arrow10->setPosition(Vec2(_player1->getPosition().x, _player1->getPosition().y - 5));
				Sequence* seq10 = Sequence::create(rot->clone(), move10, move->clone(), CallFunc::create([=]() {
					removeChild(arrow10);
				}), NULL);
				arrow10->runAction(seq10);
				auto arrow11 = Sprite::create("arrow.png");
				arrowVector.pushBack(arrow11);
				this->addChild(arrow11);
				arrow11->setPosition(Vec2(_player1->getPosition().x, _player1->getPosition().y - 15));
				Sequence* seq11 = Sequence::create(rot->clone(), move->clone(), CallFunc::create([=]() {
					removeChild(arrow11);
				}), NULL);
				arrow11->runAction(seq11);
				auto arrow12 = Sprite::create("arrow.png");
				arrowVector.pushBack(arrow12);
				this->addChild(arrow12);
				arrow12->setPosition(Vec2(_player1->getPosition().x, _player1->getPosition().y - 10));
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
				arrow13->setPosition(Vec2(_player1->getPosition().x, _player1->getPosition().y - 15));
				Sequence* seq13 = Sequence::create(rot->clone(), move13, move->clone(), CallFunc::create([=]() {
					removeChild(arrow13);
				}), NULL);
				arrow13->runAction(seq13);
				auto arrow14 = Sprite::create("arrow.png");
				arrowVector.pushBack(arrow14);
				this->addChild(arrow14);
				arrow14->setPosition(Vec2(_player1->getPosition().x, _player1->getPosition().y - 10));
				RotateTo* rot14 = RotateTo::create(0, degree + 90);
				MoveBy* move14 = MoveBy::create(1.0f, Vec2(-(po.y - arrow->getPosition().y) * 300 / a, (po.x - arrow->getPosition().x) * 300 / a));
				Sequence* seq14 = Sequence::create(rot14, move14, CallFunc::create([=]() {
					removeChild(arrow14);
				}), NULL);
				arrow14->runAction(seq14);
				auto arrow15 = Sprite::create("arrow.png");
				arrowVector.pushBack(arrow15);
				this->addChild(arrow15);
				arrow15->setPosition(Vec2(_player1->getPosition().x, _player1->getPosition().y - 10));
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
				arrow16->setPosition(Vec2(_player1->getPosition().x, _player1->getPosition().y - 5));
				Sequence* seq16 = Sequence::create(rot->clone(), move16, move->clone(), CallFunc::create([=]() {
					removeChild(arrow16);
				}), NULL);
				arrow16->runAction(seq16);
				auto arrow17 = Sprite::create("arrow.png");
				arrowVector.pushBack(arrow17);
				this->addChild(arrow17);
				arrow17->setPosition(Vec2(_player1->getPosition().x, _player1->getPosition().y - 15));
				Sequence* seq17 = Sequence::create(rot->clone(), move->clone(), CallFunc::create([=]() {
					removeChild(arrow17);
				}), NULL);
				arrow17->runAction(seq17);
				auto arrow18 = Sprite::create("arrow.png");
				arrowVector.pushBack(arrow18);
				this->addChild(arrow18);
				arrow18->setPosition(Vec2(_player1->getPosition().x, _player1->getPosition().y - 10));
				RotateTo* rot18 = RotateTo::create(0, degree + 90);
				MoveBy* move18 = MoveBy::create(1.0f, Vec2(-(po.y - arrow->getPosition().y) * 300 / a, (po.x - arrow->getPosition().x) * 300 / a));
				Sequence* seq18 = Sequence::create(rot18, move18, CallFunc::create([=]() {
					removeChild(arrow18);
				}), NULL);
				arrow18->runAction(seq18);
				auto arrow19 = Sprite::create("arrow.png");
				arrowVector.pushBack(arrow19);
				this->addChild(arrow19);
				arrow19->setPosition(Vec2(_player1->getPosition().x, _player1->getPosition().y - 10));
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
				arrow20->setPosition(Vec2(_player1->getPosition().x, _player1->getPosition().y - 15));
				Sequence* seq20 = Sequence::create(rot->clone(), move20, move->clone(), CallFunc::create([=]() {
					removeChild(arrow20);
				}), NULL);
				arrow20->runAction(seq20);
				auto arrow21 = Sprite::create("arrow.png");
				arrowVector.pushBack(arrow21);
				this->addChild(arrow21);
				arrow21->setPosition(Vec2(_player1->getPosition().x, _player1->getPosition().y - 10));
				RotateTo* rot21 = RotateTo::create(0, degree + 90);
				MoveBy* move21 = MoveBy::create(1.0f, Vec2(-(po.y - arrow->getPosition().y) * 300 / a, (po.x - arrow->getPosition().x) * 300 / a));
				Sequence* seq21 = Sequence::create(rot21, move21, CallFunc::create([=]() {
					removeChild(arrow21);
				}), NULL);
				arrow21->runAction(seq21);
				auto arrow22 = Sprite::create("arrow.png");
				arrowVector.pushBack(arrow22);
				this->addChild(arrow22);
				arrow22->setPosition(Vec2(_player1->getPosition().x, _player1->getPosition().y - 10));
				RotateTo* rot22 = RotateTo::create(0, degree + 270);
				MoveBy* move22 = MoveBy::create(1.0f, Vec2((po.y - arrow->getPosition().y) * 300 / a, -(po.x - arrow->getPosition().x) * 300 / a));
				Sequence* seq22 = Sequence::create(rot22, move22, CallFunc::create([=]() {
					removeChild(arrow22);
				}), NULL);
				arrow22->runAction(seq22);
				auto arrow23 = Sprite::create("arrow.png");
				arrowVector.pushBack(arrow23);
				this->addChild(arrow23);
				arrow23->setPosition(Vec2(_player1->getPosition().x, _player1->getPosition().y - 10));
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
				arrow24->setPosition(Vec2(_player1->getPosition().x, _player1->getPosition().y - 5));
				Sequence* seq24 = Sequence::create(rot->clone(), move24, move->clone(), CallFunc::create([=]() {
					removeChild(arrow24);
				}), NULL);
				arrow24->runAction(seq24);
				auto arrow25 = Sprite::create("arrow.png");
				arrowVector.pushBack(arrow25);
				this->addChild(arrow25);
				arrow25->setPosition(Vec2(_player1->getPosition().x, _player1->getPosition().y - 15));
				Sequence* seq25 = Sequence::create(rot->clone(), move->clone(), CallFunc::create([=]() {
					removeChild(arrow25);
				}), NULL);
				arrow25->runAction(seq25);
				auto arrow26 = Sprite::create("arrow.png");
				arrowVector.pushBack(arrow26);
				this->addChild(arrow26);
				arrow26->setPosition(Vec2(_player1->getPosition().x, _player1->getPosition().y - 10));
				RotateTo* rot26 = RotateTo::create(0, degree + 90);
				MoveBy* move26 = MoveBy::create(1.0f, Vec2(-(po.y - arrow->getPosition().y) * 300 / a, (po.x - arrow->getPosition().x) * 300 / a));
				Sequence* seq26 = Sequence::create(rot26, move26, CallFunc::create([=]() {
					removeChild(arrow26);
				}), NULL);
				arrow26->runAction(seq26);
				auto arrow27 = Sprite::create("arrow.png");
				arrowVector.pushBack(arrow27);
				this->addChild(arrow27);
				arrow27->setPosition(Vec2(_player1->getPosition().x, _player1->getPosition().y - 10));
				RotateTo* rot27 = RotateTo::create(0, degree + 270);
				MoveBy* move27 = MoveBy::create(1.0f, Vec2((po.y - arrow->getPosition().y) * 300 / a, -(po.x - arrow->getPosition().x) * 300 / a));
				Sequence* seq27 = Sequence::create(rot27, move27, CallFunc::create([=]() {
					removeChild(arrow27);
				}), NULL);
				arrow27->runAction(seq27);
				auto arrow28 = Sprite::create("arrow.png");
				arrowVector.pushBack(arrow28);
				this->addChild(arrow28);
				arrow28->setPosition(Vec2(_player1->getPosition().x, _player1->getPosition().y - 10));
				RotateTo* rot28 = RotateTo::create(0, degree);
				MoveBy* move28 = MoveBy::create(1.0f, Vec2(-(po.x - arrow->getPosition().x) * 300 / a, -(po.y - arrow->getPosition().y) * 300 / a));
				Sequence* seq28 = Sequence::create(rot28, move28, CallFunc::create([=]() {
					removeChild(arrow28);
				}), NULL);
				arrow28->runAction(seq28);
			}*/

			auto sequence = Sequence::create(DelayTime::create(1.0f), CallFunc::create([=]() {
				_player1->shooting = false;
			}), NULL);
			runAction(sequence);
			return true;
		}
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_listener1, _layer1);
}

bool GameScene::isCanPass(Vec2 &coord)
{
	if (coord.x >= 0 && coord.x < getMapSize().width
		&&coord.y >= 0 && coord.y < getMapSize().height
		&&mapInfo.at(coord.x).at(coord.y) != 1)
		return true;
	else return false;
}

bool GameScene::isCanUpdate(Vec2 &coord)
{
	if (coord.x >= 0 && coord.x < getMapSize().width
		&&coord.y >= 0 && coord.y < getMapSize().height
		&&propInfo.at(coord.x).at(coord.y) == 0
		&& mapInfo.at(coord.x).at(coord.y) == 0)
		return true;
	else return false;
}

Size GameScene::getMapSize()
{
	return _tilemap->getMapSize();
}

Size GameScene::getTileSize()
{
	return _tilemap->getTileSize();
}


Vec2 GameScene::randomPosition()
{
	int randX = rand() %
		(int)(getMapSize().width)*getTileSize().width + 0.5*getTileSize().width;
	int randY = rand() %
		(int)(getMapSize().height)*getTileSize().height + 0.5*getTileSize().height;
	Vec2 position = Point(randX, randY);
	Vec2 tileCoord = tileCoordForPosition(position);
	if (!isCanUpdate(tileCoord))
		return randomPosition();
	return position;
}

void GameScene::updateEXPSprite(float delta)
{
	if (_count % 5 == 0 && _count % 15 != 0 && emptyTile - 1 > propSum)
	{
		Sprite *expSprite = Sprite::create("smallcrystal.png");
		Vec2 position = randomPosition();
		expSprite->setPosition(position);
		expSprite->setScale(1.2);
		Vec2 propCoord = tileCoordForPosition(position);
		propInfo.at(propCoord.x).at(propCoord.y) = 1;
		_tilemap->addChild(expSprite, 1);
		expVector.pushBack(expSprite);
		propSum++;
	}
}

void GameScene::judgeEXPSprite(float delta)
{
	for (int i = 0; i < expVector.size(); i++)
	{
		Sprite* expSprite = expVector.at(i);
		if (expSprite->getBoundingBox().intersectsRect(_player1->getBoundingBox())&&_player1->collect)
		{
			//添加拾取exp道具后人物属性的变化
			if (_player1->expRaise(_player1->EXP_RAISE))
			{
				setSkillButton();
				_player1->removeChild(level_text, true);
				auto content = "Lv" + std::to_string(_player1->level);
				level_text = Text::create(content, "fonts/ConcertOne-Regular.ttf", 50);
				_player1->addChild(level_text, 3);
				level_text->setColor(Color3B::YELLOW);
				level_text->setPosition(Vec2(-emptyExpBar->getContentSize().width - 200, _screenHeight / 2 - 10));
			}

			Vec2 coord = tileCoordForPosition(expSprite->getPosition());
			propInfo.at(coord.x).at(coord.y) = 0;
			_tilemap->removeChild(expSprite);
			expVector.eraseObject(expSprite);
			propSum--;
		}
	}
}

void GameScene::updateHPSprite(float delta)
{
	if (_count % 15 == 0 && emptyTile - 1 > propSum)
	{
		Sprite *hpSprite = Sprite::create("heart.png");
		Vec2 position = randomPosition();
		hpSprite->setPosition(position);
		hpSprite->setScale(0.8);
		Vec2 propCoord = tileCoordForPosition(position);
		propInfo.at(propCoord.x).at(propCoord.y) = 2;
		_tilemap->addChild(hpSprite, 1);
		hpVector.pushBack(hpSprite);
		propSum++;
	}
}

void GameScene::judgeHPSprite(float delta)
{
	for (int i=0;i<hpVector.size();i++)
	{
		Sprite *hpSprite = hpVector.at(i);
		if (hpSprite->getBoundingBox().intersectsRect(_player1->getBoundingBox())&&_player1->collect)
		{
			//添加拾取hp道具后人物属性的变化
			
			if (_player1->hp <= _player1->hpLimit)
			{
				_player1->hpRaise(-(_player1->HP_RAISE));
			}

			Vec2 coord = tileCoordForPosition(hpSprite->getPosition());
			propInfo.at(coord.x).at(coord.y) = 0;
			_tilemap->removeChild(hpSprite);
			hpVector.eraseObject(hpSprite);
			propSum--;
		}
	}
}

Vec2 GameScene::spawn()  //返回复活点坐标
{
	TMXObjectGroup* group = _tilemap->getObjectGroup("player");
	ValueMap spawnpoint = group->getObject("blue_player");
	Vec2 position;
	position.x = spawnpoint["x"].asFloat();
	position.y = spawnpoint["y"].asFloat();
	return position;
}

void GameScene::barListener(float delta)
{
	hpBar->setScaleX(_player1->hp / _player1->hpLimit);
	expBar->setScaleX(_player1->exp / _player1->expNeed);
	emptyHpBar->setScaleX(_player1->hpLimit / _player1->_hp);
}

void GameScene::deathListener(float delta)
{
	if (_player1->hp <= 0 && !running1)
	{
		running1 = true;
		_player1->hp = 0;
		_player1->die();
		dieScene = Sprite::create("diescene.jpg");
		_player1->addChild(dieScene,5);
		dieScene->setPosition(_player1->getContentSize().width / 2, 0);
		dieScene->setScale(2.3);
		dieScene->setOpacity(210);

		_downtime = _player1->restartTime-1;
		schedule(schedule_selector(GameScene::restartCount),1);

		auto sequence = Sequence::create(DelayTime::create(_player1->restartTime), CallFunc::create([=]() {
			_player1->restart(spawn());
			_player1->removeChild(dieScene, true);
			setViewPointCenter(_player1->getPosition());
			_newposition = _player1->getPosition();
			running1 = false;
		}), NULL);
		runAction(sequence);
	}
}

void GameScene::restartCount(float delta)
{
	std::string context = "restart after" + std::to_string(_downtime) + "s";
	Text* restartInfo = Text::create(context, "fonts/ConcertOne-Regular.ttf", 50);
	_player1->addChild(restartInfo, 6);
	restartInfo->setColor(Color3B::WHITE);
	restartInfo->setPosition(Vec2(_player1->getContentSize().width/2, _screenHeight / 2));
	_downtime--;
	auto sequence = Sequence::create(DelayTime::create(1), CallFunc::create([=]() {
		_player1->removeChild(restartInfo,true);
	}), NULL);
	runAction(sequence);
	log("1");
}

void GameScene::animationcreate(int direct)
{
	if (!_player1->moving) {
		_player1->moving = true;
		SpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("animate.plist");
		Animation* animation = Animation::create();
		char name[100];
		for (int i = direct; i < direct + 5; i++)
		{
			memset(name, 0, sizeof(name));
			sprintf(name, "player1_%d.png", i);
			animation->addSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name));
		}
		animation->setDelayPerUnit(0.05f);
		Animate* animate = Animate::create(animation);
		_player1->runAction(animate);
		auto sequence = Sequence::create(DelayTime::create(0.25f), CallFunc::create([=]() {
			_player1->moving = false;
		}), NULL);
		runAction(sequence);
	}
}

/*void GameScene::ifhurt(float delta)
{
	for (int i = 0; i < arrowVector.size(); i++) {
		Sprite* arrow = arrowVector.at(i);
		if (arrow->getBoundingBox().intersectsRect(monster->getBoundingBox()))
		{
			//以下加受伤后引起的属性变化


			removeChild(arrow);
		}
	}
}*/



void GameScene::update(float delta)
{
	_count++;
	updateEXPSprite(delta);
	judgeEXPSprite(delta);
	updateHPSprite(delta);
	judgeHPSprite(delta);
	barListener(delta);
	deathListener(delta);
	if(_downtime==0) unschedule(schedule_selector(GameScene::restartCount));
	Node::update(delta);
	auto leftArrow = EventKeyboard::KeyCode::KEY_LEFT_ARROW, rightArrow = EventKeyboard::KeyCode::KEY_RIGHT_ARROW;
	auto upArrow = EventKeyboard::KeyCode::KEY_UP_ARROW, downArrow = EventKeyboard::KeyCode::KEY_DOWN_ARROW;
	if (_player1->move) {
		if (isKeyPressed(leftArrow) && isCanPass(Vec2(tileCoordForPosition(_player1->getPosition()).x, tileCoordForPosition(_player1->getPosition()).y)))
		{
			keyPressedDuration(leftArrow);
			animationcreate(6);
		}
		else if (isKeyPressed(rightArrow) && isCanPass(Vec2(tileCoordForPosition(_player1->getPosition()).x + 2.9, tileCoordForPosition(_player1->getPosition()).y)))
		{
			keyPressedDuration(rightArrow);
			animationcreate(11);
		}
		else if (isKeyPressed(upArrow) && isCanPass(Vec2(tileCoordForPosition(_player1->getPosition()).x, tileCoordForPosition(_player1->getPosition()).y - 0.3)))
		{
			keyPressedDuration(upArrow);
			animationcreate(16);
		}
		else if (isKeyPressed(downArrow) && isCanPass(Vec2(tileCoordForPosition(_player1->getPosition()).x, tileCoordForPosition(_player1->getPosition()).y)))
		{
			keyPressedDuration(downArrow);
			animationcreate(1);
		}
	}
}

void GameScene::keyPressedDuration(EventKeyboard::KeyCode code) 
{
	switch (code) {
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		_newposition.x -= _player1->SPEED;
		_player1->setPosition(_newposition.x, _newposition.y);
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		_newposition.x += _player1->SPEED;
		_player1->setPosition(_newposition.x, _newposition.y);
		break;
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		_newposition.y += _player1->SPEED;
		_player1->setPosition(_newposition.x, _newposition.y);
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		_newposition.y -= _player1->SPEED;
		_player1->setPosition(_newposition.x, _newposition.y);
		break;
	default:
		break;
	}
	// 0.3s代表着动作从开始到结束所用的时间，从而显得不会那么机械。
	//auto moveTo = MoveTo::create(0, Vec2(_player1->getPositionX() + offsetX, _player1->getPositionY() + offsetY));
	//_player1->runAction(moveTo);
	setViewPointCenter(Vec2(_newposition.x, _newposition.y));
}

bool GameScene::isKeyPressed(EventKeyboard::KeyCode keyCode) {
	if (keys[keyCode]) {
		return true;
	}
	else {
		return false;
	}
}

/*bool GameScene::canMove(Vec2& coord)
{
	int tileGid1 = _collidable->getTileGIDAt(coord);
	if (tileGid1)
	{
		Value properties = _tilemap->getPropertiesForGID(tileGid1);
		ValueMap map = properties.asValueMap();
		std::string value = map.at("collidable").asString();
		if (value.compare("true") == 0)
		{
			return false;
		}
	}
	else return true;
}*/



////////////////////////////////////////////////////
//原Skill类代码
void GameScene::setSkillButton()
{
	button_1 = Button::create(skillID_1());
	_player1->addChild(button_1, 5);
	button_1->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type) {
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			break;
		case ui::Widget::TouchEventType::ENDED:
			skillChoose(skill_id_1);
			_player1->removeChild(button_1, true);
			_player1->removeChild(button_2, true);
			_player1->removeChild(button_3, true);
			break;
		default:
			break;
		}
	});

	button_1->setPosition(Vec2(-_screenWidth / 4, -_screenHeight / 3));
	button_1->setScale(0.8);

	//buttonList.pushBack(button_1);

	button_2 = Button::create(skillID_2());
	_player1->addChild(button_2, 5);
	button_2->addTouchEventListener([=](Ref*pSender, Widget::TouchEventType type)
	{
		if (type == Widget::TouchEventType::ENDED)
		{
			skillChoose(skill_id_2);
			_player1->removeChild(button_1, true);
			_player1->removeChild(button_2, true);
			_player1->removeChild(button_3, true);
		}
	});
    
	button_2->setPosition(Vec2(0, -_screenHeight / 3));
	button_2->setScale(0.8);

	//buttonList.pushBack(button_2);

	button_3 = Button::create(skillID_3());
	_player1->addChild(button_3, 5);
	button_3->addTouchEventListener([=](Ref*pSender, Widget::TouchEventType type)
	{
		if (type == Widget::TouchEventType::ENDED)
		{
			skillChoose(skill_id_3);
			_player1->removeChild(button_1, true);
			_player1->removeChild(button_2, true);
			_player1->removeChild(button_3, true);
		}
	});

    button_3->setPosition(Vec2(_screenWidth / 4, -_screenHeight / 3));
	button_3->setScale(0.8);
}

void GameScene::skillChoose(int id)
{
	switch (id)
	{
	case(1): atkUp(); break;
	case(2):rangeUp(); break;
	case(3):defUp(); break;
	case(4):hpraiseUp(); break;
	case(5):expraiseUp(); break;
	case(6):speedUp(); break;
	case(7):hpUp(); break;
	case(8):atkUphpDown(); break;
	case(9):rangeUpspeedDown(); break;
	case(10):allUp(); break;
	case(11):arrowBack(); break;
	case(12):arrowLeftRight(); break;
	case(13):arrowAhead(); break;
	}
}

std::string GameScene::skillID_1()
{
	skill_id_1 = rand() % SKILL_SUM;
	if (skill_id_1 == 0)
		return skillID_1();
	else return "_skill\\choice" + std::to_string(skill_id_1) + ".png";
}

std::string GameScene::skillID_2()
{
	skill_id_2 = rand() % SKILL_SUM;
	if (skill_id_2 == 0 || skill_id_2 == skill_id_1)
		return skillID_2();
	else return "_skill\\choice" + std::to_string(skill_id_2) + ".png";
}

std::string GameScene::skillID_3()
{
	skill_id_3 = rand() % SKILL_SUM;
	if (skill_id_3 == 0 || skill_id_3 == skill_id_2 || skill_id_3 == skill_id_1)
		return skillID_3();
	else return "_skill\\choice" + std::to_string(skill_id_3) + ".png";
}

void GameScene::atkUp()
{
	_player1->ATK += 5;
}

void GameScene::rangeUp()
{
	_player1->RANGE += 50;
}

void GameScene::defUp()
{
	_player1->DEF += 3;
}

void GameScene::hpraiseUp()
{
	_player1->HP_RAISE = 1.8;
}

void GameScene::expraiseUp()
{
	_player1->EXP_RAISE = 1.5;
}

void GameScene::speedUp()
{
	_player1->SPEED += 2;
}

void GameScene::hpUp()
{
	_player1->hp += 20;
	_player1->hpLimit += 20;
}

void GameScene::atkUphpDown()
{
	_player1->ATK += 10;
	_player1->hpLimit -= 20;
	if (_player1->hp > _player1->hpLimit)
		_player1->hp = _player1->hpLimit;
}

void GameScene::rangeUpspeedDown()
{
	_player1->RANGE += 80;
	_player1->SPEED -= 1;
}

void GameScene::allUp()
{
	_player1->ATK += 2;
	_player1->DEF += 1;
	_player1->RANGE += 30;
}

void GameScene::arrowBack()
{
	_player1->arrow_back = true;
}

void GameScene::arrowLeftRight()
{
	_player1->arrow_left_right = true;
}

void GameScene::arrowAhead()
{
	if (_player1->arrow_ahead < 3)
	{
		_player1->arrow_ahead++;
	}
}








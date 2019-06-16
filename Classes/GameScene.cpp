#include "GameScene.h"
#include "cocos2d.h"

USING_NS_CC;

Vector<Sprite*>GameScene::hpVector;
Vector<Sprite*>GameScene::expVector;

Scene* GameScene::createScene()
{
	return GameScene::create();
}

bool GameScene::init()
{
	if (!Scene::init())
	{
		return false;
	}
	srand((unsigned)time(NULL));
	setMap(randomMap());
	setSprite();

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
	Point actualPosition = Point(x, y);
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
	//_spawnpoint = _tilemap->getLayer("spawnpoint");
	//_spawnpoint->setVisible(false);

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
	_player1 = PlayerBase::create("bar.png");
	_player2 = PlayerBase::create("blood.png");

	_tilemap->addChild(_player1, 2);

    _player1->setAnchorPoint(Vec2(0, 0));
	_player2->setAnchorPoint(Vec2(0, 0));
	_player1->setPosition(spawn(mapName));
	_player2->setScaleX(0);

	_player1->addChild(_player2, 2);
	_player2->setPosition(Vec2(0, 0));
	_player1->setAnchorPoint(Vec2(0.5, 0.5));
	_newposition = _player1->getPosition();
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
	if (_count % 1 == 0 && _count % 3 != 0 && emptyTile - 1 > propSum)
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
		if (expSprite->getBoundingBox().intersectsRect(_player1->getBoundingBox()))
		{
			//添加拾取exp道具后人物属性的变化
			_player1->expRaise(_player1->EXP_RAISE);
			_player2->setScaleX(_player1->exp / _player1->expNeed);

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
	if (_count % 3 == 0 && emptyTile - 1 > propSum)
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
		if (hpSprite->getBoundingBox().intersectsRect(_player1->getBoundingBox()))
		{
			//添加拾取hp道具后人物属性的变化
			
			if (_player1->hp <= _player1->hpLimit)
			{
				_player1->hpRaise(_player1->HP_RAISE);
				//_player2->setScaleX(_player1->hp / _player1->hpLimit);
			}

			Vec2 coord = tileCoordForPosition(hpSprite->getPosition());
			propInfo.at(coord.x).at(coord.y) = 0;
			_tilemap->removeChild(hpSprite);
			hpVector.eraseObject(hpSprite);
			propSum--;
		}
	}
}

Vec2 GameScene::spawn(std::string& mapname)  //返回复活点坐标
{
	if (mapname == "map1")return Vec2(24 * _tilemap->getTileSize().width - 0.5*_tilemap->getTileSize().width, 4 * _tilemap->getTileSize().width - 0.5*_tilemap->getTileSize().width);
	if (mapname == "map2")return Vec2(5 * _tilemap->getTileSize().height - 0.5*_tilemap->getTileSize().height, 5 * _tilemap->getTileSize().height - 0.5*_tilemap->getTileSize().height);
}

void GameScene::update(float delta)
{
	_count++;
	updateEXPSprite(delta);
	judgeEXPSprite(delta);
	updateHPSprite(delta);
	judgeHPSprite(delta);
	Node::update(delta);
	auto leftArrow = EventKeyboard::KeyCode::KEY_LEFT_ARROW, rightArrow = EventKeyboard::KeyCode::KEY_RIGHT_ARROW;
	auto upArrow = EventKeyboard::KeyCode::KEY_UP_ARROW, downArrow = EventKeyboard::KeyCode::KEY_DOWN_ARROW;
	if (isKeyPressed(leftArrow)&&isCanPass(Vec2(tileCoordForPosition(_player1->getPosition()).x, tileCoordForPosition(_player1->getPosition()).y)))
	{
			keyPressedDuration(leftArrow);
	}
	else if (isKeyPressed(rightArrow)&&isCanPass(Vec2(tileCoordForPosition(_player1->getPosition()).x+2.9, tileCoordForPosition(_player1->getPosition()).y)))
	{
		keyPressedDuration(rightArrow);
	}
	else if (isKeyPressed(upArrow)&&isCanPass(Vec2(tileCoordForPosition(_player1->getPosition()).x, tileCoordForPosition(_player1->getPosition()).y-0.3)))
	{
		keyPressedDuration(upArrow);
	}
	else if (isKeyPressed(downArrow)&&isCanPass(Vec2(tileCoordForPosition(_player1->getPosition()).x, tileCoordForPosition(_player1->getPosition()).y)))
	{
		keyPressedDuration(downArrow);
	}
}

void GameScene::keyPressedDuration(EventKeyboard::KeyCode code) 
{
	switch (code) {
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		_newposition.x -= _player1->SPEED;
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		_newposition.x += _player1->SPEED;
		break;
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		_newposition.y += _player1->SPEED;
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		_newposition.y -= _player1->SPEED;
		break;
	default:
		break;
	}
	// 0.3s代表着动作从开始到结束所用的时间，从而显得不会那么机械。
	//auto moveTo = MoveTo::create(0, Vec2(_player1->getPositionX() + offsetX, _player1->getPositionY() + offsetY));
	//_player1->runAction(moveTo);
	_player1->setPosition(_newposition.x, _newposition.y);
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




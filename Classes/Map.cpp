#include "Map.h"
#include "cocos2d.h"

USING_NS_CC;

Scene* GameMap::createScene()
{
	return GameMap::create();
}

bool GameMap::init()
{
	if (!Scene::init())
	{
		return false;
	}
	int mapSum = 2;
	std::vector<std::string> mapList;
	mapList.resize(mapSum);
	mapList = { "map1","map2" };
	srand((unsigned)time(0));
	int randomMapNum = rand() % mapSum;
	while (randomMapNum == mapSum)
		randomMapNum = rand() % mapSum;
	setMap(mapList[randomMapNum]);
	scheduleUpdate();
	return true;
}

void GameMap::setViewPointCenter(float duration,Vec2 position)
{
	int x = MAX(position.x, _screenWidth / 2);
	int y = MAX(position.y, _screenHeight / 2);
	x = MIN(x, (getMapSize().width *getTileSize().width - _screenWidth / 2));
	y = MIN(y, (getMapSize().height *getTileSize().height - _screenHeight / 2));
	Point actualPosition = Point(x, y);
	Point centerOfView = Point(_screenWidth / 2, _screenHeight / 2);
	Point viewPoint = centerOfView - actualPosition;
	this->runAction(MoveTo::create(duration,viewPoint));
}

Vec2 GameMap::tileCoordForPosition(Vec2 position)
{
	float x = (int)(position.x / (getTileSize().width / CC_CONTENT_SCALE_FACTOR()));
	float pointHeight = getTileSize().height / CC_CONTENT_SCALE_FACTOR();
	float y = (int)((getMapSize().height*pointHeight - position.y) / pointHeight);
	return Vec2(x,y);
}

void GameMap::setMap(const std::string& mapName)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	_screenWidth = visibleSize.width;
	_screenHeight = visibleSize.height;
	std::string _mapName = mapName + ".tmx";
	_tilemap = TMXTiledMap::create(_mapName);
	_tilemap->setAnchorPoint(Vec2::ZERO);
	_tilemap->setPosition(Vec2::ZERO);
	_tilemap->setScale(0.3);
	this->addChild(_tilemap,-1);
	emptyTile = getMapSize().width*getMapSize().height;
	_collidable = _tilemap->getLayer("wall");
	
	mapInfo.resize(getMapSize().width);
	propInfo.resize(getMapSize().width);

	for (int i = 0; i < getMapSize().width; i++)
	{
		mapInfo.at(i).resize(getMapSize().height);
		propInfo.at(i).resize(getMapSize().height);
	}

	for (int i = 0; i < getMapSize().width; i++)
	{
		for (int j = 0; j < getMapSize().height; j++)
		{
			int tileGid = _collidable->getTileGIDAt(Vec2(i, j));
			if (tileGid)
			{
				Value properties = _tilemap->getPropertiesForGID(tileGid);
				ValueMap map = properties.asValueMap();
				std::string value = map.at("collidable").asString();
				if (value.compare("true") == 0)
				{
					mapInfo.at(i).at(j) = 1;
					emptyTile--;
				}
			}
		}
	}
}

bool GameMap::isCanPass(Vec2 &coord)
{
	if (coord.x >= 0 && coord.x < getMapSize().width
		&&coord.y >= 0 && coord.y < getMapSize().height
		&&mapInfo.at(coord.x).at(coord.y) != 1)
		return true;
	else return false;
}

bool GameMap::isCanUpdate(Vec2 &coord)
{
	if (coord.x >= 0 && coord.x < getMapSize().width
		&&coord.y >= 0 && coord.y < getMapSize().height
		&&propInfo.at(coord.x).at(coord.y) == 0
		&&mapInfo.at(coord.x).at(coord.y) == 0)
		return true;
	else return false;
}

Size GameMap::getMapSize()
{
	return _tilemap->getMapSize();
}

Size GameMap::getTileSize()
{
	return _tilemap->getTileSize();
}


Vec2 GameMap::randomPosition()
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

void GameMap::updateEXPSprite(float delta)
{
	if (_count % 1 == 0 && _count % 30 != 0 && emptyTile-2 > propSum)
	{
		Sprite *expProp = Sprite::create("smallcrystal.png");
		Vec2 position = randomPosition();
		expProp->setPosition(position);
		expProp->setScale(1.2);
		Vec2 propCoord = tileCoordForPosition(position);
		propInfo.at(propCoord.x).at(propCoord.y) = 1;
		_tilemap->addChild(expProp, 2);
		propSum++;
	}
}

void GameMap::updateHPSprite(float delta)
{
	if (_count % 3 == 0 && emptyTile-2> propSum)
	{
		Sprite *hpProp = Sprite::create("heart.png");
		Vec2 position = randomPosition();
		hpProp->setPosition(position);
		hpProp->setScale(0.8);
		Vec2 propCoord = tileCoordForPosition(position);
		propInfo.at(propCoord.x).at(propCoord.y) = 2;
		_tilemap->addChild(hpProp, 2);
		propSum++;
	}
}

void GameMap::update(float delta)
{
	_count++;
	updateEXPSprite(delta);
	updateHPSprite(delta);
}
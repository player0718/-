#pragma once
#include "cocos2d.h"	

USING_NS_CC;

class GameMap:public cocos2d::Scene
{
private:
	int _screenWidth, _screenHeight;
	TMXLayer* _collidable;
	std::vector<std::vector<int>> mapInfo;

	int _count;
	std::vector<std::vector<int>> propInfo;
	int emptyTile=0, propSum=0;

public:
	TMXTiledMap* _tilemap;

	static cocos2d::Scene* createScene();
	virtual bool init();

	void setViewPointCenter(Vec2 position);
	Vec2 tileCoordForPosition(Vec2 position);
	void setMap(const std::string& mapName);
	bool isCanPass(Vec2& coord);
	bool isCanUpdate(Vec2& coord);
	Size getMapSize();
	Size getTileSize();
	void update(float delta);
	void updateEXPSprite(float delta);
	void updateHPSprite(float delta);

    Vec2 randomPosition();


	CREATE_FUNC(GameMap);
};


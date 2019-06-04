#ifndef __MAP_H__
#define __MAP_H__
#include "cocos2d.h"	

USING_NS_CC;

class GameMap:public cocos2d::Scene
{
private:
	int _screenWidth, _screenHeight;
	TMXLayer* _collidable;
	std::vector<std::vector<int>> mapInfo; //储存地图上障碍物的信息

	int _count;
	std::vector<std::vector<int>> propInfo; //储存地图上道具和障碍物的信息
	int emptyTile=0, propSum=0;

public:
	TMXTiledMap* _tilemap;

	static cocos2d::Scene* createScene();
	virtual bool init();

	void setViewPointCenter(float duration,Vec2 position); //视野跟随人物
	Vec2 tileCoordForPosition(Vec2 position);
	void setMap(const std::string& mapName);  //初始化地图
	bool isCanPass(Vec2& coord); //判断是否有障碍物
	bool isCanUpdate(Vec2& coord); //判断是否能刷新道具
	Size getMapSize();
	Size getTileSize();
	void update(float delta);
	void updateEXPSprite(float delta);
	void updateHPSprite(float delta);

    Vec2 randomPosition();


	CREATE_FUNC(GameMap);
};

#endif

#ifndef __MAP_H__
#define __MAP_H__
#include "cocos2d.h"	

USING_NS_CC;

class GameMap:public cocos2d::Scene
{
private:
	int _screenWidth, _screenHeight;
	TMXLayer* _collidable;
	std::vector<std::vector<int>> mapInfo; //�����ͼ���ϰ������Ϣ

	int _count;
	std::vector<std::vector<int>> propInfo; //�����ͼ�ϵ��ߺ��ϰ������Ϣ
	int emptyTile=0, propSum=0;

public:
	TMXTiledMap* _tilemap;

	static cocos2d::Scene* createScene();
	virtual bool init();

	void setViewPointCenter(float duration,Vec2 position); //��Ұ��������
	Vec2 tileCoordForPosition(Vec2 position);
	void setMap(const std::string& mapName);  //��ʼ����ͼ
	bool isCanPass(Vec2& coord); //�ж��Ƿ����ϰ���
	bool isCanUpdate(Vec2& coord); //�ж��Ƿ���ˢ�µ���
	Size getMapSize();
	Size getTileSize();
	void update(float delta);
	void updateEXPSprite(float delta);
	void updateHPSprite(float delta);

    Vec2 randomPosition();


	CREATE_FUNC(GameMap);
};

#endif

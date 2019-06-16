#ifndef _GAMESCENE_H_
#define _GAMESCENE_H_
#include "cocos2d.h"
#include "PlayerBase.h"
USING_NS_CC;

/*class HP_MESS:public cocos2d::Sprite
{
public:
	int coordX, coordY;

	bool operator==(const HP_MESS &thv)
	{
		if (this->coordX == thv.coordX && this->coordY == thv.coordY)
		{
			return true;
		}
		else return false;
	}
	static HP_MESS* create(std::string filename, int randX, int randY)
	{ 
		HP_MESS *pRet = new(std::nothrow) HP_MESS(); 
		if (pRet &&pRet->initWithFile(filename)&& pRet->init( randX,randY)) 
		{ 
			pRet->autorelease(); 
			return pRet; 
		} 
		else 
		{ 
			delete pRet; 
			pRet = nullptr;
			return nullptr; 
		} 
	}
	virtual bool init( int randX, int randY)
	{
		
		 coordX = randX, coordY = randY;
		return true;
	}
};*/

/*class EXP_MESS:public cocos2d::Sprite
{
public:
	Sprite* expProp;
	int coordX, coordY;

	bool operator==(const EXP_MESS &thv)
	{
		if (this->coordX == thv.coordX && this->coordY == thv.coordY)
		{
			return true;
		}
		else return false;
	}
	static EXP_MESS* create(std::string filename, int randX, int randY)
	{
		EXP_MESS *pRet = new(std::nothrow) EXP_MESS();
		if (pRet &&pRet->initWithFile(filename)&& pRet->init( randX, randY))
		{
			pRet->autorelease();
			return pRet;
		}
		else
		{
			delete pRet;
			pRet = nullptr;
			return nullptr;
		}
	}
	virtual bool init( int randX, int randY)
	{

		coordX = randX, coordY = randY;
		return true;
	}
};*/

enum DataType {  //动作指令
	RUN,
	ATTACK,
	SHUXING,
	DEATH
};

struct ShuXing {  //人物属性变化
	int id;
	int color = BLUE;
	float level = 1;  //当前等级
	float hp = 100.0;  //当前血量
	float hpLimit = 100.0;  //血量上限
	float exp = 0;  //当前经验值
	float expNeed = 100.0;  //升到下一级所需经验
	float expOverflow;  //溢出的经验
	bool alive = true;  //是否存活
	bool attack = true;  //是否能攻击

	int ATK = 30;  //攻击力
	int DEF = 10;  //防御力
	int RANGE = 300;  //攻击范围
	int SPEED = 4;  //移动速度
	int HP_RAISE = 1;  //捡取红心恢复的血量
	int EXP_RAISE = 1;  //捡取水晶增加的经验值
};
struct GameData  //数据分类
{
	int rundir;
	int attactdir;
	int dataSize;
	DataType dataType;
	Vec2 position;
	ShuXing shuxing;



};



class GameScene : public cocos2d::Scene
{
private:
    TMXTiledMap* _tilemap;
	int _screenWidth, _screenHeight;
	std::string mapName;
    TMXLayer* _collidable;
	TMXLayer* _spawnpoint;
	std::vector<std::vector<int>> mapInfo; //储存地图上障碍物的信息
	std::vector<std::vector<int>> spawnPoint;  //储存地图上的复活点
	Vec2 _newposition;

	int _count = 0;
	std::vector<std::vector<int>> propInfo; //储存地图上道具和障碍物的信息
	int emptyTile , propSum = 0;

	static cocos2d::Vector<Sprite*>hpVector;
	static cocos2d::Vector<Sprite*>expVector;

	PlayerBase *_player1,*_player2;
	float blood = 0;


	std::map<cocos2d::EventKeyboard::KeyCode, bool> keys;
public:
	
    static cocos2d::Scene* createScene();
	virtual bool init();

	void setViewPointCenter(Vec2 position); //视野跟随人物
	Vec2 tileCoordForPosition(Vec2 position);
	Vec2 positionForTileCoord(Vec2 coord);
	void setMap(const std::string& mapName);  //初始化地图
	void setSprite();  //初始化精灵
	bool isCanPass(Vec2& coord); //判断是否有障碍物
	bool isCanUpdate(Vec2& coord); //判断是否能刷新道具
	Size getMapSize();
	Size getTileSize();
	void update(float delta);
	void updateEXPSprite(float delta);
	void judgeEXPSprite(float delta);
	void updateHPSprite(float delta);
	void judgeHPSprite(float delta);
    Vec2 randomPosition();
	Vec2 spawn(std::string& mapname);
	std::string randomMap();

	CREATE_FUNC(GameScene);



	void keyPressedDuration(EventKeyboard::KeyCode code);
	bool isKeyPressed(EventKeyboard::KeyCode keyCode);
	bool canMove(Vec2& coord);
};

#endif
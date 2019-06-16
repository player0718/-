#ifndef _GAMESCENE_H_
#define _GAMESCENE_H_
#include "cocos2d.h"
#include "PlayerBase.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;

class GameScene : public cocos2d::Layer
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
	int _downtime;
	std::vector<std::vector<int>> propInfo; //储存地图上道具和障碍物的信息
	int emptyTile , propSum = 0;

	static cocos2d::Vector<Sprite*>hpVector;
	static cocos2d::Vector<Sprite*>expVector;  //这三个精灵数组都是用来通过遍历判断碰撞的
	cocos2d::Vector<Sprite*>arrowVector;

	Sprite *emptyExpBar, *expBar, *emptyHpBar, *hpBar;
	Sprite *arrow;
	Sprite *_skill1, *_skill2, *_skill3;
	Sprite* dieScene;
	Sprite* shadow;
	cocos2d::ui::Text* level_text;
	Layer *_layer1, *_layer2;
	float blood = 0;

	bool running1 = false;
	bool button_exist = false;

	////////////////////////////////////////////////////
	//原Skill类代码
	cocos2d::ui::Button* button_1;
	cocos2d::ui::Button* button_2;
	cocos2d::ui::Button* button_3;

	int skill_id_1, skill_id_2, skill_id_3;

	EventListenerTouchOneByOne* _listener1;
	std::map<cocos2d::EventKeyboard::KeyCode, bool> keys;
public:
	PlayerBase *_player1;  //玩家1

    static cocos2d::Scene* createScene();
	virtual bool init();

	void setViewPointCenter(Vec2 position); //视野跟随人物
	Vec2 tileCoordForPosition(Vec2 position);  //将position转为coord
	Vec2 positionForTileCoord(Vec2 coord);  //将coord转为position
	void setMap(const std::string& mapName);  //初始化地图
	void setSprite();  //初始化精灵
	void setTouchController();  //初始化全屏触摸监听器（人物攻击）
	void setMusic();  //预加载音乐和音效
	bool isCanPass(Vec2& coord); //判断是否有障碍物
	bool isCanUpdate(Vec2& coord); //判断是否能刷新道具
	Size getMapSize();
	Size getTileSize();
	void update(float delta);  //每一帧调用一次
	void updateEXPSprite(float delta);  //刷新水晶（经验道具）
	void judgeEXPSprite(float delta);  //判断水晶的捡拾以及产生效果（加经验）
	void updateHPSprite(float delta);  //刷新红心（生命道具）
	void judgeHPSprite(float delta);  //判断红心的捡拾以及产生效果（加血）
	void barListener(float delta);  //更新血条和经验条
	void deathListener(float delta);  //判断死亡，生成死亡画面以及复活
	void restartCount(float delta);  //死亡画面的文本倒计时
    Vec2 randomPosition();  //生成随机坐标（服务于刷新道具）
	Vec2 spawn();  //生成复活点坐标
	std::string randomMap();  //生成随机地图

	void animationcreate(int direct);  //人物行走动画
	void ifhurt(float delta);
	/*bool onTouchBegan(Touch* touch, Event* event);*/

	CREATE_FUNC(GameScene);

	void setSkillButton();  //生成技能按钮

	void keyPressedDuration(EventKeyboard::KeyCode code);  
	bool isKeyPressed(EventKeyboard::KeyCode keyCode);
	bool canMove(Vec2& position);


	////////////////////////////////////////////////////
	//原Skill类代码
	std::string skillID_1();
	std::string skillID_2();
	std::string skillID_3();

	void skillChoose(int id); 

	void atkUp();
	void rangeUp();
	void defUp();
	void hpraiseUp();
	void expraiseUp();
	void speedUp();
	void hpUp();
	void atkUphpDown();
	void rangeUpspeedDown();
	void allUp();
	void arrowBack();
	void arrowLeftRight();
	void arrowAhead();
	void shootTwice();
	void magnet();
};

#endif
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
	static cocos2d::Vector<Sprite*>expVector;
	cocos2d::Vector<Sprite*>arrowVector;

	Sprite *emptyExpBar, *expBar, *emptyHpBar, *hpBar;
	Sprite *arrow;
	Sprite *_skill1, *_skill2, *_skill3;
	Sprite* dieScene;
	cocos2d::ui::Text* level_text;
	Layer *_layer1, *_layer2;
	float blood = 0;

	bool running1 = false;

	////////////////////////////////////////////////////
	//原Skill类代码
	cocos2d::ui::Button* button_1;
	cocos2d::ui::Button* button_2;
	cocos2d::ui::Button* button_3;

	int skill_id_1, skill_id_2, skill_id_3;


	std::map<cocos2d::EventKeyboard::KeyCode, bool> keys;
public:
	PlayerBase *_player1;

    static cocos2d::Scene* createScene();
	virtual bool init();

	void setViewPointCenter(Vec2 position); //视野跟随人物
	Vec2 tileCoordForPosition(Vec2 position);
	Vec2 positionForTileCoord(Vec2 coord);
	void setMap(const std::string& mapName);  //初始化地图
	void setSprite();  //初始化精灵
	void setTouchController();
	bool isCanPass(Vec2& coord); //判断是否有障碍物
	bool isCanUpdate(Vec2& coord); //判断是否能刷新道具
	Size getMapSize();
	Size getTileSize();
	void update(float delta);
	void updateEXPSprite(float delta);
	void judgeEXPSprite(float delta);
	void updateHPSprite(float delta);
	void judgeHPSprite(float delta);
	void barListener(float delta);
	void deathListener(float delta);
	void restartCount(float delta);
    Vec2 randomPosition();
	Vec2 spawn();
	std::string randomMap();

	void animationcreate(int direct);
	void ifhurt(float delta);
	/*bool onTouchBegan(Touch* touch, Event* event);*/

	CREATE_FUNC(GameScene);

	void setSkillButton();

	void keyPressedDuration(EventKeyboard::KeyCode code);
	bool isKeyPressed(EventKeyboard::KeyCode keyCode);
	//bool canMove(Vec2& coord);


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
};

#endif
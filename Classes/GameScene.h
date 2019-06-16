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
	std::vector<std::vector<int>> mapInfo; //�����ͼ���ϰ������Ϣ
	std::vector<std::vector<int>> spawnPoint;  //�����ͼ�ϵĸ����
	Vec2 _newposition;

	int _count = 0;
	int _downtime;
	std::vector<std::vector<int>> propInfo; //�����ͼ�ϵ��ߺ��ϰ������Ϣ
	int emptyTile , propSum = 0;

	static cocos2d::Vector<Sprite*>hpVector;
	static cocos2d::Vector<Sprite*>expVector;  //�������������鶼������ͨ�������ж���ײ��
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
	//ԭSkill�����
	cocos2d::ui::Button* button_1;
	cocos2d::ui::Button* button_2;
	cocos2d::ui::Button* button_3;

	int skill_id_1, skill_id_2, skill_id_3;

	EventListenerTouchOneByOne* _listener1;
	std::map<cocos2d::EventKeyboard::KeyCode, bool> keys;
public:
	PlayerBase *_player1;  //���1

    static cocos2d::Scene* createScene();
	virtual bool init();

	void setViewPointCenter(Vec2 position); //��Ұ��������
	Vec2 tileCoordForPosition(Vec2 position);  //��positionתΪcoord
	Vec2 positionForTileCoord(Vec2 coord);  //��coordתΪposition
	void setMap(const std::string& mapName);  //��ʼ����ͼ
	void setSprite();  //��ʼ������
	void setTouchController();  //��ʼ��ȫ�����������������﹥����
	void setMusic();  //Ԥ�������ֺ���Ч
	bool isCanPass(Vec2& coord); //�ж��Ƿ����ϰ���
	bool isCanUpdate(Vec2& coord); //�ж��Ƿ���ˢ�µ���
	Size getMapSize();
	Size getTileSize();
	void update(float delta);  //ÿһ֡����һ��
	void updateEXPSprite(float delta);  //ˢ��ˮ����������ߣ�
	void judgeEXPSprite(float delta);  //�ж�ˮ���ļ�ʰ�Լ�����Ч�����Ӿ��飩
	void updateHPSprite(float delta);  //ˢ�º��ģ��������ߣ�
	void judgeHPSprite(float delta);  //�жϺ��ĵļ�ʰ�Լ�����Ч������Ѫ��
	void barListener(float delta);  //����Ѫ���;�����
	void deathListener(float delta);  //�ж��������������������Լ�����
	void restartCount(float delta);  //����������ı�����ʱ
    Vec2 randomPosition();  //����������꣨������ˢ�µ��ߣ�
	Vec2 spawn();  //���ɸ��������
	std::string randomMap();  //���������ͼ

	void animationcreate(int direct);  //�������߶���
	void ifhurt(float delta);
	/*bool onTouchBegan(Touch* touch, Event* event);*/

	CREATE_FUNC(GameScene);

	void setSkillButton();  //���ɼ��ܰ�ť

	void keyPressedDuration(EventKeyboard::KeyCode code);  
	bool isKeyPressed(EventKeyboard::KeyCode keyCode);
	bool canMove(Vec2& position);


	////////////////////////////////////////////////////
	//ԭSkill�����
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
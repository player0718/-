#ifndef __PLAYER_BASE_H__
#define __PLAYER_BASE_H__

#define BLUE 0
#define RED 1

#include "cocos2d.h"

class PlayerBase : public cocos2d::Sprite
{
private:

public:
	static PlayerBase* create(const std::string& filename)
	{
		PlayerBase *sprite = new PlayerBase();
		if (sprite && sprite->initWithFile(filename))
		{
			sprite->autorelease();
			return sprite;
		}
		CC_SAFE_DELETE(sprite);
		return nullptr;
	}

	int id;
	int color = BLUE;  //角色的阵营
	int lives = 10;  //当前生命剩余
	float restartTime = 5;  //角色复活所需时间
	int level = 1;  //当前等级
	float hp = 100.0;  //当前血量
	float hpLimit = 100.0;  //血量上限
	float _hp = 100.0;  //初始血量上限
	float exp = 0;  //当前经验值
	float expNeed = 100.0;  //升到下一级所需经验
	float expOverflow;  //溢出的经验
	bool alive = true;  //是否存活
	bool attack = true;  //是否能攻击
	bool move = true;  //是否能移动
	bool moving = false;  //是否在移动
	bool shooting = false;  //是否在射击
	bool collect = true;  //能否捡道具
	//bool level_up = false;  //是否升级

	int ATK = 30;  //攻击力
	int DEF = 10;  //防御力
	int RANGE = 200;  //攻击范围
	int SPEED = 5;  //移动速度
	int HP_RAISE = 1;  //捡取红心恢复的血量
	int EXP_RAISE = 1;  //捡取水晶增加的经验值

	int weapon = 1;  //武器类型：1弓箭2地刺3飞镖
	int arrow_ahead = 1;
	bool arrow_left_right = false;
	bool arrow_back = false;
	bool arrow_shoot_twice = false;
	bool magnet = false;
	
	void hpRaise(int num);  //血量增加
	bool expRaise(int num);  //经验增加
	void die();  //判断是否死亡
	void restart(cocos2d::Vec2 spawnpoint);  //复活
	//bool onTouchBegan(Touch* touch, Event* event);

	virtual bool init();
};

#endif
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

	PlayerBase* player;
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
	
	void hpRaise(int num);  //血量增加
	bool expRaise(int num);  //经验增加
	bool die();  //判断是否死亡
	//void restart(Vec2& spawnpoint);  //复活

	virtual bool init();
};

#endif
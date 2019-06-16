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
	int color = BLUE;  //��ɫ����Ӫ
	int lives = 10;  //��ǰ����ʣ��
	float restartTime = 5;  //��ɫ��������ʱ��
	int level = 1;  //��ǰ�ȼ�
	float hp = 100.0;  //��ǰѪ��
	float hpLimit = 100.0;  //Ѫ������
	float _hp = 100.0;  //��ʼѪ������
	float exp = 0;  //��ǰ����ֵ
	float expNeed = 100.0;  //������һ�����辭��
	float expOverflow;  //����ľ���
	bool alive = true;  //�Ƿ���
	bool attack = true;  //�Ƿ��ܹ���
	bool move = true;  //�Ƿ����ƶ�
	bool moving = false;  //�Ƿ����ƶ�
	bool shooting = false;  //�Ƿ������
	bool collect = true;  //�ܷ�����
	//bool level_up = false;  //�Ƿ�����

	int ATK = 30;  //������
	int DEF = 10;  //������
	int RANGE = 200;  //������Χ
	int SPEED = 5;  //�ƶ��ٶ�
	int HP_RAISE = 1;  //��ȡ���Ļָ���Ѫ��
	int EXP_RAISE = 1;  //��ȡˮ�����ӵľ���ֵ

	int weapon = 1;  //�������ͣ�1����2�ش�3����
	int arrow_ahead = 1;
	bool arrow_left_right = false;
	bool arrow_back = false;
	bool arrow_shoot_twice = false;
	bool magnet = false;
	
	void hpRaise(int num);  //Ѫ������
	bool expRaise(int num);  //��������
	void die();  //�ж��Ƿ�����
	void restart(cocos2d::Vec2 spawnpoint);  //����
	//bool onTouchBegan(Touch* touch, Event* event);

	virtual bool init();
};

#endif
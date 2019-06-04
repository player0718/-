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
	float level = 1;  //��ǰ�ȼ�
	float hp = 100.0;  //��ǰѪ��
	float hpLimit = 100.0;  //Ѫ������
	float exp = 0;  //��ǰ����ֵ
	float expNeed = 100.0;  //������һ�����辭��
	float expOverflow;  //����ľ���
	bool alive = true;  //�Ƿ���
	bool attack = true;  //�Ƿ��ܹ���

	int ATK = 30;  //������
	int DEF = 10;  //������
	int RANGE = 300;  //������Χ
	int SPEED = 4;  //�ƶ��ٶ�
	int HP_RAISE = 1;  //��ȡ���Ļָ���Ѫ��
	int EXP_RAISE = 1;  //��ȡˮ�����ӵľ���ֵ
	
	void hpRaise(int num);  //Ѫ������
	bool expRaise(int num);  //��������
	bool die();  //�ж��Ƿ�����
	//void restart(Vec2& spawnpoint);  //����

	virtual bool init();
};

#endif
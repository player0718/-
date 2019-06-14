#ifndef _SKILL_H_
#define _SKILL_H_
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "PlayerBase.h"

USING_NS_CC;
using namespace ui;

class Skill : public cocos2d::Sprite
{
private:

public:
	PlayerBase* player;
	virtual bool init();

	Button* button_1;
	Button* button_2;
	Button* button_3;

	cocos2d::Vector<Button*> buttonList;

	int skill_id_1, skill_id_2, skill_id_3;

	std::string skillID_1();
	std::string skillID_2();
	std::string skillID_3();

	void skillChoose(int id);

	void atkUp();
	void rangeUp();
	void defUp();

	void remove();

	Skill* createSkillList();
	CREATE_FUNC(Skill);
	
};

#endif
#include "Skill.h"
#include "GameScene.h"
#include "ui/UIButton.h"
#include "cocos2d.h"
#define SKILL_SUM 4

USING_NS_CC;
using namespace ui;

bool Skill::init()
{
	if (!Sprite::init())
	{
		return false;
	}
	player = dynamic_cast<GameScene*>(Director::getInstance()->getRunningScene()->getChildByName("GameScene"))->_player1;
	button_1 = Button::create(skillID_1());
	button_1->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type) {
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			break;
		case ui::Widget::TouchEventType::ENDED:
			player = dynamic_cast<GameScene*>(Director::getInstance()->getRunningScene()->getChildByName("GameScene"))->_player1;
			skillChoose(skill_id_2);
			//button_1->setEnabled(false);
			//button_1->setVisible(false);
			player->removeChild(button_1, true);
			player->removeChild(button_2, true);
			player->removeChild(button_3, true);
			break;
		default:
			break;
		}
	});

	//buttonList.pushBack(button_1);

	button_2 = Button::create(skillID_2());
	button_2->addTouchEventListener([=](Ref*pSender, Widget::TouchEventType type)
	{
		if (type == Widget::TouchEventType::ENDED)
		{
			player = dynamic_cast<GameScene*>(Director::getInstance()->getRunningScene()->getChildByName("GameScene"))->_player1;
			skillChoose(skill_id_2);
			player->removeChild(button_1, true);
			player->removeChild(button_2, true);
			player->removeChild(button_3, true);
		}
	});

	//buttonList.pushBack(button_2);

	button_3 = Button::create(skillID_3());
	button_3->addTouchEventListener([=](Ref*pSender, Widget::TouchEventType type)
	{
		if (type == Widget::TouchEventType::ENDED)
		{
			player = dynamic_cast<GameScene*>(Director::getInstance()->getRunningScene()->getChildByName("GameScene"))->_player1;
			skillChoose(skill_id_3);
			player->removeChild(button_1, true);
			player->removeChild(button_2, true);
			player->removeChild(button_3, true);
		}
	});

	//buttonList.pushBack(button_3);

	return true;
}

void Skill::remove()
{
	this->removeAllChildrenWithCleanup(true);
	this->removeFromParent();
}

void Skill::skillChoose(int id)
{
	switch (id)
	{
	case(1): atkUp(); break;
	case(2):rangeUp(); break;
	case(3):defUp(); break;
	}
}

std::string Skill::skillID_1()
{
	skill_id_1 = rand() % SKILL_SUM;
	if (skill_id_1 == 0)
		return skillID_1();
	else return "_skill\\choice" + std::to_string(skill_id_1) + ".png";
}

std::string Skill::skillID_2()
{
	skill_id_2 = rand() % SKILL_SUM;
	if (skill_id_2 == 0 || skill_id_2==skill_id_1)
		return skillID_2();
	else return "_skill\\choice"+std::to_string(skill_id_2) + ".png";
}

std::string Skill::skillID_3()
{
	skill_id_3 = rand() % SKILL_SUM;
	if (skill_id_3 == 0 || skill_id_3==skill_id_2 || skill_id_3==skill_id_1)
		return skillID_3();
	else return "_skill\\choice" + std::to_string(skill_id_3) + ".png";
}

void Skill::atkUp()
{
	player->ATK += 5;
}

void Skill::rangeUp()
{
	player->RANGE += 3;
}

void Skill::defUp()
{
	player->DEF += 3;
}
 
Skill* Skill::createSkillList()
{
	auto skill_list = new Skill();
	if (skill_list&& skill_list->init())
	{
		skill_list->autorelease();
		return skill_list;
	}
	CC_SAFE_DELETE(skill_list);
	return nullptr;
}






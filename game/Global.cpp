/*************************************************
Copyright:bro7
Author:ben
Date:2014-07-08
Description:全局变量设置
**************************************************/

#include "Global.h"

Global* Global::_instance = nullptr;

Global::Global()
{
	currentLevel = 1;

	totalLevels = 8;

	//whyToMainMenu = efor_StartGame;

	currentHeroType = eBody_Small;

	currentBulletType = eBullet_common;

	lifeNum = 3;
}


Global::~Global()
{
}

Global* Global::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new Global();
	}
	return _instance;
}


void  Global::currentLevelPlusOne()
{
	++currentLevel;
}


void Global::reSetLevel()
{
	currentLevel = 1;
}


//void Global::setWhyToMainMenu(ToMainMenuFor why)
//{
//	whyToMainMenu = why;
//}
//
//ToMainMenuFor Global::getForToMainMenu()
//{
//	return whyToMainMenu;
//}

void Global::setCurrentHeroType(BodyType _type)
{
	currentHeroType = _type;
}

BodyType Global::getCurrentHeroType()
{
	return currentHeroType;
}

void Global::setCurrentBulletType(BulletType _type)
{
	currentBulletType = _type;
}

BulletType Global::getCurrentBulletType()
{
	return currentBulletType;
}

void Global::lifeNumPlusOne()
{
	++lifeNum;
}

void Global::lifeNumCutOne()
{
	--lifeNum;
}



void Global::setCurrentLifeNum(int num)
{
	lifeNum = num;
}

int Global::getCurrentLifeNum()
{
	return lifeNum;
}

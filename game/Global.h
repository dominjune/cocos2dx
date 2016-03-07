#ifndef _GLOBAL_H_ 
#define _GLOBAL_H_

#include "cocos2d.h"
#include "GameEnum.h"

class Global
{
public:
	Global();
	~Global();

private:
	static Global* _instance;

public:
	static Global* getInstance();
	void currentLevelPlusOne();



	CC_SYNTHESIZE_READONLY(int, totalLevels, TotalLevels);
	CC_SYNTHESIZE(int, currentLevel, CurrentLevel);

	void reSetLevel();

	//ToMainMenuFor whyToMainMenu;  // 记录为什么返回主菜单，或是因为死，或是因为通过本关
	//void setWhyToMainMenu(ToMainMenuFor why);
	//ToMainMenuFor getForToMainMenu();

	BodyType currentHeroType;  // 当前主角的身型状态
	void setCurrentHeroType(BodyType _type);
	BodyType getCurrentHeroType();

	BulletType currentBulletType;  // 当前主角使用的子弹类型
	void setCurrentBulletType(BulletType _type);
	BulletType getCurrentBulletType();

	// 马里奥一共的生命条数
	int lifeNum;
	void lifeNumPlusOne();
	void lifeNumCutOne();
	void setCurrentLifeNum(int num);
	int getCurrentLifeNum();

};

#endif
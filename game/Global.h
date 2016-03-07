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

	//ToMainMenuFor whyToMainMenu;  // ��¼Ϊʲô�������˵���������Ϊ����������Ϊͨ������
	//void setWhyToMainMenu(ToMainMenuFor why);
	//ToMainMenuFor getForToMainMenu();

	BodyType currentHeroType;  // ��ǰ���ǵ�����״̬
	void setCurrentHeroType(BodyType _type);
	BodyType getCurrentHeroType();

	BulletType currentBulletType;  // ��ǰ����ʹ�õ��ӵ�����
	void setCurrentBulletType(BulletType _type);
	BulletType getCurrentBulletType();

	// �����һ������������
	int lifeNum;
	void lifeNumPlusOne();
	void lifeNumCutOne();
	void setCurrentLifeNum(int num);
	int getCurrentLifeNum();

};

#endif
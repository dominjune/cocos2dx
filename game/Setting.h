/*************************************************
Copyright:bro7
Author:ben
Date:2014-07-05
Description:游戏主菜单页的设置页面
**************************************************/


#ifndef __SETTING_H__
#define __SETTING_H__

#include "cocos2d.h"
class Setting : public cocos2d::Layer
{
public:
	Setting();
	~Setting();

	bool init();
	CREATE_FUNC(Setting);
	static cocos2d::Scene * createScene();
	void menuBack(Ref *pSender);
private:
	cocos2d::Sprite* spEffectNormal;
	cocos2d::Sprite* spEffectSelect;
	cocos2d::Sprite* spBgMusicNormal;
	cocos2d::Sprite* spBgMusicSelect;

	void setEffect(Ref*);
	void setBgMusic(Ref*);
	void readSetting();

	bool effectOn;
	bool bgMusicOn;
};

#endif
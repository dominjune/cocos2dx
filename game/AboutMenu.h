/*************************************************
Copyright:bro7
Author:ben
Date:2014-07-05
Description:��Ϸ���˵�ҳ�Ĺ���ҳ��
**************************************************/


#ifndef _ABOUTMENU_H_
#define _ABOUTMENU_H_

#include "cocos2d.h"
class About : public cocos2d::Layer
{
public:
	About();
	~About();

	bool init();
	CREATE_FUNC(About);
	static cocos2d::Scene * createScene();
	void menuBack(Ref *pSender);
private:

};

#endif
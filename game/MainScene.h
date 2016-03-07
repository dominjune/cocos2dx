#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "cocos2d.h"

class MainScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // implement the "static create()" method manually
	CREATE_FUNC(MainScene);

private:

	cocos2d::MenuItem *pStartGame;
	cocos2d::MenuItem *pQuit;
	cocos2d::MenuItem *pAbout;
	cocos2d::MenuItem *pSetting;
	cocos2d::Menu * pMenu;


	void menuCallBackForStartGame(Ref *pSender);
	void menuCallBackForAbout(Ref *pSender);
	void menuQuit(Ref *pSender);
	void menuSetting(Ref *pSender);
};

#endif // __LoadingScene_SCENE_H__
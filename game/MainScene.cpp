/*************************************************
Copyright:bro7
Author:ben
Date:2014-07-08
Description:游戏loading页面展示
**************************************************/


#include "MainScene.h"
#include "AboutMenu.h"
#include "Setting.h"
#include "SelectLevel.h"
USING_NS_CC;

Scene* MainScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MainScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
	this->setTouchEnabled(true);
	Size winSize = Director::getInstance()->getWinSize();

	std::string resouceMain = "image/mainscene/";
	Texture2D *bgTex = TextureCache::getInstance()->textureForKey(resouceMain + "background.png");
	auto spBg = Sprite::createWithTexture(bgTex);
	this->addChild(spBg);
	spBg->setPosition(winSize.width / 2, winSize.height / 2);

    pStartGame = MenuItemImage::create(resouceMain + "startgame_normal.png", resouceMain + "startgame_select.png",
		this, menu_selector(MainScene::menuCallBackForStartGame));
	pStartGame->setPosition(Point(winSize.width / 2, winSize.height / 2));

	pQuit = MenuItemImage::create(resouceMain + "quitgame_normal.png", resouceMain + "quitgame_select.png",
		this, menu_selector(MainScene::menuQuit));
	pQuit->setPosition(Point(winSize.width / 2, winSize.height / 2 - 40));

	//pAbout = MenuItemImage::create(resouceMain + "about_normal.png", resouceMain + "about_select.png",
	//	this, menu_selector(MainScene::menuCallBackForAbout));
	//pAbout->setPosition(Point(winSize.width - 50, 20));

	pSetting = MenuItemImage::create(resouceMain + "Setting_n.png", resouceMain + "setting_s.png",
		this, menu_selector(MainScene::menuSetting));
	pSetting->setPosition(Point(winSize.width / 2, winSize.height / 2 - 80));

	pMenu = Menu::create(pStartGame, pQuit, pSetting, NULL);
	pMenu->setPosition(Point(0, 0));
	this->addChild(pMenu, 0);

    return true;
}

void MainScene::menuCallBackForStartGame(Ref *pSender)
{
	auto scene = SelectLevel::createScene();
	Director::getInstance()->replaceScene(scene);


}
void  MainScene::menuCallBackForAbout(Ref *pSender)
{
	Scene *pAbout = About::createScene();

	Director::getInstance()->pushScene(pAbout);
}
void  MainScene::menuQuit(Ref *pSender)
{
	Director::getInstance()->end();
}
void  MainScene::menuSetting(Ref *pSender)
{
	Scene *pSetting = Setting::createScene();
	Director::getInstance()->pushScene(pSetting);
}
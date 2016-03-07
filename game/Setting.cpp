#include "Setting.h"

USING_NS_CC;

Setting::Setting()
{

}

Setting::~Setting()
{

}

bool Setting::init()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	std::string resourceMain = "image/mainscene/";
	//返回按钮
	MenuItem *itemReturn = MenuItemImage::create(resourceMain + "backA.png", resourceMain+"backB.png",
		this, menu_selector(Setting::menuBack));
	Menu *menuReturn = Menu::create(itemReturn, nullptr);
	this->addChild(menuReturn);
	menuReturn->setPosition(20, visibleSize.height-20);

	auto spBg = Sprite::createWithTexture(
		TextureCache::getInstance()->getTextureForKey(resourceMain + "Set_Music.png")
	);
	this->addChild(spBg);
	spBg->setPosition(visibleSize.width / 2, visibleSize.height / 2);

    spEffectNormal = Sprite::createWithTexture(
		TextureCache::getInstance()->getTextureForKey(resourceMain + "sound_effect_on.png")
	);
	this->addChild(spEffectNormal);
	spEffectNormal->setPosition(Point(visibleSize.width / 2 + 80,  visibleSize.height / 2 - 40));

	spEffectSelect = Sprite::createWithTexture(
		TextureCache::getInstance()->getTextureForKey(resourceMain + "sound_effect_off.png")
	);
	this->addChild(spEffectSelect);
	spEffectSelect->setPosition(Point(visibleSize.width / 2 + 80, visibleSize.height / 2 - 40));

	spBgMusicNormal = Sprite::createWithTexture(
		TextureCache::getInstance()->getTextureForKey(resourceMain + "music_on.png")
	);
	this->addChild(spBgMusicNormal);
	spBgMusicNormal->setPosition(Point(visibleSize.width / 2 + 80, visibleSize.height / 2 + 40));
	
	spBgMusicSelect = Sprite::createWithTexture(
		TextureCache::getInstance()->getTextureForKey(resourceMain + "music_off.png")
	);
	this->addChild(spBgMusicSelect);
	spBgMusicSelect->setPosition(Point(visibleSize.width / 2 + 80, visibleSize.height / 2 + 40));


	MenuItemImage *pMusicMenu = MenuItemImage::create(resourceMain + "switchBg.png", resourceMain + "switchBg.png",
		this, menu_selector(Setting::setEffect));
	pMusicMenu->setPosition(Point( visibleSize.width / 2 + 80,  visibleSize.height / 2 - 40));
	MenuItemImage *pEffectMenu = MenuItemImage::create(resourceMain + "switchBg.png", resourceMain + "switchBg.png",
		this, menu_selector(Setting::setBgMusic));
	pEffectMenu->setPosition(Point( visibleSize.width / 2 + 80, visibleSize.height / 2 + 40));

	Menu *pMenu = Menu::create(pMusicMenu, pEffectMenu, NULL);
	pMenu->setAnchorPoint(Point(0, 0)/*CCPointZero*/);
	pMenu->setPosition(Point(0, 0)/*CCPointZero*/);
	this->addChild(pMenu, 1);

	//添加toggle菜单 测试
	//auto menuItemEffect = MenuItemImage::create(resourceMain + "sound_effect_on.png", resourceMain + "sound_effect_off.png",
	//	this,NULL
	//	);
	//auto menuItemMusic = MenuItemImage::create(resourceMain + "music_on.png", resourceMain + "music_off.png",
	//	this, NULL
	//	);
	//auto itemNNew = MenuItemToggle::createWithCallback(CC_CALLBACK_1(Setting::setEffect, this), menuItemEffect, nullptr);

	//auto menuNNew = Menu::create(itemNNew, NULL);
	//this->addChild(menuNNew);
	//menuNNew->setPosition(200, 200);
	readSetting();
	return true;
}
void Setting::readSetting()
{
	effectOn = UserDefault::getInstance()->getBoolForKey("effect");
	spEffectNormal->setVisible(effectOn);
	spEffectSelect->setVisible(!effectOn);

	bgMusicOn = UserDefault::getInstance()->getBoolForKey("music");
	spBgMusicNormal->setVisible(bgMusicOn);
	spBgMusicSelect->setVisible(!bgMusicOn);
}
void Setting::setEffect(Ref*)
{
	effectOn = !effectOn;
	UserDefault::getInstance()->setBoolForKey("effect", effectOn);
	spEffectNormal->setVisible(effectOn);
	spEffectSelect->setVisible(!effectOn);

}
void Setting::setBgMusic(Ref*)
{
	bgMusicOn = !bgMusicOn;
	UserDefault::getInstance()->setBoolForKey("music", bgMusicOn);
	spBgMusicNormal->setVisible(bgMusicOn);
	spBgMusicSelect->setVisible(!bgMusicOn);
}

void Setting::menuBack( Ref *pSender )
{
	Director::getInstance()->popScene();
}

cocos2d::Scene * Setting::createScene()
{
	auto scene = Scene::create();
	auto layer = Setting::create();
	scene->addChild(layer);
	return scene;
}
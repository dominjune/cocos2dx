#include "AboutMenu.h"

USING_NS_CC;

About::About()
{

}

About::~About()
{

}

bool About::init() 
{
	Point origin = Director::getInstance()->getVisibleOrigin();
	Size visibleSize = Director::getInstance()->getVisibleSize();

	std::string resourceMain = "image/mainscene/";
	Sprite *pBg = Sprite::createWithTexture( 
		TextureCache::getInstance()->getTextureForKey(resourceMain + "bg.png")
	);
	pBg->setPosition(Point(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
	this->addChild(pBg, 0);

	Sprite *pZhy = 
		Sprite::createWithTexture(
		TextureCache::getInstance()->getTextureForKey(resourceMain + "zhy.jpg")
	); 

	pZhy->setPosition(Point(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2 + 50));
	this->addChild(pZhy, 2);

	Sprite *pWxb =
		Sprite::createWithTexture(
		TextureCache::getInstance()->getTextureForKey(resourceMain + "wxb.jpg")
	); 
	pWxb->setPosition(Point(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2 - 50));
	this->addChild(pWxb, 2);

	MenuItemImage *pBack = MenuItemImage::create(resourceMain + "backA.png", resourceMain + "backB.png",
		this, menu_selector(About::menuBack));
	pBack->setPosition(Point(origin.x + 20, origin.y + visibleSize.height - 20));
	Menu *pMenu = Menu::create(pBack, NULL);
	pMenu->setAnchorPoint(Point(0, 0)/*CCPointZero*/);
	pMenu->setPosition(Point(0, 0)/*CCPointZero*/);
	this->addChild(pMenu, 1);

	return true;
}

void About::menuBack( Ref *pSender )
{
	Director::getInstance()->popScene();
}

cocos2d::Scene * About::createScene()
{
	auto scene = Scene::create();
	auto layer = About::create();
	scene->addChild(layer);
	return scene;
}
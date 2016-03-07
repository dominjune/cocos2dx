#include "GameLayer.h"
#include "GameMap.h"
#include "Hero.h"
#include "AnimationManager.h"
#include "Item.h"
#include "MainScene.h"
#include "Global.h"
#include "SimpleAudioEngine.h"
#include "SelectLevel.h"

float GameLayer::mapMaxH;

Scene *GameLayer::createScene()
{
	Scene *scene = Scene::create();
	auto layer = GameLayer::create();
	scene->addChild(layer);
	return scene;
}

GameLayer::GameLayer() :
mainMap(NULL),
hero(NULL)
{
	mapMaxH = 0.0f;
	mapBeginPos = Point(0.0f, 96.0f);

	birthPoint.x = 180.0f;
	birthPoint.y = 32.0f;

	isKeyDownA = false;
	isKeyDownD = false;
	isSky = false;
	heroFireable = true;

	moveOffset = 0.0f;
	moveDelta = 0.0f;
	JumpOffset = 0.0f;

	ccMoveDelta = 0.05f;
	ccMoveOffset = 2.0f;
	ccJumpOffset = 0.3f;

	currentPos.x = 0.0f;
	currentPos.y = 0.0f;

	Size winsize = Director::getInstance()->getWinSize();
	heroAnchor = Point(winsize.width / 2 - 80, winsize.height / 2);

	backKeyPos = Point(84, 48);
	leftKeyPos = Point(40, 48);
	rightKeyPos = Point(128, 48);
	jumpKeyPos = Point(432, 35);
	fireKeyPos = Point(353, 35);
	MSetKeyPos = Point(260, 33);

	FireBallPos = Point(winsize.width - 70, winsize.height - 20);
	arrowPos = Point(winsize.width - 30, winsize.height - 20);

	mainLayer = Layer::create();
	mainLayer->retain();

	isPass = false;

	isLeftKeyDown = false;
	isRightKeyDown = false;
	isJumpKeyDown = false;
	isFireKeyDown = false;
}

GameLayer::~GameLayer()
{
	//Director::getInstance()->getOpenGLView()->SetWin32KeyLayer(NULL);

	// 这里面的几个指针全部都是通过create生成的，已经autorelease，交给引擎管理，不用手动删除
	//CC_SAFE_DELETE(hero);
	//CC_SAFE_DELETE(mainMap);
	//CC_SAFE_DELETE(mainLayer);

	this->unscheduleUpdate();
	this->unscheduleAllSelectors();
}

bool GameLayer::init()
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("OnLand.ogg", true);

	this->initHeroAndMap();

	this->initcontrolUI();

	this->initBulletUI();

	this->initRect();

	this->initSetMenu();

	//Director::getInstance()->getOpenGLView()->SetWin32KeyLayer(this);
	//this->registerWithTouchDispatcher();

	//后面检查
	//Director::getInstance()->getTouchDispatcher()->addStandardDelegate(this, kCCMenuHandlerPriority);
	auto touchMore = EventListenerTouchAllAtOnce::create();
	touchMore->onTouchesBegan = CC_CALLBACK_2(GameLayer::onTouchesBegan, this);
	touchMore->onTouchesMoved = CC_CALLBACK_2(GameLayer::onTouchesMoved, this);
	touchMore->onTouchesEnded = CC_CALLBACK_2(GameLayer::onTouchesEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchMore, this);

	//键盘事件
	auto keyboardListen = EventListenerKeyboard::create();
	keyboardListen->onKeyPressed = CC_CALLBACK_2(GameLayer::onKeyPressed, this);
	keyboardListen->onKeyReleased = CC_CALLBACK_2(GameLayer::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListen, this);

	this->setTouchEnabled(true);
	this->scheduleUpdate();

	return true;
}

void GameLayer::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	switch (keyCode)
	{
	case cocos2d::EventKeyboard::KeyCode::KEY_A:
		isLeftKeyDown = true;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_D:
		isRightKeyDown = true;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_J:
		isJumpKeyDown = true;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_K:
		isFireKeyDown = true;
		break;
	default:
		break;
	}
}

void GameLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	switch (keyCode)
	{
	case cocos2d::EventKeyboard::KeyCode::KEY_A:
		isLeftKeyDown = false;
		isKeyDownA = false;
		moveOffset = 0.0f;
		moveDelta = 0.0f;
		hero->setHeroState(eNormalLeft);
		pBackKeyImage->setDisplayFrame(pBackKeyNormal);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_D:
		isRightKeyDown = false;
		isKeyDownD = false;
		moveOffset = 0.0f;
		moveDelta = 0.0f;
		hero->setHeroState(eNormalRight);
		pBackKeyImage->setDisplayFrame(pBackKeyNormal);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_J:
		isJumpKeyDown = false;
		pJumpImage->setDisplayFrame(pAB_Normal);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_K:
		isFireKeyDown = false;
		pFireImage->setDisplayFrame(pAB_Normal);
		break;
	default:
		break;
	}
}

void GameLayer::initSetMenu()
{
	pColor = LayerColor::create(ccc4(0, 0, 0, 100));
	this->addChild(pColor, this->getChildrenCount());
	pColor->setVisible(false);

	pSetMenu = Sprite::create("image/Set_Menu.png");
	pSetMenu->setAnchorPoint(Point(129.0 / 248, 71.0 / 132));
	Point origin = Director::getInstance()->getVisibleOrigin();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	pSetMenu->setPosition(Point(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	this->addChild(pSetMenu, this->getChildrenCount() + 1);
	pSetMenu->setVisible(false);

	pResume = MenuItemImage::create("image/resume_n.png", "image/resume_s.png",
		this, menu_selector(GameLayer::menuResume));
	pResume->setPosition(Point(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + 40));
	pResume->setVisible(false);
	pResume->setEnabled(false);

	pReStart = MenuItemImage::create("image/restart_n.png", "image/restart_s.png",
		this, menu_selector(GameLayer::menuReStart));
	pReStart->setPosition(Point(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	pReStart->setVisible(false);
	pReStart->setEnabled(false);

	pSelectMenu = MenuItemImage::create("image/select_n.png", "image/select_s.png",
		this, menu_selector(GameLayer::menuSelectMenu));
	pSelectMenu->setPosition(Point(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 40));
	pSelectMenu->setVisible(false);
	pSelectMenu->setEnabled(false);

	pMenu->addChild(pResume, pMenu->getChildrenCount());
	pMenu->addChild(pReStart, pMenu->getChildrenCount());
	pMenu->addChild(pSelectMenu, pMenu->getChildrenCount());
}

void GameLayer::menuMSet(Ref *pSender)
{
	this->pauseGameLayer();
}

void GameLayer::menuResume(Ref *pSender)
{
	this->resumeGameLayer();
}

void GameLayer::menuReStart(Ref *pSender)
{
	Scene *pScene = Scene::create();
	pScene->addChild(GameLayer::create());
	Director::getInstance()->replaceScene(pScene);
	this->release();
}

void GameLayer::menuSelectMenu(Ref *pSender)
{
	Scene *pScene = Scene::create();
	pScene->addChild(SelectLevel::create());
	Director::getInstance()->replaceScene(pScene);
}

void GameLayer::initRect()
{
	leftKeyRect = CCRectMake(leftKeyPos.x - pLeftKey->getContentSize().width / 2,
		leftKeyPos.y - pLeftKey->getContentSize().height / 2,
		pLeftKey->getContentSize().width,
		pLeftKey->getContentSize().height);

	rightKeyRect = CCRectMake(rightKeyPos.x - pRightKey->getContentSize().width / 2,
		rightKeyPos.y - pRightKey->getContentSize().height / 2,
		pRightKey->getContentSize().width,
		pRightKey->getContentSize().height);

	jumpKeyRect = CCRectMake(jumpKeyPos.x - pJump->getContentSize().width / 2,
		jumpKeyPos.y - pJump->getContentSize().height / 2,
		pJump->getContentSize().width,
		pJump->getContentSize().height);

	fireKeyRect = CCRectMake(fireKeyPos.x - pFire->getContentSize().width / 2,
		fireKeyPos.y - pFire->getContentSize().height / 2,
		pFire->getContentSize().width,
		pFire->getContentSize().height);
}

void GameLayer::initHeroAndMap()
{
	char temp[30] = { 0 };//小心分配空间小了崩溃
	sprintf(temp, "image/tmx/MarioMap%d.tmx", Global::getInstance()->getCurrentLevel());
	mainMap = GameMap::create(temp);

	mapSize = CCSizeMake(mainMap->getMapSize().width * mainMap->getTileSize().width,
		mainMap->getMapSize().height * mainMap->getTileSize().height);
	mainMap->setPosition(Point(0, 0));
	mainLayer->addChild(mainMap);
	hero = Hero::create();
	hero->setBodyType(Global::getInstance()->currentHeroType);
	hero->setAnchorPoint(Point(0.5f, 0.0f));
	hero->setPosition(mainMap->getMarioBirthPos());
	heroSize = hero->getCurrentSize();
	mainLayer->addChild(hero);
	mainLayer->setPosition(mapBeginPos);
	this->addChild(mainLayer);
}

void GameLayer::loadMap()
{

}

void GameLayer::initcontrolUI()
{
	controlUI = Sprite::create("image/controlUI.png");
	controlUI->setAnchorPoint(Point(0, 0));
	this->addChild(controlUI);

	Size winSize = Director::getInstance()->getWinSize();

	pGameOverBack = Sprite::create("image/gameover.png");
	pGameOverBack->setPosition(Point(winSize.width / 2, winSize.height / 2 + 50));
	this->addChild(pGameOverBack);
	pGameOverBack->setVisible(false);

	pPassFailure = Sprite::create("image/PassFailure.png");
	pPassFailure->setPosition(Point(winSize.width / 2, winSize.height / 2 + 50));
	this->addChild(pPassFailure);
	pPassFailure->setVisible(false);

	pPassSuccess = Sprite::create("image/PassSuccess.png");
	pPassSuccess->setPosition(Point(winSize.width / 2, winSize.height / 2 + 50));
	this->addChild(pPassSuccess);
	pPassSuccess->setVisible(false);


	winSize = Director::getInstance()->getWinSize();

	pBackKeyImage = Sprite::create("image/backKeyImage.png");
	pBackKeyImage->setPosition(backKeyPos);
	this->addChild(pBackKeyImage);

	pBackKeyNormal = SpriteFrame::create("image/backKeyImage.png", CCRectMake(0, 0, 72, 72));
	pBackKeyNormal->retain();
	pBackKeyLeft = SpriteFrame::create("image/backKeyLeft.png", CCRectMake(0, 0, 72, 72));
	pBackKeyLeft->retain();
	pBackKeyRight = SpriteFrame::create("image/backKeyRight.png", CCRectMake(0, 0, 72, 72));
	pBackKeyRight->retain();

	pAB_Normal = SpriteFrame::create("image/AB_normal.png", CCRectMake(0, 0, 72, 50));
	pAB_Normal->retain();
	pAB_Selected = SpriteFrame::create("image/AB_select.png", CCRectMake(0, 0, 72, 50));
	pAB_Selected->retain();

	pJumpImage = Sprite::createWithSpriteFrame(pAB_Normal);
	pJumpImage->setPosition(jumpKeyPos);
	this->addChild(pJumpImage, 3);

	pFireImage = Sprite::createWithSpriteFrame(pAB_Normal);
	pFireImage->setPosition(fireKeyPos);
	this->addChild(pFireImage, 3);

	pLeftKey = MenuItemImage::create("image/leftright.png", "image/leftright.png",
		this, menu_selector(GameLayer::menuCallBackLeftKey));
	pRightKey = MenuItemImage::create("image/leftright.png", "image/leftright.png",
		this, menu_selector(GameLayer::menuCallBackRightKey));
	pJump = MenuItemImage::create("image/AB_normal.png", "image/AB_select.png",
		this, menu_selector(GameLayer::menuCallBackJumpKey));
	pFire = MenuItemImage::create("image/AB_normal.png", "image/AB_select.png",
		this, menu_selector(GameLayer::menuCallBackFireKey));
	pMSet = MenuItemImage::create("image/M_n.png", "image/M_s.png",
		this, menu_selector(GameLayer::menuMSet));

	pBackToMenu = MenuItemImage::create("image/backToMenu.png", "image/backToMenu.png",
		this, menu_selector(GameLayer::menuCallBackBackToMenu));
	pBackToMenu->setEnabled(false);
	pBackToMenu->setVisible(false);

	pNext = MenuItemImage::create("image/nextlevel_normal.png", "image/nextlevel_select.png",
		this, menu_selector(GameLayer::menuNext));
	pNext->setPosition(Point(winSize.width / 2, winSize.height / 2 + 40));
	pNext->setVisible(false);
	pNext->setEnabled(false);

	pRetry = MenuItemImage::create("image/retry_normal.png", "image/retry_select.png",
		this, menu_selector(GameLayer::menuRetry));
	pRetry->setPosition(Point(winSize.width / 2, winSize.height / 2));
	pRetry->setVisible(false);
	pRetry->setEnabled(false);

	pLeftKey->setPosition(leftKeyPos);
	pRightKey->setPosition(rightKeyPos);
	pJump->setPosition(jumpKeyPos);
	pFire->setPosition(fireKeyPos);
	pMSet->setPosition(MSetKeyPos);
	pBackToMenu->setPosition(Point(winSize.width / 2, winSize.height / 2 + 20));

	pMenu = Menu::create(pNext, pRetry, pMSet, NULL);
	pMenu->setAnchorPoint(Point(0, 0)/*CCPointZero*/);
	pMenu->setPosition(Point(0, 0)/*CCPointZero*/);

	this->addChild(pMenu, 100);
}

void GameLayer::initBulletUI()
{
	pBulletBorderArrow = Sprite::create("image/bulletBorder.png");
	pBulletBorderArrow->setPosition(arrowPos);
	//pBulletBorderArrow->setScale(2.0f);

	pBulletBorderFireBall = Sprite::create("image/bulletBorder.png");
	pBulletBorderFireBall->setPosition(FireBallPos);

	Sprite *pArrow = Sprite::create("image/arrowBullet.png");

	pMenuArrow = MenuItemSprite::create(pArrow, pArrow, pArrow,
		this, menu_selector(GameLayer::menuCallBackArrow));
	pMenuArrow->setPosition(arrowPos);
	pMenu->addChild(pMenuArrow);

	Sprite *pFireBall = Sprite::create("image/fireBall.png");
	//pFireBall->setScale(2.0f);
	pMenuFireBall = MenuItemSprite::create(pFireBall, pFireBall, pFireBall,
		this, menu_selector(GameLayer::menuCallBackFireBall));
	pMenuFireBall->setPosition(FireBallPos);
	pMenu->addChild(pMenuFireBall);
}

void GameLayer::menuCallBackLeftKey(Ref *pSender)
{
	isKeyDownA = false;
	moveOffset = 0.0f;
	moveDelta = 0.0f;
	hero->setHeroState(eNormalLeft);
	pBackKeyImage->setDisplayFrame(pBackKeyNormal);
}

void GameLayer::menuCallBackRightKey(Ref *pSender)
{
	isKeyDownD = false;
	moveOffset = 0.0f;
	moveDelta = 0.0f;
	hero->setHeroState(eNormalRight);
	pBackKeyImage->setDisplayFrame(pBackKeyNormal);
}

void GameLayer::menuCallBackJumpKey(Ref *pSender)
{

}

void GameLayer::menuCallBackFireKey(Ref *pSender)
{

}

void GameLayer::menuCallBackArrow(Ref *pSender)
{
	if (hero->isBulletable())
	{
		Global::getInstance()->setCurrentBulletType(eBullet_arrow);
		hero->setBulletType(eBullet_arrow);
	}
}

void GameLayer::menuCallBackFireBall(Ref *pSender)
{
	if (hero->isBulletable())
	{
		Global::getInstance()->setCurrentBulletType(eBullet_arrow);
		hero->setBulletType(eBullet_arrow);
	}
}

void GameLayer::menuCallBackBackToMenu(Ref *pSender)
{
	this->toMainMenu();
}

void GameLayer::onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event)
{
	for (auto pTouch : touches)
	{
		Point touchPos = pTouch->getLocation();
		if (leftKeyRect.containsPoint(touchPos))
		{
			isLeftKeyDown = true;
		}
		if (rightKeyRect.containsPoint(touchPos))
		{
			isRightKeyDown = true;
		}
		if (jumpKeyRect.containsPoint(touchPos))
		{
			isJumpKeyDown = true;
		}
		if (fireKeyRect.containsPoint(touchPos))
		{
			isFireKeyDown = true;
		}
	}
}

void GameLayer::onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event)
{
	for (auto pTouch : touches)
	{
		Point touchPos = pTouch->getLocation();
		if (leftKeyRect.containsPoint(touchPos))
		{
			isLeftKeyDown = false;
			hero->setHeroState(eNormalLeft);
			pBackKeyImage->setDisplayFrame(pBackKeyNormal);
		}
		if (rightKeyRect.containsPoint(touchPos))
		{
			isRightKeyDown = false;
			hero->setHeroState(eNormalRight);
			pBackKeyImage->setDisplayFrame(pBackKeyNormal);
		}
		if (jumpKeyRect.containsPoint(touchPos))
		{
			isJumpKeyDown = false;
			pJumpImage->setDisplayFrame(pAB_Normal);
		}
		if (fireKeyRect.containsPoint(touchPos))
		{
			isFireKeyDown = false;
			pFireImage->setDisplayFrame(pAB_Normal);
		}
	}
}

void GameLayer::onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event)
{
	if (isLeftKeyDown)
	{
		bool flag = false;
		for (auto pTouch : touches)
		{
			Point touchPos = pTouch->getLocation();

			if (leftKeyRect.containsPoint(touchPos))
			{
				flag = true;
			}
		}
		if (flag == false)
		{
			isLeftKeyDown = false;
			isKeyDownA = false;
			moveOffset = 0.0f;
			moveDelta = 0.0f;
			hero->setHeroState(eNormalLeft);
			pBackKeyImage->setDisplayFrame(pBackKeyNormal);
		}
	}

	if (isRightKeyDown)
	{
		bool flag = false;
		for (auto pTouch : touches)
		{
			Point touchPos = pTouch->getLocation();

			if (rightKeyRect.containsPoint(touchPos))
			{
				flag = true;
			}
		}
		if (flag == false)
		{
			isRightKeyDown = false;
			isKeyDownD = false;
			moveOffset = 0.0f;
			moveDelta = 0.0f;
			hero->setHeroState(eNormalRight);
			pBackKeyImage->setDisplayFrame(pBackKeyNormal);
		}
	}

	if (isJumpKeyDown)
	{
		bool flag = false;
		for (auto pTouch : touches)
		{
			Point touchPos = pTouch->getLocation();

			if (jumpKeyRect.containsPoint(touchPos))
			{
				flag = true;
			}
		}
		if (flag == false)
		{
			isJumpKeyDown = false;
			pJumpImage->setDisplayFrame(pAB_Normal);
		}
	}

	if (isFireKeyDown)
	{
		bool flag = false;
		for (auto pTouch : touches)
		{
			Point touchPos = pTouch->getLocation();

			if (fireKeyRect.containsPoint(touchPos))
			{
				flag = true;
			}
		}
		if (flag == false)
		{
			isFireKeyDown = false;
			pFireImage->setDisplayFrame(pAB_Normal);
		}
	}
}

void GameLayer::onTouchesCancelled(const std::vector<Touch*>&touches, Event *unused_event)
{

}

//void GameLayer::registerWithTouchDispatcher()
//{
//	Layer::registerWithTouchDispatcher();
//	Director::getInstance()->getTouchDispatcher()->addStandardDelegate(this, 0);
//}

void GameLayer::menuCloseCallBack(Ref *pSender)
{
	this->toMainMenu();
}

//void GameLayer::processWin32KeyPress(UINT message, WPARAM wParam, LPARAM lParam)
//{
//	if (hero->isHeroDied())
//	{
//		return ;
//	}
//	switch (message)
//	{
//	case WM_KEYDOWN:
//		if (wParam == 65)
//		{
//			isKeyDownA = true;
//			moveOffset = -ccMoveOffset;
//			moveDelta = -ccMoveDelta;
//			hero->setHeroState(eLeft);
//		}
//		else if (wParam == 68)
//		{
//			isKeyDownD = true;
//			moveOffset = ccMoveOffset;
//			moveDelta = ccMoveDelta;
//			hero->setHeroState(eRight);
//		}
//		else if (wParam == 32)
//		{
//			hero->gadgetable = false;
//			if (!isSky)
//			{
//				CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("Jump.wma");
//				JumpOffset = 6.0f;
//				isSky = true;
//				hero->isFlying = true;
//			}
//		}
//		else if (wParam == 73)
//		{
//			// 发射子弹
//			if (hero->isBulletable())
//			{
//				if (heroFireable)
//				{
//					mainMap->createNewBullet();
//					hero->fireAction();
//					heroFireable = false;
//					DelayTime *pDelay = DelayTime::create(0.5f);
//					this->runAction(Sequence::create(pDelay, 
//						CallFunc::create(this, callfunc_selector(GameLayer::reSetHeroFireable)), NULL));
//
//				}
//			}
//		}
//		break;
//	case WM_KEYUP:
//		if (wParam == 65)
//		{
//			isKeyDownA = false;
//			moveOffset = 0.0f;
//			moveDelta = 0.0f;
//			hero->setHeroState(eNormalLeft);
//		}
//		else if (wParam == 68)
//		{
//			isKeyDownD = false;
//			moveOffset = 0.0f;
//			moveDelta = 0.0f;
//			hero->setHeroState(eNormalRight);
//		}
//		break;
//	}
//}




void GameLayer::updateControl()
{
	if (!hero->isHeroDied())
	{
		if (/*pLeftKey->isSelected()*/ isLeftKeyDown)
		{
			isKeyDownA = true;
			moveOffset = -ccMoveOffset;
			moveDelta = -ccMoveDelta;
			hero->setHeroState(eLeft);
			pBackKeyImage->setDisplayFrame(pBackKeyLeft);
		}
		else if (/*pRightKey->isSelected()*/ isRightKeyDown)
		{
			isKeyDownD = true;
			moveOffset = ccMoveOffset;
			moveDelta = ccMoveDelta;
			hero->setHeroState(eRight);
			pBackKeyImage->setDisplayFrame(pBackKeyRight);
		}
		if (/*pJump->isSelected()*/ isJumpKeyDown)
		{
			hero->gadgetable = false;
			if (!isSky)
			{
				CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("Jump.ogg");
				JumpOffset = 6.0f;
				isSky = true;
				hero->isFlying = true;
			}
			pJumpImage->setDisplayFrame(pAB_Selected);
		}
		if (/*pFire->isSelected()*/ isFireKeyDown)
		{
			// 发射子弹
			if (hero->isBulletable())
			{
				if (heroFireable)
				{
					CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("RengHuoQiu.ogg");
					mainMap->createNewBullet();
					hero->fireAction();
					heroFireable = false;
					DelayTime *pDelay = DelayTime::create(0.5f);
					this->runAction(Sequence::create(pDelay,
						CallFunc::create(this, callfunc_selector(GameLayer::reSetHeroFireable)), NULL));

				}
			}
			pFireImage->setDisplayFrame(pAB_Selected);
		}
	}

}

void GameLayer::toMainMenu()
{
	Scene *pScene = Scene::create();
	pScene->addChild(MainScene::create());
	Director::getInstance()->replaceScene(pScene);
}

void GameLayer::stopForPassFailure()
{
	if (Global::getInstance()->getCurrentLifeNum() == 0)
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(true);
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("GameOver.ogg");
		//Global::getInstance()->setWhyToMainMenu(efor_GameOver);
		pGameOverBack->setVisible(true);
	}
	else
	{
		//Global::getInstance()->setWhyToMainMenu(efor_PassFailure);
		pPassFailure->setVisible(true);
	}

	mainMap->stopUpdateForHeroDie();
	this->unscheduleUpdate();
	this->reSetKeyNo();

	DelayTime *pDelay = DelayTime::create(3);
	this->runAction(Sequence::create(pDelay,
		CallFunc::create(this, callfunc_selector(GameLayer::reShowPassFailure)), NULL));
}

void GameLayer::reSetKeyNo()
{
	//pFire->setEnabled(false);
	//pJump->setEnabled(false);
	pMenuFireBall->setEnabled(false);
	pMenuArrow->setEnabled(false);
}

void GameLayer::reShowPassFailure()
{
	if (Global::getInstance()->getCurrentLifeNum() == 0)
	{
		this->toMainMenu();
	}
	else
	{
		pRetry->setVisible(true);
		pRetry->setEnabled(true);
	}
	//pBackToMenu->setEnabled(true);
	//pBackToMenu->setVisible(true);
}

void GameLayer::stopForPassSuccess()
{
	int level = Global::getInstance()->getCurrentLevel();
	CCString *str = CCString::createWithFormat("Level%d", level + 1);
	UserDefault::sharedUserDefault()->setStringForKey(str->getCString(), "yes");


	mainMap->stopUpdateForHeroDie();
	this->unscheduleUpdate();

	DelayTime *pDelay = DelayTime::create(3);
	this->runAction(Sequence::create(pDelay,
		CallFunc::create(this, callfunc_selector(GameLayer::reShowPassSuccess)), NULL));

}

void GameLayer::reShowPassSuccess()
{
	pPassSuccess->setVisible(true);
	//pBackToMenu->setEnabled(true);
	//pBackToMenu->setVisible(true);


	if (Global::getInstance()->getCurrentLevel() == Global::getInstance()->getTotalLevels())
	{
		this->showPassAll();
	}
	else
	{
		pNext->setVisible(true);
		pNext->setEnabled(true);
	}


}

void GameLayer::showHeroJump()
{
	hero->reSetForSuccess();
	CCJumpTo *pJump = CCJumpTo::create(1.0f, mainMap->finalPoint, 32, 3);
	hero->runAction(pJump);
}

void GameLayer::update(float dt)
{
	// 是否死亡判断
	if (hero->isHeroDied())
	{
		this->stopForPassFailure();
		return;
	}
	// 是否通关判断
	if (isPass)
	{
		this->stopForPassSuccess();
		return;
	}

	this->updateControl();

	currentPos = hero->getPosition();
	heroSize = hero->getContentSize();

	if (hero->isGadgetable())
	{
		currentPos.x += moveOffset + mainMap->heroInGadget->getMoveOffset();
		currentPos.y += JumpOffset + mainMap->heroInGadget->getJumpOffset();
	}
	else
	{
		currentPos.x += moveOffset;
		currentPos.y += JumpOffset;
	}
	if (isSky)
	{
		switch (hero->face)
		{
		case eLeft:
			hero->setHeroState(eJumpLeft);
			break;
		case eRight:
			hero->setHeroState(eJumpRight);
			break;
		default:
			break;
		}
	}
	updateSpeed();
	hero->setPosition(currentPos);
	setSceneScrollPosition();
	collistionV();
	collistionH();
}

void GameLayer::updateSpeed()
{
	if (isFireKeyDown)
	{
	//	moveOffset = 3;
	}
	else
	{
	//	moveOffset = 2;
	}
}

void GameLayer::updateContent(float dt)
{
	currentPos = hero->getPosition();
	currentPos.x = currentPos.x + moveOffset + moveDelta;
	hero->setPosition(currentPos);
	setSceneScrollPosition();
	collistionH();
}

void GameLayer::updateDelta(float dt)
{
	//if (isKeyDownA)
	//{
	//	moveDelta -= ccMoveDelta;
	//}
	//else if (isKeyDownD)
	//{
	//	moveDelta += ccMoveDelta;
	//}
}

// 水平方向碰撞检测
// 悬浮在半空中大约一秒钟的bug应该是水平碰撞的问题，已解决
void GameLayer::collistionH()
{
	Point currentPos = hero->getPosition();
	// 做判断不让主角移除屏幕的左侧
	if ((currentPos.x - heroSize.width / 2 - mapMaxH) <= 0)
	{
		Point pp = Point(mapMaxH + heroSize.width / 2, currentPos.y);
		hero->setPosition(pp);
		return;
	}

	bool flag = false;
	// 右侧判断
	Point rightCollision = Point(currentPos.x + heroSize.width / 2, currentPos.y /* + heroSize.height/2*/);
	Point rightTileCoord = mainMap->positionToTileCoord(rightCollision);
	if (mainMap->isMarioEatMushroom(rightTileCoord))
	{
		hero->changeForGotMushroom();
	}
	if (mainMap->isMarioEatAddLifeMushroom(rightTileCoord))
	{
		hero->changeForGotAddLifeMushroom();
	}
	Point rightPos = mainMap->tilecoordToPosition(rightTileCoord);
	rightPos = Point(rightPos.x - heroSize.width / 2, currentPos.y);

	TileType tileType = mainMap->tileTypeforPos(rightTileCoord);
	switch (tileType)
	{
	case eTile_Block:
	case eTile_Pipe:
	case eTile_Land:
		hero->setPosition(rightPos);
		flag = true;
		break;
	case eTile_Flagpole:
	{
		isPass = true;
		mainMap->showFlagMove();
		this->showHeroJump();
		return;
		break;
	}
	case eTile_Coin:
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("EatCoin.ogg");
		mainMap->getCoinLayer()->removeTileAt(rightTileCoord);
		break;
	default:
		break;
	}

	// 主角的左侧碰撞检测点
	Point leftCollision = Point(currentPos.x - heroSize.width / 2, currentPos.y /* + heroSize.height/2*/);
	Point leftTileCoord = mainMap->positionToTileCoord(leftCollision);
	if (mainMap->isMarioEatMushroom(leftTileCoord))
	{
		hero->changeForGotMushroom();
	}
	if (mainMap->isMarioEatAddLifeMushroom(leftTileCoord))
	{
		hero->changeForGotAddLifeMushroom();
	}
	Point leftPos = mainMap->tilecoordToPosition(leftTileCoord);
	leftPos = Point(leftPos.x + heroSize.width / 2 + mainMap->getTileSize().width, currentPos.y);
	tileType = mainMap->tileTypeforPos(leftTileCoord);
	switch (tileType)
	{
	case eTile_Block:
	case eTile_Pipe:
	case eTile_Land:
		hero->setPosition(leftPos);
		flag = true;
		break;
	case eTile_Coin:
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("EatCoin.ogg");
		mainMap->getCoinLayer()->removeTileAt(leftTileCoord);
		break;
	case eTile_Flagpole:
	{
		isPass = true;
		mainMap->showFlagMove();
		this->showHeroJump();
		return;
		break;
	}
	default:
		break;
	}
}

// 垂直方向碰撞检测
void GameLayer::collistionV()
{
	Point currentPos = hero->getPosition();

	// 下面是判断马里奥的脚底是否已经接近地图下边缘的5像素处，以此为die
	if (currentPos.y <= 0)
	{
		hero->setHeroDie(true);
		hero->setPosition(Point(currentPos.x, 1));
		hero->dieForTrap();  // 执行一段动画，表示因掉入陷阱而死
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("DiaoRuXianJingSi.ogg");
		return;
	}

	// 下面是判断马里奥是否头顶越过游戏画面最上层
	if (currentPos.y > mapSize.height - heroSize.height - 2)
	{
		JumpOffset = 0.0f;
		hero->setPosition(Point(currentPos.x, mapSize.height - heroSize.height - 2));
		isSky = false;

		return;
	}

	// 主角头顶的碰撞检测
	for (int heroIdx = 6; heroIdx <= heroSize.width - 6; ++heroIdx)
	{
		Point upCollision = Point(currentPos.x - heroSize.width / 2 + heroIdx, currentPos.y + heroSize.height);
		Point upTileCoord = mainMap->positionToTileCoord(upCollision);
		// 判断是否吃到蘑菇
		if (mainMap->isMarioEatMushroom(upTileCoord))
		{
			hero->changeForGotMushroom();
		}
		if (mainMap->isMarioEatAddLifeMushroom(upTileCoord))
		{
			hero->changeForGotAddLifeMushroom();
		}
		Point upPos = mainMap->tilecoordToPosition(upTileCoord);
		upPos = Point(currentPos.x, upPos.y - heroSize.height);
		TileType tileType = mainMap->tileTypeforPos(upTileCoord);
		bool flagUp = false;
		switch (tileType)
		{
		case eTile_Block:
		case eTile_Land:
			if (JumpOffset > 0)
			{
				mainMap->breakBlock(upTileCoord, hero->getCurrentBodyType());
				JumpOffset = 0.0f;
				hero->setPosition(upPos);
				flagUp = true;
			}
			break;
		case eTile_Coin:
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("EatCoin.ogg");
			mainMap->getCoinLayer()->removeTileAt(upTileCoord);
			break;
		default:
			break;
		}
		if (flagUp)
		{
			JumpOffset -= ccJumpOffset;
			return;
		}
	}

	float heroLeftSide = currentPos.x - heroSize.width / 2;

	for (int heroIdx = 4; heroIdx <= heroSize.width - 4; ++heroIdx)
	{
		Point downCollision = Point(heroLeftSide + heroIdx, currentPos.y);
		Point downTileCoord = mainMap->positionToTileCoord(downCollision);
		if (mainMap->isMarioEatMushroom(downTileCoord))
		{
			hero->changeForGotMushroom();
		}
		if (mainMap->isMarioEatAddLifeMushroom(downTileCoord))
		{
			hero->changeForGotAddLifeMushroom();
		}
		downTileCoord.y += 1;
		Point downPos = mainMap->tilecoordToPosition(downTileCoord);
		downPos = Point(currentPos.x, downPos.y + mainMap->getTileSize().height);

		TileType tileType = mainMap->tileTypeforPos(downTileCoord);
		bool flagDown = false;
		switch (tileType)
		{
		case eTile_Flagpole:
		{
			isPass = true;
			mainMap->showFlagMove();
			this->showHeroJump();
			return;
			break;
		}
		case eTile_Coin:
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("EatCoin.ogg");
			mainMap->getCoinLayer()->removeTileAt(downTileCoord);
			break;
			//case eTile_Trap:
			//	hero->setPosition(Point(currentPos.x, 16.0f));
			//	hero->setHeroDie(true);
			//	return ;
			//	break;
		case eTile_Land:
		case eTile_Pipe:
		case eTile_Block:
		{
			if (JumpOffset < 0)
			{
				hero->setGadgetable(false);
				JumpOffset = 0.0f;
				hero->setPosition(downPos);
				isSky = false;
				hero->isFlying = false;
				switch (hero->face)
				{
				case eLeft:
					if (isKeyDownA)
					{
						hero->setHeroState(eLeft);
					}
					else
					{
						hero->setHeroState(eNormalLeft);
					}
					break;
				case eRight:
					if (isKeyDownD)
					{
						hero->setHeroState(eRight);
					}
					else
					{
						hero->setHeroState(eNormalRight);
					}
					break;
				default:
					break;
				}
			}
			flagDown = true;
		}
			break;
		default:
			break;
		}
		if (flagDown)
		{
			return;
		}

		// 检测主角是否站在梯子上
		float gadgetLevel = 0.0f;
		if (mainMap->isHeroInGadget(downCollision, gadgetLevel))
		{
			JumpOffset = 0.0f;
			downPos = Point(currentPos.x, gadgetLevel);
			hero->setPosition(downPos);
			hero->setGadgetable(true);
			isSky = false;
			hero->isFlying = false;
			switch (hero->face)
			{
			case eLeft:
				if (isKeyDownA)
				{
					hero->setHeroState(eLeft);
				}
				else
				{
					hero->setHeroState(eNormalLeft);
				}
				break;
			case eRight:
				if (isKeyDownD)
				{
					hero->setHeroState(eRight);
				}
				else
				{
					hero->setHeroState(eNormalRight);
				}
				break;
			default:
				break;
			}
			return;
		}
		else
		{
			hero->setGadgetable(false);
		}
	}

	JumpOffset -= ccJumpOffset;
}

void GameLayer::setSceneScrollPosition()
{
	Point pos = hero->getPosition();
	Size winsize = Director::getInstance()->getWinSize();

	float x = MAX(pos.x, heroAnchor.x);
	float y = MAX(pos.y, heroAnchor.y);

	x = MIN(x, mapSize.width - winsize.width / 2 - 80);
	y = MIN(y, mapSize.height - winsize.height / 2);
	Point actualPosition = Point(x, y);

	Point viewPoint = ccpSub(heroAnchor, actualPosition);

	// 使得地图不能倒退回去
	if (fabsf(viewPoint.x) <= mapMaxH)
	{
		return;
	}
	else
	{
		viewPoint.y = 96;
		mainLayer->setPosition(viewPoint);
		x = mainLayer->getPosition().x;
		mapMaxH = fabsf(x);
	}
}

float GameLayer::getMapMaxH()
{
	return mapMaxH;
}

void GameLayer::reSetHeroFireable()
{
	heroFireable = true;
}

void GameLayer::menuRetry(Ref *pSender)
{
	Scene *pScene = Scene::create();
	pScene->addChild(GameLayer::create());
	Director::getInstance()->replaceScene(pScene);
}

void GameLayer::menuNext(Ref *pSender)
{
	Global::getInstance()->currentLevelPlusOne();
	Scene *pScene = Scene::create();
	pScene->addChild(GameLayer::create());
	Director::getInstance()->replaceScene(pScene);
}

void GameLayer::showPassAll()
{
	CCLabelTTF *pPassAll = CCLabelTTF::create("You Pass All", "Arial", 40);
	Point origin = Director::getInstance()->getVisibleOrigin();
	Size visibleSize = Director::getInstance()->getVisibleSize();

	pPassAll->setPosition(Point(origin.x + visibleSize.width / 2, origin.y + visibleSize.height));
	this->addChild(pPassAll, this->getChildrenCount());
	CCMoveTo *pTo = CCMoveTo::create(0.3f, Point(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	DelayTime *pDelay = DelayTime::create(2.0f);
	pPassAll->runAction(Sequence::create(pTo, pDelay,
		CallFunc::create(this, callfunc_selector(GameLayer::toMainMenu)), NULL));
}

void GameLayer::pauseGameLayer()
{
	mainMap->pauseGameMap();
	this->unscheduleUpdate();

	//pLeftKey->setEnabled(false);
	//pRightKey->setEnabled(false);
	//pJump->setEnabled(false);
	//pFire->setEnabled(false);
	pMenuArrow->setEnabled(false);
	pMenuFireBall->setEnabled(false);

	pColor->setVisible(true);
	pSetMenu->setVisible(true);
	pResume->setVisible(true);
	pResume->setEnabled(true);
	pReStart->setVisible(true);
	pReStart->setEnabled(true);
	pSelectMenu->setVisible(true);
	pSelectMenu->setEnabled(true);
}

void GameLayer::resumeGameLayer()
{
	mainMap->resumeGameMap();
	this->scheduleUpdate();

	//pLeftKey->setEnabled(true);
	//pRightKey->setEnabled(true);
	//pJump->setEnabled(true);
	//pFire->setEnabled(true);
	pMenuArrow->setEnabled(true);
	pMenuFireBall->setEnabled(true);

	pColor->setVisible(false);
	pSetMenu->setVisible(false);
	pResume->setVisible(false);
	pResume->setEnabled(false);
	pReStart->setVisible(false);
	pReStart->setEnabled(false);
	pSelectMenu->setVisible(false);
	pSelectMenu->setEnabled(false);
}






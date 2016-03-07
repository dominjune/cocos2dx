#include "Hero.h"
#include "AnimationManager.h"
#include "Global.h"
#include "SimpleAudioEngine.h"

Hero* Hero::_heroInstance;
Hero* Hero::getInstance()
{
	return _heroInstance;
}

Hero::Hero() :
mainBody(NULL),
mainTemp(NULL),
_jumpLeft(NULL),
_jumpRight(NULL),
_lifeOverSmall(NULL),
_lifeOverNormal(NULL),
_lifeOverFire(NULL),
_normalLeft(NULL),
_normalRight(NULL),
_smallJumpLeft(NULL),
_smallJumpRight(NULL),
_smallLeft(NULL),
_smallRight(NULL),
pLabelUp(NULL)
{
	norBodySize = CCSizeMake(18, 32);
	smallSize = CCSizeMake(14, 16);
	currentSize = smallSize;
	state = eNormalRight;
	statePre = eNormalRight;
	face = eRight;
	isFlying = false;
	bodyType = eBody_Small;

	CCTexture2D *pTexture = TextureCache::getInstance()->addImage("image/hero/walkLeft.png");
	_jumpLeft = SpriteFrame::createWithTexture(pTexture, CCRectMake(180, 0, 18, 32));
	_jumpLeft->retain();
	_normalLeft = SpriteFrame::createWithTexture(pTexture, CCRectMake(18 * 9, 0, 18, 32));
	_normalLeft->retain();

	pTexture = TextureCache::getInstance()->addImage("image/hero/WalkLeft_fire.png");
	_jumpLeftFire = SpriteFrame::createWithTexture(pTexture, CCRectMake(180, 0, 18, 32));
	_jumpLeftFire->retain();
	_normalLeftFire = SpriteFrame::createWithTexture(pTexture, CCRectMake(18 * 9, 0, 18, 32));
	_normalLeftFire->retain();

	pTexture = TextureCache::getInstance()->addImage("image/hero/WalkRight_fire.png");
	_jumpRightFire = SpriteFrame::createWithTexture(pTexture, CCRectMake(180, 0, 18, 32));
	_jumpRightFire->retain();
	_normalRightFire = SpriteFrame::createWithTexture(pTexture, CCRectMake(0, 0, 18, 32));
	_normalRightFire->retain();

	pTexture = TextureCache::getInstance()->addImage("image/hero/walkRight.png");
	_jumpRight = SpriteFrame::createWithTexture(pTexture, CCRectMake(180, 0, 18, 32));
	_jumpRight->retain();
	_normalRight = SpriteFrame::createWithTexture(pTexture, CCRectMake(0, 0, 18, 32));
	_normalRight->retain();

	pTexture = TextureCache::getInstance()->addImage("image/hero/smallWalkRight.png");
	_smallRight = SpriteFrame::createWithTexture(pTexture, CCRectMake(0, 0, 14, 16));
	_smallRight->retain();
	_smallJumpRight = SpriteFrame::createWithTexture(pTexture, CCRectMake(140, 0, 14, 16));
	_smallJumpRight->retain();

	pTexture = TextureCache::getInstance()->addImage("image/hero/smallWalkLeft.png");
	_smallLeft = SpriteFrame::createWithTexture(pTexture, CCRectMake(14 * 9, 0, 14, 16));
	_smallLeft->retain();
	_smallJumpLeft = SpriteFrame::createWithTexture(pTexture, CCRectMake(140, 0, 14, 16));
	_smallJumpLeft->retain();

	pTexture = TextureCache::getInstance()->addImage("image/hero/small_die.png");
	_lifeOverSmall = SpriteFrame::createWithTexture(pTexture, CCRectMake(16, 0, 16, 18));
	_lifeOverSmall->retain();

	pTexture = TextureCache::getInstance()->addImage("image/hero/normal_die.png");
	_lifeOverNormal = SpriteFrame::createWithTexture(pTexture, CCRectMake(24, 0, 24, 34));
	_lifeOverNormal->retain();

	pTexture = TextureCache::getInstance()->addImage("image/hero/fire_die.png");
	_lifeOverFire = SpriteFrame::createWithTexture(pTexture, CCRectMake(24, 0, 24, 34));
	_lifeOverFire->retain();

	pTexture = TextureCache::getInstance()->addImage("image/hero/allow_walkLeft.png");
	_normalLeftArrow = SpriteFrame::createWithTexture(pTexture, CCRectMake(0, 0, 18, 32));
	_normalLeftArrow->retain();
	_jumpLeftArrow = SpriteFrame::createWithTexture(pTexture, CCRectMake(180, 0, 18, 32));
	_jumpLeftArrow->retain();

	pTexture = TextureCache::getInstance()->addImage("image/hero/allow_walkRight.png");
	_normalRightArrow = SpriteFrame::createWithTexture(pTexture, CCRectMake(0, 0, 18, 32));
	_normalRightArrow->retain();
	_jumpRightArrow = SpriteFrame::createWithTexture(pTexture, CCRectMake(180, 0, 18, 32));
	_jumpRightArrow->retain();

	isDied = false;

	_heroInstance = this;

	isSafeTime = false;

	bulletable = false;

	gadgetable = false;

	currentBulletType = eBullet_common;

	_heroInstance = this;
}

Hero::~Hero()
{
	this->unscheduleAllSelectors();

	// �������Щָ�붼��ͨ��create���ɵģ��Ѿ�����������������Բ���Ҫ�ֶ��ͷ�
	//_jumpLeft->release();
	//_jumpRight->release();
	//_lifeOver->release();
	//_normalLeft->release();
	//_normalRight->release();
	//_smallJumpLeft->release();
	//_smallJumpRight->release();
	//_smallLeft->release();
	//_smallRight->release();


	/*	CC_SAFE_DELETE(_jumpLeft);
	CC_SAFE_DELETE(_jumpRight);
	CC_SAFE_DELETE(_lifeOver);
	CC_SAFE_DELETE(_normalLeft);
	CC_SAFE_DELETE(_normalRight);
	CC_SAFE_DELETE(_smallJumpLeft);
	CC_SAFE_DELETE(_smallJumpRight);
	CC_SAFE_DELETE(_smallLeft);
	CC_SAFE_DELETE(_smallRight);*/
}

void Hero::setHeroState(marioDirection _state)
{
	if (isDied)
	{
		return;
	}
	if (state == _state)
	{
		return;
	}
	statePre = state;
	state = _state;

	mainBody->stopAllActions();
	switch (_state)
	{
	case eFireTheHole:
	{

		break;
	}
	case eNormalRight:
	{
		if (bodyType == eBody_Normal)
		{
			if (bulletable)
			{
				switch (Global::getInstance()->getCurrentBulletType())
				{
				case eBullet_common:
					mainBody->setDisplayFrame(_normalRightFire);
					break;
				case eBullet_arrow:
					mainBody->setDisplayFrame(_normalRightArrow);
					break;
				}
			}
			else
			{
				mainBody->setDisplayFrame(_normalRight);
			}
		}
		else
		{
			mainBody->setDisplayFrame(_smallRight);
		}
		face = eRight;
		break;
	}
	case eNormalLeft:
	{
		if (bodyType == eBody_Normal)
		{
			if (bulletable)
			{
				switch (Global::getInstance()->getCurrentBulletType())
				{
				case eBullet_common:
					mainBody->setDisplayFrame(_normalLeftFire);
					break;
				case eBullet_arrow:
					mainBody->setDisplayFrame(_normalLeftArrow);
					break;
				}
			}
			else
			{
				mainBody->setDisplayFrame(_normalLeft);
			}
		}
		else
		{
			mainBody->setDisplayFrame(_smallLeft);
		}
		face = eLeft;
		break;
	}
	case eRight:
	{
		if (!isFlying)
		{
			if (bodyType == eBody_Normal)
			{
				if (bulletable)
				{
					switch (Global::getInstance()->getCurrentBulletType())
					{
					case eBullet_common:
						mainBody->runAction(RepeatForever::create(
							AnimationManager::getInstance()->createAnimate(eAniRightFire)));
						break;
					case eBullet_arrow:
						mainBody->runAction(RepeatForever::create(
							AnimationManager::getInstance()->createAnimate(eAniArrowRight)));
						break;
					}
				}
				else
				{
					mainBody->runAction(RepeatForever::create(
						AnimationManager::getInstance()->createAnimate(eAniRight)));
				}
			}
			else
			{
				mainBody->runAction(RepeatForever::create(
					AnimationManager::getInstance()->createAnimate(eAniRightSmall)));
			}
		}
		face = eRight;
		break;
	}
	case eLeft:
	{
		if (!isFlying)
		{
			if (bodyType == eBody_Normal)
			{
				if (bulletable)
				{
					switch (Global::getInstance()->getCurrentBulletType())
					{
					case eBullet_common:
						mainBody->runAction(RepeatForever::create(
							AnimationManager::getInstance()->createAnimate(eAniLeftFire)));
						break;
					case eBullet_arrow:
						mainBody->runAction(RepeatForever::create(
							AnimationManager::getInstance()->createAnimate(eAniArrowLeft)));
						break;
					}
				}
				else
				{
					mainBody->runAction(RepeatForever::create(
						AnimationManager::getInstance()->createAnimate(eAniLeft)));
				}
			}
			else
			{
				mainBody->runAction(RepeatForever::create(
					AnimationManager::getInstance()->createAnimate(eAniLeftSmall)));
			}

		}
		face = eLeft;
		break;
	}
	case eJumpLeft:
	{
		if (bodyType == eBody_Normal)
		{
			if (bulletable)
			{
				switch (Global::getInstance()->getCurrentBulletType())
				{
				case eBullet_common:
					mainBody->setDisplayFrame(_jumpLeftFire);
					break;
				case eBullet_arrow:
					mainBody->setDisplayFrame(_jumpLeftArrow);
					break;
				}
			}
			else
			{
				mainBody->setDisplayFrame(_jumpLeft);
			}
		}
		else
		{
			mainBody->setDisplayFrame(_smallJumpLeft);
		}
		face = eLeft;
		break;
	}
	case eJumpRight:
	{
		if (bodyType == eBody_Normal)
		{
			if (bulletable)
			{
				switch (Global::getInstance()->getCurrentBulletType())
				{
				case eBullet_common:
					mainBody->setDisplayFrame(_jumpRightFire);
					break;
				case eBullet_arrow:
					mainBody->setDisplayFrame(_jumpRightArrow);
					break;
				}
			}
			else
			{
				mainBody->setDisplayFrame(_jumpRight);
			}
		}
		else
		{
			mainBody->setDisplayFrame(_smallJumpRight);
		}
		face = eRight;
		break;
	}
	default:
		break;
	}
}

marioDirection Hero::getHeroState()
{
	return state;
}

void Hero::onEnter()
{
	Node::onEnter();
}

void Hero::onExit()
{
	Node::onExit();
}

bool Hero::heroInit()
{
	this->setContentSize(smallSize);
	// ������ص���С�������
	mainBody = Sprite::create("image/hero/smallWalkRight.png", CCRectMake(0, 0, 14, 16));
	mainBody->setAnchorPoint(Point(0, 0));
	this->addChild(mainBody);
	state = eNormalRight;
	return true;
}

Hero* Hero::create()
{
	Hero *pHero = new Hero();
	if (pHero && pHero->heroInit())
	{
		pHero->autorelease();
		return pHero;
	}
	CC_SAFE_DELETE(pHero);
	return NULL;
}

void Hero::setBodyType(BodyType _bodytype)
{
	bodyType = _bodytype;
	switch (_bodytype)
	{
	case eBody_Normal:
		currentSize = norBodySize;
		mainBody->setDisplayFrame(_normalRight);
		break;
	case eBody_Small:
		currentSize = smallSize;
		mainBody->setDisplayFrame(_smallRight);
		break;
	case eBody_Fireable:
	{
		bodyType = eBody_Normal;
		currentSize = norBodySize;
		bulletable = true;
		switch (Global::getInstance()->getCurrentBulletType())
		{
		case eBullet_arrow:
			mainBody->setDisplayFrame(_normalRightArrow);
			break;
		case eBullet_common:
			mainBody->setDisplayFrame(_normalRightFire);
			break;
		}

		break;
	}
	default:
		break;
	}
	this->setContentSize(currentSize);
}

Size Hero::getCurrentSize()
{
	return currentSize;
}

BodyType Hero::getCurrentBodyType()
{
	return bodyType;
}

void Hero::setSafeTime(bool _issafe)
{
	isSafeTime = _issafe;
}

bool Hero::getIsSafeTime()
{
	return isSafeTime;
}

void Hero::changeForGotAddLifeMushroom()
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("AddLife.ogg");

	Global::getInstance()->lifeNumPlusOne();

	pLabelUp = CCLabelTTF::create("UP1!", "Arial", 20);
	pLabelUp->setPosition(Point(0, 0)/*CCPointZero*/);
	this->addChild(pLabelUp);
	JumpBy *pJump = JumpBy::create(0.5f, Point(0, this->getContentSize().height / 2),
		this->getContentSize().height, 1);
	pLabelUp->runAction(Sequence::create(pJump,
		CallFunc::create(this, callfunc_selector(Hero::clearLabelUp)), NULL));
}

void Hero::changeForGotMushroom()
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("EatMushroomOrFlower.ogg");

	switch (bodyType)
	{
	case eBody_Small:
	{
		// ����һ�α����˸�Ķ���
		Global::getInstance()->currentHeroType = eBody_Normal;
		this->setHeroTypeForNormal();
		ActionInterval *pBlink = CCBlink::create(1, 5);
		this->runAction(pBlink);
	}
		break;
	case eBody_Normal:
		// չʾһ��up����ʾ����һ���ˣ������ǽŵ�������ͷ��Ȼ����ʧ
	{
		//pLabelUp = CCLabelTTF::create("Good!!!", "Arial", 20);
		//pLabelUp->setPosition(CCPointZero);
		//this->addChild(pLabelUp);
		//JumpBy *pJump = JumpBy::create(0.5f, Point(0, this->getContentSize().height/2),
		//	this->getContentSize().height, 1);
		//pLabelUp->runAction(Sequence::create(pJump, 
		//	CallFunc::create(this, callfunc_selector(Hero::clearLabelUp))));
		bulletable = true;
		Global::getInstance()->currentHeroType = eBody_Fireable;
		if (!bulletable)
		{
			switch (face)
			{
			case eRight:
				mainBody->setDisplayFrame(_normalRightFire);
				break;
			case eLeft:
				mainBody->setDisplayFrame(_normalLeftFire);
				break;
			default:
				break;
			}
		}
	}
		break;
	default:
		break;
	}
}

void Hero::changeForGotEnemy()
{
	isSafeTime = true;
	ActionInterval *pDelay = DelayTime::create(3.0f);
	this->runAction(Sequence::create(pDelay,
		CallFunc::create(this, callfunc_selector(Hero::reSetSafeTime)), NULL));

	switch (bodyType)
	{
	case eBody_Normal:
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("SuoXiao.ogg");
		Global::getInstance()->currentHeroType = eBody_Small;
		this->setHeroTypeForSmall();
		ActionInterval *pBlink = CCBlink::create(3, 15);
		this->runAction(pBlink);
		break;
	}
	case eBody_Small:
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("YuDaoGuaiWuSi.ogg");
		mainBody->stopAllActions();
		mainBody->setDisplayFrame(_lifeOverSmall);
		this->setHeroDie(true);
		break;
	}
	default:
		break;
	}
}

void Hero::setHeroDie(bool _die)
{
	isDied = _die;
	Global::getInstance()->currentHeroType = eBody_Small;
	Global::getInstance()->lifeNumCutOne();
}
bool Hero::isHeroDied()
{
	return isDied;
}

void Hero::dieForTrap()
{
	mainBody->stopAllActions();

	switch (bodyType)
	{
	case eBody_Small:
		mainBody->setDisplayFrame(_lifeOverSmall);
		mainBody->runAction(AnimationManager::getInstance()->createAnimate(eAniSmallDie));
		break;
	case eBody_Normal:
		if (bulletable)
		{
			switch (Global::getInstance()->getCurrentBulletType())
			{
			case eBullet_common:
				mainBody->setDisplayFrame(_lifeOverFire);
				mainBody->runAction(AnimationManager::getInstance()->createAnimate(eAniFireDie));
				break;
			case eBullet_arrow:
				_lifeOverFire = SpriteFrame::create("image/hero/arrow_die.png", CCRectMake(24, 0, 24, 32));
				mainBody->setDisplayFrame(_lifeOverFire);
				mainBody->runAction(AnimationManager::getInstance()->createAnimate(eAniArrowDie));
			}
		}
		else
		{
			mainBody->setDisplayFrame(_lifeOverNormal);
			mainBody->runAction(AnimationManager::getInstance()->createAnimate(eAniNormalDie));
		}
		break;
	default:
		break;
	}

	ActionInterval *pMoveUp = MoveBy::create(0.6f, Point(0, 32));
	ActionInterval *pMoveDown = MoveBy::create(0.6f, Point(0, -32));
	ActionInterval *pDeley = DelayTime::create(0.2f);

	this->runAction(Sequence::create(pMoveUp, pDeley, pMoveDown,
		CallFunc::create(this, callfunc_selector(Hero::reSetNonVisible)), NULL));
}

void Hero::reSetNonVisible()
{
	mainBody->stopAllActions();
	this->setVisible(false);
}

void Hero::clearLabelUp()
{
	pLabelUp->removeFromParentAndCleanup(true);
	pLabelUp = NULL;
}

void Hero::setHeroTypeForSmall()
{
	this->setBodyType(eBody_Small);
	bulletable = false;
	switch (face)
	{
	case eRight:
		mainBody->setDisplayFrame(_smallRight);
		break;
	case eLeft:
		mainBody->setDisplayFrame(_smallLeft);
		break;
	default:
		break;
	}
}

void Hero::setHeroTypeForNormal()
{
	this->setBodyType(eBody_Normal);
	switch (face)
	{
	case eRight:
		mainBody->setDisplayFrame(_normalRight);
		break;
	case eLeft:
		mainBody->setDisplayFrame(_normalLeft);
		break;
	default:
		break;
	}
}

void Hero::reSetSafeTime()
{
	isSafeTime = false;
}

bool Hero::isBulletable()
{
	return bulletable;
}

void Hero::setGadgetable(bool _gadgetable)
{
	gadgetable = _gadgetable;
}
bool Hero::isGadgetable()
{
	return gadgetable;
}

void Hero::fireAction()
{
	ActionInterval *pAction = NULL;
	this->setHeroState(eFireTheHole);
	switch (face)
	{
	case eRight:
	{
		switch (Global::getInstance()->getCurrentBulletType())
		{
		case eBullet_common:
			pAction = AnimationManager::getInstance()->createAnimate(eAniFireActionR);
			break;
		case eBullet_arrow:
			pAction = AnimationManager::getInstance()->createAnimate(eAniArrowActionR);
			break;
		}
		break;
	}
	case eLeft:
	{
		switch (Global::getInstance()->getCurrentBulletType())
		{
		case eBullet_common:
			pAction = AnimationManager::getInstance()->createAnimate(eAniFireActionL);
			break;
		case eBullet_arrow:
			pAction = AnimationManager::getInstance()->createAnimate(eAniArrowActionL);
			break;
		}
		break;
	}
	default:
		break;
	}
	mainBody->runAction(Sequence::create(pAction,
		CallFunc::create(this, callfunc_selector(Hero::reSetStateForFired)), NULL));
}

void Hero::reSetStateForFired()
{
	this->setHeroState(statePre);
}

void Hero::setBulletType(BulletType _type)
{
	if (currentBulletType != _type)
	{
		currentBulletType = _type;
		switch (face)
		{
		case eRight:
		{
			switch (currentBulletType)
			{
			case eBullet_common:
				mainBody->setDisplayFrame(_normalRightFire);
				break;
			case eBullet_arrow:
				mainBody->setDisplayFrame(_normalRightArrow);
				break;
			}
			break;
		}
		case eLeft:
		{
			switch (currentBulletType)
			{
			case eBullet_common:
				mainBody->setDisplayFrame(_normalLeftFire);
				break;
			case eBullet_arrow:
				mainBody->setDisplayFrame(_normalLeftArrow);
				break;
			}
			break;
		}
		}
	}
}

BulletType Hero::getBulletType()
{
	return currentBulletType;
}

void Hero::reSetForSuccess()
{
	switch (bodyType)
	{
	case eBody_Small:
		mainBody->setDisplayFrame(_smallRight);
		break;
	case eBody_Normal:
		if (bulletable)
		{
			switch (Global::getInstance()->getCurrentBulletType())
			{
			case eBullet_common:
				mainBody->setDisplayFrame(_normalRightFire);
				break;
			case eBullet_arrow:
				mainBody->setDisplayFrame(_normalRightArrow);
				break;
			}
		}
		else
		{
			mainBody->setDisplayFrame(_normalRight);
		}
		break;
	default:
		break;
	}
}
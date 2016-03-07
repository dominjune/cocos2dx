#include "Enemy.h"
#include "Hero.h"
#include "AnimationManager.h"
#include "GameMap.h"
#include "GameLayer.h"
#include "Hero.h"
#include <math.h>
#include "Global.h"

// ******************Enemy******************** //
Enemy::Enemy()
{
	
	// 正常情况下，怪物出生后会朝着左方向移动来对主角产生威胁
	// 在特殊情况下可以通过地图上的怪物对象字典来携带朝向信息
	startFace = eLeft;
	moveOffset = 0.0f;
	ccMoveOffset = 0.6f;

	jumpOffset = 0.0f;
	ccJumpOffset = 0.3f;

	enemyState = eEnemyState_nonactive;
}

Enemy::~Enemy()
{
	//this->unscheduleAllSelectors();
}


void Enemy::setTileCoord(Point _tileCoord)
{
	tileCoord = _tileCoord;
}
Point Enemy::getTileCoord()
{
	return tileCoord;
}

void Enemy::setEnemyPos(Point _enemyPos)
{
	enemyPos = _enemyPos;
}
Point Enemy::getEnemyPos()
{
	return enemyPos;
}

void Enemy::setBodySize(Size _size)
{
	bodySize = _size;
}
Size Enemy::getBodySize()
{
	return bodySize;
}

void Enemy::setMoveOffset(float _moveoffset)
{
	moveOffset = _moveoffset;
}
float Enemy::getMoveOffset()
{
	return moveOffset;
}

void Enemy::setccMoveOffset(float _ccmoveoffset)
{
	ccMoveOffset = _ccmoveoffset;
}
float Enemy::getccMoveOffset()
{
	return ccMoveOffset;
}

EnemyType Enemy::getEnemyType()
{
	return enemyType;
}

Rect Enemy::getEnemyRect()
{
	Point pos = this->getPosition();
	return CCRectMake(pos.x - bodySize.width/2 + 2, pos.y + 2, bodySize.width - 4, bodySize.height - 4);
}

void Enemy::setEnemyState(EnemyState _state)
{
	enemyState = _state;
	switch (enemyState)
	{
	case eEnemyState_over:
		{
			this->enemyBody->stopAllActions();
			this->unscheduleUpdate();
			this->setVisible(false);
			break;
		}
	default:
		break;
	}
}

EnemyState Enemy::getEnemyState()
{
	return enemyState;
}

void Enemy::checkState()
{
	Size winSize = Director::getInstance()->getWinSize();
	float tempMaxH = GameLayer::getMapMaxH();
	Point pos = this->getPosition();

	if ( (pos.x + bodySize.width/2 - tempMaxH >= 0) &&
		(pos.x - bodySize.width/2 - tempMaxH) <= winSize.width )
	{
		enemyState = eEnemyState_active;
	}
	else
	{
		if (pos.x + bodySize.width/2 - tempMaxH < 0)
		{
			this->setEnemyState(eEnemyState_over);
		}
		else
		{
			enemyState = eEnemyState_nonactive;
		}
	}
}

void Enemy::stopEnemyUpdate()
{
	enemyBody->stopAllActions();
}

// 普通怪物的水平方向碰撞检测
void Enemy::enemyCollistionH()
{
	Point currentPos = this->getPosition();
	Size enemySize = this->getContentSize();
	Point leftCollistion = Point(currentPos.x - enemySize.width/2, currentPos.y);
	Point leftTilecoord = GameMap::getGameMap()->positionToTileCoord(leftCollistion);
	Point leftPos = GameMap::getGameMap()->tilecoordToPosition(leftTilecoord);
	leftPos = Point(leftPos.x + bodySize.width/2 + GameMap::getGameMap()->getTileSize().width, currentPos.y);

	TileType tileType;
	// 左侧检测
	tileType = GameMap::getGameMap()->tileTypeforPos(leftTilecoord);
	switch (tileType)
	{
	case eTile_Pipe:
	case eTile_Block:
		this->setPosition(leftPos);
		moveOffset *= -1;		
		break;
	default:
		break;
	}
	// 右侧检测
	Point rightCollistion = Point(currentPos.x + bodySize.width/2, currentPos.y);
	Point rightTilecoord = GameMap::getGameMap()->positionToTileCoord(rightCollistion);
	Point rightPos = GameMap::getGameMap()->tilecoordToPosition(rightTilecoord);
	rightPos = Point(rightPos.x - bodySize.width/2, currentPos.y);

	tileType = GameMap::getGameMap()->tileTypeforPos(rightTilecoord);
	switch (tileType)
	{
	case eTile_Pipe:
	case eTile_Block:
		this->setPosition(rightPos);
		moveOffset *= -1;
		break;
	default:
		break;
	}
}

// 普通怪物的竖直方向碰撞检测
void Enemy::enemyCollistionV()
{
	Point currentPos = this->getPosition();
	Point downCollision = currentPos;
	Point downTilecoord = GameMap::getGameMap()->positionToTileCoord(downCollision);
	downTilecoord.y += 1;

	Point downPos = GameMap::getGameMap()->tilecoordToPosition(downTilecoord);
	downPos = Point(currentPos.x, downPos.y + GameMap::getGameMap()->getTileSize().height);

	TileType tileType = GameMap::getGameMap()->tileTypeforPos(downTilecoord);
	bool downFlag = false;
	switch (tileType)
	{
	case eTile_Land:
	case eTile_Pipe:
	case eTile_Block:
		{
			downFlag = true;
			jumpOffset = 0.0f;
			this->setPosition(downPos);
			break;
		}
	case eTile_Trap:
		{
			this->setEnemyState(eEnemyState_over);
			break;
		}
	}

	if (downFlag)
	{
		return ;
	}

	jumpOffset -= ccJumpOffset;
}

EnemyVSHero Enemy::checkCollisionWithHero()
{
	EnemyVSHero ret = eVS_nonKilled;

	Point heroPos = Hero::getInstance()->getPosition();
	Size heroSize = Hero::getInstance()->getContentSize();
	Rect heroRect = Rect(heroPos.x - heroSize.width/2 + 2, heroPos.y + 3, 
		heroSize.width - 4, heroSize.height - 4);
	
	Rect heroRectVS = Rect(heroPos.x - heroSize.width / 2 - 3, heroPos.y,
		heroSize.width - 6, 2);

	Point enemyPos = this->getPosition();
	Rect enemyRect = Rect(enemyPos.x - bodySize.width / 2 + 1, enemyPos.y,
		bodySize.width - 2, bodySize.height - 4);

	Rect enemyRectVS = CCRectMake(enemyPos.x - bodySize.width/2 - 2, enemyPos.y + bodySize.height - 4, 
		bodySize.width - 4, 4);

	if (heroRectVS.intersectsRect(enemyRectVS))
	{
		ret = eVS_enemyKilled;
		return ret;
	}

	if (heroRect.intersectsRect(enemyRect))
	{
		ret = eVS_heroKilled;
		return ret;
	}

	return ret;
}

void Enemy::forKilledByHero()
{
	enemyState = eEnemyState_over;
	enemyBody->stopAllActions();
	this->stopAllActions();
	this->unscheduleUpdate();
	enemyBody->setDisplayFrame(enemyLifeOver);
	ActionInterval *pDelay = DelayTime::create(1.0f);
	this->runAction(Sequence::create(pDelay, 
		CallFunc::create(this, callfunc_selector(Enemy::setNonVisibleForKilledByHero)), NULL));
}

void Enemy::setNonVisibleForKilledByHero()
{
	this->setVisible(false);
}

void Enemy::forKilledByBullet()
{
	enemyState = eEnemyState_over;
	enemyBody->stopAllActions();
	this->unscheduleUpdate();

	MoveBy *pMoveBy = NULL;
	JumpBy *pJumpBy = NULL;

	switch (Global::getInstance()->getCurrentBulletType())
	{
	case eBullet_common:
		{
			if (enemyType == eEnemy_mushroom || enemyType == eEnemy_AddMushroom)
			{
				enemyBody->setDisplayFrame(overByArrow);
			}
			else
			{
				enemyBody->setDisplayFrame(enemyLifeOver);
			}

			switch (Hero::getInstance()->face)
			{
			case eRight:
				pJumpBy = JumpBy::create(0.3f, Point(bodySize.width*2, 0), bodySize.height, 1);
				break;
			case eLeft:
				pJumpBy = JumpBy::create(0.3f, Point(-bodySize.width*2, 0), bodySize.height, 1);
				break;
			default:
				break;
			}

			break;
		}
	case eBullet_arrow:
		{
			enemyBody->setDisplayFrame(overByArrow);
			Sprite *arrow = Sprite::create("image/props/arrow.png");
			arrow->setPosition(Point(bodySize.width/2, bodySize.height/2));
			this->addChild(arrow);

			switch (Hero::getInstance()->face)
			{
			case eRight:
				pMoveBy = MoveBy::create(0.1f, Point(2*bodySize.width, 0));
				break;
			case eLeft:
				pMoveBy = MoveBy::create(0.1f, Point(-2*bodySize.width, 0));
				arrow->runAction(CCFlipX::create(true));
				break;
			default:
				break;
			}

			break;
			break;
		}
	default:
		break;
	}

	// 先判断如果是食人花的话，就是原地停留一段时间然后消失掉
	if (enemyType == eEnemy_flower)
	{
		DelayTime *pDelay = DelayTime::create(0.2f);
		this->runAction(Sequence::create(pDelay,
			CallFunc::create(this, callfunc_selector(Enemy::setNonVisibleForKilledByBullet)), NULL));
		return ;
	}
	
	if (pJumpBy)
	{
		this->runAction(Sequence::create(pJumpBy, 
			CallFunc::create(this, callfunc_selector(Enemy::setNonVisibleForKilledByBullet)), NULL));
	}
	else
	{
		this->runAction(Sequence::create(pMoveBy, 
			CallFunc::create(this, callfunc_selector(Enemy::setNonVisibleForKilledByBullet)), NULL));
	}
}

void Enemy::setNonVisibleForKilledByBullet()
{
	enemyState = eEnemyState_over;
	this->setVisible(false);
}


// ******************** EnemyMushroom ***************** //
EnemyMushroom::EnemyMushroom()
{
	enemyType = eEnemy_mushroom;
	bodySize = CCSizeMake(16.0f, 16.0f);
	enemyBody = Sprite::create("image/enemy/Mushroom0.png", CCRectMake(0, 0, 16, 16));
	enemyBody->setAnchorPoint(Point(0, 0));
	this->setContentSize(bodySize);
	this->addChild(enemyBody);
	this->setAnchorPoint(Point(0.5f, 0));

	enemyLifeOver = SpriteFrame::create("image/enemy/Mushroom0.png", CCRectMake(32, 0, 16, 16));
	enemyLifeOver->retain();

	overByArrow = SpriteFrame::create("image/enemy/Mushroom0.png", CCRectMake(48, 0, 16, 16));
	overByArrow->retain();

	moveOffset = -ccMoveOffset;
}

EnemyMushroom::~EnemyMushroom()
{
	this->unscheduleAllSelectors();
}
void EnemyMushroom::onEnter()
{
	Node::onEnter();
}
void EnemyMushroom::onExit()
{
	Node::onExit();
	enemyLifeOver->release();
	overByArrow->release();
}

void EnemyMushroom::launchEnemy()
{
	enemyBody->runAction(RepeatForever::create(AnimationManager::getInstance()->createAnimate(eAniMushroom)));
	this->scheduleUpdate();
}



void EnemyMushroom::update(float dt)
{
	this->checkState();

	if (enemyState == eEnemyState_active)
	{
		Point currentPos = this->getPosition();
		currentPos.x += moveOffset;
		currentPos.y += jumpOffset;
		if (currentPos.x<0)
		{
			this->setEnemyState(eEnemyState_over);
		}
		else
		{
			this->setPosition(currentPos);
		}

		this->enemyCollistionH();
		this->enemyCollistionV();
	}
}


// ********************** EnemyFlower ****************** //
EnemyFlower::EnemyFlower()
{
	enemyType = eEnemy_flower;
	bodySize = CCSizeMake(16, 24);
	enemyBody = Sprite::create("image/enemy/flower0.png", CCRectMake(0, 0, 16, 24));
	enemyBody->setAnchorPoint(Point(0, 0));
	this->setContentSize(bodySize);
	this->addChild(enemyBody);
	this->setAnchorPoint(Point(0.5f, 0));

	// 吃人花虽然不会被踩死，但是会被子弹打死
	enemyLifeOver = SpriteFrame::create("image/enemy/flower0.png", CCRectMake(0, 0, 16, 24));
	enemyLifeOver->retain();

	overByArrow = enemyLifeOver;
}

EnemyFlower::~EnemyFlower()
{
	this->unscheduleAllSelectors();
}

void EnemyFlower::onEnter()
{
	Node::onEnter();
}

void EnemyFlower::onExit()
{
	Node::onExit();
	enemyLifeOver->release();
}

void EnemyFlower::launchEnemy()
{
	enemyBody->runAction(RepeatForever::create(AnimationManager::getInstance()->createAnimate(eAniflower)));
	Point pos = this->getPosition();	
	pos.y -= bodySize.height;
	startPos = pos;
	this->runAction(Place::create(pos));

	ActionInterval *pMoveBy = MoveBy::create(1.0f, Point(0.0f, bodySize.height));
	ActionInterval *pDelay = DelayTime::create(1.0f);
	ActionInterval *pMoveByBack = pMoveBy->reverse();
	ActionInterval *pDelay2 = DelayTime::create(2.0f);
	this->runAction(RepeatForever::create(
		(ActionInterval*)Sequence::create(pMoveBy, pDelay, pMoveByBack, pDelay2, NULL)));

	this->scheduleUpdate();
}

void EnemyFlower::update(float dt)
{
	// 食人花的帧更新函数中不做位置控制类代码
//	if (! isLaunch)
//	{
//		if (enemyState == eEnemyState_active)
//		{
//			isLaunch = true;
//			//startPos.y += bodySize.height;
//			//ActionInterval *pMoveBy = MoveBy::create(2.0f, Point(0.0f, bodySize.height));
//			//ActionInterval *pDelay = DelayTime::create(1.0f);
//			//ActionInterval *pMoveByBack = pMoveBy->reverse();
//			//this->runAction(RepeatForever::create(
//			//	(ActionInterval*)Sequence::create(pMoveBy, pDelay, pMoveByBack, NULL)));
//		}
//	}
	this->checkState();
}

EnemyVSHero EnemyFlower::checkCollisionWithHero()
{
	EnemyVSHero ret = eVS_nonKilled;

	Point heroPos = Hero::getInstance()->getPosition();
	Size heroSize = Hero::getInstance()->getCurrentSize();
	Rect heroRect = CCRectMake(heroPos.x - heroSize.width/2 + 2, heroPos.y + 2, 
		heroSize.width - 4, heroSize.height - 4);

	Point enemyPos = this->getPosition();
	Rect enemyRect = CCRectMake(enemyPos.x - bodySize.width/2 + 2, enemyPos.y + bodySize.height - (enemyPos.y-startPos.y), 
		bodySize.width - 4, enemyPos.y - startPos.y);


	if (heroRect.intersectsRect(enemyRect))
	{
		ret = eVS_heroKilled;
	}

	return ret;
}

Rect EnemyFlower::getEnemyRect()
{
	Point enemyPos = this->getPosition();
	Rect enemyRect = CCRectMake(enemyPos.x - bodySize.width/2 + 2, enemyPos.y + bodySize.height - (enemyPos.y-startPos.y), 
		bodySize.width - 4, enemyPos.y - startPos.y);
	return enemyRect;
}

// ********************** EnemyTortoise ****************** //
EnemyTortoise::EnemyTortoise(int _startface)
{
	// 地上行走的乌龟在初始化时，要先判断起始朝向
	// 0：左             1：右
	switch (_startface)
	{
	case 0:
		startFace = eLeft;
		enemyBody = Sprite::create("image/enemy/tortoise0.png", CCRectMake(18*2, 0, 18, 24));
		leftFace = SpriteFrame::create("image/enemy/tortoise0.png", CCRectMake(18*2, 0, 18, 24));
		leftFace->retain();
		moveOffset = -ccMoveOffset;
		break;
	case 1:
		startFace = eRight;
		enemyBody = Sprite::create("image/enemy/tortoise0.png", CCRectMake(18*5, 0, 18, 24));
		rightFace = SpriteFrame::create("image/enemy/tortoise0.png", CCRectMake(18*5, 0, 18, 24));
		rightFace->retain();
		moveOffset = ccMoveOffset;
		break;
	default:
		break;
	}

	enemyType = eEnemy_tortoise;
	bodySize = CCSizeMake(18.0f, 24.0f);	
	enemyBody->setAnchorPoint(Point(0, 0));
	this->setContentSize(bodySize);
	this->addChild(enemyBody);
	this->setAnchorPoint(Point(0.5f, 0.0f));

	enemyLifeOver = SpriteFrame::create("image/enemy/tortoise0.png", CCRectMake(18*9, 0, 18, 24));
	enemyLifeOver->retain();

	overByArrow = SpriteFrame::create("image/enemy/tortoise0.png", CCRectMake(18*8, 0, 18, 24));
	overByArrow->retain();
}

EnemyTortoise::~EnemyTortoise()
{
	this->unscheduleAllSelectors();
}


void EnemyTortoise::onEnter()
{
	leftFace = nullptr;
	rightFace = nullptr;
	Node::onEnter();
}

void EnemyTortoise::onExit()
{
	Node::onExit();
	CC_SAFE_RELEASE(leftFace);
	CC_SAFE_RELEASE(rightFace);
	enemyLifeOver->release();
	overByArrow->release();
}


void EnemyTortoise::launchEnemy()
{
	switch (startFace)
	{
	case eLeft:
		enemyBody->runAction(RepeatForever::create(AnimationManager::getInstance()->createAnimate(eAniTortoiseLeft)));
		break;
	case eRight:
		enemyBody->runAction(RepeatForever::create(AnimationManager::getInstance()->createAnimate(eAniTortoiseRight)));
		break;
	default:
		break;
	}
	this->scheduleUpdate();
}

void EnemyTortoise::enemyCollistionH()
{
	Point currentPos = this->getPosition();
	Size enemySize = this->getContentSize();
	Point leftCollistion = Point(currentPos.x - enemySize.width/2, currentPos.y);
	Point leftTilecoord = GameMap::getGameMap()->positionToTileCoord(leftCollistion);
	Point leftPos = GameMap::getGameMap()->tilecoordToPosition(leftTilecoord);
	leftPos = Point(leftPos.x + bodySize.width/2 + GameMap::getGameMap()->getTileSize().width, currentPos.y);

	TileType tileType;
	// 左侧检测
	tileType = GameMap::getGameMap()->tileTypeforPos(leftTilecoord);
	switch (tileType)
	{
	case eTile_Pipe:
	case eTile_Block:
		this->setPosition(leftPos);
		moveOffset *= -1;
		//enemyBody->setDisplayFrame(rightFace);
		enemyBody->stopAllActions();
		enemyBody->runAction(RepeatForever::create(AnimationManager::getInstance()->createAnimate(eAniTortoiseRight)));
		break;
	default:
		break;
	}
	// 右侧检测
	Point rightCollistion = Point(currentPos.x + bodySize.width/2, currentPos.y);
	Point rightTilecoord = GameMap::getGameMap()->positionToTileCoord(rightCollistion);
	Point rightPos = GameMap::getGameMap()->tilecoordToPosition(rightTilecoord);
	rightPos = Point(rightPos.x - bodySize.width/2, currentPos.y);

	tileType = GameMap::getGameMap()->tileTypeforPos(rightTilecoord);
	switch (tileType)
	{
	case eTile_Pipe:
	case eTile_Block:
		this->setPosition(rightPos);
		moveOffset *= -1;
		
		//enemyBody->setDisplayFrame(leftFace);
		enemyBody->stopAllActions();
		enemyBody->runAction(RepeatForever::create(AnimationManager::getInstance()->createAnimate(eAniTortoiseLeft)));
		break;
	default:
		break;
	}
}

void EnemyTortoise::update(float dt)
{
	this->checkState();

	if (enemyState == eEnemyState_active)
	{
		Point currentPos = this->getPosition();
		currentPos.x += moveOffset;
		currentPos.y += jumpOffset;
		this->setPosition(currentPos);

		this->enemyCollistionH();
		this->enemyCollistionV();
	}
}


// ********************** EnemyTortoiseRound ****************** //
EnemyTortoiseRound::EnemyTortoiseRound(float dis)
{
	enemyType = eEnemy_tortoiseRound;
	bodySize = CCSizeMake(18.0f, 24.0f);
	enemyBody = Sprite::create("image/enemy/tortoise0.png", CCRectMake(18*2, 0, 18, 24));
	enemyBody->setAnchorPoint(Point(0, 0));
	this->setContentSize(bodySize);
	this->addChild(enemyBody);
	this->setAnchorPoint(Point(0.5f, 0.0f));

	enemyLifeOver = SpriteFrame::create("image/enemy/tortoise0.png", CCRectMake(18*9, 0, 18, 24));
	enemyLifeOver->retain();

	overByArrow = SpriteFrame::create("image/enemy/tortoise0.png", CCRectMake(18*8, 0, 18, 24));
	overByArrow->retain();

	roundDis = dis;
}
EnemyTortoiseRound::~EnemyTortoiseRound()
{
	this->unscheduleAllSelectors();
}
void EnemyTortoiseRound::onEnter()
{
	Node::onEnter();
}
void EnemyTortoiseRound::onExit()
{
	Node::onExit();
	enemyLifeOver->release();
	overByArrow->release();
}
void EnemyTortoiseRound::launchEnemy()
{
	enemyState = eEnemyState_active;
	enemyBody->runAction(RepeatForever::create(AnimationManager::getInstance()->createAnimate(eAniTortoiseLeft)));
	ActionInterval *pMoveLeft = MoveBy::create(2.0f, Point(-roundDis, 0.0f));
	ActionInterval *pMoveRight = MoveBy::create(2.0f, Point(roundDis, 0.0f));
	DelayTime *pDelay = DelayTime::create(0.2f);

	this->runAction(RepeatForever::create((ActionInterval*)Sequence::create(pMoveLeft, 
		CallFunc::create(this, callfunc_selector(EnemyTortoiseRound::reRight)), 
		pMoveRight,
		CallFunc::create(this, callfunc_selector(EnemyTortoiseRound::reLeft)),
		NULL)));
}

void EnemyTortoiseRound::update(float dt)
{
	this->checkState();
}

void EnemyTortoiseRound::reRight()
{
	enemyBody->stopAllActions();
	enemyBody->runAction(RepeatForever::create(AnimationManager::getInstance()->createAnimate(eAniTortoiseRight)));
}

void EnemyTortoiseRound::reLeft()
{
	enemyBody->stopAllActions();
	enemyBody->runAction(RepeatForever::create(AnimationManager::getInstance()->createAnimate(eAniTortoiseLeft)));
}


// ********************** EnemyTortoiseFly ****************** //

EnemyTortoiseFly::EnemyTortoiseFly(float dis)
{
	enemyType = eEnemy_tortoiseFly;
	bodySize = CCSizeMake(18.0f, 24.0f);
	enemyBody = Sprite::create("image/enemy/tortoise0.png", CCRectMake(0, 0, 18, 24));
	enemyBody->setAnchorPoint(Point(0, 0));
	this->setContentSize(bodySize);
	this->addChild(enemyBody);
	this->setAnchorPoint(Point(0.5f, 0.0f));

	enemyLifeOver = SpriteFrame::create("image/enemy/tortoise0.png", CCRectMake(18*9, 0, 18, 24));
	enemyLifeOver->retain();

	overByArrow = SpriteFrame::create("image/enemy/tortoise0.png", CCRectMake(18*8, 0, 18, 24));
	overByArrow->retain();

	flyDis = dis;
}
void EnemyTortoiseFly::onEnter()
{
	Node::onEnter();
}
void EnemyTortoiseFly::onExit()
{
	Node::onExit();
	enemyLifeOver->release();
	overByArrow->release();
}
EnemyTortoiseFly::~EnemyTortoiseFly()
{
	this->unscheduleAllSelectors();
}

void EnemyTortoiseFly::launchEnemy()
{
	enemyState = eEnemyState_active;
	enemyBody->runAction(RepeatForever::create(AnimationManager::getInstance()->createAnimate(eAniTortoiseFly)));
	ActionInterval *pMoveDown = MoveBy::create(2.0f, Point(0, -flyDis));
	ActionInterval *pMoveUp = MoveBy::create(2.0f, Point(0, flyDis));

	this->runAction(RepeatForever::create(
		(ActionInterval*)Sequence::create(pMoveDown, pMoveUp, NULL)));
}

void EnemyTortoiseFly::update(float dt)
{
	this->checkState();
}

void EnemyTortoiseFly::setFlyDis(float dis)
{
	flyDis = dis;
}

float EnemyTortoiseFly::getFlyDis()
{
	return flyDis;
}

// ********************** EnemyFireString ****************** //
EnemyFireString::EnemyFireString(float _begAngle, float _time)
{
	enemyType = eEnemy_fireString;

	pArrayFire = Array::createWithCapacity(3);
	pArrayFire->retain();

	enemyBody = Sprite::create("image/props/fireBall.png");
	pArrayFire->addObject(enemyBody);
	enemyBody2 = Sprite::create("image/props/fireBall.png");
	pArrayFire->addObject(enemyBody2);
	enemyBody3 = Sprite::create("image/props/fireBall.png");
	pArrayFire->addObject(enemyBody3);
	fireSize = CCSizeMake(8.0f, 8.0f);

	enemyBody->setPosition(Point(8, 8));
	this->addChild(enemyBody);
	enemyBody2->setPosition(Point(24, 8));
	this->addChild(enemyBody2);
	enemyBody3->setPosition(Point(40, 8));
	this->addChild(enemyBody3);

	bodySize = CCSizeMake(48, 16);
	this->setContentSize(bodySize);
	this->setAnchorPoint(Point(0.0f, 0.5f));

	begAngle = _begAngle;
	time = _time;

	angle = begAngle;
	PI = 3.1415926;

	// 因为不存在火串被踩死或是被子弹打死，所以用不到这两个帧
	enemyLifeOver = NULL;
	overByArrow = NULL;
}

EnemyFireString::~EnemyFireString()
{
	this->unscheduleAllSelectors();
}

void EnemyFireString::onEnter()
{
	Node::onEnter();
}

void EnemyFireString::onExit()
{
	Node::onExit();
	pArrayFire->release();
}

void EnemyFireString::launchEnemy()
{
	enemyState = eEnemyState_active;
	enemyBody->runAction(RepeatForever::create(AnimationManager::getInstance()->createAnimate(eAniRotatedFireBall)));
	enemyBody2->runAction(RepeatForever::create(AnimationManager::getInstance()->createAnimate(eAniRotatedFireBall)));
	enemyBody3->runAction(RepeatForever::create(AnimationManager::getInstance()->createAnimate(eAniRotatedFireBall)));

	this->runAction(Sequence::create(CCRotateBy::create(1.0f, -begAngle),
		CallFunc::create(this, callfunc_selector(EnemyFireString::launchFireString)), NULL));
}

void EnemyFireString::stopEnemyUpdate()
{
	enemyBody->stopAllActions();
	enemyBody2->stopAllActions();
	enemyBody3->stopAllActions();
}

EnemyVSHero EnemyFireString::checkCollisionWithHero()
{
	EnemyVSHero ret = eVS_nonKilled;
	
	Point heroPos = Hero::getInstance()->getPosition();
	Size heroSize = Hero::getInstance()->getContentSize();
	Rect heroRect = CCRectMake(heroPos.x - heroSize.width/2 + 2, heroPos.y + 2,
		heroSize.width - 4, heroSize.height - 4);

	Point thisPos = this->getPosition();

	double angleTemp = 2*PI*angle/360.0f;
	for (int i = 0; i < 3; ++i)
	{
		Point firePos = Point(thisPos.x + (2*i*8+8)*cos(angleTemp), thisPos.y + (2*i*8+8)*sin(angleTemp));
		Rect fireRect = CCRectMake(firePos.x - fireSize.width/2, firePos.y - fireSize.height/2,
			fireSize.width, fireSize.height);
		if (heroRect.intersectsRect(fireRect))
		{
			ret = eVS_heroKilled;
			break;
		}
	}

	return ret;
}

void EnemyFireString::forKilledByBullet()
{

}

void EnemyFireString::forKilledByHero()
{

}

void EnemyFireString::launchFireString()
{
	this->runAction(RepeatForever::create(
		CCRotateBy::create(time, -360.0f)));
	this->scheduleUpdate();
}

void EnemyFireString::update(float dt)
{
	angle += (6.0/time);
	if (angle >= 360)
	{
		angle -= 360.0f;
	}
}


// ********************** EnemyFlyFish ****************** //
EnemyFlyFish::EnemyFlyFish(float _offsetH, float _offsetV, float _duration)
{
	enemyType = eEnemy_flyFish;
	enemyState = eEnemyState_active;
	enemyBody = Sprite::create("image/enemy/flyFishRight.png", CCRectMake(16*4, 0, 16, 16));
	bodySize = CCSizeMake(16, 16);
	enemyBody->setAnchorPoint(Point(0, 0));
	this->setContentSize(bodySize);
	this->addChild(enemyBody);
	this->setAnchorPoint(Point(0.5f, 0.0f));

	enemyLifeOver = SpriteFrame::create("image/enemy/flyFishRight.png", CCRectMake(16*4, 0, 16, 16));
	enemyLifeOver->retain();
	overByArrow = enemyLifeOver;

	offsetH = _offsetH;
	offsetV = _offsetV;
	offsetDuration = _duration;
	isFlying = false;
}
void EnemyFlyFish::onEnter()
{
	Node::onEnter();
}
void EnemyFlyFish::onExit()
{
	Node::onExit();
	enemyLifeOver->release();
}
EnemyFlyFish::~EnemyFlyFish()
{
	this->unscheduleAllSelectors();
}

void EnemyFlyFish::launchEnemy()
{
	this->setVisible(false);
	enemyBody->runAction(RepeatForever::create(AnimationManager::getInstance()->createAnimate(eAniFlyFishR)));
	this->scheduleUpdate();
}

EnemyVSHero EnemyFlyFish::checkCollisionWithHero()
{
	EnemyVSHero ret = eVS_nonKilled;

	Point heroPos = Hero::getInstance()->getPosition();
	Size heroSize = Hero::getInstance()->getContentSize();
	Rect heroRect = CCRectMake(heroPos.x - heroSize.width/2 + 2, heroPos.y + 2, 
		heroSize.width - 4, heroSize.height - 4);

	Point enemyPos = this->getPosition();
	Rect enemyRect = CCRectMake(enemyPos.x - bodySize.width/2 + 2, enemyPos.y + 2, 
		bodySize.width - 4, bodySize.height - 4);

	if (heroRect.intersectsRect(enemyRect))
	{
		ret = eVS_heroKilled;
	}

	return ret;
}

void EnemyFlyFish::checkState()
{
	//Point heroPos = Hero::getInstance()->getPosition();
	//if (fabs(heroPos.x - enemyPos.x) < bodySize.width)
	//{
	//	enemyState = eEnemyState_active;
	//}
}

void EnemyFlyFish::update(float dt)
{
	if (!isFlying)
	{
		Point heroPos = Hero::getInstance()->getPosition();
		if (fabs(heroPos.x - enemyPos.x) < bodySize.width)
		{
			this->flyInSky();
		}
	}
}

void EnemyFlyFish::flyInSky()
{
	isFlying = true;
	this->setVisible(true);
	ActionInterval *pMoveBy = MoveBy::create(offsetDuration, Point(offsetH, offsetV));
	this->runAction(Sequence::create(pMoveBy,
		CallFunc::create(this, callfunc_selector(EnemyFlyFish::reSetNotInSky)), NULL));
}

void EnemyFlyFish::reSetNotInSky()
{
	this->setVisible(false);
	this->runAction(Place::create(enemyPos));
	isFlying = false;
}

void EnemyFlyFish::forKilledByHero()
{

}


// ********************** EnemyBoss ****************** //
EnemyBoss::EnemyBoss()
{
	enemyType = eEnemy_Boss;
	enemyState = eEnemyState_nonactive;

	enemyBody = Sprite::create("image/enemy/boss.png", CCRectMake(0, 0, 32, 32));
	enemyBody->setAnchorPoint(Point(0, 0));
	bodySize = CCSizeMake(32, 32);
	this->setContentSize(bodySize);
	this->addChild(enemyBody);
	this->setAnchorPoint(Point(0.5f, 0.0f));
	
	enemyLifeOver = SpriteFrame::create("image/enemy/boss.png", CCRectMake(0, 0, 32, 32));
	enemyLifeOver->retain();
	overByArrow = enemyLifeOver;
	overByArrow->retain();

	ccMoveOffset = 0.5f;
	moveOffset = -ccMoveOffset;
}

EnemyBoss::~EnemyBoss()
{
	this->unscheduleAllSelectors();
}
void EnemyBoss::onEnter()
{
	Node::onEnter();
}
void EnemyBoss::onExit()
{
	Node::onExit();
	enemyLifeOver->release();
	overByArrow->release();
}

void EnemyBoss::launchEnemy()
{
	leftSide = enemyPos.x - 32;
	rightSide = enemyPos.x + 32;

	this->scheduleUpdate();
}

EnemyVSHero EnemyBoss::checkCollisionWithHero()
{
	EnemyVSHero ret = eVS_nonKilled;

	Point heroPos = Hero::getInstance()->getPosition();
	Size heroSize = Hero::getInstance()->getContentSize();
	Rect heroRect = CCRectMake(heroPos.x - heroSize.width/2 + 2, heroPos.y + 2, 
		heroSize.width - 4, heroSize.height - 4);

	Point enemyPos = this->getPosition();
	Rect enemyRect = CCRectMake(enemyPos.x - bodySize.width/2 + 2, enemyPos.y + 2, 
		bodySize.width - 4, bodySize.height - 4);

	if (heroRect.intersectsRect(enemyRect))
	{
		ret = eVS_heroKilled;
	}

	return ret;
}

void EnemyBoss::forKilledByBullet()
{
	if (Global::getInstance()->getCurrentBulletType() == eBullet_arrow)
	{
		this->runAction(Sequence::create(MoveBy::create(0.1f, Point(8, 0)),
			MoveBy::create(0.1f, Point(-8, 0)), NULL));

		static int num = 0;
		++num;
		if (num == 5)
		{
			enemyState = eEnemyState_over;
			enemyBody->stopAllActions();
			this->stopAllActions();
			this->unscheduleUpdate();
			this->setVisible(false);
		}		
	}
}

void EnemyBoss::forKilledByHero()
{

}

void EnemyBoss::moveLeft()
{
	enemyBody->stopAllActions();
	enemyBody->runAction(RepeatForever::create(AnimationManager::getInstance()->createAnimate(eAniBossMoveLeft)));
}

void EnemyBoss::moveRight()
{
	enemyBody->stopAllActions();
	enemyBody->runAction(RepeatForever::create(AnimationManager::getInstance()->createAnimate(eAniBossMoveRight)));
}

void EnemyBoss::update(float dt)
{
	this->checkState();

	if (enemyState == eEnemyState_active)
	{
		Point currentPos = this->getPosition();
		currentPos.x += moveOffset;
		this->setPosition(currentPos);

		this->enemyCollistionH();
	}
}

void EnemyBoss::enemyCollistionH()
{
	Point pos = this->getPosition();

	float leftCheck = pos.x - bodySize.width/2;

	if (leftCheck - leftSide <= 0.5f)
	{
		if (enemyState == eEnemyState_active)
		{
			//CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("BossHuoQiu.ogg");
			Point tempPos = Point(pos.x - bodySize.width/4, pos.y + 3*(bodySize.height)/4);
			GameMap::getGameMap()->createNewBulletForBoss(tempPos, eEnemy_BossFire);
		}
	}

	if (leftCheck <= leftSide)
	{
		moveOffset *= -1;
		this->moveRight();
		return ;
	}

	float rightCheck = pos.x + bodySize.width/2;
	if (rightCheck >= rightSide)
	{
		moveOffset *= -1;
		this->moveLeft();
	}
}


// ********************** EnemyBossFire ****************** //
EnemyBossFire::EnemyBossFire()
{
	enemyState = eEnemyState_active;
	enemyType = eEnemy_BossFire;

	enemyBody = Sprite::create("image/enemy/bossBullet.png", CCRectMake(0, 0, 24, 8));
	enemyBody->setAnchorPoint(Point(0, 0));
	bodySize = CCSizeMake(24, 8);
	this->setContentSize(bodySize);
	this->addChild(enemyBody);
	this->setAnchorPoint(Point(0.5f, 0.0f));

	enemyLifeOver = SpriteFrame::create("image/enemy/bossBullet.png", CCRectMake(0, 0, 24, 8));
	enemyLifeOver->retain();
	overByArrow = enemyLifeOver;
	overByArrow->retain();

	moveOffset = -3.0f;

}

EnemyBossFire::~EnemyBossFire()
{
	this->unscheduleAllSelectors();
}
void EnemyBossFire::onEnter()
{
	Node::onEnter();
}
void EnemyBossFire::onExit()
{
	Node::onExit();
	enemyLifeOver->release();
	overByArrow->release();
}
void EnemyBossFire::launchEnemy()
{
	enemyBody->runAction(RepeatForever::create(AnimationManager::getInstance()->createAnimate(eAniBossFireLeft)));
	this->scheduleUpdate();
}

EnemyVSHero EnemyBossFire::checkCollisionWithHero()
{
	EnemyVSHero ret = eVS_nonKilled;

	Point heroPos = Hero::getInstance()->getPosition();
	Size heroSize = Hero::getInstance()->getContentSize();
	Rect heroRect = CCRectMake(heroPos.x - heroSize.width/2 + 2, heroPos.y + 2, 
		heroSize.width - 4, heroSize.height - 4);

	Point enemyPos = this->getPosition();
	Rect enemyRect = CCRectMake(enemyPos.x - bodySize.width/2, enemyPos.y, 
		bodySize.width, bodySize.height);

	if (heroRect.intersectsRect(enemyRect))
	{
		ret = eVS_heroKilled;
	}

	return ret;
}

void EnemyBossFire::forKilledByBullet()
{

}

void EnemyBossFire::forKilledByHero()
{

}

void EnemyBossFire::update(float dt)
{
	this->checkState();

	if (enemyState == eEnemyState_active)
	{
		Point currentPos = this->getPosition();
		currentPos.x += moveOffset;
		this->setPosition(currentPos);
	}
}


// ********************** EnemyAddMushroom ****************** //
EnemyAddMushroom::EnemyAddMushroom(int _addnum)
{
	enemyType = eEnemy_AddMushroom;
	enemyState = eEnemyState_nonactive;

	enemyBody = Sprite::create("image/enemy/Mushroom0.png", CCRectMake(0, 0, 16, 16));
	enemyBody->setAnchorPoint(Point(0.0f, 0.0f));
	bodySize = CCSizeMake(16, 16);
	this->setContentSize(bodySize);
	this->addChild(enemyBody);
	this->setAnchorPoint(Point(0.5f, 0.0f));

	enemyLifeOver = SpriteFrame::create("image/enemy/Mushroom0.png", CCRectMake(0, 0, 16, 16));
	enemyLifeOver->retain();
	overByArrow = SpriteFrame::create("image/enemy/Mushroom0.png", CCRectMake(16*3, 0, 16, 16));
	overByArrow->retain();

	addNums = _addnum;
	isAddable = true;
}

void EnemyAddMushroom::onEnter()
{
	Node::onEnter();
}
void EnemyAddMushroom::onExit()
{
	Node::onExit();
	enemyLifeOver->release();
	overByArrow->release();
}

EnemyAddMushroom::~EnemyAddMushroom()
{
	this->unscheduleAllSelectors();
}

void EnemyAddMushroom::launchEnemy()
{
	this->scheduleUpdate();
}

EnemyVSHero EnemyAddMushroom::checkCollisionWithHero()
{
	EnemyVSHero ret = eVS_nonKilled;

	Point heroPos = Hero::getInstance()->getPosition();
	Size heroSize = Hero::getInstance()->getContentSize();
	Rect heroRect = CCRectMake(heroPos.x - heroSize.width/2 + 2, heroPos.y + 2, 
		heroSize.width - 4, heroSize.height - 4);

	Point enemyPos = this->getPosition();
	Rect enemyRect = CCRectMake(enemyPos.x - bodySize.width/2 + 2, enemyPos.y + 2, 
		bodySize.width - 4, bodySize.height - 4);

	if (heroRect.intersectsRect(enemyRect))
	{
		ret = eVS_heroKilled;
	}

	return ret;
}

void EnemyAddMushroom::forKilledByHero()
{

}

void EnemyAddMushroom::update(float dt)
{
	this->checkState();

	if (enemyState == eEnemyState_active)
	{
		if (addNums)
		{
			if (isAddable)
			{
				isAddable = false;
				this->runAction(Sequence::create(MoveBy::create(0.5f, Point(0, 16)),
					CallFunc::create(this, callfunc_selector(EnemyAddMushroom::addMushroom)), NULL));

				this->runAction(Sequence::create(DelayTime::create(2.0f),
					CallFunc::create(this, callfunc_selector(EnemyAddMushroom::reSetNonAddable)), NULL));

			}
		}
		else 
		{
			enemyState = eEnemyState_over;
			enemyBody->stopAllActions();
			this->stopAllActions();
			this->unscheduleUpdate();
			this->setVisible(false);
		}
	}
}

void EnemyAddMushroom::addMushroom()
{
	--addNums;
	GameMap::getGameMap()->createNewBulletForBoss(this->getPosition(), eEnemy_mushroom);
	this->runAction(MoveBy::create(0.5f, Point(0, -16)));
}

void EnemyAddMushroom::reSetNonAddable()
{
	isAddable = true;
}


// ********************** EnemyBattery ****************** //
EnemyBattery::EnemyBattery(float delay)
{
	enemyType = eEnemy_Battery;
	enemyState = eEnemyState_nonactive;

	enemyBody = Sprite::create("image/enemy/battery.png");
	enemyBody->setAnchorPoint(Point(0, 0));
	bodySize = CCSizeMake(32, 32);
	this->setContentSize(bodySize);
	this->addChild(enemyBody);
	this->setAnchorPoint(Point(0.5f, 0.0f));

	enemyLifeOver = NULL;
	overByArrow = NULL;

	isFireable = true;

	fireDelay = delay;
}

EnemyBattery::~EnemyBattery()
{
	this->unscheduleAllSelectors();
}

void EnemyBattery::launchEnemy()
{
	firePos = Point(enemyPos.x - bodySize.width/2, enemyPos.y + bodySize.height/2);

	this->scheduleUpdate();
}

void EnemyBattery::update(float dt)
{
	this->checkState();

	if (enemyState == eEnemyState_active)
	{
		if (isFireable)
		{
			isFireable = false;

			this->addBatteryBullet();
			
			this->runAction(Sequence::create(DelayTime::create(fireDelay),
				CallFunc::create(this, callfunc_selector(EnemyBattery::reSetNonFireable)), NULL));
		}
	}
}

EnemyVSHero EnemyBattery::checkCollisionWithHero()
{
	EnemyVSHero ret = eVS_nonKilled;

	Point heroPos = Hero::getInstance()->getPosition();
	Size heroSize = Hero::getInstance()->getContentSize();
	Rect heroRect = CCRectMake(heroPos.x - heroSize.width/2 + 2, heroPos.y + 2, 
		heroSize.width - 4, heroSize.height - 4);

	Point enemyPos = this->getPosition();
	Rect enemyRect = CCRectMake(enemyPos.x - bodySize.width/2 + 2, enemyPos.y + 2, 
		bodySize.width - 4, bodySize.height - 4);

	if (heroRect.intersectsRect(enemyRect))
	{
		ret = eVS_heroKilled;
	}

	return ret;
}

void EnemyBattery::forKilledByBullet()
{
	// 炮台只会被火球子弹打爆
	switch (Global::getInstance()->getCurrentBulletType())
	{
	case eBullet_common:
		{
			enemyBody->runAction(Sequence::create(AnimationManager::getInstance()->createAnimate(eAniBatteryBoom),
				CallFunc::create(this, callfunc_selector(EnemyBattery::stopAndClear)), NULL));

			break;
		}
	default:
		break;
	}
}

void EnemyBattery::stopAndClear()
{
	enemyState = eEnemyState_over;

	enemyBody->stopAllActions();
	this->stopAllActions();
	this->unscheduleUpdate();
	this->setVisible(false);
}

void EnemyBattery::forKilledByHero()
{
	// 炮台不会被主角踩死
}

void EnemyBattery::addBatteryBullet()
{
	GameMap::getGameMap()->createNewBulletForBoss(firePos, eEnemy_BatteryBullet);
}

void EnemyBattery::reSetNonFireable()
{
	isFireable = true;
}


// ********************** EnemyBatteryBullet ****************** //
EnemyBatteryBullet::EnemyBatteryBullet()
{
	enemyType = eEnemy_BatteryBullet;
	enemyState = eEnemyState_active;

	enemyBody = Sprite::create("image/enemy/batteryBullet.png");
	enemyBody->setAnchorPoint(Point(0, 0));
	bodySize = CCSizeMake(4, 4);
	this->setContentSize(bodySize);
	this->addChild(enemyBody);
	this->setAnchorPoint(Point(0.5f, 0.5f));

	ccMoveOffset = -1.5f;
	moveOffset = ccMoveOffset;
}

EnemyBatteryBullet::~EnemyBatteryBullet()
{
	this->unscheduleAllSelectors();
}

void EnemyBatteryBullet::launchEnemy()
{
	this->scheduleUpdate();
}

EnemyVSHero EnemyBatteryBullet::checkCollisionWithHero()
{
	EnemyVSHero ret = eVS_nonKilled;

	Point heroPos = Hero::getInstance()->getPosition();
	Size heroSize = Hero::getInstance()->getContentSize();
	Rect heroRect = CCRectMake(heroPos.x - heroSize.width/2 + 2, heroPos.y + 2, 
		heroSize.width - 4, heroSize.height - 4);

	Point enemyPos = this->getPosition();

	// 炮台发出的子弹由于太小了，就直接用主角矩形是否包含这个小白点子弹的中心为准
	if (heroRect.containsPoint(enemyPos))
	{
		ret = eVS_heroKilled;
	}

	return ret;
}

void EnemyBatteryBullet::forKilledByBullet()
{

}

void EnemyBatteryBullet::forKilledByHero()
{

}

void EnemyBatteryBullet::update(float dt)
{
	this->checkState();

	if (enemyState == eEnemyState_active)
	{
		Point currentPos = this->getPosition();
		currentPos.x += moveOffset;
		this->setPosition(currentPos);
	}
}



// ********************** EnemyDarkCloud ****************** //
EnemyDarkCloud::EnemyDarkCloud(float _delay, int _type)
{
	enemyType = eEnemy_DarkCloud;
	enemyState = eEnemyState_nonactive;

	type = _type;
	switch (type)
	{
	case 0:
		enemyBody = Sprite::create("image/props/cloud.png", CCRectMake(114, 0, 32, 24));
		normal = SpriteFrame::create("cloud.png", CCRectMake(114, 0, 32, 24));
		bodySize = CCSizeMake(32, 32);
		dark = SpriteFrame::create("image/props/darkCloud.png", CCRectMake(114, 0, 32, 24));
		break;
	case 1:
		enemyBody = Sprite::create("image/props/cloud.png", CCRectMake(0, 0, 48, 24));
		normal = SpriteFrame::create("image/props/cloud.png", CCRectMake(0, 0, 48, 24));
		bodySize = CCSizeMake(64, 32);
		dark = SpriteFrame::create("image/props/darkCloud.png", CCRectMake(0, 0, 48, 24));
		break;
	case 2:
		enemyBody = Sprite::create("image/props/cloud.png", CCRectMake(49, 0, 64, 24));
		normal = SpriteFrame::create("image/props/cloud.png", CCRectMake(49, 0, 64, 24));
		bodySize = CCSizeMake(32, 32);
		dark = SpriteFrame::create("image/props/darkCloud.png", CCRectMake(49, 0, 64, 24));
		break;
	default:
		break;
	}

	dark->retain();
	normal->retain();

	enemyBody->setAnchorPoint(Point(0, 0));
	this->setContentSize(bodySize);
	this->addChild(enemyBody);
	this->setAnchorPoint(Point(0.5f, 0.0f));

	enemyLifeOver = NULL;
	overByArrow = NULL;

	delay = _delay;
	dropRegion = 64.0f;

	isDropable = true;
}

EnemyDarkCloud::~EnemyDarkCloud()
{
	this->unscheduleAllSelectors();
}
void EnemyDarkCloud::onEnter()
{
	Node::onEnter();
}
void EnemyDarkCloud::onExit()
{
	Node::onExit();
	dark->release();
	normal->release();
}

void EnemyDarkCloud::launchEnemy()
{
	leftSide = enemyPos.x - dropRegion;
	rightSide = enemyPos.x + dropRegion;

	this->scheduleUpdate();
}

void EnemyDarkCloud::update(float dt)
{
	this->checkState();

	Point heroPos = Hero::getInstance()->getPosition();

	if (enemyState == eEnemyState_active)
	{
		if (leftSide <= heroPos.x && heroPos.x <= rightSide)
		{
			if (isDropable)
			{
				isDropable = false;

				this->addLighting();

				this->runAction(Sequence::create(DelayTime::create(delay),
					CallFunc::create(this, callfunc_selector(EnemyDarkCloud::reSetDropable)), NULL));
			}
		}
	}
}

void EnemyDarkCloud::addLighting()
{
	enemyBody->setDisplayFrame(dark);
	GameMap::getGameMap()->createNewBulletForBoss(enemyPos, eEnemy_Lighting);
	this->runAction(Sequence::create(DelayTime::create(0.3f),
		CallFunc::create(this, callfunc_selector(EnemyDarkCloud::reSetNormal)), NULL));
}

void EnemyDarkCloud::reSetNormal()
{
	enemyBody->setDisplayFrame(normal);
}

void EnemyDarkCloud::reSetDropable()
{
	isDropable = true;
}

void EnemyDarkCloud::forKilledByHero()
{
	// 乌云是不能被主角和任何子弹打死的
}

void EnemyDarkCloud::forKilledByBullet()
{

}


// ********************** EnemyLighting ****************** //
EnemyLighting::EnemyLighting()
{
	enemyType = eEnemy_Lighting;
	enemyState = eEnemyState_active;

	enemyBody = Sprite::create("image/enemy/lighting.png", CCRectMake(0, 0, 16, 32));
	enemyBody->setAnchorPoint(Point(0, 0));
	bodySize = CCSizeMake(16, 32);
	this->setContentSize(bodySize);
	this->addChild(enemyBody);
	this->setAnchorPoint(Point(0.5f, 0.0f));

	enemyLifeOver = NULL;
	overByArrow = NULL;

	ccJumpOffset = 2.0f;
	jumpOffset = -ccJumpOffset;
}

EnemyLighting::~EnemyLighting()
{
	this->unscheduleAllSelectors();
}

void EnemyLighting::launchEnemy()
{
	enemyBody->runAction(RepeatForever::create(AnimationManager::getInstance()->createAnimate(eAniLighting)));

	this->scheduleUpdate();
}

void EnemyLighting::update(float dt)
{
	this->checkState();

	if (enemyState == eEnemyState_active)
	{
		Point currentPos = this->getPosition();
		currentPos.y += jumpOffset;
		this->setPosition(currentPos);
	}
}

void EnemyLighting::checkState()
{
	Point pos = this->getPosition();

	// 闪电的轨迹是从天上掉下来开始，一落到底，直到地板下面消失掉
	if (pos.y <= 2)
	{
		enemyState = eEnemyState_over;
		enemyBody->stopAllActions();
		this->stopAllActions();
		this->unscheduleUpdate();
		this->setVisible(false);
	}
}

EnemyVSHero EnemyLighting::checkCollisionWithHero()
{
	EnemyVSHero ret = eVS_nonKilled;

	Point heroPos = Hero::getInstance()->getPosition();
	Size heroSize = Hero::getInstance()->getContentSize();
	Rect heroRect = CCRectMake(heroPos.x - heroSize.width/2 + 2, heroPos.y + 2, 
		heroSize.width - 4, heroSize.height - 4);

	Point enemyPos = this->getPosition();
	Rect enemyRect = CCRectMake(enemyPos.x - bodySize.width/2 + 2, enemyPos.y + 2, 
		bodySize.width - 4, bodySize.height - 4);

	if (heroRect.intersectsRect(enemyRect))
	{
		ret = eVS_heroKilled;
	}

	return ret;
}

void EnemyLighting::forKilledByBullet()
{
	// 闪电这种东西是不能被任何自担打掉的
}

void EnemyLighting::forKilledByHero()
{

}





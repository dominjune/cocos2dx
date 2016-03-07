#include "Bullet.h"
#include "AnimationManager.h"
#include "Hero.h"
#include "GameMap.h"
#include "GameLayer.h"

// ****************** Bullet ******************** //
Bullet::Bullet():
bulletBody(NULL)
{
	Point heroPos = Hero::getInstance()->getPosition();
	Size heroSize = Hero::getInstance()->getContentSize();
	startPos = Point(heroPos.x, heroPos.y + heroSize.height/2);
}

Bullet::~Bullet()
{

}

void Bullet::setBodySize(Size _size)
{
	bodySize = _size;
}
Size Bullet::getBodySize()
{
	return bodySize;
}

void Bullet::setStartPos(Point pos)
{
	startPos = pos;
}
Point Bullet::getStartPos()
{
	return startPos;
}

BulletType Bullet::getBulletType()
{
	return bulletType;
}

BulletState Bullet::getBulletState()
{
	return bulletState;
}

void Bullet::checkBulletState()
{

}

Rect Bullet::getBulletRect()
{
	Point pos = this->getPosition();
	return CCRectMake(pos.x - bodySize.width/2, pos.y, bodySize.width, bodySize.height);
}

void Bullet::forKilledEnemy()
{

}

// ****************** BulletCommon ******************** //
BulletCommon::BulletCommon()
{
	bulletType = eBullet_common;
	bulletState = eBulletState_active;
	isLand = false;

	bodySize = CCSizeMake(10, 10);
	bulletBody = Sprite::create("image/props/fireRight.png", CCRectMake(0, 0, 10, 10));
	bulletBody->setAnchorPoint(Point(0, 0));
	this->setContentSize(bodySize);
	this->addChild(bulletBody);
	this->setAnchorPoint(Point(0.5f, 0.0f));

	moveOffset = 0.0f;
	ccMoveOffset = 5.0f;
	jumpOffset = 0.0f;
	ccJumpOffset = 0.3f;

	switch (Hero::getInstance()->face)
	{
	case eRight:
		moveOffset = ccMoveOffset;
		break;
	case eLeft:
		moveOffset = -ccMoveOffset;
		break;
	default:
		break;
	}
}

BulletCommon::~BulletCommon()
{
	this->unscheduleAllSelectors();
}

void BulletCommon::launchBullet()
{
	bulletBody->runAction(RepeatForever::create(AnimationManager::getInstance()->createAnimate(eAniRotatedFireBall)));
	
	this->scheduleUpdate();
}

// �ӵ���������ײ���
// �������������land��block��pipe�Ȼ��Զ���ը��
void BulletCommon::commonBulletCollisionH()
{
	Point currentPos = this->getPosition();

	// �ж��ӵ��Ƿ�������Ļ
	float leftSide = currentPos.x - bodySize.width/2;
	float rightSide = currentPos.x + bodySize.width/2;
	float mapMaxH = GameLayer::getMapMaxH();
	Size winSize = Director::getInstance()->getWinSize();
	if (fabs(leftSide - mapMaxH) <= 8)
	{
		this->showBoom();
		return ;
	}

	// �ж��Ƿ������ͼ�Ҳ�
	float mapRightSide = GameMap::getGameMap()->mapSize.width*
		GameMap::getGameMap()->tileSize.width;
	if (fabs(rightSide - mapRightSide) <= 8)
	{
		this->showBoom();
		return ;
	}

	if (leftSide - mapMaxH >= winSize.width)
	{
		bulletBody->stopAllActions();
		this->unscheduleUpdate();
		this->autoClear();
	}

	// �Ҳ�
	Point rightCollision = Point(currentPos.x + bodySize.width/2, currentPos.y + bodySize.height/2);
	Point rightTilecoord = GameMap::getGameMap()->positionToTileCoord(rightCollision);
	TileType tileType = GameMap::getGameMap()->tileTypeforPos(rightTilecoord);
	switch (tileType)
	{
	case eTile_Land:
	case eTile_Pipe:
	case eTile_Block:
	case eTile_Flagpole:
		this->showBoom();
		return ;
		break;
	}

	// ���
	Point leftCollision = Point(currentPos.x - bodySize.width/2, currentPos.y + bodySize.height/2);
	Point leftTilecoord = GameMap::getGameMap()->positionToTileCoord(leftCollision);
	tileType = GameMap::getGameMap()->tileTypeforPos(leftTilecoord);
	switch (tileType)
	{
	case eTile_Land:
	case eTile_Pipe:
	case eTile_Block:
	case eTile_Flagpole:
		this->showBoom();
		return ;
		break;
	}
}

// ��Ϊ�ӵ�ֻ�������䣬��������������ŵ׵��ж�
// �ŵ�����Ӵ���land��block�����Զ����������ǽŵ�����pipeҲ�ᱬը
void BulletCommon::commonBulletCollisionV()
{
	Point currentPos = this->getPosition();

	Point downCollision = currentPos;
	Point downTilecoord = GameMap::getGameMap()->positionToTileCoord(downCollision);
	downTilecoord.y += 1;
	Point downPos = GameMap::getGameMap()->positionToTileCoord(downTilecoord);
	downPos = Point(currentPos.x, downPos.y + GameMap::getGameMap()->getTileSize().height);

	TileType tileType = GameMap::getGameMap()->tileTypeforPos(downTilecoord);
	switch (tileType)
	{
	case eTile_Land:
		isLand = true;
		jumpOffset = 3.0f;
		this->setPosition(downPos);
		return ;
		break;
	case eTile_Pipe:
	case eTile_Block:
		this->showBoom();
		return ;
		break;
	case eTile_Trap:
		{
			bulletBody->stopAllActions();
			this->unscheduleUpdate();
			this->autoClear();
			return ;
			break;
		}
	default:
		break;
	}

	jumpOffset -= ccJumpOffset;
}

void BulletCommon::update(float dt)
{
	if (bulletState == eBulletState_active)
	{
		Point currentPos = this->getPosition();
		currentPos.x += moveOffset;
		currentPos.y += jumpOffset;
		this->setPosition(currentPos);

		this->commonBulletCollisionH();
		this->commonBulletCollisionV();
	}
}

void BulletCommon::showBoom()
{	
	bulletBody->stopAllActions();
	this->stopAllActions();
	this->unscheduleUpdate();
	bulletBody->runAction(Sequence::create(AnimationManager::getInstance()->createAnimate(eAniBoomedFireBall), 
		CallFunc::create(this, callfunc_selector(BulletCommon::autoClear)), NULL));
}

void BulletCommon::autoClear()
{	
	bulletState = eBulletState_nonactive;
	this->setVisible(false);
}

void BulletCommon::forKilledEnemy()
{
	this->showBoom();
}


// ****************** BulletArrow ******************** //
BulletArrow::BulletArrow()
{
	bulletType = eBullet_arrow;
	bulletState = eBulletState_active;

	bodySize = CCSizeMake(16, 16);
	bulletBody = Sprite::create("image/props/arrow.png");
	bulletBody->setAnchorPoint(Point(0, 0));
	this->setContentSize(bodySize);
	this->addChild(bulletBody);
	this->setAnchorPoint(Point(0.5f, 0.5f));

	ccMoveOffset = 6.0f;
	CCActionInstant *flipX = CCFlipX::create(true);

	switch (Hero::getInstance()->face)
	{
	case eRight:
		moveOffset = ccMoveOffset;
		break;
	case eLeft:
		moveOffset = -ccMoveOffset;
		bulletBody->runAction(flipX);
		break;
	default:
		break;
	}

}

BulletArrow::~BulletArrow()
{
	this->unscheduleAllSelectors();
}

void BulletArrow::launchBullet()
{
	this->scheduleUpdate();
}

void BulletArrow::arrowBulletCollisionH()
{
	Point currentPos = this->getPosition();
	
	// �ж��ӵ��Ƿ�������Ļ
	float leftSide = currentPos.x - bodySize.width/2;
	float rightSide = currentPos.x + bodySize.width/2;
	float mapMaxH = GameLayer::getMapMaxH();
	Size winSize = Director::getInstance()->getWinSize();
	if (fabs(leftSide - mapMaxH) <= 8)
	{
		this->broken();
		return ;
	}
	// �ж��Ƿ������ͼ�Ҳ�
	float mapRightSide = GameMap::getGameMap()->mapSize.width*
		GameMap::getGameMap()->tileSize.width;
	if (fabs(rightSide - mapRightSide) <= 8)
	{
		this->broken();
		return ;
	}
	
	if (leftSide - mapMaxH >= winSize.width)
	{
		bulletBody->stopAllActions();
		this->stopAllActions();
		this->unscheduleUpdate();
		this->autoClear();
	}

	// �Ҳ�
	Point rightCollision = Point(currentPos.x + bodySize.width/2, currentPos.y);
	Point rightTilecoord = GameMap::getGameMap()->positionToTileCoord(rightCollision);
	TileType tileType = GameMap::getGameMap()->tileTypeforPos(rightTilecoord);
	switch (tileType)
	{
	case eTile_Land:
	case eTile_Pipe:
	case eTile_Block:
	case eTile_Flagpole:
		this->broken();
		return ;
		break;
	}

	// ���
	Point leftCollision = Point(currentPos.x - bodySize.width/2, currentPos.y);
	Point leftTilecoord = GameMap::getGameMap()->positionToTileCoord(leftCollision);
	tileType = GameMap::getGameMap()->tileTypeforPos(leftTilecoord);
	switch (tileType)
	{
	case eTile_Land:
	case eTile_Pipe:
	case eTile_Block:
	case eTile_Flagpole:
		this->broken();
		return ;
		break;
	}
}

void BulletArrow::forKilledEnemy()
{
	bulletState = eBulletState_nonactive;
	bulletBody->stopAllActions();
	this->stopAllActions();
	this->unscheduleUpdate();
	this->setVisible(false);
}

void BulletArrow::update(float dt)
{
	if (bulletState == eBulletState_active)
	{
		Point currentPos = this->getPosition();
		currentPos.x += moveOffset;

		this->setPosition(currentPos);
		this->arrowBulletCollisionH();
	}
}

Rect BulletArrow::getBulletRect()
{
	Point pos = this->getPosition();
	return CCRectMake(pos.x - 6, pos.y - 5, 12, 10);
}

void BulletArrow::autoClear()
{
	bulletState = eBulletState_nonactive;
	this->setVisible(false);
}

void BulletArrow::broken()
{
	bulletBody->stopAllActions();
	this->stopAllActions();
	this->unscheduleUpdate();

	bulletBody->runAction(Sequence::create(AnimationManager::getInstance()->createAnimate(eAniArrowBroken),
		CallFunc::create(this, callfunc_selector(BulletArrow::autoClear)), NULL));
}
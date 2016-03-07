/*************************************************
Copyright:bro7
Author:ben
Date:2014-07-05
Description:超级玛丽的子弹
**************************************************/

#ifndef _BULLET_
#define _BULLET_

#include "cocos2d.h"
#include "GameEnum.h"
using namespace cocos2d;

class Bullet : public Node
{
public:
	Bullet();
	virtual ~Bullet();

	void setBodySize(Size _size);
	Size getBodySize();

	void setStartPos(Point pos);
	Point getStartPos();

	BulletType getBulletType();

	BulletState getBulletState();

	virtual void checkBulletState();

	// 纯虚函数
	virtual void launchBullet() = 0;

	virtual void forKilledEnemy();

	virtual Rect getBulletRect();

protected:
	BulletType bulletType;  // 子弹所属的类型
	Sprite *bulletBody;
	Size bodySize;

	Point startPos;  // 子弹发出点

	BulletState bulletState; // 子弹的活动状态
};

// 传统的普通子弹
class BulletCommon : public Bullet
{
public:
	BulletCommon();
	~BulletCommon();

	// 实现父类的纯虚函数，启动子弹
	void launchBullet();

	void commonBulletCollisionH();
	void commonBulletCollisionV();

	void update(float dt);

	void forKilledEnemy();
private:
	float moveOffset;
	float ccMoveOffset;

	float jumpOffset;
	float ccJumpOffset;

	bool isLand;  // 火焰子弹的轨迹：先是右下直线然后是地面上的抛物线

	void showBoom();
	void autoClear();
};

// 弓箭
class BulletArrow : public Bullet
{
public:
	BulletArrow();
	~BulletArrow();

	void launchBullet();

	void forKilledEnemy();

	void update(float dt);

	void arrowBulletCollisionH();

	Rect getBulletRect();
private:
	// 弓箭只有水平方向的移动
	float moveOffset;
	float ccMoveOffset;

	void autoClear();
	void broken();
};

#endif
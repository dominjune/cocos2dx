/*************************************************
Copyright:bro7
Author:ben
Date:2014-07-05
Description:�����������ӵ�
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

	// ���麯��
	virtual void launchBullet() = 0;

	virtual void forKilledEnemy();

	virtual Rect getBulletRect();

protected:
	BulletType bulletType;  // �ӵ�����������
	Sprite *bulletBody;
	Size bodySize;

	Point startPos;  // �ӵ�������

	BulletState bulletState; // �ӵ��Ļ״̬
};

// ��ͳ����ͨ�ӵ�
class BulletCommon : public Bullet
{
public:
	BulletCommon();
	~BulletCommon();

	// ʵ�ָ���Ĵ��麯���������ӵ�
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

	bool isLand;  // �����ӵ��Ĺ켣����������ֱ��Ȼ���ǵ����ϵ�������

	void showBoom();
	void autoClear();
};

// ����
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
	// ����ֻ��ˮƽ������ƶ�
	float moveOffset;
	float ccMoveOffset;

	void autoClear();
	void broken();
};

#endif
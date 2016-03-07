/*************************************************
Copyright:bro7
Author:ben
Date:2014-07-05
Description:��Ϸ�е����еĵжԶ���
**************************************************/

#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "cocos2d.h"
#include "GameEnum.h"
using namespace cocos2d;

class Enemy : public Node
{
public:
	Enemy();
	virtual ~Enemy();

	void setTileCoord(Point _tileCoord);
	Point getTileCoord();
	void setEnemyPos(Point _enemyPos);
	Point getEnemyPos();
	void setBodySize(Size _size);
	Size getBodySize();

	void setMoveOffset(float _moveoffset);
	float getMoveOffset();

	void setccMoveOffset(float _ccmoveoffset);
	float getccMoveOffset();

	EnemyType getEnemyType();

	virtual Rect getEnemyRect();

	// ר������������չʾ���ﶯ���Լ��ƶ�����ײ���ĺ���
	// ���麯������ΪEnemy�������ʾ��һ������������ϸ��Ҫ���ݾ����������
	virtual void launchEnemy() = 0;
	virtual void enemyCollistionH();
	virtual void enemyCollistionV();

	// �͹���״̬������ص�
	void setEnemyState(EnemyState _state);
	EnemyState getEnemyState();
	void checkState();
	virtual void stopEnemyUpdate();

	// �жϵ�ǰ���͵Ĺ����Ƿ�����������
	virtual EnemyVSHero checkCollisionWithHero();
	// �������Ƿ����ǲ�����Ģ���ڹ��ǿ��Ա���ô�����ģ�����ʳ�˻��Ͳ���
	//virtual bool checkKilled();
	// ���ﱻ���ǲ���ʱ���õĺ���
	virtual void forKilledByHero();
	void setNonVisibleForKilledByHero();

	// ���ӵ��������õĺ���
	virtual void forKilledByBullet();
	void setNonVisibleForKilledByBullet();

protected:
	EnemyType enemyType;  // ������ʾ���������
	Sprite *enemyBody;  // չʾ����ľ���
	Size bodySize;      // ����ĳߴ�
	SpriteFrame *enemyLifeOver;  // ������ʾ����ҵ��ľ���֡
	SpriteFrame *overByArrow;    // ���������ľ���֡

	Point tileCoord;   // ������ʾ���������ĵ�ͼ����ϵ�µ�
	Point enemyPos;    // GL����ϵ�µģ�����ֱ�����ù����λ��

	marioDirection startFace;  // �տ�������״̬ʱ������ĳ���

	// ˮƽ�����ƶ����Ʊ���
	float moveOffset;
	float ccMoveOffset;
	// ��ֱ�������ƶ����Ʊ���
	float jumpOffset;
	float ccJumpOffset;

	// ��ʶ����ĵ�ǰ״̬
	// �涨����Ļ�еĹ������ڻ�Ծ״̬����Ļ֮ǰ��֮��ľ�Ϊ�ǻ�Ծ״̬���ٻ���������
	EnemyState enemyState;

};

// ����Ģ������ǰһֱ��Ϊ�ǰײˣ�
class EnemyMushroom : public Enemy
{
public:
	EnemyMushroom();
	~EnemyMushroom();

	// ʵ�ָ���Ĵ��麯��
	void launchEnemy();
	void onEnter();
	void onExit();
	//void enemyCollistionH();
	//void enemyCollistionV();

	void update(float dt);
};

// ���˻����ӹܵ���ð�����ģ���Ҫ����ͼ��Ĺܵ�����ڵ���ϵʵ�ֳ��˻�������Ч��
class EnemyFlower : public Enemy
{
public:
	EnemyFlower();
	~EnemyFlower();

	// ʵ�ָ���Ĵ��麯��
	void launchEnemy();
	void onEnter();
	void onExit();
	void update(float dt);
	// ��Ϊʳ�˻��������㷨��֮ǰ��Ģ���ڹ�Ȳ�ͬ����Ҫ��д����麯��
	EnemyVSHero checkCollisionWithHero() override;

	// ��д��ȡ���κ���
	Rect getEnemyRect();

protected:
	Point startPos;  // ��ʶʳ�˻��ճ���ʱ��λ��
};

// �ڹ꣬�����ߵ�����
class EnemyTortoise : public Enemy
{
public:
	EnemyTortoise(int _startface);
	~EnemyTortoise();
	void onEnter();
	void onExit();

	// ʵ�ָ���Ĵ��麯��
	void launchEnemy();
	// ��дˮƽ������ײ��⣬��Ϊ�ڹ�����ˮƽ�����ϵĵ�ͷ
	void enemyCollistionH();
	void update(float dt);

private:
	SpriteFrame *leftFace;// = nullptr;
	SpriteFrame *rightFace;// = nullptr;
};

// �ڹ꣬�����ߵģ����������˶�����
class EnemyTortoiseRound : public Enemy
{
public:
	EnemyTortoiseRound(float dis);
	~EnemyTortoiseRound();

	void launchEnemy();
	void onEnter();
	void onExit();
	// ��д�����ˮƽ������£����������ڹ�Ҳֻ��Ҫˮƽ����ĸ���
	//void enemyCollistionH();

	void update(float dt);

	void setRoundDis(float dis);
	float getRoundDis();
private:
	float roundDis;  // ˮƽ�����������ľ���

	void reRight();
	void reLeft();
};

// ��ɵ��ڹ�
class EnemyTortoiseFly : public Enemy
{
public:
	EnemyTortoiseFly(float dis);
	~EnemyTortoiseFly();
	void onEnter();
	void onExit();
	void launchEnemy();

	void update(float dt);

	void setFlyDis(float dis);
	float getFlyDis();

private:
	float flyDis;
};

// ��
class EnemyFireString : public Enemy
{
public:
	EnemyFireString(float _begAngle, float _time);
	~EnemyFireString();
	void onEnter();
	void onExit();

	// �����𴮺������ת��û��״̬��⣬��������������ǻ�Ծ״̬�����ҹ����в���
	void launchEnemy();

	// ��д�����ֹͣ������£���Ϊ������������
	void stopEnemyUpdate();

	// �������ǵ���ײ����кܴ�ͬ
	EnemyVSHero checkCollisionWithHero();

	// ��������д������Ϊ��
	void forKilledByBullet();
	void forKilledByHero();	

	void update(float dt);

private:
	Sprite *enemyBody2;  // ��������������
	Sprite *enemyBody3;

	Size fireSize;  // ��������ĳߴ磬�������ǵ���ײ��������������Ҫ������õ�
	Array *pArrayFire;  // �������������

	float begAngle;  // ������תǰ�������ĽǶ�
	float time;      // ����תһ������Ҫ��ʱ��

	double angle;  // ����ʱ�̣�������ʱ�뷽��ĽǶȣ�����ֵ
	double PI;  // ��ת���õ�Բ����

	void launchFireString();
};

// ����ͨͨ�ķ���
class EnemyFlyFish : public Enemy
{
public:
	EnemyFlyFish(float _offsetH, float _offsetV, float _duration);
	~EnemyFlyFish();
	void onEnter();
	void onExit();
	void launchEnemy();
	void update(float dt);
	void checkState();

	// ������·������������ڱ����ǲ���������ֻҪ���������ཻ��Ϊ��������
	EnemyVSHero checkCollisionWithHero();

	// ���汻���ǲ����ĺ���Ϊ�գ����Ǳ��ӵ����е���������û����
	void forKilledByHero();

private:
	float offsetH;  // ����ˮƽ������ƫ��
	float offsetV;  // ��ֱ������ƫ��
	float offsetDuration;  // ƫ�Ƴ���ʱ��
	bool isFlying;    // �Ƿ��Ƿ���״̬����ֹһ���ط�����ֹ�ķ���

	void flyInSky();
	void reSetNotInSky();
};

// Boss
class EnemyBoss : public Enemy
{
public:
	EnemyBoss();
	~EnemyBoss();

	void launchEnemy();
	void onEnter();
	void onExit();
	EnemyVSHero checkCollisionWithHero();

	void forKilledByBullet();
	void forKilledByHero();

	void enemyCollistionH();

	void update(float dt);

private:

	float leftSide;
	float rightSide;

	void moveLeft();
	void moveRight();
};

// Boss�������ӵ���Ҳ�����һ�ֹ���
class EnemyBossFire : public Enemy
{
public:
	EnemyBossFire();
	~EnemyBossFire();

	void launchEnemy();
	void onEnter();
	void onExit();
	EnemyVSHero checkCollisionWithHero();

	void forKilledByBullet();
	void forKilledByHero();

	void update(float dt);
private:

};


// ���ڹܵ����������ϰ������ģ�����ð��һ������Ģ���Ĺ���
class EnemyAddMushroom : public Enemy
{
public:
	EnemyAddMushroom(int _addnum);
	~EnemyAddMushroom();

	void launchEnemy();
	void onEnter();
	void onExit();
	EnemyVSHero checkCollisionWithHero();

	void forKilledByHero();

	void update(float dt);
private:
	int addNums;  // һ��Ҫ����Ģ���ĸ���
	bool isAddable;  // ÿ��һ��ʱ������һ��������ڿ��ƹ��������ٶ�

	void addMushroom();
	void reSetNonAddable();  // ������Ģ������������Ϊ��������
};


// �궷����̨
class EnemyBattery : public Enemy
{
public:
	EnemyBattery(float delay);
	~EnemyBattery();

	void launchEnemy();

	EnemyVSHero checkCollisionWithHero();

	void forKilledByBullet();  // ������ֻ̨�ܱ������
	void forKilledByHero();  // ��̨���ܱ����ǲ���

	void update(float dt);

private:

	bool isFireable;  // ���ڿ��Ʒ����ӵ���Ƶ��
	float fireDelay;  // �����ӵ�֮���ʱ����

	void addBatteryBullet();
	void reSetNonFireable();  // ��������Ϊ���ɷ����ӵ�

	void stopAndClear();

	Point firePos;   // ����������ӵ��ĳ�ʼλ��
};


// ��̨�������ӵ���С�׵�
class EnemyBatteryBullet : public Enemy
{
public:
	EnemyBatteryBullet();
	~EnemyBatteryBullet();

	void launchEnemy();

	EnemyVSHero checkCollisionWithHero();

	void forKilledByBullet();
	void forKilledByHero();

	void update(float dt);
private:

};


// ���ƹ�����������ǿ�������ʱ��������
class EnemyDarkCloud : public Enemy
{
public:
	EnemyDarkCloud(float _delay, int _type);
	~EnemyDarkCloud();

	void onEnter();
	void onExit();

	void launchEnemy();

	void update(float dt);

	void forKilledByBullet();  // ���ϵ����Ʋ��ᱻ�ӵ�����
	void forKilledByHero();    // Ҳ���ᱻ���ǲ���������������Ϊ��

private:
	float dropRegion;  // ������������� һ��̶�
	float leftSide;    // �������߽�
	float rightSide;   // �ұ߽�

	bool isDropable;   // �����Ƿ���Ե������Ƶı���

	float delay;       // ��������֮���ʱ����  ��Ҫ�ڶ�����趨

	void addLighting();
	void reSetDropable();

	int type;   // ���ƵĴ�С�ͺ� 0 1 2�ֱ����С�д�

	SpriteFrame *dark;   // ���Ƶ��������һ˲չʾ��
	SpriteFrame *normal; // �����İ���
	void reSetNormal();
};


// ����
class EnemyLighting : public Enemy
{
public:
	EnemyLighting();
	~EnemyLighting();

	EnemyVSHero checkCollisionWithHero();

	void launchEnemy();

	void checkState();

	void update(float dt);

	void forKilledByHero();
	void forKilledByBullet();

private:

};



#endif
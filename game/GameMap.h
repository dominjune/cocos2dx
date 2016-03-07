/*************************************************
Copyright:bro7
Author:ben
Date:2014-07-05
Description:��Ϸ���˵�ҳ�Ĺ���ҳ��
**************************************************/

#ifndef _GAMEMAP_H_
#define _GAMEMAP_H_

#include "cocos2d.h"
#include "GameEnum.h"
using namespace cocos2d;

class CCItem;
class Enemy;
class Gadget;

class GameMap : public TMXTiledMap
{
	// ֻ���������ĸ���Ϸ��Ҫ�õ���ͼ��
	// �Ʋ������ƶ�

	CC_PROPERTY_READONLY(TMXLayer*, cloudLayer, CloudLayer);
	// ʣ����ĸ�ͼ����Ϊ����ײ���
	CC_PROPERTY_READONLY(TMXLayer*, blockLayer, BlockLayer);
	CC_PROPERTY_READONLY(TMXLayer*, pipeLayer, PipeLayer);
	CC_PROPERTY_READONLY(TMXLayer*, landLayer, LandLayer);
	CC_PROPERTY_READONLY(TMXLayer*, trapLayer, TrapLayer);
	// �����
	CC_PROPERTY_READONLY(TMXObjectGroup*, objectLayer, ObjectLayer);
	// ��Ҳ�
	CC_PROPERTY_READONLY(TMXLayer*, coinLayer, CoinLayer);
	// ��˲�
	CC_PROPERTY_READONLY(TMXLayer*, flagpoleLayer, FlagpoleLayer);
public:
	GameMap();
	~GameMap();

	//// ���ͼ��Ϣ���
	Size tileSize; // ��ͼ�е�ͼ��ߴ�
	Size mapSize;  // ��ͼ��ͼ��ĸ���

	// �����������ڹ��سɹ���չʾ�����õ���
	Point flagPoint;  // С���ӵĵ�
	Sprite *pFlag;
	void showFlagMove();  // չʾ���Ӵ���������µĶ���
	Point finalPoint;  // ��������Ҫ�ƶ��������ſڵĵ�
	bool isBossMap;  // ��ʶ�Ƿ�ΪBoss��

	void createNewBulletForBoss(Point pos, EnemyType _enemyType); // Boss������һ���ӵ�

	// ��ͨ�ĳ��˻����Ģ��������
	// ����Ģ�������ݵ�ǰ����µ����ʹ�С���ж϶���������Ģ�������ӵ�
	// ��¼��Ģ�����ǻ���ͼ������ϵ�µ�
	PointArray *pMushroomPointArray;

	Sprite *pAddLifeMushroom;  // ����Ģ������
	Point addLifePoint;  // ÿһ���ж���һ�����˵�Ģ����


	// ��ͼ�ϱ�������й������Ϣ����
	Array *pEnemyArray;
	// ��ͼ�����е��ӵ�����Ϣ
	Array *pBulletArray;

	// ��ͼ�����е�С���������Ϣ
	Array *pGadgetArray;
	// ��ǰ�������Ǹ������
	Gadget *heroInGadget;

	// ��¼��ǰ��Ļ�еĹ�������
	//Array *pEnemyInScreen;
	bool isHeroInGadget(Point heroPos, float &gadgetLevel);

	Sprite *pMushSprite;  //��ǰ��������Ģ������
	Point mushTileCoord;  // ��ǰĢ�����ڵ�λ��

	CCItem *pItem; // ��ǰ��������˸���ʺ�

	SpriteFrame *_brokenCoin;  // ���ʺŵĽ�ұ�������Ҫ���滻����������ʽ

	// ��ǰ��Ҫ������Ľ��ש��λ��
	Point resetCoinPoint;

	// ÿ�ŵ�ͼ������µĳ����㣬GL����ϵ�µ����꣬GameLayer��ȡ��ֱ�ӿ��õ�
	Point marioBirthPos;
	Point getMarioBirthPos();

	static GameMap* create(const char *tmxFile);

	// ���ص�ͼָ��λ���ϵ�ͼ�����ͣ�������ײ���
	TileType tileTypeforPos(Point tileCoord);

	// ��ͼ�ϵ�blockͼ���ϵ�ĳЩש���ǿ��Ա����Ƕ���
	void breakBlock(Point tileCoord, BodyType _bodyType);

	// չʾblockͼ���ϵ�ĳЩש�鱻������¶�����Ŀ�Ķ���
	void showBlockBroken(Point tileCoord);
	// չʾ��˸���ʺű�������ҵĶ���
	void showJumpUpBlinkCoin(Point tileCoord);
	// չʾש�鱻С����¶�һ�µ�Ч��
	void showBlockJump(Point tileCoord);
	// ����������´����С����ұ��������Ĺ����л�����Ž�ҵ�ש�����ƶ�����������showJumpBlock
	void showCoinJump(Point tileCoord);
	// ��չʾ���˽��ש��/Ģ��ש������ƶ�����Ҫ�������ý��ש���Ǳ�������״̬����
	void resetCoinBlockTexture();
	// ����һֻĢ�������ݵ�ǰ�������״̬���ж�Ҫ���ɵ��Ǳ��Ģ�������ӵ�Ģ��
	void showNewMushroom(Point tileCoord, BodyType _bodyType);
	// ����һֻ����Ģ��
	void showAddLifeMushroom(Point tileCoord);

	// �ж�������Ƿ�Ե�Ģ��
	bool isMarioEatMushroom(Point tileCoord);
	// �ж��Ƿ�ٵ�����Ģ��
	bool isMarioEatAddLifeMushroom(Point tileCoord);

	// ������ͼ�ϵ����й����������
	void launchEnemyInMap();

	// ������ͼ�����е����
	void launchGadgetInMap();

	void update(float dt);
	// ֹͣ������ӵ��ĸ��£�Ȼ����ֹͣ�Լ��ĸ���
	void stopUpdateForHeroDie();

	void pauseGameMap();  // ��ͣ��ͼ�ϵĸ��ֻᶯ��
	void resumeGameMap();  // ���¶�����

	// �����������active״̬�еĹ������������⣬Ȼ����ֳ������߼�
	void enemyVSHero();

	// ��Ϸͼ������ϵ��tilemapͼ������ϵ֮���ת��
	Point positionToTileCoord(Point pos);
	Point tilecoordToPosition(Point tileCoord);

	// ����һ���µ��ӵ�������������
	void createNewBullet();

	// �ӵ���������ײ���
	void bulletVSEnemy();

	static GameMap *_gameMap;
	static GameMap* getGameMap();
private:
	void deleteOneMushPointFromArray(Point tileCoord);
	void clearItem();
	void clearSpriteArray();
	void extraInit();
	void initObjects();
	bool itemCoordArrayContains(Point tileCoord);
	bool mushroomPointContains(Point tileCoord);
	PointArray *pItemCoordArray;
	Array *pSpriteArray;

	// ���Boss��׼���ı���
	Point bridgeTileStartPos;
	int bridgeTileNums;
	Enemy *pBossEnemy;
	void initBridgeArray();
	Sprite *pPrincess;  // ��������

	// ���ڶ�ש��������ɹ������ʱ����
	Point enemyTilePos;
	Enemy *pRandomEnemy;
	void randomShowEnemy();
	void randomLaunchEnemy();
};

#endif
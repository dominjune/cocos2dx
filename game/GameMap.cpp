#include "GameMap.h"
#include "AnimationManager.h"
#include "Item.h"
#include "Enemy.h"
#include "GameLayer.h"
#include "Hero.h"
#include "Bullet.h"
#include "Global.h"
#include "SimpleAudioEngine.h"

GameMap* GameMap::_gameMap;
GameMap* GameMap::getGameMap()
{
	return _gameMap;
}

GameMap::GameMap()
{
	Texture2D *pTexture = TextureCache::getInstance()->addImage("image/tmx/superMarioMap.png");
	_brokenCoin = SpriteFrame::createWithTexture(pTexture, CCRectMake(1, 18, 16, 16));
	_brokenCoin->retain();

	pItemCoordArray = PointArray::create(100);
	pItemCoordArray->retain();

	pSpriteArray = Array::createWithCapacity(4);
	pSpriteArray->retain();

	pMushroomPointArray = PointArray::create(100);
	pMushroomPointArray->retain();

	pEnemyArray = Array::create();
	pEnemyArray->retain();

	pBulletArray = Array::create();
	pBulletArray->retain();

	pGadgetArray = Array::create();
	pGadgetArray->retain();

	pMushSprite = NULL;
	pAddLifeMushroom = NULL;
	pItem = NULL;
	heroInGadget = NULL;

	isBossMap = false;

	_gameMap = this;

	enemyTilePos = Point(0, 0)/*CCPointZero*/;
	pRandomEnemy = NULL;

	// רΪBoss��ͼ׼����
	bridgeTileStartPos = Point(0, 0)/*CCPointZero*/;
	bridgeTileNums = 0;
	pBossEnemy = NULL;
	pPrincess = NULL;
}

GameMap::~GameMap()
{
	this->unscheduleAllSelectors();

	//CC_SAFE_DELETE(pItemCoordArray);
	//CC_SAFE_DELETE(pEnemyArray);
	//CC_SAFE_DELETE(pBulletArray);
	//CC_SAFE_DELETE(pMushroomPointArray);

	//CC_SAFE_DELETE(landLayer);
	//CC_SAFE_DELETE(blockLayer);
	//CC_SAFE_DELETE(pipeLayer);
	//CC_SAFE_DELETE(coinLayer);
	//CC_SAFE_DELETE(trapLayer);
	//CC_SAFE_DELETE(cloudLayer);
	//CC_SAFE_DELETE(objectLayer);

	//pItemCoordArray->release();
	//pEnemyArray->release();
}

GameMap* GameMap::create(const char *tmxFile)
{
	GameMap *pGameMap = new GameMap();
	if (pGameMap && pGameMap->initWithTMXFile(tmxFile))
	{
		pGameMap->extraInit();
		pGameMap->autorelease();
		return pGameMap;
	}
	CC_SAFE_DELETE(pGameMap);
	return NULL;
}

void GameMap::extraInit()
{
	tileSize = this->getTileSize();
	mapSize = this->getMapSize();

	cloudLayer = this->getLayer("cloud");
	blockLayer = this->getLayer("block");
	pipeLayer = this->getLayer("pipe");
	landLayer = this->getLayer("land");
	trapLayer = this->getLayer("trap");
	objectLayer = this->getObjectGroup("objects");
	coinLayer = this->getLayer("coin");
	flagpoleLayer = this->getLayer("flagpole");

	this->initObjects();

	// ������Щ��������Ӧ���ڳ�ʼ��������֮���������
	// �ڳ�ʼ�������Ӱڷŵ�󣬽�����ڵ�ͼ��
	// ����Ҫ�жϸõ�ͼ�ǲ���BossMap
	if (isBossMap)
	{
		pFlag = Sprite::create("image/props/axe.png");
		pFlag->retain();
		pPrincess = Sprite::create("image/hero/princess.png");
		pPrincess->setAnchorPoint(Point(0.5f, 0.0f));
		pPrincess->setPosition(Point(finalPoint.x + 16, finalPoint.y));
		this->addChild(pPrincess, this->getChildrenCount());
	}
	else
	{
		pFlag = Sprite::create("image/props/flag.png");
		pFlag->retain();
	}

	pFlag->setAnchorPoint(Point(0.5f, 0));
	pFlag->setPosition(flagPoint);
	this->addChild(pFlag, this->getChildrenCount());

	this->launchEnemyInMap();

	this->launchGadgetInMap();

	this->scheduleUpdate();
}

void GameMap::showFlagMove()
{
	if (isBossMap)
	{
		// ��Boss�أ�˳��ͨ�ض���Ӧ�����ŵ�����ȥ��Boss����ȥ

		this->initBridgeArray();
	}
	else
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("QiZiLuoXia.ogg");
		MoveBy *pMoveBy = MoveBy::create(2.0f, Point(0, -8 * 16));
		pFlag->runAction(pMoveBy);
	}
}

void GameMap::initObjects()
{
	ValueVector tempArray = objectLayer->getObjects();

	float x, y, w, h;
	Value objPointMap;
	for (auto objPointMap : tempArray)
	{
		ValueMap objPoint = objPointMap.asValueMap();
		int posX = objPoint.at("x").asFloat();
		int posY = objPoint.at("y").asFloat();
		posY -= this->getTileSize().height;
		Point tileXY = this->positionToTileCoord(Point(posX, posY));
		std::string name = objPoint.at("name").asString();
		std::string type = objPoint.at("type").asString();

		// ���й���ĳ�ʼ�����ȸ����������ж��ǲ���enemy����ϸ��enemy����		
		if (name == "enemy")
		{
			Enemy *pEnemy = NULL;
			if (type == "mushroom")
			{
				pEnemy = new EnemyMushroom();
			}
			else if (type == "flower")
			{
				pEnemy = new EnemyFlower();
			}
			else if (type == "tortoise")
			{
				// ��ͼ��Ԥ�ȱ�Ǻõ��ڹ궼�������
				pEnemy = new EnemyTortoise(0);
			}
			else if (type == "tortoise_round")
			{
				std::string dis = objPoint.at("roundDis").asString();
				int roundDis = atof(dis.c_str());
				pEnemy = new EnemyTortoiseRound(roundDis);
			}
			else if (type == "tortoise_fly")
			{
				std::string dis = objPoint.at("flyDis").asString();
				int flyDis = atof(dis.c_str());
				pEnemy = new EnemyTortoiseFly(flyDis);
			}
			else if (type == "fire_string")
			{
				std::string str = objPoint.at("begAngle").asString();
				float begAngle = atof(str.c_str());
				str = objPoint.at("time").asString();
				float time = atof(str.c_str());
				pEnemy = new EnemyFireString(begAngle, time);
			}
			else if (type == "flyfish")
			{
				std::string str = objPoint.at("offsetH").asString();
				float offsetH = atof(str.c_str());
				str = objPoint.at("offsetV").asString();
				float offsetV = atof(str.c_str());
				str = objPoint.at("duration").asString();
				float duration = atof(str.c_str());

				pEnemy = new EnemyFlyFish(offsetH, offsetV, duration);
			}
			else if (type == "boss")
			{
				isBossMap = true;
				pEnemy = new EnemyBoss();
				pBossEnemy = pEnemy;
			}
			else if (type == "addmushroom")
			{
				std::string str = objPoint.at("nums").asString();
				int nums = atoi(str.c_str());
				pEnemy = new EnemyAddMushroom(nums);
			}
			else if (type == "battery")
			{
				float delay = objPoint.at("delay").asFloat();
				pEnemy = new EnemyBattery(delay);
			}
			else if (type == "darkcloud")
			{
				float delay = objPoint.at("delay").asFloat();
				int style = objPoint.at("style").asFloat();
				pEnemy = new EnemyDarkCloud(delay, style);
			}

			if (pEnemy != NULL)
			{
				pEnemy->setTileCoord(tileXY);
				pEnemy->setEnemyPos(Point(posX, posY));
				pEnemyArray->addObject(pEnemy);
			}
		}
		else if (name == "gadget")
		{
			float dis = objPoint.at("ladderDis").asFloat();
			int val;
			Gadget *pGadget = NULL;
			if (type == "ladderLR")
			{
				// �����ƶ�������
				pGadget = new CCGadgetLadderLR(dis);
				val = objPoint.at("LorR").asInt();
				pGadget->setStartFace(val);
			}
			else if (type == "ladderUD")
			{
				// �����ƶ�������
				pGadget = new CCGadgetLadderUD(dis);
				val = objPoint.at("UorD").asInt();
				pGadget->setStartFace(val);
			}

			if (pGadget != NULL)
			{
				pGadget->setStartPos(Point(posX, posY));
				pGadgetArray->addObject(pGadget);
			}
		}
		else if (name == "mushroom")
		{
			if (type == "MushroomReward")
			{
				// ���˱������ӵ�Ģ����
				pMushroomPointArray->addControlPoint(tileXY);
			}
			else if (type == "MushroomAddLife")
			{
				// ��ͼ�н��е�һ������Ģ����
				addLifePoint = tileXY;
			}
		}
		else if (name == "others")
		{
			if (type == "BirthPoint")
			{
				// ������ڵ�ǰ��ͼ�ϵĳ�����
				marioBirthPos = this->tilecoordToPosition(tileXY);
				marioBirthPos.x += this->getTileSize().width / 2;
			}
			else if (type == "flagpoint")
			{
				flagPoint = Point(posX, posY);
			}
			else if (type == "finalpoint")
			{
				finalPoint = Point(posX, posY);
			}
			else if (type == "bridgestartpos")
			{
				bridgeTileStartPos = tileXY;
			}
		}
	}

}

void GameMap::launchEnemyInMap()
{
	Enemy *tempEnemy = NULL;
	unsigned int enemyCount = pEnemyArray->count();
	for (unsigned int idx = 0; idx < enemyCount; ++idx)
	{
		tempEnemy = (Enemy *)pEnemyArray->objectAtIndex(idx);
		tempEnemy->setPosition(tempEnemy->getEnemyPos());
		switch (tempEnemy->getEnemyType())
		{
		case eEnemy_flower:
		case eEnemy_AddMushroom:
			this->addChild(tempEnemy, 3);
			break;
		default:
			this->addChild(tempEnemy, 7);
			break;
		}
		tempEnemy->launchEnemy();
	}
}

void GameMap::launchGadgetInMap()
{
	Gadget *tempGadget = NULL;
	unsigned int gadgetCount = pGadgetArray->count();
	for (unsigned int idx = 0; idx < gadgetCount; ++idx)
	{
		tempGadget = (Gadget *)pGadgetArray->objectAtIndex(idx);
		tempGadget->setPosition(tempGadget->getStartPos());
		this->addChild(tempGadget, 3);
		tempGadget->launchGadget();
	}
}

void GameMap::enemyVSHero()
{
	Enemy *tempEnemy = NULL;
	EnemyVSHero vsRet;
	unsigned int enemyCount = pEnemyArray->count();
	for (unsigned int idx = 0; idx < enemyCount; ++idx)
	{
		tempEnemy = (Enemy *)pEnemyArray->objectAtIndex(idx);
		if (tempEnemy->getEnemyState() == eEnemyState_active)
		{
			vsRet = tempEnemy->checkCollisionWithHero();
			switch (vsRet)
			{
			case eVS_heroKilled:
			{
				if (!Hero::getInstance()->getIsSafeTime())
				{
					Hero::getInstance()->changeForGotEnemy();
				}
				break;
			}
			case eVS_enemyKilled:
			{
				tempEnemy->forKilledByHero();
				CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("CaiSiGuaiWu.ogg");
				break;
			}
			default:
				break;
			}
		}
	}
}

void GameMap::update(float dt)
{
	this->enemyVSHero();
	this->bulletVSEnemy();
}

void GameMap::stopUpdateForHeroDie()
{
	// ֹͣ����ĸ���
	unsigned int enemyCount = pEnemyArray->count();
	Enemy *pEnemy = NULL;
	for (unsigned int idx = 0; idx < enemyCount; ++idx)
	{
		pEnemy = (Enemy *)pEnemyArray->objectAtIndex(idx);

		if (pEnemy->getEnemyState() == eEnemyState_active)
		{
			pEnemy->stopEnemyUpdate();
			switch (pEnemy->getEnemyType())
			{
			case eEnemy_mushroom:
			case eEnemy_tortoise:
				break;
			case eEnemy_flower:
			case eEnemy_flyFish:
			case eEnemy_tortoiseRound:
			case eEnemy_tortoiseFly:
			case eEnemy_fireString:
				//case eEnemy_Boss:
				pEnemy->stopAllActions();
				break;
			}

			pEnemy->unscheduleUpdate();
		}
	}

	// ֹͣ�ӵ��ĸ���
	unsigned int bulletCount = pBulletArray->count();
	Bullet *pBullet = NULL;
	for (unsigned int idx = 0; idx < bulletCount; ++idx)
	{
		pBullet = (Bullet *)pBulletArray->objectAtIndex(idx);
		if (pBullet->getBulletState() == eBulletState_active)
		{
			pBullet->unscheduleUpdate();
		}
	}

	// ֹͣС����ĸ���
	unsigned int gadgetCount = pGadgetArray->count();
	Gadget *pGadget = NULL;
	for (unsigned int idx = 0; idx < gadgetCount; ++idx)
	{
		pGadget = (Gadget *)pGadgetArray->objectAtIndex(idx);
		pGadget->stopAllActions();
		pGadget->unscheduleUpdate();
	}

	this->unscheduleUpdate();
}

TMXLayer* GameMap::getCloudLayer() const
{
	return cloudLayer;
}

TMXLayer* GameMap::getLandLayer()const
{
	return landLayer;
}

TMXLayer* GameMap::getPipeLayer()const
{
	return pipeLayer;
}

TMXLayer* GameMap::getBlockLayer()const
{
	return blockLayer;
}

TMXLayer* GameMap::getTrapLayer()const
{
	return trapLayer;
}

CCTMXObjectGroup* GameMap::getObjectLayer()const
{
	return objectLayer;
}

TMXLayer* GameMap::getCoinLayer()const
{
	return coinLayer;
}

TMXLayer* GameMap::getFlagpoleLayer()const
{
	return flagpoleLayer;
}

Point GameMap::getMarioBirthPos()
{
	return marioBirthPos;
}

// ����һ��Tile����ϵ�µĵ㣬�����õ��Ӧ��ͼ�е�ͼ��Ԫ������
TileType GameMap::tileTypeforPos(Point tileCoord)
{
	int GID = pipeLayer->tileGIDAt(tileCoord);
	if (GID > 0)
	{
		return eTile_Pipe;
	}
	GID = blockLayer->tileGIDAt(tileCoord);
	if (GID > 0)
	{
		return eTile_Block;
	}
	GID = landLayer->tileGIDAt(tileCoord);
	if (GID > 0)
	{
		return eTile_Land;
	}
	GID = trapLayer->tileGIDAt(tileCoord);
	if (GID > 0)
	{
		return eTile_Trap;
	}
	GID = coinLayer->tileGIDAt(tileCoord);
	if (GID > 0)
	{
		return eTile_Coin;
	}
	GID = flagpoleLayer->tileGIDAt(tileCoord);
	if (GID > 0)
	{
		return eTile_Flagpole;
	}
	return eTile_NoneH;
}

// ��block���� ��Щblockש���ǿ��б�����ģ�
// ��Щש������˸���ʺţ�ͷ��һ�»ᵯ��һ�������
// �ڵ�ͼ��ͼ�������У���blockType����ʾ
// 1: ��˸���ʺ�
// 2: ��ͨ��һ�������ש��
void GameMap::breakBlock(Point tileCoord, BodyType _bodyType)
{
	int gID = blockLayer->tileGIDAt(tileCoord);

	Value pD;
	pD = this->propertiesForGID(gID);

	if (pD.isNull() == false)
	{
		int value = pD.asValueMap().at("blockType").asInt();
		if (value)
		{
			int blockType = value;
			if (blockType == 2)
			{
				switch (_bodyType)
				{
				case eBody_Normal:
				{
					// ��������:ש�鱻������Ŀ飬Ȼ����ʧ��
					this->showBlockBroken(tileCoord);
					blockLayer->removeTileAt(tileCoord);
				}
					break;
				case eBody_Small:
				{
					// ������С������£�ש��ֻ�������ƶ�һ��Ȼ���ٻָ�λ��
					this->showBlockJump(tileCoord);
					CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("DingYingZhuanKuai.ogg");
				}
					break;
				default:
					break;
				}
			}
			else if (blockType == 1)  // �������ש��
			{
				if (itemCoordArrayContains(tileCoord) == false)
				{
					// ���統ǰ�����������ʺ�û�б�����							
					pItemCoordArray->addControlPoint(tileCoord);

					// ��֮��Ҫ�ѽ���Ģ���������˸���ʺ�ש����
					// Ӧ�÷�����һ�����εĵط�����Ȼ��blockͼ�㣬�����õ�ȷʵ��ձ�����ͼ��
					//if (tileCoord.x == addLifePoint.x && tileCoord.y == addLifePoint.y)
					//{
					//	// �����������Ģ�����Ǹ���ͼ����
					//	this->showAddLifeMushroom(tileCoord);
					//}
					if (mushroomPointContains(tileCoord))
					{
						// �����Ģ����ô��ҾͲ�չʾ��Ծ����
						resetCoinPoint = tileCoord;
						this->resetCoinBlockTexture();
						this->showNewMushroom(tileCoord, _bodyType);
						this->deleteOneMushPointFromArray(tileCoord);
					}
					else
					{
						// ��ש��������������Ҳ�����
						if (CCRANDOM_0_1() > 0.4f)
						{
							CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("EatCoin.ogg");
							this->showJumpUpBlinkCoin(tileCoord);
							// �������ͨ�Ľ�ң���ôչʾһ����Ծ����
							this->showCoinJump(tileCoord);
						}
						else
						{
							this->showCoinJump(tileCoord);
							// ͣ��0.2��
							enemyTilePos = tileCoord;
							this->runAction(Sequence::create(DelayTime::create(0.2f),
								CallFunc::create(this, callfunc_selector(GameMap::randomShowEnemy)), NULL));
						}

					}
				}
				else
				{
					CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("DingYingZhuanKuai.ogg");
				}
			}
			else if (blockType == 3)
			{
				if (itemCoordArrayContains(tileCoord) == false)
				{
					pItemCoordArray->addControlPoint(tileCoord);
					// ���ص�ש�飬һ�������н���Ģ��
					Sprite *pSprite = blockLayer->tileAt(tileCoord);
					pSprite->setDisplayFrame(_brokenCoin);
					this->showAddLifeMushroom(tileCoord);
				}
			}
		}
	}
	//else
	//{
	//	// Ҫ�����еı������ӵ�Ģ������˸��ש����Ļ�������жϴ���Ҳ����ִ����
	//	//if (mushroomPointContains(tileCoord))
	//	//{
	//	//	this->showNewMushroom(tileCoord, _bodyType);
	//	//	this->deleteOneMushPointFromArray(tileCoord);
	//	//}
	//	// ��������������˵�Ļ�����ô���еĽ���Ģ�������ж϶���ִ�������
	//	if (tileCoord.x == addLifePoint.x && tileCoord.y == addLifePoint.y)
	//	{
	//		// �����������Ģ�����Ǹ���ͼ����
	//		this->showAddLifeMushroom(tileCoord);
	//	}
	//}
}

void GameMap::randomShowEnemy()
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("DingChuMoGuHua.wma");

	// �����Ĺ�����Ҳ��һ�����
	if (CCRANDOM_0_1() > 0.5f)
	{
		pRandomEnemy = new EnemyMushroom();
		if (CCRANDOM_0_1() > 0.5f)
		{
			// ���ó�ʼ��������
			pRandomEnemy->setMoveOffset(pRandomEnemy->getccMoveOffset());
		}
	}
	else
	{
		int val = 1;
		if (CCRANDOM_0_1() < 0.5f)
		{
			val = 1;
		}
		pRandomEnemy = new EnemyTortoise(val);
	}

	Point pos = this->tilecoordToPosition(enemyTilePos);
	pos.x += tileSize.width / 2;

	pRandomEnemy->setPosition(pos);
	this->addChild(pRandomEnemy, blockLayer->getZOrder() - 1);

	pRandomEnemy->runAction(Sequence::create(JumpBy::create(0.2f, Point(0, 16), 1, 20),
		CallFunc::create(this, callfunc_selector(GameMap::randomLaunchEnemy)), NULL));


}

void GameMap::randomLaunchEnemy()
{
	pEnemyArray->addObject(pRandomEnemy);
	//pRandomEnemy->setEnemyPos(pos);
	pRandomEnemy->setZOrder(7);
	pRandomEnemy->launchEnemy();
}

bool GameMap::itemCoordArrayContains(Point tileCoord)
{
	Point temp;
	bool flag = false;
	for (unsigned int idx = 0; idx < pItemCoordArray->count(); ++idx)
	{
		temp = pItemCoordArray->getControlPointAtIndex(idx);
		if (temp.x == tileCoord.x && temp.y == tileCoord.y)
		{
			flag = true;
			break;
		}
	}
	return flag;
}

bool GameMap::mushroomPointContains(Point tileCoord)
{
	Point temp;
	bool flag = false;
	for (unsigned int idx = 0; idx < pMushroomPointArray->count(); ++idx)
	{
		temp = pMushroomPointArray->getControlPointAtIndex(idx);
		if (temp.x == tileCoord.x && temp.y == tileCoord.y)
		{
			flag = true;
			break;
		}
	}
	return flag;
}

void GameMap::deleteOneMushPointFromArray(Point tileCoord)
{
	Point temp;
	for (unsigned int idx = 0; idx < pMushroomPointArray->count(); ++idx)
	{
		temp = pMushroomPointArray->getControlPointAtIndex(idx);
		if (temp.x == tileCoord.x && temp.y == tileCoord.y)
		{
			pMushroomPointArray->removeControlPointAtIndex(idx);
			break;
		}
	}
}

void GameMap::showBlockBroken(Point tileCoord)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("DingPoZhuan.ogg");

	CCTexture2D *pTexture = TextureCache::getInstance()->addImage("singleblock.png");
	SpriteFrame *frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(0, 0, 8, 8));
	Point pos = this->tilecoordToPosition(tileCoord);
	pos.x += this->getTileSize().width / 2;
	pos.y += this->getTileSize().height / 2;

	Sprite *pSprite1 = Sprite::createWithSpriteFrame(frame);
	pSprite1->setPosition(pos);
	pSpriteArray->addObject(pSprite1);
	Sprite *pSprite2 = Sprite::createWithSpriteFrame(frame);
	pSprite2->setPosition(pos);
	pSpriteArray->addObject(pSprite2);
	Sprite *pSprite3 = Sprite::createWithSpriteFrame(frame);
	pSprite3->setPosition(pos);
	pSpriteArray->addObject(pSprite3);
	Sprite *pSprite4 = Sprite::createWithSpriteFrame(frame);
	pSprite4->setPosition(pos);
	pSpriteArray->addObject(pSprite4);

	this->addChild(pSprite1);
	this->addChild(pSprite2);
	this->addChild(pSprite3);
	this->addChild(pSprite4);

	ActionInterval *pLeftUp = JumpBy::create(0.2f,
		Point(-this->getTileSize().width * 2, this->getTileSize().height), 10, 1);
	ActionInterval *pRightUp = JumpBy::create(0.2f,
		Point(this->getTileSize().width * 2, this->getTileSize().height), 10, 1);
	ActionInterval *pLeftDown = JumpBy::create(0.2f,
		Point(-this->getTileSize().width * 3, -this->getTileSize().height), 5, 1);
	ActionInterval *pRightDown = JumpBy::create(0.2f,
		Point(this->getTileSize().width * 3, -this->getTileSize().height), 5, 1);

	pSprite1->runAction(pLeftUp);
	pSprite2->runAction(pRightUp);
	pSprite3->runAction(pLeftDown);
	pSprite4->runAction(Sequence::create(pRightDown,
		CallFunc::create(this, callfunc_selector(GameMap::clearSpriteArray)), NULL));
}

void GameMap::showJumpUpBlinkCoin(Point tileCoord)
{
	pItem = CCItem::create(eBlinkCoin);
	Point pos = this->tilecoordToPosition(tileCoord);
	pos.x += this->getTileSize().width / 2;
	pos.y += this->getTileSize().height;
	pItem->setPosition(pos);
	pItem->setVisible(true);
	this->addChild(pItem);

	ActionInterval *pJump = JumpBy::create(0.16f, Point(0, this->getTileSize().height),
		this->getTileSize().height*1.5, 1);

	pItem->itemBody->runAction(AnimationManager::getInstance()->createAnimate(eAniBlinkCoin));
	pItem->runAction(Sequence::create(pJump, CallFunc::create(
		this, callfunc_selector(GameMap::clearItem)), NULL));
}

void GameMap::showBlockJump(Point tileCorrd)
{
	Sprite *tempSprite = blockLayer->tileAt(tileCorrd);
	ActionInterval *pJumpBy = JumpBy::create(0.2f, Point(0, 0)/*CCPointZero*/,
		this->getTileSize().height*0.5, 1);
	tempSprite->runAction(pJumpBy);
}

void GameMap::showCoinJump(Point tileCoord)
{
	Sprite *tempSprite = blockLayer->tileAt(tileCoord);
	ActionInterval *pJumpBy = JumpBy::create(0.2f, Point(0, 0)/*CCPointZero*/,
		this->getTileSize().height*0.5, 1);

	// ͨ�����еĳ�Ա���������ݲ��������Լ򻯺������йش��εĴ���
	resetCoinPoint = tileCoord;

	tempSprite->runAction(Sequence::create(pJumpBy,
		CallFunc::create(this, callfunc_selector(GameMap::resetCoinBlockTexture)), NULL));
}

void GameMap::resetCoinBlockTexture()
{
	Sprite *coinTile = blockLayer->tileAt(resetCoinPoint);
	coinTile->setDisplayFrame(_brokenCoin);
}



void GameMap::showNewMushroom(Point tileCoord, BodyType _bodyType)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("DingChuMoGuHuoHua.ogg");
	// ���õ�ǰĢ��������
	mushTileCoord = Point(tileCoord.x, tileCoord.y - 1);

	Point pos = this->tilecoordToPosition(tileCoord);
	pos.x += this->getTileSize().width / 2;
	pos.y += this->getTileSize().height / 2;

	switch (_bodyType)
	{
	case eBody_Small:
	{
		pMushSprite = Sprite::create("image/props/rewardMushroomSet.png", CCRectMake(0, 0, 16, 16));
	}
		break;
	case eBody_Normal:
	{
		pMushSprite = Sprite::create("image/props/Tools.png", CCRectMake(0, 0, 18, 18));
	}
		break;
	default:
		break;
	}

	pMushSprite->setPosition(pos);
	this->addChild(pMushSprite);

	MoveBy *pMushJump = MoveBy::create(0.4f, Point(0, this->getTileSize().height));
	pMushSprite->runAction(pMushJump);
}

void GameMap::showAddLifeMushroom(Point tileCoord)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("DingChuMoGuHua.wma");

	// ��һ�ε����ý���Ģ��������
	// ��һ�ν�y��������һ����λ����Ϊ�˺�����ж��Ƿ�ٵ�����Ģ������
	addLifePoint = Point(tileCoord.x, tileCoord.y - 1);

	Point pos = this->tilecoordToPosition(tileCoord);
	pos.x += this->getTileSize().width / 2;
	pos.y += this->getTileSize().height / 2;

	pAddLifeMushroom = Sprite::create("image/props/rewardMushroomSet.png", CCRectMake(16, 0, 16, 16));

	pAddLifeMushroom->setPosition(pos);
	this->addChild(pAddLifeMushroom);

	MoveBy *pMushJump = MoveBy::create(0.4f, Point(0, this->getTileSize().height));
	pAddLifeMushroom->runAction(pMushJump);
}


void GameMap::clearItem()
{
	pItem->removeFromParentAndCleanup(true);
	pItem = NULL;
}

void GameMap::clearSpriteArray()
{
	Sprite *pS = NULL;
	for (unsigned int idx = 0; idx < pSpriteArray->count(); ++idx)
	{
		pS = (Sprite *)pSpriteArray->objectAtIndex(idx);
		pS->removeFromParentAndCleanup(true);
	}
}

bool GameMap::isMarioEatMushroom(Point tileCoord)
{
	if (pMushSprite == NULL)
	{
		return false;
	}
	if (tileCoord.x == mushTileCoord.x && tileCoord.y == mushTileCoord.y)
	{
		pMushSprite->removeFromParentAndCleanup(true);
		mushTileCoord = Point(0, 0)/*CCPointZero*/;
		pMushSprite = NULL;
		return true;
	}
	else
	{
		return false;
	}
}

bool GameMap::isMarioEatAddLifeMushroom(Point tileCoord)
{
	if (pAddLifeMushroom == NULL)
	{
		return false;
	}
	if (tileCoord.x == addLifePoint.x && tileCoord.y == addLifePoint.y)
	{
		pAddLifeMushroom->removeFromParentAndCleanup(true);
		mushTileCoord = Point(0, 0)/*CCPointZero*/;
		pAddLifeMushroom = NULL;
		return true;
	}
	else
	{
		return false;
	}
}

Point GameMap::positionToTileCoord(Point pos)
{
	int x = pos.x / this->getTileSize().width;
	int y = (this->getMapSize().height - 1) - pos.y / this->getTileSize().height;
	return Point(x, y);
}

Point GameMap::tilecoordToPosition(Point tileCoord)
{
	float x = tileCoord.x * this->getTileSize().width;
	float y = (this->getMapSize().height - 1 - tileCoord.y) * this->getTileSize().height;
	return Point(x, y);
}

void GameMap::createNewBullet()
{
	Bullet *pBullet = NULL;
	switch (Global::getInstance()->getCurrentBulletType())
	{
	case eBullet_common:
		pBullet = new BulletCommon();
		break;
	case eBullet_arrow:
		pBullet = new BulletArrow();
		break;
	default:
		break;
	}

	pBulletArray->addObject(pBullet);

	pBullet->setPosition(pBullet->getStartPos());
	this->addChild(pBullet, 7);
	pBullet->launchBullet();
}

void GameMap::createNewBulletForBoss(Point pos, EnemyType _enemyType)
{
	Enemy *pEnemy = NULL;

	switch (_enemyType)
	{
	case eEnemy_BossFire:
		pEnemy = new EnemyBossFire();
		break;
	case eEnemy_mushroom:
		pEnemy = new EnemyMushroom();
		break;
	case eEnemy_BatteryBullet:
		pEnemy = new EnemyBatteryBullet();
		break;
	case eEnemy_Lighting:
		pEnemy = new EnemyLighting();
		break;
	default:
		break;
	}

	if (pEnemy != NULL)
	{
		pEnemyArray->addObject(pEnemy);
		pEnemy->setEnemyPos(pos);
		pEnemy->setPosition(pos);
		this->addChild(pEnemy, 7);
		pEnemy->launchEnemy();
	}
}

void GameMap::bulletVSEnemy()
{
	unsigned int bulletCount = pBulletArray->count();
	unsigned int enemyCount = pEnemyArray->count();
	Bullet *pBullet = NULL;
	Enemy *pEnemy = NULL;
	Array *delBullet = Array::create();
	delBullet->retain();
	Array *delEnemy = NULL;
	Rect bulletRect;
	Rect enemyRect;

	for (unsigned int idxBullet = 0; idxBullet < bulletCount; ++idxBullet)
	{
		pBullet = (Bullet *)pBulletArray->objectAtIndex(idxBullet);
		if (pBullet->getBulletState() == eBulletState_nonactive)
		{
			delBullet->addObject(pBullet);
			continue;
		}
		bulletRect = pBullet->getBulletRect();

		for (unsigned int idxEnemy = 0; idxEnemy < enemyCount; ++idxEnemy)
		{
			pEnemy = (Enemy *)pEnemyArray->objectAtIndex(idxEnemy);
			switch (pEnemy->getEnemyType())
			{
				// ���ǲ��ܱ��κ��ӵ������
				// �궷�޵���̨������ӵ����ᱻ�κ��ӵ����
			case eEnemy_BatteryBullet:
			case eEnemy_fireString:
			case eEnemy_Lighting:
			case eEnemy_DarkCloud:
				continue;
				break;
			}
			if (pBullet->getBulletType() == eBullet_common &&
				pEnemy->getEnemyType() == eEnemy_Boss)
			{
				// ��ͨ���ӵ���Bossû������
				continue;
			}
			if (pBullet->getBulletType() == eBullet_common &&
				pEnemy->getEnemyType() == eEnemy_BossFire)
			{
				// ��ͨ�ӵ���Boss�����Ļ���Ҳ��û�����õ�
				continue;
			}

			if (pEnemy->getEnemyState() == eEnemyState_active)
			{
				enemyRect = pEnemy->getEnemyRect();

				if (bulletRect.intersectsRect(enemyRect))
				{
					pBullet->forKilledEnemy();
					CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("HuoQiuDaDaoGuaiWu.ogg");
					pEnemy->forKilledByBullet();
				}
			}
		}
	}

	unsigned int delCount = delBullet->count();
	for (unsigned int idxDel = 0; idxDel < delCount; ++idxDel)
	{
		pBullet = (Bullet *)delBullet->objectAtIndex(idxDel);
		pBulletArray->removeObject(pBullet, true);
		this->removeChild(pBullet, true);
	}
	delBullet->release();
}

bool GameMap::isHeroInGadget(Point heroPos, float &gadgetLevel)
{
	bool ret = false;
	Gadget *tempGadget = NULL;
	unsigned int gadgetCount = pGadgetArray->count();
	Rect gadgetRect;
	for (unsigned int idx = 0; idx < gadgetCount; ++idx)
	{
		tempGadget = (Gadget *)pGadgetArray->objectAtIndex(idx);
		if (tempGadget->getGadgetState() == eGadgetState_active)
		{
			gadgetRect = tempGadget->getGadgetRect();
			if (gadgetRect.containsPoint(heroPos))
			{
				gadgetLevel = tempGadget->getPosition().y + tempGadget->getGadgetSize().height;
				ret = true;
				this->heroInGadget = tempGadget;
				Hero::getInstance()->setGadgetable(true);
				break;
			}
		}
	}
	return ret;
}

void GameMap::initBridgeArray()
{
	bridgeTileNums = 13;

	Sprite *pS = NULL;
	Point bossPos = pBossEnemy->getPosition();
	Point pos;
	for (int i = 0; i < bridgeTileNums; ++i)
	{
		Point tilePos = bridgeTileStartPos;
		tilePos.x += i;

		pS = landLayer->tileAt(tilePos);
		pS->runAction(MoveBy::create(1.0f, Point(0, -60)));

		pos = this->tilecoordToPosition(tilePos);
		if (pos.x >= bossPos.x)
		{
			if (pBossEnemy->getEnemyState() == eEnemyState_active)
			{
				CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("BossDiaoLuoQiaoXia.ogg");
				pBossEnemy->runAction(MoveBy::create(1.0f, Point(0, -80)));
			}
		}
	}

	// ���ǰѸ�������Ϊ���ɼ�
	pFlag->setVisible(false);
}

void GameMap::pauseGameMap()
{
	// ֹͣ����ĸ���
	unsigned int enemyCount = pEnemyArray->count();
	Enemy *pEnemy = NULL;
	for (unsigned int idx = 0; idx < enemyCount; ++idx)
	{
		pEnemy = (Enemy *)pEnemyArray->objectAtIndex(idx);

		if (pEnemy->getEnemyState() == eEnemyState_active)
		{
			pEnemy->unscheduleUpdate();
		}
	}

	// ֹͣ�ӵ��ĸ���
	unsigned int bulletCount = pBulletArray->count();
	Bullet *pBullet = NULL;
	for (unsigned int idx = 0; idx < bulletCount; ++idx)
	{
		pBullet = (Bullet *)pBulletArray->objectAtIndex(idx);
		if (pBullet->getBulletState() == eBulletState_active)
		{
			pBullet->unscheduleUpdate();
		}
	}

	// ֹͣС����ĸ���
	unsigned int gadgetCount = pGadgetArray->count();
	Gadget *pGadget = NULL;
	for (unsigned int idx = 0; idx < gadgetCount; ++idx)
	{
		pGadget = (Gadget *)pGadgetArray->objectAtIndex(idx);
		pGadget->unscheduleUpdate();
	}

	this->unscheduleUpdate();
}

void GameMap::resumeGameMap()
{
	// ֹͣ����ĸ���
	unsigned int enemyCount = pEnemyArray->count();
	Enemy *pEnemy = NULL;
	for (unsigned int idx = 0; idx < enemyCount; ++idx)
	{
		pEnemy = (Enemy *)pEnemyArray->objectAtIndex(idx);

		if (pEnemy->getEnemyState() == eEnemyState_active)
		{
			pEnemy->scheduleUpdate();
		}
	}

	// ֹͣ�ӵ��ĸ���
	unsigned int bulletCount = pBulletArray->count();
	Bullet *pBullet = NULL;
	for (unsigned int idx = 0; idx < bulletCount; ++idx)
	{
		pBullet = (Bullet *)pBulletArray->objectAtIndex(idx);
		if (pBullet->getBulletState() == eBulletState_active)
		{
			pBullet->scheduleUpdate();
		}
	}

	// ֹͣС����ĸ���
	unsigned int gadgetCount = pGadgetArray->count();
	Gadget *pGadget = NULL;
	for (unsigned int idx = 0; idx < gadgetCount; ++idx)
	{
		pGadget = (Gadget *)pGadgetArray->objectAtIndex(idx);
		pGadget->scheduleUpdate();
	}

	this->scheduleUpdate();
}




/*************************************************
Copyright:bro7
Author:ben
Date:2014-07-08
Description:游戏动画预加载以及管理
**************************************************/


#include "AnimationManager.h"

USING_NS_CC;

AnimationManager *AnimationManager::_instance = nullptr;


AnimationManager::AnimationManager()
{
	allTypes.push_back(eAniLeft);
	allTypes.push_back(eAniLeft);
	allTypes.push_back(eAniRight);
	allTypes.push_back(eAniLeftSmall);
	allTypes.push_back(eAniRightSmall);
	allTypes.push_back(eAniLeftFire);
	allTypes.push_back(eAniRightFire);
	allTypes.push_back(eAniFireActionR);
	allTypes.push_back(eAniArrowActionR);
	allTypes.push_back(eAniFireActionL);
	allTypes.push_back(eAniArrowActionL);
	allTypes.push_back(eAniSmallDie);
	allTypes.push_back(eAniNormalDie);
	allTypes.push_back(eAniFireDie);
	allTypes.push_back(eAniArrowLeft);
	allTypes.push_back(eAniArrowRight);
	allTypes.push_back(eAniArrowDie);
	allTypes.push_back(eAniBlinkCoin);
	allTypes.push_back(eAniMushroom);
	allTypes.push_back(eAniflower);
	allTypes.push_back(eAniTortoiseLeft);
	allTypes.push_back(eAniTortoiseRight);
	allTypes.push_back(eAniTortoiseFly);
	allTypes.push_back(eAniRotatedFireBall);
	allTypes.push_back(eAniBoomedFireBall);
	allTypes.push_back(eAniFlyFishR);
	allTypes.push_back(eAniFlyFishL);
	allTypes.push_back(eAniBossMoveLeft);
	allTypes.push_back(eAniBossMoveRight);
	allTypes.push_back(eAniBossFireLeft);
	allTypes.push_back(eAniBossFireRight);
	/*allTypes = { 
		eAniLeft,
		eAniRight,
		eAniLeftSmall,
		eAniRightSmall,
		eAniLeftFire,
		eAniRightFire,
		eAniFireActionR,
		eAniArrowActionR,
		eAniFireActionL,
		eAniArrowActionL,
		eAniSmallDie,
		eAniNormalDie,
		eAniFireDie,
		eAniArrowLeft,
		eAniArrowRight,
		eAniArrowDie,
		eAniBlinkCoin,
		eAniMushroom,
		eAniflower,
		eAniTortoiseLeft,
		eAniTortoiseRight,
		eAniTortoiseFly,
		eAniRotatedFireBall,
		eAniBoomedFireBall,
		eAniFlyFishR,
		eAniFlyFishL,
		eAniBossMoveLeft,
		eAniBossMoveRight,
		eAniBossFireLeft,
		eAniBossFireRight };*/
}

AnimationManager::~AnimationManager()
{

}

AnimationManager * AnimationManager::getInstance()
{
	if ( _instance == nullptr )
	{
		_instance = new AnimationManager();
	}
	return _instance;
}

void AnimationManager::addAllAnimationCache()
{
	addAnimationCache(allTypes);
}

void AnimationManager::addAnimationCache(std::vector<AnimationType> vecAniTypes)
{
	for ( auto aniType : vecAniTypes)
	{
		std::string animationName = StringUtils::format("%d", aniType);
		AnimationCache::getInstance()->addAnimation(createAnimation(aniType), animationName);
	}
}

cocos2d::Animation *  AnimationManager::createAnimation(AnimationType type)
{
	Vector<SpriteFrame*> vecFrames;
	SpriteFrame * frame = nullptr;
	Texture2D *pTexture = nullptr;
	Animation *pAnimation = nullptr;
	switch (type)
	{
	case eAniLeft:
		pTexture = TextureCache::getInstance()->addImage("image/hero/walkLeft.png");
		for (size_t i = 0; i < 10; i++)
		{
			frame = SpriteFrame::createWithTexture(pTexture, Rect(i * 18, 0, 18, 32));
			vecFrames.pushBack(frame);
		}
		pAnimation = Animation::createWithSpriteFrames(vecFrames,0.02f);

		break;
	case eAniRight:
		pTexture = TextureCache::getInstance()->addImage("image/hero/walkRight.png");
		for (size_t i = 0; i < 10; i++)
		{
			frame = SpriteFrame::createWithTexture(pTexture, Rect(i * 18, 0, 18, 32));
			vecFrames.pushBack(frame);
		}
		pAnimation = Animation::createWithSpriteFrames(vecFrames, 0.02f);
		break;
	case eAniLeftSmall:
	{
		pTexture = TextureCache::getInstance()->addImage("image/hero/smallWalkLeft.png");
		for (int i = 9; i >= 0; --i)
		{
			frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(14 * i, 0, 14, 16));
			vecFrames.pushBack(frame);
		}
		pAnimation = Animation::createWithSpriteFrames(vecFrames, 0.02f);
		break;
	}
	case eAniRightSmall:
	{
		pTexture = TextureCache::getInstance()->addImage("image/hero/smallWalkRight.png");
		for (int i = 0; i < 10; ++i)
		{
			frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(14 * i, 0, 14, 16));
			vecFrames.pushBack(frame);
		}
		pAnimation = Animation::createWithSpriteFrames(vecFrames, 0.02f);
		break;
	}
	case eAniLeftFire:
	{
		pTexture = TextureCache::getInstance()->addImage("image/hero/WalkLeft_fire.png");
		for (int i = 9; i >= 0; --i)
		{
			frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(18 * i, 0, 18, 32));
			vecFrames.pushBack(frame);
		}
		pAnimation = Animation::createWithSpriteFrames(vecFrames, 0.02f);
		break;
	}
	case eAniRightFire:
	{
		pTexture = TextureCache::getInstance()->addImage("image/hero/WalkRight_fire.png");
		for (int i = 0; i < 10; ++i)
		{
			frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(18 * i, 0, 18, 32));
			vecFrames.pushBack(frame);
		}
		pAnimation = Animation::createWithSpriteFrames(vecFrames, 0.02f);
		break;
	}
	case eAniFireActionR:
	{
		pTexture = TextureCache::getInstance()->addImage("image/hero/fireActionR.png");
		for (int i = 0; i < 6; ++i)
		{
			frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(25 * i, 0, 25, 32));
			vecFrames.pushBack(frame);
		}
		pAnimation = Animation::createWithSpriteFrames(vecFrames, 0.02f);
		break;
	}
	case eAniArrowActionR:
	{
		pTexture = TextureCache::getInstance()->addImage("image/hero/arrowActionR.png");
		for (int i = 0; i < 6; ++i)
		{
			frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(25 * i, 0, 25, 32));
			vecFrames.pushBack(frame);
		}
		pAnimation = Animation::createWithSpriteFrames(vecFrames, 0.02f);
		break;
	}
	case eAniFireActionL:
	{
		pTexture = TextureCache::getInstance()->addImage("image/hero/fireActionL.png");
		for (int i = 5; i >= 0; --i)
		{
			frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(25 * i, 0, 25, 32));
			vecFrames.pushBack(frame);
		}
		pAnimation = Animation::createWithSpriteFrames(vecFrames, 0.02f);
		break;
	}
	case eAniArrowActionL:
	{
		pTexture = TextureCache::getInstance()->addImage("image/hero/arrowActionL.png");
		for (int i = 5; i >= 0; --i)
		{
			frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(25 * i, 0, 25, 32));
			vecFrames.pushBack(frame);
		}
		pAnimation = Animation::createWithSpriteFrames(vecFrames, 0.02f);
		break;
	}
	case eAniSmallDie:
	{
		pTexture = TextureCache::getInstance()->addImage("image/hero/small_die.png");
		for (int i = 0; i < 7; ++i)
		{
			frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(16 * i, 0, 16, 18));
			vecFrames.pushBack(frame);
			vecFrames.pushBack(frame);
		}

		pAnimation = Animation::createWithSpriteFrames(vecFrames, 0.1f);
		break;
	}
	case eAniNormalDie:
	{
		pTexture = TextureCache::getInstance()->addImage("image/hero/normal_die.png");
		for (int i = 0; i < 7; ++i)
		{
			frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(24 * i, 0, 24, 34));
			vecFrames.pushBack(frame);
			vecFrames.pushBack(frame);
		}

		pAnimation = Animation::createWithSpriteFrames(vecFrames, 0.1f);
		break;
	}
	case eAniFireDie:
	{
		pTexture = TextureCache::getInstance()->addImage("image/hero/fire_die.png");
		for (int i = 0; i < 7; ++i)
		{
			frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(24 * i, 0, 24, 34));
			vecFrames.pushBack(frame);
			vecFrames.pushBack(frame);
		}

		pAnimation = Animation::createWithSpriteFrames(vecFrames, 0.1f);
		break;
	}
	case eAniArrowLeft:
	{
		pTexture = TextureCache::getInstance()->addImage("image/hero/allow_walkLeft.png");
		for (int i = 9; i >= 0; --i)
		{
			frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(18 * i, 0, 18, 32));
			vecFrames.pushBack(frame);
		}
		pAnimation = Animation::createWithSpriteFrames(vecFrames, 0.02f);
		break;
	}
	case eAniArrowRight:
	{
		pTexture = TextureCache::getInstance()->addImage("image/hero/allow_walkRight.png");
		for (int i = 0; i < 10; ++i)
		{
			frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(18 * i, 0, 18, 32));
			vecFrames.pushBack(frame);
		}
		pAnimation = Animation::createWithSpriteFrames(vecFrames, 0.02f);
		break;
	}
	case eAniArrowDie:
	{
		pTexture = TextureCache::getInstance()->addImage("image/hero/arrow_die.png");
		for (int i = 0; i < 7; ++i)
		{
			frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(24 * i, 0, 24, 34));
			vecFrames.pushBack(frame);
			vecFrames.pushBack(frame);
		}
		pAnimation = Animation::createWithSpriteFrames(vecFrames, 0.1f);
		break;
	}
	case eAniBlinkCoin:
	{
		pTexture = TextureCache::getInstance()->addImage("image/props/coinani.png");
		for (int i = 0; i < 4; ++i)
		{
			frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(8 * i, 0, 8, 14));
			vecFrames.pushBack(frame);
			vecFrames.pushBack(frame);
		}
		pAnimation = Animation::createWithSpriteFrames(vecFrames, 0.02f);
		break;
	}
	case eAniMushroom:
	{
		pTexture = TextureCache::getInstance()->addImage("image/enemy/Mushroom0.png");
		for (int i = 0; i < 2; ++i)
		{
			frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(16 * i, 0, 16, 16));
			vecFrames.pushBack(frame);
		}
		pAnimation = Animation::createWithSpriteFrames(vecFrames, 0.3f);
		break;
	}
	case eAniflower:
	{
		pTexture = TextureCache::getInstance()->addImage("image/enemy/flower0.png");
		for (int i = 0; i < 2; ++i)
		{
			frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(16 * i, 0, 16, 24));
			vecFrames.pushBack(frame);
		}
		pAnimation = Animation::createWithSpriteFrames(vecFrames, 0.3f);
		break;
	}
	case eAniTortoiseLeft:
	{
		pTexture = TextureCache::getInstance()->addImage("image/enemy/tortoise0.png");
		for (int i = 2; i < 4; ++i)
		{
			frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(18 * i, 0, 18, 24));
			vecFrames.pushBack(frame);
		}
		pAnimation = Animation::createWithSpriteFrames(vecFrames, 0.3f);
		break;
	}
	case eAniTortoiseRight:
	{
		pTexture = TextureCache::getInstance()->addImage("image/enemy/tortoise0.png");
		for (int i = 4; i < 6; ++i)
		{
			frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(18 * i, 0, 18, 24));
			vecFrames.pushBack(frame);
		}
		pAnimation = Animation::createWithSpriteFrames(vecFrames, 0.3f);
		break;
	}
	case eAniTortoiseFly:
	{
		pTexture = TextureCache::getInstance()->addImage("image/enemy/tortoise0.png");
		for (int i = 0; i < 2; ++i)
		{
			frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(18 * i, 0, 18, 24));
			vecFrames.pushBack(frame);
		}
		pAnimation = Animation::createWithSpriteFrames(vecFrames, 0.3f);
		break;
	}
	case eAniRotatedFireBall:
	{
		pTexture = TextureCache::getInstance()->addImage("image/props/fireRight.png");
		for (int i = 0; i < 8; ++i)
		{
			frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(10 * i, 3, 10, 10));
			vecFrames.pushBack(frame);
		}
		pAnimation = Animation::createWithSpriteFrames(vecFrames, 0.01f);
		break;
	}
	case eAniBoomedFireBall:
	{
		pTexture = TextureCache::getInstance()->addImage("image/props/fireRight.png");

		frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(80, 3, 10, 10));
		vecFrames.pushBack(frame);
		frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(90, 0, 14, 16));
		vecFrames.pushBack(frame);
		frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(105, 0, 16, 16));
		vecFrames.pushBack(frame);

		pAnimation = Animation::createWithSpriteFrames(vecFrames, 0.05f);
		break;
	}

	case eAniFlyFishR:
	{
		pTexture = TextureCache::getInstance()->addImage("image/enemy/flyFishRight.png");
		frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(16 * 4, 0, 16, 16));
		vecFrames.pushBack(frame);
		frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(16 * 5, 0, 16, 16));
		vecFrames.pushBack(frame);

		pAnimation = Animation::createWithSpriteFrames(vecFrames, 0.05f);
		break;
	}
	case eAniFlyFishL:
	{
		pTexture = TextureCache::getInstance()->addImage("image/enemy/flyFishLeft.png");
		frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(16 * 4, 0, 16, 16));
		vecFrames.pushBack(frame);
		frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(16 * 5, 0, 16, 16));
		vecFrames.pushBack(frame);

		pAnimation = Animation::createWithSpriteFrames(vecFrames, 0.05f);
		break;
	}

	case eAniBossMoveLeft:
	{
		pTexture = TextureCache::getInstance()->addImage("image/enemy/boss.png");
		for (int i = 3; i >= 0; --i)
		{
			frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(32 * i, 0, 32, 32));
			vecFrames.pushBack(frame);
		}

		pAnimation = Animation::createWithSpriteFrames(vecFrames, 0.2f);
		break;
	}
	case eAniBossMoveRight:
	{
		pTexture = TextureCache::getInstance()->addImage("image/enemy/boss.png");
		for (int i = 4; i < 8; ++i)
		{
			frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(32 * i, 0, 32, 32));
			vecFrames.pushBack(frame);
		}

		pAnimation = Animation::createWithSpriteFrames(vecFrames, 0.2f);
		break;
	}
	case eAniBossFireLeft:
	{
		pTexture = TextureCache::getInstance()->addImage("image/enemy/bossBullet.png");
		for (int i = 0; i < 2; ++i)
		{
			frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(24 * i, 0, 24, 8));
			vecFrames.pushBack(frame);
		}

		pAnimation = Animation::createWithSpriteFrames(vecFrames, 0.1f);
		break;
	}
	case eAniBossFireRight:
	{
		pTexture = TextureCache::getInstance()->addImage("image/enemy/bossBullet.png");
		for (int i = 3; i >= 2; --i)
		{
			frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(24 * i, 0, 24, 8));
			vecFrames.pushBack(frame);
		}

		pAnimation = Animation::createWithSpriteFrames(vecFrames, 0.1f);
		break;
	}

	default:
		break;
	}

	return pAnimation;
}

cocos2d::Animation* AnimationManager::getAnimation(AnimationType key)
{
	std::string aniName = StringUtils::format("%d", key);
	return AnimationCache::getInstance()->getAnimation(aniName);
}

cocos2d::Animate *AnimationManager::createAnimate(AnimationType key)
{
	Animation *animation = getAnimation(key);
	if (animation)
	{
		return Animate::create(animation);
	}
	return nullptr; 
}
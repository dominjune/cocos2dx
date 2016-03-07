#ifndef __ANIMATION_MANAGER_H__
#define __ANIMATION_MANAGER_H__

#include "GameEnum.h"
#include <vector>
#include "cocos2d.h"


class AnimationManager
{
public:
	AnimationManager();
	~AnimationManager();

private:
	static AnimationManager *_instance;

public:
	static AnimationManager *getInstance();
	void addAllAnimationCache();
	cocos2d::Animate *createAnimate(AnimationType key);

private:
	cocos2d::Animation * createAnimation(AnimationType);
	void addAnimationCache( std::vector<AnimationType> vecAniTypes);
	std::vector < AnimationType > allTypes;

	cocos2d::Animation* getAnimation(AnimationType key);

};



#endif // !__ANIMATION_MANAGER_H__


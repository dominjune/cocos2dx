#ifndef __LOADING_SCENE_H__
#define __LOADING_SCENE_H__

#include "cocos2d.h"

class LoadingScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // implement the "static create()" method manually
    CREATE_FUNC(LoadingScene);
	virtual void onEnter() override;
	virtual void onExit() override;

private:
	float progressPercent ; //= 0.0f;
    float everyAdd; //= 5.0f;
	cocos2d::ProgressTimer *loadProgress; // = nullptr;
	cocos2d::LabelTTF* loadLabel;
	cocos2d::LabelTTF* percentLabel;
	void preloadResource();
	void progessAdd();
	void loadingCallback (cocos2d::Texture2D*);
};

#endif // __LoadingScene_SCENE_H__
/*************************************************
Copyright:bro7
Author:ben
Date:2014-07-08
Description:��Ϸloadingҳ��չʾ
**************************************************/


#include "LoadingScene.h"
#include "MainScene.h"
#include "AnimationManager.h"

USING_NS_CC;

Scene* LoadingScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = LoadingScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool LoadingScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
	progressPercent = 0.0f;
    everyAdd = 5.0f;
	loadProgress = nullptr;

    Size visibleSize = Director::getInstance()->getWinSize();
	auto spBg = Sprite::create("image/bg_load.jpg");
	this->addChild(spBg);
	spBg->setPosition(visibleSize.width / 2, visibleSize.height / 2);

	//������
	loadLabel = LabelTTF::create("Loading:", "Arial", 20);//������ʾLoading: ��label
	loadLabel->setPosition(Point(visibleSize.width / 2 - 30, visibleSize.height / 2 + 30));
	this->addChild(loadLabel, 1);

	percentLabel = LabelTTF::create("0%", "Arial", 20);//������ʾ�ٷֱȵ�label
	percentLabel->setPosition(Point(visibleSize.width / 2 + 35, visibleSize.height / 2 + 30));
	this->addChild(percentLabel, 2);

	loadProgress = ProgressTimer::create(Sprite::create("image/loadingbar.png"));
	loadProgress->setBarChangeRate(Point(1, 0));//���ý������ı仯����
	loadProgress->setType(ProgressTimer::Type::BAR);//���ý�����������
	loadProgress->setMidpoint(Point(0, 1));//���ý��ȵ��˶�����

	loadProgress->setPosition(visibleSize.width / 2, visibleSize.height / 2 );
	loadProgress->setPercentage(progressPercent);
	this->addChild(loadProgress);

    return true;
}


void LoadingScene::onEnter()
{
	Layer::onEnter();
	preloadResource();
}


void LoadingScene::onExit()
{
	Layer::onExit();
}


/*************************************************
Function:       // preloadResource
Description:    // Ԥ������������Դ�Լ����֡�
                ��ǰд����Ϸ������Ԥ������Դ�ӿڡ�
Calls:          // �����������õĺ����嵥
*************************************************/
void LoadingScene::preloadResource()
{
	//����ֿ�д���ԣ� �������ȷ����һ�������е�ֱ��ʹ��plist����
	std::string resouceMain = "image/mainscene/";
	float count = 20;//һ������ʮ���� 
	everyAdd = 100 / count;
	AnimationManager::getInstance()->addAllAnimationCache();
	Director::getInstance()->getTextureCache()->addImageAsync(resouceMain+"about_normal.png", CC_CALLBACK_1(LoadingScene::loadingCallback, this));
	Director::getInstance()->getTextureCache()->addImageAsync(resouceMain+"about_select.png", CC_CALLBACK_1(LoadingScene::loadingCallback, this));
	Director::getInstance()->getTextureCache()->addImageAsync(resouceMain+"backA.png", CC_CALLBACK_1(LoadingScene::loadingCallback, this));
	Director::getInstance()->getTextureCache()->addImageAsync(resouceMain+"backB.png", CC_CALLBACK_1(LoadingScene::loadingCallback, this));
	Director::getInstance()->getTextureCache()->addImageAsync(resouceMain+"background.png", CC_CALLBACK_1(LoadingScene::loadingCallback, this));
	Director::getInstance()->getTextureCache()->addImageAsync(resouceMain+"bg.png", CC_CALLBACK_1(LoadingScene::loadingCallback, this));
	Director::getInstance()->getTextureCache()->addImageAsync(resouceMain+"music_off.png", CC_CALLBACK_1(LoadingScene::loadingCallback, this));
	Director::getInstance()->getTextureCache()->addImageAsync(resouceMain+"music_on.png", CC_CALLBACK_1(LoadingScene::loadingCallback, this));
	Director::getInstance()->getTextureCache()->addImageAsync(resouceMain+"quitgame_normal.png", CC_CALLBACK_1(LoadingScene::loadingCallback, this));
	Director::getInstance()->getTextureCache()->addImageAsync(resouceMain+"quitgame_select.png", CC_CALLBACK_1(LoadingScene::loadingCallback, this));
	Director::getInstance()->getTextureCache()->addImageAsync(resouceMain+"Set_Music.png", CC_CALLBACK_1(LoadingScene::loadingCallback, this));
	Director::getInstance()->getTextureCache()->addImageAsync(resouceMain+"Setting_n.png", CC_CALLBACK_1(LoadingScene::loadingCallback, this));
	Director::getInstance()->getTextureCache()->addImageAsync(resouceMain+"setting_s.png", CC_CALLBACK_1(LoadingScene::loadingCallback, this));
	Director::getInstance()->getTextureCache()->addImageAsync(resouceMain+"startgame_normal.png", CC_CALLBACK_1(LoadingScene::loadingCallback, this));
	Director::getInstance()->getTextureCache()->addImageAsync(resouceMain+"startgame_select.png", CC_CALLBACK_1(LoadingScene::loadingCallback, this));
	Director::getInstance()->getTextureCache()->addImageAsync(resouceMain+"wxb.jpg", CC_CALLBACK_1(LoadingScene::loadingCallback, this));
	Director::getInstance()->getTextureCache()->addImageAsync(resouceMain+"zhy.jpg", CC_CALLBACK_1(LoadingScene::loadingCallback, this));
	Director::getInstance()->getTextureCache()->addImageAsync(resouceMain + "sound_effect_off.png", CC_CALLBACK_1(LoadingScene::loadingCallback, this));
	Director::getInstance()->getTextureCache()->addImageAsync(resouceMain + "sound_effect_on.png", CC_CALLBACK_1(LoadingScene::loadingCallback, this));
	Director::getInstance()->getTextureCache()->addImageAsync(resouceMain + "switchBg.png", CC_CALLBACK_1(LoadingScene::loadingCallback, this));

}
void LoadingScene::loadingCallback(Texture2D*)
{
	progessAdd();
}

void LoadingScene::progessAdd()
{
	progressPercent += everyAdd;

	if (100-progressPercent <everyAdd)
	{
		progressPercent = 100;
		auto scene = MainScene::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(1, scene));
	}
	percentLabel->setString(StringUtils::format("%d", int(progressPercent)));//����percentLabel��ֵ
	loadProgress->setPercentage(progressPercent);
}

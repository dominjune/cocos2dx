/*************************************************
Copyright:bro7
Author:ben
Date:2014-07-08
Description:游戏选择关卡页面展示
**************************************************/


#include "SelectLevel.h"
#include "Global.h"
#include "GameLayer.h"

Scene* SelectLevel::createScene()
{
	auto scene = Scene::create();
	auto layer = SelectLevel::create();
	scene->addChild(layer);
	return scene;
}


bool SelectLevel::init()
{
	if ( !Layer::init() )
	{
		return false;
	}

	spWidth = 400;
	spHeight = 223;

	Size size = Director::getInstance()->getVisibleSize();

	Sprite *pBg = Sprite::create("image/mainscene/bg.png");
	pBg->setPosition(size.width / 2, size.height / 2);
	this->addChild(pBg, 0);

	//关卡初始化
	auto layerr = Layer::create();

	layerr->setContentSize(Size(spWidth, spHeight));
	layerr->setPosition(size.width/2, size.height/2);
	
	layerr->setZOrder(111);
	this->addChild(layerr);
	auto listenTouch = EventListenerTouchOneByOne::create();
	listenTouch->onTouchBegan = CC_CALLBACK_2(SelectLevel::onTouchBegan, this);
	//listenTouch->onTouchMoved = CC_CALLBACK_2(SelectLevel::onTouchMoved, this);
	listenTouch->onTouchEnded = CC_CALLBACK_2(SelectLevel::onTouchEnded, this);
	//listenTouch->onTouchCancelled = CC_CALLBACK_2(SelectLevel::onTouchCancelled, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listenTouch, layerr);

	winSize = Director::getInstance()->getWinSize();
 /*************************************Tabel view********************************************************
	tableView = TableView::create(this, Size(spWidth, spHeight));
	tableView->setDirection(ScrollView::Direction::HORIZONTAL);
	tableView->setPosition(Point((winSize.width - spWidth)/2, (winSize.height-spHeight)/2));
	tableView->setDelegate(this);
	this->addChild(tableView);
	tableView->reloadData();
	**************************************************************************************************/

    pageView = PageView::create();
	pageView->setSize(Size(winSize.width, winSize.height));
	pageView->setPosition(Point(0,0));

	for (int i = 1; i <= Global::getInstance()->getTotalLevels(); i++)
	{
		Layout* layout = Layout::create();
		layout->setSize(Size(winSize.width, winSize.height));

		ImageView* imageView = ImageView::create(StringUtils::format("image/level/select%d.jpg", i));
		imageView->setScale9Enabled(true);
		imageView->setSize(Size(spWidth, spHeight));
		imageView->setPosition(Point(layout->getSize().width / 2.0f, layout->getSize().height / 2.0f));
		layout->addChild(imageView);

		Text* label = Text::create(StringUtils::format("Level: %d", i), "fonts/Marker Felt.ttf", 30);
		label->setColor(Color3B(192, 192, 192));
		label->setPosition(Point(layout->getSize().width / 2.0f, layout->getSize().height / 2.0f));
		layout->addChild(label);

		pageView->addPage(layout);
	}
	pageView->addEventListenerPageView(this, pagevieweventselector(SelectLevel::pageViewEvent));

	this->addChild(pageView);


	return true;
}
void SelectLevel::pageViewEvent(Ref *pSender, PageViewEventType type)
{

}
bool SelectLevel::onTouchBegan(Touch* touch, Event* event)
{
	clickBeginPoint = touch->getLocation();
	return true;
}
//
//void SelectLevel::onTouchMoved(Touch* touch, Event*)
//{
//	CCLOG("TOUCH_MOVE");
//	adjustPostion(touch->getLocation().x);
//}
//
void SelectLevel::onTouchEnded(Touch* touch, Event* event)
{
	int dragDistance = touch->getLocation().x - clickBeginPoint.x;
	//如果单击超过5像素 切换场景
	if (abs(dragDistance) < 5)
	{
		Rect layerRect = Rect((winSize.width - spWidth) / 2, (winSize.height - spHeight) / 2, spWidth, spHeight);
		if (layerRect.containsPoint(touch->getLocation()))
		{
			//要切换的关
			int level = pageView->getCurPageIndex()+1;
			Global::getInstance()->setCurrentLevel(level);
			auto scene = GameLayer::createScene();
			Director::getInstance()->replaceScene(scene);
		}
	}
}

//void SelectLevel::onTouchCancelled(Touch*, Event*)
//{
//	CCLOG("TOUCH_CANCEL");
//}
//void SelectLevel::adjustPostion(int x)
//{
//	CCLOG("%d", x - pointX);
//	levelsLayer->setPositionX(m_levelsLayerX + x - pointX);
//
//}
//
//void adjustNowLevel(int x)
//{
//
//}



//void SelectLevel::tableCellTouched(TableView* table, TableViewCell* cell)
//{
//	CCLOG("cell touched at index: %ld", cell->getIdx());
//}
//
//Size SelectLevel::tableCellSizeForIndex(TableView *table, ssize_t idx)
//{
//	return Director::getInstance()->getVisibleSize();
//}
//
//TableViewCell* SelectLevel::tableCellAtIndex(TableView *table, ssize_t idx)
//{
//	auto string = String::createWithFormat("%ld", idx+1);
//	TableViewCell *cell = table->dequeueCell();
//	if (!cell) {
//		cell = new TableViewCell();
//		cell->autorelease();
//		auto sprite = Sprite::create(StringUtils::format("image/level/select%d.jpg", idx));
//		sprite->setAnchorPoint(Point::ZERO);
//		sprite->setPosition(Point(0, 0));
//		cell->addChild(sprite);
//
//		auto label = Label::createWithSystemFont(string->getCString(), "Helvetica", 20.0);
//		label->setPosition(Point::ZERO);
//		label->setAnchorPoint(Point::ZERO);
//		label->setTag(123);
//		cell->addChild(label);
//	}
//	else
//	{
//		auto label = (Label*)cell->getChildByTag(123);
//		label->setString(string->getCString());
//	}
//
//
//	return cell;
//}
//
//ssize_t SelectLevel::numberOfCellsInTableView(TableView *table)
//{
//	return Global::getInstance()->getTotalLevels();
//}

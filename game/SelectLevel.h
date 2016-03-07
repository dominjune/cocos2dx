#ifndef _SELECTLEVEL_H_
#define _SELECTLEVEL_H_

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h" 
#include "ui/CocosGUI.h"  

USING_NS_CC;

using namespace ui;

class SelectLevel :
	public cocos2d::Layer
{
public:
	virtual bool init() override;
	CREATE_FUNC(SelectLevel);
	
	static cocos2d::Scene* createScene();
	/*******************************************************************************
	virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view) {};
	virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view) {}
	virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell);
	virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx);
	virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx);
	virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table);

	***********************************************************************************/

	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

	void pageViewEvent(Ref *pSender, PageViewEventType type);

private:
	cocos2d::Size winSize;
	int spWidth;// = 400;
	int spHeight;// = 223;
	Point clickBeginPoint;
	PageView* pageView;
//	cocos2d::extension::TableView* tableView;
};



#endif

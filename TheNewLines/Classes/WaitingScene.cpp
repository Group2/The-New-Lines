#include "WaitingScene.h"
#include "HelloWorldScene.h"
#include "AppMacros.h"
#include "CurlTest.h"

USING_NS_CC;

CCScene* WaitingScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    WaitingScene *layer = WaitingScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool WaitingScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayerColor::initWithColor(ccc4(255,255,255,255)) )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	
    CCMenuItemImage *playButton = CCMenuItemImage::create(
                                        "play.png",
                                        "play.png",
                                        this,
                                        menu_selector(WaitingScene::singlePlay));
	
    CCMenuItemImage *multiPlayButton = CCMenuItemImage::create(
                                        "multiplay.png",
                                        "multiplay.png",
                                        this,
                                        menu_selector(WaitingScene::multiPlay));
    
    exitButton = CCMenuItemImage::create(
                                        "exit.png",
                                        "exit.png",
                                        this,
                                        menu_selector(WaitingScene::exit));
	
	exitButton->setPosition(ccp(origin.x + visibleSize.width - 50,
								origin.y + 30));
    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create();
	pMenu->addChild(playButton);
	pMenu->addChild(multiPlayButton);
    pMenu->setPosition(ccp(origin.x + visibleSize.width/2,
								origin.y + visibleSize.height/2 - 30));
	pMenu->alignItemsVertically();

    this->addChild(pMenu, 1);
	this->addChild(exitButton);
	this->setTouchEnabled(true);

	notify = "";
    pLabel = CCLabelTTF::create(notify, "Arial", TITLE_FONT_SIZE);
    
    pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - 20));
	ccColor3B color = { 0,0,0};
    pLabel->setColor(color);

	this->addChild(pLabel);

    return true;
}

void WaitingScene::ccTouchesEnded(CCSet* pTouches, CCEvent* pEvent){
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	CCTouch* touch = (CCTouch*)(pTouches->anyObject());
	CCPoint location = touch->getLocationInView();
	location = CCDirector::sharedDirector()->convertToGL(location);	

	if(exitButton->boundingBox().containsPoint(location))
		CCDirector::sharedDirector()->end();
}

void WaitingScene::setNotify(char* notify){
	this->removeChild(pLabel);
	this->notify = notify;
	pLabel->setString(notify);
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - pLabel->getContentSize().height));
	this->addChild(pLabel);
}
void WaitingScene::singlePlay(CCObject* pSender)
{
	HelloWorld *playLayer = HelloWorld::create();
//	waitScene.removeAllChildrenWithCleanup(true);
	CCScene * scene = CCScene::create();
	scene->addChild(playLayer);
	CCDirector::sharedDirector()->replaceScene(scene);
}

void WaitingScene::multiPlay(CCObject* pSender)
{
	CurlTest *multiPlayLayer = CurlTest::create();
//	waitScene.removeAllChildrenWithCleanup(true);
	CCScene * scene = CCScene::create();
	scene->addChild(multiPlayLayer);
	CCDirector::sharedDirector()->replaceScene(scene);
}

void WaitingScene::exit(CCObject* pSender)
{
	CCLOG("exit game");
	
}

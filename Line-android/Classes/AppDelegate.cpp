#include "cocos2d.h"
#include "CCEGLView.h"
#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "WaitingScene.h"
#include "SimpleAudioEngine.h"
#include "CurlTest.h"

using namespace CocosDenshion;

USING_NS_CC;

static float sizeX[3] = {480, 1024, 2048};
static float sizeY[3] = {320, 768, 1536};
static int numberOfSize = 3;

static float designSizeX = 480, designSizeY = 320;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
    SimpleAudioEngine::end();
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
	CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
    pDirector->setOpenGLView(pEGLView);
	
	
    pEGLView->setDesignResolutionSize(designSizeX, designSizeY, kResolutionShowAll);
	
	
    // turn on display FPS
//    pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 30);

    // create a scene. it's an autorelease object
	
	WaitingScene *waitLayer = WaitingScene::create();
	waitLayer->setNotify("Welcome to new game of Group2");
//	waitScene.removeAllChildrenWithCleanup(true);
	CCScene * scene = CCScene::create();
	scene->addChild(waitLayer);
	pDirector->runWithScene(scene);
//	pScene.removeAllChildrenWithCleanup(true);
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();

    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();

    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}

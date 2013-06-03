#include "HelloWorldScene.h"

using namespace cocos2d;

void HelloWorld::increaseLoadingBar(){
	//mLoadingBar->setPercentage(mLoadingBar->getPercentage() + 10);
	mLoadingBar->setPercentage(countNumber*100/limitLine);
	if (mLoadingBar->getPercentage() >= 80)
		stopActionByTag(123);
}

void HelloWorld::runLoadingBar(){
	CCCallFunc *action = CCCallFunc::create(this, callfunc_selector (HelloWorld::increaseLoadingBar));
	this->runAction(action);
}
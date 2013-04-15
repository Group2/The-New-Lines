#include "HelloWorldScene.h"

using namespace cocos2d;

void HelloWorld::showKeyBack(){

	CCSize size = CCDirector::sharedDirector()->getWinSize();

	this->removeChild(keyBack,true);
	if (countKeyBack == 0) keyBack = CCSprite::create("backInvi.png");
	else keyBack = CCSprite::create("back.png");
	keyBack->setPosition(ccp(size.width-60, size.height-60));
	this->addChild(keyBack,1);

	char countKeyBackString[20];
	sprintf (countKeyBackString, "%i", countKeyBack);
	this->removeChild(countKeyBackLabel);
	countKeyBackLabel = CCLabelTTF::create(countKeyBackString,"TimeNewsRoman", 20);
	countKeyBackLabel->setPosition(ccp(size.width-50, size.height - 80));
	this->addChild(countKeyBackLabel,1);

}

void HelloWorld::showKeyMove(){

	CCSize size = CCDirector::sharedDirector()->getWinSize();

	this->removeChild(keyMove,true);
	if (countKeyMove > 0) keyMove = CCSprite::create("move.png");
	else keyMove = CCSprite::create("moveInvi.png");
	keyMove->setPosition(ccp(size.width-25, size.height-60));
	addChild(keyMove);

	char countKeyMoveString[20];
	sprintf (countKeyMoveString, "%i", countKeyMove);
	this->removeChild(countKeyMoveLabel);
	countKeyMoveLabel = CCLabelTTF::create(countKeyMoveString,"TimeNewsRoman", 20);
	countKeyMoveLabel->setPosition(ccp(size.width-10, size.height - 80));
	this->addChild(countKeyMoveLabel,1);
}

void HelloWorld::showCountNumber(){

	CCSize size = CCDirector::sharedDirector()->getWinSize();

	char countNumberString[20];
	sprintf (countNumberString, "%i", countNumber);
	this->removeChild(countNumberLabel);
	countNumberLabel = CCLabelTTF::create(countNumberString,"TimeNewsRoman", 20);
	countNumberLabel->setPosition(ccp(90, size.height - 50));
	this->addChild(countNumberLabel,1);
}

void HelloWorld::showTotalScore(){

	CCSize size = CCDirector::sharedDirector()->getWinSize();

	char totalScoreString[20];
	sprintf (totalScoreString, "%i", totalScore);
	this->removeChild(totalScoreLabel);
	totalScoreLabel = CCLabelTTF::create(totalScoreString,"TimeNewsRoman", 20);
	totalScoreLabel->setPosition(ccp(30, size.height - 125));
	this->addChild(totalScoreLabel,1);
}

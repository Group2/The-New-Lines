#include "HelloWorldScene.h"

using namespace cocos2d;

void HelloWorld::showKeyBack(){

	CCSize size = CCDirector::sharedDirector()->getWinSize();

	this->removeChild(keyBack,true);
	if (countKeyBack == 0) keyBack = CCSprite::create("backInvi.png");
	else keyBack = CCSprite::create("back.png");
	keyBack->setPosition(ccp(size.width-60, size.height-55));
	this->addChild(keyBack,1);

	char countKeyBackString[20];
	sprintf (countKeyBackString, "%i", countKeyBack);
	this->removeChild(countKeyBackLabel);
	countKeyBackLabel = CCLabelTTF::create(countKeyBackString,"TimeNewsRoman", 15);
	countKeyBackLabel->setPosition(ccp(size.width-50, size.height - 75));
	ccColor3B color = { 0,0,0};
    countKeyBackLabel->setColor(color);
	this->addChild(countKeyBackLabel,3);

}

void HelloWorld::showKeyMove(){

	CCSize size = CCDirector::sharedDirector()->getWinSize();

	this->removeChild(keyMove,true);
	if (countKeyMove > 0) keyMove = CCSprite::create("move.png");
	else keyMove = CCSprite::create("moveInvi.png");
	keyMove->setPosition(ccp(size.width-25, size.height-55));
	addChild(keyMove);

	char countKeyMoveString[20];
	sprintf (countKeyMoveString, "%i", countKeyMove);
	this->removeChild(countKeyMoveLabel);
	countKeyMoveLabel = CCLabelTTF::create(countKeyMoveString,"TimeNewsRoman", 15);
	countKeyMoveLabel->setPosition(ccp(size.width-10, size.height - 75));
	ccColor3B color = { 0,0,0};
    countKeyMoveLabel->setColor(color);
	this->addChild(countKeyMoveLabel,3);
}

void HelloWorld::showCountNumber(){

	CCSize size = CCDirector::sharedDirector()->getWinSize();

	char countNumberString[20];
	sprintf (countNumberString, "Remain: %i", limitLine - countNumber);
	this->removeChild(countNumberLabel);
	countNumberLabel = CCLabelTTF::create(countNumberString,"TimeNewsRoman", 15);
	countNumberLabel->setPosition(ccp(size.width - 120, size.height - 50));
	ccColor3B color = { 0,0,0};
    countNumberLabel->setColor(color);
	this->addChild(countNumberLabel,3);
}

void HelloWorld::showTotalScore(){

	CCSize size = CCDirector::sharedDirector()->getWinSize();

	char totalScoreString[20];
	sprintf (totalScoreString, "Score : %i", totalScore);
	this->removeChild(totalScoreLabel);
	totalScoreLabel = CCLabelTTF::create(totalScoreString,"TimeNewsRoman", 15);
	ccColor3B color = { 0,0,0};
    totalScoreLabel->setColor(color);
	totalScoreLabel->setPosition(ccp(size.width -130, size.height - 70));
	this->addChild(totalScoreLabel,3);
}

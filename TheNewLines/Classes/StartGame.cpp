#include "HelloWorldScene.h"


void HelloWorld::showStartGame(){
    CCSize size = CCDirector::sharedDirector()->getWinSize();

    startGame = CCSprite::create("startGame.jpg");
    startGame->setPosition(ccp(size.width/2, size.height/2));
    this->addChild(startGame,1);

}


#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

#include "SimpleAudioEngine.h"

using namespace cocos2d;

struct coordinate{
	int x, y, color;
};

class HelloWorld : public cocos2d::CCLayerColor
{

	//char* color[5];

	CCSprite* tiles[7][7], * tilesHint[2][3];
	
	int a[7][7], aBack[7][7];

	coordinate tilesHintPos[6];

	CCLabelTTF * CountScoreLabel;

	bool haveWay;

	bool firstTouch, gotScore;

	int x1, y1, x2, y2;

	int totalScore;

	CCSprite* keyBack, * keyMove;
	bool keyBackTouch, keyMoveTouch;

public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);

    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);

	void HelloWorld::ccTouchesEnded(CCSet* pTouches, CCEvent* pEvent);
	void HelloWorld::bfs(int startx, int starty, int finishx, int finishy);
	void HelloWorld::generateLines(int numberOfPoint);
	void HelloWorld::generatePrediction(int numberOfPoint);
	void HelloWorld::update(float pDt);
	void HelloWorld::checkResult();
	void HelloWorld::setKeyBack();
	void HelloWorld::setKeyMove();
	void HelloWorld::initTilesHint();

};

#endif  // __HELLOWORLD_SCENE_H__
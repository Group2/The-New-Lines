#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

#include "SimpleAudioEngine.h"

using namespace cocos2d;

class HelloWorld : public cocos2d::CCLayerColor
{

	//char* color[5];

	CCSprite* tiles[7][7];
	
	int a[7][7], aBack[7][7];

	bool haveWay;

	bool firstTouch, gotScore;

	int x1, y1, x2, y2;

	int totalScore;

	CCSprite* keyBack;
	bool keyBackTouch;

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
	void HelloWorld::update(float pDt);
	void HelloWorld::checkResult();
	void HelloWorld::setKeyBack();
	void HelloWorld::setKeyMove();

};

#endif  // __HELLOWORLD_SCENE_H__
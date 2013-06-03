#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "WaitingScene.h"
#include "SimpleAudioEngine.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
//#include "pthread.h"
#include <thread>
#include <mutex>

using namespace cocos2d;

using namespace  std;


struct coordinate{
	int x, y, color;
};

struct eat{
	int col, row, dial, diar;
};

class HelloWorld : public cocos2d::CCLayerColor
{

	//char* color[5];
	CCSprite* tiles[7][7], * tilesHint[2][3];

	coordinate tilesHintPos[6];

	CCLabelTTF * totalScoreLabel, *countNumberLabel, *countKeyBackLabel, *countKeyMoveLabel;

	bool haveWay, usedOneTime;

	bool firstTouch, gotScore;

	int x1, y1, x2, y2;

	int oldLevel, level, oldCountNumber, countNumber, limitLine;

	CCSprite* keyBack, * keyMove, * mLoadingBarBG;
	int animationControl;
    float duration;
	bool keyBackTouch, keyMoveTouch;

	CCProgressTimer* mLoadingBar;

public:
	int a[7][7], aBack[7][7];
	int totalScore, oldTotalScore, friendScore;
	int countKeyBack, countKeyMove;
	std::mutex *mutexMapView;
	std::mutex *mutexMapPlay;
//	pthread_mutex_t* mutexMapView;
//	pthread_mutex_t* mutexMapPlay;
	bool multiPlayer;
	bool bonusPoint;
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);

    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);

	void ccTouchesEnded(CCSet* pTouches, CCEvent* pEvent);
	void touchesAction(CCSet* pTouches, CCEvent* pEvent);
	void bfs(int startx, int starty, int finishx, int finishy);
	bool generateLines(int numberOfPoint);
	void generatePrediction(int numberOfPoint);
	//void HelloWorld::update(float pDt);
	void checkResult();

	void showKeyBack();
	void setKeyBack();
	void showKeyMove();
	void setKeyMove();

	void showCountNumber();
	void showTotalScore();

	void initTilesHint();
	
	void runLoadingBar();
	void increaseLoadingBar();
	
	void addBox(int i, int j, char name[]);
	void removeBox(int i, int j);
	void finishAnimation(CCNode * pNode);
	bool checkEndGame();
	void endGame();
};

#endif  // __HELLOWORLD_SCENE_H__
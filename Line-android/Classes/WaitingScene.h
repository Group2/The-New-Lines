#ifndef __WAITING_SCENE_H__
#define __WAITING_SCENE_H__

#include "cocos2d.h"
using namespace cocos2d;

class WaitingScene : public cocos2d::CCLayerColor
{
public:
	char* notify;
	CCLabelTTF* pLabel;
	CCMenuItemImage *exitButton;
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
	void ccTouchesEnded(CCSet* pTouches, CCEvent* pEvent);
	void setNotify(char* notify);
    // a selector callback
    void singlePlay(CCObject* pSender);
    void multiPlay(CCObject* pSender);
	void exit(CCObject* pSender);
    // implement the "static node()" method manually
    CREATE_FUNC(WaitingScene);
};

#endif // __HELLOWORLD_SCENE_H__

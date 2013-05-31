#ifndef _CURL_TEST_H_
#define _CURL_TEST_H_

#include "cocos2d.h"
using namespace cocos2d;

int curlWriter
  (char *data, size_t size, size_t nmemb, std::string *outputBuffer);
  
class CurlTest : public cocos2d::CCLayerColor
{
private:
	CCTextFieldTTF * pTextField;
public:
	float x, y;
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
	void initStartGame();
	virtual void draw (void);
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    void ccTouchesEnded(CCSet* pTouches, CCEvent* pEvent);
	
	void send(CCObject* pSender);
	void recv(CCObject* pSender);
    // implement the "static node()" method manually
    CREATE_FUNC(CurlTest);
};

#endif // __HELLOWORLD_SCENE_H__

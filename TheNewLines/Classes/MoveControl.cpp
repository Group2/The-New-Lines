#include "HelloWorldScene.h"

using namespace cocos2d;

void HelloWorld::setKeyMove(){

	if (!keyMoveTouch) bfs(x1, y1, x2, y2);
	else{
		haveWay = true;
		keyMoveTouch = false;
		countKeyMove--;

		showKeyMove();
	}
}
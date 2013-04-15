#include "HelloWorldScene.h"

using namespace cocos2d;

void HelloWorld::setKeyBack(){

	char* color[6] = {"white","blue","yellow","red","green","pink"};


	for (int i = 0; i < 7; i++)
		for (int j = 0; j < 7; j++)
			if (a[i][j] != aBack[i][j]){

				a[i][j] = aBack[i][j];

				char name[20];
				sprintf (name, "%s.png", color[a[i][j]]);

				this->removeChild(tiles[i][j]);
				tiles[i][j] = CCSprite::create(name);
				tiles[i][j]->setPosition(ccp(44*i + 1.5*(i+1) + 22, 44*j + 1.5*(j+1) + 22));
				addChild(tiles[i][j]);

			}

			countKeyBack--;
			usedOneTime = true;

			totalScore = oldTotalScore;
			if (level != oldLevel){
				level = oldLevel;
				limitLine -= 2;
			}
			countNumber = oldCountNumber;

			CCCallFunc *action = CCCallFunc::create(this, callfunc_selector (HelloWorld::increaseLoadingBar));
			this->runAction(action);

			showKeyBack();

			showCountNumber();

			showTotalScore();
}

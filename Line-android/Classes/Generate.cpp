#include "HelloWorldScene.h"

using namespace cocos2d;


bool HelloWorld::checkEndGame(){
	for(int i=0; i < 7; i++)
		for(int j=0; j < 7; j++)
			if(a[i][j] == 0)
				return false;
	return true;
}

bool HelloWorld::generateLines(int numberOfPoint){

	char* color[6] = {"white","blue","yellow","red","green","pink"};

	int numberOfColor = 5;
	int i, j;

	char name[20];

	for (int k = 0; k < numberOfPoint; k++){
		if(checkEndGame())
			return false;
		do{
			i = rand() % 7;
			j = rand() % 7;
		} while (a[i][j] != 0);
		
		int v = k % (this->level+2);
		sprintf (name, "%s.png", color[tilesHintPos[v].color]);
		
		this->addBox(i, j, name);

		a[i][j] = tilesHintPos[v].color;
	}
	return (!checkEndGame());
}

void HelloWorld::generatePrediction(int numberOfPoint){

	char* color[6] = {"white","blue","yellow","red","green","pink"};

	int numberOfColor = 5;
	int i = 0, j = 0;

	char name[20];

	for (int k = 0; k < numberOfPoint; k++){

		int actual = rand() % 5 + 1;
		sprintf (name, "%sHint.png", color[actual]);

		this->removeChild(tilesHint[i][j]);
		tilesHint[i][j] = CCSprite::create(name);
		tilesHint[i][j]->setPosition(ccp(tilesHintPos[k].x, tilesHintPos[k].y));
		addChild(tilesHint[i][j]);
		tilesHintPos[k].color = actual;

		j++;
		if (j > 2){
			i++; j = 0;
		}
	}

}
